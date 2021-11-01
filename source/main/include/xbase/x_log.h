#ifndef __XBASE_LOG_H__
#define __XBASE_LOG_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_runes.h"

namespace xcore
{
    // Forward declares
    class va_list_t;
    class logger_t;

#define X_LOG

#ifdef X_LOG

    struct log_info_t
    {
        s32         mSequenceCount;
        s32         mLogLineNumber;
        const char* mLogFileName;

        typedef log_info_t& _delegate();
    };

    ///< In a multi-threaded environment the user needs to call this to set a delegate that can
    ///< return a log_info_t per thread. The default one only works in a single threaded environment.
    void x_SetGetLogInfo(log_info_t::_delegate* Function);

    void __x_LogMessage(const char* channel, const char* message);
    void __x_LogWarning(const char* channel, const char* message);
    void __x_LogError(const char* channel, const char* message);
    void __x_LogMessage(const char* channel, const char* messageFormat, const va_list_t& formatVars);
    void __x_LogWarning(const char* channel, const char* messageFormat, const va_list_t& formatVars);
    void __x_LogError(const char* channel, const char* messageFormat, const va_list_t& formatVars);

    void x_LogPush(const char* fileName, s32 lineNum);
    void x_LogFlush(void);

#define x_LogMessage                      \
    xcore::x_LogPush(__FILE__, __LINE__); \
    xcore::__x_LogMessage
#define x_LogWarning                      \
    xcore::x_LogPush(__FILE__, __LINE__); \
    xcore::__x_LogWarning
#define x_LogError                        \
    xcore::x_LogPush(__FILE__, __LINE__); \
    xcore::__x_LogError
#else
    inline void x_LogPush(const char* fileName, s32 lineNum) {}
    inline void x_LogFlush(void) {}

    inline void __x_LogFake(const char* channel, const char* message) {}
    inline void __x_LogFake(const char* channel, const char* messageFormat, const va_list_t& formatVars) {}

#define x_LogMessage xcore::__x_LogFake
#define x_LogWarning xcore::__x_LogFake
#define x_LogError xcore::__x_LogFake
#endif

    class log_t
    {
    public:
        enum ELevel
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

        static logger_t* redirect(ELevel inLevel, logger_t* inLogger);

        static void write(ELevel inLevel, bool _value);
        static void write(ELevel inLevel, s32 _value);
        static void write(ELevel inLevel, s64 _value);
        static void write(ELevel inLevel, u32 _value);
        static void write(ELevel inLevel, u64 _value);
        static void write(ELevel inLevel, f32 _value);
        static void write(ELevel inLevel, f64 _value);
        static void write(ELevel inLevel, const crunes_t& str);
        static void write(ELevel inLevel, const crunes_t& format, const va_list_t& arguments);

        static void writeLine(ELevel inLevel);
        static void writeLine(ELevel inLevel, bool _value);
        static void writeLine(ELevel inLevel, s32 _value);
        static void writeLine(ELevel inLevel, s64 _value);
        static void writeLine(ELevel inLevel, u32 _value);
        static void writeLine(ELevel inLevel, u64 _value);
        static void writeLine(ELevel inLevel, f32 _value);
        static void writeLine(ELevel inLevel, f64 _value);
        static void writeLine(ELevel inLevel, const crunes_t& str);
        static void writeLine(ELevel inLevel, const crunes_t& format, const va_list_t& arguments);
    };

    /**
     * Interface class, has specific implementations for different environments/platforms
     */
    class logger_t
    {
        friend class log_t;

        virtual void initialize() = 0;
        virtual void flush()      = 0;
        virtual void shutdown()   = 0;
        virtual void clear()      = 0;

    public:
        virtual ~logger_t(){};

        virtual void write(bool _value)                                        = 0;
        virtual void write(s32 _value)                                         = 0;
        virtual void write(s64 _value)                                         = 0;
        virtual void write(u32 _value)                                         = 0;
        virtual void write(u64 _value)                                         = 0;
        virtual void write(f32 _value)                                         = 0;
        virtual void write(f64 _value)                                         = 0;
        virtual void write(const crunes_t& str)                                = 0;
        virtual void write(const crunes_t& format, const va_list_t& arguments) = 0;

        virtual void writeLine()                                                   = 0;
        virtual void writeLine(bool _value)                                        = 0;
        virtual void writeLine(s32 _value)                                         = 0;
        virtual void writeLine(s64 _value)                                         = 0;
        virtual void writeLine(u32 _value)                                         = 0;
        virtual void writeLine(u64 _value)                                         = 0;
        virtual void writeLine(f32 _value)                                         = 0;
        virtual void writeLine(f64 _value)                                         = 0;
        virtual void writeLine(const crunes_t& str)                                = 0;
        virtual void writeLine(const crunes_t& format, const va_list_t& arguments) = 0;
    };

}; // namespace xcore

#include "private/x_log_to_console.h"

#endif ///< END __XBASE_LOG_H__
