#ifndef __XCORE_CHARS_H__
#define __XCORE_CHARS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_debug.h"
#include "xbase/x_buffer.h"

namespace xcore
{
	template<typename T>
	class xcuchars_t;

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// a mutable range of []uchar (ASCII)
	template<typename T>
	class xuchars_t
	{
	public:
		xuchars_t();
		xuchars_t(T* str);
		xuchars_t(T* str, T* end);
		xuchars_t(T* str, T* end, T* eos);
		xuchars_t(T* str, T* end, const xuchars_t<T>& other);
		xuchars_t(const xuchars_t<T>& other);

		u32			size() const;
		u32			cap() const;

		bool		is_empty() const;
		bool		is_full() const;

		void		reset();
		void		clear();
		void		clone(const xuchars_t<T>& other);

		xcuchars_t<T> cchars() const;

		void		copy(const xuchars_t<T>& other);
		void		copy(const xcuchars_t<T>& other);

		s32			compare(const T* other) const;
		s32			compare(const xuchars_t<T>& other) const;
		s32			compare(const xcuchars_t<T>& other) const;

		xuchars_t<T>&	operator = (const T* other);
		xuchars_t<T>&	operator = (const xuchars_t<T>& other);
		xuchars_t<T>&	operator = (const xcuchars_t<T>& other);

		xuchars_t<T>&	operator += (uchar32 other);
		xuchars_t<T>&	operator += (const char* other);
		xuchars_t<T>&	operator += (const xuchars_t<T>& other);
		xuchars_t<T>&	operator += (const xcuchars_t<T>& other);

		T			operator[] (s32 index) const;

		bool		operator == (const xuchars_t<T>& other) const;
		bool		operator != (const xuchars_t<T>& other) const;
		bool		operator <  (const xuchars_t<T>& other) const;
		bool		operator <= (const xuchars_t<T>& other) const;
		bool		operator >  (const xuchars_t<T>& other) const;
		bool		operator >= (const xuchars_t<T>& other) const;

		bool		operator == (const xcuchars_t<T>& other) const;
		bool		operator != (const xcuchars_t<T>& other) const;
		bool		operator <  (const xcuchars_t<T>& other) const;
		bool		operator <= (const xcuchars_t<T>& other) const;
		bool		operator >  (const xcuchars_t<T>& other) const;
		bool		operator >= (const xcuchars_t<T>& other) const;

		T*			m_str;
		T*			m_end;
		T*			m_eos;
	};

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// a read-only range of []uchar (ASCII)
	template<typename T>
	class xcuchars_t
	{
	public:
		inline		xcuchars_t();
		inline		xcuchars_t(T const* str);
		inline		xcuchars_t(T const* str, T const* end);

		u32			size() const;
		bool		is_empty() const;

		T const*	str() const		{ return m_str; }
		T const*	end() const		{ return m_end; }

		s32			compare(const T* other) const;
		s32			compare(const xuchars_t<T>& other) const;
		s32			compare(const xcuchars_t<T>& other) const;

		T			operator[] (s32 index) const;

		xcuchars_t<T>&	operator = (const T* other);
		xcuchars_t<T>&	operator = (const xuchars_t<T>& other);
		xcuchars_t<T>&	operator = (const xcuchars_t<T>& other);

		bool		operator == (const xuchars_t<T>& other) const;
		bool		operator != (const xuchars_t<T>& other) const;
		bool		operator <  (const xuchars_t<T>& other) const;
		bool		operator <= (const xuchars_t<T>& other) const;
		bool		operator >  (const xuchars_t<T>& other) const;
		bool		operator >= (const xuchars_t<T>& other) const;

		bool		operator == (const xcuchars_t<T>& other) const;
		bool		operator != (const xcuchars_t<T>& other) const;
		bool		operator <  (const xcuchars_t<T>& other) const;
		bool		operator <= (const xcuchars_t<T>& other) const;
		bool		operator >  (const xcuchars_t<T>& other) const;
		bool		operator >= (const xcuchars_t<T>& other) const;

		T const*	m_str;
		T const*	m_end;
	};

	typedef		xuchars_t<char>			xuchars;
	typedef		xcuchars_t<char>		xcuchars;
	typedef		xuchars_t<uchar8>		xuchar8s;
	typedef		xcuchars_t<uchar8>		xcuchar8s;
	typedef		xuchars_t<uchar32>		xuchar32s;
	typedef		xcuchars_t<uchar32>		xcuchar32s;


	// Inline functions
	#include "xbase/private/x_chars.h"

	
	template<s32 L>
	class xucharz : public xuchars_t<T>
	{
	public:
		enum { SIZE = L+1 };
		char			m_str[SIZE];
		inline			xucharz() : xuchars_t<T>(m_str, m_str, &m_str[SIZE - 1]) {}
	};

	template<s32 L>
	class xuchar8z : public xuchar8s_t<T>
	{
	public:
		enum { SIZE = L+1 };
		uchar8			m_str[SIZE];
		inline			xuchar8z() : xuchar8s_t<T>(m_str, m_str, &m_str[SIZE - 1]) {}
	};

	template<s32 L>
	class xuchar32z : public xuchar32s_t<T>
	{
	public:
		enum { SIZE = L+1 };
		uchar32			m_str[SIZE];
		inline			xuchar32z() : xuchar32s_t<T>(m_str, m_str, &m_str[SIZE - 1]) {}
	};

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	template<typename T>
	inline				xuchars_t<T>::xuchars_t()
		: m_str(NULL)
		, m_end(NULL)
		, m_eos(NULL)
	{
	}
	template<typename T>
	inline				xuchars_t<T>::xuchars_t(T* str)
		: m_str(str)
		, m_end(str)
		, m_eos(str)
	{
		while (*m_end != '\0')
			++m_end;
		m_eos = m_end;
	}
	template<typename T>
	inline				xuchars_t<T>::xuchars_t(T* str, T* end)
		: m_str(str)
		, m_end(end)
		, m_eos(end)
	{
	}
	template<typename T>
	inline				xuchars_t<T>::xuchars_t(T* str, T* end, T* eos)
		: m_str(str)
		, m_end(end)
		, m_eos(eos)
	{
	}

	template<typename T>
	inline				xuchars_t<T>::xuchars_t(T* str, T* eos, const xuchars_t<T>& other)
		: m_str(str)
		, m_end(str)
		, m_eos(eos)
	{
		copy(other);
	}

	template<typename T>
	inline	xuchars_t<T>::xuchars_t(const xuchars_t<T>& other)
		: m_str(other.m_str)
		, m_end(other.m_end)
		, m_eos(other.m_eos)
	{
	}

	template<typename T>
	inline u32			xuchars_t<T>::size() const
	{
		return (u32)(u64)(m_end - m_str);
	}

	template<typename T>
	inline u32			xuchars_t<T>::cap() const
	{
		return (u32)(u64)(m_eos - m_str);
	}

	template<typename T>
	inline bool			xuchars_t<T>::is_empty() const
	{
		return m_str == m_end;
	}

	template<typename T>
	inline bool			xuchars_t<T>::is_full() const
	{
		return m_end == m_eos;
	}

	template<typename T>
	inline void			xuchars_t<T>::reset()
	{
		m_str[0] = '\0';
	}

	template<typename T>
	inline void			xuchars_t<T>::clear()
	{
		m_str[0] = '\0';
		T* str = m_str;
		while (str < m_end)
			*str++ = '\0';
	}

	template<typename T>
	inline void			xuchars_t<T>::clone(const xuchars_t<T>& other)
	{
		m_str = other.m_str;
		m_end = other.m_end;
		m_eos = other.m_eos;
	}

	template<typename T>
	inline xbuffer		xuchars_t<T>::buffer() const
	{
		return xbuffer((xbyte*)m_end - (xbyte*)m_str, (xbyte*)m_end);
	}

	template<typename T>
	inline xcbuffer		xuchars_t<T>::cbuffer() const
	{
		return xcbuffer((xbyte const*)m_end - (xbyte const*)m_str, (xbyte const*)m_end);
	}

	template<typename T>
	inline xcuchars_t<T>		xuchars_t<T>::cchars() const
	{
		return xcuchars_t<T>(m_str, m_end);
	}

	template<typename T>
	inline void			xuchars_t<T>::copy(const xuchars_t<T>& _other)
	{
		xcuchars_t<T> other(_other.m_str, _other.m_end);
		copy(other);
	}

	template<typename T>
	inline void			xuchars_t<T>::copy(const xcuchars_t<T>& other)
	{
		T const* src = other.m_str;
		m_end = m_str;
		while (m_end < m_eos && src <= other.m_end)
			*m_end++ = *src++;
		if (m_end < m_eos)
			*m_end = '\0';
	}

	template<typename T>
	inline s32			xuchars_t<T>::compare(const T* _other) const
	{
		xcuchars_t<T> self(m_str, m_end);
		xcuchars_t<T> other(_other);
		return self.compare(other);
	}

	template<typename T>
	inline s32			xuchars_t<T>::compare(const xuchars_t<T>& _other) const
	{
		xcuchars_t<T> self(m_str, m_end);
		xcuchars_t<T> other(_other.m_str, _other.m_end);
		return self.compare(other);
	}

	template<typename T>
	inline s32			xuchars_t<T>::compare(const xcuchars_t<T>& _other) const
	{
		xcuchars_t<T> self(m_str, m_end);
		xcuchars_t<T> other(_other.m_str, _other.m_end);
		return self.compare(other);
	}

	template<typename T>
	inline T					xuchars_t<T>::operator[] (s32 index) const
	{
		T const* ptr = &m_str[index];
		if (ptr < m_end)
		{
			return *ptr;
		}
		return '\0';
	}

	template<typename T>
	inline xuchars_t<T>&		xuchars_t<T>::operator += (uchar32 other)
	{
		if (m_end < m_eos)
		{
			*m_end++ = other;
			*m_end = '\0';
		}
		return *this;
	}

	template<typename T>
	inline xuchars_t<T>&		xuchars_t<T>::operator += (const char* other)
	{
		while (m_end < m_eos && *other != '\0')
		{
			*m_end++ = *other++;
			*m_end = '\0';
		}
		if (m_end < m_eos)
		{
			*m_end = '\0';
		}
		return *this;
	}

	template<typename T>
	inline xuchars_t<T>&		xuchars_t<T>::operator += (const xuchars_t<T>& other)
	{
		xuchars_t<T> str(other);
		while (!str.is_empty() && !is_full())
		{
			*m_end++ = *str.m_str++;
		}
		return *this;
	}

	template<typename T>
	inline xuchars_t<T>&		xuchars_t<T>::operator += (const xcuchars_t<T>& other)
	{
		xcuchars_t<T> str(other);
		while (!str.is_empty() && !is_full())
		{
			*m_end++ = *str.m_str++;
		}
		return *this;
	}

	template<typename T>
	inline xuchars_t<T>&	xuchars_t<T>::operator = (const T* _other)
	{
		xcuchars_t<T> other(_other);
		copy(other);
		return *this;
	}

	template<typename T>
	inline xuchars_t<T>&	xuchars_t<T>::operator = (const xuchars_t<T>& other)
	{
		copy(other);
		return *this;
	}

	template<typename T>
	inline xuchars_t<T>&	xuchars_t<T>::operator = (const xcuchars_t<T>& other)
	{
		copy(other);
		return *this;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator == (const xuchars_t<T>& other) const
	{
		return compare(other) == 0;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator != (const xuchars_t<T>& other) const
	{
		return compare(other) != 0;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator < (const xuchars_t<T>& other) const
	{
		return compare(other) == -1;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator <= (const xuchars_t<T>& other) const
	{
		return compare(other) <= 0;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator > (const xuchars_t<T>& other) const
	{
		return compare(other) == 1;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator >= (const xuchars_t<T>& other) const
	{
		return compare(other) >= 0;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator == (const xcuchars_t<T>& other) const
	{
		return compare(other) == 0;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator != (const xcuchars_t<T>& other) const
	{
		return compare(other) != 0;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator < (const xcuchars_t<T>& other) const
	{
		return compare(other) == -1;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator <= (const xcuchars_t<T>& other) const
	{
		return compare(other) <= 0;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator > (const xcuchars_t<T>& other) const
	{
		return compare(other) == 1;
	}

	template<typename T>
	inline bool		xuchars_t<T>::operator >= (const xcuchars_t<T>& other) const
	{
		return compare(other) >= 0;
	}

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	template<typename T>
	inline			xcuchars_t<T>::xcuchars_t()
	{
		m_str = NULL;
		m_end = NULL;
	}
	template<typename T>
	inline			xcuchars_t<T>::xcuchars_t(T const* str)
		: m_str(str)
		, m_end(str)
	{
		while (*m_end != '\0')
			++m_end;
	}
	template<typename T>
	inline			xcuchars_t<T>::xcuchars_t(T const* str, T const* end)
		: m_str(str)
		, m_end(end)
	{
	}

	template<typename T>
	inline u32		xcuchars_t<T>::size() const
	{
		return (u32)(u64)(m_end - m_str);
	}

	template<typename T>
	inline bool		xcuchars_t<T>::is_empty() const
	{
		return m_str == m_end;
	}

	template<typename T>
	inline xcbuffer		xcuchars_t<T>::buffer() const
	{
		return xcbuffer((xbyte const*)m_end - (xbyte const*)m_str, (xbyte const*)m_end);
	}

	template<typename T>
	inline s32		xcuchars_t<T>::compare(const T* _other) const
	{
		xcuchars_t<T> other(_other);
		return compare(other);
	}

	template<typename T>
	inline s32		xcuchars_t<T>::compare(const xuchars_t<T>& _other) const
	{
		xcuchars_t<T> other(_other.m_str, _other.m_end);
		return compare(other);
	}

	template<typename T>
	inline s32		xcuchars_t<T>::compare(const xcuchars_t<T>& other) const
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

	template<typename T>
	inline T					xcuchars_t<T>::operator[] (s32 index) const
	{
		T const* ptr = &m_str[index];
		if (ptr < m_end)
		{
			return *ptr;
		}
		return '\0';
	}

	template<typename T>
	inline xcuchars_t<T>& xcuchars_t<T>::operator = (const T* str)
	{
		m_str = str;
		m_end = str;
		while (*m_end != '\0')
			++m_end;
		return *this;
	}

	template<typename T>
	inline xcuchars_t<T>& xcuchars_t<T>::operator = (const xuchars_t<T>& str)
	{
		m_str = str.m_str;
		m_end = str.m_end;
		return *this;
	}

	template<typename T>
	inline xcuchars_t<T>& xcuchars_t<T>::operator = (const xcuchars_t<T>& str)
	{
		m_str = str.m_str;
		m_end = str.m_end;
		return *this;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator == (const xuchars_t<T>& other) const
	{
		return compare(other) == 0;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator != (const xuchars_t<T>& other) const
	{
		return compare(other) != 0;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator <  (const xuchars_t<T>& other) const
	{
		return compare(other) == -1;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator <= (const xuchars_t<T>& other) const
	{
		return compare(other) <= 0;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator >  (const xuchars_t<T>& other) const
	{
		return compare(other) == 1;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator >= (const xuchars_t<T>& other) const
	{
		return compare(other) >= 0;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator == (const xcuchars_t<T>& other) const
	{
		return compare(other) == 0;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator != (const xcuchars_t<T>& other) const
	{
		return compare(other) != 0;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator <  (const xcuchars_t<T>& other) const
	{
		return compare(other) == -1;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator <= (const xcuchars_t<T>& other) const
	{
		return compare(other) <= 0;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator >  (const xcuchars_t<T>& other) const
	{
		return compare(other) == 1;
	}

	template<typename T>
	inline bool		xcuchars_t<T>::operator >= (const xcuchars_t<T>& other) const
	{
		return compare(other) >= 0;
	}

}

#endif	///< __XCORE_CHARS_H__
