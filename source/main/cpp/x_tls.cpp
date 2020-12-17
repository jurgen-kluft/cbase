#include "xbase/x_tls.h"

namespace xcore
{
	// For xbase what would we like to have per thread?:
	// - A global heap allocator (in a multi-threaded environment this one should be thread-safe)
	// - A stack allocator (local to a function call)
	// - A local thread allocator (local to the thread)
	// - A random generator

	class xtls_SingleThread : public tls_t
	{
	public:
		virtual			~xtls_SingleThread() { }

		virtual s32		vmax() const { return 32; }

	protected:
		virtual	void	vset(s32 slot, void * pvData) { mSlots[slot] = pvData; }
		virtual	void	vget(s32 slot, void *& pvData) { pvData = mSlots[slot]; }
		virtual	void	vget(s32 slot, void const*& pvData) const { pvData = mSlots[slot]; }

		void*			mSlots[32];
	};

	static xtls_SingleThread	sNonThreadSafeInstance;
	static tls_t*				sInstance = &sNonThreadSafeInstance;

	void			tls_t::sSet(tls_t* instance)
	{
		sInstance = instance;
	}

	tls_t*			tls_t::sGet()
	{
		return sInstance;
	}

}

