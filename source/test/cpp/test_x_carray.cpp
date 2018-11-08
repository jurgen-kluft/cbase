#include "xbase/x_carray.h"
#include "xunittest/xunittest.h"

using namespace xcore;


UNITTEST_SUITE_BEGIN(xcarray)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static s32	array_data[64];

		UNITTEST_TEST(construct)
		{
			xcarray a(array_data, sizeof(s32), 5);

			CHECK_EQUAL(0, a.size());
			CHECK_EQUAL(5, a.reserved());
		}

		UNITTEST_TEST(push_back)
		{
			xcarray a(array_data, sizeof(s32), 5);

			CHECK_EQUAL(0, a.size());
			CHECK_EQUAL(5, a.reserved());

			s32 value = 1;
			a.push_back(&value);

			CHECK_EQUAL(1, a.size());
			CHECK_EQUAL(5, a.reserved());
		}

		UNITTEST_TEST(pop_back)
		{
			xcarray a(array_data, sizeof(s32), 5);

			CHECK_EQUAL(0, a.size());
			CHECK_EQUAL(5, a.reserved());

			s32 value;
			CHECK_FALSE(a.pop_back(&value));

			CHECK_EQUAL(0, a.size());
			CHECK_EQUAL(5, a.reserved());
		}

		UNITTEST_TEST(push_and_pop_back)
		{
			xcarray a(array_data, sizeof(s32), 5);

			CHECK_EQUAL(0, a.size());
			CHECK_EQUAL(5, a.reserved());

			s32 value = 1;
			a.push_back(&value);
			CHECK_EQUAL(1, a.size());
			CHECK_EQUAL(5, a.reserved());

			s32 value;
			CHECK_TRUE(a.pop_back(&value));
			CHECK_EQUAL(1, value);

			CHECK_EQUAL(0, a.size());
			CHECK_EQUAL(5, a.reserved());
		}

		UNITTEST_TEST(operator_index)
		{
			xcarray a(array_data, sizeof(s32), 10);
			for (u32 i=0; i<a.reserved(); ++i)
				a.push_back(&i);

			s32* value = (s32*)a[0];
			*value += 10;
			CHECK_EQUAL(10, *((s32*)a[0]));
			CHECK_EQUAL(6, *((s32*)a[6]));
		}

		UNITTEST_TEST(swap)
		{
			xcarray a(array_data, sizeof(s32), 10);
			for (u32 i=0; i<a.reserved(); ++i)
				a.push_back(&i);

			a.swap(4, 7);
			CHECK_EQUAL(4, *((s32*)a[7]));
			CHECK_EQUAL(7, *((s32*)a[4]));
		}
		
		UNITTEST_TEST(remove)
		{
			xcarray a(array_data, sizeof(s32), 10);
			for (u32 i=0; i<a.reserved(); ++i)
				a.push_back(&i);

			CHECK_EQUAL(10, a.size());
			CHECK_EQUAL(10, a.reserved());

			a.remove(4);

			CHECK_EQUAL(9, a.size());
			CHECK_EQUAL(10, a.reserved());

			CHECK_EQUAL(5, *((s32*)a[4]));
			CHECK_EQUAL(6, *((s32*)a[5]));
			CHECK_EQUAL(9, *((s32*)a[8]));

			a.remove(8);
			CHECK_EQUAL(8, a.size());
			CHECK_EQUAL(10, a.reserved());
			CHECK_EQUAL(8, *((s32*)a[7]));
		}

		UNITTEST_TEST(swap_remove)
		{
			xcarray a(array_data, sizeof(s32), 10);
			for (u32 i=0; i<a.reserved(); ++i)
				a.push_back(&i);

			CHECK_EQUAL(10, a.size());
			CHECK_EQUAL(10, a.reserved());

			a.swap_remove(4);

			CHECK_EQUAL(9, a.size());
			CHECK_EQUAL(10, a.reserved());

			CHECK_EQUAL(9, *((s32*)a[4]));
			CHECK_EQUAL(5, *((s32*)a[5]));
			CHECK_EQUAL(8, *((s32*)a[8]));

			a.swap_remove(8);
			CHECK_EQUAL(8, a.size());
			CHECK_EQUAL(10, a.reserved());
			CHECK_EQUAL(7, *((s32*)a[7]));
		}
	}
}
UNITTEST_SUITE_END
