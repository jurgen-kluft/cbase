#include "xbase/x_darray.h"
#include "xunittest/xunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(darray_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(create_destroy)
		{
			array_t<s32>* darray = array_t<s32>::create(128, 512);
			array_t<s32>::destroy(darray);
			CHECK_NULL(darray);
		}
	
		UNITTEST_TEST(create_use_destroy)
		{
			array_t<s32>* darray = array_t<s32>::create(0, 512);

			CHECK_EQUAL(512, darray->cap_cur());

			for (s32 i = 0; i < 512; i++)
				darray->add_item(i);
			CHECK_EQUAL(512, darray->size());

			array_t<s32>::destroy(darray);
			CHECK_NULL(darray);
		}

		UNITTEST_TEST(create_setcap_use_destroy)
		{
			array_t<s32>* darray = array_t<s32>::create(0, 512);
			CHECK_EQUAL(512, darray->cap_cur());
			
			darray->set_capacity(1024);
			CHECK_EQUAL(1024, darray->cap_cur());
			CHECK_EQUAL(0, darray->size());

			for (s32 i = 0; i < 1024; i++)
				darray->add_item(i);
			CHECK_EQUAL(1024, darray->size());

			array_t<s32>::destroy(darray);
			CHECK_NULL(darray);
		}
	}
}
UNITTEST_SUITE_END
