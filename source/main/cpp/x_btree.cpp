#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#include "xbase/x_btree.h"

namespace xcore
{
    namespace indexed
    {
        enum
        {
            Null      = 0xffffffff,
            Type_Mask = 0x80000000,
            Type_Leaf = 0x00000000,
            Type_Node = 0x80000000
        };

        static inline bool is_null(u32 i) { return i == Null; }
        static inline bool is_node(u32 i) { return (i != Null) && ((i & Type_Mask) == Type_Node); }
        static inline bool is_leaf(u32 i) { return (i != Null) && ((i & Type_Mask) == Type_Leaf); }

        static inline u32 as_node(u32 i) { ASSERT(i != Null); return i | Type_Node; }
        static inline u32 as_leaf(u32 i) { ASSERT(i != Null); return i | Type_Leaf; }
        static inline u32 as_index(u32 i) { ASSERT(i != Null); return i & ~Type_Mask; }

        struct node
        {
            inline bool is_empty() const { return is_null(m_nodes[0]) && is_null(m_nodes[1]) && is_null(m_nodes[2]) && is_null(m_nodes[3]); }
			void		clear()
			{
				m_nodes[0] = Null;
				m_nodes[1] = Null;
				m_nodes[2] = Null;
				m_nodes[3] = Null;
			}
            u32         m_nodes[4];
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

        void btree_t::init(keydexer const& indexer, xdexedfxsa* node_allocator, indexed::keyvalue* kv)
        {
            m_idxr              = indexer;
            m_node_alloc        = node_allocator;
            indexed::node* root = (indexed::node*)m_node_alloc->allocate();
			root->clear();
            m_root              = m_node_alloc->ptr2idx(root);
            m_kv                = kv;
        }

        bool btree_t::add(u64 key, u32 value)
        {
            s32            level      = 0;
            indexed::node* parentNode = (indexed::node*)m_node_alloc->idx2ptr(m_root);
            do
            {
                s32 childIndex     = m_idxr.get_index(key, level);
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
                    if (m_idxr.keys_equal(child_key, key))
                    {
                        return false;
                    }

                    // Create new node and add the existing item first and continue
                    indexed::node* newChildNode      = (indexed::node*)m_node_alloc->allocate();
					newChildNode->clear();
                    u32            newChildNodeIndex = m_node_alloc->ptr2idx(newChildNode);
                    parentNode->m_nodes[childIndex]  = as_node(newChildNodeIndex);

                    // Compute the child index of this already existing leaf one level up
                    // and insert this existing leaf there.
                    s32 const childChildIndex              = m_idxr.get_index(child_key, level + 1);
                    newChildNode->m_nodes[childChildIndex] = childNodeIndex;

                    // Continue, at the next level, correct parentNode
					parentNode = newChildNode;
					level += 1;
                }
                else if (is_node(childNodeIndex))
                {
                    // Continue traversing the tree until we find a null-child to place
                    // our value.
                    parentNode = (indexed::node*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
                    level++;
                }
            } while (level < m_idxr.max_levels());

            return false;
        }

        bool btree_t::rem(u64 key, u32& value)
        {
            struct utils
            {
                static bool set_child_null_and_check(indexed::node* node, s32& in_out_index)
                {
					node->m_nodes[in_out_index] = Null;
					in_out_index = (in_out_index+1)&0x3;
                    s32 c = is_null(node->m_nodes[in_out_index]) ? 0 : (0x0100 + in_out_index);
					in_out_index = (in_out_index+1)&0x3;
                    c += is_null(node->m_nodes[in_out_index]) ? 0 : (0x0100 + in_out_index);
					in_out_index = (in_out_index+1)&0x3;
                    c += is_null(node->m_nodes[in_out_index]) ? 0 : (0x0100 + in_out_index);
                    in_out_index = c & 0xFF;
                    return (c & 0xFF00) == 0x0100;
                }
            };

            s32            level            = 0;
            indexed::node* parent           = nullptr;
            s32            parentChildIndex = 0;
            indexed::node* node             = (indexed::node*)m_node_alloc->idx2ptr(m_root);
            do
            {
                s32 childIndex = m_idxr.get_index(key, level);
                u32 nodeIndex  = node->m_nodes[childIndex];
                if (is_null(nodeIndex))
                {
                    break;
                }
                else if (is_leaf(nodeIndex))
                {
                    u64 const nodeKey = m_kv->get_key(nodeIndex);
                    if (m_idxr.keys_equal(nodeKey, key))
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
                    node = (indexed::node*)m_node_alloc->idx2ptr(as_index(nodeIndex));
                }

                level++;
            } while (level < m_idxr.max_levels());

            return false;
        }

        bool btree_t::find(u64 key, u32& value) const
        {
            s32                  level = 0;
            indexed::node const* node  = (indexed::node*)m_node_alloc->idx2ptr(m_root);
            do
            {
                s32 childIndex     = m_idxr.get_index(key, level);
                u32 childNodeIndex = node->m_nodes[childIndex];
                if (is_null(childNodeIndex))
                {
                    break;
                }
                else if (is_leaf(childNodeIndex))
                {
                    u64 const childNodeKey = m_kv->get_key(childNodeIndex);
                    if (m_idxr.keys_equal(childNodeKey, key))
                    {
                        value = childNodeIndex;
                        return true;
                    }
                    break;
                }
                else if (is_node(childNodeIndex))
                {
                    // Continue
                    node = (indexed::node*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
                }
                level++;
            } while (level < m_idxr.max_levels());

            value = Null;
            return false;
        }

        struct history_t
        {
            inline history_t() {}

            void set(s32 level, indexed::node const* n, s8 c)
            {
                m_node[level]  = n;
                m_child[level] = c;
            }

            void get(s32 level, indexed::node const*& n, s8& c)
            {
                n = m_node[level];
                c = m_child[level];
            }

            indexed::node const* m_node[32];
            s8                   m_child[32];
        };

        // Find an entry 'less-or-equal' to 'value'
        bool btree_t::lower_bound(u64 key, u32& value) const
        {
            // When traversing the tree to find a lower-bound we might fail.
            // So we have to keep a traversal history so that we can traverse
            // back up to find a branch that is actually going to give us a
            // lower-bound.
            history_t            path;
            s32                  level = 0;
            s32                  state = 0; // 0 = we are on target branch, -1 = we are on a lower-bound branch
            indexed::node const* node  = (indexed::node*)m_node_alloc->idx2ptr(m_root);
            s8                   childIndex;
            u32                  childNodeIndex;
            while (state == 0 && level < m_idxr.max_levels())
            {
                childIndex     = m_idxr.get_index(key, level);
                childNodeIndex = node->m_nodes[childIndex];

                if (is_leaf(childNodeIndex))
                {
                    u64 const childNodeKey = m_kv->get_key(childNodeIndex);
                    s32 const c            = m_idxr.keys_compare(childNodeKey, key);
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
                    node = (indexed::node*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
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

            while (state == -1 && level < m_idxr.max_levels())
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

                node = (indexed::node*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
                level++;
            }

            value = Null;
            return false;
        }

        bool btree_t::upper_bound(u64 key, u32& value) const
        {
            // When traversing the tree to find a lower-bound we might fail.
            // So we have to keep a traversal history so that we can traverse
            // back up to find a branch that is actually going to give us a
            // lower-bound.
            history_t            path;
            s32                  level = 0;
            s32                  state = 0; // 0 = we are on target branch, -1 = we are on a lower-bound branch
            indexed::node const* node  = (indexed::node*)m_node_alloc->idx2ptr(m_root);
            s8                   childIndex;
            u32                  childNodeIndex;
            while (state == 0 && level < m_idxr.max_levels())
            {
                childIndex     = m_idxr.get_index(key, level);
                childNodeIndex = node->m_nodes[childIndex];

                if (is_leaf(childNodeIndex))
                {
                    u64 const childNodeKey = m_kv->get_key(childNodeIndex);
                    s32 const c            = m_idxr.keys_compare(childNodeKey, key);
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
                    node = (indexed::node*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
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

            while (state == -1 && level < m_idxr.max_levels())
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

                node = (indexed::node*)m_node_alloc->idx2ptr(as_index(childNodeIndex));
                level++;
            }

            value = Null;
            return false;
        }

    } // namespace btree

    /*
    Key Dexer notes:

        // 'value' spans the lower 26 bits
        // masks  = { 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3 };
        // shifts = { 24,   22,  20,  18,  16,  14,  12,  10,   8,   6,   4,   2,   0 };

        // 65536 number of items
        //  16 K nodes
        //   4 K
        //   1 K
        // 256
        //  64
        //	16
        //   4
        // 21*1024 + 256 + 64 + 16 + 4 + 1 = 21845 nodes

        // Note: Make sure you keep the mask/shift in order otherwise you will break the
        //       presumption that all values are ordered.
        //       Also all masks should connect and have no bit gaps, you may skip a range of
        //       upper bits as long as you are sure that they do not change for the complete
        //       set of values.
        // Example:
        // Let's say you have an address range of 32 GB with a alignment of 1024, this means
        // that you can mask these bits: 0x#######7FFFFFC00. Since a virtual memory address
        // can have bits set in ####### part you know that this doesn't matter.

        // -------------- 1st brute-force approach to compute the mask:
        // uptr base_address = 0x07F0500800000000;
        // u32 const num_pages = 8192;
        // u32 const page_size = 65536;
        // u32 const granularity = 1024;
        // uptr end_address = base_address + num_pages * page_size;
        // uptr address = base_address;
        // uptr mask = 0;
        // uptr fixed = 0xffffffffffffffff;
        // while (address < end_address)
        // {
        //     mask = mask | address;
        //     fixed = fixed & mask;
        //     address += alignment;
        // }
        //
        // -------------- 2nd approach to getting the mask
        //
        // uptr address_range = page_size * num_pages;
        // u64 mask = address_range;
        // mask = mask | (mask >> 1);
        // mask = mask | (mask >> 2);
        // mask = mask | (mask >> 4);
        // mask = mask | (mask >> 8);
        // mask = mask | (mask >> 16);
        // mask = mask | (mask >> 32);
        // mask = mask & ~(granularity - 1);
        //
        // u32 maskbitcnt = xcountBits(mask);
        // u32 numlevels = (maskbitcnt + 1) / 2;
        // if ((maskbitcnt & 1) == 1)
        // {
        //     // Which way should we extend the mask to make the count 'even'
        //     if ((mask & (1<<63)) == 0)
        //     {
        //         mask = mask | (mask << 1);
        //     }
        //     else
        //     {
        //         mask = mask | (mask >> 1);
        //     }
        //     maskbitcnt += 1;
        // }
    */

} // namespace xcore