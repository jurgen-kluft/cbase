#include "xbase/x_string_utf.h"
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_console.h"
#include "xbase/x_va_list.h"
#include "xbase/x_chars.h"

namespace xcore
{
	namespace utf
	{
		inline s32	sequence_sizeof_utf8(uchar8 c)
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


		uchar32		peek(xcuchars const & str)
		{
			uchar32 c = '\0';
			if (can_read(str))
				c = str.m_const_str[0];
			return c;
		}

		uchar32		peek(xcuchar8s const& str)
		{
			uchar8 const* src = str.m_const_str;
			uchar32 c = '\0';
			if (can_read(str))
			{
				c = *src++;
				s32 const l = sequence_sizeof_utf8((uchar8)c);
				switch (l)
				{
				case 1:
					break;
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

		uchar32			peek(xcuchar32s const & str)
		{
			uchar32 c = '\0';
			if (can_read(str))
				c = str.m_const_str[0];
			return c;
		}

		uchar32			read(const uchar*  & str, const uchar* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				str++;
			}
			return c;
		}

		uchar32			read(const uchar8* & str, const uchar8* end)
		{
			uchar32 c = *str;
			if (c != '\0')
			{
				s32 const l = sequence_sizeof_utf8((uchar8)c);
				if (end != NULL && (str + l) > end)
					return c;

				switch (l)
				{
				case 1:
					str++;
					break;
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

		uchar32			read(const uchar16*& str, const uchar16* end)
		{
			uchar32 out_c;
			uchar16 c = *str;
			if (c == '\0' || (end != NULL && str >= end))
			{
				out_c = '\0';
				return out_c;
			}

			s32 l = 0;
			if (c < 0xd800) { l = 1; }
			else if (c < 0xdc00) { l = 2; }

			if (end == NULL || (str + l) <= end)
			{
				out_c = 0;
				for (s32 i = 0; i < l; i++) {
					c = str[i];
					out_c = out_c << 16;
					out_c = out_c | c;
				}
				str += l;
			}
			else {
				out_c = '\0';
			}
			return out_c;
		}

		uchar32			read(const uchar32*& str, const uchar32* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				str++;
			}
			return c;
		}

		uchar32			read(uchar*& str, const uchar* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				str++;
			}
			return c;
		}

		uchar32			read(uchar8*& str, uchar8 const* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				const uchar8* cstr = str;
				c = read(cstr, end);
				str = (uchar8*)cstr;
			}
			return c;
		}

		uchar32			read(uchar32*& str, const uchar32* end)
		{
			uchar32 c = *str;
			if (c != '\0' && (end == NULL || str < end))
			{
				str++;
			}
			return c;
		}

		uchar32			read(xcuchars & str)
		{
			return read(str.m_const_str, str.m_const_end);
		}

		uchar32			read(xcuchar8s& str)
		{
			return read(str.m_const_str, str.m_const_end);
		}

		uchar32			read(xcuchar32s & str)
		{
			return read(str.m_const_str, str.m_const_end);
		}


		// ####### WRITE ########


		void			write(uchar32 c, uchar*  & dest, uchar const* end)
		{
			if (c > 0x7f) {
				c = '?';
			}
			if (dest < end)
			{
				dest[0] = (uchar)c;
				dest++;
			}
		}

		void			write(uchar32 cp, uchar8* & dest, uchar8 const* end)
		{
			if (cp < 0x80)
			{	// one octet
				if (dest < end)
				{
					*(dest++) = static_cast<uchar8>(cp);
				}
			}
			else if (cp < 0x800)
			{	// two octets
				if ((dest + 1) < end)
				{
					*(dest++) = static_cast<uchar8>((cp >> 6) | 0xc0);
					*(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
				}
			}
			else if (cp < 0x10000)
			{	// three octets
				if ((dest + 2) < end)
				{
					*(dest++) = static_cast<uchar8>((cp >> 12) | 0xe0);
					*(dest++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
					*(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
				}
			}
			else
			{	// four octets
				if ((dest + 3) < end)
				{
					*(dest++) = static_cast<uchar8>((cp >> 18) | 0xf0);
					*(dest++) = static_cast<uchar8>(((cp >> 12) & 0x3f) | 0x80);
					*(dest++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
					*(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
				}
			}
		}

		void			write(uchar32 rune, uchar16*& dest, uchar16 const* end)
		{
			s32 len = 0;
			if (rune < 0xd800) { len = 1; }
			else if (rune < 0xe000) { len = 0; }
			else if (rune < 0x010000) { len = 1; }
			else if (rune < 0x110000) { len = 2; }

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
					*dest++ = static_cast<uchar16>((iv >> 10) + 0xd800);
					*dest++ = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
				}
			}
		}

		void			write(uchar32 c, uchar32*& dst, uchar32 const* end)
		{
			if (dst < end)
			{
				dst[0] = c;
				dst++;
			}
		}

		bool			write(uchar32 c, xuchars& dst)
		{
			if (can_write(dst))
			{
				write(c, dst.m_end, dst.m_eos);
				dst.m_const_end = dst.m_end;
				return true;
			}
			return false;
		}

		bool			write(uchar32 c, xuchar8s& dst)
		{
			if (can_write(dst))
			{
				write(c, dst.m_end, dst.m_eos);
				dst.m_const_end = dst.m_end;
				return true;
			}
			return false;
		}

		bool			write(uchar32 c, xuchar32s& dst)
		{
			if (can_write(dst))
			{
				write(c, dst.m_end, dst.m_eos);
				dst.m_const_end = dst.m_end;
				return true;
			}
			return false;
		}

		bool			write(uchar32 c, uchar16*& dst, uchar16* end)
		{
			if (!is_eos(dst) && dst < end)
			{
				write(c, dst, end);
				return true;
			}
			return false;
		}


		void			copy(xcuchars const & sstr, xuchars& dstr, ETermType term_type)
		{
			xcuchars src = sstr;
			while (can_read(src))
			{
				uchar32 c = read(src);
				if (!write(c, dstr))
					break;
			}
			if (term_type == TERMINATOR_WRITE)
				write('\0', dstr);
		}

		void			copy(xcuchar32s const & sstr, xuchars& dstr, ETermType term_type)
		{
			xcuchar32s src = sstr;
			while (can_read(src))
			{
				uchar32 c = read(src);
				if (!write(c, dstr))
					break;
			}
			if (term_type == TERMINATOR_WRITE)
				write('\0', dstr);
		}

		void			copy(xcuchars   const & sstr, xuchar32s& dstr, ETermType term_type)
		{
			xcuchars src = sstr;
			while (can_read(src))
			{
				uchar32 c = read(src);
				if (!write(c, dstr))
					break;
			}
			if (term_type == TERMINATOR_WRITE)
				write('\0', dstr);
		}

		void			copy(xcuchar32s const & sstr, xuchar32s& dstr, ETermType term_type)
		{
			xcuchar32s src = sstr;
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

		s32				size(uchar32 c)
		{
			s32 len = 0;
			if (c <= 0x7f) { len = 1; }
			else if (c < 0x0800) { len = 2; }
			else if (c < 0xd800) { len = 3; }
			else if (c < 0xe000) {}
			else if (c < 0x010000) { len = 3; }
			else if (c < 0x110000) { len = 4; }

			return len;
		}

		bool			is_eos(uchar const* str)
		{
			return str[0] == '\0';
		}

		bool			is_eos(uchar8 const* str)
		{
			return str[0] == '\0';
		}

		bool			is_eos(uchar16 const* str)
		{
			return str[0] == '\0';
		}

		bool			is_eos(uchar32 const* str)
		{
			return str[0] == '\0';
		}


		bool			can_read(xuchars   const& str)
		{
			return str.m_const_str < str.m_const_end && str.m_const_str[0] != '\0';
		}

		bool			can_read(xuchar8s  const& str)
		{
			return str.m_const_str < str.m_const_end && str.m_const_str[0] != '\0';
		}

		bool			can_read(xuchar32s const& str)
		{
			return str.m_const_str < str.m_const_end && str.m_const_str[0] != '\0';
		}

		bool			can_read(xcuchars   const& str)
		{
			return str.m_const_str < str.m_const_end && str.m_const_str[0] != '\0';
		}

		bool			can_read(xcuchar8s  const& str)
		{
			return str.m_const_str < str.m_const_end && str.m_const_str[0] != '\0';
		}

		bool			can_read(xcuchar32s const& str)
		{
			return str.m_const_str < str.m_const_end && str.m_const_str[0] != '\0';
		}

		bool			can_write(xuchars   const& str)
		{
			return str.m_end < str.m_eos;
		}

		bool			can_write(xuchar8s  const& str)
		{
			return str.m_end < str.m_eos;
		}

		bool			can_write(xuchar32s const& str)
		{
			return str.m_end < str.m_eos;
		}


		bool			read_is_crln(xuchars   const& str)
		{
			return (str.m_const_str + 1) < str.m_const_end && (str.m_const_str[0] == '\r' && str.m_const_str[1] == '\n');
		}

		bool			read_is_crln(xuchar8s  const& str)
		{
			return (str.m_const_str + 1) < str.m_const_end && (str.m_const_str[0] == '\r' && str.m_const_str[1] == '\n');
		}

		bool			read_is_crln(xuchar32s const& str)
		{
			return (str.m_const_str + 1) < str.m_const_end && (str.m_const_str[0] == '\r' && str.m_const_str[1] == '\n');
		}

		bool			read_is_crln(xcuchars   const& str)
		{
			return (str.m_const_str + 1) < str.m_const_end && (str.m_const_str[0] == '\r' && str.m_const_str[1] == '\n');
		}

		bool			read_is_crln(xcuchar8s  const& str)
		{
			return (str.m_const_str + 1) < str.m_const_end && (str.m_const_str[0] == '\r' && str.m_const_str[1] == '\n');
		}

		bool			read_is_crln(xcuchar32s const& str)
		{
			return (str.m_const_str + 1) < str.m_const_end && (str.m_const_str[0] == '\r' && str.m_const_str[1] == '\n');
		}


	}	// utf
};

namespace xcore
{
#define __XBASE_GENERIC_STRING_FUNCS__

	namespace utf8
	{
		static inline s32		peek_char(runes const& _src, uchar32& out_c)
		{
			out_c = '\0';
			if (!_src.is_empty())
			{
				pcrune src = _src.m_const_str;
				pcrune end = _src.m_const_end;
				out_c = utf::read(src, end);
				return 1;
			}
			return 0;
		}
		static inline s32		peek_char(crunes const& _src, uchar32& out_c)
		{
			out_c = '\0';
			if (!_src.is_empty())
			{
				pcrune src = _src.m_const_str;
				pcrune end = _src.m_const_end;
				out_c = utf::read(src, end);
				return 1;
			}
			return 0;
		}

		//------------------------------------------------------------------------------
		static bool				write_char(uchar32 c, runes& dst)
		{
			if (dst.m_end < dst.m_eos)
			{
				utf::write(c, dst.m_end, dst.m_eos);
				dst.m_const_end = dst.m_end;
				return true;
			}
			return false;
		}

		#include "x_string_funcs.cpp"
	}





	namespace utf32
	{
		static inline s32		peek_char(runes const& src, uchar32& out_c)
		{
			out_c = '\0';
			if (!src.is_empty())
			{
				out_c = src.m_const_str[0];
				return 1;
			}
			return 0;
		}
		static inline s32		peek_char(crunes const& src, uchar32& out_c)
		{
			out_c = '\0';
			if (!src.is_empty())
			{
				pcrune str = src.m_const_str;
				out_c = utf::peek(str);
				return 1;
			}
			return 0;
		}

		//------------------------------------------------------------------------------
		static bool				write_char(uchar32 c, runes& dst)
		{
			if (dst.m_end < dst.m_eos)
			{
				utf::write(c, dst.m_end, dst.m_eos);
				dst.m_const_end = dst.m_end;
				return true;
			}
			return false;
		}

		#include "x_string_funcs.cpp"
	}
};

/**
 *  END xCore namespace
 */
