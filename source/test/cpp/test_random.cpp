#include "cbase/c_allocator.h"
#include "cbase/c_random.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(random)
{
	UNITTEST_FIXTURE(xor)
	{
        //UNITTEST_ALLOCATOR;

		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(random32)
		{
			xor_random_t random;
			random.reset(0);
			for (size_t i = 0; i < 100; ++i)
			{
                u32 value1 = random.rand32();
                u32 value2 = random.rand32();
                CHECK_NOT_EQUAL(value1, value2);
			}
		}

		UNITTEST_TEST(random64)
        {
            xor_random_t random;
            random.reset(0);
            for (size_t i = 0; i < 100; ++i)
            {
                u64 value1 = random.rand64();
                u64 value2 = random.rand64();
                CHECK_NOT_EQUAL(value1, value2);
            }
        }
    }

	UNITTEST_FIXTURE(wyhash)
    {
        // UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(random32)
        {
            wyrand_t random;
            random.reset(0);
            for (size_t i = 0; i < 100; ++i)
            {
                u32 value1 = random.rand32();
                u32 value2 = random.rand32();
                CHECK_NOT_EQUAL(value1, value2);
            }
        }

        UNITTEST_TEST(random64)
        {
            wyrand_t random;
            random.reset(0);
            for (size_t i = 0; i < 100; ++i)
            {
                u64 value1 = random.rand64();
                u64 value2 = random.rand64();
                CHECK_NOT_EQUAL(value1, value2);
            }
        }
    }
}
UNITTEST_SUITE_END
