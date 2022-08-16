#ifndef __CBASE_TYPES_H__
#define __CBASE_TYPES_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
#ifdef H_NULL
#pragma error
#endif

    struct handle_t
    {
        u32 mHandle;

        enum
        {
            H_NULL = 0xffffffff
        };

        inline handle_t(void)
            : mHandle(H_NULL)
        {
        }
        inline explicit handle_t(u32 value) { mHandle = value; }
        inline bool isValid(void) const { return bool(mHandle != H_NULL); }
        inline bool isNull(void) const { return bool(mHandle == H_NULL); }
        inline void setNull(void) { mHandle = (u32)H_NULL; }
        inline u32  get(void) const { return mHandle; }
        inline void set(u32 v) { mHandle = v; }
        inline bool operator==(handle_t h) const { return h.mHandle == mHandle; }
        inline bool operator!=(handle_t h) const { return h.mHandle != mHandle; }
        inline bool operator<(handle_t h) const { return mHandle < h.mHandle; }
        inline bool operator<=(handle_t h) const { return mHandle <= h.mHandle; }
        inline bool operator>(handle_t h) const { return mHandle > h.mHandle; }
        inline bool operator>=(handle_t h) const { return mHandle <= h.mHandle; }
    };

#define DHANDLE_TYPE(TYPE_NAME)                                                          \
    struct TYPE_NAME : public ncore::handle_t                                            \
    {                                                                                    \
        inline TYPE_NAME(void) {}                                                        \
        inline explicit TYPE_NAME(ncore::u32 value) { mHandle = value; }                 \
        inline bool operator==(ncore::handle_t h) const { return h.mHandle == mHandle; } \
        inline bool operator!=(ncore::handle_t h) const { return h.mHandle != mHandle; } \
    };
}; // namespace ncore

#endif
