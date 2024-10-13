#ifndef __CBASE_DEBUG_H__
#define __CBASE_DEBUG_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_debug.h"

namespace cbase
{
    ncore::asserthandler_t* gSetAssertHandler();

};  // namespace cbase

#endif  /// __CBASE_DEBUG_H__
