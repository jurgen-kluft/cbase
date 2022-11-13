#include "cbase/c_allocator.h"
#include "cbase/c_slice.h"

#include "cunittest/cunittest.h"

using namespace ncore;

extern ncore::alloc_t* gTestAllocator;

UNITTEST_SUITE_BEGIN(test_slice)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(slice_100)
		{
			slice_t s;
			slice_t::alloc(s, gTestAllocator, 100, 4);
			s.release();
		}
	}
}
UNITTEST_SUITE_END
