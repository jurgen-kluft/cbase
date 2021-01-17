#define TEST_TRIEMAP2
#ifdef TEST_TRIEMAP2

#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"
#include "xbase/x_random.h"

#include "xbase/x_integer.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

#include "xunittest/xunittest.h"

namespace xcore
{
    typedef u64 key_t;
    typedef u32 val_t;

    static inline bool  helper_is_bit_set(key_t data, s32 bit) { return (data & ((key_t)1 << bit)) != 0; }
    static inline key_t helper_get_run_keybits(key_t key, s32 pos, s32 len)
    {
        key_t const mask = (((key_t)1 << len) - 1) << ((sizeof(key_t) * 8) - (pos + len));
        key              = key & mask;
        return key;
    }

    struct node_t
    {
        typedef u32 child_t;

        enum
        {
            RUN_MAXLEN        = 48, 
            CHILD_TYPE_VALUE  = 0,
            CHILD_TYPE_NODE   = 1,
            BRANCH_NUMBITS    = 1,
            KEY_MAX_BITCNT    = 64 // Actually this has a maximum of 256 bits, but the current implementation uses a 64-bit unsigned integer as the key
        };

        inline node_t()
            : m_data(0)
        {
            m_children[0] = 0xffffffff;
            m_children[1] = 0xffffffff;
        }

        inline s32   get_run_bitpos() const { return m_pos; }
        inline s32   get_run_bitlen() const { return m_run; }
        inline s32   get_run_endpos() const { return get_run_bitpos() + get_run_bitlen() + BRANCH_NUMBITS; } // The start pos for the child nodes
        inline key_t get_run_keymask() const { return (((key_t)1 << get_run_bitlen()) - 1) << (KEY_MAX_BITCNT - (get_run_bitpos() + get_run_bitlen())); }
        inline key_t get_run_keybits() const { return helper_get_run_keybits(m_key, get_run_bitpos(), get_run_bitlen()); }

        inline s32  get_child_bit() const { return (RUN_MAXLEN - (get_run_bitpos() + get_run_bitlen() + BRANCH_NUMBITS)); }
        inline u8   get_child_type(s8 child) const { return m_child[child] >> 31; }
        inline void set_child_type(s8 child, u8 type)
        {
            m_child[child] = m_child[child] & ~((u32)1 << 31);
            m_child[child] = m_child[child] | ((u32)type << 31);
        }

        inline void set_run_bitpos(u32 pos) { m_pos = pos; }
        inline void set_run_bitlen(u32 len) { m_run = run; }
        inline void set_run_bits(key_t bits)
        {
            s32 const vec = pos / RUN_MAXLEN; }
            m_key         = bits >> (KEY_MAX_BITCNT - RUN_MAXLEN - (vec * RUN_MAXLEN));
        }

        inline s8 get_indexof_child(child_t vvalue) const
        {
            s8 const i = ((m_children[0] & 0x7fffffff) == vvalue) ? 0 : 1;
            ASSERT((m_children[i] & 0x7fffffff) == vvalue);
            return i;
        }

        inline child_t get_child_as_node(s8 child) const
        {
            ASSERT(get_child_type(child) == CHILD_TYPE_NODE);
            return m_children[child] & 0x7fffffff;
        }
        inline child_t get_child_as_value(s8 child) const
        {
            ASSERT(get_child_type(child) == CHILD_TYPE_VALUE);
            return m_children[child] & 0x7fffffff;
        }

        inline void set_child_as_value(s8 child, child_t cvalue)
        {
            m_children[child] = cvalue;
            set_child_type(child, CHILD_TYPE_VALUE);
        }
        inline void set_child_as_node(s8 child, child_t cnode)
        {
            m_children[child] = cnode;
            set_child_type(child, CHILD_TYPE_NODE);
        }

        // 16 bytes
        u8      m_key[6];      // maximum run of 47 bits + 1 branch bit
        u8      m_run, m_pos;  // No more shifts if we do like this
        child_t m_children[2]; // Highest bit indicates value or node
    };

    // 12 bytes
    struct value_t
    {
        key_t m_key;
        val_t m_value;
    };

    // Trie data structure where every node can cover a range of bits (not only 1 bit)
    class xdtrie2
    {
    public:
        inline xdtrie2(fsa_t* fsa)
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
        node_t* branch_two_values(node_t* parent, value_t* v1, value_t* v2, s32 pos);

        u32      node2idx(node_t* n) const;
        u32      value2idx(value_t* n) const;
        node_t*  idx2node(u32 i) const;
        value_t* idx2value(u32 i) const;

        node_t*     m_root;
        value_t*    m_value;
        s32         m_num_values;
        s32         m_num_nodes;
        fsadexed_t* m_nodes;
        fsadexed_t* m_values;
    };

    u32      xdtrie2::node2idx(node_t* n) const { return 0; }
    u32      xdtrie2::value2idx(value_t* n) const { return 0; }
    node_t*  xdtrie2::idx2node(u32 i) const { return nullptr; }
    value_t* xdtrie2::idx2value(u32 i) const { return nullptr; }

    node_t* xdtrie2::branch_two_values(node_t* parent, value_t* v1, value_t* v2, s32 ppos)
    {
        // Determine the length of the run (pos = start position (from msb to lsb))
        key_t const mask = (0xFFFFFFFFFFFFFFFFUL >> ppos);
        key_t const diff = (v1->m_key ^ v2->m_key) & mask;
        s32 const   clen = xcountLeadingZeros(diff) - ppos;

        // Do we always need to create a branch at multiples of RUN_MAXLEN
        // Nope, we only need to create nodes for when the run is longer than RUN_MAXLEN
        // At which bit position can v1 and v2 diverge ?
        // In which vec does that bit position lie ?
        // Which vec are we currently at ?
        // Where is the next multiple of RUN_MAXLEN ?
        s32     p    = ppos;
        s32     l    = clen;
        node_t* head = parent;
        if (l > node_t::RUN_MAXLEN)
        {
            while (l > node_t::RUN_MAXLEN)
            {
                // We need to create nodes to cover the identical run.
                // Can take more than 1 node!
                m_num_nodes++;
                node_t* intnode = m_nodes->construct<node_t>();

                // What about 'vec'?
                // I don' think 'vec' matters, the only point that matters is the run length
                intnode->set_run_bitpos(p);
                intnode->set_run_bitlen(node_t::RUN_MAXLEN);
                intnode->set_run_bits(v1->m_key); // Node should take its part from the full key

                if (parent != nullptr)
                {
                    s32 childbit = parent->get_child_bit();
                    parent->set_child_as_node(helper_is_bit_set(v1->m_key, childbit) ? 1 : 0, node2idx(intnode));
                }
                else
                {
                    head = intnode;
                }

                parent = intnode;
                l -= (node_t::RUN_MAXLEN + node_t::BRANCH_NUMBITS); // The +1 is for the bit used for determining the branch (left=0, right=1)
                p += (node_t::RUN_MAXLEN + node_t::BRANCH_NUMBITS); // Move the position further by the running length and the branch bit
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
            parent->set_child_as_node(helper_is_bit_set(v1->m_key, childbit) ? 1 : 0, node2idx(n));
        }
        else
        {
            head = n;
        }

        // Set the 2 values as children
        s32 const cbit = n->get_child_bit();
        s32 const cidx = (helper_is_bit_set(v1->m_key, cbit)) ? 1 : 0;
        n->set_child_as_value(cidx, value2idx(v1));
        n->set_child_as_value(1 - cidx, value2idx(v2));

        return head;
    }

    bool xdtrie2::insert(key_t key, val_t value)
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
                    s32 const   len  = xcountLeadingZeros(diff) - pos;

                    sn->set_run_bitpos(pos);
                    sn->set_run_bitlen(len);
                    sn->set_run_bits(key);

                    m_num_values++;
                    value_t* const v  = m_values->construct<value_t>(key, value);
                    s32 const      sc = helper_is_bit_set(key, sn->get_child_bit()) ? 1 : 0;
                    sn->set_child_as_value(sc, value2idx(v));
                    sn->set_child_as_node(1 - sc, node2idx(n));

                    // Modify the run pos, run length of n
                    n->set_run_bitpos(sn->get_run_endpos());
                    n->set_run_bitlen(span - (len + 1));

                    // If parent != nullptr then we should replace child 'n' with 'sn'
                    // If parent == nullptr then it means we should update 'root'
                    if (parent != nullptr)
                    {
                        parent->set_child_as_node(child, node2idx(sn));
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
                u32 const ni = n->get_child_as_node(child);
                n            = idx2node(ni);
            }
            else
            { // Branch is a value, see if this is the same key-value
                u32 const      v1i = n->get_child_as_value(child);
                value_t* const v1  = idx2value(v1i);
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

    bool xdtrie2::find(key_t key, val_t& value)
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
                u32 const ni = n->get_child_as_node(child);
                n            = idx2node(ni);
            }
            else
            { // Branch is a value, see if this is the same key-value
                u32      cvaluei = n->get_child_as_value(child);
                value_t* cvalue  = idx2value(cvaluei);
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

    bool xdtrie2::remove(key_t key, val_t& value)
    {
        // Find the value by traversing the trie and keep track of parent
        //
        // When removing the value the following 2 cases can apply:
        //
        // 1. Node has 2 children, both are values:
        //    The value that is left, replace the 'node' child of the parent with the 'value'
        //    We can only do this when the combined run of both nodes <= node_t::RUN_MAXLEN
        //    Delete this node.
        // 2. Node has 2 children, one is a value, one is a node:
        //    Remove the value.
        //    We are only allowed to merge the nodes when their combined run is <= node_t::RUN_MAXLEN
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
                u32 const ni = n->get_child_as_node(child);
                n            = idx2node(ni);
            }
            else
            { // Branch is a value, see if this is the key
                u32 const cvaluei = n->get_child_as_value(child);
                value_t*  cvalue  = idx2value(cvaluei);
                if (cvalue->m_key == key)
                {
                    // Found it, now remove this node
                    u8 const ctype = n->get_child_type(1 - child);

                    // Can we actually merge the nodes, check if the resulting run
                    // can fit into the maximum run length.
                    // @TODO
                    if (ctype == node_t::CHILD_TYPE_VALUE)
                    {
                        // Case 1: node 'n' has two values
                        u32 const ovaluei = n->get_child_as_value(1 - child);
                        value_t*  ovalue  = idx2value(ovaluei);
                        if (p != nullptr)
                        {
                            s8 const pc = p->get_indexof_child(node2idx(n));
                            p->set_child_as_value(pc, ovaluei);
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
                        u32 const onodei = n->get_child_as_node(1 - child);
                        node_t*   onode  = idx2node(onodei);
                        if (p != nullptr)
                        {
                            s8 const pc = p->get_indexof_child(node2idx(n));
                            p->set_child_as_node(pc, onodei);
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

}; // namespace xcore

namespace xcore
{
    class xfsa_test : public fsadexed_t
    {
    public:
        struct data_t
        {
            data_t* m_next;
            u32     m_data;
        };

        s32     m_pivot;
        s32     m_capacity;
        data_t  m_memory[8192];
        data_t* m_freelist;

        inline xfsa_test()
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

        virtual void* v_idx2ptr(u32 index) const { return &m_memory[index]; }

        virtual u32 v_ptr2idx(void* ptr) const
        {
            data_t*   d = (data_t*)ptr;
            u32 const i = (d - &m_memory[0]);
            return i;
        }

        XCORE_CLASS_PLACEMENT_NEW_DELETE
    };
} // namespace xcore

extern xcore::alloc_t* gTestAllocator;

UNITTEST_SUITE_BEGIN(xdtrie2_v2)
{
    UNITTEST_FIXTURE(main)
    {
        xcore::xfsa_test* m_fsa;

        UNITTEST_FIXTURE_SETUP() { m_fsa = gTestAllocator->construct<xcore::xfsa_test>(); }

        UNITTEST_FIXTURE_TEARDOWN() { gTestAllocator->destruct(m_fsa); }

        UNITTEST_TEST(trienode1)
        {
            // node_t n;
        }

        UNITTEST_TEST(insert_find)
        {
            xcore::xdtrie2 trie(m_fsa);

            xcore::key_t key1 = (xcore::key_t)0xABAB0ABC1F0000L;
            xcore::key_t key2 = (xcore::key_t)0xABAB0ABC0F0000L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));

            xcore::val_t value;
            CHECK_EQUAL(true, trie.find(key1, value));

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_find_1)
        {
            xcore::xdtrie2 trie(m_fsa);

            xcore::key_t key1 = (xcore::key_t)0xABAB0ABC1F0000L;
            xcore::key_t key2 = (xcore::key_t)0xABAB0ABC0F0000L;
            xcore::key_t key3 = (xcore::key_t)0xABAB0A4C0F0000L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));
            CHECK_EQUAL(true, trie.insert(key3, 30));

            xcore::val_t value;
            CHECK_EQUAL(true, trie.find(key1, value));
            CHECK_EQUAL(true, trie.find(key2, value));
            CHECK_EQUAL(true, trie.find(key3, value));

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_find_2)
        {
            xcore::xdtrie2 trie(m_fsa);

            xcore::key_t key1 = (xcore::key_t)0xABAB0ABC1F0000L;
            xcore::key_t key2 = (xcore::key_t)0xABAB0ABC0F0000L;
            xcore::key_t key3 = (xcore::key_t)0xABAB0ABC0F0080L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));
            CHECK_EQUAL(true, trie.insert(key3, 30));

            xcore::val_t value;
            CHECK_EQUAL(true, trie.find(key1, value));
            CHECK_EQUAL(true, trie.find(key2, value));
            CHECK_EQUAL(true, trie.find(key3, value));

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_max_run_len)
        {
            xcore::xdtrie2 trie(m_fsa);

            xcore::key_t key1 = (xcore::key_t)0xAABBC0000F00L;
            xcore::key_t key2 = (xcore::key_t)0xAABBC0001F00L;
            xcore::key_t keyX = (xcore::key_t)0xAABBC0011F00L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));

            xcore::val_t value;
            CHECK_EQUAL(true, trie.find(key1, value));
            CHECK_EQUAL(true, trie.find(key2, value));

            CHECK_EQUAL(false, trie.find(keyX, value));

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_many)
        {
            xcore::xdtrie2 trie(m_fsa);

            const xcore::s32 numitems = 16;
            xcore::key_t     keys[numitems];
            xcore::key_t     seed = 0;
            for (xcore::s32 i = 0; i < numitems; ++i)
            {
                seed    = seed * 1664525UL + 1013904223UL;
                keys[i] = (xcore::key_t)seed;
            }

            for (xcore::s32 i = 0; i < numitems; ++i)
            {
                CHECK_EQUAL(true, trie.insert(keys[i], i));
            }

            xcore::val_t value;
            for (s32 i = 0; i < numitems; ++i)
            {
                CHECK_EQUAL(true, trie.find(keys[i], value));
                CHECK_EQUAL(value, i);
            }

            m_fsa->reset();
        }

        UNITTEST_TEST(insert_remove_3)
        {
            xcore::xdtrie2 trie(m_fsa);

            xcore::key_t key1 = (xcore::key_t)0xABAB0ABC1F0000L;
            xcore::key_t key2 = (xcore::key_t)0xABAB0ABC0F0000L;
            xcore::key_t key3 = (xcore::key_t)0xABAB0ABC0F0080L;

            CHECK_EQUAL(true, trie.insert(key1, 10));
            CHECK_EQUAL(true, trie.insert(key2, 20));
            CHECK_EQUAL(true, trie.insert(key3, 30));

            xcore::val_t value;
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
            xcore::xdtrie2 trie(m_fsa);

            const xcore::s32 numitems = 64;
            xcore::key_t     keys[numitems];
            xcore::key_t     seed = 0;
            for (xcore::s32 i = 0; i < numitems; ++i)
            {
                seed    = seed * 1664525UL + 1013904223UL;
                keys[i] = (xcore::key_t)seed;
            }

            for (xcore::s32 i = 0; i < numitems; ++i)
            {
                CHECK_EQUAL(true, trie.insert(keys[i], i));
            }

            xcore::val_t value;
            for (xcore::s32 i = 0; i < numitems; ++i)
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

#include "xbase\x_target.h"
#include "xbase\x_allocator.h"
#include "xbase\x_debug.h"
#include "xbase\x_integer.h"

#include "xunittest\xunittest.h"

UNITTEST_SUITE_BEGIN(xdtrie2)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}

        UNITTEST_FIXTURE_TEARDOWN() {}
    }
}
UNITTEST_SUITE_END

#endif