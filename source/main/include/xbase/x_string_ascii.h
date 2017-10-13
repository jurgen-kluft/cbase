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

	struct xbstr
	{
		inline			xbstr() : mStr(sNullStr), mEnd(sNullStr), mEos(sNullStr) { }
		inline			xbstr(prune str) : mStr(str), mEnd(NULL), mEos(NULL) { }
		inline			xbstr(prune str, prune eos) : mStr(str), mEnd(NULL), mEos(eos) { }
		inline			xbstr(prune str, prune end, prune eos) : mStr(str), mEnd(end), mEos(eos) { }
		inline			xbstr(xbstr const& other) : mStr(other.mStr), mEnd(other.mEnd), mEnd(other.mEos) { }
		
		inline s32		len() const			{ return mEnd - mStr; }
		inline s32		is_empty() const	{ return mStr == mEnd; }

		inline xstr		instance() const
		{
			if (mEnd == NULL)
			{
				if (mEos == NULL)
				{
					mEnd = ucstr::Len(str, NULL); 
					mEos = mEnd;
				}
				else 
				{
					mEnd = ucstr::Len(str, mEos, NULL); 
				}
			}
			return xstr(this, mStr, mEnd);
		}
		
	protected:
		prune			mStr;
		prune			mEnd;
		prune			mEos;
		static prune	sNullStr = "";
	};

	struct xbstrc
	{
		inline			xbstrc() : mStr(sNullStr), mEnd(sNullStr) { }
		inline			xbstrc(pcrune str) : mStr(str), mEnd(NULL) { }
		inline			xbstrc(pcrune str, pcrune end) : mStr(str), mEnd(end) { }
		inline			xbstrc(xbstr const& other) : mStr(other.mStr), mEnd(other.mEnd) { }
		inline			xbstrc(xbstrc const& other) : mStr(other.mStr), mEnd(other.mEnd) { }
		
		inline s32		len() const			{ return mEnd - mStr; }
		inline s32		is_empty() const	{ return mStr == mEnd; }

		inline xcstr	instance() const
		{
			if (mEnd == NULL)
				mEnd = ucstr::Len(mStr, NULL);
			return xcstr(this, mStr, mEnd);
		}
		
	protected:
		pcrune			mStr;
		pcrune			mEnd;
		static pcrune	sNullStr = "";
	};

	
	// ascii, 'c' style 'const' string
	class xcstr
	{
		friend class xstr;
		
		xbstrc*			mStr;
		pcrune			mBegin;
		pcrune			mEnd;

		inline			xcstr(xbstrc* str) : mStr(str), Begin(str->mBegin), mEnd(str->mEnd) { }
		inline			xcstr(xbstrc* str, pcrune begin, pcrune end) : mStr(str), mBegin(begin), mEnd(end) { }

	public:
		inline 			xcstr(xcstr const& other) : mStr(other.mStr), mBegin(other.mBegin), mEnd(other.mEnd) {}

		inline s32		len() const					{ return mEnd - mBegin; }

		bool			is_space() const	{ rune const c = mBegin[0]; return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
		bool			is_digit() const	{ rune const c = mBegin[0]; return((c >= '0') && (c <= '9')); }
		bool			is_alpha() const	{ rune const c = mBegin[0]; return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
		bool			is_upper() const	{ rune const c = mBegin[0]; return((c >= 'A') && (c <= 'Z')); }
		bool			is_lower() const	{ rune const c = mBegin[0]; return((c >= 'a') && (c <= 'z')); }
		bool			is_hex()   const	{ rune const c = mBegin[0]; return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }
		
		xcstr			read(rune& c) const
		{
			if (mBegin < mEnd)
			{
				c = mBegin[0];
				return xccstr(mStr, mBegin + 1, mEnd, mStr->mEos);
			}
			else
			{
				c = '\0';
				return xccstr();
			}
		}

		xcstr			find(xstr const& other) const	{ return find(other.mBegin, other.mEnd); }
		xcstr			find(xcstr const& other) const	{ return find(other.mBegin, other.mEnd); }
		xcstr			find(rune find) const			{ rune str[] = { find, '\0'}; return find(str, str + 1); }

		s32				compare(xcstr const& other, bool ignore_case) const		{ return compare(other.mBegin, other.mEnd, ignore_case); }
		s32				compare(xccstr const& other, bool ignore_case) const	{ return compare(other.mBegin, other.mEnd, ignore_case); }

		static xcstr	select(xcstr const& from, xcstr const& to)
		{
			if (from.mStr == to.mStr)
				return xccstr(from.mStr, from.mBegin, to.mBegin);
			return xccstr(from.mStr, from.mStr->mEnd, from.mStr->mEnd);
		}

		inline			operator rune() const						{ return mBegin[0]; }
		
		bool			operator == (const xcstr& other) const		{ return compare(mBegin, mEnd, other.mBegin, other.mEnd, false) == 0; }
		bool			operator != (const xcstr& other) const		{ return compare(mBegin, mEnd, other.mBegin, other.mEnd, false) != 0; }
		bool			operator == (const xccstr& other) const		{ return compare(mBegin, mEnd, other.mBegin, other.mEnd, false) == 0; }
		bool			operator != (const xccstr& other) const		{ return compare(mBegin, mEnd, other.mBegin, other.mEnd, false) != 0; }

		xcstr			operator [](s32 i) const	{ if ((mBegin + i) <= mEnd) return xccstr(mStr, mBegin + i, mEnd, mStr->mEos); return xccstr(); }
		xcstr			operator ++(s32 i)			{ if ((mBegin + 1) <= mEnd) return xccstr(mStr, mBegin + 1, mEnd, mStr->mEos); return xccstr(); }

	
	protected:
		xcstr			find(pcrune find, pcrune find_end) const
		{
			pcrune str = mBegin;
			if (ucstr::Find(find, find_end, str, mEnd))
				return xcstr(mStr, str, mEnd, mStr->mEos);
			return xcstr();
		}
		s32				compare(pcrune other, pcrune other_end, bool ignore_case) const
		{
			if (ignore_case)
				return ucstr::Compare_Case_Ignore(mBegin, mEnd, other, other_end);
			return ucstr::Compare_Case_Sensitive(mBegin, mEnd, other, other_end);
		}
	
	};


	// ascii, 'c' style string
	// string <string, string_end>
	// view   <view  , view_end  >
	class xstr
	{
		friend class xcstr;

		xbstr*			mStr;
		prune			mBegin;
		prune			mEnd;
		
		inline			xstr(xbstr* str) : mStr(str), Begin(str->mBegin), mEnd(str->mEnd) { }
		inline			xstr(xbstr* str, prune begin, prune end) : mStr(str), mBegin(begin), mEnd(end) { }
		
	public:
		inline			xstr(const xstr& other) : mStr(other.mStr), mBegin(other.mBegin), mEnd(other.mEnd) {}

		inline s32		len() const			{ return mEnd - mBegin; }
		inline void		reset()				{ mBegin = mStr->mStr; mEnd = mStr->mEnd; }

		bool			is_space() const	{ rune const c = mBegin[0]; return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
		bool			is_digit() const	{ rune const c = mBegin[0]; return((c >= '0') && (c <= '9')); }
		bool			is_alpha() const	{ rune const c = mBegin[0]; return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
		bool			is_upper() const	{ rune const c = mBegin[0]; return((c >= 'A') && (c <= 'Z')); }
		bool			is_lower() const	{ rune const c = mBegin[0]; return((c >= 'a') && (c <= 'z')); }
		bool			is_hex()   const	{ rune const c = mBegin[0]; return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

		void			to_upper()			{ ucstr::ToUpper(mBegin, mEnd); }
		void			to_lower()			{ ucstr::ToLower(mBegin, mEnd); }
		
		xstr			format(u32 value = 0, pcrune format = "%u");
		xstr			format(s32 value = 0, pcrune format = "%d");
		xstr			format(u64 value = 0, pcrune format = "%u");
		xstr			format(s64 value = 0, pcrune format = "%d");
		xstr			format(f32 value = 0.0f, pcrune format = "%f");
		xstr			format(f64 value = 0.0 , pcrune format = "%f");

		xstr			read(rune& c) const { if (mBegin < mEnd)	{ c = mBegin[0]; return xstr(mStr, mBegin + 1, mEnd); } return xstr(mStr, mEnd, mEnd); }
		xstr			write(rune c) { if (mBegin < mEnd)			{ mBegin[0] = c; return xstr(mStr, mBegin + 1, mEnd); } return xstr(mStr, mEnd, mEnd); }

		xstr&			append(rune c)
		{
			if (!is_empty() && (mEnd < mStr->mEos))
			{
				if (mEnd[0] == '\0')
					mEnd[1] = '\0';
				mEnd[0] = c;
				mEnd += 1;
			}
			return *this;
		}
		xstr&			operator += (rune c) { return append(c); }

		xstr&			operator += (xstr const& str) { return append(str.mBegin, str.mEnd); }
		xstr&			operator += (xcstr const& str) { return append(str.mBegin, str.mEnd); }
		
		xstr			last() const
		{
			if (!is_empty())
			{
				return xstr(mStr, mEnd - 1, mEnd, mStr->mEos);
			}
			return xcstr();
		}

		xstr			end() const
		{
			if (!is_empty())
			{
				return xstr(mStr, mEnd, mEnd, mStr->mEos);
			}
			return xstr();
		}

		xstr			find(xcstr const& find) const
		{
			return find(find.mBegin, find.mEnd);
		}

		xstr			find_one_of(xccstr const& set) const
		{
			pcrune f = ucstr::FindOneOf(mBegin, mEnd, set.mBegin, set.mEnd);
			if (f == NULL)
				f = mEnd;
			return xstr(mStr, f, mEnd, mStr->mEos);
		}

		xstr			find(rune find) const
		{
			rune str[2];
			str[0] = find;
			str[1] = '\0';
			return find(str, str + 1);
		}

		static xstr&	select(xcstr const& from, xcstr const& to);

		xstr&			select_until_one_of(xcstr const& any_of);
		xstr&			select_delimited(rune left, rune right);
	
		xstr			replace(xstr const& str)
		{
			return replace(str.mBegin, str.mEnd);
		}

		void			copy(xstr const& src)
		{
			mStr->End = ucstr::Copy(mStr->mStr, mStr->mEos, src.mBegin, src.mEnd);
			mBegin = mStr->mStr;
			mEnd = mStr->mEnd;
		}

		void			copy(xccstr const& src)
		{
			mStr->End = ucstr::Copy(mStr->mStr, mStr->mEos, src.mBegin, src.mEnd);
			mBegin = mStr->mStr;
			mEnd = mStr->mEnd;
		}

		xstr			operator [](s32 i) const { if ((mBegin + i) <= mEnd) return xstr(mStr, mBegin + i, mEnd); return xstr(mStr, mEnd, mEnd); }
		xstr			operator ++(s32 i) { if (*mBegin != '\0' && (mBegin + i) <= mEnd) return xstr(mStr, mBegin + i, mEnd); return xstr(mStr, mEnd, mEnd); }

		bool			operator == (const xcstr& other) const { return ucstr::Compare_Case_Sensitive(mBegin, mEnd, other.mBegin, other.mEnd) == 0; }
		bool			operator != (const xcstr& other) const { return ucstr::Compare_Case_Sensitive(mBegin, mEnd, other.mBegin, other.mEnd) != 0; }
		bool			operator == (const xccstr& other) const { return ucstr::Compare_Case_Sensitive(mBegin, mEnd, other.mBegin, other.mEnd) == 0; }
		bool			operator != (const xccstr& other) const { return ucstr::Compare_Case_Sensitive(mBegin, mEnd, other.mBegin, other.mEnd) != 0; }

		xcstr&			operator = (const xcstr& other) { mEnd = ucstr::Copy(mBegin, mStr->mEos, other.mBegin, other.mEnd); return *this; }
		xcstr&			operator = (const xccstr& other) { mEnd = ucstr::Copy(mBegin, mStr->mEos, other.mBegin, other.mEnd); return *this; }
		
		inline			operator rune() const { if (!is_empty()) return mBegin[0]; else return '\0'; }

	protected:
		xstr&			append(pcrune str, pcrune str_end)
		{
			if (!is_empty() && (mEnd < mStr->mEos))
			{
				bool has_term = (mEnd[0] == '\0')
				prune str_dst = mEnd;
				while (str_dst < mStr->mEos && str < str_end)
				{
					uchar32 c = ReadChar(str);
					WriteChar(c, str_dst, str_end);
				}
			}
			return *this;
		}

		xstr			find(pcrune find, pcrune find_end) const
		{
			pcrune str = mBegin;
			if (ucstr::Find(find, find_end, str, mEnd))
			{
				s32 const pos = (str - mBegin);
				return xstr(mStr, mBegin + pos, mEnd);
			}
			return xstr(mStr, mEnd, mEnd);
		}

		xstr			replace(pcrune replace_str, pcrune replace_end)
		{
			mEnd = ucstr::Replace(mStr->mStr, mBegin, mEnd, mStr->mEos, replace_str, replace_end);
			return *this;
		}
	
	};



	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif    ///< __X_STRING_STD_H__
