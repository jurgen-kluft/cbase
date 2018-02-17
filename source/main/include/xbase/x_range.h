#ifndef __XBASE_RANGE_H__
#define __XBASE_RANGE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"


/**
 * xCore namespace
 */
namespace xcore
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

		bool		operator == (range_t<T> const& range) { return m_state == range.m_state; }
		bool		operator != (range_t<T> const& range) { return m_state != range.m_state; }

		range_t<T>&	operator ++ () 
		{
			iterate();
			return *this;
		}

		range_t<T>	operator ++ (s32) 
		{
			range_t<T> cr(*this);
			iterate();
			return cr;
		}


		enum EState 
		{ 
			STATE_START   = 0x001, 
			STATE_ITER    = 0x002, 
			STATE_END     = 0x000,
			STATE_MASK    = 0x00F,
			DIR_FORWARDS  = 0x000, 
			DIR_BACKWARDS = 0x100,
			DIR_MASK      = 0xF00
		};

		void		reset(s32 direction)
		{
			m_state = STATE_START | (direction & DIR_MASK);
		}

		range_t<T>	begin() const 
		{
			range_t<T> range(*this);
			range.reset(m_state);
			range.iterate();
			return range;
		}

		range_t<T>	end() const 
		{
			return range_t<T>();
		}

		bool		iterate()
		{
			switch (m_state & STATE_MASK)
			{
			case STATE_START:
				if ((m_state & DIR_MASK) == DIR_FORWARDS)
				{
					m_index = 0;
					m_current = m_from;
					m_state = STATE_ITER | DIR_FORWARDS;
				} 
				else if ((m_state & DIR_MASK) == DIR_BACKWARDS)
				{
					m_index = 0;
					m_current = m_to - m_step;
					m_state = STATE_ITER | DIR_BACKWARDS;
				}
				else 
				{
					m_state = STATE_END;
				}
				break;
			case STATE_ITER:
				if ((m_state & DIR_MASK) == DIR_FORWARDS)
				{
					m_index += 1;
					m_current += m_step;
					if (m_current >= m_to)
					{
						m_state = STATE_END;
						return false;
					}
				}
				else if ((m_state & DIR_MASK) == DIR_BACKWARDS)
				{
					m_index += 1;
					m_current -= m_step;
					if (m_current < m_from)
					{
						m_state = STATE_END;
						return false;
					}
				}
				else 
				{
					m_state = STATE_END;
					return false;
				}
				break;
			case STATE_END:
				return false;
			}
			return true;
		}

		T			m_from;
		T			m_to;
		s32			m_state;
		s32			m_index;
		T			m_current;
		T			m_step;
	};

	template<typename T>
	inline range_t<T>::range_t()
		: m_state(STATE_END)
		, m_index(0)
		, m_step(0)
	{
		m_from = 0;
		m_to = 0;
		m_current = 0;
	}

	template<typename T>
	inline range_t<T>::range_t(until_t<T> _until)
		: m_state(STATE_START | DIR_FORWARDS)
		, m_index(0)
		, m_step(1)
	{
		m_from = 0;
		m_to = _until.m_until;
		m_current = m_from;
	}

	template<typename T>
	inline range_t<T>::range_t(from_t<T> _from, until_t<T> _until)
		: m_state(STATE_START | DIR_FORWARDS)
		, m_index(0)
		, m_step(1)
	{
		m_from = _from.m_from;
		m_to = _until.m_until;
		m_current = m_from;
	}

	template<typename T>
	inline range_t<T>::range_t(from_t<T> _from, count_t _count)
		: m_state(STATE_START | DIR_FORWARDS)
		, m_index(0)
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
		, m_state(range.m_state)
		, m_index(range.m_index)
		, m_current(range.m_current)
		, m_step(range.m_step)
	{
	}

	template<typename T>
	inline 	range_t<T>	forwards(range_t<T> const& iter)
	{
		range_t<T> range(iter);
		range.reset(range_t<T>::DIR_FORWARDS);
		return range;
	}

	template<typename T>
	inline range_t<T>	backwards(range_t<T> const& iter)
	{
		range_t<T> range(iter);
		range.reset(range_t<T>::DIR_BACKWARDS);
		return range;
	}
};

#endif ///< END __XBASE_LIMITS_H__
