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
	//      Red-black tree functions
	// 
	//<CODE [lang= "c++"]>
	// 		
	// 	See unittest for examples
	// 		
	//</CODE>
	//     
	//------------------------------------------------------------------------------
	
	// @note	A 'C' style way of managing your own red-black tree, you only need a 'root'.
	// @see		See test_x_tree.cpp for examples on how to use this.
	extern bool			xtree_insert(xrbnode*& root, void    * key, xrbnode * node, xrbnode_cmp_f cmp);
	extern bool			xtree_remove(xrbnode*& root, void    * key, xrbnode_cmp_f cmp, xrbnode_remove_f remove_f, xrbnode*& node);
	extern bool			xtree_find  (xrbnode*  root, void    * key, xrbnode_cmp_f cmp, xrbnode*& node);

	extern bool			xtree_min   (xrbnode*  root, xrbnode*& node);
	extern bool			xtree_max   (xrbnode*  root, xrbnode*& node);

	// @note	Initialize 'iterator' to NULL, iterate until NULL is returned.
	//			The returned node can be destroyed and it is already detached from the tree.
	extern xrbnode*		xtree_clear (xrbnode*& root);
}


#endif	///< __XBASE_TREE_H__

