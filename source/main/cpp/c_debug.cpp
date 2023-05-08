#include "ccore/c_target.h"
#include "cbase/c_context.h"
#include "ccore/c_debug.h"

#ifdef D_ASSERT
#include "cbase/c_log.h"
#endif

namespace ncore
{
#ifdef D_ASSERT

    //==============================================================================
    // Default input func
    //==============================================================================
    class asserthandler_cbase_t : public asserthandler_t
    {
    public:
        bool handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
    };

    static asserthandler_cbase_t sBaseAssertHandler;

    //------------------------------------------------------------------------------
    // Summary:
    //     PRIVATE FUNCTION
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
    //     gAssertHandler
    //------------------------------------------------------------------------------

    bool asserthandler_cbase_t::handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
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
        if (fileName == nullptr)
            fileName = "Unknown";
        if (exprString == nullptr)
            exprString = "Unknown";
        if (messageString == nullptr)
            messageString = "Unknown";

        //
        // Dump the scope info
        //
        D_LogError("Assert", "%s(%d): %s; %s ", va_t(fileName), va_t(lineNumber), va_t(exprString), va_t(messageString));

        //
        // Default: Skip this assert
        //
        return false;
    }

#else

    class asserthandler_release_t : public asserthandler_t
    {
    public:
        bool handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString) { return false; }
    };

    static asserthandler_release_t sBaseAssertHandler;

#endif

}  // namespace ncore

namespace cbase
{
    ncore::asserthandler_t* gSetAssertHandler()
    {
        ncore::asserthandler_t* assertHandler = &ncore::sBaseAssertHandler;
        ncore::gSetAssertHandler(assertHandler);
        return assertHandler;
    }
}  // namespace nbase