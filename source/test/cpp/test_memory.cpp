#include "ccore/c_target.h"
#include "cbase/c_memory.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(memory)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(readunaligned16)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5 };

			ncore::u16 r;
			ncore::u16 const* ptr = nmem::readunaligned16((ncore::u16 const*)&buffer1[1], r);
			CHECK_EQUAL(2, (ncore::uint_t)ptr - (ncore::uint_t)&buffer1[1]);
			CHECK_EQUAL(0x0708, r);
		}

		UNITTEST_TEST(readunaligned24)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5 };

			ncore::u32 r1;
			ncore::u32 const* ptr1 = nmem::readunaligned24((ncore::u32 const*)&buffer1[1], r1);
			CHECK_EQUAL(3, (ncore::uint_t)ptr1 - (ncore::uint_t)&buffer1[1]);
			CHECK_EQUAL(0x070809, r1);
			ncore::u32 r2;
			ncore::u32 const* ptr2 = nmem::readunaligned24((ncore::u32 const*)&buffer1[2], r2);
			CHECK_EQUAL(3, (ncore::uint_t)ptr2 - (ncore::uint_t)&buffer1[2]);
			CHECK_EQUAL(0x08090a, r2);
		}

		UNITTEST_TEST(readunaligned32)
		{
			char buffer1[] = { 6,7,8,9,10,1,2,3,4,5 };

			ncore::u32 r1;
			ncore::u32 const* ptr1 = nmem::readunaligned32((ncore::u32 const*)&buffer1[1], r1);
			CHECK_EQUAL(4, (ncore::uint_t)ptr1 - (ncore::uint_t)&buffer1[1]);
			CHECK_EQUAL(0x0708090a, r1);
			ncore::u32 r2;
			ncore::u32 const* ptr2 = nmem::readunaligned32((ncore::u32 const*)&buffer1[2], r2);
			CHECK_EQUAL(4, (ncore::uint_t)ptr2 - (ncore::uint_t)&buffer1[2]);
			CHECK_EQUAL(0x08090a01, r2);
		}
	}
}
UNITTEST_SUITE_END
