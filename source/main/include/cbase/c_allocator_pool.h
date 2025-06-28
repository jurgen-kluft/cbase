#ifndef __CBASE_ALLOCATOR_POOL_H__
#define __CBASE_ALLOCATOR_POOL_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_allocator.h"
#include "cbase/c_context.h"
#include "ccore/c_debug.h"

namespace ncore
{
    template <typename T>
    class fixed_pool_t
    {
    public:
        T*  m_data;
        s32 m_capacity;
        s32 m_free_index;
        s32 m_free_head;

        inline void setup(T* data, s32 capacity)
        {
            ASSERT(data != nullptr && capacity > 0);
            m_data       = data;
            m_capacity   = capacity;
            m_free_index = 0;
            m_free_head  = -1;
        }

        inline void setup(alloc_t* allocator, s32 capacity)
        {
            m_data     = g_allocate_array_and_clear<T>(allocator, capacity);
            m_capacity = capacity;
        }

        inline void teardown()
        {
            m_data       = nullptr;
            m_capacity   = 0;
            m_free_index = 0;
            m_free_head  = -1;
        }

        inline void teardown(alloc_t* allocator)
        {
            ASSERT(m_data != nullptr);
            allocator->deallocate(m_data);
            m_data       = nullptr;
            m_capacity   = 0;
            m_free_index = 0;
            m_free_head  = -1;
        }

        inline u32 obj2idx(T const* object) const
        {
            ASSERT(object != nullptr);
            return (u32)(object - m_data);
        }

        inline T* idx2obj(u32 idx) const
        {
            ASSERT(idx < (u32)m_capacity);
            return &m_data[idx];
        }

        inline T* allocate()
        {
            T* object = nullptr;
            if (m_free_head >= 0)
            {
                const s32 index = m_free_head;
                object          = &m_data[index];

                s32* node   = (s32*)object;
                m_free_head = *node;  // Get the next free index

                byte* byte_object = (byte*)object;
                for (s32 i = 0; i < sizeof(T); i++)
                    byte_object[i] = 0;
            }
            else if (m_free_index < m_capacity)
            {
                const s32 index = m_free_index++;
                object          = &m_data[index];

                byte* byte_object = (byte*)object;
                for (s32 i = 0; i < sizeof(T); i++)
                    byte_object[i] = 0;
            }
            return object;
        }

        inline void deallocate(T* object)
        {
            ASSERT(object != nullptr);
            s32* node   = (s32*)object;
            *node       = m_free_head;             // Set the next free index
            m_free_head = (s32)(object - m_data);  // Update the head to this object
        }
    };

};  // namespace ncore

#endif  // __CBASE_ALLOCATOR_POOL_H__
