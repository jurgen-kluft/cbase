#ifndef __CBASE_CARRAY_H__
#define __CBASE_CARRAY_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "cbase/c_debug.h"
#include "cbase/c_memory.h"
#include "cbase/c_allocator.h"

namespace ncore
{
	class carray_t
	{
	public:
		inline				carray_t() : mLength(0), mReservedItems(0), mSizeOfItem(0), mArray(0)		{ }
		inline				carray_t(void* _array, s32 size_of_element, s32 number_of_elements) : mLength(0), mReservedItems(number_of_elements), mSizeOfItem(size_of_element), mArray((u8*)_array)	{ }

		inline void			clear()									{ mLength = 0; }

		inline s32			size() const							{ return mLength; }
		inline s32			reserved() const						{ return mReservedItems; }
		inline bool			is_empty() const						{ return mLength == 0; }
		inline bool			is_full() const							{ ASSERT(mLength <= reserved()); return mLength == mReservedItems; }

		inline void			push_back(void const* item)				{ ASSERT(mLength<reserved()); nmem::memcpy(&mArray[mLength * mSizeOfItem], item, mSizeOfItem); mLength += 1; }
		inline bool			pop_back(void * out_item)				{ if (mLength > 0) { --mLength; nmem::memcpy(out_item, &mArray[mLength * mSizeOfItem], mSizeOfItem); return true; } else { return false; } }

		inline u8*		begin() const							{ return mArray; }
		inline u8*		next(u8* current) const				{ ASSERT(g_ptr_in_range(mArray, mReservedItems * mSizeOfItem, current)); return current + (uint_t)mSizeOfItem; }
		inline u8*		end() const								{ return &mArray[mLength * mSizeOfItem]; }

		inline void*		operator [] (s32 index)					{ ASSERT(index<(s32)mLength); return &mArray[index * mSizeOfItem]; }
		inline void const*	operator [] (s32 index) const			{ ASSERT(index<(s32)mLength); return &mArray[index * mSizeOfItem]; }

		inline carray_t		operator() (s32 from, s32 to) const		{ ASSERT(from<to && from<mReservedItems && to<mReservedItems); carray_t c(&mArray[from* mSizeOfItem], mSizeOfItem, to-from);  return c; }

		void				swap(s32 a, s32 b);
		void				remove(s32 i);
		void				swap_remove(s32 i);

	private:
		s32					mLength;
		s32					mReservedItems;
		s32					mSizeOfItem;
		u8*				mArray;
	};

	inline void			carray_t::swap(s32 index_a, s32 index_b)
	{
		ASSERT (index_a < mLength && index_b < mLength);
		u8* srca = &mArray[index_a * mSizeOfItem];
		u8* srcb = &mArray[index_b * mSizeOfItem];
		for (s32 i = 0; i < mSizeOfItem; ++i)
		{
			u8 tmp = srca[i];
			srca[i] = srcb[i];
			srcb[i] = tmp;
		}
	}

	inline void			carray_t::remove(s32 index)
	{
		if (index < mLength)
		{
			s32 const s = index + 1;
			for (s32 i = s; i < mLength; ++i)
				g_memcpy(&mArray[(i-1) * mSizeOfItem], &mArray[i * mSizeOfItem], mSizeOfItem);
			mLength--;
		}
	}

	inline void			carray_t::swap_remove(s32 index)
	{
		if (mLength > 0)
		{
			mLength -= 1;
			if (index < mLength)
			{
				g_memcpy(&mArray[index * mSizeOfItem], &mArray[mLength * mSizeOfItem], mSizeOfItem);
			}
		}
	}
};

#endif // __CBASE_CARRAY_H__
