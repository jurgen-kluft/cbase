#ifndef __X_BASE_DEFAULT_HASH_H__
#define __X_BASE_DEFAULT_HASH_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace xcore
{
    struct crunes_t;

    // Using SipHash
    class hashing_t
    {
    public:
        hashing_t();
        void reset();
        void hash(const void* data, s32 numBytes);
        u64  finalize();

    private:
        u64 d_v0, d_v1, d_v2, d_v3;
        union
        {
            u64   d_alignment;
            xbyte d_buf[8];
        };

        s32 d_bufSize;     // The length of the data currently stored in the buffer.
        s32 d_totalLength; // The total length of all data that has been passed into the algorithm.

        enum
        {
            SEED_LENGTH = 16
        }; // Seed length in bytes.
        void reset(xbyte const* seed);
    };

    u64 calchash(xbyte const* data, u32 size);
    u64 calchash(crunes_t const& str);

    template <typename K> class hasher_t
    {
    public:
        u64 hash(K const& k) const { return 0; }
    };

    template <> class hasher_t<s32>
    {
    public:
        u64 hash(s32 const& k) const { return calchash((const xbyte*)&k, sizeof(k)); }
    };

    template <> class hasher_t<u32>
    {
    public:
        u64 hash(u32 const& k) const { return calchash((const xbyte*)&k, sizeof(k)); }
    };

    template <> class hasher_t<s64>
    {
    public:
        u64 hash(s64 const& k) const { return calchash((const xbyte*)&k, sizeof(k)); }
    };

    template <> class hasher_t<u64>
    {
    public:
        u64 hash(u64 const& k) const { return calchash((const xbyte*)&k, sizeof(k)); }
    };

    template <> class hasher_t<f32>
    {
    public:
        u64 hash(f32 const& k) const { return calchash((const xbyte*)&k, sizeof(k)); }
    };

    template <> class hasher_t<f64>
    {
    public:
        u64 hash(f64 const& k) const { return calchash((const xbyte*)&k, sizeof(k)); }
    };

    template <> class hasher_t<void*>
    {
    public:
        u64 hash(void* const k) const { return calchash((const xbyte*)&k, sizeof(k)); }
    };

    template <> class hasher_t<const char*>
    {
    public:
        u64 hash(const char* const k) const 
        {
            const char* i = k;
            while (*i != '\0') i++;
            return calchash((const xbyte*)k, (s32)(i-k));
        }
    };
} // namespace xcore

#endif // __X_BASE_DEFAULT_HASH_H__