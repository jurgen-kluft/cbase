#ifndef __XCORE_CHARS_H__
#define __XCORE_CHARS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_debug.h"

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

		s32			compare(const T* other) const;
		s32			compare(const xuchars_t<T>& other) const;
		s32			compare(const xcuchars_t<T>& other) const;

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

	
	template<typename T, s32 L>
	class xuchars_buffer_t				
	{																					
	public:
		enum { SIZE = L };
		T				m_str[SIZE];
		xuchars_t<T>	m_chars;

		typedef			xuchars_buffer_t<T, L>			self;

		inline			xuchars_buffer_t() { reset(); }
		inline			xuchars_buffer_t(self const& other) { reset(); *this = other; }
		inline			xuchars_buffer_t(xcuchars_t<T> const& other) { reset(); *this = other; }
		inline			xuchars_buffer_t(T const* other) { reset(); *this = other; }
		
		u32				size() const { return m_chars.size(); }
		
		T *				str() { return m_chars.m_str; }
		T *				end() { return m_chars.m_end; }
		T const*		end() const { return m_chars.m_end; }
		T const*		eos() const { return m_chars.m_eos; }
		
		xuchars_t<T>&	chars() { return m_chars; }
		xcuchars_t<T>	cchars() const { return xcuchars_t<T>(m_chars.m_str, m_chars.m_end); }			
		void			reset() {
			m_chars.m_str = m_str;
			m_chars.m_end = m_chars.m_str;
			*m_chars.m_end = '\0';
			m_chars.m_eos = &m_str[SIZE - 1];
			*m_chars.m_eos = '\0';
		}
		void		clear() {
			reset();
		}
		s32			compare(const xuchars_t<T>& _other) const {
			xcuchars_t<T> self(m_chars.m_str, m_chars.m_end);
			xcuchars_t<T> other(_other.m_str, _other.m_end);
			return self.compare(other);
		}
		s32			compare(const xcuchars_t<T>& _other) const {
			xcuchars_t<T> self(m_chars.m_str, m_chars.m_end);
			xcuchars_t<T> other(_other.m_str, _other.m_end);					
			return self.compare(other);											
		}																		
		s32			compare(const self& _other) const {							
			xcuchars_t<T> self(m_chars.m_str, m_chars.m_end);
			xcuchars_t<T> other(_other.m_str, _other.m_end);					
			return self.compare(other);											
		}																		
		self&		operator = (const T* other) {								
			T * dst = m_chars.m_str;
			while (dst < eos() && *other != '\0')
				*dst++ = *other++;
			m_chars.m_end = dst;
			*m_chars.m_end = '\0';
			return *this;														
		}																		
		self&		operator = (const self& other) {							
			T const* src = other.m_str;											
			T * dst = m_chars.m_str;
			while (dst < eos() && src < other.m_end)							
				*dst++ = *src++;												
			m_chars.m_end = dst;
			*m_chars.m_end = '\0';
			return *this;														
		}																		
		self&		operator = (const xcuchars_t<T>& other) {					
			T const* src = other.m_str;											
			T * dst = m_chars.m_str;
			while (dst < eos() && src < other.m_end)							
				*dst++ = *src++;												
			m_chars.m_end = dst;
			*m_chars.m_end = '\0';
			return *this;														
		}																		
		bool		operator == (const self& other) const {						
			return compare(other) == 0;											
		}																		
		bool		operator != (const self& other) const {						
			return compare(other) != 0;											
		}																		
		bool		operator > (const self& other) const {						
			return compare(other) == 1;											
		}																		
		bool		operator >= (const self& other) const {						
			return compare(other) >= 0;											
		}																		
		bool		operator < (const self& other) const {						
			return compare(other) == -1;										
		}																		
		bool		operator <= (const self& other) const {						
			return compare(other) <= 0;											
		}																		
	};																			

	#define XUCHARS_T(name, type, size) typedef	xuchars_buffer_t<type, size>		name;

	// Predefined 
	XUCHARS_T(xuchar8s8, uchar8, 8);
	XUCHARS_T(xuchar8s16, uchar8, 16);
	XUCHARS_T(xuchar8s32, uchar8, 32);
	XUCHARS_T(xuchar8s64, uchar8, 64);
	XUCHARS_T(xuchar8s128, uchar8, 128);
	XUCHARS_T(xuchar8s256, uchar8, 256);
	XUCHARS_T(xuchar8s512, uchar8, 512);
	XUCHARS_T(xuchar8s1024, uchar8, 1024);

	XUCHARS_T(xuchar32s16, uchar32, 16);
	XUCHARS_T(xuchar32s32, uchar32, 32);
	XUCHARS_T(xuchar32s64, uchar32, 64);
	XUCHARS_T(xuchar32s128, uchar32, 128);
	XUCHARS_T(xuchar32s256, uchar32, 256);
	XUCHARS_T(xuchar32s512, uchar32, 512);
	XUCHARS_T(xuchar32s1024, uchar32, 1024);

	XUCHARS_T(xuchars8, char, 8);
	XUCHARS_T(xuchars16, char, 16);
	XUCHARS_T(xuchars32, char, 32);
	XUCHARS_T(xuchars64, char, 64);
	XUCHARS_T(xuchars128, char, 128);
	XUCHARS_T(xuchars256, char, 256);
	XUCHARS_T(xuchars512, char, 512);
	XUCHARS_T(xuchars1024, char, 1024);




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
