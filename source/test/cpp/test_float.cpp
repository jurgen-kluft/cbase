#include "cbase/c_float.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_float)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(Pow)
        {
            f32 x = 3.0;
            u32 n = 5;
            f32 r = math::powf(x, n);
            CHECK_TRUE(r == 243.0);
        }

		UNITTEST_TEST(Abs)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(math::abs(a), 1.0f);
			CHECK_EQUAL(math::abs(b), 0.0f);
			CHECK_EQUAL(math::abs(c), 1.0f);
		}

		UNITTEST_TEST(Neg)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(math::neg(a), -1.0f);
			CHECK_EQUAL(math::neg(b), 0.0f);
			CHECK_EQUAL(math::neg(c), 1.0f);
		}

		UNITTEST_TEST(Min)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(math::fmin(a,b), 0.0f);
			CHECK_EQUAL(math::fmin(b,c), -1.0f);
			CHECK_EQUAL(math::fmin(c,a), -1.0f);
		}

		UNITTEST_TEST(Max)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(math::fmax(a,b), 1.0f);
			CHECK_EQUAL(math::fmax(b,c), 0.0f);
			CHECK_EQUAL(math::fmax(c,a), 1.0f);
		}

		UNITTEST_TEST(Clamp)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(math::clamp(a,b,a), 1.0f);
			CHECK_EQUAL(math::clamp(b,c,c), -1.0f);
			CHECK_EQUAL(math::clamp(c,b,a), 0.0f);
		}

		UNITTEST_TEST(PositiveZero)
		{
#ifndef COMPILER_SN_PSP // PSP does not support infinite value
			CHECK_TRUE(1.0f / math::positiveZero() >= 1.0f);
#endif
			CHECK_EQUAL(math::positiveZero(), -1.0f * -1.0f * 0.0f);
		}

		UNITTEST_TEST(NegativeZero)
		{
			CHECK_EQUAL(math::negativeZero(), -1.0f * 0.0f);
			CHECK_EQUAL(math::negativeZero() * math::negativeZero(), math::positiveZero());
#ifndef COMPILER_SN_PSP // PSP does not support infinite value
			CHECK_TRUE(1.0f / math::negativeZero() <= -1.0f);
			CHECK_TRUE(1.0f / (math::negativeZero()*math::negativeZero()) >= 1.0f);
#endif
		}
#ifdef D_IEEE_FLOATS
		UNITTEST_TEST(Nan)
		{
			CHECK_NOT_EQUAL(math::nan(), math::nan());
		}

		UNITTEST_TEST(PositiveInfinity)
		{
			CHECK_EQUAL(math::positiveInfinity() + 1.0f, math::positiveInfinity());
			CHECK_EQUAL(math::positiveInfinity(), (1.0f / math::positiveZero()));
			CHECK_TRUE(math::positiveInfinity() >= 0xffffffff);
			CHECK_TRUE(math::bin(math::positiveInfinity()) == (u32)0x7f800000);
		}

		UNITTEST_TEST(NegativeInfinity)
		{
			CHECK_EQUAL(math::negativeInfinity() - 1.0f, math::negativeInfinity());
			CHECK_EQUAL(math::negativeInfinity(), (1.0f / math::negativeZero()));
			CHECK_TRUE(math::negativeInfinity() <= 0xffffffff);
			CHECK_TRUE(math::bin(math::negativeInfinity()) == (u32)0xff800000);
		}
#endif // D_IEEE_FLOATS
		UNITTEST_TEST(PositiveMaximum)
		{
			CHECK_EQUAL(math::positiveMaximum(), math::positiveMaximum());
			CHECK_TRUE(math::positiveMaximum() >= (f32)0xffffffff);
			CHECK_TRUE(math::positiveMaximum() >= 9999.9f);
		}

		UNITTEST_TEST(NegativeMaximum)
		{
			CHECK_EQUAL(math::negativeMaximum(), math::negativeMaximum());
			CHECK_TRUE(math::negativeMaximum() <= (f32)0xffffffff);
			CHECK_TRUE(math::negativeMaximum() <= (f32)0x000fffff);
			CHECK_TRUE(math::negativeMaximum() <= 0.1f);
		}

		UNITTEST_TEST(PositiveMinimum)
		{
			CHECK_EQUAL(math::positiveMinimum(), math::positiveMinimum());
			CHECK_TRUE(math::positiveMinimum() > 0.0f);
			CHECK_TRUE(math::positiveMinimum() <= 1.0f);
			CHECK_TRUE(math::positiveMinimum() > math::positiveZero());
		}

		UNITTEST_TEST(NegativeMinimum)
		{
			CHECK_EQUAL(math::negativeMinimum(), math::negativeMinimum());
			CHECK_TRUE(math::negativeMinimum() < 0.0f);
			CHECK_TRUE(math::negativeMinimum() < math::negativeZero());
			CHECK_TRUE(math::negativeMinimum() > -0.1f);
		}

		UNITTEST_TEST(IsPositiveZero)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(math::isPositiveZero(a));
			CHECK_TRUE(!math::isPositiveZero(b));
			CHECK_TRUE(math::isPositiveZero(math::positiveZero()));
			CHECK_TRUE(!math::isPositiveZero(math::negativeZero()));
		}

		UNITTEST_TEST(IsNegativeZero)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!math::isNegativeZero(a));
			CHECK_TRUE(!math::isNegativeZero(b));
			CHECK_TRUE(math::isNegativeZero(math::negativeZero()));
			CHECK_TRUE(!math::isNegativeZero(math::positiveZero()));
		}
#ifdef D_IEEE_FLOATS
		UNITTEST_TEST(IsPositiveInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!math::isPositiveInfinite(a));
			CHECK_TRUE(!math::isPositiveInfinite(b));
			CHECK_TRUE(math::isPositiveInfinite(math::positiveInfinity()));
			CHECK_TRUE(!math::isPositiveInfinite(math::negativeInfinity()));
		}

		UNITTEST_TEST(IsNegativeInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!math::isNegativeInfinite(a));
			CHECK_TRUE(!math::isNegativeInfinite(b));
			CHECK_TRUE(math::isNegativeInfinite(math::negativeInfinity()));
			CHECK_TRUE(!math::isNegativeInfinite(math::positiveInfinity()));
		}

		UNITTEST_TEST(IsInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!math::isInfinite(a));
			CHECK_TRUE(!math::isInfinite(b));
			CHECK_TRUE(math::isInfinite(math::negativeInfinity()));
			CHECK_TRUE(math::isInfinite(math::positiveInfinity()));
		}

		UNITTEST_TEST(IsNAN)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!math::isNAN(a));
			CHECK_TRUE(!math::isNAN(b));
			CHECK_TRUE(math::isNAN(math::nan()));
			CHECK_TRUE(math::isNAN(math::nan()-1.0f));
			CHECK_TRUE(!math::isNAN(math::negativeInfinity()));
		}

		UNITTEST_TEST(IsFinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(math::isFinite(a));
			CHECK_TRUE(math::isFinite(b));
			CHECK_TRUE(!math::isFinite(math::negativeInfinity()));
			CHECK_TRUE(!math::isFinite(math::nan()));
		}

		UNITTEST_TEST(IsRational)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(math::isRational(a));
			CHECK_TRUE(math::isRational(b));
			CHECK_TRUE(!math::isRational(math::negativeInfinity()));
			CHECK_TRUE(!math::isRational(math::nan()));
		}
#endif
		UNITTEST_TEST(Bin)
		{
			CHECK_TRUE(math::bin(0.0f) == 0);
#ifdef D_IEEE_FLOATS
            CHECK_NOT_EQUAL(math::bin(math::nan()), math::bin(math::nan() + 1));
#endif
			CHECK_TRUE(math::bin(1.0f) == (u32)0x3f800000);
			CHECK_TRUE(math::bin(-2.0f) == (u32)0xc0000000);
		}

		UNITTEST_TEST(Sbin)
		{
			CHECK_EQUAL(math::sbin(0.0f), 0);
			CHECK_EQUAL(math::sbin(-2.0f), (s32)0xc0000000);
#ifdef D_IEEE_FLOATS
            CHECK_NOT_EQUAL(math::sbin(math::nan()), math::sbin(math::nan() + 1));
#endif
		}

		UNITTEST_TEST(ToF32)
		{
			CHECK_EQUAL(math::toF32((u32)0), 0.0f);
			//infinity is 0x7f800000
			CHECK_EQUAL(math::toF32(0x7f800000) + 1.0f, math::toF32(0x7f800000));
			CHECK_EQUAL(math::toF32(0x3f800000), 1.0f);
			CHECK_EQUAL(math::toF32(0xc0000000), -2.0f);
		}

		UNITTEST_TEST(ToS32)
		{
			CHECK_EQUAL(math::toS32(0.0f), 0);
			CHECK_EQUAL(math::toS32(-2.0f), (s32)-2);
			CHECK_EQUAL(math::toS32(2000.0f), (s32)2000);
		}

		UNITTEST_TEST(ToU32)
		{
			CHECK_TRUE(math::toU32(0.0f) == 0);
			CHECK_TRUE(math::toU32(20.0f) == (u32)20);
		}

		UNITTEST_TEST(IsEqual)
		{
			CHECK_TRUE(!math::isEqual(0.0001f,0.0f));
			CHECK_TRUE(math::isEqual(20.0f,20.000f));
#ifdef D_IEEE_FLOATS
			CHECK_TRUE(math::isEqual(math::negativeZero(),-0.0f));
			CHECK_TRUE(math::isEqual(math::nan(),math::nan()));
#endif
		}

		UNITTEST_TEST(IsNotEqual)
		{
			CHECK_TRUE(math::isNotEqual(0.0001f,0.0f));
			CHECK_TRUE(!math::isNotEqual(20.0f,20.000f));
#ifdef D_IEEE_FLOATS
			CHECK_TRUE(!math::isNotEqual(math::negativeZero(),-0.0f));
			CHECK_TRUE(!math::isNotEqual(math::nan(),math::nan()));
#endif
		}

		UNITTEST_TEST(BinaryAnd)
		{
			CHECK_EQUAL(math::binaryAnd(0.0f,(u32)0), 0.0f);
			CHECK_EQUAL(math::binaryAnd(-2.0f,(u32)0x10000000), math::toF32(0xc0000000&0x10000000));
		}

		UNITTEST_TEST(BinaryOr)
		{
			CHECK_EQUAL(math::binaryOr(0.0f,(u32)0), 0.0f);
			CHECK_EQUAL(math::binaryOr(-2.0f,(u32)10), math::toF32(0xc0000000|10));
		}

		UNITTEST_TEST(Fraction)
		{
			CHECK_EQUAL(math::fraction(0.0f), 0);
			CHECK_EQUAL(math::fraction(-2.0f),0);
			//0.15625f : 0  01111100  01000000000000000000000
			CHECK_EQUAL(math::fraction(0.15625f),(s32)(0x200000));
		}

		UNITTEST_TEST(ExponentBinary)
		{
			CHECK_EQUAL(math::exponentBinary(0.0f), 0);
			CHECK_EQUAL(math::exponentBinary(-2.0f),(s32)128);
			CHECK_EQUAL(math::exponentBinary(0.15625f),(s32)(0x7c));
		}

		UNITTEST_TEST(Exponent)
		{
			CHECK_EQUAL(math::exponent(1.0f), 0);
			CHECK_EQUAL(math::exponent(-2.0f),(s32)1);
			CHECK_EQUAL(math::exponent(0.15625f),(s32)(0x7c - 0x7f));
		}

		UNITTEST_TEST(SignMask)
		{
			CHECK_TRUE(math::signMask(1.0f) == 0);
			CHECK_TRUE(math::signMask(-2.0f) == (u32)0x80000000);
			CHECK_TRUE(math::signMask(0.15625f) == 0);
		}

		UNITTEST_TEST(SignBit)
		{
			CHECK_EQUAL(math::signBit(1.0f), 0);
			CHECK_EQUAL(math::signBit(-2.0f),1);
			CHECK_EQUAL(math::signBit(0.15625f),0);
		}

		UNITTEST_TEST(SignBitSigned)
		{
			CHECK_EQUAL(math::signBitSigned(1.0f), 0);
			CHECK_EQUAL(math::signBitSigned(-2.0f),-1);
			CHECK_EQUAL(math::signBitSigned(0.15625f),0);
			CHECK_EQUAL(math::signBitSigned(math::positiveZero()),0);
			CHECK_EQUAL(math::signBitSigned(math::negativeZero()),-1);
		}

		UNITTEST_TEST(Sign)
		{
			CHECK_EQUAL(math::sign(1.0f), 1);
			CHECK_EQUAL(math::sign(-2.0f),-1);
			CHECK_EQUAL(math::sign(0.15625f),1);
			CHECK_EQUAL(math::sign(math::positiveZero()),0);
			CHECK_EQUAL(math::sign(math::negativeZero()),0);
		}

		UNITTEST_TEST(Sign2)
		{
			CHECK_EQUAL(math::sign(1.0f, 0.5f), 1);
			CHECK_EQUAL(math::sign(-2.0f, 0.5f),-1);
			CHECK_EQUAL(math::sign(0.15625f, 0.5),0);
			CHECK_EQUAL(math::sign(math::negativeZero(), 0.1f),0);
		}

		UNITTEST_TEST(IsZero)
		{
			CHECK_FALSE(math::isZero(0.0001f));
			CHECK_TRUE(math::isZero(math::negativeZero()));
		}

		UNITTEST_TEST(IsNearZero)
		{
			CHECK_FALSE(math::isNearZero(0.0001f));
			CHECK_TRUE(math::isNearZero(0.0001f, -1));
			CHECK_TRUE(math::isNearZero(math::positiveZero()));
			CHECK_TRUE(math::isNearZero(math::negativeZero()));
		}

		UNITTEST_TEST(IsNear)
		{
			CHECK_FALSE(math::isNear(0.0001f, 0.0f, -80));
			CHECK_TRUE(math::isNear(0.0001f, 0.0f, -1));
			CHECK_FALSE(math::isNear(1.0001f, 1.0f, -80));
			CHECK_TRUE(math::isNear(1.0001f, 1.0f, -1));
			CHECK_TRUE(math::isNear(math::positiveZero(), 0.0f, -80));
			CHECK_TRUE(math::isNear(math::negativeZero(), 0.0f, -80));
		}

		UNITTEST_TEST(RoundToInt)
		{
			CHECK_EQUAL(math::roundToInt(0.5f), 1);
			CHECK_EQUAL(math::roundToInt(0.4f), 0);
			CHECK_EQUAL(math::roundToInt(14.0f/3.0f), 5);
		}

		UNITTEST_TEST(Squared)
		{
			CHECK_EQUAL(math::squared(0.5f), 0.25f);
			CHECK_EQUAL(math::squared(0.4f), 0.4f * 0.4f);
			CHECK_EQUAL(math::squared(0.1f), 0.1f * 0.1f);
			CHECK_EQUAL(math::squared(14.0f/3.0f), (14.0f / 3.0f) * (14.0f / 3.0f));
		}

		UNITTEST_TEST(OneOver)
		{
			CHECK_EQUAL(math::oneOver(0.5f), 1.0f/0.5f);
			CHECK_EQUAL(math::oneOver(0.4f), 1.0f/0.4f);
			CHECK_EQUAL(math::oneOver(0.1f), 1.0f/0.1f);
			CHECK_EQUAL(math::oneOver(14.0f/3.0f), 1.0f/(14.0f / 3.0f));
		}

		UNITTEST_TEST(ScaledFloatToInt)
		{
			CHECK_EQUAL(math::scaledFloatToInt(0.0f, 4), 0);
			CHECK_EQUAL(math::scaledFloatToInt(1.0f, 3), 7);
			CHECK_EQUAL(math::scaledFloatToInt(0.5f, 4), 8);
			CHECK_EQUAL(math::scaledFloatToInt(3.0f/16.0f, 4), 3);
		}

		UNITTEST_TEST(MinimumIncrement)
		{
			CHECK_EQUAL(math::minimumIncrement(0.0f), math::toF32(0x7f800000));
			CHECK_EQUAL(math::minimumIncrement(1.0f), math::toF32(0x3f800001));
			CHECK_EQUAL(math::minimumIncrement(-2.0f), math::toF32(0xbffffffe));
		}

		UNITTEST_TEST(IsPositive)
		{
			CHECK_FALSE(math::isPositive(0.0f));
			CHECK_TRUE(math::isPositive(1.0f));
			CHECK_FALSE(math::isPositive(-2.0f));
		}

		UNITTEST_TEST(IsNegative)
		{
			CHECK_FALSE(math::isNegative(0.0f));
			CHECK_FALSE(math::isNegative(1.0f));
			CHECK_TRUE(math::isNegative(-2.0f));
		}

		UNITTEST_TEST(IsLessPositive)
		{
			CHECK_TRUE(math::isLessPositive(0.1f, 1.0f));
			CHECK_FALSE(math::isLessPositive(1.0f, 0.1f));
			CHECK_TRUE(math::isLessPositive(math::positiveMinimum(),0.0001f));
		}

		UNITTEST_TEST(IsGreater)
		{
			CHECK_FALSE(math::isGreater(0.1f, 1.0f));
			CHECK_TRUE(math::isGreater(1.0f, 0.1f));
			CHECK_FALSE(math::isGreater(math::positiveMinimum(),0.0001f));
		}
	}
}
UNITTEST_SUITE_END
