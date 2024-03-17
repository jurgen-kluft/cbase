#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/test_allocator.h"
#include "cbase/c_tree2.h"

#include "cunittest/cunittest.h"

namespace ncore
{
    struct rbnode_t
    {
        rbnode_t()
            : m_item(nullptr)
            , m_color(ntree2::RED)
        {
            m_chld[ntree2::LEFT]  = nullptr;
            m_chld[ntree2::RIGHT] = nullptr;
        }

        void set_right(rbnode_t *node) { m_chld[ntree2::RIGHT] = node; }
        void set_left(rbnode_t *node) { m_chld[ntree2::LEFT] = node; }
        void set_child(s32 side, rbnode_t *node)
        {
            ASSERT(side == ntree2::LEFT || side == ntree2::RIGHT);
            m_chld[side] = node;
        }

        void set_red() { m_color = ntree2::RED; }
        void set_black() { m_color = ntree2::BLACK; }

        rbnode_t *get_right() { return m_chld[ntree2::RIGHT]; }
        rbnode_t *get_left() { return m_chld[ntree2::LEFT]; }
        rbnode_t *get_child(s32 side)
        {
            ASSERT(side == ntree2::LEFT || side == ntree2::RIGHT);
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
        n->m_item                = (void *)nullptr;
        n->m_chld[ntree2::LEFT]  = nullptr;
        n->m_chld[ntree2::RIGHT] = nullptr;
        n->m_color               = ntree2::RED;
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
        node->m_color         = ntree2::RED;
        save->m_color         = ntree2::BLACK;
        return save;
    }

    static inline rbnode_t *rotate_single_track_parent(rbnode_t *node, s32 dir, rbnode_t *fn, rbnode_t *&fp)
    {
        rbnode_t *save        = node->m_chld[1 - dir];
        node->m_chld[1 - dir] = save->m_chld[dir];
        save->m_chld[dir]     = node;
        node->m_color         = ntree2::RED;
        save->m_color         = ntree2::BLACK;

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

    static inline s32 is_red(rbnode_t *node) { return node != nullptr && node->m_color == ntree2::RED; }

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
                n->set_child(ntree2::LEFT, fn->get_left());
                n->set_child(ntree2::RIGHT, fn->get_right());
                n->m_color = fn->m_color;
            }

            if (root == fn)
                root = n;

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
    }

    s32 compare_s32(void const *p1, void const *p2)
    {
        s32 const s1 = *(s32 const *)p1;
        s32 const s2 = *(s32 const *)p2;
        if (s1 < s2)
            return -1;
        else if (s1 > s2)
            return 1;
        return 0;
    }

    class ctxt_tree2_t : public ncore::ntree2::tree_t
    {
        alloc_t *m_allocator;
        s32      m_size;
        s32      m_cap;

        struct node16_t
        {
            s32 m_data;
            u16 m_color;
            u16 m_branches[3];
        };

        node16_t *m_nodes;
        s32       m_freeindex;
        node16_t *m_freelist;
        s32      *m_keys;
        node16_t *m_temp;
        node16_t *m_root;

    public:
        ctxt_tree2_t()
            : m_allocator(nullptr)
            , m_size(0)
            , m_cap(0)
            , m_nodes(nullptr)
            , m_freeindex(0)
            , m_freelist(nullptr)
            , m_keys(nullptr)
            , m_temp(nullptr)
            , m_root(nullptr)
        {
        }

        void init(alloc_t *allocator)
        {
            m_allocator                      = allocator;
            m_size                           = 0;
            m_cap                            = 8192;
            m_nodes                          = (node16_t *)m_allocator->allocate(sizeof(node16_t) * m_cap);
            m_freeindex                      = 0;
            m_freelist                       = nullptr;
            m_keys                           = (s32 *)m_allocator->allocate(sizeof(s32) * m_cap);
            m_nodes[m_cap - 1].m_branches[0] = 0xffff;
            m_nodes[m_cap - 1].m_branches[1] = 0xffff;
            m_nodes[m_cap - 1].m_branches[2] = 0xffff;

            m_root = nullptr;
            m_temp = (node16_t *)v_new_node(nullptr, nullptr);
        }

        void exit()
        {
            m_allocator->deallocate(m_nodes);
            m_nodes = nullptr;
            m_allocator->deallocate(m_keys);
            m_keys = nullptr;
        }

        virtual ntree2::node_t *v_get_root() const final { return (ntree2::node_t *)m_root; }
        virtual void            v_set_root(ntree2::node_t *node) { m_root = (node16_t *)node; }
        virtual ntree2::node_t *v_get_temp() const { return (ntree2::node_t *)m_temp; }

        virtual s32 v_size() const final { return m_size; }
        virtual s32 v_capacity() const final { return m_cap; }

        virtual void            v_set_color(ntree2::node_t *node, ntree2::color_e color) final { ((node16_t *)node)->m_color = color; }
        virtual ntree2::color_e v_get_color(ntree2::node_t const *node) const final { return (ntree2::color_e)((node16_t const *)node)->m_color; }
        virtual void const     *v_get_key(ntree2::node_t const *node) const final
        {
            u16 const index = (u16)((node16_t const *)node - m_nodes);
            return (void const *)&m_keys[index];
        }
        virtual void const *v_get_value(ntree2::node_t const *node) const final
        {
            u16 const index = (u16)((node16_t const *)node - m_nodes);
            return (void const *)&m_keys[index];
        }
        virtual ntree2::node_t *v_get_node(ntree2::node_t const *node, ntree2::node_e ne) const final
        {
            u16 const index = ((node16_t const *)node)->m_branches[ne];
            return (index == 0) ? nullptr : (ntree2::node_t *)(&m_nodes[index]);
        }
        virtual void v_set_node(ntree2::node_t *node, ntree2::node_e ne, ntree2::node_t *set) final
        {
            u16 const index                    = (set == nullptr) ? 0 : (u16)((node16_t *)set - m_nodes);
            ((node16_t *)node)->m_branches[ne] = index;
        }
        virtual ntree2::node_t *v_new_node(void const *key, void const *value) final
        {
            node16_t *node = nullptr;
            if (m_freelist != nullptr)
            {
                node       = m_freelist;
                m_freelist = (node->m_branches[2] == 0xffff) ? nullptr : &m_nodes[node->m_branches[2]];
            }
            else if (m_freeindex < m_cap)
            {
                node = &m_nodes[m_freeindex++];
            }
            else
            {
                return nullptr;
            }

            u16 const index     = (u16)(node - m_nodes);
            m_keys[index]       = key != nullptr ? *((const s32 *)key) : 0;
            node->m_color       = ntree2::RED;
            node->m_data        = index;
            node->m_branches[0] = 0;
            node->m_branches[1] = 0;
            node->m_branches[2] = 0;
            m_size++;
            return (ntree2::node_t *)node;
        }
        virtual void v_del_node(ntree2::node_t *node)
        {
            if (node == nullptr)
                return;
            node16_t *n      = (node16_t *)node;
            n->m_branches[0] = 0;
            n->m_branches[1] = 0;
            if (m_freelist == nullptr)
            {
                n->m_branches[2] = 0xffff;
            }
            else
            {
                n->m_branches[2] = (u16)(m_freelist - m_nodes);
            }
            m_freelist = n;
            m_size--;
            if (m_size == 0)
            {
                m_freelist  = nullptr;
                m_freeindex = 0;
            }
        }
        virtual s32 v_compare_nodes(ntree2::node_t const *node, ntree2::node_t const *other) const { return compare_s32(v_get_key(node), v_get_key(other)); }
        virtual s32 v_compare_insert(void const *key, ntree2::node_t const *node) const { return compare_s32(key, v_get_key(node)); }
    };

    UNITTEST_FIXTURE(tree2)
    {
        static ctxt_tree2_t    ctxt_instance;
        static ntree2::tree_t *ctxt = &ctxt_instance;

        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() { ctxt_instance.init(Allocator); }
        UNITTEST_FIXTURE_TEARDOWN() { ctxt_instance.exit(); }

        UNITTEST_TEST(tree_node)
        {
            s32 const key = 0;

            ntree2::node_t *node  = ctxt->v_new_node(&key, nullptr);
            ntree2::node_t *left  = ctxt->v_new_node(&key, nullptr);
            ntree2::node_t *right = ctxt->v_new_node(&key, nullptr);

            ntree2::node_t *left_ptr = left;
            // node->set_left(ctxt, left_ptr);
            ctxt->v_set_node(node, ntree2::LEFT, left_ptr);

            // CHECK_EQUAL(left_ptr, node->get_left(ctxt));
            CHECK_EQUAL(left_ptr, ctxt->v_get_node(node, ntree2::LEFT));

            ntree2::node_t *right_ptr = right;
            // node->set_right(ctxt, right_ptr);
            ctxt->v_set_node(node, ntree2::RIGHT, right_ptr);
            // CHECK_EQUAL(right_ptr, node->get_right(ctxt));
            CHECK_EQUAL(right_ptr, ctxt->v_get_node(node, ntree2::RIGHT));

            // CHECK_EQUAL(true, node->is_red(ctxt));
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree2::RED);

            // node->set_black(ctxt);
            // CHECK_EQUAL(false, node->is_red(ctxt));
            // node->set_red(ctxt);
            // CHECK_EQUAL(true, node->is_red(ctxt));
            // node->set_black(ctxt);
            // CHECK_EQUAL(false, node->is_red(ctxt));
            // CHECK_EQUAL(true, node->is_black(ctxt));
            ctxt->v_set_color(node, ntree2::BLACK);
            CHECK_EQUAL(false, ctxt->v_get_color(node) == ntree2::RED);
            ctxt->v_set_color(node, ntree2::RED);
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree2::RED);
            ctxt->v_set_color(node, ntree2::BLACK);
            CHECK_EQUAL(false, ctxt->v_get_color(node) == ntree2::RED);
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree2::BLACK);

            left_ptr  = left;
            right_ptr = right;
            // node->set_left(ctxt, left_ptr);
            // node->set_right(ctxt, right_ptr);
            // CHECK_EQUAL(left_ptr, node->get_child(ctxt, 0));
            // CHECK_EQUAL(right_ptr, node->get_child(ctxt, 1));
            // CHECK_EQUAL(true, node->is_black(ctxt));
            ctxt->v_set_node(node, ntree2::LEFT, left_ptr);
            ctxt->v_set_node(node, ntree2::RIGHT, right_ptr);
            CHECK_EQUAL(left_ptr, ctxt->v_get_node(node, ntree2::LEFT));
            CHECK_EQUAL(right_ptr, ctxt->v_get_node(node, ntree2::RIGHT));
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree2::BLACK);

            // node->set_child(ctxt, 0, left_ptr);
            // node->set_child(ctxt, 1, right_ptr);
            ctxt->v_set_node(node, ntree2::LEFT, left_ptr);
            ctxt->v_set_node(node, ntree2::RIGHT, right_ptr);

            // CHECK_EQUAL(left_ptr, node->get_child(ctxt, 0));
            // CHECK_EQUAL(right_ptr, node->get_child(ctxt, 1));
            CHECK_EQUAL(left_ptr, ctxt->v_get_node(node, ntree2::LEFT));
            CHECK_EQUAL(right_ptr, ctxt->v_get_node(node, ntree2::RIGHT));

            // CHECK_EQUAL(true, node->is_black(ctxt));
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree2::BLACK);

            ctxt->v_del_node(node);
            ctxt->v_del_node(left);
            ctxt->v_del_node(right);
        }

        UNITTEST_TEST(void_tree)
        {
            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            const char *result = nullptr;

            bool inserted;
            inserted = ntree2::insert(ctxt, &a);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &b);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &b);  // Duplicate insert should fail
            CHECK_FALSE(inserted);
            inserted = ntree2::insert(ctxt, &c);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &d);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &e);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &f);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &g);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &h);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &i);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));

            CHECK_EQUAL(9 + 2, ntree2::size(ctxt));

            ntree2::node_t *node = nullptr;
            CHECK_EQUAL(true, ntree2::find(ctxt, &a, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &b, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &c, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &d, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &e, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &f, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &g, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &h, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &i, node));

            s32 x(99);
            CHECK_FALSE(ntree2::find(ctxt, &x, node));

            CHECK_EQUAL(11, ntree2::size(ctxt));

            while (!ntree2::clear(ctxt)) {}

            CHECK_EQUAL(2, ntree2::size(ctxt));
        }

        UNITTEST_TEST(void_tree_iterate_preorder)
        {
            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(ntree2::insert(ctxt, &a));
            CHECK_TRUE(ntree2::insert(ctxt, &b));
            CHECK_TRUE(ntree2::insert(ctxt, &c));
            CHECK_TRUE(ntree2::insert(ctxt, &d));
            CHECK_TRUE(ntree2::insert(ctxt, &e));
            CHECK_TRUE(ntree2::insert(ctxt, &f));
            CHECK_TRUE(ntree2::insert(ctxt, &g));
            CHECK_TRUE(ntree2::insert(ctxt, &h));
            CHECK_TRUE(ntree2::insert(ctxt, &i));

#if 0
            ntree2::iterator_t iterator = ntree2::iterate(ctxt);

            s32         round      = 0;
            s32         preorder[] = {d, b, a, c, f, e, h, g, i};
            void const *data;
            while (iterator.preorder(ntree2::RIGHT, data))
            {
                CHECK_EQUAL(preorder[round++], *(s32 *)data);
            }
            CHECK_EQUAL(9, round);
#endif
            while (!ntree2::clear(ctxt)) {}
        }

        UNITTEST_TEST(void_tree_iterate_sortorder)
        {
            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(ntree2::insert(ctxt, &a));
            CHECK_TRUE(ntree2::insert(ctxt, &b));
            CHECK_TRUE(ntree2::insert(ctxt, &c));
            CHECK_TRUE(ntree2::insert(ctxt, &d));
            CHECK_TRUE(ntree2::insert(ctxt, &e));
            CHECK_TRUE(ntree2::insert(ctxt, &f));
            CHECK_TRUE(ntree2::insert(ctxt, &g));
            CHECK_TRUE(ntree2::insert(ctxt, &h));
            CHECK_TRUE(ntree2::insert(ctxt, &i));

#if 0
            ntree2::iterator_t iterator = ntree2::iterate(ctxt);

            s32         round       = 0;
            s32         sortorder[] = {a, b, c, d, e, f, g, h, i};
            void const *data;
            while (iterator.sortorder(ntree2::RIGHT, data))
            {
                CHECK_EQUAL(sortorder[round++], *(s32 *)data);
            }
            CHECK_EQUAL(9, round);
#endif

            while (!ntree2::clear(ctxt)) {}
        }

        UNITTEST_TEST(void_tree_iterate_sortorder_backwards)
        {
            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(ntree2::insert(ctxt, &a));
            CHECK_TRUE(ntree2::insert(ctxt, &b));
            CHECK_TRUE(ntree2::insert(ctxt, &c));
            CHECK_TRUE(ntree2::insert(ctxt, &d));
            CHECK_TRUE(ntree2::insert(ctxt, &e));
            CHECK_TRUE(ntree2::insert(ctxt, &f));
            CHECK_TRUE(ntree2::insert(ctxt, &g));
            CHECK_TRUE(ntree2::insert(ctxt, &h));
            CHECK_TRUE(ntree2::insert(ctxt, &i));

#if 0
            ntree2::iterator_t iterator = ntree2::iterate(ctxt);

            s32         round       = 0;
            s32         sortorder[] = {i, h, g, f, e, d, c, b, a};
            void const *data;
            while (iterator.sortorder(ntree2::LEFT, data))
            {
                CHECK_EQUAL(sortorder[round++], *(s32 *)data);
            }
            CHECK_EQUAL(9, round);
#endif

            while (!ntree2::clear(ctxt)) {}
        }

        UNITTEST_TEST(void_tree_iterate_postorder)
        {
            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(ntree2::insert(ctxt, &a));
            CHECK_TRUE(ntree2::insert(ctxt, &b));
            CHECK_TRUE(ntree2::insert(ctxt, &c));
            CHECK_TRUE(ntree2::insert(ctxt, &d));
            CHECK_TRUE(ntree2::insert(ctxt, &e));
            CHECK_TRUE(ntree2::insert(ctxt, &f));
            CHECK_TRUE(ntree2::insert(ctxt, &g));
            CHECK_TRUE(ntree2::insert(ctxt, &h));
            CHECK_TRUE(ntree2::insert(ctxt, &i));

#if 0
            ntree2::iterator_t iterator = ntree2::iterate(ctxt);

            s32 round       = 0;
            s32 postorder[] = {a, c, b, e, g, i, h, f, d};

            void const *data;
            while (iterator.postorder(ntree2::RIGHT, data))
            {
                CHECK_EQUAL(postorder[round++], *(s32 *)data);
            }
            CHECK_EQUAL(9, round);
#endif

            while (!ntree2::clear(ctxt)) {}
        }

        UNITTEST_TEST(void_tree_search)
        {
            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(ntree2::insert(ctxt, &a));
            CHECK_TRUE(ntree2::insert(ctxt, &b));
            CHECK_TRUE(ntree2::insert(ctxt, &c));
            CHECK_TRUE(ntree2::insert(ctxt, &d));
            CHECK_TRUE(ntree2::insert(ctxt, &e));
            CHECK_TRUE(ntree2::insert(ctxt, &f));
            CHECK_TRUE(ntree2::insert(ctxt, &g));
            CHECK_TRUE(ntree2::insert(ctxt, &h));
            CHECK_TRUE(ntree2::insert(ctxt, &i));

            ntree2::iterator_t iterator = ntree2::iterate(ctxt);

            s32         dir = ntree2::LEFT;
            void const *data;
            void       *find = &f;
            while (iterator.traverse(dir, data))
            {
                s32 const c = compare_s32(find, data);
                if (c == 0)
                    break;
                dir = iterator.getdir(c);
            }
            CHECK_EQUAL(0, compare_s32(data, find));

            while (!ntree2::clear(ctxt)) {}
        }

        UNITTEST_TEST(s32_tree)
        {
            s32 a(1);
            s32 b(2);
            s32 c(3);
            s32 d(4);

            const char *result = nullptr;

            bool inserted;
            inserted = ntree2::insert(ctxt, &a);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &b);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &b);  // Duplicate insert should fail
            CHECK_FALSE(inserted);
            inserted = ntree2::insert(ctxt, &c);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));
            inserted = ntree2::insert(ctxt, &d);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree2::validate(ctxt, result));

            CHECK_EQUAL(6, ntree2::size(ctxt));

            ntree2::node_t *node = nullptr;
            CHECK_EQUAL(true, ntree2::find(ctxt, &a, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &b, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &c, node));
            CHECK_EQUAL(true, ntree2::find(ctxt, &d, node));

            s32 e(5);
            CHECK_FALSE(ntree2::find(ctxt, &e, node));

            while (!ntree2::clear(ctxt)) {}

            CHECK_EQUAL(2, ntree2::size(ctxt));
        }
    }
}
UNITTEST_SUITE_END
