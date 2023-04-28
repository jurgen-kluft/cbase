#ifndef __CBASE_DEFAULT_HASH_H__
#define __CBASE_DEFAULT_HASH_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    struct crunes_t;

    namespace nhash
    {
        u64 datahash(u8 const* data, u32 size, u64 seed = 0);

        u64 strhash(const char* str, u64 seed = 0);
        u64 strhash(const char* str, const char* end, u64 seed = 0);
        u64 strhash(crunes_t const& str, u64 seed = 0);
        u64 strhash_lowercase(const char* str, u64 seed = 0);
        u64 strhash_lowercase(const char* str, const char* end, u64 seed = 0);
        u64 strhash_lowercase(crunes_t const& str, u64 seed = 0);

        u32 strhash32(const char* str, u64 seed = 0);
        u32 strhash32(const char* str, const char* end, u64 seed = 0);
        u32 strhash32(crunes_t const& str, u64 seed = 0);
        u32 strhash32_lowercase(const char* str, u64 seed = 0);
        u32 strhash32_lowercase(const char* str, const char* end, u64 seed = 0);
        u32 strhash32_lowercase(crunes_t const& str, u64 seed = 0);

        template <typename K> class hasher_t
        {
        public:
            bool is_hashable() const { return false; }
            u64  hash(K const& k) const { return static_cast<u64>(k); }
        };

        template <> class hasher_t<void*>
        {
        public:
            bool is_hashable() const { return false; }
            u64  hash(void* const k) const { return (u64)k; }
        };

        template <> class hasher_t<const char*>
        {
        public:
            bool is_hashable() const { return true; }
            u64  hash(const char* const k) const
            {
                const char* i = k;
                while (*i != '\0')
                    i++;
                return strhash(k, i);
            }
        };

    } // namespace nhash
} // namespace ncore

#endif // __CBASE_DEFAULT_HASH_H__