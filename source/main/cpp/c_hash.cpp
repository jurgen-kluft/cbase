#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_hash.h"
#include "cbase/c_memory.h"
#include "cbase/c_runes.h"

namespace ncore
{
    namespace nhash
    {
        // calculate the hash of a string, this string can be ascii, utf-8, utf-16 or utf-32.
        // so use an intermediate buffer to convert the string into utf-32 and feed that to
        // the hash algorithm.
        u64 strhash(crunes_t const& strdata, u64 seed)
        {
            if ((strdata.m_type) == ascii::TYPE)
            {
                const char* begin = &strdata.m_ascii[strdata.m_str];
                const char* end   = &strdata.m_ascii[strdata.m_str];
                return strhash64(begin, end, seed);
            }

            // We could extend it to support utf-32 and utf-16, but we don't need it for now.

            ASSERT(false);
            return 0;
        }

        u64 strhash_lowercase(crunes_t const& strdata, u64 seed)
        {
            if ((strdata.m_type) == ascii::TYPE)
            {
                const char* begin = &strdata.m_ascii[strdata.m_str];
                const char* end   = &strdata.m_ascii[strdata.m_str];
                return strhash64_lowercase(begin, end, seed);
            }

            // We could extend it to support utf-32 and utf-16, but we don't need it for now.

            ASSERT(false);
            return 0;
        }

        u32 strhash32(crunes_t const& str, u32 seed)
        {
            if ((str.m_type) == ascii::TYPE)
            {
                const char* begin = &str.m_ascii[str.m_str];
                const char* end   = &str.m_ascii[str.m_str];
                return strhash32(begin, end, seed);
            }

            // We could extend it to support utf-32 and utf-16, but we don't need it for now.

            ASSERT(false);
            return 0;
        }

        u32 strhash32_lowercase(crunes_t const& str, u32 seed)
        {
            if ((str.m_type) == ascii::TYPE)
            {
                const char* begin = &str.m_ascii[str.m_str];
                const char* end   = &str.m_ascii[str.m_str];
                return strhash32_lowercase(begin, end, seed);
            }

            // We could extend it to support utf-32 and utf-16, but we don't need it for now.

            ASSERT(false);
            return 0;
        }

    }  // namespace nhash
};  // namespace ncore
