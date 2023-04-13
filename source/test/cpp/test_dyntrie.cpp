#define TEST_DYNTRIE
#ifdef TEST_DYNTRIE

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
    typedef const u8 * dynkey_t;

    // Trie data structure where the key is a variable length byte array, the first
    // byte is the length of the key, the rest is the key.

    struct dynnode_t // 12 bytes
    {
        u32 m_branch[2]; // 0: left, 1: right (the top byte is != 0 if the node is a value node)
        u32 m_bit_key;   // bit-index : key-offset, bit index indicates where we branch in the key
        // we can hold a maximum of 32 million keys, since we use 21 bits to store the offset to the key
        // the maximum key length is 256 bytes, the maximum bit index is 2048
        // so the bit index requires 11 bits, the key offset can be stored in the remaining 21 bits

    };

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

        bool insert(dynkey_t key, val_t value);
        bool find(dynkey_t key, val_t& value);
        bool remove(dynkey_t key, val_t& value);

    protected:
        dynnode_t* branch_two_values(dynnode_t* parent, value_t* v1, value_t* v2, s8 pos);

        dynnode_t*  m_root;
        value_t* m_value;
        s32      m_num_values;
        s32      m_num_nodes;
        fsa_t*   m_nodes;
        fsa_t*   m_values;
    };

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