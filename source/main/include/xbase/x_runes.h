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
	#undef printf

	namespace ascii
	{
		typedef			char					rune;
		typedef			char *					prune;
		typedef			const char*				pcrune;
		struct runes
		{
			inline			runes() : m_str(NULL), m_end(NULL), m_eos(NULL) {}
			inline			runes(prune _str, prune _end, pcrune _eos) : m_str(_str), m_end(_end), m_eos(_eos) {}
			inline			runes(runes const& other) : m_str(other.m_str), m_end(other.m_end), m_eos(other.m_eos) {}
			u32				size() const { return (u32)(m_end - m_str); }
			bool			is_empty() const { return m_str == m_end; }
			runes 			operator = (runes const& other) { m_str = other.m_str; m_end = other.m_end; m_eos = other.m_eos; return *this; }
			prune			m_str;
			prune			m_end;
			pcrune			m_eos;
		};
		struct crunes
		{
			inline			crunes() : m_str(NULL), m_end(NULL) {}
			inline			crunes(pcrune _str) : m_str(_str), m_end(NULL) {}
			inline			crunes(pcrune _str, pcrune _end) : m_str(_str), m_end(_end) {}
			inline			crunes(runes const& other) : m_str(other.m_str), m_end(other.m_end) {}
			inline			crunes(crunes const& other) : m_str(other.m_str), m_end(other.m_end) {}
			u32				size() const { return (u32)(m_end - m_str); }
			bool			is_empty() const { return m_str == m_end; }
			crunes &		operator = (crunes const& other) { m_str = other.m_str; m_end = other.m_end; return *this; }
			pcrune			m_str;
			pcrune 			m_end;
		};

		template<s32 L>
		class runez : public runes
		{
		public:
			enum { SIZE = L + 1 };
			rune			m_str[SIZE];
			inline			runez() : runes(m_str, m_str, &m_str[SIZE - 1]) {}
			void			reset() { m_end = m_str; m_str[0] = '\0'; }
		};

		class alloc
		{
		public:
			virtual runes  allocate(s32 len, s32 cap) = 0;
			virtual void   deallocate(runes& slice) = 0;
		};		
	}

	namespace utf8
	{
		typedef			uchar8					rune;
		typedef			uchar8 *				prune;
		typedef			const uchar8 *			pcrune;
		struct runes
		{
			inline			runes() : m_str(NULL), m_end(NULL), m_eos(NULL) {}
			inline			runes(prune _str, prune _end, pcrune _eos) : m_str(_str), m_end(_end), m_eos(_eos) {}
			inline			runes(runes const& other) : m_str(other.m_str), m_end(other.m_end), m_eos(other.m_eos) {}
			u32				size() const { return (u32)(m_end - m_str); }
			bool			is_empty() const { return m_str == m_end; }
			runes &			operator = (runes const& other) { m_str = other.m_str; m_end = other.m_end; m_eos = other.m_eos; return *this; }
			prune			m_str;
			prune			m_end;
			pcrune			m_eos;
		};
		struct crunes
		{
			inline			crunes() : m_str(NULL), m_end(NULL) {}
			inline			crunes(pcrune _str, pcrune _end) : m_str(_str), m_end(_end) {}
			inline			crunes(runes const& other) : m_str(other.m_str), m_end(other.m_end) {}
			inline			crunes(crunes const& other) : m_str(other.m_str), m_end(other.m_end) {}
			u32				size() const { return (u32)(m_end - m_str); }
			bool			is_empty() const { return m_str == m_end; }
			crunes &		operator = (crunes const& other) { m_str = other.m_str; m_end = other.m_end; return *this; }
			pcrune			m_str;
			pcrune 			m_end;
		};
		template<s32 L>
		class runez : public runes
		{
		public:
			enum { SIZE = L + 1 };
			rune			m_str[SIZE];
			inline			runez() : runes(m_str, m_str, &m_str[SIZE - 1]) {}
		};

		class alloc
		{
		public:
			virtual runes  allocate(s32 len, s32 cap) = 0;
			virtual void   deallocate(runes& slice) = 0;
		};		
	}

	namespace utf16
	{
		typedef			uchar16					rune;
		typedef			uchar16 *				prune;
		typedef			const uchar16 *			pcrune;
		struct runes
		{
			inline			runes() : m_str(NULL), m_end(NULL), m_eos(NULL) {}
			inline			runes(prune _str, prune _end, pcrune _eos) : m_str(_str), m_end(_end), m_eos(_eos) {}
			inline			runes(runes const& other) : m_str(other.m_str), m_end(other.m_end), m_eos(other.m_eos) {}
			u32				size() const { return (u32)(m_end - m_str); }
			bool			is_empty() const { return m_str == m_end; }
			runes &			operator = (runes const& other) { m_str = other.m_str; m_end = other.m_end; m_eos = other.m_eos; return *this; }
			prune			m_str;
			prune			m_end;
			pcrune			m_eos;
		};
		struct crunes
		{
			inline			crunes() : m_str(NULL), m_end(NULL) {}
			inline			crunes(pcrune _str, pcrune _end) : m_str(_str), m_end(_end) {}
			inline			crunes(runes const& other) : m_str(other.m_str), m_end(other.m_end) {}
			inline			crunes(crunes const& other) : m_str(other.m_str), m_end(other.m_end) {}
			u32				size() const { return (u32)(m_end - m_str); }
			bool			is_empty() const { return m_str == m_end; }
			crunes &		operator = (crunes const& other) { m_str = other.m_str; m_end = other.m_end; return *this; }
			pcrune			m_str;
			pcrune 			m_end;
		};
		template<s32 L>
		class runez : public runes
		{
		public:
			enum { SIZE = L + 1 };
			rune			m_str[SIZE];
			inline			runez() : runes(m_str, m_str, &m_str[SIZE - 1]) {}
		};

		class alloc
		{
		public:
			virtual runes  allocate(s32 len, s32 cap) = 0;
			virtual void   deallocate(runes& slice) = 0;
		};		
	}

	namespace utf32
	{
		typedef			uchar32					rune;
		typedef			uchar32 *				prune;
		typedef			const uchar32 *			pcrune;
		struct runes
		{
			inline			runes() : m_str(NULL), m_end(NULL), m_eos(NULL) {}
			inline			runes(prune _str, prune _end, pcrune _eos) : m_str(_str), m_end(_end), m_eos(_eos) {}
			inline			runes(runes const& other) : m_str(other.m_str), m_end(other.m_end), m_eos(other.m_eos) {}
			u32				size() const { return (u32)(m_end - m_str); }
			bool			is_empty() const { return m_str == m_end; }
			runes &			operator = (runes const& other) { m_str = other.m_str; m_end = other.m_end; m_eos = other.m_eos; return *this; }
			prune			m_str;
			prune			m_end;
			pcrune			m_eos;
		};
		struct crunes
		{
			inline			crunes() : m_str(NULL), m_end(NULL) {}
			inline			crunes(pcrune _str, pcrune _end) : m_str(_str), m_end(_end) {}
			inline			crunes(runes const& other) : m_str(other.m_str), m_end(other.m_end) {}
			inline			crunes(crunes const& other) : m_str(other.m_str), m_end(other.m_end) {}
			u32				size() const { return (u32)(m_end - m_str); }
			bool			is_empty() const { return m_str == m_end; }
			crunes &		operator = (crunes const& other) { m_str = other.m_str; m_end = other.m_end; return *this; }
			pcrune			m_str;
			pcrune			m_end;
		};
		template<s32 L>
		class runez : public runes
		{
		public:
			enum { SIZE = L + 1 };
			rune			m_str[SIZE];
			inline			runez() : runes(m_str, m_str, &m_str[SIZE - 1]) {}
		};

		class alloc
		{
		public:
			virtual runes  allocate(s32 len, s32 cap) = 0;
			virtual void   deallocate(runes& slice) = 0;
		};		
	}
}

namespace xcore
{
	// NOTE:
	//   UTF string manipulation is the easiest with UTF-32 since it is the only UTF format that
	//   has a fixed rune size and thus is easier to index directly than UTF-8 or UTF-16.
	// 

	// ASCII, UTF-8, UTF-16, UTF-32 conversions
	namespace utf
	{
		enum ETermType { TERMINATOR_NONE = 0, TERMINATOR_WRITE = 1 };

		uchar32			peek(ascii::crunes const & str);
		uchar32			peek(utf8::crunes const & str);
		uchar32			peek(utf32::crunes const & str);

		uchar32			read(ascii::rune const* & str, ascii::rune const * end);
		uchar32			read(utf8::rune  const* & str, utf8::rune  const * end);
		uchar32			read(utf32::rune const* & str, utf32::rune const * end);

		uchar32			read(ascii::rune * & str, ascii::rune const * end);
		uchar32			read(utf8::rune  * & str, utf8::rune  const * end);
		uchar32			read(utf32::rune * & str, utf32::rune const * end);

		uchar32			read(uchar16*& str, const uchar16* end);

		uchar32			read(ascii::crunes& str);
		uchar32			read(utf8::crunes & str);
		uchar32			read(utf32::crunes& str);

		void			write(uchar32 c, uchar*  & str, const uchar  * end);
		void			write(uchar32 c, uchar8* & str, const uchar8 * end);
		void			write(uchar32 c, uchar16*& str, const uchar16* end);
		void			write(uchar32 c, uchar32*& str, const uchar32* end);

		bool			write(uchar32 c, ascii::runes & str);
		bool			write(uchar32 c, utf8::runes  & str);
		bool			write(uchar32 c, utf32::runes & str);

		void			copy(ascii::crunes const & sstr, ascii::runes& dstr, ETermType term_type = TERMINATOR_WRITE);
		void			copy(utf32::crunes const & sstr, ascii::runes& dstr, ETermType term_type = TERMINATOR_WRITE);

		void			copy(ascii::crunes const & sstr, utf32::runes& dstr, ETermType term_type = TERMINATOR_WRITE);
		void			copy(utf32::crunes const & sstr, utf32::runes& dstr, ETermType term_type = TERMINATOR_WRITE);

		s32				size(uchar32 c);

		bool			is_eos(uchar   const* str);
		bool			is_eos(uchar8  const* str);
		bool			is_eos(uchar16 const* str);
		bool			is_eos(uchar32 const* str);

		bool			can_read(ascii::runes const& str);
		bool			can_read(utf8::runes const& str);
		bool			can_read(utf32::runes const& str);
		bool			can_read(ascii::crunes const& str);
		bool			can_read(utf8::crunes const& str);
		bool			can_read(utf32::crunes const& str);

		bool			can_write(ascii::runes const& str);
		bool			can_write(utf8::runes const& str);
		bool			can_write(utf32::runes const& str);

		bool			read_is_crln(ascii::runes const& str);
		bool			read_is_crln(utf8::runes  const& str);
		bool			read_is_crln(utf32::runes const& str);
		bool			read_is_crln(ascii::crunes const& str);
		bool			read_is_crln(utf8::crunes  const& str);
		bool			read_is_crln(utf32::crunes const& str);
	};	// utf


	namespace ascii
	{
		inline bool		has_fixed_size_rune() { return true; }
		inline s32		get_fixed_sizeof_rune() { return sizeof(rune); }

		s32				len(prune str, prune end);
		s32				len(pcrune str, pcrune end);

		prune			endof(prune str, prune end);
		pcrune			endof(pcrune str, pcrune end);

		inline u32		size(runes const& str) { return len(str.m_str, str.m_end); }
		inline u32		size(crunes const& str) { return len(str.m_str, str.m_end); }

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

		void			replace(runes& str, crunes const& find, crunes const& replace);

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

		uchar32			first_char(crunes const& str);
		inline uchar32	first_char(runes const& str) { return first_char(str); }

		runes			select(runes const& str, u32 from, u32 to);
		crunes			select(crunes const& str, u32 from, u32 to);
	}; ///< end of ascii namespace

	namespace utf8
	{
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
	}; ///< end of utf8 namespace

	namespace utf32
	{
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
	}; ///< end of utf32 namespace


	inline bool		operator == (ascii::runes const& r1, ascii::runes const& r2) { return ascii::compare(r1, r2) == 0; }
	inline bool		operator == (ascii::crunes const& r1, ascii::crunes const& r2) { return ascii::compare(r1, r2) == 0; }

};	// xCore namespace

#endif    ///< __XBASE_STRING_UTF_H__
