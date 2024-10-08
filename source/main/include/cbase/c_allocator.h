#ifndef __CBASE_ALLOCATOR_H__
#define __CBASE_ALLOCATOR_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_allocator.h"
#include "ccore/c_debug.h"
#include "cbase/c_context.h"

namespace ncore
{
    void     g_init_system_alloc();
    void     g_exit_system_alloc();
    alloc_t* g_get_system_alloc();

    // The dexer interface, 'pointer to index' and 'index to pointer'
    class dexer_t
    {
    public:
        inline void* idx2ptr(u32 index) { return v_idx2ptr(index); }
        inline u32   ptr2idx(void const* ptr) const { return v_ptr2idx(ptr); }

        template <typename T>
        inline T* idx2obj(u32 index)
        {
            return static_cast<T*>(v_idx2ptr(index));
        }
        template <typename T>
        inline u32 obj2idx(T const* ptr) const
        {
            return v_ptr2idx(ptr);
        }

    protected:
        virtual void* v_idx2ptr(u32 index)             = 0;
        virtual u32   v_ptr2idx(void const* ptr) const = 0;
    };

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
    class pool_t
    {
    public:
        pool_t();

        DCORE_CLASS_PLACEMENT_NEW_DELETE
        void setup(T* array_item, u32 countof_item);

        T*   allocate();
        void deallocate(T*);
        u32  allocsize() const;
        T*   idx2ptr(u32 index);
        u32  ptr2idx(T const* ptr) const;

        T*  m_data;
        u32 m_countof;
        u32 m_freelist;
        u32 m_freeindex;
    };

    template <typename T>
    inline pool_t<T>::pool_t()
        : m_data(nullptr)
        , m_countof(0)
        , m_freelist(0xffffffff)
        , m_freeindex(0xffffffff)
    {
    }

    template <typename T>
    inline void pool_t<T>::setup(T* array_item, u32 countof_item)
    {
        m_data      = (array_item);
        m_countof   = (countof_item);
        m_freelist  = (0xffffffff);
        m_freeindex = (0);
        ASSERT(sizeof(T) >= sizeof(u32));  // Can only deal with items that are 4 bytes or more
    }

    template <typename T>
    u32 pool_t<T>::allocsize() const
    {
        return (u32)sizeof(T);
    }

    template <typename T>
    T* pool_t<T>::allocate()
    {
        u32 freeitem = m_freelist;
        if (freeitem != 0xffffffff)
        {
            m_freelist = *(u32*)idx2ptr(freeitem);
        }
        else if (m_freeindex < m_countof)
        {
            freeitem = m_freeindex++;
        }

        if (freeitem == 0xffffffff)
            return nullptr;

        return idx2ptr(freeitem);
    }

    template <typename T>
    void pool_t<T>::deallocate(T* p)
    {
        u32 const idx  = ptr2idx(p);
        u32*      item = (u32*)p;
        *item          = m_freelist;
        m_freelist     = idx;
    }

    template <typename T>
    T* pool_t<T>::idx2ptr(u32 index)
    {
        ASSERT(index != 0xffffffff && index < m_freeindex);
        return &m_data[index];
    }

    template <typename T>
    u32 pool_t<T>::ptr2idx(T const* ptr) const
    {
        if (ptr == nullptr)
            return 0xffffffff;
        u32 const i = (u32)(ptr - m_data);
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
