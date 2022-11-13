#include "cbase/c_allocator.h"
#include "cbase/c_singleton.h"
#include "cbase/c_integer.h"
#include "cunittest/cunittest.h"

using namespace ncore;

extern ncore::alloc_t* gTestAllocator;

UNITTEST_SUITE_BEGIN(test_singleton_t)
{

	class test_singleton1 : public singleton_t<test_singleton1>
	{
	public:
					test_singleton1() : mInteger(22) {}
		
		static alloc_t*	singleton_allocator()
		{
			return gTestAllocator;
		}

		static u32				singleton_alignment()
		{
			return 64;
		}
		XCORE_CLASS_NEW_DELETE(singleton_allocator, 64)

		int			mInteger;
	};

	class test_singleton2 : public singleton_t<test_singleton2, static_instantiation_t>
	{
	public:
		test_singleton2() : mInteger(22) {}

		enum { SINGLETON_ALIGNMENT = 128 };
		XCORE_CLASS_PLACEMENT_NEW_DELETE

		int			mInteger;
	};

    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(heap_policy)
        {
			CHECK_NULL(test_singleton1::s_instance());
			test_singleton1::s_create();
			CHECK_NOT_NULL(test_singleton1::s_instance());

			test_singleton1* s = test_singleton1::s_instance();
			CHECK_EQUAL(s->mInteger, 22);

			test_singleton1::s_destroy();
			CHECK_NULL(test_singleton1::s_instance());
        }

		UNITTEST_TEST(static_policy)
		{
			CHECK_NULL(test_singleton2::s_instance());
			test_singleton2::s_create();
			CHECK_NOT_NULL(test_singleton2::s_instance());

			// CHECK alignment on singleton pointer, should be 128

			test_singleton2* s = test_singleton2::s_instance();
			CHECK_EQUAL(s->mInteger, 22);

			test_singleton2::s_destroy();
			CHECK_NULL(test_singleton2::s_instance());
		}
	}
}
UNITTEST_SUITE_END
