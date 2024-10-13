#ifndef __CBASE_LIMITS_H__
#define __CBASE_LIMITS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    static const u8 u8_min = 0;
    static const u8 u8_max = 0xFF;

    static const u16 u16_min = 0;
    static const u16 u16_max = 0xFFFF;

    static const u32 u32_min = 0;
    static const u32 u32_max = 0xFFFFFFFF;

    static const u64 u64_min = 0;
    static const u64 u64_max = 0xFFFFFFFFFFFFFFFF;

    static const s8 s8_min = -128;
    static const s8 s8_max = 127;

    static const s16 s16_min = -32768;
    static const s16 s16_max = 32767;

    static const s32 s32_min = (s32)0x80000000;
    static const s32 s32_max = 2147483647;

    static const s64 s64_min = (s64)0x8000000000000000LL;
    static const s64 s64_max = (s64)0x7FFFFFFFFFFFFFFFLL;

    static const f32 f32_min = -3.402823466e+38f;
    static const f32 f32_max = 3.402823466e+38f;

    static const f64 f64_min = -1.7976931348623158e+308;
    static const f64 f64_max = 1.7976931348623158e+308;

    static const byte byte_min = 0;
    static const byte byte_max = 0xFF;

    static const sbyte sbyte_min = -128;
    static const sbyte sbyte_max = 127;

    static const wchar wchar_min = 0;
    static const wchar wchar_max = 0xFFFF;

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

        static __type  min() { return u8_min; }
        static __type  max() { return u8_max; }
        static bool    has_sign() { return false; }
        static type_id id() { return type_id(type_id::UInt8); }
    };

    template <>
    class type_t<u16>
    {
    public:
        typedef u16 __type;

        static __type  min() { return u16_min; }
        static __type  max() { return u16_max; }
        static bool    has_sign() { return false; }
        static type_id id() { return type_id(type_id::UInt16); }
    };

    template <>
    class type_t<u32>
    {
    public:
        typedef u32 __type;

        static __type  min() { return u32_min; }
        static __type  max() { return u32_max; }
        static bool    has_sign() { return false; }
        static type_id id() { return type_id(type_id::UInt32); }
    };

    template <>
    class type_t<u64>
    {
    public:
        typedef u64 __type;

        static __type  min() { return u64_min; }
        static __type  max() { return u64_max; }
        static bool    has_sign() { return false; }
        static type_id id() { return type_id(type_id::UInt64); }
    };

    template <>
    class type_t<s8>
    {
    public:
        typedef s8 __type;

        static __type  min() { return s8_min; }
        static __type  max() { return s8_max; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Int8); }
    };

    template <>
    class type_t<s16>
    {
    public:
        typedef s16 __type;

        static __type  min() { return s16_min; }
        static __type  max() { return s16_max; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Int16); }
    };

    template <>
    class type_t<s32>
    {
    public:
        typedef s32 __type;

        static __type  min() { return s32_min; }
        static __type  max() { return s32_max; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Int32); }
    };

    template <>
    class type_t<s64>
    {
    public:
        typedef s64 __type;

        static __type  min() { return s64_min; }
        static __type  max() { return s64_max; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Int64); }
    };

    template <>
    class type_t<f32>
    {
    public:
        typedef f32 __type;

        static __type  min() { return f32_min; }
        static __type  max() { return f32_max; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Float); }
    };

    template <>
    class type_t<f64>
    {
    public:
        typedef f64 __type;

        static __type  min() { return f64_min; }
        static __type  max() { return f64_max; }
        static bool    has_sign() { return true; }
        static type_id id() { return type_id(type_id::Double); }
    };

};  // namespace ncore

#endif
