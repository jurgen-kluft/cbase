/**
 * @file x_console.cpp
 * Core console
 */

#include "xbase/x_debug.h"
#include "xbase/x_memory.h"
#include "xbase/x_va_list.h"
#include "xbase/x_console.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

namespace xcore
{
//#define XCONSOLE_LOCAL_STR_BUF(type, local_var_name, size)	type local_var_name##Buffer[size + 1]; local_var_name##Buffer[size] = '\0'; type* local_var_name = &local_var_name##Buffer[0]; type* local_var_name##_eos = &local_var_name##Buffer[size];
#define XCONSOLE_LOCAL_STR_BUF(type, local_var_name, size) \
    type::rune local_var_name##Buffer[size];               \
    runes_t    local_var_name(local_var_name##Buffer, local_var_name##Buffer, local_var_name##Buffer, &local_var_name##Buffer[size - 1], type::TYPE);

    class xconsole_null : public xconsole
    {
    public:
        xconsole_null() {}
        virtual ~xconsole_null() {}

        virtual void initialize();
        virtual void shutdown();

        virtual s32 setColor(xconsole::EColor color);

        virtual void write(bool _value);
        virtual void write(f64 _value);
        virtual void write(s32 _value);
        virtual void write(s64 _value);
        virtual void write(f32 _value);

        virtual void write(u32 _value);
        virtual void write(u64 _value);

        virtual void write(const crunes_t& buffer);
        virtual void write(const ascii::crunes_t& str);
        virtual void write(const ascii::crunes_t& fmt, const x_va_list& args);
        virtual void write(const utf32::crunes_t& str);
        virtual void write(const utf32::crunes_t& fmt, const x_va_list& args);

        virtual void writeLine();
    };

    void xconsole_null::initialize() {}
    void xconsole_null::shutdown() {}
    s32  xconsole_null::setColor(xconsole::EColor color) { return 0; }
    void xconsole_null::write(bool _value) {}
    void xconsole_null::write(f64 _value) {}
    void xconsole_null::write(s32 _value) {}
    void xconsole_null::write(s64 _value) {}
    void xconsole_null::write(f32 _value) {}
    void xconsole_null::write(u32 _value) {}
    void xconsole_null::write(u64 _value) {}
    void xconsole_null::write(const crunes_t& str) {}
    void xconsole_null::write(const ascii::crunes_t& str) {}
    void xconsole_null::write(const ascii::crunes_t& fmt, const x_va_list& args) {}
    void xconsole_null::write(const utf32::crunes_t& str) {}
    void xconsole_null::write(const utf32::crunes_t& fmt, const x_va_list& args) {}
    void xconsole_null::writeLine() {}

    static xconsole_null sNullConsole;

    class xconsole_default : public xconsole
    {
        xconsole::xout* mOut;

    public:
        xconsole_default(xconsole::xout* _out) : mOut(_out) {}

        virtual ~xconsole_default() {}

        virtual void initialize();
        virtual void shutdown();

        virtual s32 setColor(xconsole::EColor color);

        virtual void write(bool _value);
        virtual void write(f64 _value);
        virtual void write(s32 _value);
        virtual void write(s64 _value);
        virtual void write(f32 _value);

        virtual void write(u32 _value);
        virtual void write(u64 _value);

        virtual void write(const crunes_t& str);

        virtual void write(const ascii::crunes_t& str);
        virtual void write(const ascii::crunes_t& fmt, const x_va_list& args);
        virtual void write(const utf32::crunes_t& str);
        virtual void write(const utf32::crunes_t& fmt, const x_va_list& args);

        virtual void writeLine();
    };

    void xconsole_default::initialize() {}

    void xconsole_default::shutdown() {}

    s32 xconsole_default::setColor(xconsole::EColor color) { return mOut->color(color); }

    void xconsole_default::write(bool _value)
    {
        crunes_t truestr("true", 4);
        crunes_t falsestr("false", 5);
        write(_value ? truestr : falsestr);
    }

    void xconsole_default::write(f64 _value)
    {
        XCONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(s32 _value)
    {
        XCONSOLE_LOCAL_STR_BUF(ascii, tmp, 64);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(s64 _value)
    {
        XCONSOLE_LOCAL_STR_BUF(ascii, tmp, 64);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(f32 _value)
    {
        XCONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(u32 _value)
    {
        XCONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(u64 _value)
    {
        XCONSOLE_LOCAL_STR_BUF(ascii, tmp, 256);
        to_string(tmp, _value, 2);
        write(tmp);
    }

    void xconsole_default::write(const crunes_t& str)
    {
        switch (str.m_type)
        {
            case ascii::TYPE: return write(str.m_runes.m_ascii);
            case utf32::TYPE: return write(str.m_runes.m_utf32);
            default:
                break;
                //@todo: UTF-8 and UTF-16
        }
    }

    void xconsole_default::write(const ascii::crunes_t& str) { mOut->write(str); }

    void xconsole_default::write(const ascii::crunes_t& fmt, const x_va_list& args)
    {
        XCONSOLE_LOCAL_STR_BUF(ascii, str, 1024);
        vsprintf(str, crunes_t(fmt), args);
        crunes_t outstr(str);
        mOut->write(outstr);
    }

    void xconsole_default::write(const utf32::crunes_t& str) { mOut->write(str); }

    void xconsole_default::write(const utf32::crunes_t& fmt, const x_va_list& args)
    {
        XCONSOLE_LOCAL_STR_BUF(utf32, str, 1024);
        vsprintf(str, utf32::crunes_t(fmt), args);
        crunes_t outstr(str);
        mOut->write(outstr);
    }

    void xconsole_default::writeLine() { mOut->writeln(); }

    extern xconsole::xout* gGetDefaultConsoleOut();
    xconsole*              console = NULL;

    void xconsole::init_default_console()
    {
        static xconsole_default sDefaultConsole(gGetDefaultConsoleOut());
        console = &sDefaultConsole;
    }

}; // namespace xcore
