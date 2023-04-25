#ifndef __CBASE_STR_FMT_H__
#define __CBASE_STR_FMT_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif
#include "cbase/c_debug.h"
#include "cbase/c_runes.h"
#include "cbase/c_va_list.h"

namespace ncore
{
    namespace fmt
    {
        enum TypeId
        {
            kBool = 0,
            kChar,
            kInt32,
            kUint32,
            kInt64,
            kUint64,
            kPointer,
            kFloat,
            kString,
            kCustom
        };

        struct arg_t
        {
            arg_t(const bool value)
                : m_bool(value)
                , m_type_id(TypeId::kBool)
            {
            }

            arg_t(const uchar32 value)
                : m_char(value)
                , m_type_id(TypeId::kChar)
            {
            }

            arg_t(const s32 value)
                : m_int32(value)
                , m_type_id(TypeId::kInt32)
            {
            }

            arg_t(const u32 value)
                : m_uint32(value)
                , m_type_id(TypeId::kUint32)
            {
            }

            arg_t(const s64 value)
                : m_int64(value)
                , m_type_id(TypeId::kInt64)
            {
            }

            arg_t(const u64 value)
                : m_uint64(value)
                , m_type_id(TypeId::kUint64)
            {
            }

            arg_t(const void* value)
                : m_pointer(reinterpret_cast<u64>(value))
                , m_type_id(TypeId::kPointer)
            {
            }

            arg_t(const double value)
                : m_float(value)
                , m_type_id(TypeId::kFloat)
            {
            }

            arg_t(const char* value)
                : m_string(value)
                , m_type_id(TypeId::kString)
            {
            }

            // --------------------------------------------------------------------
            // PRIVATE MEMBER VARIABLES
            // --------------------------------------------------------------------

            enum TypeId
            {
                kBool = 0,
                kChar,
                kInt8,
                kUint8,
                kInt16,
                kUint16,
                kInt32,
                kUint32,
                kInt64,
                kUint64,
                kPointer,
                kFloat,
                kDouble,
                kString,
                kCustom
            };

            union
            {
                bool        m_bool;
                uchar32     m_char;
                s8          m_int8;
                u8          m_uint8;
                s16         m_int16;
                u16         m_uint16;
                s32         m_int32;
                u32         m_uint32;
                s64         m_int64;
                u64         m_uint64;
                u64         m_pointer;
                double      m_float;
                const char* m_string;
                void*       m_custom;
            };

            inline operator u64() const { return m_uint64; }

            TypeId m_type_id;

            static arg_t empty;
        };

        struct type_t
        {
            type_t(const bool value)
                : m_type_id(TypeId::kBool)
            {
            }

            type_t(const uchar32 value)
                : m_type_id(TypeId::kChar)
            {
            }

            type_t(const s32 value)
                : m_type_id(TypeId::kInt32)
            {
            }

            type_t(const u32 value)
                : m_type_id(TypeId::kUint32)
            {
            }

            type_t(const s64 value)
                : m_type_id(TypeId::kInt64)
            {
            }

            type_t(const u64 value)
                : m_type_id(TypeId::kUint64)
            {
            }

            type_t(const void* value)
                : m_type_id(TypeId::kPointer)
            {
            }

            type_t(const double value)
                : m_type_id(TypeId::kFloat)
            {
            }

            type_t(const char* value)
                : m_type_id(TypeId::kString)
            {
            }

            u8 m_type_id;
        };

        template <typename T> struct typed
        {
            const u8 value = kCustom;
        };

        template <> struct typed<bool>
        {
            const u8 value = kBool;
        };

        template <> struct typed<uchar32>
        {
            const u8 value = kChar;
        };

        template <> struct typed<s32>
        {
            const u8 value = kInt32;
        };

        template <> struct typed<u32>
        {
            const u8 value = kUint32;
        };

        template <> struct typed<s64>
        {
            const u8 value = kInt64;
        };

        template <> struct typed<u64>
        {
            const u8 value = kUint64;
        };

        template <> struct typed<void*>
        {
            const u8 value = kPointer;
        };

        template <> struct typed<float>
        {
            const u8 value = kFloat;
        };

        template <> struct typed<double>
        {
            const u8 value = kDouble;
        };

        template <> struct typed<char*>
        {
            const u8 value = kString;
        };

        struct args_t
        {
            u64 const* args;
            u8 const*  types;
            s32        size;
        };

        ascii::prune                        toStr(ascii::prune str, ascii::prune end, ascii::prune fmt, args_t& args);
        template <typename T1> ascii::prune toStr(ascii::prune str, s32 strMaxLen, ascii::prune fmt, T1 a)
        {
            const u8  types[]  = {typed<T1>::value};
            const u64 values[] = {arg_t(a)};
            args_t    args     = {values, types, (s32)(sizeof(types) / sizeof(types[0]))};
            return toStr(str, &str[strMaxLen], fmt, args);
        }

        template <typename T1, typename T2> ascii::prune toStr(ascii::prune str, s32 strMaxLen, ascii::prune fmt, T1 a, T2 b)
        {
            const u8  types[]  = {typed<T1>::value, typed<T2>::value};
            const u64 values[] = {arg_t(a), arg_t(b)};
            args_t    args     = {values, types, (s32)(sizeof(types) / sizeof(types[0]))};
            return toStr(str, &str[strMaxLen], fmt, args);
        }

        template <typename T1, typename T2, typename T3> ascii::prune toStr(ascii::prune str, s32 strMaxLen, ascii::prune fmt, T1 a, T2 b, T3 c)
        {
            const u8  types[]  = {typed<T1>::value, typed<T2>::value, typed<T3>::value};
            const u64 values[] = {arg_t(a), arg_t(b), arg_t(c)};
            args_t    args     = {values, types, (s32)(sizeof(types) / sizeof(types[0]))};
            return toStr(str, &str[strMaxLen], fmt, args);
        }

    } // namespace fmt
} // namespace ncore

#endif // __CBASE_STR_FMT_H__