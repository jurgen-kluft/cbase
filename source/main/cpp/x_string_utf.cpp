#include "xbase/x_runes.h"
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_console.h"
#include "xbase/x_va_list.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

namespace xcore
{
	namespace utf
	{
		inline s32 sequence_sizeof_utf8(uchar8 c)
		{
			u8 lead = c;
			if (lead < 0x80)
				return 1;
			else if ((lead >> 5) == 0x6)
				return 2;
			else if ((lead >> 4) == 0xe)
				return 3;
			else if ((lead >> 3) == 0x1e)
				return 4;
			else
				return 0;
		}

		uchar32 peek(ascii::crunes const& str)
		{
			uchar32 c = '\0';
			if (can_read(str))
				c = str.m_str[0];
			return c;
		}

		uchar32 peek(utf8::crunes const& str)
		{
			uchar8 const* src = str.m_str;
			uchar32		  c   = '\0';
			if (can_read(str))
			{
				c			= *src++;
				s32 const l = sequence_sizeof_utf8((uchar8)c);
				switch (l)
				{
					case 1: break;
					case 2:
						c = ((c << 6) & 0x7ff) + ((src[0]) & 0x3f);
						src++;
						break;
					case 3:
						c = ((c << 12) & 0xffff) + (((src[0]) << 6) & 0xfff);
						++src;
						c += (src[0]) & 0x3f;
						++src;
						break;
					case 4:
						c = ((c << 18) & 0x1fffff) + (((src[0]) << 12) & 0x3ffff);
						++src;
						c += ((src[0]) << 6) & 0xfff;
						++src;
						c += (src[0]) & 0x3f;
						++src;
						break;
				}
			}
			return c;
		}

		uchar32 peek(utf32::crunes const& str)
		{
			uchar32 c = '\0';
			if (can_read(str))
				c = str.m_str[0];
			return c;
		}

		uchar32 read(ascii::rune const*& str, ascii::rune const* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				str++;
			}
			return c;
		}

		uchar32 read(utf8::rune const*& str, utf8::rune const* end)
		{
			uchar32 c = *str;
			if (c != '\0')
			{
				s32 const l = sequence_sizeof_utf8((uchar8)c);
				if (end != NULL && (str + l) > end)
					return c;

				switch (l)
				{
					case 1: str++; break;
					case 2:
						str++;
						c = ((c << 6) & 0x7ff) + ((str[0]) & 0x3f);
						str++;
						break;
					case 3:
						str++;
						c = ((c << 12) & 0xffff) + (((str[0]) << 6) & 0xfff);
						++str;
						c += (str[0]) & 0x3f;
						++str;
						break;
					case 4:
						str++;
						c = ((c << 18) & 0x1fffff) + (((str[0]) << 12) & 0x3ffff);
						++str;
						c += ((str[0]) << 6) & 0xfff;
						++str;
						c += (str[0]) & 0x3f;
						++str;
						break;
				}
			}
			return c;
		}

		uchar32 read(utf16::rune const*& str, utf16::rune const* end)
		{
			uchar32 out_c;
			uchar16 c = *str;
			if (c == '\0' || (end != NULL && str >= end))
			{
				out_c = '\0';
				return out_c;
			}

			s32 l = 0;
			if (c < 0xd800)
			{
				l = 1;
			}
			else if (c < 0xdc00)
			{
				l = 2;
			}

			if (end == NULL || (str + l) <= end)
			{
				out_c = 0;
				for (s32 i = 0; i < l; i++)
				{
					c	 = str[i];
					out_c = out_c << 16;
					out_c = out_c | c;
				}
				str += l;
			}
			else
			{
				out_c = '\0';
			}
			return out_c;
		}

		uchar32 read(utf32::rune const*& str, utf32::rune const* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				str++;
			}
			return c;
		}

		uchar32 read(ascii::rune*& str, ascii::rune const* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				str++;
			}
			return c;
		}

		uchar32 read(utf8::rune*& str, utf8::rune const* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				const uchar8* cstr = str;
				c				   = read(cstr, end);
				str				   = (uchar8*)cstr;
			}
			return c;
		}

		uchar32 read(utf32::rune*& str, utf32::rune const* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				str++;
			}
			return c;
		}

		uchar32 read(ascii::crunes& str) { return read(str.m_str, str.m_end); }

		uchar32 read(utf8::crunes& str) { return read(str.m_str, str.m_end); }

		uchar32 read(utf32::crunes& str) { return read(str.m_str, str.m_end); }

		// ####### WRITE ########

		void write(uchar32 c, uchar*& dest, uchar const* end)
		{
			if (c > 0x7f)
			{
				c = '?';
			}
			if (dest < end)
			{
				dest[0] = (uchar)c;
				dest++;
			}
		}

		void write(uchar32 cp, uchar8*& dest, uchar8 const* end)
		{
			if (cp < 0x80)
			{ // one octet
				if (dest < end)
				{
					*(dest++) = static_cast<uchar8>(cp);
				}
			}
			else if (cp < 0x800)
			{ // two octets
				if ((dest + 1) < end)
				{
					*(dest++) = static_cast<uchar8>((cp >> 6) | 0xc0);
					*(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
				}
			}
			else if (cp < 0x10000)
			{ // three octets
				if ((dest + 2) < end)
				{
					*(dest++) = static_cast<uchar8>((cp >> 12) | 0xe0);
					*(dest++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
					*(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
				}
			}
			else
			{ // four octets
				if ((dest + 3) < end)
				{
					*(dest++) = static_cast<uchar8>((cp >> 18) | 0xf0);
					*(dest++) = static_cast<uchar8>(((cp >> 12) & 0x3f) | 0x80);
					*(dest++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
					*(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
				}
			}
		}

		void write(uchar32 rune, uchar16*& dest, uchar16 const* end)
		{
			s32 len = 0;
			if (rune < 0xd800)
			{
				len = 1;
			}
			else if (rune < 0xe000)
			{
				len = 0;
			}
			else if (rune < 0x010000)
			{
				len = 1;
			}
			else if (rune < 0x110000)
			{
				len = 2;
			}

			if (dest != NULL && len > 0)
			{
				if (len == 1 && dest < end)
				{
					*dest++ = (uchar16)rune;
				}
				else if ((dest + 1) < end)
				{
					// 20-bit intermediate value
					u32 const iv = rune - 0x10000;
					*dest++		 = static_cast<uchar16>((iv >> 10) + 0xd800);
					*dest++		 = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
				}
			}
		}

		void write(uchar32 c, uchar32*& dst, uchar32 const* end)
		{
			if (dst < end)
			{
				dst[0] = c;
				dst++;
			}
		}

		bool write(uchar32 c, ascii::runes& dst)
		{
			if (can_write(dst))
			{
				write(c, dst.m_end, dst.m_eos);
				dst.m_end = dst.m_end;
				return true;
			}
			return false;
		}

		bool write(uchar32 c, utf8::runes& dst)
		{
			if (can_write(dst))
			{
				write(c, dst.m_end, dst.m_eos);
				dst.m_end = dst.m_end;
				return true;
			}
			return false;
		}

		bool write(uchar32 c, utf32::runes& dst)
		{
			if (can_write(dst))
			{
				write(c, dst.m_end, dst.m_eos);
				dst.m_end = dst.m_end;
				return true;
			}
			return false;
		}

		bool write(uchar32 c, uchar16*& dst, uchar16* end)
		{
			if (!is_eos(dst) && dst < end)
			{
				write(c, dst, end);
				return true;
			}
			return false;
		}

		void copy(ascii::crunes const& sstr, ascii::runes& dstr, ETermType term_type)
		{
			ascii::crunes src = sstr;
			while (can_read(src))
			{
				uchar32 c = read(src);
				if (!write(c, dstr))
					break;
			}
			if (term_type == TERMINATOR_WRITE)
				write('\0', dstr);
		}

		void copy(utf32::crunes const& sstr, ascii::runes& dstr, ETermType term_type)
		{
			utf32::crunes src = sstr;
			while (can_read(src))
			{
				uchar32 c = read(src);
				if (!write(c, dstr))
					break;
			}
			if (term_type == TERMINATOR_WRITE)
				write('\0', dstr);
		}

		void copy(ascii::crunes const& sstr, utf32::runes& dstr, ETermType term_type)
		{
			ascii::crunes src = sstr;
			while (can_read(src))
			{
				uchar32 c = read(src);
				if (!write(c, dstr))
					break;
			}
			if (term_type == TERMINATOR_WRITE)
				write('\0', dstr);
		}

		void copy(utf32::crunes const& sstr, utf32::runes& dstr, ETermType term_type)
		{
			utf32::crunes src = sstr;
			while (true)
			{
				uchar32 c = read(src);
				if (c == '\0')
					break;
				if (!write(c, dstr))
					break;
			}
			if (term_type == TERMINATOR_WRITE)
				write('\0', dstr);
		}

		s32 size(uchar32 c)
		{
			s32 len = 0;
			if (c <= 0x7f)
			{
				len = 1;
			}
			else if (c < 0x0800)
			{
				len = 2;
			}
			else if (c < 0xd800)
			{
				len = 3;
			}
			else if (c < 0xe000)
			{
			}
			else if (c < 0x010000)
			{
				len = 3;
			}
			else if (c < 0x110000)
			{
				len = 4;
			}

			return len;
		}

		bool is_eos(uchar const* str) { return str[0] == '\0'; }

		bool is_eos(uchar8 const* str) { return str[0] == '\0'; }

		bool is_eos(uchar16 const* str) { return str[0] == '\0'; }

		bool is_eos(uchar32 const* str) { return str[0] == '\0'; }

		bool can_read(ascii::runes const& str)
		{
			if (str.m_end == nullptr)
				return str.m_str[0] != '\0';
			return str.m_str < str.m_end && str.m_str[0] != '\0';
		}

		bool can_read(utf8::runes const& str)
		{
			if (str.m_end == nullptr)
				return str.m_str[0] != '\0';
			return str.m_str < str.m_end && str.m_str[0] != '\0';
		}

		bool can_read(utf32::runes const& str)
		{
			if (str.m_end == nullptr)
				return str.m_str[0] != '\0';
			return str.m_str < str.m_end && str.m_str[0] != '\0';
		}

		bool can_read(ascii::crunes const& str)
		{
			if (str.m_end == nullptr)
				return str.m_str[0] != '\0';
			return str.m_str < str.m_end && str.m_str[0] != '\0';
		}

		bool can_read(utf8::crunes const& str)
		{
			if (str.m_end == nullptr)
				return str.m_str[0] != '\0';
			return str.m_str < str.m_end && str.m_str[0] != '\0';
		}

		bool can_read(utf32::crunes const& str)
		{
			if (str.m_end == nullptr)
				return str.m_str[0] != '\0';
			return str.m_str < str.m_end && str.m_str[0] != '\0';
		}

		bool can_write(ascii::runes const& str)
		{
			ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
			return str.m_end < str.m_eos;
		}

		bool can_write(utf8::runes const& str)
		{
			ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
			return str.m_end < str.m_eos;
		}

		bool can_write(utf32::runes const& str)
		{
			ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
			return str.m_end < str.m_eos;
		}

		bool read_is_crln(ascii::runes const& str)
		{
			ASSERT(str.m_end != nullptr);
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}

		bool read_is_crln(utf8::runes const& str)
		{
			ASSERT(str.m_end != nullptr);
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}

		bool read_is_crln(utf32::runes const& str)
		{
			ASSERT(str.m_end != nullptr);
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}

		bool read_is_crln(ascii::crunes const& str)
		{
			ASSERT(str.m_end != nullptr);
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}

		bool read_is_crln(utf8::crunes const& str)
		{
			ASSERT(str.m_end != nullptr);
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}

		bool read_is_crln(utf32::crunes const& str)
		{
			ASSERT(str.m_end != nullptr);
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}

	} // utf
};

namespace xcore
{
#define __XBASE_GENERIC_STRING_FUNCS__

	namespace utf16
	{
		static inline s32 peek_char(runes const& _src, uchar32& out_c)
		{
			out_c = '\0';
			if (utf::can_read(_src))
			{
				pcrune src = _src.m_str;
				pcrune end = _src.m_end;
				out_c	  = utf::read(src, end);
				return 1;
			}
			return 0;
		}
		static inline s32 peek_char(crunes const& _src, uchar32& out_c)
		{
			out_c = '\0';
			if (utf::can_read(_src))
			{
				pcrune src = _src.m_str;
				pcrune end = _src.m_end;
				out_c	  = utf::read(src, end);
				return 1;
			}
			return 0;
		}

		//------------------------------------------------------------------------------
		static bool write_char(uchar32 c, runes& dst)
		{
			if (dst.m_end < dst.m_eos)
			{
				utf::write(c, dst.m_end, dst.m_eos);
				dst.m_end = dst.m_end;
				return true;
			}
			return false;
		}

		#include "x_string_funcs.cpp"
	}


	namespace utf32
	{
		static inline s32 peek_char(runes const& _src, uchar32& out_c)
		{
			out_c = '\0';
			if (utf::can_read(_src))
			{
				out_c = _src.m_str[0];
				return 1;
			}
			return 0;
		}
		static inline s32 peek_char(crunes const& _src, uchar32& out_c)
		{
			out_c = '\0';
			if (utf::can_read(_src))
			{
				out_c = _src.m_str[0];
				return 1;
			}
			return 0;
		}

		//------------------------------------------------------------------------------
		static bool write_char(uchar32 c, runes& dst)
		{
			if (dst.m_end < dst.m_eos)
			{
				utf::write(c, dst.m_end, dst.m_eos);
				dst.m_end = dst.m_end;
				return true;
			}
			return false;
		}

		//#include "x_string_funcs.cpp"

		//------------------------------------------------------------------------------
		static bool read_char(runes& src, uchar32& out_c)
		{
			s32 const l = peek_char(src, out_c);
			src.m_str += l;
			return l > 0;
		}

		//------------------------------------------------------------------------------
		static bool read_char(crunes& src, uchar32& out_c)
		{
			s32 l = peek_char(src, out_c);
			src.m_str += l;
			return l > 0;
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		s32 len(prune str, prune end)
		{
			s32 l = 0;
			while (end == NULL || str < end)
			{
				uchar32 c = utf::read(str, end);
				if (c == '\0')
					break;
				l++;
			}
			return l;
		}

		s32 len(pcrune str, pcrune end)
		{
			s32 l = 0;
			if (end == NULL)
			{
				while (true)
				{
					uchar32 c = utf::read(str, end);
					if (c == '\0')
						break;
					l++;
				}
			}
			else
			{
				while (str < end)
				{
					uchar32 c = utf::read(str, end);
					if (c == '\0')
						break;
					l++;
				}
			}
			return l;
		}

		prune endof(prune str, prune end)
		{
			while (str < end)
			{
				prune   s = str;
				uchar32 c = utf::read(s, end);
				if (c == '\0')
					break;
				str = s;
			}
			return str;
		}

		pcrune endof(pcrune str, pcrune end)
		{
			while (str < end)
			{
				pcrune  s = str;
				uchar32 c = utf::read(s, end);
				if (c == '\0')
					break;
				str = s;
			}
			return str;
		}

		crunes find(crunes const& _str, uchar32 _c, bool inCaseSensitive)
		{
			crunes str(_str);
			uchar32 c;
			while (!str.is_empty())
			{
				crunes pos = str;
				if (!read_char(str, c))
					break;

				bool equal;
				switch (inCaseSensitive)
				{
					case true: equal = is_equal(c, _c); break;
					case false: equal = is_equalfold(c, _c); break;
				}

				if (equal)
				{
					pos.m_end = str.m_end;
					return pos;
				}
			}
			return crunes();
		}

		bool contains(crunes const& _str, uchar32 _c, bool inCaseSensitive = true)
		{ 
			crunes pos = find(_str, _c, inCaseSensitive);
			return pos.is_empty();
		}

		runes find(runes const& _str, uchar32 _c, bool inCaseSensitive)
		{
			runes   str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				runes pos = str;
				if (!read_char(str, c))
					break;

				bool equal;
				switch (inCaseSensitive)
				{
					case true: equal = is_equal(c, _c); break;
					case false: equal = is_equalfold(c, _c); break;
				}

				if (equal)
				{
					pos.m_end = str.m_end;
					return pos;
				}
			}
			return runes();
		}

		runes find(runes const& _str, crunes const& _find, bool inCaseSensitive)
		{
			if (_str.is_empty())
				return runes();
			if (_find.is_empty())
				return runes();

			runes str_iter = _str;
			runes str_found;

			bool  found		= false;
			while (str_iter.is_empty() == false)
			{
				// Remember the current selection because here is
				// where we start to check if the 'find' string
				// can be found at this particular position.
				str_found = _str;

				uchar32 sc, fc;
				if (!read_char(str_iter, sc))
					break;

				runes  str_find = str_iter;
				crunes find_str = _find;

				found = true;
				while (found && read_char(find_str, fc))
				{
					if (!read_char(str_find, sc))
					{
						found = false;
						break;
					}
					switch (inCaseSensitive)
					{
						case true: found = is_equal(sc, fc); break;
						case false: found = is_equalfold(sc, fc); break;
					}
				}

				if (found)
				{
					str_found.m_end = str_find.m_str;
					break;
				}
			}
			return found ? str_found : runes();
		}

		runes findSelectUntil(runes const& _str, uchar32 _c, bool inCaseSensitive) 
		{
			runes selection = find(_str, _c, inCaseSensitive);
			if (selection.is_empty())
				return selection;
			selection.m_str = _str.m_str;
			return selection;
		}

		runes findSelectUntil(runes const& _str, crunes const& _find, bool inCaseSensitive)
		{
			runes selection = find(_str, _find, inCaseSensitive);
			if (selection.is_empty())
				return selection;
			selection.m_str = _str.m_str;
			return selection;
		}

		runes findOneOf(runes const& _str, crunes const& _set, bool inCaseSensitive)
		{
			runes   str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				// Remember the current position so that if we found
				// the char in the set we can return this position.
				runes pos = str;

				// Read next character from 'str'
				if (!read_char(str, c))
					break;

				// See if this char can be found in the 'set'
				crunes found = find(_set, c, inCaseSensitive);
				if (!found.is_empty())
					return pos;
			}
			// Nothing found, return an empty object
			return runes();
		}

		s32 compare(crunes const& _lstr, crunes const& _rstr, bool inCaseSensitive)
		{
			crunes  lstr = _lstr;
			crunes  rstr = _rstr;
			uchar32 lc, rc;
			while (read_char(lstr, lc) && read_char(rstr, rc))
			{
				if (lc != rc)
					break;
			}
			if (lc < rc)
				return -1;
			else if (lc > rc)
				return 1;
			return 0;
		}

		//------------------------------------------------------------------------------
		crunes parse(crunes const& _str, bool& value)
		{
			crunes str			= _str;
			rune   format_str[] = {'%', 'b'};
			crunes format(format_str, endof(format_str, NULL));
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes parse(crunes const& _str, s32& value, s32 base)
		{
			crunes str			= _str;
			rune   format_str[] = {'%', 'd'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str, endof(format_str, NULL));
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes parse(crunes const& _str, u32& value, s32 base)
		{
			crunes str			= _str;
			rune   format_str[] = {'%', 'd'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str, endof(format_str, NULL));
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes parse(crunes const& _str, s64& value, s32 base)
		{
			crunes str			= _str;
			rune   format_str[] = {'%', 'd'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str, endof(format_str, NULL));
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes parse(crunes const& _str, u64& value, s32 base)
		{
			crunes str			= _str;
			rune   format_str[] = {'%', 'd'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str, endof(format_str, NULL));
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes parse(crunes const& _str, f32& value)
		{
			crunes str			= _str;
			rune   format_str[] = {'%', 'f'};
			crunes format(format_str, endof(format_str, NULL));
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes parse(crunes const& _str, f64& value)
		{
			crunes str			= _str;
			rune   format_str[] = {'%', 'f'};
			crunes format(format_str, endof(format_str, NULL));
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		//------------------------------------------------------------------------------
		bool is_decimal(crunes const& _str)
		{
			crunes  str = _str;
			uchar32 c;
			while (peek_char(str, c))
			{
				if (c != '-')
					break;
				read_char(str, c);
			}
			while (read_char(str, c))
			{
				if (c < '0' || c > '9')
					return false;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool is_hexadecimal(crunes const& _str, bool with_prefix)
		{
			crunes str = _str;

			uchar32 p, c;
			s32		pos = 0;
			while (read_char(str, c))
			{
				if (c == 'x')
				{
					if (pos == 1 && with_prefix && p == '0')
					{
						// Ok, 0x.... prefix
					}
					else
					{
						return false;
					}
				}
				else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
				{
					// Ok, digit or hex character
					p = c;
				}
				else
				{
					return false;
				}
				pos++;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool is_float(crunes const& _str)
		{
			rune   f32chars_str[] = {'E', 'e', '.', '#', 'Q', 'N', 'A', 'B', 'I', 'F'};
			crunes f32chars(f32chars_str, endof(f32chars_str, NULL));

			crunes  str = _str;
			uchar32 c;
			while (read_char(str, c))
			{
				if (c >= '0' && c <= '9')
					continue;
				if (!contains(f32chars, c))
					return false;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool is_GUID(crunes const& _str)
		{
			rune   f32chars_str[] = {'E', 'e', '.', '#', 'Q', 'N', 'A', 'B', 'I', 'F'};
			crunes f32charslen(f32chars_str, endof(f32chars_str, NULL));

			crunes str = _str;

			// Does it have only 0-9, a-f, A-F characters
			s32		n = 0;
			s32		l = 0;
			uchar32 c;
			while (read_char(str, c))
			{
				if (c >= '0' && c <= '9')
				{
					n++;
					continue;
				}
				if (c >= 'A' && c <= 'F')
				{
					n++;
					continue;
				}
				if (c >= 'a' && c <= 'f')
				{
					n++;
					continue;
				}
				if (c == ':')
				{
					l++;
					if (l > 3)
						return false;
					continue;
				}
			}
			// the length of the string must be 17 characters long (4*8 + 3 ':')
			return (n == (4 * 8) && l == 3);
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------

		void to_string(runes& str, s32 val, s32 base)
		{
			rune format_str[] = {'%', 'd'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str, endof(format_str, NULL));
			sprintf(str, format, x_va(val));
		}

		void to_string(runes& str, u32 val, s32 base)
		{
			rune format_str[] = {'%', 'u'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'u'; break;
				case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str, endof(format_str, NULL));
			sprintf(str, format, x_va(val));
		}

		// *cFHbRXV[N]WWWR6FETY+

		void to_string(runes& str, s64 val, s32 base)
		{
			rune format_str[] = {'%', 'd'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str, endof(format_str, NULL));
			sprintf(str, format, x_va(val));
		}

		void to_string(runes& str, u64 val, s32 base)
		{
			rune format_str[] = {'%', 'd'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str, endof(format_str, NULL));
			sprintf(str, format, x_va(val));
		}

		void to_string(runes& str, f32 val, s32 numFractionalDigits)
		{
			rune format_str[] = {'%', '.', '0', '2', 'f', '\0'};
			if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
			{
				format_str[2] = '0' + numFractionalDigits / 10;
				format_str[3] = '0' + numFractionalDigits % 10;
			}
			crunes format(format_str, endof(format_str, NULL));
			sprintf(str, format, x_va(val));
		}

		void to_string(runes& str, f64 val, s32 numFractionalDigits)
		{
			rune format_str[] = {'%', '.', '0', '2', 'f', '\0'};
			if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
			{
				format_str[2] = '0' + numFractionalDigits / 10;
				format_str[3] = '0' + numFractionalDigits % 10;
			}
			crunes format(format_str, endof(format_str, NULL));
			sprintf(str, format, x_va(val));
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------

		bool is_upper(crunes const& _str)
		{
			crunes  str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				if (!read_char(str, c))
					break;
				if (is_lower(c))
					return false;
			}
			return true;
		}

		bool is_lower(crunes const& _str)
		{
			crunes  str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				if (!read_char(str, c))
					break;
				if (is_upper(c))
					return false;
			}
			return true;
		}

		bool is_capitalized(crunes const& _str)
		{
			crunes str = _str;
			bool   b   = true;
			while (!str.is_empty())
			{
				uchar32 c;
				if (!read_char(str, c))
					break;
				if (is_upper(c) != b)
					return false;
				b = (c == ' ');
			}
			return true;
		}

		bool is_delimited(crunes const& _str, rune delimit_left, rune delimit_right)
		{
			crunes str = _str;

			uchar32 c;
			if (!read_char(str, c))
			{
				if (c == delimit_left)
				{
					while (!str.is_empty())
					{
						if (!read_char(str, c))
							break;
					}
					return c == delimit_right;
				}
			}
			return false;
		}

		/**
		 * Assumption: Letters A-Z and a-z are contiguous in the character set.
		 * This is true for ASCII and UniCode.  (Not so for EBCDIC!)
		 */
		void to_upper(runes& _str)
		{
			runes   str = _str;
			uchar32 c;
			while (peek_char(str, c))
			{
				if ((c >= 'a') && (c <= 'z'))
				{
					c += ('A' - 'a');
					write_char(c, str);
				}
				else
				{
					read_char(str, c);
				}
			}
		}

		/**
		 * Assumption: Letters A-Z and a-z are contiguous in the character set.
		 * This is true for ASCII and UniCode.  (Not so for EBCDIC!)
		 */
		void to_lower(runes& _str)
		{
			runes   str = _str;
			uchar32 c;
			while (peek_char(str, c))
			{
				if ((c >= 'A') && (c <= 'Z'))
				{
					c += ('a' - 'A');
					write_char(c, str);
				}
				else
				{
					read_char(str, c);
				}
			}
		}

		// ------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------

		bool starts_with(crunes const& _str, uchar32 start_char)
		{
			crunes  str = _str;
			uchar32 c   = '\0';
			if (read_char(str, c))
				return start_char == c;
			return false;
		}

		bool starts_with(crunes const& _str, crunes const& _start)
		{
			crunes  lstr = _str;
			crunes  rstr = _start;
			uchar32 lc, rc;
			while (read_char(rstr, rc))
			{
				if (!read_char(lstr, lc))
					return false;
				if (lc != rc)
					return false;
				;
			}
			return true;
		}

		bool ends_with(crunes const& _str, uchar32 end_char)
		{
			crunes str = _str;
			if (!str.is_empty())
			{
				if (has_fixed_size_rune())
					str.m_str = str.m_end - 1;

				// Need to fast-forward to the last character
				uchar32 p = 0xffffffff;
				uchar32 c = 0xffffffff;
				while (read_char(str, c))
					p = c;
				return end_char == p;
			}
			return false;
		}

		bool ends_with(crunes const& _str, crunes const& _end)
		{
			crunes str = _str;
			crunes end = _end;
			if (!str.is_empty() && !end.is_empty())
			{
				if (str.size() > end.size())
					return false;

				if (has_fixed_size_rune())
					str.m_str = str.m_end - end.size();

				uchar32 c1, c2;
				while (read_char(str, c1) && read_char(end, c2))
				{
					if (c1 != c2)
						return false;
				}
			}
			return str.is_empty() && end.is_empty();
		}

		uchar32 first_char(crunes const& str)
		{
			uchar32 c;
			if (peek_char(str, c))
				return c;
			return '\0';
		}

		uchar32 last_char(crunes const& _str)
		{
			crunes str = _str;
			if (has_fixed_size_rune())
			{
				str.m_str = str.m_end - 1;
			}
			// Fast-forward to end of str, keep track of previous character
			uchar32 c, pc = '\0';
			while (read_char(str, c))
				pc = c;
			return pc;
		}

		// ------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------

		void removeSelection(runes& str, crunes const& selection)
		{ 
			prune src = (prune)selection.m_end;
			prune dst = (prune)selection.m_str;
			pcrune end = str.m_end;
			while (dst < end)
			{
				*dst++ = *src++;
			}
			*dst	  = '\0';
			str.m_end = dst;
		}

		void keepOnlySelection(runes& str, crunes const& keep)
		{
			prune  src = (prune)keep.m_str;
			prune  dst = (prune)str.m_str;
			pcrune end = keep.m_end;
			while (dst < end)
			{
				*dst++ = *src++;
			}
			*dst	  = '\0';
			str.m_end = dst;
		}


		void replaceSelection(runes& str, crunes const& selection, crunes const& replace)
		{
			// The logic here is based on memory copy, we do not consider characters
			s32 const selected_len = (s32)((xbyte const*)selection.m_end - (xbyte const*)selection.m_str);
			s32 const replace_len  = (s32)((xbyte const*)replace.m_end - (xbyte const*)replace.m_str);

			prune end = NULL;
			if (selected_len < replace_len)
			{
				// Move, expanding
				s32 move_len = replace_len - selected_len;
				if (move_len > ((xbyte const*)str.m_eos - (xbyte*)str.m_end))
					move_len = (s32)(((xbyte const*)str.m_eos - (xbyte*)str.m_end));

				// No matter what, push out anything at the end!
				xbyte* dst = (xbyte*)((xbyte*)str.m_end + ((xbyte const*)str.m_eos - (xbyte*)str.m_end) - 1);
				xbyte* src = (xbyte*)((xbyte*)dst - move_len);
				while (dst > (xbyte*)str.m_end)
					*dst-- = *src--;

				end = (prune)((xbyte*)str.m_str + selected_len + move_len); // Update str_end
			}
			else if (selected_len > replace_len)
			{
				// Move, shrinking
				s32	move_len = selected_len - replace_len;
				xbyte* dst		= (xbyte*)((xbyte*)str.m_end - move_len);
				xbyte* src		= (xbyte*)(str.m_end);
				while (src < (xbyte const*)str.m_eos)
					*dst++ = *src++;

				end = (prune)((xbyte*)str.m_str + selected_len - move_len); // Update str_end
			}
			else
			{
				end = (prune)(str.m_str + selected_len);
			}

			// Replace
			xbyte const* src	 = (xbyte const*)replace.m_str;
			xbyte const* src_end = (xbyte const*)replace.m_str + replace_len;
			xbyte*		 dst	 = (xbyte*)str.m_str;
			while (src < src_end)
				*dst++ = *src++;

			str.m_end = end;
		}

		static inline bool isIn(runes const& str, crunes const& selection)
		{
			return (selection.m_str >= str.m_str && selection.m_str < str.m_end && selection.m_end <= str.m_end);
		}

		crunes	relocate(runes const& from, runes const& to, crunes const& selection)
		{
			if (isIn(from, selection))
			{
				crunes nselection(to);
				s32 stroffset = selection.m_str - from.m_str;
				s32 endoffset = selection.m_end - from.m_str;
				nselection.m_end = nselection.m_str + endoffset;
				nselection.m_str = nselection.m_str + stroffset;
				return nselection;
			}
		}

		void replaceSelection(runes& str, crunes const& selection, crunes const& replace, alloc* allocator, s32 size_alignment) 
		{
			s32 const diff = replace.size() - selection.size();
			if (diff > 0)
			{
				// Do we need to allocate additional space for @str ?
				// This will invalidate @selection
				if ((str.size() + diff) > str.cap())
				{
					runes nstr = allocator->allocate(0, (str.size() + diff + size_alignment)&~(size_alignment - 1));
					copy(str, nstr);

					// Relocate selection
					crunes nselection = relocate(str, nstr, selection);

					// Deallocate original str 
					allocator->deallocate(str);
					str = nstr;
				}
			}
			replaceSelection(str, selection, replace);
		}


		void findReplace(runes& _str, uchar32 _find, uchar32 _replace, bool inCaseSensitive)
		{
			runez<3> find;
			utf::write(_find, find.m_end, find.m_eos);
			utf::write('\0', find.m_end, find.m_eos);
			
			runez<3> replace;
			utf::write(_replace, replace.m_end, replace.m_eos);
			utf::write('\0', replace.m_end, replace.m_eos);

			findReplace(_str, find, replace, inCaseSensitive);
		}

		void findReplace(runes& _str, crunes const& _find, crunes const& _replace, bool inCaseSensitive)
		{
			// Find
			runes selected = find(_str, _find, inCaseSensitive);
			if (selected.is_empty())
				return;
			replaceSelection(_str, selected, _replace);
		}

		void insert(runes& str, crunes const& insert)
		{

		}

		void insert(runes& str, crunes const& insert, alloc* allocator, s32 size_alignment)
		{

		}


		void insert(runes& str, crunes const& selection, crunes const& insert)
		{

		}

		void insert(runes& str, crunes const& selection, crunes const& insert, alloc* allocator, s32 size_alignment)
		{

		}


		void trim(runes& )
		{

		}

		void trimLeft(runes&)
		{

		}

		void trimRight(runes&)
		{

		}

		void trim(runes&, uchar32 inChar)
		{

		}

		void trimLeft(runes&, uchar32 inChar)
		{

		}

		void trimRight(runes&, uchar32 inChar)
		{

		}

		void trim(runes&, crunes const& inCharSet)
		{

		}

		void trimLeft(runes&, crunes const& inCharSet)
		{

		}

		void trimRight(runes&, crunes const& inCharSet)
		{

		}

		void trimQuotes(runes&)
		{

		}

		void trimQuotes(runes&, uchar32 quote)
		{

		}

		void trimDelimiters(runes&, uchar32 inLeft, uchar32 inRight)
		{

		}


		void copy(crunes const& _src, runes& _dest)
		{
			crunes src = _src;
			while (utf::can_write(_dest))
			{
				if (src.m_end != NULL && src.m_str == src.m_end)
				{
					break;
				}
				uchar32 c;
				if (read_char(src, c) == false)
				{
					write_char(c, _dest);
					break;
				}
				write_char(c, _dest);
			}
			if (utf::can_write(_dest))
				write_char('\0', _dest);
		}

		void concatenate(runes& str, crunes const& _concat)
		{
			crunes  concat = _concat;
			uchar32 c;
			while (true)
			{
				if (!read_char(concat, c))
					break;
				if (!write_char(c, str))
					break;
			}
		}

		void concatenate(runes& str, const crunes& concat, alloc* allocator, s32 size_alignment)
		{

		}

		void concatenate(runes& str, const crunes& concat1, const crunes& concat2, alloc* allocator, s32 size_alignment)
		{

		}

	}
};
