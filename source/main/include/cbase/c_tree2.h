#ifndef __CBASE_TREE2_H__
#define __CBASE_TREE2_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"

namespace ncore
{
    // binary balanced search tree implemented using the red-black tree algorithm
    namespace ntree2
    {
        class tree_t;
        struct node_t;

        enum node_e
        {
            LEFT  = 0,
            RIGHT = 1,
        };

        enum color_e
        {
            RED   = LEFT,
            BLACK = RIGHT
        };

        class tree_t
        {
        public:
            virtual ~tree_t() {}

            virtual s32         v_size() const                                                 = 0;
            virtual s32         v_capacity() const                                             = 0;
            virtual node_t*     v_get_root() const                                             = 0;
            virtual void        v_set_root(node_t* node)                                       = 0;
            virtual node_t*     v_get_temp() const                                             = 0;
            virtual void        v_set_color(node_t* node, color_e color)                       = 0;
            virtual color_e     v_get_color(node_t const* node) const                          = 0;
            virtual void const* v_get_key(node_t const* node) const                            = 0;
            virtual void const* v_get_value(node_t const* node) const                          = 0;
            virtual node_t*     v_get_node(node_t const* node, node_e ne) const                = 0;
            virtual void        v_set_node(node_t* node, node_e ne, node_t* set)               = 0;
            virtual node_t*     v_new_node(void const* key, void const* value)                 = 0;
            virtual void        v_del_node(node_t* node)                                       = 0;
            virtual s32         v_compare_nodes(node_t const* node, node_t const* other) const = 0;
            virtual s32         v_compare_insert(void const* data, node_t const* node) const   = 0;
        };

        struct iterator_t
        {
            bool              traverse(s32 d, void const*& data);
            bool              preorder(s32 d, void const*& data);
            bool              sortorder(s32 d, void const*& data);
            bool              postorder(s32 d, void const*& data);
            static inline s32 getdir(s32 compare) { return (compare + 1) >> 1; }

            tree_t* m_tree;
            node_t* m_it;
            node_t* m_stack_array[32];
            s32     m_stack;
        };

        inline int_t size(tree_t* c) { return c->v_size(); }
        inline int_t capacity(tree_t* c) { return c->v_capacity(); }
        bool         clear(tree_t* c);  // Repeatedly call 'clear' until true is returned
        bool         find(tree_t* c, void const* key, node_t*& found);
        bool         insert(tree_t* c, void const* key, void const* value = nullptr);
        bool         remove(tree_t* c, void const* key);
        bool         validate(tree_t* c, const char*& error_str);
        iterator_t   iterate(tree_t* c);

    }  // namespace ntree2

}  // namespace ncore

#endif
