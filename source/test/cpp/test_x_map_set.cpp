#include "xbase/x_allocator.h"
#include "xbase/x_slice.h"
#include "xbase/x_map.h"

#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::alloc_t* gTestAllocator;

UNITTEST_SUITE_BEGIN(xmap_and_set)
{
    UNITTEST_FIXTURE(map_t)
    {
        UNITTEST_FIXTURE_SETUP()
        {
        }

        UNITTEST_FIXTURE_TEARDOWN()
        {
        }

        UNITTEST_TEST(map_s32)
        {
            map_t<s32, s32> map(gTestAllocator);

			s32 k = 0;
			s32 v = 1000;
			s32 f = 0;
			CHECK_TRUE(map.insert(k, v));
			CHECK_TRUE(map.find(k, f));
			CHECK_EQUAL(v, f);
			CHECK_TRUE(map.remove(k, v));
        }
    }

    UNITTEST_FIXTURE(xset)
    {
        UNITTEST_FIXTURE_SETUP()
        {
        }

        UNITTEST_FIXTURE_TEARDOWN()
        {
        }

        UNITTEST_TEST(set_s32)
        {
            xset<s32> set(gTestAllocator);

			s32 v = 1000;

			CHECK_TRUE(set.insert(v));
			CHECK_TRUE(set.contains(v));
			CHECK_TRUE(set.remove(v));
        }
    }
}
UNITTEST_SUITE_END
