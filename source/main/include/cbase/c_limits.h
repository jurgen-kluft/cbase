#ifndef __CBASE_LIMITS_H__
#define __CBASE_LIMITS_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
#define X_U8_MIN (0)
#define X_U8_MAX (255)

#define X_U16_MIN (0)
#define X_U16_MAX (65535)

#define X_U32_MIN (0)
#define X_U32_MAX (D_CONSTANT_U32(4294967295))

#define X_U64_MIN (D_CONSTANT_U64(0))
#define X_U64_MAX (D_CONSTANT_U64(18446744073709551615))

#define X_S8_MIN -(128)
#define X_S8_MAX (127)

#define X_S16_MIN -(32768)
#define X_S16_MAX (32767)

#define X_S32_MIN (0x80000000L)
#define X_S32_MAX (2147483647)

#define X_S64_MIN (0x8000000000000000LL)
#define X_S64_MAX (D_CONSTANT_64(9223372036854775807))

#define X_F32_MIN -3.402823466e+38f
#define X_F32_MAX 3.402823466e+38f

#define X_F64_MIN -1.7976931348623158e+308
#define X_F64_MAX 1.7976931348623158e+308

#define X_XBYTE_MIN X_U8_MIN
#define X_XBYTE_MAX X_U8_MAX

#define X_XWCHAR_MIN U16_MIN
#define X_XWCHAR_MAX U16_MAX

    /**
     * Description:
     *     class limits_t is a template class, which use the C++ class template specialization.
     *     The idea of C++ class template specialization is similar to function template
     *     overloading. This can make the template code for certain data types to be fixed.
     *
     * Example:
     *<CODE>
     *      template<class T>
     *      bool isInRange(T inNum)
     *      {
     *          if ((inNum >= limits_t<T>::minimum()) && (inNum <= limits_t<T>::maximum()))
     *          {
     *                return true;
     *            }
     *            return false;
     *         }
     *
     *</CODE>
     *
     */
    template <typename T> class limits_t
    {
    };

#undef min
#undef max

    template <> class limits_t<u8>
    {
    public:
        typedef u8 type_t;

        static type_t minimum() { return X_U8_MIN; }
        static type_t maximum() { return X_U8_MAX; }
        static bool has_sign() { return false; }
    };

    template <> class limits_t<u16>
    {
    public:
        typedef u16 type_t;

        static type_t minimum() { return X_U16_MIN; }
        static type_t maximum() { return X_U16_MAX; }
        static bool has_sign() { return false; }
    };

    template <> class limits_t<u32>
    {
    public:
        typedef u32 type_t;

        static type_t minimum() { return X_U32_MIN; }
        static type_t maximum() { return X_U32_MAX; }
        static bool has_sign() { return false; }
    };

    template <> class limits_t<u64>
    {
    public:
        typedef u64 type_t;

        static type_t minimum() { return X_U64_MIN; }
        static type_t maximum() { return X_U64_MAX; }
        static bool has_sign() { return false; }
    };

    template <> class limits_t<s8>
    {
    public:
        typedef s8 type_t;

        static type_t minimum() { return X_S8_MIN; }
        static type_t maximum() { return X_S8_MAX; }
        static bool has_sign() { return true; }
    };

    template <> class limits_t<s16>
    {
    public:
        typedef s16 type_t;

        static type_t minimum() { return X_S16_MIN; }
        static type_t maximum() { return X_S16_MAX; }
        static bool has_sign() { return true; }
    };

    template <> class limits_t<s32>
    {
    public:
        typedef s32 type_t;

        static type_t minimum() { return X_S32_MIN; }
        static type_t maximum() { return X_S32_MAX; }
        static bool has_sign() { return true; }
    };

    template <> class limits_t<s64>
    {
    public:
        typedef s64 type_t;

        static type_t minimum() { return X_S64_MIN; }
        static type_t maximum() { return X_S64_MAX; }
        static bool has_sign() { return true; }
    };

    template <> class limits_t<f32>
    {
    public:
        typedef f32 type_t;

        static type_t minimum() { return X_F32_MIN; }
        static type_t maximum() { return X_F32_MAX; }
        static bool has_sign() { return true; }
    };

    template <> class limits_t<f64>
    {
    public:
        typedef f64 type_t;

        static type_t minimum() { return X_F64_MIN; }
        static type_t maximum() { return X_F64_MAX; }
        static bool has_sign() { return true; }
    };

}; // namespace ncore

#endif ///< END __CBASE_LIMITS_H__
