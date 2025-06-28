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

    void* g_malloc(u32 size, u32 align = sizeof(void*));
    void  g_free(void* ptr);

};  // namespace ncore

#endif  ///< __CBASE_ALLOCATOR_H__
