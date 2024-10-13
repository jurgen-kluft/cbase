namespace ncore
{
    namespace ntree32
    {
        inline void g_init(tree_t& tree)
        {
            tree.m_nodes      = nullptr;
            tree.m_free_index = 0;
            tree.m_free_head  = c_invalid_node;
        }

        inline void tree_t::reset()
        {
            m_free_head  = c_invalid_node;
            m_free_index = 0;
        }

        inline void tree_t::set_color(node_t node, u8 color)
        {
            ASSERT(node != c_invalid_index);
            m_nodes[node].m_child[LEFT] = (m_nodes[node].m_child[LEFT] & 0x7FFFFFFF) | (color << 31);
        }

        inline u8 tree_t::get_color(node_t const node) const
        {
            ASSERT(node != c_invalid_index);
            return (m_nodes[node].m_child[LEFT] & 0x80000000) >> 31;
        }

        inline node_t tree_t::get_node(node_t const node, s8 ne) const
        {
            ASSERT(node != c_invalid_index);
            // Since we are using the highest bit to store the color of the node, we need to mask it out
            // but that also means that 0x7FFFFFFF is the highest value we can store in a node.
            // To return 0xFFFFFFFF when the node is 0x7FFFFFFF we can do the following bitwise operation:
            // - ((node + 1) & 0x80000000) | (node & 0x7FFFFFFF)
            node_t n = m_nodes[node].m_child[ne] & 0x7FFFFFFF;
            return ((n + 1) & 0x80000000) | n;
        }
        inline void tree_t::set_node(node_t node, s8 ne, node_t set)
        {
            ASSERT(node != c_invalid_index);
            m_nodes[node].m_child[ne] = (m_nodes[node].m_child[ne] & 0x80000000) | set;
        }

        inline node_t tree_t::new_node()
        {
            node_t node = m_free_head;
            if (node != c_invalid_node)
            {
                m_free_head = m_nodes[node].m_child[LEFT];
            }
            else
            {
                node = m_free_index++;
            }

            m_nodes[node].m_child[0] = c_invalid_node;
            m_nodes[node].m_child[1] = c_invalid_node;
            set_color(node, RED);
            return node;
        }

        inline void tree_t::del_node(node_t node)
        {
            ASSERT(node != c_invalid_index);
            m_nodes[node].m_child[LEFT]  = m_free_head;
            m_nodes[node].m_child[RIGHT] = c_invalid_node;
            m_free_head                  = node;
        }

    }  // namespace ntree32
}  // namespace ncore
