#include "xbase\x_binary_search.h"

namespace xcore
{

	s32		x_BinarySearch(const void* inItem, const void* inData, s32 inLength, compare_predicate predicate)
	{
		// Do a binary search on the data to obtain the index of the found item
		s32 lowerBoundIndex = 0;
		s32 upperBoundIndex = inLength;
		s32 midIndex        = upperBoundIndex / 2;

		while (true)
		{
			const s32 r = predicate(inItem, inData, midIndex);
			if (r == 0)
			{
				// We have a hit, move back to lowest equal entry
				while (midIndex>0 && (predicate(inItem, inData, midIndex-1)==0))
				{
					--midIndex;
				}

				break;
			}
			else if (r<0)
			{
				if (midIndex == upperBoundIndex)
				{
					midIndex = -1;
					break;
				}

				// Adjust the upper bound, entry can only be found lower then current midIndex!
				upperBoundIndex = midIndex;
			}
			else
			{
				if (midIndex == lowerBoundIndex)
				{
					midIndex = -1;
					break;
				}

				// Adjust the lower bound, entry can only be found higher then current midIndex!
				lowerBoundIndex = midIndex;
			}

			// upperBoundIndex or lowerBoundIndex are changed, calculate new midIndex
			midIndex = lowerBoundIndex + ((upperBoundIndex - lowerBoundIndex) / 2);
		}

		return midIndex;
	}

};
