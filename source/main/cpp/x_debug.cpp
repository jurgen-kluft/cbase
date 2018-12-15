//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_target.h"
#ifdef X_ASSERT

#include "xbase/x_tls.h"
#include "xbase/x_debug.h"
#include "xbase/x_console.h"
#include "xbase/x_log.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"


namespace xcore
{
	//==============================================================================
	// Default input func
	//==============================================================================
	class xassert_default : public xasserthandler
	{
	public:
		xbool	handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
	};

	static xasserthandler* sInternalSetAssertHandler(xasserthandler* handler)
	{
		if (handler==NULL)
		{
			static xassert_default sDefaultAssertHandler;
			handler = &sDefaultAssertHandler;
		}
		xtls::set<xtls::ASSERT_HANDLER, xasserthandler>(handler);
		return handler;
	}

	void	xasserthandler::sRegisterHandler(xasserthandler* handler)
	{
		sInternalSetAssertHandler(handler);
	}
	

	//------------------------------------------------------------------------------
	// Summary:
	//     PRIVATE XBASE FUNCTION. IGNORE IGNORE.
	// Arguments:
	//     flags                - Reference to a local copy of flags for the assert.
	//     fileName             - File name in which the assert happen
	//     lineNumber           - Line number where the assert happen
	//     exprString           - Expression of the assert
	//     messageString        - Additional string containing information about the 
	//                            assert.
	// Returns:
	//     xbool - xTRUE when the program should be halted, xFALSE other wise
	// Description:
	//     This function is call when an assert happens.
	// See Also:
	//     x_SetAssertHandler
	//------------------------------------------------------------------------------

	xbool xAssertHandler(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
	{
		// From the TLS, get the debug object
		// Call handle_assert() on that object and return
		xasserthandler* handler;
		xtls::get<xtls::ASSERT_HANDLER, xasserthandler>(handler);

		if (handler == NULL)
			handler = sInternalSetAssertHandler(NULL);

		return handler->handle_assert(flags, fileName, lineNumber, exprString, messageString);
	}


	xbool xassert_default::handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
	{
		//
		// handle flags
		//
		if ((flags & xasserthandler::XDB_FLAG_IGNORE) != 0) 
		{
			return xFALSE;
		}

		//
		// next time ignore it
		//
		flags |= xasserthandler::XDB_FLAG_IGNORE;

		//
		// Survive NULL entries
		//
		if(fileName      == NULL) fileName      = "Unknown";
		if(exprString    == NULL) exprString    = "Unknown";
		if(messageString == NULL) messageString = "Unknown";

		//
		// Create the report to print
		//
		// utf32::runez<1024> report;
		// utf32::runez<64> fmtstr;
		// ascii::crunes fmtascii("*  EXPR: %s\n*  MSG : %s\n*  FILE: %s\n*  LINE: %d\n");
		// utf::copy(fmtascii, fmtstr);
		// utf32::sprintf(report, fmtstr, x_va(exprString), x_va(messageString), x_va(fileName), x_va(lineNumber));

		//
		// Dump the scope info
		//
		x_LogError("Assert", "%s(%d): %s; %s ", x_va_list(x_va(fileName), x_va(lineNumber), x_va(exprString), x_va(messageString)));

		//
		// Default: Skip this assert
		//
		return xFALSE;
	}

};

#endif

