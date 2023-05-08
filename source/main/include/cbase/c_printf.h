#ifndef __CCORE_CHARS_H__
#define __CCORE_CHARS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_va_list.h"

namespace ncore
{
    // Output a character to a custom device like UART, used by the printf() function.
    // This function is declared here, you have to write your custom implementation.
    void _putchar(char character);
    void _putflush(); // Indicate that we are done 

    int printf_(const char* format, const char* format_end, const va_t* argv, s32 argc);
    int snprintf_(char* buffer, const char* buffer_end, const char* format, const char* format_end, const va_t* argv, s32 argc);
    int vprintf_(const char* format, const char* format_end, const va_t* argv, s32 argc);
    int fctprintf(void (*out)(const char* str, u32 n, void* arg), void* arg, const char* format, const char* format_end, const va_t* argv, s32 argc);
    int vzprintf(irunes_writer_t& writer, const crunes_t& str, const va_t* argv, s32 argc);

    inline void printf(crunes_t const& str)
    {
        const va_t* argv = nullptr;
        const s32   argc = 0;
        printf_(&str.m_ascii.m_bos[str.m_ascii.m_str], &str.m_ascii.m_bos[str.m_ascii.m_end], argv, argc);
    }

    template <typename... Args>
    inline void printf(crunes_t const& format, Args&&... args)
    {
        const va_t argv[] = {args...};
        const s32  argc   = sizeof(argv) / sizeof(argv[0]);
        printf_(&format.m_ascii.m_bos[format.m_ascii.m_str], &format.m_ascii.m_bos[format.m_ascii.m_end], argv, argc);
    }

    template <typename... Args>
    inline void sprintf(runes_t& str, crunes_t const& format, Args&&... args)
    {
        const va_t argv[] = {args...};
        const s32  argc   = sizeof(argv) / sizeof(argv[0]);
        int ret = snprintf_(&str.m_ascii.m_bos[str.m_ascii.m_str], &str.m_ascii.m_bos[str.m_ascii.m_eos], &format.m_ascii.m_bos[format.m_ascii.m_str], &format.m_ascii.m_bos[format.m_ascii.m_end], argv, argc);
        str.m_ascii.m_end = str.m_ascii.m_str + ret;
    }

    template <typename... Args>
    inline s32 cprintf(crunes_t const& format, Args&&... args)
    {
        const va_t argv[] = {args...};
        const s32  argc   = sizeof(argv) / sizeof(argv[0]);
        return vprintf_(&format.m_ascii.m_bos[format.m_ascii.m_str], &format.m_ascii.m_bos[format.m_ascii.m_end], argv, argc);
    }

    s32 sscanf_(crunes_t& str, crunes_t const& format, const va_r_t* argv, s32 argc);

    template <typename... Args>
    inline s32 sscanf(crunes_t& str, crunes_t const& format, Args&&... args)
    {
        const va_r_t argv[] = {args...};
        const s32  argc   = sizeof(argv) / sizeof(argv[0]);
        return sscanf_(str, format, argv, argc);
    }

}  // namespace ncore

#endif  ///< __CCORE_CHARS_H__
