#ifndef __CBASE_LOG_H__
#define __CBASE_LOG_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_runes.h"
#include "cbase/c_va_list.h"

namespace ncore
{
    // Forward declares
    class logger_t;

#define D_LOG
#ifdef D_LOG

    struct log_info_t
    {
        s32         mSequenceCount;
        s32         mLogLineNumber;
        const char* mLogFileName;

        typedef log_info_t& _delegate();
    };

    ///< In a multi-threaded environment the user needs to call this to set a delegate that can
    ///< return a log_info_t per thread. The default one only works in a single threaded environment.
    void g_SetGetLogInfo(log_info_t::_delegate* Function);

    void __LogMessage(const char* channel, const char* message);
    void __LogWarning(const char* channel, const char* message);
    void __LogError(const char* channel, const char* message);

    void __LogMessage2(const char* channel, const char* messageFormat, const va_t* argv, s32 argc);
    void __LogWarning2(const char* channel, const char* messageFormat, const va_t* argv, s32 argc);
    void __LogError2(const char* channel, const char* messageFormat, const va_t* argv, s32 argc);

    template <typename... Args>
    inline void __LogMessage(const char* channel, const char* messageFormat, Args&&... _args)
    {
        const va_t argv[] = {_args...};
        const s32  argc   = sizeof(argv) / sizeof(argv[0]);
        __LogMessage2(channel, messageFormat, argv, argc);
    }

    template <typename... Args>
    inline void __LogWarning(const char* channel, const char* messageFormat, Args&&... _args)
    {
        const va_t argv[] = {_args...};
        const s32  argc   = sizeof(argv) / sizeof(argv[0]);
        __LogWarning2(channel, messageFormat, argv, argc);
    }

    template <typename... Args>
    inline void __LogError(const char* channel, const char* messageFormat, Args&&... _args)
    {
        const va_t argv[] = {_args...};
        const s32  argc   = sizeof(argv) / sizeof(argv[0]);
        __LogError2(channel, messageFormat, argv, argc);
    }

    void __LogPush(const char* fileName, s32 lineNum);
    void __LogFlush(void);

#    define D_LogMessage                      \
        ncore::__LogPush(__FILE__, __LINE__); \
        ncore::__LogMessage
#    define D_LogWarning                      \
        ncore::__LogPush(__FILE__, __LINE__); \
        ncore::__LogWarning
#    define D_LogError                        \
        ncore::__LogPush(__FILE__, __LINE__); \
        ncore::__LogError
#else
    inline void __LogPush(const char* fileName, s32 lineNum) {}
    inline void __LogFlush(void) {}

    inline void __LogFake(const char* channel, const char* message) {}
    inline void __LogFake(const char* channel, const char* messageFormat, const va_list_t& formatVars) {}

#    define D_LogMessage ncore::__LogFake
#    define D_LogWarning ncore::__LogFake
#    define D_LogError   ncore::__LogFake
#endif

    class log_t
    {
    public:
        enum elevel
        {
            INFO      = 0,
            WARNING   = 1,
            ERROR     = 2,
            EXCEPTION = 3,
            COUNT,
        };

    private:
        static logger_t* sLogger[COUNT];

    public:
        static void init();
        static void shutdown();
        static void flush();
        static void clear();

        static logger_t* redirect(elevel inLevel, logger_t* inLogger);

        static void write(elevel inLevel, bool _value);
        static void write(elevel inLevel, s32 _value);
        static void write(elevel inLevel, s64 _value);
        static void write(elevel inLevel, u32 _value);
        static void write(elevel inLevel, u64 _value);
        static void write(elevel inLevel, f32 _value);
        static void write(elevel inLevel, f64 _value);
        static void write(elevel inLevel, const crunes_t& str);
        static void write(elevel inLevel, const crunes_t& format, const va_t* argv, s32 argc);

        template <typename... Args>
        inline static void write(elevel inLevel, crunes_t const& format, Args&&... _args)
        {
            const va_t argv[] = {_args...};
            const s32  argc   = sizeof(argv) / sizeof(argv[0]);
            write(inLevel, format, argv, argc);
        }

        static void writeLine(elevel inLevel);
        static void writeLine(elevel inLevel, bool _value);
        static void writeLine(elevel inLevel, s32 _value);
        static void writeLine(elevel inLevel, s64 _value);
        static void writeLine(elevel inLevel, u32 _value);
        static void writeLine(elevel inLevel, u64 _value);
        static void writeLine(elevel inLevel, f32 _value);
        static void writeLine(elevel inLevel, f64 _value);
        static void writeLine(elevel inLevel, const crunes_t& str);
        static void writeLine(elevel inLevel, const crunes_t& format, const va_t* argv, s32 argc);

        template <typename... Args>
        inline static void writeLine(elevel inLevel, crunes_t const& format, Args&&... _args)
        {
            const va_t argv[] = {_args...};
            const s32  argc   = sizeof(argv) / sizeof(argv[0]);
            write(inLevel, format, argv, argc);
        }
    };

    // Interface class, has specific implementations for different environments/platforms
    class logger_t
    {
        friend class log_t;

        virtual void initialize() = 0;
        virtual void flush()      = 0;
        virtual void shutdown()   = 0;
        virtual void clear()      = 0;

    public:
        virtual ~logger_t() {};

        virtual void write(bool _value)                                        = 0;
        virtual void write(s32 _value)                                         = 0;
        virtual void write(s64 _value)                                         = 0;
        virtual void write(u32 _value)                                         = 0;
        virtual void write(u64 _value)                                         = 0;
        virtual void write(f32 _value)                                         = 0;
        virtual void write(f64 _value)                                         = 0;
        virtual void write(const crunes_t& str)                                = 0;
        virtual void write(const crunes_t& format, const va_t* argv, s32 argc) = 0;

        template <typename... Args>
        inline void write(crunes_t const& format, Args&&... _args)
        {
            const va_t argv[] = {_args...};
            const s32  argc   = sizeof(argv) / sizeof(argv[0]);
            write(format, argv, argc);
        }

        virtual void writeLine()                                                   = 0;
        virtual void writeLine(bool _value)                                        = 0;
        virtual void writeLine(s32 _value)                                         = 0;
        virtual void writeLine(s64 _value)                                         = 0;
        virtual void writeLine(u32 _value)                                         = 0;
        virtual void writeLine(u64 _value)                                         = 0;
        virtual void writeLine(f32 _value)                                         = 0;
        virtual void writeLine(f64 _value)                                         = 0;
        virtual void writeLine(const crunes_t& str)                                = 0;
        virtual void writeLine(const crunes_t& format, const va_t* argv, s32 argc) = 0;

        template <typename... Args>
        inline void writeLine(crunes_t const& format, Args&&... _args)
        {
            const va_t argv[] = {_args...};
            const s32  argc   = sizeof(argv) / sizeof(argv[0]);
            write(format, argv, argc);
        }
    };

};  // namespace ncore

#include "private/c_log_to_console.h"

#endif  ///< END __CBASE_LOG_H__
