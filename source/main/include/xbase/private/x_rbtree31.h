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
	struct xrbnode31
	{
	protected:
		static inline void	set_idx(u32& member, u32 value)				{ member = (member&0x80000000) | (value&0x7fffffff); }
		static inline u32	get_idx(u32 member)							{ return (member&0x7fffffff); }
		static inline s32	get_flag(u32 member)						{ return (member&0x80000000) ? 1 : 0; }
		static inline void	set_flag0(u32& member)						{ member = (member&0x7fffffff); }
		static inline void	set_flag1(u32& member)						{ member = (member|0x80000000); }
		static inline void	set_flag(u32& member, s32 flag)				{ member = (member&0x7fffffff) | ((flag!=0) ? 0x80000000 : 0); }
		static inline bool	is_flag0(u32 member)						{ return (member&0x80000000) == 0; }
		static inline bool	is_flag1(u32 member)						{ return (member&0x80000000) != 0; }

	public:
		enum ESide	{ LEFT  = 0, RIGHT = 1 };
		enum EColor { BLACK = 0, RED   = 1 };

		inline void			clear(u32 nill)								{ parent = sibling = child[LEFT] = child[RIGHT] = nill; }

		inline void			set_child(u32 node, s32 dir)				{ child[dir] = node; }
		inline u32			get_child(s32 dir) const					{ return child[dir]; }
		inline u32			get_non_null_child(u32 nill) const			{ s32 d = child[LEFT]==nill; return child[d]; }

		inline void			set_right(u32 node)							{ child[RIGHT] = node; }
		inline u32			get_right() const							{ return child[RIGHT]; }

		inline void			set_left(u32 node)							{ child[LEFT] = node; }
		inline u32			get_left() const							{ return child[LEFT]; }

		inline void			set_parent(u32 node)						{ set_idx(parent, node); }
		inline u32			get_parent() const							{ return get_idx(parent); }

		inline void			set_parent_side(s32 side)					{ ASSERT(side==LEFT || side==RIGHT); set_flag(parent, side); }
		inline s32			get_parent_side() const						{ return get_flag(parent); }

		inline void			set_red()									{ set_flag1(sibling); }
		inline void			set_black()									{ set_flag0(sibling); }
		inline void			set_color(s32 colr)							{ ASSERT(colr==RED || colr==BLACK); set_flag(sibling, colr); }
		inline s32			get_color() const							{ s32 colr = get_flag(sibling); ASSERT(colr==RED || colr==BLACK); return colr; }

		inline bool			is_red() const								{ return is_flag1(sibling); }
		inline bool			is_black() const							{ return is_flag0(sibling); }

		static inline 
		xrbnode31*			to_ptr(x_indexer* a, u32 i)					{ return (xrbnode31*)a->to_ptr(i); }

		inline bool			has_sibling(u32 _nill) const				{ return get_idx(sibling)!=_nill; }
		inline void			set_sibling(u32 s)							{ set_idx(sibling, s); }
		inline u32			get_sibling() const							{ return get_idx(sibling); }
		inline bool			is_sibling(u32 _nill) const					{ return (child[LEFT]!=_nill) && (get_idx(parent)==child[LEFT]) && (child[RIGHT]==get_idx(sibling)); }

		void				insert_sibling(u32 _this, u32 _sib, u32 _nill, x_indexer* a)
		{
			xrbnode31* sibp = to_ptr(a, _sib);

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
				u32 const next = get_sibling();
				xrbnode31* nextp = to_ptr(a, next);
				u32 const prev = nextp->get_left();
				xrbnode31* prevp = to_ptr(a, prev);

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
			xrbnode31* sibp = to_ptr(a, sib);
			ASSERT(sibp->is_sibling(nill));

			u32 const next = sibp->get_right();
			if (next != sib)
			{
				xrbnode31* nextp = to_ptr(a, next);
				u32 const prev = sibp->get_left();
				xrbnode31* prevp = to_ptr(a, prev);
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
	protected:
		u32					parent;
		u32					child[2];
		u32					sibling;
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
		if (node == nill)
			return node;

		xrbnode31* nodep = xrbnode31::to_ptr(a, node);
		u32 next = nodep->get_child(1-dir);
		if (next == nill)
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
				xrbnode31* nextp = xrbnode31::to_ptr(a, next);
				next = nextp->get_child(dir);
			} while (next!=nill);
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

		ASSERT(rootp->get_right() == root);
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
				u32 const left = wp->get_left();
				u32 const right = wp->get_right();
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
						u32 const  c  = (s==xrbnode31::LEFT) ? left : right;
						xrbnode31* cp = (s==xrbnode31::LEFT) ? leftp : rightp;
						cp->set_black();
						wp->set_red();
						rb31_rotate(w, o, a);
						w  = c;
						wp = cp;
						ASSERT(w != head);
					}
					{
						u32 const  c  = (o==xrbnode31::LEFT) ? left : right;
						xrbnode31* cp = (o==xrbnode31::LEFT) ? leftp : rightp;
						ASSERT(cp->is_red());
						wp->set_color(parentp->get_color());
						parentp->set_black();
						cp->set_black();
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

