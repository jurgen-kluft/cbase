#include "xbase/x_tree.h"
#include "xbase/x_allocator.h"
#include "xbase/x_slice.h"

#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;


s32				compare_s32(void const* p1, void const* p2)
{
	s32 const s1 = *(s32 const*)p1;
	s32 const s2 = *(s32 const*)p2;
	if (s1 < s2)
		return -1;
	else if (s1 > s2)
		return 1;
	return 0;
}

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

		UNITTEST_TEST(tree_node)
		{
			xnode_t node;
			node.clear();

			xnode_t* left_ptr = (xnode_t*)((uptr)0 - (uptr)1);
			node.set_left(left_ptr);
			
			CHECK_EQUAL(left_ptr, node.get_left());

			xnode_t* right_ptr = (xnode_t*)((uptr)0 - (uptr)2);
			node.set_right(right_ptr);
			CHECK_EQUAL(right_ptr, node.get_right());

			CHECK_EQUAL(true, node.is_red());

			node.set_right(left_ptr);
			CHECK_EQUAL(false, node.is_red());
			node.set_red();
			CHECK_EQUAL(true, node.is_red());
			node.set_black();
			CHECK_EQUAL(false, node.is_red());

			CHECK_EQUAL(true, node.is_black());

			left_ptr = (xnode_t*)0x100;
			right_ptr = (xnode_t*)0x200;
			node.set_left(left_ptr);
			node.set_right(right_ptr);
			CHECK_EQUAL(left_ptr, node.get_child(0));
			CHECK_EQUAL(right_ptr, node.get_child(1));

			CHECK_EQUAL(true, node.is_black());

			node.set_child(0, left_ptr);
			node.set_child(1, right_ptr);

			CHECK_EQUAL(left_ptr, node.get_child(0));
			CHECK_EQUAL(right_ptr, node.get_child(1));

			CHECK_EQUAL(true, node.is_black());
		}

		UNITTEST_TEST(void_tree)
		{
			xtree tree(gTestAllocator);

			s32 a = 1;
			s32 b = 2;
			s32 c = 3;
			s32 d = 4;
			s32 e = 5;
			s32 f = 6;
			s32 g = 7;
			s32 h = 8;
			s32 i = 9;

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
			inserted = tree.insert(&e);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&f);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&g);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&h);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&i);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));

			void* found;
			CHECK_EQUAL(true, tree.find(&a, found));
			CHECK_EQUAL(true, tree.find(&b, found));
			CHECK_EQUAL(true, tree.find(&c, found));
			CHECK_EQUAL(true, tree.find(&d, found));
			CHECK_EQUAL(true, tree.find(&e, found));
			CHECK_EQUAL(true, tree.find(&f, found));
			CHECK_EQUAL(true, tree.find(&g, found));
			CHECK_EQUAL(true, tree.find(&h, found));
			CHECK_EQUAL(true, tree.find(&i, found));

			s32 x(99);
			CHECK_FALSE(tree.find(&x, found));

			CHECK_EQUAL(9, tree.size());

			void * data;
			while (!tree.clear(data))
			{
			}

			CHECK_EQUAL(0, tree.size());
		}

		UNITTEST_TEST(void_tree_iterate_preorder)
		{
			xtree tree(gTestAllocator);
			tree.set_cmp(compare_s32);

			s32 a = 1;
			s32 b = 2;
			s32 c = 3;
			s32 d = 4;
			s32 e = 5;
			s32 f = 6;
			s32 g = 7;
			s32 h = 8;
			s32 i = 9;

			CHECK_TRUE(tree.insert(&c));
			CHECK_TRUE(tree.insert(&d));
			CHECK_TRUE(tree.insert(&a));
			CHECK_TRUE(tree.insert(&b));
			CHECK_TRUE(tree.insert(&e));
			CHECK_TRUE(tree.insert(&f));
			CHECK_TRUE(tree.insert(&g));
			CHECK_TRUE(tree.insert(&h));
			CHECK_TRUE(tree.insert(&i));

			// preorder: F B A D C E F I H
			// sortorder: A B C D E F G H I 
			// postorder: A C E D B H I G F
			// levelorder: F B G A D I C E H

			xtree_iterator iterator;
			tree.iterate(iterator);
			iterator.init_sortorder();

			s32 round = 0;
			s32 preorder[] = { a,b,c,d,e,f,g,h,i };
			void* data;
			while (iterator.iterate(data))
			{
				CHECK_EQUAL(preorder[round++], *(s32*)data);
			}

			while (!tree.clear(data))
			{
			}
		}


		UNITTEST_TEST(s32_tree)
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

			s32* found;
			CHECK_EQUAL(true, tree.find(&a, found));
			CHECK_EQUAL(true, tree.find(&b, found));
			CHECK_EQUAL(true, tree.find(&c, found));
			CHECK_EQUAL(true, tree.find(&d, found));

			s32 e(5);
			CHECK_FALSE(tree.find(&e, found));

			s32 * data;
			while (!tree.clear(data))
			{
			}

			CHECK_EQUAL(0, tree.size());
		}

	}
}
UNITTEST_SUITE_END
