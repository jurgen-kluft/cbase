#ifndef __CBASE_CORE_VA_LIST_H__
#define __CBASE_CORE_VA_LIST_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_runes.h"

namespace ncore
{
    /**
     *------------------------------------------------------------------------------
     * Author:
     *     Jurgen
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
    public:
        enum EType
        {
            TYPE_EMPTY   = 0 << 24,
            TYPE_INT8    = 1 << 24,
            TYPE_INT16   = 2 << 24,
            TYPE_INT     = 3 << 24,
            TYPE_INT32   = 4 << 24,
            TYPE_INT64   = 5 << 24,
            TYPE_UINT8   = 6 << 24,
            TYPE_UINT16  = 7 << 24,
            TYPE_UINT    = 8 << 24,
            TYPE_UINT32  = 9 << 24,
            TYPE_UINT64  = 10 << 24,
            TYPE_BOOL    = 11 << 24,
            TYPE_FLOAT32 = 12 << 24,
            TYPE_FLOAT64 = 13 << 24,
            TYPE_PCRUNES = 14 << 24,
            TYPE_PINT32  = 15 << 24,
            TYPE_PCCHAR  = 16 << 24,
            TYPE_PCWHAR  = 17 << 24,
            TYPE_MASK    = 0xFF000000,
        };

    public:
        va_t()
            : mArg3(TYPE_EMPTY)
        {
            mArg = mArg2 = 0;
        }
        va_t(const va_t &c)
            : mArg(c.mArg)
            , mArg2(c.mArg2)
            , mArg3(c.mArg3)
        {
        }
        explicit va_t(const s8 inVar)
            : mArg3(TYPE_INT8)
        {
            *(s8 *)&mArg = inVar;
        }
        explicit va_t(const u8 inVar)
            : mArg3(TYPE_UINT8)
        {
            *(u8 *)&mArg = inVar;
        }
        explicit va_t(const s16 inVar)
            : mArg3(TYPE_INT16)
        {
            *(s16 *)&mArg = inVar;
        }
        explicit va_t(const u16 inVar)
            : mArg3(TYPE_UINT16)
        {
            *(u16 *)&mArg = inVar;
        }
        explicit va_t(const s32 inVar)
            : mArg3(TYPE_INT32)
        {
            *(s32 *)&mArg = inVar;
        }
        explicit va_t(s32 *inVar)
            : mArg3(TYPE_PINT32)
        {
            *(s32 **)&mArg = inVar;
        }
        explicit va_t(const u32 inVar)
            : mArg3(TYPE_UINT32)
        {
            *(u32 *)&mArg = inVar;
        }
        explicit va_t(const s64 inVar)
            : mArg3(TYPE_INT64)
        {
            *(s64 *)&mArg = inVar;
        }
        explicit va_t(const u64 inVar)
            : mArg3(TYPE_UINT64)
        {
            *(u64 *)&mArg = inVar;
        }
        explicit va_t(const bool inVar)
            : mArg3(TYPE_BOOL)
        {
            *(u32 *)&mArg = inVar ? 1 : 0;
        }
        explicit va_t(const f32 inVar)
            : mArg3(TYPE_FLOAT32)
        {
            *(f32 *)&mArg = inVar;
        }
        explicit va_t(const f64 inVar)
            : mArg3(TYPE_FLOAT64)
        {
            *(f64 *)&mArg = inVar;
        }
        explicit va_t(const char *inVar);
        explicit va_t(const wchar_t *inVar);
        explicit va_t(crunes_t const &str);

        EType type() const { return (EType)(mArg3 & TYPE_MASK); }
        char  specifier() const;

        bool isInteger() const { return ((mArg3 & TYPE_MASK) >= TYPE_INT8 && (mArg3 & TYPE_MASK) <= TYPE_UINT64); }
        bool isSignedInteger() const { return ((mArg3 & TYPE_MASK) >= TYPE_INT8 && (mArg3 & TYPE_MASK) <= TYPE_INT64); }
        bool isUnsignedInteger() const { return ((mArg3 & TYPE_MASK) >= TYPE_UINT8 && (mArg3 & TYPE_MASK) <= TYPE_UINT64); }

        bool isBool() const { return bool((mArg3 & TYPE_MASK) == TYPE_BOOL); }
        bool isInt8() const { return bool((mArg3 & TYPE_MASK) == TYPE_INT8); }
        bool isInt16() const { return bool((mArg3 & TYPE_MASK) == TYPE_INT16); }
        bool isInt32() const { return bool((mArg3 & TYPE_MASK) == TYPE_INT32); }
        bool isInt64() const { return bool((mArg3 & TYPE_MASK) == TYPE_INT64); }
        bool isUInt8() const { return bool((mArg3 & TYPE_MASK) == TYPE_UINT8); }
        bool isUInt16() const { return bool((mArg3 & TYPE_MASK) == TYPE_UINT16); }
        bool isUInt32() const { return bool((mArg3 & TYPE_MASK) == TYPE_UINT32); }
        bool isUInt64() const { return bool((mArg3 & TYPE_MASK) == TYPE_UINT64); }
        bool isF32() const { return bool((mArg3 & TYPE_MASK) == TYPE_FLOAT32); }
        bool isF64() const { return bool((mArg3 & TYPE_MASK) == TYPE_FLOAT64); }
        bool isPCRunes() const { return bool((mArg3 & TYPE_MASK) == TYPE_PCRUNES); }
        bool isPInt32() const { return bool((mArg3 & TYPE_MASK) == TYPE_PINT32); }

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
        operator const char *() const { return *(const char **)&mArg; }
        operator const wchar_t *() const { return *(const wchar_t **)&mArg; }
        operator s32 *() const { return *(s32 **)&mArg; }

        void convertToRunes(runes_t &chars) const;

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

        ptr_t mArg;
        u32   mArg2;
        u32   mArg3;
    };

    class va_list_t
    {
    public:
        typedef const va_t &rcva;

        va_list_t()
            : mLength(0)
        {
        }
        va_list_t(rcva a1)
            : mLength(1)
        {
            mArg[0] = a1;
        }
        va_list_t(rcva a1, rcva a2)
            : mLength(2)
        {
            mArg[0] = a1;
            mArg[1] = a2;
        }
        va_list_t(rcva a1, rcva a2, rcva a3)
            : mLength(3)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4)
            : mLength(4)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5)
            : mLength(5)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6)
            : mLength(6)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7)
            : mLength(7)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
        }
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8)
            : mLength(8)
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
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9)
            : mLength(9)
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
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10)
            : mLength(10)
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
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11)
            : mLength(11)
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
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12)
            : mLength(12)
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
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13)
            : mLength(13)
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
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13, rcva a14)
            : mLength(14)
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
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13, rcva a14, rcva a15)
            : mLength(15)
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
        va_list_t(rcva a1, rcva a2, rcva a3, rcva a4, rcva a5, rcva a6, rcva a7, rcva a8, rcva a9, rcva a10, rcva a11, rcva a12, rcva a13, rcva a14, rcva a15, rcva a16)
            : mLength(16)
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

        va_list_t(rcva a1, const va_list_t &inList)
            : mLength(1)
        {
            mArg[0] = a1;
            add(inList);
        }
        va_list_t(rcva a1, rcva a2, const va_list_t &inList)
            : mLength(2)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            add(inList);
        }

        s32         length() const { return mLength; }
        s32         argc() const { return mLength; }
        const va_t *argv() const { return mArg; }

        bool add(const va_t &inArg1)
        {
            if (mLength < 11)
            {
                mArg[mLength++] = inArg1;
                return true;
            }
            return false;
        }
        void add(const va_list_t &inList)
        {
            for (s32 j = mLength, i = 0; i < inList.mLength && j < 11; ++i, ++j)
                mArg[j] = inList[i];
        }

        const va_t &operator[](s32 inIndex) const
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
     *     Jurgen
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
    public:
        enum EType
        {
            TYPE_EMPTY = 0,
            TYPE_MASK  = 0xFF,

            TYPE_INT8      = 1,
            TYPE_INT16     = 2,
            TYPE_INT32     = 3,
            TYPE_INT64     = 4,
            TYPE_UINT8     = 5,
            TYPE_UINT16    = 6,
            TYPE_UINT32    = 7,
            TYPE_UINT64    = 8,
            TYPE_BOOL      = 9,
            TYPE_UCHAR     = 10,
            TYPE_FLOAT32   = 11,
            TYPE_FLOAT64   = 12,
            TYPE_AINT32    = 13,
            TYPE_AUINT32   = 14,
            TYPE_ASCII_STR = 15,
            TYPE_USC2_STR  = 16,
            TYPE_UTF8_STR  = 17,
            TYPE_UTF16_STR = 18,
            TYPE_UTF32_STR = 19,
        };

    public:
        va_r_t()
            : mType(TYPE_EMPTY)
            , mSize(0)
            , mCap(0)
            , mDummy(0)
        {
            mRef = 0;
        }
        va_r_t(const va_r_t &c)
            : mType(c.mType)
            , mSize(c.mSize)
            , mCap(c.mCap)
            , mDummy(c.mDummy)
        {
            mRef = c.mRef;
        }
        va_r_t(s8 *inRef)
            : mType(TYPE_INT8)
            , mSize(sizeof(s8))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(u8 *inRef)
            : mType(TYPE_UINT8)
            , mSize(sizeof(u8))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(s16 *inRef)
            : mType(TYPE_INT16)
            , mSize(sizeof(s16))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(u16 *inRef)
            : mType(TYPE_UINT16)
            , mSize(sizeof(u16))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(s32 *inRef)
            : mType(TYPE_INT32)
            , mSize(sizeof(s32))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(u32 *inRef)
            : mType(TYPE_UINT32)
            , mSize(sizeof(u32))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(s32 *inRef, u16 inCap)
            : mType(TYPE_AINT32)
            , mSize(0)
            , mCap(inCap)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(u32 *inRef, u16 inCap)
            : mType(TYPE_AUINT32)
            , mSize(0)
            , mCap(inCap)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(s64 *inRef)
            : mType(TYPE_INT64)
            , mSize(sizeof(s64))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(u64 *inRef)
            : mType(TYPE_UINT64)
            , mSize(sizeof(u64))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(bool *inRef)
            : mType(TYPE_BOOL)
            , mSize(sizeof(bool))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(f32 *inRef)
            : mType(TYPE_FLOAT32)
            , mSize(sizeof(f32))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(f64 *inRef)
            : mType(TYPE_FLOAT64)
            , mSize(sizeof(f64))
            , mCap(0)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(ascii::prune inRef, s32 inLength)
            : mType(TYPE_ASCII_STR)
            , mSize(0)
            , mCap(inLength)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(ucs2::prune inRef, s32 inLength)
            : mType(TYPE_USC2_STR)
            , mSize(0)
            , mCap(inLength)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(utf8::prune inRef, s32 inLength)
            : mType(TYPE_UTF8_STR)
            , mSize(0)
            , mCap(inLength)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(utf16::prune inRef, s32 inLength)
            : mType(TYPE_UTF16_STR)
            , mSize(0)
            , mCap(inLength)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }
        va_r_t(utf32::prune inRef, s32 inLength)
            : mType(TYPE_UTF32_STR)
            , mSize(0)
            , mCap(inLength)
            , mDummy(0)
        {
            mRef = (ptr_t)inRef;
        }

        va_r_t &operator=(s8 rhs);
        va_r_t &operator=(u8 rhs);
        va_r_t &operator=(s16 rhs);
        va_r_t &operator=(u16 rhs);
        va_r_t &operator=(s32 rhs);
        va_r_t &operator=(u32 rhs);
        va_r_t &operator=(s64 rhs);
        va_r_t &operator=(u64 rhs);
        va_r_t &operator=(f32 rhs);
        va_r_t &operator=(f64 rhs);
        va_r_t &operator=(bool rhs);
        va_r_t &operator=(const crunes_t &rhs);
        va_r_t &operator=(va_t const &rhs);

        va_r_t &operator+=(s32 rhs);

        EType type() const { return (EType)mType; }
        u16   var() const { return mType; }
        u16   size() const { return mSize; }

        bool isArray() const { return (mType & TYPE_AINT32) == TYPE_AINT32 || (mType & TYPE_AUINT32) == TYPE_AUINT32; }
        bool isFloat() const { return (mType & TYPE_FLOAT32) == TYPE_FLOAT32 || (mType & TYPE_FLOAT64) == TYPE_FLOAT64; }
        bool isInteger() const { return (mType >= TYPE_INT8 && mType <= TYPE_UINT64); }
        bool isSignedInteger() const { return (mType >= TYPE_INT8 && mType <= TYPE_INT64); }
        bool isUnsignedInteger() const { return (mType >= TYPE_UINT8 && mType <= TYPE_UINT64); }

        s32 sizeInBytes() const { return mSize; }

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
        bool isAsciiString() const { return bool(mType == TYPE_ASCII_STR); }
        bool isUsc2String() const { return bool(mType == TYPE_USC2_STR); }
        bool isUtf8String() const { return bool(mType == TYPE_UTF8_STR); }
        bool isUtf16String() const { return bool(mType == TYPE_UTF16_STR); }
        bool isUtf32String() const { return bool(mType == TYPE_UTF32_STR); }

        runes_t getRunes() const
        {
            switch (mType)
            {
                case TYPE_ASCII_STR: return runes_t((ascii::prune)mRef, 0, 0, mCap);
                case TYPE_USC2_STR: return runes_t((ucs2::prune)mRef, 0, 0, mCap);
                case TYPE_UTF8_STR: return runes_t((utf8::prune)mRef, 0, 0, mCap);
                case TYPE_UTF16_STR: return runes_t((utf16::prune)mRef, 0, 0, mCap);
                case TYPE_UTF32_STR: return runes_t((utf32::prune)mRef, 0, 0, mCap);
            }
            return runes_t("", 0, 0, 0);
        }

        static va_r_t sEmpty;

        ptr_t mRef;
        u16   mType;
        u16   mSize;  // When PROP_ARRAY, this is the final size
        u16   mCap;   // When PROP_ARRAY, this is the maximum capacity
        u16   mDummy;
    };

    struct va_r_list_t
    {
        va_r_list_t()
            : mLength(0)
        {
        }
        va_r_list_t(va_r_t a1)
            : mLength(1)
        {
            mArg[0] = a1;
        }
        va_r_list_t(va_r_t a1, va_r_t a2)
            : mLength(2)
        {
            mArg[0] = a1;
            mArg[1] = a2;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3)
            : mLength(3)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4)
            : mLength(4)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5)
            : mLength(5)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6)
            : mLength(6)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7)
            : mLength(7)
        {
            mArg[0] = a1;
            mArg[1] = a2;
            mArg[2] = a3;
            mArg[3] = a4;
            mArg[4] = a5;
            mArg[5] = a6;
            mArg[6] = a7;
        }
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8)
            : mLength(8)
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
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9)
            : mLength(9)
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
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10)
            : mLength(10)
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
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11)
            : mLength(11)
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
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12)
            : mLength(12)
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
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12, va_r_t a13)
            : mLength(13)
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
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12, va_r_t a13, va_r_t a14)
            : mLength(14)
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
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12, va_r_t a13, va_r_t a14, va_r_t a15)
            : mLength(15)
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
        va_r_list_t(va_r_t a1, va_r_t a2, va_r_t a3, va_r_t a4, va_r_t a5, va_r_t a6, va_r_t a7, va_r_t a8, va_r_t a9, va_r_t a10, va_r_t a11, va_r_t a12, va_r_t a13, va_r_t a14, va_r_t a15, va_r_t a16)
            : mLength(16)
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

        bool add(const va_r_t &inArg1)
        {
            if (mLength < 16)
            {
                mArg[mLength++] = inArg1;
                return true;
            }
            return false;
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

#define D_VA_ARGS_16                                                                                                                                                                                                                                      \
    const va_t &v1, const va_t &v2, const va_t &v3, const va_t &v4, const va_t &v5, const va_t &v6, const va_t &v7, const va_t &v8, const va_t &v9, const va_t &v10, const va_t &v11, const va_t &v12, const va_t &v13, const va_t &v14, const va_t &v15, \
      const va_t &v16
#define D_VA_ARGS_16_DEF                                                                                                                                                                                                                                      \
    const va_t &v1, const va_t &v2 = va_t::sEmpty, const va_t &v3 = va_t::sEmpty, const va_t &v4 = va_t::sEmpty, const va_t &v5 = va_t::sEmpty, const va_t &v6 = va_t::sEmpty, const va_t &v7 = va_t::sEmpty, const va_t &v8 = va_t::sEmpty,                  \
                               const va_t &v9 = va_t::sEmpty, const va_t &v10 = va_t::sEmpty, const va_t &v11 = va_t::sEmpty, const va_t &v12 = va_t::sEmpty, const va_t &v13 = va_t::sEmpty, const va_t &v14 = va_t::sEmpty, const va_t &v15 = va_t::sEmpty, \
                               const va_t &v16 = va_t::sEmpty

#define D_VA_R_ARGS_16                                                                                                                                                                                                                            \
    const va_r_t &v1, const va_r_t &v2, const va_r_t &v3, const va_r_t &v4, const va_r_t &v5, const va_r_t &v6, const va_r_t &v7, const va_r_t &v8, const va_r_t &v9, const va_r_t &v10, const va_r_t &v11, const va_r_t &v12, const va_r_t &v13, \
      const va_r_t &v14, const va_r_t &v15, const va_r_t &v16
#define D_VA_R_ARGS_16_DEF                                                                                                                                                                                                                               \
    const va_r_t &v1, const va_r_t &v2 = va_r_t::sEmpty, const va_r_t &v3 = va_r_t::sEmpty, const va_r_t &v4 = va_r_t::sEmpty, const va_r_t &v5 = va_r_t::sEmpty, const va_r_t &v6 = va_r_t::sEmpty, const va_r_t &v7 = va_r_t::sEmpty,                  \
                                   const va_r_t &v8 = va_r_t::sEmpty, const va_r_t &v9 = va_r_t::sEmpty, const va_r_t &v10 = va_r_t::sEmpty, const va_r_t &v11 = va_r_t::sEmpty, const va_r_t &v12 = va_r_t::sEmpty, const va_r_t &v13 = va_r_t::sEmpty, \
                                   const va_r_t &v14 = va_r_t::sEmpty, const va_r_t &v15 = va_r_t::sEmpty, const va_r_t &v16 = va_r_t::sEmpty
};  // namespace ncore

#endif  ///< END __C_CORE_VA_LIST_H__
