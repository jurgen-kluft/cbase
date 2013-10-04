/**
* @file xlog.cpp
* Logging
*/

#ifndef SPU

#include "xbase\x_log.h"
#include "xbase\x_va_list.h"
#include "xbase\x_string_std.h"

#include "xbase\private\x_log_to_console.h"

/**
 * xCore namespace
 */
namespace xcore
{
	static x_log_info	x_ST_LogInfo;	///< Single-Thread log info
	x_log_info&			x_GetLogInfo_ST()
	{
		return x_ST_LogInfo;
	}

	static x_log_info::_delegate*	sGetLogInfoFunc = x_GetLogInfo_ST;

	void				x_SetGetLogInfo(x_log_info::_delegate* Function)
	{
		sGetLogInfoFunc = Function;
	}

	xlog_to_console_imp	sLogDefaultToConsole;
	xlog_imp*			xlog::sLogger[xlog::LOG_COUNT] = 
	{
		&sLogDefaultToConsole,
		&sLogDefaultToConsole,
		&sLogDefaultToConsole,
		&sLogDefaultToConsole
	};

	xlog_imp*			xlog::redirect(ELevel inLevel, xlog_imp* inLogger)
	{
		xlog_imp* old = xlog::sLogger[inLevel];
		xlog::sLogger[inLevel] = inLogger;
		if (old == &sLogDefaultToConsole)
			return NULL;
		return old;
	}

	void				xlog::init()
	{
		for (s32 i=0; i<LOG_COUNT; ++i)
			sLogger[i] = &sLogDefaultToConsole;
	}

	void				xlog::flush()
	{
		for (s32 i=0; i<LOG_COUNT; ++i)
			sLogger[i]->flush();
	}

	void				xlog::shutdown()
	{
		for (s32 i=0; i<LOG_COUNT; ++i)
		{
			if (sLogger[i]!=NULL)
				sLogger[i] = &sLogDefaultToConsole;
		}
	}

	void				xlog::clear()
	{
		for (s32 i=0; i<LOG_COUNT; ++i)
			if (sLogger[i]!=NULL)
				sLogger[i]->clear();
	}

	//-------------------------------------------------------------------------------------------------
	// 
	//-------------------------------------------------------------------------------------------------
	void 				xlog::write(ELevel inLevel, bool _value)
	{
		sLogger[inLevel]->write(_value);
	}


	void 				xlog::write(ELevel inLevel, f64 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				xlog::write(ELevel inLevel, s32 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				xlog::write(ELevel inLevel, s64 _value)
	{
		sLogger[inLevel]->write(_value);
	}


	void 				xlog::write(ELevel inLevel, f32 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				xlog::write(ELevel inLevel, u32 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				xlog::write(ELevel inLevel, u64 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				xlog::write(ELevel inLevel, char _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				xlog::write(ELevel inLevel, const char* buffer)
	{
		sLogger[inLevel]->write(buffer);
	}

	void 				xlog::write(ELevel inLevel, const char* buffer, s32 index, s32 count)
	{
		sLogger[inLevel]->write(buffer, index, count);
	}

	void 				xlog::write(ELevel inLevel, const char* format, const x_va_list& arguments)
	{
		sLogger[inLevel]->write(format, arguments);
	}


	void 				xlog::writeLine(ELevel inLevel)
	{
		sLogger[inLevel]->writeLine();
	}

	void 				xlog::writeLine(ELevel inLevel, bool _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}


	void 				xlog::writeLine(ELevel inLevel, f64 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				xlog::writeLine(ELevel inLevel, s32 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				xlog::writeLine(ELevel inLevel, s64 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}


	void 				xlog::writeLine(ELevel inLevel, f32 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				xlog::writeLine(ELevel inLevel, u32 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				xlog::writeLine(ELevel inLevel, u64 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				xlog::writeLine(ELevel inLevel, char _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				xlog::writeLine(ELevel inLevel, const char* buffer)
	{
		sLogger[inLevel]->writeLine(buffer);
	}

	void 				xlog::writeLine(ELevel inLevel, const char* buffer, s32 index, s32 count)
	{
		sLogger[inLevel]->writeLine(buffer, index, count);
	}

	void 				xlog::writeLine(ELevel inLevel, const char* format, const x_va_list& arguments)
	{
		sLogger[inLevel]->writeLine(format, arguments);
	}


	void 				xlog_imp::write(const char* buffer)									
	{ 
		write(buffer); 
	}

	void 				xlog_imp::writeLine(const char* buffer)
	{
		writeLine(buffer); 
	}

	void 				xlog_imp::write(const char* buffer, s32 index, s32 count)	
	{ 
		write(buffer, index, count);
	}

	void 				xlog_imp::writeLine(const char* buffer, s32 index, s32 count)	
	{ 
		writeLine(buffer, index, count); 
	}

	void 				xlog_imp::write(const char* buffer, const x_va_list& arguments)	
	{ 
		write(buffer, arguments); 
	}

	void 				xlog_imp::writeLine(const char* buffer, const x_va_list& arguments)
	{ 
		writeLine(buffer, arguments); 
	}

	
	#ifdef X_LOG

	//==============================================================================
	//==============================================================================
	//==============================================================================
	// LOG FUNCTIONS
	//==============================================================================
	//==============================================================================
	//==============================================================================

    /**
     *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     Output log message.
	 * Arguments:
	 *     channel        - Message channel. This is only a string to output. 
	 *                      No affect for function.
	 *     messageFormat  - Message format. See format for x_sprintf() for detail.
	 *     args           - The arguments output consists with message format.
	 * Returns:
	 *     void
	 * Description:
	 *     The function outputs log message. It works like x_printf except:
	 *     (a)The output goes to the debugger if possible.
	 *     (b)It only works when X_DEBUG is defined.
	 *     (c)Add necessary information like file name before message.
	 * See Also:
	 *     __x_LogWarning, __x_LogError
	 *------------------------------------------------------------------------------
	 */
	void __x_LogMessage(const char* channel, const char* messageFormat, const x_va_list& args)
	{
		x_log_info& info = sGetLogInfoFunc();

		xlog::writeLine(xlog::LOG_INFO, "%s(%d) : SEQUENCE:%d TYPE:Info CHANNEL:%s", x_va_list(x_va(info.mLogFileName), x_va(info.mLogLineNumber), x_va(info.mSequenceCount++), x_va(channel)));
		if (args.length() > 0)
			xlog::writeLine(xlog::LOG_INFO, messageFormat, args);
		else
			xlog::writeLine(xlog::LOG_INFO, messageFormat);
	}

	void __x_LogMessage(const char* channel, const char* message)
	{
		__x_LogMessage(channel, message, x_va_list());
	}

	/**
     *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     Output log warning.
	 * Arguments:
	 *     channel        - Warning channel. This is only a string to output. 
	 *                      No affect for function.
	 *     messageFormat  - Warning format. See format for x_sprintf() for detail.
	 *     args           - The arguments output consists with warning format.
	 * Returns:
	 *     void
	 * Description:
	 *     The function outputs log warning. It works like x_printf except:
	 *     (a)The output goes to the debugger if possible.
	 *     (b)It only works when X_DEBUG is defined.
	 *     (c)Add necessary information like file name before warning.
	 * See Also:
	 *     __x_LogError, __x_LogMessage
	 *------------------------------------------------------------------------------
	 */
	void __x_LogWarning(const char* channel, const char* messageFormat, const x_va_list& args)
	{
		x_log_info& info = sGetLogInfoFunc();

		xlog::writeLine(xlog::LOG_WARNING, "%s(%d) : SEQUENCE:%d TYPE:Warning CHANNEL:%s", x_va_list(x_va(info.mLogFileName), x_va(info.mLogLineNumber), x_va(info.mSequenceCount++), x_va(channel)));
		if (args.length() > 0)
			xlog::writeLine(xlog::LOG_WARNING, messageFormat, args);
		else
			xlog::writeLine(xlog::LOG_WARNING, messageFormat);
	}

	void __x_LogWarning(const char* channel, const char* message)
	{
		__x_LogWarning(channel, message, x_va_list());
	}

    /**
     *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     Output log error.
	 * Arguments:
	 *     channel        - Error channel. This is only a string to output. 
	 *                      No affect for function.
	 *     messageFormat  - Error format. See format for x_sprintf() for detail.
	 *     args           - The arguments output consists with error format.
	 * Returns:
	 *     void
	 * Description:
	 *     The function outputs log error. It works like x_printf except:
	 *     (a)The output goes to the debugger if possible.
	 *     (b)It only works when X_DEBUG is defined.
	 *     (c)Add necessary information like file name before error.
	 * See Also:
	 *     __x_LogWarning, __x_LogMessage
	 *------------------------------------------------------------------------------
	 */
	void __x_LogError(const char* channel, const char* messageFormat, const x_va_list& args)
	{
		x_log_info& info = sGetLogInfoFunc();

		xlog::writeLine(xlog::LOG_ERROR, "%s(%d) : SEQUENCE:%d TYPE:Error CHANNEL:%s", x_va_list(x_va(info.mLogFileName), x_va(info.mLogLineNumber), x_va(info.mSequenceCount++), x_va(channel)));
		if (args.length() > 0)
			xlog::writeLine(xlog::LOG_ERROR, messageFormat, args);
		else
			xlog::writeLine(xlog::LOG_ERROR, messageFormat);
	}

	void __x_LogError(const char* channel, const char* message)
	{
		__x_LogError(channel, message, x_va_list());
	}

	//------------------------------------------------------------------------------

	void x_LogFlush(void)
	{
		xlog::flush();
	}

    /**
     *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     Log flush into debug local.
	 * Arguments:
	 *     fileName        - FileName to log.
	 *     lineNum         - Line number to log.
	 * Returns:
	 *     void
	 * Description:
	 *     The function log fileName and lineNum into current x_log_info.
	 *------------------------------------------------------------------------------
	 */
	void x_LogPush(const char* fileName, s32 lineNum)
	{
		x_log_info& info = sGetLogInfoFunc();
		info.mLogLineNumber = lineNum;
		info.mLogFileName  = fileName;
	}

	#endif
};
/**
 *  END xCore namespace
 */


#endif