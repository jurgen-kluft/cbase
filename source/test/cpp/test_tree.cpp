#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_tree.h"
#include "cbase/test_allocator.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_tree)
{
    UNITTEST_FIXTURE(main)
    {
        static ntree::tree_t s_tree;

        // the values here should not touch bit 0
        static void* a = (void*)2;
        static void* b = (void*)4;
        static void* c = (void*)6;
        static void* d = (void*)8;
        static void* e = (void*)10;
        static void* f = (void*)12;
        static void* g = (void*)14;
        static void* h = (void*)16;
        static void* i = (void*)18;

        static void* all[] = {a, b, c, d, e, f, g, h, i};
        static s32   all_size = sizeof(all) / sizeof(all[0]);

        static void* x = (void*)100;

        s8 compare_key_with_item(void const* key, void const* item)
        {
            ASSERT(key == a || key == b || key == c || key == d || key == e || key == f || key == g || key == h || key == i || key == x);
            ASSERT(item == a || item == b || item == c || item == d || item == e || item == f || item == g || item == h || item == i || item == x);

            int_t a = (int_t)key;
            int_t b = (int_t)item;
            if (a < b)
                return -1;
            else if (a > b)
                return 1;
            return 0;
        }

        s8 compare_items(void const* itemA, void const* itemB)
        {
            ASSERT(itemA == a || itemA == b || itemA == c || itemA == d || itemA == e || itemA == f || itemA == g || itemA == h || itemA == i || itemA == x);
            ASSERT(itemB == a || itemB == b || itemB == c || itemB == d || itemB == e || itemB == f || itemB == g || itemB == h || itemB == i || itemB == x);

            int_t a = (int_t)itemA;
            int_t b = (int_t)itemB;
            if (a < b)
                return -1;
            else if (a > b)
                return 1;
            return 0;
        }

        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() { ntree::setup_tree(Allocator, s_tree); }
        UNITTEST_FIXTURE_TEARDOWN() { ntree::teardown_tree(Allocator, s_tree); }

        UNITTEST_TEST(tree_node)
        {
            void* key = (void*)1;

            ntree::node_t* node  = s_tree.v_new_node();
            ntree::node_t* left  = s_tree.v_new_node();
            ntree::node_t* right = s_tree.v_new_node();

            ntree::node_t* left_ptr = left;
            s_tree.v_set_node(node, ntree::LEFT, left_ptr);

            CHECK_EQUAL(left_ptr, s_tree.v_get_node(node, ntree::LEFT));

            ntree::node_t* right_ptr = right;
            s_tree.v_set_node(node, ntree::RIGHT, right_ptr);
            CHECK_EQUAL(right_ptr, s_tree.v_get_node(node, ntree::RIGHT));

            CHECK_EQUAL(true, s_tree.v_get_color(node) == ntree::RED);

            s_tree.v_set_color(node, ntree::BLACK);
            CHECK_EQUAL(false, s_tree.v_get_color(node) == ntree::RED);
            s_tree.v_set_color(node, ntree::RED);
            CHECK_EQUAL(true, s_tree.v_get_color(node) == ntree::RED);
            s_tree.v_set_color(node, ntree::BLACK);
            CHECK_EQUAL(false, s_tree.v_get_color(node) == ntree::RED);
            CHECK_EQUAL(true, s_tree.v_get_color(node) == ntree::BLACK);

            left_ptr  = left;
            right_ptr = right;
            s_tree.v_set_node(node, ntree::LEFT, left_ptr);
            s_tree.v_set_node(node, ntree::RIGHT, right_ptr);
            CHECK_EQUAL(left_ptr, s_tree.v_get_node(node, ntree::LEFT));
            CHECK_EQUAL(right_ptr, s_tree.v_get_node(node, ntree::RIGHT));
            CHECK_EQUAL(true, s_tree.v_get_color(node) == ntree::BLACK);

            s_tree.v_set_node(node, ntree::LEFT, left_ptr);
            s_tree.v_set_node(node, ntree::RIGHT, right_ptr);

            CHECK_EQUAL(left_ptr, s_tree.v_get_node(node, ntree::LEFT));
            CHECK_EQUAL(right_ptr, s_tree.v_get_node(node, ntree::RIGHT));

            CHECK_EQUAL(true, s_tree.v_get_color(node) == ntree::BLACK);

            s_tree.v_del_node(node);
            s_tree.v_del_node(left);
            s_tree.v_del_node(right);
        }

        UNITTEST_TEST(insert_find_remove)
        {
            for (int i = 0; i < 100; i++)
            {
                bool           inserted;
                ntree::node_t* inserted_node;
                inserted = ntree::insert(s_tree, a, compare_key_with_item, inserted_node);
                CHECK_TRUE(inserted);
                s_tree.v_set_item(inserted_node, a);

                const char* result = nullptr;
                CHECK_TRUE(ntree::validate(s_tree, result, compare_items));

                ntree::node_t* found;
                CHECK_EQUAL(true, ntree::find(s_tree, a, compare_key_with_item, found));

                CHECK_FALSE(ntree::find(s_tree, x, compare_items, found));

                ntree::node_t* removed;
                CHECK_TRUE(ntree::remove(s_tree, a, compare_items, removed));
                s_tree.v_del_node(removed);
            }

            ntree::node_t* remove;
            while (!ntree::clear(s_tree, remove))
            {
                s_tree.v_del_node(remove);
            }

            CHECK_EQUAL(0, ntree::size(s_tree));
        }

        UNITTEST_TEST(void_tree)
        {
            const char* result = nullptr;

            bool           inserted;
            ntree::node_t* inserted_node;

            for (int i = 0; i < all_size; i++)
            {
                inserted = ntree::insert(s_tree, all[i], compare_key_with_item, inserted_node);
                CHECK_TRUE(inserted);
                s_tree.v_set_item(inserted_node, all[i]);
                CHECK_TRUE(ntree::validate(s_tree, result, compare_items));
            }

            inserted = ntree::insert(s_tree, b, compare_key_with_item, inserted_node);  // Duplicate insert should fail
            CHECK_FALSE(inserted);

            CHECK_EQUAL(all_size, ntree::size(s_tree));

            for (int i = 0; i < all_size; i++)
            {
                ntree::node_t* found;
                CHECK_EQUAL(true, ntree::find(s_tree, all[i], compare_items, found));
            }

            ntree::node_t* found;
            CHECK_FALSE(ntree::find(s_tree, x, compare_items, found));

            CHECK_EQUAL(all_size, ntree::size(s_tree));

            ntree::node_t* remove;
            while (!ntree::clear(s_tree, remove))
            {
                s_tree.v_del_node(remove);
            }

            CHECK_EQUAL(0, ntree::size(s_tree));
        }

        UNITTEST_TEST(void_tree_iterate_preorder)
        {
            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree, all[i], compare_key_with_item, inserted_node));
                s_tree.v_set_item(inserted_node, all[i]);
            }

            ntree::iterator_t iterator = ntree::iterate();

            s32   round      = 0;
            void* preorder[] = {d, b, a, c, f, e, h, g, i};
            void* data;
            while (iterator.preorder(s_tree, ntree::LEFT, data))
            {
                CHECK_EQUAL(preorder[round++], data);
            }
            CHECK_EQUAL(9, round);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree, remove))
            {
                s_tree.v_del_node(remove);
            }
        }

        UNITTEST_TEST(void_tree_iterate_sortorder)
        {
            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree, all[i], compare_key_with_item, inserted_node));
                s_tree.v_set_item(inserted_node, all[i]);
            }

            ntree::iterator_t iterator = ntree::iterate();

            s32   round       = 0;
            void* sortorder[] = {a, b, c, d, e, f, g, h, i};
            void* data;
            while (iterator.sortorder(s_tree, ntree::LEFT, data))
            {
                CHECK_EQUAL(sortorder[round++], data);
            }
            CHECK_EQUAL(9, round);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree, remove))
            {
                s_tree.v_del_node(remove);
            }
        }

        UNITTEST_TEST(void_tree_iterate_sortorder_backwards)
        {
            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree, all[i], compare_key_with_item, inserted_node));
                s_tree.v_set_item(inserted_node, all[i]);
            }

            ntree::iterator_t iterator = ntree::iterate();

            s32   round       = 0;
            void* sortorder[] = {i, h, g, f, e, d, c, b, a};
            void* data;
            while (iterator.sortorder(s_tree, ntree::RIGHT, data))
            {
                CHECK_EQUAL(sortorder[round++], data);
            }
            CHECK_EQUAL(9, round);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree, remove))
            {
                s_tree.v_del_node(remove);
            }
        }

        UNITTEST_TEST(void_tree_iterate_postorder)
        {
            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree, all[i], compare_key_with_item, inserted_node));
                s_tree.v_set_item(inserted_node, all[i]);
            }

            ntree::iterator_t iterator = ntree::iterate();

            s32   round       = 0;
            void* postorder[] = {a, c, b, e, g, i, h, f, d};

            void* data;
            while (iterator.postorder(s_tree, ntree::LEFT, data))
            {
                CHECK_EQUAL(postorder[round++], data);
            }
            CHECK_EQUAL(9, round);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree, remove))
            {
                s_tree.v_del_node(remove);
            }
        }

        UNITTEST_TEST(void_tree_search)
        {
            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree, all[i], compare_key_with_item, inserted_node));
                s_tree.v_set_item(inserted_node, all[i]);
            }

            ntree::iterator_t iterator = ntree::iterate();

            s32   dir = ntree::LEFT;
            void* data;
            void* find = f;
            while (iterator.traverse(s_tree, dir, data))
            {
                s32 const c = compare_items(find, data);
                if (c == 0)
                    break;
                dir = iterator.getdir(c);
            }
            CHECK_EQUAL(0, compare_items(data, find));

            ntree::node_t* remove;
            while (!ntree::clear(s_tree, remove))
            {
                s_tree.v_del_node(remove);
            }
        }

        UNITTEST_TEST(s32_tree)
        {
            const char* result = nullptr;

            bool           inserted;
            ntree::node_t* inserted_node;

            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree, all[i], compare_key_with_item, inserted_node));
                s_tree.v_set_item(inserted_node, all[i]);
            }

            CHECK_EQUAL(all_size, ntree::size(s_tree));

            ntree::node_t* found;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_EQUAL(true, ntree::find(s_tree, all[i], compare_items, found));
                CHECK_EQUAL(all[i], s_tree.v_get_item(found));
            }

            CHECK_FALSE(ntree::find(s_tree, x, compare_items, found));
            CHECK_NULL(found);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree, remove))
            {
                s_tree.v_del_node(remove);
            }

            CHECK_EQUAL(0, ntree::size(s_tree));
        }
    }
}
UNITTEST_SUITE_END
