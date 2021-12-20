#include "xbase/x_darray.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(darray_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(create_destroy)
		{
			s32* darray = create_darray<s32>(128, 512, 4096);
			destroy_darray(darray);
			CHECK_NULL(darray);
		}
	
		UNITTEST_TEST(create_use_destroy)
		{
			s32* darray = create_darray<s32>(0, 512, 4096);

			CHECK_EQUAL(512, get_cap(darray));

			for (s32 i = 0; i < 512; i++)
				add_item(darray, i);
			CHECK_EQUAL(512, get_size(darray));

			destroy_darray(darray);
			CHECK_NULL(darray);
		}

		UNITTEST_TEST(create_setcap_use_destroy)
		{
			s32* darray = create_darray<s32>(0, 512, 4096);
			CHECK_EQUAL(512, get_cap(darray));
			
			set_cap(darray, 1024);
			CHECK_EQUAL(1024, get_cap(darray));
			CHECK_EQUAL(0, get_size(darray));

			for (s32 i = 0; i < 1024; i++)
				add_item(darray, i);
			CHECK_EQUAL(1024, get_size(darray));

			destroy_darray(darray);
			CHECK_NULL(darray);
		}
	}
}
UNITTEST_SUITE_END
