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

    // Trie data structure where the key is a variable length byte array.

    struct dynnode_t // 12 bytes
    {
        u32 m_branch[2]; // 0: left, 1: right (the top byte is reserved)
        u32 m_bit_key;   // top-byte (8 bits) : key-offset (24 bits)

        // we can hold a maximum of 2^24 keys, since we use 24 bits to store the offset to the key
        // the maximum key length is 256 bytes (is 2048 bits)
        // so the bit index requires at least 11 bits to store
    };

    static s32 ff_different_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end);
    static s32 ff_different_bit_open(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start);

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

        bool insert(s8 keylen, dynkey_t key, val_t value);
        bool find(s8 keylen, dynkey_t key, val_t& value);
        bool remove(s8 keylen, dynkey_t key, val_t& value);

    protected:
        dynnode_t* branch_two_values(dynnode_t* parent, value_t* v1, value_t* v2, s8 pos);

        dynnode_t* m_root;
        value_t*   m_value;
        s32        m_num_values;
        s32        m_num_nodes;
        fsa_t*     m_nodes;
        fsa_t*     m_values;
    };

    bool dyntrie_t::insert(s8 keylen, dynkey_t key, val_t value) {}

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
    static s32 ff_different_bit_range(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start, s32 bit_end)
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
        return bit_end;
    }

    static s32 ff_different_bit_open(s8 keylen1, dynkey_t key1, s8 keylen2, dynkey_t key2, s32 bit_start)
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
}