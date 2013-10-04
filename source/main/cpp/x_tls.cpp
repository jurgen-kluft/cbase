#include "xbase\x_tls.h"

#ifdef TARGET_PC
#include <Windows.h>

namespace xcore
{
	u32				sTlsAlloc()
	{
		return ::TlsAlloc();
	}

	void			sTlsFree(u32 id)
	{
		if (id != TLS_OUT_OF_INDEXES)
			::TlsFree(id);
	}

	void			sTlsSetValue(u32 id, void* data)
	{
		if (id != TLS_OUT_OF_INDEXES)
			::TlsSetValue(id, data);
	}

	void*			sTlsGetValue(u32 id)
	{
		if (id != TLS_OUT_OF_INDEXES)
			return ::TlsGetValue(id);
		else
			return NULL;
	}
}

#else

#error This platform has no implementation of the Thread Local Storage API

#endif