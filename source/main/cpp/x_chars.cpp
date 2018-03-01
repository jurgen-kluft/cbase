#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_base.h"
#include "xbase/x_buffer.h"
#include "xbase/x_console.h"
#include "xbase/x_debug.h"
#include "xbase/x_string_ascii.h"
#include "xbase/x_string_utf.h"

namespace xcore
{
	bool			ascii_chars::peek(ascii_chars::rune * str, const ascii_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			ascii_chars::cpeek(ascii_chars::rune const* str, const ascii_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			ascii_chars::read(ascii_chars::rune *& str, const ascii_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			ascii_chars::cread(ascii_chars::rune const *& str, const ascii_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			ascii_chars::write(ascii_chars::rune* str, const ascii_chars::rune* end, uchar32 c) const
	{
		if (str < end)
		{
			utf::write(c, str, end);
			return true;
		}
		return false;
	}



	bool			utf8_chars::peek(utf8_chars::rune * str, const utf8_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			utf8_chars::cpeek(utf8_chars::rune const* str, const utf8_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			utf8_chars::read(utf8_chars::rune *& str, const utf8_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			utf8_chars::cread(utf8_chars::rune const *& str, const utf8_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			utf8_chars::write(utf8_chars::rune* str, const utf8_chars::rune* end, uchar32 c) const
	{
		if (str < end)
		{
			utf::write(c, str, end);
			return true;
		}
		return false;
	}






	bool			utf32_chars::peek(utf32_chars::rune * str, const utf32_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			utf32_chars::cpeek(utf32_chars::rune const* str, const utf32_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			utf32_chars::read(utf32_chars::rune *& str, const utf32_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			utf32_chars::cread(utf32_chars::rune const *& str, const utf32_chars::rune* end, uchar32& c) const
	{
		if (str < end)
		{
			c = utf::read(str, end);
			return true;
		}
		return false;
	}

	bool			utf32_chars::write(utf32_chars::rune* str, const utf32_chars::rune* end, uchar32 c) const
	{
		if (str < end)
		{
			utf::write(c, str, end);
			return true;
		}
		return false;
	}


};


