#ifndef __CCORE_CHARS_H__
#define __CCORE_CHARS_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_va_list.h"

namespace ncore
{
    s32  cprintf(crunes_t const& format, X_VA_ARGS_16_DEF);
    s32  vcprintf(crunes_t const& format, const va_list_t& args);
    void sprintf(runes_t& dst, crunes_t const& format, X_VA_ARGS_16_DEF);
    void vsprintf(runes_t& dst, crunes_t const& format, const va_list_t& args);
    void zprintf(irunes_writer_t& dst, crunes_t const& format, X_VA_ARGS_16_DEF);
    void vzprintf(irunes_writer_t& dst, crunes_t const& format, const va_list_t& args);
    void printf(crunes_t const& str);
    void printf(crunes_t const& format, X_VA_ARGS_16_DEF);
    void printf(crunes_t const& format, const va_list_t& args);
    s32  sscanf(crunes_t& str, crunes_t const& format, X_VA_R_ARGS_16_DEF);
    s32  vsscanf(crunes_t& str, crunes_t const& format, const va_r_list_t& vr_args);

    char* itoa(s64 value, char* dst, char const* end, s32 radix);
    char* utoa(u64 value, char* dst, char const* end, s32 radix);
    char* ftoa(f32 value, char* dst, char const* end);
    char* dtoa(f64 value, char* dst, char const* end);
    char* btoa(bool value, char* dst, char const* end);
    
} // namespace ncore

#endif ///< __CCORE_CHARS_H__
