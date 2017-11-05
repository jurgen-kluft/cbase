//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_target.h"
#ifdef X_ASSERT

#include "xbase/x_tls.h"
#include "xbase/x_bit_field.h"
#include "xbase/x_debug.h"
#include "xbase/x_string_ascii.h"
#include "xbase/x_console.h"
#include "xbase/x_log.h"


//==============================================================================
// xCore namespace 
//==============================================================================
namespace xcore
{
	//==============================================================================
	// Default input func
	//==============================================================================
	class x_assert_default : public x_asserthandler
	{
	public:
		xbool	HandleAssert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
	};

	static x_asserthandler* sInternalSetAssertHandler(x_asserthandler* handler)
	{
		if (handler!=NULL)
		{
			xtls<x_asserthandler>::Instance().SetValue(handler);
		}
		else
		{
			static x_assert_default sDefaultAssertHandler;
			handler = &sDefaultAssertHandler;
			xtls<x_asserthandler>::Instance().SetValue(handler);
		}
		return handler;
	}

	void	x_asserthandler::sRegisterHandler(x_asserthandler* handler)
	{
		sInternalSetAssertHandler(handler);
	}
	

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
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

	xbool x_AssertHandler(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
	{
		// From the TLS, get the debug object
		// Call HandleAssert() on that object and return
		x_asserthandler* handler = (x_asserthandler*)xtls<x_asserthandler>::Instance().GetValue();
		if (handler == NULL)
			handler = sInternalSetAssertHandler(NULL);

		return handler->HandleAssert(flags, fileName, lineNumber, exprString, messageString);
	}


	xbool x_assert_default::HandleAssert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
	{
		//
		// handle flags
		//
		if(xbfIsSet(flags, x_asserthandler::XDB_FLAG_IGNORE)) 
		{
			return xFALSE;
		}

		//
		// next time ignore it
		//
		flags |= x_asserthandler::XDB_FLAG_IGNORE;

		//
		// Survive NULL entries
		//
		if(fileName      == NULL) fileName      = "Unknown";
		if(exprString    == NULL) exprString    = "Unknown";
		if(messageString == NULL) messageString = "Unknown";

		//
		// Create the report to print
		//
		char report_buffer[1024 + 1];
		uchar* report = (uchar*)&report_buffer[0];
		uchar const* report_eos = report + 1024;
		uchar const* fmt = (uchar const*)"*  EXPR: %s\n*  MSG : %s\n*  FILE: %s\n*  LINE: %d\n";
		uchar const* fmt_eos = fmt + (4*12);
		s32 index = ascii::sprintf(report, report_eos, fmt, fmt_eos, exprString, messageString, fileName, lineNumber);

		//
		// Dump the scope info
		//
		x_LogError("Assert", "%s(%d): %s; %s ", x_va_list(fileName, lineNumber, exprString, messageString));

		//
		// Default: Skip this assert
		//
		return xFALSE;
	}

//==============================================================================
// END xCore namespace
//==============================================================================
};

#endif

