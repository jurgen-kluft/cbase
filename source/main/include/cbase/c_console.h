/**
 * @file Head file x_console.h
 * Core Console for Input, Output and Error
 */

#ifndef __CBASE_CONSOLE_H__
#define __CBASE_CONSOLE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"

namespace ncore
{
    // Interface class, has specific (or configurable) implementations for different environments/platforms

    class console_t;
    extern console_t* console;

    class console_t
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
        class out_t
        {
        public:
            virtual s32  color(console_t::EColor color)                         = 0;
            virtual s32  write(crunes_t const& str)                             = 0;
            virtual s32  write(crunes_t const& str, const va_t* argv, s32 argc) = 0;
            virtual void writeln()                                              = 0;

            template <typename... Args>
            inline s32 write(crunes_t const& str, Args&&... _args)
            {
                const va_t argv[] = {_args...};
                const s32  argc   = sizeof(argv) / sizeof(argv[0]);
                write(str, argv, argc);
            }
        };

        virtual ~console_t() {}

        static void init_default_console();

        virtual void initialize() = 0;
        virtual void shutdown()   = 0;

        virtual s32 setColor(console_t::EColor color) = 0;

        virtual void write(bool _value) = 0;
        virtual void write(f64 _value)  = 0;
        virtual void write(s32 _value)  = 0;
        virtual void write(s64 _value)  = 0;
        virtual void write(f32 _value)  = 0;
        virtual void write(u32 _value)  = 0;
        virtual void write(u64 _value)  = 0;

        virtual void write(const crunes_t& str)                             = 0;
        virtual void write(const crunes_t& fmt, const va_t* argv, s32 argc) = 0;

        inline void write(const char* str)
        {
            crunes_t r(str);
            write(r);
        }

        inline void write(const char* str, const char* end)
        {
            crunes_t r(str, end);
            write(r);
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
            crunes_t r(str);
            writeLine(r);
        }

        inline void writeLine(const char* str, const va_t* argv, s32 argc)
        {
            crunes_t r(str);
            writeLine(r, argv, argc);
        }

        inline void writeLine(const crunes_t& str)
        {
            write(str);
            writeLine();
        }
        inline void writeLine(const crunes_t& format, const va_t* argv, s32 argc)
        {
            write(format, argv, argc);
            writeLine();
        }
    };

};  // namespace ncore

#endif  ///< __CBASE_CONSOLE_H__
