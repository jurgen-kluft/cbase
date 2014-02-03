#include "xbase\x_target.h"
#include "xbase\x_allocator.h"
#include "xbase\x_base.h"
#include "xbase\x_debug.h"

namespace xbase
{
#ifdef X_ASSERT
	void			x_Init()
	{
		xcore::x_asserthandler::sRegisterHandler(NULL);		// This will initialize the default handler
	}

	void			x_Exit()
	{
		xcore::x_asserthandler::sRegisterHandler(NULL);		// This will initialize the default handler
	}
#else
	void			x_Init()
	{
	}

	void			x_Exit()
	{
	}
#endif
};


