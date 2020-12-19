#ifndef __XBASE_SLICE_H__
#define __XBASE_SLICE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "xbase/x_allocator.h"
#include "xbase/x_debug.h"

namespace xcore
{
    struct slice_data_t;

    //==============================================================================
    // A reference counted slice_t owning a memory block with a view/window (from,to).
    //==============================================================================
    struct slice_t
    {
        slice_t();
        slice_t(alloc_t* allocator, s32 item_count, s32 item_size);
        slice_t(slice_data_t* data, s32 from, s32 to);

        static void alloc(slice_t& slice_t, alloc_t* allocator, s32 item_count, s32 item_size);
        slice_t     construct(s32 _item_count, s32 _item_size) const;
        s32         size() const;
        s32         refcnt() const;
        slice_t     obtain() const;
        void        release();
        void        resize(s32 count);
        void        insert(s32 count);
        void        remove(s32 count);
        slice_t     view(s32 from, s32 to) const;
        bool        split(s32 mid, slice_t& left, slice_t& right) const;

        void*       begin();
        void const* begin() const;
        void*       end();
        void const* end() const;
        void const* eos() const;
        void*       at(s32 index);
        void const* at(s32 index) const;

        slice_data_t* mData;
        s32           mFrom;
        s32           mTo;
    };

    // ----------------------------------------------------------------------------------------
    //   SLICE REFERENCE COUNTED DATA
    // ----------------------------------------------------------------------------------------

    struct slice_data_t
    {
        slice_data_t();
        slice_data_t(s32 item_count, s32 item_size);
        slice_data_t(xbyte* data, s32 item_count, s32 item_size);

        static slice_data_t sNull;

        slice_data_t* incref();
        slice_data_t* incref() const;
        slice_data_t* decref();

        // This function makes a new 'slice_data_t' with content copied from this
        slice_data_t* copy(s32 from, s32 to);

        // These functions do not 'reallocate' this
        void resize(s32 from, s32 to);
        void insert(s32 at, s32 count);
        void remove(s32 at, s32 count);

        static slice_data_t* alloc(alloc_t* allocator, s32& to_itemcount, s32& to_itemsize);

        mutable s32 mRefCount;
        s32         mItemCount; /// Count of total items
        s32         mItemSize;  /// Size of one item
        s32         mDummy;
        alloc_t*    mAllocator;
        xbyte*      mData;
    };
} // namespace xcore

#endif
