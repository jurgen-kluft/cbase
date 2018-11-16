#include "xbase/x_tls.h"

namespace xcore
{
	// For xbase what would we like to have per thread?:
	// - A global heap allocator (in a multi-threaded environment this one should be thread-safe)
	// - A stack allocator (local to a function call)
	// - A local thread allocator (local to the thread)
	// - A random generator

	class xTLS_SingleThread : public xTLS
	{
	public:
		virtual			~xTLS_SingleThread() { }

		virtual s32		max() const { return 32; }

	protected:
		virtual	void	set(s32 slot, void * pvData) { mSlots[slot] = pvData; }
		virtual	void	get(s32 slot, void *& pvData) { pvData = mSlots[slot]; }
		virtual	void	get(s32 slot, void const*& pvData) const { pvData = mSlots[slot]; }

		void*			mSlots[32];
	};

	static xTLS_SingleThread	sNonThreadSafeInstance;
	static xTLS*				sInstance = &sNonThreadSafeInstance;

	void			xTLS::sSet(xTLS* instance)
	{
		sInstance = instance;
	}

	xTLS*			xTLS::sGet()
	{
		return sInstance;
	}

}

