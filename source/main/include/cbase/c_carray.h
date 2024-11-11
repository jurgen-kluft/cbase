#ifndef __CBASE_CARRAY_H__
#define __CBASE_CARRAY_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_debug.h"
#include "cbase/c_memory.h"
#include "cbase/c_allocator.h"

namespace ncore
{
    template <typename T>
    struct carray_t
    {
        s32 mSize;
        s32 mCapacity;
        T*  mArray;
    };

    template <typename T>
    struct cview_t
    {
        s32 const      mSize;
        s32 const      mCapacity;
        T const* const mArray;
    };

    template <typename T>
    carray_t<T> g_create_carray(alloc_t* allocator, s32 number_of_elements)
    {
        carray_t<T> array;
        array.mSize     = 0;
        array.mCapacity = number_of_elements;
        array.mArray    = g_allocate_array<T>(allocator, number_of_elements);
        return array;
    }

    template <typename T>
    inline void g_destroy_carray(alloc_t* allocator, carray_t<T>& array)
    {
        g_deallocate_array(allocator, array.mArray);
        array.mArray    = nullptr;
        array.mSize     = 0;
        array.mCapacity = 0;
    }

    template <typename T>
    inline void g_clear(carray_t<T>& array)
    {
        array.mSize = 0;
    }

    template <typename T>
    inline s32 g_size(carray_t<T> const& array)
    {
        return array.mSize;
    }

    template <typename T>
    inline s32 g_size(cview_t<T> const& array)
    {
        return array.mSize;
    }

    template <typename T>
    inline s32 g_reserved(carray_t<T> const& array)
    {
        return array.mCapacity;
    }

    template <typename T>
    inline bool g_is_empty(carray_t<T> const& array)
    {
        return array.mSize == 0;
    }

        template <typename T>
    inline bool g_is_empty(cview_t<T> const& array)
    {
        return array.mSize == 0;
    }

    template <typename T>
    inline bool g_is_full(carray_t<T> const& array)
    {
        ASSERT(array.mSize <= g_reserved());
        return array.mSize == array.mCapacity;
    }

    template <typename T>
    inline void g_push_back(carray_t<T>& array, T const& item)
    {
        ASSERT(array.mSize < g_reserved(array));
        nmem::memcpy(&array.mArray[array.mSize], &item, sizeof(T));
        array.mSize += 1;
    }

    template <typename T>
    inline bool g_pop_back(carray_t<T>& array, T& out_item)
    {
        if (array.mSize > 0)
        {
            --array.mSize;
            nmem::memcpy(&out_item, &array.mArray[array.mSize], sizeof(T));
            return true;
        }
        return false;
    }

    template <typename T>
    inline T* g_begin(carray_t<T>& array)
    {
        if (array.mSize == 0)
            return nullptr;
        return array.mArray;
    }

    template <typename T>
    inline T* g_next(carray_t<T>& array, T* current)
    {
        if (current >= &array.mArray[0] && current < &array.mArray[array.mSize - 1])
            return current + 1;
        return nullptr;
    }

    template <typename T>
    inline T* g_end(carray_t<T>& array)
    {
        return nullptr;
    }

    template <typename T>
    inline T const* g_begin(carray_t<T> const& array)
    {
        if (array.mSize == 0)
            return nullptr;
        return array.mArray;
    }

    template <typename T>
    inline T const* g_next(carray_t<T> const& array, T const* current)
    {
        if (current >= &array.mArray[0] && current < &array.mArray[array.mSize - 1])
            return current + 1;
        return nullptr;
    }

    template <typename T>
    inline T const* g_end(carray_t<T> const& array)
    {
        return nullptr;
    }

    template <typename T>
    inline T* g_get(carray_t<T>& array, s32 index)
    {
        ASSERT(index < (s32)array.mSize);
        return &array.mArray[index];
    }

    template <typename T>
    inline T const* g_get(carray_t<T> const& array, s32 index)
    {
        ASSERT(index < (s32)array.mSize);
        return &array.mArray[index];
    }

    template <typename T>
    inline T const* g_get(cview_t<T> const& array, s32 index)
    {
        ASSERT(index < (s32)array.mSize);
        return &array.mArray[index];
    }

    template <typename T>
    inline cview_t<T> g_view(carray_t<T>& array, s32 from, s32 to)
    {
        ASSERT(from < to && from < array.mCapacity && to < array.mCapacity);
        return cview_t<T> { to - from, to - from, &array.mArray[from] };
    }

    template <typename T>
    inline void g_swap(carray_t<T>& array, s32 index_a, s32 index_b)
    {
        ASSERT(index_a < array.mSize && index_b < array.mSize);
        nmem::memswap(&array.mArray[index_a], &array.mArray[index_b], sizeof(T));
    }

    template <typename T>
    inline void g_remove(carray_t<T>& array, s32 index)
    {
        if (index < array.mSize)
        {
            s32 const s = index + 1;
            for (s32 i = s; i < array.mSize; ++i)
                g_memcpy(&array.mArray[(i - 1)], &array.mArray[i], sizeof(T));
            array.mSize--;
        }
    }

    template <typename T>
    inline void g_swap_remove(carray_t<T>& array, s32 index)
    {
        if (array.mSize > 0)
        {
            array.mSize -= 1;
            if (index < array.mSize)
            {
                g_memcpy(&array.mArray[index], &array.mArray[array.mSize], sizeof(T));
            }
        }
    }
};  // namespace ncore

#endif   
