#ifndef __XBASE_THREAD_LOCAL_STORAGE_H__
#define __XBASE_THREAD_LOCAL_STORAGE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xcore
{
	// Thread-Local-Storage API
	extern u32				sTlsAlloc();
	extern void				sTlsFree(u32 id);
	extern void				sTlsSetValue(u32 id, void* data);
	extern void*			sTlsGetValue(u32 id);


	template <class T>
	class xtls
	{
	private:
						xtls(xtls const&) {}
						~xtls() {}

	protected:
						xtls()	{}

	public:
		typedef			xtls<T>		self;

		static self&	Instance(void);
		static void		Release(void);

	public:
		void			SetValue(T* pvData)
		{ 
			sTlsSetValue(mTlsIndex, reinterpret_cast<void*>(pvData)); 
		}

		T const*		GetValue(void) const
		{ 
			return reinterpret_cast<T const*>(sTlsGetValue(mTlsIndex));
		}

		T*				GetValue(void)
		{ 
			return reinterpret_cast<T*>(sTlsGetValue(mTlsIndex)); 
		}

	protected:
		u32				mTlsIndex;
		static xtls<T>*	mInstance;
	};

	template <class T>
	xtls<T>*	xtls<T>::mInstance = 0;

	template <class T>
	xtls<T>& xtls<T>::Instance(void)
	{
		if (mInstance == 0)
		{
			static xtls<T> instance;
			mInstance = &instance;
			mInstance->mTlsIndex = sTlsAlloc();
			sTlsSetValue(mInstance->mTlsIndex, 0);
		}

		return (*mInstance);
	}

	template <class T>
	void xtls<T>::Release(void)
	{
		if (mInstance != 0)
		{
			sTlsFree(mInstance->mTlsIndex);
			mInstance->mTlsIndex = -1;
			mInstance = 0;
		}
	}

}

#endif // __XBASE_THREAD_LOCAL_STORAGE_H__
