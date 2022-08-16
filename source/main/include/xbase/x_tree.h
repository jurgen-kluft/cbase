#ifndef __CBASE_TREE_H__
#define __CBASE_TREE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_allocator.h"

namespace ncore
{
    // binary balanced search tree implemented using the red-black tree algorithm
    // memory behaviour: 32 bytes per node
    class tree_t
    {
    public:
        tree_t();

        enum { LEFT = 0, RIGHT = 1, RED = 0, BLACK = 1 };

        struct node_t
        {
            inline			node_t() : data(0) { parent = link[0] = link[1] = nullptr; }

            void *			get_data() const { ptr_t p = data & ~(ptr_t)0x1; return (void*)p; }
            void			set_data(void * d) { data = (data & (ptr_t)0x1) | ((ptr_t)d & ~(ptr_t)0x1); }

            node_t *		get_parent() const { return parent; }

            void			set_parent(node_t * p) { parent = p; }
            node_t *		get_left() const { return link[LEFT]; }
            node_t *		get_right() const { return link[RIGHT]; }
            void			set_left(node_t * c) { link[0] = c; }
            void			set_right(node_t * c) { link[1] = c; }
            node_t *		get_child(s32 child) const { return link[child&1]; }
            void			set_child(s32 child, node_t*n) { link[child&1] = n; }

            bool			is_red() const { ptr_t p = data & (ptr_t)0x1; return p == RED; }
            bool			is_black() const { ptr_t p = data & (ptr_t)0x1; return p == BLACK; }
            void			set_red() { ptr_t p = data & (ptr_t)~0x1; data = p; }
            void			set_black() { ptr_t p = data | (ptr_t)0x1; data = p; }
            s32				get_color() const { s32 p = (s32)data & (s32)0x1; return p; }
            void			set_color(s32 c) { ptr_t p = data & (ptr_t)~0x1; data = p | ((ptr_t)c & 0x1); }

            s32				get_parent_side() const
            {
                node_t* parent = get_parent();
                if (parent->get_left() == this)
                    return LEFT;
                return RIGHT;
            }

        protected:
            friend class tree_t;

            ptr_t			data;		// User-defined content 
            node_t *		parent;		// Parent
            node_t *		link[2];	// Left (0) and right (1) links 
        };

        struct iterator
        {
            bool		traverse(s32 d, void *& data);
            bool		preorder(s32 d, void *& data);
            bool		sortorder(s32 d, void *& data);
            bool		postorder(s32 d, void *& data);
            inline s32	getdir(s32 compare) const { return (compare + 1) >> 1; }

        protected:
            friend class tree_t;

            tree_t *	m_tree;
            node_t *	m_it;
        };

        u32				size() const			{ return m_size;}
        bool			clear(void *& data, node_t*& node);	// Repeatedly call 'clear' until true is returned

        bool			find(void * data, node_t *& found);
        bool			insert(void * data, node_t * node);
        bool			remove(void * data, node_t*& removed_node);

        bool			validate(const char*& error_str);

        typedef s32(*compare_f) (void const* p1, void const* p2);
        void			set_cmp(compare_f func) { m_compare = func; }

        iterator		iterate();

    protected:
        friend class tree_internal;
        friend struct iterator;
        friend struct search;

        compare_f		m_compare;
        node_t			m_nill;
        node_t 			m_root;				// Top of the tree
        u32				m_size;				// Number of items (user-defined)
    };


}

#endif
