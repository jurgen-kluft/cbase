#include "ccore/c_debug.h"
#include "cbase/c_runes.h"
#include "cbase/c_log.h"
#include "cbase/c_console.h"

namespace ncore
{
    void log_to_console_imp::initialize() { console->initialize(); }
    void log_to_console_imp::flush() {}
    void log_to_console_imp::shutdown() { console->shutdown(); }
    void log_to_console_imp::clear() {}
    void log_to_console_imp::write(bool value) { console->write(value); }
    void log_to_console_imp::write(f64 value) { console->write(value); }
    void log_to_console_imp::write(s32 value) { console->write(value); }
    void log_to_console_imp::write(s64 value) { console->write(value); }
    void log_to_console_imp::write(f32 value) { console->write(value); }
    void log_to_console_imp::write(u32 value) { console->write(value); }
    void log_to_console_imp::write(u64 value) { console->write(value); }
    void log_to_console_imp::write(const crunes_t& str) { console->write(str); }
    void log_to_console_imp::write(const crunes_t& format, const va_t* argv, s32 argc) { console->write(format, argv, argc); }
    void log_to_console_imp::writeLine() { console->writeLine(); }
    void log_to_console_imp::writeLine(bool value) { console->writeLine(value); }
    void log_to_console_imp::writeLine(f64 value) { console->writeLine(value); }
    void log_to_console_imp::writeLine(s32 value) { console->writeLine(value); }
    void log_to_console_imp::writeLine(s64 value) { console->writeLine(value); }
    void log_to_console_imp::writeLine(f32 value) { console->writeLine(value); }
    void log_to_console_imp::writeLine(u32 value) { console->writeLine(value); }
    void log_to_console_imp::writeLine(u64 value) { console->writeLine(value); }
    void log_to_console_imp::writeLine(const crunes_t& str) { console->writeLine(str); }
    void log_to_console_imp::writeLine(const crunes_t& format, const va_t* argv, s32 argc) { console->writeLine(format, argv, argc); }

}; // namespace ncore
