#ifndef __XBASE_QUICK_SORT_H__
#define __XBASE_QUICK_SORT_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_debug.h"

namespace xcore
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
            s32 const* const lhs = (s32 const*)_lhs;
            s32 const* const rhs = (s32 const*)_rhs;
            if (*lhs < *rhs)
                return -1;
            if (*lhs > *rhs)
                return 1;
            return 0;
        };
        g_qsort((void*)a, n, sizeof(T), cmper, nullptr);
    }
    

};

#endif // __XBASE_QUICK_SORT_H__
