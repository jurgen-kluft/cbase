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

			hbb_t hbb;
			hbb.init(bitmap, 8192);
			hbb.reset();

			CHECK_EQUAL(false, hbb.is_set(10));
			hbb.set(10);
			CHECK_EQUAL(true, hbb.is_set(10));
		}

		UNITTEST_TEST(set_and_is_set_many)
		{
			clear_bitmap();

			hbb_t hbb;
			hbb.init(bitmap, 8192);
			hbb.reset();

			u32 b1;
			CHECK_FALSE(hbb.find(b1));

			for (s32 b=0; b<1024; b++)
			{
				hbb.set(b);
				CHECK_TRUE(hbb.find(b1));
				CHECK_EQUAL(b, b1);
				hbb.clr(b);
			}
		}

		UNITTEST_TEST(find_free_bit_1)
		{
			clear_bitmap();

			hbb_t hbb;
			hbb.init(bitmap, 8192);
			hbb.reset();

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

			hbb_t hbb;
			hbb.init(bitmap, 8192);
			hbb.reset();

			// Should not be able to find any '1'
			u32 free_bit;
			CHECK_EQUAL(false, hbb.find(free_bit));

			for (s32 b=1024-1; b>=0; --b)
			{
				hbb.set(b);
				hbb.find(free_bit);
				CHECK_EQUAL(b, free_bit);
			}
		}

	}
}
UNITTEST_SUITE_END
