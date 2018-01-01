#ifndef __XBASE_STRING_STD_H__
#define __XBASE_STRING_STD_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_va_list.h"

namespace xcore
{
	namespace ascii
	{
		typedef			uchar					rune;
		typedef			uchar *					prune;
		typedef			const uchar*			pcrune;

		inline bool		has_fixed_size_rune() { return true; }
		inline s32		get_fixed_sizeof_rune() { return sizeof(rune); }

		enum ECopyType { NONE = 0, COPY_AND_WRITE_TERMINATOR = 1, COPY_AND_WRITE_MATCHING_TERMINATOR = 2 };
		enum ECmpMode { CASE_SENSITIVE, CASE_IGNORE };

		void			copy(xuchars& dst, xcuchars& src, ECopyType type = COPY_AND_WRITE_MATCHING_TERMINATOR);
		void			concatenate(xuchars& dst, xcuchars& src);		// Concatenate strings, dst = dst + src

		xuchars			find(xuchars& str, xcuchars& find, ECmpMode mode = CASE_SENSITIVE);			// Return position of first occurrence of <inString> or -1 if not found
		xuchars			find_one_of(xuchars& str, xcuchars& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		xcuchars		find(xcuchars& str, xcuchars& find, ECmpMode mode = CASE_SENSITIVE);		// Return position of first occurrence of <inString> or -1 if not found
		xcuchars		find_one_of(xcuchars& str, xcuchars& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found
		
		void			replace(xuchars& str, xcuchars& replace);

		s32  			compare(xcuchars& str1, xcuchars& str2, ECmpMode mode = CASE_SENSITIVE);

		xcuchars		parse(xcuchars& str, bool& value);
		xcuchars		parse(xcuchars& str, s32& value, s32 base = 10);
		xcuchars		parse(xcuchars& str, u32& value, s32 base = 10);
		xcuchars		parse(xcuchars& str, s64& value, s32 base = 10);
		xcuchars		parse(xcuchars& str, u64& value, s32 base = 10);
		xcuchars		parse(xcuchars& str, f32& value);
		xcuchars		parse(xcuchars& str, f64& value);

		bool			is_decimal(xcuchars& str);
		bool			is_hexadecimal(xcuchars& str, bool with_prefix=false);
		bool			is_float(xcuchars& str);
		bool			is_GUID (xcuchars& str);

		void			to_string(xuchars& str, s32 val, s32 base = 10);
		void			to_string(xuchars& str, u32 val, s32 base = 10);
		void			to_string(xuchars& str, s64 val, s32 base = 10);
		void			to_string(xuchars& str, u64 val, s32 base = 10);
		void			to_string(xuchars& str, f32 val, s32 num_fractional_digits = 4);
		void			to_string(xuchars& str, f64 val, s32 num_fractional_digits = 4);

		inline bool		is_space(uchar32 c) { return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
		inline bool		is_upper(uchar32 c) { return((c >= 'A') && (c <= 'Z')); }
		inline bool		is_lower(uchar32 c) { return((c >= 'a') && (c <= 'z')); }
		inline bool		is_alpha(uchar32 c) { return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
		inline bool		is_digit(uchar32 c) { return((c >= '0') && (c <= '9')); }
		inline bool		is_hexa(uchar32 c) { return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

		inline uchar32	to_upper(uchar32 c) { return((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
		inline uchar32	to_lower(uchar32 c) { return((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
		inline u32		to_digit(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
		inline u32		to_number(uchar32 c) { if (is_digit(c)) return to_digit(c); else if (c >= 'A' && c <= 'F') return(c - 'A' + 10); else if (c >= 'a' && c <= 'f') return(c - 'a' + 10); else return(c); }

		inline bool		is_equal(uchar32 a, uchar32 b, ECmpMode mode = CASE_SENSITIVE) { if (mode == CASE_IGNORE) { a = to_lower(a); b = to_lower(b); } return (a == b); }

		bool			is_upper(xcuchars& str);											/// Check if string is all upper case
		bool			is_lower(xcuchars& str);											/// Check if string is all lower case
		bool			is_capitalized(xcuchars& str);										/// Check if string is capitalized ("Shanghai")
		bool			is_delimited(xcuchars& str, rune delimit_left = '\"', rune delimit_right = '\"');		///< Check if string is enclosed between left and right delimiter
		inline bool		is_quoted(xcuchars& str, rune quote = '\"')							{ return is_delimited(str, quote, quote); }

		void			to_upper(xuchars& str);
		void			to_lower(xuchars& str);

		bool			starts_with(xcuchars& str, uchar32 start);
		bool			starts_with(xcuchars& str, xcuchars& start);

		bool			ends_with(xcuchars& str, uchar32 end_char);
		bool			ends_with(xcuchars& str, xcuchars& end);

		uchar32			first_char(xcuchars& str);
		uchar32			last_char(xcuchars& str);
		
		s32				cprintf(xcuchars& format, X_VA_ARGS_16_DEF);
		s32				vcprintf(xcuchars& format, const x_va_list& args);

		void			sprintf(xuchars& dst, xcuchars& format, X_VA_ARGS_16_DEF);
		void			vsprintf(xuchars& dst, xcuchars& format, const x_va_list& args);

		void			printf(xcuchars& str);
		void			printf(xcuchars& format, X_VA_ARGS_16_DEF);
		void			printf(xcuchars& format, const x_va_list& args);

		void			sscanf(xcuchars& str, xcuchars& format, X_VA_R_ARGS_16_DEF);
		void			vsscanf(xcuchars& str, xcuchars& format, const x_va_r_list& vr_args);

	}; ///< end of ascii namespace

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif    ///< __X_STRING_STD_H__
