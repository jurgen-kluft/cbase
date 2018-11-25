#include "xbase/x_target.h"
#include "xbase/x_slice.h"
#include "xbase/x_integer.h"

namespace xcore
{
	slice_data::slice_data()
	{
		mAllocator = NULL;
		mRefCount = 0;
		mItemCount = 0;
		mItemSize = 1;
		mData = NULL;
	}

	slice_data::slice_data(u32 item_count, u32 item_size)
	{
		mAllocator = NULL;
		mRefCount = 0;
		mItemCount = item_count;
		mItemSize = item_size;
		mData = NULL;
	}

	slice_data::slice_data(xbyte* data, u32 item_count, u32 item_size)
	{
		mAllocator = NULL;
		mRefCount = 0;
		mItemCount = item_count;
		mItemSize = item_size;
		mData = data;
	}

	slice_data	sNull;

	slice_data* 	slice_data::incref()
	{ 
		if (mAllocator!=nullptr)
			mRefCount++; 
		return (slice_data*)this; 
	}
	slice_data* 	slice_data::incref() const
	{ 
		if (mAllocator != nullptr)
			mRefCount++;
		return (slice_data*)this;
	}

	slice_data*		slice_data::decref()
	{
		if (mAllocator != nullptr)
			return this;

		s32 const refs = mRefCount;
		if (refs == 0)
			return &sNull;
		if (refs == 1)
		{
			mAllocator->deallocate(this);
			return &sNull;
		}

		mRefCount--;
		return this;
	}

	slice_data*		slice_data::alloc(xalloc* allocator, u32& to_itemcount, u32& to_itemsize)
	{
		slice_data* data = (slice_data*)allocator->allocate((to_itemcount * to_itemsize) + sizeof(slice_data), sizeof(void*));
		data->mRefCount = 1;
		data->mItemCount = to_itemcount;
		data->mItemSize = to_itemsize;
		data->mAllocator = allocator;
		data->mData = (xbyte*)data + sizeof(slice_data);
		return data;
	}

	slice_data*	slice_data::resize(s32 from, s32 to)
	{
		slice_data* data = &sNull;
		if (mAllocator!=nullptr)
		{
			u32 const to_itemsize = mItemSize;
			u32 const to_itemcount = to - from;
			data = (slice_data*)mAllocator->allocate((to_itemcount * to_itemsize) + sizeof(slice_data), sizeof(void*));
			data->mAllocator = mAllocator;
			data->mRefCount = 1;
			data->mItemSize = to_itemsize;
			data->mItemCount = to_itemcount;
			data->mData = (xbyte*)data + sizeof(slice_data);
			u32 const count2copy = xmin(to_itemcount, mItemCount);
			xmem_utils::memcpy(data->mData, this->mData + (from * mItemSize), count2copy * mItemSize);
			decref();
		}
		return data;
	}

	slice_data	slice_data::sNull;


	slice::slice()
	{
		mData = NULL;
		mFrom = 0;
		mTo = 0;
	}

	slice::slice(slice_data* blk, s32 from, s32 to)
	{
		mData = blk->incref();
		mFrom = from;
		mTo = to;
	}

	slice::slice(xalloc* allocator, u32 item_count, u32 item_size)
	{
		mData = slice_data::alloc(allocator, item_count, item_size);
		mFrom = 0;
		mTo = item_count;
	}

	void			slice::alloc(slice& slice, xalloc* allocator, u32 item_count, u32 item_size)
	{ 
		slice.mData = slice_data::alloc(allocator, item_count, item_size);
		slice.mFrom = 0; 
		slice.mTo = item_count; 
	}

	u32				slice::size() const
	{
		return mTo - mFrom;
	}

	u32				slice::refcnt() const
	{
		return mData->mRefCount;
	}

	void			slice::resize(u32 count)
	{ 
		mData = mData->resize(mFrom, mFrom + count); 
		mFrom = 0; 
		mTo = count; 
	}

	slice			slice::view(s32 from, s32 to) const
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

	bool			slice::split(s32 mid, slice& left, slice& right) const
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

	slice			slice::obtain() const
	{
		return slice(mData, mFrom, mTo);
	}

	void			slice::release()
	{
		if (mData != NULL)
		{
			mData->decref();
		}
		mData = NULL;
		mFrom = 0;
		mTo = 0;
	}

	slice			slice::construct(u32 _item_count, u32 _item_size) const
	{
		if (mData != NULL)
		{
			return slice(mData->mAllocator, _item_count, _item_size);
		}
		return slice();
	}

	void*			slice::get_at(s32 index)
	{
		if (mData == NULL)
			return NULL;
		index += mFrom;
		if (index < 0)
			return NULL;
		else if ((u32)index >= mTo)
			return NULL;
		u32 const data_offset = mData->mItemSize * index;
		return &mData->mData[data_offset];
	}

	void const*		slice::get_at(s32 index) const
	{
		if (mData == NULL)
			return NULL;
		index += mFrom;
		if (index < 0) 
			return NULL;
		else if ((u32)index >= mTo)
			return NULL;
		u32 const data_offset = mData->mItemSize * index;
		return &mData->mData[data_offset];
	}

	void *			slice::begin()
	{
		return get_at(mFrom);
	}

	void const *	slice::begin() const
	{
		return get_at(mFrom);
	}

	void *			slice::end()
	{
		return get_at(mTo);
	}

	void const *	slice::end() const
	{
		return get_at(mTo);
	}

	void const *	slice::eos() const
	{
		void const* eos = &mData->mData[mData->mItemCount * mData->mItemSize];
		return eos;
	}

}

