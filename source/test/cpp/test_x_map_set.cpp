#include "xbase/x_allocator.h"
#include "xbase/x_slice.h"
#include "xbase/x_map.h"

#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::xalloc* gTestAllocator;

UNITTEST_SUITE_BEGIN(xmap_and_set)
{
    UNITTEST_FIXTURE(xmap)
    {
        UNITTEST_FIXTURE_SETUP()
        {
        }

        UNITTEST_FIXTURE_TEARDOWN()
        {
        }

        UNITTEST_TEST(map_s32)
        {
            xmap<s32, s32> map(gTestAllocator);
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

            // TODO: Write some tests!
        }
    }
}
UNITTEST_SUITE_END
