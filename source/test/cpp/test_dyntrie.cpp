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

namespace ncore
{
    typedef const u8* dynkey_t;
    typedef const u8* dynval_t;

    // Trie data structure where the key is a variable length byte array.

    struct dynnode_t // 12 bytes
    {
        inline bool is_node() const { return (m_branch[0] & 0x80000000) == 0; }
        inline bool is_value() const { return (m_branch[0] & 0x80000000) != 0; }

        inline u32  branch(s32 i) const { return (m_branch[i] & 0x00ffffff); }
        inline u32  key() const { return (m_bit_key & 0x00ffffff); }
        inline u32  key_len() const { return (m_bit_key & 0xff000000) >> 24; }
        inline s32  bit() const { return (node->m_branch[0] & 0x7f000000) >> 16 | (node->m_branch[1] & 0x7f000000) >> 24; }

        inline void set_branch(s32 i, s32 branch) { m_branch[i] = (m_branch[i] & 0xff000000) | (branch & 0x00ffffff); }
        inline void set_bit(s16 bit) { m_branch[0] = (m_branch[0] & 0x00ffffff) | ((bit & 0x0000007f) << 24); m_branch[1] = (m_branch[1] & 0x00ffffff) | ((bit & 0x00003f80) << (1+16)); }
        inline void set_key(s32 key, s8 keylen) { m_bit_key = (m_bit_key & 0xff000000) | (key & 0x00ffffff); m_bit_key = (m_bit_key & 0x00ffffff) | ((keylen & 0x000000ff) << 24); }

        u32 m_branch[2]; // left/right (node or value (1 bit) : bit-index (7 bits) : 'node or value'-index (24 bits))
        u32 m_bit_key;   // key-len (8 bits) : key-index (24 bits)
    };

    static s32 findfirst_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end, dynkey_t nullkey);

    class dyntrie_t
    {
    public:
        inline dyntrie_t(fsa_t* fsa)
            : m_root(nullptr)
            , m_value(nullptr)
            , m_num_values(0)
            , m_num_nodes(0)
            , m_nodes(fsa)
            , m_values(fsa)
        {
        }

        bool insert(s8 keylen, dynkey_t key, dynval_t value);
        bool find(s8 keylen, dynkey_t key, dynval_t& value);
        bool remove(s8 keylen, dynkey_t key, dynval_t& value);

    protected:
        const char m_nullkey[256];
        dynnode_t* m_root;
        dynval_t*  m_value;
        s32        m_num_values;
        s32        m_num_nodes;

        fsadexed_t* m_nodes;  // the nodes
        // key/value are stored in pairs
        fsadexed_t* m_keys;   // pointers to keys
        fsadexed_t* m_values; // pointers to values
    };

    bool dyntrie_t::insert(s8 keylen, dynkey_t key, dynval_t value)
    {
        if (m_root == nullptr)
        {
            m_value             = m_values->construct<dynval_t>();
            m_value[0]          = value;
            m_root              = m_nodes->construct<dynnode_t>();
            m_root->m_branch[0] = 0;
            m_root->m_branch[1] = 0;
            m_root->m_bit_key   = 0;
            m_num_values        = 1;
            m_num_nodes         = 1;
            return true;
        }

        // go through the tree to find a place to insert the new key or find a key that is the same as the incoming key
        // this mainly means to find the first bit that is different between two keys and to insert a node for that bit
    }

    static s32 get_bit(s8 keylen, dynkey_t key, s32 bit)
    {
        s32 byte = bit >> 3;
        s32 bit  = bit & 0x07;
        return (key[byte] & (1 << bit)) != 0 ? 1 : 0;
    }

    static bool keys_are_identical(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2)
    {
        if (keylen1 != keylen2)
            return false;

        for (s32 i = 0; i < keylen1; ++i)
        {
            if (key1[i] != key2[i])
                return false;
        }

        return true;
    }

    bool dyntrie_t::find(s8 keylen, dynkey_t key, dynval_t& value)
    {
        if (m_root == nullptr)
            return false;

        dynnode_t* node      = m_root;
        s32        bit_start = 0;
        while (true)
        {
            s32 bit_end     = node->bit();
            s32 bit_key     = node->key();
            s32 bit_key_len = node->key_len();
            s32 bit_key_end = bit_key + bit_key_len;

            s32 bit_diff = findfirst_bit_range(keylen, key, bit_key_len, m_keys->idx2obj<const char*>(bit_key), bit_start, bit_key_end, m_nullkey);
            if (bit_diff == bit_key_end + 1)
            {
                const s32 branch = get_bit(keylen, key, bit_diff);
                if (node->is_value(branch))
                {
                    dynkey_t k = m_key->idx2obj<dynkey_t>(node->branch(branch));

                    // check if the incoming and the stored key are the same
                    if (keys_are_identical(keylen, key, bit_key_len, k))
                    {
                        value = m_value[node->branch(branch)];
                        return true;
                    }
                    else
                    {   // keys are not the same so we need to insert a new node
                        // TODO: insert a new node
                        return false;
                    }
                }
                else // branch is a node, so we need to continue the search
                {
                    node = m_nodes->idx2obj<dynnode_t>(node->branch(branch));
                    bit_start = bit_diff + 1;
                }
            }
            else
            {
                // we have found a bit that is within the [bit_start, bit_end] range and so
                // we need to insert a node before this node

                dynnode_t* new_node = m_nodes->construct<dynnode_t>();
                new_node->set_key(keylen, key);
                new_node->set_bit(bit_diff);
                
                // figure out left/right of 'node' and set the new node's branches

                return true;                
            }
        }
    }

}; // namespace ncore

extern ncore::alloc_t* gTestAllocator;
using namespace ncore;

UNITTEST_SUITE_BEGIN(test_dyntrie)
{
    UNITTEST_FIXTURE(main)
    {

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

#include "cbase\x_target.h"
#include "cbase\x_allocator.h"
#include "cbase\x_debug.h"
#include "cbase\x_integer.h"

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
    // nullkey is used to compare the key outside the range of a key
    // return:
    // - bit_end + 1 if the two keys are equal in the range
    // - bit_start + n if the two keys are different in the range and the n-th bit is different
    static s32 findfirst_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end, dynkey_t nullkey)
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

            return bit_start + g_findfirst_bit((byte1 ^ byte2) & mask);
        }

        // start and end are not in the same byte
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = 0xFF >> (bit_start & 7);
            if ((byte1 & mask) != (byte2 & mask))
            {
                return bit_start + g_findfirst_bit((byte1 ^ byte2) & mask);
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
                    return (byte_start << 3) + g_findfirst_bit(byte1 ^ byte2);
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
                    return (byte_start << 3) + g_findfirst_bit(byte1 ^ byte2);
                }
            }
        }

        {
            const u8 byte1 = key1[byte_end];
            const u8 byte2 = key2[byte_end];
            const u8 mask  = 0xFF << (7 - (bit_end & 7));
            if ((byte1 & mask) != (byte2 & mask))
            {
                return (byte_end << 3) + g_findfirst_bit((byte1 ^ byte2) & mask);
            }
        }

        return bit_end + 1;
    }
} // namespace ncore