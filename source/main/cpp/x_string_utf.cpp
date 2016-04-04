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
		static sUTF8LC[] = { 0, 0, 0xc0, 0xe0, 0xf0 };
		static s32 uchar32to8(uchar32 rune, ustr8* dest)
		{
			s32 len = 0;
			if (c <= 0x7f) { len 1; }
            else if (c < 0x0800) { len = 2; }
            else if (c < 0xd800) { len = 3; }
            else if (c < 0xe000) { len = 0; }
            else if (c < 0x010000) { len = 3; }
            else if (c < 0x110000) { len = 4; }

			uchar8 res[4];
			switch (len) {
                case 4: res[3] = (c & 0x3f) | 0x80; c = c >> 6;
                case 3: res[2] = (c & 0x3f) | 0x80; c = c >> 6;
                case 2: res[1] = (c & 0x3f) | 0x80; c = c >> 6;
                default: len = 0;
            };
			res[0] = c | sUTF8LC[len];

            for (s32 i = 0; i < len; ++i) {
                *dest++ = res[i];
            }
			return len;
		}

		static s32 uchar32to16(uchar32 rune, ustr16* dest)
		{
			s32 len = 0;
			if (c < 0xd800) { len = 1; }
            else if (c < 0xe000) { len = 0; }
            else if (c < 0x010000) { len = 1; }
            else if (c < 0x110000) { len = 2; }

			if (len == 1) {
                *dest = c;
                ++dest;
            } else {
				// 20-bit intermediate value
	            u32 const iv = c - 0x10000;
	            dest[0] = static_cast<uchar16>((iv >> 10) + 0xd800);
	            dest[1] = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
			}
			return len;
		}

		s32			readu8(ustr8 const* str, u32& out_char)
		{
			uchar8 c = *str;
			s32 l = 0;
            if ((c & 0x80) == 0x00) { l = 1; }
            else if ((c & 0xe0) == 0xc0) { l = 2; }
            else if ((c & 0xf0) == 0xe0) { l = 3; }
            else if ((c & 0xf8) == 0xf0) { l = 4; }

			for (s32 i=0; i<l; i++)
			{
				c = *str++;
				out_char = out_char << 8;
				out_char = out_char | c;
			}
			return l;
		}

		s32			readu16(ustr16 const * str, u32& out_char)
		{
			uchar16 c = *str;
			s32 l = 0;
			if (c < 0xd800) { l = 1; }
			else if (c < 0xdc00) { l = 2; }

			for (s32 i=0; i<l; i++)
			{
				c = *str++;
				out_char = out_char << 16;
				out_char = out_char | c;
			}
			return l;
		}

		s32			convert		(ustr8  const * from, uchar32 * to)
		{
			s32 l = 0;
			while (!iseos(from))
			{
				u32 c;
				readu8(from, c);
				if (to != NULL)
					*to++ = c;
				l++;
			}
			return l;
		}
		s32			convert		(ustr16 const * from, uchar32 * to)
		{
			s32 l = 0;
			while (!iseos(from))
			{
				u32 c;
				readu16(from, c);
				if (to != NULL)
					*to++ = c;
				l++;
			}
			return l;
		}

		s32			convert		(uchar32 const * from, ustr8  * to)
		{
			s32 len = 0;
			while (!iseos(from))
			{
				s32 l = uchar32to8(*from, to);
				to += l;
				from += 1;
				len += l;
			}
			return len;
		}
		s32			convert		(uchar32 const * from, ustr16 * to)
		{
			s32 len = 0;
			while (!iseos(from))
			{
				s32 l = uchar32to16(*from, to);
				to += l;
				from += 1;
				len += l;
			}
			return len;
		}

		bool		iseos		(ustr8  const * str)
		{
			return *str == 0;
		}
		bool		iseos		(uchar16 const * str)
		{
			return *str == 0;
		}
		bool		iseos		(uchar32 const * str)
		{
			return *str == 0;
		}

		bool		iscrln		(uchar8  const* ustr)
		{
			return *ustr == '\r' && *ustr == '\n';
		}

		bool		iscrln		(uchar16 const* ustr)
		{
			return *ustr == '\r' && *ustr == '\n';
		}

		bool		iscrln		(uchar32 const* ustr)
		{
			return *ustr == '\r' && *ustr == '\n';
		}


	}	// UTF

	xstring::xstring()
	{
	}

	xstring::xstring(xstring & copy)
	: mSlice(copy.mSlice.incref())
	{
	}

	xstring::xstring(x_iallocator* allocator)
	{
		mSlice = slice::alloc(allocator, 0);
	}

	s32				xstring::len() const
	{
		return mSlice.len();
	}

	xstring		xstring::view(s32 from, s32 to)
	{
		xstring s;
		s.mSlice = mSlice.view(from, to);
		return s;
	}

	uchar			xstring::operator[](s32 i) const
	{
		if ((mSlice.mFrom + i) < mSlice.mTo)
		{
			return mSlice.mData->mData[mSlice.mFrom + i];
		}
		return (uchar)0;
	}

	xstring&		xstring::operator=(const char* str)
	{
		s32 len = UTF::convert((ustr8 const*)str, NULL);
		ustr32* str32 = (ustr32*)mSlice.resize(len * sizeof(uchar32));
		UTF::convert((ustr8 const*)str, str32);
	}

	xstring&		xstring::operator=(xstring & other)
	{
		if (&other != this)
		{
			if (other.mSlice.mData == this->mSlice.mData)
			{
				mSlice.mFrom = other.mSlice.mFrom;
				mSlice.mTo = other.mSlice.mTo;
			}
			else
			{
				mSlice.mData->decref();
				mSlice.mData = other.mSlice.mData->incref();
				mSlice.mFrom = other.mSlice.mFrom;
				mSlice.mTo = other.mSlice.mTo;
			}
		}
		return *this;
	}

	void			xstring::writeto(xwriter* writer) const
	{
		if (len() > 0)
		{
			writer->write((xbyte const*)&mSlice.mData->mData[mSlice.mFrom], mSlice.len());
		}
	}

	xstring			xstring::create(s32 len) const
	{
		xstring s;
		s.mSlice.mData = mSlice.mData->resize(mSlice.len());
		s.mSlice.mFrom = 0;
		s.mSlice.mTo = len;
		return s;
	}

	xstring			xstring::copy() const
	{
		xstring s;
		s.mSlice.mData = mSlice.mData->resize(mSlice.len());
		s.mSlice.mFrom = 0;
		s.mSlice.mTo = mSlice.len();
		return s;
	}



	static void xstring_usecase()
	{
		xstring strings(gCreateSystemAllocator());

		xstring string = strings.create(16);
		xstring view_4_8 = string(4, 8);

		uchar c = view_4_8[0];
		view_4_8 = xstring::cursor{ 0, '?' };	// Write a character at first position
	}
};

/**
 *  END xCore namespace
 */
