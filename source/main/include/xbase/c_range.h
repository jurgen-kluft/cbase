#ifndef __CBASE_RANGE_H__
#define __CBASE_RANGE_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace ncore
{
	template<typename T>
	class range_iter_t;

	template<typename T>
	struct from_t
	{
		from_t(T from) : m_from(from) {}
		T m_from;
	};
	template<typename T>
	struct until_t
	{
		until_t(T until) : m_until(until) {}
		T m_until;
	};
	template<typename T>
	struct step_t
	{
		step_t(T s) : m_step(s) {}
		T m_step;
	};

	struct count_t
	{
		count_t(s32 c) : m_count(c) {}
		s32 m_count;
	};

	template<typename T>
	inline from_t<T>	from(T value) { return from_t<T>(value); }
	template<typename T>
	inline until_t<T>	until(T value) { return until_t<T>(value); }
	template<typename T>
	inline step_t<T>	step(T value) { return step_t<T>(value); }

	inline count_t		count(s32 value) { return value; }


	template<typename T>
	class range_t
	{
	public:
		inline		range_t();
		inline		range_t(until_t<T> _until);
		inline		range_t(from_t<T> _from, until_t<T> _until);
		inline		range_t(from_t<T> _from, count_t _count);
		inline		range_t(range_t<T> const& range);

		s32			index() const { return m_index; }
		T			item() const { return m_current; }

		T &			operator * (void) { return m_current; }
		T const&	operator * (void) const { return m_current; }

		bool		operator == (range_t<T> const& range) const { return m_current == range.m_current; }
		bool		operator != (range_t<T> const& range) const { return m_current != range.m_current; }

		range_t<T>&	operator ++ () { m_index += 1; m_current += m_step; return *this; }

		void		reset()
		{
			m_index = 0;
			m_current = m_from;
		}

		range_t<T>	begin() const 
		{
			range_t<T> range(*this);
			range.reset();
			return range;
		}

		range_t<T>	end() const 
		{
			range_t<T> e(*this);
			e.m_current = m_to;
			return e;
		}

		inline bool	iterate()
		{
			m_index += 1;
			m_current += m_step;
			return (m_current >= m_to);
		}

		T			m_from;
		T			m_to;
		s32			m_index;
		T			m_current;
		T			m_step;
	};

	template<typename T>
	inline range_t<T>::range_t()
		: m_index(0)
		, m_step(0)
	{
		m_from = 0;
		m_to = 0;
		m_current = 0;
	}

	template<typename T>
	inline range_t<T>::range_t(until_t<T> _until)
		: m_index(0)
		, m_step(1)
	{
		m_from = 0;
		m_to = _until.m_until;
		m_current = m_from;
	}

	template<typename T>
	inline range_t<T>::range_t(from_t<T> _from, until_t<T> _until)
		: m_index(0)
		, m_step(1)
	{
		m_from = _from.m_from;
		m_to = _until.m_until;
		m_current = m_from;
	}

	template<typename T>
	inline range_t<T>::range_t(from_t<T> _from, count_t _count)
		: m_index(0)
		, m_step(1)
	{
		m_from = _from.m_from;
		m_to = m_from + _count.m_count;
		m_current = m_from;
	}

	template<typename T>
	inline range_t<T>::range_t(range_t<T> const& range)
		: m_from(range.m_from)
		, m_to(range.m_to)
		, m_index(range.m_index)
		, m_current(range.m_current)
		, m_step(range.m_step)
	{
	}

	template<typename T>
	inline 	range_t<T>	forwards(range_t<T> const& iter)
	{
		range_t<T> range(iter);
		if (range.m_step < 0)
		{
			range.m_step = -range.m_step;
			range.m_from = iter.m_to;
			range.m_to = iter.m_from;
		}
		range.reset();
		return range;
	}

	template<typename T>
	inline range_t<T>	backwards(range_t<T> const& iter)
	{
		range_t<T> range(iter);
		if (range.m_step > 0)
		{
			range.m_step = -range.m_step;
			range.m_from = iter.m_to;
			range.m_to = iter.m_from;
		}
		range.reset();
		return range;
	}
};

#endif
