#include "xbase/x_hibitset.h"
#include "xbase/x_memory.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(hibitset_t)
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

			hibitset_t bset;
			bset.init(bitmap, 8192);
			bset.reset();

			CHECK_EQUAL(false, bset.is_set(10));
			bset.set(10);
			CHECK_EQUAL(true, bset.is_set(10));
		}

		UNITTEST_TEST(set_and_is_set_many)
		{
			clear_bitmap();

			hibitset_t bset;
			bset.init(bitmap, 8192);
			bset.reset();

			u32 b1;
			CHECK_FALSE(bset.find(b1));

			for (s32 b=0; b<1024; b++)
			{
				bset.set(b);
				CHECK_TRUE(bset.find(b1));
				CHECK_EQUAL(b, b1);
				bset.clr(b);
			}
		}

		UNITTEST_TEST(find_free_bit_1)
		{
			clear_bitmap();

			hibitset_t bset;
			bset.init(bitmap, 8192);
			bset.reset();

			for (s32 b=0; b<1024; b++)
			{
				bset.set(b);
			}

			for (s32 b=0; b<1024; b++)
			{
				u32 free_bit;
				bset.find(free_bit);
				CHECK_EQUAL(b, free_bit);
				bset.clr(b);
			}
		}

		UNITTEST_TEST(find_free_bit_2)
		{
			clear_bitmap();

			hibitset_t bset;
			bset.init(bitmap, 8192);
			bset.reset();

			// Should not be able to find any '1'
			u32 free_bit;
			CHECK_EQUAL(false, bset.find(free_bit));

			for (s32 b=1024-1; b>=0; --b)
			{
				bset.set(b);
				bset.find(free_bit);
				CHECK_EQUAL(b, free_bit);
			}
		}

	}
}
UNITTEST_SUITE_END
