#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_tree.h"
#include "cbase/test_allocator.h"

#include "cunittest/cunittest.h"

using namespace ncore;

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

class ctxt_tree_t : public ncore::ntree::tree_t
{
    alloc_t* m_allocator;
    s32      m_size;
    s32      m_cap;

    struct node16_t
    {
        s32 m_data;
        u16 m_color;
        u16 m_branches[3];
    };

    node16_t* m_nodes;
    s32       m_freeindex;
    node16_t* m_freelist;
    s32*      m_keys;
    node16_t* m_nill;
    node16_t* m_root;

public:
    ctxt_tree_t()
        : m_allocator(nullptr)
        , m_size(0)
        , m_cap(0)
        , m_nodes(nullptr)
        , m_freeindex(0)
        , m_freelist(nullptr)
        , m_keys(nullptr)
        , m_nill(nullptr)
        , m_root(nullptr)
    {
    }

    void init(alloc_t* allocator)
    {
        m_allocator                      = allocator;
        m_size                           = 0;
        m_cap                            = 8192;
        m_nodes                          = (node16_t*)m_allocator->allocate(sizeof(node16_t) * m_cap);
        m_freeindex                      = 0;
        m_freelist                       = nullptr;
        m_keys                           = (s32*)m_allocator->allocate(sizeof(s32) * m_cap);
        m_nodes[m_cap - 1].m_branches[0] = 0xffff;
        m_nodes[m_cap - 1].m_branches[1] = 0xffff;
        m_nodes[m_cap - 1].m_branches[2] = 0xffff;

        m_root = (node16_t*)v_new_node(nullptr, nullptr);
        m_nill = (node16_t*)v_new_node(nullptr, nullptr);

        ntree::node_t*& root = (ntree::node_t*&)m_root;
        ntree::node_t*& nill = (ntree::node_t*&)m_nill;
        ntree::initialize(this, nill, root);
    }

    void exit()
    {
        m_allocator->deallocate(m_nodes);
        m_nodes = nullptr;
        m_allocator->deallocate(m_keys);
        m_keys = nullptr;
    }

    virtual s32            v_size() const final { return m_size; }
    virtual s32            v_capacity() const final { return m_cap; }
    virtual ntree::node_t* v_get_nill() const final { return (ntree::node_t*)m_nill; }
    virtual ntree::node_t* v_get_root() const final { return (ntree::node_t*)m_root; }

    virtual void           v_set_color(ntree::node_t* node, ntree::color_e color) final { ((node16_t*)node)->m_color = color; }
    virtual ntree::color_e v_get_color(ntree::node_t const* node) const final { return (ntree::color_e)((node16_t const*)node)->m_color; }
    virtual void const*    v_get_key(ntree::node_t const* node) const final
    {
        u16 const index = (u16)((node16_t const*)node - m_nodes);
        return (void const*)&m_keys[index];
    }
    virtual void const* v_get_value(ntree::node_t const* node) const final
    {
        u16 const index = (u16)((node16_t const*)node - m_nodes);
        return (void const*)&m_keys[index];
    }
    virtual ntree::node_t* v_get_node(ntree::node_t const* node, ntree::node_e ne) const final { return (ntree::node_t*)(&m_nodes[((node16_t const*)node)->m_branches[ne]]); }
    virtual void           v_set_node(ntree::node_t* node, ntree::node_e ne, ntree::node_t* set) final { ((node16_t*)node)->m_branches[ne] = (u16)((node16_t*)set - m_nodes); }
    virtual ntree::node_t* v_new_node(void const* key, void const* value) final
    {
        node16_t* node = nullptr;
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
        m_keys[index]       = key != nullptr ? *((const s32*)key) : 0;
        node->m_color       = ntree::RED;
        node->m_data        = index;
        node->m_branches[0] = 0;
        node->m_branches[1] = 0;
        node->m_branches[2] = 0;
        m_size++;
        return (ntree::node_t*)node;
    }
    virtual void v_del_node(ntree::node_t* node)
    {
        if (node == nullptr)
            return;
        node16_t* n      = (node16_t*)node;
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
            m_freelist = nullptr;
            m_freeindex = 0;
        }
    }
    virtual s32 v_compare_nodes(ntree::node_t const* node, ntree::node_t const* other) const { return compare_s32(v_get_key(node), v_get_key(other)); }
    virtual s32 v_compare_insert(void const* key, ntree::node_t const* node) const { return compare_s32(key, v_get_key(node)); }
};

UNITTEST_SUITE_BEGIN(test_tree)
{
    UNITTEST_FIXTURE(main)
    {
        static ctxt_tree_t    ctxt_instance;
        static ntree::tree_t* ctxt = &ctxt_instance;

        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() { ctxt_instance.init(Allocator); }
        UNITTEST_FIXTURE_TEARDOWN() { ctxt_instance.exit(); }

        UNITTEST_TEST(tree_node)
        {
            s32 const key = 0;

            ntree::node_t* node  = ctxt->v_new_node(&key, nullptr);
            ntree::node_t* left  = ctxt->v_new_node(&key, nullptr);
            ntree::node_t* right = ctxt->v_new_node(&key, nullptr);

            ntree::node_t* left_ptr = left;
            // node->set_left(ctxt, left_ptr);
            ctxt->v_set_node(node, ntree::LEFT, left_ptr);

            // CHECK_EQUAL(left_ptr, node->get_left(ctxt));
            CHECK_EQUAL(left_ptr, ctxt->v_get_node(node, ntree::LEFT));

            ntree::node_t* right_ptr = right;
            // node->set_right(ctxt, right_ptr);
            ctxt->v_set_node(node, ntree::RIGHT, right_ptr);
            // CHECK_EQUAL(right_ptr, node->get_right(ctxt));
            CHECK_EQUAL(right_ptr, ctxt->v_get_node(node, ntree::RIGHT));

            // CHECK_EQUAL(true, node->is_red(ctxt));
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree::RED);

            // node->set_black(ctxt);
            // CHECK_EQUAL(false, node->is_red(ctxt));
            // node->set_red(ctxt);
            // CHECK_EQUAL(true, node->is_red(ctxt));
            // node->set_black(ctxt);
            // CHECK_EQUAL(false, node->is_red(ctxt));
            // CHECK_EQUAL(true, node->is_black(ctxt));
            ctxt->v_set_color(node, ntree::BLACK);
            CHECK_EQUAL(false, ctxt->v_get_color(node) == ntree::RED);
            ctxt->v_set_color(node, ntree::RED);
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree::RED);
            ctxt->v_set_color(node, ntree::BLACK);
            CHECK_EQUAL(false, ctxt->v_get_color(node) == ntree::RED);
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree::BLACK);

            left_ptr  = left;
            right_ptr = right;
            // node->set_left(ctxt, left_ptr);
            // node->set_right(ctxt, right_ptr);
            // CHECK_EQUAL(left_ptr, node->get_child(ctxt, 0));
            // CHECK_EQUAL(right_ptr, node->get_child(ctxt, 1));
            // CHECK_EQUAL(true, node->is_black(ctxt));
            ctxt->v_set_node(node, ntree::LEFT, left_ptr);
            ctxt->v_set_node(node, ntree::RIGHT, right_ptr);
            CHECK_EQUAL(left_ptr, ctxt->v_get_node(node, ntree::LEFT));
            CHECK_EQUAL(right_ptr, ctxt->v_get_node(node, ntree::RIGHT));
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree::BLACK);

            // node->set_child(ctxt, 0, left_ptr);
            // node->set_child(ctxt, 1, right_ptr);
            ctxt->v_set_node(node, ntree::LEFT, left_ptr);
            ctxt->v_set_node(node, ntree::RIGHT, right_ptr);

            // CHECK_EQUAL(left_ptr, node->get_child(ctxt, 0));
            // CHECK_EQUAL(right_ptr, node->get_child(ctxt, 1));
            CHECK_EQUAL(left_ptr, ctxt->v_get_node(node, ntree::LEFT));
            CHECK_EQUAL(right_ptr, ctxt->v_get_node(node, ntree::RIGHT));

            // CHECK_EQUAL(true, node->is_black(ctxt));
            CHECK_EQUAL(true, ctxt->v_get_color(node) == ntree::BLACK);

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

            const char* result = nullptr;

            bool inserted;
            inserted = ntree::insert(ctxt, &a);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &b);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &b);  // Duplicate insert should fail
            CHECK_FALSE(inserted);
            inserted = ntree::insert(ctxt, &c);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &d);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &e);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &f);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &g);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &h);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &i);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));

            CHECK_EQUAL(9, ntree::size(ctxt));

            ntree::node_t* node = nullptr;
            CHECK_EQUAL(true, ntree::find(ctxt, &a, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &b, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &c, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &d, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &e, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &f, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &g, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &h, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &i, node));

            s32 x(99);
            CHECK_FALSE(ntree::find(ctxt, &x, node));

            CHECK_EQUAL(9, ntree::size(ctxt));

            while (!ntree::clear(ctxt)) {}

            CHECK_EQUAL(0, ntree::size(ctxt));
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

            CHECK_TRUE(ntree::insert(ctxt, &a));
            CHECK_TRUE(ntree::insert(ctxt, &b));
            CHECK_TRUE(ntree::insert(ctxt, &c));
            CHECK_TRUE(ntree::insert(ctxt, &d));
            CHECK_TRUE(ntree::insert(ctxt, &e));
            CHECK_TRUE(ntree::insert(ctxt, &f));
            CHECK_TRUE(ntree::insert(ctxt, &g));
            CHECK_TRUE(ntree::insert(ctxt, &h));
            CHECK_TRUE(ntree::insert(ctxt, &i));

            ntree::iterator_t iterator = ntree::iterate(ctxt);

            s32         round      = 0;
            s32         preorder[] = {d, b, a, c, f, e, h, g, i};
            void const* data;
            while (iterator.preorder(ntree::RIGHT, data))
            {
                CHECK_EQUAL(preorder[round++], *(s32*)data);
            }
            CHECK_EQUAL(9, round);

            while (!ntree::clear(ctxt)) {}
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

            CHECK_TRUE(ntree::insert(ctxt, &a));
            CHECK_TRUE(ntree::insert(ctxt, &b));
            CHECK_TRUE(ntree::insert(ctxt, &c));
            CHECK_TRUE(ntree::insert(ctxt, &d));
            CHECK_TRUE(ntree::insert(ctxt, &e));
            CHECK_TRUE(ntree::insert(ctxt, &f));
            CHECK_TRUE(ntree::insert(ctxt, &g));
            CHECK_TRUE(ntree::insert(ctxt, &h));
            CHECK_TRUE(ntree::insert(ctxt, &i));

            ntree::iterator_t iterator = ntree::iterate(ctxt);

            s32         round       = 0;
            s32         sortorder[] = {a, b, c, d, e, f, g, h, i};
            void const* data;
            while (iterator.sortorder(ntree::RIGHT, data))
            {
                CHECK_EQUAL(sortorder[round++], *(s32*)data);
            }
            CHECK_EQUAL(9, round);

            while (!ntree::clear(ctxt)) {}
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

            CHECK_TRUE(ntree::insert(ctxt, &a));
            CHECK_TRUE(ntree::insert(ctxt, &b));
            CHECK_TRUE(ntree::insert(ctxt, &c));
            CHECK_TRUE(ntree::insert(ctxt, &d));
            CHECK_TRUE(ntree::insert(ctxt, &e));
            CHECK_TRUE(ntree::insert(ctxt, &f));
            CHECK_TRUE(ntree::insert(ctxt, &g));
            CHECK_TRUE(ntree::insert(ctxt, &h));
            CHECK_TRUE(ntree::insert(ctxt, &i));

            ntree::iterator_t iterator = ntree::iterate(ctxt);

            s32         round       = 0;
            s32         sortorder[] = {i, h, g, f, e, d, c, b, a};
            void const* data;
            while (iterator.sortorder(ntree::LEFT, data))
            {
                CHECK_EQUAL(sortorder[round++], *(s32*)data);
            }
            CHECK_EQUAL(9, round);

            while (!ntree::clear(ctxt)) {}
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

            CHECK_TRUE(ntree::insert(ctxt, &a));
            CHECK_TRUE(ntree::insert(ctxt, &b));
            CHECK_TRUE(ntree::insert(ctxt, &c));
            CHECK_TRUE(ntree::insert(ctxt, &d));
            CHECK_TRUE(ntree::insert(ctxt, &e));
            CHECK_TRUE(ntree::insert(ctxt, &f));
            CHECK_TRUE(ntree::insert(ctxt, &g));
            CHECK_TRUE(ntree::insert(ctxt, &h));
            CHECK_TRUE(ntree::insert(ctxt, &i));

            ntree::iterator_t iterator = ntree::iterate(ctxt);

            s32 round       = 0;
            s32 postorder[] = {a, c, b, e, g, i, h, f, d};

            void const* data;
            while (iterator.postorder(ntree::RIGHT, data))
            {
                CHECK_EQUAL(postorder[round++], *(s32*)data);
            }
            CHECK_EQUAL(9, round);

            while (!ntree::clear(ctxt)) {}
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

            CHECK_TRUE(ntree::insert(ctxt, &a));
            CHECK_TRUE(ntree::insert(ctxt, &b));
            CHECK_TRUE(ntree::insert(ctxt, &c));
            CHECK_TRUE(ntree::insert(ctxt, &d));
            CHECK_TRUE(ntree::insert(ctxt, &e));
            CHECK_TRUE(ntree::insert(ctxt, &f));
            CHECK_TRUE(ntree::insert(ctxt, &g));
            CHECK_TRUE(ntree::insert(ctxt, &h));
            CHECK_TRUE(ntree::insert(ctxt, &i));

            ntree::iterator_t iterator = ntree::iterate(ctxt);

            s32         dir = ntree::LEFT;
            void const* data;
            void*       find = &f;
            while (iterator.traverse(dir, data))
            {
                s32 const c = compare_s32(find, data);
                if (c == 0)
                    break;
                dir = iterator.getdir(c);
            }
            CHECK_EQUAL(0, compare_s32(data, find));

            while (!ntree::clear(ctxt)) {}
        }

        UNITTEST_TEST(s32_tree)
        {
            s32 a(1);
            s32 b(2);
            s32 c(3);
            s32 d(4);

            const char* result = nullptr;

            bool inserted;
            inserted = ntree::insert(ctxt, &a);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &b);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &b);  // Duplicate insert should fail
            CHECK_FALSE(inserted);
            inserted = ntree::insert(ctxt, &c);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));
            inserted = ntree::insert(ctxt, &d);
            CHECK_TRUE(inserted);
            CHECK_TRUE(ntree::validate(ctxt, result));

            CHECK_EQUAL(4, ntree::size(ctxt));

            ntree::node_t* node = nullptr;
            CHECK_EQUAL(true, ntree::find(ctxt, &a, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &b, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &c, node));
            CHECK_EQUAL(true, ntree::find(ctxt, &d, node));

            s32 e(5);
            CHECK_FALSE(ntree::find(ctxt, &e, node));

            while (!ntree::clear(ctxt)) {}

            CHECK_EQUAL(0, ntree::size(ctxt));
        }
    }
}
UNITTEST_SUITE_END
