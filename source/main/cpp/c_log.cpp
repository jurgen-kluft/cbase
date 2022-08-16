#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"

#include "cbase/private/c_log_to_console.h"

namespace ncore
{
	static log_info_t	x_ST_LogInfo;	///< Single-Thread log info
	log_info_t&			x_GetLogInfo_ST()
	{
		return x_ST_LogInfo;
	}

	static log_info_t::_delegate*	sGetLogInfoFunc = x_GetLogInfo_ST;

	void				x_SetGetLogInfo(log_info_t::_delegate* Function)
	{
		sGetLogInfoFunc = Function;
	}

	xlog_to_console_imp	sLogDefaultToConsole;
	logger_t*			log_t::sLogger[log_t::COUNT] = 
	{
		&sLogDefaultToConsole,
		&sLogDefaultToConsole,
		&sLogDefaultToConsole,
		&sLogDefaultToConsole
	};

	logger_t*			log_t::redirect(ELevel inLevel, logger_t* inLogger)
	{
		logger_t* old = log_t::sLogger[inLevel];
		log_t::sLogger[inLevel] = inLogger;
		if (old == &sLogDefaultToConsole)
			return nullptr;
		return old;
	}

	void				log_t::init()
	{
		for (s32 i=0; i<COUNT; ++i)
			sLogger[i] = &sLogDefaultToConsole;
	}

	void				log_t::flush()
	{
		for (s32 i=0; i<COUNT; ++i)
			sLogger[i]->flush();
	}

	void				log_t::shutdown()
	{
		for (s32 i=0; i<COUNT; ++i)
		{
			if (sLogger[i]!=nullptr)
				sLogger[i] = &sLogDefaultToConsole;
		}
	}

	void				log_t::clear()
	{
		for (s32 i=0; i<COUNT; ++i)
			if (sLogger[i]!=nullptr)
				sLogger[i]->clear();
	}

	//-------------------------------------------------------------------------------------------------
	// 
	//-------------------------------------------------------------------------------------------------
	void 				log_t::write(ELevel inLevel, bool _value)
	{
		sLogger[inLevel]->write(_value);
	}


	void 				log_t::write(ELevel inLevel, f64 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(ELevel inLevel, s32 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(ELevel inLevel, s64 _value)
	{
		sLogger[inLevel]->write(_value);
	}


	void 				log_t::write(ELevel inLevel, f32 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(ELevel inLevel, u32 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(ELevel inLevel, u64 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(ELevel inLevel, const crunes_t& str)
	{
		sLogger[inLevel]->write(str);
	}

	void 				log_t::write(ELevel inLevel, const crunes_t& format, const va_list_t& arguments)
	{
		sLogger[inLevel]->write(format, arguments);
	}

	void 				log_t::writeLine(ELevel inLevel)
	{
		sLogger[inLevel]->writeLine();
	}

	void 				log_t::writeLine(ELevel inLevel, bool _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}


	void 				log_t::writeLine(ELevel inLevel, f64 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(ELevel inLevel, s32 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(ELevel inLevel, s64 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}


	void 				log_t::writeLine(ELevel inLevel, f32 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(ELevel inLevel, u32 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(ELevel inLevel, u64 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(ELevel inLevel, const crunes_t& str)
	{
		sLogger[inLevel]->writeLine(str);
	}

	void 				log_t::writeLine(ELevel inLevel, const crunes_t& format, const va_list_t& arguments)
	{
		sLogger[inLevel]->writeLine(format, arguments);
	}

	
	#ifdef D_LOG

	//==============================================================================
	//==============================================================================
	//==============================================================================
	// LOG FUNCTIONS
	//==============================================================================
	//==============================================================================
	//==============================================================================

    /**
     *------------------------------------------------------------------------------
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
	void __x_LogMessage(const char* channel, const char* msgFormat, const va_list_t& args)
	{
		log_info_t& info = sGetLogInfoFunc();

		const char* logformat = "%s(%d) : SEQUENCE:%d TYPE:Info CHANNEL:%s";
		ascii::crunes_t logformatrunes((ascii::pcrune)logformat, ascii::strlen(logformat));
		ascii::crunes_t msgformatrunes((ascii::pcrune)msgFormat, ascii::strlen(msgFormat));

		log_t::writeLine(log_t::INFO, logformatrunes, va_list_t(va_t(info.mLogFileName), va_t(info.mLogLineNumber), va_t(info.mSequenceCount++), va_t(channel)));
		if (args.length() > 0)
			log_t::writeLine(log_t::INFO, msgformatrunes, args);
		else
			log_t::writeLine(log_t::INFO, msgformatrunes);
	}

	void __x_LogMessage(const char* channel, const char* message)
	{
		__x_LogMessage(channel, message, va_list_t());
	}

	/**
     *------------------------------------------------------------------------------
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
	void __x_LogWarning(const char* channel, const char* msgFormat, const va_list_t& args)
	{
		log_info_t& info = sGetLogInfoFunc();

		const char* logformat = "%s(%d) : SEQUENCE:%d TYPE:Warning CHANNEL:%s";
		ascii::crunes_t logformatrunes((ascii::pcrune)logformat, ascii::strlen(logformat));
		ascii::crunes_t msgformatrunes((ascii::pcrune)msgFormat, ascii::strlen(msgFormat));

		log_t::writeLine(log_t::WARNING, logformatrunes, va_list_t(va_t(info.mLogFileName), va_t(info.mLogLineNumber), va_t(info.mSequenceCount++), va_t(channel)));
		if (args.length() > 0)
			log_t::writeLine(log_t::WARNING, msgformatrunes, args);
		else
			log_t::writeLine(log_t::WARNING, msgformatrunes);
	}

	void __x_LogWarning(const char* channel, const char* message)
	{
		__x_LogWarning(channel, message, va_list_t());
	}

    /**
     *------------------------------------------------------------------------------
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
	void __x_LogError(const char* channel, const char* msgFormat, const va_list_t& args)
	{
		log_info_t& info = sGetLogInfoFunc();

		const char* logformat = "%s(%d) : SEQUENCE:%d TYPE:Info CHANNEL:%s";
		ascii::crunes_t logformatrunes((ascii::pcrune)logformat, ascii::strlen(logformat));
		ascii::crunes_t msgformatrunes((ascii::pcrune)msgFormat, ascii::strlen(msgFormat));

		log_t::writeLine(log_t::ERROR, logformatrunes, va_list_t(va_t(info.mLogFileName), va_t(info.mLogLineNumber), va_t(info.mSequenceCount++), va_t(channel)));
		if (args.length() > 0)
			log_t::writeLine(log_t::ERROR, msgformatrunes, args);
		else
			log_t::writeLine(log_t::ERROR, msgformatrunes);
	}

	void __x_LogError(const char* channel, const char* message)
	{
		__x_LogError(channel, message, va_list_t());
	}

	//------------------------------------------------------------------------------

	void x_LogFlush(void)
	{
		log_t::flush();
	}

    /**
     *------------------------------------------------------------------------------
	 * Summary:
	 *     Log flush into debug local.
	 * Arguments:
	 *     fileName        - FileName to log.
	 *     lineNum         - Line number to log.
	 * Returns:
	 *     void
	 * Description:
	 *     The function log fileName and lineNum into current log_info_t.
	 *------------------------------------------------------------------------------
	 */
	void x_LogPush(const char* fileName, s32 lineNum)
	{
		log_info_t& info = sGetLogInfoFunc();
		info.mLogLineNumber = lineNum;
		info.mLogFileName  = fileName;
	}

	#endif
};

