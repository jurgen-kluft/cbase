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

        struct nnode_t
        {
            node_t m_child[2];
        };

        struct tree_t
        {
            tree_t();

            void   reset();
            void   set_color(node_t node, u8 color);
            u8     get_color(node_t const node) const;
            node_t get_node(node_t const node, s8 ne) const;
            void   set_node(node_t node, s8 ne, node_t set);
            node_t new_node();
            void   del_node(node_t node);

            static inline s8 getdir(s8 compare) { return (compare + 1) >> 1; }

            nnode_t* m_nodes;
            u32      m_free_index;
            u32      m_free_head;
        };

        struct iterator_t
        {
            iterator_t(tree_t& tree, node_t root)
                : m_root(root)
                , m_it(c_invalid_node)
                , m_stack(0)
            {
            }

            bool traverse(tree_t& tree, s32 d, node_t& node);
            bool preorder(tree_t& tree, s32 d, node_t& node);
            bool sortorder(tree_t& tree, s32 d, node_t& node);
            bool postorder(tree_t& tree, s32 d, node_t& node);

            node_t m_root;
            node_t m_it;
            node_t m_stack_array[32];
            s32    m_stack;
        };

        void setup_tree(tree_t& c, nnode_t* nodes);
        void teardown_tree(tree_t& c);

        bool       clear(tree_t& c, node_t& root, node_t& n);  // Repeatedly call 'clear' until true is returned
        bool       find(tree_t const& c, node_t root, index_t key, compare_fn comparer, void const* user_data, node_t& found);
        bool       insert(tree_t& c, node_t& root, node_t temp, index_t key, compare_fn comparer, void const* user_data, node_t& inserted_or_found);
        bool       remove(tree_t& c, node_t& root, node_t temp, index_t key, compare_fn comparer, void const* user_data, node_t& removed);
        bool       validate(tree_t& c, node_t root, const char*& error_str, compare_fn comparer, void const* user_data);
        iterator_t iterate(tree_t& c, node_t root);

    }  // namespace ntree32

}  // namespace ncore

#include "cbase/private/c_tree32_internal.h"

#endif
