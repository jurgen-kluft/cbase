#include "cbase/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_tree.h"

#include "cunittest/cunittest.h"

using namespace ncore;

extern ncore::alloc_t* gTestAllocator;

s32 compare_s32(void const* p1, void const* p2)
{
    s32 const s1 = *(s32 const*)p1;
    s32 const s2 = *(s32 const*)p2;
    if (s1 < s2)
        return -1;
    else if (s1 > s2)
        return 1;
    return 0;
}

class ctxt_tree_t : public tree_t::ctxt_t
{
    alloc_t* m_allocator;
    int_t    m_size;
    int_t    m_cap;

    struct node16_t
    {
        u16 m_color;
        u16 m_data;
        u16 m_branches[3];
    };

    node16_t*    m_nodes;
    node16_t*    m_freelist;
    void const** m_keys;

public:
    ctxt_tree_t() { m_allocator = gTestAllocator; }

    void init()
    {
        m_size     = 0;
        m_cap      = 32;
        m_nodes    = (node16_t*)m_allocator->allocate(sizeof(node16_t) * m_cap);
        m_freelist = m_nodes;
        m_keys     = (void const**)m_allocator->allocate(sizeof(void*) * m_cap);
        for (int_t i = 0; i < m_cap; ++i)
        {
            m_nodes[i].m_color       = tree_t::BLACK;
            m_nodes[i].m_data        = i;
            m_nodes[i].m_branches[0] = i + 1;
            m_nodes[i].m_branches[1] = i + 1;
            m_nodes[i].m_branches[2] = i + 1;
        }
        m_nodes[m_cap - 1].m_branches[0] = 0xffff;
        m_nodes[m_cap - 1].m_branches[1] = 0xffff;
        m_nodes[m_cap - 1].m_branches[2] = 0xffff;
    }

    virtual int_t           v_size() const { return m_size; }
    virtual int_t           v_capacity() const { return m_cap; }
    virtual void            v_set_color(tree_t::node_t* node, tree_t::color_e color) { ((node16_t*)node)->m_color = color; }
    virtual tree_t::color_e v_get_color(tree_t::node_t const* node) const { return (tree_t::color_e)((node16_t const*)node)->m_color; }
    virtual void const*     v_get_key(tree_t::node_t const* node) const
    {
        u16 const index = (u16)((node16_t const*)node - m_nodes);
        return (void const*)&m_keys[index];
    }
    virtual void const* v_get_value(tree_t::node_t const* node) const
    {
        u16 const index = (u16)((node16_t const*)node - m_nodes);
        return (void const*)&m_keys[index];
    }
    virtual tree_t::node_t* v_get_node(tree_t::node_t const* node, tree_t::node_e ne) const { return (tree_t::node_t*)(&m_nodes[((node16_t const*)node)->m_branches[ne]]); }
    virtual void            v_set_node(tree_t::node_t* node, tree_t::node_e ne, tree_t::node_t* set) const { ((node16_t*)node)->m_branches[ne] = (u16)((node16_t*)set - m_nodes); }
    virtual tree_t::node_t* v_new_node(void const* key, void const* value)
    {
        if (m_freelist == nullptr)
            return nullptr;
        node16_t* node = m_freelist;
        if (node->m_branches[2] == 0xffff)
            m_freelist = nullptr;
        else
            m_freelist = &m_nodes[node->m_branches[2]];

        u16 const index     = (u16)(node - m_nodes);
        m_keys[index]       = key;
        node->m_color       = tree_t::RED;
        node->m_data        = index;
        node->m_branches[0] = 0;
        node->m_branches[1] = 0;
        node->m_branches[2] = 0;
        return (tree_t::node_t*)node;
    }
    virtual void v_del_node(tree_t::node_t* node)
    {
        if (node == nullptr)
            return;
        node16_t* n      = (node16_t*)node;
        n->m_branches[2] = (u16)(m_freelist - m_nodes);
        m_freelist       = n;
    }
    virtual s32 v_compare_nodes(tree_t::node_t const* node, tree_t::node_t const* other) const { return compare_s32(v_get_key(node), v_get_key(other)); }
    virtual s32 v_compare_insert(void const* key, tree_t::node_t const* node) const { return compare_s32(key, v_get_key(node)); }
};

UNITTEST_SUITE_BEGIN(test_tree)
{
    UNITTEST_FIXTURE(main)
    {
        static ctxt_tree_t     ctxt_instance;
        static tree_t::ctxt_t* ctxt = &ctxt_instance;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(tree_node)
        {
            tree_t::node_t* node  = ctxt->v_new_node(nullptr, nullptr);
            tree_t::node_t* left  = ctxt->v_new_node(nullptr, nullptr);
            tree_t::node_t* right = ctxt->v_new_node(nullptr, nullptr);

            tree_t::node_t* left_ptr = left;
            node->set_left(ctxt, left_ptr);

            CHECK_EQUAL(left_ptr, node->get_left(ctxt));

            tree_t::node_t* right_ptr = right;
            node->set_right(ctxt, right_ptr);
            CHECK_EQUAL(right_ptr, node->get_right(ctxt));

            CHECK_EQUAL(true, node->is_red(ctxt));

            node->set_black(ctxt);
            CHECK_EQUAL(false, node->is_red(ctxt));
            node->set_red(ctxt);
            CHECK_EQUAL(true, node->is_red(ctxt));
            node->set_black(ctxt);
            CHECK_EQUAL(false, node->is_red(ctxt));

            CHECK_EQUAL(true, node->is_black(ctxt));

            left_ptr  = left;
            right_ptr = right;
            node->set_left(ctxt, left_ptr);
            node->set_right(ctxt, right_ptr);
            CHECK_EQUAL(left_ptr, node->get_child(ctxt, 0));
            CHECK_EQUAL(right_ptr, node->get_child(ctxt, 1));

            CHECK_EQUAL(true, node->is_black(ctxt));

            node->set_child(ctxt, 0, left_ptr);
            node->set_child(ctxt, 1, right_ptr);

            CHECK_EQUAL(left_ptr, node->get_child(ctxt, 0));
            CHECK_EQUAL(right_ptr, node->get_child(ctxt, 1));

            CHECK_EQUAL(true, node->is_black(ctxt));

            ctxt->v_del_node(node);
            ctxt->v_del_node(left);
            ctxt->v_del_node(right);
        }

        UNITTEST_TEST(void_tree)
        {
            tree_t tree;
            tree.init(ctxt);

            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            const char* result = nullptr;

            bool inserted;
            inserted = tree.insert(&a);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&b);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&b); // Duplicate insert should fail
            CHECK_FALSE(inserted);
            inserted = tree.insert(&c);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&d);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&e);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&f);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&g);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&h);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&i);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));

            CHECK_EQUAL(9, tree.size());

            tree_t::node_t* node = nullptr;
            CHECK_EQUAL(true, tree.find(&a, node));
            CHECK_EQUAL(true, tree.find(&b, node));
            CHECK_EQUAL(true, tree.find(&c, node));
            CHECK_EQUAL(true, tree.find(&d, node));
            CHECK_EQUAL(true, tree.find(&e, node));
            CHECK_EQUAL(true, tree.find(&f, node));
            CHECK_EQUAL(true, tree.find(&g, node));
            CHECK_EQUAL(true, tree.find(&h, node));
            CHECK_EQUAL(true, tree.find(&i, node));

            s32 x(99);
            CHECK_FALSE(tree.find(&x, node));

            CHECK_EQUAL(9, tree.size());

            void const* data;
            while (!tree.clear(data)) {}

            CHECK_EQUAL(0, tree.size());
        }

        UNITTEST_TEST(void_tree_iterate_preorder)
        {
            tree_t tree;
            tree.init(ctxt);

            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(tree.insert(&a));
            CHECK_TRUE(tree.insert(&b));
            CHECK_TRUE(tree.insert(&c));
            CHECK_TRUE(tree.insert(&d));
            CHECK_TRUE(tree.insert(&e));
            CHECK_TRUE(tree.insert(&f));
            CHECK_TRUE(tree.insert(&g));
            CHECK_TRUE(tree.insert(&h));
            CHECK_TRUE(tree.insert(&i));

            tree_t::iterator_t iterator = tree.iterate();

            s32   round      = 0;
            s32   preorder[] = {d, b, a, c, f, e, h, g, i};
            void const* data;
            while (iterator.preorder(tree_t::RIGHT, data))
            {
                CHECK_EQUAL(preorder[round++], *(s32*)data);
            }
            CHECK_EQUAL(9, round);

            while (!tree.clear(data)) {}
        }

        UNITTEST_TEST(void_tree_iterate_sortorder)
        {
            tree_t tree;
            tree.init(ctxt);

            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(tree.insert(&a));
            CHECK_TRUE(tree.insert(&b));
            CHECK_TRUE(tree.insert(&c));
            CHECK_TRUE(tree.insert(&d));
            CHECK_TRUE(tree.insert(&e));
            CHECK_TRUE(tree.insert(&f));
            CHECK_TRUE(tree.insert(&g));
            CHECK_TRUE(tree.insert(&h));
            CHECK_TRUE(tree.insert(&i));

            tree_t::iterator_t iterator = tree.iterate();

            s32   round       = 0;
            s32   sortorder[] = {a, b, c, d, e, f, g, h, i};
            void const* data;
            while (iterator.sortorder(tree_t::RIGHT, data))
            {
                CHECK_EQUAL(sortorder[round++], *(s32*)data);
            }
            CHECK_EQUAL(9, round);

            while (!tree.clear(data)) {}
        }

        UNITTEST_TEST(void_tree_iterate_sortorder_backwards)
        {
            tree_t tree;
            tree.init(ctxt);

            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(tree.insert(&a));
            CHECK_TRUE(tree.insert(&b));
            CHECK_TRUE(tree.insert(&c));
            CHECK_TRUE(tree.insert(&d));
            CHECK_TRUE(tree.insert(&e));
            CHECK_TRUE(tree.insert(&f));
            CHECK_TRUE(tree.insert(&g));
            CHECK_TRUE(tree.insert(&h));
            CHECK_TRUE(tree.insert(&i));

            tree_t::iterator_t iterator = tree.iterate();

            s32   round       = 0;
            s32   sortorder[] = {i, h, g, f, e, d, c, b, a};
            void const* data;
            while (iterator.sortorder(tree_t::LEFT, data))
            {
                CHECK_EQUAL(sortorder[round++], *(s32*)data);
            }
            CHECK_EQUAL(9, round);

            while (!tree.clear(data)) {}
        }

        UNITTEST_TEST(void_tree_iterate_postorder)
        {
            tree_t tree;
            tree.init(ctxt);

            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(tree.insert(&a));
            CHECK_TRUE(tree.insert(&b));
            CHECK_TRUE(tree.insert(&c));
            CHECK_TRUE(tree.insert(&d));
            CHECK_TRUE(tree.insert(&e));
            CHECK_TRUE(tree.insert(&f));
            CHECK_TRUE(tree.insert(&g));
            CHECK_TRUE(tree.insert(&h));
            CHECK_TRUE(tree.insert(&i));

            tree_t::iterator_t iterator = tree.iterate();

            s32 round       = 0;
            s32 postorder[] = {a, c, b, e, g, i, h, f, d};

            void const* data;
            while (iterator.postorder(tree_t::RIGHT, data))
            {
                CHECK_EQUAL(postorder[round++], *(s32*)data);
            }
            CHECK_EQUAL(9, round);

            while (!tree.clear(data)) {}
        }

        UNITTEST_TEST(void_tree_search)
        {
            tree_t tree;
            tree.init(ctxt);

            s32 a = 1;
            s32 b = 2;
            s32 c = 3;
            s32 d = 4;
            s32 e = 5;
            s32 f = 6;
            s32 g = 7;
            s32 h = 8;
            s32 i = 9;

            CHECK_TRUE(tree.insert(&a));
            CHECK_TRUE(tree.insert(&b));
            CHECK_TRUE(tree.insert(&c));
            CHECK_TRUE(tree.insert(&d));
            CHECK_TRUE(tree.insert(&e));
            CHECK_TRUE(tree.insert(&f));
            CHECK_TRUE(tree.insert(&g));
            CHECK_TRUE(tree.insert(&h));
            CHECK_TRUE(tree.insert(&i));

            tree_t::iterator_t iterator = tree.iterate();

            s32   dir = tree_t::LEFT;
            void const* data;
            void* find = &f;
            while (iterator.traverse(dir, data))
            {
                s32 const c = compare_s32(find, data);
                if (c == 0)
                    break;
                dir = iterator.getdir(c);
            }
            CHECK_EQUAL(0, compare_s32(data, find));

            while (!tree.clear(data)) {}
        }

        UNITTEST_TEST(s32_tree)
        {
            tree_t tree;
            tree.init(ctxt);

            s32 a(1);
            s32 b(2);
            s32 c(3);
            s32 d(4);

            const char* result = nullptr;

            bool inserted;
            inserted = tree.insert(&a);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&b);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&b); // Duplicate insert should fail
            CHECK_FALSE(inserted);
            inserted = tree.insert(&c);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));
            inserted = tree.insert(&d);
            CHECK_TRUE(inserted);
            CHECK_TRUE(tree.validate(result));

            CHECK_EQUAL(4, tree.size());

            tree_t::node_t* node = nullptr;
            CHECK_EQUAL(true, tree.find(&a, node));
            CHECK_EQUAL(true, tree.find(&b, node));
            CHECK_EQUAL(true, tree.find(&c, node));
            CHECK_EQUAL(true, tree.find(&d, node));

            s32 e(5);
            CHECK_FALSE(tree.find(&e, node));

            void const* data;
            while (!tree.clear(data)) {}

            CHECK_EQUAL(0, tree.size());
        }
    }
}
UNITTEST_SUITE_END
