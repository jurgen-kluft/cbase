#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#include "xbase/x_btree.h"

namespace xcore
{
    class xbtree32_keyvalue_ptr : public xbtree32::keyvalue
    {
    public:
        inline xbtree32_keyvalue_ptr(xdexer* dx)
            : m_dexer(dx)
        {
        }
        virtual u64 get_key(u32 value) const
        {
            void* obj = m_dexer->idx2ptr(value);
            return (u64)obj;
        }
        virtual void set_key(u32 value, u64 key) const {}

    private:
        xdexer* m_dexer;
    };

    class keydexer : public key_indexer
    {
        key_indexer_data const* m_data;

    public:
        keydexer(key_indexer_data const* data)
            : m_data(data)
        {
        }

        virtual s32 max_levels() const { return m_data->m_levels; }

        // The indexer knows which bits are being 'indexed' so it can
        // tell us if two values are equal from its perspective.
        virtual bool keys_equal(u64 lhs, u64 rhs) const { return (lhs & m_data->m_mask) == (rhs & m_data->m_mask); }

        virtual s32 keys_compare(u64 lhs, u64 rhs) const
        {
            lhs = lhs & m_data->m_mask;
            rhs = rhs & m_data->m_mask;
            if (lhs == rhs)
                return 0;
            return (lhs < rhs) ? -1 : 1;
        }

        virtual s32 get_index(u64 value, s32 level) const
        {
            s8 const  msk = 0x3;
            s32 const shr = (level * m_data->m_vars[0]) + m_data->m_vars[1];
            return (s8)(value >> shr) & msk;
        }
    };

    void initialize_from_index(key_indexer_data& keydexer, u32 max_index)
    {
        u64 mask = max_index;
        mask     = mask | (mask >> 1);
        mask     = mask | (mask >> 2);
        mask     = mask | (mask >> 4);
        mask     = mask | (mask >> 8);
        mask     = mask | (mask >> 16);
        mask     = mask | (mask >> 32);

        u32 trailbitcnt = xcountTrailingZeros(mask);
        u32 maskbitcnt  = xcountBits(mask);
        if ((maskbitcnt & 1) == 1)
        {
            // Which way should we extend the mask to make the count 'even'
            if ((mask & (u64(1) << 63)) == 0)
            {
                mask = mask | (mask << 1);
            }
            else
            {
                mask = mask | (mask >> 1);
            }
            maskbitcnt += 1;
        }
        keydexer.m_mask    = mask;
        keydexer.m_levels  = maskbitcnt / 2;
        keydexer.m_vars[0] = 2;
        keydexer.m_vars[1] = 0;
    }

    void initialize_from_mask(key_indexer_data& keydexer, u64 mask, bool sorted)
    {
        u64 lrange = mask;
        lrange     = lrange | (lrange >> 1);
        lrange     = lrange | (lrange >> 2);
        lrange     = lrange | (lrange >> 4);
        lrange     = lrange | (lrange >> 8);
        lrange     = lrange | (lrange >> 16);
        lrange     = lrange | (lrange >> 32);

        u64 hrange = mask;
        hrange     = hrange | (hrange << 1);
        hrange     = hrange | (hrange << 2);
        hrange     = hrange | (hrange << 4);
        hrange     = hrange | (hrange << 8);
        hrange     = hrange | (hrange << 16);
        hrange     = hrange | (hrange << 32);

        mask = hrange & lrange;

        u32 maskbitcnt = xcountBits(mask);
        if ((maskbitcnt & 1) == 1)
        {
            // Which way should we extend the mask to make the count 'even'
            if ((mask & (u64(1) << 63)) == 0)
            {
                mask = mask | (mask << 1);
            }
            else
            {
                mask = mask | (mask >> 1);
            }
            maskbitcnt += 1;
        }
        keydexer.m_mask   = mask;
        keydexer.m_levels = maskbitcnt / 2;
        if (sorted)
        {
            keydexer.m_vars[0] = -2;
            keydexer.m_vars[1] = (keydexer.m_levels - 1) * 2;
        }
        else
        {
            keydexer.m_vars[0] = 2;
            keydexer.m_vars[1] = 0;
        }
    }

    enum
    {
        Null      = 0xffffffff,
        Type_Mask = 0x80000000,
        Type_Leaf = 0x00000000,
        Type_Node = 0x80000000
    };

#if 0
    class vmalloc : public xdexedfxsa
    {
        enum
        {
            Null32 = 0xffffffff,
            Null16 = 0xffff,
        };

        // Note: Keep bit 31-30 free! So only use bit 0-29 (30 bits)
        inline u32 page_index(u32 index) const { return (index >> 13) & 0x1FFFF; }
        inline u32 item_index(u32 index) const { return (index >> 0) & 0x1FFF; }
        inline u32 to_index(u32 page_index, u32 item_index) const { return ((page_index & 0x1FFFF) << 13) | (item_index & 0x1FFF); }

    public:
        virtual void* allocate()
        {
            u32 page_index;
            if (!m_notfull_used_pages.find(page_index))
            {
                if (!alloc_page(page_index))
                {
                    return nullptr;
                }
                m_notfull_used_pages.set(page_index);
            }

            page_t* page = get_page(page_index);

            u32   i;
            void* ptr = page->allocate(i);
            if (page->m_alloc_cnt == m_page_max_alloc_count)
            { // Page is full
                m_notfull_used_pages.clr(page_index);
            }
            return ptr;
        }

        virtual void deallocate(void* p)
        {
            u32     page_index = (u32)(((uptr)p - (uptr)m_base_addr) / m_page_size);
            page_t* page       = get_page(page_index);
            u32     item_index = page->ptr2idx(p);
            page->deallocate(item_index);
            if (page->m_alloc_cnt == 0)
            {
                dealloc_page(page_index);
            }
        }

        virtual void* idx2ptr(u32 i) const
        {
            page_t* page = get_page(page_index(i));
            return page->idx2ptr(item_index(i));
        }

        virtual u32 ptr2idx(void* ptr) const
        {
            u32     page_index = (u32)(((uptr)ptr - (uptr)m_base_addr) / m_page_size);
            page_t* page       = get_page(page_index);
            u32     item_index = page->ptr2idx(ptr);
            return to_index(page_index, item_index);
        }

        // Every used page has it's first 'alloc size' used for book-keeping
        // sizeof == 8 bytes
        struct page_t
        {
            void init(u32 alloc_size)
            {
                m_list_head  = Null16;
                m_alloc_cnt  = 0;
                m_alloc_size = alloc_size;
                m_alloc_ptr  = alloc_size * 1;
                // We do not initialize a free-list, instead we allocate from
                // a 'pointer' which grows until we reach 'max alloc count'.
                // After that allocation happens from m_list_head.
            }

            void* allocate(u32& item_index)
            {
                if (m_list_head != Null16)
                { // Take it from the list
                    item_index  = m_list_head;
                    m_list_head = ((u16*)this)[m_list_head >> 1];
                }
                else
                {
                    item_index = m_alloc_ptr / m_alloc_size;
                    m_alloc_ptr += m_alloc_size;
                }

                m_alloc_cnt += 1;
                return (void*)((uptr)this + (item_index * m_alloc_size));
            }

            void deallocate(u32 item_index)
            {
                ((u16*)this)[item_index >> 1] = m_list_head;
                m_list_head                   = item_index;
                m_alloc_cnt -= 1;
            }

            void* idx2ptr(u32 index) const
            {
                void* ptr = (void*)((uptr)this + (index * m_alloc_size));
                return ptr;
            }

            u32 ptr2idx(void* ptr) const
            {
                u32 item_index = (u32)(((uptr)ptr - (uptr)this) / m_alloc_size);
                return item_index;
            }

            u16 m_list_head;
            u16 m_alloc_cnt;
            u16 m_alloc_size;
            u16 m_alloc_ptr;
        };

        page_t* get_page(u32 page_index) const
        {
            page_t* page = (page_t*)((uptr)m_base_addr + page_index * m_page_size);
            return page;
        }

        bool alloc_page(u32& page_index)
        {
            u32 freepage_index;
            return m_notused_free_pages.find(freepage_index);
        }

        void dealloc_page(u32 page_index)
        {
            page_t* page = get_page(page_index);
            m_vmem->decommit((void*)page, m_page_size, 1);
            m_notused_free_pages.set(page_index);
        }

        void init(xalloc* a, xvirtual_memory* vmem, u32 alloc_size, u64 addr_range, u32 page_size)
        {
            m_alloc                = a;
            m_vmem                 = vmem;
            m_page_size            = page_size;
            m_page_max_count       = (u32)(addr_range / page_size);
            m_page_max_alloc_count = (page_size / alloc_size) - 1;
            xheap heap(a);
            m_notfull_used_pages.init(heap, m_page_max_count, false, true);
            m_notused_free_pages.init(heap, m_page_max_count, true, true);
        }

        virtual void release()
        {
            // deallocate any allocated resources
            // decommit all used pages
            // release virtual memory
            xheap heap(m_alloc);
            m_notfull_used_pages.release(heap);
            m_notused_free_pages.release(heap);
        }

        XCORE_CLASS_PLACEMENT_NEW_DELETE

        xalloc*          m_alloc;
        xvirtual_memory* m_vmem;
        void*            m_base_addr;
        u32              m_page_size;
        s32              m_page_max_count;
        u32              m_page_max_alloc_count;
        xbitlist         m_notfull_used_pages;
        xbitlist         m_notused_free_pages;
    };

    xdexedfxsa* gCreateVMemBasedDexAllocator(xalloc* a, xvirtual_memory* vmem, u32 alloc_size, u64 addr_range, u32 page_size)
    {
        xheap    heap(a);
        vmalloc* allocator = heap.construct<vmalloc>();
        allocator->init(a, vmem, alloc_size, addr_range, page_size);
        return allocator;
    }
#endif

    static inline bool is_null(u32 i) { return i == Null; }
    static inline bool is_node(u32 i) { return (i != Null) && ((i & Type_Mask) == Type_Node); }
    static inline bool is_leaf(u32 i) { return (i != Null) && ((i & Type_Mask) == Type_Leaf); }

    static inline u32 as_node(u32 i)
    {
        ASSERT(i != Null);
        return i | Type_Node;
    }
    static inline u32 as_leaf(u32 i)
    {
        ASSERT(i != Null);
        return i | Type_Leaf;
    }
    static inline u32 as_index(u32 i)
    {
        ASSERT(i != Null);
        return i & ~Type_Mask;
    }

    struct xbtree32::node_t
    {
        inline node_t() { clear(); }

        inline bool is_empty() const { return is_null(m_nodes[0]) && is_null(m_nodes[1]) && is_null(m_nodes[2]) && is_null(m_nodes[3]); }

        void clear()
        {
            m_nodes[0] = Null;
            m_nodes[1] = Null;
            m_nodes[2] = Null;
            m_nodes[3] = Null;
        }

        XCORE_CLASS_PLACEMENT_NEW_DELETE

        u32 m_nodes[4];
    };
    void xbtree32::init(xfsallocdexed* node_allocator, keyvalue* kv)
    {
        m_idxr       = nullptr;
        m_node_alloc = node_allocator;
        xpool   pool(m_node_alloc);
        node_t* root = pool.construct<node_t>();
        root->clear();
        m_root = m_node_alloc->ptr2idx(root);
        m_kv   = kv;
    }

    void xbtree32::init(xfsallocdexed* node_allocator, keyvalue* kv, key_indexer const* indexer)
    {
        init(node_allocator, kv);
        m_idxr = indexer;
    }

    void xbtree32::init_from_index(xfsallocdexed* node_allocator, keyvalue* kv, u32 max_index)
    {
        init(node_allocator, kv);
        initialize_from_index(m_idxr_data, max_index);
    }

    void xbtree32::init_from_mask(xfsallocdexed* node_allocator, keyvalue* kv, u64 mask, bool sorted)
    {
        init(node_allocator, kv);
        initialize_from_mask(m_idxr_data, mask, sorted);
    }

    bool xbtree32::add(u64 key, u32 value)
    {
        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        s32     level      = 0;
        node_t* parentNode = (node_t*)m_node_alloc->idx2ptr(m_root);
        do
        {
            s32 childIndex     = idxr->get_index(key, level);
            u32 childNodeIndex = parentNode->m_nodes[childIndex];
            if (is_null(childNodeIndex))
            {
                m_kv->set_key(value, key);
                parentNode->m_nodes[childIndex] = as_leaf(value);
                return true;
            }
            else if (is_leaf(childNodeIndex))
            {
                // Check for duplicate, see if this value is the value of this leaf
                u64 const child_key = m_kv->get_key(as_index(childNodeIndex));
                if (idxr->keys_equal(child_key, key))
                {
                    return false;
                }

                // Create new node and add the existing item first and continue
                xpool   pool(m_node_alloc);
                node_t* newChildNode = pool.construct<node_t>();
                newChildNode->clear();
                u32 newChildNodeIndex           = m_node_alloc->ptr2idx(newChildNode);
                parentNode->m_nodes[childIndex] = as_node(newChildNodeIndex);

                // Compute the child index of this already existing leaf one level up
                // and insert this existing leaf there.
                s32 const childChildIndex              = idxr->get_index(child_key, level + 1);
                newChildNode->m_nodes[childChildIndex] = childNodeIndex;

                // Continue, at the next level, correct parentNode
                parentNode = newChildNode;
                level += 1;
            }
            else if (is_node(childNodeIndex))
            {
                // Continue traversing the tree until we find a null-child to place
                // our value.
                parentNode = (node_t*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
                level++;
            }
        } while (level < idxr->max_levels());

        return false;
    }

    bool xbtree32::rem(u64 key, u32& value)
    {
        struct utils
        {
            static bool set_child_null_and_check(node_t* node, s32& in_out_index)
            {
                node->m_nodes[in_out_index] = Null;
                in_out_index                = (in_out_index + 1) & 0x3;
                s32 c                       = is_null(node->m_nodes[in_out_index]) ? 0 : (0x0100 + in_out_index);
                in_out_index                = (in_out_index + 1) & 0x3;
                c += is_null(node->m_nodes[in_out_index]) ? 0 : (0x0100 + in_out_index);
                in_out_index = (in_out_index + 1) & 0x3;
                c += is_null(node->m_nodes[in_out_index]) ? 0 : (0x0100 + in_out_index);
                in_out_index = c & 0xFF;
                return (c & 0xFF00) == 0x0100;
            }
        };

        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        s32     level            = 0;
        node_t* parent           = nullptr;
        s32     parentChildIndex = 0;
        node_t* node             = (node_t*)m_node_alloc->idx2ptr(m_root);
        do
        {
            s32 childIndex = idxr->get_index(key, level);
            u32 nodeIndex  = node->m_nodes[childIndex];
            if (is_null(nodeIndex))
            {
                break;
            }
            else if (is_leaf(nodeIndex))
            {
                u64 const nodeKey = m_kv->get_key(nodeIndex);
                if (idxr->keys_equal(nodeKey, key))
                {
                    // Remove this leaf from node, if this results in node
                    // having no more children then this node should also be
                    // removed etc...
                    value = as_index(nodeIndex);

                    // Null out the leaf from this node and check if this
                    // node has only 1 child left, if so we should 'unchain' it.
                    if (utils::set_child_null_and_check(node, childIndex))
                    {
                        // childIndex contains the index of the only child left
                        if (parent != nullptr)
                        {
                            parent->m_nodes[parentChildIndex] = node->m_nodes[childIndex];
                            m_node_alloc->deallocate(node);
                        }
                    }

                    return true;
                }
                break;
            }
            else if (is_node(nodeIndex))
            {
                parent           = node;
                parentChildIndex = childIndex;
                node             = (node_t*)m_node_alloc->idx2ptr(as_index(nodeIndex));
            }

            level++;
        } while (level < idxr->max_levels());

        return false;
    }

    bool xbtree32::find(u64 key, u32& value) const
    {
        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        s32           level = 0;
        node_t const* node  = (node_t*)m_node_alloc->idx2ptr(m_root);
        do
        {
            s32 childIndex     = idxr->get_index(key, level);
            u32 childNodeIndex = node->m_nodes[childIndex];
            if (is_null(childNodeIndex))
            {
                break;
            }
            else if (is_leaf(childNodeIndex))
            {
                u64 const childNodeKey = m_kv->get_key(childNodeIndex);
                if (idxr->keys_equal(childNodeKey, key))
                {
                    value = childNodeIndex;
                    return true;
                }
                break;
            }
            else if (is_node(childNodeIndex))
            {
                // Continue
                node = (node_t*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
            }
            level++;
        } while (level < idxr->max_levels());

        value = Null;
        return false;
    }

    struct xbtree32::history_t
    {
        inline history_t() {}

        void set(s32 level, node_t const* n, s8 c)
        {
            m_node[level]  = n;
            m_child[level] = c;
        }

        void get(s32 level, node_t const*& n, s8& c)
        {
            n = m_node[level];
            c = m_child[level];
        }

        node_t const* m_node[32];
        s8            m_child[32];
    };

    // Find an entry 'less-or-equal' to 'value'
    bool xbtree32::lower_bound(u64 key, u32& value) const
    {
        // When traversing the tree to find a lower-bound we might fail.
        // So we have to keep a traversal history so that we can traverse
        // back up to find a branch that is actually going to give us a
        // lower-bound.
        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        history_t     path;
        s32           level = 0;
        s32           state = 0; // 0 = we are on target branch, -1 = we are on a lower-bound branch
        node_t const* node  = (node_t*)m_node_alloc->idx2ptr(m_root);
        s8            childIndex;
        u32           childNodeIndex;
        while (state == 0 && level < idxr->max_levels())
        {
            childIndex     = idxr->get_index(key, level);
            childNodeIndex = node->m_nodes[childIndex];

            if (is_leaf(childNodeIndex))
            {
                u64 const childNodeKey = m_kv->get_key(childNodeIndex);
                s32 const c            = idxr->keys_compare(childNodeKey, key);
                if (c <= 0)
                {
                    value = childNodeIndex;
                    return true;
                }
            }

            path.set(level, node, childIndex);
            level++;

            if (is_node(childNodeIndex))
            {
                node = (node_t*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
            }
            else
            {
                // Travel up until we can traverse into a lower-bound branch
                state = -2;
                while (state == -2 && level > 0)
                {
                    level--;
                    path.get(level, node, childIndex);
                    while (childIndex > 0)
                    {
                        childIndex -= 1;
                        if (!is_null(node->m_nodes[childIndex]))
                        {
                            state = -1; // We are now on a lower-bound branch for sure
                            break;
                        }
                    }
                }
            }
        }

        while (state == -1 && level < idxr->max_levels())
        {
            do
            {
                childNodeIndex = node->m_nodes[childIndex];
                childIndex -= 1;
            } while (is_null(childNodeIndex) && childIndex > 0);

            if (is_null(childNodeIndex))
                break;

            if (is_leaf(childNodeIndex))
            {
                value = childNodeIndex;
                return true;
            }

            // When we are on a lower-bound branch, we have to search a child index that is
            // not null starting at the highest index.
            childIndex = 3;

            node = (node_t*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
            level++;
        }

        value = Null;
        return false;
    }

    bool xbtree32::upper_bound(u64 key, u32& value) const
    {
        // When traversing the tree to find a lower-bound we might fail.
        // So we have to keep a traversal history so that we can traverse
        // back up to find a branch that is actually going to give us a
        // lower-bound.
        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        history_t     path;
        s32           level = 0;
        s32           state = 0; // 0 = we are on target branch, -1 = we are on a lower-bound branch
        node_t const* node  = (node_t const*)m_node_alloc->idx2ptr(m_root);
        s8            childIndex;
        u32           childNodeIndex;
        while (state == 0 && level < idxr->max_levels())
        {
            childIndex     = idxr->get_index(key, level);
            childNodeIndex = node->m_nodes[childIndex];

            if (is_leaf(childNodeIndex))
            {
                u64 const childNodeKey = m_kv->get_key(childNodeIndex);
                s32 const c            = idxr->keys_compare(childNodeKey, key);
                if (c >= 0)
                {
                    value = childNodeIndex;
                    return true;
                }
            }

            path.set(level, node, childIndex);
            level++;

            if (is_node(childNodeIndex))
            {
                node = (node_t*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
            }
            else
            {
                // Travel back to the root level until we can traverse into an upper-bound branch
                state = -2;
                while (state == -2 && level > 0)
                {
                    level--;
                    path.get(level, node, childIndex);
                    while (childIndex < 3)
                    {
                        childIndex += 1;
                        if (!is_null(node->m_nodes[childIndex]))
                        {
                            state = -1; // We are now on a lower-bound branch for sure
                            break;
                        }
                    }
                }
            }
        }

        while (state == -1 && level < idxr->max_levels())
        {
            do
            {
                childNodeIndex = node->m_nodes[childIndex];
                childIndex += 1;
            } while (is_null(childNodeIndex) && childIndex <= 3);

            if (is_null(childNodeIndex))
                break;

            if (is_leaf(childNodeIndex))
            {
                value = childNodeIndex;
                return true;
            }

            // When we are on an upper-bound branch, we have to search a child index that is
            // not null starting at the lowest index.
            childIndex = 0;

            node = (node_t*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
            level++;
        }

        value = Null;
        return false;
    }

    // ######################################################################################################################################
    // ######################################################################################################################################
    // ######################################################################################################################################
    //                                              btree using pointers
    // ######################################################################################################################################
    // ######################################################################################################################################
    // ######################################################################################################################################
    struct xbtree::node_t
    {
        inline node_t() { clear(); }

        inline bool is_empty() const { return m_nodes[0] == 0 && m_nodes[1] == 0 && m_nodes[2] == 0 && m_nodes[3] == 0; }

        void clear()
        {
            m_nodes[0] = 0;
            m_nodes[1] = 0;
            m_nodes[2] = 0;
            m_nodes[3] = 0;
        }

        XCORE_CLASS_PLACEMENT_NEW_DELETE

        uptr m_nodes[4];
    };

    static inline bool            is_null(uptr n) { return n == 0; }
    static inline bool            is_node(uptr n) { return (n != 0) && ((n & 1) == 1); }
    static inline bool            is_value(uptr n) { return (n != 0) && ((n & 1) == 0); }
    static inline uptr            as_node(uptr n) { return ((uptr)n | 1); }
    static inline uptr            as_value(uptr n) { return ((uptr)n); }
    static inline void*           as_value_ptr(uptr n) { ASSERT(is_value(n));  return (void*)((uptr)n); }
    static inline xbtree::node_t* as_node_ptr(uptr n) { ASSERT(is_node(n)); return (xbtree::node_t*)((uptr)n & ~(u64)1); }

    void xbtree::init(xfsalloc* node_allocator, keyvalue* kv)
    {
        m_idxr       = nullptr;
        m_node_alloc = node_allocator;
        m_root       = nullptr;
        m_kv         = kv;
    }

    void xbtree::init(xfsalloc* node_allocator, keyvalue* kv, key_indexer const* indexer)
    {
        init(node_allocator, kv);
        m_idxr = indexer;
    }

    void xbtree::init_from_index(xfsalloc* node_allocator, keyvalue* kv, u32 max_index)
    {
        init(node_allocator, kv);
        initialize_from_index(m_idxr_data, max_index);
    }

    void xbtree::init_from_mask(xfsalloc* node_allocator, keyvalue* kv, u64 mask, bool sorted)
    {
        init(node_allocator, kv);
        initialize_from_mask(m_idxr_data, mask, sorted);
    }

    bool xbtree::add(u64 key, void* value)
    {
        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        if (m_root == nullptr)
        {
            xpool heap(m_node_alloc);
            m_root = heap.construct<node_t>();
        }

        s32     level = 0;
        node_t* node  = m_root;
        do
        {
            s32  childIndex = idxr->get_index(key, level);
            uptr childPtr   = node->m_nodes[childIndex];
            if (is_null(childPtr))
            {
                m_kv->set_key(value, key);
                node->m_nodes[childIndex] = as_value((uptr)value);
                return true;
            }
            else if (is_value(childPtr))
            {
                // Check for duplicate, see if this value is the value of this leaf
                u64 const child_key = m_kv->get_key(as_value_ptr(childPtr));
                if (idxr->keys_equal(child_key, key))
                {
                    return false;
                }

                // Create new node and add the existing item first and continue
                xpool   pool(m_node_alloc);
                node_t* newChildNode      = pool.construct<node_t>();
                node->m_nodes[childIndex] = as_node((uptr)newChildNode);

                // Compute the child index of this already existing leaf one level up
                // and insert this existing leaf there.
                s32 const childChildIndex              = idxr->get_index(child_key, level + 1);
                newChildNode->m_nodes[childChildIndex] = childPtr;

                // Continue, at the next level, correct node
                node = newChildNode;
                level += 1;
            }
            else if (is_node(childPtr))
            {
                // Continue traversing the tree until we find a null-child to place
                // our value.
                node = as_node_ptr(childPtr);
                level++;
            }
        } while (level < idxr->max_levels());

        return false;
    }

    bool xbtree::rem(u64 key, void*& value)
    {
        struct utils
        {
            static u32 get_counts(uptr n, s32 i) { return (n == 0) ? 0x01000000 : (((n & 1) == 1) ? 0x00010000 : (0x00000100 | i)); }

            static u32 check(node_t* node)
            {
                u32 c = 0;
                c += get_counts(node->m_nodes[0], 0);
                c += get_counts(node->m_nodes[1], 1);
                c += get_counts(node->m_nodes[2], 2);
                c += get_counts(node->m_nodes[3], 3);
                return (c & 0xFFFFFF00); // Return the counts [3:NULL][2:NODE][1:VALUE][0:0]
            }

            static u32 set_child_null_and_check(node_t* node, s32& index)
            {
                node->m_nodes[index] = 0;

                // 0x..NNVV00
                u32 c = 0x01000000;
                index = (index + 1) & 0x3;
                c += get_counts(node->m_nodes[index], index);
                index = (index + 1) & 0x3;
                c += get_counts(node->m_nodes[index], index);
                index = (index + 1) & 0x3;
                c += get_counts(node->m_nodes[index], index);

                index = c & 0xFF;
                return (c & 0xFFFFFF00); // Return the counts [3:NULL][2:NODE][1:VALUE][0:0]
            }
        };

        if (m_root == nullptr)
            return false;

        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        node_t* nodes[32];
        s8      childs[32];

        s32     level = 0;
        node_t* node  = m_root;
        do
        {
            s32 childIndex = idxr->get_index(key, level);

            nodes[level]  = node;
            childs[level] = childIndex;
            uptr nodePtr  = node->m_nodes[childIndex];

            if (is_null(nodePtr))
            {
                break;
            }
            else if (is_value(nodePtr))
            {
                u64 const nodeKey = m_kv->get_key(as_value_ptr(nodePtr));
                if (idxr->keys_equal(nodeKey, key))
                {
                    // Remove this leaf from node, if this results in node
                    // having no more children then this node should also be
                    // removed etc...
                    value = as_value_ptr(nodePtr);

                    // Null out the leaf from this node and check if this
                    // node has only 1 or no child left, if so we should 'unchain' it.
                    s32 const n = utils::set_child_null_and_check(node, childIndex);
                    if (n == 0x03000100 || n == 0x04000000)
                    {
                        if (n == 0x03000100)
                        {
                            // This case means that there is one 'value' left on this node.
                            // We can move this value up only until we reach a parent node
                            // that has more than 1 children.

                            // Remember the value
                            uptr otherValue = node->m_nodes[childIndex];

                            // Move up and delete nodes that only have 1 child
                            while (level > 0)
                            {
                                m_node_alloc->deallocate(node);

                                level       = level - 1;
                                node        = nodes[level];
                                childIndex  = childs[level];
                                s32 const c = utils::check(node);
                                if (c != 0x03010000)
                                    break;
                            }

                            // Place the value at its correct place
                            u64 const childKey        = m_kv->get_key(as_value_ptr(otherValue));
                            childIndex                = idxr->get_index(childKey, level);
                            node->m_nodes[childIndex] = otherValue;
                        }
                    }
                    return true;
                }
                break;
            }
            else if (is_node(nodePtr))
            {
                node = as_node_ptr(nodePtr);
            }

            level++;
        } while (level < idxr->max_levels());

        return false;
    }

    void xbtree::clear()
    {
        if (m_root == nullptr)
            return;

        node_t* m_node[32];
        s8      m_child[32];

        m_node[0]  = m_root;
        m_child[0] = 0;

        s32 level = 1;
        while (level > 0)
        {
            level -= 1;

            node_t* node  = m_node[level];
            s8      child = m_child[level];

            // See if we still have a child node to traverse
            while (child < 4)
            {
                if (is_node(node->m_nodes[child]))
                {
                    m_node[level]    = node;
                    m_child[level++] = child;
                    m_node[level]    = as_node_ptr(node->m_nodes[child]);
                    m_child[level++] = 0;
                    break;
                }
                child += 1;
            }
            if (child == 4)
            {
				xpool heap(m_node_alloc);
				heap.destruct(node);
            }
        }
    }

    bool xbtree::find(u64 key, void*& value) const
    {
        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        s32           level = 0;
        node_t const* node  = m_root;
        do
        {
            s32  childIndex = idxr->get_index(key, level);
            uptr nodePtr    = node->m_nodes[childIndex];
            if (is_null(nodePtr))
            {
                break;
            }
            else if (is_value(nodePtr))
            {
                u64 const childNodeKey = m_kv->get_key(as_value_ptr(nodePtr));
                if (idxr->keys_equal(childNodeKey, key))
                {
                    value = as_value_ptr(nodePtr);
                    return true;
                }
                break;
            }
            else if (is_node(nodePtr))
            {
                // Continue
                node = as_node_ptr(nodePtr);
            }
            level++;
        } while (level < idxr->max_levels());

        value = nullptr;
        return false;
    }

    struct xbtree::history_t
    {
        inline history_t() {}

        void set(s32 level, node_t const* n, s8 c)
        {
            m_node[level]  = n;
            m_child[level] = c;
        }

        void get(s32 level, node_t const*& n, s8& c)
        {
            n = m_node[level];
            c = m_child[level];
        }

        node_t const* m_node[32];
        s8            m_child[32];
    };

    // Find an entry 'less-or-equal' to 'value'
    bool xbtree::lower_bound(u64 key, void*& value) const
    {
        // When traversing the tree to find a lower-bound we might fail.
        // So we have to keep a traversal history so that we can traverse
        // back up to find a branch that is actually going to give us a
        // lower-bound.
        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        history_t     path;
        s32           level = 0;
        s32           state = 0; // 0 = we are on target branch, -1 = we are on a lower-bound branch
        node_t const* node  = m_root;
        s8            childIndex;
        uptr          childPtr;
        while (state == 0 && level < idxr->max_levels())
        {
            childIndex = idxr->get_index(key, level);
            childPtr   = node->m_nodes[childIndex];

            if (is_value(childPtr))
            {
                u64 const childNodeKey = m_kv->get_key(as_value_ptr(childPtr));
                s32 const c            = idxr->keys_compare(childNodeKey, key);
                if (c <= 0)
                {
                    value = as_value_ptr(childPtr);
                    return true;
                }
            }

            path.set(level, node, childIndex);
            level++;

            if (is_node(childPtr))
            {
                node = as_node_ptr(childPtr);
            }
            else
            {
                // Travel up until we can traverse into a lower-bound branch
                state = -2;
                while (state == -2 && level > 0)
                {
                    level--;
                    path.get(level, node, childIndex);
                    while (childIndex > 0)
                    {
                        childIndex -= 1;
                        if (!is_null(node->m_nodes[childIndex]))
                        {
                            state = -1; // We are now on a lower-bound branch for sure
                            break;
                        }
                    }
                }
            }
        }

        while (state == -1 && level < idxr->max_levels())
        {
            do
            {
                childPtr = node->m_nodes[childIndex];
                childIndex -= 1;
            } while (is_null(childPtr) && childIndex > 0);

            if (is_null(childPtr))
                break;

            if (is_value(childPtr))
            {
                value = as_value_ptr(childPtr);
                return true;
            }

            // When we are on a lower-bound branch, we have to search a child index that is
            // not null starting at the highest index.
            childIndex = 3;

            node = as_node_ptr(childPtr);
            level++;
        }

        value = nullptr;
        return false;
    }

    bool xbtree::upper_bound(u64 key, void*& value) const
    {
        // When traversing the tree to find a lower-bound we might fail.
        // So we have to keep a traversal history so that we can traverse
        // back up to find a branch that is actually going to give us a
        // lower-bound.
        keydexer           default_dexer(&m_idxr_data);
        key_indexer const* idxr = m_idxr == nullptr ? &default_dexer : m_idxr;

        history_t     path;
        s32           level = 0;
        s32           state = 0; // 0 = we are on target branch, -1 = we are on a lower-bound branch
        node_t const* node  = m_root;
        s8            childIndex;
        uptr          childPtr;
        while (state == 0 && level < idxr->max_levels())
        {
            childIndex = idxr->get_index(key, level);
            childPtr   = node->m_nodes[childIndex];

            if (is_value(childPtr))
            {
                u64 const childNodeKey = m_kv->get_key(as_value_ptr(childPtr));
                s32 const c            = idxr->keys_compare(childNodeKey, key);
                if (c >= 0)
                {
                    value = as_value_ptr(childPtr);
                    return true;
                }
            }

            path.set(level, node, childIndex);
            level++;

            if (is_node(childPtr))
            {
                node = as_node_ptr(childPtr);
            }
            else
            {
                // Travel back to the root level until we can traverse into an upper-bound branch
                state = -2;
                while (state == -2 && level > 0)
                {
                    level--;
                    path.get(level, node, childIndex);
                    while (childIndex < 3)
                    {
                        childIndex += 1;
                        if (!is_null(node->m_nodes[childIndex]))
                        {
                            state = -1; // We are now on a lower-bound branch for sure
                            break;
                        }
                    }
                }
            }
        }

        while (state == -1 && level < idxr->max_levels())
        {
            do
            {
                childPtr = node->m_nodes[childIndex];
                childIndex += 1;
            } while (is_null(childPtr) && childIndex <= 3);

            if (is_null(childPtr))
                break;

            if (is_value(childPtr))
            {
                value = as_value_ptr(childPtr);
                return true;
            }

            // When we are on an upper-bound branch, we have to search a child index that is
            // not null starting at the lowest index.
            childIndex = 0;

            node = as_node_ptr(childPtr);
            level++;
        }

        value = nullptr;
        return false;
    }

} // namespace xcore