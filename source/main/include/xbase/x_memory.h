#ifndef __XBASE_MEMORY_STD_H__
#define __XBASE_MEMORY_STD_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_debug.h"

#ifndef X_NO_CUSTOM_UINT128
#include "xbase/private/x_uint128.h"
#endif

namespace xcore
{
    ///@description:	Interface/Utility class for MemSet, MemClear, MemCopy, MemMove
    ///@note:			In DEBUG these functions should perform sanity checks on the source and destination memory blocks
    namespace xmem
    {

        void*  memset(void* inDest, u32 inValue, uptr inLength);
        void*  memcpy(void* inDest, const void* inSrc, uptr inLength);
        s32   memcmp(const void* inLHS, const void* inRHS, u32 inLength);
        void  memswap(void* inLHS, void* inRHS, u32 inLength);
        void* memmove(void* inDest, const void* inSrc, u32 inLength);

        template <class T> inline void memswap(T& inLHS, T& inRHS) { memswap(&inLHS, &inRHS, sizeof(T)); }
        inline void                    memclr(void* inDest, u32 inLength) { memset(inDest, 0, inLength); }

        ///@name Pointer arithmetic
        inline void* ptr_add(void* ptr, s32 size) { return (void*)((xbyte*)ptr + size); }
        inline void* ptr_add_clamp(void* ptr, s32 size, void* lower, void* upper)
        {
            void* p = (void*)((xbyte*)ptr + size);
            if (p < lower)
                p = lower;
            else if (p > upper)
                p = upper;
            return p;
        }
        inline void* ptr_align(void* ptr, u32 alignment) { return (void*)(((X_PTR_SIZED_INT)ptr + (X_PTR_SIZED_INT)(alignment - 1)) & ~((X_PTR_SIZED_INT)(alignment - 1))); }
        inline s32   ptr_diff(void* ptr, void* other)
        {
            X_PTR_SIZED_INT d = (xbyte*)other - (xbyte*)ptr;
            return (s32)d;
        }

        ///@name Conversion
        inline s64 toKb(s64 inNumBytes) { return (inNumBytes + (s64)512) / (s64)1024; }
        inline s64 toMb(s64 inNumBytes) { return (inNumBytes + (s64)(512 * 1024)) / (s64)(1024 * 1024); }

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
            UI16 >>= 8;
        }

        inline u16 makeu16(u8 inLow, u8 inHigh) { return (inLow | (inHigh << 8)); }
        inline u32 makeu32(u16 inLow, u16 inHigh) { return (inLow | (inHigh << 16)); }
        inline u32 makeu32(u8 inB0, u8 inB1, u8 inB2, u8 inB3) { return ((u32)inB0 | ((u32)inB1 << 8) | ((u32)inB2 << 16) | ((u32)inB3 << 24)); }
        inline u64 makeu64(u16 inW0, u16 inW1, u16 inW2, u16 inW3) { return (((u64)inW0) | (((u64)inW1) << 16) | (((u64)inW2) << 32) | (((u64)inW3) << 48)); }
        inline u64 makeu64(u32 inLow, u32 inHigh) { return ((u64)inLow | (((u64)inHigh) << 32)); }
        inline u64 makeu64(u8 inB0, u8 inB1, u8 inB2, u8 inB3, u8 inB4, u8 inB5, u8 inB6, u8 inB7)
        {
            return (((u64)inB0) | (((u64)inB1) << 8) | (((u64)inB2) << 16) | (((u64)inB3) << 24) | (((u64)inB4) << 32) | (((u64)inB5) << 40) | (((u64)inB6) << 48) | (((u64)inB7) << 56));
        }
    }; // namespace xmem

    template <class T> inline void x_swap(T& a, T& b)
    {
        const T c(a);
        a = b;
        b = c;
    }

    inline void x_memset(void* inDest, u32 inValue, u32 inLength) { xmem::memset(inDest, inValue, inLength); }
    inline void x_memclr(void* inDest, u32 inLength) { xmem::memclr(inDest, inLength); }
    inline void x_memcpy(void* inDest, const void* inSource, u32 inLength) { xmem::memcpy(inDest, inSource, inLength); }
    inline s32  x_memcmp(const void* inLHS, const void* inRHS, u32 inLength) { return xmem::memcmp(inLHS, inRHS, inLength); }
    inline void x_memswap(void* inLHS, void* inRHS, u32 inLength) { xmem::memswap(inLHS, inRHS, inLength); }

    inline s32 x_memcmp2(const u16* inLHS, const u16* inRHS, u32 inLength)
    {
        for (u32 i = 0; i < inLength; i++, inLHS++, inRHS++)
        {
            if (*inLHS < *inRHS)
                return -1;
            else if (*inLHS > *inRHS)
                return 1;
        }
        return 0;
    }
    inline s32 x_memcmp4(const u32* inLHS, const u32* inRHS, u32 inLength)
    {
        for (u32 i = 0; i < inLength; i++, inLHS++, inRHS++)
        {
            if (*inLHS < *inRHS)
                return -1;
            else if (*inLHS > *inRHS)
                return 1;
        }
        return 0;
    }
    inline s32 x_memcmp8(const u64* inLHS, const u64* inRHS, u32 inLength)
    {
        for (u32 i = 0; i < inLength; i++, inLHS++, inRHS++)
        {
            if (*inLHS < *inRHS)
                return -1;
            else if (*inLHS > *inRHS)
                return 1;
        }
        return 0;
    }

    inline void x_memzero(void* inBlock, s32 inLength) { x_memset(inBlock, 0, inLength); }
    inline void x_memcopy2(u16* inDest, const u16* inSource, u32 inLength)
    {
        for (u32 i = 0; i < inLength; i++)
            *inDest++ = *inSource++;
    }
    ///< Copy <inLength> aligned 16-bit integers from <inSource> to <inDest>
    inline void x_memcopy4(u32* inDest, const u32* inSource, u32 inLength)
    {
        for (u32 i = 0; i < inLength; i++)
            *inDest++ = *inSource++;
    }
    ///< Copy <inLength> aligned 32-bit integers from <inSource> to <inDest>
    inline void x_memcopy8(u64* inDest, const u64* inSource, u32 inLength)
    {
        for (u32 i = 0; i < inLength; i++)
            *inDest++ = *inSource++;
    }
    ///< Copy <inLength> aligned 64-bit integers from <inSource> to <inDest>
    inline void x_memcopy16(u128* inDest, const u128* inSource, u32 inLength)
    {
        for (u32 i = 0; i < inLength; i++)
            *inDest++ = *inSource++;
    }
    ///< Copy <inLength> aligned 128-bit integers from <inSource> to <inDest>

    inline void* x_memmove(void* inDest, const void* inSource, u32 inLength) { return xmem::memmove(inDest, inSource, inLength); }
    inline void  x_memcopy(void* inDest, const void* inSource, u32 inLength) { x_memcpy(inDest, inSource, inLength); }
    inline s32   x_memcompare(const void* inBlock1, const void* inBlock2, u32 inLength) { return x_memcmp(inBlock1, inBlock2, inLength); }
    ///< Return the result of the lexicographical compare between memory block <inBlock1> and <inBlock2> of length <inLength>, return <0 if inBlock1<inBlock2, >0 if inBlock1>inBlock2 and 0 if inBlock1==inBlock2
    inline bool x_memequal(const void* inBlock1, const void* inBlock2, u32 inLength) { return (x_memcompare(inBlock1, inBlock2, inLength) == 0); }

} // namespace xcore
#endif ///< __X_MEMORY_STD_H__
