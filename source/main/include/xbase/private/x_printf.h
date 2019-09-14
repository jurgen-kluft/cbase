#ifndef __XBASE_STRING_UTILS_H__
#define __XBASE_STRING_UTILS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_debug.h"
#include "xbase/x_double.h"
#include "xbase/x_runes.h"
#include "xbase/x_console.h"

namespace xcore
{
	struct AsciiBuffer
	{
		inline			AsciiBuffer(char * str, u32 len) : m_str((uchar*)str), m_end((uchar*)str + len), m_eos((uchar*)str + len) { }
		inline			AsciiBuffer(char * str, char const * end) : m_str((uchar*)str), m_end((uchar *)end), m_eos((uchar *)end) { }
		inline			AsciiBuffer(char * str, char const * end, char const * eos) : m_str((uchar *)str), m_end((uchar *)end), m_eos((uchar *)eos) { }

		uchar *			m_str;
		uchar const *	m_end;
		uchar const *	m_eos;
	};
	
	struct Utf32Buffer
	{
		inline			Utf32Buffer(uchar32 * str, u32 len) : m_str(str), m_end(str + len), m_eos(str + len) { }
		inline			Utf32Buffer(uchar32 * str, uchar32 const * end) : m_str(str), m_end(end), m_eos(end) { }
		inline			Utf32Buffer(uchar32 * str, uchar32 const * end, uchar32 const * eos) : m_str(str), m_end(end), m_eos(eos) { }
		uchar32 *		m_str;
		uchar32 const *	m_end;
		uchar32 const *	m_eos;
	};

	struct AsciiConstBuffer
	{
		inline			AsciiConstBuffer(char const* str, u32 len) : m_str((uchar*)str), m_end((uchar*)str + len) { }
		inline			AsciiConstBuffer(char const* str, char const* end) : m_str((uchar*)str), m_end((uchar*)end) { }

		uchar const*	m_str;
		uchar const*	m_end;
	};

	struct Utf32ConstBuffer
	{
		inline			Utf32ConstBuffer(uchar32 const* str, u32 len) : m_str(str), m_end(str + len) { }
		inline			Utf32ConstBuffer(uchar32 const* str, uchar32 const* end) : m_str(str), m_end(end) { }

		uchar32 const*	m_str;
		uchar32 const*	m_end;
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
		inline				CharBufferFill(uchar32 c) : m_fill(c) {}

		virtual void		Do(AsciiBuffer b)
		{
			uchar* ptr = b.m_str;
			while (ptr < b.m_end)
				*ptr++ = (uchar)m_fill;
		}

		virtual void		Do(Utf32Buffer b)
		{
			uchar32* ptr = b.m_str;
			while (ptr < b.m_end)
				*ptr++ = m_fill;
		}
	private:
		uchar32		m_fill;
	};

	class CharBufferPad : public CharBufferModifier
	{
	public:
		enum EMode { HEAD = 0, TAIL = 1 };
		inline				CharBufferPad(EMode mode, uchar32 f, uchar32 r) : m_mode(mode), m_find(f), m_replace(r) {}

		virtual void		Do(AsciiBuffer b)
		{
			if (m_mode == HEAD)
			{
				uchar* ptr = b.m_str;
				while (ptr < b.m_end && *ptr == m_find)
					*ptr++ = (uchar)m_replace;
			}
			else
			{
				uchar* ptr = b.m_str + (b.m_end - b.m_str) - 1;
				while (ptr >= b.m_str && *ptr == m_find)
					*ptr-- = (uchar)m_replace;
			}
		}

		virtual void		Do(Utf32Buffer b)
		{
			if (m_mode == HEAD)
			{
				uchar32* ptr = b.m_str;
				while (ptr < b.m_end && *ptr == m_find)
					*ptr++ = m_replace;
			}
			else
			{
				uchar32* ptr = b.m_str + (b.m_end - b.m_str) - 1;
				while (ptr >= b.m_str && *ptr == m_find)
					*ptr-- = m_replace;
			}
		}
	private:
		EMode		m_mode;
		uchar32		m_find;
		uchar32		m_replace;
	};


	class CharBufferToCharWriter : public CharBufferIterator
	{
	public:
		inline				CharBufferToCharWriter(CharWriter* writer) : m_writer(writer) { }

		virtual void		Do(AsciiConstBuffer b)
		{
			uchar const* src = b.m_str;
			while (src < b.m_end)
			{
				char c = *src++;
				m_writer->Write(c);
			}
		}

		virtual void		Do(Utf32ConstBuffer b)
		{
			uchar32 const* src = b.m_str;
			while (src < b.m_end)
			{
				uchar32 c = *src++;
				m_writer->Write(c);
			}
		}

	private:
		CharWriter * m_writer;
	};

	class CharBufferContainsChar : public CharBufferIterator
	{
	public:
		inline				CharBufferContainsChar(uchar32 c) : m_char(c), m_found(false) { }

		inline bool			Found() const { return m_found; }

		virtual void		Do(AsciiConstBuffer b)
		{
			uchar const* src = b.m_str;
			while (src < b.m_end)
			{
				uchar32 c = *src++;
				if (c == m_char)
				{
					m_found = true;
					break;
				}
			}
		}

		virtual void		Do(Utf32ConstBuffer b)
		{
			uchar32 const* src = b.m_str;
			while (src < b.m_end)
			{
				uchar32 c = *src++;
				if (c == m_char)
				{
					m_found = true;
					break;
				}
			}
		}

	private:
		bool				m_found;
		uchar32				m_char;
	};

	class CharBufferFindOneOf : public CharBufferIterator
	{
	public:
		inline				CharBufferFindOneOf(CharReader* one_of) : m_oneof(one_of), m_found('\0') { }

		inline uchar		Found() const { return (uchar)m_found; }

		virtual void		Do(AsciiConstBuffer b)
		{
			m_found = '\0';
			uchar const* src = b.m_str;
			while (src < b.m_end)
			{
				uchar32 c = *src++;
				CharBufferContainsChar contains(c);
				m_oneof->Iterate(&contains);
				if (contains.Found())
				{
					m_found = c;
					break;
				}
			}
		}

		virtual void		Do(Utf32ConstBuffer b)
		{
			m_found = '\0';
			uchar32 const* src = b.m_str;
			while (src < b.m_end)
			{
				uchar32 c = *src++;
				CharBufferContainsChar contains(c);
				m_oneof->Iterate(&contains);
				if (contains.Found())
				{
					m_found = c;
					break;
				}
			}
		}

	private:
		CharReader * m_oneof;
		uchar32		 m_found;
	};

	class CharBufferReversedWriter : public CharWriter
	{
	public:
		utf32::runes	m_str;
		uchar32*		m_begin;
		uchar32*		m_end;

		inline			CharBufferReversedWriter() {}

		inline			CharBufferReversedWriter(utf32::runes str) : m_str(str) { Reset(); }

		virtual u64		Count() const { return 0; }

		virtual void	Reset()
		{
			m_str.m_end = m_str.m_str;
			*m_str.m_end = '\0';

			while (utf::can_write(m_str))
				*m_str.m_end++ = ' ';

			m_begin = m_str.m_end;
			m_end = m_str.m_end;
		}

		void			Flush()
		{
		}

		virtual bool	Write(uchar32 c)
		{
			if (m_begin > m_str.m_str)
			{
				m_begin -= 1;
				*m_begin = c;
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
		inline			CharWriterCounter() : m_count(0) { }

		virtual u64		Count() const { return m_count; }
		virtual void	Reset() { m_count = 0; }

		virtual bool	Write(uchar32 c)
		{
			m_count++;
			return true;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			m_count += b.m_end - b.m_str;
			return true;
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			m_count += b.m_end - b.m_str;
			return true;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
		}

		virtual void	Modify(CharBufferModifier* m)
		{
		}

	private:
		u64				m_count;
	};

	class CharWriterAsAsciiToConsole : public CharWriter
	{
	public:
		inline			CharWriterAsAsciiToConsole(AsciiBuffer cache) : m_count(0), m_cache(cache), m_cursor(cache.m_str) { }

		virtual u64		Count() const { return m_count; }

		virtual void	Reset() { m_count = 0; m_cursor = m_cache.m_str; m_cursor[0] = '\0'; }

		void			Flush()
		{
			console->write(m_cache.m_str);
			m_cursor = m_cache.m_str;
			m_cursor[0] = '\0';
		}

		virtual bool	Write(uchar32 c)
		{
			if (m_cursor == m_cache.m_end)
				Flush();

			m_cursor[0] = (char)c;
			m_cursor[1] = '\0';
			m_cursor += 1;

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
			AsciiConstBuffer buffer(m_cache.m_str, m_cursor);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			AsciiBuffer buffer(m_cache.m_str, m_cursor, m_cache.m_end);
			m->Do(buffer);
		}

	private:
		u64				m_count;
		AsciiBuffer		m_cache;
		uchar*			m_cursor;
	};

	class CharWriterAsUtf32ToConsole : public CharWriter
	{
	public:
		inline			CharWriterAsUtf32ToConsole(Utf32Buffer cache) : m_count(0), m_cache(cache), m_cursor(cache.m_str) { }

		virtual u64		Count() const { return m_count; }
		virtual void	Reset() { m_count = 0; m_cursor = m_cache.m_str; m_cursor[0] = '\0'; }

		void			Flush()
		{
			console->write(m_cache.m_str);
			m_cursor = m_cache.m_str;
			*m_cursor = '\0';
		}

		virtual bool	Write(uchar32 c)
		{
			if (m_cursor == m_cache.m_end)
				Flush();
			*m_cursor++ = c;
			*m_cursor = '\0';
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
			Utf32ConstBuffer buffer(m_cache.m_str, m_cursor);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			Utf32Buffer buffer(m_cache.m_str, m_cursor, m_cache.m_end);
			m->Do(buffer);
		}

	private:
		u64				m_count;
		Utf32Buffer		m_cache;
		uchar32*		m_cursor;
	};

	class CharWriterToAsciiBuffer : public CharWriter
	{
	public:
		inline			CharWriterToAsciiBuffer(char* str, char const * str_end) : m_str(str), m_ptr(str), m_end(str_end), m_count(0) { }
		inline			CharWriterToAsciiBuffer(AsciiBuffer str) : m_str(str.m_str), m_ptr(str.m_str), m_end(str.m_end), m_count(0) { }

		virtual u64		Count() const { return m_count; }
		virtual void	Reset() { m_ptr = m_str; m_count = 0; }

		virtual bool	Write(uchar32 c)
		{
			bool const ok = Write_Prologue(c);
			if (ok)
			{
				m_count += 1;
				*m_ptr++ = (uchar)c;
				*m_ptr = '\0';
			}
			return ok;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			if (m_ptr >= m_end)
				return false;
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			if (m_ptr >= m_end)
				return false;
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			AsciiConstBuffer buffer(m_str, m_ptr);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			AsciiBuffer buffer(m_str, m_ptr, m_end);
			m->Do(buffer);
		}

		u64				m_count;
		char *			m_str;
		char *			m_ptr;
		char const *	m_end;

	private:
		bool			Write_Prologue(uchar32& c) const
		{
			if (m_ptr >= m_str && m_ptr < m_end)
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
		inline			CharWriterToUtf32Buffer(uchar32 * str, uchar32 const * str_end) : m_str(str), m_ptr(str), m_end(str_end), m_count(0) { }
		inline			CharWriterToUtf32Buffer(Utf32Buffer str) : m_str(str.m_str), m_ptr(str.m_str), m_end(str.m_end), m_count(0) { }

		virtual u64		Count() const { return m_count; }
		virtual void	Reset() { m_ptr = m_str; m_count = 0; }

		virtual bool	Write(uchar32 c)
		{
			if (m_ptr >= m_end)
				return false;
			*m_ptr++ = c;
			return true;
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			if (m_ptr >= m_end)
				return false;
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			if (m_ptr >= m_end)
				return false;
			CharBufferToCharWriter writer(this);
			writer.Do(b);
			return true;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			Utf32ConstBuffer buffer(m_str, m_ptr);
			i->Do(buffer);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			Utf32Buffer buffer(m_str, m_ptr, m_end);
			m->Do(buffer);
		}

		u64				m_count;
		uchar32 *		m_str;
		uchar32 *		m_ptr;
		uchar32 const *	m_end;
	};

	template<int SIZE>
	class CharWriterToAsciiBufferWithBuffer : public CharWriter
	{
	public:
		inline			CharWriterToAsciiBufferWithBuffer() : m_writer(m_buffer, &m_buffer[SIZE])
		{
			m_buffer[SIZE] = '\0';
			m_buffer[SIZE + 1] = '\0';
			m_buffer[SIZE + 2] = '\0';
			m_buffer[SIZE + 3] = '\0';
		}

		virtual u64		Count() const { return m_writer.Count(); }
		virtual void	Reset() { m_writer.Reset(); }

		virtual bool	Write(uchar32 c)
		{
			return m_writer.Write(c);
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			return m_writer.Write(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			return m_writer.Write(b);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			m_writer.Iterate(i);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			m_writer.Modify(m);
		}

	private:
		uchar			m_buffer[SIZE + 4];
		CharWriterToAsciiBuffer m_writer;
	};

	template<int SIZE>
	class CharWriterToUtf32BufferWithBuffer : public CharWriter
	{
	public:
		inline			CharWriterToUtf32BufferWithBuffer() : m_writer(m_buffer, &m_buffer[SIZE])
		{
			m_buffer[SIZE] = '\0';
		}

		virtual u64		Count() const { return m_writer.Count(); }
		virtual void	Reset() { m_writer.Reset(); }

		virtual bool	Write(uchar32 c)
		{
			return m_writer.Write(c);
		}

		virtual bool	Write(AsciiConstBuffer b)
		{
			return m_writer.Write(b);
		}

		virtual bool	Write(Utf32ConstBuffer b)
		{
			return m_writer.Write(b);
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			m_writer.Iterate(i);
		}

		virtual void	Modify(CharBufferModifier* m)
		{
			m_writer.Modify(m);
		}

	private:
		uchar32			m_buffer[SIZE + 1];
		CharWriterToUtf32Buffer m_writer;
	};

	class CharReaderFromAsciiBuffer : public CharReader
	{
	public:
		inline			CharReaderFromAsciiBuffer(const char* str, const char* str_end) 
			: m_str(str)
			, m_ptr(str)
			, m_end(str_end) 
		{ 
			if (m_end == NULL) 
			{
				m_end = m_str;
				while (*m_end != '\0')
				{
					m_end++;
				}
			}
		}

		virtual uchar32	Peek()
		{
			if (m_ptr >= m_end)
				return '\0';
			uchar32 c = *m_ptr;
			return c;
		}

		virtual uchar32	Read()
		{
			if (m_ptr >= m_end)
				return '\0';
			uchar32 c = *m_ptr++;
			return c;
		}

		virtual void	Skip()
		{
			if (m_ptr < m_end)
				m_ptr++;
		}

		virtual void	Reset()
		{
			m_ptr = m_str;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			AsciiConstBuffer buffer(m_str, m_end);
			i->Do(buffer);
		}

		const char*		m_str;
		const char*		m_ptr;
		const char*		m_end;
	};

	class CharReaderFromUtf32Buffer : public CharReader
	{
	public:
		inline			CharReaderFromUtf32Buffer(const uchar32* str, const uchar32* str_end) 
			: m_str(str)
			, m_ptr(str)
			, m_end(str_end) 
		{ 
			if (m_end == NULL) 
			{
				m_end = m_str;
				while (*m_end != '\0')
					++m_end;
			}
		}

		virtual uchar32	Peek()
		{
			if (m_ptr >= m_end)
				return '\0';
			uchar32 c = *m_ptr;
			return c;
		}

		virtual uchar32	Read()
		{
			if (m_ptr >= m_end)
				return '\0';
			uchar32 c = *m_ptr++;
			return c;
		}

		virtual void	Skip()
		{
			if (m_ptr < m_end)
				m_ptr++;
		}

		virtual void	Reset()
		{
			m_ptr = m_str;
		}

		virtual void	Iterate(CharBufferIterator* i)
		{
			Utf32ConstBuffer buffer(m_str, m_end);
			i->Do(buffer);
		}

		const uchar32*		m_str;
		const uchar32*		m_ptr;
		const uchar32*		m_end;
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

#endif	