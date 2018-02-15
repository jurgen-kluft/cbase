#ifndef __XBASE_SLICE_T_H__
#define __XBASE_SLICE_T_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"
#include "xbase/x_integer.h"
#include "xbase/x_slice.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{

	//==============================================================================
	// Slice
	// A reference counted slice owning a memory block with a view/window (from,to).
	//==============================================================================
	template<typename T>
	class slice_t
	{
	public:
						slice_t();
						slice_t(x_iallocator* mem, s32 size);
						slice_t(slice_t<T> const& other);
						~slice_t();

		s32				size() const;

		slice_t			operator () (s32 to) const;
		slice_t			operator () (s32 from, s32 to) const;

		T &				operator [] (s32 index);
		T const &		operator [] (s32 index) const;

		slice_t<T>&		operator = (const slice_t<T>& other)
		{
			m_slice.release();
			m_slice = other.m_slice.obtain();
			return *this;
		}

		slice			m_slice;
	};

	template<typename T>
	inline slice_t<T>::slice_t()
		: m_slice()
	{
	}

	template<typename T>
	inline slice_t<T>::slice_t(x_iallocator* mem, s32 count)
		: m_slice(mem, count, sizeof(T))
	{
	}

	template<typename T>
	inline slice_t<T>::slice_t(slice_t<T> const& other)
		: m_slice(other.m_slice.obtain())
	{
	}

	template<typename T>
	inline slice_t<T>::~slice_t()
	{
		m_slice.release();
	}

	template<typename T>
	inline s32			slice_t<T>::size() const
	{
		return m_slice.size();
	}

	template<typename T>
	inline slice_t<T>	slice_t<T>::operator () (s32 to) const
	{
		slice_t<T> slice;
		slice.m_slice = m_slice.view(0, to);
		return slice;
	}
	
	template<typename T>
	inline slice_t<T>	slice_t<T>::operator () (s32 from, s32 to) const
	{
		slice_t<T> slice;
		slice.m_slice = m_slice.view(from, to);
		return slice;
	}

	template<typename T>
	inline T &			slice_t<T>::operator [] (s32 index)
	{
		T * item = (T *)m_slice.get_at(index);
		return *item;
	}

	template<typename T>
	inline T const &	slice_t<T>::operator [] (s32 index) const
	{
		T const* item = (T const*)m_slice.get_at(index);
		return *item;
	}


	template<typename T>
	void				make(xallocator mem, slice_t<T>& proto, s32 size);

	template<typename T>
	inline void			make(xallocator mem, slice_t<T>& proto, s32 size)
	{
		slice::alloc(proto.m_slice, mem, size, sizeof(T));
	}

}

#endif
