#include "xbase/x_float.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xfloat)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(Abs)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(x_f32u::abs(a), 1.0f);
			CHECK_EQUAL(x_f32u::abs(b), 0.0f);
			CHECK_EQUAL(x_f32u::abs(c), 1.0f);
		}

		UNITTEST_TEST(Neg)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(x_f32u::neg(a), -1.0f);
			CHECK_EQUAL(x_f32u::neg(b), 0.0f);
			CHECK_EQUAL(x_f32u::neg(c), 1.0f);
		}

		UNITTEST_TEST(Min)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(x_f32u::fmin(a,b), 0.0f);
			CHECK_EQUAL(x_f32u::fmin(b,c), -1.0f);
			CHECK_EQUAL(x_f32u::fmin(c,a), -1.0f);
		}

		UNITTEST_TEST(Max)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(x_f32u::fmax(a,b), 1.0f);
			CHECK_EQUAL(x_f32u::fmax(b,c), 0.0f);
			CHECK_EQUAL(x_f32u::fmax(c,a), 1.0f);
		}

		UNITTEST_TEST(Clamp)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(x_f32u::clamp(a,b,a), 1.0f);
			CHECK_EQUAL(x_f32u::clamp(b,c,c), -1.0f);
			CHECK_EQUAL(x_f32u::clamp(c,b,a), 0.0f);
		}

		UNITTEST_TEST(PositiveZero)
		{
#ifndef COMPILER_SN_PSP // PSP does not support infinite value
			CHECK_TRUE(1.0f / x_f32u::positiveZero() >= 1.0f);
#endif
			CHECK_EQUAL(x_f32u::positiveZero(), -1.0f * -1.0f * 0.0f);
		}

		UNITTEST_TEST(NegativeZero)
		{
			CHECK_EQUAL(x_f32u::negativeZero(), -1.0f * 0.0f);
			CHECK_EQUAL(x_f32u::negativeZero() * x_f32u::negativeZero(), x_f32u::positiveZero());
#ifndef COMPILER_SN_PSP // PSP does not support infinite value
			CHECK_TRUE(1.0f / x_f32u::negativeZero() <= -1.0f);
			CHECK_TRUE(1.0f / (x_f32u::negativeZero()*x_f32u::negativeZero()) >= 1.0f);
#endif
		}
#ifdef IEEE_FLOATS
		UNITTEST_TEST(Nan)
		{
			CHECK_NOT_EQUAL(x_f32u::nan(), x_f32u::nan());
		}

		UNITTEST_TEST(PositiveInfinity)
		{
			CHECK_EQUAL(x_f32u::positiveInfinity() + 1.0f, x_f32u::positiveInfinity());
			CHECK_EQUAL(x_f32u::positiveInfinity(), (1.0f / x_f32u::positiveZero()));
			CHECK_TRUE(x_f32u::positiveInfinity() >= 0xffffffff);
			CHECK_TRUE(x_f32u::bin(x_f32u::positiveInfinity()) == (u32)0x7f800000);
		}

		UNITTEST_TEST(NegativeInfinity)
		{
			CHECK_EQUAL(x_f32u::negativeInfinity() - 1.0f, x_f32u::negativeInfinity());
			CHECK_EQUAL(x_f32u::negativeInfinity(), (1.0f / x_f32u::negativeZero()));
			CHECK_TRUE(x_f32u::negativeInfinity() <= 0xffffffff);
			CHECK_TRUE(x_f32u::bin(x_f32u::negativeInfinity()) == (u32)0xff800000);
		}
#endif // IEEE_FLOATS
		UNITTEST_TEST(PositiveMaximum)
		{
			CHECK_EQUAL(x_f32u::positiveMaximum(), x_f32u::positiveMaximum());
			CHECK_TRUE(x_f32u::positiveMaximum() >= 0xffffffff);
			CHECK_TRUE(x_f32u::positiveMaximum() >= 9999.9f);
		}

		UNITTEST_TEST(NegativeMaximum)
		{
			CHECK_EQUAL(x_f32u::negativeMaximum(), x_f32u::negativeMaximum());
			CHECK_TRUE(x_f32u::negativeMaximum() <= 0xffffffff);
			CHECK_TRUE(x_f32u::negativeMaximum() <= 0x000fffff);
			CHECK_TRUE(x_f32u::negativeMaximum() <= 0.1f);
		}

		UNITTEST_TEST(PositiveMinimum)
		{
			CHECK_EQUAL(x_f32u::positiveMinimum(), x_f32u::positiveMinimum());
			CHECK_TRUE(x_f32u::positiveMinimum() > 0.0f);
			CHECK_TRUE(x_f32u::positiveMinimum() <= 1.0f);
			CHECK_TRUE(x_f32u::positiveMinimum() > x_f32u::positiveZero());
		}

		UNITTEST_TEST(NegativeMinimum)
		{
			CHECK_EQUAL(x_f32u::negativeMinimum(), x_f32u::negativeMinimum());
			CHECK_TRUE(x_f32u::negativeMinimum() < 0.0f);
			CHECK_TRUE(x_f32u::negativeMinimum() < x_f32u::negativeZero());
			CHECK_TRUE(x_f32u::negativeMinimum() > -0.1f);
		}

		UNITTEST_TEST(IsPositiveZero)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(x_f32u::isPositiveZero(a));
			CHECK_TRUE(!x_f32u::isPositiveZero(b));
			CHECK_TRUE(x_f32u::isPositiveZero(x_f32u::positiveZero()));
			CHECK_TRUE(!x_f32u::isPositiveZero(x_f32u::negativeZero()));
		}

		UNITTEST_TEST(IsNegativeZero)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!x_f32u::isNegativeZero(a));
			CHECK_TRUE(!x_f32u::isNegativeZero(b));
			CHECK_TRUE(x_f32u::isNegativeZero(x_f32u::negativeZero()));
			CHECK_TRUE(!x_f32u::isNegativeZero(x_f32u::positiveZero()));
		}
#ifdef IEEE_FLOATS
		UNITTEST_TEST(IsPositiveInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!x_f32u::isPositiveInfinite(a));
			CHECK_TRUE(!x_f32u::isPositiveInfinite(b));
			CHECK_TRUE(x_f32u::isPositiveInfinite(x_f32u::positiveInfinity()));
			CHECK_TRUE(!x_f32u::isPositiveInfinite(x_f32u::negativeInfinity()));
		}

		UNITTEST_TEST(IsNegativeInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!x_f32u::isNegativeInfinite(a));
			CHECK_TRUE(!x_f32u::isNegativeInfinite(b));
			CHECK_TRUE(x_f32u::isNegativeInfinite(x_f32u::negativeInfinity()));
			CHECK_TRUE(!x_f32u::isNegativeInfinite(x_f32u::positiveInfinity()));
		}

		UNITTEST_TEST(IsInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!x_f32u::isInfinite(a));
			CHECK_TRUE(!x_f32u::isInfinite(b));
			CHECK_TRUE(x_f32u::isInfinite(x_f32u::negativeInfinity()));
			CHECK_TRUE(x_f32u::isInfinite(x_f32u::positiveInfinity()));
		}

		UNITTEST_TEST(IsNAN)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!x_f32u::isNAN(a));
			CHECK_TRUE(!x_f32u::isNAN(b));
			CHECK_TRUE(x_f32u::isNAN(x_f32u::nan()));
			CHECK_TRUE(x_f32u::isNAN(x_f32u::nan()-1.0f));
			CHECK_TRUE(!x_f32u::isNAN(x_f32u::negativeInfinity()));
		}

		UNITTEST_TEST(IsFinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(x_f32u::isFinite(a));
			CHECK_TRUE(x_f32u::isFinite(b));
			CHECK_TRUE(!x_f32u::isFinite(x_f32u::negativeInfinity()));
			CHECK_TRUE(!x_f32u::isFinite(x_f32u::nan()));
		}

		UNITTEST_TEST(IsRational)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(x_f32u::isRational(a));
			CHECK_TRUE(x_f32u::isRational(b));
			CHECK_TRUE(!x_f32u::isRational(x_f32u::negativeInfinity()));
			CHECK_TRUE(!x_f32u::isRational(x_f32u::nan()));
		}
#endif
		UNITTEST_TEST(Bin)
		{
			CHECK_TRUE(x_f32u::bin(0.0f) == 0);
#ifdef IEEE_FLOATS
			CHECK_TRUE(x_f32u::bin(x_f32u::nan()) == x_f32u::bin(x_f32u::nan()+1));
#endif
			CHECK_TRUE(x_f32u::bin(1.0f) == (u32)0x3f800000);
			CHECK_TRUE(x_f32u::bin(-2.0f) == (u32)0xc0000000);
		}

		UNITTEST_TEST(Sbin)
		{
			CHECK_EQUAL(x_f32u::sbin(0.0f), 0);
			CHECK_EQUAL(x_f32u::sbin(-2.0f), (s32)0xc0000000);
#ifdef IEEE_FLOATS
			CHECK_EQUAL(x_f32u::sbin(x_f32u::nan()), x_f32u::sbin(x_f32u::nan()+1));
#endif
		}

		UNITTEST_TEST(ToF32)
		{
			CHECK_EQUAL(x_f32u::toF32((u32)0), 0.0f);
			//infinity is 0x7f800000
			CHECK_EQUAL(x_f32u::toF32(0x7f800000) + 1.0f, x_f32u::toF32(0x7f800000));
			CHECK_EQUAL(x_f32u::toF32(0x3f800000), 1.0f);
			CHECK_EQUAL(x_f32u::toF32(0xc0000000), -2.0f);
		}

		UNITTEST_TEST(ToS32)
		{
			CHECK_EQUAL(x_f32u::toS32(0.0f), 0);
			CHECK_EQUAL(x_f32u::toS32(-2.0f), (s32)-2);
			CHECK_EQUAL(x_f32u::toS32(2000.0f), (s32)2000);
		}

		UNITTEST_TEST(ToU32)
		{
			CHECK_TRUE(x_f32u::toU32(0.0f) == 0);
			CHECK_TRUE(x_f32u::toU32(20.0f) == (u32)20);
		}

		UNITTEST_TEST(IsEqual)
		{
			CHECK_TRUE(!x_f32u::isEqual(0.0001f,0.0f));
			CHECK_TRUE(x_f32u::isEqual(20.0f,20.000f));
#ifdef IEEE_FLOATS
			CHECK_TRUE(x_f32u::isEqual(x_f32u::negativeZero(),-0.0f));
			CHECK_TRUE(x_f32u::isEqual(x_f32u::nan(),x_f32u::nan()));
#endif
		}

		UNITTEST_TEST(IsNotEqual)
		{
			CHECK_TRUE(x_f32u::isNotEqual(0.0001f,0.0f));
			CHECK_TRUE(!x_f32u::isNotEqual(20.0f,20.000f));
#ifdef IEEE_FLOATS
			CHECK_TRUE(!x_f32u::isNotEqual(x_f32u::negativeZero(),-0.0f));
			CHECK_TRUE(!x_f32u::isNotEqual(x_f32u::nan(),x_f32u::nan()));
#endif
		}

		UNITTEST_TEST(BinaryAnd)
		{
			CHECK_EQUAL(x_f32u::binaryAnd(0.0f,(u32)0), 0.0f);
			CHECK_EQUAL(x_f32u::binaryAnd(-2.0f,(u32)0x10000000), x_f32u::toF32(0xc0000000&0x10000000));
		}

		UNITTEST_TEST(BinaryOr)
		{
			CHECK_EQUAL(x_f32u::binaryOr(0.0f,(u32)0), 0.0f);
			CHECK_EQUAL(x_f32u::binaryOr(-2.0f,(u32)10), x_f32u::toF32(0xc0000000|10));
		}

		UNITTEST_TEST(Fraction)
		{
			CHECK_EQUAL(x_f32u::fraction(0.0f), 0);
			CHECK_EQUAL(x_f32u::fraction(-2.0f),0);
			//0.15625f : 0  01111100  01000000000000000000000
			CHECK_EQUAL(x_f32u::fraction(0.15625f),(s32)(0x200000));
		}

		UNITTEST_TEST(ExponentBinary)
		{
			CHECK_EQUAL(x_f32u::exponentBinary(0.0f), 0);
			CHECK_EQUAL(x_f32u::exponentBinary(-2.0f),(s32)128);
			CHECK_EQUAL(x_f32u::exponentBinary(0.15625f),(s32)(0x7c));
		}

		UNITTEST_TEST(Exponent)
		{
			CHECK_EQUAL(x_f32u::exponent(1.0f), 0);
			CHECK_EQUAL(x_f32u::exponent(-2.0f),(s32)1);
			CHECK_EQUAL(x_f32u::exponent(0.15625f),(s32)(0x7c - 0x7f));
		}

		UNITTEST_TEST(SignMask)
		{
			CHECK_TRUE(x_f32u::signMask(1.0f) == 0);
			CHECK_TRUE(x_f32u::signMask(-2.0f) == (u32)0x80000000);
			CHECK_TRUE(x_f32u::signMask(0.15625f) == 0);
		}

		UNITTEST_TEST(SignBit)
		{
			CHECK_EQUAL(x_f32u::signBit(1.0f), 0);
			CHECK_EQUAL(x_f32u::signBit(-2.0f),1);
			CHECK_EQUAL(x_f32u::signBit(0.15625f),0);
		}

		UNITTEST_TEST(SignBitSigned)
		{
			CHECK_EQUAL(x_f32u::signBitSigned(1.0f), 0);
			CHECK_EQUAL(x_f32u::signBitSigned(-2.0f),-1);
			CHECK_EQUAL(x_f32u::signBitSigned(0.15625f),0);
			CHECK_EQUAL(x_f32u::signBitSigned(x_f32u::positiveZero()),0);
			CHECK_EQUAL(x_f32u::signBitSigned(x_f32u::negativeZero()),-1);
		}

		UNITTEST_TEST(Sign)
		{
			CHECK_EQUAL(x_f32u::sign(1.0f), 1);
			CHECK_EQUAL(x_f32u::sign(-2.0f),-1);
			CHECK_EQUAL(x_f32u::sign(0.15625f),1);
			CHECK_EQUAL(x_f32u::sign(x_f32u::positiveZero()),0);
			CHECK_EQUAL(x_f32u::sign(x_f32u::negativeZero()),0);
		}

		UNITTEST_TEST(Sign2)
		{
			CHECK_EQUAL(x_f32u::sign(1.0f, 0.5f), 1);
			CHECK_EQUAL(x_f32u::sign(-2.0f, 0.5f),-1);
			CHECK_EQUAL(x_f32u::sign(0.15625f, 0.5),0);
			CHECK_EQUAL(x_f32u::sign(x_f32u::negativeZero(), 0.1f),0);

			// this will break psp release compiling
			// not compile errors, just break the compiler
			// TODO: fix it
			//CHECK_EQUAL(x_f32u::sign(0.1f, x_f32u::positiveZero()),1);
		}

		UNITTEST_TEST(IsZero)
		{
			CHECK_FALSE(x_f32u::isZero(0.0001f));
			CHECK_TRUE(x_f32u::isZero(x_f32u::negativeZero()));

			// this will break psp release compiling
			// not compile errors, just break the compiler
			// TODO: fix it
			//CHECK_TRUE(x_f32u::isZero(x_f32u::positiveZero()));
		}

		UNITTEST_TEST(IsNearZero)
		{
			CHECK_FALSE(x_f32u::isNearZero(0.0001f));
			CHECK_TRUE(x_f32u::isNearZero(0.0001f, -1));
			CHECK_TRUE(x_f32u::isNearZero(x_f32u::positiveZero()));
			CHECK_TRUE(x_f32u::isNearZero(x_f32u::negativeZero()));
		}

		UNITTEST_TEST(IsNear)
		{
			CHECK_FALSE(x_f32u::isNear(0.0001f, 0.0f, -80));
			CHECK_TRUE(x_f32u::isNear(0.0001f, 0.0f, -1));
			CHECK_FALSE(x_f32u::isNear(1.0001f, 1.0f, -80));
			CHECK_TRUE(x_f32u::isNear(1.0001f, 1.0f, -1));
			CHECK_TRUE(x_f32u::isNear(x_f32u::positiveZero(), 0.0f, -80));
			CHECK_TRUE(x_f32u::isNear(x_f32u::negativeZero(), 0.0f, -80));
		}

		UNITTEST_TEST(RoundToInt)
		{
			CHECK_EQUAL(x_f32u::roundToInt(0.5f), 1);
			CHECK_EQUAL(x_f32u::roundToInt(0.4f), 0);
			CHECK_EQUAL(x_f32u::roundToInt(14.0f/3.0f), 5);
		}

		UNITTEST_TEST(Squared)
		{
			CHECK_EQUAL(x_f32u::squared(0.5f), 0.25f);
			CHECK_EQUAL(x_f32u::squared(0.4f), 0.4f * 0.4f);
			CHECK_EQUAL(x_f32u::squared(0.1f), 0.1f * 0.1f);
			CHECK_EQUAL(x_f32u::squared(14.0f/3.0f), (14.0f / 3.0f) * (14.0f / 3.0f));
		}

		UNITTEST_TEST(OneOver)
		{
			CHECK_EQUAL(x_f32u::oneOver(0.5f), 1.0f/0.5f);
			CHECK_EQUAL(x_f32u::oneOver(0.4f), 1.0f/0.4f);
			CHECK_EQUAL(x_f32u::oneOver(0.1f), 1.0f/0.1f);
			CHECK_EQUAL(x_f32u::oneOver(14.0f/3.0f), 1.0f/(14.0f / 3.0f));
		}

		UNITTEST_TEST(ScaledFloatToInt)
		{
			CHECK_EQUAL(x_f32u::scaledFloatToInt(0.0f, 4), 0);
			CHECK_EQUAL(x_f32u::scaledFloatToInt(1.0f, 3), 7);
			CHECK_EQUAL(x_f32u::scaledFloatToInt(0.5f, 4), 8);
			CHECK_EQUAL(x_f32u::scaledFloatToInt(3.0f/16.0f, 4), 3);
		}

		UNITTEST_TEST(MinimumIncrement)
		{
			CHECK_EQUAL(x_f32u::minimumIncrement(0.0f), x_f32u::toF32(0x7f800000));
			CHECK_EQUAL(x_f32u::minimumIncrement(1.0f), x_f32u::toF32(0x3f800001));
			CHECK_EQUAL(x_f32u::minimumIncrement(-2.0f), x_f32u::toF32(0xbffffffe));
		}

		UNITTEST_TEST(IsPositive)
		{
			CHECK_FALSE(x_f32u::isPositive(0.0f));
			CHECK_TRUE(x_f32u::isPositive(1.0f));
			CHECK_FALSE(x_f32u::isPositive(-2.0f));
		}

		UNITTEST_TEST(IsNegative)
		{
			CHECK_FALSE(x_f32u::isNegative(0.0f));
			CHECK_FALSE(x_f32u::isNegative(1.0f));
			CHECK_TRUE(x_f32u::isNegative(-2.0f));
		}

		UNITTEST_TEST(IsLessPositive)
		{
			CHECK_TRUE(x_f32u::isLessPositive(0.1f, 1.0f));
			CHECK_FALSE(x_f32u::isLessPositive(1.0f, 0.1f));
			CHECK_TRUE(x_f32u::isLessPositive(x_f32u::positiveMinimum(),0.0001f));
		}

		UNITTEST_TEST(IsGreater)
		{
			CHECK_FALSE(x_f32u::isGreater(0.1f, 1.0f));
			CHECK_TRUE(x_f32u::isGreater(1.0f, 0.1f));
			CHECK_FALSE(x_f32u::isGreater(x_f32u::positiveMinimum(),0.0001f));
		}
	}
}
UNITTEST_SUITE_END
