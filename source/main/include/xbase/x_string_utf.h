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
		s32			uchar32to8	(uchar32 rune, ustr8  * dest);
		s32			uchar32to16	(uchar32 rune, ustr16 * dest);

		s32			convert		(uchar8  const * from, uchar32 * to);
		s32			convert		(uchar16 const * from, uchar32 * to);

		s32			convert		(uchar32 const * from, uchar8  * to);
		s32			convert		(uchar32 const * from, uchar16 * to);

		s32			readu8		(ustr8 const* str, uchar32& out_char);
		s32			readu16		(ustr16 const * str, uchar32& out_char);

		s32			len			(ustr8 const* str);

		bool		iseos		(ustr8  const * str);
		bool		iseos		(ustr16 const * str);
		bool		iseos		(ustr32 const * str);

		bool		iscrln		(ustr8  const* ustr);
		bool		iscrln		(ustr16 const* ustr);
		bool		iscrln		(ustr32 const* ustr);
	}	// UTF

	typedef uchar(*transform_functor)(s32 i, uchar32* begin, uchar32* end);

	// String class, acts as a slice/view with a behaviour of explicit copy (not copy-on-write)
	class xstring
	{
	public:
						xstring();
						xstring(x_iallocator*);
						xstring(xstring& copy);
						~xstring();

		inline s32		len() const												{ return mSlice.len() / sizeof(uchar32); }

		// Return a copy of this string
		xstring			copy() const;

		// Return a slice/view of this string
		xstring			operator()(s32 from, s32 to)							{ return view(from, to); }

		uchar32	*		operator[](s32);
		uchar32 const *	operator[](s32) const;

		xstring&		operator=(uchar32);
		xstring&		operator=(char const*);
		xstring&		operator=(ustr8 const*);
		xstring&		operator=(ustr16 const*);
		xstring&		operator=(ustr32 const*);
		xstring&		operator=(xstring &);

		xstring			operator+ (char const* other);
		xstring&		operator+=(char const* other);
		xstring			operator+ (ustr8 const* other);
		xstring&		operator+=(ustr8 const* other);
		xstring			operator+ (xstring const& other);
		xstring&		operator+=(xstring const& other);

		void 			writeto(xwriter*) const;

		s32				explode(int n, xstring*& parts) const;

		enum eflags
		{
			CASE   = 0,
			NOCASE = 1,
			SELECT = 2,
			LEFT   = 0,
			RIGHT  = 4,
		};

		xstring			find (uchar c, eflags flags = NOCASE) const;
		xstring			find (char const* str, eflags flags = NOCASE) const;
		xstring			find (ustr8 const* str, eflags flags = NOCASE) const;
		xstring			find (xstring const& c, eflags flags = NOCASE) const;

		xstring			findr(uchar c, eflags flags = NOCASE) const;
		xstring			findr(char const* str, eflags flags = NOCASE) const;
		xstring			findr(ustr8 const* str, eflags flags = NOCASE) const;
		xstring			findr(xstring const& c, eflags flags = NOCASE) const;

		xstring			left_of(xstring const&) const;
		xstring			right_of(xstring const&) const;

		xstring&		tolower();
		xstring&		toupper();
		xstring&		tocamel();

		struct data;
		explicit		xstring(data*);

	protected:
		xstring			view(s32 from, s32 to);

		uchar32*		str_ptr();
		uchar32 const*	str_ptr() const;

		slice			mSlice;
	};

};	// xCore namespace


#endif    ///< __XBASE_STRING_UTF_H__
