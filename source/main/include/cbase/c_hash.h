#ifndef __CBASE_DEFAULT_HASH_H__
#define __CBASE_DEFAULT_HASH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_hash.h"

namespace ncore
{
    struct crunes_t;

    class hasher_t
    {
    public:
        virtual ~hasher_t() {}

        virtual s32  size() const                         = 0;
        virtual void reset()                              = 0;
        virtual void begin()                              = 0;
        virtual void hash(const u8* begin, const u8* end) = 0;
        virtual void end(u8* out_hash, s32 size)          = 0;
    };

    namespace nhash
    {
        u64 strhash(crunes_t const& str, u64 seed = 0);
        u64 strhash_lowercase(crunes_t const& str, u64 seed = 0);

        u32 strhash32(crunes_t const& str, u64 seed = 0);
        u32 strhash32_lowercase(crunes_t const& str, u64 seed = 0);
    }  // namespace nhash

}  // namespace ncore

#endif  // __CBASE_DEFAULT_HASH_H__
