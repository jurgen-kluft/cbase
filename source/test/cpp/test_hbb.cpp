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

        static u32 bitmap[512];
        static void clear_bitmap()
        {
            for (s32 i=0; i<512; ++i)
                bitmap[i] = 0;
        }

        UNITTEST_TEST(some_sizes)
        {
            CHECK_TRUE(g_hbb_sizeof_data(1024) == 32 + 1);
            CHECK_TRUE(g_hbb_sizeof_data(8192) == (256 + 8 + 1));

            clear_bitmap();

            hbb_t hbb;
            hbb.init(256, 1, bitmap);
            CHECK_TRUE(hbb.m_hbb[0] == 0x000000FF);

            hbb.init(248, 1, bitmap);
            CHECK_TRUE(hbb.m_hbb[0] == 0x000000FF);
            CHECK_TRUE(hbb.m_hbb[1+7] == 0x00FFFFFF);
        }

        UNITTEST_TEST(set_and_is_set)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            CHECK_TRUE(g_hbb_sizeof_data(maxbits) < 4096);

            hbb_t hbb;
            hbb.init(maxbits, 0, bitmap);
            hbb.reset(0);

            CHECK_EQUAL(false, hbb.is_set(10));
            hbb.set(10);
            CHECK_EQUAL(true, hbb.is_set(10));
        }

        UNITTEST_TEST(set_and_is_set_many)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            CHECK_TRUE(g_hbb_sizeof_data(maxbits) < 4096);

            hbb_t hbb; 
            hbb.init(maxbits, 0, bitmap);
            hbb.reset(0);

            u32 b1;
            CHECK_FALSE(hbb.find(b1));

            for (s32 b=0; b<1024; b++)
            {
                hbb.set(b);
                CHECK_TRUE(hbb.find(b1));
                CHECK_EQUAL(b, b1);
                hbb.clr(b);
                CHECK_FALSE(hbb.find(b1));
            }
        }

        UNITTEST_TEST(find_free_bit_1)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            hbb_t hbb;
            CHECK_TRUE(g_hbb_sizeof_data(maxbits) < 4096);
            hbb.init(maxbits, 0, bitmap);
            hbb.reset(0);

            for (s32 b=0; b<1024; b++)
            {
                hbb.set(b);
            }

            for (s32 b=0; b<1024; b++)
            {
                u32 free_bit;
                hbb.find(free_bit);
                CHECK_EQUAL(b, free_bit);
                hbb.clr(b);
            }
        }

        UNITTEST_TEST(find_free_bit_2)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            hbb_t hbb;
            CHECK_TRUE(g_hbb_sizeof_data(maxbits) < 4096);
            hbb.init(maxbits, 0, bitmap);
            hbb.reset(0);

            // Should not be able to hbb.find any '1'
            u32 free_bit;
            CHECK_EQUAL(false, hbb.find(free_bit));

            for (s32 b=1024-1; b>=0; --b)
            {
                hbb.set(b);
                hbb.find(free_bit);
                CHECK_EQUAL(b, free_bit);
            }
        }

        UNITTEST_TEST(iterator)
        {
            clear_bitmap();

            u32 const maxbits = 8192;
            hbb_t hbb;
            CHECK_TRUE(g_hbb_sizeof_data(maxbits) < 4096);
            hbb.init(maxbits, 0, bitmap);
            hbb.reset(0);

            u32 const numbits = 2500;
            for (s32 b=0; b < numbits; b += 5)
            {
                hbb.set(b);
                u32 free_bit;
                hbb.find(free_bit);
                CHECK_EQUAL(b, free_bit);
                hbb.clr(b);
            }

            for (s32 b=0; b < numbits; b += 5)
            {
                hbb.set(b);
            }

            s32 i = 0;
            hbb_iter_t iter = hbb.iterator(0, maxbits);
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
            CHECK_TRUE(g_hbb_sizeof_data(maxbits) == 32 + 1);

            hbb_t hbb;
            alloc_t* alloc = context_t::system_alloc();
            hbb.init(maxbits, 0, alloc);

            u32 const numbits = 1024;
            for (s32 b=0; b < numbits; b++)
            {
                hbb.set(b);
            }

            hbb.resize(8192, 0, alloc);

            for (s32 b=0; b < numbits; b++)
            {
                CHECK_TRUE(hbb.is_set(b));
            }

            hbb.release(alloc);
        }
    }
}
UNITTEST_SUITE_END
