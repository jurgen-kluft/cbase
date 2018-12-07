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
		inline				xcarray(void* _array, s32 element_size, s32 array_size) : mLength(0), mArraySize(array_size), mElementSize(element_size), mArray((xbyte*)_array)	{ }

		inline void			clear()									{ mLength = 0; }

		inline s32			max() const								{ return mArraySize; }
		inline s32			size() const							{ return mLength; }
		inline s32			reserved() const						{ return mArraySize; }
		inline bool			is_empty() const						{ return mLength == 0; }
		inline bool			is_full() const							{ ASSERT(mLength <= mArraySize); return mLength == mArraySize; }

		inline void			push_back(void const* item)				{ ASSERT(mLength<mArraySize); x_memcpy(&mArray[mLength * mElementSize], item, mElementSize); mLength += 1; }
		inline bool			pop_back(void * out_item)				{ if (mLength > 0) { --mLength; x_memcpy(out_item, &mArray[mLength], mElementSize); return true; } else { return false; } }

		inline xbyte*		begin() const							{ return mArray; }
		inline xbyte*		next(xbyte* current) const				{ return current + mElementSize; }
		inline xbyte*		end() const								{ return &mArray[mLength * mElementSize]; }

		inline void*		operator [] (s32 index)					{ ASSERT(index<(s32)mLength); return &mArray[index * mElementSize]; }
		inline void const*	operator [] (s32 index) const			{ ASSERT(index<(s32)mLength); return &mArray[index * mElementSize]; }

		inline xcarray		operator() (s32 from, s32 to) const		{ ASSERT(from<to && from<mArraySize && to<mArraySize); xcarray c(&mArray[from* mElementSize], mElementSize, to-from);  return c; }

		void				swap(s32 a, s32 b);
		void				remove(s32 i);
		void				swap_remove(s32 i);

	private:
		s32					mLength;
		s32					mArraySize;
		s32					mElementSize;
		xbyte*				mArray;
	};

	inline void			xcarray::swap(s32 index_a, s32 index_b)
	{
		ASSERT (index_a < mLength && index_b < mLength);
		xbyte* srca = &mArray[index_a * mElementSize];
		xbyte* srcb = &mArray[index_b * mElementSize];
		for (s32 i = 0; i < mElementSize; ++i)
		{
			xbyte tmp = srca[i];
			srca[i] = srcb[i];
			srcb[i] = tmp;
		}
	}

	inline void			xcarray::remove(s32 index)
	{
		if (index < mLength)
		{
			s32 const s = index + 1;
			for (s32 i = s; i < mLength; ++i)
				x_memcpy(&mArray[(i-1) * mElementSize], &mArray[i * mElementSize], mElementSize);
			mLength--;
		}
	}

	inline void			xcarray::swap_remove(s32 index)
	{
		if (mLength > 0)
		{
			mLength -= 1;
			if (index < mLength)
			{
				x_memcpy(&mArray[index * mElementSize], &mArray[mLength * mElementSize], mElementSize);
			}
		}
	}
};

//==============================================================================
// END
//==============================================================================
#endif // __XBASE_CARRAY_H__
