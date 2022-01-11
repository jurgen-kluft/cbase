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

		static inline u32 s_level_bits(u32 config, s8 level)
		{
			config = config >> 5;
			s32 numbits = 32 - (config & 0x1F);
			while (level > 0)
			{
				config = config >> 5;
				numbits = (numbits << 5) - (config & 0x1F);
				level -= 1;
			}
			return numbits;
		}

		UNITTEST_TEST(large_maxbits)
		{
			for (s32 x = 1; x < 32; ++x)
			{
				for (s32 i = 0; i < 32; ++i)
				{
					u32 config;
					u32 maxbits = 1024 * 1024 * x - i;
					u32 number_of_dwords = sizeof_hbb(maxbits, config);
					u32 numbits = s_level_bits(config, (config & 0x1F) - 1);
					CHECK_EQUAL(maxbits, numbits);
				}
			}
		}

		UNITTEST_TEST(set_and_is_set)
		{
			clear_bitmap();

			u32 const maxbits = 8192;
			hbb_t hbb = { bitmap };
			u32 config;
			CHECK_TRUE(sizeof_hbb(maxbits, config) < 4096);
			init(hbb, maxbits, config, 0);
			reset(hbb, config, maxbits, 0);

			CHECK_EQUAL(false, is_set(hbb, config, maxbits, 10));
			set(hbb, config, maxbits, 10);
			CHECK_EQUAL(true, is_set(hbb, config, maxbits, 10));
		}

		UNITTEST_TEST(set_and_is_set_many)
		{
			clear_bitmap();

			u32 const maxbits = 8192;
			hbb_t hbb = { bitmap };
			u32 config;
			CHECK_TRUE(sizeof_hbb(maxbits, config) < 4096);
			init(hbb, maxbits, config, 0);
			reset(hbb, config, maxbits, 0);

			u32 b1;
			CHECK_FALSE(find(hbb, config, maxbits, b1));

			for (s32 b=0; b<1024; b++)
			{
				set(hbb, config, maxbits, b);
				CHECK_TRUE(find(hbb, config, maxbits, b1));
				CHECK_EQUAL(b, b1);
				clr(hbb, config, maxbits, b);
				CHECK_FALSE(find(hbb, config, maxbits, b1));
			}
		}

		UNITTEST_TEST(find_free_bit_1)
		{
			clear_bitmap();

			u32 const maxbits = 8192;
			hbb_t hbb = { bitmap };
			u32 config;
			CHECK_TRUE(sizeof_hbb(maxbits, config) < 4096);
			init(hbb, maxbits, config, 0);
			reset(hbb, config, maxbits, 0);

			for (s32 b=0; b<1024; b++)
			{
				set(hbb, config, maxbits, b);
			}

			for (s32 b=0; b<1024; b++)
			{
				u32 free_bit;
				find(hbb, config, maxbits, free_bit);
				CHECK_EQUAL(b, free_bit);
				clr(hbb, config, maxbits, b);
			}
		}

		UNITTEST_TEST(find_free_bit_2)
		{
			clear_bitmap();

			u32 const maxbits = 8192;
			hbb_t hbb = { bitmap };
			u32 config;
			CHECK_TRUE(sizeof_hbb(maxbits, config) < 4096);
			init(hbb, maxbits, config, 0);
			reset(hbb, config, maxbits, 0);

			// Should not be able to find any '1'
			u32 free_bit;
			CHECK_EQUAL(false, find(hbb, config, maxbits, free_bit));

			for (s32 b=1024-1; b>=0; --b)
			{
				set(hbb, config, maxbits, b);
				find(hbb, config, maxbits, free_bit);
				CHECK_EQUAL(b, free_bit);
			}
		}

	}
}
UNITTEST_SUITE_END
