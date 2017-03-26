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

	//
	// This red-black tree node uses pointers to reference nodes, this
	// results in a node sizeof 8 bytes in 32-bit and 16 bytes in 64-bit.
	//
	struct xrbnode
	{
	public:
		enum ESide	{ LEFT  = 0x0, RIGHT = 0x1 };
		enum EColor	{ BLACK = 0x0, RED   = 0x1, COLOR_MASK = 0x1 };

	private:
		static inline xrbnode*	get_ptr(xrbnode* p, uptr m)					{ return (xrbnode*)((uptr)p & ~m); }
		static inline xrbnode*	set_ptr(xrbnode* p, xrbnode* n, uptr m)		{ return (xrbnode*)(((uptr)p & m) | ((uptr)n & ~m)); }

		static inline uptr		get_bit(xrbnode* p, uptr b)		 			{ return (uptr)p & b;}
		static inline xrbnode*	set_bit(xrbnode* p, uptr m, uptr b)			{ return (xrbnode*)(((uptr)p & ~m) | b); }

	public:
		inline void			clear()										{ left = (xrbnode*)RED; right = NULL; }

		inline void			set_child(xrbnode* node, s32 dir)
		{
			switch (dir)
			{
			case LEFT: 
				left = set_ptr(left, node, COLOR_MASK); 
				break;
			case RIGHT: 
				right = node; 
				break;
			}
		}
		inline xrbnode*		get_child(s32 dir) const
		{ 
			switch (dir)
			{
			case LEFT: 
				return get_ptr(left, COLOR_MASK);
			}
			return right;
		}

		inline void			set_right(xrbnode* node)					{ right = node; }
		inline xrbnode* 	get_right() const							{ return right; }

		inline void			set_left(xrbnode* node)						{ left = set_ptr(left, node, COLOR_MASK); }
		inline xrbnode* 	get_left() const							{ return get_ptr(left, COLOR_MASK); }

		inline s32			get_side(xrbnode* node)						{ ASSERT(node==get_left() || node==get_right()); return right == node ? RIGHT : LEFT; }

		inline void			set_red()									{ left = set_bit(left, COLOR_MASK, RED); }
		inline void			set_black()									{ left = set_bit(left, COLOR_MASK, BLACK); }
		inline void			set_color(s32 colr)							{ ASSERT(colr==RED || colr==BLACK); left = set_bit(left, COLOR_MASK, colr); }
		inline s32			get_color() const							{ s32 colr = (s32)get_bit(left, COLOR_MASK); ASSERT(colr==RED || colr==BLACK); return colr; }
		inline bool			is_red() const								{ return get_bit(left, COLOR_MASK) == RED; }
		inline bool			is_black() const							{ return get_bit(left, COLOR_MASK) == BLACK; }

		XCORE_CLASS_PLACEMENT_NEW_DELETE

	private:
		xrbnode				*left, *right;
	};


	struct xmrbnode : public xrbnode
	{
		void				clear()										{ siblings = NULL; xrbnode::clear(); }

		xmrbnode*			get_siblings() const						{ return siblings; }

		void				insert_sibling(xmrbnode* sib)
		{
			if (siblings == NULL) siblings = sib;
			else
			{
				xmrbnode* next = siblings;
				xmrbnode* prev = (xmrbnode*)next->get_child(LEFT);
				sib->set_child(prev, LEFT);
				sib->set_child(next, RIGHT);
				prev->set_child(sib, RIGHT);
				next->set_child(sib, LEFT);
			}
		}

		void				remove_sibling(xmrbnode* sib)
		{
			xmrbnode* next = (xmrbnode*)sib->get_child(RIGHT);
			xmrbnode* prev = (xmrbnode*)sib->get_child(LEFT);
			next->set_child(prev, LEFT);
			prev->set_child(next, RIGHT);
			if (siblings == sib && next != sib)
				siblings = next;
			else
				siblings = NULL;
		}

	protected:
		xmrbnode*		siblings;
	};

	inline s32 rb_flip_dir(s32 dir)
	{
		ASSERT(xrbnode::RIGHT == 1);
		ASSERT(dir == xrbnode::RIGHT || dir == xrbnode::LEFT);
		return dir ^ xrbnode::RIGHT;
	}

	inline bool	rb_is_red(xrbnode* node)
	{
		return node != NULL ? node->is_red() : xrbnode::BLACK;
	}

	struct rb_iterator
	{
		inline		rb_iterator() : node(NULL), top(0) {}
		inline		rb_iterator(xrbnode* n) : node(n), top(0) {}

		enum EType
		{
			FORWARDS = 1,
			BACKWARDS = 0,
			MINIMUM = 0,
			MAXIMUM = 1,
			MAX_HEIGHT = 64
		};

		xrbnode*	init(xrbnode * root, EType dir)
		{
			xrbnode * result = NULL;
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

		xrbnode*	move(EType dir)
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
				xrbnode * last = NULL;
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

		void		push(xrbnode* node)		{ ASSERT(top < MAX_HEIGHT); path[top++] = node; }
		xrbnode*	pop()					{ ASSERT(top > 0); return path[--top]; }

		xrbnode*	node;
		s32			top;
		xrbnode*	path[MAX_HEIGHT];
	};

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	inline xrbnode*		rb_rotate(xrbnode* self, s32 dir)
	{
		xrbnode* result = NULL;
		if (self)
		{
			xrbnode* rchild;
			
			s32 const sdir = rb_flip_dir(dir);

			result = self->get_child(sdir);
			rchild =  result->get_child(dir);
			
			self->set_child(rchild, sdir);
			result->set_child(self, dir);
			
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
			s32 const sdir = rb_flip_dir(dir);

			xrbnode* child = rb_rotate(self->get_child(sdir), sdir);
			self->set_child(child, sdir);
			result = rb_rotate(self, dir);
		}
		return result;
	}

	typedef s32(*xrbnode_cmp_f) (xrbnode* a, xrbnode* b);
	typedef void(*xrbnode_remove_f) (xrbnode* a, xrbnode* b);

	// Test the integrity of the red-black tree
	// @return: The depth of the tree
	// @result: If any error it returns a description of the error in 'result', when no error it will be NULL
	inline s32 rb_tree_test(xrbnode* root, xrbnode_cmp_f cmp_f, const char*& result)
	{
		s32 lh, rh;
		if (root == NULL)
		{
			return 1;
		}
		else
		{
			xrbnode* ln = root->get_left();
			xrbnode* rn = root->get_right();

			// Consecutive red links
			if (rb_is_red(root))
			{
				if (rb_is_red(ln) || rb_is_red(rn))
				{
					result = "Red violation";
					return 0;
				}
			}

			lh = rb_tree_test(ln, cmp_f, result);
			rh = rb_tree_test(rn, cmp_f, result);

			// Invalid binary search tree 
			if ((ln != NULL && cmp_f(ln, root) >= 0) || (rn != NULL && cmp_f(rn, root) <= 0))
			{
				result = "Binary tree violation";
				return 0;
			}

			// Black height mismatch
			if (lh != 0 && rh != 0 && lh != rh)
			{
				result = "Black violation";
				return 0;
			}

			// Only count black links
			if (lh != 0 && rh != 0)
			{
				return rb_is_red(root) ? lh : lh + 1;
			}
			return 0;
		}
	}

	// Returns 1 on success, 0 otherwise.
	inline bool	rb_insert_node(xrbnode*& root, xrbnode * node, xrbnode_cmp_f cmp_f)
	{
		bool result = false;
		if (node)
		{
			if (root == NULL)
			{
				root = node;
				result = true;
			}
			else
			{
				xrbnode head;		// False tree root
				head.clear();

				s32 dir = 0;
				s32 lastdir = 0;

				// Set up our helpers
				xrbnode *g = NULL;	// Grandparent
				xrbnode *t = &head;	// Parent
				xrbnode *p = NULL;	// Parent
				xrbnode *q = root;	// Iterator

				t->set_child(root, xrbnode::RIGHT);

				// Search down the tree for a place to insert
				result = false;
				while (true)
				{
					if (q == NULL)
					{	// Insert node at the first null link.
						q = node;
						result = true;
						p->set_child(q, dir);
					}
					else if (rb_is_red(q->get_left()) && rb_is_red(q->get_right()))
					{	// Simple red violation: color flip
						q->set_red();
						q->get_left()->set_black();
						q->get_right()->set_black();
					}

					if (rb_is_red(q) && rb_is_red(p))
					{	// Hard red violation: rotations necessary
						s32 const dir2 = (t->get_right() == g) ? xrbnode::RIGHT : xrbnode::LEFT;
						if (q == p->get_child(lastdir))
						{
							xrbnode* r = rb_rotate(g, !lastdir);
							t->set_child(r, dir2);
						}
						else
						{
							xrbnode* r = rb_rotate2(g, !lastdir);
							t->set_child(r, dir2);
						}
					}

					// Stop working if we inserted a node.
					// This check also disallows duplicates in the tree
					s32 const c = cmp_f(node, q);
					if (c == 0)
						break;

					lastdir = dir;
					dir = c < 0 ? xrbnode::LEFT : xrbnode::RIGHT;

					// Move the helpers down
					if (g != NULL) 
					{
						t = g;
					}

					g = p;
					p = q;
					q = q->get_child(dir);
				}
				root = head.get_child(xrbnode::RIGHT);		// Update the root (it may be different)
			}
			root->set_black();	// Make the root black for simplified logic
		}
		return result;
	}


	// Returns 1 if the value was removed, 0 otherwise. Optional node callback
	// can be provided to dealloc node and/or user data. Use rb_tree_node_dealloc
	// default callback to deallocate node created by rb_tree_insert(...).
	inline bool	rb_remove_node(xrbnode *& root, xrbnode* find, xrbnode_cmp_f cmp_f, xrbnode_remove_f remove_f, xrbnode*& outnode)
	{
		if (root != NULL)
		{
			xrbnode head; // False tree root
			head.clear();

			// Set up our helpers
			xrbnode * q = &head;
			xrbnode * g = NULL;
			xrbnode * p = NULL;
			xrbnode * f = NULL;  // Found item
			q->set_child(root, xrbnode::RIGHT);

			// Search and push a red node down to fix red violations as we go
			s32 dir = xrbnode::RIGHT;
			while (q->get_child(dir) != NULL)
			{
				s32 const lastdir = dir;

				// Move the helpers down
				g = p;
				p = q;
				q = q->get_child(dir);

				s32 const c = cmp_f(find, q);

				// Save the node with matching value and keep
				// going; we'll do removal tasks at the end
				if (c == 0)
				{
					f = q;
				}

				dir = (c < 0) ? xrbnode::LEFT : xrbnode::RIGHT;

				// Push the red node down with rotations and color flips
				if (!rb_is_red(q) && !rb_is_red(q->get_child(dir)))
				{
					if (rb_is_red(q->get_child(!dir)))
					{
						xrbnode* r = rb_rotate(q, dir);
						p->set_child(r, lastdir);
						p = r;
					}
					else if (!rb_is_red(q->get_child(!dir)))
					{
						xrbnode * s = p->get_child(!lastdir);
						if (s)
						{
							if (!rb_is_red(s->get_child(!lastdir)) && !rb_is_red(s->get_child(lastdir)))
							{	// Color flip
								p->set_black();
								s->set_red();
								q->set_red();
							}
							else
							{
								s32 dir2 = g->get_child(1) == p;

								if (rb_is_red(s->get_child(lastdir)))
								{
									xrbnode* r = rb_rotate2(p, lastdir);
									g->set_child(r, dir2);
								}
								else if (rb_is_red(s->get_child(!lastdir)))
								{
									xrbnode* r = rb_rotate(p, lastdir);
									g->set_child(r, dir2);
								}

								// Ensure correct coloring
								s = g->get_child(dir2);
								s->set_red();
								q->set_red();
								s->get_left()->set_black();
								s->get_right()->set_black();
							}
						}
					}
				}
			}

			// Replace and remove the saved node
			if (f != NULL)
			{
				remove_f(f, q);

				s32 const ps = p->get_child(xrbnode::RIGHT) == q ? xrbnode::RIGHT : xrbnode::LEFT;
				s32 const qs = q->get_child(xrbnode::LEFT) == NULL ? xrbnode::RIGHT : xrbnode::LEFT;
				p->set_child(q->get_child(qs), ps);

				// Give the 'removed' node back
				q->set_left(NULL);
				q->set_right(NULL);
				outnode = q;
			}

			// Update the root (it may be different)
			root = head.get_child(xrbnode::RIGHT);

			// Make the root black for simplified logic
			if (root != NULL)
			{
				root->set_black();
			}

			return f != NULL;
		}
		return true;
	}
};


#endif	///< __XBASE_REDBLACK_TREE_H__

