#ifndef __CBASE_QUICK_SORT_H__
#define __CBASE_QUICK_SORT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_debug.h"

namespace ncore
{
    //----------------------------------------------------------------------------------------------------------------
    // Custom QuickSort

    extern void g_qsort(void* a,   // element_array
                        s32   ec,  // element_count
                        s32   es,  // element_size
                        s32 (*cmp)(const void* le, const void* re, void* user_data), void* user_data = 0);

    extern void g_qsort2(u16* a,   // element_array (2 bytes, u16, s16)
                         s32  ec,  // element_count
                         s32 (*cmp)(const void* le, const void* re, void* user_data), void* user_data = 0);

    extern void g_qsort4(u32* a,   // element_array (4 bytes, u32, s32, f32)
                         s32  ec,  // element_count
                         s32 (*cmp)(const void* le, const void* re, void* user_data), void* user_data = 0);

    extern void g_qsort8(u64* a,   // element_array (8 bytes, u64, s64, f64)
                         s32  ec,  // element_count
                         s32 (*cmp)(const void* le, const void* re, void* user_data), void* user_data = 0);

    template <typename T>
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

};  // namespace ncore

#endif  // __CBASE_QUICK_SORT_H__
