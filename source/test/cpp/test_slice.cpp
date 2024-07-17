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

			CHECK_EQUAL(100, s.size());
			CHECK_EQUAL(0, s.from());
			CHECK_EQUAL(100, s.to());
		}

		UNITTEST_TEST(duplicate)
		{			
			slice_t s;
			slice_t::allocate(s, Allocator, 100, 4);

			slice_t d = slice_t::duplicate(s);

			CHECK_EQUAL(100, d.size());
			CHECK_EQUAL(0, d.from());
			CHECK_EQUAL(100, d.to());
		}

		UNITTEST_TEST(split)
		{			
			slice_t s;
			slice_t::allocate(s, Allocator, 100, 4);

			slice_t left, right;
			bool ok = slice_t::split(s, 50, left, right);

			CHECK_TRUE(ok);
			
			CHECK_EQUAL(50, left.size());
			CHECK_EQUAL(0, left.from());
			CHECK_EQUAL(50, left.to());

			CHECK_EQUAL(50, right.size());
			CHECK_EQUAL(50, right.from());
			CHECK_EQUAL(100, right.to());
		}

		UNITTEST_TEST(split_join)
		{			
			slice_t s;
			slice_t::allocate(s, Allocator, 100, 4);

			slice_t left, right;
			slice_t::split(s, 50, left, right);

			slice_t j = slice_t::join(left, right);

			CHECK_EQUAL(100, j.size());
			CHECK_EQUAL(0, j.from());
			CHECK_EQUAL(100, j.to());
		}

		UNITTEST_TEST(join)
		{			
			slice_t s1;
			slice_t::allocate(s1, Allocator, 50, 4);
			slice_t s2;
			slice_t::allocate(s2, Allocator, 50, 4);

			slice_t j = slice_t::join(s1, s2);

			CHECK_EQUAL(100, j.size());
			CHECK_EQUAL(0, j.from());
			CHECK_EQUAL(100, j.to());
		}

		UNITTEST_TEST(slice)
		{			
			slice_t s;
			slice_t::allocate(s, Allocator, 100, 4);

			slice_t d = s.slice(50);

			CHECK_EQUAL(50, d.size());
			CHECK_EQUAL(0, d.from());
			CHECK_EQUAL(50, d.to());
		}

	}
}
UNITTEST_SUITE_END
