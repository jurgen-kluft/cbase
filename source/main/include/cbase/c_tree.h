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
            void    set_color(u8 color);
            u8      get_color() const;
            void    set_child(u8 ne, node_t* set);
            node_t* get_child(u8 ne) const;

        private:
            node_t* m_child[2];
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

        typedef s8 (*compare_key_and_node_fn)(void const* key, node_t const* node);
        typedef s8 (*compare_node_and_node_fn)(node_t const* node1, node_t const* node2);
        typedef node_t* (*new_node_fn)(void* user_data);

        struct iterator_t
        {
            iterator_t()
                : m_it(nullptr)
                , m_stack(-1)
            {
            }

            bool              traverse(node_t const* root, s32 d, node_t const*& node);
            bool              preorder(node_t const* root, s32 d, node_t const*& node);
            bool              sortorder(node_t const* root, s32 d, node_t const*& node);
            bool              postorder(node_t const* root, s32 d, node_t const*& node);
            static inline s32 getdir(s32 compare) { return (compare + 1) >> 1; }

            node_t const* m_it;
            node_t const* m_stack_array[32];
            s32           m_stack;
        };

        bool clear(node_t*& root, node_t*& n);  // Repeatedly call 'clear' until true is returned
        bool find(node_t* root, void const* key, compare_key_and_node_fn comparer, node_t*& found);
        bool insert(node_t*& root, node_t* temp, void const* key, compare_key_and_node_fn comparer, new_node_fn new_node, void* user_data, node_t*& _inserted);
        bool remove(node_t*& root, node_t* temp, void const* key, compare_key_and_node_fn comparer, node_t*& removed_node);
        bool validate(node_t const* root, const char*& error_str, compare_node_and_node_fn comparer);
    }  // namespace ntree

}  // namespace ncore

#include "cbase/private/c_tree_internal.h"

#endif
