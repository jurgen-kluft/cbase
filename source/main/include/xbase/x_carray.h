#ifndef __XBASE_CARRAY_H__
#define __XBASE_CARRAY_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_memory.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class xcarray
	{
	public:
		inline				xcarray() : mLength(0), mArraySize(0), mElementSize(0), mArray(0)		{ }
		inline				xcarray(void* _array, u32 element_size, u32 array_size) : mLength(0), mArraySize(array_size), mElementSize(element_size), mArray((xbyte*)_array)	{ }

		inline void			clear()									{ mLength = 0; }

		inline u32			max() const								{ return mArraySize; }
		inline u32			size() const							{ return mLength; }
		inline u32			reserved() const						{ return mArraySize; }
		inline bool			is_empty() const						{ return mLength == 0; }
		inline bool			is_full() const							{ ASSERT(mLength <= mArraySize); return mLength == mArraySize; }

		inline void			push_back(void const* item)				{ ASSERT(mLength<mArraySize); x_memcpy(&mArray[mLength++], item, mElementSize); }
		inline bool			pop_back(void * out_item)				{ if (mLength > 0) { --mLength; x_memcpy(out_item, &mArray[mLength], mElementSize); return true; } else { return false; } }

		inline xbyte*		begin() const							{ return mArray; }
		inline xbyte*		next(xbyte* current) const				{ return current + mElementSize; }
		inline xbyte*		end() const								{ return &mArray[mLength]; }

		inline void*		operator [] (s32 index)					{ ASSERT(index<(s32)mLength); return &mArray[index]; }
		inline void const*	operator [] (s32 index) const			{ ASSERT(index<(s32)mLength); return &mArray[index]; }

		inline xcarray		operator() (s32 from, s32 to) const		{ ASSERT(from<mArraySize && to<mArraySize); xcarray c(&mArray[from], mElementSize, to-from);  return c; }

		void				swap(u32 a, u32 b);
		void				remove(u32 i);
		void				swap_remove(u32 i);

	private:
		u32					mLength;
		u32					mArraySize;
		u32					mElementSize;
		xbyte*				mArray;
	};

	inline void			xcarray::swap(u32 index_a, u32 index_b)
	{
		ASSERT (index_a < mLength && index_b < mLength);
		xbyte* srca = &mArray[index_a];
		xbyte* srcb = &mArray[index_b];
		for (s32 i = 0; i < mElementSize; ++i)
		{
			xbyte tmp = srca[i];
			srca[i] = srcb[i];
			srcb[i] = tmp;
		}
	}

	inline void			xcarray::remove(u32 index)
	{
		if (index < mLength)
		{
			s32 const s = index + 1;
			for (u32 i = s; i < mLength; ++i)
				x_memcpy(&mArray[i-1], &mArray[i], mElementSize);
			mLength--;
		}
	}

	inline void			xcarray::swap_remove(u32 index)
	{
		if (mLength > 0)
		{
			mLength -= 1;
			if (index < mLength)
			{
				x_memcpy(&mArray[index], &mArray[mLength], mElementSize);
			}
		}
	}
};

//==============================================================================
// END
//==============================================================================
#endif // __XBASE_CARRAY_H__
