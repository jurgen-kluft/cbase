#include "xbase/x_runes.h"
#include "xbase/x_printf.h"
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_console.h"

namespace xcore
{
	namespace ascii
	{
		//------------------------------------------------------------------------------
		static inline s32		peek_char(ascii::runes const& src, uchar32& out_c)
		{
			out_c = '\0';
			if (!src.is_empty())
			{
				out_c = src.m_str[0];
				return 1;
			}
			return 0;
		}
		static inline s32		peek_char(ascii::crunes const& src, uchar32& out_c)
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
		static bool				write_char(uchar32 c, ascii::runes& dst)
		{
			if (utf::can_write(dst))
			{
				if (c > 0x7f)
					c = '?';
				*dst.m_end++ = (uchar)c;
				*dst.m_end = '\0';
				dst.m_end = dst.m_end;
				return true;
			}
			return false;
		}

		//------------------------------------------------------------------------------
		static bool				read_char(runes& src, uchar32& out_c)
		{
			s32 const l = peek_char(src, out_c);
			src.m_str += l;
			return l > 0;
		}

		//------------------------------------------------------------------------------
		static bool				read_char(crunes& src, uchar32& out_c)
		{
			s32 l = peek_char(src, out_c);
			src.m_str += l;
			return l > 0;
		}

	};	///< END ascii namespace
};	///< END xcore namespace

