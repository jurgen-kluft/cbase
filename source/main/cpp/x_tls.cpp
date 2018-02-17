#include "xbase/x_tls.h"

namespace xcore
{
	// For xbase what would we like to have per thread?:
	// - An allocator
	// - A local string allocator
	// - Thread Local Storage
	// - ?

	class x_TLS_NULL : public x_TLS
	{
	public:
		virtual			~x_TLS_NULL() { }

		virtual s32		max() const { return 32; }

	protected:
		virtual	void	i_set(s32 index, void * pvData) { mSlots[index] = pvData; }
		virtual	void	i_get(s32 index, void *& pvData) { pvData = mSlots[index]; }
		virtual	void	i_get(s32 index, void const*& pvData) const { pvData = mSlots[index]; }

		void*			mSlots[32];
	};

	static x_TLS_NULL	sNonThreadSafeInstance;
	static x_TLS*		sInstance = &sNonThreadSafeInstance;

	void			x_TLS::sSet(x_TLS* instance)
	{
		sInstance = instance;
	}

	x_TLS*			x_TLS::sGet()
	{
		return sInstance;
	}

}

