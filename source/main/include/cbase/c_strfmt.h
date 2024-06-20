#ifndef __CBASE_STR_FMT_H__
#define __CBASE_STR_FMT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif
#include "ccore/c_debug.h"
#include "cbase/c_runes.h"

namespace ncore
{
    namespace fmt
    {
        class strspan_t;
        class format_t;

        struct TypeId
        {
            enum EType
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
                kVoidPointer,
                kConstVoidPointer,
                kFloat,
                kDouble,
                kString,
                kConstString,
                kStrSpan,
                kCustom
            };

            inline TypeId()
                : value(kBool)
            {
            }
            inline TypeId(EType value)
                : value(value)
            {
            }
            inline TypeId(const TypeId& other)
                : value(other.value)
            {
            }

            u8 value;
        };

        struct Flags
        {
            enum EFlag
            {
                None         = (0U << 0U),
                AlignNone    = (0U << 1U),
                AlignLeft    = (1U << 1U),
                AlignRight   = (2U << 1U),
                AlignCenter  = (3U << 1U),
                AlignNumeric = (4U << 1U),
                AlignDefault = AlignRight,
                AlignBitmask = AlignLeft | AlignRight | AlignCenter | AlignNumeric,
                SignNone     = (0U << 4U),
                SignMinus    = (1U << 4U),
                SignPlus     = (2U << 4U),
                SignSpace    = (3U << 4U),
                SignBitmask  = SignMinus | SignPlus | SignSpace,
                Hash         = (1U << 6U),
                Uppercase    = (1U << 7U)
            };

            inline Flags()
                : value(None)
            {
            }
            inline Flags(EFlag value)
                : value((u8)value)
            {
            }
            inline Flags(const Flags& other)
                : value(other.value)
            {
            }

            inline constexpr bool operator==(EFlag rhs) const noexcept { return value == rhs; }
            inline constexpr bool operator!=(EFlag rhs) const noexcept { return value != rhs; }

            inline Flags& operator=(EFlag rhs) noexcept
            {
                value = rhs;
                return *this;
            }

            inline Flags& operator|=(EFlag rhs) noexcept
            {
                value |= rhs;
                return *this;
            }

            u8 value;
        };

        struct Align
        {
            enum EAlign
            {
                kNone    = Flags::AlignNone,
                kLeft    = Flags::AlignLeft,
                kRight   = Flags::AlignRight,
                kCenter  = Flags::AlignCenter,
                kNumeric = Flags::AlignNumeric
            };

            inline Align()
                : value(kNone)
            {
            }
            inline Align(s32 value)
                : value((u8)value)
            {
            }

            inline constexpr bool operator==(const EAlign& rhs) const noexcept { return value == rhs; }
            inline constexpr bool operator!=(const EAlign& rhs) const noexcept { return value != rhs; }

            u8 value;
        };

        struct Sign
        {
            enum ESign
            {
                kNone  = Flags::SignNone,
                kMinus = Flags::SignMinus,
                kPlus  = Flags::SignPlus,
                kSpace = Flags::SignSpace
            };
            inline Sign()
                : value(kNone)
            {
            }
            inline Sign(s32 value)
                : value((u8)value)
            {
            }

            inline constexpr bool operator==(ESign rhs) const noexcept { return value == rhs; }
            inline constexpr bool operator!=(ESign rhs) const noexcept { return value != rhs; }

            u8 value;
        };

        struct Type
        {
            enum EType
            {
                kNone,
                kBoolean,
                kIntegerDec,
                kIntegerHex,
                kIntegerOct,
                kIntegerBin,
                kFloatFixed,
                kFloatScientific,
                kFloatGeneral,
                kPointer,
                kString,
                kChar,
                kInvalid
            };

            inline constexpr bool operator==(EType rhs) const noexcept { return value == rhs; }
            inline constexpr bool operator!=(EType rhs) const noexcept { return value != rhs; }

            inline Type& operator=(const EType& rhs) noexcept
            {
                value = rhs;
                return *this;
            }
            inline Type& operator|=(const EType& rhs) noexcept
            {
                value |= rhs;
                return *this;
            }

            u8 value;
        };

        class cstrspan_t;

        class strspan_t
        {
            enum
            {
                Type_Ascii,
                Type_Utf8,
                Type_Utf16,
                Type_Utf32
            } m_type;

        public:
            strspan_t() noexcept = delete;
            strspan_t(char* first, char* last);
            strspan_t(const strspan_t&) noexcept = default;
            strspan_t(strspan_t&&) noexcept      = default;

            template <uint_t N>
            inline strspan_t(char (&array)[N])
                : m_begin(array)
                , m_end(array + N - 1)
            {
            }

            strspan_t& operator=(const strspan_t&) noexcept = default;

            inline constexpr bool at_end() const noexcept { return m_begin == m_end; }
            void                  write(uchar32 c = '\0');
            void                  skip(const uint_t n);
            void                  write(cstrspan_t const& str);

            char* begin() const noexcept { return m_begin; }
            char* end() const noexcept { return m_end; }

        protected:
            char* m_begin;
            char* m_end;
        };

        class state_t
        {
        protected:
            friend class format_t;

        public:
            state_t() noexcept               = default;
            state_t(const state_t&) noexcept = default;

            static bool format_string(strspan_t& it, state_t& state, const char* str, const char* end);
            static bool format_string(strspan_t& it, state_t& state, const cstrspan_t& str);

            inline constexpr char fill_char() const noexcept { return m_fill_char; }
            inline constexpr Type type() const noexcept { return m_type; }
            inline constexpr s32  width() const noexcept { return static_cast<s32>(m_width); }
            inline constexpr s32  precision() const noexcept { return static_cast<s32>(m_precision); }
            inline constexpr s32  index() const noexcept { return static_cast<s32>(m_index); }

            inline Align align() const noexcept { return Align(m_flags.value & Flags::AlignBitmask); }
            inline Sign  sign() const noexcept { return Sign(m_flags.value & Flags::SignBitmask); }

            inline constexpr bool hash() const noexcept { return (m_flags.value & Flags::Hash) != 0; }
            inline constexpr bool uppercase() const noexcept { return (m_flags.value & Flags::Uppercase) != 0; }

            inline constexpr bool type_is_none() const noexcept { return m_type == Type::kNone; }
            inline constexpr bool type_is_char() const noexcept { return m_type == Type::kChar; }
            inline constexpr bool type_is_string() const noexcept { return m_type == Type::kString; }
            inline constexpr bool type_is_pointer() const noexcept { return m_type == Type::kPointer; }

            inline constexpr bool type_is_integer() const noexcept { return m_type.value >= Type::kIntegerDec && m_type.value <= Type::kIntegerBin; }
            inline constexpr bool type_is_float() const noexcept { return m_type.value >= Type::kFloatFixed && m_type.value <= Type::kFloatGeneral; }
            inline constexpr bool type_is_numeric() const noexcept { return m_type.value >= Type::kIntegerDec && m_type.value <= Type::kFloatGeneral; }

            inline constexpr bool type_is_integer_dec() const noexcept { return m_type == Type::kIntegerDec; }
            inline constexpr bool type_is_integer_hex() const noexcept { return m_type == Type::kIntegerHex; }
            inline constexpr bool type_is_integer_oct() const noexcept { return m_type == Type::kIntegerOct; }
            inline constexpr bool type_is_integer_bin() const noexcept { return m_type == Type::kIntegerBin; }

            inline constexpr bool type_is_float_fixed() const noexcept { return m_type == Type::kFloatFixed; }
            inline constexpr bool type_is_float_scientific() const noexcept { return m_type == Type::kFloatScientific; }
            inline constexpr bool type_is_float_general() const noexcept { return m_type == Type::kFloatGeneral; }

            inline constexpr bool type_allow_hash() const noexcept
            {
                // Alternative format is valid for hexadecimal (including
                // pointers), octal, binary and all floating point types.
                return m_type.value >= Type::kIntegerHex && m_type.value <= Type::kFloatGeneral;
            }

        protected:
            friend class argument_t;

            // Writes the alignment (sign, prefix and fill before) for any
            // argument type. Returns the fill counter to write after argument.
            s32  write_alignment(strspan_t& it, s32 characters, const bool negative) const;
            void write_sign(strspan_t& it, const bool negative) const noexcept;
            void write_prefix(strspan_t& it) const noexcept;

            static bool format_string(strspan_t& it, const state_t& state, const cstrspan_t& str, const s32 str_length, const bool negative);

            inline constexpr s32 sign_width(const bool negative) const noexcept { return (!negative && sign().value <= Sign::kMinus) ? 0 : 1; }

            // Alternative format is valid for hexadecimal (including pointers), octal, binary and all floating point types.
            inline constexpr s32 prefix_width() const noexcept { return (!hash() || type_is_float()) ? 0 : type_is_integer_oct() ? 1 : 2; }

            // 8 bytes for argument
            char  m_fill_char = ' ';
            Type  m_type      = {Type::kNone};
            Flags m_flags     = {Flags::None};
            u8    m_width     = 0;
            s8    m_precision = -1;
            s8    m_index     = -1;
            u8    m_color     = 0;
            u8    m_dummy     = 0;
        };

        struct encoded_arg_t
        {
            union uvalue
            {
                bool        b;
                char        c;
                s8          s8;
                s16         s16;
                s32         s32;
                s64         s64;
                u8          u8;
                u16         u16;
                u32         u32;
                u64         u64;
                void*       vp;
                void const* cvp;
                float       f;
                double      d;
                const char* cstr;
                char*       str;
            };

            TypeId type;
            uvalue first;
            uvalue second;
        };

        typedef void (*format_func_t)(encoded_arg_t const& argValue, strspan_t& dst, state_t& format);

        // Arg defs
        // clang-format off
        template <typename T>
        struct arg_t
        {
            static inline encoded_arg_t encode(T v) { encoded_arg_t e; e.type = TypeId::kBool; e.first.b = false; e.second.b = false; return e; }
            static inline T             decode(encoded_arg_t const& v) { return v.first.b; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<bool>
        {
            static inline encoded_arg_t encode(bool v) { encoded_arg_t e; e.type = TypeId::kBool; e.first.b = v; e.second.b = v; return e; }
            static inline bool          decode(encoded_arg_t const& v) { return v.first.b; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<char>
        {
            static inline encoded_arg_t encode(char v) { encoded_arg_t e; e.type = TypeId::kChar; e.first.c = v; e.second.c = v; return e; }
            static inline char          decode(encoded_arg_t const& v) { return v.first.c; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<s8>
        {
            static inline encoded_arg_t encode(s8 v) { encoded_arg_t e; e.type = TypeId::kInt8; e.first.s8 = v; e.second.s8 = v; return e; }
            static inline s8            decode(encoded_arg_t const& v) { return v.first.s8; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<s16>
        {
            static inline encoded_arg_t encode(s16 v) { encoded_arg_t e; e.type = TypeId::kInt16; e.first.s16 = v; e.second.s16 = v; return e; }
            static inline s16           decode(encoded_arg_t const& v) { return v.first.s16; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<s32>
        {
            static inline encoded_arg_t encode(s32 v) { encoded_arg_t e; e.type = TypeId::kInt32; e.first.s32 = v; e.second.s32 = v; return e; }
            static inline s32           decode(encoded_arg_t const& v) { return v.first.s32; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<u8>
        {
            static inline encoded_arg_t encode(u8 v) { encoded_arg_t e; e.type = TypeId::kUint8; e.first.u8 = v; e.second.u8 = v; return e; }
            static inline u8            decode(encoded_arg_t const& v) { return v.first.u8; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<u16>
        {
            static inline encoded_arg_t encode(u16 v) { encoded_arg_t e; e.type = TypeId::kUint16; e.first.u16 = v; e.second.u16 = v; return e; }
            static inline u16           decode(encoded_arg_t const& v) { return v.first.u16; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<u32>
        {
            static inline encoded_arg_t encode(u32 v) { encoded_arg_t e; e.type = TypeId::kUint32; e.first.u32 = v; e.second.u32 = v; return e; }
            static inline u32           decode(encoded_arg_t const& v) { return v.first.u32; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<s64>
        {
            static inline encoded_arg_t encode(s64 v) { encoded_arg_t e; e.type = TypeId::kInt64; e.first.s64 = v; e.second.s64 = v; return e; }
            static inline s64           decode(encoded_arg_t const& v) { return v.first.s64; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<u64>
        {
            static inline encoded_arg_t encode(u64 v) { encoded_arg_t e; e.type = TypeId::kUint64; e.first.u64 = v; e.second.u64 = v; return e; }
            static inline u64           decode(encoded_arg_t const& v) { return v.first.u64; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<void*>
        {
            static inline encoded_arg_t encode(void* v) { encoded_arg_t e; e.type = TypeId::kVoidPointer; e.first.vp = v; e.second.vp = v; return e; }
            static inline void*         decode(encoded_arg_t const& v) { return v.first.vp; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<const void*>
        {
            static inline encoded_arg_t encode(const void* v) { encoded_arg_t e; e.type = TypeId::kConstVoidPointer; e.first.cvp = v; e.second.cvp = v; return e; }
            static inline const void*   decode(encoded_arg_t const& v) { return v.first.cvp; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<float>
        {
            static inline encoded_arg_t encode(float v) { encoded_arg_t e; e.type = TypeId::kFloat; e.first.f = v; e.second.f = v; return e; }
            static inline float         decode(encoded_arg_t const& v) { return v.first.f; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<double>
        {
            static inline encoded_arg_t encode(double v) { encoded_arg_t e; e.type = TypeId::kDouble; e.first.d = v; e.second.d = v; return e; }
            static inline double        decode(encoded_arg_t const& v) { return v.first.d; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<char*>
        {
            static inline encoded_arg_t encode(char* v) { encoded_arg_t e; e.type = TypeId::kString; e.first.str = v; e.second.str = nullptr; return e; }
            static inline char*         decode(encoded_arg_t const& v) { return v.first.str; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<const char*>
        {
            static inline encoded_arg_t encode(const char* v) { encoded_arg_t e; e.type = TypeId::kConstString; e.first.cstr = v; e.second.cstr = nullptr; return e; }
            static inline const char*   decode(encoded_arg_t const& v) { return v.first.cstr; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <>
        struct arg_t<strspan_t>
        {
            static inline encoded_arg_t encode(const strspan_t& v) { encoded_arg_t e; e.type = TypeId::kStrSpan; e.first.str = v.begin(); e.second.str = v.end(); return e; }
            static inline strspan_t     decode(encoded_arg_t const& v) { return strspan_t(v.first.str, v.second.str); }
            static inline format_func_t formatter() { return nullptr; }
        };

        // clang-format on

        struct args_t
        {
            encoded_arg_t const* args;
            format_func_t const* funcs;
            s64                  size;
        };

        bool toStr(ascii::prune str, ascii::prune end, ascii::pcrune fmt, args_t const& args);

        template <typename... Args>
        bool toStr(ascii::prune str, s32 strMaxLen, ascii::pcrune fmt, Args... args)
        {
            const encoded_arg_t encoded_args[] = {arg_t<Args>::encode(args)...};
            const format_func_t arg_funcs[]  = {arg_t<Args>::formatter()...};

            args_t _args = {encoded_args, arg_funcs, (s64)(sizeof(encoded_args) / sizeof(encoded_arg_t))};
            return toStr(str, &str[strMaxLen], fmt, _args);
        }

    }  // namespace fmt
}  // namespace ncore

#endif  // __CBASE_STR_FMT_H__
