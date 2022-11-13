#include "cbase/c_endian.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_endian)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(_u8)
        {
			u8 a = 1;
			u8 b = x_endian_swap::swap(a);
			CHECK_EQUAL(1, a);
			CHECK_EQUAL(1, b);
        }
        UNITTEST_TEST(_s8)
        {
			s8 a = 1;
			s8 b = x_endian_swap::swap(a);
			CHECK_EQUAL(1, a);
			CHECK_EQUAL(1, b);
        }

        UNITTEST_TEST(_u16)
        {
			u16 a = 0x1234;
			u16 b = x_endian_swap::swap(a);
			CHECK_EQUAL(0x1234, a);
			CHECK_EQUAL(0x3412, b);
        }

        UNITTEST_TEST(_s16)
        {
			s16 a = 0x1234;
			s16 b = x_endian_swap::swap(a);
			CHECK_EQUAL(0x1234, a);
			CHECK_EQUAL(0x3412, b);
        }

        UNITTEST_TEST(_u32)
        {
			u32 a = 0x12345678;
			u32 b = x_endian_swap::swap(a);
			CHECK_EQUAL(0x12345678, a);
			CHECK_EQUAL(0x78563412, b);
        }

        UNITTEST_TEST(_s32)
        {
			s32 a = 0x12345678;
			s32 b = x_endian_swap::swap(a);
			CHECK_EQUAL(0x12345678, a);
			CHECK_EQUAL(0x78563412, b);
        }

        UNITTEST_TEST(_u64)
        {
			u64 a = D_CONSTANT_64(0x90ABCDEF12345678);
			u64 b = x_endian_swap::swap(a);
			CHECK_EQUAL(D_CONSTANT_64(0x90ABCDEF12345678), a);
			CHECK_EQUAL(D_CONSTANT_64(0x78563412EFCDAB90), b);
        }

        UNITTEST_TEST(_s64)
        {
			s64 a = D_CONSTANT_64(0x90ABCDEF12345678);
			s64 b = x_endian_swap::swap(a);
			CHECK_EQUAL(D_CONSTANT_64(0x90ABCDEF12345678), a);
			CHECK_EQUAL(D_CONSTANT_64(0x78563412EFCDAB90), b);
        }
	}
}
UNITTEST_SUITE_END
