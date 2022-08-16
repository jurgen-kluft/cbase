#ifndef __CBASE_LOG_H__
#define __CBASE_LOG_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_runes.h"

namespace ncore
{
    // Forward declares
    class va_list_t;
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
    void __LogMessage(const char* channel, const char* messageFormat, const va_list_t& formatVars);
    void __LogWarning(const char* channel, const char* messageFormat, const va_list_t& formatVars);
    void __LogError(const char* channel, const char* messageFormat, const va_list_t& formatVars);

    void __LogPush(const char* fileName, s32 lineNum);
    void __LogFlush(void);

#define D_LogMessage                      \
    ncore::__LogPush(__FILE__, __LINE__); \
    ncore::__LogMessage
#define D_LogWarning                      \
    ncore::__LogPush(__FILE__, __LINE__); \
    ncore::__LogWarning
#define D_LogError                        \
    ncore::__LogPush(__FILE__, __LINE__); \
    ncore::__LogError
#else
    inline void __LogPush(const char* fileName, s32 lineNum) {}
    inline void __LogFlush(void) {}

    inline void __LogFake(const char* channel, const char* message) {}
    inline void __LogFake(const char* channel, const char* messageFormat, const va_list_t& formatVars) {}

#define D_LogMessage ncore::__LogFake
#define D_LogWarning ncore::__LogFake
#define D_LogError ncore::__LogFake
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
        static void write(elevel inLevel, const crunes_t& format, const va_list_t& arguments);

        static void writeLine(elevel inLevel);
        static void writeLine(elevel inLevel, bool _value);
        static void writeLine(elevel inLevel, s32 _value);
        static void writeLine(elevel inLevel, s64 _value);
        static void writeLine(elevel inLevel, u32 _value);
        static void writeLine(elevel inLevel, u64 _value);
        static void writeLine(elevel inLevel, f32 _value);
        static void writeLine(elevel inLevel, f64 _value);
        static void writeLine(elevel inLevel, const crunes_t& str);
        static void writeLine(elevel inLevel, const crunes_t& format, const va_list_t& arguments);
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

}; // namespace ncore

#include "private/x_log_to_console.h"

#endif ///< END __CBASE_LOG_H__
