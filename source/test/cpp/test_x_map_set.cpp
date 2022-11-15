#include "cbase/c_allocator.h"
#include "cbase/c_slice.h"
#include "cbase/c_map.h"

#include "cunittest/cunittest.h"

using namespace ncore;

extern ncore::alloc_t* gTestAllocator;

UNITTEST_SUITE_BEGIN(test_map_and_set)
{
    UNITTEST_FIXTURE(map)
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

            for (s32 v = 0; v < 1000; ++v)
            {
                s32        k = v+65536;
                s32 const* f = nullptr;
                CHECK_TRUE(map.insert(k, v));
                CHECK_TRUE(map.find(k, f));
                CHECK_EQUAL(v, *f);
                CHECK_TRUE(map.remove(k));
            }
            for (s32 v = 0; v < 1000; ++v)
            {
                s32        k = v + 65536;
                CHECK_TRUE(map.insert(k, v));
            }
            for (s32 v = 0; v < 1000; ++v)
            {
                s32        k = v + 65536;
                s32 const* f = nullptr;
                CHECK_TRUE(map.find(k, f));
                CHECK_EQUAL(v, *f);
                CHECK_TRUE(map.remove(k));
            }
        }
    }

    UNITTEST_FIXTURE(set)
    {
        UNITTEST_FIXTURE_SETUP()
        {
        }

        UNITTEST_FIXTURE_TEARDOWN()
        {
        }

        UNITTEST_TEST(set_s32)
        {
            set_t<s32> set(gTestAllocator);

			s32 v = 1000;

			CHECK_TRUE(set.insert(v));
			CHECK_TRUE(set.contains(v));
			CHECK_TRUE(set.remove(v));
        }
    }
}
UNITTEST_SUITE_END
