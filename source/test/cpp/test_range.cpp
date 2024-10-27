#include "cbase/c_allocator.h"
#include "cbase/c_context.h"
#include "cbase/c_binmap.h"
#include "cbase/c_integer.h"
#include "cbase/c_memory.h"

#include "cunittest/cunittest.h"

namespace ncore
{
    /*
    1 TB / 16384 = 64 MB

    Since we also want to use the u16 node[16384] array as a mapping array when
    deallocating memory, we need to make sure that when we reserve a size of for
    example 256 MB that we fill in 4 nodes in the array.
    Same for 512 MB, 1 GB, 2 GB, 4 GB, we need to fill in 8, 16, 32, 64 nodes.

    So we have a node[16384] array and another array section_index[16384] that
    we use to get the section index.

    For above a certain allocation size we can indicate that we are not mapping
    to a section but directly to a chunk ?

    Should we take the below range_t and make it hold u16, so that we can cover
    the 1 TB range with max 65536 nodes, each node is 16 MB in size.
    This would make managing sections easier, as we can have a global mechanic
    to allocate and deallocate sections.
    */

    // clang-format off
    // 64 nodes
    //   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59  60  61  62  63
    // [ 1 | 1 | 2 | . | 3 | . | . | . | 4 | . | . | . | . | . | . | . | 5 | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | 6 | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . ]
    // [ 7 | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . ]
    // clang-format on

    // Segment, 2GB is split into sections of different power-of-2 sizes
    // 4 GB / 64 = 64 MB
    const u32 c_address_null = 0xffffffff;
    struct range_t
    {
        const u8 c_null = 0xff;

        // node state is 1 bit per node, 0 = free, 0x80 = allocated
        // the lower 7 bits is the size range of the node
        u8 m_size[64];         // Size is the size_index + 1, so 1 = 32 MB, 2 = 64 MB, 3 = 128 MB, etc.
        u8 m_node_next[64];    // Next node in the size list
        u8 m_node_prev[64];    // Previous node in the size list
        u8 m_size_lists[64];   // Size starts at 32 MB (0) with increments, 64 MB (1), 128 MB (2), 256 MB (3), 512 MB (4), 1 GB (5), 2 GB (6), 4 GB (7)
        u8 m_base_size_shift;  // ilog2(address_range / 64)
        u8 m_max_size;
        u8 m_size_list_occupancy;  // Which size list index is occupied

        void setup(u64 address_range);

        bool allocate(u64 size, u64& out_address);  // Returns false if the size is not available
        void deallocate(u64 address);               // Deallocates the memory at the given address

        bool split(u8 node);  // Splits the node into two nodes
        u8   get_sibling(u8 node) const;

        void add_size(u8 size_index, u8 node);
        void remove_size(u8 size_index, u8 node);
    };

    void range_t::setup(u64 address_range)
    {
        m_base_size_shift = math::ilog2(address_range / 64);
        for (u32 i = 0; i < 8; ++i)
            m_size_lists[i] = c_null;

        m_max_size               = math::ilog2(address_range) - m_base_size_shift;
        m_size_list_occupancy    = 1 << m_max_size;  // Mark the largest size as occupied
        m_size_lists[m_max_size] = 0;                // The largest node in the size list
        m_size[0]                = m_max_size;       // Full size range (size list index 7)
        m_node_next[0]           = 0;                // Link the node to itself
        m_node_prev[0]           = 0;                // Link the node to itself
    }

    inline u8 range_t::get_sibling(u8 node) const
    {
        u8 const d = 64 >> (m_max_size - m_size[node]);
        return (node & 1) ? (node - d) : (node + d);
    }

    bool range_t::split(u8 node)
    {
        const u8 size = m_size[node];
        remove_size(size, node);

        // Split the node into two nodes
        m_size[node] = size - 1;
        u8 const d   = 64 >> (m_max_size - (size - 1));
        if (d == 0)
            return false;

        u8 const insert = (node & 1) ? (node - d) : (node + d);
        m_size[insert]  = size - 1;

        // Invalidate the next and previous pointers of the new node (debugging)
        m_node_next[insert] = c_null;
        m_node_prev[insert] = c_null;

        // Add node and insert into the size list that is (size - 1)
        add_size(size - 1, node);
        add_size(size - 1, insert);
        return true;
    }

    void range_t::add_size(u8 size, u8 node)
    {
        u8& head = m_size_lists[size];
        if (head == c_null)
        {
            head              = node;
            m_node_next[node] = node;
            m_node_prev[node] = node;
            return;
        }

        u8 const prev     = m_node_prev[head];
        m_node_next[prev] = node;
        m_node_prev[head] = node;
        m_node_next[node] = head;
        m_node_prev[node] = prev;

        m_size_list_occupancy |= (1 << size);
    }

    void range_t::remove_size(u8 size, u8 node)
    {
        ASSERT(size >= 0 && size <= m_max_size);
        u8& head                       = m_size_lists[size];
        head                           = (head == node) ? m_node_next[node] : head;
        head                           = (head == node) ? c_null : head;
        m_node_next[m_node_prev[node]] = m_node_next[node];
        m_node_prev[m_node_next[node]] = m_node_prev[node];
        m_node_next[node]              = c_null;
        m_node_prev[node]              = c_null;

        if (head == c_null)
            m_size_list_occupancy &= ~(1 << (size - 1));
    }

    // The only size requests allowed are: 32 MB, 64 MB, 128 MB, 256 MB, 512 MB, 1 GB, 2 GB, 4 GB
    bool range_t::allocate(u64 allocation_size, u64& out_address)
    {
        s8 const pow2_index = math::ilog2(allocation_size);
        if (pow2_index < m_base_size_shift || pow2_index > (m_base_size_shift + m_max_size))
            return false;

        s32 const size = (pow2_index - m_base_size_shift);
        ASSERT(size <= m_max_size);

        u8 node = m_size_lists[size];
        if (node == c_null)
        {
            // In the occupancy find a bit set that is above our size
            // Mask out the bits below size and then do a find first bit
            u8 const occupancy = m_size_list_occupancy & ~((1 << size) - 1);
            if (occupancy == 0)  // There are no free sizes available
                return false;

            u8 bit = math::findFirstBit(occupancy);
            node   = m_size_lists[bit];
            while (bit > size)
            {
                split(node);  // Split the node until we reach the size
                bit -= 1;
            }
        }
        else
        {
            node = m_size_lists[size];
        }

        remove_size(size, node);

        m_size[node] |= 0x80;  // Set the node as allocated

        out_address = (u64)node << m_base_size_shift;
        return true;
    }

    void range_t::deallocate(u64 address)
    {
        if (address == c_address_null || address >= (64 << m_base_size_shift))
            return;

        s16 const node = address >> m_base_size_shift;
        m_size[node] &= 0x7f;  // Clear the allocation bit

        // Could be that we are freeing the largest size allocation, this means
        // that we should not merge the node with its sibling.
        if (m_size_list_occupancy == 0)
            return;

        // Check if the sibling is free, if so merge the two nodes
        u8 const d = 64 >> (m_max_size - m_size[node]);
        if (d == 0)
            return;
        u8 const sibling = (node & 1) ? (node - d) : (node + d);
        if ((m_size[sibling] & 0x80) == 0)
        {
            remove_size(m_size[sibling], sibling);  // Sibling is being merged, remove it
            m_size[node] += 1;                      // Double the size of the node
            m_size[sibling] = c_null;               // Invalidate the sibling node
            add_size(m_size[node], node);           // Add the merged node to the size list
        }
    }
}  // namespace ncore

using namespace ncore;

UNITTEST_SUITE_BEGIN(range)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(some_sizes)
        {
            range_t range;
            range.setup(2 * cGB);

            u64 address = 0;
            CHECK_TRUE(range.allocate(32 * cMB, address));
            CHECK_EQUAL(0, address);

            CHECK_TRUE(range.allocate(32 * cMB, address));
            CHECK_EQUAL(32 * cGB, address);

            CHECK_TRUE(range.allocate(64 * cMB, address));
            CHECK_EQUAL(64 * cMB, address);

            CHECK_TRUE(range.allocate(128 * cMB, address));
            CHECK_EQUAL(128 * cMB, address);

            CHECK_TRUE(range.allocate(256 * cMB, address));
            CHECK_EQUAL(256 * cMB, address);

            CHECK_TRUE(range.allocate(512 * cMB, address));
            CHECK_EQUAL(512 * cMB, address);

            CHECK_TRUE(range.allocate(1 * cGB, address));
            CHECK_EQUAL(1 * cGB, address);
        }
    }
}
