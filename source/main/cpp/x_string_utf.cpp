#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"
#include "xbase\x_integer.h"
#include "xbase\x_memory_std.h"
#include "xbase\x_string_ascii.h"
#include "xbase\x_string_utf.h"


namespace xcore
{
	namespace UTF
	{
		s32			utf32_to_ascii(uchar32 rune, char* dest)
		{
			if (rune <= 0x7f) {
				*dest++ = (char)rune;
			} else {
				*dest++ = '?';
			}
			return 1;
		}

		static u8 sUTF8LC[] = { 0, 0, 0xc0, 0xe0, 0xf0 };
		s32			utf32_to_utf8(uchar32 rune, ustr8* dest)
		{
			s32 len = 0;
			if (rune <= 0x7f) { len = 1; }
            else if (rune < 0x0800) { len = 2; }
            else if (rune < 0xd800) { len = 3; }
            else if (rune < 0xe000) { len = 0; }
            else if (rune < 0x010000) { len = 3; }
            else if (rune < 0x110000) { len = 4; }

			uchar8 res[4];
			switch (len) {
                case 4: res[3] = (rune & 0x3f) | 0x80; rune = rune >> 6;
                case 3: res[2] = (rune & 0x3f) | 0x80; rune = rune >> 6;
                case 2: res[1] = (rune & 0x3f) | 0x80; rune = rune >> 6;
                default: len = 0;
            };
			res[0] = (uchar8)rune | sUTF8LC[len];

			if (dest != NULL) {
				for (s32 i = 0; i < len; ++i) {
					*dest++ = res[i];
				}
			}
			return len;
		}

		s32			utf32_to_utf16(uchar32 rune, ustr16* dest)
		{
			s32 len = 0;
			if (rune < 0xd800) { len = 1; }
            else if (rune < 0xe000) { len = 0; }
            else if (rune < 0x010000) { len = 1; }
            else if (rune < 0x110000) { len = 2; }

			if (len == 1) {
				if (dest != NULL) {
					*dest = (uchar16)rune;
					++dest;
				}
            } else if (dest != NULL) {
				// 20-bit intermediate value
	            u32 const iv = rune - 0x10000;
	            dest[0] = static_cast<uchar16>((iv >> 10) + 0xd800);
	            dest[1] = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
			}
			return len;
		}

		s32			ascii_to_utf32(char const* str, uchar32& out_char)
		{
			return read_utf8((ustr8 const*)str, out_char);
		}

		s32			utf8_to_utf32(ustr8 const* str, uchar32& out_char)
		{
			uchar8 c = str->c;
			s32 l = 0;
			if ((c & 0x80) == 0x00) { l = 1; }
			else if ((c & 0xe0) == 0xc0) { l = 2; }
			else if ((c & 0xf0) == 0xe0) { l = 3; }
			else if ((c & 0xf8) == 0xf0) { l = 4; }

			out_char = 0;
			for (s32 i = 0; i<l; i++) {
				c = str[i].c;
				out_char = out_char << 8;
				out_char = out_char | c;
			}
			return l;
		}

		s32			utf16_to_utf32(ustr16 const * str, uchar32& out_char)
		{
			uchar16 c = str->c;
			s32 l = 0;
			if (c < 0xd800) { l = 1; }
			else if (c < 0xdc00) { l = 2; }

			for (s32 i = 0; i<l; i++) {
				c = str[i].c;
				out_char = out_char << 16;
				out_char = out_char | c;
			}
			return l;
		}



		s32			read_ascii(char const* str, uchar32& out_char)
		{
			return read_utf8((ustr8 const*)str, out_char);
		}

		s32			read_utf8(ustr8 const* str, uchar32& out_char)
		{
			uchar8 c = str->c;
			s32 l = 0;
            if ((c & 0x80) == 0x00) { l = 1; }
            else if ((c & 0xe0) == 0xc0) { l = 2; }
            else if ((c & 0xf0) == 0xe0) { l = 3; }
            else if ((c & 0xf8) == 0xf0) { l = 4; }

			out_char = 0;
			for (s32 i=0; i<l; i++) {
				c = str[i].c;
				out_char = out_char << 8;
				out_char = out_char | c;
			}
			return l;
		}

		s32			read_utf16(ustr16 const * str, uchar32& out_char)
		{
			uchar16 c = str->c;
			s32 l = 0;
			if (c < 0xd800) { l = 1; }
			else if (c < 0xdc00) { l = 2; }

			for (s32 i=0; i<l; i++) {
				c = str[i].c;
				out_char = out_char << 16;
				out_char = out_char | c;
			}
			return l;
		}

		s32			read_utf32(ustr32 const * str, uchar32& out_char)
		{
			out_char = str->c;
			return 1;
		}

		s32			convert		(char const * from, uchar32 * to)
		{
			return convert((ustr8 const *)from, to);
		}

		s32			convert		(ustr8 const * from, uchar32 * to)
		{
			s32 l = 0;
			while (!iseos(from)) {
				uchar32 c;
				s32 n = read_utf8(from, c);
				if (to != NULL)
					*to++ = c;
				from += n;
				l++;
			}
			return l;
		}
		s32			convert		(ustr16 const * from, uchar32 * to)
		{
			s32 l = 0;
			while (!iseos(from)) {
				uchar32 c;
				s32 n = read_utf16(from, c);
				if (to != NULL)
					*to++ = c;
				from += n;
				l++;
			}
			return l;
		}

		s32			convert		(ustr32 const * from, ustr8  * to)
		{
			s32 len = 0;
			while (!iseos(from)) {
				s32 l = utf32_to_utf8(from->c, to);
				to += l;
				from += 1;
				len += l;
			}
			return len;
		}
		s32			convert		(ustr32 const * from, ustr16 * to)
		{
			s32 len = 0;
			while (!iseos(from)) {
				s32 l = utf32_to_utf16(from->c, to);
				to += l;
				from += 1;
				len += l;
			}
			return len;
		}

		s32			len(ustr8 const* str)
		{
			uchar c;
			s32 len = 0;
			while (!iseos(str)) {
				s32 l = read_utf8(str, c);
				len += l;
				str += l;
			}
			return len;
		}

		s32			len(ustr32 const* str)
		{
			uchar c;
			s32 len = 0;
			while (!iseos(str)) {
				s32 l = read_utf32(str, c);
				len += l;
				str += l;
			}
			return len;
		}

		bool		iseos		(ustr8  const * str)
		{
			return str->c == 0;
		}
		bool		iseos		(ustr16 const * str)
		{
			return str->c == 0;
		}
		bool		iseos		(ustr32 const * str)
		{
			return str->c == 0;
		}

		bool		iscrln		(ustr8  const* ustr)
		{
			return ustr[0].c == '\r' && ustr[1].c == '\n';
		}

		bool		iscrln		(ustr16 const* ustr)
		{
			return ustr[0].c == '\r' && ustr[1].c == '\n';
		}

		bool		iscrln		(ustr32 const* ustr)
		{
			return ustr[0].c == '\r' && ustr[1].c == '\n';
		}


	}	// UTF



	//------------------------------------------------------------------------------
	uchar32			PeekChar(const uchar32 * str, s32* len = NULL)
	{
		uchar32 c = *str;
		if ((c & 0x80) == 0x00) {
			if (len != NULL)
				*len = 1;
			return (uchar32)c;
		}

		s32 l = 0;
		if ((c & 0xe0) == 0xc0) { l = 2; }
		else if ((c & 0xf0) == 0xe0) { l = 3; }
		else if ((c & 0xf8) == 0xf0) { l = 4; }

		uchar32 c32 = 0;
		for (s32 i = 0; i<l; i++) {
			c = str[i];
			c32 = c32 << 8;
			c32 = c32 | c;
		}
		if (len != NULL)
			*len = l;
		return c32;
	}

	//------------------------------------------------------------------------------
	uchar32			ReadChar(uchar32 const*& str)
	{
		s32 l = 0;
		uchar32 c = PeekChar(str, &l);
		str += l;
		return c;
	}

	//------------------------------------------------------------------------------
	bool	StrToBool(const uchar32 * str)
	{
		const uchar32* end;
		return StrToBool(str, &end);
	}

	//------------------------------------------------------------------------------
	bool	StrToBool(const uchar32 * str, const uchar32 ** scan_end)
	{
		ASSERT(str != NULL);

		// Skip whitespace.
		while (PeekChar(str) == ' ') {
			ReadChar(str);
		}

		uchar32 c;				// Current character.
		c = PeekChar(str);		// Save sign indication.

		const bool   boolean_value_values[] = { true, false,  true,   false, true, false, false };
		const char * boolean_value_strings[] = { "yes", "no", "true", "false", "on", "off", NULL };
		if (IsAlpha(c))
		{
			const bool*  value_values = boolean_value_values;
			const char** value_strs = boolean_value_strings;

			c = ToLower(c);
			while (*value_strs != NULL)
			{
				const char* value_str = *value_strs;
				if (value_str[0] == c)
				{
					uchar32 c1, c2;
					do
					{
						c1 = ReadChar(str);
						c2 = *value_str++;
						if ((c1 == '\0' || c1 == ' ') && (c2 == '\0'))
						{
							return *value_values;
						}
						else if (c2 == '\0')
						{
							break;
						}
						c1 = ToLower(c1);
					} while (c1 == c2);
				}
				value_strs++;
				value_values++;
			}
		}
		return false;
	}

	/**
	*------------------------------------------------------------------------------
	* Author:
	*     Virtuos Games
	* Summary:
	*     atod64          - Converts a string value to integer base from a particular
	* Arguments:
	*        str            - Source string encoded with a particular base
	*        Base           - Base of the numeric string
	* Returns:
	*        Actual integer number
	* Description:
	*      Converts a string that has been encoded into an integer of a particular base
	*      into a actual atomic integer of a particular size (32vs64)bits. If the
	*      string contains '_' or ':' characters they will be ignore.
	* See Also:
	*      atod32 atoi32 atoi64 atof32 atof64
	*------------------------------------------------------------------------------
	*/
	s64 StrToS64(uchar32 const * str, s32 base)
	{
		ASSERT(str != NULL);
		ASSERT(base > 2);
		ASSERT(base <= (26 + 26 + 10));

		// Skip whitespace.
		while (PeekChar(str) == ' ') {
			ReadChar(str);
		}

		uchar32 c;				// Current character.
		c = PeekChar(str);		// Save sign indication.
		uchar32 sign = c;		// If '-', then negative, otherwise positive.

								// Skip sign.
		if ((c == '-') || (c == '+')) {
			c = ReadChar(str);
		}

		s64  total = 0;   // Current total.
		s32  validBase;
		// Decode the rest of the string
		for (;; c = ReadChar(str))
		{
			if ((c >= '0') && (c <= '9'))
			{
				validBase = c - '0';
			}
			else if ((c >= 'a') && (c <= 'z'))
			{
				validBase = c - 'a' + 10;
			}
			else if ((c >= 'A') && (c <= 'Z'))
			{
				validBase = c - 'A' + 10 + 26;
			}
			else if (c == '_' || c == ':')
			{
				// Ignore
				continue;
			}
			else
			{
				// Negate the total if negative.
				if (sign == '-')
					total = -total;

				// Any other character is bad news
				//return total;
				break;
			}

			ASSERT(validBase >= 0);
			ASSERT(validBase <  base);

			// Accumulate digit.
			total = (base * total) + validBase;
		}

		// No way to get here
		return total;
	}

	// <COMBINE atod64 >
	s32 StrToS32(uchar32 const * str, s32 base)
	{
		return (s32)StrToS64(str, base);
	}


	xstring::xstring()
	{
	}

	xstring::xstring(slice& block)
		: mSlice()
	{
	}

	xstring::xstring(xstring const & rhs)
		: mSlice(rhs.mSlice)
	{
	}

	xstring::xstring(x_iallocator* allocator)
	{
		slice::alloc(mSlice, allocator, 0);
	}

	xstring::xstring(x_iallocator* allocator, u32 initial_len)
	{
		slice::alloc(mSlice, allocator, initial_len);
	}


	xstring::~xstring()
	{
	}

	xstring			xstring::operator[](s32 i) const
	{
		if ((mSlice.mFrom + (i * sizeof(uchar32))) < mSlice.mTo) {
			return view(mSlice.mFrom + i, mSlice.mTo);
		}
		return xstring();
	}

	xstring&		xstring::operator=(xstring::overwrite const& other)
	{
		if (other.mType == xstring::operation::TYPE_PCCHAR)
		{
			char const* str = other.mPCCharStr;
			uchar32* str32 = (uchar32*)&mSlice.mData->mData[0];
			uchar32* str32_end = str32 + len();

			uchar32 c;
			s32 l = UTF::read_ascii(str, c);
			while (c != '\0')
			{
				if (str32 < str32_end)
				{
					*str32++ = c;
					if (str32 == str32_end)
					{
						str32[-1] = '\0';
						break;
					}
					str += l;
					l = UTF::read_ascii(str, c);
				}
				else
				{
					break;
				}
			};
		}
		else if (other.mType == xstring::operation::TYPE_PCUSTR8)
		{
			ustr8 const* str = other.mPCUStr8Str;
			uchar32* str32 = (uchar32*)&mSlice.mData->mData[0];
			uchar32* str32_end = str32 + len();

			uchar32 c;
			s32 l = UTF::read_utf8(str, c);
			while (c != '\0')
			{
				if (str32 < str32_end)
				{
					*str32++ = c;
					str += l;
					l = UTF::read_utf8(str, c);
					if (l == 0)
						break;
				}
				else
				{
					break;
				}
			};
		}
		else if (other.mType == xstring::operation::TYPE_PCUSTR16)
		{
			ustr16 const* str = other.mPCUStr16Str;
			uchar32* str32 = (uchar32*)&mSlice.mData->mData[0];
			uchar32* str32_end = str32 + len();

			uchar32 c;
			s32 l = UTF::read_utf16(str, c);
			while (c != '\0')
			{
				if (str32 < str32_end)
				{
					*str32++ = c;
					str += l;
					l = UTF::read_utf16(str, c);
					if (l == 0)
						break;
				}
				else
				{
					break;
				}
			};
		}
		else if (other.mType == xstring::operation::TYPE_PCUSTR32)
		{
			ustr32 const* str = other.mPCUStr32Str;
			uchar32* str32 = (uchar32*)&mSlice.mData->mData[0];
			uchar32* str32_end = str32 + len();

			uchar32 c;
			s32 l = UTF::read_utf32(str, c);
			while (c != '\0')
			{
				if (str32 < str32_end)
				{
					*str32++ = c;
					str += l;
					UTF::read_utf32(str, c);
				}
				else
				{
					break;
				}
			};
		}
		else if (other.mType == xstring::operation::TYPE_PCXSTRING)
		{
			uchar32* src32 = (uchar32*)&other.mPCXStringStr->mSlice.mData->mData[0];
			uchar32* src32_end = src32 + other.mPCXStringStr->len();

			uchar32* dst32 = (uchar32*)&mSlice.mData->mData[0];
			uchar32* dst32_end = dst32 + len();

			s32 i = 0;
			uchar32 c = *src32++;
			while (c != '\0')
			{
				*dst32++ = c;
				if (src32 < src32_end && dst32 < dst32_end)
				{
					c = *src32++;
				}
				else
				{
					break;
				}
			};
		}
		return *this;
	}

	xstring&		xstring::operator=(xstring::insert const& other)
	{
		if (other.mType == xstring::operation::TYPE_PCUSTR32)
		{
			s32 l = 1 + UTF::len(other.mPCUStr32Str);
			uchar32* str32 = (uchar32*)&mSlice.mData->mData[0];
			if (l > mSlice.len()) {
				str32 = (uchar32*)mSlice.resize(l * sizeof(uchar32));
			}
			for (s32 i = 0; i < l; ++i)
			{
				str32[i] = other.mPCUStr32Str[i].c;
			}
		}
		return *this;
	}

	xstring			xstring::operator +(xstring const& str)
	{
		xstring result(*this);
		s32 const length = len() + str.len();
		result.mSlice.resize(length * sizeof(uchar32));
		x_memcopy(result.str_ptr(), str.str_ptr(), length * sizeof(uchar32));
		result.mSlice.mTo = length * sizeof(uchar32);
		return result;
	}

	xstring&		xstring::operator+=(xstring const& str)
	{
		s32 const length = len() + str.len();
		mSlice.resize(length * sizeof(uchar32));
		x_memcopy(str_ptr(), str.str_ptr(), length * sizeof(uchar32));
		mSlice.mTo = length * sizeof(uchar32);
		return *this;
	}

	bool			xstring::operator==(char const* ascii_str) const
	{
		uchar32 ca32;
		uchar32 c32;

		s32 l = 0;
		uchar32 const* s = str_ptr();
		do {
			UTF::ascii_to_utf32(&ascii_str[l], ca32);
			c32 = s[l];
			l++;
		} while (c32 == ca32 && c32 != 0);

		return c32 == ca32;
	}

	bool			xstring::operator==(ustr8 const* utf8_str) const
	{
		uchar32 ca32;
		uchar32 c32;

		s32 l = 0;
		uchar32 const* s = str_ptr();
		do {
			s32 n = UTF::utf8_to_utf32(utf8_str, ca32);
			utf8_str += n;
			c32 = s[l];
			l++;
		} while (c32 == ca32 && c32 != 0);

		return c32 == ca32;
	}

	bool			xstring::operator==(ustr16 const* utf16_str) const
	{
		uchar32 ca32;
		uchar32 c32;

		s32 l = 0;
		uchar32 const* s = str_ptr();
		do {
			s32 n = UTF::utf16_to_utf32(utf16_str, ca32);
			utf16_str += n;
			c32 = s[l];
			l++;
		} while (c32 == ca32 && c32 != 0);

		return c32 == ca32;
	}

	bool			xstring::operator==(ustr32 const* utf32_str) const
	{
		uchar32 c32a;
		uchar32 c32b;

		s32 l = 0;
		uchar32 const* s = str_ptr();
		do {
			c32a = utf32_str[l].c;
			c32b = s[l];
			l++;
		} while (c32a == c32b && c32a != 0);

		return c32a == c32b;
	}

	bool			xstring::operator==(xstring const & str) const
	{
		uchar32 c32a;
		uchar32 c32b;

		s32 l = 0;
		uchar32 const* s1 = str_ptr();
		uchar32 const* s2 = str.str_ptr();
		do {
			c32a = s2[l];
			c32b = s1[l];
			l++;
		} while (c32a == c32b && c32a != 0);

		return c32a == c32b;
	}

	uchar32			xstring::readat(s32 i) const
	{
		uchar32 const* str = str_ptr();
		if (i >= 0 && i < (s32)mSlice.mTo)
		{
			return str[i];
		}
		else
		{
			return '\0';
		}
	}

	void			xstring::writeto(xwriter* writer) const
	{
		if (mSlice.len() > 0) {
			writer->write((xbyte const*)&mSlice.mData->mData[mSlice.mFrom], mSlice.len());
		}
	}

	xstring			xstring::copy() const
	{
		xstring s;
		s.mSlice.mData = mSlice.mData->resize(mSlice.mFrom, mSlice.mTo);
		s.mSlice.mFrom = 0;
		s.mSlice.mTo = mSlice.len();
		return s;
	}

	typedef uchar(*transform_functor)(s32 i, uchar32* begin, uchar32* end);

	static void		transform(uchar32* str, s32 len, transform_functor _transformation)
	{
		uchar32* end = str + len;
		for (s32 i = 0; i < len; ++i) {
			str[i] = _transformation(i, str, end);
		}
	}

	static uchar	transform_tolower(s32 i, uchar32* begin, uchar32* end)
	{
		uchar32 c = begin[i];
		if (c >= 'A' && c <= 'Z') {
			c = uchar('a') + (c - uchar('A'));
		}
		return c;
	}

	xstring&		xstring::tolower()
	{
		transform(str_ptr(), len(), &transform_tolower);
		return *this;
	}

	static uchar	transform_toupper(s32 i, uchar32* begin, uchar32* end)
	{
		uchar32 c = begin[i];
		if (c >= 'a' && c <= 'z') {
			c = uchar('A') + (c - uchar('a'));
		}
		return c;
	}

	xstring&		xstring::toupper()
	{
		transform(str_ptr(), len(), &transform_toupper);
		return *this;
	}

	static uchar	transform_tocamel(s32 i, uchar32* begin, uchar32* end)
	{
		uchar32 p = '\0';
		if (i > 0) {
			p = begin[i - 1];
		}
		uchar32 c = begin[i];
		if (p == '\0') {
			if (c >= 'a' && c <= 'z') {
				c = uchar('A') + (c - uchar('a'));
			}
		}
		return c;
	}

	xstring&		xstring::tocamel()
	{
		transform(str_ptr(), len(), &transform_tocamel);
		return *this;
	}

	xstring			xstring::view(s32 from, s32 to) const
	{
		xstring s;
		s.mSlice = mSlice.view(from * sizeof(uchar32), to * sizeof(uchar32));
		return s;
	}

	uchar32*		xstring::str_ptr()
	{
		return (uchar32*)&mSlice.mData->mData[mSlice.mFrom];
	}

	uchar32 const*	xstring::str_ptr() const
	{
		return (uchar32*)&mSlice.mData->mData[mSlice.mFrom];
	}


	static void xstring_usecase()
	{
		xstring strings(gCreateSystemAllocator());

		xstring str1(strings);
		str1 = "Hello UTF World!";

		xstring view_UTF = str1.find("UTF", xstring::SELECT);

		str1[15] = xstring::overwrite('?');

		xstring str3(str1);
		str3 = str1.left_of(view_UTF) + "SLICE" + str1.right_of(view_UTF);
		str3.tolower();
		str3.tocamel();

		xstring str4(strings);
		str4 = str1 + str3;
	}
};

/**
 *  END xCore namespace
 */
