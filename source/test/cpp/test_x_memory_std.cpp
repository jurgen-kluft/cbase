#include "xbase/x_va_list.h"
#include "xbase/x_memory.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xmemory_std)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(xmem_utils__memset)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5,88,88 };
			CHECK_EQUAL(6, buffer1[0]);
			CHECK_EQUAL(7, buffer1[1]);
			CHECK_EQUAL(8, buffer1[2]);
			CHECK_EQUAL(9, buffer1[3]);
			
			xmem_utils::memset(buffer1, 1, 4);
			CHECK_EQUAL(1, buffer1[0]);
			CHECK_EQUAL(1, buffer1[1]);
			CHECK_EQUAL(1, buffer1[2]);
			CHECK_EQUAL(1, buffer1[3]);
			CHECK_EQUAL(10, buffer1[4]);

			xmem_utils::memset(&buffer1[5], 33, 4);
			CHECK_EQUAL(33, buffer1[5]);
			CHECK_EQUAL(33, buffer1[6]);
			CHECK_EQUAL(33, buffer1[7]);
			CHECK_EQUAL(33, buffer1[8]);
			CHECK_EQUAL( 5, buffer1[9]);

			CHECK_EQUAL(88, buffer1[10]);
			CHECK_EQUAL(88, buffer1[11]);
		}

		UNITTEST_TEST(xmem_utils__memclr)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5,88,88 };
			CHECK_EQUAL(6, buffer1[0]);
			CHECK_EQUAL(7, buffer1[1]);
			CHECK_EQUAL(8, buffer1[2]);
			CHECK_EQUAL(9, buffer1[3]);

			xmem_utils::memclr(buffer1, 4);
			CHECK_EQUAL( 0, buffer1[0]);
			CHECK_EQUAL( 0, buffer1[1]);
			CHECK_EQUAL( 0, buffer1[2]);
			CHECK_EQUAL( 0, buffer1[3]);
			CHECK_EQUAL(10, buffer1[4]);

			xmem_utils::memclr(&buffer1[5], 4);
			CHECK_EQUAL(0, buffer1[5]);
			CHECK_EQUAL(0, buffer1[6]);
			CHECK_EQUAL(0, buffer1[7]);
			CHECK_EQUAL(0, buffer1[8]);
			CHECK_EQUAL(5, buffer1[9]);

			CHECK_EQUAL(88, buffer1[10]);
			CHECK_EQUAL(88, buffer1[11]);
		}

		UNITTEST_TEST(xmem_utils__memcpy)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5,88,88 };
			char buffer2[] = { 1,2,3,4,5,6,7,8,9,10,99,99 };
			CHECK_EQUAL(6, buffer1[0]);
			CHECK_EQUAL(7, buffer1[1]);
			CHECK_EQUAL(8, buffer1[2]);
			CHECK_EQUAL(9, buffer1[3]);

			xmem_utils::memcpy(buffer1, buffer2, 4);
			CHECK_EQUAL(1, buffer1[0]);
			CHECK_EQUAL(2, buffer1[1]);
			CHECK_EQUAL(3, buffer1[2]);
			CHECK_EQUAL(4, buffer1[3]);
			CHECK_EQUAL(10, buffer1[4]);

			xmem_utils::memcpy(buffer2, buffer1, 10);
			for (xcore::s32 i=1; i<=10; ++i)
				CHECK_EQUAL(buffer1[i-1], buffer2[i-1]);

			CHECK_EQUAL(88, buffer1[10]);
			CHECK_EQUAL(88, buffer1[11]);
			CHECK_EQUAL(99, buffer2[10]);
			CHECK_EQUAL(99, buffer2[11]);
		}

		UNITTEST_TEST(xmem_utils__memcmp)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5,88,88 };
			char buffer2[] = { 1,2,3,4,5,6,7,8,9,10,99,99 };
			CHECK_EQUAL(6, buffer1[0]);
			CHECK_EQUAL(7, buffer1[1]);
			CHECK_EQUAL(8, buffer1[2]);
			CHECK_EQUAL(9, buffer1[3]);

			CHECK_FALSE(xmem_utils::memcmp(buffer1, buffer2, 10)==0);
			CHECK_TRUE(xmem_utils::memcmp(&buffer1[5], buffer2, 5)==0);
		}

		UNITTEST_TEST(xmem_utils__memswap)
		{
			char buffer1[] = { 1,2,3,4,5,6,7,8,9,10,88,88 };
			char buffer2[] = { 0,1,2,3,4,5,6,7,8, 9,99,99 };

			for (xcore::s32 i=1; i<=10; ++i)
				CHECK_EQUAL(i, buffer1[i-1]);
			for (xcore::s32 i=1; i<=10; ++i)
				CHECK_EQUAL(i-1, buffer2[i-1]);

			xmem_utils::memswap(buffer1, buffer2, 10);

			for (xcore::s32 i=1; i<=10; ++i)
				CHECK_EQUAL(i, buffer2[i-1]);
			for (xcore::s32 i=1; i<=10; ++i)
				CHECK_EQUAL(i-1, buffer1[i-1]);

			CHECK_EQUAL(88, buffer1[10]);
			CHECK_EQUAL(88, buffer1[11]);
			CHECK_EQUAL(99, buffer2[10]);
			CHECK_EQUAL(99, buffer2[11]);
		}

		UNITTEST_TEST(xmem_utils__memmove)
		{
			char buffer1[] = { 1,2,3,4,5,6,7,8,9,10,22,22,22,22,22,88,88 };
			for (xcore::s32 i=1; i<=10; ++i)
			{
				CHECK_EQUAL(i, buffer1[i-1]);
			}
			for (xcore::s32 i=11; i<=15; ++i)
			{
				CHECK_EQUAL(22, buffer1[i-1]);
			}

			// Overlapping memory copy
			xmem_utils::memmove(&buffer1[5], buffer1, 10);

			// Testing for 1,2,3,4,5,1,2,3,4,5
			for (xcore::s32 i=1; i<=5; ++i)
			{
				CHECK_EQUAL(i, buffer1[i-1]);
			}
			for (xcore::s32 i=6; i<=10; ++i)
			{
				CHECK_EQUAL(i-5, buffer1[i-1]);
			}
			// Testing for 6,7,8,9,10 (was 22,22,22,22,22)
			for (xcore::s32 i=11; i<=15; ++i)
			{
				CHECK_EQUAL(i-5, buffer1[i-1]);
			}

			CHECK_EQUAL(88, buffer1[15]);
			CHECK_EQUAL(88, buffer1[16]);
		}

		UNITTEST_TEST(toKb)
		{
			CHECK_EQUAL(0, xmem_utils::toKb(500));
			CHECK_EQUAL(1, xmem_utils::toKb(1000));
			CHECK_EQUAL(1, xmem_utils::toKb(1024));
			CHECK_EQUAL(1000, xmem_utils::toKb(1000*1024));
		}

		UNITTEST_TEST(toMb)
		{
			CHECK_EQUAL(0, xmem_utils::toMb(500*1024));
			CHECK_EQUAL(1, xmem_utils::toMb(1000*1024));
			CHECK_EQUAL(1, xmem_utils::toMb(1024*1024));
			CHECK_EQUAL(1000, xmem_utils::toMb(1000*1024*1024));
		}

		UNITTEST_TEST(readunaligned16)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5 };

			xcore::u16 r;
			xcore::u16 const* ptr = xmem_utils::readunaligned16((xcore::u16 const*)&buffer1[1], r);
			CHECK_EQUAL(2, (xcore::xsize_t)ptr - (xcore::xsize_t)&buffer1[1]);
			CHECK_EQUAL(0x0708, r);
		}

		UNITTEST_TEST(readunaligned24)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5 };

			xcore::u32 r1;
			xcore::u32 const* ptr1 = xmem_utils::readunaligned24((xcore::u32 const*)&buffer1[1], r1);
			CHECK_EQUAL(3, (xcore::xsize_t)ptr1 - (xcore::xsize_t)&buffer1[1]);
			CHECK_EQUAL(0x070809, r1);
			xcore::u32 r2;
			xcore::u32 const* ptr2 = xmem_utils::readunaligned24((xcore::u32 const*)&buffer1[2], r2);
			CHECK_EQUAL(3, (xcore::xsize_t)ptr2 - (xcore::xsize_t)&buffer1[2]);
			CHECK_EQUAL(0x08090a, r2);
		}

		UNITTEST_TEST(readunaligned32)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5 };

			xcore::u32 r1;
			xcore::u32 const* ptr1 = xmem_utils::readunaligned32((xcore::u32 const*)&buffer1[1], r1);
			CHECK_EQUAL(4, (xcore::xsize_t)ptr1 - (xcore::xsize_t)&buffer1[1]);
			CHECK_EQUAL(0x0708090a, r1);
			xcore::u32 r2;
			xcore::u32 const* ptr2 = xmem_utils::readunaligned32((xcore::u32 const*)&buffer1[2], r2);
			CHECK_EQUAL(4, (xcore::xsize_t)ptr2 - (xcore::xsize_t)&buffer1[2]);
			CHECK_EQUAL(0x08090a01, r2);
		}

		UNITTEST_TEST(x_swap)
		{
			u32 a=1, b=2;
			CHECK_EQUAL(1, a);
			CHECK_EQUAL(2, b);
			x_swap(a,b);
			CHECK_EQUAL(2, a);
			CHECK_EQUAL(1, b);
		}

		UNITTEST_TEST(x_memcpy)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5 };
			char buffer2[] = { 1,2,3,4,5,6,7,8,9,10 };

			x_memcpy(buffer2, buffer1, 5);

			char buffer3[] = { 6,7,8,9,10,6,7,8,9,10 };

			CHECK_TRUE(x_memcmp(buffer2, buffer3, 10) == 0);
		}
	}
}
UNITTEST_SUITE_END
