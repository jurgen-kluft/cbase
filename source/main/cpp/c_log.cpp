#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"

#include "cbase/private/c_log_to_console.h"

namespace ncore
{
	static log_info_t	g_ST_LogInfo;	///< Single-Thread log info
	log_info_t&			g_GetLogInfo_ST()
	{
		return g_ST_LogInfo;
	}

	static log_info_t::_delegate*	sGetLogInfoFunc = g_GetLogInfo_ST;

	void				g_SetGetLogInfo(log_info_t::_delegate* Function)
	{
		sGetLogInfoFunc = Function;
	}

	log_to_console_imp	sLogDefaultToConsole;
	logger_t*			log_t::sLogger[log_t::COUNT] =
	{
		&sLogDefaultToConsole,
		&sLogDefaultToConsole,
		&sLogDefaultToConsole,
		&sLogDefaultToConsole
	};

	logger_t*			log_t::redirect(elevel inLevel, logger_t* inLogger)
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
	void 				log_t::write(elevel inLevel, bool _value)
	{
		sLogger[inLevel]->write(_value);
	}


	void 				log_t::write(elevel inLevel, f64 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(elevel inLevel, s32 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(elevel inLevel, s64 _value)
	{
		sLogger[inLevel]->write(_value);
	}


	void 				log_t::write(elevel inLevel, f32 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(elevel inLevel, u32 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(elevel inLevel, u64 _value)
	{
		sLogger[inLevel]->write(_value);
	}

	void 				log_t::write(elevel inLevel, const crunes_t& str)
	{
		sLogger[inLevel]->write(str);
	}

	void 				log_t::write(elevel inLevel, const crunes_t& format, const va_t* argv, s32 argc)
	{
		sLogger[inLevel]->write(format, argv, argc);
	}

	void 				log_t::writeLine(elevel inLevel)
	{
		sLogger[inLevel]->writeLine();
	}

	void 				log_t::writeLine(elevel inLevel, bool _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}


	void 				log_t::writeLine(elevel inLevel, f64 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(elevel inLevel, s32 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(elevel inLevel, s64 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}


	void 				log_t::writeLine(elevel inLevel, f32 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(elevel inLevel, u32 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(elevel inLevel, u64 _value)
	{
		sLogger[inLevel]->writeLine(_value);
	}

	void 				log_t::writeLine(elevel inLevel, const crunes_t& str)
	{
		sLogger[inLevel]->writeLine(str);
	}

	void 				log_t::writeLine(elevel inLevel, const crunes_t& format, const va_t* argv, s32 argc)
	{
		sLogger[inLevel]->writeLine(format, argv, argc);
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
	 *     (b)It only works when D_DEBUG is defined.
	 *     (c)Add necessary information like file name before message.
	 * See Also:
	 *     __x_LogWarning, __x_LogError
	 *------------------------------------------------------------------------------
	 */
	void __LogMessage2(const char* channel, const char* msgFormat, const va_t* argv, s32 argc)
	{
		log_info_t& info = sGetLogInfoFunc();

		const char* logformat = "%s(%d) : SEQUENCE:%d TYPE:Info CHANNEL:%s";
		crunes_t logformatrunes((ascii::pcrune)logformat, (u32)ascii::strlen(logformat));
		crunes_t msgformatrunes((ascii::pcrune)msgFormat, (u32)ascii::strlen(msgFormat));

		log_t::writeLine(log_t::INFO, logformatrunes, va_t(info.mLogFileName), va_t(info.mLogLineNumber), va_t(info.mSequenceCount++), va_t(channel));
		if (argc > 0)
			log_t::writeLine(log_t::INFO, msgformatrunes, argv, argc);
		else
			log_t::writeLine(log_t::INFO, msgformatrunes);
	}

	void __LogMessage(const char* channel, const char* message)
	{
		const va_t argv[] = { va_t(0) };
		__LogMessage2(channel, message, argv, 0);
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
	 *     (b)It only works when D_DEBUG is defined.
	 *     (c)Add necessary information like file name before warning.
	 * See Also:
	 *     __x_LogError, __x_LogMessage
	 *------------------------------------------------------------------------------
	 */
	void __LogWarning2(const char* channel, const char* msgFormat, const va_t* argv, s32 argc)
	{
		log_info_t& info = sGetLogInfoFunc();

		const char* logformat = "%s(%d) : SEQUENCE:%d TYPE:Warning CHANNEL:%s";
		crunes_t logformatrunes((ascii::pcrune)logformat, (u32)ascii::strlen(logformat));
		crunes_t msgformatrunes((ascii::pcrune)msgFormat, (u32)ascii::strlen(msgFormat));

		log_t::writeLine(log_t::WARNING, logformatrunes, va_t(info.mLogFileName), va_t(info.mLogLineNumber), va_t(info.mSequenceCount++), va_t(channel));
		if (argc > 0)
			log_t::writeLine(log_t::WARNING, msgformatrunes, argv, argc);
		else
			log_t::writeLine(log_t::WARNING, msgformatrunes);
	}

	void __LogWarning(const char* channel, const char* message)
	{
		const va_t argv[] = { va_t(0) };
		__LogWarning2(channel, message, argv, 0);
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
	 *     (b)It only works when D_DEBUG is defined.
	 *     (c)Add necessary information like file name before error.
	 * See Also:
	 *     __x_LogWarning, __x_LogMessage
	 *------------------------------------------------------------------------------
	 */
	void __LogError2(const char* channel, const char* msgFormat, const va_t* argv, s32 argc)
	{
		log_info_t& info = sGetLogInfoFunc();

		const char* logformat = "%s(%d) : SEQUENCE:%d TYPE:Info CHANNEL:%s";
		crunes_t logformatrunes((ascii::pcrune)logformat, (u32)ascii::strlen(logformat));
		crunes_t msgformatrunes((ascii::pcrune)msgFormat, (u32)ascii::strlen(msgFormat));

		log_t::writeLine(log_t::ERROR, logformatrunes, va_t(info.mLogFileName), va_t(info.mLogLineNumber), va_t(info.mSequenceCount++), va_t(channel));
		if (argc > 0)
			log_t::writeLine(log_t::ERROR, msgformatrunes, argv, argc);
		else
			log_t::writeLine(log_t::ERROR, msgformatrunes);
	}

	void __LogError(const char* channel, const char* message)
	{
		const va_t argv[] = { va_t(0) };
		__LogError2(channel, message, argv, 0);
	}

	//------------------------------------------------------------------------------

	void __LogFlush(void)
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
	void __LogPush(const char* fileName, s32 lineNum)
	{
		log_info_t& info = sGetLogInfoFunc();
		info.mLogLineNumber = lineNum;
		info.mLogFileName  = fileName;
	}

	#endif
};

