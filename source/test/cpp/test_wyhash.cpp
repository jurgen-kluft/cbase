#include "cbase/c_wyhash.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(wyhash)
{
    UNITTEST_FIXTURE(wyreg)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        static const u32 prime = 2654435761U;

        UNITTEST_TEST(insert)
        {
            nhash::wyreg reg;
            reg.init(Allocator, 1024, prime);

            s32 test_values[] = {0, 1, 2, 3, 4, 5, 6, 7};

            for (uint_t i = 0; i < sizeof(test_values) / sizeof(s32); ++i)
            {
                s32 pos;
                CHECK_TRUE(reg.insert(&test_values[i], sizeof(s32), pos));
            }
        }


        UNITTEST_TEST(insert_find)
        {
            nhash::wyreg reg;
            reg.init(Allocator, 1024, prime);

            s32 test_values[] = {0, 1, 2, 3, 4, 5, 6, 7};
            s32 test_pos[] = {0, 1, 2, 3, 4, 5, 6, 7};

            for (uint_t i = 0; i < sizeof(test_values) / sizeof(s32); ++i)
            {
                s32 pos;
                CHECK_TRUE(reg.insert(&test_values[i], sizeof(s32), pos));
                test_pos[i] = pos;
            }

            for (uint_t i = 0; i < sizeof(test_values) / sizeof(s32); ++i)
            {
                s32 pos;
                CHECK_TRUE(reg.find(&test_values[i], sizeof(s32), pos));
                CHECK_EQUAL(test_pos[i], pos);
            }
        }

    }
}
UNITTEST_SUITE_END
