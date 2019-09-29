#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_map.h"

namespace xcore
{
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

	static inline bool	node_has_child(s8 child, u32 nodemap, u32 valuemap)
	{
		u32 items = (nodemap | valuemap) & (1<<child);
		return (items == 0);
	}



	static void TestMap()
	{
		xmap<s32, s32> mymap;

		mymap.insert(0, 1000);
		
		s32 value;
		mymap.find(0,value);


	}

};


