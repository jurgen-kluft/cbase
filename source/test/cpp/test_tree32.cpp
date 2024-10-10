#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_tree32.h"

#include "cunittest/cunittest.h"

#define TEST_ITERATOR

namespace ncore
{
    struct rbnode_t
    {
        rbnode_t()
            : m_item(nullptr)
            , m_color(ntree32::RED)
        {
            m_chld[ntree32::LEFT]  = nullptr;
            m_chld[ntree32::RIGHT] = nullptr;
        }

        void set_right(rbnode_t *node) { m_chld[ntree32::RIGHT] = node; }
        void set_left(rbnode_t *node) { m_chld[ntree32::LEFT] = node; }
        void set_child(s32 side, rbnode_t *node)
        {
            ASSERT(side == ntree32::LEFT || side == ntree32::RIGHT);
            m_chld[side] = node;
        }

        void set_red() { m_color = ntree32::RED; }
        void set_black() { m_color = ntree32::BLACK; }

        rbnode_t *get_right() { return m_chld[ntree32::RIGHT]; }
        rbnode_t *get_left() { return m_chld[ntree32::LEFT]; }
        rbnode_t *get_child(s32 side)
        {
            ASSERT(side == ntree32::LEFT || side == ntree32::RIGHT);
            return m_chld[side];
        }

        rbnode_t *m_chld[2];
        void     *m_item;
        s8        m_color;
    };

    // Insertion and Deletion are Top-Down
    //
    // X:  Current Node
    // P:  Parent Node         (Parent of X)
    // T:  X's Sibling Node
    // G:  Grand Parent Node   (Parent of P)
    // Q:  Great Parent Node   (Parent of Grand Parent)
    //
    // Y:  X's left child
    // Z:  X's right child
    //
    // sX: side of X           (if X is left then sX=0, right sX=1)
    // sP: side of P
    // sG: side of GD
    // sN: side of Next X      (Side of Next Current Node)
    //
    // cX: color of X

    void rb_init_node(rbnode_t *n)
    {
        n->m_item                 = (void *)nullptr;
        n->m_chld[ntree32::LEFT]  = nullptr;
        n->m_chld[ntree32::RIGHT] = nullptr;
        n->m_color                = ntree32::RED;
    }

    // show what a single red-black tree rotation does in ASCII art
    //
    // Left
    //     |            |
    //     x            y
    //    / \          / \
    //   a   y   ->   x   c
    //      / \      / \
    //     b   c    a   b
    //
    // Right
    //     |            |
    //     x            y
    //    / \          / \
    //   y   c   ->   a   x
    //  / \              / \
    // a   b            b   c
    //

    static inline rbnode_t *rotate_single(rbnode_t *node, s32 dir)
    {
        rbnode_t *save        = node->m_chld[1 - dir];
        node->m_chld[1 - dir] = save->m_chld[dir];
        save->m_chld[dir]     = node;
        node->m_color         = ntree32::RED;
        save->m_color         = ntree32::BLACK;
        return save;
    }

    static inline rbnode_t *rotate_single_track_parent(rbnode_t *node, s32 dir, rbnode_t *fn, rbnode_t *&fp)
    {
        rbnode_t *save        = node->m_chld[1 - dir];
        node->m_chld[1 - dir] = save->m_chld[dir];
        save->m_chld[dir]     = node;
        node->m_color         = ntree32::RED;
        save->m_color         = ntree32::BLACK;

        if (fn == node)
            fp = save;
        else if (fn == node->m_chld[1 - dir])  // never triggered
            fp = node;

        return save;
    }

    // show what a double red-black tree rotation does in ASCII art
    //
    // Left
    //     |            |
    //     x            z
    //    / \          / \
    //   a   y   ->   x   y
    //      / \      / \ / \
    //     b   z    a  b c  d
    //        / \
    //       c   d
    //
    // Right
    //     |            |
    //     x            z
    //    / \          / \
    //   y   c   ->   y   x
    //  / \          / \ / \
    // a   z        a  b c  d
    //    / \
    //   b   d
    //
    static inline rbnode_t *rotate_double(rbnode_t *node, s32 dir)
    {
        node->m_chld[1 - dir] = rotate_single(node->m_chld[1 - dir], 1 - dir);
        return rotate_single(node, dir);
    }

    static inline rbnode_t *rotate_double_track_parent(rbnode_t *node, s32 dir, rbnode_t *fn, rbnode_t *&fp)
    {
        node->m_chld[1 - dir] = rotate_single_track_parent(node->m_chld[1 - dir], 1 - dir, fn, fp);
        if (fn == node->m_chld[1 - dir])  // never triggered
            fp = node;
        rbnode_t *save = rotate_single_track_parent(node, dir, fn, fp);
        if (fn == node)
            fp = save;
        return save;
    }

    static inline s32 is_red(rbnode_t *node) { return node != nullptr && node->m_color == ntree32::RED; }

    // validate the tree (return violation description in 'result'), also returns black height
    static s32 rb_validate(rbnode_t *root, s32 (*compare)(void *a, void *b), const char *&result)
    {
        if (root == nullptr)
        {
            return 1;
        }
        else
        {
            rbnode_t *ln = root->get_left();
            rbnode_t *rn = root->get_right();

            // Consecutive red links
            if (is_red(root))
            {
                if (is_red(ln) || is_red(rn))
                {
                    result = "Red violation";
                    return 0;
                }
            }

            const s32 lh = rb_validate(ln, compare, result);
            const s32 rh = rb_validate(rn, compare, result);

            // Invalid binary search tree
            if ((ln != nullptr && compare(ln->m_item, root->m_item) >= 0) || (rn != nullptr && compare(rn->m_item, root->m_item) <= 0))
            {
                result = "Binary tree violation";
                return 0;
            }

            if (lh != 0 && rh != 0 && lh != rh)  // Black height mismatch
            {
                result = "Black violation";
                return 0;
            }

            if (lh != 0 && rh != 0)  // Only count black links
            {
                return is_red(root) ? lh : lh + 1;
            }
        }
        return 0;
    }

    s32 rb_insert(rbnode_t *&root, s32 &count, void *item, s32 (*compare)(void *a, void *b), rbnode_t *&new_node)
    {
        s32 result = 0;
        if (root == nullptr)
        {
            // We have an empty tree; attach the
            // new node directly to the root
            rb_init_node(new_node);
            root         = new_node;
            new_node     = nullptr;
            root->m_item = item;
            result       = 1;
        }
        else
        {
            rbnode_t  head;   // False tree root
            rbnode_t *g, *t;  // Grandparent & parent
            rbnode_t *p, *n;  // Iterator & parent
            s32       dir = 0, last = 0;

            // Set up our helpers
            t         = &head;
            t->m_item = nullptr;
            t->set_black();  // Color it black
            t->set_right(root);
            t->set_left(nullptr);

            g = p = nullptr;
            n     = root;

            // Search down the tree for a place to insert
            for (;;)
            {
                if (n == nullptr)
                {
                    // Insert a new node at the first null link
                    rb_init_node(new_node);
                    n         = new_node;
                    new_node  = nullptr;
                    n->m_item = item;
                    p->set_child(dir, n);
                    result = 1;
                }
                else if (is_red(n->get_left()) && is_red(n->get_right()))
                {
                    // Simple red violation: color flip
                    n->set_red();
                    n->get_left()->set_black();
                    n->get_right()->set_black();
                }

                if (is_red(n) && is_red(p))
                {
                    // Hard red violation: rotations necessary
                    const s32 dir2 = (t->get_right() == g) ? 1 : 0;

                    if (n == p->get_child(last))
                        t->m_chld[dir2] = rotate_single(g, 1 - last);
                    else
                        t->m_chld[dir2] = rotate_double(g, 1 - last);
                }

                // Stop working if we inserted a node. This
                // check also disallows duplicates in the tree
                last = dir;
                dir  = compare(item, n->m_item);
                if (dir == 0)
                    break;
                dir = ((dir + 1) >> 1);

                // Move the helpers down
                if (g != nullptr)
                    t = g;

                g = p, p = n;
                n = n->get_child(dir);
            }

            // Update the root (it may be different)
            root = head.get_right();
        }

        // Make the root black for simplified logic
        root->set_black();
        count += result;
        return result;
    }

    rbnode_t *rb_remove(rbnode_t *&root, s32 &count, void *item, s32 (*compare)(void *a, void *b))
    {
        if (root == nullptr)
            return nullptr;

        rbnode_t  head;           // False tree root
        rbnode_t *n, *p, *g;      // Helpers, node, parent, grandparent
        rbnode_t *fn  = nullptr;  // Found node
        rbnode_t *fp  = nullptr;  // Found node parent
        s32       dir = 1;

        // Set up our helpers
        g = p = nullptr;

        n         = &head;
        n->m_item = nullptr;
        n->set_black();  // Color it black
        n->set_right(root);
        n->set_left(nullptr);

        // Search and push a red node down
        // to fix red violations as we go
        while (n->get_child(dir) != nullptr)
        {
            const s32 last = dir;

            // Move the helpers down
            g = p, p = n;
            n   = n->get_child(dir);
            dir = compare(item, n->m_item);

            // Save the node with matching data and keep
            // going; we'll do removal tasks at the end
            if (dir == 0)
            {
                fn = n;
                fp = p;
            }
            dir = ((dir + 1) >> 1);

            /* Push the red node down with rotations and color flips */
            if (!is_red(n) && !is_red(n->get_child(dir)))
            {
                if (is_red(n->get_child(1 - dir)))
                {
                    rbnode_t *r = rotate_single_track_parent(n, dir, fn, fp);
                    p->set_child(last, r);
                    if (fn == r)  // never triggered
                        fp = p;
                    p = r;
                }
                else if (!is_red(n->get_child(1 - dir)))
                {
                    rbnode_t *s = p->get_child(1 - last);
                    if (s != nullptr)
                    {
                        if (!is_red(s->get_child(1 - last)) && !is_red(s->get_child(last)))
                        {
                            // Color flip
                            p->set_black();
                            s->set_red();
                            n->set_red();
                        }
                        else
                        {
                            const s32 dir2 = g->get_right() == p ? 1 : 0;
                            if (is_red(s->get_child(last)))
                            {
                                rbnode_t *r = rotate_double_track_parent(p, last, fn, fp);
                                g->set_child(dir2, r);
                                if (fn == r)  // never triggered
                                    fp = g;
                            }
                            else if (is_red(s->get_child(1 - last)))
                            {
                                rbnode_t *r = rotate_single_track_parent(p, last, fn, fp);
                                g->set_child(dir2, r);
                                if (fn == r)  // never triggered
                                    fp = g;
                            }

                            // Ensure correct coloring
                            n->set_red();
                            g->get_child(dir2)->set_red();

                            g->get_child(dir2)->get_left()->set_black();
                            g->get_child(dir2)->get_right()->set_black();
                        }
                    }
                }
            }
        }

        // Update the root (it may be different)
        root = head.get_right();

        // Replace and remove the saved node
        if (fn != nullptr)
        {
            ASSERT(fp->get_right() == fn || fp->get_left() == fn);

            void *item = fn->m_item;

            ASSERT(p->get_right() == n || p->get_left() == n);
            rbnode_t *child1 = n->get_child(n->get_left() == nullptr);
            p->set_child(p->get_right() == n, child1);

            if (fn != n)
            {
                ASSERT(fp != p);

                // swap 'n' and 'fn', we want to remove the node that was holding 'item'
                fp->set_child(fp->get_right() == fn, n);
                n->set_child(ntree32::LEFT, fn->get_left());
                n->set_child(ntree32::RIGHT, fn->get_right());
                n->m_color = fn->m_color;

                if (root == fn)
                    root = n;
            }
            else
            {
                if (root == fn)
                    root = nullptr;
            }

            rb_init_node(fn);
            fn->m_item = item;
        }

        // Make the root black for simplified logic
        if (root != nullptr)
            root->set_black();

        count -= 1;
        return fn;
    }

    rbnode_t *rb_find(rbnode_t *root, void *item, s32 (*compare)(void *a, void *b))
    {
        rbnode_t *iter = root;
        while (iter != nullptr)
        {
            const s32 cmpRet = compare(item, iter->m_item);
            if (cmpRet == 0)
                break;
            iter = iter->m_chld[(cmpRet + 1) >> 1];
        }
        return iter;
    }

}  // namespace ncore

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_tree2)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        struct XorRandom
        {
            u64 s0, s1;
            inline XorRandom(u64 seed)
                : s0(seed)
                , s1(0)
            {
                next();
                next();
            }

            inline void seed(u64 seed)
            {
                s0 = seed;
                s1 = 0;
                next();
                next();
            }

            inline u64 next(void)
            {
                u64 ss1    = s0;
                u64 ss0    = s1;
                u64 result = ss0 + ss1;
                s0         = ss0;
                ss1 ^= ss1 << 23;
                s1 = ss1 ^ ss0 ^ (ss1 >> 18) ^ (ss0 >> 5);
                return result;
            }
        };
        static XorRandom s_rand(0x1234567890abcdef);

        static s32 compare(void *aa, void *bb)
        {
            s64 a = (s64)aa;
            s64 b = (s64)bb;
            if (a < b)
                return -1;
            if (a > b)
                return 1;
            return 0;
        }

        UNITTEST_TEST(insert_remove)
        {
            const s32 num_nodes = 2048;
            rbnode_t *nodes     = (rbnode_t *)Allocator->allocate(num_nodes * sizeof(rbnode_t));
            s64      *values    = (s64 *)Allocator->allocate(num_nodes * sizeof(s64));

            s_rand.seed(0x1234567890abcdef);
            for (s32 i = 0; i < num_nodes; ++i)
                values[i] = s_rand.next();

            rbnode_t *root  = nullptr;
            s32       count = 0;
            for (s32 i = 0; i < num_nodes; ++i)
            {
                rbnode_t *node = &nodes[i];
                rb_insert(root, count, (void *)(ptr_t)values[i], compare, node);
                CHECK_NOT_NULL(root);
                CHECK_NULL(node);
                rbnode_t *removed = rb_remove(root, count, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(removed);
                CHECK_EQUAL((ptr_t)removed->m_item, values[i]);
                CHECK_EQUAL(&nodes[i], removed);
            }

            Allocator->deallocate(nodes);
            Allocator->deallocate(values);
        }

        UNITTEST_TEST(insert_find)
        {
            const s32 num_nodes = 2048;
            rbnode_t *nodes     = (rbnode_t *)Allocator->allocate(num_nodes * sizeof(rbnode_t));
            s64      *values    = (s64 *)Allocator->allocate(num_nodes * sizeof(s64));

            s_rand.seed(0x1234567890abcdef);
            for (s32 i = 0; i < num_nodes; ++i)
                values[i] = s_rand.next();

            rbnode_t *root  = nullptr;
            s32       count = 0;
            for (s32 i = 0; i < num_nodes; ++i)
            {
                rbnode_t *node = &nodes[i];
                rb_insert(root, count, (void *)(ptr_t)values[i], compare, node);
                CHECK_NOT_NULL(root);
                CHECK_NULL(node);
            }

            for (s32 i = 0; i < num_nodes; ++i)
            {
                rbnode_t *found = rb_find(root, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(found);
                CHECK_EQUAL((ptr_t)found->m_item, values[i]);
            }

            Allocator->deallocate(nodes);
            Allocator->deallocate(values);
        }

        static void RandomShuffle(s64 * v, s32 size, u64 seed)
        {
            XorRandom rng(seed);
            for (s32 i = 0; i < size; ++i)
            {
                const s32 j = i + ((s32)(rng.next() & 0x7fffffff) % (size - i));
                const s64 t = v[i];
                v[i]        = v[j];
                v[j]        = t;
            }
        }

        UNITTEST_TEST(insert_remove_find_sorted_ascending)
        {
            const s32 num_nodes = 32768;
            rbnode_t *nodes     = (rbnode_t *)Allocator->allocate(num_nodes * sizeof(rbnode_t));
            s64      *values    = (s64 *)Allocator->allocate(num_nodes * sizeof(s64));

            for (s32 i = 0; i < num_nodes; ++i)
            {
                values[i] = 1000 + i * 9;
            }

            rbnode_t *root  = nullptr;
            s32       count = 0;
            for (s32 i = 0; i < num_nodes; ++i)
            {
                rbnode_t *node = &nodes[i];
                rb_insert(root, count, (void *)(ptr_t)values[i], compare, node);
                CHECK_NOT_NULL(root);
                CHECK_NULL(node);
            }

            const char *error = nullptr;
            rb_validate(root, compare, error);
            CHECK_NULL(error);

            // remove all half of the values
            for (s32 i = 0; i < (num_nodes / 2); ++i)
            {
                rbnode_t *found = rb_find(root, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(found);
                CHECK_EQUAL((ptr_t)found->m_item, values[i]);
                rbnode_t *removed = rb_remove(root, count, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(removed);
                CHECK_EQUAL((ptr_t)removed->m_item, values[i]);
                CHECK_EQUAL(found, removed);
            }

            rb_validate(root, compare, error);
            CHECK_NULL(error);

            for (s32 i = (num_nodes / 2); i < num_nodes; ++i)
            {
                rbnode_t *found = rb_find(root, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(found);
                CHECK_EQUAL((ptr_t)found->m_item, values[i]);
            }

            Allocator->deallocate(nodes);
            Allocator->deallocate(values);
        }

        UNITTEST_TEST(insert_remove_find_sorted_descending)
        {
            const s32 num_nodes = 32768;
            rbnode_t *nodes     = (rbnode_t *)Allocator->allocate(num_nodes * sizeof(rbnode_t));
            s64      *values    = (s64 *)Allocator->allocate(num_nodes * sizeof(s64));

            for (s32 i = 0; i < num_nodes; ++i)
            {
                values[i] = (num_nodes * 32) - (i * 9);
            }

            rbnode_t *root  = nullptr;
            s32       count = 0;
            for (s32 i = 0; i < num_nodes; ++i)
            {
                rbnode_t *node = &nodes[i];
                rb_insert(root, count, (void *)(ptr_t)values[i], compare, node);
                CHECK_NOT_NULL(root);
                CHECK_NULL(node);
            }

            const char *error = nullptr;
            rb_validate(root, compare, error);
            CHECK_NULL(error);

            // remove all half of the values
            for (s32 i = 0; i < (num_nodes / 2); ++i)
            {
                rbnode_t *found = rb_find(root, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(found);
                CHECK_EQUAL((ptr_t)found->m_item, values[i]);
                rbnode_t *removed = rb_remove(root, count, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(removed);
                CHECK_EQUAL((ptr_t)removed->m_item, values[i]);
                CHECK_EQUAL(found, removed);
            }

            rb_validate(root, compare, error);
            CHECK_NULL(error);

            for (s32 i = (num_nodes / 2); i < num_nodes; ++i)
            {
                rbnode_t *found = rb_find(root, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(found);
                CHECK_EQUAL((ptr_t)found->m_item, values[i]);
            }

            Allocator->deallocate(nodes);
            Allocator->deallocate(values);
        }

        UNITTEST_TEST(insert_remove_find)
        {
            const s32 num_nodes = 32768;
            rbnode_t *nodes     = (rbnode_t *)Allocator->allocate(num_nodes * sizeof(rbnode_t));
            s64      *values    = (s64 *)Allocator->allocate(num_nodes * sizeof(s64));

            s_rand.seed(0x1234567890abcdef);
            for (s32 i = 0; i < num_nodes; ++i)
            {
                // make sure we have no duplicates
                s64 r = s_rand.next();
                for (s32 j = 0; j < i; ++j)
                {
                    if (values[j] == r)
                    {
                        r = s_rand.next();
                        j = -1;
                    }
                }
                values[i] = r;
            }

            rbnode_t *root  = nullptr;
            s32       count = 0;
            for (s32 i = 0; i < num_nodes; ++i)
            {
                rbnode_t *node = &nodes[i];
                rb_insert(root, count, (void *)(ptr_t)values[i], compare, node);
                CHECK_NOT_NULL(root);
                CHECK_NULL(node);
            }

            RandomShuffle(values, num_nodes, 0x1234567890abcdef);

            const char *error = nullptr;
            rb_validate(root, compare, error);
            CHECK_NULL(error);

            // remove all
            for (s32 i = 0; i < num_nodes; ++i)
            {
                rbnode_t *found = rb_find(root, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(found);
                CHECK_EQUAL((ptr_t)found->m_item, values[i]);
                rbnode_t *removed = rb_remove(root, count, (void *)(ptr_t)(values[i]), compare);
                CHECK_NOT_NULL(removed);
                CHECK_EQUAL((ptr_t)removed->m_item, values[i]);
                CHECK_EQUAL(found, removed);
            }

            Allocator->deallocate(nodes);
            Allocator->deallocate(values);
        }
    }

    UNITTEST_FIXTURE(tree2)
    {
        UNITTEST_ALLOCATOR;

        const s32  c_find_slot = 62;
        const s32  c_temp_slot = 63;
        const s32  c_num_keys  = 62;
        static s32 s_find[c_num_keys + 2];
        static s32 s_keys[c_num_keys + 2];  // +2 for 'find' and 'temp' slots

        static void s_init_keys()
        {
            for (s32 i = 0; i < c_num_keys; ++i)
            {
                s_find[i] = 33 + i * 33;
                s_keys[i] = -1;
            }
        }

        static s8 s_compare_key_and_node(u32 _key, u32 _item, void const *user_data)
        {
            s32 const *keys = (s32 const *)user_data;
            s32        key  = keys[_key];
            s32        item = keys[_item];
            if (key < item)
                return -1;
            else if (key > item)
                return 1;
            else
                return 0;
        }

        static s8 s_compare_node_and_node(u32 _nodeA, u32 _nodeB, void const *user_data)
        {
            s32 const *keys = (s32 const *)user_data;
            s32        key  = keys[_nodeA];
            s32        item = keys[_nodeB];
            if (key < item)
                return -1;
            else if (key > item)
                return 1;
            else
                return 0;
        }

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        const u32        c_max_nodes = 1022;
        ntree32::nnode_t m_nodes[c_max_nodes + 2];

        UNITTEST_TEST(tree_node)
        {
            ntree32::tree_t tree;
            ntree32::setup_tree(tree, m_nodes);
            s_init_keys();

            ntree32::index_t const key = 0;

            ntree32::node_t node  = tree.new_node();
            ntree32::node_t left  = tree.new_node();
            ntree32::node_t right = tree.new_node();

            ntree32::node_t left_ptr = left;
            tree.set_node(node, ntree32::LEFT, left_ptr);

            CHECK_EQUAL(left_ptr, tree.get_node(node, ntree32::LEFT));

            ntree32::node_t right_ptr = right;
            tree.set_node(node, ntree32::RIGHT, right_ptr);
            CHECK_EQUAL(right_ptr, tree.get_node(node, ntree32::RIGHT));

            CHECK_EQUAL(true, tree.get_color(node) == ntree32::RED);

            tree.set_color(node, ntree32::BLACK);
            CHECK_EQUAL(false, tree.get_color(node) == ntree32::RED);
            tree.set_color(node, ntree32::RED);
            CHECK_EQUAL(true, tree.get_color(node) == ntree32::RED);
            tree.set_color(node, ntree32::BLACK);
            CHECK_EQUAL(false, tree.get_color(node) == ntree32::RED);
            CHECK_EQUAL(true, tree.get_color(node) == ntree32::BLACK);

            left_ptr  = left;
            right_ptr = right;
            tree.set_node(node, ntree32::LEFT, left_ptr);
            tree.set_node(node, ntree32::RIGHT, right_ptr);
            CHECK_EQUAL(left_ptr, tree.get_node(node, ntree32::LEFT));
            CHECK_EQUAL(right_ptr, tree.get_node(node, ntree32::RIGHT));
            CHECK_EQUAL(true, tree.get_color(node) == ntree32::BLACK);

            tree.set_node(node, ntree32::LEFT, left_ptr);
            tree.set_node(node, ntree32::RIGHT, right_ptr);

            CHECK_EQUAL(left_ptr, tree.get_node(node, ntree32::LEFT));
            CHECK_EQUAL(right_ptr, tree.get_node(node, ntree32::RIGHT));

            CHECK_EQUAL(true, tree.get_color(node) == ntree32::BLACK);

            tree.del_node(node);
            tree.del_node(left);
            tree.del_node(right);

            ntree32::teardown_tree(tree);
        }

        UNITTEST_TEST(insert_remove)
        {
            ntree32::node_t root = ntree32::c_invalid_node;
            ntree32::tree_t tree;
            ntree32::setup_tree(tree, m_nodes);
            s_init_keys();

            for (s32 i = 0; i < c_num_keys; ++i)
            {
                s_keys[c_find_slot] = s_find[i];
                ntree32::node_t inserted;
                CHECK_TRUE(ntree32::insert(tree, root, c_temp_slot, c_find_slot, s_compare_key_and_node, &s_keys, inserted));
                s_keys[inserted] = s_find[i];

                const char *result = nullptr;
                CHECK_TRUE(ntree32::validate(tree, root, result, s_compare_key_and_node, &s_keys));
                ntree32::node_t removed;
                CHECK_TRUE(ntree32::remove(tree, root, c_temp_slot, c_find_slot, s_compare_key_and_node, &s_keys, removed));
                CHECK_NOT_EQUAL(ntree32::c_invalid_node, removed);
                tree.del_node(removed);
            }

            ntree32::node_t node;
            while (!ntree32::clear(tree, root, node)) {}

            ntree32::teardown_tree(tree);
        }

        UNITTEST_TEST(void_tree_iterate_preorder)
        {
            ntree32::node_t root = ntree32::c_invalid_node;
            ntree32::tree_t tree;
            ntree32::setup_tree(tree, m_nodes);
            s_init_keys();

            for (s32 i = 0; i < 9; ++i)
            {
                s_keys[c_find_slot] = s_find[i];
                ntree32::node_t inserted;
                CHECK_TRUE(ntree32::insert(tree, root, c_temp_slot, c_find_slot, s_compare_key_and_node, &s_keys, inserted));
                s_keys[inserted] = s_find[i];
            }

#ifdef TEST_ITERATOR
            ntree32::iterator_t iterator = ntree32::iterate(tree, root);

            s32             round      = 0;
            s32             preorder[] = {3, 1, 0, 2, 5, 4, 7, 6, 8};
            ntree32::node_t node;
            while (iterator.preorder(tree, ntree32::LEFT, node))
            {
                CHECK_EQUAL(s_find[preorder[round++]], s_keys[node]);
            }
            CHECK_EQUAL(9, round);
#endif
            while (!ntree32::clear(tree, root, node)) {}

            ntree32::teardown_tree(tree);
        }

        UNITTEST_TEST(void_tree_iterate_sortorder)
        {
            ntree32::node_t root = ntree32::c_invalid_node;
            ntree32::tree_t tree;
            ntree32::setup_tree(tree, m_nodes);
            s_init_keys();

            for (s32 i = 0; i < c_num_keys; ++i)
            {
                s_keys[c_find_slot] = s_find[i];
                ntree32::node_t inserted;
                CHECK_TRUE(ntree32::insert(tree, root, c_temp_slot, c_find_slot, s_compare_key_and_node, &s_keys, inserted));
                s_keys[inserted] = s_find[i];
            }

#ifdef TEST_ITERATOR
            ntree32::iterator_t iterator = ntree32::iterate(tree, root);

            s32             sortorder = 0;
            ntree32::node_t node;
            while (iterator.sortorder(tree, ntree32::LEFT, node))
            {
                CHECK_EQUAL(s_find[sortorder++], s_keys[node]);
            }
            CHECK_EQUAL(c_num_keys, sortorder);
#endif

            while (!ntree32::clear(tree, root, node)) {}

            ntree32::teardown_tree(tree);
        }

        UNITTEST_TEST(void_tree_iterate_sortorder_backwards)
        {
            ntree32::node_t root = ntree32::c_invalid_node;
            ntree32::tree_t tree;
            ntree32::setup_tree(tree, m_nodes);
            s_init_keys();

            for (s32 i = 0; i < c_num_keys; ++i)
            {
                s_keys[c_find_slot] = s_find[i];
                ntree32::node_t inserted;
                CHECK_TRUE(ntree32::insert(tree, root, c_temp_slot, c_find_slot, s_compare_key_and_node, &s_keys, inserted));
                s_keys[inserted] = s_find[i];
            }

#ifdef TEST_ITERATOR
            ntree32::iterator_t iterator = ntree32::iterate(tree, root);

            s32             sortorder = c_num_keys;
            ntree32::node_t node;
            while (iterator.sortorder(tree, ntree32::RIGHT, node))
            {
                sortorder--;
                CHECK_EQUAL(s_find[sortorder], s_keys[node]);
            }
            CHECK_EQUAL(0, sortorder);
#endif

            while (!ntree32::clear(tree, root, node)) {}

            ntree32::teardown_tree(tree);
        }

        UNITTEST_TEST(void_tree_iterate_postorder)
        {
            ntree32::node_t root = ntree32::c_invalid_node;
            ntree32::tree_t tree;
            ntree32::setup_tree(tree, m_nodes);
            s_init_keys();

            for (s32 i = 0; i < 9; ++i)
            {
                s_keys[c_find_slot] = s_find[i];
                ntree32::node_t inserted;
                CHECK_TRUE(ntree32::insert(tree, root, c_temp_slot, c_find_slot, s_compare_key_and_node, &s_keys, inserted));
                s_keys[inserted] = s_find[i];
            }

#ifdef TEST_ITERATOR
            ntree32::iterator_t iterator = ntree32::iterate(tree, root);

            s32       round       = 0;
            s32 const postorder[] = {0, 2, 1, 4, 6, 8, 7, 5, 3};

            ntree32::node_t node;
            while (iterator.postorder(tree, ntree32::LEFT, node))
            {
                CHECK_EQUAL(s_find[postorder[round++]], s_keys[node]);
            }
            CHECK_EQUAL(9, round);
#endif

            while (!ntree32::clear(tree, root, node)) {}

            ntree32::teardown_tree(tree);
        }

        UNITTEST_TEST(void_tree_search)
        {
            ntree32::node_t root = ntree32::c_invalid_node;
            ntree32::tree_t tree;
            ntree32::setup_tree(tree, m_nodes);
            s_init_keys();

            for (s32 i = 0; i < c_num_keys; ++i)
            {
                s_keys[c_find_slot] = s_find[i];
                ntree32::node_t inserted;
                CHECK_TRUE(ntree32::insert(tree, root, c_temp_slot, c_find_slot, s_compare_key_and_node, &s_keys, inserted));
                s_keys[inserted] = s_find[i];
            }

            ntree32::iterator_t iterator = ntree32::iterate(tree, root);

            s32             dir = ntree32::LEFT;
            ntree32::node_t node;
            s32             find = (c_num_keys / 2);  // find the key at s_keys[find]
            while (iterator.traverse(tree, dir, node))
            {
                s8 const c = s_compare_key_and_node(find, node, &s_keys);
                if (c == 0)
                    break;
                dir = tree.getdir(c);
            }
            CHECK_EQUAL(0, s_compare_key_and_node(find, node, &s_keys));

            while (!ntree32::clear(tree, root, node)) {}

            ntree32::teardown_tree(tree);
        }

        UNITTEST_TEST(s32_tree)
        {
            ntree32::node_t root = ntree32::c_invalid_node;
            ntree32::tree_t tree;
            ntree32::setup_tree(tree, m_nodes);
            s_init_keys();

            for (s32 i = 0; i < c_num_keys; ++i)
            {
                s_keys[c_find_slot] = s_find[i];
                ntree32::node_t inserted;
                CHECK_TRUE(ntree32::insert(tree, root, c_temp_slot, c_find_slot, s_compare_key_and_node, &s_keys, inserted));
                s_keys[inserted] = s_find[i];
            }

            for (s32 i = 0; i < c_num_keys; ++i)
            {
                ntree32::node_t node = ntree32::c_invalid_node;
                s_keys[c_find_slot]  = s_keys[i];
                CHECK_EQUAL(true, ntree32::find(tree, root, c_find_slot, s_compare_key_and_node, &s_keys, node));
                CHECK_NOT_EQUAL(ntree32::c_invalid_node, node);
                CHECK_EQUAL(i, node);
            }

            ntree32::node_t node = ntree32::c_invalid_node;
            s_keys[c_find_slot]  = 11111;
            CHECK_FALSE(ntree32::find(tree, root, c_find_slot, s_compare_key_and_node, &s_keys, node));

            while (!ntree32::clear(tree, root, node)) {}

            ntree32::teardown_tree(tree);
        }
    }
}
UNITTEST_SUITE_END
