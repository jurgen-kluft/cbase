#include "cbase/c_target.h"
#include "cbase/c_context.h"
#include "cbase/c_debug.h"

#ifdef D_ASSERT
#include "cbase/c_console.h"
#include "cbase/c_log.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"


namespace ncore
{
    //==============================================================================
    // Default input func
    //==============================================================================
    class xassert_default : public asserthandler_t
    {
    public:
        bool	handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
    };

    asserthandler_t*	asserthandler_t::sGetDefaultAssertHandler()
    {
        static xassert_default sAssertHandler;
        asserthandler_t* handler = &sAssertHandler;
        return handler;		
    }
    
    class xassert_release : public asserthandler_t
    {
    public:
        bool	handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
        {
            return false;
        }
    };

    asserthandler_t*	asserthandler_t::sGetReleaseAssertHandler()
    {
        static xassert_release sAssertHandler;
        asserthandler_t* handler = &sAssertHandler;
        return handler;		
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
    //     bool - True when the program should be halted, False other wise
    // Description:
    //     This function is call when an assert happens.
    // See Also:
    //     x_SetAssertHandler
    //------------------------------------------------------------------------------

    bool gAssertHandler(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
    {
        // From the thread context, get the assert handler
        // Call handle_assert() on that object and return
        asserthandler_t* handler = context_t::assert_handler();

        if (handler == nullptr)
            handler = asserthandler_t::sGetDefaultAssertHandler();

        return handler->handle_assert(flags, fileName, lineNumber, exprString, messageString);
    }


    bool xassert_default::handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
    {
        //
        // handle flags
        //
        if ((flags & asserthandler_t::DB_FLAG_IGNORE) != 0) 
        {
            return False;
        }

        //
        // next time ignore it
        //
        flags |= asserthandler_t::DB_FLAG_IGNORE;

        //
        // Survive nullptr entries
        //
        if(fileName      == nullptr) fileName      = "Unknown";
        if(exprString    == nullptr) exprString    = "Unknown";
        if(messageString == nullptr) messageString = "Unknown";

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
        D_LogError("Assert", "%s(%d): %s; %s ", va_list_t(va_t(fileName), va_t(lineNumber), va_t(exprString), va_t(messageString)));

        //
        // Default: Skip this assert
        //
        return False;
    }

};

#else

namespace ncore
{
    class xassert_release : public asserthandler_t
    {
    public:
        bool	handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
        {
            return false;
        }
    };

	asserthandler_t*	asserthandler_t::sGetReleaseAssertHandler()
	{
		static xassert_release sAssertHandler;
		asserthandler_t* handler = &sAssertHandler;
		return handler;		
	}
}

#endif

