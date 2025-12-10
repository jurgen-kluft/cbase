#include "cbase/c_guid.h"
#include "cbase/c_runes.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(guid_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(constructor1)
        {
            guid_t i;
            CHECK_EQUAL(true, i.isNull());

            guid_t nill;
            nill.setNull();
            CHECK_EQUAL(true, nill.isNull());

            CHECK_TRUE(nill == i);
        }

        UNITTEST_TEST(constructor2)
        {
            guid_t nill;

            guid_t i1(1, 2, 3, 4);
            CHECK_EQUAL(false, i1.isNull());

            guid_t i2(i1);
            CHECK_EQUAL(false, i2.isNull());

            CHECK_TRUE(i1 == i2);

            CHECK_FALSE(i1 == nill);
            CHECK_TRUE(i1 != nill);

            CHECK_FALSE(i2 == nill);
            CHECK_TRUE(i2 != nill);
        }

        UNITTEST_TEST(constructor3)
        {
            guid_t i(1, 2, 3, 4);
            CHECK_EQUAL(false, i.isNull());

            guid_t nill;
            nill.setNull();

            CHECK_FALSE(i == nill);
            CHECK_TRUE(i != nill);
        }

        UNITTEST_TEST(setNull)
        {
            guid_t i(1, 2, 3, 4);
            CHECK_EQUAL(false, i.isNull());

            i.setNull();
            CHECK_EQUAL(true, i.isNull());
        }

        UNITTEST_TEST(isNull)
        {
            guid_t i(1, 2, 3, 4);
            CHECK_EQUAL(false, i.isNull());
            i.setNull();
            CHECK_EQUAL(true, i.isNull());

            guid_t i2;
            CHECK_EQUAL(true, i2.isNull());
        }

        UNITTEST_TEST(set)
        {
            guid_t i;
            i.set(1, 2, 3, 4);

            u32 a, b, c, d;
            i.get(a, b, c, d);
            CHECK_EQUAL((u32)1, a);
            CHECK_EQUAL((u32)2, b);
            CHECK_EQUAL((u32)3, c);
            CHECK_EQUAL((u32)4, d);
        }

        UNITTEST_TEST(get)
        {
            guid_t i1(1, 2, 3, 4);

            u32 a, b, c, d;
            i1.get(a, b, c, d);
            CHECK_EQUAL((u32)1, a);
            CHECK_EQUAL((u32)2, b);
            CHECK_EQUAL((u32)3, c);
            CHECK_EQUAL((u32)4, d);

            guid_t i2(11, 33, 44, 99);
            i2.get(a, b, c, d);
            CHECK_EQUAL((u32)11, a);
            CHECK_EQUAL((u32)33, b);
            CHECK_EQUAL((u32)44, c);
            CHECK_EQUAL((u32)99, d);
        }

        UNITTEST_TEST(assignment_operator)
        {
            guid_t i1(1, 2, 3, 4);
            guid_t i2;
            CHECK_TRUE(i2.isNull());
            i2 = i1;
            CHECK_TRUE(i1 == i2);
            CHECK_TRUE(i2 == i1);
        }

        UNITTEST_TEST(equal_operator)
        {
            guid_t i1(1, 2, 3, 4);

            guid_t i2;
            i2.set(1, 2, 3, 4);

            CHECK_TRUE(i1 == i2);
            CHECK_TRUE(i2 == i1);

            i1.setNull();
            i2.setNull();
            CHECK_TRUE(i1 == i2);
            CHECK_TRUE(i2 == i1);
        }

        UNITTEST_TEST(not_equal_operator)
        {
            guid_t i(1, 2, 3, 4);
            CHECK_EQUAL(false, i.isNull());

            guid_t nill;
            nill.setNull();

            CHECK_FALSE(i == nill);
            CHECK_TRUE(i != nill);
        }

        UNITTEST_TEST(toString)
        {
            guid_t id(0x11335577, 0x22446688, 0x557799BB, 0x88AACCEE);

            utf32::rune dst_runes[256];
            dst_runes[0]   = 0;
            dst_runes[1]   = 0;
            dst_runes[255] = 0;
            runes_t dst    = utf32::make_runes(dst_runes, 0, 0, 256 - 1);

            crunes_t guidStr = ascii::make_crunes("11335577:22446688:557799BB:88AACCEE");
            id.toString(dst);
            CHECK_EQUAL(0, nrunes::compare(dst, guidStr));
        }

        UNITTEST_TEST(fromString)
        {
            crunes_t guidStr = ascii::make_crunes("11335577:22446688:557799BB:88AACCEE");
            guid_t   id1;
            id1.fromString(guidStr);
            guid_t id2(0x11335577, 0x22446688, 0x557799BB, 0x88AACCEE);
            CHECK_TRUE(id1 == id2);
        }
    }
}
UNITTEST_SUITE_END
