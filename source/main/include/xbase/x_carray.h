#ifndef __XBASE_CARRAY_H__
#define __XBASE_CARRAY_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_debug.h"
#include "xbase/x_memory.h"
#include "xbase/x_allocator.h"

namespace xcore
{
	class xcarray
	{
	public:
		inline				xcarray() : mLength(0), mReservedItems(0), mSizeOfItem(0), mArray(0)		{ }
		inline				xcarray(void* _array, s32 size_of_element, s32 number_of_elements) : mLength(0), mReservedItems(number_of_elements), mSizeOfItem(size_of_element), mArray((xbyte*)_array)	{ }

		inline void			clear()									{ mLength = 0; }

		inline s32			size() const							{ return mLength; }
		inline s32			reserved() const						{ return mReservedItems; }
		inline bool			is_empty() const						{ return mLength == 0; }
		inline bool			is_full() const							{ ASSERT(mLength <= reserved()); return mLength == mReservedItems; }

		inline void			push_back(void const* item)				{ ASSERT(mLength<reserved()); x_memcpy(&mArray[mLength * mSizeOfItem], item, mSizeOfItem); mLength += 1; }
		inline bool			pop_back(void * out_item)				{ if (mLength > 0) { --mLength; x_memcpy(out_item, &mArray[mLength * mSizeOfItem], mSizeOfItem); return true; } else { return false; } }

		inline xbyte*		begin() const							{ return mArray; }
		inline xbyte*		next(xbyte* current) const				{ ASSERT(x_is_in_range(mArray, mReservedItems * mSizeOfItem, current)); return current + mSizeOfItem; }
		inline xbyte*		end() const								{ return &mArray[mLength * mSizeOfItem]; }

		inline void*		operator [] (s32 index)					{ ASSERT(index<(s32)mLength); return &mArray[index * mSizeOfItem]; }
		inline void const*	operator [] (s32 index) const			{ ASSERT(index<(s32)mLength); return &mArray[index * mSizeOfItem]; }

		inline xcarray		operator() (s32 from, s32 to) const		{ ASSERT(from<to && from<mReservedItems && to<mReservedItems); xcarray c(&mArray[from* mSizeOfItem], mSizeOfItem, to-from);  return c; }

		void				swap(s32 a, s32 b);
		void				remove(s32 i);
		void				swap_remove(s32 i);

	private:
		s32					mLength;
		s32					mReservedItems;
		s32					mSizeOfItem;
		xbyte*				mArray;
	};

	inline void			xcarray::swap(s32 index_a, s32 index_b)
	{
		ASSERT (index_a < mLength && index_b < mLength);
		xbyte* srca = &mArray[index_a * mSizeOfItem];
		xbyte* srcb = &mArray[index_b * mSizeOfItem];
		for (s32 i = 0; i < mSizeOfItem; ++i)
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
				x_memcpy(&mArray[(i-1) * mSizeOfItem], &mArray[i * mSizeOfItem], mSizeOfItem);
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
				x_memcpy(&mArray[index * mSizeOfItem], &mArray[mLength * mSizeOfItem], mSizeOfItem);
			}
		}
	}
};

#endif // __XBASE_CARRAY_H__
