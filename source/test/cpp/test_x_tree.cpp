#include "xbase/x_allocator.h"
#include "xbase/x_slice.h"
#include "xbase/x_btree.h"

#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::xalloc* gTestAllocator;

namespace xcore
{
    class xfixedsizealloc : public xfsalloc
    {
        u32     m_alloc_size;
        s32     m_count;
        xalloc* m_allocator;

    public:
        inline xfixedsizealloc()
            : m_alloc_size(0)
            , m_count(0)
            , m_allocator(nullptr)
        {
        }

        s32 count() const { return m_count; }

        void init(u32 alloc_size, xalloc* a)
        {
            m_alloc_size = alloc_size;
            m_count      = 0;
            m_allocator  = a;
        }

        virtual void* allocate()
        {
            m_count++;
            return m_allocator->allocate(sizeof(m_alloc_size), sizeof(void*));
        }
        virtual void deallocate(void* p)
        {
            --m_count;
            return m_allocator->deallocate(p);
        }
        virtual void release() {}
    };

    class xnode_array : public xfsallocdexed
    {
        enum
        {
            Null = 0xffffffff
        };
        struct node
        {
            union {
                u32   children[4];
                node* next;
            };
        };
        node  m_nodes[32768];
        u32   m_direct;
        node* m_head;
        s32   m_count;

    public:
        xnode_array() { reset(); }

        void reset()
        {
            m_direct = 0;
            m_head   = nullptr;
            m_count  = 0;
        }

        s32 count() const { return m_count; }

        virtual void* allocate()
        {
            void* p = nullptr;
            if (m_head == nullptr)
            {
                if (m_direct < sizeof(m_nodes))
                {
                    p = &m_nodes[m_direct];
                    m_direct++;
                    m_count += 1;
                }
            }
            else
            {
                p      = m_head;
                m_head = m_head->next;
                m_count += 1;
            }
            return p;
        }

        virtual void deallocate(void* p)
        {
            x_memset(p, 0xDA, sizeof(node));
            node* n = (node*)p;
            n->next = m_head;
            m_head  = n;
            m_count -= 1;
        }

        virtual void* idx2ptr(u32 index) const { return (void*)&m_nodes[index]; }

        virtual u32 ptr2idx(void* ptr) const
        {
            node const* n     = (node const*)ptr;
            u32 const   index = (u32)(n - m_nodes);
            return index;
        }

        XCORE_CLASS_PLACEMENT_NEW_DELETE

        virtual void release() {}
    };

    struct value_t
    {
        f32 f;
        u64 key;
    };

    class xvalue_array : public xfsallocdexed, public xbtree32::keyvalue
    {
        enum
        {
            Null = 0xffffffff
        };
        struct node
        {
            union {
                value_t value;
                node*   next;
            };
        };
        node  m_nodes[32768];
        u32   m_direct;
        node* m_head;
        s32   m_count;

    public:
        xvalue_array() { reset(); }

        void reset()
        {
            m_direct = 0;
            m_head   = nullptr;
            m_count  = 0;
        }

        s32 count() const { return m_count; }

        virtual void* allocate()
        {
            void* p = nullptr;
            if (m_head == nullptr)
            {
                if (m_direct < sizeof(m_nodes))
                {
                    p = &m_nodes[m_direct];
                    m_direct++;
                    m_count += 1;
                }
            }
            else
            {
                p      = m_head;
                m_head = m_head->next;
                m_count += 1;
            }
            return p;
        }

        virtual void deallocate(void* p)
        {
            x_memset(p, 0xDA, sizeof(node));
            node* n = (node*)p;
            n->next = m_head;
            m_head  = n;
            m_count -= 1;
        }

        virtual void* idx2ptr(u32 index) const { return (void*)&m_nodes[index]; }

        virtual u32 ptr2idx(void* ptr) const
        {
            node const* n     = (node const*)ptr;
            u32 const   index = (u32)(n - m_nodes);
            return index;
        }

        virtual u64 get_key(u32 value) const
        {
            node* n = (node*)idx2ptr(value);
            return n->value.key;
        }

        virtual void set_key(u32 value, u64 key)
        {
            node* n      = (node*)idx2ptr(value);
            n->value.key = key;
        }

        XCORE_CLASS_PLACEMENT_NEW_DELETE

        virtual void release() {}
    };
} // namespace xcore

UNITTEST_SUITE_BEGIN(xbtree)
{
    UNITTEST_FIXTURE(btree32)
    {
        xcore::xnode_array*  nodes  = nullptr;
        xcore::xvalue_array* values = nullptr;

        UNITTEST_FIXTURE_SETUP()
        {
            xheap heap(gTestAllocator);
            nodes  = heap.construct<xcore::xnode_array>();
            values = heap.construct<xcore::xvalue_array>();
        }

        UNITTEST_FIXTURE_TEARDOWN()
        {
            xheap heap(gTestAllocator);
            heap.destruct(nodes);
            heap.destruct(values);
        }

        UNITTEST_TEST(init)
        {
            xbtree32 tree;
            tree.init(nodes, values);

            nodes->reset();
            values->reset();
        }

        UNITTEST_TEST(add)
        {
            xbtree32 tree;
            tree.init_from_mask(nodes, values, 0xFF, true);

            value_t* v1 = (value_t*)values->allocate();
            v1->f       = 1.0f;
            v1->key     = 1;
            value_t* v2 = (value_t*)values->allocate();
            v2->f       = 2.0f;
            v2->key     = 2;

            CHECK_TRUE(tree.add(v1->key, values->ptr2idx(v1)));
            CHECK_TRUE(tree.add(v2->key, values->ptr2idx(v2)));

            CHECK_EQUAL(1, v1->key);
            CHECK_EQUAL(2, v2->key);

            CHECK_EQUAL(4, nodes->count());
            CHECK_EQUAL(2, values->count());

            nodes->reset();
            values->reset();
        }

        UNITTEST_TEST(add_many)
        {
            u32 const value_count = 1024;

            xbtree32 tree;
            tree.init_from_index(nodes, values, value_count);

            for (u32 i = 0; i < value_count; ++i)
            {
                value_t* v = (value_t*)values->allocate();
                v->f       = 1.0f;
                v->key     = value_count + i;
                CHECK_TRUE(tree.add(i, values->ptr2idx(v)));
            }
            for (u32 i = 0; i < value_count; ++i)
            {
                value_t* v = (value_t*)values->idx2ptr(i);
                CHECK_EQUAL(i, v->key);
            }
            CHECK_EQUAL(256 + 64 + 16 + 4 + 1, nodes->count());
            CHECK_EQUAL(value_count, (u32)values->count());

            nodes->reset();
            values->reset();
        }

        UNITTEST_TEST(find_many)
        {
            u32 const value_count = 1024;

            xbtree32 tree;
            tree.init_from_index(nodes, values, value_count);

            for (u32 i = 0; i < value_count; ++i)
            {
                value_t* v = (value_t*)values->allocate();
                v->f       = 1.0f;
                v->key     = value_count + i;
                CHECK_TRUE(tree.add(i, values->ptr2idx(v)));
            }
            for (u32 i = 0; i < value_count; ++i)
            {
                value_t* v = (value_t*)values->idx2ptr(i);
                CHECK_EQUAL(i, v->key);
            }
            CHECK_EQUAL(256 + 64 + 16 + 4 + 1, nodes->count());
            CHECK_EQUAL(value_count, (u32)values->count());

            // One node = 16 bytes
            // Number of nodes = (256 + 64 + 16 + 4 + 1) = 341
            // (341 * 16) / 1024 = 5.328125 bytes per value

            for (u32 i = 0; i < value_count; ++i)
            {
                u32 vi;
                CHECK_TRUE(tree.find(i, vi));
                value_t* v = (value_t*)values->idx2ptr(vi);
                CHECK_NOT_NULL(v);
                if (v != nullptr)
                {
                    CHECK_EQUAL(i, v->key);
                }
            }
            nodes->reset();
            values->reset();
        }

        UNITTEST_TEST(remove)
        {
            u32 const value_count = 1024;

            xbtree32 tree;
            tree.init_from_index(nodes, values, value_count);

            for (u32 i = 0; i < value_count; ++i)
            {
                value_t* v = (value_t*)values->allocate();
                v->f       = 1.0f;
                v->key     = value_count + i;
                CHECK_TRUE(tree.add(i, values->ptr2idx(v)));
            }
            for (u32 i = 0; i < value_count; ++i)
            {
                value_t* v = (value_t*)values->idx2ptr(i);
                CHECK_EQUAL(i, v->key);
            }
            CHECK_EQUAL(256 + 64 + 16 + 4 + 1, nodes->count());
            CHECK_EQUAL(value_count, (u32)values->count());

            // Remove them
            for (u32 i = 0; i < value_count; ++i)
            {
                u32 vi;
                CHECK_TRUE(tree.rem(i, vi));
                value_t* v = (value_t*)values->idx2ptr(vi);
                CHECK_NOT_NULL(v);
                if (v != nullptr)
                {
                    CHECK_EQUAL(i, v->key);
                    values->deallocate(v);
                }
            }

            CHECK_EQUAL(1, nodes->count());
            CHECK_EQUAL(0, (u32)values->count());

            nodes->reset();
            values->reset();
        }
    }

    UNITTEST_FIXTURE(btree)
    {
        class myvalue
        {
        public:
            u64 m_key;
            f32 m_value;

            XCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        static xfixedsizealloc nodes;

        class myvalue_kv : public xbtree::keyvalue
        {
        public:
            virtual u64 get_key(void* value) const
            {
                myvalue* v = (myvalue*)value;
                return v->m_key;
            }

            virtual void set_key(void* value, u64 key) const
            {
                myvalue* v = (myvalue*)value;
                v->m_key   = key;
            }
        };

        static myvalue_kv values;

        UNITTEST_FIXTURE_SETUP() { nodes.init(sizeof(myvalue), gTestAllocator); }

        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(init)
        {
            xbtree tree;
            tree.init(&nodes, &values);
            tree.clear();
        }

        UNITTEST_TEST(add)
        {
            xbtree tree;
            tree.init_from_mask(&nodes, &values, 0xFF, true);

            xheap    heap(gTestAllocator);
            myvalue* v1 = heap.construct<myvalue>();
            v1->m_value = 1.0f;
            v1->m_key   = 0;
            myvalue* v2 = heap.construct<myvalue>();
            v2->m_value = 2.0f;
            v2->m_key   = 0;

            CHECK_TRUE(tree.add(1, v1));
            CHECK_TRUE(tree.add(2, v2));
            CHECK_EQUAL(1, v1->m_key);
            CHECK_EQUAL(2, v2->m_key);

            CHECK_EQUAL(4, nodes.count());

            void* vv1;
            CHECK_TRUE(tree.rem(v1->m_key, vv1));
            heap.destruct(vv1);

            void* vv2;
            CHECK_TRUE(tree.rem(v1->m_key, vv2));
            heap.destruct(vv2);

            tree.clear();
        }
    }
}
UNITTEST_SUITE_END
