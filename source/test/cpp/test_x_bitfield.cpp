#include "xbase/x_bit_field.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xbitfield)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		enum ETestEnum
		{
			TEST_BIT_00 = 1<<0,
			TEST_BIT_01 = 1<<1,
			TEST_BIT_02 = 1<<2,
			TEST_BIT_03 = 1<<3,
			TEST_BIT_04 = 1<<4,
			TEST_BIT_05 = 1<<5,
			TEST_BIT_06 = 1<<6,
			TEST_BIT_07 = 1<<7,
			TEST_BIT_08 = 1<<8,
			TEST_BIT_09 = 1<<9,
			TEST_BIT_10 = 1<<10,
			TEST_BIT_11 = 1<<11,
			TEST_BIT_12 = 1<<12,
			TEST_BIT_13 = 1<<13,
			TEST_BIT_14 = 1<<14,
			TEST_BIT_15 = 1<<15,
			TEST_BIT_16 = 1<<16,
			TEST_BIT_17 = 1<<17,
			TEST_BIT_18 = 1<<18,
			TEST_BIT_19 = 1<<19,
			TEST_BIT_20 = 1<<20,
			TEST_BIT_21 = 1<<21,
			TEST_BIT_22 = 1<<22,
			TEST_BIT_23 = 1<<23,
			TEST_BIT_24 = 1<<24,
			TEST_BIT_25 = 1<<25,
			TEST_BIT_26 = 1<<26,
			TEST_BIT_27 = 1<<27,
			TEST_BIT_28 = 1<<28,
			TEST_BIT_29 = 1<<29,
			TEST_BIT_30 = 1<<30,
			TEST_BIT_31 = 1<<31,

			TEST_BIT_ALL = 0xffffffff,
			TEST_BITS = 0x00F0,
		};

		UNITTEST_TEST(isSet)
		{
			u32 b1(TEST_BIT_20);
			CHECK_EQUAL(TEST_BIT_20, (s32)b1);

			CHECK_FALSE(xbfIsSet(b1, TEST_BIT_00));
			CHECK_TRUE(xbfIsSet(b1, TEST_BIT_20));
			CHECK_FALSE(xbfIsSet(b1, TEST_BIT_21));
		}

		UNITTEST_TEST(isAnySet)
		{
			u32 b1(TEST_BITS);
			CHECK_EQUAL(TEST_BITS, (s32)b1);

			CHECK_FALSE(xbfIsAnySet(b1, (TEST_BIT_02 | TEST_BIT_03)));
			CHECK_TRUE(xbfIsAnySet(b1, (TEST_BIT_03 | TEST_BIT_04)));
			CHECK_TRUE(xbfIsAnySet(b1, (TEST_BIT_04 | TEST_BIT_05)));
			CHECK_FALSE(xbfIsAnySet(b1, (TEST_BIT_08 | TEST_BIT_09)));
		}

		UNITTEST_TEST(_isOnlySet)
		{
			u32 b1(TEST_BITS);
			CHECK_EQUAL(TEST_BITS, (s32)b1);

			CHECK_TRUE (xbfIsOnlySet(b1, ((TEST_BIT_04 | TEST_BIT_05 | TEST_BIT_06 | TEST_BIT_07))));
			CHECK_FALSE(xbfIsOnlySet(b1, ((TEST_BIT_03 | TEST_BIT_04))));
			CHECK_FALSE(xbfIsOnlySet(b1, ((TEST_BIT_04 | TEST_BIT_05))));
			CHECK_FALSE(xbfIsOnlySet(b1, ((TEST_BIT_08 | TEST_BIT_09))));
		}

		UNITTEST_TEST(_isCleared)
		{
			u32 b1(TEST_BITS);
			CHECK_EQUAL(TEST_BITS, (s32)b1);

			CHECK_TRUE (xbfIsCleared(b1, (TEST_BIT_00 | TEST_BIT_01 | TEST_BIT_02 | TEST_BIT_03)));
			CHECK_FALSE(xbfIsCleared(b1, (TEST_BIT_04 | TEST_BIT_01 | TEST_BIT_02 | TEST_BIT_03)));
		}

		UNITTEST_TEST(_getState)
		{
			u32 b1(TEST_BITS);
			CHECK_EQUAL(TEST_BITS, (s32)b1);

			CHECK_TRUE (xbfGetState(b1, TEST_BIT_04)==BIT_SET);
			CHECK_FALSE(xbfGetState(b1, TEST_BIT_03)==BIT_SET);
			CHECK_TRUE(xbfGetState(b1, TEST_BIT_03)==BIT_CLEARED);
			CHECK_TRUE(xbfGetState(b1, (TEST_BIT_03|TEST_BIT_04))==BIT_MIXED);
		}
		
		UNITTEST_TEST(set1)
		{
			u32 b1 = 0;
			CHECK_EQUAL(0, (s32)b1);

			xbfSet(b1, TEST_BIT_19);
			CHECK_TRUE(xbfIsSet(b1, TEST_BIT_19));
			xbfSet(b1, TEST_BIT_27);
			CHECK_TRUE(xbfIsSet(b1, TEST_BIT_27));

			xbfSet(b1, (TEST_BIT_30|TEST_BIT_31));
			CHECK_TRUE(xbfIsSet(b1, (TEST_BIT_30)));
			CHECK_TRUE(xbfIsSet(b1, (TEST_BIT_31)));
			CHECK_TRUE(xbfIsSet(b1, (TEST_BIT_30|TEST_BIT_31)));
		}

		UNITTEST_TEST(set2)
		{
			u32 b1 = 0;
			CHECK_EQUAL(0, (s32)b1);

			xbfSet(b1, TEST_BIT_04, TEST_BITS);
			CHECK_TRUE(xbfIsSet(b1, TEST_BIT_04));
			xbfSet(b1, TEST_BIT_05, TEST_BITS);
			CHECK_TRUE(xbfIsSet(b1, TEST_BIT_05));

			xbfSet(b1, (u32)(TEST_BIT_06|TEST_BIT_07), (u32)TEST_BITS);
			CHECK_TRUE(xbfIsSet(b1, (TEST_BIT_06|TEST_BIT_07)));

			CHECK_TRUE(xbfIsCleared(b1, (TEST_BIT_04|TEST_BIT_05)));
		}

		UNITTEST_TEST(set3)
		{
			u32 b1 = 0;
			CHECK_EQUAL(0, (s32)b1);

			CHECK_TRUE(xbfIsCleared(b1, TEST_BIT_10));
			xbfTestSet(b1, TEST_BIT_10, true);
			CHECK_TRUE(xbfIsSet(b1, TEST_BIT_10));
			xbfTestSet(b1, TEST_BIT_10, false);
			CHECK_TRUE(xbfIsCleared(b1, TEST_BIT_10));
		}

		UNITTEST_TEST(_clear)
		{
			u32 b1(TEST_BITS);
			xbfClear(b1, TEST_BITS);
			CHECK_EQUAL(0, (s32)b1);

			xbfSet(b1, TEST_BIT_10);
			CHECK_TRUE(xbfIsSet(b1, TEST_BIT_10));
			xbfClear(b1, TEST_BIT_10);
			CHECK_TRUE(xbfIsCleared(b1, TEST_BIT_10));
		}

		UNITTEST_TEST(_clearAll) 
		{
			u32 b1(TEST_BITS);
			CHECK_EQUAL(TEST_BITS, (s32)b1);
			xbfClearAll(b1);
			CHECK_EQUAL(0, (s32)b1);
		}

		UNITTEST_TEST(_toggle)
		{
			u32 b1(TEST_BITS);
			CHECK_EQUAL(TEST_BITS, (s32)b1);

			xbfToggle(b1, TEST_BIT_08);
			CHECK_NOT_EQUAL(TEST_BITS, (s32)b1);
			CHECK_TRUE(xbfIsSet(b1, TEST_BIT_05));
			xbfToggle(b1, TEST_BIT_08);
			CHECK_EQUAL(TEST_BITS, (s32)b1);
			CHECK_TRUE(xbfIsCleared(b1, TEST_BIT_08));
		}
	}

	UNITTEST_FIXTURE(enum)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		XENUM_BEGIN(ETestEnum)
		{
			TEST01 = 1<<1,
			TEST02 = 1<<2,
			TEST04 = 1<<4,
			TEST08 = 1<<8,
			TEST20 = 1<<20,
		} XENUM_END(ETestEnum)


		UNITTEST_TEST(xenum_instance)
		{
			ETestEnum e1(ETestEnum::TEST01);
			xenum<ETestEnum::EEnum> i1(e1.e);

			CHECK_TRUE(i1.IsSet(e1.e));

		}

	}

}
UNITTEST_SUITE_END
