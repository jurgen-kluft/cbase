#include "xbase/x_tls.h"

namespace xcore
{
	// For xbase what would we like to have per thread?:
	// - A global heap allocator (in a multi-threaded environment this one should be thread-safe)
	// - A stack allocator (local to a function call)
	// - A local thread allocator (local to the thread)
	// - A random generator

	class xtls_SingleThread : public xtls
	{
	public:
		virtual			~xtls_SingleThread() { }

		virtual s32		max() const { return 32; }

	protected:
		virtual	void	vset(s32 slot, void * pvData) { mSlots[slot] = pvData; }
		virtual	void	vget(s32 slot, void *& pvData) { pvData = mSlots[slot]; }
		virtual	void	vget(s32 slot, void const*& pvData) const { pvData = mSlots[slot]; }

		void*			mSlots[32];
	};

	static xtls_SingleThread	sNonThreadSafeInstance;
	static xtls*				sInstance = &sNonThreadSafeInstance;

	void			xtls::sSet(xtls* instance)
	{
		sInstance = instance;
	}

	xtls*			xtls::sGet()
	{
		return sInstance;
	}

}

