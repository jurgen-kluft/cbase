#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_map.h"

namespace xcore
{
	static inline s8	calc_index(s32 level, u64 key)
	{
		return 0;
	}

	static inline s32	calc_array_index(s8 child, u32 nodemap, u32 valuemap)
	{
		// Example: node  map = 0 0 1 0 0 1 0 0
		//          value map = 0 1 0 0 1 0 0 1
		// index = 3
		// items = (0 1 1 0 1 1 0 1) & (0 1 1 1) = 0 1 0 1
		// index = countbits(0 1 0 1) = 2

		u32 items = (nodemap | valuemap) & ((1<<child) - 1);
		s32 index = xcountBits(items);
		if (items == 0)
			return -1;
		return index;
	}

	xztree::node* xztree::find(node* n, s8 level, u64 k, node*& _node, void*& _value, path_t* path)
	{
		// We need to return the next child node if there is one
		s8 child = calc_index(level, k);
		if (((n->m_valuemap) & (1<<child)) == 0)
		{
			// There is no value at this index, maybe a node?
			if (((n->m_nodemap) & (1<<child)) == 0)
			{
				// No node here as well
				_value = nullptr;
				_node = n;
			}
			else
			{
				// There is a node here, so we return the child node
				s32 index = calc_array_index(child, n->m_nodemap, n->m_valuemap);
				node* childnode = (node*)n->m_branches[index];
				if (path != nullptr)
				{
					path->m_level[path->m_levels++] = n;
				}
				return childnode;
			}
		}
		else
		{
			s32 index = calc_array_index(child, n->m_nodemap, n->m_valuemap);
			_value = n->m_branches[index];
			_node = n;
		}

		return nullptr;
	}

	xztree::node* xztree::insert(node*& root, s32 level, node* node, u64 hash1, void* value1, u64 hash2, void* value2, xalloc* allocator)
	{
		// Here we insert 1 or 2 values into our tree
		// If it is 1 value we just have to enlarge the branch array 
		// If we have 2 values then we have to branch until the hash keys are different.

		return nullptr;
	}

	xztree::node* xztree::remove(node*& root, path_t& path, s8 level, node* node, u64 key, xalloc* allocator)
	{
		// Removal of a value can mean that we have to traverse up because the current node is empty.

		return nullptr;
	}



	static void TestMap()
	{
		xmap<s32, s32> mymap;

		mymap.insert(0, 1000);
		
		s32 value;
		mymap.find(0,value);
	}

};


