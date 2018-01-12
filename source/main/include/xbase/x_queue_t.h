#ifndef __XBASE_QUEUE_T_H__
#define __XBASE_QUEUE_T_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"
#include "xbase/x_range.h"
#include "xbase/x_slice_t.h"


//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	// ----------------------------------------------------------------------------------------
	//   QUEUE
	// ----------------------------------------------------------------------------------------

	template<typename T>
	class queue_t;

	template<typename T>
	class queue_iter_t
	{
	public:
		queue_iter_t(slice_t<T>& _slice, s32 head, s32 tail) 
			: m_state(START)
			, m_index(head)
			, m_head(head)
			, m_tail(tail)
			, m_size(head - tail)
			, m_slice(_slice) 
		{ 
		}

		s32				index() const			{ return m_index; }
		T const&		item() const			{ return m_slice[m_index]; }

		T const&		operator * (void) const	{ return m_slice[m_index]; }

		void			reset();
		bool			iterate();

		enum EState { START, ITERATE, END };
		s32				m_state;
		u32				m_index;

		s32				m_head;
		s32				m_tail;
		s32				m_size;
		slice_t<T>&		m_slice;
	};

	template<typename T>
	class queue_t
	{
	public:
		inline			queue_t() : m_head(0), m_tail(0) {}

		u32				size() const;
		u32				max() const;

		bool			is_empty() const;
		bool			is_full() const;

		bool			push(T const& item);
		bool			pop(T& item);

		typedef			queue_iter_t<T>		iter_t;
		iter_t			begin() const;

		u32				m_head;
		u32				m_tail;
		slice_t<T>		m_data;
	};

	template<typename T>
	u32				queue_t<T>::size() const
	{
		return m_head - m_tail;
	}

	template<typename T>
	u32				queue_t<T>::max() const
	{
		return m_data.size();
	}

	template<typename T>
	bool			queue_t<T>::is_empty() const
	{
		return m_head == m_tail;
	}

	template<typename T>
	bool			queue_t<T>::is_full() const
	{
		return (m_head > m_tail) && (m_head % max()) == m_tail;
	}

	template<typename T>
	bool			queue_t<T>::push(T const& item)
	{
		if (is_full())
			return false;
		s32 const index = m_head % max();
		m_data[index] = item;
		m_head++;
		return true;
	}

	template<typename T>
	bool			queue_t<T>::pop(T& item)
	{
		if (is_empty())
			return false;
		s32 const index = m_tail % max();
		item = m_data[index];
		m_tail += 1;
		if (m_tail >= max())
		{
			m_tail = m_tail % max();
			m_head = m_head % max();
		}
		return true;
	}

	template<typename T>
	queue_iter_t<T>	queue_t<T>::begin() const
	{
		queue_t<T>::iter_t iter(m_data, m_head, m_tail);
		return iter;
	}



	template<typename T>
	void				make(xallocator mem, queue_t<T>& proto, s32 cap);
	template<typename T>
	bool				append(queue_t<T>& array, T const& element);
	template<typename T>
	bool				iterate(queue_iter_t<T>& iter);



	template<typename T>
	inline void			make(xallocator mem, queue_t<T>& proto, s32 cap)
	{
		make(mem, proto.m_data, cap);
		proto.m_head = 0;
		proto.m_tail = 0;
	}

	template<typename T>
	inline bool			append(queue_t<T>& q, T const& element)
	{
		return q.push(element);
	}

	template<typename T>
	inline bool			iterate(queue_iter_t<T>& iter)
	{
		return iter.next();
	}

}

#endif
