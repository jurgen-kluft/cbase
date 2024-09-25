#include "ccore/c_target.h"
#include "cbase/c_slice.h"
#include "cbase/c_integer.h"
#include "cbase/c_memory.h"

namespace ncore
{
    // ----------------------------------------------------------------------------------------
    // Reference counted slice data.
    // ----------------------------------------------------------------------------------------

    struct slice_t::data_t
    {
        data_t();

        static data_t sNull;

        void    incref();
        void    incref() const;
        data_t* decref();

        // This function makes a new 'data_t' with content copied from this
        data_t* copy(s32 from, s32 to, s32 capacity, bool memclear);

        // These functions do not 'reallocate' this
        void resize(s32 from, s32 to);
        void insert(s32 at, s32 count, void const* _data = nullptr);
        void remove(s32 at, s32 count);
        void overwrite(s32 from, s32 to, u8 const* _data, s32 count);

        bool clamp(s32& i, s32 from, s32 to)
        {
            if (i < from)
            {
                i = from;
                return true;
            }
            if (i >= to)
            {
                i = to;
                return true;
            }
            return false;
        }

        static data_t* allocate(alloc_t* allocator, s32 itemcount, s32 itemsize, s32 capacity, bool memclear);
        static void    deallocate(alloc_t* allocator, data_t* _data);

        mutable s32 mRefCount;
        s32         mItemCount;  // Count of total items
        s32         mItemSize;   // Size of one item
        s32         mCapacity;   // Total capacity of the data
        u8*         mData;
        alloc_t*    mAllocator;
    };

    // ============================================================================
    // SLICE DATA
    // ============================================================================
    static byte s_null_data[] = {0, 0, 0, 0, 0, 0, 0, 0};

    slice_t::data_t::data_t()
        : mRefCount(0)
        , mItemCount(0)
        , mItemSize(1)
        , mCapacity(0)
        , mData(s_null_data)
        , mAllocator(nullptr)
    {
    }

    void slice_t::data_t::incref() { mRefCount += (mAllocator != nullptr) ? 1 : 0; }
    void slice_t::data_t::incref() const { mRefCount += (mAllocator != nullptr) ? 1 : 0; }

    slice_t::data_t* slice_t::data_t::decref()
    {
        if (mAllocator == nullptr)
            return this;

        s32 const refs = mRefCount;
        if (refs == 0)
            return &sNull;
        if (refs == 1)
        {
            deallocate(this->mAllocator, this);
            return &sNull;
        }

        mRefCount--;
        return this;
    }

    slice_t::data_t* slice_t::data_t::copy(s32 from, s32 to, s32 capacity, bool memclear)
    {
        data_t* data = &sNull;
        if (mAllocator != nullptr)
        {
            math::clampRange(from, to, 0, mItemCount);

            if (capacity < (to - from))
                capacity = to - from;

            data             = (data_t*)mAllocator->allocate(sizeof(data_t), sizeof(void*));
            data->mAllocator = mAllocator;
            data->mRefCount  = 1;
            data->mItemCount = to - from;
            data->mItemSize  = mItemSize;
            data->mCapacity  = capacity;
            data->mData      = (u8*)mAllocator->allocate((u32)(data->mItemSize * data->mCapacity), sizeof(void*));
            nmem::memcpy(data->mData, mData + from * mItemSize, data->mItemSize * data->mItemCount);
            if (memclear)
            {
                nmem::memset(data->mData + data->mItemCount * data->mItemSize, 0, data->mItemSize * (data->mCapacity - data->mItemCount));
            }
        }
        return data;
    }

    void slice_t::data_t::resize(s32 from, s32 to)
    {
        if (mAllocator != nullptr)
        {
            if (to > mItemCount)
            {
                s32 const to_itemcount = to;
                u8*       data         = (u8*)mAllocator->allocate((u32)(to_itemcount * mItemSize), sizeof(void*));
                nmem::memcpy(data, mData, to_itemcount * mItemSize);
                mItemCount = to_itemcount;
            }
        }
    }

    void slice_t::data_t::insert(s32 at, s32 count, void const* _data)
    {
        // TODO check if the capacity is enough to handle the insert
        if (mAllocator != nullptr)
        {
            s32 const to_itemcount = mItemCount + count;
            u8*       data         = (u8*)mAllocator->allocate((u32)(to_itemcount * mItemSize), sizeof(void*));
            s32 const head2copy    = at * mItemSize;
            s32 const gap2skip     = count * mItemSize;
            s32 const tail2copy    = (mItemCount - at) * mItemSize;
            if (head2copy > 0)
            {
                nmem::memcpy(data, this->mData, head2copy);
            }
            if (_data != nullptr)
            {
                nmem::memcpy(data + head2copy, _data, count);
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

    void slice_t::data_t::remove(s32 at, s32 count)
    {
        // TODO should we always re-allocate ?
        if (mAllocator != nullptr)
        {
            s32 const to_itemsize  = mItemSize;
            s32 const to_itemcount = mItemCount - count;
            u8*       data         = (u8*)mAllocator->allocate((u32)(to_itemcount * to_itemsize), sizeof(void*));
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

    void slice_t::data_t::overwrite(s32 from, s32 to, u8 const* _data, s32 _count)
    {
        u8*       dst     = (u8*)mData + from;
        u8 const* dst_end = dst + to;
        u8 const* src     = _data;
        u8 const* src_end = _data + _count;
        while (dst < dst_end && src < src_end)
        {
            *dst++ = *src++;
        }
    }

    slice_t::data_t* slice_t::data_t::allocate(alloc_t* allocator, s32 itemcount, s32 itemsize, s32 capacity, bool memclear)
    {
        if (capacity < itemcount)
            capacity = itemcount;

        data_t* data     = (data_t*)allocator->allocate(sizeof(data_t), sizeof(void*));
        data->mData      = (u8*)allocator->allocate((u32)(capacity * itemsize), sizeof(void*));
        data->mRefCount  = 1;
        data->mItemCount = itemcount;
        data->mItemSize  = itemsize;
        data->mCapacity  = capacity;
        data->mAllocator = allocator;
        if (memclear)
        {
            nmem::memset(data->mData, 0, data->mItemSize * data->mCapacity);
        }
        return data;
    }

    void slice_t::data_t::deallocate(alloc_t* allocator, data_t* _data)
    {
        ASSERT(_data != nullptr);
        if (_data == &sNull)
            return;
        allocator->deallocate(_data->mData);
        allocator->deallocate(_data);
    }

    slice_t::data_t slice_t::data_t::sNull;

    // ============================================================================
    // SLICE
    // ============================================================================

    slice_t::slice_t()

        : mData(&slice_t::data_t::sNull)
        , mFrom(0)
        , mTo(0)
    {
    }

    slice_t::~slice_t() { release(); }

    void slice_t::allocate(slice_t& slice, alloc_t* allocator, s32 item_count, s32 item_size)
    {
        slice.mData = slice_t::data_t::allocate(allocator, item_count, item_size, item_count, true);
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

    s32  slice_t::refcnt() const { return mData->mRefCount; }
    void slice_t::release()
    {
        mData->decref();
        mData = &slice_t::data_t::sNull;
        mFrom = 0;
        mTo   = 0;
    }

    slice_t slice_t::slice(s32 from, s32 to) const
    {
        slice_t s;
        from += mFrom;
        to += mFrom;
        if (from > mTo)
        {
            from = mTo;
            to   = mTo;
        }
        if (to > from)
        {
            mData->incref();
            s.mData = mData;
            s.mFrom = from;
            s.mTo   = to;
            return s;
        }
        s.mData = &slice_t::data_t::sNull;
        return s;
    }

    void slice_t::insert(slice_t const& other)
    {
        if (mData->mItemSize == other.mData->mItemSize)
            mData->insert(mFrom, other.size(), other.mData->mData);
    }

    void slice_t::overwrite(slice_t const& _other)
    {
        if (mData->mItemSize == _other.mData->mItemSize)
            mData->overwrite(mFrom, mTo, _other.mData->mData + _other.mFrom, _other.size());
    }

    void slice_t::remove(slice_t const& other) {}

    bool slice_t::split(slice_t const& slice, s32 at, slice_t& left, slice_t& right)
    {
        if ((slice.mFrom + at) <= slice.mTo)
        {
            slice.mData->incref();
            left.mData = slice.mData;
            left.mFrom = slice.mFrom;
            left.mTo   = slice.mFrom + at;

            slice.mData->incref();
            right.mData = slice.mData;
            right.mFrom = slice.mFrom + at;
            right.mTo   = slice.mTo;
            return true;
        }
        return false;
    }

    slice_t slice_t::join(slice_t const& sliceA, slice_t const& sliceB)
    {
        if (sliceA.mData == sliceB.mData && sliceA.mTo == sliceB.mFrom)
        {
            slice_t slice;
            slice.mData = sliceA.mData;
            slice.mFrom = sliceA.mFrom;
            slice.mTo   = sliceB.mTo;
            return slice;
        }

        slice_t slice;
        slice.mData         = slice_t::data_t::allocate(sliceA.mData->mAllocator, sliceA.size() + sliceB.size(), sliceA.mData->mItemSize, sliceA.size() + sliceB.size(), false);
        slice.mFrom         = 0;
        slice.mTo           = sliceA.size() + sliceB.size();
        s32 const head2copy = sliceA.size() * sliceA.mData->mItemSize;
        if (head2copy > 0)
        {
            nmem::memcpy(slice.mData->mData, sliceA.mData->mData + sliceA.mFrom * sliceA.mData->mItemSize, head2copy);
        }
        s32 const tail2copy = sliceB.size() * sliceB.mData->mItemSize;
        if (tail2copy > 0)
        {
            nmem::memcpy(slice.mData->mData + head2copy, sliceB.mData->mData + sliceB.mFrom * sliceB.mData->mItemSize, tail2copy);
        }
        return slice;
    }

    bool slice_t::vnext(void*& ptr) const
    {
        ASSERT(ptr >= vbegin() && ptr <= vend());
        if (ptr < vend())
        {
            ptr = (u8*)ptr + mData->mItemSize;
            return true;
        }
        return false;
    }

    bool slice_t::vnext(void const*& ptr) const
    {
        ASSERT(ptr >= vbegin() && ptr <= vend());
        if (ptr < vend())
        {
            ptr = (u8 const*)ptr + mData->mItemSize;
            return true;
        }
        return false;
    }

    void* slice_t::vat(s32 index)
    {
        if (mData == &slice_t::data_t::sNull)
            return nullptr;
        index += mFrom;
        if (index < 0)
            return nullptr;
        else if (index > mTo)
            return nullptr;
        s32 const data_offset = mData->mItemSize * index;
        return &mData->mData[data_offset];
    }

    void const* slice_t::vat(s32 index) const
    {
        if (mData == &slice_t::data_t::sNull)
            return nullptr;
        index += mFrom;
        if (index < 0)
            return nullptr;
        else if (index > mTo)
            return nullptr;
        s32 const data_offset = mData->mItemSize * index;
        return &mData->mData[data_offset];
    }

}  // namespace ncore
