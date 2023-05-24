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

    void rb_insert(rbnode_t *&root, rbnode_t *const nill, s32 &count, void *item, s32 (*compare)(void *a, void *b), rbnode_t *&new_node)
    {
        new_node->m_chld[RB_LEFT]  = nill;
        new_node->m_chld[RB_RIGHT] = nill;
        new_node->m_item           = item;

        if (root->m_chld[RB_RIGHT] == nill)
        {
            RB_MKBLCK(new_node);
            root->m_chld[RB_RIGHT] = new_node;
            count++;
            return;
        }

        int sQ = 1;
        int sG = 1;
        int sP = 1;
        int sX = 1;

        rbnode_t *W = root;
        rbnode_t *P = root;
        rbnode_t *G = root;
        rbnode_t *Q = root;
        rbnode_t *X = P->m_chld[RB_RIGHT];

        // Top-Down Insert
        do
        {
            // main case : two children are red
            if (RB_ISRED(X->m_chld[RB_LEFT]) && RB_ISRED(X->m_chld[RB_RIGHT]))
            {
                // case 1: P is black
                if (!RB_ISRED(P))
                {
                    // make X red
                    RB_MKRED(X);

                    // make two children black
                    RB_MKBLCK(X->m_chld[RB_LEFT]);
                    RB_MKBLCK(X->m_chld[RB_RIGHT]);
                }

                // P is red
                else
                {
                    RB_MKRED(G);

                    // case 2: X and P are both left/right children
                    if (sX == sP)
                    {
                        // single rotation

                        RB_MKRED(X);
                        RB_MKBLCK(P);
                        RB_MKBLCK(X->m_chld[RB_LEFT]);
                        RB_MKBLCK(X->m_chld[RB_RIGHT]);

                        Q->m_chld[sG]  = P;
                        G->m_chld[sP]  = P->m_chld[!sP];
                        P->m_chld[!sP] = G;

                        G = Q;
                        Q = W;
                    }

                    // case 3: X and P are opposide side
                    else
                    {
                        RB_MKBLCK(X);
                        RB_MKBLCK(X->m_chld[RB_LEFT]);
                        RB_MKBLCK(X->m_chld[RB_RIGHT]);

                        Q->m_chld[sG] = X;
                        P->m_chld[sX] = X->m_chld[sP];
                        G->m_chld[sP] = X->m_chld[sX];
                        X->m_chld[sP] = P;
                        X->m_chld[sX] = G;

                        G  = W;
                        P  = Q;
                        sX = sG;
                        sP = sQ;
                    }
                }
            }

            const s32 cmp = compare(item, X->m_item);  // -1, 0, 1
            if (cmp == 0)
                goto duplicate;

            sQ = sG;
            sG = sP;
            sP = sX;
            sX = (cmp + 1) >> 1;  // -1 -> 0, 0 -> 0, 1 -> 1
            W  = Q;
            Q  = G;
            G  = P;
            P  = X;
            X  = X->m_chld[sX];
        } while (X != nill);

        X = P->m_chld[sX] = new_node;
        new_node          = nullptr;

        // make current red
        RB_MKRED(X);

        // check for red violation, we know uncle is black
        if (RB_ISRED(P))
        {
            RB_MKRED(G);

            // double rotation
            if (sX != sP)
            {
                RB_MKBLCK(X);

                Q->m_chld[sG]  = X;
                P->m_chld[sX]  = X->m_chld[!sX];
                G->m_chld[sP]  = X->m_chld[sX];
                X->m_chld[!sX] = P;
                X->m_chld[sX]  = G;
            }

            // single rotation
            else
            {
                RB_MKBLCK(P);

                G->m_chld[sP]  = P->m_chld[!sP];
                P->m_chld[!sP] = G;
                Q->m_chld[sG]  = P;
            }
        }

        // make root black
        RB_MKBLCK(root->m_chld[RB_RIGHT]);
        count++;

    duplicate:
        return;
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
