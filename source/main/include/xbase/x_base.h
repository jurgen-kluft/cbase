#ifndef __XBASE_H__
#define __XBASE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"

namespace xbase
{

	//----------------------------------------------------------------------------------------------------------------
	// Init / Exit

	extern void		x_Init();
	extern void		x_Exit();

};

#endif // __XBASE_H__
