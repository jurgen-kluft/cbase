// x_rbtree31.h - xCore red-black tree
#ifndef __XBASE_REDBLACK_TREE_31_H__
#define __XBASE_REDBLACK_TREE_31_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_debug.h"
#include "xbase\x_idx_allocator.h"

namespace xcore
{
	//
	// This red-black tree node uses 32 bit indices to reference nodes, this
	// allows us to compress one node into 16 bytes. This node is also capable
	// of managing siblings (nodes with the same key).
	//
	// Maximum number of nodes that we can use is 2*1024*1024*1024 since we need the upper
	// bit of the u32 to store rb-color/parent-side/is-sibling.
	//
	// Siblings are managed as a double linked list using the 'left' and 'right'
	// child as 'prev' and 'next'. The 'parent' will point to the node that is
	// in the tree and thus contains the index to the head.
	// The double linked list is organized as a ring.
	//
	// sizeof(xrbnode31) == 16 bytes
	//
	struct xrbnode31_base
	{
	public:
		enum ESide { LEFT = 0x0, RIGHT = 0x1, SIDE_MASK = 0x1 };
		enum EColor { BLACK = 0x0, RED = 0x2, COLOR_MASK = 0x2 };

	protected:
		static inline s32	get_side(u32 member)						{ return (member & SIDE_MASK) == RIGHT ? RIGHT : LEFT; }
		static inline void	set_side(u32& member, ESide side)			{ member = (member & ~SIDE_MASK) | side; }
		static inline s32	get_color(u32 member)						{ return (member & COLOR_MASK) == RED ? RED : BLACK; }
		static inline void	set_color(u32& member, EColor color)		{ member = (member & ~COLOR_MASK) | color; }

	public:
		inline void			set_child(u32 node, s32 dir)				{ child[dir] = node; }
		inline u32			get_child(s32 dir) const					{ return child[dir]; }

		inline void			set_right(u32 node)							{ child[RIGHT] = node; }
		inline u32			get_right() const							{ return child[RIGHT]; }

		inline void			set_left(u32 node)							{ child[LEFT] = node; }
		inline u32			get_left() const							{ return child[LEFT]; }

		inline void			set_parent(u32 node)						{ parent = node; }
		inline u32			get_parent() const							{ return parent; }

		inline void			set_parent_side(s32 side)					{ ASSERT(side == LEFT || side == RIGHT); set_side(flags, (ESide)side); }
		inline s32			get_parent_side() const						{ return get_side(flags); }

		inline void			set_red()									{ set_color(flags, RED); }
		inline void			set_black()									{ set_color(flags, BLACK); }
		inline void			set_color(s32 colr)							{ ASSERT(colr == RED || colr == BLACK); set_color(flags, (EColor)colr); }
		inline s32			get_color() const							{ s32 colr = get_color(flags); ASSERT(colr == RED || colr == BLACK); return colr; }

		inline bool			is_red() const								{ return get_color(flags) == RED; }
		inline bool			is_black() const							{ return get_color(flags) == BLACK; }

		inline bool			is_nill(u32 nill) const						{ return child[RIGHT] == nill; }

	protected:
		u32					flags;
		u32					parent;
		u32					child[2];
	};

	struct xrbnode31 : public xrbnode31_base
	{
	public:
		inline void			clear(u32 nill)								{ flags = 0; parent = child[LEFT] = child[RIGHT] = nill; }

		static inline 
		xrbnode31*			to_ptr(x_indexer* a, u32 i)					{ return (xrbnode31*)a->to_ptr(i); }
	};

	struct xrbsnode31 : public xrbnode31_base
	{
		inline void			clear(u32 nill) { flags = 0; parent = sibling = child[LEFT] = child[RIGHT] = nill; }

		static inline
		xrbsnode31*			to_ptr(x_indexer* a, u32 i)					{ return (xrbsnode31*)a->to_ptr(i); }

		inline bool			has_sibling(u32 _nill) const				{ return sibling != _nill; }
		inline void			set_sibling(u32 s)							{ sibling = s; }
		inline u32			get_sibling() const							{ return sibling; }
		inline bool			is_sibling(u32 _nill) const 
		{ 
			return (child[LEFT] != _nill) && (parent == child[LEFT]) && (child[RIGHT] == sibling); 
		}

		void				insert_sibling(u32 _this, u32 _sib, u32 _nill, x_indexer* a)
		{
			xrbsnode31* sibp = (xrbsnode31*)a->to_ptr(_sib);

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
				xrbsnode31* nextp = (xrbsnode31*)a->to_ptr(next);
				u16 const prev = nextp->get_left();
				xrbsnode31* prevp = (xrbsnode31*)a->to_ptr(prev);
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

		void				remove_sibling(u32 sib, u32 nill, x_indexer* a)
		{
			xrbsnode31* sibp = (xrbsnode31*)a->to_ptr(sib);
			ASSERT(sibp->is_sibling(nill));

			u32 const next = sibp->get_right();
			if (next != sib)
			{
				xrbsnode31* nextp = (xrbsnode31*)a->to_ptr(next);
				u32 const prev = sibp->get_left();
				xrbsnode31* prevp = (xrbsnode31*)a->to_ptr(prev);
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

		u32	sibling;
	};

	inline u32	rb31_minimum(u32 root, x_indexer* a)
	{
		xrbnode31* rootp     = (xrbnode31*)xrbnode31::to_ptr(a, root);
	
		u32	       node      = rootp->get_left();
		u32	       lastNode  = node;
		while (node != root)
		{
			lastNode  = node;
			xrbnode31* nodep = (xrbnode31*)xrbnode31::to_ptr(a, node);
			node      = nodep->get_left();
		};
		return lastNode;
	}

	inline u32	rb31_maximum(u32 root, x_indexer* a)
	{
		xrbnode31* rootp     = (xrbnode31*)xrbnode31::to_ptr(a, root);

		u32	       node      = rootp->get_left();
		u32	       lastNode  = node;
		while (node != root)
		{
			lastNode  = node;
			xrbnode31* nodep = (xrbnode31*)xrbnode31::to_ptr(a, node);
			node      = nodep->get_right();
		};
		return lastNode;
	}

	// Before using this you need to either traverse to the 
	// minimum (far left) or maximum (far right).
	// 0 = successor
	// 1 = predecessor
	inline u32	rb31_inorder(s32 dir, u32 node, u32 nill, x_indexer* a)
	{
		xrbnode31* nodep = xrbnode31::to_ptr(a, node);
		if (nodep->is_nill(node))
			return node;

		u32 next = nodep->get_child(1-dir);
		xrbnode31* nextp = xrbnode31::to_ptr(a, next);
		if (nextp->is_nill(next))
		{
			if (nodep->get_parent_side() != dir)
			{
				while (nodep->get_parent_side() != dir)
				{
					node = nodep->get_parent();
					nodep = xrbnode31::to_ptr(a, node);
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
				nextp = xrbnode31::to_ptr(a, next);
			} while (!nextp->is_nill(next));
		}
		return node;
	}


	#ifdef TARGET_TEST
		#define DEBUG_RB31TREE
	#endif

#ifdef DEBUG_RB31TREE
	inline
	u32				rb31_check_height(u32 nill, u32 node, x_indexer* a)
	{
		if (node == nill)
			return 0;
		
		xrbnode31* nodep = xrbnode31::to_ptr(a, node);

		u32 left  = nodep->get_child(xrbnode31::LEFT );
		u32 right = nodep->get_child(xrbnode31::RIGHT);
		if (nodep->is_black())
			return rb31_check_height(nill, left, a) + rb31_check_height(nill, right, a) + 1;

		xrbnode31* leftp  = xrbnode31::to_ptr(a, left);
		xrbnode31* rightp = xrbnode31::to_ptr(a, right);
		ASSERT(leftp->is_black() && rightp->is_black());
		xcore::u32 lh = rb31_check_height(nill, left , a);
		xcore::u32 rh = rb31_check_height(nill, right, a);
		ASSERT(lh == rh);
		return lh;
	}

	inline
	void			rb31_check(u32 root, x_indexer* a)
	{
		xrbnode31* rootp = xrbnode31::to_ptr(a, root);

		u32 left  = rootp->get_left();
		u32 right = rootp->get_right();

		ASSERT(rootp->is_nill(root));
		ASSERT(rootp->is_black());
		ASSERT(right == root);
		xrbnode31* leftp = xrbnode31::to_ptr(a, left);
		ASSERT(left == root || leftp->is_black());
		rb31_check_height(root, left, a);
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	inline
	void			rb31_rotate(u32 node, s32 s, x_indexer* a)
	{
		xrbnode31* nodep = xrbnode31::to_ptr(a, node);
		u32 parent = nodep->get_parent();
		xrbnode31* parentp = xrbnode31::to_ptr(a, parent);
		ASSERT(parentp->get_child(nodep->get_parent_side()) == node);
		s32 o = (1 - s);
		ASSERT(o == xrbnode31::LEFT || o == xrbnode31::RIGHT);
		s32 ps = nodep->get_parent_side();
		u32 top = nodep->get_child(o);
		xrbnode31* topp = xrbnode31::to_ptr(a, top);
		u32 top_child = topp->get_child(s);
		xrbnode31* top_childp = xrbnode31::to_ptr(a, top_child);
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
	void			rb31_insert_fixup(u32 head, u32 node, x_indexer* a) 
	{
		u32 cur = node;
		xrbnode31* curp = xrbnode31::to_ptr(a, cur);
		u32 parent = curp->get_parent();
		xrbnode31* parentp = xrbnode31::to_ptr(a, parent);
		while (parentp->is_red()) 
		{
			u32 gp = parentp->get_parent();
			xrbnode31* gpp = xrbnode31::to_ptr(a, gp);
			ASSERT(gp != head);
			s32 s = parentp->get_parent_side();
			s32 o = (1 - s);
			ASSERT(s == xrbnode31::LEFT || s == xrbnode31::RIGHT);
			ASSERT(o == xrbnode31::LEFT || o == xrbnode31::RIGHT);
			u32 gp_right = gpp->get_child(o);
			xrbnode31* pp_right = xrbnode31::to_ptr(a, gp_right);
			if (pp_right->is_red())
			{
				parentp->set_color(xrbnode31::BLACK);
				pp_right->set_color(xrbnode31::BLACK);
				gpp->set_color(xrbnode31::RED);
				cur = gp;
				curp = gpp;
				parent = curp->get_parent();
				parentp = xrbnode31::to_ptr(a, parent);
			} 
			else 
			{
				if (cur == parentp->get_child(o))
				{
					cur = parent;
					curp = parentp;
					rb31_rotate(cur, s, a);
					parent = curp->get_parent();
					parentp = xrbnode31::to_ptr(a, parent);
				}
				parentp->set_color(xrbnode31::BLACK);
				gpp->set_color(xrbnode31::RED);
				rb31_rotate(gp, o, a);
			} 
		}
		xrbnode31* headp = xrbnode31::to_ptr(a, head);
		u32 head_left = headp->get_child(xrbnode31::LEFT);
		xrbnode31* head_leftp = xrbnode31::to_ptr(a, head_left);
		head_leftp->set_color(xrbnode31::BLACK);
	}

	inline
	void			rb31_erase_fixup(u32 root, u32 node, x_indexer* a)
	{
		u32 head = root;
		xrbnode31* headp = xrbnode31::to_ptr(a, root);

		u32 cur = node;
		xrbnode31* curp = xrbnode31::to_ptr(a, cur);
		while (!curp->is_red() && cur != headp->get_child(xrbnode31::LEFT)) 
		{
			u32 parent = curp->get_parent();
			xrbnode31* parentp = xrbnode31::to_ptr(a, parent);

			ASSERT(parentp != headp && parent != head);
			s32 const s = curp->get_parent_side();
			s32 const o = (1 - s);
			ASSERT(s == xrbnode31::LEFT || s == xrbnode31::RIGHT);
			ASSERT(o == xrbnode31::LEFT || o == xrbnode31::RIGHT);
			u32 w = parentp->get_child(o);
			xrbnode31* wp = xrbnode31::to_ptr(a, w);

			ASSERT(w != head);
			if (wp->is_red()) 
			{
				u32 const left = wp->get_left();
				u32 const right = wp->get_right();
				xrbnode31* leftp = xrbnode31::to_ptr(a, left);
				xrbnode31* rightp = xrbnode31::to_ptr(a, right);

				ASSERT(leftp->is_black() && rightp->is_black());
				wp->set_black();
				parentp->set_red();
				w  = (s==xrbnode31::LEFT) ? left : right;
				wp = (s==xrbnode31::LEFT) ? leftp : rightp;
				rb31_rotate(parent, s, a);
			}

			{
				ASSERT(w != head);
				u32 left = wp->get_left();
				u32 right = wp->get_right();
				xrbnode31* leftp = xrbnode31::to_ptr(a, left);
				xrbnode31* rightp = xrbnode31::to_ptr(a, right);
				if (leftp->is_black() && rightp->is_black())
				{ 
					wp->set_red();
					cur = parent;
					curp = parentp;
				} 
				else 
				{
					xrbnode31* op = (o==xrbnode31::LEFT) ? leftp : rightp;
					if (op->is_black()) 
					{
						u32 const  sc  = (s==xrbnode31::LEFT) ? left : right;
						xrbnode31* scp = (s==xrbnode31::LEFT) ? leftp : rightp;
						scp->set_black();
						wp->set_red();
						rb31_rotate(w, o, a);
						w  = sc;
						wp = scp;
						ASSERT(w != head);

						// w/wp have changed, update left/right
						left = wp->get_left();
						right = wp->get_right();
						leftp = xrbnode31::to_ptr(a, left);
						rightp = xrbnode31::to_ptr(a, right);
					}
					{
						u32 const  oc  = (o==xrbnode31::LEFT) ? left : right;
						xrbnode31* ocp = (o==xrbnode31::LEFT) ? leftp : rightp;
						ASSERT(ocp->is_red());
						wp->set_color(parentp->get_color());
						parentp->set_black();
						ocp->set_black();
						rb31_rotate(parent, s, a);
						cur = headp->get_child(xrbnode31::LEFT);
						curp = xrbnode31::to_ptr(a, cur);
					}
				}
			}
		}
		curp->set_black();
	}

	inline
	void			rb31_attach_to(u32 _this, u32 _parent, s32 s, x_indexer* a)
	{
		xrbnode31* _thisp   = xrbnode31::to_ptr(a, _this);
		xrbnode31* _parentp = xrbnode31::to_ptr(a, _parent);

		u32 const child = _parentp->get_child(s);
		_thisp->set_child(child, xrbnode31::LEFT);
		_thisp->set_child(child, xrbnode31::RIGHT);
		_thisp->set_parent(_parent);
		_thisp->set_parent_side(s);
		_thisp->set_color(xrbnode31::RED);
		_parentp->set_child(_this, s);
	}

	inline
	void			rb31_substitute_with(u32 _this, u32 _child, x_indexer* a)
	{
		xrbnode31* _thisp  = xrbnode31::to_ptr(a, _this);
		xrbnode31* _childp = xrbnode31::to_ptr(a, _child);
		s32 ps = _thisp->get_parent_side();
		u32 _parent = _thisp->get_parent();
		xrbnode31* _parentp = xrbnode31::to_ptr(a, _parent);
		_childp->set_parent(_parent); 
		_childp->set_parent_side(ps);
		_parentp->set_child(_child, ps); 
	}

	inline
	void			rb31_switch_with(u32 _this, u32 _node, x_indexer* a)
	{
		ASSERT(_this != _node);

		// Copy pointers (includes color and parent side)
		xrbnode31* _thisp = xrbnode31::to_ptr(a, _this);
		xrbnode31* _nodep = xrbnode31::to_ptr(a, _node);
		_thisp->set_parent(_nodep->get_parent());
		u32 left  = _nodep->get_left();
		_thisp->set_left(left);
		u32 right = _nodep->get_right();
		_thisp->set_right(right);

		_thisp->set_color(_nodep->get_color());
		s32 const ps = _nodep->get_parent_side();
		_thisp->set_parent_side(ps);

		xrbnode31* leftp  = xrbnode31::to_ptr(a, left);
		xrbnode31* rightp = xrbnode31::to_ptr(a, right);
		leftp->set_parent(_this);
		leftp->set_parent_side(xrbnode31::LEFT);
		rightp->set_parent(_this);
		rightp->set_parent_side(xrbnode31::RIGHT);

		// Set ourselves as the new child of our parent
		u32 _parent = _thisp->get_parent();
		xrbnode31* _parentp = xrbnode31::to_ptr(a, _parent);
		_parentp->set_child(_this, ps);
	}
};


#endif	///< __XBASE_REDBLACK_TREE_31_H__
