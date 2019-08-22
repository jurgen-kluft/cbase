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
    xfsallocdexed* gCreateVMemBasedDexAllocator(xalloc* a, xvirtual_memory* vmem, u32 alloc_size, u64 addr_range,
                                                u32 page_size);

    //
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
        xfsallocdexed*     m_node_alloc;
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

        void init(xfsalloc* node_allocator, keyvalue* kv);
        void init(xfsalloc* node_allocator, keyvalue* kv, key_indexer const* idxr);
        void init_from_index(xfsalloc* node_allocator, keyvalue* kv, u32 max_index);
        void init_from_mask(xfsalloc* node_allocator, keyvalue* kv, u64 mask, bool sorted);

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
        xfsalloc*          m_node_alloc;
        keyvalue*          m_kv;
    };

    // Dynamic sized nodes, using bitset to identify the slots that are set.
    // Size of node is from 16B to 264B so this works great with an allocator
    // that has a good Fixed Size Allocator setup.

    // Every node consumes 5 bits, so a key of 32 bits and a root of 7 bits will
    // have a maximum depth of 5 (5 * 5-bits = 25 bits + 7 bits root = 32 bits).
    // Note: A root of 2 bits and a depth of 6 is also possible.

    struct xztree
    {
        struct znode
        {
            u32    m_nodemap;
            u32    m_valuemap;
            znode* m_branches[]; // Node*[->] - Value*[<-]
        };

        static s32    size(znode* node);
        static s32    max_level() { return 6; }
        static s8     calc_index(s32 level, u32 key);

        // Insert: Keep calling insert as long as it returns a valid znode*
        static znode* insert(znode*& node, s8 i, void* value, xalloc* allocator);
        // Remove: Keep calling remove as long as it returns a valid znode*
        static znode* remove(znode*& node, s8 i, void* value, xalloc* allocator);
        // Insert: Keep calling find as long as it returns a valid znode*
        static znode* find(znode*& node, s8 i, void*& value);
    };

    // Now that we have a btree we can implement other well known data structures.
    // set<V>
    // map<K,V>
    // vector<V>
    template <typename K> class xhasher
    {
    public:
        u32 hash(K const& k) const { return 0; }
    };
    template <> class xhasher<s32>
    {
    public:
        u32 hash(s32 const& k) const { return (u32)k; }
    };
    template <> class xhasher<u32>
    {
    public:
        u32 hash(u32 const& k) const { return k; }
    };
    template <> class xhasher<s64>
    {
    public:
        u32 hash(s64 const& k) const
        {
            u64 key = (u64)k;
            key     = (~key) + (key << 18); // key = (key << 18) - key - 1;
            key     = key ^ (key >> 31);
            key     = key * 21; // key = (key + (key << 2)) + (key << 4);
            key     = key ^ (key >> 11);
            key     = key + (key << 6);
            key     = key ^ (key >> 22);
            return (u32)key;
        }
    };
    template <> class xhasher<u64>
    {
    public:
        u32 hash(u64 const& k) const
        {
            u64 key = k;
            key     = (~key) + (key << 18); // key = (key << 18) - key - 1;
            key     = key ^ (key >> 31);
            key     = key * 21; // key = (key + (key << 2)) + (key << 4);
            key     = key ^ (key >> 11);
            key     = key + (key << 6);
            key     = key ^ (key >> 22);
            return (u32)key;
        }
    };

    template <typename K, typename V> class xmap
    {
    public:
        xmap(xalloc* a) : m_allocator(a) {}
        xmap(xalloc* a, xhasher<K> hasher) : m_hash(hasher), m_allocator(a) {}

        bool insert(K const& key, V const& value);
        bool remove(K const& key, V& value);
        bool find(K const& key, V& value) const;

    private:
        struct value_t
        {
            inline value_t(const K& key, const V& value) : m_hash(0), m_key(key), m_value(value) {}
            u64 m_hash;
            K   m_key;
            V   m_value;
        };
        xhasher<K> m_hash;
        xalloc*    m_allocator;
        xztree     m_tree;
    };

    template <typename T> class xset
    {
    public:
        xset();
        xset(xhasher<T> hasher);

        bool insert(T const& value);
        bool remove(T const& value);
        bool contains(T const& value) const;

    private:
        struct value_t
        {
            inline value_t(u32 key, const T& value) : m_hash(key), m_value(value) {}
            u32 m_key;
            T   m_value;
        };
        xhasher<T> m_hash;
        xalloc*    m_allocator;
        xztree     m_tree;
    };
}; // namespace xcore

#endif // __X_ALLOCATOR_BTREE_H__