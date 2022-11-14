#ifndef __CBASE_TREE_H__
#define __CBASE_TREE_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_allocator.h"

namespace ncore
{
    // binary balanced search tree implemented using the red-black tree algorithm
    class tree_t
    {
    public:
        class ctxt_t;
        struct node_t;

        tree_t();
        
        void init(ctxt_t* ctxt);
        void exit();

        enum node_e
        {
            LEFT   = 0,
            RIGHT  = 1,
            PARENT = 2,
        };
        enum color_e
        {
            RED   = LEFT,
            BLACK = RIGHT
        };

        class ctxt_t
        {
        public:
            virtual ~ctxt_t() {}
            virtual s32         v_size() const                                                 = 0;
            virtual s32         v_capacity() const                                             = 0;
            virtual void        v_set_color(node_t* node, color_e color)                       = 0;
            virtual color_e     v_get_color(node_t const* node) const                          = 0;
            virtual void const* v_get_key(node_t const* node) const                            = 0;
            virtual void const* v_get_value(node_t const* node) const                          = 0;
            virtual node_t*     v_get_node(node_t const* node, node_e ne) const                = 0;
            virtual void        v_set_node(node_t* node, node_e ne, node_t* set) const         = 0;
            virtual node_t*     v_new_node(void const* key, void const* value)                 = 0;
            virtual void        v_del_node(node_t* node)                                       = 0;
            virtual s32         v_compare_nodes(node_t const* node, node_t const* other) const = 0;
            virtual s32         v_compare_insert(void const* data, node_t const* node) const   = 0;
        };

        struct node_t
        {
            inline void const* get_key(ctxt_t* c) const { return c->v_get_key(this); }
            inline node_t*     get_parent(ctxt_t* c) const { return c->v_get_node(this, PARENT); }
            inline void        set_parent(ctxt_t* c, node_t* p) { c->v_set_node(this, PARENT, p); }
            inline node_t*     get_left(ctxt_t* c) const { return c->v_get_node(this, LEFT); }
            inline node_t*     get_right(ctxt_t* c) const { return c->v_get_node(this, RIGHT); }
            inline void        set_left(ctxt_t* c, node_t* child) { c->v_set_node(this, LEFT, child); }
            inline void        set_right(ctxt_t* c, node_t* child) { c->v_set_node(this, RIGHT, child); }
            inline node_t*     get_child(ctxt_t* c, s32 child) const { return c->v_get_node(this, (node_e)child); }
            inline void        set_child(ctxt_t* c, s32 child, node_t* n) { c->v_set_node(this, (node_e)child, n); }

            inline bool is_red(ctxt_t* c) const { return c->v_get_color(this) == RED; }
            inline bool is_black(ctxt_t* c) const { return c->v_get_color(this) == BLACK; }
            inline void set_red(ctxt_t* c) { c->v_set_color(this, RED); }
            inline void set_black(ctxt_t* c) { c->v_set_color(this, BLACK); }
            inline s32  get_color(ctxt_t* c) const { return c->v_get_color(this); }
            inline void set_color(ctxt_t* c, s32 color) { c->v_set_color(this, (color_e)color); }

            inline s32 get_parent_side(ctxt_t* c) const
            {
                node_t* parent = get_parent(c);
                if (parent->get_left(c) == this)
                    return LEFT;
                return RIGHT;
            }
        };

        struct iterator_t
        {
            bool       traverse(s32 d, void const*& data);
            bool       preorder(s32 d, void const*& data);
            bool       sortorder(s32 d, void const*& data);
            bool       postorder(s32 d, void const*& data);
            inline s32 getdir(s32 compare) const { return (compare + 1) >> 1; }

        protected:
            friend class tree_t;

            tree_t* m_tree;
            node_t* m_it;
        };

        inline int_t size() const { return m_ctxt->v_size(); }
        inline int_t capacity() const { return m_ctxt->v_capacity(); }

        bool clear(void const*& key); // Repeatedly call 'clear' until true is returned
        bool find(void const* key, node_t*& found) const;
        bool insert(void const* key, void const* value=nullptr);
        bool remove(void const* key);

        bool validate(const char*& error_str);

        iterator_t iterate();

    protected:
        friend class tree_internal_t;
        friend struct iterator_t;
        friend struct search;

        ctxt_t* m_ctxt;
        node_t* m_nill;
        node_t* m_root;
    };

} // namespace ncore

#endif
