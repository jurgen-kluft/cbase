#define TEST_TRIEMAP
#ifdef TEST_TRIEMAP

#include "cbase/c_target.h"
#include "cbase/c_debug.h"
#include "cbase/c_allocator.h"
#include "cbase/c_random.h"

#include "cbase/c_integer.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"

#include "cunittest/cunittest.h"

namespace ncore
{
    // Note:
    // Using a FSA allocator we can change the pointers into u32 indices.
    // Making the run maximum 32-bit and splitting off the run into its own u32.

    typedef u64 key_t;
    typedef u32 val_t;

    static inline bool  helper_is_bit_set(key_t data, s32 bit) { return (data & ((key_t)1 << bit)) != 0; }
    static inline key_t helper_get_run_keybits(key_t key, s32 pos, s32 len)
    {
        key_t const mask = (((key_t)1 << len) - 1) << ((sizeof(key_t) * 8) - (pos + len));
        key              = key & mask;
        return key;
    }

    struct value_t;

    struct node_t
    {
        // Could go as high as 7 bits for POS and LEN, so maximum key bitcnt is 128 bits.
        enum
        {
            KEY_MAX_BITCNT = 64,
            BRANCH_NUMBITS = 1,
            POS_BITCNT     = 6,
            POS_SHIFT      = 0,
            POS_MASK       = 0x3F,
            RUN_BITCNT     = 6,
            RUN_SHIFT      = 0
        };
        enum
        {
            CHILD_TYPE_BITCNT = 2,
            CHILD_TYPE_SHIFT  = 6,
            CHILD_TYPE_VALUE  = 0,
            CHILD_TYPE_NODE   = 1,
            CHILD_TYPE_CHILD  = 0x40,
            CHILD_TYPE_MASK   = 0xC0
        };

        inline node_t()
            : m_run(0)
            , m_pos(0)
        {
            m_children[0] = nullptr;
            m_children[1] = nullptr;
        }

        static inline s32 run_max_numbits() { return KEY_MAX_BITCNT; }

        inline s32   get_run_bitpos() const { return (m_pos & POS_MASK); }
        inline s32   get_run_bitlen() const { return m_run; }
        inline key_t get_run_keymask() const { return (((key_t)1 << get_run_bitlen()) - 1) << (KEY_MAX_BITCNT - (get_run_bitpos() + get_run_bitlen())); }
        inline key_t get_run_keybits() const { return helper_get_run_keybits(m_key, get_run_bitpos(), get_run_bitlen()); }
        inline s32   get_run_endpos() const { return get_run_bitpos() + get_run_bitlen() + BRANCH_NUMBITS; } // The start pos for the child nodes

        // Which bit(s) determines the child index ?
        inline s32  get_child_bit() const { return (KEY_MAX_BITCNT - (get_run_bitpos() + get_run_bitlen() + BRANCH_NUMBITS)); }
        inline u8   get_child_type(s8 child) const { return (((m_pos & CHILD_TYPE_MASK) >> CHILD_TYPE_SHIFT) >> child) & 1; }
        inline void set_child_type(s8 child, u8 type)
        {
            m_pos = m_pos & ~((u8)CHILD_TYPE_CHILD << child);
            m_pos = m_pos | ((u8)type << (CHILD_TYPE_SHIFT + child));
        }

        inline void set_run_bitpos(u32 pos) { m_pos = (m_pos & CHILD_TYPE_MASK) | (pos & POS_MASK); }
        inline void set_run_bitlen(u32 len) { m_run = len; }
        inline void set_run_bits(key_t bits) { m_key = bits; }

        inline s8 get_indexof_child(void* vvalue)
        {
            s8 const i = (m_children[0] == vvalue) ? 0 : 1;
            ASSERT(m_children[i] == vvalue);
            return i;
        }

        inline node_t* get_child_as_node(s8 child) const
        {
            ASSERT(get_child_type(child) == CHILD_TYPE_NODE);
            return m_children[child];
        }
        inline value_t* get_child_as_value(s8 child) const
        {
            ASSERT(get_child_type(child) == CHILD_TYPE_VALUE);
            return (value_t*)(m_children[child]);
        }

        inline void set_child_as_value(s8 child, value_t* cvalue)
        {
            m_children[child] = (node_t*)(cvalue);
            set_child_type(child, CHILD_TYPE_VALUE);
        }
        inline void set_child_as_node(s8 child, node_t* cnode)
        {
            m_children[child] = cnode;
            set_child_type(child, CHILD_TYPE_NODE);
        }

        XCORE_CLASS_PLACEMENT_NEW_DELETE

        // 32 bytes (can be 24 bytes if key == u32)
        key_t   m_key;
        u8      m_run, m_pos;
        node_t* m_children[2];

        /* 16 bytes (this is really the smallest we can get and with an 48 bits key size)
        u8      m_key[6];
        u8      m_run, m_pos;
        u32     m_children[2];
        */
    };

    // Virtual Memory Approach
    // Node array is using a virtual address range
    // Value array is using a virtual address range

    // A more optimized layout and size (16) would be:
    // struct
    // {
    //    u32  m_key;
    //    u32  m_data;
    //    u32  m_children[2];
    // }

    // Currently we are storing bit-pos and run-len. However you do not really need
    // run-len. If every next node in the tree just indicates the bit-pos then the
    // run-len is obvious. bit-pos only needs 6 bits for a 64-bit key or 5 bits for
    // a 32-bit key. The children both need 1 bit to indicate if they are a value
    // or a node. So for data we only need 5/6 + 2 = 7/8 bits.

    struct value_t
    {
        inline value_t(key_t key, val_t value)
            : m_key(key)
            , m_value(value)
        {
        }
        key_t m_key;
        val_t m_value;
        XCORE_CLASS_PLACEMENT_NEW_DELETE
    };

    // Trie data structure where every node can cover a range of bits (not only 1 bit)
    class dtrie_t
    {
    public:
        inline dtrie_t(fsa_t* fsa)
            : m_root(nullptr)
            , m_value(nullptr)
            , m_num_values(0)
            , m_num_nodes(0)
            , m_nodes(fsa)
            , m_values(fsa)
        {
        }

        bool insert(key_t key, val_t value);
        bool find(key_t key, val_t& value);
        bool remove(key_t key, val_t& value);

    protected:
        node_t* branch_two_values(node_t* parent, value_t* v1, value_t* v2, s8 pos);

        node_t*  m_root;
        value_t* m_value;
        s32      m_num_values;
        s32      m_num_nodes;
        fsa_t*   m_nodes;
        fsa_t*   m_values;
    };

    node_t* dtrie_t::branch_two_values(node_t* parent, value_t* v1, value_t* v2, s8 pos)
    {
        // Determine the length of the run (pos = start position (from msb to lsb))
        key_t const mask = (0xFFFFFFFFFFFFFFFFUL >> pos);
        key_t const diff = (v1->m_key ^ v2->m_key) & mask;
        s32 const   len  = math::countLeadingZeros(diff) - pos;

        s32     p    = pos;
        s32     l    = len;
        node_t* head = parent;
        if (l > node_t::run_max_numbits())
        {
            while (l > node_t::run_max_numbits())
            {
                // We need to create nodes to cover the identical run.
                // Could take more than 1 node!
                m_num_nodes++;
                node_t* intnode = m_nodes->construct<node_t>();

                intnode->set_run_bitpos(p);
                intnode->set_run_bitlen(node_t::run_max_numbits());
                intnode->set_run_bits(v1->m_key);

                if (parent != nullptr)
                {
                    s32 childbit = parent->get_child_bit();
                    parent->set_child_as_node(helper_is_bit_set(v1->m_key, childbit) ? 1 : 0, intnode);
                }
                else
                {
                    head = intnode;
                }

                parent = intnode;
                l -= (node_t::run_max_numbits() + node_t::BRANCH_NUMBITS); // The +1 is for the bit used for determining the branch (left=0, right=1)
                p += (node_t::run_max_numbits() + node_t::BRANCH_NUMBITS); // Move the position further by the running length and the branch bit
            }
        }

        m_num_nodes++;
        node_t* n = m_nodes->construct<node_t>();
        n->set_run_bitpos(p);
        n->set_run_bitlen(l);
        n->set_run_bits(v1->m_key);

        if (parent != nullptr)
        {
            s32 childbit = parent->get_child_bit();
            parent->set_child_as_node(helper_is_bit_set(v1->m_key, childbit) ? 1 : 0, n);
        }
        else
        {
            head = n;
        }

        // Set the 2 values as children
        s32 const cbit = n->get_child_bit();
        s32 const cidx = (helper_is_bit_set(v1->m_key, cbit)) ? 1 : 0;
        n->set_child_as_value(cidx, v1);
        n->set_child_as_value(1 - cidx, v2);

        return head;
    }

    bool dtrie_t::insert(key_t key, val_t value)
    {
        // Before creating the first node (root) we wait until we have 2 elements
        if (m_root == nullptr)
        {
            if (m_value == nullptr)
            {
                m_num_values++;
                m_value = m_values->construct<value_t>(key, value);
            }
            else if (m_value->m_key != key)
            {
                m_num_values++;
                value_t* othervalue = m_values->construct<value_t>(key, value);
                m_root              = branch_two_values(nullptr, m_value, othervalue, 0);
                m_value             = nullptr;
            }
            else
            {
                // Override the value
                m_value->m_value = value;
            }
            return true;
        }

        // Find the node where we should branch off
        node_t* parent = nullptr;
        s32     child  = 0;
        node_t* n      = m_root;
        while (n != nullptr)
        {
            // The run should be the same
            s32 const runlen = n->get_run_bitlen();
            if (runlen > 0)
            {
                key_t const nbits = n->get_run_keybits();
                key_t const kbits = helper_get_run_keybits(key, n->get_run_bitpos(), runlen);
                if (kbits != nbits)
                {
                    // Case 1: We have a split somewhere in this run. For this we
                    //         need to create a new node which will take the head
                    //         of the run and the bit for the branching.
                    //         Node 'n' will take the rest part of the run.
                    //		   The inserted node will have a child for node 'n' and
                    //         a child for new value_t(key-value).

                    // + New node
                    // + Determine head run
                    // + Determine the child where the current node is going, set it
                    // + The other child is for the new value
                    m_num_nodes++;
                    node_t* const sn = m_nodes->construct<node_t>();

                    // u64 const diff = (kbits ^ nbits);
                    // s32 const len = xcountLeadingZeros(diff) - n->get_run_pos();

                    // Determine the length of the run (pos = start position (from msb to lsb))
                    s32 const   span = n->get_run_bitlen();
                    s32 const   pos  = n->get_run_bitpos();
                    key_t const mask = n->get_run_keymask();
                    key_t const diff = n->get_run_keybits() ^ (key & mask);
                    s32 const   len  = math::countLeadingZeros(diff) - pos;

                    sn->set_run_bitpos(pos);
                    sn->set_run_bitlen(len);
                    sn->set_run_bits(key);

                    m_num_values++;
                    value_t* const v  = m_values->construct<value_t>(key, value);
                    s32 const      sc = helper_is_bit_set(key, sn->get_child_bit()) ? 1 : 0;
                    sn->set_child_as_value(sc, v);
                    sn->set_child_as_node(1 - sc, n);

                    // Modify the run pos, run length of n
                    n->set_run_bitpos(sn->get_run_endpos());
                    n->set_run_bitlen(span - (len + 1));

                    // If parent != nullptr then we should replace child 'n' with 'sn'
                    // If parent == nullptr then it means we should update 'root'
                    if (parent != nullptr)
                    {
                        parent->set_child_as_node(child, sn);
                    }
                    else
                    {
                        m_root = sn;
                    }

                    return true;
                }
            }

            // Keep tracking parent of node
            // Determine left/right child
            parent = n;
            child  = (key >> n->get_child_bit()) & 1;

            // Determine type-of child
            if (n->get_child_type(child) == node_t::CHILD_TYPE_NODE)
            { // Branch is a node, traverse further
                n = n->get_child_as_node(child);
            }
            else
            { // Branch is a value, see if this is the same key-value
                value_t* const v1 = n->get_child_as_value(child);
                if (v1->m_key == key)
                {
                    // Override the value
                    value = v1->m_value;
                    return true;
                }

                // Case 2: At this point we need to determine where these 2
                //         values differ in the run that is left.
                //         First create a node and replace the child, then
                //         determine the run for the node. Another node
                //         might have to be created if the bit pos is
                //         beyond the maximum length of a node run.

                m_num_values++;
                value_t* const v2 = m_values->construct<value_t>(key, value);
                branch_two_values(n, v1, v2, n->get_run_endpos());
                return true;
            }
        }

        return false;
    }

    bool dtrie_t::find(key_t key, val_t& value)
    {
        if (m_value != nullptr)
        {
            if (m_value->m_key == key)
            {
                value = m_value->m_value;
                return true;
            }
        }

        node_t* n = m_root;
        while (n != nullptr)
        {
            // The run should be the same
            s32 const runlen = n->get_run_bitlen();
            if (runlen > 0)
            {
                key_t const nbits = n->get_run_keybits();
                key_t const kbits = helper_get_run_keybits(key, n->get_run_bitpos(), runlen);
                if (kbits != nbits)
                    break;
            }

            // Determine left/right child
            s32 const cbit  = n->get_child_bit();
            s32 const child = helper_is_bit_set(key, cbit) ? 1 : 0;

            // Determine type-of child
            if (n->get_child_type(child) == node_t::CHILD_TYPE_NODE)
            { // Branch is a node, traverse further
                n = n->get_child_as_node(child);
            }
            else
            { // Branch is a value, see if this is the same key-value
                value_t* cvalue = n->get_child_as_value(child);
                if (cvalue->m_key == key)
                {
                    value = cvalue->m_value;
                    return true;
                }
                break;
            }
        }

        return false;
    }

    bool dtrie_t::remove(key_t key, val_t& value)
    {
        // Find the value by traversing the trie and keep track of parent
        //
        // When removing the value the following 2 cases can apply:
        //
        // 1. Node has 2 children, both are values:
        //    The value that is left, replace the 'node' child of the parent with the 'value'
        //    Delete this node.
        // 2. Node has 2 children, one is a value, one is a node:
        //    Remove the value.
        //    Merge this 'node' with the child 'node'.
        //    Delete this node.

        if (m_value != nullptr)
        {
            if (m_value->m_key == key)
            {
                m_num_values--;
                value = m_value->m_value;
                m_values->destruct(m_value);
                m_value = nullptr;
                return true;
            }
        }

        node_t* p = nullptr;
        node_t* n = m_root;
        while (n != nullptr)
        {
            // The run should be the same
            s32 const runlen = n->get_run_bitlen();
            if (runlen > 0)
            {
                key_t const nbits = n->get_run_keybits();
                key_t const kbits = helper_get_run_keybits(key, n->get_run_bitpos(), runlen);
                if (kbits != nbits)
                    break;
            }

            // Determine left/right child
            s32 const cbit  = n->get_child_bit();
            s32 const child = helper_is_bit_set(key, cbit) ? 1 : 0;

            // Determine type-of child
            if (n->get_child_type(child) == node_t::CHILD_TYPE_NODE)
            {
                // Keep track of parent
                p = n;
                // Branch is a node, traverse further
                n = n->get_child_as_node(child);
            }
            else
            { // Branch is a value, see if this is the key
                value_t* cvalue = n->get_child_as_value(child);
                if (cvalue->m_key == key)
                {
                    // Found it, now remove this node
                    u8 const ctype = n->get_child_type(1 - child);

                    if (ctype == node_t::CHILD_TYPE_VALUE)
                    {
                        // Case 1: node 'n' has two values
                        value_t* ovalue = n->get_child_as_value(1 - child);
                        if (p != nullptr)
                        {
                            s8 const pc = p->get_indexof_child(n);
                            p->set_child_as_value(pc, ovalue);
                        }
                        else
                        {
                            m_root  = nullptr;
                            m_value = ovalue;
                        }
                    }
                    else
                    {
                        // Case 2: node 'n' has 1 value and 1 node
                        node_t* onode = n->get_child_as_node(1 - child);
                        if (p != nullptr)
                        {
                            s8 const pc = p->get_indexof_child(n);
                            p->set_child_as_node(pc, onode);
                        }
                        else
                        {
                            m_root = onode;
                        }
                    }

                    value = cvalue->m_value;

                    // Node 'n' and value 'cvalue' can be deallocated
                    m_values->destruct<>(cvalue);
                    m_nodes->destruct<>(n);
                    m_num_values--;

                    return true;
                }
                break;
            }
        }

        return false;
    }

}; // namespace ncore

namespace ncore
{
    class fsa_test_t : public fsa_t
    {
    public:
        struct data_t
        {
            data_t* m_next;
            u64     m_data[3];
        };

        s32     m_pivot;
        s32     m_capacity;
        data_t  m_memory[8192];
        data_t* m_freelist;

        inline fsa_test_t()
            : m_pivot(0)
            , m_capacity(8192)
            , m_freelist(nullptr)
        {
        }

        void reset()
        {
            m_pivot    = 0;
            m_freelist = nullptr;
        }

        virtual u32   v_size() const { return sizeof(data_t); }
        virtual void* v_allocate()
        {
            void* ptr;
            if (m_pivot < m_capacity)
            {
                ptr = &m_memory[m_pivot++];
            }
            else if (m_freelist != nullptr)
            {
                ptr        = m_freelist;
                m_freelist = m_freelist->m_next;
            }
            return ptr;
        }

        virtual u32 v_deallocate(void* ptr)
        {
            data_t* d  = (data_t*)ptr;
            d->m_next  = m_freelist;
            m_freelist = d;
            return sizeof(data_t);
        }

        virtual void v_release() {}

        XCORE_CLASS_PLACEMENT_NEW_DELETE
    };
} // namespace ncore

extern ncore::alloc_t* gTestAllocator;
using namespace ncore;

UNITTEST_SUITE_BEGIN(test_dtrie_t)
{
    UNITTEST_FIXTURE(main)
    {
        fsa_test_t* m_fsa;

        UNITTEST_FIXTURE_SETUP() { m_fsa = gTestAllocator->construct<fsa_test_t>(); }

        UNITTEST_FIXTURE_TEARDOWN() { gTestAllocator->destruct(m_fsa); }

        UNITTEST_TEST(trienode1)
        {
            // node_t n;
        }

        UNITTEST_TEST(insert_find)
        {
            dtrie_t trie(m_fsa);

            key_t key1 = (key_t)0xABAB0ABC1F0000L;
            key_t key2 = (key_t)0xABAB0ABC0F0000L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));

            val_t value;
            CHECK_EQUAL(true, trie.find(key1, value));

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_find_1)
        {
            dtrie_t trie(m_fsa);

            key_t key1 = (key_t)0xABAB0ABC1F0000L;
            key_t key2 = (key_t)0xABAB0ABC0F0000L;
            key_t key3 = (key_t)0xABAB0A4C0F0000L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));
            CHECK_EQUAL(true, trie.insert(key3, 30));

            val_t value;
            CHECK_EQUAL(true, trie.find(key1, value));
            CHECK_EQUAL(true, trie.find(key2, value));
            CHECK_EQUAL(true, trie.find(key3, value));

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_find_2)
        {
            dtrie_t trie(m_fsa);

            key_t key1 = (key_t)0xABAB0ABC1F0000L;
            key_t key2 = (key_t)0xABAB0ABC0F0000L;
            key_t key3 = (key_t)0xABAB0ABC0F0080L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));
            CHECK_EQUAL(true, trie.insert(key3, 30));

            val_t value;
            CHECK_EQUAL(true, trie.find(key1, value));
            CHECK_EQUAL(true, trie.find(key2, value));
            CHECK_EQUAL(true, trie.find(key3, value));

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_max_run_len)
        {
            dtrie_t trie(m_fsa);

            key_t key1 = (key_t)0xAABBC0000F00L;
            key_t key2 = (key_t)0xAABBC0001F00L;
            key_t keyX = (key_t)0xAABBC0011F00L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));

            val_t value;
            CHECK_EQUAL(true, trie.find(key1, value));
            CHECK_EQUAL(true, trie.find(key2, value));

            CHECK_EQUAL(false, trie.find(keyX, value));

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_many)
        {
            dtrie_t trie(m_fsa);

            const s32 numitems = 4096;
            key_t     keys[numitems];
            key_t     seed = 0;
            for (s32 i = 0; i < numitems; ++i)
            {
                seed    = seed * 1664525UL + 1013904223UL;
                keys[i] = (key_t)seed;
            }

            for (s32 i = 0; i < numitems; ++i)
            {
                CHECK_EQUAL(true, trie.insert(keys[i], i));
            }

            val_t value;
            for (s32 i = 0; i < numitems; ++i)
            {
                CHECK_EQUAL(true, trie.find(keys[i], value));
                CHECK_EQUAL(value, i);
            }

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_remove_3)
        {
            dtrie_t trie(m_fsa);

            key_t key1 = (key_t)0xABAB0ABC1F0000L;
            key_t key2 = (key_t)0xABAB0ABC0F0000L;
            key_t key3 = (key_t)0xABAB0ABC0F0080L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));
            CHECK_EQUAL(true, trie.insert(key3, 30));

            val_t value;
            CHECK_EQUAL(true, trie.remove(key1, value));
            CHECK_EQUAL(10, value);
            CHECK_EQUAL(true, trie.remove(key2, value));
            CHECK_EQUAL(20, value);
            CHECK_EQUAL(true, trie.remove(key3, value));
            CHECK_EQUAL(30, value);

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_remove_many)
        {
            dtrie_t trie(m_fsa);

            const s32 numitems = 4096;
            key_t     keys[numitems];
            key_t     seed = 0;
            for (s32 i = 0; i < numitems; ++i)
            {
                seed    = seed * 1664525UL + 1013904223UL;
                keys[i] = (key_t)seed;
            }

            for (s32 i = 0; i < numitems; ++i)
            {
                CHECK_EQUAL(true, trie.insert(keys[i], i));
            }

            val_t value;
            for (s32 i = 0; i < numitems; ++i)
            {
                CHECK_EQUAL(true, trie.remove(keys[i], value));
                CHECK_EQUAL(value, i);
            }

            m_fsa->reset();
        }
    }
}
UNITTEST_SUITE_END

#else

#include "cbase\x_target.h"
#include "cbase\x_allocator.h"
#include "cbase\x_debug.h"
#include "cbase\x_integer.h"

#include "cunittest\cunittest.h"

UNITTEST_SUITE_BEGIN(dtrie_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}

        UNITTEST_FIXTURE_TEARDOWN() {}
    }
}
UNITTEST_SUITE_END

#endif