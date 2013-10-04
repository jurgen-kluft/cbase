#include "xbase\x_memory_std.h"

#ifdef TARGET_360
	#include <Xtl.h>
#endif
#ifdef TARGET_PC
	#include <stdlib.h>
	#include <memory.h>
	#include <string.h>
#endif
#ifdef TARGET_PS3
	#include <string.h>
#endif

#ifdef TARGET_WII
	#include <revolution/os.h>
	#include <string.h>
#endif
#ifdef TARGET_3DS
	#include <nn/os.h>
	#include <string.h>
#endif

namespace xcore
{
#ifdef TARGET_WII
	/**
	 * Memory copy
	 */
	void xmem_utils::memcpy(void *dest, const void *src, u32 count)
	{
		::memcpy(dest, src, count);
	}

	/**
	 * Sets the first count characters of dest to the character c.
	 */
	void xmem_utils::memset(void* buf, u32 c, u32 inLength)
	{
		::memset(buf, c, inLength);
	}

	//------------------------------------------------------------------------------
	void* xmem_utils::memmove(void* inDest, const void* inSrc, u32 inLength)
	{
		return ::memmove(inDest, inSrc, inLength);
	}

	/**
	 * Compares the first count characters of buf1 and buf2 and
	 * \returns a value indicating their relationship.
	 */
	s32 xmem_utils::memcmp(const void* bufPtr1, const void* bufPtr2, u32 inLength)
	{
		return ::memcmp(bufPtr1, bufPtr2, inLength);
	}
#endif

#ifdef TARGET_PC
	/**
	 * Memory copy
	 */
	void xmem_utils::memcpy(void *dest, const void *src, u32 count)
	{
		::memcpy(dest, src, count);
	}

	/**
	 * Sets the first count characters of dest to the character c.
	 */
	void xmem_utils::memset(void* buf, u32 c, u32 inLength)
	{
		::memset(buf, c, inLength);
	}

	//------------------------------------------------------------------------------
	void* xmem_utils::memmove(void* inDest, const void* inSrc, u32 inLength)
	{
		return ::memmove(inDest, inSrc, inLength);
	}

	/**
	 * Compares the first count characters of buf1 and buf2 and
	 * \returns a value indicating their relationship.
	 */
	s32 xmem_utils::memcmp(const void* bufPtr1, const void* bufPtr2, u32 inLength)
	{
		return ::memcmp(bufPtr1, bufPtr2, inLength);
	}
#endif
#if defined(TARGET_PS3)

	/**
	 * Memory copy
	 */
	void xmem_utils::memcpy(void *dest, const void *src, u32 count)
	{
		::memcpy(dest, src, count);
	}

	/**
	 * Sets the first count characters of inDest to the character c.
	 */
	void xmem_utils::memset(void* inDest, u32 c, u32 inLength)
	{
		::memset(inDest, c, inLength);
	}

	//------------------------------------------------------------------------------
	void* xmem_utils::memmove(void* inDest, const void* inSrc, u32 inLength)
	{
		return ::memmove(inDest, inSrc, inLength);
	}

	/**
	 * Compares the first count characters of buf1 and buf2 and
	 * \returns a value indicating their relationship.
	 */
	s32 xmem_utils::memcmp(const void* bufPtr1, const void* bufPtr2, u32 inLength)
	{
		return ::memcmp(bufPtr1, bufPtr2, inLength);
	}
#endif

#ifdef TARGET_360

	/*
	 * Memory copy
	 */
	void xmem_utils::memcpy(void *dest, const void *src, u32 count)
	{
		::memcpy(dest, src, count);
	}

	/**
	 * Sets the first count characters of dest to the character c.
	 */
	void xmem_utils::memset(void* buf, u32 c, u32 inLength)
	{
		::memset(buf, c, inLength);
	}

	//------------------------------------------------------------------------------
	void* xmem_utils::memmove(void* inDest, const void* inSrc, u32 inLength)
	{
		return ::memmove(inDest, inSrc, inLength);
	}

	/**
	 * Compares the first count characters of buf1 and buf2 and
	 * \returns a value indicating their relationship.
	 */
	s32 xmem_utils::memcmp(const void* bufPtr1, const void* bufPtr2, u32 inLength)
	{
		return ::memcmp(bufPtr1, bufPtr2, inLength);
	}
#endif

#ifdef TARGET_3DS

	/**
	 * Memory copy
	 */
	void xmem_utils::memcpy(void *dest, const void *src, u32 count)
	{
		::memcpy(dest, src, count);
	}

	/**
	 * Sets the first count characters of inDest to the character c.
	 */
	void xmem_utils::memset(void* inDest, u32 c, u32 inLength)
	{
		::memset(inDest, c, inLength);
	}

	//------------------------------------------------------------------------------
	void* xmem_utils::memmove(void* inDest, const void* inSrc, u32 inLength)
	{
		return ::memmove(inDest, inSrc, inLength);
	}

	/**
	 * Compares the first count characters of buf1 and buf2 and
	 * \returns a value indicating their relationship.
	 */
	s32 xmem_utils::memcmp(const void* bufPtr1, const void* bufPtr2, u32 inLength)
	{
		return ::memcmp(bufPtr1, bufPtr2, inLength);
	}
#endif

	void			xmem_utils::memswap(void* inLHS, const void* inRHS, u32 inLength)
	{
		u8* lhs = (u8*)inLHS;
		u8* rhs = (u8*)inRHS;
		for (u32 i=0; i<inLength; i++)
		{
			const u8 t = lhs[i];
			lhs[i] = rhs[i];
			rhs[i] = t;
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
	u16 const*	xmem_utils::readunaligned16(u16 const* inAddress, u16& outValue)
	{
		u8 byte0 = *(const u8*)(inAddress);
		u8 byte1 = *((const u8*)(inAddress)+1);
		outValue = ((u16)byte0<<8)|byte1;
		return (u16 const*)((const u8*)(inAddress)+2);
	}

	u32 const*	xmem_utils::readunaligned24(u32 const* inAddress, u32& outValue)
	{
		u8 byte0 = *(const u8*)(inAddress);
		u8 byte1 = *((const u8*)(inAddress)+1);
		u8 byte2 = *((const u8*)(inAddress)+2);
		outValue = ((u32)byte0<<16) | ((u32)byte1<<8) | ((u32)byte2<<0);
		return (u32 const*)((const u8*)(inAddress)+3);
	}

	u32 const*	xmem_utils::readunaligned32(u32 const* inAddress, u32& outValue)
	{
		u8 byte0 = *(const u8*)(inAddress);
		u8 byte1 = *((const u8*)(inAddress)+1);
		u8 byte2 = *((const u8*)(inAddress)+2);
		u8 byte3 = *((const u8*)(inAddress)+3);
		outValue = ((u32)byte0<<24) | ((u32)byte1<<16) | ((u32)byte2<<8) | ((u32)byte3<<0);
		return (u32 const*)((const u8*)(inAddress)+4);
	}

	u64 const*	xmem_utils::readunaligned64(u64 const* inAddress, u64& outValue)
	{
		u8 byte0 = *(const u8*)(inAddress);
		u8 byte1 = *((const u8*)(inAddress)+1);
		u8 byte2 = *((const u8*)(inAddress)+2);
		u8 byte3 = *((const u8*)(inAddress)+3);
		u8 byte4 = *((const u8*)(inAddress)+4);
		u8 byte5 = *((const u8*)(inAddress)+5);
		u8 byte6 = *((const u8*)(inAddress)+6);
		u8 byte7 = *((const u8*)(inAddress)+7);
		outValue = ((u64)byte0<<56) | ((u64)byte1<<48) | ((u64)byte2<<40) | ((u64)byte3<<32) | ((u64)byte4<<24) | ((u64)byte5<<16) | ((u64)byte6<<8) | ((u64)byte7<<0);
		return (u64 const*)((const u8*)(inAddress)+8);
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
	u16*			xmem_utils::writeunaligned16(u16* inAddress, u16 inData)
	{
		*(u8*)(inAddress)= inData>>8;
		*((u8*)(inAddress)+1)= inData&0xff;
		return inAddress + 1;
	}
	u32*			xmem_utils::writeunaligned24(u32* inAddress, u32 inData)
	{
		*((u8*)(inAddress)+0)= inData>>16;
		*((u8*)(inAddress)+1)= inData>>8;
		*((u8*)(inAddress)+2)= inData&0xff;
		return (u32*)((u8*)(inAddress)+3);
	}
	u32*			xmem_utils::writeunaligned32(u32* inAddress, u32 inData)
	{
		*((u8*)(inAddress)+0)= inData>>24;
		*((u8*)(inAddress)+1)= inData>>16;
		*((u8*)(inAddress)+2)= inData>> 8;
		*((u8*)(inAddress)+3)= inData&0xff;
		return (u32*)((u8*)(inAddress)+4);
	}
	u64*			xmem_utils::writeunaligned64(u64* inAddress, u64 inData)
	{
		*((u8*)(inAddress)+0)= (u8)(inData>>56) & 0xFF;
		*((u8*)(inAddress)+1)= (u8)(inData>>48) & 0xFF;
		*((u8*)(inAddress)+2)= (u8)(inData>>40) & 0xFF;
		*((u8*)(inAddress)+3)= (u8)(inData>>32) & 0xFF;
		*((u8*)(inAddress)+4)= (u8)(inData>>24) & 0xFF;
		*((u8*)(inAddress)+5)= (u8)(inData>>16) & 0xFF;
		*((u8*)(inAddress)+6)= (u8)(inData>> 8) & 0xFF;
		*((u8*)(inAddress)+7)= (u8)(inData>> 0) & 0xFF;
		return (u64*)((u8*)(inAddress)+8);
	}


	u16 const*		xmem_utils::readnative16(u16 const* inSrc, u16& outValue)
	{
#ifdef X_LITTLE_ENDIAN
		outValue = ((u32)((u8*)inSrc)[0])|((u32)((u8*)inSrc)[1]<<8);
#else
		outValue = ((u32)((u8*)inSrc)[1]<<8)|((u32)((u8*)inSrc)[2]);
#endif
		return (u16 const*)((u8*)(inSrc)+2);
	}

	u32 const*		xmem_utils::readnative24(u32 const* inSrc, u32& outValue)
	{
#ifdef X_LITTLE_ENDIAN
		outValue = ((u32)((u8*)inSrc)[0])|((u32)((u8*)inSrc)[1]<<8)|((u32)((u8*)inSrc)[2]<<16);
#else
		outValue = ((u32)((u8*)inSrc)[0]<<16)|((u32)((u8*)inSrc)[1]<<8)|((u32)((u8*)inSrc)[2]);
#endif
		return (u32 const*)((u8*)(inSrc)+3);
	}

	u32 const*		xmem_utils::readnative32(u32 const* inSrc, u32& outValue)
	{
#ifdef X_LITTLE_ENDIAN
		outValue = ((u32)((u8*)inSrc)[0])|((u32)((u8*)inSrc)[1]<<8)|((u32)((u8*)inSrc)[2]<<16)|((u32)((u8*)inSrc)[3]<<24);
#else
		outValue = ((u32)((u8*)inSrc)[0]<<24)|((u32)((u8*)inSrc)[1]<<16)|((u32)((u8*)inSrc)[1]<<8)|((u32)((u8*)inSrc)[3]);
#endif
		return (u32 const*)((u8 const*)(inSrc)+4);
	}

	u64 const*		xmem_utils::readnative64(u64 const* inSrc, u64& outValue)
	{
#ifdef X_LITTLE_ENDIAN
		outValue = ((u64)((u8*)inSrc)[0]    )|((u64)((u8*)inSrc)[1]<< 8)|((u64)((u8*)inSrc)[2]<<16)|((u64)((u8*)inSrc)[3]<<24)|((u64)((u8*)inSrc)[4]<<32)|((u64)((u8*)inSrc)[5]<<40)|((u64)((u8*)inSrc)[6]<<48)|((u64)((u8*)inSrc)[7]<<56);
#else
		outValue = ((u64)((u8*)inSrc)[0]<<56)|((u64)((u8*)inSrc)[1]<<48)|((u64)((u8*)inSrc)[2]<<40)|((u64)((u8*)inSrc)[3]<<32)|((u64)((u8*)inSrc)[4]<<24)|((u64)((u8*)inSrc)[5]<<16)|((u64)((u8*)inSrc)[6]<< 8)|((u64)((u8*)inSrc)[7]<< 0);
#endif
		return (u64 const*)((u8 const*)(inSrc)+8);
	}


	u16*			xmem_utils::writenative16(u16* inDest, u16 inData)
	{
#ifndef X_LITTLE_ENDIAN
		((u8*)inDest)[0] = (u8)((inData >>  8) & 0xFF);
		((u8*)inDest)[1] = (u8)((inData >>  0) & 0xFF);
#else
		((u8*)inDest)[0] = (u8)((inData >>  0) & 0xFF);
		((u8*)inDest)[1] = (u8)((inData >>  8) & 0xFF);
#endif
		return (u16*)((u8*)(inDest)+2);
	}
	u32*			xmem_utils::writenative24(u32* inDest, u32 inData)
	{
#ifndef X_LITTLE_ENDIAN
		((u8*)inDest)[0] = (u8)((inData >> 16) & 0xFF);
		((u8*)inDest)[1] = (u8)((inData >>  8) & 0xFF);
		((u8*)inDest)[2] = (u8)((inData >>  0) & 0xFF);
#else
		((u8*)inDest)[0] = (u8)((inData >>  0) & 0xFF);
		((u8*)inDest)[1] = (u8)((inData >>  8) & 0xFF);
		((u8*)inDest)[2] = (u8)((inData >> 16) & 0xFF);
#endif
		return (u32*)((u8*)(inDest)+3);
	}
	u32*			xmem_utils::writenative32(u32* inDest, u32 inData)
	{
#ifndef X_LITTLE_ENDIAN
		((u8*)inDest)[0] = (u8)((inData >> 24) & 0xFF);
		((u8*)inDest)[1] = (u8)((inData >> 16) & 0xFF);
		((u8*)inDest)[2] = (u8)((inData >>  8) & 0xFF);
		((u8*)inDest)[3] = (u8)((inData >>  0) & 0xFF);
#else
		((u8*)inDest)[0] = (u8)((inData >>  0) & 0xFF);
		((u8*)inDest)[1] = (u8)((inData >>  8) & 0xFF);
		((u8*)inDest)[2] = (u8)((inData >> 16) & 0xFF);
		((u8*)inDest)[3] = (u8)((inData >> 24) & 0xFF);
#endif
		return (u32*)((u8*)(inDest)+4);
	}
	u64*			xmem_utils::writenative64(u64* inDest, u64 inData)
	{
#ifndef X_LITTLE_ENDIAN
		((u8*)inDest)[0] = (u8)((inData >> 56) & 0xFF);
		((u8*)inDest)[1] = (u8)((inData >> 48) & 0xFF);
		((u8*)inDest)[2] = (u8)((inData >> 40) & 0xFF);
		((u8*)inDest)[3] = (u8)((inData >> 32) & 0xFF);
		((u8*)inDest)[4] = (u8)((inData >> 24) & 0xFF);
		((u8*)inDest)[5] = (u8)((inData >> 16) & 0xFF);
		((u8*)inDest)[6] = (u8)((inData >>  8) & 0xFF);
		((u8*)inDest)[7] = (u8)((inData >>  0) & 0xFF);
#else
		((u8*)inDest)[0] = (u8)((inData >>  0) & 0xFF);
		((u8*)inDest)[1] = (u8)((inData >>  8) & 0xFF);
		((u8*)inDest)[2] = (u8)((inData >> 16) & 0xFF);
		((u8*)inDest)[3] = (u8)((inData >> 24) & 0xFF);
		((u8*)inDest)[4] = (u8)((inData >> 32) & 0xFF);
		((u8*)inDest)[5] = (u8)((inData >> 40) & 0xFF);
		((u8*)inDest)[6] = (u8)((inData >> 48) & 0xFF);
		((u8*)inDest)[7] = (u8)((inData >> 56) & 0xFF);
#endif
		return (u64*)((u8*)(inDest)+8);
	}

}
