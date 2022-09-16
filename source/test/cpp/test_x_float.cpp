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
			CHECK_EQUAL(nf32::abs(a), 1.0f);
			CHECK_EQUAL(nf32::abs(b), 0.0f);
			CHECK_EQUAL(nf32::abs(c), 1.0f);
		}

		UNITTEST_TEST(Neg)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(nf32::neg(a), -1.0f);
			CHECK_EQUAL(nf32::neg(b), 0.0f);
			CHECK_EQUAL(nf32::neg(c), 1.0f);
		}

		UNITTEST_TEST(Min)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(nf32::fmin(a,b), 0.0f);
			CHECK_EQUAL(nf32::fmin(b,c), -1.0f);
			CHECK_EQUAL(nf32::fmin(c,a), -1.0f);
		}

		UNITTEST_TEST(Max)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(nf32::fmax(a,b), 1.0f);
			CHECK_EQUAL(nf32::fmax(b,c), 0.0f);
			CHECK_EQUAL(nf32::fmax(c,a), 1.0f);
		}

		UNITTEST_TEST(Clamp)
		{
			f32 a = 1.0f, b = 0.0f, c = -1.0f;
			CHECK_EQUAL(nf32::clamp(a,b,a), 1.0f);
			CHECK_EQUAL(nf32::clamp(b,c,c), -1.0f);
			CHECK_EQUAL(nf32::clamp(c,b,a), 0.0f);
		}

		UNITTEST_TEST(PositiveZero)
		{
#ifndef COMPILER_SN_PSP // PSP does not support infinite value
			CHECK_TRUE(1.0f / nf32::positiveZero() >= 1.0f);
#endif
			CHECK_EQUAL(nf32::positiveZero(), -1.0f * -1.0f * 0.0f);
		}

		UNITTEST_TEST(NegativeZero)
		{
			CHECK_EQUAL(nf32::negativeZero(), -1.0f * 0.0f);
			CHECK_EQUAL(nf32::negativeZero() * nf32::negativeZero(), nf32::positiveZero());
#ifndef COMPILER_SN_PSP // PSP does not support infinite value
			CHECK_TRUE(1.0f / nf32::negativeZero() <= -1.0f);
			CHECK_TRUE(1.0f / (nf32::negativeZero()*nf32::negativeZero()) >= 1.0f);
#endif
		}
#ifdef IEEE_FLOATS
		UNITTEST_TEST(Nan)
		{
			CHECK_NOT_EQUAL(nf32::nan(), nf32::nan());
		}

		UNITTEST_TEST(PositiveInfinity)
		{
			CHECK_EQUAL(nf32::positiveInfinity() + 1.0f, nf32::positiveInfinity());
			CHECK_EQUAL(nf32::positiveInfinity(), (1.0f / nf32::positiveZero()));
			CHECK_TRUE(nf32::positiveInfinity() >= 0xffffffff);
			CHECK_TRUE(nf32::bin(nf32::positiveInfinity()) == (u32)0x7f800000);
		}

		UNITTEST_TEST(NegativeInfinity)
		{
			CHECK_EQUAL(nf32::negativeInfinity() - 1.0f, nf32::negativeInfinity());
			CHECK_EQUAL(nf32::negativeInfinity(), (1.0f / nf32::negativeZero()));
			CHECK_TRUE(nf32::negativeInfinity() <= 0xffffffff);
			CHECK_TRUE(nf32::bin(nf32::negativeInfinity()) == (u32)0xff800000);
		}
#endif // IEEE_FLOATS
		UNITTEST_TEST(PositiveMaximum)
		{
			CHECK_EQUAL(nf32::positiveMaximum(), nf32::positiveMaximum());
			CHECK_TRUE(nf32::positiveMaximum() >= (f32)0xffffffff);
			CHECK_TRUE(nf32::positiveMaximum() >= 9999.9f);
		}

		UNITTEST_TEST(NegativeMaximum)
		{
			CHECK_EQUAL(nf32::negativeMaximum(), nf32::negativeMaximum());
			CHECK_TRUE(nf32::negativeMaximum() <= (f32)0xffffffff);
			CHECK_TRUE(nf32::negativeMaximum() <= (f32)0x000fffff);
			CHECK_TRUE(nf32::negativeMaximum() <= 0.1f);
		}

		UNITTEST_TEST(PositiveMinimum)
		{
			CHECK_EQUAL(nf32::positiveMinimum(), nf32::positiveMinimum());
			CHECK_TRUE(nf32::positiveMinimum() > 0.0f);
			CHECK_TRUE(nf32::positiveMinimum() <= 1.0f);
			CHECK_TRUE(nf32::positiveMinimum() > nf32::positiveZero());
		}

		UNITTEST_TEST(NegativeMinimum)
		{
			CHECK_EQUAL(nf32::negativeMinimum(), nf32::negativeMinimum());
			CHECK_TRUE(nf32::negativeMinimum() < 0.0f);
			CHECK_TRUE(nf32::negativeMinimum() < nf32::negativeZero());
			CHECK_TRUE(nf32::negativeMinimum() > -0.1f);
		}

		UNITTEST_TEST(IsPositiveZero)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(nf32::isPositiveZero(a));
			CHECK_TRUE(!nf32::isPositiveZero(b));
			CHECK_TRUE(nf32::isPositiveZero(nf32::positiveZero()));
			CHECK_TRUE(!nf32::isPositiveZero(nf32::negativeZero()));
		}

		UNITTEST_TEST(IsNegativeZero)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!nf32::isNegativeZero(a));
			CHECK_TRUE(!nf32::isNegativeZero(b));
			CHECK_TRUE(nf32::isNegativeZero(nf32::negativeZero()));
			CHECK_TRUE(!nf32::isNegativeZero(nf32::positiveZero()));
		}
#ifdef IEEE_FLOATS
		UNITTEST_TEST(IsPositiveInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!nf32::isPositiveInfinite(a));
			CHECK_TRUE(!nf32::isPositiveInfinite(b));
			CHECK_TRUE(nf32::isPositiveInfinite(nf32::positiveInfinity()));
			CHECK_TRUE(!nf32::isPositiveInfinite(nf32::negativeInfinity()));
		}

		UNITTEST_TEST(IsNegativeInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!nf32::isNegativeInfinite(a));
			CHECK_TRUE(!nf32::isNegativeInfinite(b));
			CHECK_TRUE(nf32::isNegativeInfinite(nf32::negativeInfinity()));
			CHECK_TRUE(!nf32::isNegativeInfinite(nf32::positiveInfinity()));
		}

		UNITTEST_TEST(IsInfinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!nf32::isInfinite(a));
			CHECK_TRUE(!nf32::isInfinite(b));
			CHECK_TRUE(nf32::isInfinite(nf32::negativeInfinity()));
			CHECK_TRUE(nf32::isInfinite(nf32::positiveInfinity()));
		}

		UNITTEST_TEST(IsNAN)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(!nf32::isNAN(a));
			CHECK_TRUE(!nf32::isNAN(b));
			CHECK_TRUE(nf32::isNAN(nf32::nan()));
			CHECK_TRUE(nf32::isNAN(nf32::nan()-1.0f));
			CHECK_TRUE(!nf32::isNAN(nf32::negativeInfinity()));
		}

		UNITTEST_TEST(IsFinite)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(nf32::isFinite(a));
			CHECK_TRUE(nf32::isFinite(b));
			CHECK_TRUE(!nf32::isFinite(nf32::negativeInfinity()));
			CHECK_TRUE(!nf32::isFinite(nf32::nan()));
		}

		UNITTEST_TEST(IsRational)
		{
			f32 a = 0.0f, b= 999.0f;
			CHECK_TRUE(nf32::isRational(a));
			CHECK_TRUE(nf32::isRational(b));
			CHECK_TRUE(!nf32::isRational(nf32::negativeInfinity()));
			CHECK_TRUE(!nf32::isRational(nf32::nan()));
		}
#endif
		UNITTEST_TEST(Bin)
		{
			CHECK_TRUE(nf32::bin(0.0f) == 0);
#ifdef IEEE_FLOATS
			CHECK_TRUE(nf32::bin(nf32::nan()) == nf32::bin(nf32::nan()+1));
#endif
			CHECK_TRUE(nf32::bin(1.0f) == (u32)0x3f800000);
			CHECK_TRUE(nf32::bin(-2.0f) == (u32)0xc0000000);
		}

		UNITTEST_TEST(Sbin)
		{
			CHECK_EQUAL(nf32::sbin(0.0f), 0);
			CHECK_EQUAL(nf32::sbin(-2.0f), (s32)0xc0000000);
#ifdef IEEE_FLOATS
			CHECK_EQUAL(nf32::sbin(nf32::nan()), nf32::sbin(nf32::nan()+1));
#endif
		}

		UNITTEST_TEST(ToF32)
		{
			CHECK_EQUAL(nf32::toF32((u32)0), 0.0f);
			//infinity is 0x7f800000
			CHECK_EQUAL(nf32::toF32(0x7f800000) + 1.0f, nf32::toF32(0x7f800000));
			CHECK_EQUAL(nf32::toF32(0x3f800000), 1.0f);
			CHECK_EQUAL(nf32::toF32(0xc0000000), -2.0f);
		}

		UNITTEST_TEST(ToS32)
		{
			CHECK_EQUAL(nf32::toS32(0.0f), 0);
			CHECK_EQUAL(nf32::toS32(-2.0f), (s32)-2);
			CHECK_EQUAL(nf32::toS32(2000.0f), (s32)2000);
		}

		UNITTEST_TEST(ToU32)
		{
			CHECK_TRUE(nf32::toU32(0.0f) == 0);
			CHECK_TRUE(nf32::toU32(20.0f) == (u32)20);
		}

		UNITTEST_TEST(IsEqual)
		{
			CHECK_TRUE(!nf32::isEqual(0.0001f,0.0f));
			CHECK_TRUE(nf32::isEqual(20.0f,20.000f));
#ifdef IEEE_FLOATS
			CHECK_TRUE(nf32::isEqual(nf32::negativeZero(),-0.0f));
			CHECK_TRUE(nf32::isEqual(nf32::nan(),nf32::nan()));
#endif
		}

		UNITTEST_TEST(IsNotEqual)
		{
			CHECK_TRUE(nf32::isNotEqual(0.0001f,0.0f));
			CHECK_TRUE(!nf32::isNotEqual(20.0f,20.000f));
#ifdef IEEE_FLOATS
			CHECK_TRUE(!nf32::isNotEqual(nf32::negativeZero(),-0.0f));
			CHECK_TRUE(!nf32::isNotEqual(nf32::nan(),nf32::nan()));
#endif
		}

		UNITTEST_TEST(BinaryAnd)
		{
			CHECK_EQUAL(nf32::binaryAnd(0.0f,(u32)0), 0.0f);
			CHECK_EQUAL(nf32::binaryAnd(-2.0f,(u32)0x10000000), nf32::toF32(0xc0000000&0x10000000));
		}

		UNITTEST_TEST(BinaryOr)
		{
			CHECK_EQUAL(nf32::binaryOr(0.0f,(u32)0), 0.0f);
			CHECK_EQUAL(nf32::binaryOr(-2.0f,(u32)10), nf32::toF32(0xc0000000|10));
		}

		UNITTEST_TEST(Fraction)
		{
			CHECK_EQUAL(nf32::fraction(0.0f), 0);
			CHECK_EQUAL(nf32::fraction(-2.0f),0);
			//0.15625f : 0  01111100  01000000000000000000000
			CHECK_EQUAL(nf32::fraction(0.15625f),(s32)(0x200000));
		}

		UNITTEST_TEST(ExponentBinary)
		{
			CHECK_EQUAL(nf32::exponentBinary(0.0f), 0);
			CHECK_EQUAL(nf32::exponentBinary(-2.0f),(s32)128);
			CHECK_EQUAL(nf32::exponentBinary(0.15625f),(s32)(0x7c));
		}

		UNITTEST_TEST(Exponent)
		{
			CHECK_EQUAL(nf32::exponent(1.0f), 0);
			CHECK_EQUAL(nf32::exponent(-2.0f),(s32)1);
			CHECK_EQUAL(nf32::exponent(0.15625f),(s32)(0x7c - 0x7f));
		}

		UNITTEST_TEST(SignMask)
		{
			CHECK_TRUE(nf32::signMask(1.0f) == 0);
			CHECK_TRUE(nf32::signMask(-2.0f) == (u32)0x80000000);
			CHECK_TRUE(nf32::signMask(0.15625f) == 0);
		}

		UNITTEST_TEST(SignBit)
		{
			CHECK_EQUAL(nf32::signBit(1.0f), 0);
			CHECK_EQUAL(nf32::signBit(-2.0f),1);
			CHECK_EQUAL(nf32::signBit(0.15625f),0);
		}

		UNITTEST_TEST(SignBitSigned)
		{
			CHECK_EQUAL(nf32::signBitSigned(1.0f), 0);
			CHECK_EQUAL(nf32::signBitSigned(-2.0f),-1);
			CHECK_EQUAL(nf32::signBitSigned(0.15625f),0);
			CHECK_EQUAL(nf32::signBitSigned(nf32::positiveZero()),0);
			CHECK_EQUAL(nf32::signBitSigned(nf32::negativeZero()),-1);
		}

		UNITTEST_TEST(Sign)
		{
			CHECK_EQUAL(nf32::sign(1.0f), 1);
			CHECK_EQUAL(nf32::sign(-2.0f),-1);
			CHECK_EQUAL(nf32::sign(0.15625f),1);
			CHECK_EQUAL(nf32::sign(nf32::positiveZero()),0);
			CHECK_EQUAL(nf32::sign(nf32::negativeZero()),0);
		}

		UNITTEST_TEST(Sign2)
		{
			CHECK_EQUAL(nf32::sign(1.0f, 0.5f), 1);
			CHECK_EQUAL(nf32::sign(-2.0f, 0.5f),-1);
			CHECK_EQUAL(nf32::sign(0.15625f, 0.5),0);
			CHECK_EQUAL(nf32::sign(nf32::negativeZero(), 0.1f),0);

			// this will break psp release compiling
			// not compile errors, just break the compiler
			// TODO: fix it
			//CHECK_EQUAL(nf32::sign(0.1f, nf32::positiveZero()),1);
		}

		UNITTEST_TEST(IsZero)
		{
			CHECK_FALSE(nf32::isZero(0.0001f));
			CHECK_TRUE(nf32::isZero(nf32::negativeZero()));

			// this will break psp release compiling
			// not compile errors, just break the compiler
			// TODO: fix it
			//CHECK_TRUE(nf32::isZero(nf32::positiveZero()));
		}

		UNITTEST_TEST(IsNearZero)
		{
			CHECK_FALSE(nf32::isNearZero(0.0001f));
			CHECK_TRUE(nf32::isNearZero(0.0001f, -1));
			CHECK_TRUE(nf32::isNearZero(nf32::positiveZero()));
			CHECK_TRUE(nf32::isNearZero(nf32::negativeZero()));
		}

		UNITTEST_TEST(IsNear)
		{
			CHECK_FALSE(nf32::isNear(0.0001f, 0.0f, -80));
			CHECK_TRUE(nf32::isNear(0.0001f, 0.0f, -1));
			CHECK_FALSE(nf32::isNear(1.0001f, 1.0f, -80));
			CHECK_TRUE(nf32::isNear(1.0001f, 1.0f, -1));
			CHECK_TRUE(nf32::isNear(nf32::positiveZero(), 0.0f, -80));
			CHECK_TRUE(nf32::isNear(nf32::negativeZero(), 0.0f, -80));
		}

		UNITTEST_TEST(RoundToInt)
		{
			CHECK_EQUAL(nf32::roundToInt(0.5f), 1);
			CHECK_EQUAL(nf32::roundToInt(0.4f), 0);
			CHECK_EQUAL(nf32::roundToInt(14.0f/3.0f), 5);
		}

		UNITTEST_TEST(Squared)
		{
			CHECK_EQUAL(nf32::squared(0.5f), 0.25f);
			CHECK_EQUAL(nf32::squared(0.4f), 0.4f * 0.4f);
			CHECK_EQUAL(nf32::squared(0.1f), 0.1f * 0.1f);
			CHECK_EQUAL(nf32::squared(14.0f/3.0f), (14.0f / 3.0f) * (14.0f / 3.0f));
		}

		UNITTEST_TEST(OneOver)
		{
			CHECK_EQUAL(nf32::oneOver(0.5f), 1.0f/0.5f);
			CHECK_EQUAL(nf32::oneOver(0.4f), 1.0f/0.4f);
			CHECK_EQUAL(nf32::oneOver(0.1f), 1.0f/0.1f);
			CHECK_EQUAL(nf32::oneOver(14.0f/3.0f), 1.0f/(14.0f / 3.0f));
		}

		UNITTEST_TEST(ScaledFloatToInt)
		{
			CHECK_EQUAL(nf32::scaledFloatToInt(0.0f, 4), 0);
			CHECK_EQUAL(nf32::scaledFloatToInt(1.0f, 3), 7);
			CHECK_EQUAL(nf32::scaledFloatToInt(0.5f, 4), 8);
			CHECK_EQUAL(nf32::scaledFloatToInt(3.0f/16.0f, 4), 3);
		}

		UNITTEST_TEST(MinimumIncrement)
		{
			CHECK_EQUAL(nf32::minimumIncrement(0.0f), nf32::toF32(0x7f800000));
			CHECK_EQUAL(nf32::minimumIncrement(1.0f), nf32::toF32(0x3f800001));
			CHECK_EQUAL(nf32::minimumIncrement(-2.0f), nf32::toF32(0xbffffffe));
		}

		UNITTEST_TEST(IsPositive)
		{
			CHECK_FALSE(nf32::isPositive(0.0f));
			CHECK_TRUE(nf32::isPositive(1.0f));
			CHECK_FALSE(nf32::isPositive(-2.0f));
		}

		UNITTEST_TEST(IsNegative)
		{
			CHECK_FALSE(nf32::isNegative(0.0f));
			CHECK_FALSE(nf32::isNegative(1.0f));
			CHECK_TRUE(nf32::isNegative(-2.0f));
		}

		UNITTEST_TEST(IsLessPositive)
		{
			CHECK_TRUE(nf32::isLessPositive(0.1f, 1.0f));
			CHECK_FALSE(nf32::isLessPositive(1.0f, 0.1f));
			CHECK_TRUE(nf32::isLessPositive(nf32::positiveMinimum(),0.0001f));
		}

		UNITTEST_TEST(IsGreater)
		{
			CHECK_FALSE(nf32::isGreater(0.1f, 1.0f));
			CHECK_TRUE(nf32::isGreater(1.0f, 0.1f));
			CHECK_FALSE(nf32::isGreater(nf32::positiveMinimum(),0.0001f));
		}
	}
}
UNITTEST_SUITE_END
