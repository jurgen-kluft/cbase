#include "xbase/x_tree.h"
#include "xbase/x_allocator.h"
#include "xbase/x_slice.h"

#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;


UNITTEST_SUITE_BEGIN(xtree)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
			
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{

		}

	
		UNITTEST_TEST(simple_s32_tree)
		{
			xtree_t<s32> tree(gTestAllocator);

			s32 a(1);
			s32 b(2);
			s32 c(3);
			s32 d(4);

			const char* result = NULL;

			bool inserted;
			inserted = tree.insert(&a);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&b);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&b);				// Duplicate insert should fail
			CHECK_FALSE(inserted);
			inserted = tree.insert(&c);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&d);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));

			CHECK_EQUAL(4, tree.size());

			CHECK_TRUE(tree.find(&a));
			CHECK_TRUE(tree.find(&b));
			CHECK_TRUE(tree.find(&c));
			CHECK_TRUE(tree.find(&d));

			s32 e(5);
			CHECK_FALSE(tree.find(&e));

			s32 * data;
			while (!tree.clear(data))
			{

			}

			CHECK_EQUAL(0, tree.size());
		}

	}
}
UNITTEST_SUITE_END
