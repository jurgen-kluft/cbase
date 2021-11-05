#include "xbase/x_integer.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xinteger)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(MACRO_x_Bin8)
		{
			CHECK_EQUAL(0, xbin8(00000000));
			CHECK_EQUAL(8, xbin8(00001000));
			CHECK_EQUAL(9, xbin8(00001001));
			CHECK_EQUAL(60, xbin8(00111100));
			CHECK_EQUAL(192, xbin8(11000000));
		}

		UNITTEST_TEST(MACRO_x_Bin16)
		{
			CHECK_EQUAL(0, xbin16(00000000, 00000000));
			CHECK_EQUAL(264, xbin16(00000001, 00001000));
			CHECK_EQUAL(265, xbin16(00000001, 00001001));
			CHECK_EQUAL(316, xbin16(00000001, 00111100));
			CHECK_EQUAL(448, xbin16(00000001, 11000000));
		}

		UNITTEST_TEST(Min)
		{
			u8 au8 = 1, bu8 = 2, cu8 = 0;
			CHECK_EQUAL(1, xmin(au8,bu8));
			CHECK_EQUAL(0, xmin(au8,cu8));

			u16 au16 = 1, bu16 = 2, cu16 = 0;
			CHECK_EQUAL(1, xmin(au16,bu16));
			CHECK_EQUAL(0, xmin(au16,cu16));

			u32 au32 = 1, bu32 = 2, cu32 = 0;
			CHECK_EQUAL(1, xmin(au32,bu32));
			CHECK_EQUAL(0, xmin(au32,cu32));

			s32 as32 = 1, bs32 = 2, cs32 = 0;
			CHECK_EQUAL(1, xmin(as32,bs32));
			CHECK_EQUAL(0, xmin(as32,cs32));

			u64 au64 = 1, bu64 = 2, cu64 = 0;
			CHECK_EQUAL(1, xmin(au64,bu64));
			CHECK_EQUAL(0, xmin(au64,cu64));
		}

		UNITTEST_TEST(Max)
		{
			u8 au8 = 1, bu8 = 2, cu8 = 0;
			CHECK_EQUAL(2, xmax(au8,bu8));
			CHECK_EQUAL(1, xmax(au8,cu8));

			u16 au16 = 1, bu16 = 2, cu16 = 0;
			CHECK_EQUAL(2, xmax(au16,bu16));
			CHECK_EQUAL(1, xmax(au16,cu16));

			u32 au32 = 1, bu32 = 2, cu32 = 0;
			CHECK_EQUAL(2, xmax(au32,bu32));
			CHECK_EQUAL(1, xmax(au32,cu32));

			u32 as32 = 1, bs32 = 2, cs32 = 0;
			CHECK_EQUAL(2, xmax(as32,bs32));
			CHECK_EQUAL(1, xmax(as32,cs32));

			u64 au64 = 1, bu64 = 2, cu64 = 0;
			CHECK_EQUAL(2, xmax(au64,bu64));
			CHECK_EQUAL(1, xmax(au64,cu64));
		}

		UNITTEST_TEST(Clamp)
		{
			u8 au8 = 0, bu8 = 1, cu8 = 2;
			CHECK_EQUAL(1, xclamp(au8,bu8,cu8));
			CHECK_EQUAL(1, xclamp(bu8,bu8,cu8));

			u16 au16 = 0, bu16 = 1, cu16 = 2;
			CHECK_EQUAL(1, xclamp(au16,bu16,cu16));
			CHECK_EQUAL(2, xclamp(cu16,bu16,cu16));

			u32 au32 = 0, bu32 = 1, cu32 = 2;
			CHECK_EQUAL(1, xclamp(au32,bu32,cu32));
			CHECK_EQUAL(2, xclamp(au32,cu32,cu32));

			u64 au64 = 0, bu64 = 1, cu64 = 2;
			CHECK_EQUAL(1, xclamp(au64,bu64,cu64));
			CHECK_EQUAL(1, xclamp(bu64,au64,cu64));
		}

		UNITTEST_TEST(Align)
		{
			CHECK_EQUAL(16, xalign(17, 16));
			CHECK_EQUAL(256, xalign(300, 256));
			CHECK_EQUAL(1024, xalign(2011, 1024));
		}
		
		UNITTEST_TEST(AlignDown)
		{
			CHECK_EQUAL(16, xalignDown(17, 16));
			CHECK_EQUAL(256, xalignDown(300, 256));
			CHECK_EQUAL(1024, xalignDown(2011, 1024));
		}

		UNITTEST_TEST(AlignUp)
		{
			CHECK_EQUAL(16, xalignUp(12, 16));
			CHECK_EQUAL(32, xalignUp(17, 16));
			CHECK_EQUAL(256, xalignUp(200, 256));
			CHECK_EQUAL(512, xalignUp(300, 256));
			CHECK_EQUAL(2048, xalignUp(2011, 1024));
		}

		UNITTEST_TEST(IsAligned)
		{
			CHECK_TRUE(xisAligned(16, 16));
			CHECK_TRUE(xisAligned(256, 256));
			CHECK_TRUE(xisAligned(1024, 1024));
		}

		UNITTEST_TEST(Abs)
		{
			s32 a = 0, b = 1, c = -2;
			CHECK_EQUAL(0, abs(a));
			CHECK_EQUAL(1, abs(b));
			CHECK_EQUAL(2, abs(c));
		}

		UNITTEST_TEST(Sqr)
		{
			s32 a = 0, b = 1, c = -2;
			CHECK_EQUAL(0, xsqr(a));
			CHECK_EQUAL(1, xsqr(b));
			CHECK_EQUAL(4, xsqr(c));

			u32 au = 0, bu = 1, cu = 2;
			CHECK_EQUAL(0, xsqr(au));
			CHECK_EQUAL(1, xsqr(bu));
			CHECK_EQUAL(4, xsqr(cu));
		}

		UNITTEST_TEST(Mod)
		{
			s32 a = 3, b = 50, c = -2;
			CHECK_EQUAL(3, xmod(a,b));
			CHECK_EQUAL(0, xmod(b,c));
			CHECK_EQUAL(2, xmod(b,a));

			// WII: truncates towards zero

			CHECK_EQUAL(-2, xmod(c,a));
			c = -22;
			a = 7;
			CHECK_EQUAL(-1, xmod(c,a));
		}

		UNITTEST_TEST(Gcd)
		{
			s32 a = 12, b = 50, c = -2;
			CHECK_EQUAL(2, xgcd(a,b));
			CHECK_EQUAL(-2, xgcd(b,c));
			CHECK_EQUAL(-2, xgcd(c,a));
		}

		UNITTEST_TEST(Average)
		{
			s32 a = 13, b = 50, c = -200;
			CHECK_TRUE(xaverage(a,b) >= 31);
			CHECK_TRUE(xaverage(a,b) <= 32);
			CHECK_TRUE(xaverage(b,c) == -75);
			CHECK_TRUE(xaverage(c,a) <= -93);
			CHECK_TRUE(xaverage(c,a) >= -94);
		}

		UNITTEST_TEST(IsPowerOf2)
		{
			s32 a = 13, b = 64;
			CHECK_FALSE(xispo2(a));
			CHECK_TRUE(xispo2(b));
		}

		UNITTEST_TEST(CeilPower2)
		{
			u32 a = 13, b = 64;
			CHECK_EQUAL(16, xceilpo2(a));
			CHECK_EQUAL(64, xceilpo2(b));
		}

		UNITTEST_TEST(FloorPower2)
		{
			u32 a = 13, b = 64;
			CHECK_EQUAL(8, xfloorpo2(a));
			CHECK_EQUAL(64, xfloorpo2(b));
		}

		UNITTEST_TEST(BitReverse)
		{
			u32 a = 13, b = 64;
			CHECK_EQUAL(X_CONSTANT_U32(2952790016), xbitReverse(a));
			CHECK_EQUAL(33554432, xbitReverse(b));
		}

		UNITTEST_TEST(CountBits)
		{
			u32 a = 13, b = 64;
			CHECK_EQUAL(3, xcountBits(a));
			CHECK_EQUAL(1, xcountBits(b));
		}

		UNITTEST_TEST(Log2)
		{
			s32 a = 13, b = 64, c = 1;
			CHECK_EQUAL(3, xilog2(a));
			CHECK_EQUAL(6, xilog2(b));
			CHECK_EQUAL(0, xilog2(c));
		}

		UNITTEST_TEST(CountTrailingZeros16)
		{
			u16 a = 13, b = 64, c = 0, d = -1;
			CHECK_EQUAL(0, xcountTrailingZeros(a));
			CHECK_EQUAL(6, xcountTrailingZeros(b));
			CHECK_EQUAL(0, xcountTrailingZeros(c));
			CHECK_EQUAL(0, xcountTrailingZeros(d));
		}
		UNITTEST_TEST(CountTrailingZeros32)
		{
			u32 a = 13, b = 64, c = 0, d = -1;
			CHECK_EQUAL(0, xcountTrailingZeros(a));
			CHECK_EQUAL(6, xcountTrailingZeros(b));
			CHECK_EQUAL(0, xcountTrailingZeros(c));
			CHECK_EQUAL(0, xcountTrailingZeros(d));
		}
		UNITTEST_TEST(CountTrailingZeros64)
		{
			u64 a = 13, b = 64, c = 0, d = -1, e = 0xffff000000000000UL;
			CHECK_EQUAL( 0, xcountTrailingZeros(a));
			CHECK_EQUAL( 6, xcountTrailingZeros(b));
			CHECK_EQUAL( 0, xcountTrailingZeros(c));
			CHECK_EQUAL( 0, xcountTrailingZeros(d));
			CHECK_EQUAL(48, xcountTrailingZeros(e));
		}

		UNITTEST_TEST(CountLeadingZeros16)
		{
			u16 a = 13, b = 64, c = 0, d = -1;
			CHECK_EQUAL(12, xcountLeadingZeros(a));
			CHECK_EQUAL(9, xcountLeadingZeros(b));
			CHECK_EQUAL(16, xcountLeadingZeros(c));
			CHECK_EQUAL( 0, xcountLeadingZeros(d));
		}
		UNITTEST_TEST(CountLeadingZeros32)
		{
			u32 a = 13, b = 64, c = 0, d = -1;
			CHECK_EQUAL(28, xcountLeadingZeros(a));
			CHECK_EQUAL(25, xcountLeadingZeros(b));
			CHECK_EQUAL(32, xcountLeadingZeros(c));
			CHECK_EQUAL( 0, xcountLeadingZeros(d));
		}
		UNITTEST_TEST(CountLeadingZeros64)
		{
			u64 a = 13, b = 64, c = 0, d = -1, e = 0x0000ffff00000000UL;
			CHECK_EQUAL(32+28, xcountLeadingZeros(a));
			CHECK_EQUAL(32+25, xcountLeadingZeros(b));
			CHECK_EQUAL(32+32, xcountLeadingZeros(c));
			CHECK_EQUAL(0, xcountLeadingZeros(d));
			CHECK_EQUAL(16, xcountLeadingZeros(e));
		}

		UNITTEST_TEST(LeastSignificantOneBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(4, xleastSignificantOneBit(a));
			CHECK_EQUAL(64, xleastSignificantOneBit(b));
			CHECK_EQUAL(0, xleastSignificantOneBit(c));
		}

		UNITTEST_TEST(MostSignificantOneBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(8, xmostSignificantOneBit(a));
			CHECK_EQUAL(64, xmostSignificantOneBit(b));
			CHECK_EQUAL(0, xmostSignificantOneBit(c));
		}

		UNITTEST_TEST(LeastSignificantBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(2, xleastSignificantBit(a));
			CHECK_EQUAL(6, xleastSignificantBit(b));
			CHECK_EQUAL(0, xleastSignificantBit(c));
		}

		UNITTEST_TEST(MostSignificantBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(3, xmostSignificantBit(a));
			CHECK_EQUAL(6, xmostSignificantBit(b));
			CHECK_EQUAL(-1, xmostSignificantBit(c));
		}

		UNITTEST_TEST(FindFirstBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(2, xfindFirstBit(a));
			CHECK_EQUAL(6, xfindFirstBit(b));
			CHECK_EQUAL(-1, xfindFirstBit(c));
		}

		UNITTEST_TEST(FindLastBit)
		{
			u32 a = 12, b = 64, c = 0;
			CHECK_EQUAL(3 , xfindLastBit(a));
			CHECK_EQUAL(6 , xfindLastBit(b));
			CHECK_EQUAL(-1, xfindLastBit(c));
		}

		UNITTEST_TEST(Rol32)
		{
			u32 a = 12, b = 64, c = 0 , m = 5;
			CHECK_EQUAL(24, xrol32(a,1));
			CHECK_EQUAL(64, xrol32(b,0));
			CHECK_EQUAL(0, xrol32(c,m));
			CHECK_EQUAL(12, xrol32(a,b));
			CHECK_EQUAL(0x000000FF, xrol32(0xF000000F,4));
		}

		UNITTEST_TEST(Ror32)
		{
			u32 a = 12, b = 64, c = 0 , m = 5;
			CHECK_EQUAL(6, xror32(a,1));
			CHECK_EQUAL(64, xror32(b,0));
			CHECK_EQUAL(0, xror32(c,m));
			CHECK_EQUAL(12, xror32(a,b));
			CHECK_EQUAL(0xFF000000, xror32(0xF000000F,4));
		}
	}
}
UNITTEST_SUITE_END
