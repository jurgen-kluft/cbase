#ifndef __CBASE_SLICE_H__
#define __CBASE_SLICE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"

namespace ncore
{
    struct slice_data_t;

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

        void*       begin();
        void const* begin() const;
        void*       end();
        void const* end() const;
        bool        next(void*& ptr) const;
        bool        next(void const*& ptr) const;
        void*       at(s32 index);
        void const* at(s32 index) const;

    protected:
        friend struct slice_data_t;

        s32  refcnt() const;

        slice_data_t* mData;
        s32           mFrom;
        s32           mTo;
    };

} // namespace ncore

#endif
