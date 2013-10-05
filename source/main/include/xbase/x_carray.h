#ifndef __XBASE_CARRAY_H__
#define __XBASE_CARRAY_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	template<class T, s32 N>
	class xcarray
	{
		enum
		{
			SIZE = N
		};
	public:
		inline			xcarray() : mSize(0)					{ }

		inline void		clear()									{ mSize = 0; }

		inline s32		size() const							{ return mSize; }
		inline s32		reserved() const						{ return SIZE; }
		inline bool		empty() const							{ return mSize == 0; }
		inline bool		full() const							{ return mSize == SIZE; }

		inline void		push_back(T const& item)				{ ASSERT(mSize<SIZE); mArray[mSize++] = item; }
		inline bool		pop_back(T & out_item)					{ if (mSize>0) { out_item = mArray[--mSize]; return true; } return false; }

		inline T&		operator [] (s32 index)					{ ASSERT(index<mSize); return mArray[index]; }
		inline T const&	operator [] (s32 index) const			{ ASSERT(index<mSize); return mArray[index]; }

		s32				index_of(T const& item) const;

		void			swap(s32 a, s32 b);
		void			remove(s32 i);
		void			swap_remove(s32 i);

	private:
		s32				mSize;
		T				mArray[SIZE];
	};

	template<class T, s32 N>
	inline s32			xcarray<T,N>::index_of(T const& item) const
	{
		s32 f = -1;
		for (s32 i=0; i<mSize; ++i)
		{
			if (mArray[i] == item)
			{
				f = i;
				break;
			}
		}
		return f;
	}

	template<class T, s32 N>
	inline void			xcarray<T,N>::swap(s32 a, s32 b)
	{
		ASSERT (a < mSize && b < mSize);
		T temp = mArray[a];
		mArray[a] = mArray[b];
		mArray[b] = temp;
	}

	template<class T, s32 N>
	inline void			xcarray<T,N>::remove(s32 index)
	{
		s32 const s = index + 1;
		for (s32 i = s; i < mSize; ++i)
			mArray[i-1] = mArray[i];
		mSize -= 1;
	}

	template<class T, s32 N>
	inline void			xcarray<T,N>::swap_remove(s32 index)
	{
		mSize -= 1;
		if (index < mSize)
		{
			mArray[index] = mArray[mSize];
		}
	}
};

//==============================================================================
// END
//==============================================================================
#endif // __XBASE_CARRAY_H__
