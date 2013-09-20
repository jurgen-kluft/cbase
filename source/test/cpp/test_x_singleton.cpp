#include "xbase\x_allocator.h"
#include "xbase\x_singleton.h"
#include "xbase\x_integer.h"
#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;

UNITTEST_SUITE_BEGIN(xsingleton)
{

	class test_singleton1 : public xsingleton<test_singleton1>
	{
	public:
					test_singleton1() : mInteger(22) {}
		
		static x_iallocator*	singleton_allocator()
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

	class test_singleton2 : public xsingleton<test_singleton2, xstatic_instantiation>
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
