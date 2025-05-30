#ifndef __C_GUID_H__
#define __C_GUID_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    struct runes_t;
    struct crunes_t;

    /**
     * Note: For a generator, take a look here http://cvs.ossp.org/dir?d=ossp-pkg/uuid
     * Description:
     *     This class described a 128bit guid. The guid is not created here, this is
     *     a data container holding the guid value. From the point of view of ncore
     *     this defines a system type.
     */
    struct guid_t
    {
    public:
        guid_t();
        guid_t(const guid_t&);
        guid_t(u32 wordh, u32 word2, u32 word1, u32 wordl);

        void setNull(void);
        bool isNull(void) const;

        void set(u32 wordh, u32 word2, u32 word1, u32 wordl);
        void get(u32& wordh, u32& word2, u32& word1, u32& wordl) const;

        guid_t& operator=(const guid_t&);

        bool operator==(const guid_t&) const;
        bool operator!=(const guid_t&) const;

        void toString(runes_t& str) const;
        void fromString(crunes_t const& str);

    private:
        enum ESizes
        {
            SIZE32 = 4,
            SIZE16 = 8,
            SIZE8  = 16
        };
        union uguid_t
        {
            u8  ma8[SIZE8];
            u16 ma16[SIZE16];
            u32 ma32[SIZE32];
        };
        uguid_t mGuid;
    };

#include "private/c_guid_inline.h"

};  // namespace ncore

#endif
