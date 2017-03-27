// x_rbtree.h - xCore red-black tree
#ifndef __XBASE_REDBLACK_TREE_SENTINEL_PTR_H__
#define __XBASE_REDBLACK_TREE_SENTINEL_PTR_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_allocator.h"
#include "xbase\x_debug.h"

namespace xcore
{
	//////////////////////////////////////////////////////////////////////////
	// This C implementation works with a sentinel and has left/right/parent
	// https://www.cs.auckland.ac.nz/~jmor159/PLDS210/niemann/s_rbt.htm
	// http://c-f.spb.ru/files/Sources/rbt.c
	//////////////////////////////////////////////////////////////////////////

	struct xrbsnode;

	//////////////////////////////////////////////////////////////////////////

	typedef s32(*xrbsnode_cmp_f) (void* data, xrbsnode* n);


	//////////////////////////////////////////////////////////////////////////
	//
	// This red-black tree node uses pointers to reference nodes, this
	// results in a node sizeof 12 bytes in 32-bit and 24 bytes in 64-bit.
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
		inline void			clear()										{ parent = (xrbsnode*)RED; child[LEFT] = child[RIGHT] = NULL; }

		inline void			set_parent(xrbsnode* node)					{ parent = set_ptr(parent, node, COLOR_MASK); }
		inline xrbsnode*	get_parent() const							{ return get_ptr(parent, COLOR_MASK); }

		inline void			set_child(xrbsnode* node, s32 dir)			{ child[dir] = node; }
		inline xrbsnode*	get_child(s32 dir) const					{ return child[dir]; }

		inline void			set_right(xrbsnode* node)					{ child[RIGHT] = node; }
		inline xrbsnode* 	get_right() const							{ return child[RIGHT]; }

		inline void			set_left(xrbsnode* node)					{ child[LEFT] = node; }
		inline xrbsnode* 	get_left() const							{ return child[LEFT]; }

		inline s32			get_side(xrbsnode* node)					{ ASSERT(node==get_left() || node==get_right()); return child[RIGHT] == node ? RIGHT : LEFT; }

		inline void			set_red()									{ parent = set_bit(parent, COLOR_MASK, RED); }
		inline void			set_black()									{ parent = set_bit(parent, COLOR_MASK, BLACK); }
		inline void			set_color(s32 colr)							{ ASSERT(colr==RED || colr==BLACK); parent = set_bit(parent, COLOR_MASK, colr); }
		inline s32			get_color() const							{ s32 colr = (s32)get_bit(parent, COLOR_MASK); ASSERT(colr==RED || colr==BLACK); return colr; }
		inline bool			is_red() const								{ return get_bit(parent, COLOR_MASK) == RED; }
		inline bool			is_black() const							{ return get_bit(parent, COLOR_MASK) == BLACK; }

		XCORE_CLASS_PLACEMENT_NEW_DELETE

	private:
		xrbsnode			*parent;
		xrbsnode			*child[2];
	};

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	struct xrbstree
	{
		inline				xrbstree()									
		{ 
			m_sentinel.set_parent(NULL);
			m_sentinel.set_left(&m_sentinel);
			m_sentinel.set_right(&m_sentinel);
			m_sentinel.set_black();
			m_root = NULL;
			m_cmp_f = NULL;
		}

		void				init(xrbsnode_cmp_f cmp_f)
		{
			m_cmp_f = cmp_f;
		}

		bool				find(void* data, xrbsnode *& n) const;
		bool				insert(void* data, xrbsnode * n);
		bool				remove(xrbsnode * );

		xrbsnode*			clear(xrbsnode*& iterator);

		bool				test(const char*& result) const;

	private:
		xrbsnode			m_sentinel;
		xrbsnode			*m_root;
		xrbsnode_cmp_f		m_cmp_f;
		
		inline xrbsnode*	get_nill()									{ return &m_sentinel; }
		inline bool			is_nill(xrbsnode* n) const					{ return n == &m_sentinel; }

		void 				rotate_right(xrbsnode * x, xrbsnode *& p_root);
		void 				rotate_left(xrbsnode * x, xrbsnode*& p_root);

		void				insert_fixup(xrbsnode * x, xrbsnode *& p_root);
		void				remove_fixup(xrbsnode * x, xrbsnode * x_parent, xrbsnode *& p_root);

		void				insert_to_parent(xrbsnode * x, xrbsnode * parent, xrbsnode *& p_root);

		xrbsnode *			find(void * find, xrbsnode * p_root, xrbsnode *& p_insert_parent) const;
		void				remove(xrbsnode * z, xrbsnode *& p_root);
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
		if (!is_nill(x))
			x->set_parent( y );
	}

	//////////////////////////////////////////////////////////////////////////

	inline void xrbstree::insert_fixup(xrbsnode * x, xrbsnode *& p_root)
	{
		// check Red-Black properties
		while (x != p_root && x->get_parent()->is_red())
		{
			// we have a violation
			if (x->get_parent() == x->get_parent()->get_parent()->get_left())
			{
				xrbsnode * y = x->get_parent()->get_parent()->get_right();
				if (y->is_red())
				{
					// uncle is RED
					x->get_parent()->set_black();
					y->set_black();
					x->get_parent()->get_parent()->set_red();
					x = x->get_parent()->get_parent();
				}
				else
				{
					// uncle is BLACK
					if (x == x->get_parent()->get_right())
					{
						// make x a left child
						x = x->get_parent();
						rotate_left(x, p_root);
					}

					// recolor and rotate
					x->get_parent()->set_black();
					x->get_parent()->get_parent()->set_red();
					rotate_right(x->get_parent()->get_parent(), p_root);
				}
			}
			else
			{
				// mirror image of above code
				xrbsnode * y = x->get_parent()->get_parent()->get_left();
				if (y->is_red())
				{
					// uncle is RED
					x->get_parent()->set_black();
					y->set_black();
					x->get_parent()->get_parent()->set_red();
					x = x->get_parent()->get_parent();
				}
				else
				{
					// uncle is BLACK
					if (x == x->get_parent()->get_left())
					{
						x = x->get_parent();
						rotate_right(x, p_root);
					}
					x->get_parent()->set_black();
					x->get_parent()->get_parent()->set_red();
					rotate_left(x->get_parent()->get_parent(), p_root);
				}
			}
		}
		p_root->set_black();
	}

	inline void xrbstree::insert_to_parent(xrbsnode * x, xrbsnode * parent, xrbsnode *& p_root)
	{
		x->set_parent(parent);
		x->set_left(get_nill());
		x->set_right(get_nill());
		x->set_color(xrbsnode::RED);
		
		// insert node in tree
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

	inline void xrbstree::remove_fixup(xrbsnode * x, xrbsnode * x_parent, xrbsnode *& p_root)
	{
		while (x != p_root && x->is_black())
		{
			if (x == x_parent->get_left())
			{
				xrbsnode * w = x_parent->get_right();
				if (w->is_red())
				{
					w->set_black();
					x_parent->set_red();
					rotate_left(x_parent, p_root);
					w = x_parent->get_right();
				}
				if (w->get_left()->is_black() && w->get_right()->is_black())
				{
					w->set_red();
					x = x_parent;
					x_parent = x->get_parent();
				}
				else
				{
					if (w->get_right()->is_black())
					{
						w->get_left()->set_black();
						w->set_red();
						rotate_right(w, p_root);
						w = x_parent->get_right();
					}
					w->set_color(x_parent->get_color());
					x_parent->set_black();
					w->get_right()->set_black();
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
					w->set_black();
					x_parent->set_red();
					rotate_right(x_parent, p_root);
					w = x_parent->get_left();
				}
				if (w->get_right()->is_black() && w->get_left()->is_black())
				{
					w->set_red();
					x = x_parent;
					x_parent = x->get_parent();
				}
				else
				{
					if (w->get_left()->is_black())
					{
						w->get_right()->set_black();
						w->set_red();
						rotate_left (w, p_root);
						w = x_parent->get_left();
					}
					w->set_color(x_parent->get_color());
					x_parent->set_black();
					w->get_left()->set_black();
					rotate_right (x_parent, p_root);
					x = p_root;
					x_parent = NULL;
				}
			}
		}
		x->set_black();
	}

	inline void xrbstree::remove(xrbsnode * z, xrbsnode *& p_root)
	{
		if (!p_root)
			return;

		if (!z || is_nill(z))
			return;

		xrbsnode *x, *y, *x_parent;
		if (is_nill(z->get_left()) || is_nill(z->get_right()))
		{	// y has a NIL node as a child
			y = z;
		}
		else
		{	// find tree successor with a NIL node as a child
			y = z->get_right();
			while (!is_nill(y->get_left()))
				y = y->get_left();
		}

		// x is y's only child
		if (!is_nill(y->get_left()))
			x = y->get_left();
		else
			x = y->get_right();

		// remove y from the parent chain
		x_parent = y->get_parent();
		if (!is_nill(x))
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

		bool const f = (y->is_black());
		
		if (y != z)
		{   // replace z with y
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

	//////////////////////////////////////////////////////////////////////////

	inline xrbsnode * xrbstree::find(void * find, xrbsnode * p_root, xrbsnode *& p_insert_parent) const
	{
		if (p_root)
		{
			xrbsnode *current = p_root;
			xrbsnode *parent = NULL;
			while (!is_nill(current))
			{
				s32 c = m_cmp_f(find, current);
				if (c == 0)
					return (current);

				c = (c + 1) >> 1;
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


	//////////////////////////////////////////////////////////////////////////

	inline bool			xrbstree::find(void* data, xrbsnode *& n) const
	{
		xrbsnode* p;
		n = find(data, m_root, p);
		return n != NULL;
	}

	//////////////////////////////////////////////////////////////////////////

	inline bool			xrbstree::insert(void* data, xrbsnode * n)
	{
		ASSERT(m_cmp_f(data, n) == 0);

		xrbsnode* p;
		xrbsnode* f = find(data, m_root, p);
		if (f == NULL)
		{
			insert_to_parent(n, p, m_root);
			return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	inline bool			xrbstree::remove(xrbsnode * f)
	{
		if (f != NULL)
		{
			remove(f, m_root);
			return true;
		}
		return false;
	}

	inline xrbsnode*	xrbstree::clear(xrbsnode*& iterator)
	{
		//	Rotate away the left links so that
		//	we can treat this like the destruction
		//	of a linked list
		if (m_root == NULL && iterator == NULL)
			return NULL;

		if (iterator == NULL)
			iterator = m_root;

		xrbsnode* it = iterator;
		iterator = NULL;

		while (!is_nill(it))
		{
			if (is_nill(it->get_left()))
			{	// No left links, just kill the node and move on
				iterator = (xrbsnode*)it->get_right();
				it->clear();
				return it;
			}
			else
			{	// Rotate away the left link and check again
				iterator = it->get_left();
				it->set_left(iterator->get_right());
				iterator->set_right(it);
			}
			it = iterator;
			iterator = NULL;
		}
		m_root = NULL;
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////

	inline bool			xrbstree::test(const char*& result) const
	{
		result = NULL;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	struct rbs_iterator
	{
		inline		rbs_iterator(xrbsnode* nill) : m_nill(nill), m_node(NULL) {}

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
				m_node = root;
				// Save the path for later traversal
				while (m_node->get_child(dir) != m_nill)
				{
					m_node = m_node->get_child(dir);
				}
				result = m_node;
			}
			return result;
		}

		xrbsnode*	move(EType dir)
		{
			if (m_node->get_child(dir) != m_nill)
			{	// Continue down this branch
				m_node = m_node->get_child(dir);
				while (m_node->get_child(!dir) != m_nill)
				{
					m_node = m_node->get_child(!dir);
				}
			}
			else
			{	// Move to the next branch
				xrbsnode * last = NULL;
				do {
					last = m_node;
					m_node = m_node->get_parent();
				} while (last == m_node->get_child(dir));
			}
			return m_node;
		}

		xrbsnode*	m_nill;
		xrbsnode*	m_node;
	};

};


#endif	///< __XBASE_REDBLACK_TREE_SENTINEL_PTR_H__

