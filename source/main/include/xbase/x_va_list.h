/**
* @file Head file x_va_list.h
* Core Variable Argument List
*/

#ifndef __XBASE_CORE_VA_LIST_H__
#define __XBASE_CORE_VA_LIST_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================


/**
 * xCore namespace
 */
namespace xcore
{
	//==============================================================================
	//  FORWARD DECLARES
	//==============================================================================
	class xstring_tmp;
	class xstring;

	/** 
     *------------------------------------------------------------------------------
	 * Author:
	 *     Jurgen Kluft
	 * Description:
	 *     x_va_list using x_va is a type safe implementation of the variable argument
	 *     functionality that is provided by C/C++. The disadvantage of the variable
	 *     argument functionality is that it is not 'type safe'. That is why we have
	 *     overridden this by using a custom va_list.
	 *
	 *<P>  The printf and sprintf as we all know can still be used as we have always
	 *     done, there are no changes in their use. The only main difference is that
	 *     these functions have become 'type safe'. This means that if you have specified
	 *     %s in the format string and have passed a variable which is NOT a string
	 *     then the printf code will not crash and it will display NULL. For some types
	 *     type conversion is supported, like s32 to float, float to s32 etc.
	 *
	 *<CODE>
	 *      printf("This is a test, %s, %d, %f", "hello", 2009, 3.1415f);
	 *      sprintf(dest, "This is a test, %s, %d, %f", "hello", 2009, 3.1415f);
	 *</CODE>
	 *
	 *<P>  When you specify a %d or %u and have passed a 'const char*', the pointer value
	 *     will be displayed instead of the string.
	 *
	 *------------------------------------------------------------------------------
	 */
	class x_va
	{
		enum EProperty
		{
			TYPE_INTEGER  = 0x80000000,
			TYPE_SIGNED   = 0x40000000,
			TYPE_UNSIGNED = 0x20000000,
			TYPE_FLOAT    = 0x10000000,
		};
		enum ESize
		{
			SIZE_MASK     = 0x0FF00000,
			SIZE_SHIFT    = 20,
			SIZE_8        = 0x00800000,
			SIZE_16       = 0x01000000,
			SIZE_32       = 0x02000000,
			SIZE_64       = 0x04000000,
		};
	public:
		enum EType
		{
			TYPE_EMPTY    = 0,

			TYPE_INT      = 0x001 | TYPE_INTEGER | TYPE_SIGNED | SIZE_32,
			TYPE_INT8     = 0x002 | TYPE_INTEGER | TYPE_SIGNED | SIZE_8,
			TYPE_INT16    = 0x004 | TYPE_INTEGER | TYPE_SIGNED | SIZE_16,
			TYPE_INT32    = 0x008 | TYPE_INTEGER | TYPE_SIGNED | SIZE_32,
			TYPE_INT64    = 0x010 | TYPE_INTEGER | TYPE_SIGNED | SIZE_64,
			TYPE_UINT     = 0x001 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_32,
			TYPE_UINT8    = 0x002 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_8,
			TYPE_UINT16   = 0x004 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_16,
			TYPE_UINT32   = 0x008 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_32,
			TYPE_UINT64   = 0x010 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_64,
			TYPE_BOOL     = 0x020 | TYPE_INTEGER | TYPE_SIGNED | SIZE_32,
			TYPE_FLOAT32  = 0x081 | TYPE_FLOAT | SIZE_32,
			TYPE_FLOAT64  = 0x082 | TYPE_FLOAT | SIZE_64,
			TYPE_PCTCHAR  = 0x100 | SIZE_32
		};

	public:
								x_va() : mType(TYPE_EMPTY)							{ }
								x_va(const x_va& c) : mType(c.mType)				{ *(u64*)mArg = *(u64*)c.mArg; }
								x_va(s8 inVar) : mType(TYPE_INT8)					{ *(s8*)mArg = inVar; }
								x_va(u8 inVar) : mType(TYPE_UINT8)					{ *(u8*)mArg = inVar; }
								x_va(s16 inVar) : mType(TYPE_INT16)					{ *(s16*)mArg = inVar; }
								x_va(u16 inVar) : mType(TYPE_UINT16)				{ *(u16*)mArg = inVar; }
								x_va(s32 inVar) : mType(TYPE_INT32)					{ *(s32*)mArg = inVar; }
								x_va(u32 inVar) : mType(TYPE_UINT32)				{ *(u32*)mArg = inVar; }
								x_va(s64 inVar) : mType(TYPE_INT64)					{ *(s64*)mArg = inVar; }
								x_va(u64 inVar) : mType(TYPE_UINT64)				{ *(u64*)mArg = inVar; }
								x_va(bool inVar) : mType(TYPE_BOOL)					{ *(u32*)mArg = inVar ? 1 : 0; }
								x_va(f32 inVar) : mType(TYPE_FLOAT32)				{ *(f32*)mArg = inVar; }
								x_va(f64 inVar) : mType(TYPE_FLOAT64)				{ *(f64*)mArg = inVar; }
								x_va(const char* inVar) : mType(TYPE_PCTCHAR)		{ *(const char**)mArg = inVar; }

		EType					type() const										{ return mType; }

		s32						sizeInBits() const									{ return (mType&SIZE_MASK)>>SIZE_SHIFT; }
		s32						sizeInBytes() const									{ return ((mType&SIZE_MASK)>>SIZE_SHIFT) >> 3; }

		xbool					isInteger() const									{ return xbool((mType&TYPE_INTEGER)==TYPE_INTEGER && sizeInBits()==8); }
		xbool					isSignedInteger() const								{ return xbool(isInteger() && ((mType&TYPE_SIGNED)==TYPE_SIGNED)); }
		xbool					isUnsignedInteger() const							{ return xbool(isInteger() && ((mType&TYPE_UNSIGNED)==TYPE_UNSIGNED)); }

		xbool					isBool() const										{ return xbool(mType == TYPE_BOOL); }
		xbool					isInt8() const										{ return xbool(isInteger() && isSignedInteger() && sizeInBits()==8); }
		xbool					isInt16() const										{ return xbool(isInteger() && isSignedInteger() && sizeInBits()==16); }
		xbool					isInt32() const										{ return xbool(isInteger() && isSignedInteger() && sizeInBits()==32); }
		xbool					isInt64() const										{ return xbool(isInteger() && isSignedInteger() && sizeInBits()==64); }
		xbool					isUInt8() const										{ return xbool(isInteger() && isUnsignedInteger() && sizeInBits()==8); }
		xbool					isUInt16() const									{ return xbool(isInteger() && isUnsignedInteger() && sizeInBits()==16); }
		xbool					isUInt32() const									{ return xbool(isInteger() && isUnsignedInteger() && sizeInBits()==32); }
		xbool					isUInt64() const									{ return xbool(isInteger() && isUnsignedInteger() && sizeInBits()==64); }
		xbool					isF32() const										{ return xbool((mType&TYPE_FLOAT32)==TYPE_FLOAT32); }
		xbool					isF64() const										{ return xbool((mType&TYPE_FLOAT64)==TYPE_FLOAT64); }
		xbool					isPCTChar() const									{ return xbool((mType&TYPE_PCTCHAR)==TYPE_PCTCHAR); }

		operator				char() const										{ return (char)convertToInt8(); }
		operator				s8() const											{ return convertToInt8(); }
		operator				u8() const											{ return convertToUInt8(); }
		operator				s16() const											{ return convertToInt16(); }
		operator				u16() const											{ return convertToUInt16(); }
		operator				s32() const											{ return convertToInt32(); }
		operator				u32() const											{ return convertToUInt32(); }
		operator				s64() const											{ return convertToInt64(); }
		operator				u64() const											{ return convertToUInt64(); }
		operator				f32() const											{ return convertToFloat(); }
		operator				f64() const											{ return convertToDouble(); }
		operator				bool() const										{ return convertToBool(); }
		operator				const char*() const									{ return convertToCharPointer(); }

		static const x_va		sEmpty;

	protected:
		s8						convertToInt8() const;
		u8						convertToUInt8() const;
		s16						convertToInt16() const;
		u16						convertToUInt16() const;
		s32						convertToInt32() const;
		u32						convertToUInt32() const;
		s64						convertToInt64() const;
		u64						convertToUInt64() const;
		f32						convertToFloat() const;
		f64						convertToDouble() const;
		bool					convertToBool() const;
		const char*				convertToCharPointer() const;

		EType					mType;
		u8						mArg[8];
	};


	class x_va_list
	{
	public:
		typedef		const x_va&		rcva;

		x_va_list() : mLength(0)													{ }
		x_va_list(rcva a1) : mLength(1)												{ mArg[0]=a1; }
		x_va_list(rcva a1, rcva a2) : mLength(2)									{ mArg[0]=a1; mArg[1]=a2; }
		x_va_list(rcva a1, rcva a2, rcva a3) : mLength(3)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4) : mLength(4)					{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5) : mLength(5)				{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6) : mLength(6)		{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7) : mLength(7)	{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8) : mLength(8)	{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9) : mLength(9)	{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10) : mLength(10)	{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11) : mLength(11)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12) : mLength(12)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13) : mLength(13)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; mArg[12]=a13; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13, rcva a14) : mLength(14)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; mArg[12]=a13; mArg[13]=a14; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13, rcva a14, rcva a15) : mLength(15)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; mArg[12]=a13; mArg[13]=a14; mArg[14]=a15; }
		x_va_list(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13, rcva a14, rcva a15, rcva a16) : mLength(16)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; mArg[12]=a13; mArg[13]=a14; mArg[14]=a15; mArg[15]=a16; }

		x_va_list(rcva a1, const x_va_list& inList) : mLength(1)					{ mArg[0] = a1; add(inList); }
		x_va_list(rcva a1, rcva a2, const x_va_list& inList) : mLength(2)			{ mArg[0] = a1; mArg[1] = a2; add(inList); }

		s32					length() const											{ return mLength; }

		xbool				add(const x_va& inArg1)									{ if (mLength<11) { mArg[mLength++] = inArg1; return xTRUE; } return xFALSE; }
		void				add(const x_va_list& inList)							{ for (s32 j=mLength, i=0; i<inList.mLength && j<11; ++i, ++j) mArg[j] = inList[i]; }

		const x_va&			operator[](s32 inIndex) const							{ if (inIndex>=0 && inIndex<mLength) return mArg[inIndex]; else return x_va::sEmpty; }

	protected:
		s32					mLength;
		x_va				mArg[16];
	};


	/** 
     *------------------------------------------------------------------------------
	 * Author:
	 *     Jurgen Kluft
	 * Description:
	 *     x_va_r_list using x_va_r is a type safe implementation of 'pointer to argument'
	 *     passing used by sscanf.
	 *
	 *<P>  The sscanf as we all know can be used as we have always done, there are no 
	 *     changes in the interface and it's use. The only main difference is that
	 *     this function has become 'type safe'. This means that if you have specified
	 *     %s in the format string and have passed a variable which is NOT a string
	 *     then the sscanf code will not crash and in some cases it will convert the type
	 *     it reads to the type the user has supplied (e.g. float to int). 
	 *
	 *<CODE>
	 *      const char* myString;
	 *      s32 myInteger;
	 *      f32 myFloat;
	 *      sscanf("This is a test, hello, 2009, 3.1415", "This is a test, %s, %d, %f", myString, myInteger, myFloat);
	 *</CODE>
	 *
	 * An example of automatic conversion (int->float, float->int):
	 * 
	 *<CODE>
	 *      s32 myInteger;
	 *      f32 myFloat;
	 *      sscanf("This is a test, 2009, 3.1415", "This is a test, %d, %f", myFloat, myInteger);
	 *</CODE>
	 *
	 *------------------------------------------------------------------------------
	 */
	class x_va_r
	{
		enum EProperty
		{
			TYPE_INTEGER  = 0x80000000,
			TYPE_SIGNED   = 0x40000000,
			TYPE_UNSIGNED = 0x20000000,
			TYPE_FLOAT    = 0x10000000,
		};
		enum ESize
		{
			SIZE_MASK     = 0x0FF00000,
			SIZE_SHIFT    = 20,
			SIZE_8        = 0x00800000,
			SIZE_16       = 0x01000000,
			SIZE_32       = 0x02000000,
			SIZE_64       = 0x04000000,
		};
	public:
		enum EType
		{
			TYPE_EMPTY    = 0,

			TYPE_INT      = 0x001 | TYPE_INTEGER | TYPE_SIGNED | SIZE_32,
			TYPE_INT8     = 0x002 | TYPE_INTEGER | TYPE_SIGNED | SIZE_8,
			TYPE_INT16    = 0x004 | TYPE_INTEGER | TYPE_SIGNED | SIZE_16,
			TYPE_INT32    = 0x008 | TYPE_INTEGER | TYPE_SIGNED | SIZE_32,
			TYPE_INT64    = 0x010 | TYPE_INTEGER | TYPE_SIGNED | SIZE_64,
			TYPE_UINT     = 0x001 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_32,
			TYPE_UINT8    = 0x002 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_8,
			TYPE_UINT16   = 0x004 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_16,
			TYPE_UINT32   = 0x008 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_32,
			TYPE_UINT64   = 0x010 | TYPE_INTEGER | TYPE_UNSIGNED | SIZE_64,
			TYPE_BOOL     = 0x020 | SIZE_32,
			TYPE_FLOAT32  = 0x081 | TYPE_FLOAT | SIZE_32,
			TYPE_FLOAT64  = 0x082 | TYPE_FLOAT | SIZE_64,
			TYPE_STRING   = 0x100
		};
	public:
								x_va_r() : mType(TYPE_EMPTY), mVar(0), mRef(NULL)				{ }
								x_va_r(const x_va_r& c) : mType(c.mType), mVar(0)				{ mRef = c.mRef; }
								x_va_r(s8* inRef) : mType(TYPE_INT8), mVar(0)					{ mRef = inRef; }
								x_va_r(u8* inRef) : mType(TYPE_UINT8), mVar(0)					{ mRef = inRef; }
								x_va_r(s16* inRef) : mType(TYPE_INT16), mVar(0)					{ mRef = inRef; }
								x_va_r(u16* inRef) : mType(TYPE_UINT16), mVar(0)				{ mRef = inRef; }
								x_va_r(s32* inRef) : mType(TYPE_INT32), mVar(0)					{ mRef = inRef; }
								x_va_r(u32* inRef) : mType(TYPE_UINT32), mVar(0)				{ mRef = inRef; }
								x_va_r(s64* inRef) : mType(TYPE_INT64), mVar(0)					{ mRef = inRef; }
								x_va_r(u64* inRef) : mType(TYPE_UINT64), mVar(0)				{ mRef = inRef; }
								x_va_r(bool* inRef) : mType(TYPE_BOOL), mVar(0)					{ mRef = inRef; }
								x_va_r(f32* inRef) : mType(TYPE_FLOAT32), mVar(0)				{ mRef = inRef; }
								x_va_r(f64* inRef) : mType(TYPE_FLOAT64), mVar(0)				{ mRef = inRef; }
								x_va_r(const char* inRef, u16 inLength) : mType(TYPE_STRING)	{ mRef = (void*)inRef; mVar = inLength; }

		x_va_r&					operator=(s8 rhs);
		x_va_r&					operator=(u8 rhs);
		x_va_r&					operator=(s16 rhs);
		x_va_r&					operator=(u16 rhs);
		x_va_r&					operator=(s32 rhs);
		x_va_r&					operator=(u32 rhs);
		x_va_r&					operator=(s64 rhs);
		x_va_r&					operator=(u64 rhs);
		x_va_r&					operator=(f32 rhs);
		x_va_r&					operator=(f64 rhs);
		x_va_r&					operator=(bool rhs);
		x_va_r&					operator=(const char* rhs);

								operator char*() const								{ if (mType==TYPE_STRING) return (char*)mRef; else return NULL; }

		EType					type() const										{ return mType; }
		u16						var() const											{ return mVar; }

		s32						sizeInBits() const									{ return (mType&SIZE_MASK)>>SIZE_SHIFT; }
		s32						sizeInBytes() const									{ return ((mType&SIZE_MASK)>>SIZE_SHIFT) >> 3; }

		xbool					isInteger() const									{ return xbool((mType&TYPE_INTEGER)==TYPE_INTEGER && sizeInBits()==8); }
		xbool					isSignedInteger() const								{ return xbool(isInteger() && ((mType&TYPE_SIGNED)==TYPE_SIGNED)); }
		xbool					isUnsignedInteger() const							{ return xbool(isInteger() && ((mType&TYPE_UNSIGNED)==TYPE_UNSIGNED)); }

		xbool					isBool() const										{ return xbool(mType == TYPE_BOOL); }
		xbool					isInt8() const										{ return xbool(isInteger() && isSignedInteger() && sizeInBits()==8); }
		xbool					isInt16() const										{ return xbool(isInteger() && isSignedInteger() && sizeInBits()==16); }
		xbool					isInt32() const										{ return xbool(isInteger() && isSignedInteger() && sizeInBits()==32); }
		xbool					isInt64() const										{ return xbool(isInteger() && isSignedInteger() && sizeInBits()==64); }
		xbool					isUInt8() const										{ return xbool(isInteger() && isUnsignedInteger() && sizeInBits()==8); }
		xbool					isUInt16() const									{ return xbool(isInteger() && isUnsignedInteger() && sizeInBits()==16); }
		xbool					isUInt32() const									{ return xbool(isInteger() && isUnsignedInteger() && sizeInBits()==32); }
		xbool					isUInt64() const									{ return xbool(isInteger() && isUnsignedInteger() && sizeInBits()==64); }
		xbool					isF32() const										{ return xbool((mType&TYPE_FLOAT32)==TYPE_FLOAT32); }
		xbool					isF64() const										{ return xbool((mType&TYPE_FLOAT64)==TYPE_FLOAT64); }
		xbool					isString() const									{ return xbool((mType&TYPE_STRING)==TYPE_STRING); }

		static x_va_r			sEmpty;

	protected:
		EType					mType;
		u16						mVar;
		void*					mRef;
	};


	struct x_va_r_list
	{
		x_va_r_list() : mLength(0)													{ }
		x_va_r_list(x_va_r a1) : mLength(1)											{ mArg[0]=a1; }
		x_va_r_list(x_va_r a1, x_va_r a2) : mLength(2)									{ mArg[0]=a1; mArg[1]=a2; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3) : mLength(3)								{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4) : mLength(4)								{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5) : mLength(5)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6) : mLength(6)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7) : mLength(7)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7, x_va_r a8) : mLength(8)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7, x_va_r a8, x_va_r a9) : mLength(9)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7, x_va_r a8, x_va_r a9, x_va_r a10) : mLength(10)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7, x_va_r a8, x_va_r a9, x_va_r a10, x_va_r a11) : mLength(11)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7, x_va_r a8, x_va_r a9, x_va_r a10, x_va_r a11, x_va_r a12) : mLength(12)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7, x_va_r a8, x_va_r a9, x_va_r a10, x_va_r a11, x_va_r a12, x_va_r a13) : mLength(13)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; mArg[12]=a13; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7, x_va_r a8, x_va_r a9, x_va_r a10, x_va_r a11, x_va_r a12, x_va_r a13, x_va_r a14) : mLength(14)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; mArg[12]=a13; mArg[13]=a14; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7, x_va_r a8, x_va_r a9, x_va_r a10, x_va_r a11, x_va_r a12, x_va_r a13, x_va_r a14, x_va_r a15) : mLength(15)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; mArg[12]=a13; mArg[13]=a14; mArg[14]=a15; }
		x_va_r_list(x_va_r a1, x_va_r a2, x_va_r a3, x_va_r a4, x_va_r a5, x_va_r a6, x_va_r a7, x_va_r a8, x_va_r a9, x_va_r a10, x_va_r a11, x_va_r a12, x_va_r a13, x_va_r a14, x_va_r a15, x_va_r a16) : mLength(16)							{ mArg[0]=a1; mArg[1]=a2; mArg[2]=a3; mArg[3]=a4; mArg[4]=a5; mArg[5]=a6; mArg[6]=a7; mArg[7]=a8; mArg[8]=a9; mArg[9]=a10; mArg[10]=a11; mArg[11]=a12; mArg[12]=a13; mArg[13]=a14; mArg[14]=a15; mArg[15]=a16; }

		xbool				add(const x_va_r& inArg1)								{ if (mLength<16) { mArg[mLength++] = inArg1; return xTRUE; } return xFALSE; }

		x_va_r				operator[](s32 inIndex) const							{ if (inIndex>=0 && inIndex<mLength) return mArg[inIndex]; else return x_va_r::sEmpty; }
		x_va_r				operator[](s32 inIndex)									{ if (inIndex>=0 && inIndex<mLength) return mArg[inIndex]; else return x_va_r::sEmpty; }

	protected:
		s32					mLength;
		x_va_r				mArg[16];
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif ///< END __X_CORE_VA_LIST_H__
