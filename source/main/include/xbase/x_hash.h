#ifndef __X_BASE_DEFAULT_HASH_H__
#define __X_BASE_DEFAULT_HASH_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
    class xhashing
    {
    private:
        u64 d_v0, d_v1, d_v2, d_v3;
        union {
            u64     d_alignment;
            xbyte   d_buf[8];
        };

        s32 d_bufSize;       // The length of the data currently stored in the buffer.
        s32 d_totalLength;   // The total length of all data that has been passed into the algorithm.

        enum { SEED_LENGTH = 16 }; // Seed length in bytes.
        void reset(xbyte const* seed);
        
    public:
        xhashing();
        void reset();
        void hash(const void *data, s32 numBytes);
        u64  finalize();
    };
    static xhashing hasher;

    // Now that we have a btree we can implement other well known data structures.
    // set<V>
    // map<K,V>
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
            s32 data[2] = {k,k};
            hasher.reset();
            hasher.hash((xbyte const*)data, sizeof(u32) * 2);    
            return hasher.finalize();
        }
    };

    template <> class xhasher<u32>
    {
    public:
        u64 hash(u32 const& k) const
        {
            u32 data[2] = {k,k};
            hasher.reset();
            hasher.hash((xbyte const*)data, sizeof(u32) * 2);    
            return hasher.finalize();
        }
    };

    template <> class xhasher<s64>
    {
    public:
        u64 hash(s64 const& k) const
        {
            hasher.reset();
            hasher.hash((xbyte const*)&k, sizeof(u64));    
            return hasher.finalize();
        }
    };

    template <> class xhasher<u64>
    {
    public:
        u64 hash(u64 const& k) const
        {
            hasher.reset();
            hasher.hash((xbyte const*)&k, sizeof(u64));    
            return hasher.finalize();
        }
    };

    template <> class xhasher<f32>
    {
    public:
        u64 hash(f32 const& k) const 
        {
            f32 data[2] = {k,k};
            hasher.reset();
            hasher.hash((xbyte const*)data, sizeof(f32) * 2);    
            return hasher.finalize();
        }
    };

    template <> class xhasher<f64>
    {
    public:
        u64 hash(f64 const& k) const 
        {
            hasher.reset();
            hasher.hash((xbyte const*)&k, sizeof(f64));    
            return hasher.finalize();
        }
    };
}