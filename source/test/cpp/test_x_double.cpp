#include "xbase/x_double.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xdouble)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(Abs)
        {
            f32 a = 1.0, b = 0.0, c = -1.0;
            CHECK_TRUE(x_f64u::abs(a) == 1.0);
            CHECK_TRUE(x_f64u::abs(b) == 0.0);
            CHECK_TRUE(x_f64u::abs(c) == 1.0);
        }

        UNITTEST_TEST(Neg)
        {
            f32 a = 1.0, b = 0.0, c = -1.0;
            CHECK_TRUE(x_f64u::neg(a) == -1.0);
            CHECK_TRUE(x_f64u::neg(b) == 0.0);
            CHECK_TRUE(x_f64u::neg(c) == 1.0);
        }

        UNITTEST_TEST(PositiveZero)
        {
            CHECK_TRUE(1.0 / x_f64u::positiveZero() >= 1.0);
            CHECK_TRUE(x_f64u::positiveZero() == -1.0 * -1.0 * 0.0);
        }

        UNITTEST_TEST(NegativeZero)
        {
            CHECK_TRUE(x_f64u::negativeZero() == -1.0 * 0.0);
            CHECK_TRUE(1.0 / x_f64u::negativeZero() <= -1.0);
            CHECK_TRUE(x_f64u::negativeZero() * x_f64u::negativeZero() == x_f64u::positiveZero());
            CHECK_TRUE(1.0 / (x_f64u::negativeZero()*x_f64u::negativeZero()) >= 1.0);
        }
#ifdef IEEE_FLOATS
        UNITTEST_TEST(Nan)
        {
            CHECK_TRUE(x_f64u::nan() != x_f64u::nan());
        }

        UNITTEST_TEST(PositiveInfinity)
        {
            CHECK_TRUE(x_f64u::positiveInfinity() + 1.0 == x_f64u::positiveInfinity());
            CHECK_TRUE(x_f64u::positiveInfinity() == (1.0 / x_f64u::positiveZero()));
            CHECK_TRUE(x_f64u::positiveInfinity() >= 0xffffffff);
            CHECK_TRUE(x_f64u::bin(x_f64u::positiveInfinity()) == X_CONSTANT_64(0x7ff0000000000000));
        }

        UNITTEST_TEST(NegativeInfinity)
        {
            CHECK_TRUE(x_f64u::negativeInfinity() - 1.0 == x_f64u::negativeInfinity());
            CHECK_TRUE(x_f64u::negativeInfinity() == (1.0 / x_f64u::negativeZero()));
            CHECK_TRUE(x_f64u::negativeInfinity() <= 0xffffffff);
            CHECK_TRUE(x_f64u::bin(x_f64u::negativeInfinity()) == X_CONSTANT_64(0xfff0000000000000));
        }
#endif
        UNITTEST_TEST(PositiveMaximum)
        {
            CHECK_TRUE(x_f64u::positiveMaximum() == x_f64u::positiveMaximum());
            CHECK_TRUE(x_f64u::positiveMaximum() >= 0xffffffff);
            CHECK_TRUE(x_f64u::positiveMaximum() >= 9999.9);
        }

        UNITTEST_TEST(NegativeMaximum)
        {
            CHECK_TRUE(x_f64u::negativeMaximum() == x_f64u::negativeMaximum());
            CHECK_TRUE(x_f64u::negativeMaximum() <= 0xffffffff);
            CHECK_TRUE(x_f64u::negativeMaximum() <= 0x000fffff);
            CHECK_TRUE(x_f64u::negativeMaximum() <= 0.1);
        }

        UNITTEST_TEST(PositiveMinimum)
        {
            CHECK_TRUE(x_f64u::positiveMinimum() == x_f64u::positiveMinimum());
            CHECK_TRUE(x_f64u::positiveMinimum() > 0.0);
            CHECK_TRUE(x_f64u::positiveMinimum() <= 1.0);
            CHECK_TRUE(x_f64u::positiveMinimum() > x_f64u::positiveZero());
        }

        UNITTEST_TEST(NegativeMinimum)
        {
            CHECK_TRUE(x_f64u::negativeMinimum() == x_f64u::negativeMinimum());
            CHECK_TRUE(x_f64u::negativeMinimum() < 0.0);
            CHECK_TRUE(x_f64u::negativeMinimum() < x_f64u::negativeZero());
            CHECK_TRUE(x_f64u::negativeMinimum() > -0.1);
        }

        UNITTEST_TEST(PositiveMinimumDEN)
        {
            CHECK_TRUE(x_f64u::positiveMinimumDEN() > 0.0);
            CHECK_TRUE(x_f64u::positiveMinimumDEN() < x_f64u::positiveMinimum());
        }

        UNITTEST_TEST(NegativeMinimumDEN)
        {
            CHECK_TRUE(x_f64u::negativeMinimumDEN() < 0.0);
            CHECK_TRUE(x_f64u::negativeMinimumDEN() > x_f64u::negativeMinimum());
        }
#ifdef IEEE_FLOATS
        UNITTEST_TEST(IsInfinite)
        {
            CHECK_FALSE(x_f64u::isInfinite(0.0));
            CHECK_FALSE(x_f64u::isInfinite(999.0));
            CHECK_TRUE(x_f64u::isInfinite(x_f64u::negativeInfinity()));
            CHECK_TRUE(x_f64u::isInfinite(x_f64u::positiveInfinity()));
        }

        UNITTEST_TEST(IsNAN)
        {
            CHECK_FALSE(x_f64u::isNAN(0.0));
            CHECK_FALSE(x_f64u::isNAN(999.0));
            CHECK_TRUE(x_f64u::isNAN(x_f64u::nan()));
            CHECK_TRUE(x_f64u::isNAN(x_f64u::nan()-1.0));
            CHECK_FALSE(x_f64u::isNAN(x_f64u::negativeInfinity()));
        }

        UNITTEST_TEST(IsFinite)
        {
            CHECK_TRUE(x_f64u::isFinite(0.0));
            CHECK_TRUE(x_f64u::isFinite(999.0));
            CHECK_FALSE(x_f64u::isFinite(x_f64u::negativeInfinity()));
            CHECK_FALSE(x_f64u::isFinite(x_f64u::nan()));
        }

        UNITTEST_TEST(IsRational)
        {
            CHECK_TRUE(x_f64u::isRational(0.0));
            CHECK_TRUE(x_f64u::isRational(999.0));
            CHECK_FALSE(x_f64u::isRational(x_f64u::negativeInfinity()));
            CHECK_FALSE(x_f64u::isRational(x_f64u::nan()));
        }
#endif
        UNITTEST_TEST(Bin)
        {
            CHECK_TRUE(x_f64u::bin(0.0) == 0);
#ifdef IEEE_FLOATS
            CHECK_TRUE(x_f64u::bin(x_f64u::nan()) == x_f64u::bin(x_f64u::nan()+1.0));
#endif
            CHECK_TRUE(x_f64u::bin(1.0) == X_CONSTANT_64(0x3ff0000000000000));
            CHECK_TRUE(x_f64u::bin(-2.0) == X_CONSTANT_64(0xc000000000000000));
        }

        UNITTEST_TEST(Sbin)
        {
            CHECK_TRUE(x_f64u::sbin(0.0) == 0);
            CHECK_TRUE(x_f64u::sbin(-2.0) == X_CONSTANT_64(0xc000000000000000));
#ifdef IEEE_FLOATS
            CHECK_TRUE(x_f64u::sbin(x_f64u::nan()) == x_f64u::sbin(x_f64u::nan()+1.0));
#endif
        }

        UNITTEST_TEST(ToF64)
        {
            CHECK_TRUE(x_f64u::toF64((u64)0) == 0.0);
            CHECK_TRUE(x_f64u::toF64(X_CONSTANT_64(0x3ff0000000000000)) == 1.0);
            CHECK_TRUE(x_f64u::toF64(X_CONSTANT_64(0xc000000000000000)) == -2.0);
        }

        UNITTEST_TEST(ToS64)
        {
            CHECK_TRUE(x_f64u::toS64(0.0) == 0);
            CHECK_TRUE(x_f64u::toS64(-2.0) == (s64)-2);
            CHECK_TRUE(x_f64u::toS64(2000.0) == (s64)2000);
        }

        UNITTEST_TEST(ToU64)
        {
            CHECK_TRUE(x_f64u::toU64(0.0) == 0);
            CHECK_TRUE(x_f64u::toU64(20.0) == (u64)20);
        }

        UNITTEST_TEST(IsEqual)
        {
            CHECK_FALSE(x_f64u::isEqual(0.0001,0.0));
            CHECK_TRUE(x_f64u::isEqual(20.0,20.000));
            CHECK_TRUE(x_f64u::isEqual(x_f64u::negativeZero(),-0.0));

#ifdef IEEE_FLOATS
            CHECK_TRUE(x_f64u::isEqual(x_f64u::nan(),x_f64u::nan()));
#endif
        }

        UNITTEST_TEST(IsNotEqual)
        {
            CHECK_TRUE(x_f64u::isNotEqual(0.0001,0.0));
            CHECK_FALSE(x_f64u::isNotEqual(20.0,20.000));
            CHECK_FALSE(x_f64u::isNotEqual(x_f64u::negativeZero(),-0.0));
#ifdef IEEE_FLOATS
            CHECK_FALSE(x_f64u::isNotEqual(x_f64u::nan(),x_f64u::nan()));
#endif
        }

        UNITTEST_TEST(BinaryAnd)
        {
            CHECK_TRUE(x_f64u::binaryAnd(0.0,(u64)0) == 0.0);
            CHECK_TRUE(x_f64u::binaryAnd(-2.0,(u64)0x10000000) == x_f64u::toF64(X_CONSTANT_64(0xc000000000000000)&0x10000000));
        }

        UNITTEST_TEST(BinaryOr)
        {
            CHECK_TRUE(x_f64u::binaryOr(0.0,(s32)0) == 0.0);
            CHECK_TRUE(x_f64u::binaryOr(-2.0,(s32)10) == x_f64u::toF64(X_CONSTANT_64(0xc000000000000000)|10));
        }

        UNITTEST_TEST(Fraction)
        {
            CHECK_TRUE(x_f64u::fraction(0.0) == 0);
            CHECK_TRUE(x_f64u::fraction(-2.0) == 0);
            //0.15625f : 0x3fc4000000000000
            CHECK_TRUE(x_f64u::fraction(0.15625f) == X_CONSTANT_64(0x0004000000000000));
        }

        UNITTEST_TEST(ExponentBinary)
        {
            CHECK_TRUE(x_f64u::exponentBinary(0.0) == 0);
            CHECK_TRUE(x_f64u::exponentBinary(-2.0) == (s32)1024);
            CHECK_TRUE(x_f64u::exponentBinary(0.15625) == (s32)(0x3fc));
        }

        UNITTEST_TEST(Exponent)
        {
            CHECK_TRUE(x_f64u::exponent(1.0) == 0);
            CHECK_TRUE(x_f64u::exponent(-2.0) == (s32)1);
            CHECK_TRUE(x_f64u::exponent(0.15625) == (s32)(0x37c - 0x37f));
        }

        UNITTEST_TEST(SignMask)
        {
            CHECK_TRUE(x_f64u::signMask(1.0f) == 0);
            CHECK_TRUE(x_f64u::signMask(-2.0f) == X_CONSTANT_64(0x8000000000000000));
            CHECK_TRUE(x_f64u::signMask(0.15625f) == 0);
        }

        UNITTEST_TEST(SignBit)
        {
            CHECK_EQUAL(x_f64u::signBit(1.0), 0);
            CHECK_EQUAL(x_f64u::signBit(-2.0),1);
            CHECK_EQUAL(x_f64u::signBit(0.15625),0);
        }

        UNITTEST_TEST(SignBitSigned)
        {
            CHECK_EQUAL(x_f64u::signBitSigned(1.0), 0);
            CHECK_EQUAL(x_f64u::signBitSigned(-2.0),-1);
            CHECK_EQUAL(x_f64u::signBitSigned(0.15625),0);
            CHECK_EQUAL(x_f64u::signBitSigned(x_f64u::positiveZero()),0);
            CHECK_EQUAL(x_f64u::signBitSigned(x_f64u::negativeZero()),-1);
        }

        UNITTEST_TEST(Sign)
        {
            CHECK_EQUAL(x_f64u::sign(1.0), 1);
            CHECK_EQUAL(x_f64u::sign(-2.0),-1);
            CHECK_EQUAL(x_f64u::sign(0.15625),1);
            CHECK_EQUAL(x_f64u::sign(x_f64u::positiveZero()),0);
            CHECK_EQUAL(x_f64u::sign(x_f64u::negativeZero()),0);
        }

        UNITTEST_TEST(Sign2)
        {
            CHECK_EQUAL(x_f64u::sign(1.0, 0.5), 1);
            CHECK_EQUAL(x_f64u::sign(-2.0, 0.5),-1);
            CHECK_EQUAL(x_f64u::sign(0.15625, 0.5),0);
            CHECK_EQUAL(x_f64u::sign(0.1, x_f64u::positiveZero()),1);
            CHECK_EQUAL(x_f64u::sign(x_f64u::negativeZero(), 0.1),0);
        }

        UNITTEST_TEST(IsNearZero)
        {
            CHECK_FALSE(x_f64u::isNearZero(0.0001));
            CHECK_TRUE(x_f64u::isNearZero(0.0001, -1));
            CHECK_TRUE(x_f64u::isNearZero(x_f64u::positiveMinimum()));
            CHECK_TRUE(x_f64u::isNearZero(x_f64u::negativeMinimum()));
        }

        UNITTEST_TEST(IsNear)
        {
            CHECK_FALSE(x_f64u::isNear(0.0001, 0.0, -80));
            CHECK_TRUE(x_f64u::isNear(0.0001, 0.0f, -1));
            CHECK_FALSE(x_f64u::isNear(1.0001, 1.0, -80));
            CHECK_TRUE(x_f64u::isNear(1.0001, 1.0, -1));
            CHECK_TRUE(x_f64u::isNear(x_f64u::positiveMinimum(), 0.0f, -80));
            CHECK_TRUE(x_f64u::isNear(x_f64u::negativeMinimum(), 0.0f, -80));
        }

        UNITTEST_TEST(RoundToInt)
        {
            CHECK_EQUAL(x_f64u::roundToInt(0.5), 1);
            CHECK_EQUAL(x_f64u::roundToInt(0.4), 0);
            CHECK_EQUAL(x_f64u::roundToInt(14.0/3.0), 5);
        }

        UNITTEST_TEST(Squared)
        {
            CHECK_TRUE(x_f64u::squared(0.5) == 0.25);
            CHECK_TRUE(x_f64u::squared(0.4) == 0.4 * 0.4);
            CHECK_TRUE(x_f64u::squared(0.1) == 0.1 * 0.1);
            CHECK_TRUE(x_f64u::squared(14.0/3.0) == (14.0 / 3.0) * (14.0 / 3.0));
        }

        UNITTEST_TEST(OneOver)
        {
            CHECK_TRUE(x_f64u::oneOver(0.5) == 1.0f/0.5);
            CHECK_TRUE(x_f64u::oneOver(0.4) == 1.0f/0.4);
            CHECK_TRUE(x_f64u::oneOver(0.1) == 1.0f/0.1);
            CHECK_TRUE(x_f64u::oneOver(14.0/3.0) == 1.0/(14.0 / 3.0));
        }

        UNITTEST_TEST(IsZero)
        {
            CHECK_FALSE(x_f64u::isZero(0.0001));
            CHECK_TRUE(x_f64u::isZero(x_f64u::positiveZero()));
            CHECK_TRUE(x_f64u::isZero(x_f64u::negativeZero()));
        }

        UNITTEST_TEST(IsPositive)
        {
            CHECK_FALSE(x_f64u::isPositive(0.0));
            CHECK_TRUE(x_f64u::isPositive(1.0));
            CHECK_FALSE(x_f64u::isPositive(-2.0));
        }

        UNITTEST_TEST(IsNegative)
        {
            CHECK_FALSE(x_f64u::isNegative(0.0));
            CHECK_FALSE(x_f64u::isNegative(1.0));
            CHECK_TRUE(x_f64u::isNegative(-2.0));
        }

        UNITTEST_TEST(IsLessPositive)
        {
            CHECK_TRUE(x_f64u::isLessPositive(0.1, 1.0));
            CHECK_FALSE(x_f64u::isLessPositive(1.0, 0.1));
            CHECK_TRUE(x_f64u::isLessPositive(x_f64u::positiveMinimum(),0.0001));
        }

        UNITTEST_TEST(IsGreater)
        {
            CHECK_FALSE(x_f64u::isGreater(0.1, 1.0));
            CHECK_TRUE(x_f64u::isGreater(1.0, 0.1));
            CHECK_FALSE(x_f64u::isGreater(x_f64u::positiveMinimum(),0.0001));
        }
    }
}
UNITTEST_SUITE_END
