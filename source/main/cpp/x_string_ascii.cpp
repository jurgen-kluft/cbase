#include "xbase/x_string_ascii.h"
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_console.h"


//==============================================================================
// INCLUDES
//==============================================================================


/**
 * xCore namespace
 */
namespace xcore
{
	namespace ascii
	{
		#define __XBASE_GENERIC_STRING_FUNCS__
		#include "x_string_funcs.cpp"
	};	///< END ascii namespace
};	///< END xcore namespace

