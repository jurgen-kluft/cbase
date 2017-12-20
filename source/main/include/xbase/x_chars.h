#ifndef __XCORE_CHARS_H__
#define __XCORE_CHARS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_debug.h"

namespace xcore
{
	class xcuchars;

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// a mutable range of []uchar (ASCII)
	class xuchars
	{
	public:
					xuchars(uchar* str);
					xuchars(uchar* str, uchar* end);
					xuchars(uchar* str, uchar* end, const xcuchars& other);

		u32			size() const;
		void		reset();
		void		clear();

		xcuchars	cchars() const;

		s32			copy(const xuchars& other);
		s32			copy(const xcuchars& other);

		s32			compare(const char* other) const;
		s32			compare(const xuchars& other) const;
		s32			compare(const xcuchars& other) const;

		xuchars&	operator = (const xuchars& other);
		xuchars&	operator = (const xcuchars& other);

		bool		operator == (const xuchars& other) const;
		bool		operator != (const xuchars& other) const;
		bool		operator <  (const xuchars& other) const;
		bool		operator <= (const xuchars& other) const;
		bool		operator >  (const xuchars& other) const;
		bool		operator >= (const xuchars& other) const;

		bool		operator == (const xcuchars& other) const;
		bool		operator != (const xcuchars& other) const;
		bool		operator <  (const xcuchars& other) const;
		bool		operator <= (const xcuchars& other) const;
		bool		operator >  (const xcuchars& other) const;
		bool		operator >= (const xcuchars& other) const;

		uchar&		operator [] (int i);
		uchar		operator [] (int i) const;

		uchar*		m_str;
		uchar*		m_end;
	};

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// a read-only range of []uchar (ASCII)
	class xcuchars
	{
	public:
		inline		xcuchars(uchar const* str) : m_str(str), m_end(str) { while (*m_end != '\0') ++m_end; }
		inline		xcuchars(uchar const* str, uchar const* end) : m_str(str), m_end(end) { }

		u32			size() const { return (u32)(u64)(m_end - m_str); }

		s32			compare(const char* other) const;
		s32			compare(const xuchars& other) const;
		s32			compare(const xcuchars& other) const;

		bool		operator == (const xuchars& other) const;
		bool		operator != (const xuchars& other) const;

		bool		operator == (const xcuchars& other) const;
		bool		operator != (const xcuchars& other) const;

		uchar		operator [] (int i) const;

		uchar const*m_str;
		uchar const*m_end;
	};


	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	class xcuchar32s;

	// a mutable range of []uchar32 (UTF-32)
	class xuchar32s
	{
	public:
					xuchar32s(uchar32* str);
					xuchar32s(uchar32* str, uchar32* end);
					xuchar32s(uchar32* str, uchar32* end, const xcuchar32s& other);

		u32			size() const;
		void		reset();
		void		clear();

		xcuchar32s	cchars() const;

		s32			copy(const xuchar32s& other);
		s32			copy(const xcuchar32s& other);

		s32			compare(const xuchar32s& other) const;
		s32			compare(const xcuchar32s& other) const;

		xuchar32s&	operator = (const xuchar32s& other);
		xuchar32s&	operator = (const xcuchar32s& other);

		bool		operator == (const xuchar32s& other) const;
		bool		operator != (const xuchar32s& other) const;
		bool		operator == (const xcuchar32s& other) const;
		bool		operator != (const xcuchar32s& other) const;

		uchar32&	operator [] (int i);
		uchar32		operator [] (int i) const;

		uchar32*	m_str;
		uchar32*	m_end;
	};

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// a read-only range of []uchar32 (UTF-32)
	class xcuchar32s
	{
	public:
		inline		xcuchar32s(uchar32 const* str) : m_str(str), m_end(str) { while (*m_end != '\0') ++m_end; }
		inline		xcuchar32s(uchar32 const* str, uchar32 const* end) : m_str(str), m_end(end) { }

		u32			size() const { return (u32)(u64)(m_end - m_str); }

		s32			compare(const xuchar32s& other) const;
		s32			compare(const xcuchar32s& other) const;

		bool		operator == (const xuchar32s& other) const;
		bool		operator != (const xuchar32s& other) const;

		bool		operator == (const xcuchar32s& other) const;
		bool		operator != (const xcuchar32s& other) const;

		uchar32		operator [] (int i) const;

		uchar32 const*m_str;
		uchar32 const*m_end;
	};


	#define XUCHARS(name, len)													\
	class name																	\
	{																			\
	public:																		\
		inline		name() { reset(); }											\
		inline		name(name const& other) { reset(); *this = other; }			\
		inline		name(xcuchars const& other) { reset(); *this = other; }		\
		u32			size() const { return (u32)(u64)(m_end - m_str); }			\
		uchar const* eos() const { return &m_str[len-1]; }						\
		xuchars		chars() { return xuchars(m_str, m_end); }					\
		xcuchars	cchars() const { return xcuchars(m_str, m_end); }			\
		void		reset() {													\
			m_end = m_str;														\
			*m_end = '\0';														\
			m_str[len-1] = '\0';												\
		}																		\
		void		clear() {													\
			reset();															\
		}																		\
		s32			compare(const xcuchars& other) const {						\
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
		name&		operator = (const xcuchars& other) {						\
			uchar const* src = other.m_str;										\
			uchar * dst = m_str;												\
			while (dst < eos() && src <= other.m_end)							\
				*dst++ = *src++;												\
			m_end = dst;														\
			if (dst < eos())													\
				m_end--;														\
			return *this;														\
		}																		\
		name&		operator = (const name& other) {							\
			uchar const* src = other.m_str;										\
			uchar * dst = m_str;												\
			while (dst < eos() && src <= other.m_end)							\
				*dst++ = *src++;												\
			m_end = dst;														\
			if (dst < eos())													\
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
			if (i < 0)															\
				return m_str[0];												\
			if (i >= (s32)size())												\
				return m_str[(s32)size() - 1];									\
			return m_str[i];													\
		}																		\
		uchar		operator [] (int i) const {									\
			if (i < 0)															\
				return m_str[0];												\
			if (i >= (s32)size())												\
				return m_str[(s32)size() - 1];									\
			return m_str[i];													\
		}																		\
		uchar		m_str[len];													\
		uchar *		m_end;														\
	}

	#define XUCHAR32S(name, len)												\
	class name																	\
	{																			\
	public:																		\
		inline		name() { reset(); }											\
		inline		name(name const& other) { reset(); *this = other; }			\
		inline		name(xcuchars const& other) { reset(); *this = other; }		\
		u32			size() const { return (u32)(u64)(m_end - m_str); }			\
		uchar32 const* eos() const { return &m_str[len-1]; }					\
		xuchar32s	chars() { return xuchar32s(m_str, m_end); }					\
		xcuchar32s	cchars() const { return xcuchar32s(m_str, m_end); }			\
		void		reset() {													\
			m_end = m_str;														\
			*m_end = '\0';														\
			m_str[len-1] = '\0';												\
		}																		\
		void		clear() {													\
			reset();															\
		}																		\
		s32			compare(const xcuchar32s& other) const {					\
			if (size() < other.size())											\
				return -1;														\
			if (size() > other.size())											\
				return 1;														\
			uchar32 const * R = other.m_str;									\
			uchar32 const * L = m_str;											\
			while (L < m_end && R < other.m_end) {								\
				if (*L < *R) return -1;											\
				if (*L > *R) return 1;											\
				L++;															\
				R++;															\
			}																	\
			return 0;															\
		}																		\
		s32			compare(const name& other) const {							\
			uchar32 const * R = other.m_str;									\
			uchar32 const * L = m_str;											\
			while (L < m_end && R < other.m_end) {								\
				if (*L < *R) return -1;											\
				if (*L > *R) return 1;											\
				L++;															\
				R++;															\
			}																	\
			return 0;															\
		}																		\
		name&		operator = (const xcuchar32s& other) {						\
			uchar32 const* src = other.m_str;									\
			uchar32 * dst = m_str;												\
			while (dst < eos() && src <= other.m_end)							\
				*dst++ = *src++;												\
			m_end = dst;														\
			if (dst < eos())													\
				m_end--;														\
			return *this;														\
		}																		\
		name&		operator = (const name& other) {							\
			uchar32 const* src = other.m_str;									\
			uchar32 * dst = m_str;												\
			while (dst < eos() && src <= other.m_end)							\
				*dst++ = *src++;												\
			m_end = dst;														\
			if (dst < eos())													\
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
		uchar32&		operator [] (int i) {									\
			if (i < 0)															\
				return m_str[0];												\
			if (i >= (s32)size())												\
				return m_str[(s32)size() - 1];									\
			return m_str[i];													\
		}																		\
		uchar32		operator [] (int i) const {									\
			if (i < 0)															\
				return m_str[0];												\
			if (i >= (s32)size())												\
				return m_str[(s32)size() - 1];									\
			return m_str[i];													\
		}																		\
		uchar32		m_str[len];													\
		uchar32 *	m_end;														\
	}



	// Inline functions
	#include "xbase/private/x_chars.h"

	XUCHARS(xuchars16, 16);
	XUCHARS(xuchars32, 32);
	XUCHARS(xuchars64, 64);
	XUCHARS(xuchars128, 128);
	XUCHARS(xuchars256, 256);
	XUCHARS(xuchars512, 512);
	XUCHARS(xuchars1024, 1024);

	XUCHAR32S(xuchar32s16, 16);
	XUCHAR32S(xuchar32s32, 32);
	XUCHAR32S(xuchar32s64, 64);
	XUCHAR32S(xuchar32s128, 128);
	XUCHAR32S(xuchar32s256, 256);
	XUCHAR32S(xuchar32s512, 512);
	XUCHAR32S(xuchar32s1024, 1024);

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	inline				xuchars::xuchars(uchar* str)
		: m_str(str)
		, m_end(str)
	{
		while (*m_end != '\0')
			++m_end;
	}
	inline				xuchars::xuchars(uchar* str, uchar* end)
		: m_str(str)
		, m_end(end)
	{
	}

	inline				xuchars::xuchars(uchar* str, uchar* end, const xcuchars& other)
		: m_str(str)
		, m_end(end)
	{
		copy(other);
	}

	inline u32			xuchars::size() const
	{
		return (u32)(u64)(m_end - m_str);
	}

	inline void			xuchars::reset()
	{
		m_str[0] = '\0';
	}

	inline void			xuchars::clear()
	{
		m_str[0] = '\0';
		uchar* str = m_str;
		while (str < m_end)
			*str++ = '\0';
	}

	inline xcuchars		xuchars::cchars() const
	{
		return xcuchars(m_str, m_end);
	}

	inline s32			xuchars::copy(const xuchars& _other)
	{
		xcuchars other(_other.m_str, _other.m_end);
		return copy(other);
	}

	inline s32			xuchars::copy(const xcuchars& other)
	{
		uchar const* src = other.m_str;
		uchar * dst = m_str;

		while (dst < m_end && src <= other.m_end)
		{
			*dst++ = *src++;
		}

		return (u32)(u64)(m_end - m_str);
	}

	inline s32			xuchars::compare(const xuchars& _other) const
	{
		xcuchars other(_other.m_str, _other.m_end);
		return compare(other);
	}

	inline s32			xuchars::compare(const xcuchars& other) const
	{
		if (size() < other.size())
			return -1;
		if (size() > other.size())
			return 1;
		for (u32 i = 0; i < size(); ++i)
		{
			if (m_str[i] < other.m_str[i])
				return -1;
			else if (m_str[i] > other.m_str[i])
				return 1;
		}
		return 0;
	}

	inline xuchars&	xuchars::operator = (const xuchars& other)
	{
		copy(other);
		return *this;
	}

	inline xuchars&	xuchars::operator = (const xcuchars& other)
	{
		copy(other);
		return *this;
	}

	inline bool		xuchars::operator == (const xuchars& other) const
	{
		return compare(other) == 0;
	}

	inline bool		xuchars::operator != (const xuchars& other) const
	{
		return compare(other) != 0;
	}

	inline bool		xuchars::operator == (const xcuchars& other) const
	{
		return compare(other) == 0;
	}

	inline bool		xuchars::operator != (const xcuchars& other) const
	{
		return compare(other) != 0;
	}

	inline uchar&		xuchars::operator [] (int i)
	{
		if (i >= 0 && i < (s32)size())
			return m_str[i];
		else if (i < 0)
			return m_str[0];
		return m_str[(s32)size() - 1];
	}

	inline uchar		xuchars::operator [] (int i) const
	{
		if (i >= 0 && i < (s32)size())
			return m_str[i];
		else if (i < 0)
			return m_str[0];
		return m_str[(s32)size() - 1];
	}

	inline s32			xcuchars::compare(const char* _other) const
	{
		xcuchars other(_other);
		return compare(other);
	}

	inline s32			xcuchars::compare(const xuchars& _other) const
	{
		xcuchars other(_other.m_str, _other.m_end);
		return compare(other);
	}

	inline s32			xcuchars::compare(const xcuchars& other) const
	{
		if (size() < other.size())
			return -1;
		if (size() > other.size())
			return 1;
		for (u32 i = 0; i < size(); ++i)
		{
			if (m_str[i] < other.m_str[i])
				return -1;
			else if (m_str[i] > other.m_str[i])
				return 1;
		}
		return 0;
	}

	inline bool		xcuchars::operator == (const xuchars& other) const
	{
		return compare(other) == 0;
	}

	inline bool		xcuchars::operator != (const xuchars& other) const
	{
		return compare(other) != 0;
	}

	inline bool		xcuchars::operator == (const xcuchars& other) const
	{
		return compare(other) == 0;
	}

	inline bool		xcuchars::operator != (const xcuchars& other) const
	{
		return compare(other) != 0;
	}

	inline uchar		xcuchars::operator [] (int i) const
	{
		if (i >= 0 && i < (int)size())
			return m_str[i];
		return '\0';
	}


}

#endif	///< __XCORE_CHARS_H__