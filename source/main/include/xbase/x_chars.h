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
	class ascii_chars
	{
	public:
		typedef			char		rune;
		bool			peek(rune * str, const rune* end, uchar32& c) const;
		bool			cpeek(rune const* str, const rune* end, uchar32& c) const;
		bool			read(rune *& str, const rune* end, uchar32& c) const;
		bool			cread(rune const *& str, const rune* end, uchar32& c) const;
		bool			write(rune* str, const rune* end, uchar32 c) const;
	};
	class utf8_chars
	{
	public:
		typedef			uchar8		rune;
		bool			peek(rune * str, const rune* end, uchar32& c) const;
		bool			cpeek(rune const* str, const rune* end, uchar32& c) const;
		bool			read(rune *& str, const rune* end, uchar32& c) const;
		bool			cread(rune const *& str, const rune* end, uchar32& c) const;
		bool			write(rune* str, const rune* end, uchar32 c) const;
	};
	class utf32_chars
	{
	public:
		typedef			uchar32		rune;
		bool			peek(rune * str, const rune* end, uchar32& c) const;
		bool			cpeek(rune const* str, const rune* end, uchar32& c) const;
		bool			read(rune *& str, const rune* end, uchar32& c) const;
		bool			cread(rune const *& str, const rune* end, uchar32& c) const;
		bool			write(rune* str, const rune* end, uchar32 c) const;
	};


	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	template<typename T, typename R>
	class xuchars_const_iterator_t
	{
	public:
		typedef			xuchars_const_iterator_t<T, R>		self;

		inline			xuchars_const_iterator_t(T const* str, T const* end) : m_str(str), m_end(end) {}

		T				operator * () { R r; uchar32 c; T const* s = m_str; r.cread(s, m_end, c); return (T)c; }

		self&			operator ++ () { R r; uchar32 c; r.read(m_str, m_end, c); return *this; }
		self			operator ++ (s32) { self iter(m_str, m_end); R r; uchar32 c; r.cread(m_str, m_end, c); return iter; }

		inline bool		operator < (const self& other) const { return m_str < m_end; }

	protected:
		T const*		m_str;
		T const*		m_end;
	};


	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// a read-only range of []uchar (ASCII)
	template<typename T, typename R>
	class xcuchars_t
	{
	public:
		inline			xcuchars_t();
		inline			xcuchars_t(T const* str);
		inline			xcuchars_t(T const* str, T const* end);

		typedef			xcuchars_t<T, R>					const_self;
		typedef			xuchars_const_iterator_t<T, R>		const_iterator;

		u32				size() const;
		bool			is_empty() const;

		T const*		str() const;

		const_iterator	begin() const;
		const_iterator	end() const;

		s32				compare(const T* other) const;
		s32				compare(const const_self& other) const;

		bool			operator == (const const_self& other) const;
		bool			operator != (const const_self& other) const;
		bool			operator <  (const const_self& other) const;
		bool			operator <= (const const_self& other) const;
		bool			operator >  (const const_self& other) const;
		bool			operator >= (const const_self& other) const;

		const_self		operator() (u32 from, u32 to) const;

		T const*		m_const_str;
		T const*		m_const_end;
	};


	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// a mutable range of []uchar (ASCII)
	template<typename T, typename R>
	class xuchars_iterator_t
	{
	public:
		typedef			xuchars_iterator_t<T, R>		self;
		typedef			xcuchars_t<T, R>				base;

		inline			xuchars_iterator_t(T* str, T const* end) : m_str(str), m_end(end) {}

		T				operator * ()					{ R r; uchar32 c; T* s = m_str; r.read(s, m_end, c); return (T)c; }

		self&			operator = (T c)				{ R r; T* s = m_str; r.write(s, m_end, (uchar32)c); return *this; }

		self&			operator ++ ()					{ R r; uchar32 c; r.read(m_str, m_end, c); return *this; }
		self			operator ++ (s32)				{ self iter(m_str, m_end); R r; uchar32 c; r.read(m_str, m_end, c); return iter; }

		inline bool		operator < (const self& other) const { return m_str < m_end; }

	protected:
		T *				m_str;
		T const*		m_end;
	};

	template<typename T, typename R>
	class xuchars_t : public xcuchars_t<T, R>
	{
	public:
		xuchars_t();
		xuchars_t(T* str);
		xuchars_t(T* str, T const* end);
		xuchars_t(T* str, T const* end, T const* eos);
		xuchars_t(const xuchars_t<T, R>& other);

		typedef			xuchars_t<T, R>					self;
		typedef			xcuchars_t<T, R>				base;

		void			reset();
		void			clear();

		bool			can_write() const;
		void			copy(const xcuchars_t<T, R>& other);
		void			clone(const xuchars_t<T, R>& other);

		xuchars_iterator_t<T, R>	begin();
		xuchars_iterator_t<T, R>	end();

		xuchars_const_iterator_t<T, R>	begin() const;
		xuchars_const_iterator_t<T, R>	end() const;

		self&			operator = (const T* other);
		self&			operator = (const base& other);

		self&			operator += (uchar32 other);
		self&			operator += (const base& other);

		bool			operator == (const base& other) const;
		bool			operator != (const base& other) const;
		bool			operator <  (const base& other) const;
		bool			operator <= (const base& other) const;
		bool			operator >  (const base& other) const;
		bool			operator >= (const base& other) const;

		self			operator() (u32 from, u32 to) const;

		T *				m_end;
		T const *		m_eos;
	};

	typedef		xuchars_t<char, ascii_chars>					xuchars;
	typedef		xcuchars_t<char, ascii_chars>					xcuchars;
	typedef		xuchars_iterator_t<char, ascii_chars>			xuchars_iterator;
	typedef		xuchars_const_iterator_t<char, ascii_chars>		xuchars_const_iterator;

	typedef		xuchars_t<uchar8, utf8_chars>					xuchar8s;
	typedef		xcuchars_t<uchar8, utf8_chars>					xcuchar8s;
	typedef		xuchars_iterator_t<uchar8, utf8_chars>			xuchar8s_iterator;
	typedef		xuchars_const_iterator_t<uchar8, utf8_chars>	xuchar8s_const_iterator;

	typedef		xuchars_t<uchar32, utf32_chars>					xuchar32s;
	typedef		xcuchars_t<uchar32, utf32_chars>				xcuchar32s;
	typedef		xuchars_iterator_t<uchar32, utf32_chars>		xuchar32s_iterator;
	typedef		xuchars_const_iterator_t<uchar32, utf32_chars>	xuchar32s_const_iterator;


	// Inline functions
	#include "xbase/private/x_chars.h"

	template<s32 L>
	class xucharz : public xuchars
	{
	public:
		enum { SIZE = L + 1 };
		char			m_str[SIZE];

		inline			xucharz() : xuchars(m_str, m_str, &m_str[SIZE - 1]) {}

		xucharz<L>&		operator = (const char* other) { this->xuchars::operator=(other); return *this; }
		xucharz<L>&		operator = (const xcuchars& other) { this->xuchars::operator=(other); return *this; }

	};

	template<s32 L>
	class xuchar8z : public xuchar8s
	{
	public:
		enum { SIZE = L + 1 };
		uchar8			m_str[SIZE];
		inline			xuchar8z() : xuchar8s(m_str, m_str, &m_str[SIZE - 1]) {}
	};

	template<s32 L>
	class xuchar32z : public xuchar32s
	{
	public:
		enum { SIZE = L + 1 };
		uchar32			m_str[SIZE];
		inline			xuchar32z() : xuchar32s(m_str, m_str, &m_str[SIZE - 1]) {}
	};



	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	template<typename T, typename R>
	inline			xcuchars_t<T, R>::xcuchars_t()
	{
		static T s_empty_chars[] = { 0,0,0,0 };
		m_const_str = s_empty_chars;
		m_const_end = m_const_str;
	}
	template<typename T, typename R>
	inline			xcuchars_t<T, R>::xcuchars_t(T const* str)
		: m_const_str(str)
		, m_const_end(str)
	{
		while (*m_const_end != '\0')
			++m_const_end;
	}
	template<typename T, typename R>
	inline			xcuchars_t<T, R>::xcuchars_t(T const* str, T const* end)
		: m_const_str(str)
		, m_const_end(end)
	{
	}

	template<typename T, typename R>
	inline u32			xcuchars_t<T, R>::size() const
	{
		return (u32)(u64)(m_const_end - m_const_str);
	}

	template<typename T, typename R>
	inline bool			xcuchars_t<T, R>::is_empty() const
	{
		return m_const_str == m_const_end;
	}

	template<typename T, typename R>
	T const*		xcuchars_t<T, R>::str() const
	{
		return m_const_str;
	}

	template<typename T, typename R>
	inline xuchars_const_iterator_t<T,R>	xcuchars_t<T, R>::begin() const
	{
		return xuchars_const_iterator_t<T, R>((T*)m_const_str, m_const_end);
	}

	template<typename T, typename R>
	inline xuchars_const_iterator_t<T, R>	xcuchars_t<T, R>::end() const
	{
		return xuchars_const_iterator_t<T, R>((T*)m_const_end, m_const_end);
	}

	template<typename T, typename R>
	inline s32		xcuchars_t<T, R>::compare(const T* _other) const
	{
		xcuchars_t<T, R> other(_other);
		return compare(other);
	}

	template<typename T, typename R>
	inline s32		xcuchars_t<T, R>::compare(const xcuchars_t<T, R>& other) const
	{
		if (size() < other.size())
			return -1;
		if (size() > other.size())
			return 1;
		for (u32 i = 0; i < size(); ++i)
		{
			if (m_const_str[i] < other.m_const_str[i])
				return -1;
			else if (m_const_str[i] > other.m_const_str[i])
				return 1;
		}
		return 0;
	}


	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	template<typename T, typename R>
	inline				xuchars_t<T, R>::xuchars_t()
		: xcuchars_t<T, R>()
		, m_end(NULL)
		, m_eos(NULL)
	{
	}
	template<typename T, typename R>
	inline				xuchars_t<T, R>::xuchars_t(T* str)
		: xcuchars_t<T, R>(str, str)
		, m_end(str)
		, m_eos(str)
	{
		while (*m_end != '\0')
			++m_end;
		m_eos = m_end;
		base::m_const_end = m_end;
	}
	template<typename T, typename R>
	inline				xuchars_t<T, R>::xuchars_t(T* str, T const* end)
		: xcuchars_t<T, R>(str, end)
		, m_end((T*)end)
		, m_eos(end)
	{
	}
	template<typename T, typename R>
	inline				xuchars_t<T, R>::xuchars_t(T* str, T const* end, T const* eos)
		: xcuchars_t<T, R>(str, end)
		, m_end((T*)end)
		, m_eos(eos)
	{
	}

	template<typename T, typename R>
	inline	xuchars_t<T, R>::xuchars_t(const xuchars_t<T, R>& other)
		: xcuchars_t<T, R>(other.m_const_str, other.m_const_end)
		, m_end(other.m_end)
		, m_eos(other.m_eos)
	{
	}

	template<typename T, typename R>
	inline void			xuchars_t<T, R>::reset()
	{
		m_end = (T*)base::m_const_str;
		m_end[0] = '\0';
		base::m_const_end = m_end;
	}

	template<typename T, typename R>
	inline void			xuchars_t<T, R>::clear()
	{
		reset();
		T* iter = m_end;
		while (iter < m_eos)
			*iter++ = '\0';
	}

	template<typename T, typename R>
	inline bool			xuchars_t<T, R>::can_write() const
	{
		return m_end < m_eos;
	}

	template<typename T, typename R>
	inline void			xuchars_t<T, R>::copy(const xcuchars_t<T, R>& other)
	{
		reset();
		T const* src = other.m_const_str;
		while (m_end < m_eos && src <= other.m_const_end)
			*m_end++ = *src++;
		if (m_end < m_eos)
			*m_end = '\0';
		base::m_const_end = m_end;
	}

	template<typename T, typename R>
	inline void			xuchars_t<T, R>::clone(const xuchars_t<T, R>& other)
	{
		base::m_const_str = other.m_const_str;
		base::m_const_end = other.m_const_end;
		m_end = other.m_end;
		m_eos = other.m_eos;
	}


	template<typename T, typename R>
	inline xuchars_iterator_t<T, R>	xuchars_t<T, R>::begin()
	{
		return xuchars_iterator_t<T, R>((T*)base::m_const_str, base::m_const_end);
	}

	template<typename T, typename R>
	inline xuchars_iterator_t<T, R>	xuchars_t<T, R>::end()
	{
		return xuchars_iterator_t<T, R>((T*)base::m_const_end, base::m_const_end);
	}

	template<typename T, typename R>
	inline xuchars_const_iterator_t<T, R>	xuchars_t<T, R>::begin() const
	{
		return xuchars_const_iterator_t<T, R>((T*)base::m_const_str, base::m_const_end);
	}

	template<typename T, typename R>
	inline xuchars_const_iterator_t<T, R>	xuchars_t<T, R>::end() const
	{
		return xuchars_const_iterator_t<T, R>((T*)base::m_const_end, base::m_const_end);
	}

	template<typename T, typename R>
	inline xuchars_t<T, R>&		xuchars_t<T, R>::operator += (uchar32 other)
	{
		if (m_end < m_eos)
		{
			*m_end++ = other;
			*m_end = '\0';
		}
		base::m_const_end = m_end;
		return *this;
	}

	template<typename T, typename R>
	inline xuchars_t<T, R>&		xuchars_t<T, R>::operator += (const xcuchars_t<T, R>& other)
	{
		const T* str = other.m_const_str;
		const T* end = other.m_const_end;
		while (str < end && m_end < m_eos)
		{
			*m_end++ = *str++;
		}
		if (m_end < m_eos)
		{
			*m_end = '\0';
		}
		return *this;
	}


	template<typename T, typename R>
	inline xuchars_t<T, R>&	xuchars_t<T, R>::operator = (const T* _other)
	{
		xcuchars_t<T, R> other(_other);
		copy(other);
		return *this;
	}

	template<typename T, typename R>
	inline xuchars_t<T, R>&	xuchars_t<T, R>::operator = (const xcuchars_t<T, R>& other)
	{
		copy(other);
		return *this;
	}

	template<typename T, typename R>
	inline bool		xuchars_t<T, R>::operator == (const xcuchars_t<T, R>& other) const
	{
		return compare(other) == 0;
	}

	template<typename T, typename R>
	inline bool		xuchars_t<T, R>::operator != (const xcuchars_t<T, R>& other) const
	{
		return compare(other) != 0;
	}

	template<typename T, typename R>
	inline bool		xuchars_t<T, R>::operator < (const xcuchars_t<T, R>& other) const
	{
		return compare(other) == -1;
	}

	template<typename T, typename R>
	inline bool		xuchars_t<T, R>::operator <= (const xcuchars_t<T, R>& other) const
	{
		return compare(other) <= 0;
	}

	template<typename T, typename R>
	inline bool		xuchars_t<T, R>::operator > (const xcuchars_t<T, R>& other) const
	{
		return compare(other) == 1;
	}

	template<typename T, typename R>
	inline bool		xuchars_t<T, R>::operator >= (const xcuchars_t<T, R>& other) const
	{
		return compare(other) >= 0;
	}

	template<typename T, typename R>
	inline xuchars_t<T, R>	xuchars_t<T, R>::operator() (u32 from, u32 to) const
	{
		T* str = m_end - (m_end - base::m_const_str);
		T const* eos = m_eos;
		if (from <= to && to <= base::size())
		{
			return xuchars_t<T, R>(str + from, str + to, m_eos);
		}
		else if (from <= base::size())
		{
			return xuchars_t<T, R>(str + from, base::m_const_end, m_eos);
		}
		else
		{
			from = base::size();
			return xuchars_t<T,R>(str + from, str + from, m_eos);
		}
	}


	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------

	template<typename T, typename R>
	inline bool		xcuchars_t<T, R>::operator == (const xcuchars_t<T, R>& other) const
	{
		return compare(other) == 0;
	}

	template<typename T, typename R>
	inline bool		xcuchars_t<T, R>::operator != (const xcuchars_t<T, R>& other) const
	{
		return compare(other) != 0;
	}

	template<typename T, typename R>
	inline bool		xcuchars_t<T, R>::operator <  (const xcuchars_t<T, R>& other) const
	{
		return compare(other) == -1;
	}

	template<typename T, typename R>
	inline bool		xcuchars_t<T, R>::operator <= (const xcuchars_t<T, R>& other) const
	{
		return compare(other) <= 0;
	}

	template<typename T, typename R>
	inline bool		xcuchars_t<T, R>::operator >  (const xcuchars_t<T, R>& other) const
	{
		return compare(other) == 1;
	}

	template<typename T, typename R>
	inline bool		xcuchars_t<T, R>::operator >= (const xcuchars_t<T, R>& other) const
	{
		return compare(other) >= 0;
	}

	template<typename T, typename R>
	inline xcuchars_t<T, R>	xcuchars_t<T, R>::operator() (u32 from, u32 to) const
	{
		if (from <= to && to <= size())
		{
			return xcuchars_t<T, R>(m_const_str + from, m_const_str + to);
		}
		else if (from <= size())
		{
			return xcuchars_t<T, R>(m_const_str + from, m_const_end);
		}
		else
		{
			from = size();
			return xcuchars_t<T, R>(m_const_end, m_const_end);
		}
	}
}

#endif	///< __XCORE_CHARS_H__
