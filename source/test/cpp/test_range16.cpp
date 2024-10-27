#include "cbase/c_allocator.h"
#include "cbase/c_context.h"
#include "cbase/c_binmap.h"
#include "cbase/c_integer.h"
#include "cbase/c_memory.h"

#include "cunittest/cunittest.h"

namespace ncore
{
    const u32 c_address_null = 0xffffffff;
    struct range16_t
    {
        const u16   c_null = 0xffff;
        typedef u16 node_t;

        // node state is 1 bit per node, 0 = free, 0x80 = allocated
        // the lower 7 bits is the size range of the node
        u8*     m_node_size;            // Size is the size_index + 1, so 1 = 32 MB, 2 = 64 MB, 3 = 128 MB, etc.
        node_t* m_node_next;            // Next node in the size list
        node_t* m_node_prev;            // Previous node in the size list
        node_t  m_size_lists[64];       // Size starts at 32 MB (0) with increments, 64 MB (1), 128 MB (2), 256 MB (3), 512 MB (4), 1 GB (5), 2 GB (6), 4 GB (7)
        u64     m_size_list_occupancy;  // Which size list index is occupied
        u8      m_base_size_min;        // ilog2(address_range >> m_node_count_shift)
        u8      m_base_size_range;      // ilog2(address_range) - m_base_size_min
        u8      m_node_count_shift;     // 16, 15, 14, 13, 12, 11, 10

        void setup(alloc_t* allocator, u64 address_range = 1 * cTB, u8 node_count_shift = 14);
        void teardown(alloc_t* allocator);

        bool allocate(u64 size, u64& out_address);  // Returns false if the size is not available
        void deallocate(u64 address);               // Deallocates the memory at the given address

        bool split(node_t node);  // Splits the node into two nodes
        void add_size(u8 size_index, node_t node);
        void remove_size(u8 size_index, node_t node);
    };

    void range16_t::setup(alloc_t* allocator, u64 address_range, u8 node_count_shift)
    {
        m_node_count_shift = node_count_shift;
        m_base_size_min    = math::ilog2(address_range >> node_count_shift);
        for (u32 i = 0; i < 64; ++i)
            m_size_lists[i] = c_null;

        u32 const node_count = 1 << node_count_shift;
        m_node_size          = g_allocate_array_and_clear<u8>(allocator, node_count);
        m_node_next          = g_allocate_array_and_clear<node_t>(allocator, node_count);
        m_node_prev          = g_allocate_array_and_clear<node_t>(allocator, node_count);

        m_base_size_range               = math::ilog2(address_range) - m_base_size_min;
        m_size_list_occupancy           = 1 << m_base_size_range;  // Mark the largest size as occupied
        m_size_lists[m_base_size_range] = 0;                       // The largest node in the size list
        m_node_size[0]                  = m_base_size_range;       // Full size range (size list index 7)
        m_node_next[0]                  = 0;                       // Link the node to itself
        m_node_prev[0]                  = 0;                       // Link the node to itself
    }

    void range16_t::teardown(alloc_t* allocator)
    {
        allocator->deallocate(m_node_size);
        allocator->deallocate(m_node_next);
        allocator->deallocate(m_node_prev);
    }

    bool range16_t::split(node_t node)
    {
        const u8 size = m_node_size[node];
        remove_size(size, node);

        // Split the node into two nodes
        m_node_size[node] = size - 1;
        u16 const d       = (1 << m_node_count_shift) >> (m_base_size_range - (size - 1));
        if (d == 0)
            return false;

        node_t const insert = (node & 1) ? (node - d) : (node + d);
        m_node_size[insert] = size - 1;

        // Invalidate the next and previous pointers of the new node (debugging)
        m_node_next[insert] = c_null;
        m_node_prev[insert] = c_null;

        // Add node and insert into the size list that is (size - 1)
        add_size(size - 1, node);
        add_size(size - 1, insert);
        return true;
    }

    void range16_t::add_size(u8 size, node_t node)
    {
        node_t& head = m_size_lists[size];
        if (head == c_null)
        {
            head              = node;
            m_node_next[node] = node;
            m_node_prev[node] = node;
            return;
        }

        node_t const prev = m_node_prev[head];
        m_node_next[prev] = node;
        m_node_prev[head] = node;
        m_node_next[node] = head;
        m_node_prev[node] = prev;

        m_size_list_occupancy |= (1 << size);
    }

    void range16_t::remove_size(u8 size, node_t node)
    {
        ASSERT(size >= 0 && size <= m_base_size_range);
        node_t& head                   = m_size_lists[size];
        head                           = (head == node) ? m_node_next[node] : head;
        head                           = (head == node) ? c_null : head;
        m_node_next[m_node_prev[node]] = m_node_next[node];
        m_node_prev[m_node_next[node]] = m_node_prev[node];
        m_node_next[node]              = c_null;
        m_node_prev[node]              = c_null;

        if (head == c_null)
            m_size_list_occupancy &= ~(1 << (size - 1));
    }

    bool range16_t::allocate(u64 allocation_size, u64& out_address)
    {
        s8 const allocation_shift = math::ilog2(allocation_size);
        if (allocation_shift < m_base_size_min || allocation_shift > (m_base_size_min + m_base_size_range))
            return false;

        s8 const size = (allocation_shift - m_base_size_min);
        ASSERT(size >= 0 && size <= m_base_size_range);

        node_t node = m_size_lists[size];
        if (node == c_null)
        {
            // In the occupancy find a bit set that is above our size
            // Mask out the bits below size and then do a find first bit
            u64 const occupancy = m_size_list_occupancy & ~((1 << size) - 1);
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

        remove_size(size, node);

        m_node_size[node] |= 0x80;  // Set the node as allocated

        out_address = (u64)node << m_base_size_min;
        return true;
    }

    void range16_t::deallocate(u64 address)
    {
        if (address == c_address_null || address >= (64 << m_base_size_min))
            return;

        node_t const node = address >> m_base_size_min;
        m_node_size[node] &= 0x7f;  // Clear the allocation bit

        // Could be that we are freeing the largest size allocation, this means
        // that we should not merge the node with its sibling.
        if (m_size_list_occupancy == 0)
            return;

        // Check if the sibling is free, if so merge the two nodes
        u8 const d = (1 << m_node_count_shift) >> (m_base_size_range - m_node_size[node]);
        if (d == 0)
            return;

        u8 const sibling = (node & 1) ? (node - d) : (node + d);
        if ((m_node_size[sibling] & 0x80) == 0)
        {
            remove_size(m_node_size[sibling], sibling);  // Sibling is being merged, remove it
            m_node_size[node] += 1;                      // Double the size of the node
            m_node_size[sibling] = c_null;               // Invalidate the sibling node
            add_size(m_node_size[node], node);           // Add the merged node to the size list
        }
    }
}  // namespace ncore

using namespace ncore;

UNITTEST_SUITE_BEGIN(range16)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(allocate)
        {
            range16_t range;
            range.setup(Allocator, 1 * cTB, 15);

            u64 address = 0;
            CHECK_TRUE(range.allocate(32 * cMB, address));
            CHECK_EQUAL(0, address);

            CHECK_TRUE(range.allocate(32 * cMB, address));
            CHECK_EQUAL(32 * cMB, address);

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

            range.teardown(Allocator);
        }
    }
}
