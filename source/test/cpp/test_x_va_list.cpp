#include "cbase/c_va_list.h"
#include "cunittest/cunittest.h"

using namespace ncore;

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
			va_t v(false);
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
			CHECK_FALSE(v.isPCRunes());

			CHECK_FALSE((bool)v);
			CHECK_EQUAL(0, (s32)v);
			CHECK_EQUAL(0.0f, (f32)v);
			CHECK_EQUAL(0.0, (f64)v);
		}

		UNITTEST_TEST(x_va_s32)
		{
			va_t v((s32)100);
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
			CHECK_FALSE(v.isPCRunes());

			CHECK_TRUE((bool)v);
			CHECK_EQUAL(100, (s32)v);
			CHECK_EQUAL(100.0f, (f32)v);
			CHECK_EQUAL(100.0, (f64)v);
		}

		UNITTEST_TEST(x_va_pcrunes)
		{
			va_t v("ascii string");
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
			CHECK_TRUE(v.isPCRunes());

			CHECK_FALSE((bool)v);	// "ascii string" != "true"
		}

	}
}
UNITTEST_SUITE_END
