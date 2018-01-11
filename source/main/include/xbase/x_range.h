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
		inline		range_t(until_t<T> _until);
		inline		range_t(from_t<T> _from, until_t<T> _until);
		inline		range_t(from_t<T> _from, count_t _count);
		inline		range_t(range_t<T> const& range);

		s32			index() const { return m_index; }
		T			item() const { return m_current; }

		T &			operator * (void) { return m_current; }
		T const&	operator * (void) const { return m_current; }

		enum EState { STATE_START, STATE_ITER, STATE_END };
		enum EMode { MODE_INIT, MODE_ITER };

		void		reset()
		{
			m_state = STATE_START;
		}

		bool		forward()
		{
			switch (m_state)
			{
			case STATE_START:
				m_index = 0;
				m_current = m_from;
				m_state = STATE_ITER;
				break;
			case STATE_ITER:
				m_index += 1;
				m_current += m_step;
				if (m_current >= m_to)
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

		bool		backward()
		{
			switch (m_state)
			{
			case range_t<T>::STATE_START:
				m_index = 0;
				m_current = m_to - m_step;
				m_state = STATE_ITER;
				break;
			case range_t<T>::STATE_ITER:
				m_index += 1;
				m_current -= m_step;
				if (m_current < m_from)
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
	inline range_t<T>::range_t(until_t<T> _until)
		: m_state(STATE_START)
		, m_index(0)
		, m_step(1)
	{
		m_from = 0;
		m_to = _until.m_until;
		m_current = m_from;
	}

	template<typename T>
	inline range_t<T>::range_t(from_t<T> _from, until_t<T> _until)
		: m_state(STATE_START)
		, m_index(0)
		, m_step(1)
	{
		m_from = _from.m_from;
		m_to = _until.m_until;
		m_current = m_from;
	}

	template<typename T>
	inline range_t<T>::range_t(from_t<T> _from, count_t _count)
		: m_state(STATE_START)
		, m_index(0)
		, m_step(1)
	{
		m_from = _from.m_from;
		m_to = m_from + _count.m_count;
		m_current = m_from;
	}

	template<typename T>
	inline bool		forward(range_t<T>& iter)
	{
		return iter.forward();
	}

	template<typename T>
	inline bool		iterate(range_t<T>& iter)
	{
		return iter.forward();
	}

	template<typename T>
	inline bool		backward(range_t<T>& iter)
	{
		return iter.backward();
	}
};

#endif ///< END __XBASE_LIMITS_H__
