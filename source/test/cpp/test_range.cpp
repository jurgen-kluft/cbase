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

    // Segment, 2GB is split into sections of different power-of-2 sizes
    // 4 GB / 64 = 64 MB
    const u32 c_address_null = 0xffffffff;
    struct range_t
    {
        const u8    c_null = 0xff;
        typedef u8  node_t;
        typedef u16 span_t;

        // node state is 1 bit per node, 0 = free, 0x80 = allocated
        // the lower 7 bits is the size range of the node
        u8*     m_node_size;            // Size is the size_index + 1, so 1 = 32 MB, 2 = 256 MB, 3 = 128 MB, etc.
        node_t* m_node_next;            // Next node in the size list
        node_t* m_node_prev;            // Previous node in the size list
        u16*    m_node_free;            // Single bit per node, 0 = free, 1 = allocated
        node_t  m_size_lists[16];       // Every size has its own list
        u16     m_size_list_occupancy;  // Which size list index is occupied
        u8      m_base_size_min;        // ilog2(address_range / 64)
        u8      m_base_size_range;
        u8      m_node_count_shift;

        void setup(alloc_t* allocator, u64 address_range = 1 * cTB, u8 node_count_shift = 8);
        void teardown(alloc_t* allocator);

        bool allocate(u64 size, u64& out_address);  // Returns false if the size is not available
        bool deallocate(u64 address);               // Deallocates the memory at the given address

        bool split(node_t node);  // Splits the node into two nodes

        void add_size(u8 size_index, node_t node);
        void remove_size(u8 size_index, node_t node);
    };

    void range_t::setup(alloc_t* allocator, u64 address_range, u8 node_count_shift)
    {
        m_node_count_shift = node_count_shift;
        m_base_size_min    = math::ilog2(address_range >> node_count_shift);
        for (u32 i = 0; i < g_array_size(m_size_lists); ++i)
            m_size_lists[i] = c_null;

        u32 const node_count = 1 << node_count_shift;
        m_node_size          = g_allocate_array_and_clear<u8>(allocator, node_count);
        m_node_next          = g_allocate_array_and_memset<node_t>(allocator, node_count, c_null);
        m_node_prev          = g_allocate_array_and_memset<node_t>(allocator, node_count, c_null);
        m_node_free          = g_allocate_array_and_clear<u16>(allocator, (node_count + 15) >> (sizeof(u16) * 2));

        m_base_size_min                 = math::ilog2(address_range / node_count);
        m_base_size_range               = math::ilog2(address_range) - m_base_size_min;
        m_size_list_occupancy           = 1 << m_base_size_range;  // Mark the largest size as occupied
        m_size_lists[m_base_size_range] = 0;                       // The largest node in the size list
        m_node_size[0]                  = node_count - 1;          // Full size range
        m_node_next[0]                  = 0;                       // Link the node to itself
        m_node_prev[0]                  = 0;                       // Link the node to itself
    }

    void range_t::teardown(alloc_t* allocator)
    {
        allocator->deallocate(m_node_size);
        allocator->deallocate(m_node_next);
        allocator->deallocate(m_node_prev);
        allocator->deallocate(m_node_free);
    }

    bool range_t::split(node_t node)
    {
        const span_t span  = (span_t)m_node_size[node] + 1;
        const u8     index = math::ilog2(span);

        remove_size(index, node);

        // Split the node into two nodes
        node_t const insert = node + (span >> 1);
        m_node_size[node]   = (node_t)((span >> 1) - 1);
        m_node_size[insert] = (node_t)((span >> 1) - 1);

        // Invalidate the next and previous pointers of the new node (debugging)
        m_node_next[insert] = c_null;
        m_node_prev[insert] = c_null;

        // Add node and insert into the size list that is (index - 1)
        add_size(index - 1, node);
        add_size(index - 1, insert);

        return true;
    }

    void range_t::add_size(u8 index, node_t node)
    {
        node_t& head = m_size_lists[index];
        if (head == c_null)
        {
            head              = node;
            m_node_next[node] = node;
            m_node_prev[node] = node;
            m_size_list_occupancy |= (1 << index);
            return;
        }

        node_t const prev = m_node_prev[head];
        m_node_next[prev] = node;
        m_node_prev[head] = node;
        m_node_next[node] = head;
        m_node_prev[node] = prev;

        m_size_list_occupancy |= (1 << index);
    }

    void range_t::remove_size(u8 index, node_t node)
    {
        ASSERT(index >= 0 && index <= 8);
        node_t& head                   = m_size_lists[index];
        head                           = (head == node) ? m_node_next[node] : head;
        head                           = (head == node) ? c_null : head;
        m_node_next[m_node_prev[node]] = m_node_next[node];
        m_node_prev[m_node_next[node]] = m_node_prev[node];
        m_node_next[node]              = c_null;
        m_node_prev[node]              = c_null;

        if (head == c_null)
            m_size_list_occupancy &= ~(1 << index);
    }

    // The only size requests allowed are: 32 MB, 64 MB, 128 MB, 256 MB, 512 MB, 1 GB, 2 GB, 4 GB
    bool range_t::allocate(u64 _size, u64& out_address)
    {
        span_t const span  = (span_t)(_size >> m_base_size_min);
        u8 const     index = math::ilog2(span);

        node_t node = m_size_lists[index];
        if (node == c_null)
        {
            // In the occupancy find a bit set that is above our size
            // Mask out the bits below size and then do a find first bit
            u64 const occupancy = m_size_list_occupancy & ~((1 << index) - 1);
            if (occupancy == 0)  // There are no free sizes available
                return false;

            u8 occ = math::findFirstBit(occupancy);
            node   = m_size_lists[occ];
            while (occ > index)
            {
                split(node);  // Split the node until we reach the size
                occ -= 1;
            }
        }

        remove_size(index, node);

        m_node_free[node >> 4] |= (1 << (node & 15));

        out_address = (u64)node << m_base_size_min;
        return true;
    }

    bool range_t::deallocate(u64 address)
    {
        if (address == c_address_null || address >= (((u64)1 << m_base_size_min) << 8))
            return false;

        node_t node = (node_t)(address >> m_base_size_min);
        m_node_free[node >> 4] &= ~(1 << (node & 15));

        span_t span  = (span_t)m_node_size[node] + 1;
        u8     index = math::ilog2(span);

        while (true)
        {
            // Could be that we are freeing the largest size allocation, this means
            // that we should not merge the node with its sibling.
            if (index == m_node_count_shift)
                break;

            const node_t sibling = ((node & ((span << 1) - 1)) == 0) ? node + span : node - span;

            // Check if the sibling is free
            if ((m_node_free[sibling >> 4] & (1 << (sibling & 15))) != 0)
                break;

            remove_size(index, sibling);                            // Sibling is being merged, remove it
            node_t const merged = node < sibling ? node : sibling;  // Always take the left node as the merged node
            node_t const other  = node < sibling ? sibling : node;  // Always take the right node as the other node
            m_node_size[merged] = (u8)((1 << (index + 1)) - 1);     // Double the span (size) of the node
            m_node_size[other]  = c_null;                           // Invalidate the node that is merged into 'merged'

            node  = merged;
            span  = (span_t)m_node_size[node] + 1;
            index = math::ilog2(span);
        }

        add_size(index, node);  // Add the merged node to the size list

        return true;
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

        UNITTEST_TEST(allocate)
        {
            range_t range;
            range.setup(Allocator, 4 * cGB, 8);

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

        UNITTEST_TEST(allocate_and_deallocate)
        {
            range_t range;
            range.setup(Allocator, 4 * cGB, 8);

            for (s32 i = 0; i < 16; ++i)
            {
                u64 address = 0;
                CHECK_TRUE(range.allocate(32 * cMB, address));
                CHECK_EQUAL(0, address);

                u64 address2 = 0;
                CHECK_TRUE(range.allocate(32 * cMB, address2));
                CHECK_EQUAL(32 * cMB, address2);

                u64 address3 = 0;
                CHECK_TRUE(range.allocate(64 * cMB, address3));
                CHECK_EQUAL(64 * cMB, address3);

                CHECK_TRUE(range.deallocate(address));
                CHECK_TRUE(range.deallocate(address2));
                CHECK_TRUE(range.deallocate(address3));
            }

            range.teardown(Allocator);
        }
    }
}
