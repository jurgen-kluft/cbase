#include "xbase/x_allocator.h"
#include "xbase/x_carray.h"
#include "xbase/x_slice.h"
#include "xbase/x_range.h"
#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;

UNITTEST_SUITE_BEGIN(containers)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test_array_t)
		{
			memory mem(gTestAllocator);

			array_t<s32> int100;
			make(mem, int100, 100, 0);

			// New Version: 99 -> 0
			range_t<s32> rng_iter(from(0), count(100));
			while (forward(rng_iter)) {
				append(int100, *rng_iter);
			}

			// 0 -> 99
			sort(int100);

			// Indexed items
			indexed_t<s32> indexed_int100;
			make(mem, indexed_int100, 100);

			// 0 -> 49
			auto int50 = int100(0, 50);

			f32 f = 0.0f;
			map_t<s32, f32> int_to_float;
			make(mem, int_to_float, 100);

			auto int100_iter = int100.begin();
			while (iterate(int100_iter)) {
				int_to_float[*int100_iter] = f;
				f += 0.1f;
			}

			// Slice the array into a view of just 2 elements
			// This will NOT make a copy of the data, only when
			// you now 'resize' one of them.
			auto int2 = int100(0, 2);

			s32 int0 = int2[0];
			s32 int1 = int2[1];

			queue_t<s32> q100;
			make(mem, q100, 100);

			// You can just start the range iterator again
			while (forward(rng_iter)) {
				append(q100, *rng_iter);
			}

			// And again!
			s32 qi;
			while (forward(rng_iter)) {
				q100.pop(qi);
			}
		}

	}
}
UNITTEST_SUITE_END
