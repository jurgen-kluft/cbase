#ifndef __XBASE_THREAD_LOCAL_STORAGE_H__
#define __XBASE_THREAD_LOCAL_STORAGE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xcore
{
	//==============================================================================
	// 
	// The TLS API
	//
	// Note:
	//    This is a general API and the default instance is NOT thread-safe.
	//    If an application needs a thread-safe TLS instance then please
	//    include the xthread package, it has a thread-safe TLS implementation.
	//==============================================================================
	class x_TLS
	{
	public:
		virtual				~x_TLS() { }

		virtual s32			max() const = 0;

		static void			sSet(x_TLS* );
		static x_TLS*		sGet();

		virtual	void		i_set(s32 index, void * inData) = 0;
		virtual	void		i_get(s32 index, void *& outData) = 0;
		virtual	void		i_get(s32 index, void const *& outData) const = 0;
	};

	template<s32 IDX, class T>
	class xtls
	{
	public:
		void	set(T* inData)					{ x_TLS::sGet()->i_set(IDX, reinterpret_cast<void*>(inData)); }
		void	get(T *& outData)				{ void * p; x_TLS::sGet()->i_get(IDX, p); outData = (T*)p;}
		void	get(T const *& outData) const	{ void const * p; x_TLS::sGet()->i_get(IDX, p); outData = (T const*)p; }

	};
}

#endif // __XBASE_THREAD_LOCAL_STORAGE_H__
