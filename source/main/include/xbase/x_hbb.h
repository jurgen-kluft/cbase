#ifndef __X_HIERARCHICAL_BITSET_H__
#define __X_HIERARCHICAL_BITSET_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
    class alloc_t;

    // Number of bits and how much memory they consume
    // 32/1Kbit/32Kbit/1Mbit/32Mbit/1Gbit
    //  4/  128/   4KB/128KB/   4MB/128MB ( * ~1.5)

    // Example:
    // When you initialize a hbb_t with maxbits = 480010 the
    // size_in_dwords() function will return (480010+31) / 32 = 15001, or 60004 bytes.
    // level 0, bits= 480010, dwords= 15001, bytes = 60004
    // level 1, bits= 15001, dwords= 469, bytes = 1876
    // level 2, bits= 469, dwords= 15, bytes = 60
    // level 3, bits= 15, dwords= 1, bytes= 4
    // total: 60004 + 1876 + 60 + 4 + 12 = 61956 bytes

    struct hbb_t
    {
        u32* m_hbb;
    };
    static u32 sizeof_hbb(u32 maxbits); // To know how many u32 to allocate

    static void init(hbb_t hbb, u32 numbits);
    static void init(hbb_t& hbb, u32 maxbits, alloc_t* alloc);
    static void resize(hbb_t& hbb, u32 maxbits, alloc_t* alloc);
    static void release(hbb_t hbb, alloc_t* alloc);
    static void reset(hbb_t hbb);

    static void set(hbb_t hbb, u32 bit);
    static void clr(hbb_t hbb, u32 bit);

    static bool is_set(hbb_t hbb, u32 bit);
    static bool is_full(hbb_t hbb);

    static bool find(hbb_t hbb, u32& bit);             // First 0 or 1
    static bool upper(hbb_t hbb, u32 pivot, u32& bit); // First 0 or 1 equal to or greater than @pivot
    static bool lower(hbb_t hbb, u32 pivot, u32& bit); // First 0 or 1 equal to or lesser than @pivot

    // can be in .cpp (not public)
    enum
    {
        AllBitsSet = 0xffffffff
    };
    // each level -> u32[level len]
    // m_hbb[0] = num-bits (level 1 len = 1)
    // m_hbb[1] = level 2 len(5 bits), level 3 len(10 bits), level 4 len(15 bits)
    // m_hbb[2] = level 5 len(20 bits)
    static u32 sizeof_level(hbb_t hbb, s8 level);

}; // namespace xcore

#endif /// __X_HIERARCHICAL_BITSET_H__