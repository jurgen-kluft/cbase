#ifndef __C_ALLOCATOR_BTREE_H__
#define __C_ALLOCATOR_BTREE_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_allocator.h"
#include "cbase/c_integer.h"

namespace ncore
{
    struct btree_indexer_t
    {
        s32 max_levels() const { return m_levels; }
        s32 key_to_index(s32 level, u64 value) const;

        s8  m_vars[2];
        s16 m_levels;
    };

    // The btree does not store the key, instead it relies on key retrieval through
    // a proxy class. This allows the user to provide the key for a given value.
    class btree_idx_kv_t
    {
    public:
        virtual u64  get_key(u32 value) const    = 0;
        virtual void set_key(u32 value, u64 key) = 0;
    };
    class btree_ptr_kv_t
    {
    public:
        virtual u64  get_key(void* value) const    = 0;
        virtual void set_key(void* value, u64 key) = 0;
    };

    //
    // A btree is a low-level BST that is unbalanced and where branches grow/shrink when
    // adding or removing items. The btree_idx_t implementation uses indices instead of pointers
    // and thus uses the same amount of memory on a 32-bit system as compared to a 64-bit
    // system. For every key-value inserted you can consider ~6/8 bytes for btree_idx_t. For
    // btree_ptr_t it is an average of ~12/16 bytes per item.
    //
    // We use an `indexer` which is responsible for computing the index at every level
    // of the tree for a given 'key'. If the keydexer goes through the key from msb to lsb
    // then the keys are sorted in the btree, otherwise they are not sorted in the normal sense
    // but according to the output of the keydexer.
    //
    // The key_indexer should be initialized in such a way so that the high frequency bits are
    // taken first, so iterating over the bits of the key from high-frequency to low-frequency.
    //
    // In the case of the key being an index from 0-65536 and if you do not care about
    // sorting you should initialize the tree with init_from_index( _, _, max index).
    //
    // The 'value' stored should only use bit 0-30, bit 31 is reserved.
    //
    // Adding two keys that are nearly identical results in the maximum branch length to be
    // created, this is the worst case behaviour.
    //
    // Since this data-structure can be 'sorted' we also provide a lower- and upper-bound find
    // function.
    //
    struct btree_idx_t
    {
        void init(fsadexed_t* node_allocator, btree_idx_kv_t* kv);
        void init_from_index(fsadexed_t* node_allocator, btree_idx_kv_t* kv, u32 max_index, bool sorted);
        void init_from_mask(fsadexed_t* node_allocator, btree_idx_kv_t* kv, u64 mask, bool sorted);

        bool add(u64 key, u32 value);
        bool rem(u64 key, u32& value);

        bool find(u64 key, u32& value) const;
        bool lower_bound(u64 key, u32& value) const;
        bool upper_bound(u64 key, u32& value) const;

        static inline s32 sizeof_node() { return 4 * sizeof(u32); }

    private:
        struct node_t;
        struct history_t;
        u32             m_root;
        btree_indexer_t m_idxr;
        fsadexed_t*     m_node_alloc;
        btree_idx_kv_t* m_kv;
    };

    struct btree_ptr_t
    {
        struct node_t;

        void init(fsa_t* node_allocator, btree_ptr_kv_t* kv);
        void init_from_index(fsa_t* node_allocator, btree_ptr_kv_t* kv, u32 max_index, bool sorted);
        void init_from_mask(fsa_t* node_allocator, btree_ptr_kv_t* kv, u64 mask, bool sorted);

        bool add(node_t*& root, u64 key, void* value);
        bool rem(node_t*& root, u64 key, void*& value);
        void clear(node_t*& root);

        bool find(node_t* root, u64 key, void*& value) const;
        bool lower_bound(node_t* root, u64 key, void*& value) const;
        bool upper_bound(node_t* root, u64 key, void*& value) const;

        static inline s32 sizeof_node() { return 4 * sizeof(void*); }

    private:
        struct history_t;
        btree_indexer_t m_idxr;
        fsa_t*          m_node_alloc;
        btree_ptr_kv_t* m_kv;
    };

}; // namespace ncore

#endif // __C_ALLOCATOR_BTREE_H__