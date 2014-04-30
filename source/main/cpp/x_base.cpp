#include "xbase\x_target.h"
#include "xbase\x_allocator.h"
#include "xbase\x_base.h"
#include "xbase\x_debug.h"

namespace xcore
{
	static x_iallocator*	sSystemAllocator = NULL;
	x_iallocator*			x_iallocator::default()
	{
		return sSystemAllocator;
	}
}

namespace xbase
{
#ifdef X_ASSERT
	void			x_Init()
	{
		xcore::sSystemAllocator = xcore::gCreateSystemAllocator();
		xcore::x_asserthandler::sRegisterHandler(NULL);		// This will initialize the default handler
	}

	void			x_Exit()
	{
		xcore::x_asserthandler::sRegisterHandler(NULL);		// This will initialize the default handler

		xcore::sSystemAllocator->release();
		xcore::sSystemAllocator = NULL;
	}
#else
	void			x_Init()
	{
		xcore::sSystemAllocator = xcore::gCreateSystemAllocator();
	}

	void			x_Exit()
	{
		xcore::sSystemAllocator->release();
		xcore::sSystemAllocator = NULL;
	}
#endif
};


