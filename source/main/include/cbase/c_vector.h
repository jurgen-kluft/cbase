#ifndef __CBASE_VECTOR_H__
#define __CBASE_VECTOR_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"

namespace ncore
{
    struct vmem_arena_t;

    namespace nvector
    {
        vmem_arena_t* g_alloc_vmem_arena(u32 reserved, u32 committed, s32 item_size);
        void          g_free_vmem_arena(vmem_arena_t*& arena);
        bool          g_set_capacity(vmem_arena_t* arena, u32& length, u32 new_capacity, s32 item_size);
        u32           g_get_reserved(vmem_arena_t* arena, s32 item_size);
        void          g_vmem_arena_reserved(vmem_arena_t* arena, u32 reserved_size);
        void*         g_vmem_arena_allocate(vmem_arena_t* arena, u32 size, s32 alignment);
    }  // namespace nvector

    // Simple vector_t<> template class that uses a virtual memory arena for storage.
    template <typename T>
    class vector_t
    {
    public:
        DCORE_CLASS_PLACEMENT_NEW_DELETE
        inline vector_t()
            : m_items(nullptr)
            , m_arena(nullptr)
            , m_size(0)
            , m_capacity(0)
        {
        }

        vector_t(u32 items_capacity, u32 items_reserved)
            : m_items(nullptr)
            , m_arena(nullptr)
            , m_size(0)
            , m_capacity(items_capacity)
        {
            ASSERT(items_capacity <= items_reserved);
            m_arena = nvector::g_alloc_vmem_arena(items_reserved, items_capacity, sizeof(T));
            nvector::g_vmem_arena_reserved(m_arena, items_reserved * sizeof(T));
            m_items = (T*)nvector::g_vmem_arena_allocate(m_arena, items_capacity * sizeof(T), alignof(T));
        }
        ~vector_t() { nvector::g_free_vmem_arena(m_arena); }

        inline bool set_capacity(u32 new_capacity) { return nvector::g_set_capacity(m_arena, m_capacity, new_capacity, sizeof(T)); }

        inline u32  size() const { return m_size; }
        inline void set_size(u32 size) { m_size = size; }
        inline u32  capacity() const { return m_capacity; }
        inline u32  reserved() const { return nvector::g_get_reserved(m_arena, sizeof(T)); }

        bool add_item(const T& item)
        {
            if (m_size < m_capacity)
            {
                m_items[m_size++] = item;
                return true;
            }
            return false;
        }

        bool set_item(u32 index, const T& item)
        {
            if (index < m_size)
            {
                m_items[index] = item;
                return true;
            }
            return false;
        }

        bool pop_item(T& item)
        {
            if (m_size > 0)
            {
                item = m_items[--m_size];
                return true;
            }
            return false;
        }

        void swap_remove(u32 index)
        {
            if (index < m_size)
            {
                m_items[index] = m_items[--m_size];
            }
        }

        T*              get_item(u32 index) { return &m_items[index]; }
        T const*        get_item(u32 index) const { return &m_items[index]; }
        inline T*       items() { return m_size; }
        inline T const* items() const { return m_size; }

        inline bool equal_items(u32 lhs_index, u32 rhs_index) const
        {
            T* lhs = &m_items[lhs_index];
            T* rhs = &m_items[rhs_index];
            return *lhs == *rhs;
        }

        inline s32 compare_items(u32 lhs_index, u32 rhs_index) const
        {
            T* lhs = &m_items[lhs_index];
            T* rhs = &m_items[rhs_index];
            if (*lhs < *rhs)
                return -1;
            else if (*lhs > *rhs)
                return -1;
            return 0;
        }

        T*            m_items;
        u32           m_size;
        u32           m_capacity;
        vmem_arena_t* m_arena;
    };

};  // namespace ncore

#endif  // __CBASE_VECTOR_H__
