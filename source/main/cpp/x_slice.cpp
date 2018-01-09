#include "xbase/x_target.h"
#include "xbase/x_slice.h"

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
		if (mAllocator != NULL) 
			mRefCount++; 
		return (slice_data*)this; 
	}
	slice_data* 	slice_data::incref() const
	{ 
		if (mAllocator != NULL) 
			mRefCount++; 
		return (slice_data*)this;
	}

	slice_data*		slice_data::decref()
	{
		if (mAllocator == NULL)
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

	slice_data*		slice_data::alloc(x_iallocator* allocator, u32& to_itemcount, u32& to_itemsize)
	{
		to_itemsize = x_intu::align(to_itemsize, sizeof(void*));
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
		if (mAllocator != NULL)
		{
			u32 const to_itemsize = mItemSize;
			u32 const to_itemcount = to - from;
			data = (slice_data*)mAllocator->allocate((to_itemcount * to_itemsize) + sizeof(slice_data), sizeof(void*));
			data->mAllocator = mAllocator;
			data->mRefCount = 1;
			data->mItemSize = to_itemsize;
			data->mItemCount = to_itemcount;
			data->mData = (xbyte*)data + sizeof(slice_data);
			u32 const count2copy = x_intu::min(to_itemcount, mItemCount);
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

	slice::slice(slice const& other)
	{
		mData = other.mData->incref();
		mFrom = other.mFrom;
		mTo = other.mTo;
	}

	slice::slice(x_iallocator* allocator, u32 item_count, u32 item_size)
	{
		mData = slice_data::alloc(allocator, item_count, item_size);
		mFrom = 0;
		mTo = item_count;
	}

	slice::~slice()
	{
		mData->decref();
		mFrom = 0;
		mTo = 0;
	}

	void			slice::alloc(slice& slice, x_iallocator* allocator, u32 item_count, u32 item_size)
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

	slice			slice::view(u32 from, u32 to) const
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

	slice			slice::obtain() const
	{
		return slice(mData->incref(), mFrom, mTo);
	}

	void			slice::release() const
	{ 
		mData->decref(); 
	}

	void*			slice::get_at(s32 index)
	{
		if (mData == NULL)
			return NULL;
		if (index < 0) index = 0;
		else if ((u32)index >= mData->mItemCount)
			index = mData->mItemCount - 1;
		u32 const data_offset = mData->mItemSize * index;
		return &mData->mData[data_offset];
	}

	void const*		slice::get_at(s32 index) const
	{
		if (mData == NULL)
			return NULL;
		if (index < 0) index = 0;
		else if ((u32)index >= mData->mItemCount)
			index = mData->mItemCount - 1;
		u32 const data_offset = mData->mItemSize * index;
		return &mData->mData[data_offset];
	}

	// ----------------------------------------------------------------------------

	u64		map_key_hasher(xbyte const* data, u32 size)
	{
		// xxhash ?
		return 0;
	}

}

