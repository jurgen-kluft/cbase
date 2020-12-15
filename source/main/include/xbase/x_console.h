/**
 * @file Head file x_console.h
 * Core Console for Input, Output and Error
 */

#ifndef __XBASE_CONSOLE_H__
#define __XBASE_CONSOLE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_va_list.h"

/**
 * xCore namespace
 */
namespace xcore
{
    // Interface class, has specific (or configurable) implementations for different environments/platforms

    class xconsole;
    extern xconsole* console;

    class xconsole
    {
    public:
        enum EColor
        {
            NORMAL = 0,
            BLACK,
            RED,
            GREEN,
            YELLOW,
            BLUE,
            MAGENTA,
            CYAN,
            LTGREY,

            BG_BLACK,
            BG_RED,
            BG_GREEN,
            BG_YELLOW,
            BG_BLUE,
            BG_MAGENTA,
            BG_CYAN,
            BG_LTGREY,
        };

        // Platform/Configuration specific console 'out'
        class xout
        {
        public:
            virtual s32  color(xconsole::EColor color)   = 0;
            virtual s32  write(ascii::crunes_t const& str) = 0;
            virtual s32  write(utf32::crunes_t const& str) = 0;
            virtual void writeln()                       = 0;
        };

        virtual ~xconsole() {}

        static void init_default_console();

        virtual void initialize() = 0;
        virtual void shutdown()   = 0;

        virtual s32 setColor(xconsole::EColor color) = 0;

        virtual void write(bool _value) = 0;
        virtual void write(f64 _value)  = 0;
        virtual void write(s32 _value)  = 0;
        virtual void write(s64 _value)  = 0;
        virtual void write(f32 _value)  = 0;
        virtual void write(u32 _value)  = 0;
        virtual void write(u64 _value)  = 0;

        virtual void write(const ascii::crunes_t& buffer)                     = 0;
        virtual void write(const ascii::crunes_t& fmt, const x_va_list& args) = 0;
        virtual void write(const utf32::crunes_t& buffer)                     = 0;
        virtual void write(const utf32::crunes_t& fmt, const x_va_list& args) = 0;

        inline void write(const char* str)
        {
            crunes_t r(str, nullptr);
            write(r.m_runes.m_ascii);
        }

        virtual void writeLine() = 0;

        inline void writeLine(bool _value)
        {
            write(_value);
            writeLine();
        }
        inline void writeLine(f64 _value)
        {
            write(_value);
            writeLine();
        }
        inline void writeLine(s32 _value)
        {
            write(_value);
            writeLine();
        }
        inline void writeLine(s64 _value)
        {
            write(_value);
            writeLine();
        }
        inline void writeLine(f32 _value)
        {
            write(_value);
            writeLine();
        }
        inline void writeLine(u32 _value)
        {
            write(_value);
            writeLine();
        }
        inline void writeLine(u64 _value)
        {
            write(_value);
            writeLine();
        }

        inline void writeLine(const char* str)
        {
            crunes_t r(str, nullptr);
            writeLine(r.m_runes.m_ascii);
        }

        inline void writeLine(const ascii::crunes_t& str)
        {
            write(str);
            writeLine();
        }
        inline void writeLine(const ascii::crunes_t& format, const x_va_list& args)
        {
            write(format, args);
            writeLine();
        }
        inline void writeLine(const utf32::crunes_t& str)
        {
            write(str);
            writeLine();
        }
        inline void writeLine(const utf32::crunes_t& format, const x_va_list& args)
        {
            write(format, args);
            writeLine();
        }
    };

}; // namespace xcore

#endif ///< __XBASE_CONSOLE_H__
