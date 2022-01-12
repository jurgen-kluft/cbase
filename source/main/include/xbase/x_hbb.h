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
    // 32 bits
    //  = 4 bytes
    // 1 K bits
    //  = 128+4 byte
    // 32 K bits
    //  = 4 K + 128 + 4 bytes
    // 1 M bits
    //  = 128 K + 4 K + 128 + 4
    // 32 M bits
    //  = 4 M + 128 K + 4 K + 128 + 4

    // NOTE: Maximum number of bits we can manage with the functions below = 32 M bits

    typedef u32* hbb_t;
    extern u32  g_sizeof_hbb(u32 maxbits, u32& config); // To know how many u32 to allocate

    extern void g_hbb_init(hbb_t hbb, u32 maxbits, u32 config, s8 bits);
    extern void g_hbb_init(hbb_t& hbb, u32 maxbits, u32& config, s8 bits, alloc_t* alloc);
    extern void g_hbb_release(hbb_t& hbb, alloc_t* alloc);
    extern void g_hbb_reset(hbb_t hbb, u32 config, u32 maxbits, s8 bits);

    extern void g_hbb_set(hbb_t hbb, u32 config, u32 maxbits, u32 bit);
    extern void g_hbb_clr(hbb_t hbb, u32 config, u32 maxbits, u32 bit);

    extern bool g_hbb_is_set(hbb_t hbb, u32 config, u32 maxbits, u32 bit);
    extern bool g_hbb_is_empty(hbb_t hbb);

    extern bool g_hbb_find(hbb_t hbb, u32 config, u32 maxbits, u32& bit);
    extern bool g_hbb_upper(hbb_t hbb, u32 config, u32 maxbits, u32 pivot, u32& bit); // First 1 equal to or greater than @pivot
    extern bool g_hbb_lower(hbb_t hbb, u32 config, u32 maxbits, u32 pivot, u32& bit); // First 1 equal to or lesser than @pivot

}; // namespace xcore

#endif /// __X_HIERARCHICAL_BITSET_H__