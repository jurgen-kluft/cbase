#include "cbase/c_memory.h"

namespace ncore
{
	namespace xmem
	{
#define	wsize	sizeof(u32)
#define	wmask	(wsize - 1)
		void* memcpy(void* dst0, const void* src0, ptr_t length)
		{
			// Copy a block of memory, handling overlap.
			// This is the routine that actually implements
			// (the portable versions of) bcopy, memcpy, and memmove.

			char *dst = (char *)dst0;
			const char *src = (const char *)src0;
			ptr_t t;

			if (length == 0 || dst == src)		/* nothing to do */
				goto done;

			/*
			* Macros: loop-t-times; and loop-t-times, t>0
			*/
#define	TLOOP(s) if (t) TLOOP1(s)
#define	TLOOP1(s) do { s; } while (--t)

			if (dst < src) {
				/*
				* Copy forward.
				*/
				t = (ptr_t)src;	/* only need low bits */
				if ((t | (ptr_t)dst) & wmask) {
					/*
					* Try to align operands.  This cannot be done
					* unless the low bits match.
					*/
					if ((t ^ (ptr_t)dst) & wmask || length < wsize)
						t = length;
					else
						t = wsize - (t & wmask);
					length -= t;
					TLOOP1(*dst++ = *src++);
				}
				/*
				* Copy whole words, then mop up any trailing bytes.
				*/
				t = length / wsize;
				TLOOP(*(u32 *)dst = *(u32 *)src; src += wsize; dst += wsize);
				t = length & wmask;
				TLOOP(*dst++ = *src++);
			} else {
				/*
				* Copy backwards.  Otherwise essentially the same.
				* Alignment works as before, except that it takes
				* (t&wmask) bytes to align, not wsize-(t&wmask).
				*/
				src += length;
				dst += length;
				t = (ptr_t)src;
				if ((t | (ptr_t)dst) & wmask) {
					if ((t ^ (ptr_t)dst) & wmask || length <= wsize)
						t = length;
					else
						t &= wmask;
					length -= t;
					TLOOP1(*--dst = *--src);
				}
				t = length / wsize;
				TLOOP(src -= wsize; dst -= wsize; *(u32 *)dst = *(u32 *)src);
				t = length & wmask;
				TLOOP(*--dst = *--src);
			}
		done:
			return (dst0);
		}

        void* memset(void* dest, u32 c, ptr_t n) 
		{
			unsigned char *s = (unsigned char *)dest;
			ptr_t k;

			// Fill head and tail with minimal branching. Each
			// conditional ensures that all the subsequently used
			// offsets are well-defined and in the dest region.

			if (!n) return dest;
			s[0] = s[n-1] = c;
			if (n <= 2) return dest;
			s[1] = s[n-2] = c;
			s[2] = s[n-3] = c;
			if (n <= 6) return dest;
			s[3] = s[n-4] = c;
			if (n <= 8) return dest;

			// Advance pointer to align it at a 4-byte boundary,
			// and truncate n to a multiple of 4. The previous code
			// already took care of any head/tail that get cut off
			// by the alignment.

			k = -(ptr_t)s & 3;
			s += k;
			n -= k;
			n &= -4;
			n /= 4;

			u32 *ws = (u32 *)s;
			u32 wc = c & 0xFF;
			wc |= ((wc << 8) | (wc << 16) | (wc << 24));

			// Pure C fallback with no aliasing violations.
			for (; n; n--, ws++) *ws = wc;

			return dest;
		}

        void* memmove(void* inDest, const void* inSrc, u32 inLength) { return memcpy(inDest, inSrc, inLength); }
        
		s32 memcmp(const void* p1, const void* p2, u32 n) 
		{ 
			u32 i;

			if(!p1)
				return 1;
			if(!p2)
				return -1;

			// p1 and p2 are the same memory? easy peasy! bail out
			if(p1 == p2)
			{
				return 0;
			}

			// This for loop does the comparing and pointer moving...
			for(i = 0; (i < n) && (*(u8*)p1 == *(u8*)p2); i++, p1 = 1 + (u8*)p1, p2 = 1 + (u8*)p2)
			{
				// empty body
			}

			// if i == length, then we have passed the test
			return (i == n) ? 0 : (*(u8*)p1 - *(u8*)p2);
		}

        void memswap(void* inLHS, void* inRHS, u32 n)
        {
			char *p1 = (char *)inLHS;
			char *p2 = (char *)inRHS;
			ASSERT(((p1 < p2) && ((p1 + n) < p2)) || ((p2 < p1) && ((p2 + n) < p1)));
            u8* lhs = (u8*)inLHS;
            u8* rhs = (u8*)inRHS;
            for (u32 i = 0; i < n; i++)
            {
                const u8 t = lhs[i];
                lhs[i]     = rhs[i];
                rhs[i]     = t;
            }
        }

        ///@name Unaligned memory reading

        /**
         * @brief		Read an unaligned N-bit unsigned integer in build target processor endian
         *  			mode (little endian in our case!)
         *
         * @desc		This function reads an unaligned N-bit integer from an address. This is
         *	    		guaranteed to be legal, even if the processor normally doesn't allow
         *  			unaligned N-bit reads.
         *
         * Note:		PC and XBOX are little endian and that PS2 and PSP can be set
         *  			to both little or big endian mode (set to little endian in our engine).
         */
        u16 const* readunaligned16(u16 const* inAddress, u16& outValue)
        {
            u8 byte0 = *(const u8*)(inAddress);
            u8 byte1 = *((const u8*)(inAddress) + 1);
            outValue = ((u16)byte0 << 8) | byte1;
            return (u16 const*)((const u8*)(inAddress) + 2);
        }

        u32 const* readunaligned24(u32 const* inAddress, u32& outValue)
        {
            u8 byte0 = *(const u8*)(inAddress);
            u8 byte1 = *((const u8*)(inAddress) + 1);
            u8 byte2 = *((const u8*)(inAddress) + 2);
            outValue = ((u32)byte0 << 16) | ((u32)byte1 << 8) | ((u32)byte2 << 0);
            return (u32 const*)((const u8*)(inAddress) + 3);
        }

        u32 const* readunaligned32(u32 const* inAddress, u32& outValue)
        {
            u8 byte0 = *(const u8*)(inAddress);
            u8 byte1 = *((const u8*)(inAddress) + 1);
            u8 byte2 = *((const u8*)(inAddress) + 2);
            u8 byte3 = *((const u8*)(inAddress) + 3);
            outValue = ((u32)byte0 << 24) | ((u32)byte1 << 16) | ((u32)byte2 << 8) | ((u32)byte3 << 0);
            return (u32 const*)((const u8*)(inAddress) + 4);
        }

        u64 const* readunaligned64(u64 const* inAddress, u64& outValue)
        {
            u8 byte0 = *(const u8*)(inAddress);
            u8 byte1 = *((const u8*)(inAddress) + 1);
            u8 byte2 = *((const u8*)(inAddress) + 2);
            u8 byte3 = *((const u8*)(inAddress) + 3);
            u8 byte4 = *((const u8*)(inAddress) + 4);
            u8 byte5 = *((const u8*)(inAddress) + 5);
            u8 byte6 = *((const u8*)(inAddress) + 6);
            u8 byte7 = *((const u8*)(inAddress) + 7);
            outValue = ((u64)byte0 << 56) | ((u64)byte1 << 48) | ((u64)byte2 << 40) | ((u64)byte3 << 32) | ((u64)byte4 << 24) | ((u64)byte5 << 16) | ((u64)byte6 << 8) | ((u64)byte7 << 0);
            return (u64 const*)((const u8*)(inAddress) + 8);
        }

        ///@name Unaligned memory writing

        /**
         * @brief		Write an unaligned N-bit unsigned integer in build target processor
         *  			endian mode (little endian in our case!)
         *
         * @desc		This function writes an unaligned N-bit integer to an address. This is
         *  			guaranteed to be legal, even if the processor normally doesn't allow
         *  			unaligned N-bit write.
         *
         * Note		PC and XBOX are little endian and that PS2 and PSP can be set
         *			to both little or big endian mode (set to little endian in our engine).
         */
        u16* writeunaligned16(u16* inAddress, u16 inData)
        {
            *(u8*)(inAddress)       = inData >> 8;
            *((u8*)(inAddress) + 1) = inData & 0xff;
            return inAddress + 1;
        }
        u32* writeunaligned24(u32* inAddress, u32 inData)
        {
            *((u8*)(inAddress) + 0) = inData >> 16;
            *((u8*)(inAddress) + 1) = inData >> 8;
            *((u8*)(inAddress) + 2) = inData & 0xff;
            return (u32*)((u8*)(inAddress) + 3);
        }
        u32* writeunaligned32(u32* inAddress, u32 inData)
        {
            *((u8*)(inAddress) + 0) = inData >> 24;
            *((u8*)(inAddress) + 1) = inData >> 16;
            *((u8*)(inAddress) + 2) = inData >> 8;
            *((u8*)(inAddress) + 3) = inData & 0xff;
            return (u32*)((u8*)(inAddress) + 4);
        }
        u64* writeunaligned64(u64* inAddress, u64 inData)
        {
            *((u8*)(inAddress) + 0) = (u8)(inData >> 56) & 0xFF;
            *((u8*)(inAddress) + 1) = (u8)(inData >> 48) & 0xFF;
            *((u8*)(inAddress) + 2) = (u8)(inData >> 40) & 0xFF;
            *((u8*)(inAddress) + 3) = (u8)(inData >> 32) & 0xFF;
            *((u8*)(inAddress) + 4) = (u8)(inData >> 24) & 0xFF;
            *((u8*)(inAddress) + 5) = (u8)(inData >> 16) & 0xFF;
            *((u8*)(inAddress) + 6) = (u8)(inData >> 8) & 0xFF;
            *((u8*)(inAddress) + 7) = (u8)(inData >> 0) & 0xFF;
            return (u64*)((u8*)(inAddress) + 8);
        }

        u16 const* readnative16(u16 const* inSrc, u16& outValue)
        {
#ifdef X_LITTLE_ENDIAN
            outValue = ((u32)((u8*)inSrc)[0]) | ((u32)((u8*)inSrc)[1] << 8);
#else
        outValue = ((u32)((u8*)inSrc)[1] << 8) | ((u32)((u8*)inSrc)[2]);
#endif
            return (u16 const*)((u8*)(inSrc) + 2);
        }

        u32 const* readnative24(u32 const* inSrc, u32& outValue)
        {
#ifdef X_LITTLE_ENDIAN
            outValue = ((u32)((u8*)inSrc)[0]) | ((u32)((u8*)inSrc)[1] << 8) | ((u32)((u8*)inSrc)[2] << 16);
#else
        outValue = ((u32)((u8*)inSrc)[0] << 16) | ((u32)((u8*)inSrc)[1] << 8) | ((u32)((u8*)inSrc)[2]);
#endif
            return (u32 const*)((u8*)(inSrc) + 3);
        }

        u32 const* readnative32(u32 const* inSrc, u32& outValue)
        {
#ifdef X_LITTLE_ENDIAN
            outValue = ((u32)((u8*)inSrc)[0]) | ((u32)((u8*)inSrc)[1] << 8) | ((u32)((u8*)inSrc)[2] << 16) | ((u32)((u8*)inSrc)[3] << 24);
#else
        outValue = ((u32)((u8*)inSrc)[0] << 24) | ((u32)((u8*)inSrc)[1] << 16) | ((u32)((u8*)inSrc)[1] << 8) | ((u32)((u8*)inSrc)[3]);
#endif
            return (u32 const*)((u8 const*)(inSrc) + 4);
        }

        u64 const* readnative64(u64 const* inSrc, u64& outValue)
        {
#ifdef X_LITTLE_ENDIAN
            outValue =
                ((u64)((u8*)inSrc)[0]) | ((u64)((u8*)inSrc)[1] << 8) | ((u64)((u8*)inSrc)[2] << 16) | ((u64)((u8*)inSrc)[3] << 24) | ((u64)((u8*)inSrc)[4] << 32) | ((u64)((u8*)inSrc)[5] << 40) | ((u64)((u8*)inSrc)[6] << 48) | ((u64)((u8*)inSrc)[7] << 56);
#else
        outValue =
            ((u64)((u8*)inSrc)[0] << 56) | ((u64)((u8*)inSrc)[1] << 48) | ((u64)((u8*)inSrc)[2] << 40) | ((u64)((u8*)inSrc)[3] << 32) | ((u64)((u8*)inSrc)[4] << 24) | ((u64)((u8*)inSrc)[5] << 16) | ((u64)((u8*)inSrc)[6] << 8) | ((u64)((u8*)inSrc)[7] << 0);
#endif
            return (u64 const*)((u8 const*)(inSrc) + 8);
        }

        u16* writenative16(u16* inDest, u16 inData)
        {
#ifndef X_LITTLE_ENDIAN
            ((u8*)inDest)[0] = (u8)((inData >> 8) & 0xFF);
            ((u8*)inDest)[1] = (u8)((inData >> 0) & 0xFF);
#else
        ((u8*)inDest)[0] = (u8)((inData >> 0) & 0xFF);
        ((u8*)inDest)[1] = (u8)((inData >> 8) & 0xFF);
#endif
            return (u16*)((u8*)(inDest) + 2);
        }
        u32* writenative24(u32* inDest, u32 inData)
        {
#ifndef X_LITTLE_ENDIAN
            ((u8*)inDest)[0] = (u8)((inData >> 16) & 0xFF);
            ((u8*)inDest)[1] = (u8)((inData >> 8) & 0xFF);
            ((u8*)inDest)[2] = (u8)((inData >> 0) & 0xFF);
#else
        ((u8*)inDest)[0] = (u8)((inData >> 0) & 0xFF);
        ((u8*)inDest)[1] = (u8)((inData >> 8) & 0xFF);
        ((u8*)inDest)[2] = (u8)((inData >> 16) & 0xFF);
#endif
            return (u32*)((u8*)(inDest) + 3);
        }
        u32* writenative32(u32* inDest, u32 inData)
        {
#ifndef X_LITTLE_ENDIAN
            ((u8*)inDest)[0] = (u8)((inData >> 24) & 0xFF);
            ((u8*)inDest)[1] = (u8)((inData >> 16) & 0xFF);
            ((u8*)inDest)[2] = (u8)((inData >> 8) & 0xFF);
            ((u8*)inDest)[3] = (u8)((inData >> 0) & 0xFF);
#else
        ((u8*)inDest)[0] = (u8)((inData >> 0) & 0xFF);
        ((u8*)inDest)[1] = (u8)((inData >> 8) & 0xFF);
        ((u8*)inDest)[2] = (u8)((inData >> 16) & 0xFF);
        ((u8*)inDest)[3] = (u8)((inData >> 24) & 0xFF);
#endif
            return (u32*)((u8*)(inDest) + 4);
        }
        u64* writenative64(u64* inDest, u64 inData)
        {
#ifndef X_LITTLE_ENDIAN
            ((u8*)inDest)[0] = (u8)((inData >> 56) & 0xFF);
            ((u8*)inDest)[1] = (u8)((inData >> 48) & 0xFF);
            ((u8*)inDest)[2] = (u8)((inData >> 40) & 0xFF);
            ((u8*)inDest)[3] = (u8)((inData >> 32) & 0xFF);
            ((u8*)inDest)[4] = (u8)((inData >> 24) & 0xFF);
            ((u8*)inDest)[5] = (u8)((inData >> 16) & 0xFF);
            ((u8*)inDest)[6] = (u8)((inData >> 8) & 0xFF);
            ((u8*)inDest)[7] = (u8)((inData >> 0) & 0xFF);
#else
        ((u8*)inDest)[0] = (u8)((inData >> 0) & 0xFF);
        ((u8*)inDest)[1] = (u8)((inData >> 8) & 0xFF);
        ((u8*)inDest)[2] = (u8)((inData >> 16) & 0xFF);
        ((u8*)inDest)[3] = (u8)((inData >> 24) & 0xFF);
        ((u8*)inDest)[4] = (u8)((inData >> 32) & 0xFF);
        ((u8*)inDest)[5] = (u8)((inData >> 40) & 0xFF);
        ((u8*)inDest)[6] = (u8)((inData >> 48) & 0xFF);
        ((u8*)inDest)[7] = (u8)((inData >> 56) & 0xFF);
#endif
            return (u64*)((u8*)(inDest) + 8);
        }
    } // namespace ncore

} // namespace ncore
