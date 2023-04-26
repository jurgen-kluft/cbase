#include "cbase/c_debug.h"
#include "cbase/c_memory.h"
#include "cbase/c_va_list.h"
#include "cbase/c_console.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"

namespace ncore
{
#define D_CONSOLE_LOCAL_STR_BUF(type, local_var_name, size) \
    type::rune local_var_name##Buffer[size];               \
    runes_t    local_var_name(local_var_name##Buffer, 0, 0, (u32)size, type::TYPE);

    class xconsole_null : public console_t
    {
    public:
        xconsole_null() {}
        virtual ~xconsole_null() {}

        virtual void initialize();
        virtual void shutdown();

        virtual s32 setColor(console_t::EColor color);

        virtual void write(bool _value);
        virtual void write(f64 _value);
        virtual void write(s32 _value);
        virtual void write(s64 _value);
        virtual void write(f32 _value);

        virtual void write(u32 _value);
        virtual void write(u64 _value);

        virtual void write(const crunes_t& str);
        virtual void write(const crunes_t& fmt, const va_list_t& args);

        virtual void writeLine();
    };

    void xconsole_null::initialize() {}
    void xconsole_null::shutdown() {}
    s32  xconsole_null::setColor(console_t::EColor color) { return 0; }
    void xconsole_null::write(bool _value) {}
    void xconsole_null::write(f64 _value) {}
    void xconsole_null::write(s32 _value) {}
    void xconsole_null::write(s64 _value) {}
    void xconsole_null::write(f32 _value) {}
    void xconsole_null::write(u32 _value) {}
    void xconsole_null::write(u64 _value) {}
    void xconsole_null::write(const crunes_t& str) {}
    void xconsole_null::write(const crunes_t& fmt, const va_list_t& args) {}
    void xconsole_null::writeLine() {}

    static xconsole_null sNullConsole;

    class xconsole_default : public console_t
    {
        console_t::out_t* mOut;

    public:
        xconsole_default(console_t::out_t* _out) : mOut(_out) {}

        virtual ~xconsole_default() {}

        virtual void initialize();
        virtual void shutdown();

        virtual s32 setColor(console_t::EColor color);

        virtual void write(bool _value);
        virtual void write(f64 _value);
        virtual void write(s32 _value);
        virtual void write(s64 _value);
        virtual void write(f32 _value);

        virtual void write(u32 _value);
        virtual void write(u64 _value);

        virtual void write(const crunes_t& str);
        virtual void write(const crunes_t& fmt, const va_list_t& args);

        virtual void writeLine();
    };

    void xconsole_default::initialize() {}

    void xconsole_default::shutdown() {}

    s32 xconsole_default::setColor(console_t::EColor color) { return mOut->color(color); }

    static const char* trueStr = "true";
    static const char* falseStr = "false";
    void xconsole_default::write(bool _value)
    {
        crunes_t truestr(trueStr, trueStr+4);
        crunes_t falsestr(falseStr, falseStr+5);
        write(_value ? truestr : falsestr);
    }

    void xconsole_default::write(f64 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(s32 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 64);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(s64 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 64);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(f32 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(u32 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(u64 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(const crunes_t& str)
    {
        mOut->write(str);
    }

    void xconsole_default::write(const crunes_t& fmt, const va_list_t& args)
    {
        mOut->write(fmt, args);
    }

    void xconsole_default::writeLine() { mOut->writeln(); }

    extern console_t::out_t* gGetDefaultConsoleOut();
    console_t*              console = nullptr;

    void console_t::init_default_console()
    {
        static xconsole_default sDefaultConsole(gGetDefaultConsoleOut());
        console = &sDefaultConsole;
    }

}; // namespace ncore
