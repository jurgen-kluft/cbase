#include "cbase/c_vector.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(vector_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        //UNITTEST_ALLOCATOR;

		UNITTEST_TEST(create_destroy)
		{
			vector_t<s32> darray(128, 512);
		}

		UNITTEST_TEST(create_use_destroy)
		{
			vector_t<s32> darray( 512, 512);
			CHECK_EQUAL(512, darray.capacity());
			for (s32 i = 0; i < 512; i++)
				darray.add_item(i);
			CHECK_EQUAL(512, darray.size());
		}

		UNITTEST_TEST(create_setcap_use_destroy)
		{
			vector_t<s32> darray( 512, 512);
			CHECK_EQUAL(512, darray.capacity());

			darray.set_capacity(1024);
			CHECK_EQUAL(1024, darray.capacity());
			CHECK_EQUAL(0, darray.size());

			for (s32 i = 0; i < 1024; i++)
				darray.add_item(i);
			CHECK_EQUAL(1024, darray.size());
		}
	}
}
UNITTEST_SUITE_END
