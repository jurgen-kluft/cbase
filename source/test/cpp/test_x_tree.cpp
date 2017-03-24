#include "xbase\x_tree.h"
#include "xbase\x_allocator.h"
#include "xbase\x_slice.h"

#include "xunittest\xunittest.h"

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

		struct mynode : public xrbnode
		{
			mynode() : id(0) { clear(); }
			mynode(uptr i) : id(i) { clear(); }
			uptr id;

			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

		s32		rb_node_compare(xrbnode* aa, xrbnode* bb)
		{
			mynode* a = (mynode*)aa;
			mynode* b = (mynode*)bb;

			if (a->id < b->id)
				return -1;
			else if (a->id > b->id)
				return 1;
			return 0;
		}

		void	rb_node_remove(xrbnode* to_replace, xrbnode* to_remove)
		{
			mynode* replacer = (mynode*)to_replace;
			mynode* removed = (mynode*)to_remove;
			uptr id = replacer->id;
			replacer->id = removed->id;
			removed->id = id;
		}

		const char*	rb_test(xrbnode* root)
		{
			const char* result = NULL;
			rb_tree_test(root, rb_node_compare, result);
			if (result != NULL)
				return result;
			return NULL;
		}

		UNITTEST_TEST(constructor1)
		{
			xrbnode* root = NULL;

			mynode a(1);
			mynode b(2);
			mynode c(3);
			mynode d(4);

			bool inserted;
			inserted = xtree_insert(root, &a, rb_node_compare);
			CHECK_TRUE(inserted);
			CHECK_NULL(rb_test(root));
			inserted = xtree_insert(root, &b, rb_node_compare);
			CHECK_TRUE(inserted);
			CHECK_NULL(rb_test(root));
			inserted = xtree_insert(root, &c, rb_node_compare);
			CHECK_TRUE(inserted);
			CHECK_NULL(rb_test(root));
			inserted = xtree_insert(root, &d, rb_node_compare);
			CHECK_TRUE(inserted);
			CHECK_NULL(rb_test(root));

			xrbnode* f = NULL;
			xtree_find(root, &c, rb_node_compare, f);
			CHECK_EQUAL(&c, f);
			CHECK_NULL(rb_test(root));
			xtree_find(root, &a, rb_node_compare, f);
			CHECK_EQUAL(&a, f);
			CHECK_NULL(rb_test(root));
			xtree_find(root, &b, rb_node_compare, f);
			CHECK_EQUAL(&b, f);
			CHECK_NULL(rb_test(root));
			xtree_find(root, &d, rb_node_compare, f);
			CHECK_EQUAL(&d, f);
			CHECK_NULL(rb_test(root));

			xrbnode* node_to_destroy = NULL;
			s32 i = 0;
			do
			{
				node_to_destroy = xtree_clear(root);
				if (i == 0)
				{
					CHECK_TRUE(node_to_destroy == &a);
				}
				else if (i == 1)
				{
					CHECK_TRUE(node_to_destroy == &b);
				}
				else if (i == 2)
				{
					CHECK_TRUE(node_to_destroy == &c);
				}
				else if (i == 3)
				{
					CHECK_TRUE(node_to_destroy == &d);
				}
				else if (i == 4)
				{
					CHECK_TRUE(node_to_destroy == NULL);
				}
				i++;
			} while (node_to_destroy!=NULL);
		}


		UNITTEST_TEST(case1)
		{
			x_type_allocator<mynode, x_cdtor_placement_new<mynode> > node_allocator(gTestAllocator);
			
			xrbnode* root = NULL;

			const int max_tracked_allocs = 1000;
			uptr allocations[max_tracked_allocs];
			for (s32 i = 0; i < max_tracked_allocs; ++i)
			{
				allocations[i] = 0;
			}

			for (s32 i = 0; i < 10000; ++i)
			{
				for (s32 a = 0; a < 32; ++a)
				{
					mynode* node = node_allocator.allocate();
					node->clear();
					node->id = (a*20000 + i);

					int alloc_idx = rand() % max_tracked_allocs;
					if (allocations[alloc_idx] != 0)
					{
						uptr pid = allocations[alloc_idx];
						mynode _p_(pid);

						xrbnode* f = NULL;
						if (xtree_remove(root, &_p_, rb_node_compare, rb_node_remove, f))
						{
							CHECK_EQUAL(((mynode*)f)->id, pid);
							CHECK_NULL(rb_test(root));
							node_allocator.deallocate((mynode*)f);
						}
						else
						{
							CHECK_EQUAL(0, 1);
						}
						allocations[alloc_idx] = 0;
					}
					allocations[alloc_idx] = node->id;
					bool inserted = xtree_insert(root, node, rb_node_compare);
					CHECK_TRUE(inserted);
					CHECK_NULL(rb_test(root));
				}
			}

			for (s32 i = 0; i < max_tracked_allocs; ++i)
			{
				if (allocations[i] != NULL)
				{
					uptr pid = allocations[i];
					mynode _p_(pid);

					xrbnode* f = NULL;
					if (xtree_remove(root, &_p_, rb_node_compare, rb_node_remove, f))
					{
						CHECK_EQUAL(((mynode*)f)->id, pid);
						node_allocator.deallocate((mynode*)f);
						if (f == root)
							root = NULL;
					}
					allocations[i] = 0;
				}
			}

			xrbnode* node_to_destroy = xtree_clear(root);
			while (node_to_destroy != NULL)
			{
				node_allocator.deallocate((mynode*)node_to_destroy);
				node_to_destroy = xtree_clear(root);
			};
		}

	}
}
UNITTEST_SUITE_END
