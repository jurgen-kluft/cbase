#include "xbase/x_tree.h"
#include "xbase/x_allocator.h"
#include "xbase/x_slice.h"

#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::xalloc* gTestAllocator;


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

UNITTEST_SUITE_BEGIN(tree_t)
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
			tree_t::node_t node;

			tree_t::node_t* left_ptr = (tree_t::node_t*)((uptr)0 - (uptr)1);
			node.set_left(left_ptr);
			
			CHECK_EQUAL(left_ptr, node.get_left());

			tree_t::node_t* right_ptr = (tree_t::node_t*)((uptr)0 - (uptr)2);
			node.set_right(right_ptr);
			CHECK_EQUAL(right_ptr, node.get_right());

			CHECK_EQUAL(true, node.is_red());

			node.set_black();
			CHECK_EQUAL(false, node.is_red());
			node.set_red();
			CHECK_EQUAL(true, node.is_red());
			node.set_black();
			CHECK_EQUAL(false, node.is_red());

			CHECK_EQUAL(true, node.is_black());

			left_ptr = (tree_t::node_t*)0x100;
			right_ptr = (tree_t::node_t*)0x200;
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
			tree_t tree;

			s32 a = 1;
			s32 b = 2;
			s32 c = 3;
			s32 d = 4;
			s32 e = 5;
			s32 f = 6;
			s32 g = 7;
			s32 h = 8;
			s32 i = 9;
			tree_t::node_t na;
			tree_t::node_t nb;
			tree_t::node_t nc;
			tree_t::node_t nd;
			tree_t::node_t ne;
			tree_t::node_t nf;
			tree_t::node_t ng;
			tree_t::node_t nh;
			tree_t::node_t ni;

			const char* result = NULL;

			bool inserted;
			inserted = tree.insert(&a, &na);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&b, &nb);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&b, &nb);				// Duplicate insert should fail
			CHECK_FALSE(inserted);
			inserted = tree.insert(&c, &nc);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&d, &nd);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&e, &ne);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&f, &nf);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&g, &ng);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&h, &nh);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&i, &ni);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));

			CHECK_EQUAL(9, tree.size());

			tree_t::node_t* found;
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

			void* data;
			tree_t::node_t* node;
			while (!tree.clear(data, node))
			{
			}

			CHECK_EQUAL(0, tree.size());
		}

		UNITTEST_TEST(void_tree_iterate_preorder)
		{
			tree_t tree;
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
			tree_t::node_t na;
			tree_t::node_t nb;
			tree_t::node_t nc;
			tree_t::node_t nd;
			tree_t::node_t ne;
			tree_t::node_t nf;
			tree_t::node_t ng;
			tree_t::node_t nh;
			tree_t::node_t ni;

			CHECK_TRUE(tree.insert(&a, &na));
			CHECK_TRUE(tree.insert(&b, &nb));
			CHECK_TRUE(tree.insert(&c, &nc));
			CHECK_TRUE(tree.insert(&d, &nd));
			CHECK_TRUE(tree.insert(&e, &ne));
			CHECK_TRUE(tree.insert(&f, &nf));
			CHECK_TRUE(tree.insert(&g, &ng));
			CHECK_TRUE(tree.insert(&h, &nh));
			CHECK_TRUE(tree.insert(&i, &ni));

			tree_t::iterator iterator = tree.iterate();

			s32 round = 0;
			s32 preorder[] = { d,b,a,c,f,e,h,g,i };
			void* data;
			while (iterator.preorder(tree_t::cRight, data))
			{
				CHECK_EQUAL(preorder[round++], *(s32*)data);
			}
			CHECK_EQUAL(9, round);

			tree_t::node_t* node;
			while (!tree.clear(data, node))
			{
			}
		}

		UNITTEST_TEST(void_tree_iterate_sortorder)
		{
			tree_t tree;
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

			tree_t::node_t na;
			tree_t::node_t nb;
			tree_t::node_t nc;
			tree_t::node_t nd;
			tree_t::node_t ne;
			tree_t::node_t nf;
			tree_t::node_t ng;
			tree_t::node_t nh;
			tree_t::node_t ni;

			CHECK_TRUE(tree.insert(&a, &na));
			CHECK_TRUE(tree.insert(&b, &nb));
			CHECK_TRUE(tree.insert(&c, &nc));
			CHECK_TRUE(tree.insert(&d, &nd));
			CHECK_TRUE(tree.insert(&e, &ne));
			CHECK_TRUE(tree.insert(&f, &nf));
			CHECK_TRUE(tree.insert(&g, &ng));
			CHECK_TRUE(tree.insert(&h, &nh));
			CHECK_TRUE(tree.insert(&i, &ni));

			tree_t::iterator iterator = tree.iterate();

			s32 round = 0;
			s32 sortorder[] = { a,b,c,d,e,f,g,h,i };
			void* data;
			while (iterator.sortorder(tree_t::cRight, data))
			{
				CHECK_EQUAL(sortorder[round++], *(s32*)data);
			}
			CHECK_EQUAL(9, round);

			tree_t::node_t* node;
			while (!tree.clear(data, node))
			{
			}
		}

		UNITTEST_TEST(void_tree_iterate_sortorder_backwards)
		{
			tree_t tree;
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

			tree_t::node_t na;
			tree_t::node_t nb;
			tree_t::node_t nc;
			tree_t::node_t nd;
			tree_t::node_t ne;
			tree_t::node_t nf;
			tree_t::node_t ng;
			tree_t::node_t nh;
			tree_t::node_t ni;

			CHECK_TRUE(tree.insert(&a, &na));
			CHECK_TRUE(tree.insert(&b, &nb));
			CHECK_TRUE(tree.insert(&c, &nc));
			CHECK_TRUE(tree.insert(&d, &nd));
			CHECK_TRUE(tree.insert(&e, &ne));
			CHECK_TRUE(tree.insert(&f, &nf));
			CHECK_TRUE(tree.insert(&g, &ng));
			CHECK_TRUE(tree.insert(&h, &nh));
			CHECK_TRUE(tree.insert(&i, &ni));

			tree_t::iterator iterator = tree.iterate();

			s32 round = 0;
			s32 sortorder[] = { i,h,g,f,e,d,c,b,a };
			void* data;
			while (iterator.sortorder(tree_t::cLeft, data))
			{
				CHECK_EQUAL(sortorder[round++], *(s32*)data);
			}
			CHECK_EQUAL(9, round);

			tree_t::node_t* node;
			while (!tree.clear(data, node))
			{
			}
		}

		UNITTEST_TEST(void_tree_iterate_postorder)
		{
			tree_t tree;
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

			tree_t::node_t na;
			tree_t::node_t nb;
			tree_t::node_t nc;
			tree_t::node_t nd;
			tree_t::node_t ne;
			tree_t::node_t nf;
			tree_t::node_t ng;
			tree_t::node_t nh;
			tree_t::node_t ni;

			CHECK_TRUE(tree.insert(&a, &na));
			CHECK_TRUE(tree.insert(&b, &nb));
			CHECK_TRUE(tree.insert(&c, &nc));
			CHECK_TRUE(tree.insert(&d, &nd));
			CHECK_TRUE(tree.insert(&e, &ne));
			CHECK_TRUE(tree.insert(&f, &nf));
			CHECK_TRUE(tree.insert(&g, &ng));
			CHECK_TRUE(tree.insert(&h, &nh));
			CHECK_TRUE(tree.insert(&i, &ni));

			tree_t::iterator iterator = tree.iterate();

			s32 round = 0;
			s32 postorder[] = { a,c,b,e,g,i,h,f,d };
			
			void* data;
			while (iterator.postorder(tree_t::cRight, data))
			{
				CHECK_EQUAL(postorder[round++], *(s32*)data);
			}
			CHECK_EQUAL(9, round);

			tree_t::node_t* node;
			while (!tree.clear(data, node))
			{
			}
		}

		UNITTEST_TEST(void_tree_search)
		{
			tree_t tree;
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

			tree_t::node_t na;
			tree_t::node_t nb;
			tree_t::node_t nc;
			tree_t::node_t nd;
			tree_t::node_t ne;
			tree_t::node_t nf;
			tree_t::node_t ng;
			tree_t::node_t nh;
			tree_t::node_t ni;

			CHECK_TRUE(tree.insert(&a, &na));
			CHECK_TRUE(tree.insert(&b, &nb));
			CHECK_TRUE(tree.insert(&c, &nc));
			CHECK_TRUE(tree.insert(&d, &nd));
			CHECK_TRUE(tree.insert(&e, &ne));
			CHECK_TRUE(tree.insert(&f, &nf));
			CHECK_TRUE(tree.insert(&g, &ng));
			CHECK_TRUE(tree.insert(&h, &nh));
			CHECK_TRUE(tree.insert(&i, &ni));

			tree_t::iterator iterator = tree.iterate();

			s32 dir = tree_t::cLeft;
			void* data;
			void* find = &f;
			while (iterator.traverse(dir, data))
			{
				s32 const c = compare_s32(find, data);
				if (c == 0)
					break;
				dir = iterator.getdir(c);
			}
			CHECK_EQUAL(0, compare_s32(data, find));

			tree_t::node_t* node;
			while (!tree.clear(data, node))
			{
			}
		}


		UNITTEST_TEST(s32_tree)
		{
			tree_t tree;

			s32 a(1);
			s32 b(2);
			s32 c(3);
			s32 d(4);

			tree_t::node_t na;
			tree_t::node_t nb;
			tree_t::node_t nc;
			tree_t::node_t nd;
			tree_t::node_t ne;
			tree_t::node_t nf;
			tree_t::node_t ng;
			tree_t::node_t nh;
			tree_t::node_t ni;

			const char* result = NULL;

			bool inserted;
			inserted = tree.insert(&a, &na);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&b, &nb);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&b, &nb);	// Duplicate insert should fail
			CHECK_FALSE(inserted);
			inserted = tree.insert(&c, &nc);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));
			inserted = tree.insert(&d, &nd);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.validate(result));

			CHECK_EQUAL(4, tree.size());

			tree_t::node_t* found_node;
			CHECK_EQUAL(true, tree.find(&a, found_node));
			CHECK_EQUAL(true, tree.find(&b, found_node));
			CHECK_EQUAL(true, tree.find(&c, found_node));
			CHECK_EQUAL(true, tree.find(&d, found_node));

			s32 e(5);
			CHECK_FALSE(tree.find(&e, found_node));

			void * data;
			tree_t::node_t* node;
			while (!tree.clear(data, node))
			{
			}

			CHECK_EQUAL(0, tree.size());
		}

	}
}
UNITTEST_SUITE_END
