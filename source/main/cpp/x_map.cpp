#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_map.h"

namespace xcore
{
	static inline s8	calc_child_index(s32 level, u64 key)
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

	static inline bool	node_has_child(xztree::node_t* n, s8 child)
	{
		u32 items = (n->m_nodemap | n->m_valuemap) & (1<<child);
		return (items == 0);
	}

	static inline xztree::node_t* node_construct(s32 array_size, xalloc* allocator)
	{
		xztree::node_t* node = static_cast<xztree::node_t*>(allocator->allocate(sizeof(xztree::node_t) + (sizeof(void*) * (array_size-1)), sizeof(void*)));
		node->m_nodemap = 0;
		node->m_valuemap = 0;
		for (s32 i=0; i<array_size; ++i)
		{
			node->m_branches[i] = nullptr;
		}
		return node;
	}

	static inline void	node_set_value(xztree::node_t*& n, s8 child, void* cv)
	{
		ASSERT(node_has_child(n, child) == false);
		n->m_valuemap |= (1 << child);
		s32 index = calc_array_index(child, n->m_nodemap, n->m_valuemap);
		n->m_branches[index] = cv;
	}
	static inline void	node_set_node(xztree::node_t*& n, s8 child, xztree::node_t* cn)
	{
		ASSERT(node_has_child(n, child) == false);
		n->m_nodemap |= (1 << child);
		s32 index = calc_array_index(child, n->m_nodemap, n->m_valuemap);
		n->m_branches[index] = cn;
	}

	static inline void	node_insert1(xztree::node_t*& n, s8 child, void* child_item, xalloc* allocator)
	{
		// Allocate new node with one extra slot
		// Determine the insertion place and move down the tail part
		// Copy the existing elements
		// Set the child item
	}
	static inline void	node_insert2(xztree::node_t*& n, s8 child1, void* child_item1, s8 child2, void* child_item2, xalloc* allocator)
	{
		// Allocate new node with two extra slots
		// Copy the existing elements, skip the slots of child1 and child2
		// Set the child items
	}

	xztree::node_t* xztree::find(node_t* n, s8 level, u64 k, node_t*& _node, void*& _value, path_t* path)
	{
		if (path != nullptr)
		{
			path->m_level[path->m_levels++] = n;
		}
	
		// We need to return the next child node if there is one
		s8 child = calc_child_index(level, k);
		if (((n->m_valuemap) & (1<<child)) == 0)
		{
			// There is no value at this index, maybe a node?
			if (((n->m_nodemap) & (1<<child)) == 0)
			{
				// No node here as well
				_value = nullptr;
				_node = nullptr;
			}
			else
			{
				// There is a node here, so we return the child node
				s32 index = calc_array_index(child, n->m_nodemap, n->m_valuemap);
				node_t* childnode = (node_t*)n->m_branches[index];
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

	static inline void node_set_item(xztree::node_t* node, s32 level, u64 hash, void* value)
	{
		s8 child = calc_child_index(level, hash);
		s32 index = calc_array_index(child, node->m_nodemap, node->m_valuemap);
		node->m_branches[index] = value;
	}

	xztree::node_t* xztree::insert(node_t*& root, s32 level, node_t* parent, node_t* node, u64 hash1, void* value1, u64 hash2, void* value2, xalloc* allocator)
	{
		// You should already have done a 'find' function call, so that the correct information
		// can be passed into this function call.
		// Here we insert 1 or 2 values
		// If it is 1 value we just have to enlarge the branch array of the current node
		// If we have 2 values then we have to branch until the hash keys are different
		if (value2 == nullptr)
		{
			s8 const child = calc_child_index(level, hash1);
			if (root == nullptr)
			{
				root = node_construct(1, allocator);
				node = root;
			}
			else
			// One value to insert
			ASSERT(node_has_child(node, child) == false);	// We should not have this child since we are inserting
			node_insert1(node, child, value1, allocator);
		}
		else
		{
			ASSERT(root != nullptr);
			ASSERT(calc_child_index(level, hash1) == calc_child_index(level, hash2));

			// We need to branch and create nodes at every level until the child index is different
			xztree::node_t* parent = node;
			while (calc_child_index(level, hash1) == calc_child_index(level, hash2))
			{
				xztree::node_t* childnode = node_construct(1, allocator);
				
			}
		}
		return nullptr;
	}

	xztree::node_t* xztree::remove(node_t*& root, path_t& path, s8 level, node_t* node, u64 key, xalloc* allocator)
	{
		// Removal of a value can mean that we have to traverse up because the node ends up being empty.
		// It could be that the whole tree ends up empty.

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


