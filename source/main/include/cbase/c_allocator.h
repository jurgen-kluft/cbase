#ifndef __CBASE_ALLOCATOR_H__
#define __CBASE_ALLOCATOR_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_allocator.h"

namespace ncore
{
    class alloc_t;

    void     g_init_system_alloc();
    void     g_exit_system_alloc();
    alloc_t* g_get_system_alloc();

    void* g_malloc(u32 size, u32 align = sizeof(void*));
    void  g_free(void* ptr);

    // Global new and delete
    template <typename T, typename... Args>
    T* g_new(Args... args)
    {
        void* mem    = g_malloc(sizeof(T), alignof(T));
        T*    object = new (mem) T(args...);
        return object;
    }

    template <typename T>
    void g_delete(T* p)
    {
        p->~T();
        g_free(p);
    }

};  // namespace ncore

#endif  ///< __CBASE_ALLOCATOR_H__
