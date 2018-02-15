/**
* @file Head file x_log_to_console.h
*
* Logging to xconsole.
*/

#ifndef __XBASE_LOG_TO_CONSOLE_H__
#define __XBASE_LOG_TO_CONSOLE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_log.h"

/**
 * xCore namespace
 */
namespace xcore
{
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

	class xlog_to_console_imp : public xlog_imp
	{
		virtual void			initialize();
		virtual void			flush();
		virtual void			shutdown();

	public:
		virtual void			clear();

		virtual void 			write(bool value);
		virtual void 			write(f64 value);
		virtual void 			write(s32 value);
		virtual void 			write(s64 value);
		virtual void 			write(f32 value);
		virtual void 			write(u32 value);
		virtual void 			write(u64 value);
		virtual void 			write(const xcuchars& str);
		virtual void 			write(const xcuchars& format, const x_va_list& arguments);
		virtual void 			write(const xcuchar32s& str);
		virtual void 			write(const xcuchar32s& format, const x_va_list& arguments);

		virtual void 			writeLine();
		virtual void 			writeLine(bool value);
		virtual void 			writeLine(f64 value);
		virtual void 			writeLine(s32 value);
		virtual void 			writeLine(s64 value);
		virtual void 			writeLine(f32 value);
		virtual void 			writeLine(u32 value);
		virtual void 			writeLine(u64 value);
		virtual void 			writeLine(const xcuchars& str);
		virtual void 			writeLine(const xcuchars& format, const x_va_list& arguments);
		virtual void 			writeLine(const xcuchar32s& str);
		virtual void 			writeLine(const xcuchar32s& format, const x_va_list& arguments);
	};
};
/**
 *  END xCore namespace
 */

#endif	///< __XBASE_LOG_TO_CONSOLE_H__
