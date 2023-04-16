#include "cbase/c_allocator.h"
#include "cbase/c_slice.h"
#include "cbase/test_allocator.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_slice)
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
	}
}
UNITTEST_SUITE_END
