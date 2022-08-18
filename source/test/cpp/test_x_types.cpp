#include "cbase/private/c_int128.h"
#include "cbase/private/c_uint128.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(xtypes)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static void sTypeOf_xbool_IsUnique(bool a) { }
		static void sTypeOf_xbool_IsUnique(s32 a) { }
		static void sTypeOf_xbool_IsUnique(u32 a) { }

		UNITTEST_TEST(type_size_check)
		{
			sTypeOf_xbool_IsUnique(bool(0));
			sTypeOf_xbool_IsUnique(s32(0));
			sTypeOf_xbool_IsUnique(u32(0));

#ifdef TARGET_32BIT
			CHECK_TRUE(sizeof(void*) == 4);
#endif

#ifdef TARGET_64BIT
			CHECK_TRUE(sizeof(void*) == 8);
#endif

			CHECK_TRUE(sizeof(u8) == 1);
			CHECK_TRUE(sizeof(u8) == 1);
			CHECK_TRUE(sizeof(s8) == 1);
			CHECK_TRUE(sizeof(u16) == 2);
			CHECK_TRUE(sizeof(s16) == 2);
			CHECK_TRUE(sizeof(u32) == 4);
			CHECK_TRUE(sizeof(s32) == 4);
			CHECK_TRUE(sizeof(u64) == 8);
			CHECK_TRUE(sizeof(s64) == 8);
			CHECK_TRUE(sizeof(u128) == 16);
			CHECK_TRUE(sizeof(s128) == 16);
			CHECK_TRUE(sizeof(f32) == 4);
			CHECK_TRUE(sizeof(f64) == 8);
			CHECK_TRUE(sizeof(bool32) == 4);
			CHECK_TRUE(sizeof(bool) == 4 || sizeof(bool) == 1);

			CHECK_TRUE(sizeof(uchar8)  == 1);
			CHECK_TRUE(sizeof(uchar16) == 2);
			CHECK_TRUE(sizeof(uchar32) == 4);
		}
	}
}
UNITTEST_SUITE_END
