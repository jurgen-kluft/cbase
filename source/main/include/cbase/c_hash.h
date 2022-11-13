#ifndef __CBASE_DEFAULT_HASH_H__
#define __CBASE_DEFAULT_HASH_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
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
            u8 d_buf[8];
        };

        s32 d_bufSize;     // The length of the data currently stored in the buffer.
        s32 d_totalLength; // The total length of all data that has been passed into the algorithm.

        enum
        {
            SEED_LENGTH = 16
        }; // Seed length in bytes.
        void reset(u8 const* seed);
    };

    u64 calchash(u8 const* data, u32 size);
    u64 strhash(const char* str);
    u64 strhash_lowercase(const char* str);
    u32 strhash32(const char* str);
    u32 strhash32_lowercase(const char* str);
    
    u64 calchash(crunes_t const& str);

    template <typename K> class hasher_t
    {
    public:
        bool is_hashable() const { return false; }
        u64 hash(K const& k) const { return 0; }
    };

    template <> class hasher_t<s32>
    {
    public:
        bool is_hashable() const { return false; }
        u64 hash(s32 const& k) const{ return (u64)k; }
    };

    template <> class hasher_t<u32>
    {
    public:
        bool is_hashable() const { return false; }
        u64 hash(u32 const& k) const{ return (u64)k; }
    };

    template <> class hasher_t<s64>
    {
    public:
        bool is_hashable() const { return false; }
        u64 hash(s64 const& k) const{ return (u64)k; }
    };

    template <> class hasher_t<u64>
    {
    public:
        bool is_hashable() const { return false; }
        u64 hash(u64 const& k) const{ return (u64)k; }
    };

    template <> class hasher_t<f32>
    {
    public:
        bool is_hashable() const { return false; }
        u64 hash(f32 const& k) const{ return (u64)k; }
    };

    template <> class hasher_t<f64>
    {
    public:
        bool is_hashable() const { return false; }
        u64 hash(f64 const& k) const{ return (u64)k; }
    };

    template <> class hasher_t<void*>
    {
    public:
        bool is_hashable() const { return false; }
        u64 hash(void* const k) const { return (u64)k; }
    };

    template <> class hasher_t<const char*>
    {
    public:
        bool is_hashable() const { return true; }
        u64 hash(const char* const k) const 
        {
            const char* i = k;
            while (*i != '\0') i++;
            return calchash((const u8*)k, (s32)(i-k));
        }
    };
} // namespace ncore

#endif // __CBASE_DEFAULT_HASH_H__