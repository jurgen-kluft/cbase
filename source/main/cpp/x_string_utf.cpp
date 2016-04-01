#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"
#include "xbase\x_integer.h"
#include "xbase\x_memory_std.h"
#include "xbase\x_string_ascii.h"
#include "xbase\x_string_utf.h"


namespace xcore
{
	xstring::xstring()
	{
	}

	xstring::xstring(xstring & copy)
	: mSlice(copy.mSlice.incref())
	{
	}

	xstring::xstring(x_iallocator* allocator)
	{
		mSlice = slice::alloc(allocator, 0);
	}

	s32				xstring::len() const
	{
		return mSlice.len();
	}

	xstring		xstring::view(s32 from, s32 to)
	{
		xstring s;
		s.mSlice = mSlice.view(from, to);
		return s;
	}

	uchar			xstring::operator[](s32 i) const
	{
		if ((mSlice.mFrom + i) < mSlice.mTo)
		{
			return mSlice.mData->mData[mSlice.mFrom + i];
		}
		return (uchar)0;
	}

	xstring&		xstring::operator=(xstring & other)
	{
		if (&other != this)
		{
			if (other.mSlice.mData == this->mSlice.mData)
			{
				mSlice.mFrom = other.mSlice.mFrom;
				mSlice.mTo = other.mSlice.mTo;
			}
			else
			{
				mSlice.mData->decref();
				mSlice.mData = other.mSlice.mData->incref();
				mSlice.mFrom = other.mSlice.mFrom;
				mSlice.mTo = other.mSlice.mTo;
			}
		}
		return *this;
	}

	void			xstring::writeto(xwriter* writer) const
	{
		if (len() > 0)
		{
			writer->write((xbyte const*)&mSlice.mData->mData[mSlice.mFrom], mSlice.len());
		}
	}

	xstring			xstring::create(s32 len) const
	{
		xstring s;
		s.mSlice.mData = mSlice.mData->resize(mSlice.len());
		s.mSlice.mFrom = 0;
		s.mSlice.mTo = len;
		return s;
	}

	xstring			xstring::copy() const
	{
		xstring s;
		s.mSlice.mData = mSlice.mData->resize(mSlice.len());
		s.mSlice.mFrom = 0;
		s.mSlice.mTo = mSlice.len();
		return s;
	}



	static void xstring_usecase()
	{
		xstring strings(gCreateSystemAllocator());
	
		xstring string = strings.create(16);
		xstring view_4_8 = string(4, 8);

		uchar c = view_4_8[0];
		view_4_8 = xstring::cursor{ 0, '?' };	// Write a character at first position
	}
};

/**
 *  END xCore namespace
 */
