#include "xbase/x_string_utf.h"
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_console.h"
#include "xbase/x_va_list.h"


namespace xcore
{
	namespace utf
	{
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

		s32		size(uchar32 c)
		{
			s32 len = 0;
			if (c <= 0x7f) { len = 1; }
			else if (c < 0x0800) { len = 2; }
			else if (c < 0xd800) { len = 3; }
			else if (c < 0xe000) { }
			else if (c < 0x010000) { len = 3; }
			else if (c < 0x110000) { len = 4; }

			return len;
		}

		uchar*	write(uchar32 rune, uchar* dest)
		{
			uchar* dst = dest;
			if (rune <= 0x7f) {
				*dst++ = (uchar)rune;
			} else {
				*dst++ = '?';
			}
			return dst;
		}

		static u8 sUTF8LC[] = { 0, 0, 0xc0, 0xe0, 0xf0 };
		uchar8*	write(uchar32 cp, uchar8* dest)
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

		uchar16* write(uchar32 rune, uchar16* dest)
		{
			s32 len = 0;
			if (rune < 0xd800) { len = 1; }
            else if (rune < 0xe000) { len = 0; }
            else if (rune < 0x010000) { len = 1; }
            else if (rune < 0x110000) { len = 2; }

			uchar16* dst = dest;
			if (dst != NULL && len > 0)
			{
				if (len == 1) 
				{
					*dst++ = (uchar16)rune;
				}
				else 
				{
					// 20-bit intermediate value
					u32 const iv = rune - 0x10000;
					*dst++ = static_cast<uchar16>((iv >> 10) + 0xd800);
					*dst++ = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
				}
			}
			return dst;
		}

		uchar32* write(uchar32 rune, uchar32* dest)
		{
			*dest = rune;
			return dest + 1;
		}

		uchar const*	read(uchar const* str, uchar32& out_c)
		{
			if (is_eos(str))
			{
				out_c = '\0';
				return str;
			}
			uchar c = *str++;
			out_c = c;
			return str;
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

		// Read utf-8 rune backwards (used in string reverse)
		uchar8  const *	rread(uchar8  const * str, uchar32& out_c)
		{
			uchar8 c = str[-1];
			s32 l = 0;
			if ((c & 0x80) == 0x00) { l = 1; }
			else if ((c & 0xe0) == 0xc0) { l = 2; }
			else if ((c & 0xf0) == 0xe0) { l = 3; }
			else if ((c & 0xf8) == 0xf0) { l = 4; }

			out_c = 0;
			for (s32 i = 0; i<l; i++) {
				c = str[0-i];
				out_c = out_c << 8;
				out_c = out_c | c;
			}
			return str - l;
		}

		uchar16 const*	read(uchar16 const* str, uchar32& out_c)
		{
			if (is_eos(str))
			{
				out_c = '\0';
				return str;
			}
			uchar16 c = *str;

			s32 l = 0;
			if (c < 0xd800) { l = 1; }
			else if (c < 0xdc00) { l = 2; }

			out_c = 0;
			for (s32 i = 0; i<l; i++) {
				c = str[i];
				out_c = out_c << 16;
				out_c = out_c | c;
			}
			return str + l;
		}

		uchar32 const *	read(uchar32 const * str, uchar32& out_c)
		{
			out_c = *str;
			return str + 1;
		}

		bool		is_eos(uchar const* str)
		{
			return *str == 0;
		}
		bool		is_eos(uchar8 const* str)
		{
			return *str == 0;
		}
		bool		is_eos(uchar16 const* str)
		{
			return *str == 0;
		}
		bool		is_eos(uchar32 const* str)
		{
			return *str == 0;
		}

		bool		is_crln(uchar const* ustr)
		{
			return ustr[0] == '\r' && ustr[1] == '\n';
		}

		bool		is_crln(uchar8 const* ustr)
		{
			return ustr[0] == '\r' && ustr[1] == '\n';
		}

		bool		is_crln(uchar16 const* ustr)
		{
			return ustr[0] == '\r' && ustr[1] == '\n';
		}

		bool		is_crln(uchar32 const* ustr)
		{
			return ustr[0] == '\r' && ustr[1] == '\n';
		}


	}	// utf
};

namespace xcore
{
	#define __XBASE_GENERIC_STRING_FUNCS__
	
	namespace utf8
	{
		#include "x_string_funcs.cpp"
	}
	namespace utf32
	{
		#include "x_string_funcs.cpp"
	}
};

/**
 *  END xCore namespace
 */
