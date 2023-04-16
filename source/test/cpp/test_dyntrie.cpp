#define TEST_DYNTRIE
#ifdef TEST_DYNTRIE

#include "cbase/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_debug.h"
#include "cbase/c_integer.h"
#include "cbase/c_memory.h"
#include "cbase/c_printf.h"
#include "cbase/c_random.h"
#include "cbase/c_runes.h"

#include "cunittest/cunittest.h"
#include "cbase/test_allocator.h"

namespace ncore
{
    typedef const u8* dynkey_t;
    typedef const u8* dynval_t;

    struct dynnode_t // 12 bytes
    {
        inline bool is_value(s32 i) const { return (m_branch[i] & 0x80000000) == 0; }
        inline bool is_node(s32 i) const { return !is_value(i); }
        inline void mark_value(s32 i) { m_branch[i] &= 0x7fffffff; }
        inline void mark_node(s32 i) { m_branch[i] |= 0x80000000; }

        inline u32 branch(s32 i) const { return (m_branch[i] & 0x00ffffff); }
        inline u32 key() const { return (m_bit_key & 0x00ffffff); }
        inline u32 key_len() const { return (m_bit_key & 0xff000000) >> 24; }
        inline s32 bit() const { return (m_branch[0] & 0x7f000000) >> (1 + 16) | (m_branch[1] & 0x7f000000) >> 24; }

        inline void set_branch(s32 i, s32 branch) { m_branch[i] = (m_branch[i] & 0xff000000) | (branch & 0x00ffffff); }
        inline void set_bit(s16 bit)
        {
            m_branch[0] = (m_branch[0] & 0x80ffffff) | ((bit & 0x00003f80) << (1 + 16));
            m_branch[1] = (m_branch[1] & 0x80ffffff) | ((bit & 0x0000007f) << 24);
        }
        inline void set_key(s32 key, s8 keylen)
        {
            m_bit_key = (m_bit_key & 0xff000000) | (key & 0x00ffffff);
            m_bit_key = (m_bit_key & 0x00ffffff) | ((keylen & 0x000000ff) << 24);
        }

        u32 m_branch[2]; // left/right (node or value (1 bit) : bit-index (7 bits) : 'node or value'-index (24 bits))
        u32 m_bit_key;   // key-len (8 bits) : key-index (24 bits)
    };

    // Trie data structure where the key is a variable length byte array (strings, etc..)
    // No hashing
    // Lookup/Insert is O(Log2(N)) where N is the number of items in the trie, the key length is not so much
    // a factor in that the key range is visited in a range by range forward fashion.
    // NOTE: We could also have a version where the keys are fixed length (byte sized) which would be quite faster.
    // NOTE: This node could even be 8 bytes if the total amount of items is less than 2^16.
    class dyntrie_t
    {
    public:
        inline dyntrie_t()
            : m_root(nullptr)
            , m_key(nullptr)
            , m_value(nullptr)
            , m_count(0)
            , m_capacity(1024)
        {
        }

        bool insert(s8 keylen, dynkey_t key, dynval_t value);
        bool find(s8 keylen, dynkey_t key, dynval_t& value) const;
        bool remove(s8 keylen, dynkey_t key, dynval_t& value);

    protected:
        static s32  get_bit(s8 keylen, dynkey_t key, s32 bit) { return (key[(bit >> 3)] & (1 << (bit & 0x07))) != 0 ? 1 : 0; }
        static s32  ffs_bit(u8 byte);
        static bool compare_keys(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start);
        static s32  findfirst_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end, dynkey_t nullkey);

        static const char s_nullkey[256];
        dynnode_t*        m_root;
        dynkey_t          m_key;
        dynval_t          m_value;
        s32               m_count;
        s32               m_capacity;
        dynnode_t         m_nodes[1024];  // the nodes
        dynkey_t          m_keys[1024];   // pointers to keys, key/value are stored in pairs at the same index
        dynval_t          m_values[1024]; // pointers to values
    };

    const char dyntrie_t::s_nullkey[256] = {0};

}; // namespace ncore

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_dyntrie)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(trienode1) {}
        UNITTEST_TEST(insert_find) {}
        UNITTEST_TEST(insert_find_1) {}
        UNITTEST_TEST(insert_find_2) {}
        UNITTEST_TEST(insert_max_run_len) {}
        UNITTEST_TEST(insert_many) {}
        UNITTEST_TEST(insert_remove_3) {}
        UNITTEST_TEST(insert_remove_many) {}
    }
}
UNITTEST_SUITE_END

#else

#include "cbase\c_target.h"
#include "cbase\c_allocator.h"
#include "cbase\c_debug.h"
#include "cbase\c_integer.h"

#include "cunittest\cunittest.h"

UNITTEST_SUITE_BEGIN(dyntrie_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}
    }
}
UNITTEST_SUITE_END

#endif

namespace ncore
{
    s32 dyntrie_t::ffs_bit(u8 byte) { return math::findLastBit(byte); }

    // nullkey is used to compare the key outside the range of a key
    // return:
    // - bit_end + 1 if the two keys are equal in the range
    // - bit_start + n if the two keys are different in the range and the n-th bit is different
    s32 dyntrie_t::findfirst_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end, dynkey_t nullkey)
    {
        // first we make sure that the key with the smaller length is key1
        if (keylen1 > keylen2)
        {
            g_swap(keylen1, keylen2);
            g_swap(key1, key2);
        }

        s32 byte_start = bit_start >> 3;
        s32 byte_end   = bit_end >> 3;
        if (byte_start > keylen1)
        {
            key1 = nullkey;
        }
        if (byte_start > keylen2)
        {
            key2 = nullkey;
        }
        if (key1 == nullkey && key2 == nullkey)
            return bit_end + 1;

        // start and end are in the same byte
        if (byte_start == byte_end)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = (0xFF >> (bit_start & 7)) & (0xFF << (7 - (bit_end & 7)));
            if ((byte1 & mask) == (byte2 & mask))
                return bit_end + 1;

            return bit_start + ffs_bit((byte1 ^ byte2) & mask);
        }

        // start and end are not in the same byte
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = 0xFF >> (bit_start & 7);
            if ((byte1 & mask) != (byte2 & mask))
            {
                return bit_start + ffs_bit((byte1 ^ byte2) & mask);
            }
        }

        // see if there are bytes between start and end
        // are we going outside any of the keys?
        if (byte_end < keylen1 && byte_end < keylen2)
        {
            while (++byte_start < byte_end)
            {
                const u8 byte1 = key1[byte_start];
                const u8 byte2 = key2[byte_start];
                if (byte1 != byte2)
                {
                    return (byte_start << 3) + ffs_bit(byte1 ^ byte2);
                }
            }
        }
        else
        {
            // here we will check key1 and key2 for going outside and switch to nullkey
            while (++byte_start < byte_end)
            {
                if (key1 != nullkey && (byte_start >= keylen1))
                    key1 = nullkey;
                if (key2 != nullkey && (byte_start >= keylen2))
                    key2 = nullkey;
                if (key1 == nullkey && key2 == nullkey)
                    return bit_end + 1;
                const u8 byte1 = key1[byte_start];
                const u8 byte2 = key2[byte_start];
                if (byte1 != byte2)
                {
                    return (byte_start << 3) + ffs_bit(byte1 ^ byte2);
                }
            }
        }

        {
            const u8 byte1 = key1[byte_end];
            const u8 byte2 = key2[byte_end];
            const u8 mask  = 0xFF << (7 - (bit_end & 7));
            if ((byte1 & mask) != (byte2 & mask))
            {
                return (byte_end << 3) + ffs_bit((byte1 ^ byte2) & mask);
            }
        }

        return bit_end + 1;
    }

    bool dyntrie_t::compare_keys(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start)
    {
        if (keylen1 != keylen2)
            return false;

        s32 byte_start = bit_start >> 3;

        if ((bit_start & 7) != 0)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = (0xFF >> (bit_start & 7));
            if ((byte1 & mask) != (byte2 & mask))
            {
                return false;
            }
        }

        // compare the rest byte by byte
        while (++byte_start < keylen1)
        {
            if (key1[byte_start] != key2[byte_start])
                return false;
        }

        return true;
    }

    bool dyntrie_t::insert(s8 keylen, dynkey_t key, dynval_t value)
    {
        if (m_root == nullptr)
            return false;

        dynnode_t* node      = m_root;
        s32        bit_start = 0;
        while (true)
        {
            const s32 bit_end     = node->bit();
            const s32 bit_key     = node->key();
            const s32 bit_key_len = node->key_len();
            const s32 bit_key_end = bit_key + bit_key_len;

            const s32 bit_diff = findfirst_bit_range(keylen, key, bit_key_len, m_keys[bit_key], bit_start, bit_key_end, (dynkey_t)s_nullkey);
            if (bit_diff == bit_end + 1)
            {
                const s32 branch = get_bit(keylen, key, bit_diff);
                if (node->is_value(branch))
                {
                    dynkey_t k = m_keys[node->branch(branch)];

                    // check if the incoming and the stored key are the same
                    if (compare_keys(keylen, key, bit_key_len, k, bit_end))
                    {
                        return false;
                    }
                    else
                    { // keys are not the same so we need to insert a new node

                        // TODO: insert a new node
                        // create a new node
                        // set the new node's key to the key of the current node
                        


                        return false;
                    }
                }
                else // branch is a node, so we need to continue the search
                {
                    node      = &m_nodes[node->branch(branch)];
                    bit_start = bit_diff;
                }
            }
            else
            {
                // we have found a bit that is within the [bit_start, bit_end] range and so
                // we need to insert a node before this node
                const u32 newkey_index = m_count++;
                m_keys[newkey_index]   = key;
                m_values[newkey_index] = value;

                dynnode_t* new_node = &m_nodes[newkey_index];
                new_node->set_key(keylen, newkey_index);
                new_node->set_bit(bit_diff);

                // figure out left/right of 'node' and set the new node's branches

                return true;
            }
        }
    }

    bool dyntrie_t::find(s8 keylen, dynkey_t key, dynval_t& value) const
    {
        if (m_root == nullptr)
            return false;

        const dynnode_t* node      = m_root;
        s32              bit_start = 0;
        while (true)
        {
            const s32 bit_end     = node->bit();
            const s32 bit_key     = node->key();
            const s32 bit_key_len = node->key_len();
            const s32 bit_key_end = bit_key + bit_key_len;

            const s32 bit_diff = findfirst_bit_range(keylen, key, bit_key_len, m_keys[bit_key], bit_start, bit_key_end, (dynkey_t)s_nullkey);
            if (bit_diff == bit_key_end + 1)
            {
                const s32 branch = get_bit(keylen, key, bit_diff);
                if (node->is_value(branch))
                {
                    dynkey_t k = m_keys[node->branch(branch)];

                    // check if the incoming and the stored key are the same
                    if (compare_keys(keylen, key, bit_key_len, k, bit_start))
                    {
                        value = m_values[node->branch(branch)];
                        return true;
                    }
                    else
                    { // keys are not the same so we need to insert a new node
                        // TODO: insert a new node
                        return false;
                    }
                }
                else // branch is a node, so we need to continue the search
                {
                    const s32 branch = node->branch(branch);
                    if (branch == 0)
                        break;
                    node      = &m_nodes[branch];
                    bit_start = bit_diff + 1;
                }
            }
            else
            {
                break;
            }
        }
        return false;
    }

    bool dyntrie_t::remove(s8 keylen, dynkey_t key, dynval_t& value) { return false; }

} // namespace ncore