// x_log_to_console.cpp - xCore logging to xconsole
#include "xbase\x_types.h"
#include "xbase\x_debug.h"
#include "xbase\x_string_std.h"
#include "xbase\x_log.h"
#include "xbase\x_console.h"


//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	void				xlog_to_console_imp::initialize()
	{
	}

	void				xlog_to_console_imp::flush()
	{

	}

	void				xlog_to_console_imp::shutdown()
	{
	}

	void				xlog_to_console_imp::clear()
	{
	}

	void 				xlog_to_console_imp::write(bool value)
	{
		xconsole::write(value);
	}

	void 				xlog_to_console_imp::write(f64 value)
	{
		xconsole::write(value);
	}

	void 				xlog_to_console_imp::write(s32 value)
	{
		xconsole::write(value);
	}

	void 				xlog_to_console_imp::write(s64 value)
	{
		xconsole::write(value);
	}

	void 				xlog_to_console_imp::write(f32 value)
	{
		xconsole::write(value);
	}

	void 				xlog_to_console_imp::write(u32 value)
	{
		xconsole::write(value);
	}

	void 				xlog_to_console_imp::write(u64 value)
	{
		xconsole::write(value);
	}

	void 				xlog_to_console_imp::write(char value)
	{
		xconsole::write(value);
	}

	void 				xlog_to_console_imp::write(const char* buffer)
	{
		xconsole::write(buffer);
	}

	void 				xlog_to_console_imp::write(const char* buffer, s32 index, s32 count)
	{
		xconsole::write(buffer, index, count);
	}

	void 				xlog_to_console_imp::write(const char* format, const x_va_list& arguments)
	{
		xconsole::write(format, arguments);
	}

	void 				xlog_to_console_imp::writeLine()
	{
		xconsole::writeLine();
	}

	void 				xlog_to_console_imp::writeLine(bool value)
	{
		xconsole::writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(f64 value)
	{
		xconsole::writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(s32 value)
	{
		xconsole::writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(s64 value)
	{
		xconsole::writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(f32 value)
	{
		xconsole::writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(u32 value)
	{
		xconsole::writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(u64 value)
	{
		xconsole::writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(char value)
	{
		xconsole::writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(const char* buffer)
	{
		xconsole::writeLine(buffer);
	}

	void 				xlog_to_console_imp::writeLine(const char* buffer, s32 index, s32 count)
	{
		xconsole::writeLine(buffer, index, count);
	}

	void 				xlog_to_console_imp::writeLine(const char* format, const x_va_list& arguments)
	{
		xconsole::writeLine(format, arguments);
	}


//==============================================================================
// END xCore namespace
//==============================================================================
};
