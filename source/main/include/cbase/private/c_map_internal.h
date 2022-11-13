namespace ncore
{
    template <typename T, typename K, typename V, typename H = hasher_t<K>> class map_tree_ctxt_t : public tree_t::ctxt_t
    {
    public:
        struct nodeT_t
        {
            T m_branches[3];
        };

        map_tree_ctxt_t(alloc_t* allocator, u32 maxsize, bool has_values = true)
            : tree_t::ctxt_t()
            , m_max_size(maxsize)
        {
            m_allocator = allocator;
            m_size      = 0;
            m_nodes     = m_allocator->obtain_array<nodeT_t>(maxsize);
            m_colors    = m_allocator->obtain_array<u32>((maxsize + 31) >> 5);
            m_keys      = m_allocator->obtain_array<K>(maxsize);

            m_values = nullptr;
            if (has_values)
                m_values = m_allocator->obtain_array<V>(maxsize);

            m_hashes = nullptr;
            H hasher;
            if (hasher.is_hashable())
            {
                m_hashes = m_allocator->obtain_array<u64>(maxsize);
            }
        }

        virtual ~map_tree_ctxt_t()
        {
            m_allocator->deallocate(m_nodes);
            m_allocator->deallocate(m_colors);
            m_allocator->deallocate(m_hashes);
            m_allocator->deallocate(m_keys);
            m_allocator->deallocate(m_values);
        }

        virtual int_t v_size() const { return m_size; }
        virtual int_t v_capacity() const { return m_max_size; }
        virtual void  v_set_color(tree_t::node_t* node, tree_t::color_e color)
        {
            T const   index = (T)(node - m_nodes);
            u32 const bit   = index & 31;
            u32 const word  = index >> 5;
            if (color == tree_t::RED)
                m_colors[word] |= (1 << bit);
            else
                m_colors[word] &= ~(1 << bit);
        }
        virtual tree_t::color_e v_get_color(tree_t::node_t const* node) const
        {
            T const   index = (T)(node - m_nodes);
            u32 const bit   = index & 31;
            u32 const word  = index >> 5;
            return (m_colors[word] & (1 << bit)) ? tree_t::RED : tree_t::BLACK;
        }
        virtual void const* v_get_key(tree_t::node_t const* node) const
        {
            T const index = (T)(node - m_nodes);
            return (void const*)&m_keys[index];
        }
        virtual void const* v_get_value(tree_t::node_t const* node) const
        {
            T const index = (T)(node - m_nodes);
            return (void const*)&m_values[index];
        }
        virtual tree_t::node_t* v_get_node(tree_t::node_t const* n, tree_t::node_e ne) const
        {
            T const  index = (T)(n - m_nodes);
            nodeT_t* node  = &m_nodes[index];
            return (tree_t::node_t*)&m_nodes[node->m_branches[ne]];
        }
        virtual void v_set_node(tree_t::node_t* n, tree_t::node_e ne, tree_t::node_t* set) const
        {
            T const  index       = (T)(n - m_nodes);
            nodeT_t* node        = &m_nodes[index];
            node->m_branches[ne] = (T)(set - m_nodes);
        }
        virtual tree_t::node_t* v_new_node(void const* key, void const* value)
        {
            if (m_size >= m_max_size)
                return nullptr;
            nodeT_t* newnode = m_freelist;
            m_freelist       = &m_nodes[newnode->m_branch[0]];
            T const index    = (T)(newnode - m_nodes);

            // Constructor K
            void* mem = (void*)(&m_keys[index]);
            new (mem) K(*(K*)data);

            // Constructor V
            mem = (void*)(&m_values[index]);
            new (mem) V(*(V*)value);

            m_size++;
            return (tree_t::node_t*)newnode;
        }
        virtual void v_del_node(tree_t::node_t* node)
        {
            nodeT_t* delnode     = (nodeT_t*)node;
            delnode->m_branch[0] = (T)(m_freelist - m_nodes);
            m_freelist           = delnode;

            T const index = (T)(delnode - m_nodes);

            // Destructor of K
            void* mem = (void*)(&m_keys[index]);
            ((K*)mem)->~K();

            // Destructor of V
            mem = (void*)(&m_values[index]);
            ((V*)mem)->~V();

            m_size--;
        }
        virtual s32 v_compare_nodes(tree_t::node_t const* node, tree_t::node_t const* other) const
        {
            T const index  = (T)(node - m_nodes);
            T const oindex = (T)(other - m_nodes);
            if (m_keys[index] < m_keys[oindex])
                return -1;
            else if (m_keys[index] > m_keys[oindex])
                return 1;
            return 0;
        }
        virtual s32 v_compare_insert(void const* data, tree_t::node_t const* node) const
        {
            T const  index = (T)(node - m_nodes);
            K const& key   = *(K const*)data;
            if (key < m_keys[index])
                return -1;
            else if (key > m_keys[index])
                return 1;
            return 0;
        }

        u32        m_max_size;
        u32        m_size;
        alloc_t*   m_allocator;
        nodeT_t*   m_nodes;
        nodeT_t*   m_freelist; // points to the head of the list of free nodes
        u32*       m_colors;
        H::hash_t* m_hashes;
        K*         m_keys;
        V*         m_values;
    };
} // namespace ncore
