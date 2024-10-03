namespace ncore
{
    namespace ntree32
    {
        inline tree_t::tree_t()
            : m_root(c_invalid_node)
            , m_temp(c_invalid_node)
            , m_num_nodes_current(0)
            , m_num_nodes_max(0)
            , m_nodes_free_index(0)
            , m_nodes_free_head(c_invalid_node)
            , m_nodes(nullptr)
            , m_colors(nullptr)
        {
        }

        inline s32 tree_t::v_size() const { return m_num_nodes_current; }
        inline s32 tree_t::v_capacity() const { return m_num_nodes_max; }

        inline node_t tree_t::v_get_root() const { return m_root; }
        inline void   tree_t::v_set_root(node_t node) { m_root = node; }
        inline node_t tree_t::v_get_temp() const { return m_temp; }

        inline void tree_t::v_set_color(node_t node, u8 color)
        {
            ASSERT(node != c_invalid_index && node <= m_num_nodes_max);
            index_t iw   = node >> 3;
            index_t ib   = node & 0x7;
            m_colors[iw] = (m_colors[iw] & ~((u8)1 << ib)) | ((u8)color << ib);
        }

        inline u8 tree_t::v_get_color(node_t const node) const
        {
            ASSERT(node != c_invalid_index && node <= m_num_nodes_max);
            index_t iw = node >> 3;
            index_t ib = node & 0x7;
            return ((m_colors[iw] >> ib) & 1);
        }

        inline node_t tree_t::v_get_node(node_t const node, s8 ne) const
        {
            ASSERT(node != c_invalid_index && node <= m_num_nodes_max);
            return m_nodes[node].m_child[ne];
        }
        inline void tree_t::v_set_node(node_t node, s8 ne, node_t set)
        {
            ASSERT(node != c_invalid_index && node <= m_num_nodes_max);
            ASSERT(set == c_invalid_index || set <= m_num_nodes_max);
            m_nodes[node].m_child[ne] = set;
        }

        inline node_t tree_t::v_new_node()
        {
            node_t node = m_nodes_free_head;
            if (node != c_invalid_node)
            {
                m_nodes_free_head = m_nodes[node].m_child[LEFT];
            }
            else
            {
                node = m_nodes_free_index++;
            }

            m_nodes[node].m_child[0] = c_invalid_node;
            m_nodes[node].m_child[1] = c_invalid_node;
            v_set_color(node, RED);

            m_num_nodes_current += 1;
            return node;
        }

        inline void tree_t::v_del_node(node_t node)
        {
            ASSERT(node != c_invalid_index && node < m_num_nodes_max);
            m_nodes[node].m_child[LEFT] = c_invalid_index;
            m_nodes_free_head           = node;
            m_num_nodes_current -= 1;
        }

    }  // namespace ntree32
}  // namespace ncore
