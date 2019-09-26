#define TEST_TRIEMAP
#ifdef TEST_TRIEMAP

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
    // A compacted bittrie
    struct value_t
    {
		inline value_t(u64 key, u64 value) : m_key(key), m_value(value) {}
        u64 m_key;
        u64 m_value;

		XCORE_CLASS_PLACEMENT_NEW_DELETE
    };

    const u64 POS_MASK = 0xFC00000000000000L;
	const u64 RUN_MASK = 0x03F0000000000000L; 
	const u64 BITS_MASK = 0x3FFFFFFFFFFFFL;
	const u64 CHILD_TYPE_MASK = 0x000C000000000000L;

    struct node_t
    {
        enum { KEY_MAX_BITCNT = 64, POS_BITCNT = 6, POS_SHIFT = 58, RUN_BITCNT = 6, RUN_SHIFT = 52, BITS_BITCNT = 50, BITS_SHIFT = 0 };
        enum { CHILD_TYPE_BITCNT = 2, CHILD_TYPE_SHIFT = 50, CHILD_TYPE_VALUE = 0, CHILD_TYPE_NODE = 1 };

		inline node_t() : m_data(0) { m_children[0] = nullptr; m_children[1] = nullptr; }

        static inline s32 run_max_len() { return BITS_BITCNT; }
		static inline s32 branch_len() { return 1; }

        inline s32 get_run_pos() const { return ((m_data & POS_MASK) >> POS_SHIFT); }
        inline s32 get_run_len() const { return ((m_data & RUN_MASK) >> RUN_SHIFT); }
		inline u64 get_run_mask() const { return (((u64)1 << get_run_len()) - 1); }
        inline u64 get_run_bits() const { return (m_data & (((u64)1 << get_run_len()) - 1)); }
        inline u64 get_run_keymask() const { return (((u64)1 << get_run_len()) - 1) << (KEY_MAX_BITCNT - (get_run_pos() + get_run_len())); }
		inline u64 get_run_keybits() const { return (m_data & BITS_MASK) << (KEY_MAX_BITCNT - (get_run_pos() + get_run_len())); }
        inline s32 get_run_endpos() const { return get_run_pos() + get_run_len() + branch_len(); }	// The start pos for the child nodes

        // Which bit(s) determines the child index ?
        inline s32  get_child_bit() const { return (KEY_MAX_BITCNT - (get_run_pos() + get_run_len() + branch_len())); }
        inline u8   get_child_type(s8 child) const { return (((m_data & CHILD_TYPE_MASK) >> CHILD_TYPE_SHIFT) >> child) & 1; }
        inline void set_child_type(s8 child, u8 type) { m_data = m_data & ~((u64)1 << (CHILD_TYPE_SHIFT + child)); m_data = m_data | ((u64)type << (CHILD_TYPE_SHIFT + child)); }

        inline void set_run_pos(u32 pos) { m_data = (m_data & ~POS_MASK) | (((u64)pos << POS_SHIFT) & POS_MASK); }
        inline void set_run_len(u32 len) { m_data = (m_data & ~RUN_MASK) | (((u64)len << RUN_SHIFT) & RUN_MASK); }
        inline void set_run_bits(u64 bits) { m_data = (m_data & ~BITS_MASK) | ((bits << BITS_SHIFT) & BITS_MASK); }

        inline node_t* get_child_as_node(s8 child) const { ASSERT(get_child_type(child) == CHILD_TYPE_NODE); return m_children[child]; }
        inline value_t* get_child_as_value(s8 child) const { ASSERT(get_child_type(child) == CHILD_TYPE_VALUE); return (value_t*)(m_children[child]); }

        inline void set_child_as_value(s8 child, value_t* cvalue) { m_children[child] = (node_t*)(cvalue); set_child_type(child, CHILD_TYPE_VALUE); }
        inline void set_child_as_node(s8 child, node_t* cnode) { m_children[child] = cnode; set_child_type(child, CHILD_TYPE_NODE); }

		XCORE_CLASS_PLACEMENT_NEW_DELETE

        u64     m_data;
        node_t* m_children[2];
    };

    static inline bool helper_is_bit_set(u64 data, s32 bit) 
	{
		return (data & ((u64)1 << bit)) != 0; 
	}
    static inline u64 helper_get_run_bits(u64 key, s32 pos, s32 len)
    {
        key = (key >> (node_t::KEY_MAX_BITCNT - (pos + len)));
		u64 const mask = (((u64)1 << len) - 1);
		key = key & mask;
		return key;
    }

    // Trie data structure where every node can cover a range of bits (not only 1 bit)
    class xdtrie
    {
    public:
		inline xdtrie(xfsa* fsa) : m_root(nullptr), m_value(nullptr), m_num_values(0), m_num_nodes(0), m_nodes(fsa), m_values(fsa) {}

        bool    insert(u64 key, u64 value);
        bool    find(u64 key, u64& value);
        bool    remove(u64 key);

    protected:
		node_t* branch_two_values(node_t* parent, value_t* v1, value_t* v2, s8 pos);

        node_t*  m_root;
        value_t* m_value;
		s32		 m_num_values;
		s32		 m_num_nodes;
        xfsa*    m_nodes;
        xfsa*    m_values;
    };

	node_t* xdtrie::branch_two_values(node_t* parent, value_t* v1, value_t* v2, s8 pos)
	{
        // Determine the length of the run (pos = start position (from msb to lsb))
		u64 const mask = (0xFFFFFFFFFFFFFFFFUL >> pos);
        u64 const diff = (v1->m_key ^ v2->m_key) & mask;
        s32 const len = xcountLeadingZeros(diff) - pos;

        s32 p = pos;
        s32 l = len;
        node_t* head = parent;
        if (l > node_t::run_max_len())
        {
            while (l > node_t::run_max_len())
            {
                // We need to create nodes to cover the identical run.
                // Could take more than 1 node!
				m_num_nodes++;
			    node_t* intnode = m_nodes->construct<node_t>();

                intnode->set_run_pos(p);
                intnode->set_run_len(node_t::run_max_len());
                intnode->set_run_bits(helper_get_run_bits(v1->m_key, p, node_t::run_max_len()));

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
                l -= (node_t::run_max_len() + node_t::branch_len());	// The +1 is for the bit used for determining the branch (left=0, right=1)
                p += (node_t::run_max_len() + node_t::branch_len());	// Move the position further by the running length and the branch bit
            }
        }

		m_num_nodes++;
        node_t* n = m_nodes->construct<node_t>();
        n->set_run_pos(p);
        n->set_run_len(l);
        n->set_run_bits(helper_get_run_bits(v1->m_key, p, l));

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
        n->set_child_as_value(1-cidx, v2);

		return head;
	}


    bool    xdtrie::insert(u64 key, u64 value)
    {
        // Before creating the first node (root) we wait until we have 2 elements
        if (m_root == nullptr)
        {
            if (m_value == nullptr)
            {
				m_num_values++;
                m_value = m_values->construct<value_t>(key,value);
            }
            else if (m_value->m_key != key)
            {
				m_num_values++;
                value_t* othervalue = m_values->construct<value_t>(key,value);
				m_root = branch_two_values(nullptr, m_value, othervalue, 0);
				m_value = nullptr;
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
		s32 child = 0;
        node_t* n = m_root;
        while (n != nullptr)
        {
            // The run should be the same
            s32 const runlen = n->get_run_len();
            if (runlen > 0)
            {
                u64 const nbits = n->get_run_bits();
                u64 const kbits = helper_get_run_bits(key, n->get_run_pos(), runlen);
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
			        node_t* sn = m_nodes->construct<node_t>();

					//u64 const diff = (kbits ^ nbits);
					//s32 const len = xcountLeadingZeros(diff) - n->get_run_pos();

					// Determine the length of the run (pos = start position (from msb to lsb))
					s32 const span = n->get_run_len();
					s32 const pos = n->get_run_pos();
					u64 const mask = n->get_run_keymask();
					u64 const diff = n->get_run_keybits() ^ (key & mask);
					s32 const len = xcountLeadingZeros(diff) - pos;

					sn->set_run_pos(pos);
					sn->set_run_len(len);
					sn->set_run_bits(helper_get_run_bits(key, pos, len));

					m_num_values++;
					value_t* v = m_values->construct<value_t>(key,value);
					s32 sc = helper_is_bit_set(key, sn->get_child_bit()) ? 1 : 0;
					sn->set_child_as_value(sc, v);
					sn->set_child_as_node(1-sc, n);

					// Modify the run pos, run length of n
					n->set_run_pos(sn->get_run_endpos());
					n->set_run_len(span - (len + 1));

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
            child = (key >> n->get_child_bit()) & 1;
            
            // Determine type-of child
            if (n->get_child_type(child) == node_t::CHILD_TYPE_NODE)
            {	// Branch is a node, traverse further
                n = n->get_child_as_node(child);
            }
            else
            {	// Branch is a value, see if this is the same key-value
                value_t* v1 = n->get_child_as_value(child);
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
				value_t* v2 = m_values->construct<value_t>(key,value);
				branch_two_values(n, v1, v2, n->get_run_endpos());
                return true;
            }
        }

        return false;
    }

    bool    xdtrie::find(u64 key, u64& value)
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
            s32 const runlen = n->get_run_len();
            if (runlen > 0)
            {
                u64 const nbits = n->get_run_bits();
                u64 const kbits = helper_get_run_bits(key, n->get_run_pos(), runlen);
                if (kbits != nbits)
                    break;
            }

            // Determine left/right child
			s32 const cbit = n->get_child_bit();
            s32 const child = helper_is_bit_set(key, cbit) ? 1 : 0;
            
            // Determine type-of child
            if (n->get_child_type(child) == node_t::CHILD_TYPE_NODE)
            {	// Branch is a node, traverse further
                n = n->get_child_as_node(child);
            }
            else
            {	// Branch is a value, see if this is the same key-value
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

	bool    xdtrie::remove(u64 key)
	{
		return false;
	}

};


namespace xcore
{
	class xfsa_test : public xfsa
	{
	public:
		struct data_t {  data_t* m_next; u64 m_data[3]; };

		s32					m_pivot;
		s32					m_capacity;
		data_t				m_memory[8192];
		data_t*				m_freelist;

		inline				xfsa_test() : m_pivot(0), m_capacity(8192), m_freelist(nullptr) {}

		void				reset()
		{
			m_pivot = 0;
			m_freelist = nullptr;
		}

		virtual u32			size() const { return sizeof(data_t); }
		virtual void*		allocate()
		{
			void* ptr;
			if (m_pivot < m_capacity)
			{
				ptr = &m_memory[m_pivot++];
			}
			else if (m_freelist != nullptr)
			{
				ptr = m_freelist;
				m_freelist = m_freelist->m_next;
			}
			return ptr;
		}

		virtual void		deallocate(void* ptr)
		{
			data_t* d = (data_t*)ptr;
			d->m_next = m_freelist;
			m_freelist = d;
		}

		virtual void		release() {}

		XCORE_CLASS_PLACEMENT_NEW_DELETE

	};
}

extern xcore::xalloc* gTestAllocator;
using namespace xcore;

UNITTEST_SUITE_BEGIN(xdtrie)
{
	UNITTEST_FIXTURE(main)
	{
		xfsa_test*		m_fsa;

		UNITTEST_FIXTURE_SETUP()
		{
			m_fsa = gTestAllocator->construct<xfsa_test>();
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
			gTestAllocator->destruct(m_fsa);
		}

		UNITTEST_TEST(trienode1)
		{
			//node_t n;

		}

		UNITTEST_TEST(insert_2)
		{
			xdtrie trie(m_fsa);

			u64 key1 = 0xABAB0ABC1F000000L;
			u64 key2 = 0xABAB0ABC0F000000L;

			CHECK_EQUAL(true, trie.insert(key1, 10));
			CHECK_EQUAL(true, trie.insert(key2, 20));

			u64 value;
			CHECK_EQUAL(true, trie.find(key1, value));

			m_fsa->reset();
		}

		UNITTEST_TEST(test_case_1)
		{
			xdtrie trie(m_fsa);

			u64 key1 = 0xABAB0ABC1F000000L;
			u64 key2 = 0xABAB0ABC0F000000L;
			u64 key3 = 0xABAB0A4C0F000000L;

			CHECK_EQUAL(true, trie.insert(key1, 10));
			CHECK_EQUAL(true, trie.insert(key2, 20));
			CHECK_EQUAL(true, trie.insert(key3, 30));

			u64 value;
			CHECK_EQUAL(true, trie.find(key1, value));
			CHECK_EQUAL(true, trie.find(key2, value));
			CHECK_EQUAL(true, trie.find(key3, value));

			m_fsa->reset();
		}

		UNITTEST_TEST(test_case_2)
		{
			xdtrie trie(m_fsa);

			u64 key1 = 0xABAB0ABC1F000000L;
			u64 key2 = 0xABAB0ABC0F000000L;
			u64 key3 = 0xABAB0ABC0F000080L;

			CHECK_EQUAL(true, trie.insert(key1, 10));
			CHECK_EQUAL(true, trie.insert(key2, 20));
			CHECK_EQUAL(true, trie.insert(key3, 30));

			u64 value;
			CHECK_EQUAL(true, trie.find(key1, value));
			CHECK_EQUAL(true, trie.find(key2, value));
			CHECK_EQUAL(true, trie.find(key3, value));

			m_fsa->reset();
		}

		UNITTEST_TEST(insert_max_run_len)
		{
			xdtrie trie(m_fsa);

			u64 key1 = 0xABAB0ABC00000F00L;
			u64 key2 = 0xABAB0ABC00001F00L;
			u64 keyX = 0xABAB0ABC00011F00L;

			CHECK_EQUAL(true, trie.insert(key1, 10));
			CHECK_EQUAL(true, trie.insert(key2, 20));

			u64 value;
			CHECK_EQUAL(true, trie.find(key1, value));
			CHECK_EQUAL(true, trie.find(key2, value));

			CHECK_EQUAL(false, trie.find(keyX, value));

			m_fsa->reset();
		}

		UNITTEST_TEST(insert_many)
		{
			xdtrie trie(m_fsa);

			const s32 numitems = 16;
			u64 keys[numitems];
			u64 seed = 0;
			for (s32 i=0; i<numitems; ++i)
			{
				seed = seed * 1664525UL + 1013904223UL; 
				keys[i] = seed;
			}

			for (s32 i=0; i<numitems; ++i)
			{
				CHECK_EQUAL(true, trie.insert(keys[i], i));
			}

			u64 value;
			for (s32 i=0; i<numitems; ++i)
			{
				CHECK_EQUAL(true, trie.find(keys[i], value));
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

UNITTEST_SUITE_BEGIN(xdtrie)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{

		}

		UNITTEST_FIXTURE_TEARDOWN()
		{

		}
	}
}
UNITTEST_SUITE_END

#endif