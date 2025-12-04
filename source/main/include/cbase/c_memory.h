#ifndef __CBASE_MEMORY_STD_H__
#define __CBASE_MEMORY_STD_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_debug.h"
#include "ccore/c_memory.h"

namespace ncore
{
    template <typename T, s32 N>
    inline void g_fill(T (&dst)[N], T v)
    {
        for (s32 i = 0; i < N; i++)
            dst[i] = v;
    }

    template <typename T, s32 N>
    inline void g_copy(T (&dst)[N], T const (&src)[N])
    {
        for (s32 i = 0; i < N; i++)
            dst[i] = src[i];
    }

    ///@description:	Interface/Utility class for MemSet, MemClear, MemCopy, MemMove
    ///@note:			In DEBUG these functions should perform sanity checks on the source and destination memory blocks
    namespace nmem
    {
        ///@name Unaligned memory reading
        u16 const* readunaligned16(u16 const* inAddress, u16& outValue);
        u32 const* readunaligned24(u32 const* inAddress, u32& outValue);
        u32 const* readunaligned32(u32 const* inAddress, u32& outValue);
        u64 const* readunaligned64(u64 const* inAddress, u64& outValue);

        ///@name Unaligned memory writing
        u16* writeunaligned16(u16* inAddress, u16 inData);
        u32* writeunaligned24(u32* inAddress, u32 inData);
        u32* writeunaligned32(u32* inAddress, u32 inData);
        u64* writeunaligned64(u64* inAddress, u64 inData);

        ///@name Unaligned native (little or big endian) memory reading
        u16 const* readnative16(u16 const* inSrc, u16& outValue);
        u32 const* readnative24(u32 const* inSrc, u32& outValue);
        u32 const* readnative32(u32 const* inSrc, u32& outValue);
        u64 const* readnative64(u64 const* inSrc, u64& outValue);

        ///@name Unaligned native (little or big endian) memory writing
        u16* writenative16(u16* inDest, u16 inData);
        u32* writenative24(u32* inDest, u32 inData);
        u32* writenative32(u32* inDest, u32 inData);
        u64* writenative64(u64* inDest, u64 inData);

        /**
         *	Small binary mathematical functions
         *  u8, u16, u32, u64 element access
         */
        inline u8  lou8(u16 inUInt16) { return (inUInt16 & 0xFF); }
        inline u8  hiu8(u16 inUInt16) { return (inUInt16 >> 8); }
        inline u16 lou16(u32 inUInt32) { return (inUInt32 & 0xFFFF); }
        inline u16 hiu16(u32 inUInt32) { return (inUInt32 >> 16); }
        inline u32 hiu32(u64 inUInt64) { return (inUInt64 >> 32); }
        inline u32 lou32(u64 inUInt64) { return ((u32)inUInt64); }

        inline void makeu8(u32 UI32, u8& B32_24, u8& B24_16, u8& B16_8, u8& B8_0)
        {
            B8_0 = (UI32 & 0xFF);
            UI32 >>= 8;
            B16_8 = (UI32 & 0xFF);
            UI32 >>= 8;
            B24_16 = (UI32 & 0xFF);
            UI32 >>= 8;
            B32_24 = (UI32 & 0xFF);
        }
        inline void makeu8(u16 UI16, u8& B16_8, u8& B8_0)
        {
            B8_0 = (UI16 & 0xFF);
            UI16 >>= 8;
            B16_8 = (UI16 & 0xFF);
        }

        inline u16 makeu16(u8 inLow, u8 inHigh) { return (u16)(((u32)inLow | ((u32)inHigh << 8)) & 0xffff); }
        inline u32 makeu32(u16 inLow, u16 inHigh) { return ((u32)inLow | ((u32)inHigh << 16)); }
        inline u32 makeu32(u8 inB0, u8 inB1, u8 inB2, u8 inB3) { return ((u32)inB0 | ((u32)inB1 << 8) | ((u32)inB2 << 16) | ((u32)inB3 << 24)); }
        inline u64 makeu64(u16 inW0, u16 inW1, u16 inW2, u16 inW3) { return (((u64)inW0) | (((u64)inW1) << 16) | (((u64)inW2) << 32) | (((u64)inW3) << 48)); }
        inline u64 makeu64(u32 inLow, u32 inHigh) { return ((u64)inLow | (((u64)inHigh) << 32)); }
        inline u64 makeu64(u8 inB0, u8 inB1, u8 inB2, u8 inB3, u8 inB4, u8 inB5, u8 inB6, u8 inB7)
        {
            return (((u64)inB0) | (((u64)inB1) << 8) | (((u64)inB2) << 16) | (((u64)inB3) << 24) | (((u64)inB4) << 32) | (((u64)inB5) << 40) | (((u64)inB6) << 48) | (((u64)inB7) << 56));
        }
    };  // namespace nmem

}  // namespace ncore
#endif  ///< __C_MEMORY_STD_H__
