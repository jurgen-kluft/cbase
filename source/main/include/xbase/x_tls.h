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
	class xTLS
	{
	public:
		virtual				~xTLS() { }

		virtual s32			max() const = 0;

		virtual	void		set(s32 slot, void * inData) = 0;
		virtual	void		get(s32 slot, void *& outData) = 0;
		virtual	void		get(s32 slot, void const *& outData) const = 0;

		static void			sSet(xTLS* );
		static xTLS*		sGet();
	};

	template<s32 SLOT, class T>
	class xtls
	{
	public:
		void	set(T* inData)					{ xTLS::sGet()->set(SLOT, reinterpret_cast<void*>(inData)); }
		void	get(T *& outData)				{ void * p; xTLS::sGet()->get(SLOT, p); outData = (T*)p;}
		void	get(T const *& outData) const	{ void const * p; xTLS::sGet()->get(SLOT, p); outData = (T const*)p; }

	};
}

#endif // __XBASE_THREAD_LOCAL_STORAGE_H__
