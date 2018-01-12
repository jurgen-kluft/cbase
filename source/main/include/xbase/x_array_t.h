#ifndef __XBASE_ARRAY_T_H__
#define __XBASE_ARRAY_T_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_slice_t.h"
#include "xbase/x_qsort.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	// ----------------------------------------------------------------------------------------
	//   ARRAY
	// ----------------------------------------------------------------------------------------
	template<typename T>
	class array_t;

	template<typename T>
	class array_iter_t
	{
	public:
						array_iter_t(array_t<T> const& _array) : m_state(START), m_index(0), m_array(_array) {}

		s32				index() const			{ return m_index; }
		T const&		item() const			{ return m_array[m_index]; }

		T const&		operator * (void) const	{ return m_array[m_index]; }

		void			reset()
		{
			m_state = START;
		}

		bool			iterate()
		{
			if (m_state == ITERATE) {
				++m_index;
			} 
			else if (m_state == END) {
				return false;
			}
			else if (m_state == START) {
				m_index = 0;
				m_max = m_array.size();
				m_state = ITERATE;
			}
		
			if (m_index < m_max)
				return true;

			m_state = END;
			return false;
		}

	protected:
		enum EState { START, ITERATE, END };
		s32				m_state;
		u32				m_index;
		u32				m_max;
		array_t<T> const&	m_array;
	};

	template<typename T>
	s32					compare_t(T const& a, T const& b)
	{
		if (a < b)
			return -1;
		else if (a > b)
			return 1;
		return 0;
	}

	template<typename T>
	class array_t
	{
	public:
						array_t() : m_size(0) { }

		inline u32		size() const	{ return m_size; }
		inline u32		max() const		{ return m_data.size(); }

		array_t<T>		operator () (u32 to) const;
		array_t<T>		operator () (u32 from, u32 to) const;

		T &				operator [] (s32 index);
		T const &		operator [] (s32 index) const;

		array_iter_t<T>	begin() const;

		static s32		compare(const void* const le, const void* const re, void* data)
		{
			T const& let = *(T const*)le;
			T const& ret = *(T const*)re;
			s32 const c = compare_t<T>(let, ret);
			return c;
		}

		u32				m_size;
		slice_t<T>		m_data;
	};

	template<typename T>
	inline array_t<T>		array_t<T>::operator () (u32 to) const
	{
		array_t<T> other;
		other.m_data = m_data(0, to);
		other.m_size = m_size;
		if (to < m_size)
			other.m_size = to;
		return other;
	}

	template<typename T>
	inline array_t<T>		array_t<T>::operator () (u32 from, u32 to) const
	{
		array_t<T> other;
		other.m_data = m_data(from, to);
		other.m_size = m_size;
		if (from >= m_size) {
			other.m_size = 0;
		}
		else if (to > m_size) {
			other.m_size = m_size - from;
		}
		else {
			other.m_size = to - from;
		}
		return other;
	}


	template<typename T>
	inline T &				array_t<T>::operator [] (s32 index)
	{
		return m_data[index];
	}

	template<typename T>
	inline T const &		array_t<T>::operator [] (s32 index) const
	{
		return m_data[index];
	}

	template<typename T>
	inline array_iter_t<T>	array_t<T>::begin() const
	{
		return array_iter_t<T>(*this);
	}


	template<typename T>
	void				make(xallocator mem, array_t<T>& proto, s32 cap, s32 len);
	template<typename T>
	bool				iterate(array_iter_t<T>& iter);
	template<typename T>
	bool				append(array_t<T>& array, T const& element);
	template<typename T>
	void				sort(array_t<T>& array);


	template<typename T>
	inline void			make(xallocator mem, array_t<T>& proto, s32 cap, s32 len)
	{
		if (len > cap)
			cap = len;

		make(mem, proto.m_data, cap);
		proto.m_size = len;
	}

	template<typename T>
	inline bool			iterate(array_iter_t<T>& iter)
	{
		return iter.iterate();
	}

	template<typename T>
	inline bool			append(array_t<T>& array, T const& element)
	{
		// Logic:
		// Appending inline will only happen when the refcount on the
		// data is 1, otherwise a new copy will be made.
		u32 maxsize = array.m_data.size();
		if (array.m_data.m_slice.refcnt() == 1 && (array.size() < maxsize))
		{
			array.m_data[array.m_size] = element;
			array.m_size += 1;
		}
		else {
			// Make a copy, first see what kind of size we need
			if (array.size() == maxsize)
				maxsize += 2 * (maxsize / 3);
			array.m_data.m_slice.resize(maxsize);

			array.m_data[array.m_size] = element;
			array.m_size += 1;
		}
		return true;
	}
	
	template<typename T>
	inline void			sort(array_t<T>& array)
	{
		xbyte* data = array.m_data.m_slice.mData->mData;
		u32 n = array.m_size;
		u32 es = array.m_data.m_slice.mData->mItemSize;
		xqsort(data, n, es, &array.compare, NULL);
	}

}

#endif
