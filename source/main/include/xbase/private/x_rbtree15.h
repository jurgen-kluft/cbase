// x_rbtree15.h - xCore red-black tree
#ifndef __XBASE_REDBLACK_TREE_15_H__
#define __XBASE_REDBLACK_TREE_15_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_debug.h"
#include "xbase\x_idx_allocator.h"

namespace xcore
{
	//
	// This red-black tree node uses 16 bit indices to reference nodes, this
	// allows us to compress one node into 8 bytes. This node is also capable
	// of managing siblings (nodes with the same key).
	//
	// Maximum number of nodes is 65535
	//
	// Siblings are managed as a double linked list using the 'left' and 'right'
	// child as 'prev' and 'next'. The 'parent' will point to the node that is
	// in the tree and thus contains the index to the head.
	// The double linked list is organized as a ring.
	//
	// sizeof(xrbnode15) == 8 bytes
	// sizeof(xrbsnode15) == 12 bytes
	//
	struct xrbnode15_base
	{
	public:
		enum ESide { LEFT = 0x0, RIGHT = 0x1, SIDE_MASK = 0x1 };
		enum EColor { BLACK = 0x0, RED = 0x2, COLOR_MASK = 0x2 };

	protected:
		static inline s32	get_side(u16 member)				{ return ((member & SIDE_MASK) == RIGHT) ? RIGHT : LEFT; }
		static inline void	set_side(u16& member, ESide side)	{ member = (member & ~SIDE_MASK) | side; }
		static inline s32	get_color(u16 member)				{ return ((member & COLOR_MASK) == RED) ? RED : BLACK; }
		static inline void	set_color(u16& member, EColor color) { member = (member & ~COLOR_MASK) | color; }

	public:
		inline void			set_child(u16 node, s32 dir)		{ child[dir] = node; }
		inline u16			get_child(s32 dir) const			{ return child[dir]; }

		inline void			set_right(u16 node)					{ child[RIGHT] = node; }
		inline u16			get_right() const					{ return child[RIGHT]; }

		inline void			set_left(u16 node)					{ child[LEFT] = node; }
		inline u16			get_left() const					{ return child[LEFT]; }

		inline void			set_parent(u16 node)				{ parent = node; }
		inline u16			get_parent() const					{ return parent; }

		inline void			set_parent_side(s32 side)			{ ASSERT(side == LEFT || side == RIGHT); set_side(flags, (ESide)side); }
		inline s32			get_parent_side() const				{ return get_side(flags); }

		inline void			set_red()							{ set_color(flags, RED); }
		inline void			set_black()							{ set_color(flags, BLACK); }
		inline void			set_color(s32 colr)					{ ASSERT(colr == RED || colr == BLACK); set_color(flags, (EColor)colr); }
		inline s32			get_color() const					{ s32 colr = get_color(flags); ASSERT(colr == RED || colr == BLACK); return colr; }

		inline bool			is_red() const						{ return get_color(flags) == RED; }
		inline bool			is_black() const					{ return get_color(flags) == BLACK; }

		inline bool			is_nill(u16 nill) const				{ return child[RIGHT] == nill; }

	protected:
		u16					flags;
		u16					parent;
		u16					child[2];
	};

	struct xrbnode15 : public xrbnode15_base
	{
	public:
		inline void			clear(u16 nill)								{ flags = 0; parent = child[LEFT] = child[RIGHT] = nill; }

		static inline
		xrbnode15*			to_ptr(x_indexer* a, u16 i)					{ return (xrbnode15*)a->to_ptr(i); }
	};

	struct xrbsnode15 : public xrbnode15_base
	{
		inline void			clear(u16 nill) { flags = 0; parent = sibling = child[LEFT] = child[RIGHT] = nill; }

		static inline
		xrbsnode15*			to_ptr(x_indexer* a, u16 i)					{ return (xrbsnode15*)a->to_ptr(i); }

		inline bool			has_sibling(u16 _nill) const				{ return sibling != _nill; }
		inline void			set_sibling(u16 s)							{ sibling = s; }
		inline u16			get_sibling() const							{ return sibling; }
		inline bool			is_sibling(u16 _nill) const 
		{ 
			return (child[LEFT] != _nill) && (parent == child[LEFT]) && (child[RIGHT] == sibling); 
		}

		void				insert_sibling(u16 _this, u16 _sib, u16 _nill, x_indexer* a)
		{
			xrbsnode15* sibp = (xrbsnode15*)a->to_ptr(_sib);

			if (get_sibling() == _nill)
			{
				set_sibling(_sib);
				sibp->set_left(_sib);
				sibp->set_parent(_sib);
				sibp->set_right(_sib);
				sibp->set_sibling(_sib);
			}
			else
			{
				u16 const next = get_sibling();
				xrbsnode15* nextp = (xrbsnode15*)a->to_ptr(next);
				u16 const prev = nextp->get_left();
				xrbsnode15* prevp = (xrbsnode15*)a->to_ptr(prev);

				sibp->set_left(prev);
				sibp->set_parent(prev);
				sibp->set_right(next);
				sibp->set_sibling(next);

				prevp->set_right(_sib);
				prevp->set_sibling(_sib);

				nextp->set_left(_sib);
				nextp->set_parent(_sib);
			}
		}

		void				remove_sibling(u16 sib, u16 nill, x_indexer* a)
		{
			xrbsnode15* sibp = (xrbsnode15*)a->to_ptr(sib);
			ASSERT(sibp->is_sibling(nill));

			u16 const next = sibp->get_right();
			if (next != sib)
			{
				xrbsnode15* nextp = (xrbsnode15*)a->to_ptr(next);
				u16 const prev = sibp->get_left();
				xrbsnode15* prevp = (xrbsnode15*)a->to_ptr(prev);
				nextp->set_left(prev);
				nextp->set_parent(prev);
				prevp->set_right(next);
				prevp->set_sibling(next);
				set_sibling(next);
			}
			else
			{
				set_sibling(nill);
			}

			sibp->clear(nill);
		}

		u16	sibling;
	};

	inline u16	rb15_minimum(u16 root, x_indexer* a)
	{
		xrbnode15* rootp     = (xrbnode15*)xrbnode15::to_ptr(a, root);
	
		u16	       node      = rootp->get_left();
		u16	       lastNode  = node;
		while (node != root)
		{
			lastNode  = node;
			xrbnode15* nodep = (xrbnode15*)xrbnode15::to_ptr(a, node);
			node      = nodep->get_left();
		};
		return lastNode;
	}

	inline u16	rb15_maximum(u16 root, x_indexer* a)
	{
		xrbnode15* rootp     = (xrbnode15*)xrbnode15::to_ptr(a, root);

		u16	       node      = rootp->get_left();
		u16	       lastNode  = node;
		while (node != root)
		{
			lastNode  = node;
			xrbnode15* nodep = (xrbnode15*)xrbnode15::to_ptr(a, node);
			node      = nodep->get_right();
		};
		return lastNode;
	}

	// Before using this you need to either traverse to the 
	// minimum (far left) or maximum (far right).
	// 0 = successor
	// 1 = predecessor
	inline u16	rb15_inorder(s32 dir, u16 node, u16 nill, x_indexer* a)
	{
		xrbnode15* nodep = xrbnode15::to_ptr(a, node);
		if (nodep->is_nill(node))
			return node;

		u16 next = nodep->get_child(1-dir);
		xrbnode15* nextp = xrbnode15::to_ptr(a, next);
		if (nextp->is_nill(next))
		{
			if (nodep->get_parent_side() != dir)
			{
				while (nodep->get_parent_side() != dir)
				{
					node = nodep->get_parent();
					nodep = xrbnode15::to_ptr(a, node);
				}
				node = nodep->get_parent();
			}
			else
			{
				node = nodep->get_parent();
			}
		}
		else
		{
			do {
				node = next;
				next = nextp->get_child(dir);
				nextp = xrbnode15::to_ptr(a, next);
			} while (!nextp->is_nill(next));
		}
		return node;
	}


	#ifdef TARGET_TEST
		#define DEBUG_RB15TREE
	#endif

#ifdef DEBUG_RB15TREE
	inline
	u32				rb15_check_height(u16 nill, u16 node, x_indexer* a)
	{
		if (node == nill)
			return 0;
			
		xrbnode15* nodep = xrbnode15::to_ptr(a, node);
		
		u16 left  = nodep->get_child(xrbnode15::LEFT );
		u16 right = nodep->get_child(xrbnode15::RIGHT);
		if (nodep->is_black())
			return rb15_check_height(nill, left, a) + rb15_check_height(nill, right, a) + 1;

		xrbnode15* leftp  = xrbnode15::to_ptr(a, left);
		xrbnode15* rightp = xrbnode15::to_ptr(a, right);
		ASSERT(leftp->is_black() && rightp->is_black());
		xcore::u32 lh = rb15_check_height(nill, left , a);
		xcore::u32 rh = rb15_check_height(nill, right, a);
		ASSERT(lh == rh);
		return lh;
	}

	inline
	void			rb15_check(u16 root, x_indexer* a)
	{
		xrbnode15* rootp = xrbnode15::to_ptr(a, root);

		u16 left  = rootp->get_left();
		u16 right = rootp->get_right();

		ASSERT(rootp->is_nill(root));
		ASSERT(rootp->is_black());
		ASSERT(right == root);
		xrbnode15* leftp = xrbnode15::to_ptr(a, left);
		ASSERT(left == root || leftp->is_black());
		rb15_check_height(root, left, a);
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	inline
	void			rb15_rotate(u16 node, s32 s, x_indexer* a)
	{
		xrbnode15* nodep = xrbnode15::to_ptr(a, node);
		u16 parent = nodep->get_parent();
		xrbnode15* parentp = xrbnode15::to_ptr(a, parent);
		ASSERT(parentp->get_child(nodep->get_parent_side()) == node);
		s32 o = (1 - s);
		ASSERT(o == xrbnode15::LEFT || o == xrbnode15::RIGHT);
		s32 ps = nodep->get_parent_side();
		u16 top = nodep->get_child(o);
		xrbnode15* topp = xrbnode15::to_ptr(a, top);
		u16 top_child = topp->get_child(s);
		xrbnode15* top_childp = xrbnode15::to_ptr(a, top_child);
		nodep->set_child(top_child, o);
		top_childp->set_parent(node);
		top_childp->set_parent_side(o);
		topp->set_parent(parent);
		topp->set_parent_side(ps);
		parentp->set_child(top, ps);
		topp->set_child(node, s);
		nodep->set_parent(top);
		nodep->set_parent_side(s);
	}

	inline
	void			rb15_insert_fixup(u16 head, u16 node, x_indexer* a) 
	{
		u16 cur = node;
		xrbnode15* curp = xrbnode15::to_ptr(a, cur);
		u16 parent = curp->get_parent();
		xrbnode15* parentp = xrbnode15::to_ptr(a, parent);
		while (parentp->is_red()) 
		{
			u16 gp = parentp->get_parent();
			xrbnode15* gpp = xrbnode15::to_ptr(a, gp);
			ASSERT(gp != head);
			s32 s = parentp->get_parent_side();
			s32 o = (1 - s);
			ASSERT(s == xrbnode15::LEFT || s == xrbnode15::RIGHT);
			ASSERT(o == xrbnode15::LEFT || o == xrbnode15::RIGHT);
			u16 gp_right = gpp->get_child(o);
			xrbnode15* pp_right = xrbnode15::to_ptr(a, gp_right);
			if (pp_right->is_red())
			{
				parentp->set_color(xrbnode15::BLACK);
				pp_right->set_color(xrbnode15::BLACK);
				gpp->set_color(xrbnode15::RED);
				cur = gp;
				curp = gpp;
				parent = curp->get_parent();
				parentp = xrbnode15::to_ptr(a, parent);
			} 
			else 
			{
				if (cur == parentp->get_child(o))
				{
					cur = parent;
					curp = parentp;
					rb15_rotate(cur, s, a);
					parent = curp->get_parent();
					parentp = xrbnode15::to_ptr(a, parent);
				}
				parentp->set_color(xrbnode15::BLACK);
				gpp->set_color(xrbnode15::RED);
				rb15_rotate(gp, o, a);
			} 
		}
		xrbnode15* headp = xrbnode15::to_ptr(a, head);
		u16 head_left = headp->get_child(xrbnode15::LEFT);
		xrbnode15* head_leftp = xrbnode15::to_ptr(a, head_left);
		head_leftp->set_color(xrbnode15::BLACK);
	}

	inline
	void			rb15_erase_fixup(u16 root, u16 node, x_indexer* a)
	{
		u16 head = root;
		xrbnode15* headp = xrbnode15::to_ptr(a, root);

		u16 cur = node;
		xrbnode15* curp = xrbnode15::to_ptr(a, cur);
		while (!curp->is_red() && cur != headp->get_child(xrbnode15::LEFT)) 
		{
			u16 parent = curp->get_parent();
			xrbnode15* parentp = xrbnode15::to_ptr(a, parent);

			ASSERT(parentp != headp && parent != head);
			s32 const s = curp->get_parent_side();
			s32 const o = (1 - s);
			ASSERT(s == xrbnode15::LEFT || s == xrbnode15::RIGHT);
			ASSERT(o == xrbnode15::LEFT || o == xrbnode15::RIGHT);

			u16 w = parentp->get_child(o);
			xrbnode15* wp = xrbnode15::to_ptr(a, w);

			ASSERT(w != head);
			if (wp->is_red()) 
			{
				u16 const left = wp->get_left();
				u16 const right = wp->get_right();
				xrbnode15* leftp = xrbnode15::to_ptr(a, left);
				xrbnode15* rightp = xrbnode15::to_ptr(a, right);

				ASSERT(leftp->is_black() && rightp->is_black());
				wp->set_black();
				parentp->set_red();
				w  = (s==xrbnode15::LEFT) ? left : right;
				wp = (s==xrbnode15::LEFT) ? leftp : rightp;
				rb15_rotate(parent, s, a);
			}

			{
				ASSERT(w != head);
				u16 left = wp->get_left();
				u16 right = wp->get_right();
				xrbnode15* leftp = xrbnode15::to_ptr(a, left);
				xrbnode15* rightp = xrbnode15::to_ptr(a, right);
				if (leftp->is_black() && rightp->is_black())
				{ 
					wp->set_red();
					cur = parent;
					curp = parentp;
				} 
				else 
				{
					xrbnode15* op = (o==xrbnode15::LEFT) ? leftp : rightp;
					if (op->is_black()) 
					{
						u16 const  sc  = (s==xrbnode15::LEFT) ? left : right;
						xrbnode15* scp = (s==xrbnode15::LEFT) ? leftp : rightp;
						scp->set_black();
						wp->set_red();
						rb15_rotate(w, o, a);
						w  = sc;
						wp = scp;
						ASSERT(w != head);
						
						// w/wp have changed, update left/right
						left = wp->get_left();
						right = wp->get_right();
						leftp = xrbnode15::to_ptr(a, left);
						rightp = xrbnode15::to_ptr(a, right);
					}

					{
						u16 const  oc  = (o==xrbnode15::LEFT) ? left : right;
						xrbnode15* ocp = (o==xrbnode15::LEFT) ? leftp : rightp;
						ASSERT(ocp->is_red());
						wp->set_color(parentp->get_color());
						parentp->set_black();
						ocp->set_black();
						rb15_rotate(parent, s, a);
						cur = headp->get_child(xrbnode15::LEFT);
						curp = xrbnode15::to_ptr(a, cur);
					}
				}
			}
		}
		curp->set_black();
	}

	inline
	void			rb15_attach_to(u16 _this, u16 _parent, s32 s, x_indexer* a)
	{
		xrbnode15* _thisp   = xrbnode15::to_ptr(a, _this);
		xrbnode15* _parentp = xrbnode15::to_ptr(a, _parent);

		u16 const child = _parentp->get_child(s);
		_thisp->set_child(child, xrbnode15::LEFT);
		_thisp->set_child(child, xrbnode15::RIGHT);
		_thisp->set_parent(_parent);
		_thisp->set_parent_side(s);
		_thisp->set_color(xrbnode15::RED);
		_parentp->set_child(_this, s);
	}

	inline
	void			rb15_substitute_with(u16 _this, u16 _child, x_indexer* a)
	{
		xrbnode15* _thisp  = xrbnode15::to_ptr(a, _this);
		xrbnode15* _childp = xrbnode15::to_ptr(a, _child);
		s32 ps = _thisp->get_parent_side();
		u16 _parent = _thisp->get_parent();
		xrbnode15* _parentp = xrbnode15::to_ptr(a, _parent);
		_childp->set_parent(_parent); 
		_childp->set_parent_side(ps);
		_parentp->set_child(_child, ps); 
	}

	inline
	void			rb15_switch_with(u16 _this, u16 _node, x_indexer* a)
	{
		ASSERT(_this != _node);

		// Copy pointers (includes color and parent side)
		xrbnode15* _thisp = xrbnode15::to_ptr(a, _this);
		xrbnode15* _nodep = xrbnode15::to_ptr(a, _node);
		_thisp->set_parent(_nodep->get_parent());
		u16 left  = _nodep->get_left();
		_thisp->set_left(left);
		u16 right = _nodep->get_right();
		_thisp->set_right(right);

		_thisp->set_color(_nodep->get_color());
		s32 const ps = _nodep->get_parent_side();
		_thisp->set_parent_side(ps);

		xrbnode15* leftp  = xrbnode15::to_ptr(a, left);
		xrbnode15* rightp = xrbnode15::to_ptr(a, right);
		leftp->set_parent(_this);
		leftp->set_parent_side(xrbnode15::LEFT);
		rightp->set_parent(_this);
		rightp->set_parent_side(xrbnode15::RIGHT);

		// Set ourselves as the new child of our parent
		u16 _parent = _thisp->get_parent();
		xrbnode15* _parentp = xrbnode15::to_ptr(a, _parent);
		_parentp->set_child(_this, ps);
	}
};


#endif	///< __XBASE_REDBLACK_TREE_15_H__

