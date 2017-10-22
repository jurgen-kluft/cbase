#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_double.h"
#include "xbase/x_string_ascii.h"
#include "xbase/x_string_utf.h"
#include "xbase/x_console.h"


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
		inline			AsciiBuffer(char * str, u32 len) : mStr(str), mEnd(str + len), mEos(str + len) { }
		inline			AsciiBuffer(char * str, char * end, char * eos) : mStr(str), mEnd(end), mEos(eos) { }
		char*			mStr;
		char*			mEnd;
		char*			mEos;
	};

	struct Utf32Buffer
	{
		inline			Utf32Buffer(uchar32 * str, u32 len) : mStr(str), mEnd(str + len), mEos(str + len) { }
		inline			Utf32Buffer(uchar32 * str, uchar32 * end, uchar32 * eos) : mStr(str), mEnd(end), mEos(eos) { }
		uchar32*		mStr;
		uchar32*		mEnd;
		uchar32*		mEos;
	};

	struct AsciiConstBuffer
	{
		inline			AsciiConstBuffer(char const* str, u32 len) : mStr(str), mEnd(str + len) { }
		inline			AsciiConstBuffer(char const* str, char const* end) : mStr(str), mEnd(end) { }
		char const*		mStr;
		char const*		mEnd;
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

	class CharBufferPad : public CharBufferModifier
	{
	public:
		enum EMode { HEAD = 0, TAIL = 1 };
		inline				CharBufferPad(EMode mode, uchar32 f, uchar32 r) : mMode(mode), mFind(f), mReplace(r) {}

		virtual void		Do(AsciiBuffer b)
		{
			if (mode == HEAD)
			{
				char* ptr = b.mStr;
				while (ptr < b.mEnd && *ptr == mFind)
					*ptr++ = mReplace;
			}
			else
			{
				char* ptr = b.mEnd - 1;
				while (ptr >= b.mStr && *ptr == mFind)
					*ptr-- = mReplace;				
			}
		}

		virtual void		Do(Utf32Buffer b)
		{
			if (mode == HEAD)
			{
				uchar32* ptr = b.mStr;
				while (ptr < b.mEnd && *ptr == mFind)
					*ptr++ = mReplace;
			}
			else
			{
				uchar32* ptr = b.mEnd - 1;
				while (ptr >= b.mStr && *ptr == mFind)
					*ptr-- = mReplace;				
			}
		}
	private:
		EMode		mMode;
		uchar32		mFind;
		uchar32		mReplace;
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

	class CharWriterToAsciiBuffer : public CharWriter
	{
	public:
		inline			CharWriterToAsciiBuffer(char* str, char* str_end) : mStr(str), mPtr(str), mEnd(str_end), mCount(0) { mEnd[0] = '\0'; }

		virtual u64		Count() const	{ return mCount; }
		virtual void	Reset() { mPtr = mStr; mCount = 0; }
		
		virtual bool	Write(uchar32 c)
		{
			bool const ok = Write_Prologue(c);
			if (ok)
			{
				*mPtr++ = c;
				*mPtr = '\0';
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

		u64				mCount;
		char*			mStr;
		char*			mPtr;
		char*			mEnd;
	};

	class CharWriterToUtf32Buffer : public CharWriter
	{
	public:
		inline			CharWriterToUtf32Buffer(uchar32* str, uchar32* str_end) : mStr(str), mPtr(str), mEnd(str_end), mCount(0) { }

		virtual u64		Count() const	{ return mCount; }
		virtual void	Reset() { mPtr = mStr; mCount = 0; }
		
		virtual bool	Write(uchar32 c)
		{
			if (mPtr >= mEnd)
				return false;
			*mPtr++ = c;
			*mPtr = '\0';
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
			Utf32ConstBuffer buffer(mStr, mPtr);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			Utf32Buffer buffer(mStr, mPtr, mEnd);
			m->Do(buffer);
		}

	private:
		u64				mCount;
		uchar32*		mStr;
		uchar32*		mPtr;
		uchar32*		mEnd;
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

		
	class CharReaderFromAsciiBuffer : public CharReader
	{
	public:
		inline			CharReaderFromAsciiBuffer(const char* str, const char* str_end) : mStr(str), mPtr(str), mEnd(str_end) { }

		virtual uchar32	Peek()
		{
			if (mPtr == mEnd)
				return '\0';
			uchar32 c = *mPtr;
			return c;
		}

		virtual uchar32	Read()
		{
			if (mPtr == mEnd)
				return '\0';
			uchar32 c = *mPtr++;
			return c;
		}

	private:
		const char*		mStr;
		const char*		mPtr;
		const char*		mEnd;
	};


	class CharReaderFromUtf32Buffer : public CharReader
	{
	public:
		inline			CharReaderFromUtf32Buffer(const uchar32* str, const uchar32* str_end) : mStr(str), mPtr(str), mEnd(str_end) { }

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

	private:
		const uchar32*		mStr;
		const uchar32*		mPtr;
		const uchar32*		mEnd;
	};

};
/**
 *  END xCore namespace
 */
