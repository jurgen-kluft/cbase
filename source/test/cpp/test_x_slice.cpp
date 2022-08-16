#include "xbase/x_allocator.h"
#include "xbase/x_slice.h"

#include "xunittest/xunittest.h"

using namespace ncore;

extern ncore::alloc_t* gTestAllocator;

UNITTEST_SUITE_BEGIN(xslice)
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
