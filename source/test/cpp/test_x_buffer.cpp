#include "xbase/x_carray.h"
#include "xbase/x_buffer.h"
#include "xunittest/xunittest.h"

using namespace xcore;


UNITTEST_SUITE_BEGIN(xcbuffer)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static s32	array_data[64];

		UNITTEST_TEST(test_xbuffer32)
		{
			xbytes32 buf1(1);
			xbytes32 buf2(2);

			CHECK_TRUE(buf1 != buf2);
			CHECK_FALSE(buf1 == buf2);

			CHECK_TRUE(buf1 < buf2);
			CHECK_TRUE(buf2 > buf1);

			buf1 = buf2;
			CHECK_TRUE(buf1 == buf2);
			CHECK_FALSE(buf1 != buf2);
		}

	}
}
UNITTEST_SUITE_END
