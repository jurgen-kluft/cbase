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

	class ucstr
	{
	public:
		static prune sCopy(prune dest, pcrune dest_end, pcrune src);

		static s32	sLen(pcrune str)
		{
			s32 len = 0;
			pcrune src = str;
			while (*src != '\0')
			{
				src += 1;
				len += 1;
			};
			return len;
		}

		static s32	sLen(pcrune str, pcrune str_end)
		{
			s32 len = 0;
			pcrune src = str;
			while (*src != '\0' && src < str_end)
			{
				src += 1;
				len += 1;
			};
			return len;
		}

		static bool	sFind(pcrune find, pcrune find_end, pcrune& str_begin, pcrune str_end)
		{
			pcrune src = str_begin;
			while (*src != '\0' && src < str_end)
			{
				rune const * find_src = src;

				pcrune find_str = find;

				bool found = (*find_str == *find_src);
				while (found)
				{
					if (*find_str != '\0' && find_str < find_end)
						break;
					find_str += 1;
					find_src += 1;
					found = (*find_str == *find_src);
				}

				if (found)
				{
					str_begin = src;
					return true;
				}
				src += 1;
			}
			return false;
		}

		static void sReplace(prune str_begin, prune str_end, pcrune str_eos, pcrune replace_str, pcrune replace_end)
		{
			s32 const selected_len = str_end - str_begin;
			s32 const replace_len = replace_end - replace_str;
			if (selected_len < replace_len)
			{
				// Move, increasing
				s32 move_len = replace_len - selected_len;
				if (move_len > (str_eos - str_end))
					move_len = (str_eos - str_end);

				// No matter what, push out anything at the end!
				rune * dst = str_end + (str_eos - str_end) - 1;
				rune * src = dst - move_len;

				while (dst > str_end)
					*dst-- = *src--;
			}
			else if (selected_len > replace_len)
			{
				// Move, reducing
				s32 move_len = selected_len - replace_len;

				rune * dst = str_end - move_len;
				rune * src = str_end;

				while (src < str_eos)
					*dst++ = *src++;
			}

			// Replace
			pcrune src = replace_str;
			pcrune end = replace_str + replace_len;
			rune* dst = str_begin;
			while (src < end)
				*dst++ = *src++;
		}

		static s32  	Cmp_cs(pcrune str1, pcrune str1_end, pcrune str2, pcrune str2_end)
		{

		}

		static s32		Cmp_ci(pcrune str1, pcrune str1_end, pcrune str2, pcrune str2_end);				// Compare str1 to str2, case insensitive

		static prune	Cat(prune dst, pcrune dst_end, pcrune dst_eos, pcrune src, pcrune src_end);		// Concatenate strings, dst = dst + src

		static pcrune	ToValue(pcrune str, pcrune str_end, bool& value);
		static pcrune	ToValue(pcrune str, pcrune str_end, s32& value, s32 base = 10);
		static pcrune	ToValue(pcrune str, pcrune str_end, s64& value, s32 base = 10);
		static pcrune	ToValue(pcrune str, pcrune str_end, f32& value);
		static pcrune	ToValue(pcrune str, pcrune str_end, f64& value);

		static bool		IsNumber(uchar32 c, s32 base);
		static bool		IsHex(pcrune str, pcrune str_end);
		static bool		IsInt(pcrune str, pcrune str_end);
		static bool		IsFloat(pcrune str, pcrune str_end);
		static bool		IsGUID(pcrune str, pcrune str_end);

		static prune	ToString(s32 val, prune str, pcrune str_end, pcrune str_eos, s32 base = 10);
		static prune	ToString(u32 val, prune str, pcrune str_end, pcrune str_eos, s32 base = 10);
		static prune	ToString(s64 val, prune str, pcrune str_end, pcrune str_eos, s32 base = 10);
		static prune	ToString(u64 val, prune str, pcrune str_end, pcrune str_eos, s32 base = 10);
		static prune	ToString(f32 val, s32 numFractionalDigits, prune str, pcrune str_end, pcrune str_eos);
		static prune	ToString(f64 val, s32 numFractionalDigits, prune str, pcrune str_end, pcrune str_eos);

		static bool		IsSpace(rune c) { return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
		static bool		IsDigit(rune c) { return((c >= '0') && (c <= '9')); }
		static bool		IsAlpha(rune c) { return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
		static bool		IsUpper(rune c) { return((c >= 'A') && (c <= 'Z')); }
		static bool		IsLower(rune c) { return((c >= 'a') && (c <= 'z')); }
		static bool		IsHex(rune  c) { return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

		static rune		ToUpper(rune c) { return((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
		static rune		ToLower(rune c) { return((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
		static rune		ToDigit(rune c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
		static rune		ToNumber(rune c) { if (IsDigit(c)) return ToDigit(c); else if (c >= 'A' && c <= 'F') return(c - 'A' + 10); else if (c >= 'a' && c <= 'f') return(c - 'a' + 10); else return(c); }
		static bool		EqualNoCase(rune a, rune b) { if (a == b) return true; a = ToLower(a); b = ToLower(b); return (a == b); }

		static bool		IsUpper(pcrune str, pcrune str_end);											/// Check if string is all upper case
		static bool		IsLower(pcrune str, pcrune str_end);											/// Check if string is all lower case
		static bool		IsCapitalized(pcrune str, pcrune str_end);										/// Check if string is capitalized ("Shanghai")
		static bool		IsDelimited(pcrune str, pcrune eos, rune delimit_left = '\"', rune delimit_right = '\"');		///< Check if string is enclosed between left and right delimiter
		static bool		IsQuoted(pcrune str, pcrune str_end, rune quote = '\"')							{ return IsDelimited(str, str_end, quote, quote); }

		static pcrune	Find(pcrune str, pcrune str_end, pcrune find, pcrune find_end);				/// Return position of first occurrence of <inString> or -1 if not found
		static pcrune	FindNoCase(pcrune str, pcrune str_end, pcrune find, pcrune find_end);		/// Return position of first occurrence of <inString> or -1 if not found
		static pcrune	FindOneOf(pcrune str, pcrune str_end, pcrune set, pcrune set_end);			/// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		static s32		CPrintf(pcrune format_str, pcrune format_str_end, X_VA_ARGS_16_DEF);
		static s32		SPrintf(prune dst_str, pcrune dst_str_end, pcrune format_str, pcrune format_str_end, X_VA_ARGS_16_DEF);

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
		inline			xcstr(rune * str) : mStr(str), mStr_Begin(str), mStr_End(NULL), mStr_Eos(NULL) { s32 const l = ucstr::sLen(str); mStr_End = mStr + l; mStr_Eos = mStr_End; }
		inline			xcstr(rune * str, pcrune eos) : mStr(str), mStr_Begin(str), mStr_End(NULL), mStr_Eos(eos) { s32 const l = ucstr::sLen(str); mStr_End = mStr + l; }
		inline			xcstr(rune * str, rune * begin, rune * end, pcrune eos) : mStr(str), mStr_Begin(begin), mStr_End(end), mStr_Eos(eos) {}
		inline			xcstr(const xcstr& other) : mStr(other.mStr), mStr_Begin(other.mStr_Begin), mStr_End(other.mStr_End), mStr_Eos(other.mStr_Eos) {}

		inline s32		len() const			{ return mStr_End - mStr_Begin; }
		inline bool		is_empty() const	{ return mStr_Begin == mStr_End; }

		bool			is_space() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
		bool			is_digit() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return((c >= '0') && (c <= '9')); }
		bool			is_alpha() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
		bool			is_upper() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return((c >= 'A') && (c <= 'Z')); }
		bool			is_lower() const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return((c >= 'a') && (c <= 'z')); }
		bool			is_hex()   const	{ if (is_empty()) return false; rune const c = mStr_Begin[0]; return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

		void			to_upper()
		{ 
			if (!is_empty())
			{
				rune* src = mStr_Begin;
				while (src < mStr_End)
				{
					rune const c = src[0];
					src[0] = ucstr::ToUpper(c);
					src++;
				}
			}
		}
		void			to_lower()
		{
			if (!is_empty())
			{
				rune* src = mStr_Begin;
				while (src < mStr_End)
				{
					rune const c = src[0];
					src[0] = ucstr::ToLower(c);
					src++;
				}
			}
		}

		xcstr			operator [](s32 i) const { if (!is_empty() && (mStr_Begin + i) <= mStr_End) return xcstr(mStr, mStr_Begin + i, mStr_End, mStr_Eos); return xcstr(); }
		xcstr			operator ++(s32 i) { if (!is_empty() && *mStr_Begin != '\0' && (mStr_Begin + i) <= mStr_End) return xcstr(mStr, mStr_Begin + i, mStr_End, mStr_Eos); return xcstr(); }

		bool			operator == (const xcstr& other) const { return ucstr::Cmp_cs(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) == 0; }
		bool			operator != (const xcstr& other) const { return ucstr::Cmp_cs(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) != 0; }
		bool			operator == (const xccstr& other) const { return ucstr::Cmp_cs(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) == 0; }
		bool			operator != (const xccstr& other) const { return ucstr::Cmp_cs(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) != 0; }

		xcstr&			operator = (const xcstr& other)
		{
			pcrune src_it  = other.mStr_Begin;
			pcrune src_end = other.mStr_End;
			prune  dst_it  = mStr_Begin;
			prune  dst_end = mStr_Eos;
			bool const has_terminator = mStr_End < mStr_Eos ? mStr_End[0] == '\0' : false;
			while (true)
			{
				if (dst_it == dst_end)
					break;
				if (src_it == src_end)
				{
					if (has_terminator)
						*dst_it = '\0';
					break;
				}
				*dst_it++ = *src_it++;
			}
			mStr_End = dst_it;
			return *this;
		}

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
			}
			return *this;
		}

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

		xcstr			find(pcrune find, pcrune find_end) const
		{
			pcrune str = mStr_Begin;
			if (ucstr::sFind(find, find_end, str, mStr_End))
			{
				s32 const pos = (str - mStr_Begin);
				return xcstr(mStr, mStr_Begin + pos, mStr_End, mStr_Eos);
			}
			return xcstr();
		}
		xcstr			find(rune find) const
		{
			rune str[2];
			str[0] = find;
			str[1] = '\0';
			return xcstr::find(str, str + 1);
		}

		xcstr			replace(pcrune replace_str, pcrune replace_end)
		{
			ucstr::sReplace(mStr_Begin, mStr_End, mStr_Eos, replace_str, replace_end);
			return *this;
		}

		void			copy(xcstr& dst);

	protected:
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
		inline			xccstr(pcrune str) : mStr(str), mStr_Eos(NULL) { mStr_Begin = str; s32 const l = ucstr::sLen(str); mStr_End = mStr + l; mStr_Eos = mStr_End; }
		inline			xccstr(pcrune str, pcrune end, pcrune eos) : mStr(str), mStr_Begin(str), mStr_End(end), mStr_Eos(eos) {}

		inline s32		len() const					{ return mStr_End - mStr_Begin; }
		inline bool		is_empty() const			{ return mStr == NULL || mStr == mStr_Eos; }

		bool			operator == (const xcstr& other) const		{ return ucstr::Cmp_cs(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) == 0; }
		bool			operator != (const xcstr& other) const		{ return ucstr::Cmp_cs(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) != 0; }
		bool			operator == (const xccstr& other) const		{ return ucstr::Cmp_cs(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) == 0; }
		bool			operator != (const xccstr& other) const		{ return ucstr::Cmp_cs(mStr_Begin, mStr_End, other.mStr_Begin, other.mStr_End) != 0; }

		xccstr			operator[](s32 i) const		{ pcrune str = mStr + 1; if (str < mStr_End) return xccstr(str, mStr_End, mStr_Eos); return xccstr(); }
		xccstr			operator ++(s32 i)			{ if (!is_empty() && (mStr + i) <= mStr_End) return xccstr(mStr + i, mStr_End, mStr_Eos); return xccstr(); }
		inline			operator rune() const		{ if (!is_empty()) return mStr[0]; else return '\0'; }

		xccstr			read(rune& c)
		{
			if (!is_empty() && (mStr + 1) <= mStr_End)
			{
				c = mStr[0];
				return xccstr(mStr + 1, mStr_End, mStr_Eos);
			}
			return xccstr();
		}

		xccstr			find(pcrune find, pcrune find_eos) const
		{
			pcrune str = mStr;
			if (ucstr::sFind(find, find_eos, str, mStr_End))
				return xccstr(str, mStr_End, mStr_Eos);
			return xccstr();
		}

		xccstr			find(rune find) const
		{
			rune str[2];
			str[0] = find;
			str[1] = '\0';
			return xccstr::find(str, str + 1);
		}
		void			copy(xcstr& dst);

	protected:
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
