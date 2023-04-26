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

        UNITTEST_TEST(PositiveZero)
        {
            CHECK_TRUE(1.0 / math::positiveZero64() >= 1.0);
            CHECK_TRUE(math::positiveZero64() == -1.0 * -1.0 * 0.0);
        }

        UNITTEST_TEST(NegativeZero)
        {
            CHECK_TRUE(math::negativeZero64() == -1.0 * 0.0);
            CHECK_TRUE(1.0 / math::negativeZero64() <= -1.0);
            CHECK_TRUE(math::negativeZero64() * math::negativeZero64() == math::positiveZero64());
            CHECK_TRUE(1.0 / (math::negativeZero64() * math::negativeZero64()) >= 1.0);
        }
#ifdef IEEE_FLOATS
        UNITTEST_TEST(Nan)
        {
            CHECK_TRUE(math::nan() != math::nan());
        }

        UNITTEST_TEST(PositiveInfinity)
        {
            CHECK_TRUE(math::positiveInfinity64() + 1.0 == math::positiveInfinity64());
            CHECK_TRUE(math::positiveInfinity64() == (1.0 / math::positiveZero64()));
            CHECK_TRUE(math::positiveInfinity64() >= 0xffffffff);
            CHECK_TRUE(math::bin(math::positiveInfinity64()) == D_CONSTANT_64(0x7ff0000000000000));
        }

        UNITTEST_TEST(NegativeInfinity)
        {
            CHECK_TRUE(math::negativeInfinity64() - 1.0 == math::negativeInfinity64());
            CHECK_TRUE(math::negativeInfinity64() == (1.0 / math::negativeZero64()));
            CHECK_TRUE(math::negativeInfinity64() <= 0xffffffff);
            CHECK_TRUE(math::bin(math::negativeInfinity64()) == D_CONSTANT_64(0xfff0000000000000));
        }
#endif
        UNITTEST_TEST(PositiveMaximum)
        {
            CHECK_TRUE(math::positiveMaximum64() == math::positiveMaximum64());
            CHECK_TRUE(math::positiveMaximum64() >= 0xffffffff);
            CHECK_TRUE(math::positiveMaximum64() >= 9999.9);
        }

        UNITTEST_TEST(NegativeMaximum)
        {
            CHECK_TRUE(math::negativeMaximum64() == math::negativeMaximum64());
            CHECK_TRUE(math::negativeMaximum64() <= 0xffffffff);
            CHECK_TRUE(math::negativeMaximum64() <= 0x000fffff);
            CHECK_TRUE(math::negativeMaximum64() <= 0.1);
        }

        UNITTEST_TEST(PositiveMinimum)
        {
            CHECK_TRUE(math::positiveMinimum64() == math::positiveMinimum64());
            CHECK_TRUE(math::positiveMinimum64() > 0.0);
            CHECK_TRUE(math::positiveMinimum64() <= 1.0);
            CHECK_TRUE(math::positiveMinimum64() > math::positiveZero64());
        }

        UNITTEST_TEST(NegativeMinimum)
        {
            CHECK_TRUE(math::negativeMinimum64() == math::negativeMinimum64());
            CHECK_TRUE(math::negativeMinimum64() < 0.0);
            CHECK_TRUE(math::negativeMinimum64() < math::negativeZero64());
            CHECK_TRUE(math::negativeMinimum64() > -0.1);
        }

        UNITTEST_TEST(PositiveMinimumDEN)
        {
            CHECK_TRUE(math::positiveMinimumDEN64() > 0.0);
            CHECK_TRUE(math::positiveMinimumDEN64() < math::positiveMinimum64());
        }

        UNITTEST_TEST(NegativeMinimumDEN)
        {
            CHECK_TRUE(math::negativeMinimumDEN64() < 0.0);
            CHECK_TRUE(math::negativeMinimumDEN64() > math::negativeMinimum64());
        }
#ifdef IEEE_FLOATS
        UNITTEST_TEST(IsInfinite)
        {
            CHECK_FALSE(math::isInfinite(0.0));
            CHECK_FALSE(math::isInfinite(999.0));
            CHECK_TRUE(math::isInfinite(math::negativeInfinity64()));
            CHECK_TRUE(math::isInfinite(math::positiveInfinity64()));
        }

        UNITTEST_TEST(IsNAN)
        {
            CHECK_FALSE(math::isNAN(0.0));
            CHECK_FALSE(math::isNAN(999.0));
            CHECK_TRUE(math::isNAN(math::nan64()));
            CHECK_TRUE(math::isNAN(math::nan64() - 1.0));
            CHECK_FALSE(math::isNAN(math::negativeInfinity64()));
        }

        UNITTEST_TEST(IsFinite)
        {
            CHECK_TRUE(math::isFinite(0.0));
            CHECK_TRUE(math::isFinite(999.0));
            CHECK_FALSE(math::isFinite(math::negativeInfinity64()));
            CHECK_FALSE(math::isFinite(math::nan64()));
        }

        UNITTEST_TEST(IsRational)
        {
            CHECK_TRUE(math::isRational(0.0));
            CHECK_TRUE(math::isRational(999.0));
            CHECK_FALSE(math::isRational(math::negativeInfinity64()));
            CHECK_FALSE(math::isRational(math::nan64()));
        }
#endif
        UNITTEST_TEST(Bin)
        {
            CHECK_TRUE(math::bin(0.0) == 0);
#ifdef IEEE_FLOATS
            CHECK_TRUE(math::bin(math::nan()) == math::bin(math::nan()+1.0));
#endif
            CHECK_TRUE(math::bin(1.0) == D_CONSTANT_64(0x3ff0000000000000));
            CHECK_TRUE(math::bin(-2.0) == D_CONSTANT_64(0xc000000000000000));
        }

        UNITTEST_TEST(Sbin)
        {
            CHECK_TRUE(math::sbin(0.0) == 0);
            CHECK_TRUE(math::sbin(-2.0) == D_CONSTANT_64(0xc000000000000000));
#ifdef IEEE_FLOATS
            CHECK_TRUE(math::sbin(math::nan()) == math::sbin(math::nan()+1.0));
#endif
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
            CHECK_FALSE(math::isEqual(0.0001,0.0));
            CHECK_TRUE(math::isEqual(20.0,20.000));
            CHECK_TRUE(math::isEqual(math::negativeZero64(), -0.0));

#ifdef IEEE_FLOATS
            CHECK_TRUE(math::isEqual(math::nan(),math::nan()));
#endif
        }

        UNITTEST_TEST(IsNotEqual)
        {
            CHECK_TRUE(math::isNotEqual(0.0001,0.0));
            CHECK_FALSE(math::isNotEqual(20.0,20.000));
            CHECK_FALSE(math::isNotEqual(math::negativeZero64(), -0.0));
#ifdef IEEE_FLOATS
            CHECK_FALSE(math::isNotEqual(math::nan(),math::nan()));
#endif
        }

        UNITTEST_TEST(BinaryAnd)
        {
            CHECK_TRUE(math::binaryAnd(0.0,(u64)0) == 0.0);
            CHECK_TRUE(math::binaryAnd(-2.0,(u64)0x10000000) == math::toF64(D_CONSTANT_64(0xc000000000000000)&0x10000000));
        }

        UNITTEST_TEST(BinaryOr)
        {
            CHECK_TRUE(math::binaryOr(0.0,(s32)0) == 0.0);
            CHECK_TRUE(math::binaryOr(-2.0,(s32)10) == math::toF64(D_CONSTANT_64(0xc000000000000000)|10));
        }

        UNITTEST_TEST(Fraction)
        {
            CHECK_TRUE(math::fraction(0.0) == 0);
            CHECK_TRUE(math::fraction(-2.0) == 0);
            //0.15625f : 0x3fc4000000000000
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
            CHECK_EQUAL(math::signBit(-2.0),1);
            CHECK_EQUAL(math::signBit(0.15625),0);
        }

        UNITTEST_TEST(SignBitSigned)
        {
            CHECK_EQUAL(math::signBitSigned(1.0), 0);
            CHECK_EQUAL(math::signBitSigned(-2.0),-1);
            CHECK_EQUAL(math::signBitSigned(0.15625),0);
            CHECK_EQUAL(math::signBitSigned(math::positiveZero64()), 0);
            CHECK_EQUAL(math::signBitSigned(math::negativeZero64()), -1);
        }

        UNITTEST_TEST(Sign)
        {
            CHECK_EQUAL(math::sign(1.0), 1);
            CHECK_EQUAL(math::sign(-2.0),-1);
            CHECK_EQUAL(math::sign(0.15625),1);
            CHECK_EQUAL(math::sign(math::positiveZero64()), 0);
            CHECK_EQUAL(math::sign(math::negativeZero64()), 0);
        }

        UNITTEST_TEST(Sign2)
        {
            CHECK_EQUAL(math::sign(1.0, 0.5), 1);
            CHECK_EQUAL(math::sign(-2.0, 0.5),-1);
            CHECK_EQUAL(math::sign(0.15625, 0.5),0);
            CHECK_EQUAL(math::sign(0.1, math::positiveZero64()), 1);
            CHECK_EQUAL(math::sign(math::negativeZero64(), 0.1), 0);
        }

        UNITTEST_TEST(IsNearZero)
        {
            CHECK_FALSE(math::isNearZero(0.0001));
            CHECK_TRUE(math::isNearZero(0.0001, -1));
            CHECK_TRUE(math::isNearZero(math::positiveMinimum64()));
            CHECK_TRUE(math::isNearZero(math::negativeMinimum64()));
        }

        UNITTEST_TEST(IsNear)
        {
            CHECK_FALSE(math::isNear(0.0001, 0.0, -80));
            CHECK_TRUE(math::isNear(0.0001, 0.0f, -1));
            CHECK_FALSE(math::isNear(1.0001, 1.0, -80));
            CHECK_TRUE(math::isNear(1.0001, 1.0, -1));
            CHECK_TRUE(math::isNear(math::positiveMinimum64(), 0.0f, -80));
            CHECK_TRUE(math::isNear(math::negativeMinimum64(), 0.0f, -80));
        }

        UNITTEST_TEST(RoundToInt)
        {
            CHECK_EQUAL(math::roundToInt(0.5), 1);
            CHECK_EQUAL(math::roundToInt(0.4), 0);
            CHECK_EQUAL(math::roundToInt(14.0/3.0), 5);
        }

        UNITTEST_TEST(Squared)
        {
            CHECK_TRUE(math::squared(0.5) == 0.25);
            CHECK_TRUE(math::squared(0.4) == 0.4 * 0.4);
            CHECK_TRUE(math::squared(0.1) == 0.1 * 0.1);
            CHECK_TRUE(math::squared(14.0/3.0) == (14.0 / 3.0) * (14.0 / 3.0));
        }

        UNITTEST_TEST(OneOver)
        {
            CHECK_TRUE(math::oneOver(0.5) == 1.0f/0.5);
            CHECK_TRUE(math::oneOver(0.4) == 1.0f/0.4);
            CHECK_TRUE(math::oneOver(0.1) == 1.0f/0.1);
            CHECK_TRUE(math::oneOver(14.0/3.0) == 1.0/(14.0 / 3.0));
        }

        UNITTEST_TEST(IsZero)
        {
            CHECK_FALSE(math::isZero(0.0001));
            CHECK_TRUE(math::isZero(math::positiveZero64()));
            CHECK_TRUE(math::isZero(math::negativeZero64()));
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
            CHECK_TRUE(math::isLessPositive(math::positiveMinimum64(), 0.0001));
        }

        UNITTEST_TEST(IsGreater)
        {
            CHECK_FALSE(math::isGreater(0.1, 1.0));
            CHECK_TRUE(math::isGreater(1.0, 0.1));
            CHECK_FALSE(math::isGreater(math::positiveMinimum64(), 0.0001));
        }
    }
}
UNITTEST_SUITE_END
