#ifndef __CBASE_TREE_POINTERS_H__
#define __CBASE_TREE_POINTERS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"

namespace ncore
{
    // binary balanced search tree implemented using the red-black tree algorithm
    namespace ntree
    {
        struct node_t
        {
            node_t* m_child[2];
            void*   m_item;
        };

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

        typedef s8 (*compare_fn)(void const* item1, void const* item2);

        struct tree_t
        {
            tree_t();

            s32     v_size() const;
            s32     v_capacity() const;
            node_t* v_get_root() const;
            void    v_set_root(node_t* node);
            void    v_set_color(node_t* node, u8 color);
            u8      v_get_color(node_t* const node) const;
            void*   v_get_item(node_t* const node) const;
            node_t* v_get_node(node_t* const node, u8 ne) const;
            void    v_set_node(node_t* node, u8 ne, node_t* set);
            node_t* v_new_node(void* item);
            void    v_del_node(node_t* node);

            node_t*  m_root;
            alloc_t* m_alloc;
            u32      m_size;
        };

        struct iterator_t
        {
            iterator_t()
                : m_it(nullptr)
                , m_stack(0)
            {
            }

            bool              traverse(tree_t const& tree, s32 d, void*& item);
            bool              preorder(tree_t const& tree, s32 d, void*& item);
            bool              sortorder(tree_t const& tree, s32 d, void*& item);
            bool              postorder(tree_t const& tree, s32 d, void*& item);
            static inline s32 getdir(s32 compare) { return (compare + 1) >> 1; }

            node_t* m_it;
            node_t* m_stack_array[32];
            s32     m_stack;
        };

        void setup_tree(alloc_t* allocator, tree_t& c);
        void teardown_tree(alloc_t* allocator, tree_t& c);

        inline int_t size(tree_t& c) { return c.v_size(); }

        bool       clear(tree_t& c, node_t*& n);  // Repeatedly call 'clear' until true is returned
        bool       find(tree_t const& c, void const* key, compare_fn comparer, node_t*& found);
        bool       insert(tree_t& c, void* key, compare_fn comparer, node_t*& inserted);
        bool       remove(tree_t& c, void const* key, compare_fn comparer, node_t*& removed);
        bool       validate(tree_t& c, const char*& error_str, compare_fn comparer);
        iterator_t iterate();

    }  // namespace ntree

}  // namespace ncore

#include "cbase/private/c_tree_internal.h"

#endif
