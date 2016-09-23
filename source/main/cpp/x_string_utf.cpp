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
				*dest++ = rune;
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
			res[0] = rune | sUTF8LC[len];

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
					*dest = rune;
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

		s32			convert		(char const * from, uchar32 * to)
		{
			return convert((ustr8 const *)from, to);
		}

		s32			convert		(ustr8 const * from, uchar32 * to)
		{
			s32 l = 0;
			while (!iseos(from)) {
				u32 c;
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
				u32 c;
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
			return ustr[0].c == '\r' && ustr->c == '\n';
		}

		bool		iscrln		(ustr32 const* ustr)
		{
			return ustr[0].c == '\r' && ustr[1].c == '\n';
		}


	}	// UTF

	xstring::xstring()
	{
	}

	xstring::xstring(xstring & copy)
	: mSlice(copy.mSlice)
	{
	}

	xstring::xstring(x_iallocator* allocator)
	{
		slice::alloc(mSlice, allocator, 0);
	}

	xstring::~xstring()
	{
	}

	uchar32 *		xstring::operator[](s32 i)
	{
		if ((mSlice.mFrom + i) < mSlice.mTo) {
			return (uchar32 *)&mSlice.mData->mData[(mSlice.mFrom + i) * sizeof(uchar32)];
		}
		return NULL;
	}

	uchar32 const*	xstring::operator[](s32 i) const
	{
		if ((mSlice.mFrom + i) < mSlice.mTo) {
			return (uchar32 const*)&mSlice.mData->mData[(mSlice.mFrom + i) * sizeof(uchar32)];
		}
		return NULL;
	}

	xstring&		xstring::operator=(const char* str)
	{
		s32 l = UTF::convert((ustr8 const*)str, (uchar32 *)0);
		uchar32* str32 = (uchar32*)&mSlice.mData[0];
		if (l > mSlice.len()) {
			str32 = (uchar32*)mSlice.resize(l * sizeof(uchar32));
		}
		UTF::convert(str, str32);
		return *this;
	}

	xstring&		xstring::operator=(xstring const& other)
	{
		if (&other != this) {
			if (other.mSlice.mData == this->mSlice.mData) {
				mSlice.mFrom = other.mSlice.mFrom;
				mSlice.mTo = other.mSlice.mTo;
			} else {
				mSlice.mData->decref();
				mSlice.mData = other.mSlice.mData->incref();
				mSlice.mFrom = other.mSlice.mFrom;
				mSlice.mTo = other.mSlice.mTo;
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
		result.mSlice.mTo = length;
		return result;
	}

	xstring&		xstring::operator+=(xstring const& str)
	{
		s32 const length = len() + str.len();
		mSlice.resize(length * sizeof(uchar32));
		x_memcopy(str_ptr(), str.str_ptr(), length * sizeof(uchar32));
		mSlice.mTo = length;
		return *this;
	}

	bool			xstring::operator==(char const* ascii_str) const
	{
		uchar32 ca32;
		uchar32 c32;
		
		s32 l = 0;
		do {
			UTF::ascii_to_utf32(&ascii_str[l], ca32);
			c32 = *(operator[](l));
			l++;
		} while (c32 == ca32 && c32 != 0);

		return c32 == ca32;
	}

	bool			xstring::operator==(ustr8 const* utf8_str) const
	{
		uchar32 ca32;
		uchar32 c32;

		s32 l = 0;
		do {
			s32 n = UTF::utf8_to_utf32(utf8_str, ca32);
			utf8_str += n;
			c32 = *(operator[](l));
			l++;
		} while (c32 == ca32 && c32 != 0);

		return c32 == ca32;
	}

	bool			xstring::operator==(ustr16 const* utf16_str) const
	{
		uchar32 ca32;
		uchar32 c32;

		s32 l = 0;
		do {
			s32 n = UTF::utf16_to_utf32(utf16_str, ca32);
			utf16_str += n;
			c32 = *(operator[](l));
			l++;
		} while (c32 == ca32 && c32 != 0);

		return c32 == ca32;
	}

	bool			xstring::operator==(ustr32 const* utf32_str) const
	{
		uchar32 c32a;
		uchar32 c32b;

		s32 l = 0;
		do {
			c32a = utf32_str[l].c;
			c32b = *(operator[](l));
			l++;
		} while (c32a == c32b && c32a != 0);

		return c32a == c32b;
	}

	bool			xstring::operator==(xstring const & str) const
	{
		uchar32 c32a;
		uchar32 c32b;

		s32 l = 0;
		do {
			c32a = *str[l];
			c32b = *(operator[](l));
			l++;
		} while (c32a == c32b && c32a != 0);

		return c32a == c32b;
	}


	void			xstring::writeto(xwriter* writer) const
	{
		if (mSlice.len() > 0) {
			writer->write((xbyte const*)&mSlice.mData->mData[mSlice.mFrom * sizeof(uchar32)], mSlice.len());
		}
	}

	xstring			xstring::copy() const
	{
		xstring s;
		s.mSlice.mData = mSlice.mData->resize(mSlice.mFrom * sizeof(uchar), mSlice.mTo * sizeof(uchar));
		s.mSlice.mFrom = 0;
		s.mSlice.mTo = mSlice.len();
		return s;
	}

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

	xstring			xstring::view(s32 from, s32 to)
	{
		xstring s;
		s.mSlice = mSlice.view(from, to);
		return s;
	}

	uchar32*		xstring::str_ptr()
	{
		return (uchar32*)&mSlice.mData->mData[mSlice.mFrom * sizeof(uchar)];
	}

	uchar32 const*	xstring::str_ptr() const
	{
		return (uchar32*)&mSlice.mData->mData[mSlice.mFrom * sizeof(uchar)];
	}

	static void xstring_usecase()
	{
		xstring strings(gCreateSystemAllocator());

		xstring str1(strings);
		str1 = "Hello UTF World!";

		xstring view_UTF = str1.find("UTF", xstring::SELECT);
		uchar32* c = view_UTF[0];
		*str1[15] = '?';

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
