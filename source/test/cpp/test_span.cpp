#include "cbase/c_carray.h"
#include "cbase/c_span.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_span)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        static uint_t TestSpan(span_t<const s32> s) { return s.m_size; }
        static uint_t TestSpan2(span_t<s32> s) { return s.m_size; }


        UNITTEST_TEST(test_c_array)
        {
            s32         integers[] = {0, 1, 2, 3};
            span_t<s32> s(integers);
            CHECK_EQUAL(4, TestSpan2(s));
        }

        UNITTEST_TEST(test_c_array2)
        {
            s32 const   integers[] = {0, 1, 2, 3};
            CHECK_EQUAL(4, TestSpan(as_span(integers)));
        }

        UNITTEST_TEST(test_c_array3)
        {
            s32         integers[] = {0, 1, 2, 3};
            CHECK_EQUAL(4, TestSpan(as_span<const s32>(integers)));
        }

        UNITTEST_TEST(test_initializer_list)
        {
            CHECK_EQUAL(4, TestSpan(as_span({1, 2, 3, 4})));
        }
    }
}
UNITTEST_SUITE_END
