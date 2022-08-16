#include "cbase/c_carray.h"
#include "cbase/c_range.h"
#include "cunittest/xunittest.h"

using namespace ncore;


UNITTEST_SUITE_BEGIN(xrange)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test_range_c11_s32)
		{
			s32 i = 0;
			range_t<s32> range(from(0), count(100));
			for (s32 it : range) 
			{
				CHECK_EQUAL(i, it);
				i += 1;
			}
			CHECK_EQUAL(100, i);

			i = 100;
			for (s32 it : backwards(range))
			{
				CHECK_EQUAL(i, it);
				i -= 1;
			}
		}

		UNITTEST_TEST(test_range_c11_f32)
		{
			f32 i = 0;
			range_t<f32> range(from(0.0f), count(100));
			for (f32 it : range) 
			{
				CHECK_EQUAL(i, it);
				i += 1;
			}
			CHECK_EQUAL(100, i);

			i = 100;
			for (f32 it : backwards(range))
			{
				CHECK_EQUAL(i, it);
				i -= 1;
			}

		}

	}
}
UNITTEST_SUITE_END
