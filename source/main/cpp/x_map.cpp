#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_map.h"

namespace xcore
{
	static s8  calc_index(s32 level, u64 key);

	xztree::node* xztree::insert(node*& root, node* node, s32 level, u64 key, void** value, xalloc* allocator)
	{
		if (root == nullptr)
		{
			s32 ivalue = calc_index(level, key);
			xztree::node* pnode = allocator->construct<xztree::node>();
			pnode->m_nodemap = 0;
			pnode->m_valuemap = 1 << ivalue;
			*value = &pnode->m_branches[0];
			return nullptr;
		}
		else
		{
			s32 index = calc_index(level, key);
			if ((node->m_nodemap & (1 << index)) != 0)
			{
				// That index is a node, so we should traverse further
			}
			if ((node->m_valuemap & (1 << index)) != 0)
			{
				// That index is a value, so we should return that value
			}
		}
	}

	xztree::node* xztree::find(node*& root, node* n, s8 level, u64 key, void*& value, s32& index, node** history)
	{

	}

	xztree::node* xztree::remove(node*& root, node** path, s8 level, u64 key, xalloc* allocator)
	{

	}

};


