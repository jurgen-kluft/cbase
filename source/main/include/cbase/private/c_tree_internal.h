namespace ncore
{
    namespace ntree
    {
        inline void node_t::set_color(u8 color)
        {
            m_child[1] = (node_t*)(((ptr_t)m_child[1] & ~1) | ((ptr_t)color & 1));
        }

        inline u8 node_t::get_color() const
        {
            return (u8)(static_cast<u8>((ptr_t)m_child[1]) & 1);
        }

        inline node_t* node_t::get_child(u8 ne) const
        {
            return (node_t*)((ptr_t)m_child[ne] & ~1);
        }

        inline void node_t::set_child(u8 ne, node_t* set)
        {
            m_child[ne] = (node_t*)((ptr_t)set | ((ptr_t)m_child[ne] & 1));
        }
    }  // namespace ntree
}  // namespace ncore
