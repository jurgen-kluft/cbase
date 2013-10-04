#include "xbase\x_tree.h"
#include "xbase\x_allocator.h"

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
			test_node(u64 id_) : id(id_) { clear(); }
			u64		id;
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

	}
}
UNITTEST_SUITE_END
