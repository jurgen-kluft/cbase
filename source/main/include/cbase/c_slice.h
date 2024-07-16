#ifndef __CBASE_SLICE_H__
#define __CBASE_SLICE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"

namespace ncore
{
    //==============================================================================
    // A reference counted slice_t owning a memory block with a view/window (from,to).
    //==============================================================================
    struct slice_t
    {
        slice_t();
        ~slice_t();

        static void    allocate(slice_t& slice_t, alloc_t* allocator, s32 item_count, s32 item_size);
        static slice_t duplicate(slice_t const& slice);
        static bool    split(slice_t const& slice, s32 mid, slice_t& left, slice_t& right);
        static slice_t join(slice_t const& sliceA, slice_t const& sliceB);

        void release();

        s32 size() const;
        s32 from() const { return mFrom; }
        s32 to() const { return mTo; }

        slice_t view() const { return view(0, size()); }
        slice_t view(s32 to) const { return view(0, to); }
        slice_t view(s32 from, s32 to) const;

        void insert(slice_t const& other);
        void overwrite(slice_t const& other);
        void remove(slice_t const& other);

        void*       vbegin();
        void const* vbegin() const;
        void*       vend();
        void const* vend() const;

        bool vnext(void*& ptr) const;
        bool vnext(void const*& ptr) const;

        void*       vat(s32 index);
        void const* vat(s32 index) const;

        template <typename T>
        inline T* begin()
        {
            return (T*)vbegin();
        }
        template <typename T>
        T const* begin() const
        {
            return (T*)vbegin();
        }
        template <typename T>
        T* end()
        {
            return (T*)vend();
        }
        template <typename T>
        T const* end() const
        {
            return (T const*)vend();
        }

        template <typename T>
        void* at(s32 index)
        {
            return (T*)vat(index);
        }
        template <typename T>
        void const* at(s32 index) const
        {
            return (T const*)vat(index);
        }

        template <typename T>
        bool next(T*& ptr) const
        {
            void* p = ptr;
            bool  r = vnext(p);
            ptr     = (T*)p;
            return r;
        }

        template <typename T>
        bool next(T const*& ptr) const
        {
            void const* p = ptr;
            bool        r = vnext(p);
            ptr           = (T const*)p;
            return r;
        }

    protected:
        struct data_t;

        s32 refcnt() const;

        data_t* mData;
        s32     mFrom;
        s32     mTo;
    };

}  // namespace ncore

#endif
