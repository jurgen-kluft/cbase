#include "ccore/c_target.h"
#include "cbase/c_allocator.h"

namespace ncore
{
    static inline u32 s_get_item_idx(void const* array_items, void const* item, u32 sizeof_item)
    {
        u32 const index = (u32)(((u64)item - (u64)array_items) / sizeof_item);
        return index;
    }

    static inline u8* s_get_item_ptr(void* array_item, u32 index, u32 sizeof_item) { return (u8*)array_item + (index * sizeof_item); }

    dexed_array_t::dexed_array_t(void* array_item, u32 sizeof_item, u32 countof_item)
        : m_data(array_item)
        , m_sizeof(sizeof_item)
        , m_countof(countof_item)
    {
    }

    void* dexed_array_t::v_idx2ptr(u32 index)
    {
        if (index == 0xffffffff)
            return nullptr;
        ASSERT(index < m_countof);
        return s_get_item_ptr(m_data, index, m_sizeof);
    }

    u32 dexed_array_t::v_ptr2idx(void const* ptr) const
    {
        if (ptr == nullptr)
            return 0xffffffff;
        u32 const i = s_get_item_idx(m_data, ptr, m_sizeof);
        ASSERT(i < m_countof);
        return i;
    }

    fsadexed_array_t::fsadexed_array_t()
        : m_data(nullptr)
        , m_sizeof(0)
        , m_countof(0)
        , m_freelist(0xffffffff)
        , m_freeindex(0xffffffff)
    {
    }

    void fsadexed_array_t::setup(void* array_item, u32 sizeof_item, u32 countof_item)
    {
        m_data = (array_item);
        m_sizeof = (sizeof_item);
        m_countof = (countof_item);
        m_freelist = (0xffffffff);
        m_freeindex = (0);
        ASSERT(m_sizeof >= sizeof(u32));  // Can only deal with items that are 4 bytes or more
    }

    u32 fsadexed_array_t::v_allocsize() const { return m_sizeof; }

    void* fsadexed_array_t::v_allocate()
    {
        u32 freeitem = 0xffffffff;
        if (m_freelist != 0xffffffff)
        {
            freeitem   = m_freelist;
            m_freelist = *(u32*)s_get_item_ptr(m_data, m_freelist, m_sizeof);
        }
        else if (m_freeindex < m_countof)
        {
            freeitem = m_freeindex++;
        }

        if (freeitem == 0xffffffff)
            return nullptr;

        return s_get_item_ptr(m_data, freeitem, m_sizeof);
    }

    void fsadexed_array_t::v_deallocate(void* p)
    {
        u32 const idx  = ptr2idx(p);
        u32*      item = (u32*)p;
        *item          = m_freelist;
        m_freelist     = idx;
    }

    void* fsadexed_array_t::v_idx2ptr(u32 index)
    {
        if (index == 0xffffffff)
            return nullptr;
        ASSERT(index < m_freeindex);
        return s_get_item_ptr(m_data, index, m_sizeof);
    }

    u32 fsadexed_array_t::v_ptr2idx(void const* ptr) const
    {
        if (ptr == nullptr)
            return 0xffffffff;
        u32 const i = s_get_item_idx(m_data, ptr, m_sizeof);
        ASSERT(i < m_freeindex);
        return i;
    }

};  // namespace ncore
