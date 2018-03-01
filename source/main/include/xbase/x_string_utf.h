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
#include "xbase/x_chars.h"

namespace xcore
{
	#undef printf

	// NOTE:
	//   UTF string manipulation is the easiest with UTF-32 since it is the only UTF format that
	//   has a fixed rune size and thus is easier to index directly than UTF-8 or UTF-16.
	// 

	// ASCII, UTF-8, UTF-16, UTF-32 conversions
	namespace utf
	{
		enum ETermType { TERMINATOR_NONE = 0, TERMINATOR_WRITE = 1 };

		uchar32			peek(xcuchars const & str);
		uchar32			peek(xcuchar8s const & str);
		uchar32			peek(xcuchar32s const & str);

		uchar32			read(const uchar*  & str, const uchar* end);
		uchar32			read(const uchar8* & str, const uchar8* end);
		uchar32			read(const uchar32*& str, const uchar32* end);

		uchar32			read(uchar*  & str, const uchar* end);
		uchar32			read(uchar8* & str, const uchar8* end);
		uchar32			read(uchar32*& str, const uchar32* end);

		uchar32			read(uchar16*& str, const uchar16* end);

		uchar32			read(xcuchars & str);
		uchar32			read(xcuchar8s & str);
		uchar32			read(xcuchar32s & str);

		void			write(uchar32 c, uchar*  & str, const uchar  * end);
		void			write(uchar32 c, uchar8* & str, const uchar8 * end);
		void			write(uchar32 c, uchar16*& str, const uchar16* end);
		void			write(uchar32 c, uchar32*& str, const uchar32* end);

		bool			write(uchar32 c, xuchars  & str);
		bool			write(uchar32 c, xuchar8s & str);
		bool			write(uchar32 c, xuchar32s& str);

		void			copy(xcuchars   const & sstr, xuchars& dstr, ETermType term_type = TERMINATOR_WRITE);
		void			copy(xcuchar32s const & sstr, xuchars& dstr, ETermType term_type = TERMINATOR_WRITE);

		void			copy(xcuchars   const & sstr, xuchar32s& dstr, ETermType term_type = TERMINATOR_WRITE);
		void			copy(xcuchar32s const & sstr, xuchar32s& dstr, ETermType term_type = TERMINATOR_WRITE);

		s32				size(uchar32 c);

		bool			is_eos(uchar   const* str);
		bool			is_eos(uchar8  const* str);
		bool			is_eos(uchar16 const* str);
		bool			is_eos(uchar32 const* str);

		bool			can_read(xuchars   const& str);
		bool			can_read(xuchar8s  const& str);
		bool			can_read(xuchar32s const& str);
		bool			can_read(xcuchars   const& str);
		bool			can_read(xcuchar8s  const& str);
		bool			can_read(xcuchar32s const& str);

		bool			can_write(xuchars   const& str);
		bool			can_write(xuchar8s  const& str);
		bool			can_write(xuchar32s const& str);

		bool			read_is_crln(xuchars   const& str);
		bool			read_is_crln(xuchar8s  const& str);
		bool			read_is_crln(xuchar32s const& str);
		bool			read_is_crln(xcuchars   const& str);
		bool			read_is_crln(xcuchar8s  const& str);
		bool			read_is_crln(xcuchar32s const& str);
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

		s32				len(prune str, prune end);
		s32				len(pcrune str, pcrune end);

		prune			endof(prune str, prune end);
		pcrune			endof(pcrune str, pcrune end);

		enum ECopyType { NONE = 0, COPY_AND_WRITE_TERMINATOR = 1, COPY_AND_WRITE_MATCHING_TERMINATOR = 2 };
		enum ECmpMode { CASE_SENSITIVE, CASE_IGNORE };

		void			copy(runes& dst, crunes const& src, ECopyType type = COPY_AND_WRITE_MATCHING_TERMINATOR);
		void			concatenate(runes& dst, crunes const& src);		// Concatenate strings, dst = dst + src

		runes			find(runes const& _str, uchar32 _c, ECmpMode mode = CASE_SENSITIVE);
		runes			find(runes const& str, crunes const& find, ECmpMode mode = CASE_SENSITIVE);			// Return position of first occurrence of <inString> or -1 if not found
		runes			find_one_of(runes const& str, crunes const& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		crunes			find(crunes const& _str, uchar32 _c, ECmpMode mode = CASE_SENSITIVE);
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
		bool			is_hexadecimal(crunes const& str, bool with_prefix = false);
		bool			is_float(crunes const& str);
		bool			is_GUID(crunes const& str);

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
		inline bool		is_quoted(crunes const& str, rune quote = '\"') { return is_delimited(str, quote, quote); }

		void			to_upper(runes& str);
		void			to_lower(runes& str);

		bool			starts_with(crunes const& str, uchar32 start);
		bool			starts_with(crunes const& str, crunes const& start);

		bool			ends_with(crunes const& str, uchar32 end_char);
		bool			ends_with(crunes const& str, crunes const& end);

		uchar32			first_char(crunes const& str);
		uchar32			last_char(crunes const& str);
		inline uchar32	first_char(runes const& str) { return first_char(str); }
		inline uchar32	last_char(runes const& str) { return last_char(str); }

		s32				cprintf(crunes const& format, X_VA_ARGS_16_DEF);
		s32				vcprintf(crunes const& format, const x_va_list& args);

		void			sprintf(runes& dst, crunes const& format, X_VA_ARGS_16_DEF);
		void			vsprintf(runes& dst, crunes const& format, const x_va_list& args);

		void			printf(crunes const& str);
		void			printf(crunes const& format, X_VA_ARGS_16_DEF);
		void			printf(crunes const& format, const x_va_list& args);

		s32				sscanf(crunes & str, crunes const& format, X_VA_R_ARGS_16_DEF);
		s32				vsscanf(crunes & str, crunes const& format, const x_va_r_list& vr_args);


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

		s32				len(prune str, prune end);
		s32				len(pcrune str, pcrune end);

		prune			endof(prune str, prune end);
		pcrune			endof(pcrune str, pcrune end);

		enum ECopyType { NONE = 0, COPY_AND_WRITE_TERMINATOR = 1, COPY_AND_WRITE_MATCHING_TERMINATOR = 2 };
		enum ECmpMode { CASE_SENSITIVE, CASE_IGNORE };

		void			copy(runes& dst, crunes const& src, ECopyType type = COPY_AND_WRITE_MATCHING_TERMINATOR);
		void			concatenate(runes& dst, crunes const& src);		// Concatenate strings, dst = dst + src

		runes			find(runes const& _str, uchar32 _c, ECmpMode mode = CASE_SENSITIVE);
		runes			find(runes const& str, crunes const& find, ECmpMode mode = CASE_SENSITIVE);			// Return position of first occurrence of <inString> or -1 if not found
		runes			find_one_of(runes const& str, crunes const& set, ECmpMode mode = CASE_SENSITIVE);	// Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		crunes			find(crunes const& _str, uchar32 _c, ECmpMode mode = CASE_SENSITIVE);
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
		bool			is_hexadecimal(crunes const& str, bool with_prefix = false);
		bool			is_float(crunes const& str);
		bool			is_GUID(crunes const& str);

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
		inline bool		is_quoted(crunes const& str, rune quote = '\"') { return is_delimited(str, quote, quote); }

		void			to_upper(runes& str);
		void			to_lower(runes& str);

		bool			starts_with(crunes const& str, uchar32 start);
		bool			starts_with(crunes const& str, crunes const& start);

		bool			ends_with(crunes const& str, uchar32 end_char);
		bool			ends_with(crunes const& str, crunes const& end);

		uchar32			first_char(crunes const& str);
		uchar32			last_char(crunes const& str);
		inline uchar32	first_char(runes const& str) { return first_char(str); }
		inline uchar32	last_char(runes const& str) { return last_char(str); }

		s32				cprintf(crunes const& format, X_VA_ARGS_16_DEF);
		s32				vcprintf(crunes const& format, const x_va_list& args);

		void			sprintf(runes& dst, crunes const& format, X_VA_ARGS_16_DEF);
		void			vsprintf(runes& dst, crunes const& format, const x_va_list& args);

		void			printf(crunes const& str);
		void			printf(crunes const& format, X_VA_ARGS_16_DEF);
		void			printf(crunes const& format, const x_va_list& args);

		s32				sscanf(crunes & str, crunes const& format, X_VA_R_ARGS_16_DEF);
		s32				vsscanf(crunes & str, crunes const& format, const x_va_r_list& vr_args);


	}; ///< end of utf32 namespace

};	// xCore namespace

#endif    ///< __XBASE_STRING_UTF_H__
