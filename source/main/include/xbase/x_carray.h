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
	template<class T>
	class xcarray
	{
	public:
		inline				xcarray() : mLen(0), mSize(0), mArray(0)		{ }
		inline				xcarray(T* _array, u32 _size) : mLen(0), mSize(_size), mArray(_array)	{ }

		inline void			clear()									{ mLen = 0; }

		inline u32			max() const								{ return mSize; }
		inline u32			size() const								{ return mLen; }
		inline u32			reserved() const							{ return mSize; }
		inline bool			empty() const							{ return mLen == 0; }
		inline bool			full() const								{ ASSERT(mLen<=mSize); return mLen == mSize; }

		inline void			push_back(T const& item)					{ ASSERT(mLen<mSize); mArray[mLen++] = item; }
		inline bool			pop_back(T & out_item)					{ if (mLen>0) { out_item = mArray[--mLen]; return true; } else return false; }

		inline char const*	begin() const							{ return mArray; }
		inline char const*	end() const								{ return &mArray[mLen]; }

		inline T&			operator [] (s32 index)					{ ASSERT(index<(s32)mLen); return mArray[index]; }
		inline T const&		operator [] (s32 index) const			{ ASSERT(index<(s32)mLen); return mArray[index]; }

		inline T&			operator * ()							{ ASSERT(mLen<mSize); return mArray[mLen]; }
		inline xcarray<T>	operator ++ (s32)						{ ASSERT(mLen<mSize); xcarray<T> c(&mArray[mLen], mSize-mLen); ++mLen; return c; }

		s32					index_of(T const& item) const;

		void					swap(u32 a, u32 b);
		void					remove(u32 i);
		void					swap_remove(u32 i);

	private:
		u32					mLen;
		u32					mSize;
		T*					mArray;
	};

	template<class T>
	inline s32			xcarray<T>::index_of(T const& item) const
	{
		s32 f = -1;
		for (u32 i=0; i<mLen; ++i)
		{
			if (mArray[i] == item)
			{
				f = i;
				break;
			}
		}
		return f;
	}

	template<class T>
	inline void			xcarray<T>::swap(u32 a, u32 b)
	{
		ASSERT (a < mLen && b < mLen);
		T temp = mArray[a];
		mArray[a] = mArray[b];
		mArray[b] = temp;
	}

	template<class T>
	inline void			xcarray<T>::remove(u32 index)
	{
		if (index < mLen)
		{
			s32 const s = index + 1;
			for (u32 i = s; i < mLen; ++i)
				mArray[i-1] = mArray[i];
			mLen -= 1;
		}
	}

	template<class T>
	inline void			xcarray<T>::swap_remove(u32 index)
	{
		if (mLen > 0)
		{
			mLen -= 1;
			if (index < mLen)
			{
				mArray[index] = mArray[mLen];
			}
		}
	}
};

//==============================================================================
// END
//==============================================================================
#endif // __XBASE_CARRAY_H__
