#include "cbase/c_allocator.h"
#include "cbase/c_context.h"
#include "cbase/c_duomap.h"
#include "cbase/c_memory.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(duomap)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(set_used_and_is_used)
        {
            u32 const maxbits = 4096;

            duomap_t           dm;
            duomap_t::config_t cfg = duomap_t::config_t::compute(maxbits);
            dm.init_all_free(cfg, Allocator);

            CHECK_EQUAL(false, dm.is_used(10));
            dm.set_used(10);
            CHECK_EQUAL(true, dm.is_used(10));
            CHECK_EQUAL(false, dm.is_free(10));
            dm.set_free(10);

            for (u32 bit = 0; bit < maxbits; bit+=1)
            {
                CHECK_EQUAL(bit, dm.find_free());
                dm.set_used(bit);
            }

            for (s32 bit = maxbits - 1; bit >= 0; bit-=1)
            {
                dm.set_free(bit);
                CHECK_EQUAL(bit, dm.find_free());
            }

            dm.release(Allocator);
        }

        UNITTEST_TEST(set_and_find_free_and_used)
        {
            u32 const maxbits = 300;

            duomap_t           dm;
            duomap_t::config_t cfg = duomap_t::config_t::compute(maxbits);
            dm.init_all_used(cfg, Allocator);

            for (s32 bit = maxbits - 1; bit >= 0; bit-=1)
            {
                dm.set_free(bit);
                CHECK_EQUAL(bit, dm.find_free());
            }

            for (s32 bit = maxbits - 1; bit > 0; bit-=1)
            {
                dm.set_used(bit);
                CHECK_EQUAL(bit, dm.find_used());
                CHECK_EQUAL(bit-1, dm.find_free_upper());
            }

            dm.release(Allocator);
        }
    }
}
UNITTEST_SUITE_END
