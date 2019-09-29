#ifndef __X_ALLOCATOR_BTREE_H__
#define __X_ALLOCATOR_BTREE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_allocator.h"
#include "xbase/x_integer.h"

namespace xcore
{
    class key_indexer
    {
    public:
        virtual s32  max_levels() const                  = 0;
        virtual s32  key_compare(u64 lhs, u64 rhs) const = 0;
        virtual s32  key_to_index(s32 level, u64 value) const = 0;
    };

    struct key_indexer_data
    {
        s8 m_vars[2];
        s16 m_levels;
    };

    // The xbtree does not store the key, instead it relies on key retrieval through
    // a proxy class. This allows the user to provide the key for a given value.
    class xbtree32_kv
    {
    public:
        virtual u64  get_key(u32 value) const    = 0;
        virtual void set_key(u32 value, u64 key) = 0;
    };
    class xbtree_kv
    {
    public:
        virtual u64  get_key(void* value) const    = 0;
        virtual void set_key(void* value, u64 key) = 0;
    };

    // A xbtree is a BST that is unbalanced and where branches grow/shrink when adding
    // or removing items. The xbtree32 implementation uses indices instead of pointers
    // and thus uses the same amount of memory on a 32-bit system as compared to a 64-bit
    // system. For every key-value inserted you can consider ~6/8 bytes for xbtree32. For
    // xbtree it is an average of ~12/16 bytes per item.
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
    struct xbtree32
    {
        void init(xfsadexed* node_allocator, xbtree32_kv* kv);
        void init(xfsadexed* node_allocator, xbtree32_kv* kv, key_indexer const* indexer);
        void init_from_index(xfsadexed* node_allocator, xbtree32_kv* kv, u32 max_index, bool sorted);
        void init_from_mask(xfsadexed* node_allocator, xbtree32_kv* kv, u64 mask, bool sorted);

        bool add(u64 key, u32 value);
        bool rem(u64 key, u32& value);

        bool find(u64 key, u32& value) const;
        bool lower_bound(u64 key, u32& value) const;
        bool upper_bound(u64 key, u32& value) const;

        static inline s32 sizeof_node() { return 4 * sizeof(u32); }

    private:
        struct node_t;
        struct history_t;
        u32                m_root;
        key_indexer_data   m_idxr_data;
        key_indexer const* m_idxr;
        xfsadexed*         m_node_alloc;
        xbtree32_kv*       m_kv;
    };

    struct xbtree
    {
        struct node_t;

		void init(xfsa* node_allocator, xbtree_kv* kv);
        void init(xfsa* node_allocator, xbtree_kv* kv, key_indexer const* idxr);
        void init_from_index(xfsa* node_allocator, xbtree_kv* kv, u32 max_index, bool sorted);
        void init_from_mask(xfsa* node_allocator, xbtree_kv* kv, u64 mask, bool sorted);

        bool add(u64 key, void* value);
        bool rem(u64 key, void*& value);
        void clear();

        bool find(u64 key, void*& value) const;
        bool lower_bound(u64 key, void*& value) const;
        bool upper_bound(u64 key, void*& value) const;

        static inline s32 sizeof_node() { return 4 * sizeof(void*); }

	private:
        struct history_t;
        key_indexer const* m_idxr;
        key_indexer_data   m_idxr_data;
        node_t*            m_root;
        xfsa*              m_node_alloc;
        xbtree_kv*         m_kv;
    };


}; // namespace xcore

#endif // __X_ALLOCATOR_BTREE_H__