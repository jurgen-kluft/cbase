#ifndef __XBASE_SLICE_H__
#define __XBASE_SLICE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"
#include "xbase\x_integer.h"
#include "xbase\x_memory_std.h"


/**
* xCore namespace
*/
namespace xcore
{
	class x_iallocator;

	struct slice
	{
		struct block
		{
			block()
			{
				mAllocator = NULL;
				mRefCount = 0;
				mSize = 0;
			}

			static block	sNull;

			block* 			incref()			{ if (mAllocator != NULL) mRefCount++; return (block*)this; }
			block* 			incref() const		{ block* data = (block*)this;  if (mAllocator!=NULL) data->mRefCount++; return data; }

			block*			decref()
			{
				s32 const refs = mRefCount;
				if (refs == 0 || mAllocator == NULL)
					return &sNull;
				if (refs == 1)
				{
					mAllocator->deallocate(this);
					return &sNull;
				}

				mRefCount--;
				return (block*)this;
			}

			static block*	alloc(x_iallocator* allocator, s32 tosize)
			{
				tosize = x_intu::align(tosize, 16);
				block* data = (block*)allocator->allocate(tosize + sizeof(block), 16);
				data->mAllocator = allocator;
				data->mRefCount = 1;
				data->mSize = tosize;
				return data;
			}

			block*			resize(s32 from, s32 to)
			{
				block* data = &sNull;
				if (mAllocator != NULL)
				{
					s32 tosize = x_intu::align(to - from, 16);
					data = (block*)mAllocator->allocate(tosize + sizeof(block), 16);
					data->mAllocator = mAllocator;
					data->mRefCount = 1;
					data->mSize = tosize;
					u32 const size2copy = x_intu::min(tosize, mSize);
					xmem_utils::memcpy(&data->mData[0], &this->mData[from], size2copy);
					decref();
				}
				return data;
			}

			s32				mRefCount;
			s32				mSize;									/// Number of allocated bytes
			x_iallocator*	mAllocator;
			xbyte			mData[32 - 4 - sizeof(void*) - 4];		/// Align the size of this struct by itself to 24
		};

		slice()
		{
			mData = NULL;
			mFrom = 0;
			mTo = 0;
		}

		slice(s32 from, s32 to)
		{
			mData = NULL;
			mFrom = from;
			mTo = to;
		}

		slice(slice const& other)
		{
			mData = other.mData->incref();
			mFrom = other.mFrom;
			mTo = other.mTo;
		}

		slice(x_iallocator* allocator, s32 from, s32 to)
		{
			mData = block::alloc(allocator, to - from);
			mFrom = from;
			mTo = to;
		}

		~slice()
		{
			mData->decref();
			mFrom = 0;
			mTo = 0;
		}

		static void			alloc(slice& slice, x_iallocator* allocator, s32 tosize) { slice.mData = block::alloc(allocator, tosize); slice.mFrom = 0; slice.mTo = 16; }
		xbyte* 				resize(s32 len)			{ mData = mData->resize(mFrom, mFrom + len); mFrom = 0; mTo = len; return &mData->mData[0]; }

		s32					len() const				{ return mTo - mFrom; }

		slice				view(u32 from, u32 to)
		{
			slice s;
			if ((mFrom + from) < mTo && (mFrom + to) <= mTo)
			{
				s.mData = mData->incref();
				s.mFrom = mFrom + from;
				s.mTo = mFrom + to;
				return s;
			}
			s.mData = &block::sNull;
			return s;
		}

		void				release()				{ mData = mData->decref(); }

		block*				mData;
		u32					mFrom;
		u32					mTo;
	};
}

#endif
