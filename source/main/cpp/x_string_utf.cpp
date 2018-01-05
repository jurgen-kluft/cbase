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
		uchar32			peek(xcuchars const & str)
		{
			uchar32 c = '\0';
			if (!is_eos(str))
				c = str.m_str[0];
			return c;
		}

		uchar32			peek(xcuchar32s const & str)
		{
			uchar32 c = '\0';
			if (!is_eos(str))
				c = str.m_str[0];
			return c;
		}


		uchar32			read(xcuchars & str)
		{
			uchar32 c = '\0';
			if (!is_eos(str))
			{
				c = str.m_str[0];
				str.m_str++;
			}
			return c;
		}

		uchar32			read(xcuchar32s & str)
		{
			uchar32 c = '\0';
			if (!is_eos(str))
			{
				c = str.m_str[0];
				str.m_str++;
			}
			return c;
		}

		bool			write(uchar32 c, xuchars& str)
		{
			if (!is_eos(str))
			{
				if (c > 0x7f) {
					c = '?';
				}
				str.m_str[0] = (uchar)c;
				str.m_str++;
				return true;
			}
			return false;
		}

		bool			write(uchar32 c, xuchar32s& str)
		{
			if (!is_eos(str))
			{
				str.m_str[0] = c;
				str.m_str++;
				return true;
			}
			return false;
		}

		void			copy(xcuchars   const & sstr, xuchars& dstr, ETermType term_type)
		{
			xcuchars src = sstr;
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

		void			copy(xcuchar32s const & sstr, xuchars& dstr, ETermType term_type)
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

		void			copy(xcuchars   const & sstr, xuchar32s& dstr, ETermType term_type)
		{
			xcuchars src = sstr;
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

		s32		size(uchar32 c)
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

		bool			is_eos(xuchars   const& str)
		{
			return str.m_str >= str.m_end || str.m_str[0] == '\0';
		}

		bool			is_eos(xuchar32s const& str)
		{
			return str.m_str >= str.m_end || str.m_str[0] == '\0';
		}

		bool			is_eos(xcuchars   const& str)
		{
			return str.m_str >= str.m_end || str.m_str[0] == '\0';
		}

		bool			is_eos(xcuchar32s const& str)
		{
			return str.m_str >= str.m_end || str.m_str[0] == '\0';
		}


		bool			is_crln(xuchars   const& str)
		{
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}

		bool			is_crln(xuchar32s const& str)
		{
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}

		bool			is_crln(xcuchars   const& str)
		{
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}

		bool			is_crln(xcuchar32s const& str)
		{
			return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
		}










		s32		sequence_sizeof_utf8(uchar8 c)
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

		static u8 sUTF8LC[] = { 0, 0, 0xc0, 0xe0, 0xf0 };
		uchar8*		write(uchar32 cp, uchar8* dest)
		{
			if (cp < 0x80)
			{	// one octet
				*(dest++) = static_cast<uchar8>(cp);
			}
			else if (cp < 0x800)
			{	// two octets
				*(dest++) = static_cast<uchar8>((cp >> 6) | 0xc0);
				*(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
			}
			else if (cp < 0x10000)
			{	// three octets
				*(dest++) = static_cast<uchar8>((cp >> 12) | 0xe0);
				*(dest++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
				*(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
			}
			else 
			{	// four octets
				*(dest++) = static_cast<uchar8>((cp >> 18) | 0xf0);
				*(dest++) = static_cast<uchar8>(((cp >> 12) & 0x3f) | 0x80);
				*(dest++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
				*(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
			}
			return dest;
		}

		uchar8 const*	read(uchar8 const* str, uchar32& out_c)
		{
			out_c = *str;
			s32 const l = sequence_sizeof_utf8((uchar8)out_c);
			switch (l) 
			{
			case 1:
				break;
			case 2:
				str++;
				out_c = ((out_c << 6) & 0x7ff) + ((*str) & 0x3f);
				break;
			case 3:
				++str;
				out_c = ((out_c << 12) & 0xffff) + (((*str) << 6) & 0xfff);
				++str;
				out_c += (*str) & 0x3f;
				break;
			case 4:
				++str;
				out_c = ((out_c << 18) & 0x1fffff) + (((*str) << 12) & 0x3ffff);
				++str;
				out_c += ((*str) << 6) & 0xfff;
				++str;
				out_c += (*str) & 0x3f;
				break;
			}
			return ++str;
		}

	}	// utf
};

namespace xcore
{
	#define __XBASE_GENERIC_STRING_FUNCS__
	
	namespace utf8
	{

	}





	namespace utf32
	{
		static inline s32		peek_char(runes const& src, uchar32& out_c)
		{
			out_c = '\0';
			if (!src.is_empty())
			{
				out_c = src.m_str[0];
				return 1;
			}
			return 0;
		}
		static inline s32		peek_char(crunes const& src, uchar32& out_c)
		{
			out_c = '\0';
			if (!src.is_empty())
			{
				out_c = src.m_str[0];
				return 1;
			}
			return 0;
		}

		//------------------------------------------------------------------------------
		static bool				write_char(uchar32 c, runes& dst)
		{
			if (!dst.is_full())
			{
				if (c > 0x7f)
					c = '?';
				*dst.m_end++ = (uchar)c;
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
