// x_rbtree.h - xCore red-black tree
#ifndef __XBASE_REDBLACK_TREE_PTR_H__
#define __XBASE_REDBLACK_TREE_PTR_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_allocator.h"
#include "xbase\x_debug.h"

namespace xcore
{
	// This C implementation works with a sentinel and has left/right/parent
	// https://www.cs.auckland.ac.nz/~jmor159/PLDS210/niemann/s_rbt.htm
	// http://c-f.spb.ru/files/Sources/rbt.c

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	typedef s32(*xrbsnode_cmp_f) (xrbsnode* a, xrbsnode* b);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	struct xrbstree
	{
		inline				xrbstree()									{ sentinel.clear(); sentinel.set_black(); nill = &sentinel; }

		void				init(xrbsnode_cmp_f cmp_f);

		bool				find(xrbsnode * ) const;
		bool				insert(xrbsnode * );
		bool				remove(xrbsnode * );

	private:
		xrbsnode			m_sentinel;
		xrbsnode			*m_root;
		xrbsnode_cmp_f		m_cmp_f;
		
		inline bool			is_nill(xrbsnode* n) const					{ return n == &m_sentinel; }

		void 				rotate_right(xrbsnode * x, xrbsnode *& p_root);
		void 				rotate_left(xrbsnode * x, xrbsnode*& p_root);

		void				insert_fixup(xrbsnode * x, xrbsnode *& p_root);
		void				remove_fixup(xrbsnode * x, xrbsnode * x_parent, xrbsnode *& p_root);

		void				insert_to_parent(xrbsnode * x, xrbsnode * parent, xrbsnode *& p_root);

		xrbsnode *			find(xrbsnode * find, xrbsnode *& p_root, xrbsnode *& p_insert_parent) const;
		void				remove(xrbsnode * z, xrbsnode *& p_root);
	};

	//
	// This red-black tree node uses pointers to reference nodes, this
	// results in a node sizeof 8 bytes in 32-bit and 16 bytes in 64-bit.
	//
	struct xrbsnode
	{
	public:
		enum ESide	{ LEFT  = 0x0, RIGHT = 0x1 };
		enum EColor	{ RED   = 0x0, BLACK = 0x1, COLOR_MASK = 0x1 };

	private:
		static inline xrbsnode*	get_ptr(xrbsnode* p, uptr m)					{ return (xrbsnode*)((uptr)p & ~m); }
		static inline xrbsnode*	set_ptr(xrbsnode* p, xrbsnode* n, uptr m)		{ return (xrbsnode*)(((uptr)p & m) | ((uptr)n & ~m)); }

		static inline uptr		get_bit(xrbsnode* p, uptr b)		 			{ return (uptr)p & b;}
		static inline xrbsnode*	set_bit(xrbsnode* p, uptr m, uptr b)			{ return (xrbsnode*)(((uptr)p & ~m) | b); }

	public:
		inline void			clear()										{ parent = (xrbsnode*)RED; left = right = NULL; }

		inline void			set_parent(xrbsnode* node)					{ parent = set_ptr(parent, node, COLOR_MASK); }
		inline xrbsnode*	get_parent() const							{ return get_ptr(parent); }

		inline void			set_child(xrbsnode* node, s32 dir)			{ child[dir] = node; }
		inline xrbsnode*	get_child(s32 dir) const					{ return child[dir]; }

		inline void			set_right(xrbsnode* node)					{ right = node; }
		inline xrbsnode* 	get_right() const							{ return right; }

		inline void			set_left(xrbsnode* node)					{ left = set_ptr(left, node, COLOR_MASK); }
		inline xrbsnode* 	get_left() const							{ return get_ptr(left, COLOR_MASK); }

		inline s32			get_side(xrbsnode* node)					{ ASSERT(node==get_left() || node==get_right()); return right == node ? RIGHT : LEFT; }

		inline void			set_red()									{ parent = set_bit(parent, COLOR_MASK, RED); }
		inline void			set_black()									{ parent = set_bit(parent, COLOR_MASK, BLACK); }
		inline void			set_color(s32 colr)							{ ASSERT(colr==RED || colr==BLACK); parent = set_bit(parent, COLOR_MASK, colr); }
		inline s32			get_color() const							{ s32 colr = (s32)get_bit(parent, COLOR_MASK); ASSERT(colr==RED || colr==BLACK); return colr; }
		inline bool			is_red() const								{ return get_bit(parent, COLOR_MASK) == RED; }
		inline bool			is_black() const							{ return get_bit(parent, COLOR_MASK) == BLACK; }

		XCORE_CLASS_PLACEMENT_NEW_DELETE

	private:
		xrbsnode*			*parent;
		xrbsnode			*left, *right;
	};

	struct xmrbsnode : public xrbsnode
	{
		void				clear()										{ siblings = NULL; xrbsnode::clear(); }

		xmrbsnode*			get_siblings() const						{ return siblings; }

		void				insert_sibling(xmrbsnode* sib)
		{
			if (siblings == NULL) siblings = sib;
			else
			{
				xmrbsnode* next = siblings;
				xmrbsnode* prev = (xmrbsnode*)next->get_child(LEFT);
				sib->set_child(prev, LEFT);
				sib->set_child(next, RIGHT);
				prev->set_child(sib, RIGHT);
				next->set_child(sib, LEFT);
			}
		}

		void				remove_sibling(xmrbsnode* sib)
		{
			xmrbsnode* next = (xmrbsnode*)sib->get_child(RIGHT);
			xmrbsnode* prev = (xmrbsnode*)sib->get_child(LEFT);
			next->set_child(prev, LEFT);
			prev->set_child(next, RIGHT);
			if (siblings == sib && next != sib)
				siblings = next;
			else
				siblings = NULL;
		}

	protected:
		xmrbsnode*		siblings;
	};

	inline s32 rbs_flip_dir(s32 dir)
	{
		ASSERT(xrbsnode::RIGHT == 1);
		ASSERT(dir == xrbsnode::RIGHT || dir == xrbsnode::LEFT);
		return dir ^ xrbsnode::RIGHT;
	}

	inline bool	rbs_is_red(xrbsnode* node)
	{
		return node != NULL ? node->is_red() : xrbsnode::BLACK;
	}

	


	//////////////////////////////////////////////////////////////////////////

	inline void xrbstree::rotate_left(xrbsnode * x, xrbsnode*& p_root)
	{
		xrbsnode *y = x->get_right();

		// establish x->get_right() link
		xrbsnode *yl = y->get_left();
		x->set_right( yl );
		if (!is_nill(yl)) 
			yl->set_parent(x);

		// establish y->get_parent() link
		xrbsnode *xp = x->get_parent();
		if (!is_nill(y)) 
			y->set_parent( xp );

		if (xp)
		{
			s32 s = xp->get_side(x);
			xp->set_child(y, s);
		}
		else
		{
			p_root = y;
		}

		// link x and y
		y->set_left(x);
		if (!is_nill(x))
			x->set_parent( y );
	}

	inline void xrbstree::rotate_right(xrbsnode * x, xrbsnode *& p_root)
	{
		xrbsnode * y = x->get_left();

		// establish x->get_left() link
		xrbsnode* yr = y->get_right();
		x->set_left( yr );
		if (!is_nill(yr))
			yr->set_parent( x );

		// establish y->get_parent() link 
		xrbsnode* xp = x->get_parent();
		if (!is_nill(y))
			y->set_parent( xp );

		if (xp)
		{
			s32 s = xp->get_side(x);
			xp->set_child(y, s);
		}
		else
		{
			p_root = y;
		}

		// link x and y 
		y->set_right( x );
		if (is_nill(x))
			x->set_parent( y );
	}

	//////////////////////////////////////////////////////////////////////////

	void xrbstree::insert_fixup(xrbsnode * x, xrbsnode *& p_root)
	{
		/* check Red-Black properties */
		while (x != p_root && x->get_parent()->get_color() == RED)
		{
			/* we have a violation */
			if (x->get_parent() == x->get_parent()->get_parent()->get_left())
			{
				xrbsnode * y = x->get_parent()->get_parent()->get_right();
				if (y->get_color() == RED)
				{
					/* uncle is RED */
					x->get_parent()->set_color(BLACK);
					y->set_color(BLACK);
					x->get_parent()->get_parent()->set_color(RED);
					x = x->get_parent()->get_parent();
				}
				else
				{
					/* uncle is BLACK */
					if (x == x->get_parent()->get_right())
					{
						/* make x a left child */
						x = x->get_parent();
						rotate_left(x, p_root);
					}

					/* recolor and rotate */
					x->get_parent()->set_color(BLACK);
					x->get_parent()->get_parent()->set_color(RED);
					rotate_right(x->get_parent()->get_parent(), p_root);
				}
			}
			else
			{
				/* mirror image of above code */
				xrbsnode * y = x->get_parent()->get_parent()->get_left();
				if (y->get_color() == RED)
				{
					/* uncle is RED */
					x->get_parent()->set_color(BLACK);
					y->set_color(BLACK);
					x->get_parent()->get_parent()->set_color(RED);
					x = x->get_parent()->get_parent();
				}
				else
				{
					/* uncle is BLACK */
					if (x == x->get_parent()->get_left())
					{
						x = x->get_parent();
						rotate_right(x, p_root);
					}
					x->get_parent()->set_color(BLACK);
					x->get_parent()->get_parent()->set_color(RED);
					rotateLeft(x->get_parent()->get_parent(), p_root);
				}
			}
		}
		p_root->set_color(BLACK);
	}

	void xrbstree::insert_to_parent(xrbsnode * x, xrbsnode * parent, xrbsnode *& p_root)
	{
		x->set_parent(parent);
		x->set_left(NILL);
		x->set_right(NILL);
		x->set_color(RED);
		
		/* insert node in tree */
		if (parent)
		{
			s32 c = m_cmp_f(x, parent);
			c = (c+1) >> 1;
			parent->set_child(x, c);
		}
		else
		{
			p_root = x;
		}

		insert_fixup(x, p_root);
	}

	//////////////////////////////////////////////////////////////////////////

	xrbsnode * xrbstree::find(xrbsnode * find, xrbsnode *& p_root, xrbsnode *& p_insert_parent) const
	{
		if (p_root)
		{   
			xrbsnode *current = *p_root;
			xrbsnode *parent = NULL;
			while (current != NILL)
			{
				s32 const c = m_cmp_f(find, current);
				if (c == 0)
					return (current);

				c = (c+1) >> 1;
				ASSERT(xrbsnode::LEFT == 0);
				ASSERT(xrbsnode::RIGHT == 1);
				ASSERT(xrbsnode::RIGHT == c || xrbsnode::LEFT == c);
				parent = current;
				current = current->get_child(c);
			}
		
			p_insert_parent = parent;
			return(NULL);
		}
		else
		{   
			p_insert_parent = NULL;
			return(NULL);
		}
	}


	void xrbstree::remove_fixup(xrbsnode * x, xrbsnode * x_parent, xrbsnode *& p_root)
	{
		while (x != p_root && x->is_black())
		{
			if (x == x_parent->get_left())
			{
				xrbsnode * w = x_parent->get_right();
				if (w->is_red())
				{
					w->set_color(BLACK);
					x_parent->set_color(RED);
					rotate_left(x_parent, p_root);
					w = x_parent->get_right();
				}
				if (w->get_left()->is_black() && w->get_right()->is_black())
				{
					w->set_color(RED);
					x = x_parent;
					x_parent = x->get_parent();
				}
				else
				{
					if (w->get_right()->is_black())
					{
						w->get_left()->set_color(BLACK);
						w->set_color(RED);
						rotate_right(w, p_root);
						w = x_parent->get_right();
					}
					w->set_color(x_parent->get_color());
					x_parent->set_color(BLACK);
					w->get_right()->set_color(BLACK);
					rotate_left(x_parent, p_root);
					x = p_root;
					x_parent = NULL;
				}
			}
			else
			{
				xrbsnode * w = x_parent->get_left();
				if (w->is_red())
				{
					w->set_color(BLACK);
					x_parent->set_color(RED);
					rotate_right(x_parent, p_root);
					w = x_parent->get_left();
				}
				if (w->get_right()->is_black() && w->get_left()->is_black())
				{
					w->set_color(RED);
					x = x_parent;
					x_parent = x->get_parent();

				}
				else
				{
					if (w->get_left()->is_black())
					{
						w->get_right()->set_color(BLACK);
						w->set_color(RED);
						rotate_left (w, p_root);
						w = x_parent->get_left();
					}
					w->set_color(x_parent->get_color());
					x_parent->set_color(BLACK);
					w->get_left()->set_color(BLACK);
					rotate_right (x_parent, p_root);
					x = p_root;
					x_parent = NULL;
				}
			}
		}
		x->set_color(BLACK);
	}

	void xrbstree::remove(xrbsnode * z, xrbsnode *& p_root)
	{
		if (!p_root)
			return;

		if (!z || is_nill(z))
			return;

		xrbsnode *x, *y, *x_parent;
		if (is_nill(z->get_left()) || is_nill(z->get_right()))
		{
			/* y has a NIL node as a child */
			y = z;
		}
		else
		{
			/* find tree successor with a NIL node as a child */
			y = z->get_right();
			while (!is_nill(y->get_left()))
				y = y->get_left();
		}

		/* x is y's only child */
		if (!is_nill(y->get_left()))
			x = y->get_left();
		else
			x = y->get_right();

		/* remove y from the parent chain */
		x_parent = y->get_parent();
		if(!is_nill(x))
			x->set_parent(x_parent);
			
		if (y->get_parent())
		{
			s32 s = y->get_parent()->get_side(y);
			y->get_parent()->set_child(x, s);
		}
		else
		{
			p_root = x;
		}

		bool const f = (y->get_color() == BLACK);
		
		if (y != z)
		{   /* replace z with y */ 
			y->set_color(z->get_color());
			y->set_left(z->get_left());
			y->set_right(z->get_right());
			y->set_parent(z->get_parent());

			if (y->get_parent())
			{
				s32 s = y->get_parent()->get_side(z);
				y->get_parent()->set_child(y, s);
			}
			else
			{
				p_root = y;
			}

			if (!is_nill(y->get_left()))
			{
				y->get_left()->set_parent(y);
			}

			if (!is_nill(y->get_right()))
			{
				y->get_right()->set_parent(y);
			}

			if (x_parent == z)
				x_parent = y;
		}

		if (f)
			remove_fixup(x, x_parent, p_root);
	}

struct rbs_iterator
	{
		inline		rbs_iterator() : node(NULL), top(0) {}
		inline		rbs_iterator(xrbsnode* n) : node(n), top(0) {}

		enum EType
		{
			FORWARDS = 1,
			BACKWARDS = 0,
			MINIMUM = 0,
			MAXIMUM = 1,
			MAX_HEIGHT = 64
		};

		xrbsnode*	init(xrbsnode * root, EType dir)
		{
			xrbsnode * result = NULL;
			if (root)
			{
				node = root;
				top = 0;

				if (node != NULL)
				{	// Save the path for later traversal
					while (node->get_child(dir) != NULL)
					{
						push(node);
						node = node->get_child(dir);
					}
				}
				result = node;
			}
			return result;
		}

		xrbsnode*	move(EType dir)
		{
			if (node->get_child(dir) != NULL)
			{	// Continue down this branch
				push(node);
				node = node->get_child(dir);
				while (node->get_child(!dir) != NULL)
				{
					push(node);
					node = node->get_child(!dir);
				}
			}
			else
			{	// Move to the next branch
				xrbsnode * last = NULL;
				do {
					if (top == 0)
					{
						node = NULL;
						break;
					}
					last = node;
					node = pop();
				} while (last == node->get_child(dir));
			}
			return node;
		}

		void		push(xrbsnode* node)		{ ASSERT(top < MAX_HEIGHT); path[top++] = node; }
		xrbsnode*	pop()						{ ASSERT(top > 0); return path[--top]; }

		xrbsnode*	node;
		s32			top;
		xrbsnode*	path[MAX_HEIGHT];
	};

};


#endif	///< __XBASE_REDBLACK_TREE_H__

