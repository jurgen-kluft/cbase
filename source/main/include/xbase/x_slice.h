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

			block* 			incref()			{ if (mAllocator != NULL && (this != &sNull)) mRefCount++; return (block*)this; }
			block* 			incref() const		{ block* data = (block*)this;  if (this != &sNull) data->mRefCount++; return data; }

			block*			decref()
			{
				s32 const refs = mRefCount;
				if (refs == 0 || this == &sNull)
					return &sNull;
				if (refs == 1)
				{
					mAllocator->deallocate(this);
					return &sNull;
				}

				mRefCount -= 1;
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

			block*			resize(u32 tosize)
			{
				block* data = &sNull;
				if (mAllocator != NULL)
				{
					tosize = x_intu::align(tosize, 16);
					data = (block*)mAllocator->allocate(tosize + sizeof(block), 16);
					data->mAllocator = mAllocator;
					data->mRefCount = 1;
					data->mSize = tosize;
					u32 const size2copy = x_intu::min(tosize, mSize);
					xmem_utils::memcpy(&data->mData[0], &this->mData[0], size2copy);
				}
				return data;
			}

			s32				mRefCount;
			x_iallocator*	mAllocator;
			u32				mSize;									/// Number of allocated bytes
			xbyte			mData[32 - 4 - sizeof(void*) - 4];		/// Align the size of this struct by itself to 24
		};

		static slice		alloc(x_iallocator* allocator, s32 tosize) { slice s; s.mData = block::alloc(allocator, tosize); s.mFrom = 0; s.mTo = 16; return s; }

		s32					len() const { return mTo - mFrom; }
		slice				incref()				{ slice s(*this); mData->incref(); return s; }
		slice				view(u32 from, u32 to)
		{
			slice s;
			s.mFrom = 0;
			if ((mFrom + from) < mTo && (mFrom + to) < mTo)
			{
				s.mData = mData->resize(to - from);
				s.mTo = to - from;
				return s;
			}
			s.mData = &block::sNull;
			s.mTo = 0;
			return s;
		}

		block*				mData;
		u32					mFrom;
		u32					mTo;
	};


}

#endif