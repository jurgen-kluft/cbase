#ifndef __X_BASE_DEFAULT_HASH_H__
#define __X_BASE_DEFAULT_HASH_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
	// Using SipHash
    class xhashing
    {
    public:
        xhashing();
        void reset();
        void hash(const void *data, s32 numBytes);
        u64  finalize();

	private:
        u64 d_v0, d_v1, d_v2, d_v3;
        union
		{
            u64     d_alignment;
            xbyte   d_buf[8];
        };

        s32 d_bufSize;       // The length of the data currently stored in the buffer.
        s32 d_totalLength;   // The total length of all data that has been passed into the algorithm.

        enum { SEED_LENGTH = 16 }; // Seed length in bytes.
        void reset(xbyte const* seed);
    };

	u64	x_hasher(xbyte const* data, u32 size);

    template <typename K> class xhasher
    {
    public:
        u64 hash(K const& k) const { return 0; }
    };

    template <> class xhasher<s32>
    {
    public:
        u64 hash(s32 const& k) const
        {
            return x_hasher((const xbyte*)&k, sizeof(k));
        }
    };

    template <> class xhasher<u32>
    {
    public:
        u64 hash(u32 const& k) const
        {
            return x_hasher((const xbyte*)&k, sizeof(k));
        }
    };

    template <> class xhasher<s64>
    {
    public:
        u64 hash(s64 const& k) const
        {
            return x_hasher((const xbyte*)&k, sizeof(k));
        }
    };

    template <> class xhasher<u64>
    {
    public:
        u64 hash(u64 const& k) const
        {
            return x_hasher((const xbyte*)&k, sizeof(k));
        }
    };

    template <> class xhasher<f32>
    {
    public:
        u64 hash(f32 const& k) const
        {
            return x_hasher((const xbyte*)&k, sizeof(k));
        }
    };

    template <> class xhasher<f64>
    {
    public:
        u64 hash(f64 const& k) const
        {
            return x_hasher((const xbyte*)&k, sizeof(k));
        }
    };

    template <> class xhasher<void*>
    {
    public:
        u64 hash(void* const& k) const
        {
            return x_hasher((const xbyte*)&k, sizeof(k));
        }
    };
}

#endif // __X_BASE_DEFAULT_HASH_H__