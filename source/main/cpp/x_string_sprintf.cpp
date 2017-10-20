
#ifndef SPU

#include "xbase/x_limits.h"
#include "xbase/x_debug.h"
#include "xbase/x_double.h"
#include "xbase/x_string_ascii.h"
#include "xbase/x_string_utf.h"
#include "xbase/x_console.h"

#ifdef TARGET_PC
	#include <math.h>
#elif defined(TARGET_WII)
	#include <math.h>
#elif defined(TARGET_PS3)
	#include <math.h>
#elif defined(TARGET_PSP)
	#include <math.h>
#elif defined(TARGET_360)
	#include <math.h>
#elif defined(TARGET_N3DS)
	#include <math.h>
#else
	#error "Don't know how fmod for this platform"
#endif

/**
 * xCore namespace
 */
namespace xcore
{
	class CharReader
	{
	public:
		virtual uchar32		Peek() = 0;
		virtual uchar32		Read() = 0;
	};

	struct AsciiBuffer
	{
		inline			AsciiBuffer(char * str, char * end, char * eos) : mStr(str), mEnd(end), mEos(eos) { }
		char*			mStr;
		char*			mEnd;
		char*			mEos;
	};

	struct Utf32Buffer
	{
		inline			Utf32Buffer(uchar32 * str, uchar32 * end, uchar32 * eos) : mStr(str), mEnd(end), mEos(eos) { }
		uchar32*		mStr;
		uchar32*		mEnd;
		uchar32*		mEos;
	};

	struct AsciiConstBuffer
	{
		inline			AsciiConstBuffer(char const* str, char const* end) : mStr(str), mEnd(end) { }
		char const*		mStr;
		char const*		mEnd;
	};

	struct Utf32ConstBuffer
	{
		inline			Utf32ConstBuffer(uchar32 const* str, uchar32 const* end) : mStr(str), mEnd(end) { }
		uchar32 const*	mStr;
		uchar32 const*	mEnd;
	};

	class CharBufferIterator
	{
	public:
		virtual void		Do(AsciiConstBuffer b) = 0;
		virtual void		Do(Utf32ConstBuffer b) = 0;
	};
	
	class CharBufferModifier
	{
	public:
		virtual void		Do(AsciiBuffer b) = 0;
		virtual void		Do(Utf32Buffer b) = 0;
	};

	class CharBufferFill : public CharBufferModifier
	{
	public:
		inline				CharBufferFill(uchar32 c) : mFill(c) {}

		virtual void		Do(AsciiBuffer b)
		{
			char* ptr = b.mStr;
			while (ptr < b.mEnd)
				*ptr++ = mFill;
		}

		virtual void		Do(Utf32Buffer b)
		{
			uchar32* ptr = b.mStr;
			while (ptr < b.mEnd)
				*ptr++ = mFill;
		}
	private:
		uchar32		mFill;
	};

	class CharBufferReverse : public CharBufferModifier
	{
	public:
		inline				CharBufferReverse() {}

		virtual void		Do(AsciiBuffer b)
		{
			char* head = b.mStr;
			char * tail = b.mStr + (b.mEnd - b.mStr) - 1;
			while (head < tail)
			{
				char c = *head;
				*head = *tail;
				*tail = c;
				head += 1;
				tail -= 1;
			}
		}

		virtual void		Do(Utf32Buffer b)
		{
			uchar32* head = b.mStr;
			uchar32* tail = b.mStr + (b.mEnd - b.mStr) - 1;
			while (head < tail)
			{
				uchar32 c = *head;
				*head = *tail;
				*tail = c;
				head += 1;
				tail -= 1;
			}
		}
	};

	class CharBufferToCharWriter : public CharBufferIterator
	{
	public:
		inline				CharBufferToCharWriter(CharWriter* writer) : mWriter(writer) { }

		virtual void		Do(AsciiConstBuffer b)
		{
			char const* src = b.mStr;
			while (src < b.mEnd)
			{
				char c = *src++;
				mWriter->Write(c);
			}
		}

		virtual void		Do(Utf32ConstBuffer b)
		{
			uchar32 const* src = b.mStr;
			while (src < b.mEnd)
			{
				uchar32 c = *src++;
				mWriter->Write(c);
			}
		}

	private:
		CharWriter*			mWriter;
	};

	class CharWriter
	{
	public:
		virtual u64			Count() const = 0;
		virtual void		Reset() = 0;

		virtual bool		Write(uchar32 c) = 0;
		virtual bool		Write(AsciiConstBuffer c) = 0;
		virtual bool		Write(Utf32ConstBuffer c) = 0;

		virtual void		Iterate(CharBufferIterator* i) = 0;
		virtual void		Modify(CharBufferModifier* m) = 0;
	};

	class CharWriterCounter : public CharWriter
	{
	public:
		inline			CharWriterCounter() : mCount(0) { }

		virtual u64		Count() const	{ return mCount; }
		virtual void	Reset() { mCount = 0; }
		
		virtual bool	Write(uchar32 c)
		{
			mCount++;
			return true;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			mCount += b.mEnd - b.mStr;
			return true;
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			mCount += b.mEnd - b.mStr;
			return true;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
		}

		virtual void	Modify(CharBufferModifier* m)
		{
		}

	private:
		u64				mCount;
	};

	class CharWriterAsAsciiToConsole : public CharWriter
	{
	public:
		inline			CharWriterAsAsciiToConsole(AsciiBuffer cache) : mCount(0), mCache(cache), mCursor(cache.mStr) { }

		virtual u64		Count() const { return mCount; }

		virtual void	Reset() { mCount = 0; mCursor = mCache.mStr; mCursor[0] = '\0'; }
		virtual void	Reverse() { }
		virtual void	Fill(uchar32 c) { }

		void			Flush()
		{
			xconsole::write(mCache.mStr);
			mCursor = mCache.mStr;
			mCursor[0] = '\0';
		}

		virtual bool	Write(uchar32 c)
		{
			if (mCursor == mCache.mEnd)
				Flush();

			mCursor[0] = (char)c;
			mCursor[1] = '\0';
			mCursor += 1;

			return true;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			Flush();
			CharBufferToCharWriter writer(this);
			writer.Do(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			Flush();
			CharBufferToCharWriter writer(this);
			writer.Do(b);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			AsciiConstBuffer buffer(mCache.mStr, mCursor);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m) 
		{
			AsciiBuffer buffer(mCache.mStr, mCursor, mCache.mEnd);
			m->Do(buffer);
		}

	private:
		u64				mCount;
		AsciiBuffer		mCache;
		char*			mCursor;
	};

	class CharWriterAsUtf32ToConsole : public CharWriter
	{
	public:
		inline			CharWriterAsUtf32ToConsole(Utf32Buffer cache) : mCount(0), mCache(cache), mCursor(cache.mStr) { }

		virtual u64		Count() const { return mCount; }
		virtual void	Reset() { mCount = 0; mCursor = mCache.mStr; mCursor[0] = '\0'; }

		void			Flush()
		{
			xconsole::write(mCache.mStr);
			mCursor = mCache.mStr;
			mCursor[0] = '\0';
		}

		virtual bool	Write(uchar32 c)
		{
			if (mCursor == mCache.mEnd)
				Flush();
			mCursor[0] = c;
			mCursor[1] = '\0';
			mCursor += 1;
			return true;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			Flush();
			CharBufferToCharWriter writer(this);
			writer.Do(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			Flush();
			CharBufferToCharWriter writer(this);
			writer.Do(b);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			Utf32ConstBuffer buffer(mCache.mStr, mCursor);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			Utf32Buffer buffer(mCache.mStr, mCursor, mCache.mEnd);
			m->Do(buffer);
		}

	private:
		u64				mCount;
		Utf32Buffer		mCache;
		uchar32*		mCursor;
	};
	
	class CharReaderFromAsciiBuffer : public CharReader
	{
	public:
		inline			CharReaderFromAsciiBuffer(const char* str, const char* str_end) : mStr(str), mCursor(str), mStrEnd(str_end) { }

		virtual uchar32	Peek()
		{
			if (mCursor == mStrEnd)
				return '\0';
			uchar32 c = *mCursor;
			return c;
		}

		virtual uchar32	Read()
		{
			if (mCursor == mStrEnd)
				return '\0';
			uchar32 c = *mCursor++;
			return c;
		}

	private:
		const char*		mStr;
		const char*		mCursor;
		const char*		mStrEnd;
	};


	class CharReaderFromUtf32Buffer : public CharReader
	{
	public:
		inline			CharReaderFromUtf32Buffer(const uchar32* str, const uchar32* str_end) : mStr(str), mStrEnd(str_end) { }

		virtual uchar32	Peek()
		{
			if (mStr >= mStrEnd)
				return '\0';
			uchar32 c = *mStr;
			return c;
		}

		virtual uchar32	Read()
		{
			if (mStr >= mStrEnd)
				return '\0';
			uchar32 c = *mStr++;
			return c;
		}

	private:
		const uchar32*		mStr;
		const uchar32*		mStrEnd;
	};

	class CharWriterToAsciiBuffer : public CharWriter
	{
	public:
		inline			CharWriterToAsciiBuffer(char* str, char* str_end) : mStr(str), mCursor(str), mStrEnd(str_end), mCount(0) { mStrEnd[0] = '\0'; }

		virtual u64		Count() const	{ return mCount; }
		virtual void	Reset() { mCursor = mStr; mCount = 0; }
		
		virtual bool	Write(uchar32 c)
		{
			bool const ok = Write_Prologue(c);
			if (ok)
			{
				*mCursor++ = c;
				*mCursor = '\0';
			}
			return ok;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			CharBufferToCharWriter writer(this);
			writer.Do(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			CharBufferToCharWriter writer(this);
			writer.Do(b);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			AsciiConstBuffer buffer(mStr, mCursor);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			AsciiBuffer buffer(mStr, mCursor, mStrEnd);
			m->Do(buffer);
		}

	private:
		bool			Write_Prologue(uchar32& c) const
		{
			if (mCursor > mStr && mCursor < mStrEnd)
			{
				s32 len = 0;
				if (c <= 0x7f) { len = 1; }
				else if (c < 0x0800) { len = 2; }
				else if (c < 0xd800) { len = 3; }
				else if (c < 0xe000) { len = 0; }
				else if (c < 0x010000) { len = 3; }
				else if (c < 0x110000) { len = 4; }

				if (len == 0 || len > 1)
				{
					c = '?';
					len = 1;
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		u64				mCount;
		char*			mStr;
		char*			mCursor;
		char*			mStrEnd;
	};

	class CharWriterToUtf32Buffer : public CharWriter
	{
	public:
		inline			CharWriterToUtf32Buffer(uchar32* str, uchar32* str_end) : mStr(str), mCursor(str), mStrEnd(str_end), mCount(0) { }

		virtual u64		Count() const	{ return mCount; }
		virtual void	Reset() { mCursor = mStr; mCount = 0; }
		
		virtual bool	Write(uchar32 c)
		{
			if (mCursor >= mStrEnd)
				return false;
			*mCursor++ = c;
			*mCursor = '\0';
			return true;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			CharBufferToCharWriter writer(this);
			writer.Do(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			CharBufferToCharWriter writer(this);
			writer.Do(b);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			Utf32ConstBuffer buffer(mStr, mCursor);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			Utf32Buffer buffer(mStr, mCursor, mStrEnd);
			m->Do(buffer);
		}

	private:
		u64				mCount;
		uchar32*		mStr;
		uchar32*		mCursor;
		uchar32*		mStrEnd;
	};

	template<int SIZE>
	class CharWriterToAsciiBufferWithBuffer : public CharWriter
	{
	public:
		inline			CharWriterToAsciiBufferWithBuffer() : mWriter(mBuffer, &mBuffer[SIZE])
		{
			mBuffer[SIZE] = '\0';
		}

		virtual u64		Count() const { return mWriter.Count(); }
		virtual void	Reset() { mWriter.Reset(); }

		virtual bool	Write(uchar32 c)
		{
			return mWriter.Write(c);
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			mWriter.Write(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			mWriter.Write(b);
		}

		virtual void	CopyTo(CharWriter* dst) const
		{
			mWriter.CopyTo(dst);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			mWriter->Iterate(i);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			mWriter.Modify(m);
		}

	private:
		uchar			mBuffer[SIZE + 1];
		CharWriterToAsciiBuffer mWriter;
	};


	template<int SIZE>
	class CharWriterToUtf32BufferWithBuffer : public CharWriter
	{
	public:
		inline			CharWriterToUtf32BufferWithBuffer() : mWriter(mBuffer, &mBuffer[SIZE])
		{
			mBuffer[SIZE] = '\0';
		}

		virtual u64		Count() const { return mWriter.Count(); }
		virtual void	Reset() { mWriter.Reset(); }

		virtual bool	Write(uchar32 c)
		{
			return mWriter.Write(c);
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			mWriter.Write(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			mWriter.Write(b);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			mWriter->Iterate(i);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			mWriter.Modify(m);
		}

	private:
		uchar32			mBuffer[SIZE + 1];
		CharWriterToUtf32Buffer mWriter;
	};


	//==============================================================================
	// DEFINES
	//==============================================================================

	// --- FLAGS FOR THE VSPRINTF ---

	/**
	 * NUMERIC VALUES
	 */
	#define LONGDBL            0x01        // long f64; unimplemented
	#define LONGINT            0x02        // long integer
	#define QUADINT            0x04        // quad integer
	#define SHORTINT           0x08        // short integer
	#define NUMBERMASK         0x0F

	/**
	 * OTHER FLAGS
	 */
	#define ALT                0x10        // alternate form
	#define HEXPREFIX          0x20        // add 0x or 0X prefix
	#define LADJUST            0x40        // left adjustment
	#define ZEROPAD            0x80        // zero (as opposed to blank) pad

	/**
	 * TEXT FLAGS
	 */
	#define UPPERCASE          0x100       // upper-case, YES/NO
	#define LOWERCASE          0x200       // lower-case, yes/no
	#define CAMELCASE          0x400       // camel-case, Yes/No

	/**
	 * CONSTANTS
	 */
	#define SPF_LONG_MAX    0x7FFFFFFF
	#define WORKSIZE        128         // space for %c, %[diouxX], %[eEfgG]
	#define DEFPREC         6           // number of precision for the real numbers


	//==============================================================================
	// FUNCTIONS
	//==============================================================================

	/**
	 * vsprintf_pow for f64
	 */
	static f64 VSPrintf_pow(f64 x, s32 p)
	{
		f64 r;

		if (p == 0)
			return 1.0;

		if (x == 0.0)
			return 0.0;

		if (p < 0)
		{
			p = -p;
			x = 1.0 / x;
		}

		r = 1.0;

		//TODO: use while?
		for (;;)
		{
			if (p & 1)
				r *= x;

			if ((p >>= 1) == 0)
				return r;

			x *= x;
		}
	}


	/**
	 *------------------------------------------------------------------------------
	 * writer  - Where the output is written to (Make sure there are at least 24 bytes)
	 * fpnum   - number which is going to be converted
	 * cvt     - what type of conversion needs
	 * width   - total width of the output number
	 * prec    - how many digits of precision
	 *------------------------------------------------------------------------------
	 */
	static s32 dtoa(CharWriter* writer, f64 fpnum, char cvt, s32 width, s32 prec)
	{
		static const f64 powTable[] = {1,10,10e1,10e2,10e3,10e4,10e5,10e6,10e7,10e8,10e9,10e10,10e11,10e12,10e13,10e14,10e15,10e16,10e17,10e18,10e19,10e20,10e21,10e22,10e23};

		ASSERT(writer);
		ASSERT(prec >= 0);

		char    fwork[WORKSIZE];
		char*   fw               = fwork;

		 // setup integer part
		char    iwork[WORKSIZE];
		char*   iworkend         = &iwork[sizeof(iwork) - 1];
		char*   iw               = iworkend;
		*iw = 0;

		// setup exponent part
		char    ework[16];
		char*   eworkend         = &ework[sizeof(ework) - 1];;
		char*   ew               = eworkend;
		*ew = 0;

		if (x_f64u::isInfinite(fpnum))
		{
			if (fpnum < 0)
				writer->Write('-');
			ascii::pcrune str = "Inf";
			while (*str != '\0')
				writer->Write(*str++);
			return;
		}

		if (x_f64u::isNAN(fpnum))
		{
			ascii::pcrune str = "NaN";
			while (*str != '\0')
				writer->Write(*str++);
			return;
		}

		// grab sign & make non-negative
		s32 is_neg = fpnum < 0;
		if (is_neg)
			fpnum = -fpnum;

		// precision matters

		// can't have more prec than supported
		if (prec > WORKSIZE - 2)
			prec = WORKSIZE - 2;

		f64 powprec;
		if (prec == 6)
		{
			powprec = 1.0e6;
		}
		else
		{
			if (prec > 23)
			{
				powprec = VSPrintf_pow(10.0, prec);
			}
			else
			{
				powprec = powTable[prec];
			}
		}
		f64 rounder = 0.5 / powprec;


		s32 f_fmt;
		f_fmt = cvt == 'f' || ((cvt == 'g') && (fpnum == 0.0 || (fpnum >= 1e-4 && fpnum < powprec)));

		s32 iwidth = 0;
		s32 fwidth = 0;
		s32 ewidth = 0;

		if (f_fmt)  // fixed format
		{
			f64 ipart;
			f64 fpart = modf(fpnum, &ipart);

			// convert fractional part
			if (fpart >= rounder || cvt != 'g')
			{
				f64  ifpart;
				s32  i;
				f64  ffpart;

				fpart += rounder;
				if (fpart >= 1.0)
				{
					ipart += 1.0;
					fpart -= 1.0;
				}

				ffpart = fpart;

				xbool anySignificantInteger = xFALSE;
				for (i = 0; i < prec; ++i)
				{
					ffpart = modf(ffpart * 10.0, &ifpart);
					*fw++ = (char)('0' + (s32)(ifpart));

					// This was added to keep prec number of significant digits
					// When the user is looking for higher precision we will do this
					if (prec > DEFPREC)
					{
						if (anySignificantInteger == xFALSE && ifpart == '0')  prec++;
						else anySignificantInteger = xTRUE;
					}

					++fwidth;
				}

				if (cvt == 'g')  // inhibit trailing zeroes if g-fmt
				{
					char* p;
					for (p = fw - 1; p >= fwork && *p == '0'; --p)
					{
						*p = 0;
						--fwidth;
					}
				}
			}

			// convert integer part
			if (ipart == 0.0)
			{
				// Removed this because I want to always see the zero I don't like things
				// that look like ".324234" I like "0.324234" to make sure we don't give
				// up precision I incremented the space for the number.
				prec++;
				//if (cvt != 'g' || fwidth < prec || fwidth < width)
				{
					*--iw = '0'; ++iwidth;
				}
			}
			else
			{
				if (ipart <= (f64)(SPF_LONG_MAX)) // a useful speedup
				{
					s32 li = (s32)(ipart);
					while (li != 0)
					{
						*--iw = (char)('0' + (li % 10));
						li = li / 10;
						++iwidth;
						ASSERT(iwidth < WORKSIZE);
					}
				}
				else // the slow way
				{
					while (ipart > 0.5)
					{
						f64 ff = modf(ipart / 10.0, &ipart);
						ff = (ff + 0.05) * 10.0;
						*--iw = (char)('0' + (s32)(ff));
						++iwidth;
						ASSERT(iwidth < WORKSIZE);
					}
				}
			}

			// g-fmt: kill part of frac if prec/width exceeded
			if (cvt == 'g')
			{
				s32 m = prec;
				s32 adj;

				if (m < width)
					m = width;

				adj = iwidth + fwidth - m;
				if (adj > fwidth)
					adj = fwidth;

				if (adj > 0)
				{
					char* f;
					for (f = &fwork[fwidth-1]; f >= fwork && adj > 0; --adj, --f)
					{
						char ch = *f;

						--fwidth;
						*f = 0;

						if (ch > '5') // properly round: unavoidable propagation
						{
							char* p;
							s32 carry = 1;

							for (p = f - 1; p >= fwork && carry; --p)
							{
								++*p;

								if (*p > '9') *p = '0';
								else carry = 0;
							}

							if (carry)
							{
								for (p = iworkend - 1; p >= iw && carry; --p)
								{
									++*p;
									if (*p > '9') *p = '0';
									else carry = 0;
								}

								if (carry)
								{
									*--iw = '1';
									++iwidth;
									--adj;
									ASSERT(iwidth < WORKSIZE);
								}
							}
						}
					}

					// kill any additional trailing zeros
					{
						char* p;
						for (p = f; p >= fwork && *p == '0'; --p)
						{
							*p = 0;
							--fwidth;
						}
					}
				}
			}
		}
		else  // e-fmt
		{
			// normalize
			s32 exp = 0;

			while (fpnum >= 10.0)
			{
				fpnum *= 0.1;
				++exp;
			}

			f64 almost_one;
			almost_one = 1.0 - rounder;

			while (fpnum > 0.0 && fpnum < almost_one)
			{
				fpnum *= 10.0;
				--exp;
			}



			if (cvt == 'g')     // used up one digit for s32 part...
			{
				--prec;
				powprec /= 10.0;
				rounder = 0.5 / powprec;
			}

			f64 ipart = 0;
			f64 fpart = modf(fpnum, &ipart);
			f64 ffpart = 0;
			f64 ifpart = 0;
			s32 i;
			// convert fractional part -- almost same as above
			if (fpart >= rounder || cvt != 'g')
			{
				fpart += rounder;

				if (fpart >= 1.0)
				{
					fpart -= 1.0;
					ipart += 1.0;

					if (ipart >= 10.0)
					{
						++exp;
						ipart /= 10.0;
						fpart /= 10.0;
					}
				}

				ffpart = fpart;

				for (i = 0; i < prec; ++i)
				{
					ffpart = modf(ffpart * 10.0, &ifpart);
					*fw++ = (char)('0' + (s32)(ifpart));
					++fwidth;
					ASSERT(fwidth < WORKSIZE);
				}

				if (cvt == 'g')  // inhibit trailing zeroes if g-fmt
				{
					char* p;

					for (p = fw - 1; p >= fwork && *p == '0'; --p)
					{
						*p = 0;
						--fwidth;
						ASSERT(fwidth >= 0);
					}
				}
			}

			// convert exponent
			char eneg = exp < 0;
			if (eneg) exp = - exp;

			while (exp > 0)
			{
				*--ew = (char)('0' + (exp % 10));
				exp /= 10;
				++ewidth;
				ASSERT(ewidth < 16);
			}

			while (ewidth <= 2)  // ensure at least 2 zeroes
			{
				*--ew = '0';
				++ewidth;
				ASSERT(ewidth < 16);
			}

			*--ew = eneg ? '-' : '+';
			*--ew = 'e';

			ewidth += 2;

			// convert the one-digit integer part
			*--iw = (char)('0' + (s32)(ipart));
			++iwidth;

		}

		// arrange everything in returned string
		s32 const showdot = cvt != 'g' || fwidth > 0;
		s32 const fmtwidth = is_neg + iwidth + showdot + fwidth + ewidth;
		s32 const pad = width - fmtwidth;

		s32 i;
		if (pad > 0)
		{
			for (i = 0; i < pad; ++i)
				writer->Write(' ');
		}

		if (is_neg)
			writer->Write('-');

		for (i = 0; i < iwidth; ++i)
			writer->Write(*iw++);

		if (showdot)
		{
			writer->Write('.');
			fw = fwork;
			for (i = 0; i < fwidth; ++i)
				writer->Write(*fw++);
		}

		for (i = 0; i < ewidth; ++i)
			writer->Write(*ew++);
	}



	/**
	 *------------------------------------------------------------------------------
	 * Convert an unsigned long to ASCII for printf purposes, returning
	 * a pointer to the first character of the string representation.
	 * Octal numbers can be forced to have a leading zero; hex numbers
	 * use the given digits.
	 *------------------------------------------------------------------------------
	 * val      - Numeric value to be converted
	 * endp     - String to be fill in fromt the end
	 * base     - base of the number (10, 8, 16)
	 * octzero  - flag to add to the oct numbers the 0 in front
	 * xdigs    - Hexadecimal string array of number 0,1,2,3,4,5,9,A,or a, ..etc
	 *------------------------------------------------------------------------------
	 */
	#define to_digit(c) ((c) - '0')
	#define is_digit(c) ((u32)to_digit(c) >= 0 && ((u32)to_digit(c) <= 9))
	#define to_char(n)  ((u32)((n) + '0'))

	static
	void ULtoA(u32 val, CharWriter* writer, s32 base, xbool octzero, char* xdigs)
	{
		uchar32 c;
		s32   sval;

		ASSERT(writer);

		// handle the three cases separately, in the hope of getting better/faster code.

		switch (base)
		{
		case 10:
				if (val < 10)
				{   // many numbers are 1 digit
					c = to_char(val);
					writer->Write(c);
					return;
				}

				// On many machines, unsigned arithmetic is harder than
				// signed arithmetic, so we do at most one unsigned mod and
				// divide; this is sufficient to reduce the range of
				// the incoming value to where signed arithmetic works.

				if (val > SPF_LONG_MAX)
				{
					c = to_char(val % 10);
					writer->Write(c);
					sval = (s32)(val / 10);
				}
				else
				{
					sval = (s32)val;
				}

				do
				{
					c = to_char(sval % 10);
					writer->Write(c);
					sval /= 10;
				} while (sval != 0);

				break;

		case 8:
				do
				{
					c = to_char(val & 7);
					writer->Write(c);
					val >>= 3;
				} while (val);
				if (octzero && c != '0') 
					writer->Write(uchar32('0'));
				break;

		case 16:
				do
				{
					writer->Write(xdigs[val & 15]);
					val >>= 4;
				} while (val);

				break;

		default:
		        /* oops */
				break;
			}
	}

	/**
	 * Same as above but for s64
	 */
	static
	void UQtoA(u64 val, CharWriter* writer, s32 base, xbool octzero, char* xdigs)
	{
		uchar32 c;
		s64   sval;

		// handle the three cases separately, in the hope of getting better/faster code.

		switch (base)
		{
		case 10:
				if (val < 10)
				{   // many numbers are 1 digit
					c = to_char(val);
					writer->Write(c);
					return;
				}

				// On many machines, unsigned arithmetic is harder than
				// signed arithmetic, so we do at most one unsigned mod and
				// divide; this is sufficient to reduce the range of
				// the incoming value to where signed arithmetic works.

				if (val > ((~(u64)0) >> 1))
				{
					c = to_char(val % 10);
					writer->Write(c);
					sval = (s64)(val / 10);
				}
				else
				{
					sval = (s64)val;
				}

				do
				{
					c = to_char(sval % 10);
					writer->Write(c);
					sval /= 10;
				} while (sval != 0);

				break;

		case 8:
				do
				{
					c = to_char(val & 7);
					writer->Write(c);
					val >>= 3;
				} while (val);

				if (octzero && c != '0') 
					writer->Write('0');

				break;

		case 16:
				do
				{
					c = xdigs[val & 15];
					writer->Write(c);
					val >>= 4;
				} while (val);

				break;

		default:
			/* oops */

		}
	}

	static s32 boolToStr(u32 _boolean, CharWriter* writer, bool yesNo, xcore::s32 flags)
	{
		ascii::pcrune t[] = { "false", "true", "FALSE", "TRUE", "False", "True", "no", "yes", "NO", "YES", "No", "Yes" };
		s32 i = yesNo ? 6 : 0;
		if (flags & CAMELCASE)
			i += 4;
		else if (flags & UPPERCASE)
			i += 2;

		i += (_boolean==0) ? 0 : 1;

		s32 len = 0;
		ascii::pcrune src = (ascii::pcrune)t[i];
		while (*src != '\0')
		{
			uchar32 c = *src++;
			writer->Write(c);
			len += 1;
		}
		return len;
	}

	/**
	 * WriteToBuffer
	 */
	typedef void (*WriteBufferDelegate)(CharWriter* writer, uchar* string, s32 size);

	static
	void WriteToBuffer(CharWriter* writer, uchar* string, s32 size)
	{
		s32 i;

		ASSERT(string);
		ASSERT(size >= 0);

		for (i = 0; i < size; i++)
		{
			uchar32 c = string[i];
			if (writer->Write(c) == false)
				break;
		}
	}

	static void WriteToBufferDummy(CharWriter* writer, uchar* string, s32 size)
	{

	}

	static void WriteTo(CharWriter* writer, CharWriter* buffer)
	{

	}
	

	/**
	 * Choose PADSIZE to trade efficiency vs. size.  If larger printf
	 * fields occur frequently, increase PADSIZE and make the initializers
	 * below longer.
	 */
	typedef void (*PadBufferDelegate)(CharWriter* writer, s32 howMany, char with);

	static void PadBuffer(CharWriter* writer, s32 howMany, char with)
	{
		for (s32 i = 0; i < howMany; i++)
		{
			if (writer->Write(with) == false)
				break;
		}
	}

	static void PadBufferDummy(CharWriter* writer, s32 howMany, char with)
	{

	}

	///< DOM-IGNORE-END

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     write a string formatted output into a buffer.
	 * Arguments:
	 *        buffer                - Storage location for output.
	 *      maxChars            - Maximum number of characters to store
	 *        pFormatStr            - String containing the formating specification.
	 *      Args                - Pointer to list of arguments OR the actual arguments in case of x_sprintf
	 * Returns:
	 *        return the number of characters written, not including the terminating null character.
	 * Description:
	 *      These functions formats and stores a series of characters and values in buffer.
	 *      Each argument (if any) is converted and output according to the corresponding format
	 *      specification in format. The format consists of ordinary characters and has the same
	 *      form and function as the format argument for x_printf. A null character is appended after
	 *      the last character written. If copying occurs between strings that overlap, the behavior
	 *      is undefined.
	 *
	 *<P><B>Type Field Characters</B>
	 *<TABLE>
	 *      Character   Output format
	 *      =========   --------------------------------------------------------
	 *      %%            a percent sign
	 *      %b,%B,%#b     a boolean as true/false (%b=lower-case true/false, %B=upper-case TRUE/FALSE, %#b/%B#=capital-case True/False)
	 *      %c            a character with the given number
	 *      %s            a string
	 *      %d            a signed integer, in decimal
	 *      %u            an unsigned integer, in decimal
	 *      %o            an unsigned integer, in octal
	 *      %x            an unsigned integer, in hexadecimal (lower case)
	 *      %e            a floating-point number, in scientific notation
	 *      %f            a floating-point number, in fixed decimal notation
	 *      %g            a floating-point number, in %e or %f notation
	 *      %X            like %x, but using upper-case letters
	 *      %E            like %e, but using an upper-case "E"
	 *      %G            like %g, but with an upper-case "E" (if applicable)
	 *      %p            a pointer (outputs the pointer value's address in hexadecimal)
	 *      %n            special: *stores* the number of characters output so far into the next variable in the parameter list
	 *      %y,%Y,%#y     a boolean as yes/no (%y=lower-case yes/no, %Y=upper-case YES/NO, %#y/%#Y=capital-case Yes/No)
	 *</TABLE>
	 *
	 *   <B>size flags</B>
	 *<TABLE>
	 *     Character    Output
	 *     =========    ------------------------------------------------------------
	 *         h        interpret integer as s16 or u16
	 *         l        interpret integer as s32 or u32
	 *        q, L      interpret integer as s64 or u64
	 *         L        interpret floating point as higher precision adds a few extra floating point numbers
	 *</TABLE>
	 *
	 * Examples:
	 *<CODE>
	 *      x_printf("%Lf %Lf %Lf", Vec.X, Vec.Y, Vec.Z);   * Note that we use the higher precision floating point representation
	 *      x_printf("%Ld" (s64)123);                       * This is a way to print an s64 bit number
	 *      x_printf("%d" 123);                             * Nothing special here printing an 32bit integer
	 *      x_printf("%f" 123.0);                           * Nothing special here printing a floating point number
	 *      x_printf("%+010.4f", 123.456);                  * The printf works like you will expect
	 *</CODE>
	 *
	 * See Also:
	 *     x_printf x_printfxy
	 *------------------------------------------------------------------------------
	 */

	//s32 x_vsprintf_internal(char* buffer, s32 maxChars, const char* formatStr, const x_va_list& args)
	s32 VSPrintf_internal(CharWriter* writer, CharReader* reader, CharWriter* buffer, const x_va_list& args)
	{
		ASSERT(reader != NULL);
		ASSERT(writer != NULL);

		uchar32		ch;             ///< character 
		s32     	n;              ///< handy integer (short term usage)
		CharWriter*	cp = NULL;		///< handy char pointer (short term usage)
		s32			flags;          ///< flags as above
		s32			width;          ///< width from format (%8d), or 0
		s32			prec;           ///< precision from format (%.3d), or -1
		char		sign;           ///< sign prefix (' ', '+', '-', or \0)
		f64			_double;        ///< f64 precision arguments %[eEfgG]
		s32			base;           ///< base for [diouxX] conversion
		s32			dprec;          ///< a copy of prec if [diouxX], 0 otherwise
		s32			realsz;         ///< field size expanded by dprec, sign, etc
		s32			size;           ///< size of converted field or string
		uchar   	ox[2];          ///< space for 0x hex-prefix

		/// Initialize variables
		char* xdigs    = NULL;                   // digits for [xX] conversion
		s32   ret      = 0;                      // return value accumulator
		u32   ulval    = 0;                      // integer arguments %[diouxX]
		u64   uqval    = 0;                      // %q integers
		s32   argindex = 0;

		/// Scan the format for conversions (`%' character).
		for (;;)
		{
			// Find the first "interesting symbol"
			for (; ((ch=reader->Peek()) != uchar32('\0')) && (ch != uchar32('%')); )
			{
				// Write all the characters before the "interesting symbol"
				ch = reader->Read();
				writer->Write(ch);
				ret += 1;
			}


			// are we done?
			if (ch == '\0')
				goto done;

			// skip over '%'
			reader->Read();

			// reset the temp buffer
			buffer->Reset();

			// get ready for formating the info
			flags = 0;
			dprec = 0;
			width = 0;
			prec  = -1;
			sign  = '\0';

	rflag:
			ch = reader->Read();

	reswitch:
			switch (ch)
			{
				case ' ':

					// ``If the space and + flags both appear, the space
					// flag will be ignored.''
					//  -- ANSI X3J11
					if (!sign)
						sign = ' ';
					goto rflag;

				case '#':
					flags |= ALT;
					flags |= CAMELCASE;
					goto rflag;

				case '*':
					 // ``A negative field width argument is taken as a
					 // - flag followed by a positive field width.''
					 // -- ANSI X3J11
					 // They don't exclude field widths read from args.
					{
						const s32 w = (s32)args[argindex++];
						if ((width = w) >= 0)
							goto rflag;
						width = -width;
					}
					/////////>>>>>>>>>>>>>>>>>> FALLTHROUGH <<<<<<<<<<<<<<//////////
				case '-':
					flags |= LADJUST;
					goto rflag;

				case '+':
					sign = '+';
					goto rflag;

				case '.':
					ch = reader->Read();
					if (ch == '*')
					{
						n = (s32)args[argindex++];
						prec = n < 0 ? -1 : n;
						goto rflag;
					}

					n = 0;
					while (is_digit(ch))
					{
						n = 10 * n + to_digit(ch);
						ch = reader->Read();
					}

					prec = n < 0 ? -1 : n;
					goto reswitch;

				case '0':
					 // ``Note that 0 is taken as a flag, not as the
					 // beginning of a field width.''
					 // -- ANSI X3J11
					flags |= ZEROPAD;
					goto rflag;

				case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					n = 0;
					do
					{
						n = 10 * n + to_digit(ch);
						//ch = *fmt++;
						ch = reader->Read();
					} while (is_digit(ch));

					width = n;
					goto reswitch;

				case 'L':
					flags |= LONGDBL;

					/////////>>>>>>>>>>>>>>>>>> FALLTHROUGH <<<<<<<<<<<<<<//////////
				case 'q':
					flags |= QUADINT;
					goto rflag;

				case 'h':
					flags |= SHORTINT;
					goto rflag;

				case 'l':
					flags |= LONGINT;
					goto rflag;

				case 'c':
					buffer->Write((uchar32)args[argindex++]);
					size = 1;
					sign = '\0';
					break;

				case 'D':
					flags |= LONGINT;
					/////////>>>>>>>>>>>>>>>>>> FALLTHROUGH <<<<<<<<<<<<<<//////////

				case 'd':
				case 'i':

					if (flags & QUADINT || args[argindex].type()==x_va::TYPE_INT64 || args[argindex].type()==x_va::TYPE_UINT64)
					{
						flags |= QUADINT;
						s64 temp = (s64)args[argindex++];
						if (temp < 0)
						{
							temp = -temp;
							sign = '-';
						}
						uqval = (u64)temp;
					}
					else
					{
						s32 temp;
						if (flags & SHORTINT)
							temp = (s16)args[argindex++];
						else
							temp = (s32)args[argindex++];

						if (temp < 0)
						{
							temp = -temp;
							sign = '-';
						}

						ulval = (u32)(temp);
					}

					base = 10;
					goto number;

				case 'g':
				case 'G':
					if (prec == 0)
						prec = 1;

				case 'e':
				case 'E':
				case 'f':
					/////////>>>>>>>>>>>>>>>>>> FALLTHROUGH <<<<<<<<<<<<<<//////////

					if (sign == '+')
					{
						writer->Write(sign);
						width--;
						ret++;
					}

					if (prec == -1)
						prec = DEFPREC;

					_double = (f64)args[argindex++];
					if (flags & LONGDBL)
					{	// add additional precision when we say long f64
						prec += 4;
					}

					if (flags & LADJUST)
						width = -width;

					// right-adjusting zero padding
					uchar buf[WORKSIZE + 1];
					buf[WORKSIZE] = '\0';
					CharWriterToAsciiBuffer buf_writer((char*)buf, (char*)(&buf[WORKSIZE]));
					CharBufferFill filler(' ');
					buf_writer.Modify(&filler);
					dtoa(&buf_writer, _double, (char)ch, width, prec);
					buf_writer.Reverse();
					size = buf_writer.Count();

					// check whether we have to pad or not
					if (flags & ZEROPAD)
					{
						for (s32 i = 0; buf[i] == ' '; i++)
							buf[i] = '0';
					}

					WriteToBuffer(writer, buf, size);

					if (flags & LADJUST)
						PadBuffer(writer, -width - size, ' ');

					// finally, adjust ret
					ret += width > size ? width : size;

					continue;

						///////////////////// FLOATING_POINT //////////////////////
				case 'n':

					// not supported

					// no output
					continue;

				//////////////////////////////// THIS IS NOT ANSI STANDARD
				//case 'O':
				//    flags |= LONGINT;
				//    /////////>>>>>>>>>>>>>>>>>> FALLTHROUGH <<<<<<<<<<<<<<//////////

				case 'o':
					if (flags & QUADINT)
						uqval = (u64)args[argindex++];
					else if (flags & SHORTINT)
						ulval = (u16)args[argindex++];
					else
						ulval = (u32)args[argindex++];

					base = 8;
					goto nosign;

				case 'p':
					 // "The argument shall be a pointer to void.  The
					 // value of the pointer is converted to a sequence
					 // of printable characters, in an implementation-
					 // defined manner."
					 // -- ANSI X3J11
#ifdef TARGET_PC
					uqval  = (u64)args[argindex++];
					base   = 16;
					xdigs  = "0123456789abcdef";
					flags |= QUADINT;           // | HEXPREFIX; Not prefixes
					ch     = 'x';

					// make sure that the precision is at 8
					if (prec < 0)
						prec = 8;
#else
					ulval = (u32)args[argindex++];
					base  = 16;
					xdigs = "0123456789abcdef";
					flags = (flags & ~QUADINT); // | HEXPREFIX; Not prefixes
					ch    = 'x';

					// make sure that the precision is at 8
					if (prec < 0)
						prec = 8;
#endif
					goto nosign;

				case 's':
					if (args[argindex].isPCUChar32())
					{
						uchar32 const* src = (uchar32 const*)args[argindex];
						while (utf::is_eos(src) == false)
						{
							uchar32 c;
							src = utf::read(src, c);
							buffer->Write(c);
						}
					}
					else if (args[argindex].isPCUChar8())
					{
						uchar8 const* src = (uchar8 const*)args[argindex];
						while (utf::is_eos(src) == false)
						{
							uchar32 c;
							src = utf::read(src, c);
							buffer->Write(c);
						}
					}
					else if (args[argindex].isPCTChar())
					{
						uchar const* src = (uchar const*)args[argindex];
						while (utf::is_eos(src) == false)
						{
							uchar32 c;
							src = utf::read(src, c);
							buffer->Write(c);
						}
					}

					size = buffer->Count();
					sign = '\0';
					break;

				case 'B':
					ulval = ((bool)args[argindex++]) ? 1 : 0;
					size = boolToStr(ulval, buffer, false, ((flags&ALT)!=0) ? CAMELCASE : UPPERCASE);
					break;

				case 'b':
					ulval = ((bool)args[argindex++]) ? 1 : 0;
					size = boolToStr(ulval, buffer, false, ((flags&ALT)!=0) ? CAMELCASE : 0);
					break;

				case 'Y':
					ulval = ((bool)args[argindex++]) ? 1 : 0;
					size = boolToStr(ulval, buffer, true, ((flags&ALT)!=0) ? CAMELCASE : UPPERCASE);
					break;

				case 'y':
					ulval = ((bool)args[argindex++]) ? 1 : 0;
					size = boolToStr(ulval, buffer, true, ((flags&ALT)!=0) ? CAMELCASE : 0);
					break;

				case 'U':
					flags |= LONGINT;
					/////////>>>>>>>>>>>>>>>>>> FALLTHROUGH <<<<<<<<<<<<<<//////////

				case 'u':
					if (flags & QUADINT)
						uqval = (u64)args[argindex++];
					else
						ulval = (u32)args[argindex++];

					base = 10;
					goto nosign;

				case 'X': xdigs = "0123456789ABCDEF";
					goto hex;

				case 'x': xdigs = "0123456789abcdef";

		hex:
					if (flags & QUADINT || args[argindex].type()==x_va::TYPE_INT64 || args[argindex].type()==x_va::TYPE_UINT64)
					{
						flags |= QUADINT;
						uqval = (u64)args[argindex++];
					}
					else
						ulval = (u32)args[argindex++];

					base = 16;

					// leading 0x/X only if non-zero
					if (flags & ALT && (flags & QUADINT ? uqval != 0 : ulval != 0))
						flags |= HEXPREFIX;

					// unsigned conversions
		nosign:     sign = '\0';

					// ``... diouXx conversions ... if a precision is
					// specified, the 0 flag will be ignored.''
					//  -- ANSI X3J11

		number:     if ((dprec = prec) >= 0)
						flags &= ~ZEROPAD;

					// ``The result of converting a zero value with an
					// explicit precision of zero is no characters.''
					// -- ANSI X3J11

					if (flags & QUADINT)
					{
						if (uqval != 0 || prec != 0)
						{
							UQtoA(uqval, buffer, base, xbool((flags & ALT)!=0), xdigs);
							CharBufferReverse reverser;
							buffer->Modify(&reverser);
							size = buffer->Count();
						}
					}
					else
					{
						if (ulval != 0 || prec != 0)
						{
							ULtoA(ulval, buffer, base, xbool((flags & ALT)!=0), xdigs);
							CharBufferReverse reverser;
							buffer->Modify(&reverser);
							size = buffer->Count();
						}
					}

					break;

				default:    // "%?" prints ?, unless ? is NUL
					if (ch == '\0')
						goto done;

					// pretend it was %c with argument ch
					buffer->Write(ch);
					size = 1;
					sign = '\0';

					break;
			}

			/**
			 * All reasonable formats wind up here.  at this point, buffer
			 * contains a string which (if not flags&LADJUST) should be
			 * padded out to `width' places.  If flags&ZEROPAD, it should
			 * first be prefixed by any sign or other prefix; otherwise,
			 * it should be blank padded before the prefix is emitted.
			 * After any left-hand padding and prefixing, emit zeroes
			 * required by a decimal [diouxX] precision, then print the
			 * string proper, then emit zeroes required by any leftover
			 * floating precision; finally, if LADJUST, pad with blanks.
			 */

			/**
			 * Compute actual size, so we know how much to pad.
			 * size excludes decimal prec; realsz includes it.
			 */
			realsz = dprec > size ? dprec : size;
			if (sign)
				realsz++;
			else if (flags & HEXPREFIX) 
				realsz += 2;

			// right-adjusting blank padding
			if ((flags & (LADJUST|ZEROPAD)) == 0)
				PadBuffer(writer, width - realsz, ' ');

			// prefix
			if (sign)
			{
				WriteToBuffer(writer, (uchar*)&sign, 1);
			}
			else
			{
				if (flags & HEXPREFIX)
				{
					ox[0] = '0';
					ox[1] = (u8)ch;
					WriteToBuffer(writer, ox, 2);
				}
			}

			// right-adjusting zero padding
			if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD)
				PadBuffer(writer, width - realsz, '0');

			// leading zeroes from decimal precision
			PadBuffer(writer, dprec - size, '0');

			// write the integer number
			CharBufferToCharWriter iterate_to_copy(writer);
			buffer->Iterate(&iterate_to_copy);

			// left-adjusting padding (always blank)
			if (flags & LADJUST)
				PadBuffer(writer, width - realsz, ' ');

			// finally, adjust ret
			ret += width > realsz ? width : realsz;
		}

	done:
		//WriteToBuffer(buffer, (uchar8*)"\0", 1);

		return (ret);
	}

	namespace ascii
	{
		s32 VSPrintf(prune str, prune str_end, pcrune format_str, pcrune format_str_end, const x_va_list& args)
		{
			CharReaderFromAsciiBuffer reader(format_str, format_str_end);
			CharWriterToAsciiBuffer writer(str, str_end);
			CharWriterToAsciiBufferWithBuffer<WORKSIZE> buffer;
			s32 strLen = VSPrintf_internal(&writer, &reader, &buffer, args);
			return writer.Count();
		}

		s32 SPrintf(prune str, prune str_end, pcrune format_str, pcrune format_str_end, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
																		 const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16);
			CharReaderFromAsciiBuffer reader(format_str, format_str_end);
			CharWriterToAsciiBuffer writer(str, str_end);
			CharWriterToAsciiBufferWithBuffer<WORKSIZE> buffer;
			VSPrintf_internal(&writer, &reader, &buffer, args);
			return writer.Count();
		}

		s32 VCPrintf(pcrune format_str, pcrune format_str_end, const x_va_list& args)
		{
			CharReaderFromAsciiBuffer reader(format_str, format_str_end);
			CharWriterCounter writer;
			CharWriterToAsciiBufferWithBuffer<WORKSIZE> buffer;
			VSPrintf_internal(&writer, &reader, &buffer, args);
			return writer.Count();
		}

		s32 CPrintf(pcrune format_str, pcrune format_str_end, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
					const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
		{
			x_va_list args(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16);
			CharReaderFromAsciiBuffer reader(format_str, format_str_end);
			CharWriterCounter writer;
			CharWriterToAsciiBufferWithBuffer<WORKSIZE> buffer;
			VSPrintf_internal(&writer, &reader, &buffer, args);
			return writer.Count();
		}

		 s32		Printf(pcrune format_str, pcrune format_str_end, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
							const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
		 {
			 x_va_list args(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16);
			 CharReaderFromAsciiBuffer reader(format_str, format_str_end);
			 CharWriterAsAsciiToConsole console;
			 CharWriterToAsciiBufferWithBuffer<WORKSIZE> buffer;

			 VSPrintf_internal(&writer, &reader, &buffer, args);
			 return writer.Count();
		 }

		 s32		Printf(pcrune format_str, pcrune format_str_end, const x_va_list& args)
		 {
			 CharReaderFromAsciiBuffer reader(format_str, format_str_end);
			 uchar32 cache[WORKSIZE + 1];
			 cache[WORKSIZE] = '\0';
			 CharWriterAsAsciiToConsole writer((char*)cache, (char*)cache + WORKSIZE);
			 VSPrintf_internal(&writer, &reader, args);
			 return writer.Count();
		 }

		 s32		Printf(pcrune str, pcrune str_end)
		 {
			 return xconsole::write(str, str_end);
		 }
	}

	 namespace utf32
	 {
		s32 VSPrintf(prune str, prune str_end, pcrune format_str, pcrune format_str_end, const x_va_list& args)
		{
			CharReaderFromUtf32Buffer reader(format_str, format_str_end);
			CharWriterToUtf32Buffer writer(str, str_end);
			CharWriterToUtf32BufferWithBuffer<WORKSIZE> buffer;

			s32 strLen = VSPrintf_internal(&writer, &reader, &buffer, args);
			return writer.Count();
		}

		s32 SPrintf(prune str, prune str_end, pcrune format_str, pcrune format_str_end, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
																		 const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16);
			CharReaderFromUtf32Buffer reader(format_str, format_str_end);
			CharWriterToUtf32Buffer writer(str, str_end);
			CharWriterToUtf32BufferWithBuffer<WORKSIZE> buffer;

			VSPrintf_internal(&writer, &reader, &buffer, args);
			return writer.Count();
		}

		s32 VCPrintf(pcrune format_str, pcrune format_str_end, const x_va_list& args)
		{
			CharReaderFromUtf32Buffer reader(format_str, format_str_end);
			CharWriterCounter writer;
			CharWriterToUtf32BufferWithBuffer<WORKSIZE> buffer;

			VSPrintf_internal(&writer, &reader, &buffer, args);
			return writer.Count();
		}

		s32 CPrintf(pcrune format_str, pcrune format_str_end, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
					const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
		{
			x_va_list args(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16);
			CharReaderFromUtf32Buffer reader(format_str, format_str_end);
			CharWriterCounter writer;
			CharWriterToUtf32BufferWithBuffer<WORKSIZE> buffer;

			VSPrintf_internal(&writer, &reader, &buffer, args);
			return writer.Count();
		}

		 s32		Printf(pcrune format_str, pcrune format_str_end, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
							const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
		 {
			CharReaderFromUtf32Buffer reader(format_str, format_str_end);
			return xconsole::write(&reader,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16);
		 }

		 s32		Printf(pcrune format_str, pcrune format_str_end, const x_va_list& args)
		 {
			CharReaderFromUtf32Buffer reader(format_str, format_str_end);
			return xconsole::write(&reader, args);
		 }

		 s32		Printf(pcrune str, pcrune str_end)
		 {
			 return xconsole::write(str, str_end);
		 }

	 }
};
/**
 *  END xCore namespace
 */


#endif
