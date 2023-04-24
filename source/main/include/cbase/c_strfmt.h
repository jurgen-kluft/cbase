#ifndef __CBASE_STR_FMT_H__
#define __CBASE_STR_FMT_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif
#include "cbase/c_debug.h"
#include "cbase/c_runes.h"
#include "cbase/c_va_list.h"

namespace ncore
{
    bool format_to(ascii::prune str, ascii::prune end, ascii::prune fmt, va_list_t args);
    bool format_to(utf16::prune str, utf16::prune end, utf16::prune fmt, va_list_t args);
    bool format_to(utf32::prune str, utf32::prune end, utf32::prune fmt, va_list_t args);
}


#endif // __CBASE_STR_FMT_H__