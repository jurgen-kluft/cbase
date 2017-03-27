#include "xbase\x_debug.h"
#include "xbase\x_tree.h"

namespace xcore
{
	bool		xtree_insert(xrbnode*& root, void * data, xrbnode * node_to_insert, xrbnode_cmp_f cmp_f)
	{
		ASSERT(node_to_insert!=0);
		return rb_insert_node(root, data, node_to_insert, cmp_f);
	}

	bool		xtree_remove(xrbnode*& root, void * key, xrbnode_cmp_f cmp_f, xrbnode_remove_f remove_f, xrbnode *& remove_node)
	{
		return (rb_remove_node(root, key, cmp_f, remove_f, remove_node));
	}

	bool		xtree_find  (xrbnode* root, void * key, xrbnode_cmp_f cmp_f, xrbnode*& find_node)
	{
		xrbnode* it = root;
		while ( it != NULL )
		{
			s32 const c = cmp_f(key, it);
			if ( c == 0 )
				break;
			s32 dir = c < 0 ? xrbnode::LEFT : xrbnode::RIGHT;
			it = it->get_child(dir);
		}
		find_node = it;
		return find_node != NULL;
	}

	xrbnode*	xtree_clear (xrbnode*& iterator)
	{
		//	Rotate away the left links so that
		//	we can treat this like the destruction
		//	of a linked list
		if (iterator == NULL)
			return NULL;

		xrbnode* it = iterator;
		iterator = NULL;

		while ( it != NULL ) 
		{
			if ( it->get_child(xrbnode::LEFT) == NULL ) 
			{	// No left links, just kill the node and move on
				iterator = (xrbnode*)it->get_child(xrbnode::RIGHT);
				it->clear();
				return it;
			}
			else
			{	// Rotate away the left link and check again
				iterator = (xrbnode*)it->get_child(xrbnode::LEFT);
				it->set_child(iterator->get_child(xrbnode::RIGHT), xrbnode::LEFT);
				iterator->set_child(it, xrbnode::RIGHT);
			}
			it = iterator;
			iterator = NULL;
		}
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
