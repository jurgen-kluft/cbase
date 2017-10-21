#ifndef __XBASE_STRING_UTF_H__
#define __XBASE_STRING_UTF_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================


namespace xcore
{
	// NOTE:
	//   UTF string manipulation is the easiest with UTF-32 since it is the only UTF format that
	//   has a fixed rune size and thus is easier to index directly than UTF-8 or UTF-16.
	// 

	// ASCII, UTF-8, UTF-16, UTF-32 conversions
	namespace utf
	{
		uchar   const *	read	(uchar   const * str, uchar32& out_c);
		uchar8  const *	read	(uchar8  const * str, uchar32& out_c);
		uchar16 const *	read	(uchar16 const * str, uchar32& out_c);
		uchar32 const *	read	(uchar32 const * str, uchar32& out_c);

		uchar   *		write	(uchar32 c, uchar   * str);
		uchar8  *		write	(uchar32 c, uchar8  * str);
		uchar16 *		write	(uchar32 c, uchar16 * str);
		uchar32 *		write	(uchar32 c, uchar32 * str);

		bool			is_eos	(uchar   const * str);
		bool			is_eos	(uchar8  const * str);
		bool			is_eos	(uchar16 const * str);
		bool			is_eos	(uchar32 const * str);

		bool			is_crln	(uchar   const * ustr);
		bool			is_crln	(uchar8  const * ustr);
		bool			is_crln	(uchar16 const * ustr);
		bool			is_crln	(uchar32 const * ustr);
	}	// utf


	namespace utf32
	{
		typedef			uchar32					rune;
		typedef			uchar32 *				prune;
		typedef			const uchar32 *			pcrune;

		static pcrune	len(pcrune str);
		static pcrune	len(pcrune str, s32* str_len);
		static pcrune	len(pcrune str, pcrune str_eos, s32* str_len);

		static prune	copy(prune dest, pcrune dest_end, pcrune src, pcrune src_end);

		static pcrune	find(pcrune str, pcrune str_end, pcrune find, pcrune find_end, ECmpMode mode = CASE_SENSITIVE);		/// Return position of first occurrence of <inString> or -1 if not found
		static pcrune	find(pcrune str_begin, pcrune str_end, pcrune find, pcrune find_end);
		static pcrune	find_one_of(pcrune str, pcrune str_end, pcrune set, pcrune set_end);								/// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		static prune	replace(prune str_begin, prune& str_end, pcrune str_eos, pcrune replace_str, pcrune replace_end);

		enum ECmpMode { CASE_SENSITIVE, CASE_IGNORE };
		static s32  	compare(pcrune str1, pcrune str1_end, pcrune str2, pcrune str2_end, ECmpMode mode = CASE_SENSITIVE);

		static prune	concatenate(prune dst, pcrune dst_end, pcrune dst_eos, pcrune src, pcrune src_end);		// Concatenate strings, dst = dst + src

		static s32		parse(pcrune str, pcrune str_end, bool& value);
		static s32		parse(pcrune str, pcrune str_end, s32& value, s32 base = 10);
		static s32		parse(pcrune str, pcrune str_end, s64& value, s32 base = 10);
		static s32		parse(pcrune str, pcrune str_end, f32& value);
		static s32		parse(pcrune str, pcrune str_end, f64& value);

		static bool		is_decimal(pcrune str, pcrune str_end);
		static bool		is_hexadecimal(pcrune str, pcrune str_end);
		static bool		is_float(pcrune str, pcrune str_end);
		static bool		is_GUID(pcrune str, pcrune str_end);

		static prune	to_string(prune str, prune str_end, pcrune str_eos, s32 val, s32 base = 10);
		static prune	to_string(prune str, prune str_end, pcrune str_eos, u32 val, s32 base = 10);
		static prune	to_string(prune str, prune str_end, pcrune str_eos, s64 val, s32 base = 10);
		static prune	to_string(prune str, prune str_end, pcrune str_eos, u64 val, s32 base = 10);
		static prune	to_string(prune str, prune str_end, pcrune str_eos, f32 val, s32 num_fractional_digits = 2);
		static prune	to_string(prune str, prune str_end, pcrune str_eos, f64 val, s32 num_fractional_digits = 2);

		static bool		is_space(rune c) { return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
		static bool		is_upper(rune c) { return((c >= 'A') && (c <= 'Z')); }
		static bool		is_lower(rune c) { return((c >= 'a') && (c <= 'z')); }
		static bool		is_alpha(rune c) { return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
		static bool		is_digit(rune c) { return((c >= '0') && (c <= '9')); }
		static bool		is_number(rune c) { return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

		static rune		to_upper(rune c) { return((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
		static rune		to_lower(rune c) { return((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
		static u32		to_digit(rune c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
		static u32		to_number(rune c) { if (is_digit(c)) return to_digit(c); else if (c >= 'A' && c <= 'F') return(c - 'A' + 10); else if (c >= 'a' && c <= 'f') return(c - 'a' + 10); else return(c); }
		static bool		is_equal(rune a, rune b, ECmpMode mode = CASE_SENSITIVE) { if (mode == CASE_IGNORE) { a = to_lower(a); b = to_lower(b); } return (a == b); }

		static bool		is_upper(pcrune str, pcrune str_end);											/// Check if string is all upper case
		static bool		is_lower(pcrune str, pcrune str_end);											/// Check if string is all lower case
		static bool		is_capitalized(pcrune str, pcrune str_end);										/// Check if string is capitalized ("Shanghai")
		static bool		is_delimited(pcrune str, pcrune str_end, rune delimit_left = '\"', rune delimit_right = '\"');		///< Check if string is enclosed between left and right delimiter
		static bool		is_quoted(pcrune str, pcrune str_end, rune quote = '\"') { return is_delimited(str, str_end, quote, quote); }

		static prune	to_upper(prune str, pcrune str_end);
		static prune	to_lower(prune str, pcrune str_end);

		static bool		starts_with(pcrune str, pcrune str_end, uchar32 start_char);
		static bool		starts_with(pcrune str, pcrune str_end, pcrune start_str, pcrune start_str_end);

		static bool		ends_with(pcrune str, pcrune str_end, uchar32 end_char);
		static bool		ends_with(pcrune srcstr, pcrune srcstr_end, pcrune endstr, pcrune endstr_end);

		static uchar32	first_char(pcrune str, pcrune str_end);
		static uchar32	last_char(pcrune str, pcrune str_end);

		static s32		cprintf(pcrune format_str, pcrune format_str_end, X_VA_ARGS_16_DEF);
		static prune	sprintf(prune dst_str, prune dst_str_end, pcrune format_str, pcrune format_str_end, X_VA_ARGS_16_DEF);

		static s32		vcprintf(pcrune format_str, pcrune format_str_end, const x_va_list& args);
		static s32		vsprintf(prune dst_str, prune dst_str_end, pcrune format_str, pcrune format_str_end, const x_va_list& args);

		static s32		printf(pcrune format_str, pcrune format_str_end, X_VA_ARGS_16_DEF);
		static s32		printf(pcrune format_str, pcrune format_str_end, const x_va_list& args);
		static s32		printf(pcrune str, pcrune str_end);

		static s32		sscanf(pcrune str, pcrune str_end, pcrune fmt_str, pcrune fmt_str_end, X_VA_R_ARGS_16_DEF);
		static s32		vsscanf(pcrune str, pcrune str_end, pcrune fmt_str, pcrune fmt_str_end, const x_va_r_list& vr_args);

	} ///< end of utf32 namespace

};	// xCore namespace

#endif    ///< __XBASE_STRING_UTF_H__
