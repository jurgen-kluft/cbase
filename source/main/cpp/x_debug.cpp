//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_target.h"
#ifdef X_ASSERT

#include "xbase\x_bit_field.h"
#include "xbase\x_debug.h"
#include "xbase\x_string_std.h"
#include "xbase\x_console.h"
#include "xbase\x_log.h"


//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	static xbool sDefaultAssertHandler(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
	static xassert_fn* sFunction = sDefaultAssertHandler;

	//==============================================================================
	//==============================================================================
	//==============================================================================
	// LOCAL STUFF
	//==============================================================================
	//==============================================================================
	//==============================================================================
	//DOM-IGNORE-BEGIN
	//==============================================================================
	// Locals
	//==============================================================================
	enum
	{
		XDB_FLAG_IGNORE             = 1<<1,
		XDB_FLAG_CONCADENATE        = 1<<2,
	};


	//==============================================================================
	// Debug input func
	//==============================================================================
	static void xassert_input_fn_default(xbool& _button0, xbool& _button1, xbool& _button2, xbool& _button3)
	{
		_button0 = true;
		_button1 = false;
		_button2 = false;
		_button3 = false;
	}

	static xassert_input_fn*	sAssertInputFunction = xassert_input_fn_default;

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
		static s32 sCount=0;

		sCount++;
		if (sCount >= 10) 
		{
			// Hitting infinite recursion
			BREAK;
		}

		xbool shouldBeHalted = sFunction(flags, fileName, lineNumber, exprString, messageString);
		sCount--;
		return shouldBeHalted;
	}

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Set assert handler.
	// Arguments:
	//     AssertHandler - This is the new assert handler to be set.
	// Returns:
	//     void
	// Description:
	//     In x_DebugInstanceInit(), the default assert handler is set to
	//     sDefaultAssertHandler.
	// See Also:
	//     x_AssertHandler, sDefaultAssertHandler
	//------------------------------------------------------------------------------
	void x_SetAssertHandler(xassert_fn* AssertHandler)
	{
		if (AssertHandler == NULL)
			return;

		sFunction = AssertHandler;
	}


	//==============================================================================
	//==============================================================================
	//==============================================================================
	// LOCAL STUFF
	//==============================================================================
	//==============================================================================
	//==============================================================================

	//==============================================================================
	// VARIABLES
	//==============================================================================
	//DOM-IGNORE-BEGIN
	xbool   gSkipAllThrowDialogs = xFALSE;
	xbool   gIsInExceptionBreak  = xTRUE;
	//DOM-IGNORE-END


	//==============================================================================
	//==============================================================================
	//==============================================================================
	// CUSTOM MESSAGE BOX
	//==============================================================================
	// This is so that I can add a few more buttons to the regular messagebox
	// also I need to have a way to change the way the buttons display info.
	//==============================================================================
	//==============================================================================
	//==============================================================================
	//DOM-IGNORE-BEGIN

	//------------------------------------------------------------------------------
	enum EButtonCmd
	{
		CMD_BUTTON1 = 0,
		CMD_BUTTON2 = 1,
		CMD_BUTTON3 = 2,
		CMD_BUTTON4 = 3,

		NUM_BUTTONS,
	};

	char* gButtonNameWII[] = { "A", "B", "1", "2" };
	char* gButtonName3DS[] = { "A", "B", "X", "Y" };
	char* gButtonNamePSP[] = { "CIRCLE", "CROSS", "SQUARE", "TRIANGLE" };
	char* gButtonNamePS3[] = { "CIRCLE", "CROSS", "SQUARE", "TRIANGLE" };
	char* gButtonName360[] = { "A", "B", "X", "Y" };
	char* gButtonNamePC[]  = { "A", "B", "X", "Y" };

#if defined(TARGET_WII)
	char** gButtonName = gButtonNameWII;
#elif defined(TARGET_3DS)
	char** gButtonName = gButtonName3DS;
#elif defined(TARGET_PSP)
	char** gButtonName = gButtonNamePSP;
#elif defined(TARGET_PS3)
	char** gButtonName = gButtonNamePS3;
#elif defined(TARGET_360)
	char** gButtonName = gButtonName360;
#elif defined(TARGET_PC)
	char** gButtonName = gButtonNamePC;
#endif

	struct custom_msgbox
	{
		s32				mNumButtons;
		char*			mButtonName[NUM_BUTTONS];
	};

	//------------------------------------------------------------------------------
	// TODO: May be this should be thread safe?
	//------------------------------------------------------------------------------
	static s32 customMessageBox(char* szText, char* szCaption, custom_msgbox& msgBoxInfo)
	{
		ASSERT(msgBoxInfo.mNumButtons > 0);
		xconsole::writeLine("***********************************");
		xconsole::writeLine("%s*********************************", szText);
		xconsole::writeLine("******************%s******************", szCaption);
		for (int i = 0; i < msgBoxInfo.mNumButtons; ++i)
		{
			xconsole::writeLine("Press %s = %s", gButtonName[i], msgBoxInfo.mButtonName[i]);
		}
		xconsole::writeLine();

		s32 retval = 0;

		while(xTRUE)
		{
			xbool button[4];
			sAssertInputFunction(button[0], button[1], button[2], button[3]);

			if (button[0])
			{
				retval = CMD_BUTTON1;
				break;
			}
			else if (button[1] && msgBoxInfo.mNumButtons >= 2)
			{
				retval = CMD_BUTTON2;
				break;
			}
			else if (button[2] && msgBoxInfo.mNumButtons >= 3)
			{
				retval = CMD_BUTTON3;
				break;
			}
			else if (button[3] && msgBoxInfo.mNumButtons >= 4)
			{
				retval = CMD_BUTTON4;
				break;
			}
		}

		// done
		return retval;
	}

	/*
	static void simpleMessageBox(char* szText, char* szCaption)
	{
		xconsole::writeLine("***********************************");
		xconsole::writeLine("********** %s *********", szText);
		xconsole::writeLine("********** %s *********", szCaption);
		xconsole::writeLine("Press %s to skip", gButtonName[0]);
		xconsole::writeLine("***********************************");
		while(xTRUE)
		{
			xbool button[4];
			sAssertInputFunction(button[0], button[1], button[2], button[3]);
			if (button[0])
				break;
		}
	}
	*/

	//------------------------------------------------------------------------------

	//DOM-IGNORE-END

	//------------------------------------------------------------------------------


	//==============================================================================
	//==============================================================================
	//==============================================================================
	//==============================================================================
	//==============================================================================
	// ASSERT
	//==============================================================================
	//==============================================================================
	//==============================================================================
	//==============================================================================
	//==============================================================================
	//DOM-IGNORE-BEGIN

	//==============================================================================
	// Functions
	//==============================================================================

	//==============================================================================

	xbool sDefaultAssertHandler(
		u32&           flags,
		const char*    fileName,
		s32            lineNumber,
		const char*    exprString,
		const char*    messageString)
	{
		//
		// handle flags
		//
		if(x_FlagIsOn(flags, XDB_FLAG_IGNORE)) 
		{
			return xFALSE;
		}

		//
		// Survive NULL entries
		//
		if(fileName      == NULL) fileName      = "Unknown";
		if(exprString    == NULL) exprString    = "Unknown";
		if(messageString == NULL) messageString = "Unknown";

		//
		// Create the report to print
		//
		char report[1024];
		s32 index = x_sprintf(report, sizeof(report)-1,  "*  EXPR: %s\n"
			"*  MSG : %s\n" 
			"*  FILE: %s\n"
			"*  LINE: %d\n"
			, exprString, messageString, fileName, lineNumber);
		//
		// Dump the scope info
		//
		x_LogPush(fileName, lineNumber);
		x_LogError("Assert", ">>>>> EXPR:%s MSG:%s ", x_va_list(exprString, messageString));

		//
		// Display menu to the user
		//
		custom_msgbox   msgBox;
		msgBox.mNumButtons = 4;
		msgBox.mButtonName[0] = "Debug";
		msgBox.mButtonName[1] = "Ignore";
		msgBox.mButtonName[2] = "IgnoreAlways";
		msgBox.mButtonName[3] = "CallStack";

		EButtonCmd answer = (EButtonCmd)customMessageBox(report, "!!!ASSERT!!!! - Do you wish to debug?", msgBox);

		if(answer == CMD_BUTTON1)
		{
			return xTRUE;
		}
		if(answer == CMD_BUTTON2)
		{
			return xFALSE;
		}
		if(answer == CMD_BUTTON3) 
		{ 
			x_FlagOn(flags, XDB_FLAG_IGNORE); 
			return xFALSE;
		}
		if(answer == CMD_BUTTON4) 
		{ 
			custom_msgbox   msgBox;

			msgBox.mNumButtons = 2;
			msgBox.mButtonName[0] = "Yes";
			msgBox.mButtonName[1] = "No";

			char* callStack = "callstack not implemented";
			answer = (EButtonCmd)customMessageBox(callStack, "!!!ASSERT!!!! - Do you wish to debug?", msgBox);
			if (answer == CMD_BUTTON1)
			{
				return xTRUE;
			}        
		}

		return xFALSE;
	}

//==============================================================================
// END xCore namespace
//==============================================================================
};

#endif

