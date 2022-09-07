#include "cbase/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_map.h"

namespace ncore
{
	static inline s32	calc_array_index(s8 child, u32 nodemap, u32 valuemap)
	{
		// Example: node  map = 0 0 1 0 0 1 0 0
		//          value map = 0 1 0 0 1 0 0 1
		// index = 3
		// items = (0 1 1 0 1 1 0 1) & (0 1 1 1) = 0 1 0 1
		// index = countbits(0 1 0 1) = 2
		u32 items = (nodemap | valuemap) & ((1<<child) - 1);
        s32 index = math::countBits(items);
		if (items == 0)
			return -1;
		return index;
	}

	static inline bool	node_has_child(s8 child, u32 nodemap, u32 valuemap)
	{
		u32 items = (nodemap | valuemap) & (1<<child);
		return (items == 0);
	}

	static void TestMap()
	{
		map_t<s32, s32> mymap;
		s32 k = 0;
		s32 v = 1000;
		mymap.insert(k, v);
		mymap.find(k, v);
		mymap.remove(k, v);
	}

	static void TestSet()
	{
		set_t<s32> myset;
		s32 v = 1000;
		myset.insert(v);
		myset.contains(v);
		myset.remove(v);
	}

};


