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
    class xvirtual_memory;

    class key_indexer
    {
    public:
        virtual s32  max_levels() const                    = 0;
        virtual bool keys_equal(u64 lhs, u64 rhs) const    = 0;
        virtual s32  keys_compare(u64 lhs, u64 rhs) const  = 0;
        virtual s32  get_index(u64 value, s32 level) const = 0;
    };

    struct key_indexer_data
    {
        u64 m_mask;
        s16 m_vars[2];
        s32 m_levels;
    };

    // Move this to xallocator
    // gCreateVMemBasedAllocator(a, vmem, sizeof(myleaf) (32?), 1*xGB, 64*xKB);
    // Maximum number of myleaf allocations = (2048 - 1) * 16384 = 33.538.048 = ~32 Million nodes
    xfsallocdexed* gCreateVMemBasedDexAllocator(xalloc* a, xvirtual_memory* vmem, u32 alloc_size, u64 addr_range, u32 page_size);

    //
    // A btree_t is a BST that is unbalanced and where branches grow/shrink when adding
    // or removing items. This particular implementation uses indices instead of pointers
    // and thus uses the same amount of memory on a 32-bit system compared to a 64-bit
    // system. It is possible to make an implementation where the indices are 16-bit when
    // the number of leafs are less then 16384 (we already use the highest bit to identify
    // between node and leaf plus a tree uses (N*2 - 1) nodes + leafs).
    //
    // We use an `indexer` here which is responsible for computing the index at every level
    // of the tree for a given 'key'. If the keydexer goes through the key from msb to lsb
    // then the keys are sorted in the btree, otherwise they are not sorted in the normal sense
    // but according to the output of the keydexer.
    // The keydexer should be initialized in such a way so that the high frequency bits are
    // taken first iterating over the bits from high-frequency to low-frequency.
    // In the case of the key being an index from 0-65536 and if you do not care about
    // sorting you should initialize the keydexer like this:
    // shift: 0, 2, 4, 6, 8, 10, 12, 14
    // mask : 0x3
    // The 'value' stored should only use bit 0-30, bit 31 is reserved.
    //
    // Except the root every other node in this tree has 2 or more children.
    //
    // Since this data-structure is 'sorted' we also provide a lower- and upper-bound find
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

        void init(xfsallocdexed* node_allocator, keyvalue* kv);
        void init(xfsallocdexed* node_allocator, keyvalue* kv, key_indexer const* indexer);
	    void init_from_index(xfsallocdexed* node_allocator, keyvalue* kv, u32 max_index);
	    void init_from_mask(xfsallocdexed* node_allocator, keyvalue* kv, u64 mask, bool sorted);

        bool add(u64 key, u32 value);
        bool rem(u64 key, u32& value);

        bool find(u64 key, u32& value) const;
        bool lower_bound(u64 key, u32& value) const;
        bool upper_bound(u64 key, u32& value) const;

    private:
        struct node_t;
        struct history_t;
        u32                m_root;
        key_indexer_data   m_idxr_data;
        key_indexer const* m_idxr;
        xfsallocdexed*        m_node_alloc;
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

        void init(key_indexer const* idxr, xfsalloc* node_allocator, keyvalue* kv);

        bool add(u64 key, void* value);
        bool rem(u64 key, void*& value);

        bool find(u64 key, void*& value) const;
        bool lower_bound(u64 key, void*& value) const;
        bool upper_bound(u64 key, void*& value) const;

    private:
        struct node_t;
        struct history_t;
        key_indexer const* m_idxr;
        key_indexer_data   m_idxr_data;
        node_t*            m_root;
        xfsalloc*          m_node_alloc;
        keyvalue*          m_kv;
    };

    // Now that we have a btree we can implement other well known data structures.
    // map<K,V>
    // set<V>

    template <typename T> class xmap
    {
    public:
        xmap(xalloc* a)
            : m_allocator(a)
        {
        }
        bool insert(u64 key, T const& value);
        bool remove(u64 key, T& value);
        bool find(u64 key, T& value) const;

    private:
        struct value_t
        {
            inline value_t(u64 key, const T& value)
                : m_key(key)
                , m_value(value)
            {
            }
            u64 m_key;
            T   m_value;
        };
        xalloc*          m_allocator;
        xbtree          m_tree;
    };

    template <typename T> class xhashset
    {
    public:
        typedef u64 (*hashfunc)(T const& value);

        xhashset();
        xhashset(hashfunc hash);

        bool insert(T const& value);
        bool remove(T const& value);
        bool contains(T const& value) const;

    private:
        struct value_t
        {
            u64 m_key;
            T   m_value;
        };
        hashfunc m_hash;
        xalloc*  m_allocator;
        xbtree  m_tree;
    };
};

#endif // __X_ALLOCATOR_BTREE_H__