#ifndef __CBASE_ALLOCATOR_H__
#define __CBASE_ALLOCATOR_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_allocator.h"
#include "cbase/c_context.h"
#include "ccore/c_debug.h"

namespace ncore
{
    void     g_init_system_alloc();
    void     g_exit_system_alloc();
    alloc_t* g_get_system_alloc();

    // Global new and delete
    template <typename T, typename... Args>
    T* g_new(alloc_t* alloc, Args... args)
    {
        void* mem    = alloc->allocate(sizeof(T));
        T*    object = new (mem) T(args...);
        return object;
    }

    template <typename T>
    void g_delete(alloc_t* alloc, T* p)
    {
        p->~T();
        alloc->deallocate(p);
    }

    template <typename T>
    class array_pool_t : public pool_t<T>
    {
    public:
        array_pool_t();
        ~array_pool_t() = default;

        void setup(T* array_item, u32 countof_item);

        DCORE_CLASS_PLACEMENT_NEW_DELETE

        T*  m_data;
        u32 m_countof;
        u32 m_freelist;
        u32 m_freeindex;

    protected:
        virtual u32   v_allocsize() const final;
        virtual void* v_allocate() final;
        virtual void  v_deallocate(void*) final;

        virtual void* v_idx2ptr(u32 index) final;
        virtual u32   v_ptr2idx(void const* ptr) const final;
    };

    template <typename T>
    inline array_pool_t<T>::array_pool_t()
        : m_data(nullptr)
        , m_countof(0)
        , m_freelist(D_NILL_U32)
        , m_freeindex(D_NILL_U32)
    {
    }

    template <typename T>
    inline void array_pool_t<T>::setup(T* array_items, u32 number_of_items)
    {
        m_data      = array_items;
        m_countof   = number_of_items;
        m_freelist  = D_NILL_U32;
        m_freeindex = 0;
        ASSERT(sizeof(T) >= sizeof(u32));  // Can only deal with items that are 4 bytes or more
    }

    template <typename T>
    u32 array_pool_t<T>::v_allocsize() const
    {
        return (u32)sizeof(T);
    }

    template <typename T>
    void* array_pool_t<T>::v_allocate()
    {
        u32 freeitem = m_freelist;
        if (freeitem != D_NILL_U32)
        {
            m_freelist = *(u32*)v_idx2ptr(freeitem);
        }
        else if (m_freeindex < m_countof)
        {
            freeitem = m_freeindex++;
        }

        if (freeitem == D_NILL_U32)
            return nullptr;

        return v_idx2ptr(freeitem);
    }

    template <typename T>
    void array_pool_t<T>::v_deallocate(void* p)
    {
        u32 const idx  = v_ptr2idx(p);
        u32*      item = (u32*)p;
        *item          = m_freelist;
        m_freelist     = idx;
    }

    template <typename T>
    void* array_pool_t<T>::v_idx2ptr(u32 index)
    {
        ASSERT(index != D_NILL_U32 && index < m_freeindex);
        return &m_data[index];
    }

    template <typename T>
    u32 array_pool_t<T>::v_ptr2idx(void const* ptr) const
    {
        if (ptr == nullptr)
            return D_NILL_U32;
        u32 const i = (u32)((T const*)ptr - m_data);
        ASSERT(i < m_freeindex);
        return i;
    }

    void* g_malloc(u32 size, u32 align = sizeof(void*));
    void  g_free(void* ptr);

    // Type malloc and free
    template <typename T, typename... Args>
    T* g_new(Args... args)
    {
        void* mem = g_malloc(sizeof(T));
        T*    obj = new (mem) T(args...);
        return obj;
    }

    template <class T>
    inline void g_delete(T* p)
    {
        p->~T();
        g_free(p);
    }

};  // namespace ncore

#endif  ///< __CBASE_ALLOCATOR_H__
