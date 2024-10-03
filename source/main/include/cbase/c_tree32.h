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
        typedef u32         index_t;
        typedef u32         node_t;
        typedef const void* key_t;
        typedef const void* value_t;

        const u32     c_invalid_index = 0xFFFFFFFF;
        const node_t  c_invalid_node  = 0xFFFFFFFF;
        const key_t   c_invalid_key   = nullptr;
        const value_t c_invalid_value = nullptr;

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

        typedef s8 (*compare_keys_fn)(void const* key1, void const* key2);

        struct tree_t
        {
            tree_t();

            s32     v_size() const;
            s32     v_capacity() const;
            node_t  v_get_root() const;
            void    v_set_root(node_t node);
            node_t  v_get_temp() const;
            void    v_set_color(node_t node, u8 color);
            u8      v_get_color(node_t const node) const;
            key_t   v_get_key(node_t const node) const;
            value_t v_get_value(node_t const node) const;
            node_t  v_get_node(node_t const node, s8 ne) const;
            void    v_set_node(node_t node, s8 ne, node_t set);
            node_t  v_new_node();
            void    v_del_node(node_t node);
            s32     v_compare_nodes(node_t const node, node_t const other) const;
            s32     v_compare_insert(key_t key, node_t const node) const;

            node_t m_root;  // root node is always index 0 of the node array
            node_t m_temp;  // temp node is always index 1 of the node array

            u32 m_num_nodes_current;
            u32 m_num_nodes_max;
            u32 m_nodes_free_index;
            u32 m_nodes_free_head;

            struct children_t
            {
                node_t m_child[2];
            };

            children_t*     m_children;
            u8*             m_colors;
            compare_keys_fn m_compare_keys;
            byte const*     m_keys_array;  // not owned
            u32             m_sizeof_key;
            u32             m_sizeof_value;
            byte const*     m_values_array;  // not owned
        };

        struct iterator_t
        {
            iterator_t(tree_t& tree)
                : m_tree(tree)
                , m_it(c_invalid_node)
                , m_stack(0)
            {
            }

            bool              traverse(s32 d, key_t& key, value_t& value);
            bool              preorder(s32 d, key_t& key, value_t& value);
            bool              sortorder(s32 d, key_t& key, value_t& value);
            bool              postorder(s32 d, key_t& key, value_t& value);
            static inline s32 getdir(s32 compare) { return (compare + 1) >> 1; }

            tree_t& m_tree;
            node_t  m_it;
            node_t  m_stack_array[32];
            s32     m_stack;
        };

        void create_tree(alloc_t* allocator, tree_t& c, u32 max_nodes, compare_keys_fn comparer, void const* keys_array, u32 sizeof_key, void const* values_array, u32 sizeof_value);
        void destroy_tree(alloc_t* allocator, tree_t& c);

        inline int_t size(tree_t& c) { return c.v_size(); }
        inline int_t capacity(tree_t& c) { return c.v_capacity(); }

        bool       clear(tree_t& c, node_t& n);  // Repeatedly call 'clear' until true is returned
        bool       find(tree_t const& c, key_t key, node_t& found);
        bool       insert(tree_t& c, node_t new_node);
        bool       remove(tree_t& c, key_t key, node_t& removed);
        bool       validate(tree_t& c, const char*& error_str);
        iterator_t iterate(tree_t& c);

    }  // namespace ntree32

}  // namespace ncore

#include "cbase/private/c_tree32_internal.h"

#endif
