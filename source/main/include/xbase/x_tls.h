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
	class xtls
	{
	public:
		static void			sSet(xtls*);
		static xtls*		sGet();

		enum
		{
			ASSERT_HANDLER = 0,
			HEAP_ALLOCATOR = 1,
			STACK_ALLOCATOR = 2,
			STRING_ALLOCATOR = 3,
			RANDOM_GENERATOR = 4,
		};

		template<s32 SLOT, class T>
		static void	set(T* inData)					{ sGet()->vset(SLOT, reinterpret_cast<void*>(inData)); }

		template<s32 SLOT, class T>
		static void	get(T *& outData)				{ void * p= nullptr; sGet()->vget(SLOT, p); outData = (T*)p;}

	protected:
		virtual				~xtls() { }

		virtual s32			vmax() const = 0;
		virtual	void		vset(s32 SLOT, void * inData) = 0;
		virtual	void		vget(s32 SLOT, void *& outData) = 0;
		virtual	void		vget(s32 SLOT, void const *& outData) const = 0;
	};
}

#endif // __XBASE_THREAD_LOCAL_STORAGE_H__
