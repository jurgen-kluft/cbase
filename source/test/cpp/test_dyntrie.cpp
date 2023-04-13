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
    typedef u32       dynval_t;

    // Trie data structure where the key is a variable length byte array.

    struct dynnode_t // 12 bytes
    {
        u32 m_branch[2]; // 0: left, 1: right (the top byte is reserved)
        u32 m_bit_key;   // top-byte (8 bits) : key-offset (24 bits)

        // we can hold a maximum of 2^24 keys, since we use 24 bits to store the offset to the key
        // the maximum key length is 256 bytes (is 2048 bits)
        // so the bit index requires at least 11 bits to store
    };

    static s32  findfirst_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end);
    static s32  findfirst_bit_open(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start);
    static bool compare_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end);

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
        dynnode_t* m_root;
        dynval_t*  m_value;
        s32        m_num_values;
        s32        m_num_nodes;
        fsa_t*     m_nodes;
        fsa_t*     m_values;
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
    static s32 findfirst_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end)
    {
        // first we make sure that the key with the smaller length is key1
        if (keylen1 > keylen2)
        {
            g_swap(keylen1, keylen2);
            g_swap(key1, key2);
        }

        if (bit_start > keylen1 * 8)
        {
            bit_start = keylen1 * 8;
            bit_end   = bit_start;
        }
        else
        {
            if (bit_end > keylen1 * 8)
                bit_end = keylen1 * 8;
        }

        // check if start and end are on the same byte
        s32 byte_start = bit_start >> 3;
        s32 byte_end   = bit_end >> 3;

        // start and end are in the same byte
        if (byte_start == byte_end)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = (0xFF >> (bit_start & 7)) & (0xFF << (7 - (bit_end & 7)));

            if ((byte1 & mask) == (byte2 & mask))
                return bit_end;

            return bit_start + g_findfirst_bit((byte1 ^ byte2) & mask);
        }

        if (bit_start & 7 != 0)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = 0xFF >> (bit_start & 7);
            if ((byte1 & mask) != (byte2 & mask))
            {
                return bit_start + g_findfirst_bit((byte1 ^ byte2) & mask);
            }
            byte_start += 1;
            bit_start = (byte_start << 3);
        }

        while (byte_start < byte_end)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            if (byte1 != byte2)
            {
                return bit_start + g_findfirst_bit(byte1 ^ byte2);
            }
            byte_start += 1;
            bit_start += 8;
        }

        {
            const u8 byte1 = key1[byte_end];
            const u8 byte2 = key2[byte_end];
            const u8 mask  = 0xFF << (7 - (bit_end & 7));
            if ((byte1 & mask) != (byte2 & mask))
            {
                return bit_start + g_findfirst_bit((byte1 ^ byte2) & mask);
            }
        }

        return bit_end + 1;
    }

    static s32 findfirst_bit_open(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start)
    {
        if (keylen1 > keylen2)
        {
            g_swap(keylen1, keylen2);
            g_swap(key1, key2);
        }

        if (bit_start > keylen1 * 8)
            return keylen1 * 8;

        // compute bit_end for correct termination
        const s32 bit_end = keylen1 * 8;

        s32 bit_index = bit_start;
        while (bit_index < bit_end)
        {
            const s32 byte_index = bit_index >> 3;
            const u8  byte1      = key1[byte_index];
            const u8  byte2      = key2[byte_index];
            if (byte1 != byte2)
            {
                u8 mask = 0x80 >> (bit_index & 7);
                while (mask != 0 && (byte1 & mask) == (byte2 & mask))
                {
                    ++bit_index;
                    mask >>= 1;
                }
                return bit_index;
            }
            bit_index += 8;
        }
    }

    static bool compare_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end)
    {
        // first we make sure that the key with the smaller length is key1
        if (keylen1 > keylen2)
        {
            g_swap(keylen1, keylen2);
            g_swap(key1, key2);
        }

        if (bit_start > keylen1 * 8)
        {
            bit_start = keylen1 * 8;
            bit_end   = bit_start;
        }
        else
        {
            if (bit_end > keylen1 * 8)
                bit_end = keylen1 * 8;
        }

        s32 byte_start = bit_start >> 3;
        s32 byte_end   = bit_end >> 3;

        // start and end are in the same byte
        if (byte_start == byte_end)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = (0xFF >> (bit_start & 7)) & (0xFF << (7 - (bit_end & 7)));
            return (byte1 & mask) == (byte2 & mask);
        }

        // start and end are not in the same byte
        u8 byte1 = key1[byte_start];
        u8 byte2 = key2[byte_start];
        u8 mask  = 0xFF >> (bit_start & 7);
        if ((byte1 & mask) != (byte2 & mask))
            return false;

        byte1 = key1[byte_end];
        byte2 = key2[byte_end];
        mask  = 0xFF << (7 - (bit_end & 7));
        if ((byte1 & mask) != (byte2 & mask))
            return false;

        // see if there are bytes between start and end
        while (++byte_start < byte_end)
        {
            if (key1[byte_start] != key2[byte_start])
                return false;
        }

        return true;
    }

    dynnode_t* dyntrie_t::branch_two_values(dynnode_t* parent, dynval_t* v1, dynval_t* v2, s8 pos)
    {
        const s32 byte_index = pos >> 3;
        const u8  mask       = 0x80 >> (pos & 7);
        const u8  byte1      = v1->key[byte_index];
        const u8  byte2      = v2->key[byte_index];
        if ((byte1 & mask) == (byte2 & mask))
        {
            if (pos < v1->keylen * 8)
            {
                dynnode_t* node = m_nodes->alloc<dynnode_t>();
                node->parent    = parent;
                node->pos       = pos;
                node->value     = nullptr;
                node->child1    = branch_two_values(node, v1, v2, pos + 1);
                node->child2    = branch_two_values(node, v1, v2, pos + 1);
                ++m_num_nodes return false;
            }
        }