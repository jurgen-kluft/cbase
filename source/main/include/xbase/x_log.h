/**
* @file Head file xlog.h
* Logging
*/

#ifndef __XBASE_LOG_H__
#define __XBASE_LOG_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif


/**
 * xCore namespace
 */
namespace xcore
{
	// Forward declares
	class x_va_list;
	class xlog_imp;

	template<typename T>
	class xuchars_t;
	template<typename T>
	class xcuchars_t;

	typedef xuchars_t<char>		xuchars;
	typedef xuchars_t<uchar8>	xuchar8s;
	typedef xuchars_t<uchar32>	xuchar32s;

	typedef xcuchars_t<char>	xcuchars;
	typedef xcuchars_t<uchar8>	xcuchar8s;
	typedef xcuchars_t<uchar32>	xcuchar32s;

	#define X_LOG

	//==============================================================================
	//  LOG
	//==============================================================================
	//
	//==============================================================================

	#ifdef X_LOG

		struct x_log_info
		{
			s32                         mSequenceCount;
			s32                         mLogLineNumber;
			const char*                 mLogFileName;

			typedef x_log_info&			_delegate();
		};

		///< In a multi-threaded environment the user needs to call this to set a delegate that can
		///< return a x_log_info per thread. The default one only works in a single threaded environment.
		extern void				x_SetGetLogInfo(x_log_info::_delegate* Function);

				void		__x_LogMessage		(const char* channel, const char* message);
				void		__x_LogWarning		(const char* channel, const char* message);
				void		__x_LogError  		(const char* channel, const char* message);
				void		__x_LogMessage		(const char* channel, const char* messageFormat, const x_va_list& formatVars);
				void		__x_LogWarning		(const char* channel, const char* messageFormat, const x_va_list& formatVars);
				void		__x_LogError  		(const char* channel, const char* messageFormat, const x_va_list& formatVars);

				void		x_LogPush			(const char* fileName, s32 lineNum);
				void		x_LogFlush			(void);
					
		#define				x_LogMessage		xcore::x_LogPush(__FILE__, __LINE__); xcore::__x_LogMessage
		#define				x_LogWarning		xcore::x_LogPush(__FILE__, __LINE__); xcore::__x_LogWarning
		#define				x_LogError			xcore::x_LogPush(__FILE__, __LINE__); xcore::__x_LogError
	#else
		inline void			x_LogPush			(const char* fileName, s32 lineNum){}
		inline void			x_LogFlush			(void){}

		inline void			__x_LogFake	    	(const char* channel, const char* message){}
		inline void			__x_LogFake 		(const char* channel, const char* messageFormat, const x_va_list& formatVars){}

		#define				x_LogMessage		xcore::__x_LogFake
		#define				x_LogWarning		xcore::__x_LogFake
		#define				x_LogError			xcore::__x_LogFake
	#endif


	class xlog
	{
	public:
		enum ELevel
		{
			LOG_INFO		= 0,
			LOG_WARNING		= 1,
			LOG_ERROR		= 2,
			LOG_EXCEPTION	= 3,
			LOG_COUNT,
		};
	private:
		static xlog_imp*		sLogger[LOG_COUNT];

	public:
		static void				init();
		static void				shutdown();
		static void				flush();
		static void				clear();

		static xlog_imp*		redirect(ELevel inLevel, xlog_imp* inLogger);

		static void 			write(ELevel inLevel, bool _value);
		static void 			write(ELevel inLevel, uchar32 _value);
		static void 			write(ELevel inLevel, s32 _value);
		static void 			write(ELevel inLevel, s64 _value);
		static void 			write(ELevel inLevel, u32 _value);
		static void 			write(ELevel inLevel, u64 _value);
		static void 			write(ELevel inLevel, f32 _value);
		static void 			write(ELevel inLevel, f64 _value);
		static void 			write(ELevel inLevel, const xcuchars& str);
		static void 			write(ELevel inLevel, const xcuchars& format, const x_va_list& arguments);
		static void 			write(ELevel inLevel, const xcuchar32s& str);
		static void 			write(ELevel inLevel, const xcuchar32s& format, const x_va_list& arguments);

		static void 			writeLine(ELevel inLevel);
		static void 			writeLine(ELevel inLevel, bool _value);
		static void 			writeLine(ELevel inLevel, uchar32 _value);
		static void 			writeLine(ELevel inLevel, s32 _value);
		static void 			writeLine(ELevel inLevel, s64 _value);
		static void 			writeLine(ELevel inLevel, u32 _value);
		static void 			writeLine(ELevel inLevel, u64 _value);
		static void 			writeLine(ELevel inLevel, f32 _value);
		static void 			writeLine(ELevel inLevel, f64 _value);
		static void 			writeLine(ELevel inLevel, const xcuchars& str);
		static void 			writeLine(ELevel inLevel, const xcuchars& format, const x_va_list& arguments);
		static void 			writeLine(ELevel inLevel, const xcuchar32s& str);
		static void 			writeLine(ELevel inLevel, const xcuchar32s& format, const x_va_list& arguments);
	};

	/**
	 * Interface class, has specific implementations for different environments/platforms
	 */
	class xlog_imp
	{
		friend class xlog;

		virtual void			initialize() = 0;
		virtual void			flush() = 0;
		virtual void			shutdown() = 0;
		virtual void			clear() = 0;

	public:
		virtual                 ~xlog_imp(){};

		virtual void 			write(bool _value) = 0;
		virtual void 			write(uchar32 _value) = 0;
		virtual void 			write(s32 _value) = 0;
		virtual void 			write(s64 _value) = 0;
		virtual void 			write(u32 _value) = 0;
		virtual void 			write(u64 _value) = 0;
		virtual void 			write(f32 _value) = 0;
		virtual void 			write(f64 _value) = 0;
		virtual void 			write(const xcuchars& str) = 0;
		virtual void 			write(const xcuchars& format, const x_va_list& arguments) = 0;
		virtual void 			write(const xcuchar32s& str) = 0;
		virtual void 			write(const xcuchar32s& format, const x_va_list& arguments) = 0;

		virtual void 			writeLine() = 0;
		virtual void 			writeLine(bool _value) = 0;
		virtual void 			writeLine(uchar32 _value) = 0;
		virtual void 			writeLine(s32 _value) = 0;
		virtual void 			writeLine(s64 _value) = 0;
		virtual void 			writeLine(u32 _value) = 0;
		virtual void 			writeLine(u64 _value) = 0;
		virtual void 			writeLine(f32 _value) = 0;
		virtual void 			writeLine(f64 _value) = 0;
		virtual void 			writeLine(const xcuchars& str) = 0;
		virtual void 			writeLine(const xcuchars& format, const x_va_list& arguments) = 0;
		virtual void 			writeLine(const xcuchar32s& str) = 0;
		virtual void 			writeLine(const xcuchar32s& format, const x_va_list& arguments) = 0;
	};

//==============================================================================
// END xCore namespace
//==============================================================================
};
/**
 *  END xCore namespace
 */

#include "private/x_log_to_console.h"

#endif	///< END __XBASE_LOG_H__
