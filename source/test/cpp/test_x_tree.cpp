#include "xbase/private/x_rbtree_sentinel.h"
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

		struct mynode : public xrbsnode
		{
			mynode() : id(0) { clear(); }
			mynode(uptr i) : id(i) { clear(); }
			uptr id;

			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

		s32		rb_node_compare(void* aa, xrbsnode* bb)
		{
			uptr aid = (uptr)aa;
			mynode* b = (mynode*)bb;

			if (aid < b->id)
				return -1;
			else if (aid > b->id)
				return 1;
			return 0;
		}
		s32		rb_node_node_compare(xrbsnode* aa, xrbsnode* bb)
		{
			mynode* a = (mynode*)aa;
			mynode* b = (mynode*)bb;

			if (a->id < b->id)
				return -1;
			else if (a->id > b->id)
				return 1;
			return 0;
		}

		void	rb_node_remove(xrbsnode* to_replace, xrbsnode* to_remove)
		{
			mynode* replacer = (mynode*)to_replace;
			mynode* removed = (mynode*)to_remove;
			uptr id = replacer->id;
			replacer->id = removed->id;
			removed->id = id;
		}

	
		UNITTEST_TEST(constructor1)
		{
			xrbstree tree;
			tree.init(rb_node_compare);

			mynode a(1);
			mynode b(2);
			mynode c(3);
			mynode d(4);

			const char* result = NULL;

			bool inserted;
			inserted = tree.insert((void*)a.id, &a);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.test(result));
			inserted = tree.insert((void*)b.id, &b);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.test(result));
			inserted = tree.insert((void*)c.id, &c);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.test(result));
			inserted = tree.insert((void*)d.id, &d);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.test(result));

			xrbsnode* f = NULL;
			tree.find((void*)c.id, f);
			CHECK_EQUAL(&c, f);
			CHECK_TRUE(tree.test(result));
			tree.find((void*)a.id, f);
			CHECK_EQUAL(&a, f);
			CHECK_TRUE(tree.test(result));
			tree.find((void*)b.id, f);
			CHECK_EQUAL(&b, f);
			CHECK_TRUE(tree.test(result));
			tree.find((void*)d.id, f);
			CHECK_EQUAL(&d, f);
			CHECK_TRUE(tree.test(result));

			xrbsnode* iterator = NULL;
			s32 i = 0;
			do
			{
				xrbsnode* node_to_destroy = tree.clear(iterator);
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
			} while (iterator !=NULL);
		}


		UNITTEST_TEST(case1)
		{
			x_type_allocator<mynode, x_cdtor_placement_new<mynode> > node_allocator(gTestAllocator);
			
			xrbstree tree;
			tree.init(rb_node_compare);

			const int max_tracked_allocs = 1000;
			uptr allocations[max_tracked_allocs];
			for (s32 i = 0; i < max_tracked_allocs; ++i)
			{
				allocations[i] = 0;
			}

			const char* test_result = NULL;

			for (s32 i = 0; i < 3200; ++i)
			{
				mynode* node = node_allocator.allocate();
				node->clear();
				node->id = (i);

				int alloc_idx = rand() % max_tracked_allocs;
				if (allocations[alloc_idx] != 0)
				{
					uptr pid = allocations[alloc_idx];
					mynode _p_(pid);

					xrbsnode* f = NULL;
					if (tree.remove((void*)pid, f))
					{
						CHECK_EQUAL(((mynode*)f)->id, pid);
						CHECK_TRUE(tree.test(test_result));
						node_allocator.deallocate((mynode*)f);
					}
					else
					{
						CHECK_EQUAL(0, 1);
					}
					allocations[alloc_idx] = 0;
				}
				allocations[alloc_idx] = node->id;
				bool inserted = tree.insert((void*)node->id, node);
				CHECK_TRUE(inserted);
				CHECK_TRUE(tree.test(test_result));
			}

			for (s32 i = 0; i < max_tracked_allocs; ++i)
			{
				if (allocations[i] != NULL)
				{
					uptr pid = allocations[i];
					mynode _p_(pid);

					xrbsnode* f = NULL;
					if (tree.remove((void*)pid, f))
					{
						CHECK_EQUAL(((mynode*)f)->id, pid);
						node_allocator.deallocate((mynode*)f);
					}
					allocations[i] = 0;
				}
			}

			xrbsnode* iterator = NULL;
			xrbsnode* node_to_destroy = NULL;
			do 
			{
				node_to_destroy = tree.clear(iterator);
				if (node_to_destroy != NULL)
					node_allocator.deallocate((mynode*)node_to_destroy);
			} while (iterator != NULL);
		}

	}
}
UNITTEST_SUITE_END
