#include "xbase/x_carray.h"
#include "xbase/x_range.h"
#include "xunittest/xunittest.h"

using namespace xcore;


UNITTEST_SUITE_BEGIN(range_t)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test_range_simple)
		{
			s32 i = 0;
			range_t<s32> iter(from(0), count(100));
			while (forward(iter)) {
				CHECK_EQUAL(i, *iter);
				i += 1;
			}

			i = 99;
			while (backward(iter)) {
				CHECK_EQUAL(i, *iter);
				i -= 1;
			}

		}
	}
}
UNITTEST_SUITE_END
