// x_tree.h - xcore C style red-black tree functions
#ifndef __XBASE_TREE_H__
#define __XBASE_TREE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_debug.h"

namespace xcore
{
	//------------------------------------------------------------------------------
	// Author:
	//		Jurgen Kluft
	// Description:
	//
	//      Red-black tree structure and helper-functions
	// 
	//<CODE [lang= "c++"]>
	// 		
	// 	Need an example here
	// 		
	//</CODE>
	//     
	//------------------------------------------------------------------------------
	struct xrbnode;
	
	// @note	If the key is greater than the key of node then return 1. If the key is less than the key of node then return -1 otherwise return 0
	typedef	s32			(*xtree_compare_delegate)(const xrbnode* node, const void* key);

	// @note	A 'C' style way of managing your own red-black tree, the only xrbnode object you need is a 'root'.
	// @see		See test_x_tree.cpp for examples on how to use this.
	extern bool			xtree_insert(xrbnode* root, xrbnode * node, void* key, xtree_compare_delegate comparer);
	extern bool			xtree_remove(xrbnode* root, xrbnode*& node, void* key, xtree_compare_delegate comparer);
	extern bool			xtree_find  (xrbnode* root, xrbnode*& node, void* key, xtree_compare_delegate comparer);

	// @note	Initialize 'iterator' to NULL, iterate until NULL is returned.
	//			The returned node can be destroyed and it is already detached from the tree.
	extern xrbnode*		xtree_clear (xrbnode* root, xrbnode*& iterator);

	struct xrbnode
	{
		enum ESide	{ LEFT = 0, RIGHT = 1 };
		enum EColor { RED  = 2, BLACK = 0 };

		inline void			clear()										{ parent = child[LEFT] = child[RIGHT] = this; }

		inline void			set_child(xrbnode* node, s32 dir)			{ child[dir] = node; }
		inline xrbnode*		get_child(s32 dir) const						{ return child[dir]; }
		inline xrbnode*		get_non_null_child() const					{ s32 d = child[LEFT]->is_nill(); return child[d]; }

		inline void			set_right(xrbnode* node)						{ child[RIGHT] = node; }
		inline xrbnode*		get_right() const							{ return child[RIGHT]; }

		inline void			set_left(xrbnode* node)						{ child[LEFT] = node; }
		inline xrbnode*		get_left() const								{ return child[LEFT]; }

		inline void			set_parent(xrbnode* node)					{ u32 const flags = (u32)parent & 0x3; parent = (xrbnode*)((X_PTR_SIZED_INT)node | (X_PTR_SIZED_INT)flags); }
		inline xrbnode*		get_parent() const							{ return (xrbnode*)((u8*)parent - ((u32)parent&0x3)); }

		inline void			set_parent_side(s32 side)					{ ASSERT(side==LEFT || side==RIGHT); if (side==RIGHT) { parent = (xrbnode*)((X_PTR_SIZED_INT)parent | (X_PTR_SIZED_INT)0x1); } else { parent = (xrbnode*)((X_PTR_SIZED_INT)parent & (X_PTR_SIZED_INT)-2); } }
		inline s32			get_parent_side() const						{ s32 const side = (s32)parent & 0x1; ASSERT(side==LEFT || side==RIGHT); return side; }

		inline void			set_red()									{ parent = (xrbnode*)((X_PTR_SIZED_INT)parent | (X_PTR_SIZED_INT)RED); }
		inline void			set_black()									{ parent = (xrbnode*)((X_PTR_SIZED_INT)parent & (X_PTR_SIZED_INT)-3); }
		inline void			set_color(s32 colr)							{ if (colr==RED) { parent = (xrbnode*)((X_PTR_SIZED_INT)parent | (X_PTR_SIZED_INT)RED); } else { parent = (xrbnode*)((X_PTR_SIZED_INT)parent & (X_PTR_SIZED_INT)-3); } }
		inline s32			get_color() const							{ s32 const colr = (s32)parent & 0x2; ASSERT(colr == RED || colr == BLACK); return colr; }

		inline bool			is_red() const								{ return ((u32)parent & 0x2) == RED; }
		inline bool			is_black() const								{ return ((u32)parent & 0x2) == BLACK; }
		inline bool			is_used() const								{ return this != child[RIGHT]; }
		inline bool			is_nill() const								{ return this == child[RIGHT]; }

	protected:
		xrbnode*			parent;
		xrbnode*			child[2];
	};

	struct xrbnode_multi : public xrbnode
	{
		void				clear()										{ siblings = NULL; xrbnode::clear(); }

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


	// Before using this you need to either traverse to the 
	// minimum (far left) or maximum (far right).
	// 0 = successor
	// 1 = predecessor
	inline xrbnode*	rb_inorder(s32 dir, xrbnode* node)
	{
		if (node->is_nill())
			return node;

		xrbnode* next = node->get_child(1-dir);
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

}


#endif	///< __XBASE_TREE_H__

