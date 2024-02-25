#include "ccore/c_debug.h"
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

    class console_null : public console_t
    {
    public:
        console_null() {}
        virtual ~console_null() {}

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
        virtual void write(const crunes_t& fmt, const va_t* args, s32 argc);

        virtual void writeLine();
    };

    void console_null::initialize() {}
    void console_null::shutdown() {}
    s32  console_null::setColor(console_t::EColor color) { return 0; }
    void console_null::write(bool _value) {}
    void console_null::write(f64 _value) {}
    void console_null::write(s32 _value) {}
    void console_null::write(s64 _value) {}
    void console_null::write(f32 _value) {}
    void console_null::write(u32 _value) {}
    void console_null::write(u64 _value) {}
    void console_null::write(const crunes_t& str) {}
    void console_null::write(const crunes_t& fmt, const va_t* args, s32 argc) {}
    void console_null::writeLine() {}

    static console_null sNullConsole;

    class console_default : public console_t
    {
        console_t::out_t* mOut;

    public:
        console_default(console_t::out_t* _out) : mOut(_out) {}

        virtual ~console_default() {}

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
        virtual void write(const crunes_t& fmt, const va_t* args, s32 argc);

        virtual void writeLine();
    };

    void console_default::initialize() {}

    void console_default::shutdown() {}

    s32 console_default::setColor(console_t::EColor color) { return mOut->color(color); }

    static const char* trueStr = "true";
    static const char* falseStr = "false";
    void console_default::write(bool _value)
    {
        crunes_t truestr(trueStr, trueStr+4);
        crunes_t falsestr(falseStr, falseStr+5);
        write(_value ? truestr : falsestr);
    }

    void console_default::write(f64 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        nrunes::to_string(tmp, _value, 2);
        write(tmp);
    }

    void console_default::write(s32 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 64);
        nrunes::to_string(tmp, _value, 2);
        write(tmp);
    }

    void console_default::write(s64 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 64);
        nrunes::to_string(tmp, _value, 2);
        write(tmp);
    }

    void console_default::write(f32 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        nrunes::to_string(tmp, _value, 2);
        write(tmp);
    }

    void console_default::write(u32 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        nrunes::to_string(tmp, _value, 2);
        write(tmp);
    }

    void console_default::write(u64 _value)
    {
        D_CONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        nrunes::to_string(tmp, _value, 2);
        write(tmp);
    }

    void console_default::write(const crunes_t& str)
    {
        mOut->write(str);
    }

    void console_default::write(const crunes_t& fmt, const va_t* args, s32 argc)
    {
        mOut->write(fmt, args, argc);
    }

    void console_default::writeLine() { mOut->writeln(); }

    extern console_t::out_t* gGetDefaultConsoleOut();
    console_t*              console = nullptr;

    void console_t::init_default_console()
    {
        static console_default sDefaultConsole(gGetDefaultConsoleOut());
        console = &sDefaultConsole;
    }

}; // namespace ncore
