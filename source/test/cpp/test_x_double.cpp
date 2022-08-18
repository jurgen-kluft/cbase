#include "cbase/c_double.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(xdouble)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(Abs)
        {
            f32 a = 1.0, b = 0.0, c = -1.0;
            CHECK_TRUE(xf64::abs(a) == 1.0);
            CHECK_TRUE(xf64::abs(b) == 0.0);
            CHECK_TRUE(xf64::abs(c) == 1.0);
        }

        UNITTEST_TEST(Neg)
        {
            f32 a = 1.0, b = 0.0, c = -1.0;
            CHECK_TRUE(xf64::neg(a) == -1.0);
            CHECK_TRUE(xf64::neg(b) == 0.0);
            CHECK_TRUE(xf64::neg(c) == 1.0);
        }

        UNITTEST_TEST(PositiveZero)
        {
            CHECK_TRUE(1.0 / xf64::positiveZero() >= 1.0);
            CHECK_TRUE(xf64::positiveZero() == -1.0 * -1.0 * 0.0);
        }

        UNITTEST_TEST(NegativeZero)
        {
            CHECK_TRUE(xf64::negativeZero() == -1.0 * 0.0);
            CHECK_TRUE(1.0 / xf64::negativeZero() <= -1.0);
            CHECK_TRUE(xf64::negativeZero() * xf64::negativeZero() == xf64::positiveZero());
            CHECK_TRUE(1.0 / (xf64::negativeZero()*xf64::negativeZero()) >= 1.0);
        }
#ifdef IEEE_FLOATS
        UNITTEST_TEST(Nan)
        {
            CHECK_TRUE(xf64::nan() != xf64::nan());
        }

        UNITTEST_TEST(PositiveInfinity)
        {
            CHECK_TRUE(xf64::positiveInfinity() + 1.0 == xf64::positiveInfinity());
            CHECK_TRUE(xf64::positiveInfinity() == (1.0 / xf64::positiveZero()));
            CHECK_TRUE(xf64::positiveInfinity() >= 0xffffffff);
            CHECK_TRUE(xf64::bin(xf64::positiveInfinity()) == X_CONSTANT_64(0x7ff0000000000000));
        }

        UNITTEST_TEST(NegativeInfinity)
        {
            CHECK_TRUE(xf64::negativeInfinity() - 1.0 == xf64::negativeInfinity());
            CHECK_TRUE(xf64::negativeInfinity() == (1.0 / xf64::negativeZero()));
            CHECK_TRUE(xf64::negativeInfinity() <= 0xffffffff);
            CHECK_TRUE(xf64::bin(xf64::negativeInfinity()) == X_CONSTANT_64(0xfff0000000000000));
        }
#endif
        UNITTEST_TEST(PositiveMaximum)
        {
            CHECK_TRUE(xf64::positiveMaximum() == xf64::positiveMaximum());
            CHECK_TRUE(xf64::positiveMaximum() >= 0xffffffff);
            CHECK_TRUE(xf64::positiveMaximum() >= 9999.9);
        }

        UNITTEST_TEST(NegativeMaximum)
        {
            CHECK_TRUE(xf64::negativeMaximum() == xf64::negativeMaximum());
            CHECK_TRUE(xf64::negativeMaximum() <= 0xffffffff);
            CHECK_TRUE(xf64::negativeMaximum() <= 0x000fffff);
            CHECK_TRUE(xf64::negativeMaximum() <= 0.1);
        }

        UNITTEST_TEST(PositiveMinimum)
        {
            CHECK_TRUE(xf64::positiveMinimum() == xf64::positiveMinimum());
            CHECK_TRUE(xf64::positiveMinimum() > 0.0);
            CHECK_TRUE(xf64::positiveMinimum() <= 1.0);
            CHECK_TRUE(xf64::positiveMinimum() > xf64::positiveZero());
        }

        UNITTEST_TEST(NegativeMinimum)
        {
            CHECK_TRUE(xf64::negativeMinimum() == xf64::negativeMinimum());
            CHECK_TRUE(xf64::negativeMinimum() < 0.0);
            CHECK_TRUE(xf64::negativeMinimum() < xf64::negativeZero());
            CHECK_TRUE(xf64::negativeMinimum() > -0.1);
        }

        UNITTEST_TEST(PositiveMinimumDEN)
        {
            CHECK_TRUE(xf64::positiveMinimumDEN() > 0.0);
            CHECK_TRUE(xf64::positiveMinimumDEN() < xf64::positiveMinimum());
        }

        UNITTEST_TEST(NegativeMinimumDEN)
        {
            CHECK_TRUE(xf64::negativeMinimumDEN() < 0.0);
            CHECK_TRUE(xf64::negativeMinimumDEN() > xf64::negativeMinimum());
        }
#ifdef IEEE_FLOATS
        UNITTEST_TEST(IsInfinite)
        {
            CHECK_FALSE(xf64::isInfinite(0.0));
            CHECK_FALSE(xf64::isInfinite(999.0));
            CHECK_TRUE(xf64::isInfinite(xf64::negativeInfinity()));
            CHECK_TRUE(xf64::isInfinite(xf64::positiveInfinity()));
        }

        UNITTEST_TEST(IsNAN)
        {
            CHECK_FALSE(xf64::isNAN(0.0));
            CHECK_FALSE(xf64::isNAN(999.0));
            CHECK_TRUE(xf64::isNAN(xf64::nan()));
            CHECK_TRUE(xf64::isNAN(xf64::nan()-1.0));
            CHECK_FALSE(xf64::isNAN(xf64::negativeInfinity()));
        }

        UNITTEST_TEST(IsFinite)
        {
            CHECK_TRUE(xf64::isFinite(0.0));
            CHECK_TRUE(xf64::isFinite(999.0));
            CHECK_FALSE(xf64::isFinite(xf64::negativeInfinity()));
            CHECK_FALSE(xf64::isFinite(xf64::nan()));
        }

        UNITTEST_TEST(IsRational)
        {
            CHECK_TRUE(xf64::isRational(0.0));
            CHECK_TRUE(xf64::isRational(999.0));
            CHECK_FALSE(xf64::isRational(xf64::negativeInfinity()));
            CHECK_FALSE(xf64::isRational(xf64::nan()));
        }
#endif
        UNITTEST_TEST(Bin)
        {
            CHECK_TRUE(xf64::bin(0.0) == 0);
#ifdef IEEE_FLOATS
            CHECK_TRUE(xf64::bin(xf64::nan()) == xf64::bin(xf64::nan()+1.0));
#endif
            CHECK_TRUE(xf64::bin(1.0) == X_CONSTANT_64(0x3ff0000000000000));
            CHECK_TRUE(xf64::bin(-2.0) == X_CONSTANT_64(0xc000000000000000));
        }

        UNITTEST_TEST(Sbin)
        {
            CHECK_TRUE(xf64::sbin(0.0) == 0);
            CHECK_TRUE(xf64::sbin(-2.0) == X_CONSTANT_64(0xc000000000000000));
#ifdef IEEE_FLOATS
            CHECK_TRUE(xf64::sbin(xf64::nan()) == xf64::sbin(xf64::nan()+1.0));
#endif
        }

        UNITTEST_TEST(ToF64)
        {
            CHECK_TRUE(xf64::toF64((u64)0) == 0.0);
            CHECK_TRUE(xf64::toF64(X_CONSTANT_64(0x3ff0000000000000)) == 1.0);
            CHECK_TRUE(xf64::toF64(X_CONSTANT_64(0xc000000000000000)) == -2.0);
        }

        UNITTEST_TEST(ToS64)
        {
            CHECK_TRUE(xf64::toS64(0.0) == 0);
            CHECK_TRUE(xf64::toS64(-2.0) == (s64)-2);
            CHECK_TRUE(xf64::toS64(2000.0) == (s64)2000);
        }

        UNITTEST_TEST(ToU64)
        {
            CHECK_TRUE(xf64::toU64(0.0) == 0);
            CHECK_TRUE(xf64::toU64(20.0) == (u64)20);
        }

        UNITTEST_TEST(IsEqual)
        {
            CHECK_FALSE(xf64::isEqual(0.0001,0.0));
            CHECK_TRUE(xf64::isEqual(20.0,20.000));
            CHECK_TRUE(xf64::isEqual(xf64::negativeZero(),-0.0));

#ifdef IEEE_FLOATS
            CHECK_TRUE(xf64::isEqual(xf64::nan(),xf64::nan()));
#endif
        }

        UNITTEST_TEST(IsNotEqual)
        {
            CHECK_TRUE(xf64::isNotEqual(0.0001,0.0));
            CHECK_FALSE(xf64::isNotEqual(20.0,20.000));
            CHECK_FALSE(xf64::isNotEqual(xf64::negativeZero(),-0.0));
#ifdef IEEE_FLOATS
            CHECK_FALSE(xf64::isNotEqual(xf64::nan(),xf64::nan()));
#endif
        }

        UNITTEST_TEST(BinaryAnd)
        {
            CHECK_TRUE(xf64::binaryAnd(0.0,(u64)0) == 0.0);
            CHECK_TRUE(xf64::binaryAnd(-2.0,(u64)0x10000000) == xf64::toF64(X_CONSTANT_64(0xc000000000000000)&0x10000000));
        }

        UNITTEST_TEST(BinaryOr)
        {
            CHECK_TRUE(xf64::binaryOr(0.0,(s32)0) == 0.0);
            CHECK_TRUE(xf64::binaryOr(-2.0,(s32)10) == xf64::toF64(X_CONSTANT_64(0xc000000000000000)|10));
        }

        UNITTEST_TEST(Fraction)
        {
            CHECK_TRUE(xf64::fraction(0.0) == 0);
            CHECK_TRUE(xf64::fraction(-2.0) == 0);
            //0.15625f : 0x3fc4000000000000
            CHECK_TRUE(xf64::fraction(0.15625f) == X_CONSTANT_64(0x0004000000000000));
        }

        UNITTEST_TEST(ExponentBinary)
        {
            CHECK_TRUE(xf64::exponentBinary(0.0) == 0);
            CHECK_TRUE(xf64::exponentBinary(-2.0) == (s32)1024);
            CHECK_TRUE(xf64::exponentBinary(0.15625) == (s32)(0x3fc));
        }

        UNITTEST_TEST(Exponent)
        {
            CHECK_TRUE(xf64::exponent(1.0) == 0);
            CHECK_TRUE(xf64::exponent(-2.0) == (s32)1);
            CHECK_TRUE(xf64::exponent(0.15625) == (s32)(0x37c - 0x37f));
        }

        UNITTEST_TEST(SignMask)
        {
            CHECK_TRUE(xf64::signMask(1.0f) == 0);
            CHECK_TRUE(xf64::signMask(-2.0f) == X_CONSTANT_64(0x8000000000000000));
            CHECK_TRUE(xf64::signMask(0.15625f) == 0);
        }

        UNITTEST_TEST(SignBit)
        {
            CHECK_EQUAL(xf64::signBit(1.0), 0);
            CHECK_EQUAL(xf64::signBit(-2.0),1);
            CHECK_EQUAL(xf64::signBit(0.15625),0);
        }

        UNITTEST_TEST(SignBitSigned)
        {
            CHECK_EQUAL(xf64::signBitSigned(1.0), 0);
            CHECK_EQUAL(xf64::signBitSigned(-2.0),-1);
            CHECK_EQUAL(xf64::signBitSigned(0.15625),0);
            CHECK_EQUAL(xf64::signBitSigned(xf64::positiveZero()),0);
            CHECK_EQUAL(xf64::signBitSigned(xf64::negativeZero()),-1);
        }

        UNITTEST_TEST(Sign)
        {
            CHECK_EQUAL(xf64::sign(1.0), 1);
            CHECK_EQUAL(xf64::sign(-2.0),-1);
            CHECK_EQUAL(xf64::sign(0.15625),1);
            CHECK_EQUAL(xf64::sign(xf64::positiveZero()),0);
            CHECK_EQUAL(xf64::sign(xf64::negativeZero()),0);
        }

        UNITTEST_TEST(Sign2)
        {
            CHECK_EQUAL(xf64::sign(1.0, 0.5), 1);
            CHECK_EQUAL(xf64::sign(-2.0, 0.5),-1);
            CHECK_EQUAL(xf64::sign(0.15625, 0.5),0);
            CHECK_EQUAL(xf64::sign(0.1, xf64::positiveZero()),1);
            CHECK_EQUAL(xf64::sign(xf64::negativeZero(), 0.1),0);
        }

        UNITTEST_TEST(IsNearZero)
        {
            CHECK_FALSE(xf64::isNearZero(0.0001));
            CHECK_TRUE(xf64::isNearZero(0.0001, -1));
            CHECK_TRUE(xf64::isNearZero(xf64::positiveMinimum()));
            CHECK_TRUE(xf64::isNearZero(xf64::negativeMinimum()));
        }

        UNITTEST_TEST(IsNear)
        {
            CHECK_FALSE(xf64::isNear(0.0001, 0.0, -80));
            CHECK_TRUE(xf64::isNear(0.0001, 0.0f, -1));
            CHECK_FALSE(xf64::isNear(1.0001, 1.0, -80));
            CHECK_TRUE(xf64::isNear(1.0001, 1.0, -1));
            CHECK_TRUE(xf64::isNear(xf64::positiveMinimum(), 0.0f, -80));
            CHECK_TRUE(xf64::isNear(xf64::negativeMinimum(), 0.0f, -80));
        }

        UNITTEST_TEST(RoundToInt)
        {
            CHECK_EQUAL(xf64::roundToInt(0.5), 1);
            CHECK_EQUAL(xf64::roundToInt(0.4), 0);
            CHECK_EQUAL(xf64::roundToInt(14.0/3.0), 5);
        }

        UNITTEST_TEST(Squared)
        {
            CHECK_TRUE(xf64::squared(0.5) == 0.25);
            CHECK_TRUE(xf64::squared(0.4) == 0.4 * 0.4);
            CHECK_TRUE(xf64::squared(0.1) == 0.1 * 0.1);
            CHECK_TRUE(xf64::squared(14.0/3.0) == (14.0 / 3.0) * (14.0 / 3.0));
        }

        UNITTEST_TEST(OneOver)
        {
            CHECK_TRUE(xf64::oneOver(0.5) == 1.0f/0.5);
            CHECK_TRUE(xf64::oneOver(0.4) == 1.0f/0.4);
            CHECK_TRUE(xf64::oneOver(0.1) == 1.0f/0.1);
            CHECK_TRUE(xf64::oneOver(14.0/3.0) == 1.0/(14.0 / 3.0));
        }

        UNITTEST_TEST(IsZero)
        {
            CHECK_FALSE(xf64::isZero(0.0001));
            CHECK_TRUE(xf64::isZero(xf64::positiveZero()));
            CHECK_TRUE(xf64::isZero(xf64::negativeZero()));
        }

        UNITTEST_TEST(IsPositive)
        {
            CHECK_FALSE(xf64::isPositive(0.0));
            CHECK_TRUE(xf64::isPositive(1.0));
            CHECK_FALSE(xf64::isPositive(-2.0));
        }

        UNITTEST_TEST(IsNegative)
        {
            CHECK_FALSE(xf64::isNegative(0.0));
            CHECK_FALSE(xf64::isNegative(1.0));
            CHECK_TRUE(xf64::isNegative(-2.0));
        }

        UNITTEST_TEST(IsLessPositive)
        {
            CHECK_TRUE(xf64::isLessPositive(0.1, 1.0));
            CHECK_FALSE(xf64::isLessPositive(1.0, 0.1));
            CHECK_TRUE(xf64::isLessPositive(xf64::positiveMinimum(),0.0001));
        }

        UNITTEST_TEST(IsGreater)
        {
            CHECK_FALSE(xf64::isGreater(0.1, 1.0));
            CHECK_TRUE(xf64::isGreater(1.0, 0.1));
            CHECK_FALSE(xf64::isGreater(xf64::positiveMinimum(),0.0001));
        }
    }
}
UNITTEST_SUITE_END
