#ifndef __XBASE_STRING_UTF_H__
#define __XBASE_STRING_UTF_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_va_list.h"
#include "xbase\x_integer.h"
#include "xbase\x_carray.h"

/**
* xCore namespace
*/
namespace xcore
{
	namespace utf
	{
		s32  		strlen		(ustr8 const* _str, ustr8 const** _end = NULL);
		s32  		strlen		(ustr16 const* _str, ustr16 const** _end = NULL);

		u32			numBytes	(uchar8 _c);
		u32			numBytes	(uchar16 _c);

		s32			digest		(ustr8  const* _str, ustr8  const* _src_end, uchar8 & _out);
		s32			digest		(ustr16 const* _str, ustr16 const* _src_end, uchar16& _out);

		bool		isLegal		(uchar8  _c);
		bool		isLegal		(uchar16 _c);
		bool		isLegal		(const ustr8 * _src, const ustr8 * _src_end = NULL);
		bool		isLegal		(const ustr16* _src, const ustr16* _src_end = NULL);

		bool		convert		(uchar16 _from, uchar8  _to);
		bool		convert		(uchar8  _from, uchar16 _to);

		s32			convert		(ustr8  const* _src, ustr8  const* _src_end, ustr16*& _dst, ustr16 const* _dst_end);
		s32			convert		(ustr16 const* _src, ustr16 const* _src_end, ustr8 *& _dst, ustr8  const* _dst_end);

	}	// utf  
};	// xCore namespace



namespace xcore
{
	namespace utf
	{
		inline s32  		strlen    			(ustr8 const* _str, ustr8 const** _end)	// We assume the string is a valid UTF8 string!
		{
			s32 len = 0;
			ustr8 const* e = (_end != NULL) ? *_end : NULL;
			if (e != NULL)
			{
				ustr8 const* s = _str;
				while (s < e)
				{
					if ((s->c & 0xc0) != 0x80) len++;
					s++;
				}
			}
			else
			{
				ustr8 const* s = _str;
				while (s->c != 0)
				{
					if ((s->c & 0xc0) != 0x80) len++;
					s++;
				}
				if (_end != NULL)
					*_end = s;
			}
			return len;
		}

		inline s32  		strlen    			(ustr16 const* _str, ustr16 const** _end)
		{
			s32 len = 0;
			ustr16 const* e = (_end != NULL) ? *_end : NULL;
			if (e != NULL)
			{
				ustr16 const* s = _str;
				while (s < e)
				{
					++len;
					u16 ch = s->c;
					++s;
					if (ch >= 0xD800 && ch <= 0xDBFF)
					{
						if (s >= e)
							return -1;	// Invalid string range

						ch = s->c;
						++s;
						if (ch < 0xDC00 || ch > 0xDFFF)
							return -1;	// Invalid UTF-16
					}
				}
			}
			else
			{
				ustr16 const* s = _str;
				while (s->c != 0)
				{
					++len;
					u16 ch = s->c;
					++s;
					if (ch >= 0xD800 && ch <= 0xDBFF)
					{
						if (s->c == 0)
							return -1;	// Invalid string range

						ch = s->c;
						++s;
						if (ch < 0xDC00 || ch > 0xDFFF)
							return -1;	// Invalid UTF-16
					}
				}
			}
			return len;
		}

		inline u32			numBytes			(uchar8 _c)		// We assume the character is a valid UTF8 character!
		{
			u32 n;
			u8 c = _c.c & 0xFF;
			if (_c.c < 0x80)	// ASCII
			{
				n=1;
			}
			else
			{
				switch (0xF0 & _c.c)
				{
				case 0xE0: n=3; break;
				case 0xF0: n=4; break;
				default:   n=2; break;
				}
			}
			return n;
		}

		inline u32			numBytes			(uchar16 _c)
		{
			u16 const ch = _c.c & 0xFF;
			if (ch >= 0xD800 && ch <= 0xDBFF)
				return 4;
			return 2;
		}


		inline s32			digest		(ustr8  const* _str, ustr8  const* _src_end, uchar8 & _out)
		{
			if (_str >= _src_end)
				return 0;

			uchar8 c((u32)_str->c);
			_str++;

			const s32 n = numBytes(c);
			if (n==2)
			{
				if (_str >= _src_end)
					return -1;
				c.c = c.c | ((u32)_str->c << 8);
				_str++;
			}
			_out = c;
			return n;
		}

		inline s32			digest		(ustr16 const* _str, ustr16 const* _src_end, uchar16& _out)
		{
			if (_str >= _src_end)
				return 0;

			uchar16 c((u32)_str->c);
			_str++;

			const s32 n = numBytes(c);
			if (n==2)
			{
				if (_str >= _src_end)
					return -1;
				c.c = c.c | ((u32)_str->c << 16);
				_str++;
			}
			_out = c;
			return n;
		}

		inline s32			write		(ustr8* _dst, ustr8 const* _dst_end, uchar8 ch)
		{

		}


		inline bool			isLegal				(uchar8  _c)
		{
			u8 const a = _c.c & 0xFF;
			if (a<0x80)		// ASCII, 0 - 127
				return true;

			u8 b;
			switch (0xF0 & a)
			{
			case 0xF0: // 4
				{
					b = (_c.c >> 24) & 0xFF;
					if (b<0x80 || b>0xBF) return false;
				};
			case 0xE0: // 3 
				{
					b = (_c.c >> 16) & 0xFF;
					if (b<0x80 || b>0xBF) return false;
				};
			default:   // 2
				{
					b = (_c.c >> 8) & 0xFF;
					if (b<0x80 || b>0xBF) return false;
				};
			}

			switch (a)
			{
				// no fall-through in this inner switch
			case 0xE0: if (b < 0xA0) return false; break;
			case 0xED: if (b > 0x9F) return false; break;
			case 0xF0: if (b < 0x90) return false; break;
			case 0xF4: if (b > 0x8F) return false; break;
			default:   if (b < 0x80) return false;
			}

			return true;
		}

		inline bool			isLegal				(uchar16 _c)
		{
			u16 const ch1 = _c.c & 0xFFFF;
			if (ch1 >= 0xD800 && ch1 <= 0xDBFF)
			{
				u16 const ch2 = ((_c.c>>16) & 0xFFFF);
				return (ch2 >= 0xDC00 && ch2 <= 0xDFFF);
			}
			else
			{
				return !(ch1 >= 0xDC00 && ch1 <= 0xDFFF);
			}
		}

		inline bool			isLegal		(const ustr8 * _src, const ustr8 * _src_end)
		{
			const ustr8* s = _src;
			uchar8 ch;
			do
			{
				s32 n = digest(s, _src_end, ch);
				if (n<=0)
					return false;
				if (!isLegal(ch))
					return false;
				s += n;
			} while(s<_src_end);
			return true;
		}

		inline bool			isLegal		(const ustr16* _src, const ustr16* _src_end)
		{
			const ustr16* s = _src;
			uchar16 ch;
			do
			{
				s32 n = digest(s, _src_end, ch);
				if (n<=0)
					return false;
				if (!isLegal(ch))
					return false;
				s += n;
			} while(s<_src_end);
			return true;
		}

		static const s32 halfShift  = 10; // used for shifting by 10 bits

		static const u32 halfBase = 0x00010000;
		static const u32 halfMask = 0x000003FF;

		static const u32 byteMask = 0xBF;
		static const u32 byteMark = 0x80; 

		inline bool			convert		(uchar16 _from, uchar8  _to)
		{
			u16 ch = _from.c & 0xFFFF;
			// If we have a surrogate pair, convert to u32 first
			if (ch >= 0xD800 && ch <= 0xDBFF)
			{
				u32 ch2 = (_from.c>>16) & 0xFFFF;

				// If it's a low surrogate, convert to u32
				if (ch2 >= 0xDC00 && ch2 <= 0xDFFF)
				{
					ch = ((ch - 0xD800) << halfShift) + (ch2 - 0xDC00) + halfBase;
				}
				else 
				{
					// it's an unpaired high surrogate
					return false;
				}
			}
			else
			{
				if (ch >= 0xDC00 && ch <= 0xDFFF)
					return false;
			}

			// Figure out how many bytes the result will require
			u32 bytesToWrite;
			if      (ch < (u32)0x80)     { bytesToWrite = 1;}
			else if (ch < (u32)0x800)    { bytesToWrite = 2; }
			else if (ch < (u32)0x10000)  { bytesToWrite = 3; }
			else if (ch < (u32)0x110000) { bytesToWrite = 4; }
			else                         { return false; }

			static const u8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

			switch (bytesToWrite)
			{ // note: everything falls through.
			case 4: _to.c = (u8)((ch | byteMark) & byteMask); _to.c = _to.c << 8; ch >>= 6;
			case 3: _to.c = (u8)((ch | byteMark) & byteMask); _to.c = _to.c << 8; ch >>= 6;
			case 2: _to.c = (u8)((ch | byteMark) & byteMask); _to.c = _to.c << 8; ch >>= 6;
			case 1: _to.c = (u8) (ch | firstByteMark[bytesToWrite]);
			}

			return true;
		}

		inline bool			convert		(uchar8  _from, uchar16 _to)
		{
			// Do this check whether lenient or strict
			if (!isLegal(_from))
				return false;

			u16 const bytesToRead = numBytes(_from);

			u32 ch8 = _from.c;
			u32 ch16 = 0;

			switch (bytesToRead)
			{
			case 4: ch16 += ch8 & 0xFF; ch8=ch8>>8; ch16<<=6;
			case 3: ch16 += ch8 & 0xFF; ch8=ch8>>8; ch16<<=6;
			case 2: ch16 += ch8 & 0xFF; ch8=ch8>>8; ch16<<=6;
			case 1: ch16 += ch8 & 0xFF; break;
			case 0: 
			default:
				return false;
				break;
			}
			static const u32 offsetsFromUTF8[5] = { 0/*UNUSED*/, 0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL };
			ch16 -= offsetsFromUTF8[bytesToRead];

			if (ch16 <= 0x0000FFFF)	// Target is a character <= 0xFFFF
			{	
				if (ch16 >= 0xD800 && ch16 <= 0xDFFF)	// UTF-16 surrogate values are illegal
					return false;
				_to.c = ch16;
			} 
			else if (ch16 > 0x0010FFFF)
			{
				return false;
			}
			else
			{
				ch16 -= halfBase;
				_to.c = (u16)((ch16 >> halfShift) + 0xD800);
				_to.c = _to.c | (((ch16 & halfMask) + 0xDC00)<<16);
			}

			return false;
		}

		inline s32			convert		(ustr8  const* _src, ustr8  const* _src_end, ustr16*& _dst, ustr16 const* _dst_end)
		{
			s32 n = 0;
			ustr8  const* src = _src;
			while (src < _src_end)
			{
				uchar8 ch8;
				s32 const srcn = digest(src, _src_end, ch8);
				if (srcn<=0)
					return srcn;

				uchar16 ch16;
				if (!convert(ch8, ch16))
					return -1;

				u32 const dstn = (numBytes(ch16) >> 1) - 1;
				if ((_dst+dstn)<_dst_end)
				{
					switch (dstn)
					{
					case 1:
						_dst->c = (ch16.c & 0xFFFF); 
						_dst++;
						ch16.c >>= 16;
					case 0:
						_dst->c = (ch16.c & 0xFFFF); 
						_dst++;
						break;
					}
				}
				else
				{
					return -1;
				}
				++n;
			}
			return n;
		}

		inline s32			convert		(ustr16 const* _src, ustr16 const* _src_end, ustr8 *& _dst, ustr8  const* _dst_end)
		{
			s32 n = 0;
			ustr16 const* src = _src;
			while (src < _src_end)
			{
				uchar16 ch16;
				s32 const srcn = digest(src, _src_end, ch16);
				if (srcn<=0)
					return srcn;

				uchar8 ch8;
				if (!convert(ch16, ch8))
					return -1;

				u32 const dstn = numBytes(ch8);
				if ((_dst+dstn)<=_dst_end)
				{
					switch (dstn)
					{
					case 4: _dst->c=ch8.c&0xFF; _dst++; ch8.c>>=8;
					case 3: _dst->c=ch8.c&0xFF; _dst++; ch8.c>>=8;
					case 2: _dst->c=ch8.c&0xFF; _dst++; ch8.c>>=8;
					case 1: _dst->c=ch8.c&0xFF; 
					}
				}
				else
				{
					return -1;
				}

				++n;
			}
			return n;
		}
	}
}

#endif    ///< __XBASE_STRING_UTF_H__

