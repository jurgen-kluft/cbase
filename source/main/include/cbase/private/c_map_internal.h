namespace ncore
{
    template <typename T, typename K, typename V, typename H = nhash::hasher_t<K>> class map_tree_ctxt_t : public tree_t::ctxt_t
    {
    public:
        struct nodeT_t
        {
            T m_branches[3];
        };

        DCORE_CLASS_PLACEMENT_NEW_DELETE

        map_tree_ctxt_t(alloc_t* allocator, u32 maxsize, bool has_values = true)
            : tree_t::ctxt_t()
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

        virtual s32   v_size() const final { return m_size; }
        virtual s32   v_capacity() const final { return m_max_size; }
        virtual void  v_set_color(tree_t::node_t* node, tree_t::color_e color) final 
        {
            T const   index = (T)((nodeT_t*)node - m_nodes);
            u32 const bit   = index & 31;
            u32 const word  = index >> 5;
            if (color == tree_t::RED)
                m_colors[word] |= (1 << bit);
            else
                m_colors[word] &= ~(1 << bit);
        }
        virtual tree_t::color_e v_get_color(tree_t::node_t const* node) const final
        {
            T const   index = (T)((nodeT_t*)node - m_nodes);
            u32 const bit   = index & 31;
            u32 const word  = index >> 5;
            return (m_colors[word] & (1 << bit)) ? tree_t::RED : tree_t::BLACK;
        }
        virtual void const* v_get_key(tree_t::node_t const* node) const final
        {
            T const index = (T)((nodeT_t*)node - m_nodes);
            return (void const*)&m_keys[index];
        }
        virtual void const* v_get_value(tree_t::node_t const* node) const final
        {
            T const index = (T)((nodeT_t*)node - m_nodes);
            return (void const*)&m_values[index];
        }
        virtual tree_t::node_t* v_get_node(tree_t::node_t const* n, tree_t::node_e ne) const final
        {
            T const  index = (T)((nodeT_t*)n - m_nodes);
            nodeT_t* node  = &m_nodes[index];
            return (tree_t::node_t*)&m_nodes[node->m_branches[ne]];
        }
        virtual void v_set_node(tree_t::node_t* n, tree_t::node_e ne, tree_t::node_t* set) const final
        {
            T const  index       = (T)((nodeT_t*)n - m_nodes);
            nodeT_t* node        = &m_nodes[index];
            node->m_branches[ne] = (T)((nodeT_t*)set - m_nodes);
        }
        virtual tree_t::node_t* v_new_node(void const* key, void const* value) final
        {
            if (m_size >= m_max_size)
                return nullptr;

            nodeT_t* newnode;
            if (m_freelist != nullptr)
            {
                newnode    = m_freelist;
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
            
            T const index          = (T)(newnode - m_nodes);

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

            // Color
            v_set_color((tree_t::node_t*)newnode, tree_t::RED);

            m_size++;
            return (tree_t::node_t*)newnode;
        }

        virtual void v_del_node(tree_t::node_t* node) final 
        {
            nodeT_t* delnode       = (nodeT_t*)node;
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

        virtual s32 v_compare_nodes(tree_t::node_t const* node, tree_t::node_t const* other) const final
        {
            T const index  = (T)((nodeT_t*)node - m_nodes);
            T const oindex = (T)((nodeT_t*)other - m_nodes);
            if (m_keys[index] < m_keys[oindex])
                return -1;
            else if (m_keys[index] > m_keys[oindex])
                return 1;
            return 0;
        }
        
        virtual s32 v_compare_insert(void const* data, tree_t::node_t const* node) const final
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
        nodeT_t* m_freelist; // points to the head of the list of free nodes
        u32*     m_colors;
        u64*     m_hashes;
        K*       m_keys;
        V*       m_values;
    };
} // namespace ncore
