#include "xbase/x_allocator.h"
#include "xbase/x_debug.h"

#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::xalloc* gTestAllocator;

UNITTEST_SUITE_BEGIN(xallocator)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(test_alignment)
        {
			void * ptr = gTestAllocator->allocate(200, 8);
			CHECK_EQUAL(0, (uptr)ptr & (8-1));
			gTestAllocator->deallocate(ptr);

			ptr = gTestAllocator->allocate(200, 16);
			CHECK_EQUAL(0, (uptr)ptr & (16-1));
			gTestAllocator->deallocate(ptr);
	
			ptr = gTestAllocator->allocate(200, 32);
			CHECK_EQUAL(0, (uptr)ptr & (32-1));
			gTestAllocator->deallocate(ptr);

			ptr = gTestAllocator->allocate(200, 64);
			CHECK_EQUAL(0, (uptr)ptr & (64-1));
			gTestAllocator->deallocate(ptr);
		}

		struct test_object1
		{
			test_object1():mInteger(1),mFloat(2.0){}
			s32	mInteger;
			f32	mFloat;
			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

        UNITTEST_TEST(_XCORE_CLASS_PLACEMENT_NEW_DELETE)
        {
			void* object1_tmp = gTestAllocator->allocate(sizeof(test_object1),4);
			test_object1* object1 = new(object1_tmp) test_object1;
			CHECK_NOT_NULL(object1);
			CHECK_TRUE(1 == object1->mInteger);
			CHECK_TRUE(2.0 == object1->mFloat);
			delete object1;
			CHECK_TRUE(1 == object1->mInteger);
			CHECK_TRUE(2.0 == object1->mFloat);
			gTestAllocator->deallocate(object1_tmp);
        }

		struct test_object2
		{
			test_object2():mInteger(3),mFloat(4.0){}
			s32	mInteger;
			f32	mFloat;
			static xalloc* get_allocator() { return gTestAllocator; }
			XCORE_CLASS_NEW_DELETE(get_allocator, 16)
		};

		UNITTEST_TEST(_XCORE_CLASS_NEW_DELETE)
		{
			test_object2* object2 = new test_object2;
			CHECK_NOT_NULL(object2);
			CHECK_TRUE(3 == object2->mInteger);
			CHECK_TRUE(4.0 == object2->mFloat);
			CHECK_TRUE((uptr)object2%4 == 0);
			delete object2;
		}

		struct test_object3
		{
			test_object3():mInteger(2),mFloat(3.0){}
			s32	mInteger;
			f32	mFloat;
			static xalloc* get_allocator() { return gTestAllocator; }
			XCORE_CLASS_ARRAY_NEW_DELETE(get_allocator, 32)
		};

		UNITTEST_TEST(_XCORE_CLASS_ARRAY_NEW_DELETE)
		{
			test_object3* object3 = new test_object3[3];
			CHECK_NOT_NULL(object3);
			CHECK_TRUE(object3[0].mInteger = 2);
			CHECK_TRUE(object3[0].mFloat = 3.0);
			CHECK_TRUE(object3[1].mInteger = 2);
			CHECK_TRUE(object3[1].mFloat = 3.0);
			CHECK_TRUE(object3[2].mInteger = 2);
			CHECK_TRUE(object3[2].mFloat = 3.0);
			CHECK_EQUAL(0, (uptr)object3%32);
			delete[] object3;
		}



	}
}
UNITTEST_SUITE_END
