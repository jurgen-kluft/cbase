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
                kInt32,
                kUint32,
                kInt64,
                kUint64,
                kPointer,
                kFloat,
                kString,
                kCustom
            };

            union
            {
                bool        m_bool;
                uchar32     m_char;
                s32         m_int32;
                u32         m_uint32;
                s64         m_int64;
                u64         m_uint64;
                u64         m_pointer;
                double      m_float;
                const char* m_string;
                void*       m_custom;
            };

            TypeId m_type_id;

            static arg_t empty;
        };

        bool to(ascii::prune str, ascii::prune end, ascii::prune fmt, arg_t a, arg_t b = arg_t::empty, arg_t c = arg_t::empty, arg_t d = arg_t::empty, arg_t e = arg_t::empty, arg_t f = arg_t::empty, arg_t g = arg_t::empty, arg_t h = arg_t::empty, arg_t i = arg_t::empty, arg_t j = arg_t::empty, arg_t k = arg_t::empty, arg_t l = arg_t::empty);

    } // namespace fmt
} // namespace ncore

#endif // __CBASE_STR_FMT_H__