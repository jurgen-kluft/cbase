#include "xbase/x_hbb.h"
#include "xbase/x_memory.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(hbb_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        static u32 bitmap[4096];
        static void clear_bitmap()
        {
            for (s32 i=0; i<4096; ++i)
                bitmap[i] = 0;
        }

        UNITTEST_TEST(set_and_is_set)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            hbb_t hbb = { bitmap };
            CHECK_TRUE(g_sizeof_hbb(maxbits) < 4096);
            g_hbb_init(hbb, maxbits, 0);
            g_hbb_reset(hbb, 0);

            CHECK_EQUAL(false, g_hbb_is_set(hbb, 10));
            g_hbb_set(hbb, 10);
            CHECK_EQUAL(true, g_hbb_is_set(hbb, 10));
        }

        UNITTEST_TEST(set_and_is_set_many)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            hbb_t hbb = { bitmap };
            CHECK_TRUE(g_sizeof_hbb(maxbits) < 4096);
            g_hbb_init(hbb, maxbits, 0);
            g_hbb_reset(hbb, 0);

            u32 b1;
            CHECK_FALSE(g_hbb_find(hbb, b1));

            for (s32 b=0; b<1024; b++)
            {
                g_hbb_set(hbb, b);
                CHECK_TRUE(g_hbb_find(hbb, b1));
                CHECK_EQUAL(b, b1);
                g_hbb_clr(hbb, b);
                CHECK_FALSE(g_hbb_find(hbb, b1));
            }
        }

        UNITTEST_TEST(find_free_bit_1)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            hbb_t hbb = { bitmap };
            CHECK_TRUE(g_sizeof_hbb(maxbits) < 4096);
            g_hbb_init(hbb, maxbits, 0);
            g_hbb_reset(hbb, 0);

            for (s32 b=0; b<1024; b++)
            {
                g_hbb_set(hbb, b);
            }

            for (s32 b=0; b<1024; b++)
            {
                u32 free_bit;
                g_hbb_find(hbb, free_bit);
                CHECK_EQUAL(b, free_bit);
                g_hbb_clr(hbb, b);
            }
        }

        UNITTEST_TEST(find_free_bit_2)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            hbb_t hbb = { bitmap };
            CHECK_TRUE(g_sizeof_hbb(maxbits) < 4096);
            g_hbb_init(hbb, maxbits, 0);
            g_hbb_reset(hbb, 0);

            // Should not be able to g_hbb_find any '1'
            u32 free_bit;
            CHECK_EQUAL(false, g_hbb_find(hbb, free_bit));

            for (s32 b=1024-1; b>=0; --b)
            {
                g_hbb_set(hbb, b);
                g_hbb_find(hbb, free_bit);
                CHECK_EQUAL(b, free_bit);
            }
        }

    }
}
UNITTEST_SUITE_END
