#ifndef __XBASE_STRING_UTF_H__
#define __XBASE_STRING_UTF_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_slice.h"

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
		// ASCII, UTF-8, UTF-16, UTF-32 conversions
		s32			utf32_to_ascii	(uchar32 rune, char   * dest);
		s32			utf32_to_utf8	(uchar32 rune, ustr8  * dest);
		s32			utf32_to_utf16	(uchar32 rune, ustr16 * dest);

		s32			ascii_to_utf32	(char   const * src, uchar32& rune);
		s32			utf8_to_utf32	(ustr8  const * src, uchar32& rune);
		s32			utf16_to_utf32	(ustr16 const * src, uchar32& rune);

		s32			convert			(char const * from, uchar32 * to);
		s32			convert			(ustr8  const * from, uchar32 * to);
		s32			convert			(ustr16 const * from, uchar32 * to);

		s32			convert			(uchar32 const * from, ustr8  * to);
		s32			convert			(uchar32 const * from, ustr16 * to);

		s32			read_ascii		(char const* str, uchar32& out_char);
		s32			read_utf8		(ustr8 const* str, uchar32& out_char);
		s32			read_utf16		(ustr16 const * str, uchar32& out_char);
		s32			read_utf32		(ustr32 const * str, uchar32& out_char);

		s32			len				(ustr8 const* str);
		s32			len				(ustr32 const* str);

		bool		iseos			(ustr8  const * str);
		bool		iseos			(ustr16 const * str);
		bool		iseos			(ustr32 const * str);

		bool		iscrln			(ustr8  const* ustr);
		bool		iscrln			(ustr16 const* ustr);
		bool		iscrln			(ustr32 const* ustr);
	}	// UTF

	extern bool  			StrToBool(const uchar32 * s);
	extern bool  			StrToBool(const uchar32 * s, const uchar32 ** scan_end);
	extern f32  			StrToF32(const uchar32 * s);
	extern f32  			StrToF32(const uchar32 * s, const uchar32 ** scan_end);
	extern f64  			StrToF64(const uchar32 * s);
	extern f64  			StrToF64(const uchar32 * s, const uchar32 ** scan_end);
	extern s32				StrToS32(const uchar32 * str);
	extern s32				StrToS32(const uchar32 * str, s32 base);
	extern s32				StrToS32(const uchar32 * str, const uchar32 ** scanEnd, s32 base);
	extern s64				StrToS64(const uchar32 * str);
	extern s64				StrToS64(const uchar32 * str, s32 base);
	extern s64				StrToS64(const uchar32 * str, s32 base, const uchar32 ** scanEnd);

	// String class, acts as a slice/view with a behaviour of explicit copy (not copy-on-write)
	class xstring
	{
	public:
		xstring();
		xstring(x_iallocator* allocator);
		xstring(x_iallocator* allocator, u32 initial_len);
		xstring(xstring const& rhs);
		~xstring();

		inline s32		len() const { return mSlice.len() / sizeof(uchar32); }

		class operation
		{
		public:
			operation(uchar32 c) : mChar(c), mType(TYPE_UCHAR32), mMode(MODE_OVERWRITE) {}
			operation(char const* str) : mPCCharStr(str), mType(TYPE_PCCHAR), mMode(MODE_OVERWRITE) {}
			operation(ustr8 const* str) : mPCUStr8Str(str), mType(TYPE_PCUSTR8), mMode(MODE_OVERWRITE) {}
			operation(ustr16 const* str) : mPCUStr16Str(str), mType(TYPE_PCUSTR16), mMode(MODE_OVERWRITE) {}
			operation(ustr32 const* str) : mPCUStr32Str(str), mType(TYPE_PCUSTR32), mMode(MODE_OVERWRITE) {}
			operation(xstring const& str) : mPCXStringStr(&str), mType(TYPE_PCXSTRING), mMode(MODE_OVERWRITE) {}

			enum EType
			{
				TYPE_UCHAR32,
				TYPE_PCCHAR,
				TYPE_PCUSTR8,
				TYPE_PCUSTR16,
				TYPE_PCUSTR32,
				TYPE_PCXSTRING,
			};

			enum EMode
			{
				MODE_OVERWRITE,
				MODE_INSERT
			};

			union
			{
				uchar32			mChar;
				char const*		mPCCharStr;
				ustr8 const*	mPCUStr8Str;
				ustr16 const*	mPCUStr16Str;
				ustr32 const*	mPCUStr32Str;
				xstring const*	mPCXStringStr;
			};
			EType		mType;
			EMode		mMode;
		};

		class overwrite : public operation
		{
		public:
			overwrite(uchar32 c) : operation(c) {}
			overwrite(char const* str) : operation(str) {}
			overwrite(ustr8 const* str) : operation(str) {}
			overwrite(ustr16 const* str) : operation(str) {}
			overwrite(ustr32 const* str) : operation(str) {}
			overwrite(xstring const& str) : operation(str) {}
		};

		class insert : public operation
		{
		public:
			insert(uchar32 c) : operation(c) { mMode = MODE_INSERT; }
			insert(char const* str) : operation(str) { mMode = MODE_INSERT; }
			insert(ustr8 const* str) : operation(str) { mMode = MODE_INSERT; }
			insert(ustr16 const* str) : operation(str) { mMode = MODE_INSERT; }
			insert(ustr32 const* str) : operation(str) { mMode = MODE_INSERT; }
			insert(xstring const& str) : operation(str) { mMode = MODE_INSERT; }
		};

		// Return a copy of this string
		xstring			copy() const;

		// Return a slice/view of this string
		xstring			operator()(s32 from, s32 to) const { return view(from, to); }
		xstring			operator[](s32) const;

		// Overwrite = explicit behavior
		xstring&		operator=(overwrite const& op);

		// Overwrite = default behavior
		xstring&		operator=(uchar32 c) { *this = overwrite(c); return *this; }
		xstring&		operator=(char const* other) { *this = overwrite(other); return *this; }
		xstring&		operator=(ustr8 const* other) { *this = overwrite(other); return *this; }
		xstring&		operator=(ustr16 const* other) { *this = overwrite(other); return *this; }
		xstring&		operator=(ustr32 const* other) { *this = overwrite(other); return *this; }
		xstring&		operator=(xstring const& other) { *this = overwrite(other); return *this; }

		// Insert
		xstring&		operator=(insert const& op);

		xstring			operator+ (char const* other);
		xstring&		operator+=(char const* other);
		xstring			operator+ (ustr8 const* other);
		xstring&		operator+=(ustr8 const* other);
		xstring			operator+ (xstring const& other);
		xstring&		operator+=(xstring const& other);

		bool			operator==(char const*) const;
		bool			operator==(ustr8 const*) const;
		bool			operator==(ustr16 const*) const;
		bool			operator==(ustr32 const*) const;
		bool			operator==(xstring const&) const;

		uchar32			readat(s32) const;
		void 			writeto(xwriter*) const;

		s32				explode(int n, xstring*& parts) const;

		enum eflags
		{
			CASE = 0,
			NOCASE = 1,
			SELECT = 2,
			LEFT = 0,
			RIGHT = 4,
		};

		xstring			find(uchar c, eflags flags = NOCASE) const;
		xstring			find(char const* str, eflags flags = NOCASE) const;
		xstring			find(ustr8 const* str, eflags flags = NOCASE) const;
		xstring			find(xstring const& c, eflags flags = NOCASE) const;

		xstring			findr(uchar c, eflags flags = NOCASE) const;
		xstring			findr(char const* str, eflags flags = NOCASE) const;
		xstring			findr(ustr8 const* str, eflags flags = NOCASE) const;
		xstring			findr(xstring const& c, eflags flags = NOCASE) const;

		xstring			left_of(xstring const&) const;
		xstring			right_of(xstring const&) const;

		xstring&		tolower();
		xstring&		toupper();
		xstring&		tocamel();

	protected:
		xstring(slice& block);

		struct data;
		explicit		xstring(data*);

		xstring			view(s32 from, s32 to) const;

		uchar32*		str_ptr();
		uchar32 const*	str_ptr() const;

		slice			mSlice;
	};


};	// xCore namespace


#endif    ///< __XBASE_STRING_UTF_H__
