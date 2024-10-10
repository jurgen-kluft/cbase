#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"
#include "cbase/c_buffer.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_allocator)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(test_alignment)
        {
            void* ptr = Allocator->allocate(200, 8);
            CHECK_EQUAL(0, (ptr_t)ptr & (8 - 1));
            Allocator->deallocate(ptr);

            ptr = Allocator->allocate(200, 16);
            CHECK_EQUAL(0, (ptr_t)ptr & (16 - 1));
            Allocator->deallocate(ptr);

            ptr = Allocator->allocate(200, 32);
            CHECK_EQUAL(0, (ptr_t)ptr & (32 - 1));
            Allocator->deallocate(ptr);

            ptr = Allocator->allocate(200, 64);
            CHECK_EQUAL(0, (ptr_t)ptr & (64 - 1));
            Allocator->deallocate(ptr);
        }

        struct test_object1
        {
            test_object1()
                : mInteger(1)
                , mFloat(2.0)
            {
            }
            s32 mInteger;
            f32 mFloat;
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        UNITTEST_TEST(_DCORE_CLASS_PLACEMENT_NEW_DELETE)
        {
            void*         object1_tmp = Allocator->allocate(sizeof(test_object1), 4);
            test_object1* object1     = new (object1_tmp) test_object1;
            CHECK_NOT_NULL(object1);
            CHECK_TRUE(1 == object1->mInteger);
            CHECK_TRUE(2.0 == object1->mFloat);
            delete object1;
            CHECK_TRUE(1 == object1->mInteger);
            CHECK_TRUE(2.0 == object1->mFloat);
            Allocator->deallocate(object1_tmp);
        }

        struct test_object2
        {
            test_object2()
                : mInteger(3)
                , mFloat(4.0)
            {
            }
            s32             mInteger;
            f32             mFloat;
            static alloc_t* get_allocator() { return Allocator; }
            DCORE_CLASS_NEW_DELETE(get_allocator, 16)
        };

        UNITTEST_TEST(_DCORE_CLASS_NEW_DELETE)
        {
            test_object2* object2 = new test_object2;
            CHECK_NOT_NULL(object2);
            CHECK_TRUE(3 == object2->mInteger);
            CHECK_TRUE(4.0 == object2->mFloat);
            CHECK_TRUE((ptr_t)object2 % 4 == 0);
            delete object2;
        }

        struct test_object3
        {
            test_object3()
                : mInteger(2)
                , mFloat(3.0)
            {
            }
            s32             mInteger;
            f32             mFloat;
            static alloc_t* get_allocator() { return Allocator; }
            DCORE_CLASS_ARRAY_NEW_DELETE(get_allocator, 32)
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        UNITTEST_TEST(test_placement_new)
        {
            test_object3* object3 = new test_object3[3];
            CHECK_NOT_NULL(object3);
            CHECK_TRUE(object3[0].mInteger = 2);
            CHECK_TRUE(object3[0].mFloat = 3.0);
            CHECK_TRUE(object3[1].mInteger = 2);
            CHECK_TRUE(object3[1].mFloat = 3.0);
            CHECK_TRUE(object3[2].mInteger = 2);
            CHECK_TRUE(object3[2].mFloat = 3.0);
            CHECK_EQUAL(0, (ptr_t)object3 % 32);
            delete[] object3;
        }

        struct test_object4
        {
            test_object4()
                : mInteger(2)
                , mFloat(3.0)
            {
            }
            s32             mInteger;
            f32             mFloat;
            static alloc_t* get_allocator() { return Allocator; }

            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        UNITTEST_TEST(test_construct)
        {
            test_object4* obj = Allocator->construct<test_object4>();
            Allocator->destruct<test_object4>(obj);
        }

        UNITTEST_TEST(test_alloc_buffer)
        {
            u8             inplace[256];
            alloc_buffer_t sa;
            sa.init(inplace, sizeof(inplace));
            test_object4* obj = sa.construct<test_object4>();
            sa.destruct<>(obj);
        }

        struct my_type
        {
            s32 value;

            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        UNITTEST_TEST(malloc_typed_and_free_typed)
        {
            my_type* p = ncore::g_new<my_type>();
            CHECK_TRUE(p != nullptr);
            ncore::g_delete(p);
        }
    }
}
UNITTEST_SUITE_END
