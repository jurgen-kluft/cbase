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
        u64 m_mask;
        s16 m_vars[2];
        s32 m_levels;
    };

    // A xbtree32 is a BST that is unbalanced and where branches grow/shrink when adding
    // or removing items. This particular implementation uses indices instead of pointers
    // and thus uses the same amount of memory on a 32-bit system compared to a 64-bit
    // system. It is possible to make an implementation where the indices are 16-bit when
    // the number of items are less then 16384 (we already use the highest bit to identify
    // between node and leaf plus a tree uses (N*2 - 1) nodes + leafs).
    // But the node allocator need to be able to give back indices that are < 32768.
    //
    // We use an `indexer` here which is responsible for computing the index at every level
    // of the tree for a given 'key'. If the keydexer goes through the key from msb to lsb
    // then the keys are sorted in the btree, otherwise they are not sorted in the normal sense
    // but according to the output of the keydexer.
    //
    // The key_indexer should be initialized in such a way so that the high frequency bits are
    // taken first iterating over the bits from high-frequency to low-frequency.
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
        class keyvalue
        {
        public:
            virtual u64  get_key(u32 value) const    = 0;
            virtual void set_key(u32 value, u64 key) = 0;
        };

        struct node_t
        {
            inline node_t() {}
            inline bool is_empty() const { return m_nodes[0] == 0xffffffff && m_nodes[1] == 0xffffffff && m_nodes[2] == 0xffffffff && m_nodes[3] == 0xffffffff; }
            inline void clear() { m_nodes[0] = m_nodes[1] = m_nodes[2] = m_nodes[3] = 0xffffffff; }
            u32 m_nodes[4];
            XCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        void init(xfsadexed* node_allocator, keyvalue* kv);
        void init(xfsadexed* node_allocator, keyvalue* kv, key_indexer const* indexer);
        void init_from_index(xfsadexed* node_allocator, keyvalue* kv, u32 max_index);
        void init_from_mask(xfsadexed* node_allocator, keyvalue* kv, u64 mask, bool sorted);

        bool add(u64 key, u32 value);
        bool rem(u64 key, u32& value);

        bool find(u64 key, u32& value) const;
        bool lower_bound(u64 key, u32& value) const;
        bool upper_bound(u64 key, u32& value) const;

    private:
        struct history_t;
        u32                m_root;
        key_indexer_data   m_idxr_data;
        key_indexer const* m_idxr;
        xfsadexed*         m_node_alloc;
        keyvalue*          m_kv;
    };

    struct xbtree
    {
        class keyvalue
        {
        public:
            virtual u64  get_key(void* value) const          = 0;
            virtual void set_key(void* value, u64 key) const = 0;
        };

        struct node_t
        {
            inline node_t() {}
            inline bool is_empty() const { return m_nodes[0] == 0 && m_nodes[1] == 0 && m_nodes[2] == 0 && m_nodes[3] == 0; }
            inline void clear() { m_nodes[0] = m_nodes[1] = m_nodes[2] = m_nodes[3] = 0; }
            uptr m_nodes[4];
            XCORE_CLASS_PLACEMENT_NEW_DELETE
        };        

        void init(xfsa* node_allocator, keyvalue* kv);
        void init(xfsa* node_allocator, keyvalue* kv, key_indexer const* idxr);
        void init_from_index(xfsa* node_allocator, keyvalue* kv, u32 max_index);
        void init_from_mask(xfsa* node_allocator, keyvalue* kv, u64 mask, bool sorted);

        bool add(u64 key, void* value);
        bool rem(u64 key, void*& value);
        void clear();

        bool find(u64 key, void*& value) const;
        bool lower_bound(u64 key, void*& value) const;
        bool upper_bound(u64 key, void*& value) const;

        struct node_t;

    private:
        struct history_t;
        key_indexer const* m_idxr;
        key_indexer_data   m_idxr_data;
        node_t*            m_root;
        xfsa*              m_node_alloc;
        keyvalue*          m_kv;
    };


}; // namespace xcore

#endif // __X_ALLOCATOR_BTREE_H__