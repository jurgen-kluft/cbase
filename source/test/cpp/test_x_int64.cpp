#include "xbase/x_target.h"

#ifndef X_NO_CUSTOM_INT64

#include "xbase/x_int64.h"
#include "xunittest/xunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(__xint64)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(Constructors)
        {
			__xint64 v1((s8)1);
            CHECK_EQUAL(1, (s32)v1);

			__xint64 v21((s16)2);
			CHECK_EQUAL(2, (s32)v21);
			__xint64 v22((s16)0x1234);
			CHECK_EQUAL(0x1234, (s32)v22);

			__xint64 v31((s32)3);
			CHECK_EQUAL(3, (s32)v31);
			__xint64 v32((s32)0x12345678);
			CHECK_EQUAL(0x12345678, (s32)v32);

			__xint64 v4((u8)4);
			CHECK_EQUAL(4, (u32)v4);

			__xint64 v51((u16)5);
			CHECK_EQUAL(5, (u32)v51);
			__xint64 v52((u16)0x1234);
			CHECK_EQUAL(0x1234, (u32)v52);

			__xint64 v61((u32)6);
			CHECK_EQUAL(6, (u32)v61);
			__xint64 v62((u32)0x12345678);
			CHECK_EQUAL(0x12345678, (u32)v62);

			__xint64 v71((s64)7);
			CHECK_EQUAL(7, (u64)v71);
			__xint64 v72(X_CONSTANT_64(0x12345678ABCDEF33));
			CHECK_EQUAL(X_CONSTANT_64(0x12345678ABCDEF33), (u64)v72);

			__xint64 v81(7,8);
			CHECK_EQUAL(X_CONSTANT_64(0x0000000700000008), (u64)v81);
		}

		UNITTEST_TEST(Asignment)
		{
			__xint64 v1; v1 = ((s8)1);
			CHECK_EQUAL(1, (s32)v1);

			__xint64 v21; v21 = ((s16)2);
			CHECK_EQUAL(2, (s32)v21);
			__xint64 v22; v22 = ((s16)0x1234);
			CHECK_EQUAL(0x1234, (s32)v22);

			__xint64 v31; v31 = ((s32)3);
			CHECK_EQUAL(3, (s32)v31);
			__xint64 v32; v32 = ((s32)0x12345678);
			CHECK_EQUAL(0x12345678, (s32)v32);

			__xint64 v4; v4 = ((u8)4);
			CHECK_EQUAL(4, (u32)v4);

			__xint64 v51; v51 = ((u16)5);
			CHECK_EQUAL(5, (u32)v51);
			__xint64 v52; v52 = ((u16)0x1234);
			CHECK_EQUAL(0x1234, (u32)v52);

			__xint64 v61; v61 = ((u32)6);
			CHECK_EQUAL(6, (u32)v61);
			__xint64 v62; v62 = ((u32)0x12345678);
			CHECK_EQUAL(0x12345678, (u32)v62);

			__xint64 v71; v71 = ((s64)7);
			CHECK_EQUAL(7, (u64)v71);
			__xint64 v72; v72 = (X_CONSTANT_64(0x12345678ABCDEF33));
			CHECK_EQUAL(X_CONSTANT_64(0x12345678ABCDEF33), (u64)v72);
		}

		UNITTEST_TEST(Increment)
		{
			__xint64 v1(1);
			CHECK_EQUAL(1, (s32)v1);
			v1++;
			CHECK_EQUAL(2, (s32)v1);
			++v1;
			CHECK_EQUAL(3, (s32)v1);

			__xint64 v2(X_CONSTANT_64(0x00000000FFFFFFFF));
			CHECK_EQUAL(X_CONSTANT_64(0x00000000FFFFFFFF), (u64)v2);
			v2++;
			CHECK_EQUAL(X_CONSTANT_64(0x0000000100000000), (u64)v2);
			++v2;
			CHECK_EQUAL(X_CONSTANT_64(0x0000000100000001), (u64)v2);
		}

		UNITTEST_TEST(Decrement)
		{
			__xint64 v1(3);
			CHECK_EQUAL(3, (s32)v1);
			v1--;
			CHECK_EQUAL(2, (s32)v1);
			--v1;
			CHECK_EQUAL(1, (s32)v1);

			__xint64 v2(X_CONSTANT_64(0x0000000100000000));
			CHECK_EQUAL(X_CONSTANT_64(0x0000000100000000), (u64)v2);
			v2--;
			CHECK_EQUAL(X_CONSTANT_64(0x00000000FFFFFFFF), (u64)v2);
			--v2;
			CHECK_EQUAL(X_CONSTANT_64(0x00000000FFFFFFFE), (u64)v2);
		}

		UNITTEST_TEST(Multiply)
		{
			__xint64 v1(300);
			__xint64 v2(450);
			__xint64 v3 = v1*v2;
			CHECK_EQUAL(300*450, (u32)v3);

			v2 *= v1;
			CHECK_EQUAL(300*450, (u32)v2);

			v2 = 0x12348888;
			v2 *= 0x10000;
			CHECK_EQUAL(0x1234, v2[0]);
			CHECK_EQUAL(0x88880000, v2[1]);
		}

		UNITTEST_TEST(Divide)
		{
			__xint64 v11(100, 0);
			__xint64 v21( 33, 0);
			__xint64 v31 = v11 / v21;
			CHECK_EQUAL(0, v31[0]);
			CHECK_EQUAL(3, v31[1]);

			__xint64 v12(0, 100);
			__xint64 v22(0, 33);
			__xint64 v32 = v12 / v22;
			CHECK_EQUAL(0, v32[0]);
			CHECK_EQUAL(3, v32[1]);
		}

		UNITTEST_TEST(Addition)
		{
			__xint64 v11(100, 0);
			__xint64 v21(0, 33);
			__xint64 v31 = v11 + v21;
			CHECK_EQUAL(100, v31[0]);
			CHECK_EQUAL(33, v31[1]);

			__xint64 v12(100, 0x70000000);
			__xint64 v22(0, 0xA0000001);
			__xint64 v32 = v12 + v22;
			CHECK_EQUAL(101, v32[0]);
			CHECK_EQUAL(0x10000001, v32[1]);
		}

		UNITTEST_TEST(Substraction)
		{
			__xint64 v1(100);
			__xint64 v2(33);
			__xint64 v3 = v1 - v2;
			CHECK_EQUAL(67, (s32)v3);

			__xint64 v11(100, 0);
			__xint64 v21(0, 33);
			__xint64 v31 = v11 - v21;
			CHECK_EQUAL(99, v31[0]);
			CHECK_EQUAL(-33, v31[1]);
		}

		UNITTEST_TEST(Modulo)
		{
			__xint64 v1(100);
			__xint64 v2 = v1 % 3;
			CHECK_EQUAL(1, (s32)v2);
			__xint64 v3 = v1 % 7;
			CHECK_EQUAL(2, (s32)v3);
		}

		UNITTEST_TEST(Negation)
		{
			__xint64 v(1);
			v = -v;
			CHECK_EQUAL(0xFFFFFFFF, v[0]);
			CHECK_EQUAL(0xFFFFFFFF, v[1]);
		}

		UNITTEST_TEST(Invertion)
		{
			__xint64 v(0);
			v = ~v;
			CHECK_EQUAL(0xFFFFFFFF, v[0]);
			CHECK_EQUAL(0xFFFFFFFF, v[1]);
		}
	}
}
UNITTEST_SUITE_END

#endif //X_NO_CUSTOM_INT64
