#include "cbase/c_hash.h"
#include "cbase/c_tree.h"

namespace ncore
{
    template <typename T, typename K, typename V, typename H = nhash::hashing_t<K>>
    class map_tree_ctxt_t : public ntree::tree_t
    {
    public:
        // red-black tree node
        // u16, size of node is 6 bytes + 1 bit for color
        // u32, size of node is 12 bytes + 1 bit for color
        struct nodeT_t
        {
            T m_branches[3];
        };

        DCORE_CLASS_PLACEMENT_NEW_DELETE

        map_tree_ctxt_t(alloc_t* allocator, u32 maxsize, bool has_values = true)
            : ntree::tree_t()
            , m_max_size(maxsize)
        {
            m_allocator = allocator;
            m_size      = 0;
            m_freeindex = 0;
            m_nodes     = g_allocate_array<nodeT_t>(m_allocator, maxsize);
            m_colors    = g_allocate_array<u32>(m_allocator, (maxsize + 31) >> 5);
            m_keys      = g_allocate_array<K>(m_allocator, maxsize);

            m_values = nullptr;
            if (has_values)
                m_values = g_allocate_array<V>(m_allocator, maxsize);

            m_hashes = nullptr;
            H hasher;
            if (hasher.is_hashable())
            {
                m_hashes = g_allocate_array<u64>(m_allocator, maxsize);
            }

            m_freelist = nullptr;

            m_root = (nodeT_t*)v_new_node(nullptr, nullptr);
            m_nill = (nodeT_t*)v_new_node(nullptr, nullptr);

            ntree::node_t*& root = (ntree::node_t*&)m_root;
            ntree::node_t*& nill = (ntree::node_t*&)m_nill;

            ntree::initialize(this, nill, root);
        }

        virtual ~map_tree_ctxt_t()
        {
            g_deallocate_array(m_allocator, m_nodes);
            g_deallocate_array(m_allocator, m_colors);
            g_deallocate_array(m_allocator, m_keys);
            if (m_hashes != nullptr)
                g_deallocate_array(m_allocator, m_hashes);
            if (m_values != nullptr)
                g_deallocate_array(m_allocator, m_values);
        }

        virtual s32            v_size() const final { return m_size; }
        virtual s32            v_capacity() const final { return m_max_size; }
        virtual ntree::node_t* v_get_nill() const final { return (ntree::node_t*)m_nill; }
        virtual ntree::node_t* v_get_root() const final { return (ntree::node_t*)m_root; }
        virtual void           v_set_color(ntree::node_t* node, ntree::color_e color) final
        {
            T const   index = (T)((nodeT_t*)node - m_nodes);
            u32 const bit   = index & 31;
            u32 const word  = index >> 5;
            if (color == ntree::RED)
                m_colors[word] |= (1 << bit);
            else
                m_colors[word] &= ~(1 << bit);
        }
        virtual ntree::color_e v_get_color(ntree::node_t const* node) const final
        {
            T const   index = (T)((nodeT_t*)node - m_nodes);
            u32 const bit   = index & 31;
            u32 const word  = index >> 5;
            return (m_colors[word] & (1 << bit)) ? ntree::RED : ntree::BLACK;
        }
        virtual void const* v_get_key(ntree::node_t const* node) const final
        {
            T const index = (T)((nodeT_t*)node - m_nodes);
            return (void const*)&m_keys[index];
        }
        virtual void const* v_get_value(ntree::node_t const* node) const final
        {
            T const index = (T)((nodeT_t*)node - m_nodes);
            return (void const*)&m_values[index];
        }
        virtual ntree::node_t* v_get_node(ntree::node_t const* n, ntree::node_e ne) const final
        {
            T const  index = (T)((nodeT_t*)n - m_nodes);
            nodeT_t* node  = &m_nodes[index];
            return (ntree::node_t*)&m_nodes[node->m_branches[ne]];
        }
        virtual void v_set_node(ntree::node_t* n, ntree::node_e ne, ntree::node_t* set) final
        {
            T const  index       = (T)((nodeT_t*)n - m_nodes);
            nodeT_t* node        = &m_nodes[index];
            node->m_branches[ne] = (T)((nodeT_t*)set - m_nodes);
        }
        virtual ntree::node_t* v_new_node(void const* key, void const* value) final
        {
            if (m_size >= m_max_size)
                return nullptr;

            nodeT_t* newnode;
            if (m_freelist != nullptr)
            {
                newnode             = m_freelist;
                u32 const freeindex = newnode->m_branches[0];
                if (freeindex == type_t<T>::max())
                {
                    m_freelist = nullptr;
                }
                else
                {
                    m_freelist = &m_nodes[freeindex];
                }
            }
            else
            {
                newnode = &m_nodes[m_freeindex];
                m_freeindex++;
            }

            newnode->m_branches[0] = type_t<T>::max();
            newnode->m_branches[1] = type_t<T>::max();
            newnode->m_branches[2] = type_t<T>::max();

            T const index = (T)(newnode - m_nodes);

            // Constructor K
            K* pkey = (K*)(&m_keys[index]);
            if (key != nullptr)
            {
                cstd::copy_construct(pkey, *(K const*)key);
            }
            else
            {
                cstd::construct(pkey);
            }

            if (m_values != nullptr)
            {
                // Constructor V
                V* pvalue = (V*)(&m_values[index]);
                if (value != nullptr)
                {
                    cstd::copy_construct(pvalue, *(V const*)value);
                }
                else
                {
                    cstd::construct(pvalue);
                }
            }
            
            // Color
            v_set_color((ntree::node_t*)newnode, ntree::RED);

            m_size++;
            return (ntree::node_t*)newnode;
        }

        virtual void v_del_node(ntree::node_t* node) final
        {
            nodeT_t* delnode = (nodeT_t*)node;
            if (m_freelist == nullptr)
            {
                delnode->m_branches[0] = type_t<T>::max();
            }
            else
            {
                delnode->m_branches[0] = (T)(m_freelist - m_nodes);
            }
            m_freelist = delnode;

            T const index = (T)(delnode - m_nodes);

            cstd::destruct(&m_keys[index]);
            cstd::destruct(&m_values[index]);

            m_size--;

            if (m_size == 0)
                m_freeindex = 0;
        }

        virtual s32 v_compare_nodes(ntree::node_t const* node, ntree::node_t const* other) const final
        {
            T const index  = (T)((nodeT_t*)node - m_nodes);
            T const oindex = (T)((nodeT_t*)other - m_nodes);
            if (m_keys[index] < m_keys[oindex])
                return -1;
            else if (m_keys[index] > m_keys[oindex])
                return 1;
            return 0;
        }

        virtual s32 v_compare_insert(void const* data, ntree::node_t const* node) const final
        {
            T const  index = (T)((nodeT_t*)node - m_nodes);
            K const& key   = *(K const*)data;
            if (key < m_keys[index])
                return -1;
            else if (key > m_keys[index])
                return 1;
            return 0;
        }

        u32      m_max_size;
        u32      m_size;
        u32      m_freeindex;
        alloc_t* m_allocator;
        nodeT_t* m_nodes;
        nodeT_t* m_root;
        nodeT_t* m_nill;
        nodeT_t* m_freelist;  // points to the head of the list of free nodes
        u32*     m_colors;
        u64*     m_hashes;
        K*       m_keys;
        V*       m_values;
    };
}  // namespace ncore
