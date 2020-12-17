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
	class xassert_default : public asserthandler_t
	{
	public:
		bool	handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
	};

	static asserthandler_t* sInternalSetAssertHandler(asserthandler_t* handler)
	{
		if (handler==NULL)
		{
			static xassert_default sDefaultAssertHandler;
			handler = &sDefaultAssertHandler;
		}
		tls_t::set<tls_t::ASSERT_HANDLER, asserthandler_t>(handler);
		return handler;
	}

	void	asserthandler_t::sRegisterHandler(asserthandler_t* handler)
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
	//     bool - xTRUE when the program should be halted, xFALSE other wise
	// Description:
	//     This function is call when an assert happens.
	// See Also:
	//     x_SetAssertHandler
	//------------------------------------------------------------------------------

	bool xAssertHandler(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
	{
		// From the TLS, get the debug object
		// Call handle_assert() on that object and return
		asserthandler_t* handler;
		tls_t::get<tls_t::ASSERT_HANDLER, asserthandler_t>(handler);

		if (handler == NULL)
			handler = sInternalSetAssertHandler(NULL);

		return handler->handle_assert(flags, fileName, lineNumber, exprString, messageString);
	}


	bool xassert_default::handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
	{
		//
		// handle flags
		//
		if ((flags & asserthandler_t::XDB_FLAG_IGNORE) != 0) 
		{
			return xFALSE;
		}

		//
		// next time ignore it
		//
		flags |= asserthandler_t::XDB_FLAG_IGNORE;

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
		// ascii::crunes_t fmtascii("*  EXPR: %s\n*  MSG : %s\n*  FILE: %s\n*  LINE: %d\n");
		// utf::copy(fmtascii, fmtstr);
		// utf32::sprintf(report, fmtstr, va_t(exprString), va_t(messageString), va_t(fileName), va_t(lineNumber));

		//
		// Dump the scope info
		//
		x_LogError("Assert", "%s(%d): %s; %s ", va_list_t(va_t(fileName), va_t(lineNumber), va_t(exprString), va_t(messageString)));

		//
		// Default: Skip this assert
		//
		return xFALSE;
	}

};

#endif

