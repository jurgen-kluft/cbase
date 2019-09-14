#include "xbase/x_target.h"
#include "xbase/x_slice.h"
#include "xbase/x_integer.h"

namespace xcore
{
	// ============================================================================
	// SLICE DATA
	// ============================================================================

	slice_data::slice_data()
	{
		mAllocator = NULL;
		mRefCount = 0;
		mItemCount = 0;
		mItemSize = 1;
		mData = NULL;
	}

	slice_data::slice_data(s32 item_count, s32 item_size)
	{
		mAllocator = NULL;
		mRefCount = 0;
		mItemCount = item_count;
		mItemSize = item_size;
		mData = NULL;
	}

	slice_data::slice_data(xbyte *data, s32 item_count, s32 item_size)
	{
		mAllocator = NULL;
		mRefCount = 0;
		mItemCount = item_count;
		mItemSize = item_size;
		mData = data;
	}

	slice_data *slice_data::incref()
	{
		if (mAllocator != nullptr)
			mRefCount++;
		return (slice_data *)this;
	}
	slice_data *slice_data::incref() const
	{
		if (mAllocator != nullptr)
			mRefCount++;
		return (slice_data *)this;
	}

	slice_data *slice_data::decref()
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

	slice_data *slice_data::copy(s32 from, s32 to)
	{
		slice_data * data = &sNull;
		if (mAllocator != nullptr)
		{
			data = (slice_data *)mAllocator->allocate(sizeof(slice_data), sizeof(void *));
			data->mAllocator = mAllocator;
			data->mRefCount = 1;
			data->mItemCount = to-from;
			data->mItemSize = mItemSize;
			data->mData = (xbyte*)mAllocator->allocate(data->mItemSize * data->mItemCount, sizeof(void *));
		}
		return data;
	}

	void slice_data::resize(s32 from, s32 to)
	{
		if (mAllocator != nullptr)
		{
			if (to > mItemCount)
			{
				s32 const to_itemcount = to;
				xbyte* data = (xbyte *)mAllocator->allocate((to_itemcount * mItemSize), sizeof(void *));
				xmem_utils::memcpy(data, mData, to_itemcount * mItemSize);
				mItemCount = to_itemcount;
			}
		}
	}

	void slice_data::insert(s32 at, s32 count)
	{
		if (mAllocator != nullptr)
		{
			s32 const to_itemcount = mItemCount + count;
			xbyte* data = (xbyte*)mAllocator->allocate((to_itemcount * mItemSize), sizeof(void *));
			s32 const head2copy = at * mItemSize;
			s32 const gap2skip = count * mItemSize;
			s32 const tail2copy = (mItemCount - at) * mItemSize;
			if (head2copy > 0)
			{
				xmem_utils::memcpy(data, this->mData, head2copy);
			}
			if (tail2copy > 0)
			{
				xmem_utils::memcpy(data + head2copy + gap2skip, this->mData + head2copy, tail2copy);
			}
			mItemCount += count;
			mAllocator->deallocate(this->mData);
			this->mData = data;
		}
	}

	void slice_data::remove(s32 at, s32 count)
	{
		if (mAllocator != nullptr)
		{
			s32 const to_itemsize = mItemSize;
			s32 const to_itemcount = mItemCount - count;
			xbyte* data = (xbyte*)mAllocator->allocate((to_itemcount * to_itemsize), sizeof(void *));
			s32 const head2copy = at * mItemSize;
			s32 const gap2close = count * mItemSize;
			s32 const tail2copy = (mItemCount - at) * mItemSize - gap2close;
			if (head2copy > 0)
			{
				xmem_utils::memcpy(data, this->mData, head2copy);
			}
			if (tail2copy > 0)
			{
				xmem_utils::memcpy(data + head2copy, this->mData + head2copy + gap2close, tail2copy);
			}
			mItemCount -= count;
			mAllocator->deallocate(this->mData);
			this->mData = data;
		}
	}

	slice_data *slice_data::alloc(xalloc *allocator, s32 &to_itemcount, s32 &to_itemsize)
	{
		slice_data *data = (slice_data *)allocator->allocate(sizeof(slice_data), sizeof(void *));
		data->mData = (xbyte *)allocator->allocate((to_itemcount * to_itemsize), sizeof(void *));
		data->mRefCount = 1;
		data->mItemCount = to_itemcount;
		data->mItemSize = to_itemsize;
		data->mAllocator = allocator;
		return data;
	}

	slice_data slice_data::sNull;

	// ============================================================================
	// SLICE
	// ============================================================================

	slice::slice()
	{
		mData = &slice_data::sNull;
		mFrom = 0;
		mTo = 0;
	}

	slice::slice(slice_data *blk, s32 from, s32 to)
	{
		mData = blk->incref();
		mFrom = from;
		mTo = to;
	}

	slice::slice(xalloc *allocator, s32 item_count, s32 item_size)
	{
		mData = slice_data::alloc(allocator, item_count, item_size);
		mFrom = 0;
		mTo = item_count;
	}

	void slice::alloc(slice &slice, xalloc *allocator, s32 item_count, s32 item_size)
	{
		slice.mData = slice_data::alloc(allocator, item_count, item_size);
		slice.mFrom = 0;
		slice.mTo = item_count;
	}

	slice slice::construct(s32 _item_count, s32 _item_size) const
	{
		return slice(mData->mAllocator, _item_count, _item_size);
	}

	s32 slice::size() const
	{
		return mTo - mFrom;
	}

	s32 slice::refcnt() const
	{
		return mData->mRefCount;
	}

	slice slice::obtain() const
	{
		return slice(mData, mFrom, mTo);
	}

	void slice::release()
	{
		mData->decref();
		mData = &slice_data::sNull;
		mFrom = 0;
		mTo = 0;
	}

	void slice::resize(s32 count)
	{
		mData->resize(mFrom, mFrom + count);
		mFrom = 0;
		mTo = count;
	}

	void slice::insert(s32 count)
	{
		mData->insert(mFrom, count);
		mTo += count;
	}

	void slice::remove(s32 count)
	{
		mData->remove(mFrom, count);
		mTo -= count;
	}

	slice slice::view(s32 from, s32 to) const
	{
		slice s;
		if ((mFrom + from) < mTo && (mFrom + to) <= mTo)
		{
			s.mData = mData->incref();
			s.mFrom = mFrom + from;
			s.mTo = mFrom + to;
			return s;
		}
		s.mData = &slice_data::sNull;
		return s;
	}

	bool slice::split(s32 mid, slice &left, slice &right) const
	{
		if ((mFrom + mid) <= mTo)
		{
			left.mData = mData->incref();
			left.mFrom = mFrom;
			left.mTo = mFrom + mid;

			right.mData = mData->incref();
			right.mFrom = mFrom + mid;
			right.mTo = mTo;

			return true;
		}
		return false;
	}

	void *slice::begin()
	{
		return at(mFrom);
	}

	void const *slice::begin() const
	{
		return at(mFrom);
	}

	void *slice::end()
	{
		return at(mTo);
	}

	void const *slice::end() const
	{
		return at(mTo);
	}

	void const *slice::eos() const
	{
		void const *eos = &mData->mData[mData->mItemCount * mData->mItemSize];
		return eos;
	}

	void *slice::at(s32 index)
	{
		if (mData == &slice_data::sNull)
			return nullptr;
		index += mFrom;
		if (index < 0)
			return nullptr;
		else if (index > mTo)
			return nullptr;
		u32 const data_offset = mData->mItemSize * index;
		return &mData->mData[data_offset];
	}

	void const *slice::at(s32 index) const
	{
		if (mData == &slice_data::sNull)
			return nullptr;
		index += mFrom;
		if (index < 0)
			return nullptr;
		else if (index > mTo)
			return nullptr;
		u32 const data_offset = mData->mItemSize * index;
		return &mData->mData[data_offset];
	}

}
