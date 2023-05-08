#ifndef __CBASE_DARRAY_H__
#define __CBASE_DARRAY_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"

namespace ncore
{
    // NOTE; this is a dynamically resizing array, however you should only use it for POD types since it will not consider constructors/destructors

    class array_capacity_handler_t
    {
    public:
        virtual bool set_capacity(void*& items, u32 sizeof_item, u32 cur_size, u32& cur_capacity, u32 new_capacity) = 0;
        virtual u32  max_capacity() const                                                                           = 0;
    };

    extern array_capacity_handler_t* g_get_default_array_capacity_handler();

    template <typename T>
    class array_t
    {
    public:
        DCORE_CLASS_PLACEMENT_NEW_DELETE

        static array_t<T>* create(u32 initial_size, u32 initial_capacity, array_capacity_handler_t* capacity_handler = nullptr)
        {
            if (capacity_handler == nullptr)
                capacity_handler = g_get_default_array_capacity_handler();

            alloc_t*    alloc         = context_t::runtime_alloc();
            void*       mem           = alloc->allocate(sizeof(array_t<T>), sizeof(void*));
            array_t<T>* array         = new (mem) array_t<T>();
            array->m_capacity_handler = capacity_handler;
            array->m_cap_cur          = 0;
            array->m_size             = 0;
            array->set_capacity(initial_capacity);
            array->set_size(initial_size);
            return array;
        }
        static void destroy(array_t<T>*& a)
        {
            a->set_capacity(0);
            alloc_t* alloc = context_t::runtime_alloc();
            alloc->deallocate(a);
            a = nullptr;
        }

        inline bool set_capacity(u32 capacity) { return m_capacity_handler->set_capacity((void*&)m_items, sizeof(T), m_size, m_cap_cur, capacity); }

        inline u32  size() const { return m_size; }
        inline void set_size(u32 size) { m_size = size; }
        inline u32  cap_cur() const { return m_cap_cur; }
        inline u32  cap_max() const { return m_capacity_handler->max_capacity(); }

        bool add_item(const T& item)
        {
            if (m_size < m_cap_cur)
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

    protected:
        inline array_t()
            : m_items(nullptr)
            , m_capacity_handler(nullptr)
            , m_size(0)
            , m_cap_cur(0)
        {
        }
        ~array_t() { m_capacity_handler->set_capacity((void*&)m_items, sizeof(T), m_size, m_cap_cur, 0); }

        T*                        m_items;
        array_capacity_handler_t* m_capacity_handler;
        u32                       m_size;
        u32                       m_cap_cur;
    };

};  // namespace ncore

#endif  // __CBASE_CARRAY_H__
