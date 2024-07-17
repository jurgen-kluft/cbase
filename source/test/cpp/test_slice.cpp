#include "cbase/c_allocator.h"
#include "cbase/c_slice.h"
#include "cbase/test_allocator.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(slice)
{
	UNITTEST_FIXTURE(main)
	{
        UNITTEST_ALLOCATOR;

		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(slice_100)
		{
			slice_t s;
			slice_t::allocate(s, Allocator, 100, 4);
			s.release();
		}

		UNITTEST_TEST(duplicate)
		{
			return;
			
			slice_t s;
			slice_t::allocate(s, Allocator, 100, 4);

			slice_t d = slice_t::duplicate(s);
			s.release();

			CHECK_EQUAL(100, d.size());
			CHECK_EQUAL(0, d.from());
			CHECK_EQUAL(100, d.to());
		}
	}
}
UNITTEST_SUITE_END
