#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_map.h"

namespace xcore
{
	static s8  calc_index(s32 level, u64 key)
	{
		return 0;
	}

	xztree::node* xztree::insert(node*& root, s32 level, node** nodes, u64* keys, void** values, s32 count, xalloc* allocator)
	{
		if (root == nullptr)
		{
			//s32 ivalue = calc_index(level, key);
			//xztree::node* pnode = allocator->construct<xztree::node>();
			//pnode->m_nodemap = 0;
			//pnode->m_valuemap = 1 << ivalue;
			//*value = &pnode->m_branches[0];
			return nullptr;
		}
		else
		{
			//s32 index = calc_index(level, key);
			//if ((node->m_nodemap & (1 << index)) != 0)
			//{
			//	// That index is a node, so we should traverse further
			//}
			//if ((node->m_valuemap & (1 << index)) != 0)
			//{
			//	// That index is a value, so we should return that value
			//}

		}
		return nullptr;
	}

	xztree::node* xztree::find(node*& root, node* n, s8 level, u64 k, void ** v, node ** path)
	{
		return nullptr;
	}

	xztree::node* xztree::remove(node*& root, node** path, s8 level, u64 key, xalloc* allocator)
	{
		return nullptr;
	}

};


