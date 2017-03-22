#include "xbase\x_debug.h"
#include "xbase\x_tree.h"

namespace xcore
{

	bool		xtree_insert(xrbnode*& root, xrbnode * node_to_insert, xtree_compare_nodes cmp)
	{
		ASSERT(node_to_insert!=0);
		return rb_insert_node(root, node_to_insert, cmp) == 1 ? xTRUE : xFALSE;
	}

	bool		xtree_remove(xrbnode*& root, xrbnode*& remove_node, xtree_compare_nodes cmp, xtree_swap_nodes swap)
	{
		xrbnode* removed_node;
		if (rb_remove_node(root, remove_node, cmp, swap, removed_node) == 1)
		{
			remove_node = removed_node;
			return true;
		}
		remove_node = NULL;
		return false;
	}

	bool		xtree_find  (xrbnode* root, xrbnode*& find_node, xtree_compare_nodes cmp)
	{
		xrbnode* it = root;
		while ( it != NULL )
		{
			s32 c = cmp(it, find_node);
			if ( c == 0 )
				break;
			s32 const s = (c + 1) >> 1;
			it = it->get_child(s);
		}
		find_node = ( it == NULL) ? NULL : it;
		return find_node != NULL;
	}

	xrbnode*	xtree_clear (xrbnode* root, xrbnode*& save)
	{
		//	Rotate away the left links so that
		//	we can treat this like the destruction
		//	of a linked list
		if (save == NULL)
			save = root;

		xrbnode* it = save;
		save = NULL;

		while ( it != NULL ) 
		{
			if ( it->get_child(xrbnode::LEFT) == root ) 
			{
				/* No left links, just kill the node and move on */
				save = (xrbnode*)it->get_child(xrbnode::RIGHT);
				if (it != root)
				{
					it->clear();
					return it;
				}
			}
			else
			{
				/* Rotate away the left link and check again */
				save = (xrbnode*)it->get_child(xrbnode::LEFT);
				it->set_child(save->get_child(xrbnode::RIGHT), xrbnode::LEFT);
				save->set_child(it, xrbnode::RIGHT);
			}

			it = save;
			save = NULL;
		}
		root->clear();
		return NULL;
	}


	bool			xtree_min(xrbnode* root, xrbnode*& node)
	{
		if (root == NULL)
			return false;

		xrbnode* it = root;
		{
			node = it;
			it = it->get_child(xrbnode::LEFT);
		} while (it != NULL);
		return node;
	}

	bool			xtree_max(xrbnode* root, xrbnode*& node)
	{
		if (root == NULL)
			return false;

		xrbnode* it = root;
		{
			node = it;
			it = it->get_child(xrbnode::RIGHT);
		} while (it != NULL);
		return node;
	}

}
