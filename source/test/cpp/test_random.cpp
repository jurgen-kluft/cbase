#include "cbase/c_allocator.h"
#include "cbase/c_random.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(random)
{
	UNITTEST_FIXTURE(main)
	{
        UNITTEST_ALLOCATOR;

		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(random)
		{
			xor_random_t random;
			random.reset(0);
			for (size_t i = 0; i < 100; ++i)
			{
                u32 value1;
                random.generate((u8*)&value1, sizeof(value1));
                u32 value2;
                random.generate((u8*)&value2, sizeof(value2));
                CHECK_NOT_EQUAL(value1, value2);
			}
		}
	}
}
UNITTEST_SUITE_END
