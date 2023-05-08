#include "ccore/c_target.h"
#include "cbase/c_slice.h"
#include "cbase/c_integer.h"

namespace ncore
{
    // ----------------------------------------------------------------------------------------
    // Reference counted slice data.
    // ----------------------------------------------------------------------------------------

    struct slice_data_t
    {
        slice_data_t();

        static slice_data_t sNull;

        slice_data_t* incref();
        slice_data_t* incref() const;
        slice_data_t* decref();

        // This function makes a new 'slice_data_t' with content copied from this
        slice_data_t* copy(s32 from, s32 to, s32 capacity, bool memclear);

        // These functions do not 'reallocate' this
        void resize(s32 from, s32 to);
        void insert(s32 at, s32 count);
        void remove(s32 at, s32 count);

        static slice_data_t* allocate(alloc_t* allocator, s32 itemcount, s32 itemsize, s32 capacity, bool memclear);

        mutable s32 mRefCount;
        s32         mItemCount; // Count of total items
        s32         mItemSize;  // Size of one item
        s32         mCapacity;  // Total capacity of the data
        u8*         mData;
        alloc_t*    mAllocator;
    };

    // ============================================================================
    // SLICE DATA
    // ============================================================================

    slice_data_t::slice_data_t()
    {
        mAllocator = nullptr;
        mRefCount  = 0;
        mItemCount = 0;
        mItemSize  = 1;
        mData      = nullptr;
    }

    slice_data_t* slice_data_t::incref()
    {
        if (mAllocator != nullptr)
            mRefCount++;
        return (slice_data_t*)this;
    }
    slice_data_t* slice_data_t::incref() const
    {
        if (mAllocator != nullptr)
            mRefCount++;
        return (slice_data_t*)this;
    }

    slice_data_t* slice_data_t::decref()
    {
        if (mAllocator == nullptr)
            return this;

        s32 const refs = mRefCount;
        if (refs == 0)
            return &sNull;
        if (refs == 1)
        {
            mAllocator->deallocate(this->mData);
            mAllocator->deallocate(this);
            return &sNull;
        }

        mRefCount--;
        return this;
    }

    slice_data_t* slice_data_t::copy(s32 from, s32 to, s32 capacity, bool memclear)
    {
        slice_data_t* data = &sNull;
        if (mAllocator != nullptr)
        {
            math::clampRange(from, to, 0, mItemCount);

            if (capacity < (to - from))
                capacity = to - from;

            data             = (slice_data_t*)mAllocator->allocate(sizeof(slice_data_t), sizeof(void*));
            data->mAllocator = mAllocator;
            data->mRefCount  = 1;
            data->mItemCount = to - from;
            data->mItemSize  = mItemSize;
            data->mCapacity  = capacity;
            data->mData      = (u8*)mAllocator->allocate(data->mItemSize * data->mCapacity, sizeof(void*));
            nmem::memcpy(data->mData, mData + from * mItemSize, data->mItemSize * data->mItemCount);
            if (memclear)
            {
                nmem::memset(data->mData + data->mItemCount * data->mItemSize, 0, data->mItemSize * (data->mCapacity - data->mItemCount));
            }
        }
        return data;
    }

    void slice_data_t::resize(s32 from, s32 to)
    {
        if (mAllocator != nullptr)
        {
            if (to > mItemCount)
            {
                s32 const to_itemcount = to;
                u8*       data         = (u8*)mAllocator->allocate((to_itemcount * mItemSize), sizeof(void*));
                nmem::memcpy(data, mData, to_itemcount * mItemSize);
                mItemCount = to_itemcount;
            }
        }
    }

    void slice_data_t::insert(s32 at, s32 count)
    {
        if (mAllocator != nullptr)
        {
            s32 const to_itemcount = mItemCount + count;
            u8*       data         = (u8*)mAllocator->allocate((to_itemcount * mItemSize), sizeof(void*));
            s32 const head2copy    = at * mItemSize;
            s32 const gap2skip     = count * mItemSize;
            s32 const tail2copy    = (mItemCount - at) * mItemSize;
            if (head2copy > 0)
            {
                nmem::memcpy(data, this->mData, head2copy);
            }
            if (tail2copy > 0)
            {
                nmem::memcpy(data + head2copy + gap2skip, this->mData + head2copy, tail2copy);
            }
            mItemCount += count;
            mAllocator->deallocate(this->mData);
            this->mData = data;
        }
    }

    void slice_data_t::remove(s32 at, s32 count)
    {
        if (mAllocator != nullptr)
        {
            s32 const to_itemsize  = mItemSize;
            s32 const to_itemcount = mItemCount - count;
            u8*       data         = (u8*)mAllocator->allocate((to_itemcount * to_itemsize), sizeof(void*));
            s32 const head2copy    = at * mItemSize;
            s32 const gap2close    = count * mItemSize;
            s32 const tail2copy    = (mItemCount - at) * mItemSize - gap2close;
            if (head2copy > 0)
            {
                nmem::memcpy(data, this->mData, head2copy);
            }
            if (tail2copy > 0)
            {
                nmem::memcpy(data + head2copy, this->mData + head2copy + gap2close, tail2copy);
            }
            mItemCount -= count;
            mAllocator->deallocate(this->mData);
            this->mData = data;
        }
    }

    slice_data_t* slice_data_t::allocate(alloc_t* allocator, s32 itemcount, s32 itemsize, s32 capacity, bool memclear)
    {
        if (capacity < itemcount)
            capacity = itemcount;

        slice_data_t* data = (slice_data_t*)allocator->allocate(sizeof(slice_data_t), sizeof(void*));
        data->mData        = (u8*)allocator->allocate((capacity * itemsize), sizeof(void*));
        data->mRefCount    = 1;
        data->mItemCount   = itemcount;
        data->mItemSize    = itemsize;
        data->mCapacity    = capacity;
        data->mAllocator   = allocator;
        if (memclear)
        {
            nmem::memset(data->mData, 0, data->mItemSize * data->mCapacity);
        }
        return data;
    }

    slice_data_t slice_data_t::sNull;

    // ============================================================================
    // SLICE
    // ============================================================================

    slice_t::slice_t()
    {
        mData = &slice_data_t::sNull;
        mFrom = 0;
        mTo   = 0;
    }
    slice_t::~slice_t() 
    {
        if (mData != &slice_data_t::sNull)
        {
            mData->decref();
        }
        mData = nullptr;
        mFrom = 0;
        mTo   = 0;
    }

    void slice_t::allocate(slice_t& slice, alloc_t* allocator, s32 item_count, s32 item_size)
    {
        slice.mData = slice_data_t::allocate(allocator, item_count, item_size, item_count, true);
        slice.mFrom = 0;
        slice.mTo   = item_count;
    }

    slice_t slice_t::duplicate(slice_t const& slice)
    {
        slice_t newslice;
        newslice.mFrom = 0;
        newslice.mTo   = slice.size();
        newslice.mData = slice.mData->copy(slice.mFrom, slice.mTo, slice.mTo - slice.mFrom, false);
        return newslice;
    }

    s32  slice_t::size() const { return mTo - mFrom; }
    s32  slice_t::refcnt() const { return mData->mRefCount; }
    void slice_t::release()
    {
        mData->decref();
        mData = &slice_data_t::sNull;
        mFrom = 0;
        mTo   = 0;
    }

    slice_t slice_t::view(s32 from, s32 to) const
    {
        slice_t s;
        math::clampRange(from, to, 0, (mTo - mFrom));
        if (to > from)
        {
            s.mData = mData->incref();
            s.mFrom = mFrom + from;
            s.mTo   = mFrom + to;
            return s;
        }
        s.mData = &slice_data_t::sNull;
        return s;
    }

    bool slice_t::split(slice_t const& slice, s32 mid, slice_t& left, slice_t& right)
    {
        if ((slice.mFrom + mid) <= slice.mTo)
        {
            left.mData = slice.mData->incref();
            left.mFrom = slice.mFrom;
            left.mTo   = slice.mFrom + mid;

            right.mData = slice.mData->incref();
            right.mFrom = slice.mFrom + mid;
            right.mTo   = slice.mTo;

            return true;
        }
        return false;
    }

    slice_t slice_t::join(slice_t const& sliceA, slice_t const& sliceB)
    {
        slice_t slice;
        slice.mData         = slice_data_t::allocate(sliceA.mData->mAllocator, sliceA.size() + sliceB.size(), sliceA.mData->mItemSize, sliceA.size() + sliceB.size(), false);
        slice.mFrom         = 0;
        slice.mTo           = slice.size();
        s32 const head2copy = sliceA.size() * sliceA.mData->mItemSize;
        s32 const tail2copy = sliceB.size() * sliceB.mData->mItemSize;
        if (head2copy > 0)
        {
            nmem::memcpy(slice.mData->mData, sliceA.mData->mData + sliceA.mFrom * sliceA.mData->mItemSize, head2copy);
        }
        if (tail2copy > 0)
        {
            nmem::memcpy(slice.mData->mData + head2copy, sliceB.mData->mData + sliceB.mFrom * sliceB.mData->mItemSize, tail2copy);
        }
        return slice;
    }

    void*       slice_t::begin() { return at(mFrom); }
    void const* slice_t::begin() const { return at(mFrom); }
    void*       slice_t::end() { return at(mTo); }
    void const* slice_t::end() const { return at(mTo); }
    bool        slice_t::next(void*& ptr) const
    {
        ASSERT(ptr >= begin() && ptr <= end());
        if (ptr < end())
        {
            ptr = (u8*)ptr + mData->mItemSize;
            return true;
        }
        return false;
    }

    bool slice_t::next(void const*& ptr) const
    {
        ASSERT(ptr >= begin() && ptr <= end());
        if (ptr < end())
        {
            ptr = (u8 const*)ptr + mData->mItemSize;
            return true;
        }
        return false;
    }

    void* slice_t::at(s32 index)
    {
        if (mData == &slice_data_t::sNull)
            return nullptr;
        index += mFrom;
        if (index < 0)
            return nullptr;
        else if (index > mTo)
            return nullptr;
        u32 const data_offset = mData->mItemSize * index;
        return &mData->mData[data_offset];
    }

    void const* slice_t::at(s32 index) const
    {
        if (mData == &slice_data_t::sNull)
            return nullptr;
        index += mFrom;
        if (index < 0)
            return nullptr;
        else if (index > mTo)
            return nullptr;
        u32 const data_offset = mData->mItemSize * index;
        return &mData->mData[data_offset];
    }

} // namespace ncore
