#ifndef __XBASE_STRING_STD_H__
#define __XBASE_STRING_STD_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_chars.h"
#include "xbase/x_va_list.h"

namespace xcore
{
	namespace ascii
	{
		typedef			uchar					rune;
		typedef			uchar *					prune;
		typedef			const uchar*			pcrune;
		typedef			xuchars					runes;
		typedef			xcuchars				crunes;

		inline bool		has_fixed_size_rune() { return true; }
		inline s32		get_fixed_sizeof_rune() { return sizeof(rune); }

		enum ECopyType { NONE = 0, COPY_AND_WRITE_TERMINATOR = 1, COPY_AND_WRITE_MATCHING_TERMINATOR = 2 };
		enum ECmpMode { CASE_SENSITIVE, CASE_IGNORE };

		void			copy(runes& dst, crunes const& src, ECopyType type = COPY_AND_WRITE_MATCHING_TERMINATOR);
		void			concatenate(runes& dst, crunes const& src);		// Concatenate strings, dst = dst + src

		runes			find(runes const& str, crunes const& find, ECmpMode mode = CASE_SENSITIVE);			// Return position of first occurrence of <inString> or -1 if not found
		runes			find_one_of(runes const& str, crunes const& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		crunes			find(crunes const& str, crunes const& find, ECmpMode mode = CASE_SENSITIVE);		// Return position of first occurrence of <inString> or -1 if not found
		crunes			find_one_of(crunes const& str, crunes const& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found
		
		void			replace(runes& str, crunes const& replace);

		s32  			compare(crunes const& str1, crunes const& str2, ECmpMode mode = CASE_SENSITIVE);

		crunes			parse(crunes const& str, bool& value);
		crunes			parse(crunes const& str, s32& value, s32 base = 10);
		crunes			parse(crunes const& str, u32& value, s32 base = 10);
		crunes			parse(crunes const& str, s64& value, s32 base = 10);
		crunes			parse(crunes const& str, u64& value, s32 base = 10);
		crunes			parse(crunes const& str, f32& value);
		crunes			parse(crunes const& str, f64& value);

		bool			is_decimal(crunes const& str);
		bool			is_hexadecimal(crunes const& str, bool with_prefix=false);
		bool			is_float(crunes const& str);
		bool			is_GUID (crunes const& str);

		void			to_string(runes& str, s32 val, s32 base = 10);
		void			to_string(runes& str, u32 val, s32 base = 10);
		void			to_string(runes& str, s64 val, s32 base = 10);
		void			to_string(runes& str, u64 val, s32 base = 10);
		void			to_string(runes& str, f32 val, s32 num_fractional_digits = 4);
		void			to_string(runes& str, f64 val, s32 num_fractional_digits = 4);

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

		bool			is_upper(crunes const& str);											/// Check if string is all upper case
		bool			is_lower(crunes const& str);											/// Check if string is all lower case
		bool			is_capitalized(crunes const& str);										/// Check if string is capitalized ("Shanghai")
		bool			is_delimited(crunes const& str, rune delimit_left = '\"', rune delimit_right = '\"');		///< Check if string is enclosed between left and right delimiter
		inline bool		is_quoted(crunes const& str, rune quote = '\"')							{ return is_delimited(str, quote, quote); }

		void			to_upper(runes& str);
		void			to_lower(runes& str);

		bool			starts_with(crunes const& str, uchar32 start);
		bool			starts_with(crunes const& str, crunes const& start);

		bool			ends_with(crunes const& str, uchar32 end_char);
		bool			ends_with(crunes const& str, crunes const& end);

		uchar32			first_char(crunes const& str);
		uchar32			last_char(crunes const& str);
		
		s32				cprintf(crunes const& format, X_VA_ARGS_16_DEF);
		s32				vcprintf(crunes const& format, const x_va_list& args);

		void			sprintf(runes& dst, crunes const& format, X_VA_ARGS_16_DEF);
		void			vsprintf(runes& dst, crunes const& format, const x_va_list& args);

		void			printf(crunes const& str);
		void			printf(crunes const& format, X_VA_ARGS_16_DEF);
		void			printf(crunes const& format, const x_va_list& args);

		void			sscanf(crunes const& str, crunes const& format, X_VA_R_ARGS_16_DEF);
		void			vsscanf(crunes const& str, crunes const& format, const x_va_r_list& vr_args);

	}; ///< end of ascii namespace

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif    ///< __X_STRING_STD_H__
