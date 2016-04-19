#include "xbase\x_va_list.h"
#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xva)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(x_va_bool)
		{
			x_va v(false);
			CHECK_TRUE(v.isBool());

			CHECK_FALSE(v.isInt8());
			CHECK_FALSE(v.isInt16());
			CHECK_FALSE(v.isInt32());
			CHECK_FALSE(v.isInt64());
			CHECK_FALSE(v.isUInt8());
			CHECK_FALSE(v.isUInt16());
			CHECK_FALSE(v.isUInt32());
			CHECK_FALSE(v.isUInt64());
			CHECK_FALSE(v.isF32());
			CHECK_FALSE(v.isF64());
			CHECK_FALSE(v.isPCTChar());

			CHECK_FALSE((bool)v);
			CHECK_EQUAL(0, (s32)v);
			CHECK_EQUAL(0.0f, (f32)v);
			CHECK_EQUAL(0.0, (f64)v);
		}

		UNITTEST_TEST(x_va_s32)
		{
			x_va v((s32)100);
			CHECK_FALSE(v.isBool());

			CHECK_FALSE(v.isInt8());
			CHECK_FALSE(v.isInt16());
			CHECK_TRUE(v.isInt32());
			CHECK_FALSE(v.isInt64());
			CHECK_FALSE(v.isUInt8());
			CHECK_FALSE(v.isUInt16());
			CHECK_FALSE(v.isUInt32());
			CHECK_FALSE(v.isUInt64());
			CHECK_FALSE(v.isF32());
			CHECK_FALSE(v.isF64());
			CHECK_FALSE(v.isPCTChar());

			CHECK_TRUE((bool)v);
			CHECK_EQUAL(100, (s32)v);
			CHECK_EQUAL(100.0f, (f32)v);
			CHECK_EQUAL(100.0, (f64)v);
		}

		UNITTEST_TEST(x_va_pctchar)
		{
			x_va v("ascii string");
			CHECK_FALSE(v.isBool());

			CHECK_FALSE(v.isInt8());
			CHECK_FALSE(v.isInt16());
			CHECK_FALSE(v.isInt32());
			CHECK_FALSE(v.isInt64());
			CHECK_FALSE(v.isUInt8());
			CHECK_FALSE(v.isUInt16());
			CHECK_FALSE(v.isUInt32());
			CHECK_FALSE(v.isUInt64());
			CHECK_FALSE(v.isF32());
			CHECK_FALSE(v.isF64());
			CHECK_TRUE(v.isPCTChar());

			CHECK_TRUE((bool)v);	// ptr != NULL
		}

	}
}
UNITTEST_SUITE_END
