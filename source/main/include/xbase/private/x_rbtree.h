// x_rbtree.h - xCore red-black tree
#ifndef __XBASE_REDBLACK_TREE_PTR_H__
#define __XBASE_REDBLACK_TREE_PTR_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_debug.h"

namespace xcore
{
	//
	// This red-black tree node uses pointers to reference nodes, this
	// results in a node sizeof 16 bytes in 32-bit and 32 bytes in 64-bit.
	//
	struct xrbnode
	{
	public:
		enum ESide { LEFT = 0x0, RIGHT = 0x1, SIDE_MASK = 0x1 };
		enum EColor { BLACK = 0x0, RED = 0x2, COLOR_MASK = 0x2 };

	protected:
		static inline s32	get_side(xsize_t member)					{ return ((member & SIDE_MASK) == RIGHT) ? RIGHT : LEFT; }
		static inline void	set_side(xsize_t& member, ESide side)		{ member = (member & ~SIDE_MASK) | side; }
		static inline s32	get_color(xsize_t member)					{ return ((member & COLOR_MASK) == RED) ? RED : BLACK; }
		static inline void	set_color(xsize_t& member, EColor color)	{ member = (member & ~COLOR_MASK) | color; }

	public:
		inline void			clear(xrbnode* nill)						{ flags = 0; parent = child[LEFT] = child[RIGHT] = nill; }

		inline void			set_child(xrbnode* node, s32 dir)			{ child[dir] = node; }
		inline xrbnode*		get_child(s32 dir) const					{ return child[dir]; }
		inline xrbnode*		get_non_null_child() const					{ s32 d = (child[LEFT]->is_nill()) ? RIGHT : LEFT; return child[d]; }

		inline void			set_right(xrbnode* node)					{ child[RIGHT] = node; }
		inline xrbnode* 	get_right() const							{ return child[RIGHT]; }

		inline void			set_left(xrbnode* node)						{ child[LEFT] = node; }
		inline xrbnode* 	get_left() const							{ return child[LEFT]; }

		inline void			set_parent(xrbnode* node)					{ parent = node; }
		inline xrbnode*		get_parent() const							{ return parent; }

		inline void			set_parent_side(s32 side)					{ ASSERT(side==LEFT || side==RIGHT); set_side(flags, (ESide)side); }
		inline s32			get_parent_side() const						{ return get_side(flags); }

		inline void			set_red()									{ set_color(flags, RED); }
		inline void			set_black()									{ set_color(flags, BLACK); }
		inline void			set_color(s32 colr)							{ ASSERT(colr==RED || colr==BLACK); set_color(flags, (EColor)colr); }
		inline s32			get_color() const							{ s32 colr = get_color(flags); ASSERT(colr==RED || colr==BLACK); return colr; }

		inline bool			is_red() const								{ return get_color(flags) == RED; }
		inline bool			is_black() const							{ return get_color(flags) == BLACK; }

		inline bool			is_used() const								{ return this != child[RIGHT]; }
		inline bool			is_nill() const								{ return this == child[RIGHT]; }

	protected:
		xsize_t				flags;
		xrbnode*			parent;
		xrbnode*			child[2];
	};


	struct xrbnode_multi : public xrbnode
	{
		void				clear(xrbnode* nill) { siblings = NULL; xrbnode::clear(nill); }

		xrbnode_multi*		get_siblings() const
		{
			return siblings;
		}

		void				insert_sibling(xrbnode_multi* sib)
		{
			if (siblings == NULL) siblings = sib;
			else
			{
				xrbnode_multi* next = siblings;
				xrbnode_multi* prev = (xrbnode_multi*)next->get_child(LEFT);
				sib->set_child(prev, LEFT);
				sib->set_child(next, RIGHT);
				prev->set_child(sib, RIGHT);
				next->set_child(sib, LEFT);
			}
		}

		void				remove_sibling(xrbnode_multi* sib)
		{
			xrbnode_multi* next = (xrbnode_multi*)sib->get_child(RIGHT);
			xrbnode_multi* prev = (xrbnode_multi*)sib->get_child(LEFT);
			next->set_child(prev, LEFT);
			prev->set_child(next, RIGHT);
			if (siblings == sib && next != sib)
				siblings = next;
			else
				siblings = NULL;
		}

	protected:
		xrbnode_multi*		siblings;
	};


	inline xrbnode*	rb_minimum(xrbnode* root)
	{
		xrbnode*	rootp     = root;
		xrbnode*	node      = rootp->get_left();
		xrbnode*	lastNode  = node;
		while (node != root)
		{
			lastNode  = node;
			xrbnode* nodep = node;
			node      = nodep->get_left();
		};
		return lastNode;
	}

	inline xrbnode*	rb_maximum(xrbnode* root)
	{
		xrbnode*	rootp     = root;
		xrbnode*	node      = rootp->get_left();
		xrbnode*	lastNode  = node;
		while (node != root)
		{
			lastNode  = node;
			xrbnode* nodep = node;
			node      = nodep->get_right();
		};
		return lastNode;
	}

	// Before using this you need to either traverse to the 
	// minimum (far left) or maximum (far right).
	// 0 = successor
	// 1 = predecessor
	inline xrbnode*	rb_inorder(s32 dir, xrbnode* node)
	{
		if (node->is_nill())
			return node;

		xrbnode* next = node->get_child(1 - dir);
		if (next->is_nill())
		{
			if (node->get_parent_side() != dir)
			{
				while (node->get_parent_side() != dir)
					node = node->get_parent();
				node = node->get_parent();
			}
			else
			{
				node = node->get_parent();
			}
		}
		else
		{
			do {
				node = next;
				next = next->get_child(dir);
			} while (!next->is_nill());
		}
		return node;
	}

	#ifdef TARGET_TEST
		#define DEBUG_RBTREE
	#endif

#ifdef DEBUG_RBTREE
	inline
	u32				rb_check_height(xrbnode* nill, xrbnode* node)
	{
		if (node == nill)
			return 0;
		if (node->is_black())
			return rb_check_height(nill, node->get_child(xrbnode::LEFT)) + rb_check_height(nill, node->get_child(xrbnode::RIGHT)) + 1;
		ASSERT(node->get_child(xrbnode::LEFT)->is_black() && node->get_child(xrbnode::RIGHT)->is_black());
		xcore::u32 lh = rb_check_height(nill, node->get_child(xrbnode::LEFT));
		xcore::u32 rh = rb_check_height(nill, node->get_child(xrbnode::RIGHT));
		ASSERT(lh == rh);
		return lh;
	}

	inline
	void			rb_check(xrbnode* root)
	{
		ASSERT(root->is_nill());
		ASSERT(root->is_black());
		ASSERT(root->get_child(xrbnode::RIGHT) == root);
		ASSERT(root->get_child(xrbnode::LEFT) == root || root->get_child(xrbnode::LEFT)->is_black());
		rb_check_height(root, root->get_child(xrbnode::LEFT));
	}
#endif

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	inline
		void			rb_rotate(xrbnode* node, s32 s)
	{
		ASSERT(node->get_parent()->get_child(node->get_parent_side()) == node);
		s32 o = (1 - s);
		s32 ps = node->get_parent_side();
		xrbnode* top = node->get_child(o);
		xrbnode* top_child = top->get_child(s);
		node->set_child(top_child, o);
		top_child->set_parent(node);
		top_child->set_parent_side(o);
		top->set_parent(node->get_parent());
		top->set_parent_side(ps);
		node->get_parent()->set_child(top, ps);
		top->set_child(node, s);
		node->set_parent(top);
		node->set_parent_side(s);
	}

	extern void		rb_insert_fixup(xrbnode& head, xrbnode* node);
	extern void		rb_erase_fixup(xrbnode* root, xrbnode* node);

	inline
		void			rb_attach_to(xrbnode* _this, xrbnode* _parent, s32 _s)
	{
		xrbnode* child = _parent->get_child(_s);
		_this->set_child(child, xrbnode::LEFT);
		_this->set_child(child, xrbnode::RIGHT);
		_this->set_parent(_parent);
		_this->set_parent_side(_s);
		_this->set_color(xrbnode::RED);
		_parent->set_child(_this, _s);
	}

	inline
		void			rb_substitute_with(xrbnode* _this, xrbnode* _child)
	{
		ASSERT(_this != _child);
		s32 ps = _this->get_parent_side();
		xrbnode* parent = _this->get_parent();
		_child->set_parent(parent);
		_child->set_parent_side(ps);
		parent->set_child(_child, ps);
	}

	extern void		rb_switch_with(xrbnode* _this, xrbnode* _node);

};


#endif	///< __XBASE_REDBLACK_TREE_H__

