#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_tree.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(tree)
{
    UNITTEST_FIXTURE(main)
    {
        struct item_t : public ntree::node_t
        {
            void* key;
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        struct data_t
        {
            ntree::node_t* m_root;
            alloc_t*       m_allocator;
            s32            m_size;

            inline s32 size() const { return m_size; }

            void        set_key(ntree::node_t* node, void* item) { ((item_t*)node)->key = item; }
            void*       get_key(ntree::node_t* node) const { return ((item_t*)node)->key; }
            void const* get_key(ntree::node_t const* node) const { return ((item_t*)node)->key; }

            void deallocate_node(ntree::node_t* node)
            {
                g_destruct(m_allocator, node);
                m_size--;
            }
        };

        static data_t s_tree;

        static ntree::node_t* s_allocate_node(void* user_data)
        {
            data_t* data = (data_t*)user_data;
            data->m_size++;
            item_t* item                = g_construct<item_t>(data->m_allocator);
            item->key                   = nullptr;
            item->set_child(ntree::LEFT, nullptr);
            item->set_child(ntree::RIGHT,nullptr);
            return (ntree::node_t*)item;
        }

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

        static void* all[]    = {a, b, c, d, e, f, g, h, i};
        static s32   all_size = sizeof(all) / sizeof(all[0]);

        static void* x = (void*)100;

        s8 compare_key_with_node(void const* key, ntree::node_t const* item)
        {
            ASSERT(key == a || key == b || key == c || key == d || key == e || key == f || key == g || key == h || key == i || key == x);

            int_t inta = (int_t)key;
            int_t intb = (int_t)((item_t*)item)->key;

            ASSERT(intb == (int_t)a || intb == (int_t)b || intb == (int_t)c || intb == (int_t)d || intb == (int_t)e || intb == (int_t)f || intb == (int_t)g || intb == (int_t)h || intb == (int_t)i || intb == (int_t)x);

            if (inta < intb)
                return -1;
            else if (inta > intb)
                return 1;
            return 0;
        }

        s8 compare_node_with_node(ntree::node_t const* nodeA, ntree::node_t const* nodeB)
        {
            int_t a = (int_t)((item_t*)nodeA)->key;
            int_t b = (int_t)((item_t*)nodeB)->key;
            if (a < b)
                return -1;
            else if (a > b)
                return 1;
            return 0;
        }

        s8 compare_key_with_node(void const* itemA, void const* itemB)
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

        UNITTEST_FIXTURE_SETUP()
        {
            s_tree.m_root      = nullptr;
            s_tree.m_allocator = Allocator;
            s_tree.m_size      = 0;
        }
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(tree_node)
        {
            void* key = (void*)1;

            ntree::node_t* node  = s_allocate_node(&s_tree);
            ntree::node_t* left  = s_allocate_node(&s_tree);
            ntree::node_t* right = s_allocate_node(&s_tree);

            ntree::node_t* left_ptr = left;
            node->set_child(ntree::LEFT, left_ptr);

            CHECK_EQUAL(left_ptr, node->get_child(ntree::LEFT));

            ntree::node_t* right_ptr = right;
            node->set_child(ntree::RIGHT, right_ptr);
            CHECK_EQUAL(right_ptr, node->get_child(ntree::RIGHT));

            CHECK_EQUAL(true, node->get_color() == ntree::RED);

            node->set_color(ntree::BLACK);
            CHECK_EQUAL(false, node->get_color() == ntree::RED);
            node->set_color(ntree::RED);
            CHECK_EQUAL(true, node->get_color() == ntree::RED);
            node->set_color(ntree::BLACK);
            CHECK_EQUAL(false, node->get_color() == ntree::RED);
            CHECK_EQUAL(true, node->get_color() == ntree::BLACK);

            left_ptr  = left;
            right_ptr = right;
            node->set_child(ntree::LEFT, left_ptr);
            node->set_child(ntree::RIGHT, right_ptr);
            CHECK_EQUAL(left_ptr, node->get_child(ntree::LEFT));
            CHECK_EQUAL(right_ptr, node->get_child(ntree::RIGHT));
            CHECK_EQUAL(true, node->get_color() == ntree::BLACK);

            node->set_child(ntree::LEFT, left_ptr);
            node->set_child(ntree::RIGHT, right_ptr);

            CHECK_EQUAL(left_ptr, node->get_child(ntree::LEFT));
            CHECK_EQUAL(right_ptr, node->get_child(ntree::RIGHT));

            CHECK_EQUAL(true, node->get_color() == ntree::BLACK);

            s_tree.deallocate_node(node);
            s_tree.deallocate_node(left);
            s_tree.deallocate_node(right);
        }

        UNITTEST_TEST(insert_find_remove)
        {
            item_t temp;
            temp.set_child(0, nullptr);
            temp.set_child(1, nullptr);

            for (int i = 0; i < 100; i++)
            {
                bool           inserted;
                ntree::node_t* inserted_node;
                inserted = ntree::insert(s_tree.m_root, &temp, a, compare_key_with_node, s_allocate_node, &s_tree, inserted_node);
                CHECK_TRUE(inserted);
                s_tree.set_key(inserted_node, a);

                const char* result = nullptr;
                CHECK_TRUE(ntree::validate(s_tree.m_root, result, compare_node_with_node));

                ntree::node_t* found;
                CHECK_EQUAL(true, ntree::find(s_tree.m_root, a, compare_key_with_node, found));

                CHECK_FALSE(ntree::find(s_tree.m_root, x, compare_key_with_node, found));

                ntree::node_t* removed;
                CHECK_TRUE(ntree::remove(s_tree.m_root, &temp, a, compare_key_with_node, removed));
                s_tree.deallocate_node(removed);
            }

            ntree::node_t* remove;
            while (!ntree::clear(s_tree.m_root, remove))
            {
                s_tree.deallocate_node(remove);
            }

            CHECK_EQUAL(0, s_tree.size());
        }

        UNITTEST_TEST(void_tree)
        {
            const char* result = nullptr;

            item_t temp;
            temp.set_child(0, nullptr);
            temp.set_child(1, nullptr);
            temp.key        = nullptr;

            bool           inserted;
            ntree::node_t* inserted_node;

            for (int i = 0; i < all_size; i++)
            {
                inserted = ntree::insert(s_tree.m_root, &temp, all[i], compare_key_with_node, s_allocate_node, &s_tree, inserted_node);
                CHECK_TRUE(inserted);
                s_tree.set_key(inserted_node, all[i]);
                CHECK_TRUE(ntree::validate(s_tree.m_root, result, compare_node_with_node));
            }

            inserted = ntree::insert(s_tree.m_root, &temp, b, compare_key_with_node, s_allocate_node, &s_tree, inserted_node);  // Duplicate insert should fail
            CHECK_FALSE(inserted);

            CHECK_EQUAL(all_size, s_tree.size());

            for (int i = 0; i < all_size; i++)
            {
                ntree::node_t* found;
                CHECK_EQUAL(true, ntree::find(s_tree.m_root, all[i], compare_key_with_node, found));
            }

            ntree::node_t* found;
            CHECK_FALSE(ntree::find(s_tree.m_root, x, compare_key_with_node, found));

            CHECK_EQUAL(all_size, s_tree.size());

            ntree::node_t* remove;
            while (!ntree::clear(s_tree.m_root, remove))
            {
                s_tree.deallocate_node(remove);
            }

            CHECK_EQUAL(0, s_tree.size());
        }

        UNITTEST_TEST(void_tree_iterate_preorder)
        {
            item_t temp;
            temp.set_child(0, nullptr);
            temp.set_child(1, nullptr);

            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree.m_root, &temp, all[i], compare_key_with_node, s_allocate_node, &s_tree, inserted_node));
                s_tree.set_key(inserted_node, all[i]);
            }

            ntree::iterator_t iterator;

            s32                  round      = 0;
            void*                preorder[] = {d, b, a, c, f, e, h, g, i};
            ntree::node_t const* data;
            while (iterator.preorder(s_tree.m_root, ntree::LEFT, data))
            {
                CHECK_EQUAL(preorder[round++], s_tree.get_key(data));
            }
            CHECK_EQUAL(9, round);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree.m_root, remove))
            {
                s_tree.deallocate_node(remove);
            }
        }

        UNITTEST_TEST(void_tree_iterate_sortorder)
        {
            item_t temp;
            temp.set_child(0, nullptr);
            temp.set_child(1, nullptr);

            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree.m_root, &temp, all[i], compare_key_with_node, s_allocate_node, &s_tree, inserted_node));
                s_tree.set_key(inserted_node, all[i]);
            }

            ntree::iterator_t iterator;

            s32                  round       = 0;
            void*                sortorder[] = {a, b, c, d, e, f, g, h, i};
            ntree::node_t const* data;
            while (iterator.sortorder(s_tree.m_root, ntree::LEFT, data))
            {
                CHECK_EQUAL(sortorder[round++], s_tree.get_key(data));
            }
            CHECK_EQUAL(9, round);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree.m_root, remove))
            {
                s_tree.deallocate_node(remove);
            }
        }

        UNITTEST_TEST(void_tree_iterate_sortorder_backwards)
        {
            item_t temp;
            temp.set_child(0, nullptr);
            temp.set_child(1, nullptr);

            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree.m_root, &temp, all[i], compare_key_with_node, s_allocate_node, &s_tree, inserted_node));
                s_tree.set_key(inserted_node, all[i]);
            }

            ntree::iterator_t iterator;

            s32                  round       = 0;
            void*                sortorder[] = {i, h, g, f, e, d, c, b, a};
            ntree::node_t const* data;
            while (iterator.sortorder(s_tree.m_root, ntree::RIGHT, data))
            {
                CHECK_EQUAL(sortorder[round++], s_tree.get_key(data));
            }
            CHECK_EQUAL(9, round);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree.m_root, remove))
            {
                s_tree.deallocate_node(remove);
            }
        }

        UNITTEST_TEST(void_tree_iterate_postorder)
        {
            item_t temp;
            temp.set_child(0, nullptr);
            temp.set_child(1, nullptr);

            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree.m_root, &temp, all[i], compare_key_with_node, s_allocate_node, &s_tree, inserted_node));
                s_tree.set_key(inserted_node, all[i]);
            }

            ntree::iterator_t iterator;

            s32   round       = 0;
            void* postorder[] = {a, c, b, e, g, i, h, f, d};

            ntree::node_t const* data;
            while (iterator.postorder(s_tree.m_root, ntree::LEFT, data))
            {
                CHECK_EQUAL(postorder[round++], s_tree.get_key(data));
            }
            CHECK_EQUAL(9, round);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree.m_root, remove))
            {
                s_tree.deallocate_node(remove);
            }
        }

        UNITTEST_TEST(void_tree_search)
        {
            item_t temp;
            temp.set_child(0, nullptr);
            temp.set_child(1, nullptr);

            ntree::node_t* inserted_node;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree.m_root, &temp, all[i], compare_key_with_node, s_allocate_node, &s_tree, inserted_node));
                s_tree.set_key(inserted_node, all[i]);
            }

            ntree::iterator_t iterator;

            s32                  dir = ntree::LEFT;
            ntree::node_t const* data = nullptr;
            void*                find = f;
            while (iterator.traverse(s_tree.m_root, dir, data))
            {
                s32 const c = compare_key_with_node(find, data);
                if (c == 0)
                    break;
                dir = iterator.getdir(c);
            }
            CHECK_EQUAL(0, compare_key_with_node(find, data));

            ntree::node_t* remove;
            while (!ntree::clear(s_tree.m_root, remove))
            {
                s_tree.deallocate_node(remove);
            }
        }

        UNITTEST_TEST(s32_tree)
        {
            const char* result = nullptr;

            ntree::node_t* inserted_node;

            item_t temp;
            temp.set_child(0, nullptr);
            temp.set_child(1, nullptr);

            for (int i = 0; i < all_size; i++)
            {
                CHECK_TRUE(ntree::insert(s_tree.m_root, &temp, all[i], compare_key_with_node, s_allocate_node, &s_tree, inserted_node));
                s_tree.set_key(inserted_node, all[i]);
            }

            CHECK_EQUAL(all_size, s_tree.size());

            ntree::node_t* found;
            for (int i = 0; i < all_size; i++)
            {
                CHECK_EQUAL(true, ntree::find(s_tree.m_root, all[i], compare_key_with_node, found));
                CHECK_EQUAL(all[i], s_tree.get_key(found));
            }

            CHECK_FALSE(ntree::find(s_tree.m_root, x, compare_key_with_node, found));
            CHECK_NULL(found);

            ntree::node_t* remove;
            while (!ntree::clear(s_tree.m_root, remove))
            {
                s_tree.deallocate_node(remove);
            }

            CHECK_EQUAL(0, s_tree.size());
        }
    }
}
UNITTEST_SUITE_END
