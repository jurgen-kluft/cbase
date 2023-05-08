#ifndef __CBASE_LIMITS_H__
#define __CBASE_LIMITS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
#define D_U8_MIN (0)
#define D_U8_MAX (255)

#define D_U16_MIN (0)
#define D_U16_MAX (65535)

#define D_U32_MIN (0)
#define D_U32_MAX (D_CONSTANT_U32(4294967295))

#define D_U64_MIN (D_CONSTANT_U64(0))
#define D_U64_MAX (D_CONSTANT_U64(18446744073709551615))

#define D_S8_MIN -(128)
#define D_S8_MAX (127)

#define D_S16_MIN -(32768)
#define D_S16_MAX (32767)

#define D_S32_MIN (0x80000000L)
#define D_S32_MAX (2147483647)

#define D_S64_MIN (0x8000000000000000LL)
#define D_S64_MAX (D_CONSTANT_64(9223372036854775807))

#define D_F32_MIN -3.402823466e+38f
#define D_F32_MAX 3.402823466e+38f

#define D_F64_MIN -1.7976931348623158e+308
#define D_F64_MAX 1.7976931348623158e+308

#define D_BYTE_MIN D_U8_MIN
#define D_BYTE_MAX D_U8_MAX

#define D_WCHAR_MIN U16_MIN
#define D_WCHAR_MAX U16_MAX

    /**
     * Description:
     *     class type_t is a template class, which use the C++ class template specialization.
     *     The idea of C++ class template specialization is similar to function template
     *     overloading. This can make the template code for certain data types to be fixed.
     *
     * Example:
     *<CODE>
     *      template<class T>
     *      bool isInRange(T inNum)
     *      {
     *          if ((inNum >= type_t<T>::min()) && (inNum <= type_t<T>::max()))
     *          {
     *                return true;
     *            }
     *            return false;
     *         }
     *
     *</CODE>
     *
     */

    struct type_id
    {
        enum ETypeId
        {
            Int8   = 0,
            Int16  = 1,
            Int32  = 2,
            Int64  = 3,
            UInt8  = 4,
            UInt16 = 5,
            UInt32 = 6,
            UInt64 = 7,
            Float  = 8,
            Double = 9,
            Bool   = 10,
            Char   = 11,
        };

        inline type_id()
            : value(Bool)
        {
        }
        inline type_id(ETypeId value)
            : value(value)
        {
        }
        inline type_id(const type_id& other)
            : value(other.value)
        {
        }

        u8 value;
    };

    template <typename T>
    class type_t
    {
    };

#undef min
#undef max

    template <>
    class type_t<bool>
    {
    public:
        typedef bool __type;

        static __type  min() { return false; }
        static __type  max() { return true; }
        static bool    has_sign() { return false; }
        static type_id id() { return type_id(type_id::Bool); }
    };

    template <>
    class type_t<u8>
    {
    public:
        typedef u8 __type;

        static __type  min() { return D_U8_MIN; }
        static __type  max() { return D_U8_MAX; }
        static bool    has_sign() { return false; }
        static type_id id() { return type_id(type_id::UInt8); }
    };

    template <>
    class type_t<u16>
    {
    public:
        typedef u16 __type;

        static __type  min() { return D_U16_MIN; }
        static __type  max() { return D_U16_MAX; }
        static bool    has_sign() { return false; }
        static type_id id() { return type_id(type_id::UInt16); }
    };

    template <>
    class type_t<u32>
    {
    public:
        typedef u32 __type;

        static __type  min() { return D_U32_MIN; }
        static __type  max() { return D_U32_MAX; }
        static bool    has_sign() { return false; }
        static type_id id() { return type_id(type_id::UInt32); }
    };

    template <>
    class type_t<u64>
    {
    public:
        typedef u64 __type;

        static __type  min() { return D_U64_MIN; }
        static __type  max() { return D_U64_MAX; }
        static bool    has_sign() { return false; }
        static type_id id() { return type_id(type_id::UInt64); }
    };

    template <>
    class type_t<s8>
    {
    public:
        typedef s8 __type;

        static __type  min() { return D_S8_MIN; }
        static __type  max() { return D_S8_MAX; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Int8); }
    };

    template <>
    class type_t<s16>
    {
    public:
        typedef s16 __type;

        static __type  min() { return D_S16_MIN; }
        static __type  max() { return D_S16_MAX; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Int16); }
    };

    template <>
    class type_t<s32>
    {
    public:
        typedef s32 __type;

        static __type  min() { return D_S32_MIN; }
        static __type  max() { return D_S32_MAX; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Int32); }
    };

    template <>
    class type_t<s64>
    {
    public:
        typedef s64 __type;

        static __type  min() { return D_S64_MIN; }
        static __type  max() { return D_S64_MAX; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Int64); }
    };

    template <>
    class type_t<f32>
    {
    public:
        typedef f32 __type;

        static __type  min() { return D_F32_MIN; }
        static __type  max() { return D_F32_MAX; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Float); }
    };

    template <>
    class type_t<f64>
    {
    public:
        typedef f64 __type;

        static __type  min() { return D_F64_MIN; }
        static __type  max() { return D_F64_MAX; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Double); }
    };

};  // namespace ncore

#endif  ///< END __CBASE_LIMITS_H__
