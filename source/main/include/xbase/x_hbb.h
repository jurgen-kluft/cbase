#ifndef __X_HIERARCHICAL_BITSET_H__
#define __X_HIERARCHICAL_BITSET_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace xcore
{
    class alloc_t;

    // Number of bits and how much memory they consume
    // 32/1Kbit/32Kbit/1Mbit/32Mbit/1Gbit
    //  4/  128/   4KB/128KB/   4MB/128MB ( * ~1.5)

    // Example:
    // When you initialize a hbb_t with maxbits = 512Kbit the
    // size_in_dwords() function will return 16913, or 67652 bytes.
    // level 0, bits= 524288, dwords= 16384, bytes= 65536
    // level 1, bits= 16384, dwords= 512, bytes= 2048
    // level 2, bits= 512, dwords= 16, bytes= 64
    // level 3, bits= 16, dwords= 1, bytes= 4
    // total: 65536 + 2048 + 64 + 4 = 67652 bytes

    struct hbb_t { u32* m_hbb; };
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

    enum
    {
        AllBitsSet = 0xffffffff
    };

    //  3 bits; maximum number of levels = 7
    // 29 bits; maximum number of bits 

    // m_hbb[0] = num-levels(3 bits), num-bits(29 bits)
    // m_hbb[1:1] = level 1 array, u32[1]
    // m_hbb[2:34] = level 0 array, u32[32]        

}; // namespace xcore

#endif /// __X_HIERARCHICAL_BITSET_H__