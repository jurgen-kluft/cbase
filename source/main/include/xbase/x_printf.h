#ifndef __XCORE_CHARS_H__
#define __XCORE_CHARS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "xbase/x_va_list.h"

namespace xcore
{
    s32  cprintf(crunes_t const& format, X_VA_ARGS_16_DEF);
    s32  vcprintf(crunes_t const& format, const va_list_t& args);
    void sprintf(runes_t& dst, crunes_t const& format, X_VA_ARGS_16_DEF);
    void vsprintf(runes_t& dst, crunes_t const& format, const va_list_t& args);
    void printf(crunes_t const& str);
    void printf(crunes_t const& format, X_VA_ARGS_16_DEF);
    void printf(crunes_t const& format, const va_list_t& args);
    s32  sscanf(crunes_t& str, crunes_t const& format, X_VA_R_ARGS_16_DEF);
    s32  vsscanf(crunes_t& str, crunes_t const& format, const x_va_r_list& vr_args);
} // namespace xcore

#endif ///< __XCORE_CHARS_H__
