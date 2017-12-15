#ifndef __XCORE_BUFFER_H__
#define __XCORE_BUFFER_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_debug.h"

namespace xcore
{
	class xcchars;
	class xcbuffer;

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	class xchars
	{
	public:
					xchars(uchar* str);
					xchars(uchar* str, uchar* end);
					xchars(uchar* str, uchar* end, uchar* eos);
					xchars(uchar* str, uchar* eos, const xchars& other);

		u32			size() const;
		void		reset();
		void		clear();

		xbuffer		buffer() const { return xbuffer(size(), (xbyte*)m_str); }
		xcbuffer	cbuffer() const { return xcbuffer(size(), (const xbyte*)m_str); }

		s32			copy(const xchars& other);
		s32			copy(const xcchars& other);

		s32			compare(const xchars& other) const;
		s32			compare(const xcchars& other) const;

		xchars&		operator = (const xchars& other);
		xchars&		operator = (const xcchars& other);

		bool		operator == (const xchars& other) const;
		bool		operator != (const xchars& other) const;
		bool		operator == (const xcchars& other) const;
		bool		operator != (const xcchars& other) const;

		uchar		operator [] (int i) const;

		uchar*		m_str;
		uchar*		m_end;
		uchar*		m_eos;
	};

	class xcchars
	{
	public:
		inline		xcchars(uchar const* str) : m_str(str), m_end(str) { while (*m_end != '\0') ++m_end; }
		inline		xcchars(uchar const* str, uchar const* end) : m_str(str), m_end(end) { }

		u32			size() const { return m_end - m_str; }

		xcbuffer	buffer() const { return xcbuffer(size(), (const xbyte*)m_str); }

		s32			compare(const xchars& other) const;
		s32			compare(const xcchars& other) const;

		bool		operator == (const xchars& other) const;
		bool		operator != (const xchars& other) const;

		bool		operator == (const xcchars& other) const;
		bool		operator != (const xcchars& other) const;

		uchar		operator [] (int i) const;

		uchar const*m_str;
		uchar const*m_end;
	};

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	class xbuffer
	{
	public:
		inline		xbuffer(u32 len, xbyte* data) : m_len(len), m_data(data) { reset(); }

		u32			size() const					{ return m_len; }
		void		reset();
		void		clear();

		xbuffer		buffer() const					{ return buffer_at(0); }
		xbuffer		buffer_at(u32 idx) const		{ ASSERT(idx < len); return xbuffer(size() - idx, (xbyte*)&m_data[idx]); }

		xcbuffer	cbuffer() const					{ return cbuffer_at(0); }
		xcbuffer	cbuffer_at(u32 idx) const		{ ASSERT(idx < len); return xcbuffer(size() - idx, (xbyte*)&m_data[idx]); }

		void		write(const xbuffer& other)		{ write_at(0, other.cbuffer()); }
		void		write(const xcbuffer& other)	{ write_at(0, other); }

		void		write_at(u32 idx, const xcbuffer& other)
		{
			ASSERT(idx < len);
			xbyte* dst = &m_data[idx];
			u32 dst_size = m_len - idx;
			for (u32 i = 0; i < other.size() && i < dst_size; i++)
				dst[i] = other[i];
		}

		s32			compare(const xbuffer& other) const;

		xbuffer&	operator = (const xbuffer& other);
		xbuffer&	operator = (const xcbuffer& other);

		bool		operator == (const xbuffer& other) const;
		bool		operator != (const xbuffer& other) const;

		xbyte&		operator [] (int i);
		xbyte		operator [] (int i) const;

		s32			m_len;
		xbyte*		m_data;
	};

	class xcbuffer
	{
	public:
		inline		xcbuffer(u32 len, xbyte const* data) : m_len(len), m_data(data) {}

		u32			size() const { return m_len; }

		s32			compare(const xcbuffer& other) const;

		bool		operator == (const xcbuffer& other) const;
		bool		operator != (const xcbuffer& other) const;

		xbyte		operator [] (int i) const;

		s32			m_len;
		xbyte const*m_data;
	};

	#define XBUFFER(name, len)													\
	class name																	\
	{																			\
	public:																		\
		inline		name() { reset(0); }										\
		inline		name(xbyte b) { reset(b); }									\
		inline		name(name const& other) { *this = other; }					\
		inline		name(xbuffer const& other) { *this = other; }				\
		u32			size() const { return len; }								\
		xbuffer		buffer() { return xbuffer(size(), m_data); }				\
		xcbuffer	cbuffer() const { return xcbuffer(size(), m_data); }		\
		void		reset(xbyte fill) {											\
			for (u32 i = 0; i < size(); ++i)									\
				m_data[i] = fill;												\
		}																		\
		void		clear() {													\
			reset(0);															\
		}																		\
		s32			compare(const name& other) const {							\
			for (u32 i = 0; i < size(); ++i) {									\
				if (m_data[i] < other.m_data[i])								\
					return -1;													\
				else if (m_data[i] > other.m_data[i])							\
					return 1;													\
			}																	\
			return 0;															\
		}																		\
		name&		operator = (const xbuffer& other) {							\
			u32 i = 0;															\
			for (; i < size() && i < other.size(); ++i)							\
				m_data[i] = other.m_data[i];									\
			for (; i < size(); ++i)												\
				m_data[i] = 0;													\
			return *this;														\
		}																		\
		name&		operator = (const name& other) {							\
			for (u32 i=0; i < size(); ++i)										\
				m_data[i] = other.m_data[i];									\
			return *this;														\
		}																		\
		bool		operator == (const name& other) const {						\
			return compare(other) == 0;											\
		}																		\
		bool		operator != (const name& other) const {						\
			return compare(other) != 0;											\
		}																		\
		bool		operator > (const name& other) const {						\
			return compare(other) == 1;											\
		}																		\
		bool		operator >= (const name& other) const {						\
			return compare(other) >= 0;											\
		}																		\
		bool		operator < (const name& other) const {						\
			return compare(other) == -1;										\
		}																		\
		bool		operator <= (const name& other) const {						\
			return compare(other) <= 0;											\
		}																		\
		xbyte&		operator [] (int i) {										\
			ASSERT(i >= 0 && i < (s32)size());									\
			return m_data[i];													\
		}																		\
		xbyte		operator [] (int i) const {									\
			ASSERT(i >= 0 && i < (s32)size());									\
			return m_data[i];													\
		}																		\
		xbyte		m_data[len];												\
	}

	#define XCHARS(name, len)													\
	class name																	\
	{																			\
	public:																		\
		inline		name() { reset(); }											\
		inline		name(name const& other) { reset(); *this = other; }			\
		inline		name(xcchars const& other) { reset(); *this = other; }		\
		u32			size() const { return m_end - m_str; }						\
		xchars		chars() { return xchars(m_str, m_end, m_eos); }				\
		xcchars		cchars() const { return xcchars(m_str, m_end); }			\
		void		reset() {													\
			m_end = m_str;														\
			*m_end = '\0';														\
			m_eos = &m_str[len - 1];											\
			*m_eos = '\0';														\
		}																		\
		void		clear() {													\
			reset();															\
		}																		\
		s32			compare(const xcchars& other) const {						\
			if (size() < other.size())											\
				return -1;														\
			if (size() > other.size())											\
				return 1;														\
			uchar const * R = other.m_str;										\
			uchar const * L = m_str;											\
			while (L < m_end && R < other.m_end) {								\
				if (*L < *R) return -1;											\
				if (*L > *R) return 1;											\
				L++;															\
				R++;															\
			}																	\
			return 0;															\
		}																		\
		s32			compare(const name& other) const {							\
			uchar const * R = other.m_str;										\
			uchar const * L = m_str;											\
			while (L < m_end && R < other.m_end) {								\
				if (*L < *R) return -1;											\
				if (*L > *R) return 1;											\
				L++;															\
				R++;															\
			}																	\
			return 0;															\
		}																		\
		name&		operator = (const xcchars& other) {							\
			uchar const* src = other.m_str;										\
			uchar * dst = m_str;												\
			while (dst < m_eos && src <= other.m_end)							\
				*dst++ = *src++;												\
			m_end = dst;														\
			if (dst < m_eos)													\
				m_end--;														\
			return *this;														\
		}																		\
		name&		operator = (const name& other) {							\
			uchar const* src = other.m_str;										\
			uchar * dst = m_str;												\
			while (dst < m_eos && src <= other.m_end)							\
				*dst++ = *src++;												\
			m_end = dst;														\
			if (dst < m_eos)													\
				m_end--;														\
			return *this;														\
		}																		\
		bool		operator == (const name& other) const {						\
			return compare(other) == 0;											\
		}																		\
		bool		operator != (const name& other) const {						\
			return compare(other) != 0;											\
		}																		\
		bool		operator > (const name& other) const {						\
			return compare(other) == 1;											\
		}																		\
		bool		operator >= (const name& other) const {						\
			return compare(other) >= 0;											\
		}																		\
		bool		operator < (const name& other) const {						\
			return compare(other) == -1;										\
		}																		\
		bool		operator <= (const name& other) const {						\
			return compare(other) <= 0;											\
		}																		\
		uchar&		operator [] (int i) {										\
			ASSSERT(i < size());												\
			if (i >= 0 && i < (s32)size())										\
				return m_str[i];												\
		}																		\
		uchar		operator [] (int i) const {									\
			if (i >= 0 && i < (s32)size())										\
				return m_str[i];												\
			return '\0';														\
		}																		\
		uchar		m_str[len];													\
		uchar *		m_end;														\
		uchar *		m_eos;														\
	}



	// Inline functions
	#include "xbase/private/x_buffer.h"

	XBUFFER(xbuffer16, 16);
	XBUFFER(xbuffer32, 32);
	XBUFFER(xbuffer64, 64);
	XBUFFER(xbuffer128, 128);
	XBUFFER(xbuffer256, 256);
	XBUFFER(xbuffer512, 512);
	XBUFFER(xbuffer1024, 1024);

	XCHARS(xchars16, 16);
	XCHARS(xchars32, 32);
	XCHARS(xchars64, 64);
	XCHARS(xchars128, 128);
	XCHARS(xchars256, 256);
	XCHARS(xchars512, 512);
	XCHARS(xchars1024, 1024);

}

#endif	///< __XCORE_BUFFER_H__