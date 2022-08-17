#ifndef __CBASE_QUICK_SORT_H__
#define __CBASE_QUICK_SORT_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "cbase/c_debug.h"

namespace ncore
{
    //----------------------------------------------------------------------------------------------------------------
    // Custom QuickSort

    extern void g_qsort(void *a,	// element_array
        s32 n,					// element_count
        s32 es,					// element_size
        s32 (*cmp)(const void* le, const void* re, void* data),
        void* data = 0);
    
    template<typename T>
    inline void g_qsort(T* a, s32 n)
    {
        auto cmper = [](const void* _lhs, const void* _rhs, void* data) 
        {
            T const* const lhs = (T const*)_lhs;
            T const* const rhs = (T const*)_rhs;
            if (*lhs < *rhs)
                return -1;
            if (*lhs > *rhs)
                return 1;
            return 0;
        };
        g_qsort((void*)a, n, sizeof(T), cmper, nullptr);
    }
    

};

#endif // __CBASE_QUICK_SORT_H__
