#include "cbase/c_allocator.h"
#include "cbase/c_context.h"
#include "cbase/c_hbb.h"
#include "cbase/c_memory.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_hbb_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(some_sizes)
        {
            CHECK_EQUAL(128 + 32, binmap_t::sizeof_data(1024));
            CHECK_EQUAL(1024 + 32 + 32, binmap_t::sizeof_data(8192));

            binmap_t hbb;
            hbb.init_all_used(256, Allocator);
            CHECK_TRUE(hbb.m_l0 == 0xFFFFFFFF);
            CHECK_TRUE(hbb.m_l[0][7] == 0xFFFFFFFF);
            hbb.release(Allocator);

            hbb.init_all_free(248, Allocator);
            CHECK_EQUAL(1, hbb.levels());
            CHECK_TRUE(hbb.m_l0 == 0xFFFFFF00);
            CHECK_TRUE(hbb.m_l[0][7] == 0xFF000000);
            hbb.release(Allocator);
        }

        UNITTEST_TEST(set_and_is_set)
        {
            u32 const maxbits = 8192;

            binmap_t hbb;
            hbb.init_all_free(maxbits, Allocator);

            CHECK_EQUAL(false, hbb.is_used(10));
            hbb.set_used(10);
            CHECK_EQUAL(true, hbb.is_used(10));

            hbb.release(Allocator);
        }

        UNITTEST_TEST(set_and_is_set_many)
        {
            u32 const maxbits = 8192;
            CHECK_EQUAL(1024 + 32 + 32, binmap_t::sizeof_data(maxbits));

            binmap_t hbb;
            hbb.init_all_free(maxbits, Allocator);

            u32 b1 = 0;
            CHECK_FALSE(hbb.is_used(b1));

            for (s32 b = 0; b < 1024; b++)
            {
                hbb.set_used(b);
                CHECK_TRUE(hbb.is_used(b));
                hbb.set_free(b);
            }

            for (s32 b = 0; b < 1024; b++)
            {
                hbb.set_used(b);
                for (s32 b1 = 0; b1 <= b; b1++)
                {
                    CHECK_TRUE(hbb.is_used(b1));
                }
            }

            for (s32 b = 0; b < 1024; b++)
            {
                CHECK_TRUE(hbb.is_used(b));
                hbb.set_free(b);
                for (s32 b0 = 0; b0 <= b; b0++)
                {
                    CHECK_TRUE(hbb.is_free(b0));
                }
            }

            hbb.release(Allocator);
        }

        UNITTEST_TEST(find_free_bit_1)
        {
            u32 const maxbits = 8192;
            CHECK_EQUAL(1024 + 32 + 32, binmap_t::sizeof_data(maxbits));

            binmap_t hbb;
            hbb.init_all_used(maxbits, Allocator);

            for (s32 b = 0; b < 1024; b++)
            {
                hbb.set_free(b);
                for (s32 b1 = 0; b1 <= b; b1++)
                {
                    CHECK_TRUE(hbb.is_free(b1));
                }
            }

            for (s32 b = 0; b < 1024; b++)
            {
                s32 free_bit = hbb.find();
                CHECK_EQUAL(b, free_bit);
                hbb.set_used(b);
            }

            hbb.release(Allocator);
        }

        UNITTEST_TEST(find_free_bit_2)
        {
            u32 const maxbits = 8192;
            CHECK_EQUAL(1024 + 32 + 32, binmap_t::sizeof_data(maxbits));

            binmap_t hbb;
            hbb.init_all_used(maxbits, Allocator);

            // Should not be able to hbb.find any '0'
            s32 free_bit = hbb.find();
            CHECK_EQUAL(false, free_bit >= 0);

            for (s32 b = 1024 - 1; b >= 0; --b)
            {
                hbb.set_free(b);
                free_bit = hbb.find();
                CHECK_EQUAL(b, free_bit);
            }

            hbb.release(Allocator);
        }

        UNITTEST_TEST(find_upper)
        {
            u32 const maxbits = 8192;
            CHECK_EQUAL(1024 + 32 + 32, binmap_t::sizeof_data(maxbits));

            binmap_t hbb;
            hbb.init_all_used(maxbits, Allocator);

            // Should not be able to hbb.find any '0'
            s32 free_bit = hbb.find_upper();
            CHECK_EQUAL(false, free_bit >= 0);

            hbb.set_free(maxbits / 2);
            free_bit = hbb.find_upper();
            CHECK_EQUAL((maxbits / 2), free_bit);
            hbb.set_used(maxbits / 2);

            for (s32 b = 0; b < maxbits; --b)
            {
                hbb.set_free(b);
                free_bit = hbb.find();
                CHECK_EQUAL(b, free_bit);
            }

            hbb.release(Allocator);
        }

        UNITTEST_TEST(iterator)
        {
            u32 const maxbits = 8192;
            CHECK_EQUAL(1024 + 32 + 32, binmap_t::sizeof_data(maxbits));

            binmap_t hbb;
            hbb.init_all_used(maxbits, Allocator);

            u32 const numbits = 100;
            for (s32 b = 0; b < numbits; b += 5)
            {
                hbb.set_free(b);
                s32 free_bit = hbb.find();
                CHECK_EQUAL(b, free_bit);
                hbb.set_used(b);
            }

            for (s32 b = 0; b < numbits; b += 5)
            {
                hbb.set_free(b);
            }

            s32              i = 0;
            binmap_t::iter_t iter(&hbb, 0, numbits);
            iter.begin();
            while (!iter.end() && i < numbits)
            {
                CHECK_TRUE(iter.get() == i);
                i += 5;
                iter.next();
            }

            hbb.release(Allocator);
        }

        UNITTEST_TEST(set_get)
        {
            u32       l0len, l1len, l2len, l3len;
            u32 const len = 32 * 32 * 32 * 32;
            binmap_t::compute_levels(len, l0len, l1len, l2len, l3len);

            u32* l1 = (u32*)Allocator->allocate(sizeof(u32) * ((l1len + 31) >> 5));
            u32* l2 = (u32*)Allocator->allocate(sizeof(u32) * ((l2len + 31) >> 5));
            u32* l3 = (u32*)Allocator->allocate(sizeof(u32) * ((l3len + 31) >> 5));
            nmem::memclr(l1, sizeof(u32) * ((l1len + 31) >> 5));
            nmem::memclr(l2, sizeof(u32) * ((l2len + 31) >> 5));
            nmem::memclr(l3, sizeof(u32) * ((l3len + 31) >> 5));

            for (s32 i = 0; i < 64; ++i)
            {
                u32 const count = i * 1024 + ((1 - (i & 1)) * 600);
                binmap_t::compute_levels(count, l0len, l1len, l2len, l3len);
                binmap_t bm;
                bm.init_all_free(count, l0len, l1, l1len, l2, l2len, l3, l3len);

                for (u32 b = 0; b < count; b += 2)
                {
                    bm.set_used(b);
                }
                for (u32 b = 0; b < count; b++)
                {
                    bool const s = bm.get(b);
                    CHECK_EQUAL(((b & 1) == 0), s);
                }
                for (u32 b = 1; b < count; b += 2)
                {
                    u32 const f = bm.find_and_set();
                    CHECK_EQUAL(b, f);
                }

                // There should not be any more free places in the binmap
                s32 const f = bm.find();
                CHECK_EQUAL(-1, f);
            }

            Allocator->deallocate(l1);
            Allocator->deallocate(l2);
            Allocator->deallocate(l3);
        }

        UNITTEST_TEST(lazy_init)
        {
            u32       l0len, l1len, l2len, l3len;
            u32 const len = 32 * 32 * 32 * 32;
            binmap_t::compute_levels(len, l0len, l1len, l2len, l3len);

            u32* l1 = (u32*)Allocator->allocate(sizeof(u32) * ((l1len + 31) >> 5));
            u32* l2 = (u32*)Allocator->allocate(sizeof(u32) * ((l2len + 31) >> 5));
            u32* l3 = (u32*)Allocator->allocate(sizeof(u32) * ((l3len + 31) >> 5));
            nmem::memclr(l1, sizeof(u32) * ((l1len + 31) >> 5));
            nmem::memclr(l2, sizeof(u32) * ((l2len + 31) >> 5));
            nmem::memclr(l3, sizeof(u32) * ((l3len + 31) >> 5));

            for (u32 i = 0; i < 64; ++i)
            {
                u32 const count = i * 1024 + ((1 - (i & 1)) * 600);
                binmap_t::compute_levels(count, l0len, l1len, l2len, l3len);

                binmap_t bm;
                bm.init_all_free_lazy(count, l0len, l1, l1len, l2, l2len, l3, l3len);

                for (u32 b = 0; b < count; ++b)
                {
                    bm.tick_all_free_lazy(b);
                }

                for (u32 b = 0; b < count; b += 2)
                {
                    CHECK_FALSE(bm.get(b));
                    bm.set_used(b);
                    CHECK_TRUE(bm.get(b));
                }
                for (u32 b = 0; b < count; b++)
                {
                    bool const s = bm.get(b);
                    CHECK_EQUAL(((b & 1) == 0), s);
                }

                for (u32 b = 1; b < count; b += 2)
                {
                    u32 const f = bm.find_and_set();
                    CHECK_EQUAL(b, f);
                }

                // There should not be any more free places in the binmap
                s32 const f = bm.find();
                CHECK_EQUAL(-1, f);
            }

            Allocator->deallocate(l1);
            Allocator->deallocate(l2);
            Allocator->deallocate(l3);
        }
    }
}
UNITTEST_SUITE_END
