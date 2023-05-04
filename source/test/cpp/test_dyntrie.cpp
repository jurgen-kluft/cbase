#define TEST_DYNTRIE
#ifdef TEST_DYNTRIE

#    include "cbase/c_target.h"
#    include "cbase/c_allocator.h"
#    include "cbase/c_debug.h"
#    include "cbase/c_integer.h"
#    include "cbase/c_memory.h"
#    include "cbase/c_printf.h"
#    include "cbase/c_random.h"
#    include "cbase/c_runes.h"

#    include "cunittest/cunittest.h"
#    include "cbase/test_allocator.h"

namespace ncore
{
    typedef const u8* dynkey_t;
    typedef const u8* dynval_t;

    struct dynnode_t  // 12 bytes
    {
        inline void reset()
        {
            m_branch[0] = 0;
            m_branch[1] = 0;
            m_key_index = 0;
        }
        inline bool is_value(s8 i) const { return (m_branch[i] & 0x80000000) == 0; }
        inline bool is_node(s8 i) const { return (m_branch[i] & 0x80000000) == 0x80000000; }
        inline void mark_value(s8 i) { m_branch[i] &= 0x7fffffff; }
        inline void mark_node(s8 i) { m_branch[i] |= 0x80000000; }

        inline u32 branch(s8 i) const { return (m_branch[i] & 0x00ffffff); }
        inline u32 key() const { return (m_key_index & 0x00ffffff); }
        inline u16 bit() const { return ((m_branch[0] & 0x0f000000) >> (24 - 8)) | ((m_branch[1] & 0x0f000000) >> (24 - 4)) | ((m_key_index & 0x0f000000) >> 24); }

        inline void set_branch(s8 i, u32 index) { m_branch[i] = (m_branch[i] & 0xff000000) | (index & 0x00ffffff); }
        inline void set_bit(u16 bit)
        {
            m_branch[0] = (m_branch[0] & 0x80ffffff) | ((bit & 0x00000f00) << (24 - 8));
            m_branch[1] = (m_branch[1] & 0x80ffffff) | ((bit & 0x000000f0) << (24 - 4));
            m_key_index = (m_key_index & 0x00ffffff) | ((bit & 0x0000000f) << 24);
        }
        inline void set_key(u32 key_index) { m_key_index = (m_key_index & 0xff000000) | (key_index & 0x00ffffff); }

        u32 m_branch[2];  // left/right (node or value (1 bit) : bit-index (7 bits) : 'node or value'-index (24 bits))
        u32 m_key_index;  // key-index (24 bits)
    };

    static s8 get_bit(u8 keylen, dynkey_t key, u16 bit)
    {
        ASSERT((bit >> 3) < keylen);
        return (key[(bit >> 3)] & (0x80 >> (bit & 0x07))) ? (s8)1 : (s8)0;
    }
    static u16  ffs_bit(u8 byte);
    static bool compare_keys(u8 keylen1, dynkey_t key1, u8 keylen2, dynkey_t key2, u16 bit_start);
    static u16  findfirst_bit_range(u8 keylen1, dynkey_t key1, u8 keylen2, dynkey_t key2, u16 bit_start, u16 bit_end, dynkey_t nullkey);
    static u16  findfirst_bit_range_fixedkey(u8 keylen, dynkey_t key1, dynkey_t key2, u16 bit_start, u16 bit_end);

    // Trie data structure where the key is a variable length byte array (strings, etc..)
    // No hashing
    // Lookup/Insert is O(Log2(N)) where N is the number of items in the trie, the key length is not so much
    // a factor in that the key range is visited in a range by range forward fashion.
    // NOTE: We could also have a version where the keys are fixed length (byte sized) which would be faster.
    class dyntrie_t
    {
    public:
        inline dyntrie_t()
            : m_root(nullptr)
            , m_count(0)
            , m_capacity(1024)
            , m_node_head_index(0xffffffff)
            , m_node_free_index(0)
            , m_node_count(0)
        {
        }

        // initialize

        // Key length unit is bytes
        bool insert(u8 keylen, dynkey_t key, dynval_t value);
        bool find(u8 keylen, dynkey_t key, dynval_t& value) const;
        bool remove(u8 keylen, dynkey_t key, dynval_t& value);

    protected:
        static const u8 s_nullkey[256];
        dynnode_t*      m_root;
        s32             m_count;
        s32             m_capacity;

        inline u32        node_to_index(const dynnode_t* node) const { return (u32)(node - m_nodes); }
        inline dynnode_t* index_to_node(u32 index) { return &m_nodes[index]; }

        dynnode_t* alloc_node()
        {
            if (m_node_head_index != 0xffffffff)
            {
                dynnode_t* node   = index_to_node(m_node_head_index);
                m_node_head_index = node->key();
                m_node_count++;
                node->reset();
                return node;
            }
            if (m_node_free_index < m_capacity)
            {
                m_node_count++;
                dynnode_t* node = index_to_node(m_node_free_index++);
                node->reset();
                return node;
            }
            return nullptr;
        }

        void free_node(dynnode_t* node)
        {
            node->reset();
            m_node_count--;
            if (m_node_count == 0)
            {
                m_node_head_index = 0xffffffff;
                m_node_free_index = 0;
                return;
            }
            node->set_key(m_node_head_index);
            m_node_head_index = node_to_index(node);
        }

        u32       m_node_head_index;  // head of the free node list
        u32       m_node_free_index;  // initially we use this index to obtain a free node
        u32       m_node_count;       // number of nodes allocated
        dynnode_t m_nodes[1024];      // the nodes

        dynkey_t m_keys[1024];     // pointers to keys, key/value are stored in pairs at the same index
        u8       m_keylens[1024];  // lengths of keys, key/value are stored in pairs at the same index
        dynval_t m_values[1024];   // pointers to values
    };

    const u8 dyntrie_t::s_nullkey[256] = {0};

};  // namespace ncore

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_dyntrie)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;
        u8 s_nullkey[256] = {0};

        struct XorRandom
        {
            u64 s0, s1;
            inline XorRandom(u64 seed)
                : s0(seed)
                , s1(0)
            {
                next();
                next();
            }

            inline u64 next(void)
            {
                u64 ss1    = s0;
                u64 ss0    = s1;
                u64 result = ss0 + ss1;
                s0         = ss0;
                ss1 ^= ss1 << 23;
                s1 = ss1 ^ ss0 ^ (ss1 >> 18) ^ (ss0 >> 5);
                return result;
            }
        };
        static XorRandom s_rand(0x1234567890abcdef);

        static void MakeKey(u8 * dst, u32 size, s32 bit)
        {
            for (u32 i = 0; i < size; ++i)
            {
                dst[i] = 0;
            }
            dst[bit >> 3] |= 0x80 >> (bit & 0x07);
        }

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(get_bit)
        {
            u8 key[20];

            MakeKey(key, 20, 5);
            CHECK_EQUAL(1, get_bit(20, key, 5));
            CHECK_EQUAL(0, get_bit(20, key, 6));

            MakeKey(key, 20, 85);
            CHECK_EQUAL(1, get_bit(20, key, 85));
            CHECK_EQUAL(0, get_bit(20, key, 86));
        }

        UNITTEST_TEST(ffs_bit)
        {
            CHECK_EQUAL(8, ffs_bit(0x00));
            CHECK_EQUAL(7, ffs_bit(0x01));
            CHECK_EQUAL(0, ffs_bit(0x80));
        }

        UNITTEST_TEST(compare_keys)
        {
            u8 key1[20];
            u8 key2[20];

            MakeKey(key1, 20, 5);
            MakeKey(key2, 20, 5);
            CHECK_TRUE(compare_keys(20, key1, 20, key2, 0));

            MakeKey(key1, 20, 5);
            MakeKey(key2, 20, 6);
            CHECK_FALSE(compare_keys(20, key1, 20, key2, 0));

            MakeKey(key1, 20, 100);
            MakeKey(key2, 20, 100);
            CHECK_TRUE(compare_keys(20, key1, 20, key2, 30));
        }

        UNITTEST_TEST(findfirst_bit_range)
        {
            u8 key1[20];
            u8 key2[20];

            MakeKey(key1, 20, 10);
            MakeKey(key2, 20, 32);

            CHECK_EQUAL(10, findfirst_bit_range(20, key1, 20, key2, 0, 16, s_nullkey));
            CHECK_EQUAL(10, findfirst_bit_range(20, key1, 20, key2, 0, 30, s_nullkey));
            CHECK_EQUAL(10, findfirst_bit_range(20, key1, 20, key2, 10, 60, s_nullkey));

            CHECK_EQUAL(32, findfirst_bit_range(20, key1, 20, key2, 11, 60, s_nullkey));
        }

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

#    include "cbase\c_target.h"
#    include "cbase\c_allocator.h"
#    include "cbase\c_debug.h"
#    include "cbase\c_integer.h"

#    include "cunittest\cunittest.h"

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
    u16 ffs_bit(u8 byte) { return (u16)math::findLastBit(byte); }

    // nullkey is used to compare the key outside the range of a key
    // return:
    // - bit_end + 1 if the two keys are equal in the range
    // - bit_start + n if the two keys are different in the range and the n-th bit is different
    u16 findfirst_bit_range(u8 keylen1, dynkey_t key1, u8 keylen2, dynkey_t key2, u16 bit_start, u16 bit_end, dynkey_t nullkey)
    {
        // first we make sure that the key with the smaller length is key1
        if (keylen1 > keylen2)
        {
            g_swap(keylen1, keylen2);
            g_swap(key1, key2);
        }

        u16 byte_start = bit_start >> 3;
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
        const u16 byte_end = bit_end >> 3;
        if (byte_start == byte_end)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = (0xFF >> (bit_start & 7)) & (0xFF << (7 - (bit_end & 7)));
            if ((byte1 & mask) == (byte2 & mask))
                return bit_end + 1;

            return (byte_start << 3) + ffs_bit((byte1 ^ byte2) & mask);
        }

        // start and end are not in the same byte
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = 0xFF >> (bit_start & 7);
            if ((byte1 & mask) != (byte2 & mask))
            {
                return (byte_start << 3) + ffs_bit((byte1 ^ byte2) & mask);
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

    u16 findfirst_bit_range_fixedkey(u8 keylen, dynkey_t key1, dynkey_t key2, u16 bit_start, u16 bit_end)
    {
        u16 byte_start = bit_start >> 3;

        // start and end are in the same byte
        const u16 byte_end = bit_end >> 3;
        if (byte_start == byte_end)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = (0xFF >> (bit_start & 7)) & (0xFF << (7 - (bit_end & 7)));
            if ((byte1 & mask) == (byte2 & mask))
                return bit_end + 1;

            return (byte_start << 3) + ffs_bit((byte1 ^ byte2) & mask);
        }

        // start and end are not in the same byte

        // head
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = 0xFF >> (bit_start & 7);
            if ((byte1 & mask) != (byte2 & mask))
            {
                return (byte_start << 3) + ffs_bit((byte1 ^ byte2) & mask);
            }
        }

        // see if there are bytes between start and end
        // are we going outside any of the keys?
        while (++byte_start < byte_end)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            if (byte1 != byte2)
            {
                return (byte_start << 3) + ffs_bit(byte1 ^ byte2);
            }
        }

        // tail
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

    bool compare_keys(u8 keylen1, dynkey_t key1, u8 keylen2, dynkey_t key2, u16 bit_start)
    {
        if (keylen1 != keylen2)
            return false;

        u16 byte_start = bit_start >> 3;

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
        else
        {
            if (key1[byte_start] != key2[byte_start])
                return false;
        }

        // compare the rest byte by byte
        while (++byte_start < keylen1)
        {
            if (key1[byte_start] != key2[byte_start])
                return false;
        }

        return true;
    }

    bool dyntrie_t::insert(u8 keylen, dynkey_t key, dynval_t value)
    {
        if (m_root == nullptr)
        {
            const u16  bit_diff2 = findfirst_bit_range(keylen, key, keylen, s_nullkey, 0, (keylen * 8) - 1, s_nullkey);
            const s8   branch2   = get_bit(keylen, key, bit_diff2);
            dynnode_t* new_node  = alloc_node();

            new_node->set_bit(bit_diff2);
            if (bit_diff2 == keylen * 8)
                new_node->set_bit(keylen * 8 - 1);

            u32 const ki2  = m_count++;
            m_keys[ki2]    = key;
            m_keylens[ki2] = keylen;
            m_values[ki2]  = value;

            new_node->set_key(ki2);
            new_node->set_branch(branch2, ki2);
            new_node->set_branch(!branch2, 0xFFFFFF);  // max = 0xFFFFFF, so this is an invalid value/node index
            new_node->mark_value(branch2);
            new_node->mark_value(!branch2);

            m_root = new_node;
            return true;
        }

        dynnode_t* prevnode        = nullptr;
        s8         prevnode_branch = -1;
        dynnode_t* curnode         = m_root;
        s8         curnode_branch  = -1;
        u16        bit_start       = 0;
        while (true)
        {
            const u16 bit_end   = curnode->bit();
            const u32 key_index = curnode->key();

            const u16 bit_diff = findfirst_bit_range(keylen, key, m_keylens[key_index], m_keys[key_index], bit_start, bit_end, s_nullkey);
            if (bit_diff == bit_end + 1)
            {
                const s8 branch = get_bit(keylen, key, bit_diff);
                if (curnode->is_value(branch))
                {
                    const u32 ki = curnode->branch(branch);
                    if (ki == 0xFFFFFF)
                    {
                        u32 const ki2  = m_count++;
                        m_keys[ki2]    = key;
                        m_keylens[ki2] = keylen;
                        m_values[ki2]  = value;
                        curnode->set_branch(branch, ki2);
                        return true;
                    }
                    const dynkey_t k  = m_keys[ki];
                    const u8       kl = m_keylens[ki];

                    // check if the incoming and the stored key are the same
                    const u16 bit_diff2 = findfirst_bit_range(keylen, key, kl, k, bit_diff, 4096, s_nullkey);
                    if (bit_diff2 == 4096)
                    {
                        // the keys are the same
                        return false;
                    }
                    else
                    {
                        // keys are not the same so we need to insert a new node
                        u32 const ki2  = m_count++;
                        m_keys[ki2]    = key;
                        m_keylens[ki2] = keylen;
                        m_values[ki2]  = value;

                        const s8 branch2 = get_bit(keylen, key, bit_diff2);
                        ASSERT(get_bit(kl, k, bit_diff2) == (1 - branch2));

                        dynnode_t* new_node = alloc_node();
                        new_node->set_bit(bit_diff2);
                        new_node->set_key(key_index);
                        new_node->set_branch(branch2, ki2);
                        new_node->set_branch(!branch2, ki);

                        if (prevnode == nullptr)
                        {
                            m_root = new_node;
                        }
                        else
                        {
                            curnode->set_branch(branch, node_to_index(new_node));
                            curnode->mark_node(branch);
                        }

                        return true;
                    }
                }
                else  // branch is a node, so we need to continue the search
                {
                    prevnode        = curnode;
                    prevnode_branch = curnode_branch;
                    curnode_branch  = curnode->branch(branch);
                    curnode         = &m_nodes[curnode_branch];
                    bit_start       = bit_diff;
                }
            }
            else
            {
                // we have found a bit that is within the [bit_start, bit_end] range and so
                // we need to insert a new node at 'bit_diff'
                const u32 newkey_index  = m_count++;
                m_keys[newkey_index]    = key;
                m_keylens[newkey_index] = keylen;
                m_values[newkey_index]  = value;

                //   From                  To
                //
                // prevnode              prevnode
                //    |                     |
                //    |                     v
                //    |                  newnode (bit diff)
                //    |                 /       \         
                //    v                v         v
                // curnode         curnode      value

                const s8   branch   = get_bit(keylen, key, bit_diff);
                dynnode_t* new_node = alloc_node();
                new_node->set_key(newkey_index);
                new_node->set_bit(bit_diff);
                new_node->set_branch(1 - branch, node_to_index(curnode));
                new_node->set_branch(branch, newkey_index);
                new_node->mark_node(1 - branch);
                new_node->mark_value(branch);

                // the prevnode should now point to the new node
                prevnode->set_branch(curnode_branch, node_to_index(new_node));

                return true;
            }
        }
    }

    bool dyntrie_t::find(u8 keylen, dynkey_t key, dynval_t& value) const
    {
        if (m_root == nullptr)
            return false;

        const dynnode_t* node      = m_root;
        u16              bit_start = 0;
        while (true)
        {
            const u16 bit_end   = node->bit();
            const u32 key_index = node->key();

            const s32 bit_diff = findfirst_bit_range(keylen, key, m_keylens[key_index], m_keys[key_index], bit_start, bit_end, s_nullkey);
            if (bit_diff == bit_end + 1)
            {
                const s8 branch = get_bit(keylen, key, bit_diff);
                if (node->is_value(branch))
                {
                    const dynkey_t k  = m_keys[node->branch(branch)];
                    const u8       kl = m_keylens[node->branch(branch)];

                    // check if the incoming and the stored key are the same
                    if (compare_keys(keylen, key, kl, k, bit_start))
                    {
                        value = m_values[node->branch(branch)];
                        return true;
                    }
                    else
                    {
                        // keys are not the same, it doesn't exist
                        return false;
                    }
                }
                else  // branch is a node, so we need to continue the search
                {
                    const u32 nodeIdx = node->branch(branch);
                    if (nodeIdx == 0)
                        break;
                    node      = &m_nodes[nodeIdx];
                    bit_start = bit_diff;
                }
            }
            else
            {
                // we have found a bit that is within the [bit_start, bit_end] range and so
                // this means that the key doesn't exist
                break;
            }
        }
        return false;
    }

    bool dyntrie_t::remove(u8 keylen, dynkey_t key, dynval_t& value) { return false; }

}  // namespace ncore