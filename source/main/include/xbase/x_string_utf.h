#ifndef __XBASE_STRING_UTF_H__
#define __XBASE_STRING_UTF_H__
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
	class xuchars;
	class xuchar8s;
	class xuchar32s;

	class xcuchars;
	class xcuchar8s;
	class xcuchar32s;

	// NOTE:
	//   UTF string manipulation is the easiest with UTF-32 since it is the only UTF format that
	//   has a fixed rune size and thus is easier to index directly than UTF-8 or UTF-16.
	// 

	// ASCII, UTF-8, UTF-16, UTF-32 conversions
	namespace utf
	{
		enum ETermType { TERMINATOR_NONE = 0, TERMINATOR_WRITE = 1 };

		uchar32			peek(xcuchars const & str);
		uchar32			peek(xcuchar32s const & str);

		uchar32			read(xcuchars & str);
		uchar32			read(xcuchar32s & str);

		bool			write(uchar32 c, xuchars& str);
		bool			write(uchar32 c, xuchar32s& str);

		void			copy(xcuchars   const & sstr, xuchars& dstr, ETermType term_type = TERMINATOR_WRITE);
		void			copy(xcuchar32s const & sstr, xuchars& dstr, ETermType term_type = TERMINATOR_WRITE);

		void			copy(xcuchars   const & sstr, xuchar32s& dstr, ETermType term_type = TERMINATOR_WRITE);
		void			copy(xcuchar32s const & sstr, xuchar32s& dstr, ETermType term_type = TERMINATOR_WRITE);

		s32				size(uchar32 c);

		bool			is_eos(xuchars   const& str);
		bool			is_eos(xuchar32s const& str);
		bool			is_eos(xcuchars   const& str);
		bool			is_eos(xcuchar32s const& str);

		bool			is_crln(xuchars   const& str);
		bool			is_crln(xuchar32s const& str);
		bool			is_crln(xcuchars   const& str);
		bool			is_crln(xcuchar32s const& str);
	};	// utf

	namespace utf8
	{
		typedef			uchar8					rune;
		typedef			uchar8 *				prune;
		typedef			const uchar8 *			pcrune;
		typedef			xuchar8s				runes;
		typedef			xcuchar8s				crunes;

		inline bool		has_fixed_size_rune() { return true; }
		inline s32		get_fixed_sizeof_rune() { return sizeof(rune); }

		enum ECopyType { NONE = 0, COPY_AND_WRITE_TERMINATOR = 1, COPY_AND_WRITE_MATCHING_TERMINATOR = 2 };
		enum ECmpMode { CASE_SENSITIVE, CASE_IGNORE };

		void			copy(runes& dst, crunes const& src, ECopyType type = COPY_AND_WRITE_MATCHING_TERMINATOR);
		void			concatenate(runes& dst, crunes& src);		// Concatenate strings, dst = dst + src

		runes			find(runes& str, crunes& find, ECmpMode mode = CASE_SENSITIVE);			// Return position of first occurrence of <inString> or -1 if not found
		runes			find_one_of(runes& str, crunes& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		crunes			find(crunes& str, crunes& find, ECmpMode mode = CASE_SENSITIVE);		// Return position of first occurrence of <inString> or -1 if not found
		crunes			find_one_of(crunes& str, crunes& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		void			replace(runes& str, crunes& replace);

		s32  			compare(crunes& str1, crunes& str2, ECmpMode mode = CASE_SENSITIVE);

		crunes			parse(crunes& str, bool& value);
		crunes			parse(crunes& str, s32& value, s32 base = 10);
		crunes			parse(crunes& str, u32& value, s32 base = 10);
		crunes			parse(crunes& str, s64& value, s32 base = 10);
		crunes			parse(crunes& str, u64& value, s32 base = 10);
		crunes			parse(crunes& str, f32& value);
		crunes			parse(crunes& str, f64& value);

		bool			is_decimal(crunes& str);
		bool			is_hexadecimal(crunes& str, bool with_prefix = false);
		bool			is_float(crunes& str);
		bool			is_GUID(crunes& str);

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

		bool			is_upper(crunes& str);											/// Check if string is all upper case
		bool			is_lower(crunes& str);											/// Check if string is all lower case
		bool			is_capitalized(crunes& str);										/// Check if string is capitalized ("Shanghai")
		bool			is_delimited(crunes& str, rune delimit_left = '\"', rune delimit_right = '\"');		///< Check if string is enclosed between left and right delimiter
		inline bool		is_quoted(crunes& str, rune quote = '\"') { return is_delimited(str, quote, quote); }

		void			to_upper(runes& str);
		void			to_lower(runes& str);

		bool			starts_with(crunes& str, uchar32 start);
		bool			starts_with(crunes& str, crunes& start);

		bool			ends_with(crunes& str, uchar32 end_char);
		bool			ends_with(crunes& str, crunes& end);

		uchar32			first_char(crunes& str);
		uchar32			last_char(crunes& str);

		s32				cprintf(crunes& format, X_VA_ARGS_16_DEF);
		s32				vcprintf(crunes& format, const x_va_list& args);

		void			sprintf(runes& dst, crunes& format, X_VA_ARGS_16_DEF);
		void			vsprintf(runes& dst, crunes& format, const x_va_list& args);

		void			printf(crunes& str);
		void			printf(crunes& format, X_VA_ARGS_16_DEF);
		void			printf(crunes& format, const x_va_list& args);

		void			sscanf(crunes& str, crunes& format, X_VA_R_ARGS_16_DEF);
		void			vsscanf(crunes& str, crunes& format, const x_va_r_list& vr_args);
	}; ///< end of utf8 namespace

	namespace utf32
	{
		typedef			uchar32					rune;
		typedef			uchar32 *				prune;
		typedef			const uchar32 *			pcrune;
		typedef			xuchar32s				runes;
		typedef			xcuchar32s				crunes;

		inline bool		has_fixed_size_rune() { return true; }
		inline s32		get_fixed_sizeof_rune() { return sizeof(rune); }

		enum ECopyType { NONE = 0, COPY_AND_WRITE_TERMINATOR = 1, COPY_AND_WRITE_MATCHING_TERMINATOR = 2 };
		enum ECmpMode { CASE_SENSITIVE, CASE_IGNORE };

		void			copy(runes& dst, crunes const& src, ECopyType type = COPY_AND_WRITE_MATCHING_TERMINATOR);
		void			concatenate(runes& dst, crunes& src);		// Concatenate strings, dst = dst + src

		runes			find(runes& str, crunes& find, ECmpMode mode = CASE_SENSITIVE);			// Return position of first occurrence of <inString> or -1 if not found
		runes			find_one_of(runes& str, crunes& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		crunes			find(crunes& str, crunes& find, ECmpMode mode = CASE_SENSITIVE);		// Return position of first occurrence of <inString> or -1 if not found
		crunes			find_one_of(crunes& str, crunes& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		void			replace(runes& str, crunes& replace);

		s32  			compare(crunes& str1, crunes& str2, ECmpMode mode = CASE_SENSITIVE);

		crunes			parse(crunes& str, bool& value);
		crunes			parse(crunes& str, s32& value, s32 base = 10);
		crunes			parse(crunes& str, u32& value, s32 base = 10);
		crunes			parse(crunes& str, s64& value, s32 base = 10);
		crunes			parse(crunes& str, u64& value, s32 base = 10);
		crunes			parse(crunes& str, f32& value);
		crunes			parse(crunes& str, f64& value);

		bool			is_decimal(crunes& str);
		bool			is_hexadecimal(crunes& str, bool with_prefix = false);
		bool			is_float(crunes& str);
		bool			is_GUID(crunes& str);

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

		bool			is_upper(crunes& str);											/// Check if string is all upper case
		bool			is_lower(crunes& str);											/// Check if string is all lower case
		bool			is_capitalized(crunes& str);										/// Check if string is capitalized ("Shanghai")
		bool			is_delimited(crunes& str, rune delimit_left = '\"', rune delimit_right = '\"');		///< Check if string is enclosed between left and right delimiter
		inline bool		is_quoted(crunes& str, rune quote = '\"') { return is_delimited(str, quote, quote); }

		void			to_upper(runes& str);
		void			to_lower(runes& str);

		bool			starts_with(crunes& str, uchar32 start);
		bool			starts_with(crunes& str, crunes& start);

		bool			ends_with(crunes& str, uchar32 end_char);
		bool			ends_with(crunes& str, crunes& end);

		uchar32			first_char(crunes& str);
		uchar32			last_char(crunes& str);

		s32				cprintf(crunes& format, X_VA_ARGS_16_DEF);
		s32				vcprintf(crunes& format, const x_va_list& args);

		void			sprintf(runes& dst, crunes& format, X_VA_ARGS_16_DEF);
		void			vsprintf(runes& dst, crunes& format, const x_va_list& args);

		void			printf(crunes& str);
		void			printf(crunes& format, X_VA_ARGS_16_DEF);
		void			printf(crunes& format, const x_va_list& args);

		void			sscanf(crunes& str, crunes& format, X_VA_R_ARGS_16_DEF);
		void			vsscanf(crunes& str, crunes& format, const x_va_r_list& vr_args);
	}; ///< end of utf32 namespace

};	// xCore namespace

#endif    ///< __XBASE_STRING_UTF_H__
