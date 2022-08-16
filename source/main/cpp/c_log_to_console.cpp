#include "cbase/c_debug.h"
#include "cbase/c_runes.h"
#include "cbase/c_log.h"
#include "cbase/c_console.h"

namespace ncore
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
		console->write(value);
	}

	void 				xlog_to_console_imp::write(f64 value)
	{
		console->write(value);
	}

	void 				xlog_to_console_imp::write(s32 value)
	{
		console->write(value);
	}

	void 				xlog_to_console_imp::write(s64 value)
	{
		console->write(value);
	}

	void 				xlog_to_console_imp::write(f32 value)
	{
		console->write(value);
	}

	void 				xlog_to_console_imp::write(u32 value)
	{
		console->write(value);
	}

	void 				xlog_to_console_imp::write(u64 value)
	{
		console->write(value);
	}

	void 				xlog_to_console_imp::write(const crunes_t& str)
	{
		console->write(str);
	}

	void 				xlog_to_console_imp::write(const crunes_t& format, const va_list_t& arguments)
	{
		console->write(format, arguments);
	}

	void 				xlog_to_console_imp::writeLine()
	{
		console->writeLine();
	}

	void 				xlog_to_console_imp::writeLine(bool value)
	{
		console->writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(f64 value)
	{
		console->writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(s32 value)
	{
		console->writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(s64 value)
	{
		console->writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(f32 value)
	{
		console->writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(u32 value)
	{
		console->writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(u64 value)
	{
		console->writeLine(value);
	}

	void 				xlog_to_console_imp::writeLine(const crunes_t& str)
	{
		console->writeLine(str);
	}

	void 				xlog_to_console_imp::writeLine(const crunes_t& format, const va_list_t& arguments)
	{
		console->writeLine(format, arguments);
	}

};
