#include "cbase/c_allocator.h"
#include "cbase/c_slice.h"
#include "cbase/c_btree.h"
#include "cbase/test_allocator.h"

#include "cunittest/cunittest.h"

using namespace ncore;



namespace ncore
{
    class objects_t : public fsadexed_t
    {
        struct node
        {
            node() {}
            union {
                void* m_node[4];
                node* m_next;
            };
        };

        node  m_nodes[32768];
        u32   m_direct;
        node* m_freelist;
        s32   m_count;

    public:
        inline objects_t() : m_direct(0), m_freelist(nullptr), m_count(0)
        {
        }

        void reset()
        {
            m_direct = 0;
            m_freelist   = nullptr;
            m_count  = 0;
        }

        s32 count() const { return m_count; }

        virtual u32   v_size() const { return sizeof(node); }
        virtual void* v_allocate()
        {
            void* p = nullptr;
            if (m_freelist == nullptr)
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
                p          = m_freelist;
                m_freelist = m_freelist->m_next;
                m_count   += 1;
            }
            return p;
        }

        virtual u32 v_deallocate(void* p)
        {
            nmem::memset(p, 0xDA, sizeof(node));
            node* n = (node*)p;
            n->m_next   = m_freelist;
            m_freelist  = n;
            m_count    -= 1;
            return size();
        }

        virtual void* v_idx2ptr(u32 index) const { return (void*)&m_nodes[index]; }

        virtual u32 v_ptr2idx(void* ptr) const
        {
            node const* n     = (node const*)ptr;
            u32 const   index = (u32)(n - m_nodes);
            return index;
        }

        DCORE_CLASS_PLACEMENT_NEW_DELETE

        virtual void v_release() {}
    };

    struct value_t
    {
        f32 f;
        u64 key;
    };

    class value_kv_t : public btree_idx_kv_t
    {
        objects_t*		m_objects;

    public:
        value_kv_t(objects_t* objects) : m_objects(objects) {}

        virtual u64 get_key(u32 value) const
        {
            value_t* n = (value_t*)m_objects->idx2ptr(value);
            return n->key;
        }

        virtual void set_key(u32 value, u64 key)
        {
            value_t* n = (value_t*)m_objects->idx2ptr(value);
            n->key = key;
        }

        DCORE_CLASS_PLACEMENT_NEW_DELETE
    };

} // namespace ncore

UNITTEST_SUITE_BEGIN(test_btree)
{
    UNITTEST_FIXTURE(btree32)
    {
        UNITTEST_ALLOCATOR;

        ncore::objects_t*  nodes  = nullptr;
        ncore::objects_t* values = nullptr;
        ncore::value_kv_t* value_kv = nullptr;

        UNITTEST_FIXTURE_SETUP()
        {
            nodes  = Allocator->construct<ncore::objects_t>();
            values = Allocator->construct<ncore::objects_t>();
            value_kv = Allocator->construct<ncore::value_kv_t>(values);
        }

        UNITTEST_FIXTURE_TEARDOWN()
        {
            Allocator->destruct(nodes);
            Allocator->destruct(values);
            Allocator->destruct(value_kv);
        }

        UNITTEST_TEST(init)
        {
            btree_idx_t tree;
            tree.init(nodes, value_kv);
            nodes->reset();
            values->reset();
        }

        UNITTEST_TEST(add)
        {
            btree_idx_t tree;
            tree.init_from_mask(nodes, value_kv, 0xFF00, true);

            value_t* v1 = (value_t*)values->allocate();
            v1->f       = 1.0f;
            v1->key     = 0x0100;
            value_t* v2 = (value_t*)values->allocate();
            v2->f       = 2.0f;
            v2->key     = 0x0200;

            CHECK_TRUE(tree.add(v1->key, values->ptr2idx(v1)));
            CHECK_TRUE(tree.add(v2->key, values->ptr2idx(v2)));

            CHECK_EQUAL(0x0100, v1->key);
            CHECK_EQUAL(0x0200, v2->key);

            CHECK_EQUAL(4, nodes->count());
            CHECK_EQUAL(2, values->count());

            nodes->reset();
            values->reset();
        }

        UNITTEST_TEST(add_many)
        {
            u32 const count = 1024;

            btree_idx_t tree;
            tree.init_from_index(nodes, value_kv, count, false);

            for (u32 i = 0; i < count; ++i)
            {
                value_t* v = (value_t*)values->allocate();
                v->f       = 1.0f;
                v->key     = count + i;
                CHECK_TRUE(tree.add(i, values->ptr2idx(v)));
            }
            for (u32 i = 0; i < count; ++i)
            {
                value_t* v = (value_t*)values->idx2ptr(i);
                CHECK_EQUAL(i, v->key);
            }
            CHECK_EQUAL(256 + 64 + 16 + 4 + 1, nodes->count());
            CHECK_EQUAL(count, (u32)values->count());

            nodes->reset();
            values->reset();
        }

        UNITTEST_TEST(find_many)
        {
            u32 const count = 1024;

            btree_idx_t tree;
            tree.init_from_index(nodes, value_kv, count, false);

            for (u32 i = 0; i < count; ++i)
            {
                value_t* v = (value_t*)values->allocate();
                v->f       = 1.0f;
                v->key     = count + i;
                CHECK_TRUE(tree.add(i, values->ptr2idx(v)));
            }
            for (u32 i = 0; i < count; ++i)
            {
                value_t* v = (value_t*)values->idx2ptr(i);
                CHECK_EQUAL(i, v->key);
            }
            CHECK_EQUAL(256 + 64 + 16 + 4 + 1, nodes->count());
            CHECK_EQUAL(count, (u32)values->count());

            // One node = 16 bytes
            // Number of nodes = (256 + 64 + 16 + 4 + 1) = 341
            // 341 * 16 bytes = 5456 bytes
            // 5456 bytes / 1024 items = 5.328125 bytes per item

            for (u32 i = 0; i < count; ++i)
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
            u32 const count = 1024;

            btree_idx_t tree;
            tree.init_from_index(nodes, value_kv, count, false);

            for (u32 i = 0; i < count; ++i)
            {
                value_t* v = (value_t*)values->allocate();
                v->f       = 1.0f;
                v->key     = count + i;
                CHECK_TRUE(tree.add(i, values->ptr2idx(v)));
            }
            for (u32 i = 0; i < count; ++i)
            {
                value_t* v = (value_t*)values->idx2ptr(i);
                CHECK_EQUAL(i, v->key);
            }
            CHECK_EQUAL(256 + 64 + 16 + 4 + 1, nodes->count());
            CHECK_EQUAL(count, (u32)values->count());

            // Remove them
            for (u32 i = 0; i < count; ++i)
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
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        static objects_t nodes;
        static objects_t values;

        class myvalue_kv : public btree_ptr_kv_t
        {
        public:
            virtual u64 get_key(void* value) const
            {
                myvalue* v = (myvalue*)value;
                return v->m_key;
            }

            virtual void set_key(void* value, u64 key)
            {
                myvalue* v = (myvalue*)value;
                v->m_key   = key;
            }
        };

        static myvalue_kv value_kv;

        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(init)
        {
            btree_ptr_t tree;
            tree.init(&nodes, &value_kv);
            btree_ptr_t::node_t* root = nullptr;
            tree.clear(root);
        }

        UNITTEST_TEST(add)
        {
            btree_ptr_t tree;
            tree.init_from_mask(&nodes, &value_kv, 0xFF, true);

            myvalue* v1 = Allocator->construct<myvalue>();
            v1->m_value = 1.0f;
            v1->m_key   = 0;
            myvalue* v2 = Allocator->construct<myvalue>();
            v2->m_value = 2.0f;
            v2->m_key   = 0;

            btree_ptr_t::node_t* root = nullptr;
            CHECK_TRUE(tree.add(root, 1, v1));
            CHECK_TRUE(tree.add(root, 2, v2));
            CHECK_EQUAL(1, v1->m_key);
            CHECK_EQUAL(2, v2->m_key);

            CHECK_EQUAL(4, nodes.count());

            void* vv1;
            CHECK_TRUE(tree.rem(root, v1->m_key, vv1));
            void* vv2;
            CHECK_TRUE(tree.rem(root, v2->m_key, vv2));

            Allocator->destruct<myvalue>((myvalue*)v2);
            Allocator->destruct<myvalue>((myvalue*)v1);

            tree.clear(root);
        }

        UNITTEST_TEST(add_many)
        {
            u32 const count = 1024;

            btree_ptr_t tree;
            tree.init_from_index(&nodes, &value_kv, count, false);

            btree_ptr_t::node_t* root = nullptr;

            u64     keys[count];
            u64     seed = 0;
            for (s32 i = 0; i < count; ++i)
            {
                seed    = seed * 1664525UL + 1013904223UL;
                keys[i] = seed;
            }

            for (u32 i = 0; i < count; ++i)
            {
                myvalue* v  = values.construct<myvalue>();
                v->m_value  = 1.0f;
                v->m_key    = keys[i];
                CHECK_TRUE(tree.add(root, i, v));
            }
            for (u32 i = 0; i < count; ++i)
            {
                myvalue* v = (myvalue*)values.idx2ptr(i);
                CHECK_EQUAL(i, v->m_key);
            }

            CHECK_EQUAL(256 + 64 + 16 + 4 + 1, nodes.count());
            CHECK_EQUAL(count, (u32)values.count());

            nodes.reset();
            values.reset();
        }

        UNITTEST_TEST(find_many)
        {
            u32 const count = 1024;

            btree_ptr_t tree;
            tree.init_from_index(&nodes, &value_kv, count, false);

            btree_ptr_t::node_t* root = nullptr;

            CHECK_EQUAL(0, nodes.count());
            for (u32 i = 0; i < count; ++i)
            {
                myvalue* v  = values.construct<myvalue>();
                v->m_value   = 1.0f;
                v->m_key     = count + i;
                CHECK_TRUE(tree.add(root, i, v));
            }
            for (u32 i = 0; i < count; ++i)
            {
                myvalue* v = (myvalue*)values.idx2ptr(i);
                CHECK_EQUAL(i, v->m_key);
            }
            CHECK_EQUAL(256 + 64 + 16 + 4 + 1, nodes.count());
            CHECK_EQUAL(count, (u32)values.count());

            // One node = 32 bytes
            // Number of nodes = (256 + 64 + 16 + 4 + 1) = 341
            // 341 * 32 bytes = 10912 bytes
            // 10912 bytes / 1024 items = 10.65625 bytes per item

            for (u32 i = 0; i < count; ++i)
            {
                void* vi;
                CHECK_TRUE(tree.find(root, i, vi));
                myvalue* v = (myvalue*)vi;
                CHECK_NOT_NULL(v);
                if (v != nullptr)
                {
                    CHECK_EQUAL(i, v->m_key);
                }
            }
            nodes.reset();
            values.reset();
        }

        UNITTEST_TEST(remove)
        {
            u32 const count = 1024;

            btree_ptr_t tree;
            tree.init_from_index(&nodes, &value_kv, count, false);

            btree_ptr_t::node_t* root = nullptr;

            u64     keys[count];
            u64     seed = 0;
            for (s32 i = 0; i < count; ++i)
            {
                seed    = seed * 1664525UL + 1013904223UL;
                keys[i] = seed;
            }

            for (u32 i = 0; i < count; ++i)
            {
                myvalue* v  = values.construct<myvalue>();
                v->m_value   = 1.0f;
                v->m_key     = keys[i];
                CHECK_TRUE(tree.add(root, keys[i], v));
            }
            for (u32 i = 0; i < count; ++i)
            {
                myvalue* v = (myvalue*)values.idx2ptr(i);
                CHECK_EQUAL(keys[i], v->m_key);
            }
            CHECK_EQUAL(256 + 64 + 16 + 4 + 1, nodes.count());
            CHECK_EQUAL(count, (u32)values.count());

            // Should we randomize the key array?

            // Remove them
            for (u32 i = 0; i < count; ++i)
            {
                void* vi;
                CHECK_TRUE(tree.rem(root, keys[i], vi));
                myvalue* v = (myvalue*)(vi);
                CHECK_NOT_NULL(v);
                if (v != nullptr)
                {
                    CHECK_EQUAL(keys[i], v->m_key);
                    values.deallocate(v);
                }
            }

            CHECK_EQUAL(1, nodes.count());
            CHECK_EQUAL(0, (u32)values.count());

            nodes.reset();
            values.reset();
        }

        UNITTEST_TEST(insert_random)
        {
            u32 const count = 16378;

            btree_ptr_t tree;
            tree.init_from_mask(&nodes, &value_kv, 0xffffffff, true);

            btree_ptr_t::node_t* root = nullptr;

            u32 seed = 0;
            for (u32 i = 0; i < count; ++i)
            {
                myvalue* v  = values.construct<myvalue>();

                seed = seed * 1664525 + 1013904223; 

                v->m_value   = 1.0f;
                v->m_key     = seed;

                CHECK_TRUE(tree.add(root, seed, v));
            }

            CHECK_EQUAL(11853, nodes.count());

            // One node = 32 bytes
            // Number of nodes = 11853
            // 11853 * 32 bytes = 379296 bytes
            // 379296 bytes / 16378 items = 23.158872 bytes per item

            nodes.reset();
            values.reset();
        }
    }
}
UNITTEST_SUITE_END
