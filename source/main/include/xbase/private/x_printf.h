#ifndef __XBASE_STRING_UTILS_H__
#define __XBASE_STRING_UTILS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_double.h"
#include "xbase/x_runes.h"
#include "xbase/x_console.h"


/**
 * xCore namespace
 */
namespace xcore
{
	struct AsciiBuffer
	{
		inline			AsciiBuffer(char * str, u32 len) : mStr((uchar*)str), mEnd((uchar*)str + len), mEos((uchar*)str + len) { }
		inline			AsciiBuffer(char * str, char const * end) : mStr((uchar*)str), mEnd((uchar *)end), mEos((uchar *)end) { }
		inline			AsciiBuffer(char * str, char const * end, char const * eos) : mStr((uchar *)str), mEnd((uchar *)end), mEos((uchar *)eos) { }

		uchar *			mStr;
		uchar const *	mEnd;
		uchar const *	mEos;
	};
	
	struct Utf32Buffer
	{
		inline			Utf32Buffer(uchar32 * str, u32 len) : mStr(str), mEnd(str + len), mEos(str + len) { }
		inline			Utf32Buffer(uchar32 * str, uchar32 const * end) : mStr(str), mEnd(end), mEos(end) { }
		inline			Utf32Buffer(uchar32 * str, uchar32 const * end, uchar32 const * eos) : mStr(str), mEnd(end), mEos(eos) { }
		uchar32 *		mStr;
		uchar32 const *	mEnd;
		uchar32 const *	mEos;
	};

	struct AsciiConstBuffer
	{
		inline			AsciiConstBuffer(char const* str, u32 len) : mStr((uchar*)str), mEnd((uchar*)str + len) { }
		inline			AsciiConstBuffer(char const* str, char const* end) : mStr((uchar*)str), mEnd((uchar*)end) { }

		uchar const*	mStr;
		uchar const*	mEnd;
	};

	struct Utf32ConstBuffer
	{
		inline			Utf32ConstBuffer(uchar32 const* str, u32 len) : mStr(str), mEnd(str + len) { }
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

	class CharReader
	{
	public:
		virtual uchar32		Peek() = 0;
		virtual uchar32		Read() = 0;
		virtual void		Skip() = 0;
		virtual void		Reset() = 0;

		virtual void		Iterate(CharBufferIterator* i) = 0;
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

	class CharBufferFill : public CharBufferModifier
	{
	public:
		inline				CharBufferFill(uchar32 c) : mFill(c) {}

		virtual void		Do(AsciiBuffer b)
		{
			uchar* ptr = b.mStr;
			while (ptr < b.mEnd)
				*ptr++ = (uchar)mFill;
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

	class CharBufferPad : public CharBufferModifier
	{
	public:
		enum EMode { HEAD = 0, TAIL = 1 };
		inline				CharBufferPad(EMode mode, uchar32 f, uchar32 r) : mMode(mode), mFind(f), mReplace(r) {}

		virtual void		Do(AsciiBuffer b)
		{
			if (mMode == HEAD)
			{
				uchar* ptr = b.mStr;
				while (ptr < b.mEnd && *ptr == mFind)
					*ptr++ = (uchar)mReplace;
			}
			else
			{
				uchar* ptr = b.mStr + (b.mEnd - b.mStr) - 1;
				while (ptr >= b.mStr && *ptr == mFind)
					*ptr-- = (uchar)mReplace;
			}
		}

		virtual void		Do(Utf32Buffer b)
		{
			if (mMode == HEAD)
			{
				uchar32* ptr = b.mStr;
				while (ptr < b.mEnd && *ptr == mFind)
					*ptr++ = mReplace;
			}
			else
			{
				uchar32* ptr = b.mStr + (b.mEnd - b.mStr) - 1;
				while (ptr >= b.mStr && *ptr == mFind)
					*ptr-- = mReplace;
			}
		}
	private:
		EMode		mMode;
		uchar32		mFind;
		uchar32		mReplace;
	};


	class CharBufferToCharWriter : public CharBufferIterator
	{
	public:
		inline				CharBufferToCharWriter(CharWriter* writer) : mWriter(writer) { }

		virtual void		Do(AsciiConstBuffer b)
		{
			uchar const* src = b.mStr;
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
		CharWriter * mWriter;
	};

	class CharBufferContainsChar : public CharBufferIterator
	{
	public:
		inline				CharBufferContainsChar(uchar32 c) : mChar(c), mFound(false) { }

		inline bool			Found() const { return mFound; }

		virtual void		Do(AsciiConstBuffer b)
		{
			uchar const* src = b.mStr;
			while (src < b.mEnd)
			{
				uchar32 c = *src++;
				if (c == mChar)
				{
					mFound = true;
					break;
				}
			}
		}

		virtual void		Do(Utf32ConstBuffer b)
		{
			uchar32 const* src = b.mStr;
			while (src < b.mEnd)
			{
				uchar32 c = *src++;
				if (c == mChar)
				{
					mFound = true;
					break;
				}
			}
		}

	private:
		bool				mFound;
		uchar32				mChar;
	};

	class CharBufferFindOneOf : public CharBufferIterator
	{
	public:
		inline				CharBufferFindOneOf(CharReader* one_of) : mOneOf(one_of), mFound('\0') { }

		inline uchar		Found() const { return (uchar)mFound; }

		virtual void		Do(AsciiConstBuffer b)
		{
			mFound = '\0';
			uchar const* src = b.mStr;
			while (src < b.mEnd)
			{
				uchar32 c = *src++;
				CharBufferContainsChar contains(c);
				mOneOf->Iterate(&contains);
				if (contains.Found())
				{
					mFound = c;
					break;
				}
			}
		}

		virtual void		Do(Utf32ConstBuffer b)
		{
			mFound = '\0';
			uchar32 const* src = b.mStr;
			while (src < b.mEnd)
			{
				uchar32 c = *src++;
				CharBufferContainsChar contains(c);
				mOneOf->Iterate(&contains);
				if (contains.Found())
				{
					mFound = c;
					break;
				}
			}
		}

	private:
		CharReader * mOneOf;
		uchar32				mFound;
	};

	class CharBufferReversedWriter : public CharWriter
	{
	public:
		utf32::runes	mStr;
		uchar32*		mBegin;
		uchar32*		mEnd;

		inline			CharBufferReversedWriter() {}

		inline			CharBufferReversedWriter(utf32::runes str) : mStr(str) { Reset(); }

		virtual u64		Count() const { return 0; }

		virtual void	Reset()
		{
			mStr.m_end = mStr.m_str;
			*mStr.m_end = '\0';

			while (utf::can_write(mStr))
				*mStr.m_end++ = ' ';

			mBegin = mStr.m_end;
			mEnd = mStr.m_end;
		}

		void			Flush()
		{
		}

		virtual bool	Write(uchar32 c)
		{
			if (mBegin > mStr.m_str)
			{
				mBegin -= 1;
				*mBegin = c;
				return true;
			}
			return false;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			Flush();
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			Flush();
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{

		}

		virtual void	Modify(CharBufferModifier* m)
		{

		}
	};
	class CharWriterCounter : public CharWriter
	{
	public:
		inline			CharWriterCounter() : mCount(0) { }

		virtual u64		Count() const { return mCount; }
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

		void			Flush()
		{
			console->write(mCache.mStr);
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
			return true;
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			Flush();
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
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
		uchar*			mCursor;
	};

	class CharWriterAsUtf32ToConsole : public CharWriter
	{
	public:
		inline			CharWriterAsUtf32ToConsole(Utf32Buffer cache) : mCount(0), mCache(cache), mCursor(cache.mStr) { }

		virtual u64		Count() const { return mCount; }
		virtual void	Reset() { mCount = 0; mCursor = mCache.mStr; mCursor[0] = '\0'; }

		void			Flush()
		{
			console->write(mCache.mStr);
			mCursor = mCache.mStr;
			*mCursor = '\0';
		}

		virtual bool	Write(uchar32 c)
		{
			if (mCursor == mCache.mEnd)
				Flush();
			*mCursor++ = c;
			*mCursor = '\0';
			return true;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			Flush();
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			Flush();
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
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

	class CharWriterToAsciiBuffer : public CharWriter
	{
	public:
		inline			CharWriterToAsciiBuffer(char* str, char const * str_end) : mStr(str), mPtr(str), mEnd(str_end), mCount(0) { }
		inline			CharWriterToAsciiBuffer(AsciiBuffer str) : mStr(str.mStr), mPtr(str.mStr), mEnd(str.mEnd), mCount(0) { }

		virtual u64		Count() const { return mCount; }
		virtual void	Reset() { mPtr = mStr; mCount = 0; }

		virtual bool	Write(uchar32 c)
		{
			bool const ok = Write_Prologue(c);
			if (ok)
			{
				mCount += 1;
				*mPtr++ = (uchar)c;
				*mPtr = '\0';
			}
			return ok;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			if (mPtr >= mEnd)
				return false;
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			if (mPtr >= mEnd)
				return false;
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			AsciiConstBuffer buffer(mStr, mPtr);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			AsciiBuffer buffer(mStr, mPtr, mEnd);
			m->Do(buffer);
		}

		u64				mCount;
		char *			mStr;
		char *			mPtr;
		char const *	mEnd;

	private:
		bool			Write_Prologue(uchar32& c) const
		{
			if (mPtr >= mStr && mPtr < mEnd)
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
	};
	
	class CharWriterToUtf32Buffer : public CharWriter
	{
	public:
		inline			CharWriterToUtf32Buffer(uchar32 * str, uchar32 const * str_end) : mStr(str), mPtr(str), mEnd(str_end), mCount(0) { }
		inline			CharWriterToUtf32Buffer(Utf32Buffer str) : mStr(str.mStr), mPtr(str.mStr), mEnd(str.mEnd), mCount(0) { }

		virtual u64		Count() const { return mCount; }
		virtual void	Reset() { mPtr = mStr; mCount = 0; }

		virtual bool	Write(uchar32 c)
		{
			if (mPtr >= mEnd)
				return false;
			*mPtr++ = c;
			return true;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			if (mPtr >= mEnd)
				return false;
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			if (mPtr >= mEnd)
				return false;
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			Utf32ConstBuffer buffer(mStr, mPtr);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			Utf32Buffer buffer(mStr, mPtr, mEnd);
			m->Do(buffer);
		}

		u64				mCount;
		uchar32 *		mStr;
		uchar32 *		mPtr;
		uchar32 const *	mEnd;
	};

	template<int SIZE>
	class CharWriterToAsciiBufferWithBuffer : public CharWriter
	{
	public:
		inline			CharWriterToAsciiBufferWithBuffer() : mWriter(mBuffer, &mBuffer[SIZE])
		{
			mBuffer[SIZE] = '\0';
			mBuffer[SIZE + 1] = '\0';
			mBuffer[SIZE + 2] = '\0';
			mBuffer[SIZE + 3] = '\0';
		}

		virtual u64		Count() const { return mWriter.Count(); }
		virtual void	Reset() { mWriter.Reset(); }

		virtual bool	Write(uchar32 c)
		{
			return mWriter.Write(c);
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			return mWriter.Write(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			return mWriter.Write(b);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			mWriter.Iterate(i);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			mWriter.Modify(m);
		}

	private:
		uchar			mBuffer[SIZE + 4];
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
			return mWriter.Write(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			return mWriter.Write(b);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			mWriter.Iterate(i);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			mWriter.Modify(m);
		}

	private:
		uchar32			mBuffer[SIZE + 1];
		CharWriterToUtf32Buffer mWriter;
	};

	class CharReaderFromAsciiBuffer : public CharReader
	{
	public:
		inline			CharReaderFromAsciiBuffer(const char* str, const char* str_end) 
			: mStr(str)
			, mPtr(str)
			, mEnd(str_end) 
		{ 
			if (mEnd == NULL) 
			{
				mEnd = mStr;
				while (*mEnd != '\0')
				{
					mEnd++;
				}
			}
		}

		virtual uchar32	Peek()
		{
			if (mPtr >= mEnd)
				return '\0';
			uchar32 c = *mPtr;
			return c;
		}

		virtual uchar32	Read()
		{
			if (mPtr >= mEnd)
				return '\0';
			uchar32 c = *mPtr++;
			return c;
		}

		virtual void	Skip()
		{
			if (mPtr < mEnd)
				mPtr++;
		}

		virtual void	Reset()
		{
			mPtr = mStr;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			AsciiConstBuffer buffer(mStr, mEnd);
			i->Do(buffer);
		}

		const char*		mStr;
		const char*		mPtr;
		const char*		mEnd;
	};

	class CharReaderFromUtf32Buffer : public CharReader
	{
	public:
		inline			CharReaderFromUtf32Buffer(const uchar32* str, const uchar32* str_end) 
			: mStr(str)
			, mPtr(str)
			, mEnd(str_end) 
		{ 
			if (mEnd == NULL) 
				mEnd = utf32::endof(mStr, NULL);
		}

		virtual uchar32	Peek()
		{
			if (mPtr >= mEnd)
				return '\0';
			uchar32 c = *mPtr;
			return c;
		}

		virtual uchar32	Read()
		{
			if (mPtr >= mEnd)
				return '\0';
			uchar32 c = *mPtr++;
			return c;
		}

		virtual void	Skip()
		{
			if (mPtr < mEnd)
				mPtr++;
		}

		virtual void	Reset()
		{
			mPtr = mStr;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			Utf32ConstBuffer buffer(mStr, mEnd);
			i->Do(buffer);
		}

		const uchar32*		mStr;
		const uchar32*		mPtr;
		const uchar32*		mEnd;
	};


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	inline bool StrToBool(const uchar32* str, const uchar32* end)
	{
		utf32::crunes s(str, end);
		bool value;
		utf32::parse(s, value);
		return value;
	}

	inline f32 StrToF32(const uchar32* str, const uchar32* end)
	{
		utf32::crunes s(str, end);
		f32 value;
		utf32::parse(s, value);
		return value;
	}

	inline f64 StrToF64(const uchar32* str, const uchar32* end)
	{
		utf32::crunes s(str, end);
		f64 value;
		utf32::parse(s, value);
		return value;
	}

	inline s32 StrToS32(const uchar32* str, const uchar32* end)
	{
		utf32::crunes s(str, end);
		s32 value;
		utf32::parse(s, value);
		return value;
	}

	inline u32 StrToU32(const uchar32* str, const uchar32* end)
	{
		utf32::crunes s(str, end);
		u32 value;
		utf32::parse(s, value);
		return value;
	}

	inline s64 StrToS64(const uchar32* str, const uchar32* end, s32 base)
	{
		utf32::crunes s(str, end);
		s64 value;
		utf32::parse(s, value, base);
		return value;
	}

	inline u64 StrToU64(const uchar32* str, const uchar32* end, s32 base)
	{
		utf32::crunes s(str, end);
		s64 value;
		utf32::parse(s, value, base);
		return value;
	}

};
/**
 *  END xCore namespace
 */

#endif	