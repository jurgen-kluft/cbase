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
    // total: 60004 + 1876 + 60 + 4 = 61944 bytes

    typedef u32* hbb_t;
    extern u32   sizeof_hbb(u32 maxbits, u32& config); // To know how many u32 to allocate

    extern void init(hbb_t hbb, u32 maxbits, u32 config, s8 bits);
    extern void init(hbb_t& hbb, u32 maxbits, u32& config, s8 bits, alloc_t* alloc);
    extern void release(hbb_t& hbb, alloc_t* alloc);
    extern void reset(hbb_t hbb, u32 config, u32 maxbits, s8 bits);

    extern void set(hbb_t hbb, u32 config, u32 maxbits, u32 bit);
    extern void clr(hbb_t hbb, u32 config, u32 maxbits, u32 bit);

    extern bool is_set(hbb_t hbb, u32 config, u32 maxbits, u32 bit);
    extern bool is_empty(hbb_t hbb);

    extern bool find(hbb_t hbb, u32 config, u32 maxbits, u32& bit);             // First 1
    extern bool upper(hbb_t hbb, u32 config, u32 maxbits, u32 pivot, u32& bit); // First 1 equal to or greater than @pivot
    extern bool lower(hbb_t hbb, u32 config, u32 maxbits, u32 pivot, u32& bit); // First 1 equal to or lesser than @pivot

}; // namespace xcore

#endif /// __X_HIERARCHICAL_BITSET_H__