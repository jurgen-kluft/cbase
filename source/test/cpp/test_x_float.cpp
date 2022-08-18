#include "cbase/c_float.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(xfloat)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(Abs)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(xf32::abs(a), 1.0f);
			CHECK_EQUAL(xf32::abs(b), 0.0f);
			CHECK_EQUAL(xf32::abs(c), 1.0f);
		}

		UNITTEST_TEST(Neg)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(xf32::neg(a), -1.0f);
			CHECK_EQUAL(xf32::neg(b), 0.0f);
			CHECK_EQUAL(xf32::neg(c), 1.0f);
		}

		UNITTEST_TEST(Min)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(xf32::fmin(a,b), 0.0f);
			CHECK_EQUAL(xf32::fmin(b,c), -1.0f);
			CHECK_EQUAL(xf32::fmin(c,a), -1.0f);
		}

		UNITTEST_TEST(Max)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(xf32::fmax(a,b), 1.0f);
			CHECK_EQUAL(xf32::fmax(b,c), 0.0f);
			CHECK_EQUAL(xf32::fmax(c,a), 1.0f);
		}

		UNITTEST_TEST(Clamp)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(xf32::clamp(a,b,a), 1.0f);
			CHECK_EQUAL(xf32::clamp(b,c,c), -1.0f);
			CHECK_EQUAL(xf32::clamp(c,b,a), 0.0f);
		}

		UNITTEST_TEST(PositiveZero)
		{
#ifndef COMPILER_SN_PSP // PSP does not support infinite value
			CHECK_TRUE(1.0f / xf32::positiveZero() >= 1.0f);
#endif
			CHECK_EQUAL(xf32::positiveZero(), -1.0f * -1.0f * 0.0f);
		}

		UNITTEST_TEST(NegativeZero)
		{
			CHECK_EQUAL(xf32::negativeZero(), -1.0f * 0.0f);
			CHECK_EQUAL(xf32::negativeZero() * xf32::negativeZero(), xf32::positiveZero());
#ifndef COMPILER_SN_PSP // PSP does not support infinite value
			CHECK_TRUE(1.0f / xf32::negativeZero() <= -1.0f);
			CHECK_TRUE(1.0f / (xf32::negativeZero()*xf32::negativeZero()) >= 1.0f);
#endif
		}
#ifdef IEEE_FLOATS
		UNITTEST_TEST(Nan)
		{
			CHECK_NOT_EQUAL(xf32::nan(), xf32::nan());
		}

		UNITTEST_TEST(PositiveInfinity)
		{
			CHECK_EQUAL(xf32::positiveInfinity() + 1.0f, xf32::positiveInfinity());
			CHECK_EQUAL(xf32::positiveInfinity(), (1.0f / xf32::positiveZero()));
			CHECK_TRUE(xf32::positiveInfinity() >= 0xffffffff);
			CHECK_TRUE(xf32::bin(xf32::positiveInfinity()) == (u32)0x7f800000);
		}

		UNITTEST_TEST(NegativeInfinity)
		{
			CHECK_EQUAL(xf32::negativeInfinity() - 1.0f, xf32::negativeInfinity());
			CHECK_EQUAL(xf32::negativeInfinity(), (1.0f / xf32::negativeZero()));
			CHECK_TRUE(xf32::negativeInfinity() <= 0xffffffff);
			CHECK_TRUE(xf32::bin(xf32::negativeInfinity()) == (u32)0xff800000);
		}
#endif // IEEE_FLOATS
		UNITTEST_TEST(PositiveMaximum)
		{
			CHECK_EQUAL(xf32::positiveMaximum(), xf32::positiveMaximum());
			CHECK_TRUE(xf32::positiveMaximum() >= (f32)0xffffffff);
			CHECK_TRUE(xf32::positiveMaximum() >= 9999.9f);
		}

		UNITTEST_TEST(NegativeMaximum)
		{
			CHECK_EQUAL(xf32::negativeMaximum(), xf32::negativeMaximum());
			CHECK_TRUE(xf32::negativeMaximum() <= (f32)0xffffffff);
			CHECK_TRUE(xf32::negativeMaximum() <= (f32)0x000fffff);
			CHECK_TRUE(xf32::negativeMaximum() <= 0.1f);
		}

		UNITTEST_TEST(PositiveMinimum)
		{
			CHECK_EQUAL(xf32::positiveMinimum(), xf32::positiveMinimum());
			CHECK_TRUE(xf32::positiveMinimum() > 0.0f);
			CHECK_TRUE(xf32::positiveMinimum() <= 1.0f);
			CHECK_TRUE(xf32::positiveMinimum() > xf32::positiveZero());
		}

		UNITTEST_TEST(NegativeMinimum)
		{
			CHECK_EQUAL(xf32::negativeMinimum(), xf32::negativeMinimum());
			CHECK_TRUE(xf32::negativeMinimum() < 0.0f);
			CHECK_TRUE(xf32::negativeMinimum() < xf32::negativeZero());
			CHECK_TRUE(xf32::negativeMinimum() > -0.1f);
		}

		UNITTEST_TEST(IsPositiveZero)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(xf32::isPositiveZero(a));
			CHECK_TRUE(!xf32::isPositiveZero(b));
			CHECK_TRUE(xf32::isPositiveZero(xf32::positiveZero()));
			CHECK_TRUE(!xf32::isPositiveZero(xf32::negativeZero()));
		}

		UNITTEST_TEST(IsNegativeZero)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!xf32::isNegativeZero(a));
			CHECK_TRUE(!xf32::isNegativeZero(b));
			CHECK_TRUE(xf32::isNegativeZero(xf32::negativeZero()));
			CHECK_TRUE(!xf32::isNegativeZero(xf32::positiveZero()));
		}
#ifdef IEEE_FLOATS
		UNITTEST_TEST(IsPositiveInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!xf32::isPositiveInfinite(a));
			CHECK_TRUE(!xf32::isPositiveInfinite(b));
			CHECK_TRUE(xf32::isPositiveInfinite(xf32::positiveInfinity()));
			CHECK_TRUE(!xf32::isPositiveInfinite(xf32::negativeInfinity()));
		}

		UNITTEST_TEST(IsNegativeInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!xf32::isNegativeInfinite(a));
			CHECK_TRUE(!xf32::isNegativeInfinite(b));
			CHECK_TRUE(xf32::isNegativeInfinite(xf32::negativeInfinity()));
			CHECK_TRUE(!xf32::isNegativeInfinite(xf32::positiveInfinity()));
		}

		UNITTEST_TEST(IsInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!xf32::isInfinite(a));
			CHECK_TRUE(!xf32::isInfinite(b));
			CHECK_TRUE(xf32::isInfinite(xf32::negativeInfinity()));
			CHECK_TRUE(xf32::isInfinite(xf32::positiveInfinity()));
		}

		UNITTEST_TEST(IsNAN)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!xf32::isNAN(a));
			CHECK_TRUE(!xf32::isNAN(b));
			CHECK_TRUE(xf32::isNAN(xf32::nan()));
			CHECK_TRUE(xf32::isNAN(xf32::nan()-1.0f));
			CHECK_TRUE(!xf32::isNAN(xf32::negativeInfinity()));
		}

		UNITTEST_TEST(IsFinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(xf32::isFinite(a));
			CHECK_TRUE(xf32::isFinite(b));
			CHECK_TRUE(!xf32::isFinite(xf32::negativeInfinity()));
			CHECK_TRUE(!xf32::isFinite(xf32::nan()));
		}

		UNITTEST_TEST(IsRational)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(xf32::isRational(a));
			CHECK_TRUE(xf32::isRational(b));
			CHECK_TRUE(!xf32::isRational(xf32::negativeInfinity()));
			CHECK_TRUE(!xf32::isRational(xf32::nan()));
		}
#endif
		UNITTEST_TEST(Bin)
		{
			CHECK_TRUE(xf32::bin(0.0f) == 0);
#ifdef IEEE_FLOATS
			CHECK_TRUE(xf32::bin(xf32::nan()) == xf32::bin(xf32::nan()+1));
#endif
			CHECK_TRUE(xf32::bin(1.0f) == (u32)0x3f800000);
			CHECK_TRUE(xf32::bin(-2.0f) == (u32)0xc0000000);
		}

		UNITTEST_TEST(Sbin)
		{
			CHECK_EQUAL(xf32::sbin(0.0f), 0);
			CHECK_EQUAL(xf32::sbin(-2.0f), (s32)0xc0000000);
#ifdef IEEE_FLOATS
			CHECK_EQUAL(xf32::sbin(xf32::nan()), xf32::sbin(xf32::nan()+1));
#endif
		}

		UNITTEST_TEST(ToF32)
		{
			CHECK_EQUAL(xf32::toF32((u32)0), 0.0f);
			//infinity is 0x7f800000
			CHECK_EQUAL(xf32::toF32(0x7f800000) + 1.0f, xf32::toF32(0x7f800000));
			CHECK_EQUAL(xf32::toF32(0x3f800000), 1.0f);
			CHECK_EQUAL(xf32::toF32(0xc0000000), -2.0f);
		}

		UNITTEST_TEST(ToS32)
		{
			CHECK_EQUAL(xf32::toS32(0.0f), 0);
			CHECK_EQUAL(xf32::toS32(-2.0f), (s32)-2);
			CHECK_EQUAL(xf32::toS32(2000.0f), (s32)2000);
		}

		UNITTEST_TEST(ToU32)
		{
			CHECK_TRUE(xf32::toU32(0.0f) == 0);
			CHECK_TRUE(xf32::toU32(20.0f) == (u32)20);
		}

		UNITTEST_TEST(IsEqual)
		{
			CHECK_TRUE(!xf32::isEqual(0.0001f,0.0f));
			CHECK_TRUE(xf32::isEqual(20.0f,20.000f));
#ifdef IEEE_FLOATS
			CHECK_TRUE(xf32::isEqual(xf32::negativeZero(),-0.0f));
			CHECK_TRUE(xf32::isEqual(xf32::nan(),xf32::nan()));
#endif
		}

		UNITTEST_TEST(IsNotEqual)
		{
			CHECK_TRUE(xf32::isNotEqual(0.0001f,0.0f));
			CHECK_TRUE(!xf32::isNotEqual(20.0f,20.000f));
#ifdef IEEE_FLOATS
			CHECK_TRUE(!xf32::isNotEqual(xf32::negativeZero(),-0.0f));
			CHECK_TRUE(!xf32::isNotEqual(xf32::nan(),xf32::nan()));
#endif
		}

		UNITTEST_TEST(BinaryAnd)
		{
			CHECK_EQUAL(xf32::binaryAnd(0.0f,(u32)0), 0.0f);
			CHECK_EQUAL(xf32::binaryAnd(-2.0f,(u32)0x10000000), xf32::toF32(0xc0000000&0x10000000));
		}

		UNITTEST_TEST(BinaryOr)
		{
			CHECK_EQUAL(xf32::binaryOr(0.0f,(u32)0), 0.0f);
			CHECK_EQUAL(xf32::binaryOr(-2.0f,(u32)10), xf32::toF32(0xc0000000|10));
		}

		UNITTEST_TEST(Fraction)
		{
			CHECK_EQUAL(xf32::fraction(0.0f), 0);
			CHECK_EQUAL(xf32::fraction(-2.0f),0);
			//0.15625f : 0  01111100  01000000000000000000000
			CHECK_EQUAL(xf32::fraction(0.15625f),(s32)(0x200000));
		}

		UNITTEST_TEST(ExponentBinary)
		{
			CHECK_EQUAL(xf32::exponentBinary(0.0f), 0);
			CHECK_EQUAL(xf32::exponentBinary(-2.0f),(s32)128);
			CHECK_EQUAL(xf32::exponentBinary(0.15625f),(s32)(0x7c));
		}

		UNITTEST_TEST(Exponent)
		{
			CHECK_EQUAL(xf32::exponent(1.0f), 0);
			CHECK_EQUAL(xf32::exponent(-2.0f),(s32)1);
			CHECK_EQUAL(xf32::exponent(0.15625f),(s32)(0x7c - 0x7f));
		}

		UNITTEST_TEST(SignMask)
		{
			CHECK_TRUE(xf32::signMask(1.0f) == 0);
			CHECK_TRUE(xf32::signMask(-2.0f) == (u32)0x80000000);
			CHECK_TRUE(xf32::signMask(0.15625f) == 0);
		}

		UNITTEST_TEST(SignBit)
		{
			CHECK_EQUAL(xf32::signBit(1.0f), 0);
			CHECK_EQUAL(xf32::signBit(-2.0f),1);
			CHECK_EQUAL(xf32::signBit(0.15625f),0);
		}

		UNITTEST_TEST(SignBitSigned)
		{
			CHECK_EQUAL(xf32::signBitSigned(1.0f), 0);
			CHECK_EQUAL(xf32::signBitSigned(-2.0f),-1);
			CHECK_EQUAL(xf32::signBitSigned(0.15625f),0);
			CHECK_EQUAL(xf32::signBitSigned(xf32::positiveZero()),0);
			CHECK_EQUAL(xf32::signBitSigned(xf32::negativeZero()),-1);
		}

		UNITTEST_TEST(Sign)
		{
			CHECK_EQUAL(xf32::sign(1.0f), 1);
			CHECK_EQUAL(xf32::sign(-2.0f),-1);
			CHECK_EQUAL(xf32::sign(0.15625f),1);
			CHECK_EQUAL(xf32::sign(xf32::positiveZero()),0);
			CHECK_EQUAL(xf32::sign(xf32::negativeZero()),0);
		}

		UNITTEST_TEST(Sign2)
		{
			CHECK_EQUAL(xf32::sign(1.0f, 0.5f), 1);
			CHECK_EQUAL(xf32::sign(-2.0f, 0.5f),-1);
			CHECK_EQUAL(xf32::sign(0.15625f, 0.5),0);
			CHECK_EQUAL(xf32::sign(xf32::negativeZero(), 0.1f),0);

			// this will break psp release compiling
			// not compile errors, just break the compiler
			// TODO: fix it
			//CHECK_EQUAL(xf32::sign(0.1f, xf32::positiveZero()),1);
		}

		UNITTEST_TEST(IsZero)
		{
			CHECK_FALSE(xf32::isZero(0.0001f));
			CHECK_TRUE(xf32::isZero(xf32::negativeZero()));

			// this will break psp release compiling
			// not compile errors, just break the compiler
			// TODO: fix it
			//CHECK_TRUE(xf32::isZero(xf32::positiveZero()));
		}

		UNITTEST_TEST(IsNearZero)
		{
			CHECK_FALSE(xf32::isNearZero(0.0001f));
			CHECK_TRUE(xf32::isNearZero(0.0001f, -1));
			CHECK_TRUE(xf32::isNearZero(xf32::positiveZero()));
			CHECK_TRUE(xf32::isNearZero(xf32::negativeZero()));
		}

		UNITTEST_TEST(IsNear)
		{
			CHECK_FALSE(xf32::isNear(0.0001f, 0.0f, -80));
			CHECK_TRUE(xf32::isNear(0.0001f, 0.0f, -1));
			CHECK_FALSE(xf32::isNear(1.0001f, 1.0f, -80));
			CHECK_TRUE(xf32::isNear(1.0001f, 1.0f, -1));
			CHECK_TRUE(xf32::isNear(xf32::positiveZero(), 0.0f, -80));
			CHECK_TRUE(xf32::isNear(xf32::negativeZero(), 0.0f, -80));
		}

		UNITTEST_TEST(RoundToInt)
		{
			CHECK_EQUAL(xf32::roundToInt(0.5f), 1);
			CHECK_EQUAL(xf32::roundToInt(0.4f), 0);
			CHECK_EQUAL(xf32::roundToInt(14.0f/3.0f), 5);
		}

		UNITTEST_TEST(Squared)
		{
			CHECK_EQUAL(xf32::squared(0.5f), 0.25f);
			CHECK_EQUAL(xf32::squared(0.4f), 0.4f * 0.4f);
			CHECK_EQUAL(xf32::squared(0.1f), 0.1f * 0.1f);
			CHECK_EQUAL(xf32::squared(14.0f/3.0f), (14.0f / 3.0f) * (14.0f / 3.0f));
		}

		UNITTEST_TEST(OneOver)
		{
			CHECK_EQUAL(xf32::oneOver(0.5f), 1.0f/0.5f);
			CHECK_EQUAL(xf32::oneOver(0.4f), 1.0f/0.4f);
			CHECK_EQUAL(xf32::oneOver(0.1f), 1.0f/0.1f);
			CHECK_EQUAL(xf32::oneOver(14.0f/3.0f), 1.0f/(14.0f / 3.0f));
		}

		UNITTEST_TEST(ScaledFloatToInt)
		{
			CHECK_EQUAL(xf32::scaledFloatToInt(0.0f, 4), 0);
			CHECK_EQUAL(xf32::scaledFloatToInt(1.0f, 3), 7);
			CHECK_EQUAL(xf32::scaledFloatToInt(0.5f, 4), 8);
			CHECK_EQUAL(xf32::scaledFloatToInt(3.0f/16.0f, 4), 3);
		}

		UNITTEST_TEST(MinimumIncrement)
		{
			CHECK_EQUAL(xf32::minimumIncrement(0.0f), xf32::toF32(0x7f800000));
			CHECK_EQUAL(xf32::minimumIncrement(1.0f), xf32::toF32(0x3f800001));
			CHECK_EQUAL(xf32::minimumIncrement(-2.0f), xf32::toF32(0xbffffffe));
		}

		UNITTEST_TEST(IsPositive)
		{
			CHECK_FALSE(xf32::isPositive(0.0f));
			CHECK_TRUE(xf32::isPositive(1.0f));
			CHECK_FALSE(xf32::isPositive(-2.0f));
		}

		UNITTEST_TEST(IsNegative)
		{
			CHECK_FALSE(xf32::isNegative(0.0f));
			CHECK_FALSE(xf32::isNegative(1.0f));
			CHECK_TRUE(xf32::isNegative(-2.0f));
		}

		UNITTEST_TEST(IsLessPositive)
		{
			CHECK_TRUE(xf32::isLessPositive(0.1f, 1.0f));
			CHECK_FALSE(xf32::isLessPositive(1.0f, 0.1f));
			CHECK_TRUE(xf32::isLessPositive(xf32::positiveMinimum(),0.0001f));
		}

		UNITTEST_TEST(IsGreater)
		{
			CHECK_FALSE(xf32::isGreater(0.1f, 1.0f));
			CHECK_TRUE(xf32::isGreater(1.0f, 0.1f));
			CHECK_FALSE(xf32::isGreater(xf32::positiveMinimum(),0.0001f));
		}
	}
}
UNITTEST_SUITE_END
