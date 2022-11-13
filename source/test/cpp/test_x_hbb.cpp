#include "cbase/c_context.h"
#include "cbase/c_hbb.h"
#include "cbase/c_memory.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_hbb_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        static u32 bitmap[8192];
        static void clear_bitmap()
        {
            for (s32 i=0; i<8192; ++i)
                bitmap[i] = 0;
        }

        UNITTEST_TEST(some_sizes)
        {
            CHECK_TRUE(g_sizeof_hbb(1024) == 35);
            CHECK_TRUE(g_sizeof_hbb(8192) == (256 + 8 + 1 + 2));

            clear_bitmap();

            hbb_t hbb = { bitmap };
            g_hbb_init(hbb, 256, 1);
            CHECK_TRUE(hbb[2] == 0x000000FF);

            g_hbb_init(hbb, 248, 1);
            CHECK_TRUE(hbb[2] == 0x000000FF);
            CHECK_TRUE(hbb[2+1+7] == 0x00FFFFFF);
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

        UNITTEST_TEST(iterator)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            hbb_t hbb = { bitmap };
            CHECK_TRUE(g_sizeof_hbb(maxbits) < 4096);
            g_hbb_init(hbb, maxbits, 0);
            g_hbb_reset(hbb, 0);

            u32 const numbits = 2500;
            for (s32 b=0; b < numbits; b += 5)
            {
                g_hbb_set(hbb, b);
                u32 free_bit;
                g_hbb_find(hbb, free_bit);
                CHECK_EQUAL(b, free_bit);
                g_hbb_clr(hbb, b);
            }

            for (s32 b=0; b < numbits; b += 5)
            {
                g_hbb_set(hbb, b);
            }

            s32 i = 0;
            hbb_iter_t iter = g_hbb_iterator(hbb, 0, maxbits);
            while (!iter.end())
            {
                CHECK_TRUE(iter.m_cur == i);
                i += 5;
                iter.next();
            }
        }

        UNITTEST_TEST(resize)
        {
            clear_bitmap();

            u32 const maxbits = 1024;            
            CHECK_TRUE(g_sizeof_hbb(maxbits) == 35);

            hbb_t hbb = nullptr;
            alloc_t* alloc = context_t::system_alloc();
            g_hbb_init(hbb, maxbits, 0, alloc);

            u32 const numbits = 1024;
            for (s32 b=0; b < numbits; b++)
            {
                g_hbb_set(hbb, b);
            }

            g_hbb_resize(hbb, 8192, 0, alloc);

            for (s32 b=0; b < numbits; b++)
            {
                CHECK_TRUE(g_hbb_is_set(hbb, b));
            }

            g_hbb_release(hbb, alloc);
        }
    }
}
UNITTEST_SUITE_END
