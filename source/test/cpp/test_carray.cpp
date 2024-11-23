#include "cbase/c_carray.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(carray_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        static s32 array_data[64];

        UNITTEST_TEST(construct)
        {
            carray_t<s32> a = g_create_carray<s32>(Allocator, 5);
            CHECK_EQUAL(0, g_size(a));
            CHECK_EQUAL(5, g_reserved(a));
            g_destroy_carray<s32>(Allocator, a);
        }

        UNITTEST_TEST(push_back)
        {
            carray_t<s32> a = g_create_carray<s32>(Allocator, 5);

            CHECK_EQUAL(0, g_size(a));
            CHECK_EQUAL(5, g_reserved(a));

            s32 value = 1;
            g_push_back(a, value);

            CHECK_EQUAL(1, g_size(a));
            CHECK_EQUAL(5, g_reserved(a));
            g_destroy_carray<s32>(Allocator, a);
        }

        UNITTEST_TEST(pop_back)
        {
            carray_t<s32> a = g_create_carray<s32>(Allocator, 5);

            CHECK_EQUAL(0, g_size(a));
            CHECK_EQUAL(5, g_reserved(a));

            s32 value;
            CHECK_FALSE(g_pop_back(a, value));

            CHECK_EQUAL(0, g_size(a));
            CHECK_EQUAL(5, g_reserved(a));
            g_destroy_carray<s32>(Allocator, a);
        }

        UNITTEST_TEST(push_and_pop_back)
        {
            carray_t<s32> a = g_create_carray<s32>(Allocator, 5);

            CHECK_EQUAL(0, g_size(a));
            CHECK_EQUAL(5, g_reserved(a));

            s32 value = 1;
            g_push_back(a, value);
            CHECK_EQUAL(1, g_size(a));
            CHECK_EQUAL(5, g_reserved(a));

            CHECK_TRUE(g_pop_back(a, value));
            CHECK_EQUAL(1, value);

            CHECK_EQUAL(0, g_size(a));
            CHECK_EQUAL(5, g_reserved(a));
            g_destroy_carray<s32>(Allocator, a);
        }

        UNITTEST_TEST(operator_index)
        {
            carray_t<s32> a = g_create_carray<s32>(Allocator, 10);
            for (s32 i = 0; i < g_reserved(a); ++i)
                g_push_back(a, i);

            s32& value = g_get(a, 0);
            value += 10;
            CHECK_EQUAL(10, a.mArray[0]);
            CHECK_EQUAL(6, a.mArray[6]);
            g_destroy_carray<s32>(Allocator, a);
        }

        UNITTEST_TEST(swap)
        {
            carray_t<s32> a = g_create_carray<s32>(Allocator, 10);

            for (s32 i = 0; i < g_reserved(a); ++i)
                g_push_back(a, i);

            g_swap(a, 4, 7);
            CHECK_EQUAL(4, a.mArray[7]);
            CHECK_EQUAL(7, a.mArray[4]);
            g_destroy_carray<s32>(Allocator, a);
        }

        UNITTEST_TEST(remove)
        {
            carray_t<s32> a = g_create_carray<s32>(Allocator, 10);

            for (s32 i = 0; i < g_reserved(a); ++i)
                g_push_back(a, i);

            CHECK_EQUAL(10, g_size(a));
            CHECK_EQUAL(10, g_reserved(a));

            g_remove(a, 4);

            CHECK_EQUAL(9, g_size(a));
            CHECK_EQUAL(10, g_reserved(a));

            CHECK_EQUAL(5, a.mArray[4]);
            CHECK_EQUAL(6, a.mArray[5]);
            CHECK_EQUAL(9, a.mArray[8]);

            g_remove(a, 8);
            CHECK_EQUAL(8, g_size(a));
            CHECK_EQUAL(10, g_reserved(a));
            CHECK_EQUAL(8, a.mArray[7]);
            g_destroy_carray<s32>(Allocator, a);
        }

        UNITTEST_TEST(swap_remove)
        {
            carray_t<s32> a = g_create_carray<s32>(Allocator, 10);

            for (s32 i = 0; i < g_reserved(a); ++i)
                g_push_back(a, i);

            CHECK_EQUAL(10, g_size(a));
            CHECK_EQUAL(10, g_reserved(a));

            g_swap_remove(a, 4);

            CHECK_EQUAL(9, g_size(a));
            CHECK_EQUAL(10, g_reserved(a));

            CHECK_EQUAL(9, a.mArray[4]);
            CHECK_EQUAL(5, a.mArray[5]);
            CHECK_EQUAL(8, a.mArray[8]);

            g_swap_remove(a, 8);
            CHECK_EQUAL(8, g_size(a));
            CHECK_EQUAL(10, g_reserved(a));
            CHECK_EQUAL(7, a.mArray[7]);
            g_destroy_carray<s32>(Allocator, a);
        }

        UNITTEST_TEST(view)
        {
            carray_t<s32> a = g_create_carray<s32>(Allocator, 10);

            for (s32 i = 0; i < g_reserved(a); ++i)
                g_push_back(a, i);

            cview_t<s32> b = g_view(a, 4, 8);
            CHECK_EQUAL(4, g_size(b));
            CHECK_EQUAL(4, b.mArray[b.mFrom + 0]);
            CHECK_EQUAL(4, g_get(b, 0));
            CHECK_EQUAL(5, b.mArray[b.mFrom + 1]);
            CHECK_EQUAL(5, g_get(b, 1));
            CHECK_EQUAL(6, b.mArray[b.mFrom + 2]);
            CHECK_EQUAL(6, g_get(b, 2));
            CHECK_EQUAL(7, b.mArray[b.mFrom + 3]);
            CHECK_EQUAL(7, g_get(b, 3));
            g_destroy_carray<s32>(Allocator, a);
        }
    }
}
UNITTEST_SUITE_END
