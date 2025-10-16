#define TEST_DYNTRIE
#ifdef TEST_DYNTRIE

#    include "ccore/c_target.h"
#    include "cbase/c_allocator.h"
#    include "cbase/c_debug.h"
#    include "cbase/c_integer.h"
#    include "cbase/c_memory.h"
#    include "cbase/c_printf.h"
#    include "cbase/c_random.h"
#    include "cbase/c_runes.h"

#    include "cunittest/cunittest.h"

namespace ncore
{
    typedef const u8* dynkey_t;
    typedef const u8* dynval_t;

    // If the key is a standard u32 or u64, then we can use a fixed key trie
    // and we can directly embed the key into the node.
    // The benefit is that we save not only memory but also a level of indirection
    // which means faster lookups.
    struct dynnode_fixed_t  // 12 bytes
    {
        u32 m_branch[2];   // left/right (node or value (1 get_bit) : get_bit-index (7 bits) : 'node or value'-index (24 bits))
        u32 m_key;         // key (32 bits key)
    };

    struct dynnode_fixed2_t  // 16 bytes
    {
        u32 m_branch[2];   // left/right (node or value (1 get_bit) : get_bit-index (7 bits) : 'node or value'-index (24 bits))
        u64 m_key;         // key (64 bits key)
    };



    struct dynnode_t  // 12 bytes (dynamic key)
    {
        inline void reset()
        {
            m_branch[0]  = 0;
            m_branch[1]  = 0;
            m_item_index = 0;
        }
        inline bool is_value(s8 i) const { return (m_branch[i] & 0x80000000) == 0; }
        inline bool is_node(s8 i) const { return (m_branch[i] & 0x80000000) == 0x80000000; }
        inline void mark_value(s8 i) { m_branch[i] &= 0x7fffffff; }
        inline void mark_node(s8 i) { m_branch[i] |= 0x80000000; }

        inline u32  get_item_index() const { return (m_item_index & 0x00ffffff); }
        inline void set_item_index(u32 node_item_index) { m_item_index = (m_item_index & 0xff000000) | (node_item_index & 0x00ffffff); }

        inline u32  get_branch(s8 i) const { return (m_branch[i] & 0x00ffffff); }
        inline void set_branch(s8 i, u32 index) { m_branch[i] = (m_branch[i] & 0xff000000) | (index & 0x00ffffff); }

        inline u16  get_bit() const { return ((m_branch[0] & 0x0f000000) >> (24 - 8)) | ((m_branch[1] & 0x0f000000) >> (24 - 4)) | ((m_item_index & 0x0f000000) >> 24); }
        inline void set_bit(u16 get_bit)
        {
            m_branch[0]  = (m_branch[0] & 0x80ffffff) | ((get_bit & 0x00000f00) << (24 - 8));
            m_branch[1]  = (m_branch[1] & 0x80ffffff) | ((get_bit & 0x000000f0) << (24 - 4));
            m_item_index = (m_item_index & 0x00ffffff) | ((get_bit & 0x0000000f) << 24);
        }

        u32 m_branch[2];   // left/right (node or value (1 get_bit) : get_bit-index (7 bits) : 'node or value'-index (24 bits))
        u32 m_item_index;  // key-index (24 bits)
    };

    static s8 get_bit(u8 keylen, dynkey_t key, u16 get_bit)
    {
        ASSERT((get_bit >> 3) < keylen);
        return (key[(get_bit >> 3)] & (0x80 >> (get_bit & 0x07))) ? (s8)1 : (s8)0;
    }
    static s8  ffs_bit(u8 byte);
    static bool compare_keys(u8 keylen1, dynkey_t key1, u8 keylen2, dynkey_t key2, u16 bit_start);
    static u16  findfirst_bit_range(u8 keylen1, dynkey_t key1, u8 keylen2, dynkey_t key2, u16 bit_start, u16 node_bit_end, dynkey_t nullkey);
    static u16  findfirst_bit_range_fixedkey(u8 keylen, dynkey_t key1, dynkey_t key2, u16 bit_start, u16 node_bit_end);

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
            , m_first_item(0xffffffff)
            , m_node_head_index(0xffffffff)
            , m_node_free_index(0)
            , m_node_count(0)
        {
        }

        inline u32 size() const { return m_count; }

        // Key length unit is bytes
        bool insert(u8 keylen, dynkey_t key, dynval_t value);
        bool find(u8 keylen, dynkey_t key, dynval_t& value) const;
        bool remove(u8 keylen, dynkey_t key, dynval_t& value);

    protected:
        static const u8 s_nullkey[256];
        dynnode_t*      m_root;
        u32             m_count;
        u32             m_capacity;

        inline u32        node_to_index(const dynnode_t* node) const { return (u32)(node - m_nodes); }
        inline dynnode_t* index_to_node(u32 index) { return &m_nodes[index]; }

        dynnode_t* alloc_node()
        {
            if (m_node_head_index != 0xffffffff)
            {
                dynnode_t* node   = index_to_node(m_node_head_index);
                m_node_head_index = node->get_item_index();
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
            node->set_item_index(m_node_head_index);
            m_node_head_index = node_to_index(node);
        }

        u32 add_item(dynkey_t key, u32 keylen, dynval_t value);

        u32       m_first_item;
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
        // UNITTEST_ALLOCATOR;
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

            inline void seed(u64 seed)
            {
                s0 = seed;
                s1 = 0;
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

        static void MakeKey(u8 * dst, u32 size, s32 get_bit)
        {
            for (u32 i = 0; i < size; ++i)
            {
                dst[i] = 0;
            }
            dst[get_bit >> 3] |= 0x80 >> (get_bit & 0x07);
        }

        static bool CompareKeys(u8 const* key1, u8 const* key2, s8 len)
        {
            for (s8 i = 0; i < len; ++i)
            {
                if (key1[i] != key2[i])
                    return false;
            }
            return true;
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

        UNITTEST_TEST(insert)
        {
            dyntrie_t trie;

            s_rand.seed(0x1234567890abcdef);

            u64 key1 = s_rand.next();
            u64 key2 = s_rand.next();
            u64 value1 = s_rand.next();
            u64 value2 = s_rand.next();

            CHECK_TRUE(trie.insert(sizeof(key1), (u8 *)&key1, (dynval_t)&value1));
            CHECK_TRUE(trie.insert(sizeof(key2), (u8 *)&key2, (dynval_t)&value2));
        }

        UNITTEST_TEST(insert_find_1)
        {
            dyntrie_t trie;

            s_rand.seed(0x1234567890abcdef);

            u64 key1   = s_rand.next();
            u64 key2   = s_rand.next();
            u64 key3   = s_rand.next();
            u64 key4   = s_rand.next();

            u64 value1 = s_rand.next();
            u64 value2 = s_rand.next();
            u64 value3 = s_rand.next();
            u64 value4 = s_rand.next();

            CHECK_EQUAL((u32)0, trie.size());
            CHECK_TRUE(trie.insert(sizeof(key1), (u8*)&key1, (dynval_t)&value1));
            CHECK_TRUE(trie.insert(sizeof(key2), (u8*)&key2, (dynval_t)&value2));
            CHECK_TRUE(trie.insert(sizeof(key3), (u8*)&key3, (dynval_t)&value3));
            CHECK_TRUE(trie.insert(sizeof(key4), (u8*)&key4, (dynval_t)&value4));
            CHECK_EQUAL((u32)4, trie.size());

            dynval_t v;
            CHECK_TRUE(trie.find(sizeof(key1), (u8*)&key1, v));
            CHECK_TRUE(CompareKeys((u8 const*)&value1, v, sizeof(u64)));
            CHECK_TRUE(trie.find(sizeof(key2), (u8*)&key2, v));
            CHECK_TRUE(CompareKeys((u8 const*)&value2, v, sizeof(u64)));
            CHECK_TRUE(trie.find(sizeof(key3), (u8*)&key3, v));
            CHECK_TRUE(CompareKeys((u8 const*)&value3, v, sizeof(u64)));
            CHECK_TRUE(trie.find(sizeof(key4), (u8*)&key4, v));
            CHECK_TRUE(CompareKeys((u8 const*)&value4, v, sizeof(u64)));
        }

        UNITTEST_TEST(insert_find_2)
        {
            dyntrie_t trie;

            u64 keys[1000];
            u64 values[1000];

            CHECK_EQUAL((u32)0, trie.size());

            for (s32 i = 0; i < 1000; ++i)
            {
                keys[i]   = 0xDEADBEEFDEADBEEF + i;
                values[i] = i;

                CHECK_TRUE(trie.insert(sizeof(keys[i]), (u8*)&keys[i], (dynval_t)&values[i]));

                u64 value = 0;
                CHECK_TRUE(trie.find(sizeof(keys[i]), (u8*)&keys[i], (dynval_t&)value));
            }

            CHECK_EQUAL((u32)1000, trie.size());
        }

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
    static inline s8 ffs_bit(u8 byte) { return math::g_findLastBit(byte); }

    // nullkey is used to compare the key outside the range of a key
    // return:
    // - bit_end + 1 if the two keys are equal in the range
    // - bit_start + n if the two keys are different in the range and the n-th get_bit is different
    static u16 findfirst_bit_range(u8 keylen1, dynkey_t key1, u8 keylen2, dynkey_t key2, u16 bit_start, u16 bit_end, dynkey_t nullkey)
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
            return bit_end;

        // start and end are in the same byte
        const u16 byte_end = bit_end >> 3;
        if (byte_start == byte_end)
        {
            const u8 byte1 = key1[byte_start];
            const u8 byte2 = key2[byte_start];
            const u8 mask  = (0xFF >> (bit_start & 7)) & (0xFF << (7 - (bit_end & 7)));
            const u8 bits  = (byte1 ^ byte2) & mask;
            if (bits == 0)
                return bit_end;
            return (byte_end << 3) + ffs_bit(bits);
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
            const u8 bits  = (byte1 ^ byte2) & mask;
            if (bits == 0)
                return bit_end;

            return (byte_end << 3) + ffs_bit(bits);
        }
    }

    static u16 findfirst_bit_range_fixedkey(u8 keylen, dynkey_t key1, dynkey_t key2, u16 bit_start, u16 bit_end)
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
                return bit_end;

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
            const u8 bits  = (byte1 ^ byte2) & mask;
            if (bits == 0)
                return bit_end;

            return (byte_end << 3) + ffs_bit(bits);
        }
    }

    static bool compare_keys(u8 keylen1, dynkey_t key1, u8 keylen2, dynkey_t key2, u16 bit_start)
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
            if (m_first_item == 0xffffffff)
            {
                m_first_item = add_item(key, keylen, value);
                return true;
            }

            const dynkey_t key2    = m_keys[m_first_item];
            const u8       keylen2 = m_keylens[m_first_item];

            const u16 bit_diff = findfirst_bit_range(keylen, key, keylen2, key2, 0, (keylen * 8) - 1, s_nullkey);
            if (bit_diff == (keylen * 8))
            {
                // seems the keys are the same
                return false;
            }
            const s8   branch      = get_bit(keylen, key, bit_diff);
            const u32  second_item = add_item(key, keylen, value);
            dynnode_t* node        = alloc_node();
            node->set_bit(bit_diff);
            node->set_item_index(second_item);
            node->set_branch(1 - branch, m_first_item);
            node->set_branch(branch, second_item);
            node->mark_value(1 - branch);
            node->mark_value(branch);

            m_first_item = 0xffffffff;
            m_root       = node;
            return true;
        }

        dynnode_t* prevnode        = nullptr;
        //s8         prevnode_branch = -1;
        dynnode_t* curnode         = m_root;
        s8         curnode_branch  = -1;
        u16        bit_start       = 0;
        while (true)
        {
            const u16 node_bit    = curnode->get_bit();
            const u32 node_item = curnode->get_item_index();

            const u16 bit_diff = findfirst_bit_range(keylen, key, m_keylens[node_item], m_keys[node_item], bit_start, node_bit, s_nullkey);
            if (bit_diff == node_bit)
            {
                const s8 node_branch = get_bit(keylen, key, bit_diff);
                if (curnode->is_value(node_branch))
                {
                    const u32 branch_item = curnode->get_branch(node_branch);
                    const dynkey_t branch_key    = m_keys[branch_item];
                    const u8       branch_keylen = m_keylens[branch_item];

                    // check if the incoming and the stored key are the same
                    const u16 bit_diff2 = findfirst_bit_range(keylen, key, branch_keylen, branch_key, bit_diff, 4096, s_nullkey);
                    if (bit_diff2 == 4096)
                    {
                        // the keys are the same
                        return false;
                    }
                    else
                    {
                        // keys are not the same so we need to insert a new node
                        const u32 new_item_index = add_item(key, keylen, value);
                        const s8  branch2        = get_bit(keylen, key, bit_diff2);
                        ASSERT(get_bit(branch_keylen, branch_key, bit_diff2) == (1 - branch2));
                        dynnode_t* new_node = alloc_node();
                        new_node->set_bit(bit_diff2);
                        new_node->set_item_index(new_item_index);
                        new_node->set_branch(branch2, new_item_index);
                        new_node->set_branch(1 - branch2, branch_item);
                        new_node->mark_value(branch2);
                        new_node->mark_value(1 - branch2);

                        curnode->set_branch(node_branch, node_to_index(new_node));
                        curnode->mark_node(node_branch);
                        return true;
                    }
                }
                else
                {  // branch is a node, so we need to continue the search
                    prevnode        = curnode;
                    // prevnode_branch = curnode_branch;
                    curnode         = &m_nodes[curnode->get_branch(node_branch)];
                    curnode_branch  = node_branch;
                    bit_start       = bit_diff + 1;
                }
            }
            else
            {
                // we have found a bit that is within the [bit_start, node_bit_end] range and so
                // we need to insert a new node at 'bit_diff'
                const u32  new_item_index = add_item(key, keylen, value);
                const s8   branch2     = get_bit(keylen, key, bit_diff);
                dynnode_t* new_node       = alloc_node();
                new_node->set_bit(bit_diff);
                new_node->set_item_index(new_item_index);
                new_node->set_branch(1 - branch2, node_to_index(curnode));
                new_node->set_branch(branch2, new_item_index);
                new_node->mark_node(1 - branch2);
                new_node->mark_value(branch2);

                // the prevnode (or root) should now point to the new node
                if (prevnode != nullptr)
                {
                    prevnode->set_branch(curnode_branch, node_to_index(new_node));
                }
                else
                {
                    m_root = new_node;
                }
                return true;
            }
        }
    }

    u32 dyntrie_t::add_item(dynkey_t key, u32 keylen, dynval_t value)
    {
        const u32 new_item_index  = m_count++;
        m_keys[new_item_index]    = key;
        m_keylens[new_item_index] = keylen;
        m_values[new_item_index]  = value;
        return new_item_index;
    }

    bool dyntrie_t::find(u8 keylen, dynkey_t key, dynval_t& value) const
    {
        if (m_root == nullptr)
        {
            if (m_first_item == 0xffffffff)
                return false;
            const dynkey_t first_key    = m_keys[m_first_item];
            const u8       first_keylen = m_keylens[m_first_item];
            if (compare_keys(keylen, key, first_keylen, first_key, 0))
            {
                value = m_values[m_first_item];
                return true;
            }
            return false;
        }

        const dynnode_t* node      = m_root;
        u16              bit_start = 0;
        while (true)
        {
            const u16 node_bit    = node->get_bit();
            const u32 node_item = node->get_item_index();

            const u16 bit_diff = findfirst_bit_range(keylen, key, m_keylens[node_item], m_keys[node_item], bit_start, node_bit, s_nullkey);
            if (bit_diff == node_bit)
            {
                // the bit range is identical, let's traverse further down the tree
                const s8 node_branch = get_bit(keylen, key, bit_diff);
                if (node->is_value(node_branch))
                {  // branch index is a value, ok see if they have the same key
                    const u32 branch_item_index = node->get_branch(node_branch);
                    if (compare_keys(keylen, key, m_keylens[branch_item_index], m_keys[branch_item_index], bit_start))
                    {
                        value = m_values[node->get_branch(node_branch)];
                        return true;
                    }
                    return false;  // keys are not the same, it doesn't exist
                }
                else
                {  // branch index is a node, so we can iterate further
                    const u32 node_index = node->get_branch(node_branch);
                    node      = &m_nodes[node_index];
                    bit_start = bit_diff + 1;
                }
            }
            else
            {
                // we have found a get_bit that is within the [bit_start, node_bit_end] range and so
                // this means that the key doesn't exist
                break;
            }
        }
        return false;
    }

    bool dyntrie_t::remove(u8 keylen, dynkey_t key, dynval_t& value)
    {
        // removal of a key is a bit tricky, we need to traverse the tree and find the node that
        // contains the key, then we need to remove the key from the node, remove the node by
        // replacing the parent branch with the other branch

        // when removing an item one thing to keep in mind is that the item index can be
        // used in any number of nodes. So we might have to iterate again replacing any
        // item index reference to the one being removed.

        return false;
    }

}  // namespace ncore
