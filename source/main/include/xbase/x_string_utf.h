#ifndef __XBASE_STRING_UTF_H__
#define __XBASE_STRING_UTF_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_integer.h"
#include "xbase\x_slice.h"

/**
* xCore namespace
*/
namespace xcore
{
	class x_iallocator;

	// UTF string manipulation is using UTF-32 since it is the only UTF format that has a fixed rune size
	// and thus is easier to manipulate directly than UTF-8 or UTF-16
	namespace UTF
	{
		// From UTF-8 or UTF-16 to UTF-32
		s32			convert		(ustr8  const* _src, ustr8  const* _src_end, ustr32*& _dst, u32 _dstmaxleninbytes);
		s32			convert		(ustr16 const* _src, ustr16 const* _src_end, ustr32*& _dst, u32 _dstmaxleninbytes);

		// From UTF-32 to UTF-8 or UTF-16
		s32			convert		(ustr32 const* _src, u32 _srclen, ustr8 *& _dst, u32 _dstmaxleninbytes);
		s32			convert		(ustr32 const* _src, u32 _srclen, ustr16*& _dst, u32 _dstmaxleninbytes);

	}	// UTF

	// String class, acts as a slice/view with a behaviour of explicit copy (not copy-on-write)
	class xstring
	{
	public:
						xstring();
						xstring(x_iallocator*);
						xstring(xstring& copy);


		s32				len() const;

		// Return a slice/view of this string
		xstring			operator()(s32 from, s32 to)							{ return view(from, to); }
		uchar			operator[](s32) const;

		struct cursor { s32 p; uchar c; };

		xstring&		operator=(xstring& other);
		xstring&		operator=(cursor c);

		void 			writeto(xwriter*) const;

		xstring			create(s32 len) const;
		xstring			copy() const;
		
		s32				explode(int n, xstring*& parts) const;

		s32				find(uchar c) const;
		s32				find(xstring const& c) const;
		s32				findr(uchar c) const;
		s32				findr(xstring const& c) const;

		s32				findi(uchar c) const;
		s32				findi(xstring const& c) const;
		s32				findir(uchar c) const;
		s32				findir(xstring const& c) const;

		struct data;
		explicit		xstring(data*);

	protected:

		xstring			view(s32 from, s32 to);

		slice			mSlice;
	};

};	// xCore namespace


#endif    ///< __XBASE_STRING_UTF_H__
