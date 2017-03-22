// x_tree.h - xcore C style red-black tree functions
#ifndef __XBASE_TREE_H__
#define __XBASE_TREE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_debug.h"
#include "xbase\private\x_rbtree.h"

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
	
	// @note	If the key is greater than the key of node then return 1. If the key is less than the key of node then return -1 otherwise return 0
	typedef	s32			(*xtree_compare_nodes)(xrbnode* node_a, xrbnode* node_b);
	typedef	void		(*xtree_swap_nodes)(xrbnode* node_a, xrbnode* node_b);

	// @note	A 'C' style way of managing your own red-black tree, the only xrbnode object you need is a 'root'.
	// @see		See test_x_tree.cpp for examples on how to use this.
	extern bool			xtree_insert(xrbnode*& root, xrbnode * node, xtree_compare_nodes cmp);
	extern bool			xtree_remove(xrbnode*& root, xrbnode*& node, xtree_compare_nodes cmp, xtree_swap_nodes swap);
	extern bool			xtree_find  (xrbnode*  root, xrbnode*& node, xtree_compare_nodes cmp);

	extern bool			xtree_min   (xrbnode*  root, xrbnode*& node);
	extern bool			xtree_max   (xrbnode*  root, xrbnode*& node);

	// @note	Initialize 'iterator' to NULL, iterate until NULL is returned.
	//			The returned node can be destroyed and it is already detached from the tree.
	extern xrbnode*		xtree_clear (xrbnode* root, xrbnode*& iterator);


}


#endif	///< __XBASE_TREE_H__

