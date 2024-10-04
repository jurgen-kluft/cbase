#include "cbase/c_allocator.h"
#include "cbase/c_slice.h"
#include "cbase/c_map.h"
#include "cbase/test_allocator.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_map_and_set)
{
    static const s32 c_num_keys = 100;

    UNITTEST_FIXTURE(map)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(map_s32)
        {
            map_t<s32, s32> map(Allocator);

            for (s32 v = 0; v < c_num_keys; ++v)
            {
                s32 k = v + 65536;
                s32 f = -1;
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

            map_t<s32,s32>::iterator_t iter = map.iterate();
            s32 i = 0;
            while (iter.order())
            {
                s32 k = iter.item()->key;
                s32 v = iter.item()->value;
                CHECK_EQUAL(i + 65536, k);
                CHECK_EQUAL(i, v);
                i++;
            }

            for (s32 v = 0; v < c_num_keys; ++v)
            {
                s32 k = v + 65536;
                s32 f = -1;
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

        UNITTEST_TEST(set_s32)
        {
            set_t<s32> set(Allocator);

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

            set_t<s32>::iterator_t iter = set.iterate();
            s32 i = 0;
            while (iter.order())
            {
                s32 k = iter.item()->key;
                CHECK_EQUAL(i + 65536, k);
                i++;
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
