#ifndef __CBASE_CORE_VA_LIST_H__
#define __CBASE_CORE_VA_LIST_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_runes.h"

namespace ncore
{
    /**
     *------------------------------------------------------------------------------
     * Author:
     *     Jurgen Kluft
     * Description:
     *     va_list_t using va_t is a type safe implementation of the variable argument
     *     functionality that is provided by C/C++. The disadvantage of the variable
     *     argument functionality is that it is not 'type safe'. That is why we have
     *     overridden this by using a custom va_list.
     *
     *<P>  The printf and sprintf as we all know can still be used as we have always
     *     done, there are no changes in their use. The only main difference is that
     *     these functions have become 'type safe'. This means that if you have specified
     *     %s in the format string and have passed a variable which is NOT a string
     *     then the printf code will not crash and it will display nullptr. For some types
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
    class va_t
    {
        enum EProperty
        {
            PROP_INTEGER  = 0x0800,
            PROP_SIGNED   = 0x0400,
            PROP_UNSIGNED = 0x0200,
            PROP_FLOAT    = 0x0100,
            PROP_MASK     = 0x0F00,
            PROP_SHIFT    = 8,
        };
        enum ESize
        {
            SIZE_MASK  = 0xF000,
            SIZE_SHIFT = 12,
            SIZE_8     = 0x1000,
            SIZE_16    = 0x2000,
            SIZE_32    = 0x4000,
            SIZE_64    = 0x8000,
#ifdef TARGET_32BIT
            SIZE_PTR = SIZE_32,
#else
            SIZE_PTR = SIZE_64,
#endif
        };
    public:
        enum EType
        {
            TYPE_EMPTY   = 0,
            TYPE_MASK    = 0x00FF,
            TYPE_INT8    = 0x0001 | PROP_INTEGER | PROP_SIGNED | SIZE_8,
            TYPE_INT16   = 0x0002 | PROP_INTEGER | PROP_SIGNED | SIZE_16,
            TYPE_INT     = 0x0004 | PROP_INTEGER | PROP_SIGNED | SIZE_32,
            TYPE_INT32   = 0x0004 | PROP_INTEGER | PROP_SIGNED | SIZE_32,
            TYPE_INT64   = 0x0008 | PROP_INTEGER | PROP_SIGNED | SIZE_64,
            TYPE_UINT8   = 0x0001 | PROP_INTEGER | PROP_UNSIGNED | SIZE_8,
            TYPE_UINT16  = 0x0002 | PROP_INTEGER | PROP_UNSIGNED | SIZE_16,
            TYPE_UINT    = 0x0004 | PROP_INTEGER | PROP_UNSIGNED | SIZE_32,
            TYPE_UINT32  = 0x0004 | PROP_INTEGER | PROP_UNSIGNED | SIZE_32,
            TYPE_UINT64  = 0x0008 | PROP_INTEGER | PROP_UNSIGNED | SIZE_64,
            TYPE_BOOL    = 0x0020 | PROP_INTEGER | PROP_SIGNED | SIZE_32,
            TYPE_FLOAT32 = 0x0040 | PROP_FLOAT | SIZE_32,
            TYPE_FLOAT64 = 0x0050 | PROP_FLOAT | SIZE_64,
            TYPE_PCRUNES = 0x0080 | SIZE_PTR
        };

    public:
        va_t() : mType(TYPE_EMPTY), mCap(0) { mArg[0] = mArg[1] = mArg[2] = 0; }
        va_t(const va_t& c) : mType(c.mType), mCap(0) { *(u64*)mArg = *(u64*)c.mArg; }
        explicit va_t(s8 inVar) : mType(TYPE_INT8), mCap(0) { *(s8*)mArg = inVar; }
        explicit va_t(u8 inVar) : mType(TYPE_UINT8), mCap(0) { *(u8*)mArg = inVar; }
        explicit va_t(s16 inVar) : mType(TYPE_INT16), mCap(0) { *(s16*)mArg = inVar; }
        explicit va_t(u16 inVar) : mType(TYPE_UINT16), mCap(0) { *(u16*)mArg = inVar; }
        explicit va_t(s32 inVar) : mType(TYPE_INT32), mCap(0) { *(s32*)mArg = inVar; }
        explicit va_t(u32 inVar) : mType(TYPE_UINT32), mCap(0) { *(u32*)mArg = inVar; }
        explicit va_t(s64 inVar) : mType(TYPE_INT64), mCap(0) { *(s64*)mArg = inVar; }
        explicit va_t(u64 inVar) : mType(TYPE_UINT64), mCap(0) { *(u64*)mArg = inVar; }
        explicit va_t(bool inVar) : mType(TYPE_BOOL), mCap(0) { *(u32*)mArg = inVar ? 1 : 0; }
        explicit va_t(f32 inVar) : mType(TYPE_FLOAT32), mCap(0) { *(f32*)mArg = inVar; }
        explicit va_t(f64 inVar) : mType(TYPE_FLOAT64), mCap(0) { *(f64*)mArg = inVar; }
        explicit va_t(const char* inVar);
        explicit va_t(crunes_t const& str);

        EType type() const { return (EType)mType; }

        s32 sizeInBits() const { return (mType & SIZE_MASK) >> SIZE_SHIFT; }
        s32 sizeInBytes() const { return ((mType & SIZE_MASK) >> SIZE_SHIFT) >> 3; }

        bool isInteger() const { return bool((mType & PROP_MASK) == PROP_INTEGER && sizeInBits() == 8); }
        bool isSignedInteger() const { return bool(isInteger() && ((mType & PROP_MASK) == PROP_SIGNED)); }
        bool isUnsignedInteger() const { return bool(isInteger() && ((mType & PROP_MASK) == PROP_UNSIGNED)); }

        bool isBool() const { return bool(mType == TYPE_BOOL); }
        bool isInt8() const { return bool(mType == TYPE_INT8); }
        bool isInt16() const { return bool(mType == TYPE_INT16); }
        bool isInt32() const { return bool(mType == TYPE_INT32); }
        bool isInt64() const { return bool(mType == TYPE_INT64); }
        bool isUInt8() const { return bool(mType == TYPE_UINT8); }
        bool isUInt16() const { return bool(mType == TYPE_UINT16); }
        bool isUInt32() const { return bool(mType == TYPE_UINT32); }
        bool isUInt64() const { return bool(mType == TYPE_UINT64); }
        bool isF32() const { return bool(mType == TYPE_FLOAT32); }
        bool isF64() const { return bool(mType == TYPE_FLOAT64); }
        bool isPCRunes() const { return bool(mType == TYPE_PCRUNES); }

        operator char() const { return (char)convertToInt8(); }
        operator s8() const { return convertToInt8(); }
        operator u8() const { return convertToUInt8(); }
        operator s16() const { return convertToInt16(); }
        operator u16() const { return convertToUInt16(); }
        operator s32() const { return convertToInt32(); }
        operator u32() const { return convertToUInt32(); }
        operator s64() const { return convertToInt64(); }
        operator u64() const { return convertToUInt64(); }
        operator f32() const { return convertToFloat(); }
        operator f64() const { return convertToDouble(); }
        operator bool() const { return convertToBool(); }
        operator crunes_t() const { return convertToCRunes(); }

        void convertToRunes(runes_t& chars) const;

        static const va_t sEmpty;

    protected:
        s8       convertToInt8() const;
        u8       convertToUInt8() const;
        s16      convertToInt16() const;
        u16      convertToUInt16() const;
        s32      convertToInt32() const;
        u32      convertToUInt32() const;
        s64      convertToInt64() const;
        u64      convertToUInt64() const;
        f32      convertToFloat() const;
        f64      convertToDouble() const;
        bool     convertToBool() const;
        crunes_t convertToCRunes() const;

        u16  mType;
        u16  mCap;
        ptr_t mArg[4];
    };

    class va_list_t
    {
    public:
        typedef const va_t& rcva;

        va_list_t() : mLength(0) {}
        va_list_t(rcva a1) : mLength(1) { mArg[0] = a1; }
        va_list_t(rcva a1, rcva a2) : mLength(2)
        {
            mArg[0] = a1;
            mArg[1] = a2;
        }
        va_list_t(rcva a1, rcva a2, rcva a3) : mLength(3)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4) : mLength(4)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5) : mLength(5)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6) : mLength(6)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7) : mLength(7)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8) : mLength(8)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
            mArg[7] = a8;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9) : mLength(9)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
            mArg[7] = a8;
            mArg[8] = a9;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10) : mLength(10)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
            mArg[7] = a8;
            mArg[8] = a9;
            mArg[9] = a10;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11) : mLength(11)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12) : mLength(12)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13) : mLength(13)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
            mArg[12] = a13;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13, rcva a14) : mLength(14)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
            mArg[12] = a13;
            mArg[13] = a14;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13, rcva a14, rcva a15) : mLength(15)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
            mArg[12] = a13;
            mArg[13] = a14;
            mArg[14] = a15;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13, rcva a14, rcva a15, rcva a16) : mLength(16)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
            mArg[12] = a13;
            mArg[13] = a14;
            mArg[14] = a15;
            mArg[15] = a16;
        }

        va_list_t(rcva a1, const va_list_t& inList) : mLength(1)
        {
            mArg[0] = a1;
            add(inList);
        }
        va_list_t(rcva a1, rcva a2, const va_list_t& inList) : mLength(2)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            add(inList);
        }

        s32 length() const { return mLength; }

        bool add(const va_t& inArg1)
        {
            if (mLength < 11)
            {
                mArg[mLength++] = inArg1;
                return True;
            }
            return False;
        }
        void add(const va_list_t& inList)
        {
            for (s32 j = mLength, i = 0; i < inList.mLength && j < 11; ++i, ++j)
                mArg[j] = inList[i];
        }

        const va_t& operator[](s32 inIndex) const
        {
            if (inIndex >= 0 && inIndex < mLength)
                return mArg[inIndex];
            else
                return va_t::sEmpty;
        }

    protected:
        s32  mLength;
        va_t mArg[16];
    };

    /**
     *------------------------------------------------------------------------------
     * Author:
     *     Jurgen Kluft
     * Description:
     *     va_r_list_t using va_r_t is a type safe implementation of 'pointer to argument'
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
    class va_r_t
    {
        enum EProperty
        {
            PROP_INTEGER  = 0x0800,
            PROP_UNSIGNED = 0x0400,
            PROP_ARRAY    = 0x0200,
            PROP_FLOAT    = 0x0100,
            PROP_MASK     = 0x0F00,
            PROP_SHIFT    = 8,
        };
        enum ESize
        {
            SIZE_MASK  = 0xF000,
            SIZE_SHIFT = 12,
            SIZE_8     = 0x1000,
            SIZE_16    = 0x2000,
            SIZE_32    = 0x4000,
            SIZE_64    = 0x8000,
#ifdef TARGET_32BIT
            SIZE_PTR = SIZE_32,
#else
            SIZE_PTR = SIZE_64,
#endif
        };
    public:
        enum EType
        {
            TYPE_EMPTY = 0,
            TYPE_MASK  = 0x00FF,

            TYPE_INT8    = 0x0001 | PROP_INTEGER | SIZE_8,
            TYPE_INT16   = 0x0002 | PROP_INTEGER | SIZE_16,
            TYPE_INT     = 0x0004 | PROP_INTEGER | SIZE_32,
            TYPE_INT32   = 0x0004 | PROP_INTEGER | SIZE_32,
            TYPE_INT64   = 0x0008 | PROP_INTEGER | SIZE_64,
            TYPE_UINT8   = 0x0001 | PROP_INTEGER | PROP_UNSIGNED | SIZE_8,
            TYPE_UINT16  = 0x0002 | PROP_INTEGER | PROP_UNSIGNED | SIZE_16,
            TYPE_UINT    = 0x0004 | PROP_INTEGER | PROP_UNSIGNED | SIZE_32,
            TYPE_UINT32  = 0x0004 | PROP_INTEGER | PROP_UNSIGNED | SIZE_32,
            TYPE_UINT64  = 0x0008 | PROP_INTEGER | PROP_UNSIGNED | SIZE_64,
            TYPE_BOOL    = 0x0020 | PROP_INTEGER | SIZE_32,
            TYPE_UCHAR   = 0x0030 | PROP_INTEGER | PROP_UNSIGNED | SIZE_32,
            TYPE_FLOAT32 = 0x0040 | PROP_FLOAT | SIZE_32,
            TYPE_FLOAT64 = 0x0050 | PROP_FLOAT | SIZE_64,
            TYPE_AINT32  = 0x0004 | PROP_INTEGER | SIZE_32 | PROP_ARRAY,
            TYPE_PRUNES  = 0x0080 | SIZE_PTR
        };

    public:
        va_r_t() : mType(TYPE_EMPTY), mCap(0), mSize(0), mDummy(0) { mRef[0] = 0; }
        va_r_t(const va_r_t& c) : mType(c.mType), mCap(0), mSize(0), mDummy(0) { mRef[0] = c.mRef[0]; }
        va_r_t(s8* inRef) : mType(TYPE_INT8), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(u8* inRef) : mType(TYPE_UINT8), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(s16* inRef) : mType(TYPE_INT16), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(u16* inRef) : mType(TYPE_UINT16), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(s32* inRef) : mType(TYPE_INT32), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(u32* inRef) : mType(TYPE_UINT32), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(s32* inRef, s32 inCap) : mType(TYPE_INT32), mCap(inCap), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(u32* inRef, s32 inCap) : mType(TYPE_UINT32), mCap(inCap), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(s64* inRef) : mType(TYPE_INT64), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(u64* inRef) : mType(TYPE_UINT64), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(bool* inRef) : mType(TYPE_BOOL), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(f32* inRef) : mType(TYPE_FLOAT32), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(f64* inRef) : mType(TYPE_FLOAT64), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }
        va_r_t(runes_t* inRef) : mType(TYPE_PRUNES), mCap(0), mSize(0), mDummy(0) { mRef[0] = (ptr_t)inRef; }

        va_r_t& operator=(s8 rhs);
        va_r_t& operator=(u8 rhs);
        va_r_t& operator=(s16 rhs);
        va_r_t& operator=(u16 rhs);
        va_r_t& operator=(s32 rhs);
        va_r_t& operator=(u32 rhs);
        va_r_t& operator=(s64 rhs);
        va_r_t& operator=(u64 rhs);
        va_r_t& operator=(f32 rhs);
        va_r_t& operator=(f64 rhs);
        va_r_t& operator=(bool rhs);
        va_r_t& operator=(const crunes_t& rhs);
        va_r_t& operator=(va_t const& rhs);

        va_r_t& operator+=(s32 rhs);

        EType type() const { return (EType)mType; }
        u16   var() const { return mCap; }
        u16   size() const { return mSize; }

        s32 sizeInBits() const { return ((mType & SIZE_MASK) >> SIZE_SHIFT) << 3; }
        s32 sizeInBytes() const { return ((mType & SIZE_MASK) >> SIZE_SHIFT); }

        bool isArray() const { return bool((mType & PROP_ARRAY) == PROP_ARRAY); }
        bool isFloat() const { return bool((mType & PROP_FLOAT) == PROP_FLOAT); }
        bool isInteger() const { return bool((mType & PROP_INTEGER) == PROP_INTEGER); }
        bool isSignedInteger() const { return bool(isInteger() && ((mType & PROP_UNSIGNED) == 0)); }
        bool isUnsignedInteger() const { return bool(isInteger() && ((mType & PROP_UNSIGNED) == PROP_UNSIGNED)); }

        bool isBool() const { return bool(mType == TYPE_BOOL); }
        bool isInt8() const { return bool(mType == TYPE_INT8); }
        bool isInt16() const { return bool(mType == TYPE_INT16); }
        bool isInt32() const { return bool(mType == TYPE_INT32); }
        bool isInt64() const { return bool(mType == TYPE_INT64); }
        bool isUInt8() const { return bool(mType == TYPE_UINT8); }
        bool isUInt16() const { return bool(mType == TYPE_UINT16); }
        bool isUInt32() const { return bool(mType == TYPE_UINT32); }
        bool isUInt64() const { return bool(mType == TYPE_UINT64); }
        bool isF32() const { return bool(mType == TYPE_FLOAT32); }
        bool isF64() const { return bool(mType == TYPE_FLOAT64); }
        bool isUchar() const { return bool(mType == TYPE_UCHAR); }
        bool isPRunes() const { return bool(mType == TYPE_PRUNES); }

        runes_t* getRunes() const
        {
            if (isPRunes())
                return (runes_t*)mRef[0];
            else
                return nullptr;
        }

        static va_r_t sEmpty;

        u16  mType;
        u16  mCap;      // When PROP_ARRAY, this is the maximum capacity
        u16  mSize;     // When PROP_ARRAY, this is the final size
        u16  mDummy;
        ptr_t mRef[1];
    };

    struct va_r_list_t
    {
        va_r_list_t() : mLength(0) {}
        va_r_list_t(va_r_t a1) : mLength(1) { mArg[0] = a1; }
        va_r_list_t(va_r_t a1, va_r_t a2) : mLength(2)
        {
            mArg[0] = a1;
            mArg[1] = a2;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3) : mLength(3)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4) : mLength(4)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5) : mLength(5)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6) : mLength(6)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7) : mLength(7)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8) : mLength(8)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
            mArg[7] = a8;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9) : mLength(9)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
            mArg[7] = a8;
            mArg[8] = a9;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10) : mLength(10)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
            mArg[7] = a8;
            mArg[8] = a9;
            mArg[9] = a10;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11) : mLength(11)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12) : mLength(12)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12, va_r_t a13) : mLength(13)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
            mArg[12] = a13;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12, va_r_t a13, va_r_t a14) : mLength(14)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
            mArg[12] = a13;
            mArg[13] = a14;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12, va_r_t a13, va_r_t a14, va_r_t a15) : mLength(15)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
            mArg[12] = a13;
            mArg[13] = a14;
            mArg[14] = a15;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12, va_r_t a13, va_r_t a14, va_r_t a15, va_r_t a16) : mLength(16)
        {
            mArg[0]  = a1;
            mArg[1]  = a2;
            mArg[2]  = a3;
            mArg[3]  = a4;
            mArg[4]  = a5;
            mArg[5]  = a6;
            mArg[6]  = a7;
            mArg[7]  = a8;
            mArg[8]  = a9;
            mArg[9]  = a10;
            mArg[10] = a11;
            mArg[11] = a12;
            mArg[12] = a13;
            mArg[13] = a14;
            mArg[14] = a15;
            mArg[15] = a16;
        }

        bool add(const va_r_t& inArg1)
        {
            if (mLength < 16)
            {
                mArg[mLength++] = inArg1;
                return True;
            }
            return False;
        }

        va_r_t operator[](s32 inIndex) const
        {
            if (inIndex >= 0 && inIndex < mLength)
                return mArg[inIndex];
            else
                return va_r_t::sEmpty;
        }
        va_r_t operator[](s32 inIndex)
        {
            if (inIndex >= 0 && inIndex < mLength)
                return mArg[inIndex];
            else
                return va_r_t::sEmpty;
        }

    protected:
        s32    mLength;
        va_r_t mArg[16];
    };

#define X_VA_ARGS_16                                                                                                                                                                                                                                      \
    const va_t &v1, const va_t &v2, const va_t &v3, const va_t &v4, const va_t &v5, const va_t &v6, const va_t &v7, const va_t &v8, const va_t &v9, const va_t &v10, const va_t &v11, const va_t &v12, const va_t &v13, const va_t &v14, const va_t &v15, \
        const va_t &v16
#define X_VA_ARGS_16_DEF                                                                                                                                                                                                                                      \
    const va_t &v1, const va_t &v2 = va_t::sEmpty, const va_t &v3 = va_t::sEmpty, const va_t &v4 = va_t::sEmpty, const va_t &v5 = va_t::sEmpty, const va_t &v6 = va_t::sEmpty, const va_t &v7 = va_t::sEmpty, const va_t &v8 = va_t::sEmpty,                  \
                               const va_t &v9 = va_t::sEmpty, const va_t &v10 = va_t::sEmpty, const va_t &v11 = va_t::sEmpty, const va_t &v12 = va_t::sEmpty, const va_t &v13 = va_t::sEmpty, const va_t &v14 = va_t::sEmpty, const va_t &v15 = va_t::sEmpty, \
                               const va_t &v16 = va_t::sEmpty

#define X_VA_R_ARGS_16                                                                                                                                                                                                                            \
    const va_r_t &v1, const va_r_t &v2, const va_r_t &v3, const va_r_t &v4, const va_r_t &v5, const va_r_t &v6, const va_r_t &v7, const va_r_t &v8, const va_r_t &v9, const va_r_t &v10, const va_r_t &v11, const va_r_t &v12, const va_r_t &v13, \
        const va_r_t &v14, const va_r_t &v15, const va_r_t &v16
#define X_VA_R_ARGS_16_DEF                                                                                                                                                                                                                               \
    const va_r_t &v1, const va_r_t &v2 = va_r_t::sEmpty, const va_r_t &v3 = va_r_t::sEmpty, const va_r_t &v4 = va_r_t::sEmpty, const va_r_t &v5 = va_r_t::sEmpty, const va_r_t &v6 = va_r_t::sEmpty, const va_r_t &v7 = va_r_t::sEmpty,                  \
                                   const va_r_t &v8 = va_r_t::sEmpty, const va_r_t &v9 = va_r_t::sEmpty, const va_r_t &v10 = va_r_t::sEmpty, const va_r_t &v11 = va_r_t::sEmpty, const va_r_t &v12 = va_r_t::sEmpty, const va_r_t &v13 = va_r_t::sEmpty, \
                                   const va_r_t &v14 = va_r_t::sEmpty, const va_r_t &v15 = va_r_t::sEmpty, const va_r_t &v16 = va_r_t::sEmpty
}; // namespace ncore

#endif ///< END __C_CORE_VA_LIST_H__
