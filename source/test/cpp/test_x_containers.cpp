#include "xbase/x_allocator.h"
#include "xbase/x_carray.h"
#include "xbase/x_slice.h"
#include "xbase/x_slice_t.h"
#include "xbase/x_indexed_t.h"
#include "xbase/x_array_t.h"
#include "xbase/x_map_t.h"
#include "xbase/x_queue_t.h"
#include "xbase/x_range.h"
#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::xalloc* gTestAllocator;

UNITTEST_SUITE_BEGIN(xcontainers)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test_array_t)
		{
			slice_t<s32> ss32(gTestAllocator, 100);
			CHECK_EQUAL(1, ss32.m_slice.refcnt())
			slice_t<s32> ss32_2 = ss32(10,20);
			CHECK_EQUAL(2, ss32.m_slice.refcnt())
			CHECK_EQUAL(2, ss32_2.m_slice.refcnt())

			const s32 N = 1000;

			array_t<s32> int1000;
			make(gTestAllocator, int1000, N, 0);

			CHECK_EQUAL(0, int1000.size())
			CHECK_EQUAL(N, int1000.max())
			CHECK_EQUAL(1, int1000.m_data.m_slice.refcnt())

			// New Version: 99 -> 0
			range_t<s32> rng_iter(from(0), count(N));
			for (s32 i : backwards(rng_iter)) 
			{
				append(int1000, i);
			}

			CHECK_EQUAL(N, int1000.size())
			CHECK_EQUAL(1, int1000.m_data.m_slice.refcnt())

			// 0 -> 99
			sort(int1000);

			CHECK_EQUAL(1, int1000.m_data.m_slice.refcnt())

			// 20 -> 69
			array_t<s32> int50 = int1000(20, 70);

			CHECK_EQUAL(2, int50.m_data.m_slice.refcnt())
			CHECK_EQUAL(2, int1000.m_data.m_slice.refcnt())

return;

//			// Indexed items
//			indexed_t<s32> indexed_int1000;
//			make(mem, indexed_int1000, N);
//
//			CHECK_EQUAL(50, int50.size());
//			for (u32 i = 0; i < int50.size(); ++i)
//			{
//				CHECK_EQUAL(20 + i, int50[i]);
//			}

			//f32 f = 0.0f;
			//map_t<s32, f32> int_to_float;
			//make(mem, int_to_float, N);
//
			//auto int1000_iter = int1000.begin();
			//while (iterate(int1000_iter)) {
			//	auto item = int_to_float[*int1000_iter];
			//	if (!(bool)item) {
			//		item = f;
			//	}
//
			//	auto item2 = int_to_float[*int1000_iter];
			//	CHECK_TRUE((bool)item2);
//
			//	f += 1.f;
			//}

//			f = 0.0f;
//			int1000_iter.reset();
//			while (iterate(int1000_iter)) {
//				auto item = int_to_float[*int1000_iter];
//				CHECK_TRUE((bool)item);
//				f32 v = item;
//				CHECK_EQUAL(v, f);
//				f += 1.f;
//			}
//
//			int1000_iter.reset();
//			while (iterate(int1000_iter)) {
//				if (!int_to_float.remove(*int1000_iter))
//				{
//					int_to_float.remove(*int1000_iter);
//					CHECK_TRUE(false);
//				}
//			}

			// Slice the array into a view of just 2 elements
			// This will NOT make a copy of the data, only when
			// you now 'append' to one of them.
			auto int2 = int1000(10, 12);

			s32 int0 = int2[0];
			s32 int1 = int2[1];

			queue_t<s32> q100;
			make(gTestAllocator, q100, N);

			// You can just start the range iterator again
			for (s32 i : rng_iter)
			{
				append(q100, *rng_iter);
			}
			CHECK_FALSE(append(q100, 0xFA12E));

			// And again!
			s32 qi;
			for (s32 i : rng_iter) 
			{
				q100.pop(qi);
			}
			CHECK_FALSE(q100.pop(qi));
		}

	}
}
UNITTEST_SUITE_END
