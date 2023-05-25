#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/test_allocator.h"

#include "cunittest/cunittest.h"

namespace ncore
{
#define RB_RED   1
#define RB_BLCK  0
#define RB_RIGHT 1
#define RB_LEFT  0

    struct rbnode_t
    {
        rbnode_t()
            : m_item(nullptr)
            , m_color(RB_RED)
        {
        }

        void set_right(rbnode_t *node) { m_chld[RB_RIGHT] = node; }
        void set_left(rbnode_t *node) { m_chld[RB_LEFT] = node; }
        void set_child(s32 side, rbnode_t *node) { m_chld[side] = node; }

        void set_red() { m_color = RB_RED; }
        void set_black() { m_color = RB_BLCK; }

        rbnode_t *get_right() { return m_chld[RB_RIGHT]; }
        rbnode_t *get_left() { return m_chld[RB_LEFT]; }
        rbnode_t *get_child(s32 side) { return m_chld[side]; }

        rbnode_t *m_chld[2];
        void     *m_item;
        s8        m_color;
    };

#define RB_MKBLCK(R) R->m_color = RB_BLCK
#define RB_MKRED(R)  R->m_color = RB_RED
#define RB_ISRED(R)  (R->m_color == RB_RED)
#define RB_ISBLCK(R) (R->m_color == RB_BLCK)

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

    void rb_init(rbnode_t *root, rbnode_t *nill)
    {
        nill->m_item           = (void *)nullptr;
        nill->m_chld[RB_LEFT]  = nill;
        nill->m_chld[RB_RIGHT] = nill;

        root->m_item           = (void *)nullptr;
        root->m_chld[RB_LEFT]  = nill;
        root->m_chld[RB_RIGHT] = nill;
    }

    static inline rbnode_t *rotate_single(rbnode_t *root, s32 dir)
    {
        rbnode_t *save = root->get_child(!dir);

        root->set_child(!dir, save->get_child(dir));
        save->set_child(dir, root);

        root->set_red();
        save->set_black();

        return save;
    }

    static inline rbnode_t *rotate_double(rbnode_t *root, s32 dir)
    {
        root->m_chld[!dir] = rotate_single(root->get_child(!dir), !dir);
        return rotate_single(root, dir);
    }

    static inline s32 is_red(rbnode_t *root) { return root != nullptr && root->m_color == RB_RED; }

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

            s32 lh = rb_validate(ln, compare, result);
            s32 rh = rb_validate(rn, compare, result);

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

    s32 rb_insert(rbnode_t *&root, rbnode_t *const nill, s32 &count, void *item, s32 (*compare)(void *a, void *b), rbnode_t *&new_node)
    {
        s32 result = 0;
        if (root == nullptr)
        {
            // We have an empty tree; attach the
            // new node directly to the root
            root         = new_node;
            root->m_item = item;
            result       = 1;
            if (root == nullptr)
                return result;
        }
        else
        {
            rbnode_t  head;   // False tree root
            rbnode_t *g, *t;  // Grandparent & parent
            rbnode_t *p, *q;  // Iterator & parent
            s32       dir = 0, last = 0;

            // Set up our helpers
            t = &head;
            g = p = nullptr;
            q     = root;
            t->set_right(root);

            // Search down the tree for a place to insert
            for (;;)
            {
                if (q == nullptr)
                {
                    // Insert a new node at the first null link
                    result    = 1;
                    q         = new_node;
                    q->m_item = item;
                    p->set_child(dir, q);

                    if (q == nullptr)
                        return result;
                }
                else if (is_red(q->get_left()) && is_red(q->get_right()))
                {
                    // Simple red violation: color flip
                    q->set_red();
                    q->get_left()->set_black();
                    q->get_right()->set_black();
                }

                if (is_red(q) && is_red(p))
                {
                    // Hard red violation: rotations necessary
                    s32 dir2 = t->get_right() == g;

                    rbnode_t *rotated;
                    if (q == p->get_child(last))
                        rotated = rotate_single(g, !last);
                    else
                        rotated = rotate_double(g, !last);

                    t->set_child(dir2, rotated);
                }

                // Stop working if we inserted a node. This
                // check also disallows duplicates in the tree
                dir = compare(item, q->m_item);
                if (dir == 0)
                    break;
                dir  = ((dir + 1) >> 1);
                last = dir;

                // Move the helpers down
                if (g != nullptr)
                    t = g;

                g = p, p = q;
                q = q->get_child(dir);
            }

            // Update the root (it may be different)
            root = head.get_right();
        }

        // Make the root black for simplified logic
        root->set_black();
        count += result;
        return result;
    }

    rbnode_t *rb_remove(rbnode_t *&root, rbnode_t *nill, s32 &count, void *item, s32 (*compare)(void *a, void *b))
    {
        if (item == nullptr)
            return nullptr;

        int       sP;
        int       sX     = RB_RIGHT;
        rbnode_t *G      = root;
        rbnode_t *P      = G;
        rbnode_t *X      = P->m_chld[RB_RIGHT];
        rbnode_t *T      = nullptr;
        rbnode_t *toDel  = nill;
        rbnode_t *toDelP = nill;
        int       sDel   = 0;

        // step 1: examine the root
        int c2b;
        if (RB_ISBLCK(X->m_chld[RB_LEFT]) && RB_ISBLCK(X->m_chld[RB_RIGHT]))
        {
            RB_MKRED(X);
            c2b = 0;
        }
        else
        {
            // step 2B
            c2b = 1;
        }

        goto l0;

        // Top-Down Deletion
        do
        {
            // case 2b continue: check new X
            if (c2b)
            {
                c2b = 0;

                // if new X is red continue down again
                if (RB_ISRED(X))
                    goto l0;

                G->m_chld[sP]  = T;
                P->m_chld[!sX] = T->m_chld[sX];
                T->m_chld[sX]  = P;

                RB_MKRED(P);
                RB_MKBLCK(T);

                if (toDelP == G)
                {
                    toDelP = T;
                    sDel   = sX;
                }

                G  = T;
                T  = P->m_chld[!sX];
                sP = sX;
                // if new X is black back to case 2
            }

            // case 2: X has two black children
            if (RB_ISBLCK(X->m_chld[RB_LEFT]) && RB_ISBLCK(X->m_chld[RB_RIGHT]))
            {
                // case 1.a: T has two black children
                if (T != nill && RB_ISBLCK(T->m_chld[RB_LEFT]) && RB_ISBLCK(T->m_chld[RB_RIGHT]))
                {
                    // color flip
                    RB_MKRED(X);
                    RB_MKRED(T);
                    RB_MKBLCK(P);
                }

                // case 1.b: T's left child is red
                else if (RB_ISRED(T->m_chld[sX]))
                {
                    rbnode_t *R = T->m_chld[sX];

                    // double rotate: rotate R around T, then R around P
                    T->m_chld[sX]  = R->m_chld[!sX];
                    P->m_chld[!sX] = R->m_chld[sX];
                    R->m_chld[sX]  = P;
                    R->m_chld[!sX] = T;
                    G->m_chld[sP]  = R;

                    RB_MKRED(X);
                    RB_MKBLCK(P);

                    if (toDelP == G)
                    {
                        toDelP = R;
                        sDel   = sX;
                    }
                }

                // case 1.c: T's right child is red
                else if (RB_ISRED(T->m_chld[!sX]))
                {
                    rbnode_t *R = T->m_chld[!sX];

                    // single rotate: rotate T around P
                    P->m_chld[!sX] = T->m_chld[sX];
                    T->m_chld[sX]  = P;
                    G->m_chld[sP]  = T;

                    RB_MKRED(X);
                    RB_MKRED(T);
                    RB_MKBLCK(P);
                    RB_MKBLCK(R);

                    if (toDelP == G)
                    {
                        toDelP = T;
                        sDel   = sX;
                    }
                }
            }
            else
            {
                // case 2b: X's one child is red, advance to next level
                c2b = 1;
            }

        l0:
            sP = sX;
            if (toDel != nill)
            {
                sX = toDel->m_chld[RB_RIGHT] == nill;
            }
            else
            {
                s32 const cmpRet = compare(item, X->m_item);
                if (cmpRet == 0)
                {
                    toDelP = P;
                    toDel  = X;
                    sDel   = sP;
                    sX     = toDel->m_chld[RB_RIGHT] != nill;
                }
                else
                {
                    sX = (cmpRet + 1) >> 1;
                }
            }

            G = P;
            P = X;
            X = P->m_chld[sX];
            T = P->m_chld[!sX];
        } while (X != nill);

        // make root black
        RB_MKBLCK(root->m_chld[RB_RIGHT]);

        if (toDel == nill)
            return nullptr;

        if (P != toDel)  // toDel has least one child
        {
            // P is black, save black height
            if (c2b)
                RB_MKBLCK(P->m_chld[!sX]);

            // change color
            if (RB_ISRED(toDel))
                RB_MKRED(P);
            else
                RB_MKBLCK(P);

            // replace P with its left child
            G->m_chld[sP] = P->m_chld[!sX];

            // replace X with in-order predecessor
            P->m_chld[RB_RIGHT]  = toDel->m_chld[RB_RIGHT];
            P->m_chld[RB_LEFT]   = toDel->m_chld[RB_LEFT];
            toDelP->m_chld[sDel] = P;
        }
        else  // P is toDel; there is no child
        {
            G->m_chld[sP] = nill;
        }

        count--;
        return toDel;
    }

    rbnode_t *rb_find(rbnode_t *root, rbnode_t *nill, void *item, s32 (*compare)(void *a, void *b))
    {
        if (item == nullptr)
            return nullptr;

        rbnode_t *iter = root->m_chld[RB_RIGHT];
        while (iter != nill)
        {
            const s32 cmpRet = compare(item, iter->m_item);
            if (cmpRet == 0)
                break;
            iter = iter->m_chld[(cmpRet + 1) >> 1];
        }

        if (iter == nill)
            return nullptr;
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

        UNITTEST_TEST(tree_node) {}
    }
}
UNITTEST_SUITE_END
