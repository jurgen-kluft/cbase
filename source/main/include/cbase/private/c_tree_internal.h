namespace ncore
{
    namespace ntree
    {
        inline tree_t::tree_t()
            : m_root(nullptr)
            , m_alloc(nullptr)
            , m_size(0)
        {
        }

        inline s32 tree_t::v_size() const { return m_size; }
        inline s32 tree_t::v_capacity() const { return 0x7FFFFFFF; }

        inline node_t* tree_t::v_get_root() const { return m_root; }
        inline void    tree_t::v_set_root(node_t* node) { m_root = node; }

        inline void tree_t::v_set_color(node_t* node, u8 color)
        {
            ASSERT(node != nullptr);
            node->m_item = (void*)(((ptr_t)node->m_item & ~1) | ((ptr_t)color & 1));
        }

        inline u8 tree_t::v_get_color(node_t* const node) const
        {
            ASSERT(node != nullptr);
            return (u8)(static_cast<u8>((ptr_t)node->m_item) & 1);
        }

        inline void* tree_t::v_get_item(node_t* const node) const
        {
            ASSERT(node != nullptr);
            return (void*)((ptr_t)node->m_item & ~1);
        }

        inline node_t* tree_t::v_get_node(node_t* const node, u8 ne) const
        {
            ASSERT(node != nullptr);
            return node->m_child[ne];
        }
        inline void tree_t::v_set_node(node_t* node, u8 ne, node_t* set)
        {
            ASSERT(node != nullptr);
            node->m_child[ne] = set;
        }

        inline node_t* tree_t::v_new_node(void* item)
        {
            node_t* new_node     = (node_t*)m_alloc->allocate(sizeof(node_t));
            new_node->m_child[0] = nullptr;
            new_node->m_child[1] = nullptr;
            new_node->m_item     = item;
            v_set_color(new_node, RED);
            m_size += 1;
            return new_node;
        }

        inline void tree_t::v_del_node(node_t* node)
        {
            ASSERT(node != nullptr);
            m_size -= 1;
            m_alloc->deallocate(node);
        }

    }  // namespace ntree
}  // namespace ncore
