#ifndef __CBASE_TREE_INDICES_H__
#define __CBASE_TREE_INDICES_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"

namespace ncore
{
    // binary balanced search tree implemented using the red-black tree algorithm
    namespace ntree32
    {
        typedef u32 index_t;
        typedef u32 node_t;

        const u32    c_invalid_index = 0xFFFFFFFF;
        const node_t c_invalid_node  = 0xFFFFFFFF;

        enum echild_t
        {
            LEFT  = 0,
            RIGHT = 1,
        };

        enum ecolor_t
        {
            RED   = LEFT,
            BLACK = RIGHT
        };

        // a and b are the indices of nodes/items to compare, user_data is a pointer to the data that is passed to the tree
        typedef s8 (*compare_fn)(u32 a, u32 b, void const* user_data);

        struct tree_t
        {
            tree_t();

            s32    v_size() const;
            s32    v_capacity() const;
            node_t v_get_root() const;
            void   v_set_root(node_t node);
            node_t v_get_temp() const;
            void   v_set_color(node_t node, u8 color);
            u8     v_get_color(node_t const node) const;
            node_t v_get_node(node_t const node, s8 ne) const;
            void   v_set_node(node_t node, s8 ne, node_t set);
            node_t v_new_node();
            void   v_del_node(node_t node);

            node_t m_root;
            node_t m_temp;  // temp node is always at m_nodes[m_num_nodes_max]

            u32 m_num_nodes_current;
            u32 m_num_nodes_max;
            u32 m_nodes_free_index;
            u32 m_nodes_free_head;

            struct nnode_t
            {
                node_t m_child[2];
            };

            nnode_t* m_nodes;
            u8*      m_colors;
        };

        struct iterator_t
        {
            iterator_t(tree_t& tree)
                : m_it(c_invalid_node)
                , m_stack(0)
            {
            }

            bool              traverse(tree_t& tree, s32 d, node_t& node);
            bool              preorder(tree_t& tree, s32 d, node_t& node);
            bool              sortorder(tree_t& tree, s32 d, node_t& node);
            bool              postorder(tree_t& tree, s32 d, node_t& node);
            static inline s32 getdir(s32 compare) { return (compare + 1) >> 1; }

            node_t m_it;
            node_t m_stack_array[32];
            s32    m_stack;
        };

        void setup_tree(tree_t& c, u32 max_nodes, void* nodes, void* colors);
        void teardown_tree(tree_t& c);

        inline int_t size(tree_t& c) { return c.v_size(); }
        inline int_t capacity(tree_t& c) { return c.v_capacity(); }

        bool       clear(tree_t& c, node_t& n);  // Repeatedly call 'clear' until true is returned
        bool       find(tree_t const& c, index_t key, compare_fn comparer, void const* user_data, node_t& found);
        bool       insert(tree_t& c, index_t key, compare_fn comparer, void const* user_data, node_t& inserted);
        bool       remove(tree_t& c, index_t key, compare_fn comparer, void const* user_data, node_t& removed);
        bool       validate(tree_t& c, const char*& error_str, compare_fn comparer, void const* user_data);
        iterator_t iterate(tree_t& c);

    }  // namespace ntree32

}  // namespace ncore

#include "cbase/private/c_tree32_internal.h"

#endif
