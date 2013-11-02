#include "xbase\x_target.h"
#include "xbase\x_allocator.h"
#include "xbase\x_base.h"
#include "xbase\x_debug.h"

namespace xbase
{
	void			x_Init()
	{
		xcore::x_asserthandler::sRegisterHandler(NULL);		// This will initialize the default handler
	}

	void			x_Exit()
	{
		xcore::x_asserthandler::sRegisterHandler(NULL);		// This will initialize the default handler
	}
};


