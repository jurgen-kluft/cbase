#include "cbase/c_allocator.h"
#include "cbase/c_map32.h"
#include "cbase/test_allocator.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_map32_and_set32)
{
    UNITTEST_FIXTURE(map)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(map32_s32)
        {
            map32_t<s32, s32> map(Allocator);

            const s32 c_num_keys = 100;

            for (s32 v = 0; v < c_num_keys; ++v)
            {
                s32 k = v + 65536;
                s32 f;
                CHECK_TRUE(map.insert(k, v));
                CHECK_TRUE(map.find(k, f));
                CHECK_EQUAL(v, f);
                CHECK_TRUE(map.remove(k));
            }
            for (s32 v = 0; v < c_num_keys; ++v)
            {
                s32 k = v + 65536;
                CHECK_TRUE(map.insert(k, v));
            }
            for (s32 v = 0; v < c_num_keys; ++v)
            {
                s32 k = v + 65536;
                s32 f;
                CHECK_TRUE(map.find(k, f));
                CHECK_EQUAL(v, f);
                CHECK_TRUE(map.remove(k));
            }
        }
    }

    UNITTEST_FIXTURE(set)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(set32_s32)
        {
            set32_t<s32> set(Allocator);

            const s32 c_num_keys = 100;

            for (s32 v = 0; v < c_num_keys; ++v)
            {
                s32 k = v + 65536;
                CHECK_TRUE(set.insert(k));
                CHECK_TRUE(set.contains(k));
                CHECK_TRUE(set.remove(k));
            }
            for (s32 v = 0; v < c_num_keys; ++v)
            {
                s32 k = v + 65536;
                CHECK_TRUE(set.insert(k));
            }
            for (s32 v = 0; v < c_num_keys; ++v)
            {
                s32 k = v + 65536;
                CHECK_TRUE(set.contains(k));
                CHECK_TRUE(set.remove(k));
            }
        }
    }
}
UNITTEST_SUITE_END
