#ifndef __CBASE_ENDIAN_H__
#define __CBASE_ENDIAN_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nendian_swap
    {
        inline char swap(const char A) { return A; }
        inline u8   swap(const u8 A) { return A; }
        inline s8   swap(const s8 A) { return A; }
        inline u16  swap(const u16 A) { return (A >> 8) | (A << 8); }
        inline u32  swap(const u32 A) { return (A >> 24) | (A << 24) | ((A & 0x00FF0000) >> 8) | ((A & 0x0000FF00) << 8); }
        inline u64  swap(const u64 A)
        {
            return (A >> 56) | (A << 56) | ((A & D_CONSTANT_64(0x00FF000000000000)) >> 40) | ((A & D_CONSTANT_64(0x000000000000FF00)) << 40) | ((A & D_CONSTANT_64(0x0000FF0000000000)) >> 24) | ((A & D_CONSTANT_64(0x0000000000FF0000)) << 24) |
                   ((A & D_CONSTANT_64(0x000000FF00000000)) >> 8) | ((A & D_CONSTANT_64(0x00000000FF000000)) << 8);
        }
        inline s16 swap(const s16 A) { return (s16)swap((u16)A); }
        inline s32 swap(const s32 A) { return (s32)swap((u32)A); }
        inline s64 swap(const s64 A) { return (s64)swap((u64)A); }
        inline f32 swap(const f32 A)
        {
            u32 a = swap(*((u32*)&A));
            return *(f32*)&a;
        }
        inline f64 swap(const f64 A)
        {
            u64 a = swap(*((u64*)&A));
            return *(f64*)&a;
        }
    };  // namespace nendian_swap

    namespace nendian_nop
    {
        inline char swap(const char A) { return A; }
        inline u8   swap(const u8 A) { return A; }
        inline s8   swap(const s8 A) { return A; }
        inline u16  swap(const u16 A) { return A; }
        inline u32  swap(const u32 A) { return A; }
        inline u64  swap(const u64 A) { return A; }
        inline s16  swap(const s16 A) { return A; }
        inline s32  swap(const s32 A) { return A; }
        inline s64  swap(const s64 A) { return A; }
        inline f32  swap(const f32 A) { return A; }
        inline f64  swap(const f64 A) { return A; }
    };  // namespace nendian_nop

    // NOTE: This assumes that data is always stored in Big Endian format.
	// Network byte order is always Big Endian.

#ifdef D_LITTLE_ENDIAN
    namespace nendian    = nendian_swap;
    namespace nendian_ne = nendian_swap;
#else
    namespace nendian    = nendian_nop;
    namespace nendian_ne = nendian_nop;
#endif
};      // namespace ncore

#endif  ///< END __CBASE_ENDIAN_H__
