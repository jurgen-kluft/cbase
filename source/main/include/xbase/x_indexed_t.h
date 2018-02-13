#ifndef __XBASE_INDEXED_T_H__
#define __XBASE_INDEXED_T_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_array_t.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	// ----------------------------------------------------------------------------------------
	//   INDEXED
	// ----------------------------------------------------------------------------------------
	template<typename T>
	class indexed_t
	{
	public:
		inline			indexed_t() : m_free(), m_items() {}

		u32				size() const;
		u32				max() const;

		bool			is_empty() const;
		bool			is_full() const;

		T*				get(u32 idx);
		T const*		get(u32 idx) const;

		bool			alloc(T*& ptr, u32& index);
		void			dealloc(u32 index);

		u32				m_size;		// How many items are allocated
		array_t<u32>	m_free;		// Indices that point to free items
		array_t<T>		m_items;	// The storage of items
	};

	template<typename T>
	u32				indexed_t<T>::size() const
	{
		return m_size;
	}

	template<typename T>
	u32				indexed_t<T>::max() const
	{
		return m_free.size();
	}

	template<typename T>
	bool			indexed_t<T>::is_empty() const
	{
		return size() == 0;
	}

	template<typename T>
	bool			indexed_t<T>::is_full() const
	{
		return size() == max();
	}

	template<typename T>
	T*				indexed_t<T>::get(u32 idx)
	{
		return &m_items[idx];
	}

	template<typename T>
	T const*		indexed_t<T>::get(u32 idx) const
	{
		return &m_items[idx];
	}

	template<typename T>
	bool			indexed_t<T>::alloc(T*& ptr, u32& index)
	{
		if (is_full())
			return false;
		index = m_free[m_size];
		new (&m_items[index]) T();	// Construct
		ptr = get(index);
		m_size += 1;
		return true;
	}

	template<typename T>
	void			indexed_t<T>::dealloc(u32 index)
	{
		m_size -= 1;
		m_free[m_size] = index;
		(&m_items[index])->~T();	// Destruct
	}

	template<typename T>
	void				make(xallocator mem, indexed_t<T>& proto, s32 cap);

	template<typename T>
	void				make(xallocator mem, indexed_t<T>& proto, s32 cap)
	{
		make(mem, proto.m_free, cap, cap);
		make(mem, proto.m_items, cap, cap);
		proto.m_size = 0;

		range_t<s32> iter(from(0), count(cap));
		while (iterate(iter)) {
			proto.m_free[*iter] = *iter;
		}
	}

}

#endif
