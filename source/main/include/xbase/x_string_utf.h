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
		// UTF-8, UTF-16, UTF-32 conversions
		s32			convert		(uchar8  const * from, uchar32 * to);
		s32			convert		(uchar16 const * from, uchar32 * to);

		s32			convert		(uchar32 const * from, uchar8  * to);
		s32			convert		(uchar32 const * from, uchar16 * to);

		bool		iseos		(ustr8  const * str);
		bool		iseos		(ustr16 const * str);
		bool		iseos		(ustr32 const * str);

		bool		iscrln		(ustr8  const* ustr);
		bool		iscrln		(ustr16 const* ustr);
		bool		iscrln		(ustr32 const* ustr);
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

		xstring&		operator=(const char* ascii);
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
