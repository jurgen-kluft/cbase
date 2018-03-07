#include "xbase/x_allocator.h"
#include "xbase/x_tree.h"
#include "xbase/x_bit_field.h"

#include <string>

namespace xcore
{
	typedef s32(*cmp_f) (const void *p1, const void *p2);

	s32				compare_void(void const* p1, void const* p2)
	{
		if (p1 < p2)
			return -1;
		else if (p1 > p2)
			return 1;
		return 0;
	}

	#define rbnil(t)			(&(t)->m_nill)
	#define rbfirst(t)			((t)->m_root.get_left())
	#define rbroot(t)			(&(t)->m_root)

	#define rbsetfirst(t, n)	((t)->m_root.set_left(n))

	class xtree_internal
	{
	public:
		static inline s32		is_red(xtree * tree, xtree::node * n) { return n != rbnil(tree) && n->is_red(); }

		static void				rotate_left(xtree* tree, xtree::node* node);
		static void				rotate_right(xtree* tree, xtree::node* node);

		static xtree::node *	tree_successor(xtree * tree, xtree::node* node);

		static xtree::node *	tree_insert(xtree* tree, void * data);
		static xtree::node *	tree_find(xtree* tree, void * data);
		static bool				tree_clear(xtree *tree, void *& data);
		static void				tree_repair(xtree * tree, xtree::node* node);
		static void *			tree_remove(xtree * tree, xtree::node* node);

		static s32				tree_validate(xtree *tree, xtree::node* root, const char*& result);
	};


	void xtree_internal::rotate_left(xtree* tree, xtree::node* node)
	{
		xtree::node * child;

		child = node->get_right();
		node->set_right(child->get_left());

		if (child->get_left() != rbnil(tree))
			child->get_left()->set_parent(node);

		child->set_parent(node->get_parent());

		if (node == node->get_parent()->get_left())
			node->get_parent()->set_left(child);
		else
			node->get_parent()->set_right(child);

		child->set_left(node);
		node->set_parent(child);
	}

	void xtree_internal::rotate_right(xtree* tree, xtree::node* node)
	{
		xtree::node * child;

		child = node->get_left();
		node->set_left(child->get_right());

		if (child->get_right() != rbnil(tree))
			child->get_right()->set_parent(node);
		child->set_parent(node->get_parent());

		if (node == node->get_parent()->get_left())
			node->get_parent()->set_left(child);
		else
			node->get_parent()->set_right(child);

		child->set_right(node);
		node->set_parent(child);
	}

	// Returns the successor of node, or nil if there is none.
	xtree::node *  xtree_internal::tree_successor(xtree * tree, xtree::node* node)
	{
		xtree::node * succ = succ = node->get_right();
		if (succ != rbnil(tree)) 
		{
			while (succ->get_left() != rbnil(tree))
				succ = succ->get_left();
		}
		else 
		{
			// No right child, move up until we find it or hit the root 
			for (succ = node->get_parent(); node == succ->get_right(); succ = succ->get_parent())
				node = succ;

			if (succ == rbroot(tree))
				succ = rbnil(tree);
		}
		return(succ);
	}


	// Delete node 'z' from the tree and return its data pointer.
	void * xtree_internal::tree_remove(xtree * tree, xtree::node * z)
	{
		xtree::node *x, *y;

		void * data = z->get_data();

		if (z->get_left() == rbnil(tree) || z->get_right() == rbnil(tree))
		{
			y = z;
		}
		else
		{
			y = tree_successor(tree, z);
		}

		x = (y->get_left() == rbnil(tree)) ? y->get_right() : y->get_left();

		x->set_parent(y->get_parent());
		if (x->get_parent() == rbroot(tree))
		{
			rbsetfirst(tree, x);
		}
		else
		{
			if (y == y->get_parent()->get_left())
				y->get_parent()->set_left(x);
			else
				y->get_parent()->set_right(x);
		}
		if (y->is_black())
			tree_repair(tree, x);

		if (y != z)
		{
			y->set_left(z->get_left());
			y->set_right(z->get_right());
			y->set_parent(z->get_parent());
			y->set_color(z->get_color());
			z->get_left()->set_parent(y);
			z->get_right()->set_parent(y);

			if (z == z->get_parent()->get_left())
			{
				z->get_parent()->set_left(y);
			}
			else
			{
				z->get_parent()->set_right(y);
			}
		}

		tree->m_node_allocator->deallocate(z);
		return (data);
	}

	// Repair the tree after a node has been deleted by rotating and repainting
	// colors to restore the 4 properties inherent in red-black trees.
	void xtree_internal::tree_repair(xtree * tree, xtree::node* node)
	{
		xtree::node * sibling;

		while (node->is_black())
		{
			if (node == node->get_parent()->get_left())
			{
				sibling = node->get_parent()->get_right();
				if (sibling->is_red())
				{
					sibling->set_black();
					node->get_parent()->set_red();
					rotate_left(tree, node->get_parent());
					sibling = node->get_parent()->get_right();
				}
				if (sibling->get_right()->is_black() && sibling->get_left()->is_black())
				{
					sibling->set_red();
					node = node->get_parent();
				}
				else 
				{
					if (sibling->get_right()->is_black())
					{
						sibling->get_left()->set_black();
						sibling->set_red();
						rotate_right(tree, sibling);
						sibling = node->get_parent()->get_right();
					}
					sibling->set_color(node->get_parent()->get_color());
					node->get_parent()->set_black();
					sibling->get_right()->set_black();
					rotate_left(tree, node->get_parent());
					break;
				}
			}
			else // if (node == node->get_parent()->get_right())
			{	
				sibling = node->get_parent()->get_left();
				if (sibling->is_red()) 
				{
					sibling->set_black();
					node->get_parent()->set_red();
					rotate_right(tree, node->get_parent());
					sibling = node->get_parent()->get_left();
				}
				if (sibling->get_right()->is_black() && sibling->get_left()->is_black())
				{
					sibling->set_red();
					node = node->get_parent();
				}
				else 
				{
					if (sibling->get_left()->is_black())
					{
						sibling->get_right()->set_black();
						sibling->set_red();
						rotate_left(tree, sibling);
						sibling = node->get_parent()->get_left();
					}
					sibling->set_color(node->get_parent()->get_color());
					node->get_parent()->set_black();
					sibling->get_left()->set_black();
					rotate_right(tree, node->get_parent());
					break;
				}
			}
		}
	}

	// Insert data pointer into a redblack tree.
	// Returns a NULL pointer on success.  If a node matching "data"
	// already exists, a pointer to the existant node is returned.
	xtree::node * xtree_internal::tree_insert(xtree* tree, void * data)
	{
		xtree::node * node = rbfirst(tree);
		xtree::node * parent = rbroot(tree);

		// Find correct insertion point.
		while (node != rbnil(tree))
		{
			parent = node;
			s32 res = tree->m_compare(data, node->get_data());
			if (res == 0)
				return(node);
			node = res < 0 ? node->get_left() : node->get_right();
		}

		node = (xtree::node*) tree->m_node_allocator->allocate(sizeof(xtree::node), sizeof(void*));
		node->set_data(data);
		node->set_right(rbnil(tree));
		node->set_left(rbnil(tree));
		node->set_parent(parent);
		if (parent == rbroot(tree) || tree->m_compare(data, parent->get_data()) < 0)
			parent->set_left(node);
		else
			parent->set_right(node);
		node->set_red();

		// If the parent node is black we are all set, if it is red we have
		// the following possible cases to deal with.  We iterate through
		// the rest of the tree to make sure none of the required properties
		// is violated.
		//
		//	1) The uncle is red.  We repaint both the parent and uncle black
		//     and repaint the grandparent node red.
		//
		//  2) The uncle is black and the new node is the right child of its
		//     parent, and the parent in turn is the left child of its parent.
		//     We do a left rotation to switch the roles of the parent and
		//     child, relying on further iterations to fixup the old parent.
		//
		//  3) The uncle is black and the new node is the left child of its
		//     parent, and the parent in turn is the left child of its parent.
		//     We switch the colors of the parent and grandparent and perform
		//     a right rotation around the grandparent.  This makes the former
		//     parent the parent of the new node and the former grandparent.
		//
		// Note that because we use a sentinel for the root node we never
		// need to worry about replacing the root.

		while (node->get_parent()->is_red())
		{
			xtree::node * uncle;
			if (node->get_parent() == node->get_parent()->get_parent()->get_left())
			{
				uncle = node->get_parent()->get_parent()->get_right();
				if (uncle->is_red()) {
					node->get_parent()->set_black();
					uncle->set_black();
					node->get_parent()->get_parent()->set_red();
					node = node->get_parent()->get_parent();
				}
				else /* if (uncle->color == black) */
				{
					if (node == node->get_parent()->get_right())
					{
						node = node->get_parent();
						rotate_left(tree, node);
					}
					node->get_parent()->set_black();
					node->get_parent()->get_parent()->set_red();
					rotate_right(tree, node->get_parent()->get_parent());
				}
			}
			else
			{	// if (node->get_parent() == node->get_parent()->get_parent()->get_right()) 
				uncle = node->get_parent()->get_parent()->get_left();
				if (uncle->is_red())
				{
					node->get_parent()->set_black();
					uncle->set_black();
					node->get_parent()->get_parent()->set_red();
					node = node->get_parent()->get_parent();
				}
				else // if (uncle->is_black()) 
				{
					if (node == node->get_parent()->get_left())
					{
						node = node->get_parent();
						rotate_right(tree, node);
					}
					node->get_parent()->set_black();
					node->get_parent()->get_parent()->set_red();
					rotate_left(tree, node->get_parent()->get_parent());
				}
			}
		}
		rbfirst(tree)->set_black();	// first node is always black
		return(nullptr);
	}

	// Look for a node matching key in tree.
	// Returns a pointer to the node if found, else NULL.
	xtree::node * xtree_internal::tree_find(xtree * tree, void * key)
	{
		xtree::node * node = rbfirst(tree);
		while (node != rbnil(tree))
		{
			s32 c = tree->m_compare(key, node->get_data());
			if (c == 0)
			{
				return(node);
			}
			c = (c+1) >> 1;
			node = node->get_child(c);
		}
		return(NULL);
	}

	// <summary>
	//   Releases a valid red black tree iteratively
	//   You will have to call 'clear' repeatedly until
	//   you get 'true', this is when the tree is empty.
	// <summary>
	// <param name="tree">The tree to release</param>
	bool xtree_internal::tree_clear(xtree *tree, void *& data)
	{
		data = nullptr;

		xtree::node * node = rbfirst(tree);
		if (node == rbnil(tree))
			return true;

		xtree::node* todelete = tree->m_root.get_left();
		if (node->get_left() == rbnil(tree))
		{
			tree->m_root.set_left(node->get_right());
		}
		else if (node->get_right() == rbnil(tree))
		{
			tree->m_root.set_left(node->get_left());
		}
		else
		{
			// We have left and right branches
			// Take right branch and place it
			// somewhere down the left branch
			xtree::node* branch = node->get_right();
			node->set_right(rbnil(tree));

			// Find a node in the left branch that does not
			// have both a left and right branch and place
			// our branch there.
			xtree::node* iter = node->get_left();
			while (iter->get_left() != rbnil(tree) && iter->get_right() != rbnil(tree))
			{
				iter = iter->get_left();
			}
			if (iter->get_left() == rbnil(tree))
			{
				iter->set_left(branch);
			}
			else if (iter->get_right() == rbnil(tree))
			{
				iter->set_right(branch);
			}

			tree->m_root.set_left(node->get_left());
		}

		data = todelete->get_data();
		tree->m_node_allocator->deallocate(todelete);
		tree->m_size -= 1;
		todelete = nullptr;

		return false;
	}

	// Test the integrity of the red-black tree
	// @return: The depth of the tree
	// @result: If any error it returns a description of the error in 'result', when no error it will be NULL
	s32 xtree_internal::tree_validate(xtree *tree, xtree::node* root, const char*& result)
	{
		if (root == rbnil(tree))
		{
			return 1;
		}
		else
		{
			xtree::node* ln = root->get_left();
			xtree::node* rn = root->get_right();

			// Consecutive red links
			if (is_red(tree, root))
			{
				if (is_red(tree, ln) || is_red(tree, rn))
				{
					result = "Red violation";
					return 0;
				}
			}

			s32 const lh = tree_validate(tree, ln, result);
			s32 const rh = tree_validate(tree, rn, result);

			// Invalid binary search tree 
			if ((ln != rbnil(tree) && tree->m_compare(ln->get_data(), root->get_data()) >= 0) || (rn != rbnil(tree) && tree->m_compare(rn->get_data(), root->get_data()) <= 0))
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
				return is_red(tree, root) ? lh : lh + 1;
			}
			return 0;
		}
	}

	xtree::xtree(x_iallocator* node_allocator)
		: m_node_allocator(node_allocator)
		, m_compare(&compare_void)
		, m_size(0)
	{
		// We use a self-referencing sentinel node called nil to simplify the
		// code by avoiding the need to check for NULL pointers.
		m_nill.set_parent(&m_nill);
		m_nill.set_left(&m_nill);
		m_nill.set_right(&m_nill);
		m_nill.set_black();
		m_nill.set_data(nullptr);

		// Similarly, the fake root node keeps us from having to worry
		// about splitting the root.
		m_root.set_parent(&m_nill);
		m_root.set_left(&m_nill);
		m_root.set_right(&m_nill);
		m_root.set_black();
		m_root.set_data(nullptr);
	}

	bool			xtree::clear(void *& data)
	{
		return xtree_internal::tree_clear(this, data);
	}

	bool			xtree::find(void * data, void *& found)
	{
		xtree::node* node = xtree_internal::tree_find(this, data);
		if (node == nullptr)
			return false;
		found = node->get_data();
		return true;
	}

	bool			xtree::insert(void * data)
	{
		xtree::node* node = xtree_internal::tree_insert(this, data);
		if (node == nullptr)
		{
			m_size++;
			return true;
		}
		return false;
	}

	bool			xtree::remove(void * data)
	{
		xtree::node* node = xtree_internal::tree_find(this, data);
		if (node == nullptr)
			return false;
		xtree_internal::tree_remove(this, node);
		return true;
	}

	bool			xtree::validate(const char*& error_str)
	{
		s32 depth = xtree_internal::tree_validate(this, rbfirst(this), error_str);
		return (error_str == NULL);
	}

	void			xtree::iterate(xtree::iterator& iter)
	{
		iter.m_tree = this;
		iter.m_it = nullptr;
	}


	bool		xtree::iterator::traverse(s32 d, void *& data)
	{
		if (m_it == nullptr)
		{
			m_it = rbfirst(m_tree);
		}
		else 
		{
			if (d == cLeft)
			{
				m_it = m_it->get_left();
			}
			else
			{
				m_it = m_it->get_right();
			}
		}

		if (m_it != rbnil(m_tree))
		{
			data = m_it->get_data();
			return true;
		}
		return false;
	}

	bool		xtree::iterator::preorder(s32 dir, void *& data)
	{
		if (m_it == rbnil(m_tree))
			return false;

		if (m_it == nullptr)
		{
			m_it = rbfirst(m_tree);
		}
		else {
			xtree::node* next = m_it->get_child(1 - (s32)dir);
			if (next == rbnil(m_tree))
			{
				next = m_it->get_child((s32)dir);
				if (next == rbnil(m_tree))
				{
					do {
						while (m_it != rbnil(m_tree) && m_it->get_parent_side() == dir)
						{
							m_it = m_it->get_parent();
						}
						m_it = m_it->get_parent();
					} while (m_it != rbnil(m_tree) && m_it->get_child((s32)dir) == rbnil(m_tree));

					if (m_it != rbnil(m_tree))
					{
						m_it = m_it->get_child((s32)dir);
					}
				}
				else
				{
					m_it = next;
				}
			}
			else
			{
				m_it = next;
			}
		}

		if (m_it != rbnil(m_tree))
		{
			data = m_it->get_data();
			return true;
		}

		return false;
	}

	bool		xtree::iterator::sortorder(s32 dir, void *& data)
	{
		if (m_it == rbnil(m_tree))
			return false;

		if (m_it == nullptr)
		{
			m_it = rbfirst(m_tree);
			while (m_it->get_child(1 - (s32)dir) != rbnil(m_tree))
			{
				m_it = m_it->get_child(1 - (s32)dir);
			}
		}
		else
		{
			xtree::node* next = m_it->get_child((s32)dir);
			if (next == rbnil(m_tree))
			{
				if (m_it->get_parent_side() == dir)
				{
					while (m_it != rbnil(m_tree) && m_it->get_parent_side() == dir)
					{
						m_it = m_it->get_parent();
					}
					if (m_it != rbnil(m_tree))
					{
						m_it = m_it->get_parent();
					}
				}
				else
				{
					m_it = m_it->get_parent();
				}
			}
			else
			{
				do {
					m_it = next;
					next = next->get_child(1 - (s32)dir);
				} while (next != rbnil(m_tree));
			}
		}

		if (m_it != rbnil(m_tree) && m_it != rbroot(m_tree))
		{
			data = m_it->get_data();
			return true;
		}

		return false;
	}

	bool		xtree::iterator::postorder(s32 dir, void *& data)
	{
		if (m_it == rbnil(m_tree))
			return false;

		if (m_it == nullptr)
		{
			m_it = rbfirst(m_tree);

			// Travel on the outside (left - right - left - right ....)
			xtree::node* branch = m_it->get_child(1 - (s32)dir);
			while (branch != rbnil(m_tree))
			{
				while (branch != rbnil(m_tree))
				{
					m_it = branch;
					branch = branch->get_child(1 - (s32)dir);
				}
				branch = m_it->get_child((s32)dir);
			}
		}
		else
		{
			if (m_it->get_parent_side() == dir)
			{
				m_it = m_it->get_parent();
			}
			else
			{
				m_it = m_it->get_parent();

				// Travel on the outside (left - right - left - right ....)
				xtree::node* branch = m_it->get_child((s32)dir);
				while (branch != rbnil(m_tree))
				{
					while (branch != rbnil(m_tree))
					{
						m_it = branch;
						branch = branch->get_child(1 - (s32)dir);
					}
					branch = m_it->get_child((s32)dir);
				}
			}
		}

		if (m_it != rbnil(m_tree) && m_it != rbroot(m_tree))
		{
			data = m_it->get_data();
			return true;
		}

		return false;
	}

}

