#include "xbase/x_allocator.h"
#include "xbase/x_tree.h"

#include <string>

namespace xcore
{
	struct xnode_t
	{
		s32			red;		// Color (1=red, 0=black) 
		xnode_t *	link[2];	// Left (0) and right (1) links 
		void*		data;		// User-defined content 
	};

	typedef s32		(*cmp_f) (const void *p1, const void *p2);
	typedef void *	(*dup_f) (void *p);
	typedef void	(*rel_f) (void *p);

	struct xiterator_t
	{
		enum EConfig { HEIGHT_LIMIT = 64 };		// Tallest allowable tree

		xtree *		tree;						// Paired tree
		xnode_t *	it;							// Current node
		xnode_t *	path[HEIGHT_LIMIT];			// Traversal path
		u32			top;						// Top of stack
	};

	s32				compare_void(void const* p1, void const* p2)
	{
		if (p1 < p2)
			return -1;
		else if (p1 > p2)
			return 1;
		return 0;
	}

	class xtree_internal
	{
	public:
		static s32			is_red(xnode_t *root);

		static xnode_t *	rotate_single(xnode_t *root, s32 dir);
		static xnode_t *	rotate_double(xnode_t *root, s32 dir);

		static xnode_t *	node_new(xtree *tree, void *data);
		static void			tree_clear(xtree *tree);

		static bool			tree_find(xtree *tree, void *data);
		static s32			tree_insert(xtree *tree, void *data);
		static s32			tree_erase(xtree *tree, void *data);

		static xiterator_t *iterator_new(x_iallocator* allocator);
		static void			iterator_delete(xiterator_t *trav);
		static void *		iterator_start(xiterator_t *trav, xtree *tree, s32 dir);
		static void *		iterator_move(xiterator_t *trav, s32 dir);

		static void *		iterator_first(xiterator_t *trav, xtree *tree);
		static void *		iterator_last(xiterator_t *trav, xtree *tree);
		static void *		iterator_next(xiterator_t *trav);
		static void *		iterator_prev(xiterator_t *trav);

		static s32			tree_validate(xtree *tree, xnode_t* root, const char*& result);
	};

	// <summary>
	//   Checks the color of a red black node
	// <summary>
	// <param name="root">The node to check</param>
	// <returns>1 for a red node, 0 for a black node</returns>
	// <remarks>For interator_ree.c internal use only</remarks>
	s32		xtree_internal::is_red(xnode_t *root)
	{
		return root != NULL && root->red == 1;
	}

	// <summary>
	//   Performs a single red black rotation in the specified direction
	//   This function assumes that all nodes are valid for a rotation
	// <summary>
	// <param name="root">The original root to rotate around</param>
	// <param name="dir">The direction to rotate (0 = left, 1 = right)</param>
	// <returns>The new root ater rotation</returns>
	// <remarks>For interator_ree.c internal use only</remarks>
	xnode_t *	xtree_internal::rotate_single(xnode_t *root, s32 dir)
	{
		xnode_t *save = root->link[!dir];

		root->link[!dir] = save->link[dir];
		save->link[dir] = root;

		root->red = 1;
		save->red = 0;

		return save;
	}

	// <summary>
	//    Performs a double red black rotation in the specified direction
	//    This function assumes that all nodes are valid for a rotation
	// <summary>
	// <param name="root">The original root to rotate around</param>
	// <param name="dir">The direction to rotate (0 = left, 1 = right)</param>
	// <returns>The new root after rotation</returns>
	// <remarks>For interator_ree.c internal use only</remarks>
	xnode_t *	xtree_internal::rotate_double(xnode_t *root, s32 dir)
	{
		root->link[!dir] = rotate_single(root->link[!dir], !dir);
		return rotate_single(root, dir);
	}

	// <summary>
	//   Creates an initializes a new red black node with a copy of
	//   the data. This function does not insert the new node into a tree
	// <summary>
	// <param name="tree">The red black tree this node is being created for</param>
	// <param name="data">The data value that will be stored in this node</param>
	// <returns>A pointer to the new node</returns>
	// <remarks>
	//   For interator_ree.c internal use only. The data for this node must
	//   be freed using the same tree's rel function. The returned pointer
	//   must be freed using C's free function
	// </remarks>
	xnode_t *	xtree_internal::node_new(xtree *tree, void *data)
	{
		xnode_t* rn = (xnode_t*)tree->m_node_allocator->allocate(sizeof(xnode_t), sizeof(void*));
		if (rn == NULL)
			return NULL;

		rn->red = 1;
		rn->data = data;
		rn->link[0] = rn->link[1] = NULL;

		return rn;
	}

	// <summary>
	//   Releases a valid red black tree
	// <summary>
	// <param name="tree">The tree to release</param>
	void xtree_internal::tree_clear(xtree *tree)
	{
		xnode_t *it = tree->m_root;
		xnode_t *save;

		// Rotate away the left links so that
		// we can treat this like the destruction
		// of a linked list
		while (it != NULL) 
		{
			if (it->link[0] == NULL) 
			{
				// No left links, just kill the node and move on
				save = it->link[1];
				if (tree->m_item_allocator!=NULL)
					tree->m_item_allocator->deallocate(it->data);
				tree->m_node_allocator->deallocate(it);
			}
			else 
			{
				// Rotate away the left link and check again 
				save = it->link[0];
				it->link[0] = save->link[1];
				save->link[1] = it;
			}

			it = save;
		}

		tree->m_root = NULL;
	}

	// <summary>
	//   Search for a copy of the specified
	//   node data in a red black tree
	// <summary>
	// <param name="tree">The tree to search</param>
	// <param name="data">The data value to search for</param>
	// <returns>
	//   A pointer to the data value stored in the tree,
	//   or a null pointer if no data could be found
	// </returns>
	bool	xtree_internal::tree_find(xtree *tree, void *data)
	{
		xnode_t *it = tree->m_root;

		while (it != NULL)
		{
			s32 cmp = tree->m_compare(it->data, data);

			if (cmp == 0)
				break;

			// If the tree supports duplicates, they should be
			// chained to the right subtree for this to work
			it = it->link[cmp < 0];
		}

		return it != NULL;
	}

	// <summary>
	//    Insert a copy of the user-specified
	//    data into a red black tree
	// <summary>
	// <param name="tree">The tree to insert into</param>
	// <param name="data">The data value to insert</param>
	// <returns>
	//    1 if the value was inserted successfully,
	//    0 if the insertion failed for any reason
	// </returns>
	s32		xtree_internal::tree_insert(xtree *tree, void *data)
	{
		if (tree->m_root == NULL)
		{
			// We have an empty tree; attach the
			// new node directly to the root
			tree->m_root = node_new(tree, data);

			if (tree->m_root == NULL)
				return 0;
		}
		else 
		{
			xnode_t head = { 0 };	// False tree root 
			xnode_t *g, *t;			// Grandparent & parent 
			xnode_t *p, *q;			// Iterator & parent 
			s32 dir = 0, last = 0;

			// Set up our helpers 
			t = &head;
			g = p = NULL;
			q = t->link[1] = tree->m_root;

			// Search down the tree for a place to insert 
			for (; ; )
			{
				if (q == NULL) 
				{
					// Insert a new node at the first null link 
					p->link[dir] = q = node_new(tree, data);

					if (q == NULL)
						return 0;
				}
				else if (is_red(q->link[0]) && is_red(q->link[1]))
				{
					// Simple red violation: color flip 
					q->red = 1;
					q->link[0]->red = 0;
					q->link[1]->red = 0;
				}

				if (is_red(q) && is_red(p)) 
				{
					// Hard red violation: rotations necessary
					s32 dir2 = t->link[1] == g;

					if (q == p->link[last])
						t->link[dir2] = rotate_single(g, !last);
					else
						t->link[dir2] = rotate_double(g, !last);
				}

				// Stop working if we inserted a node. This
				// check also disallows duplicates in the tree
				if (tree->m_compare(q->data, data) == 0)
					break;

				last = dir;
				dir = tree->m_compare(q->data, data) < 0;

				// Move the helpers down 
				if (g != NULL)
					t = g;

				g = p, p = q;
				q = q->link[dir];
			}

			// Update the root (it may be different) 
			tree->m_root = head.link[1];
		}

		// Make the root black for simplified logic 
		tree->m_root->red = 0;
		tree->m_size += 1;

		return 1;
	}


	// <summary>
	// 	  Remove a node from a red black tree
	// 	  that matches the user-specified data
	// <summary>
	// <param name="tree">The tree to remove from</param>
	// <param name="data">The data value to search for</param>
	// <returns>
	//    1 if the value was removed successfully,
	//    0 if the removal failed for any reason
	// </returns>
	// <remarks>
	//    The most common failure reason should be
	//    that the data was not found in the tree
	// </remarks>
	s32 xtree_internal::tree_erase(xtree *tree, void *data)
	{
		if (tree->m_root != NULL) 
		{
			xnode_t head = { 0 };	// False tree root
			xnode_t *q, *p, *g;		// Helpers
			xnode_t *f = NULL;		// Found item
			s32 dir = 1;

			// Set up our helpers
			q = &head;
			g = p = NULL;
			q->link[1] = tree->m_root;

			// Search and push a red node down
			// to fix red violations as we go
			while (q->link[dir] != NULL) 
			{
				s32 last = dir;

				// Move the helpers down
				g = p, p = q;
				q = q->link[dir];
				dir = tree->m_compare(q->data, data) < 0;

				// Save the node with matching data and keep
				// going; we'll do removal tasks at the end
				if (tree->m_compare(q->data, data) == 0)
					f = q;

				/* Push the red node down with rotations and color flips */
				if (!is_red(q) && !is_red(q->link[dir])) 
				{
					if (is_red(q->link[!dir]))
					{
						p = p->link[last] = rotate_single(q, dir);
					}
					else if (!is_red(q->link[!dir])) 
					{
						xnode_t *s = p->link[!last];

						if (s != NULL) 
						{
							if (!is_red(s->link[!last]) && !is_red(s->link[last])) 
							{
								// Color flip 
								p->red = 0;
								s->red = 1;
								q->red = 1;
							}
							else 
							{
								s32 dir2 = g->link[1] == p;

								if (is_red(s->link[last]))
								{
									g->link[dir2] = rotate_double(p, last);
								}
								else if (is_red(s->link[!last]))
								{
									g->link[dir2] = rotate_single(p, last);
								}

								// Ensure correct coloring 
								q->red = g->link[dir2]->red = 1;
								g->link[dir2]->link[0]->red = 0;
								g->link[dir2]->link[1]->red = 0;
							}
						}
					}
				}
			}

			// Replace and remove the saved node
			if (f != NULL) 
			{
				void* old_data = f->data;
				f->data = q->data;
				p->link[p->link[1] == q] = q->link[q->link[0] == NULL];
				if (tree->m_item_allocator!=NULL)
					tree->m_item_allocator->deallocate(old_data);
				tree->m_node_allocator->deallocate(q);
			}

			// Update the root (it may be different)
			tree->m_root = head.link[1];

			// Make the root black for simplified logic
			if (tree->m_root != NULL)
				tree->m_root->red = 0;

			tree->m_size -= 1;
		}

		return 1;
	}

	// <summary>
	//   Create a new traversal object
	// <summary>
	// <returns>A pointer to the new object</returns>
	// <remarks>
	//   The traversal object is not initialized until
	//   interator_first or interator_last are called.
	//   The pointer must be released with interator_delete
	// </remarks>
	xiterator_t *	xtree_internal::iterator_new(x_iallocator* allocator)
	{
		return (xiterator_t*)malloc(sizeof(xiterator_t));
	}

	// <summary>
	//    Release a traversal object
	// <summary>
	// <param name="trav">The object to release</param>
	// <remarks>
	//    The object must have been created with interator_new
	// </remarks>
	void			xtree_internal::iterator_delete(xiterator_t *trav)
	{
		free(trav);
	}

	// <summary>
	//    Initialize a traversal object. The user-specified
	//    direction determines whether to begin traversal at the
	//    smallest or largest valued node
	// <summary>
	// <param name="trav">The traversal object to initialize</param>
	// <param name="tree">The tree that the object will be attached to</param>
	// <param name="dir">
	//    The direction to traverse (0 = ascending, 1 = descending)
	// </param>
	// <returns>A pointer to the smallest or largest data value</returns>
	// <remarks>For interator_ree.c internal use only</remarks>
	void *	xtree_internal::iterator_start(xiterator_t *trav, xtree *tree, s32 dir)
	{
		trav->tree = tree;
		trav->it = tree->m_root;
		trav->top = 0;

		// Save the path for later traversal
		if (trav->it != NULL) 
		{
			while (trav->it->link[dir] != NULL) 
			{
				trav->path[trav->top++] = trav->it;
				trav->it = trav->it->link[dir];
			}
		}

		return trav->it == NULL ? NULL : trav->it->data;
	}

	// <summary>
	//    Traverse a red black tree in the user-specified direction
	// <summary>
	// <param name="trav">The initialized traversal object</param>
	// <param name="dir">
	//    The direction to traverse (0 = ascending, 1 = descending)
	// </param>
	// <returns>
	//    A pointer to the next data value in the specified direction
	// </returns>
	// <remarks>For interator_ree.c internal use only</remarks>
	void *	xtree_internal::iterator_move(xiterator_t *trav, s32 dir)
	{
		if (trav->it->link[dir] != NULL) 
		{
			// Continue down this branch
			trav->path[trav->top++] = trav->it;
			trav->it = trav->it->link[dir];

			while (trav->it->link[!dir] != NULL) 
			{
				trav->path[trav->top++] = trav->it;
				trav->it = trav->it->link[!dir];
			}
		}
		else 
		{
			// Move to the next branch 
			xnode_t *last;

			do 
			{
				if (trav->top == 0) 
				{
					trav->it = NULL;
					break;
				}

				last = trav->it;
				trav->it = trav->path[--trav->top];
			} while (last == trav->it->link[dir]);
		}

		return trav->it == NULL ? NULL : trav->it->data;
	}

	// <summary>
	//    Initialize a traversal object to the smallest valued node
	// <summary>
	// <param name="trav">The traversal object to initialize</param>
	// <param name="tree">The tree that the object will be attached to</param>
	// <returns>A pointer to the smallest data value</returns>
	void *	xtree_internal::iterator_first(xiterator_t *trav, xtree *tree)
	{
		return iterator_start(trav, tree, 0); // Min value
	}

	// <summary>
	//    Initialize a traversal object to the largest valued node
	// <summary>
	// <param name="trav">The traversal object to initialize</param>
	// <param name="tree">The tree that the object will be attached to</param>
	// <returns>A pointer to the largest data value</returns>
	void *	xtree_internal::iterator_last(xiterator_t *trav, xtree *tree)
	{
		return iterator_start(trav, tree, 1); // Max value
	}

	// <summary>
	//    Traverse to the next value in ascending order
	// <summary>
	// <param name="trav">The initialized traversal object</param>
	// <returns>A pointer to the next value in ascending order</returns>
	void *	xtree_internal::iterator_next(xiterator_t *trav)
	{
		return iterator_move(trav, 1); // Toward larger items 
	}

	// <summary>
	//    Traverse to the next value in descending order
	// <summary>
	// <param name="trav">The initialized traversal object</param>
	// <returns>A pointer to the next value in descending order</returns>
	void *	xtree_internal::iterator_prev(xiterator_t *trav)
	{
		return iterator_move(trav, 0); // Toward smaller items
	}


	// Test the integrity of the red-black tree
	// @return: The depth of the tree
	// @result: If any error it returns a description of the error in 'result', when no error it will be NULL
	inline s32 xtree_internal::tree_validate(xtree *tree, xnode_t* root, const char*& result)
	{
		if (root == NULL)
		{
			return 1;
		}
		else
		{
			xnode_t* ln = root->link[0];
			xnode_t* rn = root->link[1];

			// Consecutive red links
			if (is_red(root))
			{
				if (is_red(ln) || is_red(rn))
				{
					result = "Red violation";
					return 0;
				}
			}

			s32 lh = tree_validate(tree, ln, result);
			s32 rh = tree_validate(tree, rn, result);

			// Invalid binary search tree 
			if ((ln != NULL && tree->m_compare(ln->data, root->data) >= 0) || (rn != NULL && tree->m_compare(rn->data, root->data) <= 0))
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
				return is_red(root) ? lh : lh + 1;
			}
			return 0;
		}
	}


	xtree::xtree(x_iallocator* node_allocator, x_iallocator* item_allocator)
		: m_node_allocator(node_allocator)
		, m_item_allocator(item_allocator)
		, m_compare(&compare_void)
		, m_root(NULL)
		, m_size(0)
	{
	}

	void			xtree::clear()
	{
		xtree_internal::tree_clear(this);
	}

	bool			xtree::find(void * data)
	{
		return xtree_internal::tree_find(this, data);
	}

	s32				xtree::insert(void * data)
	{
		return xtree_internal::tree_insert(this, data);
	}

	s32				xtree::erase(void * data)
	{
		return xtree_internal::tree_erase(this, data);
	}

	bool			xtree::validate(const char*& error_str)
	{
		s32 depth = xtree_internal::tree_validate(this, m_root, error_str);
		return (error_str == NULL);
	}
}

