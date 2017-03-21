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

		struct test_node : public xrbnode
		{
			test_node(u64 id_ = 0) : id(id_) { clear(); }
			u64		id;

			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

		xrbnode*	test_tnd(void* key)
		{
			test_node* tn = (test_node*)key;
			return tn;
		}

		s32			test_id(const xrbnode* node, const void* key)
		{
			test_node* tn_node = (test_node*)node;
			u64 id = (u64)key;
			if (tn_node->id < id) return 1;
			else if (tn_node->id > id) return -1;
			else return 0;
		}

		s32			test_cd(const xrbnode* node, const void* key)
		{
			test_node* tn_node = (test_node*)node;
			test_node* tn_key  = (test_node*)key;
			if (tn_node->id < tn_key->id) return 1;
			else if (tn_node->id > tn_key->id) return -1;
			else return 0;
		}

		UNITTEST_TEST(constructor1)
		{
			xrbnode root_node;
			root_node.clear();
			xrbnode* root = &root_node;

			test_node a(1);
			test_node b(2);
			test_node c(3);
			test_node d(4);

			xtree_insert(root, &c, &c, test_cd);
			xtree_insert(root, &b, &b, test_cd);
			xtree_insert(root, &d, &d, test_cd);
			xtree_insert(root, &a, &a, test_cd);

			xrbnode* f3;
			xtree_find(root, f3, (void*)3, test_id);
			CHECK_EQUAL(&c, f3);
			xrbnode* f1;
			xtree_find(root, f1, (void*)1, test_id);
			CHECK_EQUAL(&a, f1);
			xrbnode* f2;
			xtree_find(root, f2, (void*)2, test_id);
			CHECK_EQUAL(&b, f2);
			xrbnode* f4;
			xtree_find(root, f4, (void*)4, test_id);
			CHECK_EQUAL(&d, f4);

			xrbnode* node_to_destroy = NULL;
			xrbnode* iterator = NULL;
			do
			{
				node_to_destroy = xtree_clear(root, iterator);
			} while (node_to_destroy!=NULL);
		}


		UNITTEST_TEST(case1)
		{
			x_type_allocator<test_node, x_cdtor_placement_new<test_node> > node_allocator(gTestAllocator);
			test_node* root = node_allocator.allocate();

			const int max_tracked_allocs = 1000;
			test_node*	allocations[max_tracked_allocs];
			for (s32 i = 0; i < max_tracked_allocs; ++i)
			{
				allocations[i] = NULL;
			}

			for (s32 i = 0; i < 10000; ++i)
			{
				for (s32 a = 0; a < 32; ++a)
				{
					test_node* node = node_allocator.allocate();
					node->id = (u64)node;

					int alloc_idx = rand() % max_tracked_allocs;
					if (allocations[alloc_idx] != NULL)
					{
						test_node* p = allocations[alloc_idx];

						xrbnode* f;
						if (xtree_remove(root, f, p, test_cd))
						{
							node_allocator.deallocate(p);
							allocations[alloc_idx] = NULL;
						}
					}
					allocations[alloc_idx] = node;
					xtree_insert(root, node, node, test_cd);
				}
			}

			for (s32 i = 0; i < max_tracked_allocs; ++i)
			{
				if (allocations[i] != NULL)
				{
					test_node* p = allocations[i];
					xrbnode* f;
					if (xtree_remove(root, f, p, test_cd))
					{
						node_allocator.deallocate(p);
						allocations[i] = NULL;
					}
				}
			}

			xrbnode* node_to_destroy = NULL;
			xrbnode* iterator = NULL;
			do
			{
				node_to_destroy = xtree_clear(root, iterator);
			} while (node_to_destroy != NULL);

			node_allocator.deallocate(root);
		}

	}
}
UNITTEST_SUITE_END
