#ifndef __XBASE_RUNES_V2_H__
#define __XBASE_RUNES_V2_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
#undef printf

	namespace ascii
	{
		typedef uchar        rune;
		typedef uchar*       prune;
		typedef const uchar* pcrune;
		static const rune    TERMINATOR = '\0';
		static const s32     TYPE       = 1;
		struct runes_t
		{
			prune m_bos; // Begin of stream
			prune m_str; // Begin of string
			prune m_end; // End of string (does not necessarily point at TERMINATOR)
			prune m_eos; // End of stream, points to a TERMINATOR
		};
		struct crunes_t
		{
			pcrune m_bos; // Begin of stream
			pcrune m_str; // Begin of string
			pcrune m_end; // End of string (does not necessarily point at TERMINATOR)
			pcrune m_eos; // End of stream, points to a TERMINATOR
		};

	} // namespace ascii
	namespace utf8
	{
		typedef uchar8        rune;
		typedef uchar8*       prune;
		typedef const uchar8* pcrune;
		static const s32      TYPE       = 2;
		static const rune     TERMINATOR = '\0';

		struct runes_t
		{
			prune m_bos; // Begin of stream
			prune m_str; // Begin of string
			prune m_end; // End of string (does not necessarily point at TERMINATOR)
			prune m_eos; // End of stream, points to a TERMINATOR
		};
		struct crunes_t
		{
			pcrune m_bos; // Begin of stream
			pcrune m_str; // Begin of string
			pcrune m_end; // End of string (does not necessarily point at TERMINATOR)
			pcrune m_eos; // End of stream, points to a TERMINATOR
		};
	} // namespace utf8
	namespace utf16
	{
		typedef uchar16        rune;
		typedef uchar16*       prune;
		typedef const uchar16* pcrune;
		static const s32       TYPE       = 3;
		static const rune      TERMINATOR = '\0';

		struct runes_t
		{
			prune m_bos; // Begin of stream
			prune m_str; // Begin of string
			prune m_end; // End of string (does not necessarily point at TERMINATOR)
			prune m_eos; // End of stream, points to a TERMINATOR
		};
		struct crunes_t
		{
			pcrune m_bos; // Begin of stream
			pcrune m_str; // Begin of string
			pcrune m_end; // End of string (does not necessarily point at TERMINATOR)
			pcrune m_eos; // End of stream, points to a TERMINATOR
		};
	} // namespace utf16
	namespace utf32
	{
		typedef uchar32        rune;
		typedef uchar32*       prune;
		typedef const uchar32* pcrune;
		static const s32       TYPE       = 4;
		static const rune      TERMINATOR = '\0';

		struct runes_t
		{
			prune m_bos; // Begin of stream
			prune m_str; // Begin of string
			prune m_end; // End of string (does not necessarily point at TERMINATOR)
			prune m_eos; // End of stream, points to a TERMINATOR
		};
		struct crunes_t
		{
			pcrune m_bos; // Begin of stream
			pcrune m_str; // Begin of string
			pcrune m_end; // End of string (does not necessarily point at TERMINATOR)
			pcrune m_eos; // End of stream, points to a TERMINATOR
		};
	} // namespace utf32

	struct runes_t
	{
		runes_t();
		runes_t(runes_t const& other);
		runes_t(ascii::prune _str, ascii::prune _end, s32 _type = ascii::TYPE);
		runes_t(ascii::prune _str, ascii::prune _end, ascii::prune _eos, s32 _type = ascii::TYPE);
		runes_t(ascii::prune _bos, ascii::prune _str, ascii::prune _end, ascii::prune _eos, s32 _type = ascii::TYPE);

		runes_t(utf32::prune _str, utf32::prune _end, s32 _type = utf32::TYPE);
		runes_t(utf32::prune _str, utf32::prune _end, utf32::prune _eos, s32 _type = utf32::TYPE);
		runes_t(utf32::prune _bos, utf32::prune _str, utf32::prune _end, utf32::prune _eos, s32 _type = utf32::TYPE);

		s32  size() const;
		s32  cap() const;
		bool is_empty() const;
		bool is_valid() const;
		void reset();
		void clear();
		void term();

		void concatenate(ascii::rune c);
		void concatenate(utf32::rune c);
		void concatenate(const ascii::crunes_t& str);
		void concatenate(const utf32::crunes_t& str);

		runes_t& operator+=(const ascii::crunes_t& str)
		{
			concatenate(str);
			return *this;
		}
		runes_t& operator+=(const utf32::crunes_t& str)
		{
			concatenate(str);
			return *this;
		}
		runes_t& operator+=(ascii::rune c)
		{
			concatenate(c);
			return *this;
		}
		runes_t& operator+=(utf32::rune c)
		{
			concatenate(c);
			return *this;
		}

		runes_t& operator=(runes_t const& other);

		union urunes_t
		{
			inline urunes_t()
				: m_ascii()
			{
			}
			ascii::runes_t m_ascii;
			utf8::runes_t  m_utf8;
			utf16::runes_t m_utf16;
			utf32::runes_t m_utf32;
		};
		urunes_t m_runes;
		s32      m_type;
	};

	struct crunes_t
	{
		crunes_t();
		crunes_t(ascii::pcrune _str);
		crunes_t(ascii::pcrune _str, ascii::pcrune _end);
		crunes_t(ascii::pcrune _str, ascii::pcrune _end, ascii::pcrune _eos, s32 _type = ascii::TYPE);
		crunes_t(ascii::pcrune _bos, ascii::pcrune _str, ascii::pcrune _end, ascii::pcrune _eos, s32 _type = ascii::TYPE);

		crunes_t(utf32::pcrune _str);
		crunes_t(utf32::pcrune _str, utf32::pcrune _end);
		crunes_t(utf32::pcrune _str, utf32::pcrune _end, utf32::pcrune _eos, s32 _type = utf32::TYPE);
		crunes_t(utf32::pcrune _bos, utf32::pcrune _str, utf32::pcrune _end, utf32::pcrune _eos, s32 _type = utf32::TYPE);

		crunes_t(runes_t const& other);
		crunes_t(crunes_t const& other);

		s32  size() const;
		bool is_valid() const;
		bool is_empty() const;
		void reset();
		void clear();

		crunes_t& operator=(crunes_t const& other);

		union urunes_t
		{
			inline urunes_t()
				: m_ascii()
			{
			}
			ascii::crunes_t m_ascii;
			utf8::crunes_t  m_utf8;
			utf16::crunes_t m_utf16;
			utf32::crunes_t m_utf32;
		};
		urunes_t m_runes;
		s32      m_type;
	};

	template <typename T, s32 L> class runez_t : public runes_t
	{
	public:
		enum
		{
			SIZE = L + 1
		};
		T m_run[SIZE];
		inline runez_t()
			: runes_t(m_run, m_run, &m_run[SIZE - 1])
		{
		}
		inline runez_t(uchar32 c)
			: runes_t(m_run, &m_run[SIZE - 1])
		{
			m_run[0] = c;
			m_run[1] = 0;
			m_runes.m_ascii.m_end += sizeof(T);
		}
		inline runez_t(const char* str)
			: runes_t(m_run, m_run, &m_run[SIZE - 1])
		{
			concatenate(str);
			term();
		}
	};

	template <typename T, s32 L> class crunez_t : public crunes_t
	{
	public:
		enum
		{
			SIZE = L + 1
		};
		T m_run[SIZE];

		inline crunez_t(uchar32 c)
			: crunes_t(m_run, &m_run[SIZE - 1])
		{
			m_run[0] = c;
			m_run[1] = 0;
			m_runes.m_ascii.m_end += sizeof(T);
		}
		inline crunez_t(const char* str)
			: crunes_t(m_run, &m_run[SIZE - 1])
		{
			runes_t run(m_run, m_run, &m_run[SIZE - 1]);
			run.concatenate(str);
			run.term();
			m_end = run.m_end;
		}
	};

	class runes_alloc_t
	{
	public:
		virtual runes_t allocate(s32 len, s32 cap, s32 type = ascii::TYPE) = 0;
		virtual void deallocate(runes_t& slice) = 0;
	};

	crunes_t find(crunes_t const& _str, uchar32 _c, bool inCaseSensitive = true);
	runes_t  find(runes_t const& _str, uchar32 _c, bool inCaseSensitive = true);
	crunes_t findLast(crunes_t const& _str, uchar32 _c, bool inCaseSensitive = true);
	runes_t  findLast(runes_t const& _str, uchar32 _c, bool inCaseSensitive = true);

	crunes_t find(crunes_t const& str, crunes_t const& find, bool inCaseSensitive = true);
	runes_t  find(runes_t const& str, crunes_t const& find, bool inCaseSensitive = true);
	crunes_t findLast(crunes_t const& str, crunes_t const& find, bool inCaseSensitive = true);
	runes_t  findLast(runes_t const& str, crunes_t const& find, bool inCaseSensitive = true);

	crunes_t findOneOf(crunes_t const& str, crunes_t const& set, bool inCaseSensitive = true);
	runes_t  findOneOf(runes_t const& str, crunes_t const& set, bool inCaseSensitive = true);

	// -------------------------------------------------------------------------------
	// crunes_t, functions
	crunes_t findSelectUntil(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	crunes_t findSelectUntil(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
	crunes_t findLastSelectUntil(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	crunes_t findLastSelectUntil(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
	crunes_t findSelectUntilIncluded(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	crunes_t findSelectUntilIncluded(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
	crunes_t findLastSelectUntilIncluded(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	crunes_t findLastSelectUntilIncluded(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);

	crunes_t findSelectAfter(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	crunes_t findSelectAfter(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
	crunes_t findLastSelectAfter(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	crunes_t findLastSelectAfter(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);

	crunes_t selectBetween(const crunes_t& inStr, uchar32 inLeft, uchar32 inRight);
	crunes_t selectNextBetween(const crunes_t& inStr, const crunes_t& inSelection, uchar32 inLeft, uchar32 inRight);

	crunes_t selectBetweenLast(const crunes_t& inStr, uchar32 inLeft, uchar32 inRight);
	crunes_t selectPreviousBetween(const crunes_t& inStr, const crunes_t& inSelection, uchar32 inLeft, uchar32 inRight);

	crunes_t selectFromBeginningExcludeSelection(const crunes_t& inStr, const crunes_t& inSelection);
	crunes_t selectFromBeginningIncludeSelection(const crunes_t& inStr, const crunes_t& inSelection);
	crunes_t selectUntilEndExcludeSelection(const crunes_t& inStr, const crunes_t& inSelection);
	crunes_t selectUntilEndIncludeSelection(const crunes_t& inStr, const crunes_t& inSelection);

	crunes_t selectOverlap(const crunes_t& inStr, const crunes_t& inRight);

	s32 compare(crunes_t const& str1, crunes_t const& str2, bool inCaseSensitive = true);

	// -------------------------------------------------------------------------------
	// runes_t, functions
	runes_t findSelectUntil(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	runes_t findSelectUntil(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
	runes_t findLastSelectUntil(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	runes_t findLastSelectUntil(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
	runes_t findSelectUntilIncluded(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	runes_t findSelectUntilIncluded(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
	runes_t findLastSelectUntilIncluded(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	runes_t findLastSelectUntilIncluded(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);

	runes_t findSelectAfter(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	runes_t findSelectAfter(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
	runes_t findLastSelectAfter(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
	runes_t findLastSelectAfter(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);

	runes_t selectBetween(const runes_t& inStr, uchar32 inLeft, uchar32 inRight);
	runes_t selectNextBetween(const runes_t& inStr, const runes_t& inSelection, uchar32 inLeft, uchar32 inRight);

	runes_t selectBetweenLast(const runes_t& inStr, uchar32 inLeft, uchar32 inRight);
	runes_t selectPreviousBetween(const runes_t& inStr, const runes_t& inSelection, uchar32 inLeft, uchar32 inRight);

	crunes_t selectFromBeginningExcludeSelection(const crunes_t& inStr, const crunes_t& inSelection);
	crunes_t selectFromBeginningIncludeSelection(const crunes_t& inStr, const crunes_t& inSelection);
	runes_t selectUntilEndExcludeSelection(const runes_t& inStr, const runes_t& inSelection);
	runes_t selectUntilEndIncludeSelection(const runes_t& inStr, const runes_t& inSelection);

	crunes_t selectBeforeExcludeSelection(const crunes_t& inStr, const crunes_t& inSelection);

	runes_t selectOverlap(const runes_t& inStr, const runes_t& inRight);

	s32 compare(runes_t const& str1, runes_t const& str2, bool inCaseSensitive = true);

	// -------------------------------------------------------------------------------
	// parse/from_string, to_string
	crunes_t parse(crunes_t const& str, bool& value);
	crunes_t parse(crunes_t const& str, s8& value, s32 base = 10);
	crunes_t parse(crunes_t const& str, s16& value, s32 base = 10);
	crunes_t parse(crunes_t const& str, s32& value, s32 base = 10);
	crunes_t parse(crunes_t const& str, s64& value, s32 base = 10);
	crunes_t parse(crunes_t const& str, u8& value, s32 base = 10);
	crunes_t parse(crunes_t const& str, u16& value, s32 base = 10);
	crunes_t parse(crunes_t const& str, u32& value, s32 base = 10);
	crunes_t parse(crunes_t const& str, u64& value, s32 base = 10);
	crunes_t parse(crunes_t const& str, f32& value);
	crunes_t parse(crunes_t const& str, f64& value);

	void to_string(runes_t& str, s32 val, s32 base = 10);
	void to_string(runes_t& str, u32 val, s32 base = 10);
	void to_string(runes_t& str, s64 val, s32 base = 10);
	void to_string(runes_t& str, u64 val, s32 base = 10);
	void to_string(runes_t& str, f32 val, s32 num_fractional_digits = 4);
	void to_string(runes_t& str, f64 val, s32 num_fractional_digits = 4);

	bool is_decimal(crunes_t const& str);
	bool is_hexadecimal(crunes_t const& str, bool with_prefix = false);
	bool is_float(crunes_t const& str);
	bool is_GUID(crunes_t const& str);

	inline bool is_space(uchar32 c) { return ((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
	inline bool is_upper(uchar32 c) { return ((c >= 'A') && (c <= 'Z')); }
	inline bool is_lower(uchar32 c) { return ((c >= 'a') && (c <= 'z')); }
	inline bool is_alpha(uchar32 c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
	inline bool is_digit(uchar32 c) { return ((c >= '0') && (c <= '9')); }
	inline bool is_hexa(uchar32 c) { return (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

	void           to_upper(runes_t& str);
	void           to_lower(runes_t& str);
	inline uchar32 to_upper(uchar32 c) { return ((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
	inline uchar32 to_lower(uchar32 c) { return ((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
	inline u32     to_digit(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
	inline u32     to_number(uchar32 c)
	{
		if (is_digit(c))
			return to_digit(c);
		else if (c >= 'A' && c <= 'F')
			return (c - 'A' + 10);
		else if (c >= 'a' && c <= 'f')
			return (c - 'a' + 10);
		return 0;
	}

	inline bool is_equal(uchar32 a, uchar32 b) { return (a == b); }
	inline bool is_equalfold(uchar32 a, uchar32 b)
	{
		a = to_lower(a);
		b = to_lower(b);
		return (a == b);
	}

	bool    is_upper(crunes_t const& str);
	bool    is_lower(crunes_t const& str);
	bool    is_capitalized(crunes_t const& str);
	bool    is_delimited(crunes_t const& str, uchar32 delimit_left = '\"', uchar32 delimit_right = '\"');
	bool    is_quoted(crunes_t const& str, uchar32 quote = '\"');
	bool    starts_with(crunes_t const& str, uchar32 start);
	bool    starts_with(crunes_t const& str, crunes_t const& start);
	bool    ends_with(crunes_t const& str, uchar32 end_char);
	bool    ends_with(crunes_t const& str, crunes_t const& end);
	uchar32 first_char(crunes_t const& str);
	uchar32 last_char(crunes_t const& str);

	void removeSelection(runes_t& str, crunes_t const& sel);
	void keepOnlySelection(runes_t& str, crunes_t const& sel);

	void replaceSelection(runes_t& str, crunes_t const& sel, crunes_t const& replace);
	void replaceSelection(runes_t& str, crunes_t const& sel, crunes_t const& replace, runes_alloc_t* allocator, s32 size_alignment);

	void findReplace(runes_t& str, uchar32 find, uchar32 replace, bool inCaseSensitive = true);
	void findReplace(runes_t& str, crunes_t const& find, crunes_t const& replace, bool inCaseSensitive = true);

	void insert(runes_t& str, crunes_t const& insert);
	void insert(runes_t& str, crunes_t const& insert, runes_alloc_t* allocator, s32 size_alignment);

	void insert(runes_t& str, crunes_t const& sel, crunes_t const& insert);
	void insert(runes_t& str, crunes_t const& sel, crunes_t const& insert, runes_alloc_t* allocator, s32 size_alignment);

	void trim(runes_t&);                                            // Trim whitespace from left and right side
	void trimLeft(runes_t&);                                        // Trim whitespace from left side
	void trimRight(runes_t&);                                       // Trim whitespace from right side
	void trim(runes_t&, uchar32 inChar);                            // Trim characters in <inCharSet> from left and right side
	void trimLeft(runes_t&, uchar32 inChar);                        // Trim character <inChar> from left side
	void trimRight(runes_t&, uchar32 inChar);                       // Trim character <inChar> from right side
	void trim(runes_t&, crunes_t const& inCharSet);                 // Trim characters in <inCharSet> from left and right side
	void trimLeft(runes_t&, crunes_t const& inCharSet);             // Trim characters in <inCharSet> from left side
	void trimRight(runes_t&, crunes_t const& inCharSet);            // Trim characters in <inCharSet> from right side
	void trimQuotes(runes_t&);                                      // Trim double quotes from left and right side
	void trimQuotes(runes_t&, uchar32 quote);                       // Trim double quotes from left and right side
	void trimDelimiters(runes_t&, uchar32 inLeft, uchar32 inRight); // Trim delimiters from left and right side

	void trim(crunes_t&);                                            // Trim whitespace from left and right side
	void trimLeft(crunes_t&);                                        // Trim whitespace from left side
	void trimRight(crunes_t&);                                       // Trim whitespace from right side
	void trim(crunes_t&, uchar32 inChar);                            // Trim characters in <inCharSet> from left and right side
	void trimLeft(crunes_t&, uchar32 inChar);                        // Trim character <inChar> from left side
	void trimRight(crunes_t&, uchar32 inChar);                       // Trim character <inChar> from right side
	void trim(crunes_t&, crunes_t const& inCharSet);                 // Trim characters in <inCharSet> from left and right side
	void trimLeft(crunes_t&, crunes_t const& inCharSet);             // Trim characters in <inCharSet> from left side
	void trimRight(crunes_t&, crunes_t const& inCharSet);            // Trim characters in <inCharSet> from right side
	void trimQuotes(crunes_t&);                                      // Trim double quotes from left and right side
	void trimQuotes(crunes_t&, uchar32 quote);                       // Trim double quotes from left and right side
	void trimDelimiters(crunes_t&, uchar32 inLeft, uchar32 inRight); // Trim delimiters from left and right side

	void copy(const crunes_t& src, runes_t& dst);
	void copy(const crunes_t& src, runes_t& dst, runes_alloc_t* allocator, s32 size_alignment);

	void concatenate(runes_t& str, const crunes_t& concat);
	void concatenate(runes_t& str, const crunes_t& concat, runes_alloc_t* allocator, s32 size_alignment);
	void concatenate(runes_t& str, const crunes_t& concat1, const crunes_t& concat2, runes_alloc_t* allocator, s32 size_alignment);

	inline bool operator==(const crunes_t& lhs, const crunes_t& rhs) { return compare(lhs, rhs) == 0; }
	inline bool operator!=(const crunes_t& lhs, const crunes_t& rhs) { return compare(lhs, rhs) != 0; }
	inline bool operator==(const runes_t& lhs, const runes_t& rhs) { return compare(lhs, rhs) == 0; }
	inline bool operator!=(const runes_t& lhs, const runes_t& rhs) { return compare(lhs, rhs) != 0; }

}; // namespace xcore

#endif ///< __XBASE_RUNES2_H__
