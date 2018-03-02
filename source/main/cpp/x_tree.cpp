#include "xbase/x_allocator.h"
#include "xbase/x_tree.h"

#include <string>

namespace xcore
{


	const s32 TRAVERSE_LEFT = 0;
	const s32 TRAVERSE_RIGHT = 1;
	const s32 VISITED = 2;

	const s32 ITERATE_PREORDER = 1;
	const s32 ITERATE_SORTORDER = 2;
	const s32 ITERATE_POSTORDER = 3;
	const s32 ITERATE_LEVELORDER = 4;


	//                            +-+
	//                            |F|
	//                         ___+-+___
	//                       _/         \_
	//                     _/             \_
	//                   _/                 \_
	//                +-+                     +-+
	//                |B|                     |G|
	//              _/+-+\_                   +-+
	//             /       \                     \__
	//          +-+         +-+                     \+-+
	//          |A|         |D|                      |I|
	//          +-+       _/+-+\                  __/+-+
	//                   /      \                /
	//                +-+        \+-+         +-+
	//                |C|         |E|         |H|
	//                +-+         +-+         +-+

	// preorder: F B A D C E F I H
	// sortorder: A B C D E F G H I 
	// postorder: A C E D B H I G F
	// levelorder: F B G A D I C E H

	void		xtree_iterator::init_preorder()
	{
		clear();
		push(m_tree->m_root, TRAVERSE_LEFT);
		m_traversal = ITERATE_PREORDER;
	}

	void		xtree_iterator::init_sortorder()
	{
		clear();
		push(m_tree->m_root, TRAVERSE_LEFT);
		m_traversal = ITERATE_SORTORDER;
	}

	void		xtree_iterator::init_postorder()
	{
		clear();
		push(m_tree->m_root, TRAVERSE_LEFT);
		m_traversal = ITERATE_POSTORDER;
	}

	void		xtree_iterator::init_levelorder()
	{
		clear();
		push(m_tree->m_root, TRAVERSE_LEFT);
		m_traversal = ITERATE_LEVELORDER;
	}

	bool		xtree_iterator::iterate(void *& data)
	{
		switch (m_traversal)
		{
		case ITERATE_PREORDER:
			return preorder(data);
		case ITERATE_SORTORDER:
			return sortorder(data);
		case ITERATE_POSTORDER:
			return postorder(data);
		case ITERATE_LEVELORDER:
			return levelorder(data);
		}
		return false;
	}

	bool		xtree_iterator::preorder(void *& data)
	{
		xbyte state;
		xnode_t* node = pop(state);

		while (node != nullptr)
		{
			if (state == TRAVERSE_LEFT)
			{
				if (node->get_left() != nullptr)
				{
					push(node, TRAVERSE_RIGHT);
					push(node->get_left(), TRAVERSE_LEFT);
					break;
				}
				else
				{
					state = TRAVERSE_RIGHT;
				}
			}

			if (state == TRAVERSE_RIGHT)
			{
				xnode_t* right = node->get_right();
				if (right != nullptr)
				{
					push(node, VISITED);
					push(right, TRAVERSE_LEFT);
				}
				else
				{
					state = VISITED;
				}
			}
			node = pop(state);
		}

		if (node == nullptr)
			return false;

		data = node->data;
		return true;
	}

	bool		xtree_iterator::sortorder(void *& data)
	{
		xbyte state;
		xnode_t* node = pop(state);

		while (node != nullptr)
		{
			if (state == TRAVERSE_LEFT)
			{
				push(node, TRAVERSE_RIGHT);
				// Traverse fully to the left again
				while (node->get_left() != nullptr)
				{
					node = node->get_left();
					push(node, TRAVERSE_RIGHT);
				}
			}
			else if (state == TRAVERSE_RIGHT)
			{
				xnode_t* right = node->get_right();
				if (right != nullptr)
				{
					push(node, VISITED);
					push(right, TRAVERSE_LEFT);
				}
				break;
			}
			node = pop(state);
		}

		if (node == nullptr)
			return false;

		data = node->data;
		return true;
	}

	bool		xtree_iterator::postorder(void *& data)
	{
		xbyte state;
		xnode_t* node = pop(state);

		while (node != nullptr)
		{
			if (state == TRAVERSE_LEFT)
			{
				push(node, TRAVERSE_RIGHT);
				// Traverse fully to the left again
				while (node->get_left() != nullptr)
				{
					node = node->get_left();
					push(node, TRAVERSE_RIGHT);
				}
			}
			else if (state == TRAVERSE_RIGHT)
			{
				xnode_t* right = node->get_right();
				if (right != nullptr)
				{
					push(node, VISITED);
					push(right, TRAVERSE_LEFT);
				}
				else
				{
					push(node, VISITED);
				}
			}
			else
			{
				break;
			}
			node = pop(state);
		}

		if (node == nullptr)
			return false;

		data = node->data;
		return true;
	}

	bool		xtree_iterator::levelorder(void *& data)
	{
		xnode_t* node = dequeue();
		if (node == nullptr)
			return false;

		enqueue(node->get_left());
		enqueue(node->get_right());

		data = node->data;
		return true;
	}

	void		xtree_iterator::clear()
	{
		m_top = 0;
		m_bottom = 0;
	}

	xnode_t*	xtree_iterator::pop(xbyte& state)
	{
		if (m_top == 0)
			return nullptr;
		xnode_t* n = m_path[--m_top];
		state = m_state[m_top];
		return n;
	}

	void		xtree_iterator::push(xnode_t* n, xbyte state)
	{
		if (n != nullptr)
		{
			m_state[m_top] = state;
			m_path[m_top++] = n;
		}
	}

	void		xtree_iterator::enqueue(xnode_t* n)
	{
		if (n != nullptr)
		{
			m_path[m_top] = n;
			m_top = m_top % HEIGHT_LIMIT;
		}
	}

	xnode_t*	xtree_iterator::dequeue()
	{
		if (m_bottom == m_top)
			return nullptr;
		xnode_t* n = m_path[m_bottom];
		m_bottom = m_bottom % HEIGHT_LIMIT;
		return n;
	}


	typedef s32		(*cmp_f) (const void *p1, const void *p2);

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

		static xnode_t *	node_new(xtree *tree, void * data);
		static bool			tree_clear(xtree *tree, void *& data);

		static bool			tree_find(xtree *tree, void * data, void *& found, xtree_iterator& iter);
		static s32			tree_insert(xtree *tree, void * data);
		static s32			tree_remove(xtree *tree, void * data);

		//static xiterator_t *iterator_new(x_iallocator* allocator);
		//static void			iterator_delete(xiterator_t *trav);
		//static void *		iterator_start(xiterator_t *trav, xtree *tree, s32 dir);
		//static void *		iterator_move(xiterator_t *trav, s32 dir);
		//
		//static void *		iterator_first(xiterator_t *trav, xtree *tree);
		//static void *		iterator_last(xiterator_t *trav, xtree *tree);
		//static void *		iterator_next(xiterator_t *trav);
		//static void *		iterator_prev(xiterator_t *trav);

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
		return root != NULL && root->is_red();
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
		xnode_t *save = root->get_child(!dir);

		root->set_child(!dir, save->get_child(dir));
		save->set_child(dir, root);

		root->set_red();
		save->set_black();

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
		//root->link[!dir] = rotate_single(root->link[!dir], !dir);
		xnode_t* child = rotate_single(root->get_child(!dir), !dir);
		root->set_child(!dir, child);
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
	xnode_t *	xtree_internal::node_new(xtree *tree, void * data)
	{
		xnode_t* rn = (xnode_t*)tree->m_node_allocator->allocate(sizeof(xnode_t), sizeof(void*));
		if (rn == NULL)
			return NULL;

		rn->set_red();
		rn->data = data;
		rn->set_left(NULL);
		rn->set_right(NULL);

		return rn;
	}

	// <summary>
	//   Releases a valid red black tree iteratively
	//   You will have to call 'clear' repeatedly until
	//   you get 'true', this is when the tree is empty.
	// <summary>
	// <param name="tree">The tree to release</param>
	bool xtree_internal::tree_clear(xtree *tree, void *& data)
	{
		data = NULL;
		if (tree->m_root == NULL)
			return true;

		xnode_t* todelete = tree->m_root;
		if (tree->m_root->get_left() == NULL)
		{
			tree->m_root = tree->m_root->get_right();
		}
		else if (tree->m_root->get_right() == NULL)
		{
			tree->m_root = tree->m_root->get_left();
		}
		else 
		{
			// We have left and right branches
			// Take right branch and place it
			// somewhere down the left branch
			xnode_t* branch = tree->m_root->get_right();
			tree->m_root->set_right(NULL);

			// Find a node in the left branch that does not
			// have both a left and right branch and place
			// our branch there.
			xnode_t* iter = tree->m_root->get_left();
			while (iter->get_left() != NULL && iter->get_right() != NULL)
			{
				iter = iter->get_left();
			}
			if (iter->get_left() == NULL)
			{
				iter->set_left(branch);
			}
			else if (iter->get_right() == NULL)
			{
				iter->set_right(branch);
			}

			tree->m_root = tree->m_root->get_left();
		}

		data = todelete->data;
		tree->m_node_allocator->deallocate(todelete);
		todelete = NULL;
		tree->m_size -= 1;

		return false;
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
	bool	xtree_internal::tree_find(xtree *tree, void * data, void *& found, xtree_iterator& iter)
	{
		xnode_t * it = tree->m_root;
		while (it != NULL)
		{
			s32 const cmp = tree->m_compare(it->data, data);
			if (cmp == 0)
				break;

			// Depending on the traversal mode we need to push the correct state
			iter.push(it, 2);

			// If the tree supports duplicates, they should be
			// chained to the right subtree for this to work
			it = it->get_child(cmp < 0 ? 1 : 0);
		}
		
		if (it != NULL)
		{
			found = it->data;
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
	s32		xtree_internal::tree_insert(xtree *tree, void * data)
	{
		s32 result = 0;
		if (tree->m_root == NULL)
		{
			// We have an empty tree; attach the
			// new node directly to the root
			tree->m_root = node_new(tree, data);
			result = 1;
			if (tree->m_root == NULL)
				return result;
		}
		else 
		{
			xnode_t head;			// False tree root 
			xnode_t *g, *t;			// Grandparent & parent 
			xnode_t *p, *q;			// Iterator & parent 
			s32 dir = 0, last = 0;

			// Set up our helpers 
			t = &head;
			g = p = NULL;
			q = tree->m_root;
			t->set_right(tree->m_root);

			// Search down the tree for a place to insert 
			for (; ; )
			{
				if (q == NULL) 
				{
					// Insert a new node at the first null link 
					result = 1;
					q = node_new(tree, data);
					p->set_child(dir, q);

					if (q == NULL)
						return result;
				}
				else if (is_red(q->get_left()) && is_red(q->get_right()))
				{
					// Simple red violation: color flip 
					q->set_red();
					q->get_left()->set_black();
					q->get_right()->set_black();
				}

				if (is_red(q) && is_red(p)) 
				{
					// Hard red violation: rotations necessary
					s32 dir2 = t->get_right() == g;

					xnode_t* rotated;
					if (q == p->get_child(last))
						rotated = rotate_single(g, !last);
					else
						rotated = rotate_double(g, !last);

					t->set_child(dir2, rotated);
				}

				// Stop working if we inserted a node. This
				// check also disallows duplicates in the tree
				dir = tree->m_compare(q->data, data);
				if (dir == 0)
					break;
				dir = 1 - ((dir + 1) >> 1);

				last = dir;

				// Move the helpers down 
				if (g != NULL)
					t = g;

				g = p, p = q;
				q = q->get_child(dir);
			}

			// Update the root (it may be different) 
			tree->m_root = head.get_right();
		}

		// Make the root black for simplified logic 
		tree->m_root->set_black();
		if (result == 1)
		{
			tree->m_size += 1;
		}
		return result;
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
	s32 xtree_internal::tree_remove(xtree *tree, void * data)
	{
		if (tree->m_root != NULL) 
		{
			xnode_t head;			// False tree root
			xnode_t *q, *p, *g;		// Helpers
			xnode_t *f = NULL;		// Found item
			s32 dir = 1;

			// Set up our helpers
			q = &head;
			g = p = NULL;
			q->set_right(tree->m_root);

			// Search and push a red node down
			// to fix red violations as we go
			while (q->get_child(dir) != NULL) 
			{
				s32 last = dir;

				// Move the helpers down
				g = p, p = q;
				q = q->get_child(dir);
				dir = tree->m_compare(q->data, data) < 0;

				// Save the node with matching data and keep
				// going; we'll do removal tasks at the end
				if (tree->m_compare(q->data, data) == 0)
					f = q;

				/* Push the red node down with rotations and color flips */
				if (!is_red(q) && !is_red(q->get_child(dir))) 
				{
					if (is_red(q->get_child(!dir)))
					{
						xnode_t* r = rotate_single(q, dir);
						p->set_child(last, r);
						p = r;
					}
					else if (!is_red(q->get_child(!dir))) 
					{
						xnode_t *s = p->get_child(!last);

						if (s != NULL) 
						{
							if (!is_red(s->get_child(!last)) && !is_red(s->get_child(last)))
							{
								// Color flip 
								p->set_black();
								s->set_red();
								q->set_red();
							}
							else 
							{
								s32 dir2 = g->get_right() == p;

								if (is_red(s->get_child(last)))
								{
									g->set_child(dir2, rotate_double(p, last));
								}
								else if (is_red(s->get_child(!last)))
								{
									g->set_child(dir2, rotate_single(p, last));
								}

								// Ensure correct coloring 
								q->set_red();
								g->get_child(dir2)->set_red();

								g->get_child(dir2)->get_left()->set_black();
								g->get_child(dir2)->get_right()->set_black();
							}
						}
					}
				}
			}

			// Replace and remove the saved node
			if (f != NULL) 
			{
				void const* old_data = f->data;
				f->data = q->data;
				xnode_t* child1 = q->get_child(q->get_left() == NULL);
				p->set_child(p->get_right() == q, child1);
				tree->m_node_allocator->deallocate(q);
			}

			// Update the root (it may be different)
			tree->m_root = head.get_right();

			// Make the root black for simplified logic
			if (tree->m_root != NULL)
				tree->m_root->set_black();

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
	//xiterator_t *	xtree_internal::iterator_new(x_iallocator* allocator)
	//{
	//	return (xiterator_t*)malloc(sizeof(xiterator_t));
	//}
	//
	// <summary>
	//    Release a traversal object
	// <summary>
	// <param name="trav">The object to release</param>
	// <remarks>
	//    The object must have been created with interator_new
	// </remarks>
	//void			xtree_internal::iterator_delete(xiterator_t *trav)
	//{
	//	free(trav);
	//}

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
	//void *	xtree_internal::iterator_start(xiterator_t *trav, xtree *tree, s32 dir)
	//{
	//	trav->tree = tree;
	//	trav->it = tree->m_root;
	//	trav->top = 0;
	//
	//	// Save the path for later traversal
	//	if (trav->it != NULL) 
	//	{
	//		while (trav->it->link[dir] != NULL) 
	//		{
	//			trav->path[trav->top++] = trav->it;
	//			trav->it = trav->it->link[dir];
	//		}
	//	}
	//
	//	return trav->it == NULL ? NULL : trav->it->data;
	//}

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
	//void *	xtree_internal::iterator_move(xiterator_t *trav, s32 dir)
	//{
	//	if (trav->it->link[dir] != NULL) 
	//	{
	//		// Continue down this branch
	//		trav->path[trav->top++] = trav->it;
	//		trav->it = trav->it->link[dir];
	//
	//		while (trav->it->link[!dir] != NULL) 
	//		{
	//			trav->path[trav->top++] = trav->it;
	//			trav->it = trav->it->link[!dir];
	//		}
	//	}
	//	else 
	//	{
	//		// Move to the next branch 
	//		xnode_t *last;
	//
	//		do 
	//		{
	//			if (trav->top == 0) 
	//			{
	//				trav->it = NULL;
	//				break;
	//			}
	//
	//			last = trav->it;
	//			trav->it = trav->path[--trav->top];
	//		} while (last == trav->it->link[dir]);
	//	}
	//
	//	return trav->it == NULL ? NULL : trav->it->data;
	//}

	// <summary>
	//    Initialize a traversal object to the smallest valued node
	// <summary>
	// <param name="trav">The traversal object to initialize</param>
	// <param name="tree">The tree that the object will be attached to</param>
	// <returns>A pointer to the smallest data value</returns>
	//void *	xtree_internal::iterator_first(xiterator_t *trav, xtree *tree)
	//{
	//	return iterator_start(trav, tree, 0); // Min value
	//}

	// <summary>
	//    Initialize a traversal object to the largest valued node
	// <summary>
	// <param name="trav">The traversal object to initialize</param>
	// <param name="tree">The tree that the object will be attached to</param>
	// <returns>A pointer to the largest data value</returns>
	//void *	xtree_internal::iterator_last(xiterator_t *trav, xtree *tree)
	//{
	//	return iterator_start(trav, tree, 1); // Max value
	//}

	// <summary>
	//    Traverse to the next value in ascending order
	// <summary>
	// <param name="trav">The initialized traversal object</param>
	// <returns>A pointer to the next value in ascending order</returns>
	//void *	xtree_internal::iterator_next(xiterator_t *trav)
	//{
	//	return iterator_move(trav, 1); // Toward larger items 
	//}

	// <summary>
	//    Traverse to the next value in descending order
	// <summary>
	// <param name="trav">The initialized traversal object</param>
	// <returns>A pointer to the next value in descending order</returns>
	//void *	xtree_internal::iterator_prev(xiterator_t *trav)
	//{
	//	return iterator_move(trav, 0); // Toward smaller items
	//}


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
			xnode_t* ln = root->get_left();
			xnode_t* rn = root->get_right();

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


	xtree::xtree(x_iallocator* node_allocator)
		: m_node_allocator(node_allocator)
		, m_compare(&compare_void)
		, m_root(NULL)
		, m_size(0)
	{
	}

	bool			xtree::clear(void *& data)
	{
		return xtree_internal::tree_clear(this, data);
	}

	bool			xtree::find(void * data, void *& found)
	{
		xtree_iterator iter;
		iterate(iter);
		iter.init_sortorder();
		return xtree_internal::tree_find(this, data, found, iter);
	}

	s32				xtree::insert(void * data)
	{
		return xtree_internal::tree_insert(this, data);
	}

	s32				xtree::remove(void * data)
	{
		return xtree_internal::tree_remove(this, data);
	}

	bool			xtree::validate(const char*& error_str)
	{
		s32 depth = xtree_internal::tree_validate(this, m_root, error_str);
		return (error_str == NULL);
	}

	void			xtree::iterate(xtree_iterator& iter)
	{
		iter.clear();
		iter.m_tree = this;
	}

}

