#include "xbase\x_types.h"
#include "xbase\x_integer.h"
#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xinteger)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(MACRO_x_Bin8)
		{
			CHECK_EQUAL(0, x_Bin8(00000000));
			CHECK_EQUAL(8, x_Bin8(00001000));
			CHECK_EQUAL(9, x_Bin8(00001001));
			CHECK_EQUAL(60, x_Bin8(00111100));
			CHECK_EQUAL(192, x_Bin8(11000000));
		}

		UNITTEST_TEST(MACRO_x_Bin16)
		{
			CHECK_EQUAL(0, x_Bin16(00000000, 00000000));
			CHECK_EQUAL(264, x_Bin16(00000001, 00001000));
			CHECK_EQUAL(265, x_Bin16(00000001, 00001001));
			CHECK_EQUAL(316, x_Bin16(00000001, 00111100));
			CHECK_EQUAL(448, x_Bin16(00000001, 11000000));
		}

		UNITTEST_TEST(Min)
		{
			u8 au8 = 1, bu8 = 2, cu8 = 0;
			CHECK_EQUAL(1, x_intu::min(au8,bu8));
			CHECK_EQUAL(0, x_intu::min(au8,cu8));

			u16 au16 = 1, bu16 = 2, cu16 = 0;
			CHECK_EQUAL(1, x_intu::min(au16,bu16));
			CHECK_EQUAL(0, x_intu::min(au16,cu16));

			u32 au32 = 1, bu32 = 2, cu32 = 0;
			CHECK_EQUAL(1, x_intu::min(au32,bu32));
			CHECK_EQUAL(0, x_intu::min(au32,cu32));

			s32 as32 = 1, bs32 = 2, cs32 = 0;
			CHECK_EQUAL(1, x_intu::min(as32,bs32));
			CHECK_EQUAL(0, x_intu::min(as32,cs32));

			u64 au64 = 1, bu64 = 2, cu64 = 0;
			CHECK_EQUAL(1, x_intu::min(au64,bu64));
			CHECK_EQUAL(0, x_intu::min(au64,cu64));
		}

		UNITTEST_TEST(Max)
		{
			u8 au8 = 1, bu8 = 2, cu8 = 0;
			CHECK_EQUAL(2, x_intu::max(au8,bu8));
			CHECK_EQUAL(1, x_intu::max(au8,cu8));

			u16 au16 = 1, bu16 = 2, cu16 = 0;
			CHECK_EQUAL(2, x_intu::max(au16,bu16));
			CHECK_EQUAL(1, x_intu::max(au16,cu16));

			u32 au32 = 1, bu32 = 2, cu32 = 0;
			CHECK_EQUAL(2, x_intu::max(au32,bu32));
			CHECK_EQUAL(1, x_intu::max(au32,cu32));

			u32 as32 = 1, bs32 = 2, cs32 = 0;
			CHECK_EQUAL(2, x_intu::max(as32,bs32));
			CHECK_EQUAL(1, x_intu::max(as32,cs32));

			u64 au64 = 1, bu64 = 2, cu64 = 0;
			CHECK_EQUAL(2, x_intu::max(au64,bu64));
			CHECK_EQUAL(1, x_intu::max(au64,cu64));
		}

		UNITTEST_TEST(Clamp)
		{
			u8 au8 = 0, bu8 = 1, cu8 = 2;
			CHECK_EQUAL(1, x_intu::clamp(au8,bu8,cu8));
			CHECK_EQUAL(1, x_intu::clamp(bu8,bu8,cu8));

			u16 au16 = 0, bu16 = 1, cu16 = 2;
			CHECK_EQUAL(1, x_intu::clamp(au16,bu16,cu16));
			CHECK_EQUAL(2, x_intu::clamp(cu16,bu16,cu16));

			u32 au32 = 0, bu32 = 1, cu32 = 2;
			CHECK_EQUAL(1, x_intu::clamp(au32,bu32,cu32));
			CHECK_EQUAL(2, x_intu::clamp(au32,cu32,cu32));

			u64 au64 = 0, bu64 = 1, cu64 = 2;
			CHECK_EQUAL(1, x_intu::clamp(au64,bu64,cu64));
			CHECK_EQUAL(1, x_intu::clamp(bu64,au64,cu64));
		}

		UNITTEST_TEST(Align)
		{
			CHECK_EQUAL(16, x_intu::align(17, 16));
			CHECK_EQUAL(256, x_intu::align(300, 256));
			CHECK_EQUAL(1024, x_intu::align(2011, 1024));
		}
		
		UNITTEST_TEST(AlignDown)
		{
			CHECK_EQUAL(16, x_intu::alignDown(17, 16));
			CHECK_EQUAL(256, x_intu::alignDown(300, 256));
			CHECK_EQUAL(1024, x_intu::alignDown(2011, 1024));
		}

		UNITTEST_TEST(AlignUp)
		{
			CHECK_EQUAL(16, x_intu::alignUp(12, 16));
			CHECK_EQUAL(32, x_intu::alignUp(17, 16));
			CHECK_EQUAL(256, x_intu::alignUp(200, 256));
			CHECK_EQUAL(512, x_intu::alignUp(300, 256));
			CHECK_EQUAL(2048, x_intu::alignUp(2011, 1024));
		}

		UNITTEST_TEST(IsAligned)
		{
			CHECK_TRUE(x_intu::isAligned(16, 16));
			CHECK_TRUE(x_intu::isAligned(256, 256));
			CHECK_TRUE(x_intu::isAligned(1024, 1024));
		}

		UNITTEST_TEST(Abs)
		{
			s32 a = 0, b = 1, c = -2;
			CHECK_EQUAL(0, x_intu::abs(a));
			CHECK_EQUAL(1, x_intu::abs(b));
			CHECK_EQUAL(2, x_intu::abs(c));
		}

		UNITTEST_TEST(Sqr)
		{
			s32 a = 0, b = 1, c = -2;
			CHECK_EQUAL(0, x_intu::sqr(a));
			CHECK_EQUAL(1, x_intu::sqr(b));
			CHECK_EQUAL(4, x_intu::sqr(c));

			u32 au = 0, bu = 1, cu = 2;
			CHECK_EQUAL(0, x_intu::sqr(au));
			CHECK_EQUAL(1, x_intu::sqr(bu));
			CHECK_EQUAL(4, x_intu::sqr(cu));
		}

		UNITTEST_TEST(Mod)
		{
			s32 a = 3, b = 50, c = -2;
			CHECK_EQUAL(3, x_intu::mod(a,b));
			CHECK_EQUAL(0, x_intu::mod(b,c));
			CHECK_EQUAL(2, x_intu::mod(b,a));

			// WII: truncates towards zero

			CHECK_EQUAL(-2, x_intu::mod(c,a));
			c = -22;
			a = 7;
			CHECK_EQUAL(-1, x_intu::mod(c,a));
		}

		UNITTEST_TEST(Gcd)
		{
			s32 a = 12, b = 50, c = -2;
			CHECK_EQUAL(2, x_intu::gcd(a,b));
			CHECK_EQUAL(-2, x_intu::gcd(b,c));
			CHECK_EQUAL(-2, x_intu::gcd(c,a));
		}

		UNITTEST_TEST(Average)
		{
			s32 a = 13, b = 50, c = -200;
			CHECK_TRUE(x_intu::average(a,b) >= 31);
			CHECK_TRUE(x_intu::average(a,b) <= 32);
			CHECK_TRUE(x_intu::average(b,c) == -75);
			CHECK_TRUE(x_intu::average(c,a) <= -93);
			CHECK_TRUE(x_intu::average(c,a) >= -94);
		}

		UNITTEST_TEST(IsPowerOf2)
		{
			s32 a = 13, b = 64;
			CHECK_FALSE(x_intu::isPowerOf2(a));
			CHECK_TRUE(x_intu::isPowerOf2(b));
		}

		UNITTEST_TEST(CeilPower2)
		{
			u32 a = 13, b = 64;
			CHECK_EQUAL(16, x_intu::ceilPower2(a));
			CHECK_EQUAL(64, x_intu::ceilPower2(b));
		}

		UNITTEST_TEST(FloorPower2)
		{
			u32 a = 13, b = 64;
			CHECK_EQUAL(8, x_intu::floorPower2(a));
			CHECK_EQUAL(64, x_intu::floorPower2(b));
		}

		UNITTEST_TEST(BitReverse)
		{
			u32 a = 13, b = 64;
			CHECK_EQUAL(X_CONSTANT_U32(2952790016), x_intu::bitReverse(a));
			CHECK_EQUAL(33554432, x_intu::bitReverse(b));
		}

		UNITTEST_TEST(CountBits)
		{
			u32 a = 13, b = 64;
			CHECK_EQUAL(3, x_intu::countBits(a));
			CHECK_EQUAL(1, x_intu::countBits(b));
		}

		UNITTEST_TEST(Log2)
		{
			s32 a = 13, b = 64, c = 1;
			CHECK_EQUAL(3, x_intu::ilog2(a));
			CHECK_EQUAL(6, x_intu::ilog2(b));
			CHECK_EQUAL(0, x_intu::ilog2(c));
		}

		UNITTEST_TEST(CountTrailingZeros)
		{
			s32 a = 13, b = 64, c = 0, d = -1;
			CHECK_EQUAL(0, x_intu::countTrailingZeros(a));
			CHECK_EQUAL(6, x_intu::countTrailingZeros(b));
			CHECK_EQUAL(32, x_intu::countTrailingZeros(c));
			CHECK_EQUAL(0, x_intu::countTrailingZeros(d));
		}

		UNITTEST_TEST(CountLeadingZeros)
		{
			s32 a = 13, b = 64, c = 0, d = -1;
			CHECK_EQUAL(28, x_intu::countLeadingZeros(a));
			CHECK_EQUAL(25, x_intu::countLeadingZeros(b));
			CHECK_EQUAL(32, x_intu::countLeadingZeros(c));
			CHECK_EQUAL(0, x_intu::countLeadingZeros(d));
		}

		UNITTEST_TEST(LeastSignificantOneBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(4, x_intu::leastSignificantOneBit(a));
			CHECK_EQUAL(64, x_intu::leastSignificantOneBit(b));
			CHECK_EQUAL(0, x_intu::leastSignificantOneBit(c));
		}

		UNITTEST_TEST(MostSignificantOneBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(8, x_intu::mostSignificantOneBit(a));
			CHECK_EQUAL(64, x_intu::mostSignificantOneBit(b));
			CHECK_EQUAL(0, x_intu::mostSignificantOneBit(c));
		}

		UNITTEST_TEST(LeastSignificantBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(2, x_intu::leastSignificantBit(a));
			CHECK_EQUAL(6, x_intu::leastSignificantBit(b));
			CHECK_EQUAL(-1, x_intu::leastSignificantBit(c));
		}

		UNITTEST_TEST(MostSignificantBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(3, x_intu::mostSignificantBit(a));
			CHECK_EQUAL(6, x_intu::mostSignificantBit(b));
			CHECK_EQUAL(-1, x_intu::mostSignificantBit(c));
		}

		UNITTEST_TEST(FindFirstBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(2, x_intu::findFirstBit(a));
			CHECK_EQUAL(6, x_intu::findFirstBit(b));
			CHECK_EQUAL(-1, x_intu::findFirstBit(c));
		}

		UNITTEST_TEST(FindLastBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(3 ,x_intu::findLastBit(a));
			CHECK_EQUAL(6 ,x_intu::findLastBit(b));
			CHECK_EQUAL(-1,x_intu::findLastBit(c));
		}

		UNITTEST_TEST(Rol32)
		{
			u32 a = 12, b = 64, c = 0 , m = 5;
			CHECK_EQUAL(24, x_intu::rol32(a,1));
			CHECK_EQUAL(64, x_intu::rol32(b,0));
			CHECK_EQUAL(0, x_intu::rol32(c,m));
			CHECK_EQUAL(12, x_intu::rol32(a,b));
			CHECK_EQUAL(0x000000FF, x_intu::rol32(0xF000000F,4));
		}

		UNITTEST_TEST(Ror32)
		{
			u32 a = 12, b = 64, c = 0 , m = 5;
			CHECK_EQUAL(6, x_intu::ror32(a,1));
			CHECK_EQUAL(64, x_intu::ror32(b,0));
			CHECK_EQUAL(0, x_intu::ror32(c,m));
			CHECK_EQUAL(12, x_intu::ror32(a,b));
			CHECK_EQUAL(0xFF000000, x_intu::ror32(0xF000000F,4));
		}
	}
}
UNITTEST_SUITE_END
