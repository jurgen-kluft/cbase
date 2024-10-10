#include "cbase/c_double.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_double)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(Pow)
        {
            f64 x = 3.0;
            u32 n = 5;
            f64 r = math::powd(x, n);
            CHECK_TRUE(r == 243.0);
        }

        UNITTEST_TEST(Abs)
        {
            f32 a = 1.0, b = 0.0, c = -1.0;
            CHECK_TRUE(math::abs(a) == 1.0);
            CHECK_TRUE(math::abs(b) == 0.0);
            CHECK_TRUE(math::abs(c) == 1.0);
        }

        UNITTEST_TEST(Neg)
        {
            f32 a = 1.0, b = 0.0, c = -1.0;
            CHECK_TRUE(math::neg(a) == -1.0);
            CHECK_TRUE(math::neg(b) == 0.0);
            CHECK_TRUE(math::neg(c) == 1.0);
        }

#ifdef D_IEEE_FLOATS
        UNITTEST_TEST(IsInfinite)
        {
            CHECK_FALSE(math::isInfinite(0.0));
            CHECK_FALSE(math::isInfinite(999.0));
        }

        UNITTEST_TEST(IsNAN)
        {
            CHECK_FALSE(math::isNAN(0.0));
            CHECK_FALSE(math::isNAN(999.0));
        }

        UNITTEST_TEST(IsFinite)
        {
            CHECK_TRUE(math::isFinite(0.0));
            CHECK_TRUE(math::isFinite(999.0));
        }

        UNITTEST_TEST(IsRational)
        {
            CHECK_TRUE(math::isRational(0.0));
            CHECK_TRUE(math::isRational(999.0));
        }
#endif
        UNITTEST_TEST(Bin)
        {
            CHECK_TRUE(math::bin(0.0) == 0);
            CHECK_TRUE(math::bin(1.0) == D_CONSTANT_64(0x3ff0000000000000));
            CHECK_TRUE(math::bin(-2.0) == D_CONSTANT_64(0xc000000000000000));
        }

        UNITTEST_TEST(Sbin)
        {
            CHECK_EQUAL(0, math::sbin(0.0));
            CHECK_TRUE(math::sbin(-2.0) == D_CONSTANT_64(0xc000000000000000));
        }

        UNITTEST_TEST(ToF64)
        {
            CHECK_TRUE(math::toF64((u64)0) == 0.0);
            CHECK_TRUE(math::toF64(D_CONSTANT_64(0x3ff0000000000000)) == 1.0);
            CHECK_TRUE(math::toF64(D_CONSTANT_64(0xc000000000000000)) == -2.0);
        }

        UNITTEST_TEST(ToS64)
        {
            CHECK_TRUE(math::toS64(0.0) == 0);
            CHECK_TRUE(math::toS64(-2.0) == (s64)-2);
            CHECK_TRUE(math::toS64(2000.0) == (s64)2000);
        }

        UNITTEST_TEST(ToU64)
        {
            CHECK_TRUE(math::toU64(0.0) == 0);
            CHECK_TRUE(math::toU64(20.0) == (u64)20);
        }

        UNITTEST_TEST(IsEqual)
        {
            CHECK_FALSE(math::isEqual(0.0001, 0.0));
            CHECK_TRUE(math::isEqual(20.0, 20.000));

#ifdef D_IEEE_FLOATS
#endif
        }

        UNITTEST_TEST(IsNotEqual)
        {
            CHECK_TRUE(math::isNotEqual(0.0001, 0.0));
            CHECK_FALSE(math::isNotEqual(20.0, 20.000));
#ifdef D_IEEE_FLOATS
#endif
        }

        UNITTEST_TEST(BinaryAnd)
        {
            CHECK_TRUE(math::binaryAnd(0.0, (u64)0) == 0.0);
            CHECK_TRUE(math::binaryAnd(-2.0, (u64)0x10000000) == math::toF64(D_CONSTANT_64(0xc000000000000000) & 0x10000000));
        }

        UNITTEST_TEST(BinaryOr)
        {
            CHECK_TRUE(math::binaryOr(0.0, (s32)0) == 0.0);
            CHECK_TRUE(math::binaryOr(-2.0, (s32)10) == math::toF64(D_CONSTANT_64(0xc000000000000000) | 10));
        }

        UNITTEST_TEST(Fraction)
        {
            CHECK_TRUE(math::fraction(0.0) == 0);
            CHECK_TRUE(math::fraction(-2.0) == 0);
            // 0.15625f : 0x3fc4000000000000
            CHECK_TRUE(math::fraction(0.15625f) == D_CONSTANT_64(0x0004000000000000));
        }

        UNITTEST_TEST(ExponentBinary)
        {
            CHECK_TRUE(math::exponentBinary(0.0) == 0);
            CHECK_TRUE(math::exponentBinary(-2.0) == (s32)1024);
            CHECK_TRUE(math::exponentBinary(0.15625) == (s32)(0x3fc));
        }

        UNITTEST_TEST(Exponent)
        {
            CHECK_TRUE(math::exponent(1.0) == 0);
            CHECK_TRUE(math::exponent(-2.0) == (s32)1);
            CHECK_TRUE(math::exponent(0.15625) == (s32)(0x37c - 0x37f));
        }

        UNITTEST_TEST(SignMask)
        {
            CHECK_TRUE(math::signMask(1.0f) == 0);
            CHECK_TRUE(math::signMask(-2.0f) == D_CONSTANT_64(0x8000000000000000));
            CHECK_TRUE(math::signMask(0.15625f) == 0);
        }

        UNITTEST_TEST(SignBit)
        {
            CHECK_EQUAL(math::signBit(1.0), 0);
            CHECK_EQUAL(math::signBit(-2.0), 1);
            CHECK_EQUAL(math::signBit(0.15625), 0);
        }

        UNITTEST_TEST(SignBitSigned)
        {
            CHECK_EQUAL(math::signBitSigned(1.0), 0);
            CHECK_EQUAL(math::signBitSigned(-2.0), -1);
            CHECK_EQUAL(math::signBitSigned(0.15625), 0);
        }

        UNITTEST_TEST(Sign)
        {
            CHECK_EQUAL(math::sign(1.0), 1);
            CHECK_EQUAL(math::sign(-2.0), -1);
            CHECK_EQUAL(math::sign(0.15625), 1);
        }

        UNITTEST_TEST(Sign2)
        {
            CHECK_EQUAL(math::sign(1.0, 0.5), 1);
            CHECK_EQUAL(math::sign(-2.0, 0.5), -1);
            CHECK_EQUAL(math::sign(0.15625, 0.5), 0);
        }

        UNITTEST_TEST(IsNearZero)
        {
            CHECK_FALSE(math::isNearZero(0.0001));
            CHECK_TRUE(math::isNearZero(0.0001, -1));
        }

        UNITTEST_TEST(IsNear)
        {
            CHECK_FALSE(math::isNear(0.0001, 0.0, -80));
            CHECK_TRUE(math::isNear(0.0001, 0.0f, -1));
            CHECK_FALSE(math::isNear(1.0001, 1.0, -80));
            CHECK_TRUE(math::isNear(1.0001, 1.0, -1));
        }

        UNITTEST_TEST(RoundToInt)
        {
            CHECK_EQUAL(math::roundToInt(0.5), 1);
            CHECK_EQUAL(math::roundToInt(0.4), 0);
            CHECK_EQUAL(math::roundToInt(14.0 / 3.0), 5);
        }

        UNITTEST_TEST(Squared)
        {
            CHECK_TRUE(math::squared(0.5) == 0.25);
            CHECK_TRUE(math::squared(0.4) == 0.4 * 0.4);
            CHECK_TRUE(math::squared(0.1) == 0.1 * 0.1);
            CHECK_TRUE(math::squared(14.0 / 3.0) == (14.0 / 3.0) * (14.0 / 3.0));
        }

        UNITTEST_TEST(OneOver)
        {
            CHECK_TRUE(math::oneOver(0.5) == 1.0f / 0.5);
            CHECK_TRUE(math::oneOver(0.4) == 1.0f / 0.4);
            CHECK_TRUE(math::oneOver(0.1) == 1.0f / 0.1);
            CHECK_TRUE(math::oneOver(14.0 / 3.0) == 1.0 / (14.0 / 3.0));
        }

        UNITTEST_TEST(IsZero)
        {
            CHECK_FALSE(math::isZero(0.0001));
        }

        UNITTEST_TEST(IsPositive)
        {
            CHECK_FALSE(math::isPositive(0.0));
            CHECK_TRUE(math::isPositive(1.0));
            CHECK_FALSE(math::isPositive(-2.0));
        }

        UNITTEST_TEST(IsNegative)
        {
            CHECK_FALSE(math::isNegative(0.0));
            CHECK_FALSE(math::isNegative(1.0));
            CHECK_TRUE(math::isNegative(-2.0));
        }

        UNITTEST_TEST(IsLessPositive)
        {
            CHECK_TRUE(math::isLessPositive(0.1, 1.0));
            CHECK_FALSE(math::isLessPositive(1.0, 0.1));
        }

        UNITTEST_TEST(IsGreater)
        {
            CHECK_FALSE(math::isGreater(0.1, 1.0));
            CHECK_TRUE(math::isGreater(1.0, 0.1));
        }
    }
}
UNITTEST_SUITE_END
