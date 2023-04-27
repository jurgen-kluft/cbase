#ifndef __CBASE_LOG_TO_CONSOLE_H__
#define __CBASE_LOG_TO_CONSOLE_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_log.h"
#include "cbase/c_runes.h"

namespace ncore
{
	class log_to_console_imp : public logger_t
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
		virtual void 			write(const crunes_t& str);
		virtual void 			write(const crunes_t& format, const va_list_t& arguments);

		virtual void 			writeLine();
		virtual void 			writeLine(bool value);
		virtual void 			writeLine(f64 value);
		virtual void 			writeLine(s32 value);
		virtual void 			writeLine(s64 value);
		virtual void 			writeLine(f32 value);
		virtual void 			writeLine(u32 value);
		virtual void 			writeLine(u64 value);
		virtual void 			writeLine(const crunes_t& str);
		virtual void 			writeLine(const crunes_t& format, const va_list_t& arguments);
	};
};

#endif	///< __CBASE_LOG_TO_CONSOLE_H__
