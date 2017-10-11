#ifndef __XBASE_STRING_STD_H__
#define __XBASE_STRING_STD_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif


//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_va_list.h"
#include "xbase\x_integer.h"
#include "xbase\x_console.h"


/**
 * xCore namespace
 */
namespace xcore
{
	typedef					rune					rune;
	typedef					pcrune				pcrune;
	typedef					rune *					prune;

	/**
		*==============================================================================
		* string functions
		*==============================================================================
		*==============================================================================
		* TODO:
		*       * I need to explain all the different options for formated strings
		*
		*  Type Field Characters
		*  Character       Output format
		*  =========       --------------------------------------------------------
		*     %%            a percent sign
		*     %b,%B,%#b     a boolean as true/false (%b=lower-case true/false, %B=upper-case TRUE/FALSE, %#b/%B#=capital-case True/False)
		*     %c            a character with the given number
		*     %s            a string
		*     %d            a signed integer, in decimal
		*     %u            an unsigned integer, in decimal
		*     %o            an unsigned integer, in octal
		*     %x            an unsigned integer, in hexadecimal (lower case)
		*     %e            a floating-point number, in scientific notation
		*     %f            a floating-point number, in fixed decimal notation
		*     %g            a floating-point number, in %e or %f notation
		*     %X            like %x, but using upper-case letters
		*     %E            like %e, but using an upper-case "E"
		*     %G            like %g, but with an upper-case "E" (if applicable)
		*     %p            a pointer (outputs the Perl value's address in hexadecimal)
		*     %n            special: *stores* the number of characters output so far into the next variable in the parameter list
		*     %y,%Y,%#y     a boolean as yes/no (%y=lower-case yes/no, %Y=upper-case YES/NO, %#y/%#Y=capital-case Yes/No)
		*
		*  Size flags
		*  Character    Output
		*  =========    ------------------------------------------------------------
		*   h           interpret integer as s16 or u16
		*   l           interpret integer as s32 or u32
		*   q, L        interpret integer as s64 or u64
		*   L           interpret floating point as higher precision adds a few extra floating point numbers
		*
		* Examples:
		*
		*      x_printf("%Lf %Lf %Lf", Vec.X, Vec.Y, Vec.Z);  // Note that we use the higher precision floating point representation
		*      x_printf("%Ld" (s64)123);                      // This is a way to print an s64 bit number
		*      x_printf("%d" 123);                            // Nothing special here printing an 32bit integer
		*      x_printf("%f" 123.0);                          // Nothing special here printing a floating point number
		*      x_printf("%+010.4f", 123.456);                 // The printf works like you will expect
		*
		*/

	namespace ucstr
	{
		static prune	Len(prune str, s32* str_len);
		static prune	Len(prune str, pcrune str_eos, s32* str_len);

		static prune	Len(pcrune str, s32* str_len);
		static prune	Len(pcrune str, pcrune str_eos, s32* str_len);

		static prune	Copy(prune dest, pcrune dest_end, pcrune src, pcrune src_end);

		static bool		Find(pcrune find, pcrune find_end, pcrune& str_begin, pcrune str_end);
		static void		Replace(prune str_begin, prune str_end, pcrune str_eos, pcrune replace_str, pcrune replace_end);

		static s32  	Compare_Case_Sensitive(pcrune str1, pcrune str1_end, pcrune str2, pcrune str2_end);
		static s32		Compare_Case_Ignore(pcrune str1, pcrune str1_end, pcrune str2, pcrune str2_end);

		static prune	Concatenate(prune dst, pcrune dst_end, pcrune dst_eos, pcrune src, pcrune src_end);		// Concatenate strings, dst = dst + src

		static pcrune	ToValue(pcrune str, pcrune str_end, bool& value);
		static pcrune	ToValue(pcrune str, pcrune str_end, s32& value, s32 base = 10);
		static pcrune	ToValue(pcrune str, pcrune str_end, s64& value, s32 base = 10);
		static pcrune	ToValue(pcrune str, pcrune str_end, f32& value);
		static pcrune	ToValue(pcrune str, pcrune str_end, f64& value);

		static bool		IsDecimal(pcrune str, pcrune str_end);
		static bool		IsHexadecimal(pcrune str, pcrune str_end);
		static bool		IsFloat(pcrune str, pcrune str_end);
		static bool		IsGUID(pcrune str, pcrune str_end);

		static prune	ToString(s32 val, prune str, pcrune str_end, pcrune str_eos, s32 base = 10);
		static prune	ToString(u32 val, prune str, pcrune str_end, pcrune str_eos, s32 base = 10);
		static prune	ToString(s64 val, prune str, pcrune str_end, pcrune str_eos, s32 base = 10);
		static prune	ToString(u64 val, prune str, pcrune str_end, pcrune str_eos, s32 base = 10);
		static prune	ToString(f32 val, s32 numFractionalDigits, prune str, pcrune str_end, pcrune str_eos);
		static prune	ToString(f64 val, s32 numFractionalDigits, prune str, pcrune str_end, pcrune str_eos);

		static bool		IsSpace(uchar32 c) { return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
		static bool		IsUpper(uchar32 c) { return((c >= 'A') && (c <= 'Z')); }
		static bool		IsLower(uchar32 c) { return((c >= 'a') && (c <= 'z')); }
		static bool		IsAlpha(uchar32 c) { return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
		static bool		IsDigit(uchar32 c) { return((c >= '0') && (c <= '9')); }
		static bool		IsNumber(uchar32 c) { return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

		static uchar32	ToUpper(uchar32 c) { return((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
		static uchar32	ToLower(uchar32 c) { return((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
		static u32		ToDigit(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
		static u32		ToNumber(uchar32 c) { if (IsDigit(c)) return ToDigit(c); else if (c >= 'A' && c <= 'F') return(c - 'A' + 10); else if (c >= 'a' && c <= 'f') return(c - 'a' + 10); else return(c); }
		static bool		EqualNoCase(uchar32 a, uchar32 b) { a = ToLower(a); b = ToLower(b); return (a == b); }

		static bool		IsUpper(pcrune str, pcrune str_end);											/// Check if string is all upper case
		static bool		IsLower(pcrune str, pcrune str_end);											/// Check if string is all lower case
		static bool		IsCapitalized(pcrune str, pcrune str_end);										/// Check if string is capitalized ("Shanghai")
		static bool		IsDelimited(pcrune str, pcrune str_end, rune delimit_left = '\"', rune delimit_right = '\"');		///< Check if string is enclosed between left and right delimiter
		static bool		IsQuoted(pcrune str, pcrune str_end, rune quote = '\"')							{ return IsDelimited(str, str_end, quote, quote); }

		static prune	ToUpper(prune str, pcrune str_end);
		static prune	ToLower(prune str, pcrune str_end);

		static pcrune	Find(pcrune str, pcrune str_end, pcrune find, pcrune find_end);				/// Return position of first occurrence of <inString> or -1 if not found
		static pcrune	FindNoCase(pcrune str, pcrune str_end, pcrune find, pcrune find_end);		/// Return position of first occurrence of <inString> or -1 if not found
		static pcrune	FindOneOf(pcrune str, pcrune str_end, pcrune set, pcrune set_end);			/// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		static s32		CPrintf(pcrune format_str, pcrune format_str_end, X_VA_ARGS_16_DEF);
		static prune	SPrintf(prune dst_str, pcrune dst_str_end, pcrune format_str, pcrune format_str_end, X_VA_ARGS_16_DEF);

		static s32		VCPrintf(pcrune format_str, pcrune format_str_end, const x_va_list& args);
		static s32		VSPrintf(prune dst_str, pcrune dst_str_end, pcrune format_str, pcrune format_str_end, const x_va_list& args);

		static s32		Printf(pcrune format_str, pcrune format_str_end, X_VA_ARGS_16_DEF);
		static s32		Printf(pcrune format_str, pcrune format_str_end, const x_va_list& args);
		static s32		Printf(pcrune str, pcrune str_end);

		static s32		SScanf(pcrune str, pcrune str_end, pcrune fmt_str, pcrune fmt_str_end, X_VA_R_ARGS_16_DEF);
		static s32		VSScanf(pcrune str, pcrune str_end, pcrune fmt_str, pcrune fmt_str_end, const x_va_r_list& vr_args);
	};

	// ascii, 'c' style string
	// string <string, string_end>
	// view   <view  , view_end  >
	class xcstr
	{
	public:
		inline 			xcstr() : mStr(NULL), mStr_Begin(NULL), mStr_End(NULL), mStr_Eos(NULL) {}
		inline			xcstr(prune str) : mStr(str), mStr_Begin(str), mStr_End(NULL), mStr_Eos(NULL) { mStr_End = ucstr::Len(str, NULL); mStr_Eos = mStr_End; }
		inline			xcstr(prune str, pcrune eos) : mStr(str), mStr_Begin(str), mStr_End(NULL), mStr_Eos(eos) { mStr_End = ucstr::Len(str, NULL); }
		inline			xcstr(prune str, prune begin, prune end, pcrune eos) : mStr(str), mStr_Begin(begin), mStr_End(end), mStr_Eos(eos) {}
		inline			xcstr(const xcstr& other) : mStr(other.mStr), mStr_Begin(other.mStr_Begin), mStr_End(other.mStr_End), mStr_Eos(other.mStr_Eos) {}

		inline s32		len() const			{ return mStr_End - mStr_Begin; }
		inline bool		is_empty() const	{ return mStr_Begin == mStr_End; }
		inline void		reset()				{ mStr = NULL; mStr_Begin = NULL; mStr_End = NULL; mStr_Eos = NULL; }

		bool			is_space() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
		bool			is_digit() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return((c >= '0') && (c <= '9')); }
		bool			is_alpha() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
		bool			is_upper() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return((c >= 'A') && (c <= 'Z')); }
		bool			is_lower() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return((c >= 'a') && (c <= 'z')); }
		bool			is_hex()   const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

		void			to_upper()			{ if (!is_empty()) ucstr::ToUpper(mStr_Begin, mStr_End); }
		void			to_lower()			{ if (!is_empty()) ucstr::ToLower(mStr_Begin, mStr_End); }
		
		xcstr			operator [](s32 i) const { if (!is_empty() && (mStr_Begin + i) <= mStr_End) return xcstr(mStr, mStr_Begin + i, mStr_End, mStr_Eos); return xcstr(); }
		xcstr			operator ++(s32 i) { if (!is_empty() && *mStr_Begin != '\0' && (mStr_Begin + i) <= mStr_End) return xcstr(mStr, mStr_Begin + i, mStr_End, mStr_Eos); return xcstr(); }

		bool			operator == (const xcstr& other) const { return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) == 0; }
		bool			operator != (const xcstr& other) const { return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) != 0; }
		bool			operator == (const xccstr& other) const { return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) == 0; }
		bool			operator != (const xccstr& other) const { return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) != 0; }

		xcstr&			operator = (const xcstr& other) { ucstr::Copy(mStr_Begin, mStr_Eos, other.mStr_Begin, other.mStr_End); return *this; }
		xcstr&			operator = (const xccstr& other) { ucstr::Copy(mStr_Begin, mStr_Eos, other.mStr_Begin, other.mStr_End); return *this; }
		
		inline			operator rune() const { if (!is_empty()) return mStr_Begin[0]; else return '\0'; }

		xcstr			format(u32 value = 0, pcrune format = "%u");
		xcstr			format(s32 value = 0, pcrune format = "%d");
		xcstr			format(u64 value = 0, pcrune format = "%u");
		xcstr			format(s64 value = 0, pcrune format = "%d");
		xcstr			format(f32 value = 0.0f, pcrune format = "%f");
		xcstr			format(f64 value = 0.0 , pcrune format = "%f");

		xcstr			read(rune& c) { if (!is_empty() && (mStr_Begin + 1) <= mStr_End) { c = mStr_Begin[0]; return xcstr(mStr, mStr_Begin + 1, mStr_End, mStr_Eos); } return xcstr(); }
		xcstr			write(rune c) { if (!is_empty() && (mStr_Begin + 1) <= mStr_End) { mStr_Begin[0] = c; return xcstr(mStr, mStr_Begin + 1, mStr_End, mStr_Eos); } return xcstr(); }

		xcstr&			append(rune c)
		{
			if (!is_empty() && (mStr_End < mStr_Eos))
			{
				if (mStr_End[0] == '\0')
					mStr_End[1] = '\0';
				mStr_End[0] = c;
				mStr_End += 1;
			}
			return *this;
		}
		xcstr&			operator += (rune c) { return append(c); }

		xcstr&			append(pcrune str, pcrune str_end)
		{
			if (!is_empty() && (mStr_End < mStr_Eos))
			{
				bool has_term = (mStr_End[0] == '\0')
				prune str_dst = mStr_End;
				while (str_dst < mStr_Eos && str < str_end)
				{
					uchar32 c = ReadChar(str);
					WriteChar(c, str_dst, str_end);
				}
			}
			return *this;
		}
		xcstr&			operator += (pcrune str) 
		{ 
			pcrune end = ucstr::Len(str, NULL);
			return append(str, end);
		}
		xcstr&			operator += (xcstr const& str) { return append(str.mStr_Begin, str.mStr_End); }
		xcstr&			operator += (xccstr const& str) { return append(str.mStr_Begin, str.mStr_End); }
		
		xcstr			last() const
		{
			if (!is_empty())
			{
				return xcstr(mStr, mStr_End - 1, mStr_End, mStr_Eos);
			}
			return xcstr();
		}

		xcstr			end() const
		{
			if (!is_empty())
			{
				return xcstr(mStr, mStr_End, mStr_End, mStr_Eos);
			}
			return xcstr();
		}

		xcstr			find(xcstr const& find) const
		{
			return find(find.mStr_Begin, find.mStr_End);
		}

		xcstr			find_one_of(xccstr const& set) const
		{
			pcrune f = ucstr::FindOneOf(mStr_Begin, mStr_End, set.mStr_Begin, set.mStr_End);
			if (f == NULL)
				f = mStr_End;
			return xcstr(mStr, f, mStr_End, mStr_Eos);
		}

		xcstr			find(rune find) const
		{
			rune str[2];
			str[0] = find;
			str[1] = '\0';
			return find(str, str + 1);
		}

		xcstr&			select_until_any_of(xccstr const& any_of);
		xcstr&			select_delimited(rune left, rune right);

		xcstr&			select(xccstr const& from, xccstr const& to)
		{
			if (from.mStr == to.mStr && from.mStr_Begin <= to.mStr_Begin)
			{
				mStr       = from.mStr;
				mStr_Begin = from.mStr_Begin;
				mStr_End   = to.mStr_Begin;
				mStr_Eos   = to.mStr_Eos;
			}
			return *this;
		}
		
		xcstr			replace(xcstr const& str)
		{
			return replace(str.mStr_Begin, str.mStr_End);
		}

		void			copy(xcstr const& src)
		{
			mStr_End = ucstr::Copy(mStr_Begin, mStr_Eos, src.mStr_Begin, src.mSrc_End);
		}

		void			copy(xccstr const& src)
		{
			mStr_End = ucstr::Copy(mStr_Begin, mStr_Eos, src.mStr_Begin, src.mSrc_End);
		}

	protected:
		xcstr			find(pcrune find, pcrune find_end) const
		{
			pcrune str = mStr_Begin;
			if (ucstr::Find(find, find_end, str, mStr_End))
			{
				s32 const pos = (str - mStr_Begin);
				return xcstr(mStr, mStr_Begin + pos, mStr_End, mStr_Eos);
			}
			return xcstr();
		}

		xcstr			replace(pcrune replace_str, pcrune replace_end)
		{
			ucstr::Replace(mS1tr_Begin, mStr_End, mStr_Eos, replace_str, replace_end);
			return *this;
		}
		friend class xccstr;
		prune			mStr;
		prune			mStr_Begin;
		prune			mStr_End;
		pcrune			mStr_Eos;
	};

	// ascii, 'c' style 'const' string
	class xccstr
	{
	public:
		inline 			xccstr() : mStr(NULL), mStr_Begin(NULL), mStr_End(NULL), mStr_Eos(NULL) {}
		inline 			xccstr(xcstr const& other) : mStr(other.mStr), mStr_Begin(other.mStr_Begin), mStr_End(other.mStr_End), mStr_Eos(other.mStr_Eos) {}
		inline 			xccstr(xccstr const& other) : mStr(other.mStr), mStr_Begin(other.mStr_Begin), mStr_End(other.mStr_End), mStr_Eos(other.mStr_Eos) {}
		inline			xccstr(pcrune str) : mStr(str), mStr_Eos(NULL) { mStr_Begin = str; mStr_End = ucstr::Len(str, NULL); mStr_Eos = mStr_End; }
		inline			xccstr(pcrune str, pcrune end, pcrune eos) : mStr(str), mStr_Begin(str), mStr_End(end), mStr_Eos(eos) {}

		inline s32		len() const					{ return mStr_End - mStr_Begin; }
		inline bool		is_empty() const			{ return mStr_Begin == mStr_End; }

		bool			operator == (const xcstr& other) const		{ return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) == 0; }
		bool			operator != (const xcstr& other) const		{ return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) != 0; }
		bool			operator == (const xccstr& other) const		{ return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) == 0; }
		bool			operator != (const xccstr& other) const		{ return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) != 0; }

		xccstr			operator [](s32 i) const	{ if (!is_empty() && (mStr_Begin + i) <= mStr_End) return xccstr(mStr, mStr_Begin + i, mStr_End, mStr_Eos); return xccstr(); }
		xccstr			operator ++(s32 i)			{ if (!is_empty() && (mStr_Begin + 1) <= mStr_End) return xccstr(mStr, mStr_Begin + 1, mStr_End, mStr_Eos); return xccstr(); }

		inline			operator rune() const		{ if (!is_empty()) return mStr_Begin[0]; else return '\0'; }

		xccstr			read(rune& c)
		{
			if (!is_empty() && (mStr_Begin + 1) <= mStr_End)
			{
				c = mStr_Begin[0];
				return xccstr(mStr, mStr_Begin + 1, mStr_End, mStr_Eos);
			}
			return xccstr();
		}

		xccstr			find(xcstr const& other) const
		{
			pcrune str = mStr_Begin;
			if (ucstr::Find(other.mStr_Begin, other.mStr_End, str, mStr_End))
				return xccstr(mStr, str, mStr_End, mStr_Eos);
			return xccstr();
		}

		xccstr			find(xccstr const& other) const
		{
			pcrune str = mStr_Begin;
			if (ucstr::Find(other.mStr_Begin, other.mStr_End, str, mStr_End))
				return xccstr(mStr, str, mStr_End, mStr_Eos);
			return xccstr();
		}

		xccstr			find(rune find) const
		{
			rune str[2];
			str[0] = find;
			str[1] = '\0';
			return find(str, str + 1);
		}

		xccstr			select(xccstr const& from, xccstr const& to)
		{
			if (from.mStr == to.mStr)
			{
				return xccstr(from.mStr, from.mStr_Begin, to.mStr_Begin, to.mStr_Eos);
			}
			else
			{
				return xccstr();
			}
		}

		s32				compare(xcstr const& other, bool ignore_case) const
		{
			if (ignore_case)
				return ucstr::Compare_Case_Ignore(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End);
			return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End);
		}

		s32				compare(xccstr const& other, bool ignore_case) const
		{
			if (ignore_case)
				return ucstr::Compare_Case_Ignore(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End);
			return ucstr::Compare_Case_Sensitive(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End);
		}

	protected:
		xccstr			find(pcrune find, pcrune find_eos) const
		{
			pcrune str = mStr_Begin;
			if (ucstr::Find(find, find_eos, str, mStr_End))
				return xccstr(mStr, str, mStr_End, mStr_Eos);
			return xccstr();
		}

		friend class xcstr;

		pcrune		mStr;
		pcrune		mStr_Begin;
		pcrune		mStr_End;
		pcrune		mStr_Eos;
	};


	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif    ///< __X_STRING_STD_H__
