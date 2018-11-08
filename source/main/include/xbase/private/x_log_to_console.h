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
#include "xbase/x_runes.h"

/**
 * xCore namespace
 */
namespace xcore
{

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
		virtual void 			write(const ascii::crunes& str);
		virtual void 			write(const ascii::crunes& format, const x_va_list& arguments);
		virtual void 			write(const utf32::crunes& str);
		virtual void 			write(const utf32::crunes& format, const x_va_list& arguments);

		virtual void 			writeLine();
		virtual void 			writeLine(bool value);
		virtual void 			writeLine(f64 value);
		virtual void 			writeLine(s32 value);
		virtual void 			writeLine(s64 value);
		virtual void 			writeLine(f32 value);
		virtual void 			writeLine(u32 value);
		virtual void 			writeLine(u64 value);
		virtual void 			writeLine(const ascii::crunes& str);
		virtual void 			writeLine(const ascii::crunes& format, const x_va_list& arguments);
		virtual void 			writeLine(const utf32::crunes& str);
		virtual void 			writeLine(const utf32::crunes& format, const x_va_list& arguments);
	};
};
/**
 *  END xCore namespace
 */

#endif	///< __XBASE_LOG_TO_CONSOLE_H__
