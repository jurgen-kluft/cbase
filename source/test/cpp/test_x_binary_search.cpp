#include "xbase\x_types.h"
#include "xbase\x_binary_search.h"
#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(x_binary_search)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		static s32		sComparePredicate(const void* inItem, const void* inData, s32 inIndex)
		{
			s32 a = *(s32*)inItem;

			s32* data = (s32*)inData;
			s32 b = data[inIndex];

			if (a < b)
				return -1;
			else if (a > b)
				return 1;
			return 0;
		}

        UNITTEST_TEST(_search)
        {
			const s32 sorted_random_number_list[] =
			{
				127,157,297,305,322,483,490,687,767,839,
				850,901,947,966,1033,1113,1174,1346,1380,1459,
				1781,1820,1857,1872,1890,1981,2303,2327,2355,2517,
				2549,2659,2749,2791,2847,3085,3293,3512,3532,3606,
				3630,3681,3727,3744,3845,3860,3899,3942,4025,4053
			};

			s32 n = 1890;
			s32 idx_of_n = x_BinarySearch(&n, sorted_random_number_list, sizeof(sorted_random_number_list)/sizeof(s32), sComparePredicate);

			CHECK_EQUAL(sorted_random_number_list[24], n);
        }
	}
}
UNITTEST_SUITE_END
