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
		inline void			clear()										{ flags = 0; parent = child[LEFT] = child[RIGHT] = NULL; }

		inline void			set_child(xrbnode* node, s32 dir)			{ child[dir] = node; }
		inline xrbnode*		get_child(s32 dir) const					{ return child[dir]; }

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

	typedef s32(*xrbnode_cmp_f) (xrbnode * a, xrbnode * b);
	typedef void(*xrbnode_swap_f) (xrbnode * a, xrbnode * b);

	inline xrbnode*	rb_minimum(xrbnode* root)
	{
		xrbnode*	node      = root->get_left();
		xrbnode*	lastNode  = node;
		while (node != NULL)
		{
			lastNode  = node;
			node      = node->get_left();
		};
		return lastNode;
	}

	inline xrbnode*	rb_maximum(xrbnode* root)
	{
		xrbnode*	node      = root->get_left();
		xrbnode*	lastNode  = node;
		while (node != NULL)
		{
			lastNode  = node;
			node      = node->get_right();
		};
		return lastNode;
	}

	// Inorder traversal (minimum -> maximum)
	inline xrbnode*	rb_inorder(xrbnode* node)
	{
		xrbnode* right = node->get_right();
		if (right == NULL)
		{
			while (node->get_parent_side() == xrbnode::RIGHT)
				node = node->get_parent();
			node = node->get_parent();
		}
		else
		{	// Right child and then traverse fully left
			node = right;
			xrbnode* left = node->get_left();
			while (left != NULL)
			{
				node = left;
				left = left->get_left();
			}
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
	inline xrbnode*		rb_rotate(xrbnode* self, s32 dir)
	{
		xrbnode* result = NULL;
		if (self)
		{
			xrbnode* rchild;

			result = self->get_child(!dir);
			rchild =  result->get_child(dir);
			
			self->set_child(rchild, !dir);
			rchild->set_parent_side(!dir);

			result->set_child(self, dir);
			self->set_parent_side(dir);
			
			self->set_red();
			result->set_black();
		}
		return result;
	}

	inline xrbnode*	rb_rotate2(xrbnode * self, s32 dir)
	{
		xrbnode * result = NULL;
		if (self)
		{
			xrbnode* child = rb_rotate(self->get_child(!dir), dir);
			self->set_child(child, !dir);
			child->set_parent_side(!dir);
			result = rb_rotate(self, dir);
		}
		return result;
	}

	// Returns 1 on success, 0 otherwise.
	int rb_insert_node(xrbnode*& root, xrbnode * node, xrbnode_cmp_f cmp)
	{
		s32 result = 0;
		if (node)
		{
			if (root == NULL)
			{
				root = node;
				result = 1;
			}
			else
			{
				xrbnode head;		// False tree root
				head.clear();

				xrbnode *g, *t;		// Grandparent & parent
				xrbnode *p, *q;		// Iterator & parent
				
				s32 dir = 0;
				s32 last = 0;

				// Set up our helpers
				g = NULL;
				t = &head;
				p = NULL;
				q = root;

				t->set_child(root, 1);

				// Search down the tree for a place to insert
				while (true)
				{
					if (q == NULL)
					{	// Insert node at the first null link.
						q = node;
						p->set_child(q, dir);
						q->set_parent_side(dir);
					}
					else if (q->get_child(0)->is_red() && q->get_child(1)->is_red())
					{	// Simple red violation: color flip
						q->set_red();
						q->get_child(0)->set_black();
						q->get_child(1)->set_black();
					}

					if (q->is_red() && p->is_red())
					{	// Hard red violation: rotations necessary
						s32 dir2 = t->get_child(1) == g ? 1 : 0;
						if (q == p->get_child(last))
						{
							xrbnode* r = rb_rotate(g, !last);
							t->set_child(r, dir2);
							r->set_parent_side(dir2);
						}
						else
						{
							xrbnode* r = rb_rotate2(g, !last);
							t->set_child(r, dir2);
							r->set_parent_side(dir2);
						}
					}

					// Stop working if we inserted a node.
					// This check also disallows duplicates in the tree
					if (cmp(q, node) == 0)
					{
						break;
					}

					last = dir;
					dir = cmp(q, node) < 0;

					// Move the helpers down
					if (g != NULL) 
					{
						t = g;
					}

					g = p;
					p = q;
					q = q->get_child(dir);
				}

				// Update the root (it may be different)
				root = head.get_child(1);
			}

			// Make the root black for simplified logic
			root->set_black();
		}

		return 1;
	}


	// Returns 1 if the value was removed, 0 otherwise. Optional node callback
	// can be provided to dealloc node and/or user data. Use rb_tree_node_dealloc
	// default callback to deallocate node created by rb_tree_insert(...).
	int	rb_remove_node(xrbnode *& root, xrbnode* find, xrbnode_cmp_f cmp, xrbnode_swap_f swap, xrbnode*& outnode)
	{
		if (root != NULL)
		{
			xrbnode head; // False tree root
			head.clear();

			xrbnode *q, *p, *g; // Helpers
			xrbnode *f = NULL;  // Found item
			s32 dir = 1;

			// Set up our helpers
			q = &head;
			g = p = NULL;
			q->set_child(root, 1);

			// Search and push a red node down
			// to fix red violations as we go
			while (q->get_child(dir) != NULL) 
			{
				s32 last = dir;

				// Move the helpers down
				g = p, p = q;
				q = q->get_child(dir);

				s32 c = cmp(q, find);

				// Save the node with matching value and keep
				// going; we'll do removal tasks at the end
				if (c == 0)
				{
					f = q;
				}

				dir = c < 0;
				// Push the red node down with rotations and color flips
				if (!q->is_red() && !q->get_child(dir)->is_red())
				{
					if (q->get_child(!dir)->is_red())
					{
						xrbnode* r = rb_rotate(q, dir);
						p->set_child(r, last);
						r->set_parent_side(last);
						p = r;
					}
					else if (!(q->get_child(!dir)->is_red()))
					{
						xrbnode * s = p->get_child(!last);
						if (s)
						{
							if (!s->get_child(!last)->is_red() && !s->get_child(last))
							{	// Color flip
								p->set_black();
								s->set_red();
								q->set_red();
							}
							else
							{
								s32 dir2 = g->get_child(1) == p;

								if (s->get_child(last)->is_red())
								{
									xrbnode* r = rb_rotate2(p, last);
									g->set_child(r, dir2);
									r->set_parent_side(dir2);
								}
								else if (s->get_child(!last)->is_red())
								{
									xrbnode* r = rb_rotate(p, last);
									g->set_child(r, dir2);
									r->set_parent_side(dir2);
								}

								// Ensure correct coloring
								s = g->get_child(dir2);
								q->set_red();
								s->set_red();
								s->get_child(0)->set_black();
								s->get_child(1)->set_black();
							}
						}
					}
				}
			}

			// Replace and remove the saved node
			if (f)
			{
				// Swap any 'values' that the nodes are holding since node @q has
				// been removed
				swap(f, q);

				s32 s1 = p->get_child(1) == q;
				s32 s2 = q->get_child(0) == NULL;
				p->set_child(q->get_child(s2), s1);

				// Give the 'removed' node back
				outnode = q;
			}

			// Update the root (it may be different)
			root = head.get_child(1);

			// Make the root black for simplified logic
			if (root != NULL)
			{
				root->set_black();
			}
		}
		return 1;
	}
};


#endif	///< __XBASE_REDBLACK_TREE_H__

