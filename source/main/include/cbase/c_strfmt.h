#ifndef __CBASE_STR_FMT_H__
#define __CBASE_STR_FMT_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif
#include "cbase/c_debug.h"
#include "cbase/c_runes.h"

namespace ncore
{
    namespace fmt
    {
        class str_t;
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
                kPointer,
                kFloat,
                kDouble,
                kString,
                kCustom
            };

            inline TypeId() : value(kBool) {}
            inline TypeId(EType value) : value(value) {}
            inline TypeId(const TypeId& other) : value(other.value) {}

            u8 value;
        };

        struct Flags
        {
            enum EFlag
            {
                kNone         = (0U << 0U),
                kEmpty        = (1U << 0U),
                kAlignNone    = (0U << 1U),
                kAlignLeft    = (1U << 1U),
                kAlignRight   = (2U << 1U),
                kAlignCenter  = (3U << 1U),
                kAlignNumeric = (4U << 1U),
                kAlignBitmask = kAlignLeft | kAlignRight | kAlignCenter | kAlignNumeric,
                kSignNone     = (0U << 4U),
                kSignMinus    = (1U << 4U),
                kSignPlus     = (2U << 4U),
                kSignSpace    = (3U << 4U),
                kSignBitmask  = kSignMinus | kSignPlus | kSignSpace,
                kHash         = (1U << 6U),
                kUppercase    = (1U << 7U)
            };

            inline Flags()
                : value(kEmpty)
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

            inline bool operator==(EFlag rhs) const noexcept { return value == rhs; }
            inline bool operator!=(EFlag rhs) const noexcept { return value != rhs; }

            inline Flags& operator =(EFlag rhs) noexcept
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
                kNone    = Flags::kAlignNone,
                kLeft    = Flags::kAlignLeft,
                kRight   = Flags::kAlignRight,
                kCenter  = Flags::kAlignCenter,
                kNumeric = Flags::kAlignNumeric
            };

            inline Align()
                : value(kNone)
            {
            }
            inline Align(s32 value)
                : value((u8)value)
            {
            }

            bool operator==(const EAlign& rhs) const noexcept { return value == rhs; }
            bool operator!=(const EAlign& rhs) const noexcept { return value != rhs; }

            u8 value;
        };

        struct Sign
        {
            enum ESign
            {
                kNone  = Flags::kSignNone,
                kMinus = Flags::kSignMinus,
                kPlus  = Flags::kSignPlus,
                kSpace = Flags::kSignSpace
            };
            inline Sign()
                : value(kNone)
            {
            }
            inline Sign(s32 value)
                : value((u8)value)
            {
            }

            inline bool operator==(ESign rhs) const noexcept { return value == rhs; }
            inline bool operator!=(ESign rhs) const noexcept { return value != rhs; }
            
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

            inline bool operator==(EType rhs) const noexcept { return value == rhs; }
            inline bool operator!=(EType rhs) const noexcept { return value != rhs; }

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

        class cstr_t;

        class str_t
        {
            enum
            {
                Type_Ascii,
                Type_Utf8,
                Type_Utf16,
                Type_Utf32
            } m_type;

        public:
            str_t() noexcept = delete;
            str_t(char* first, char* last);
            str_t(const str_t&) noexcept = default;
            str_t(str_t&&) noexcept      = default;
            template <uint_t N>
            str_t(char (&array)[N])
                : m_begin(array)
                , m_end(array + N - 1)
            {
            }

            str_t& operator=(const str_t&) noexcept = default;

            bool at_end() const noexcept { return m_begin == m_end; }
            void write(uchar32 c = '\0');
            void skip(const uint_t n);
            void write(cstr_t const& str);

        protected:
            char* m_begin;
            char* m_end;
        };

        class state_t
        {
        protected:
            friend class format_t;

        public:
            state_t() noexcept;
            state_t(const state_t&) noexcept = default;
            state_t(s8 width, TypeId type = TypeId::kString, Flags flags = Flags::kAlignCenter, s8 precision = 0, char fill_char = ' ') noexcept;

            static bool format_string(str_t& it, state_t& state, const char* str, const char* end);
            static bool format_string(str_t& it, state_t& state, const cstr_t& str);

            inline constexpr char fill_char() const noexcept { return m_fill_char; }
            inline constexpr Type type() const noexcept { return m_type; }
            inline constexpr s32  width() const noexcept { return static_cast<s32>(m_width); }
            inline constexpr s32  precision() const noexcept { return static_cast<s32>(m_precision); }
            inline constexpr s32  index() const noexcept { return static_cast<s32>(m_index); }

            inline Align align() const noexcept { return Align(m_flags.value & Flags::kAlignBitmask); }
            inline Sign  sign() const noexcept { return Sign(m_flags.value & Flags::kSignBitmask); }

            inline constexpr bool is_empty() const noexcept { return (m_flags.value & Flags::kEmpty) != 0; }
            inline constexpr bool hash() const noexcept { return (m_flags.value & Flags::kHash) != 0; }
            inline constexpr bool uppercase() const noexcept { return (m_flags.value & Flags::kUppercase) != 0; }

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
            s32  write_alignment(str_t& it, s32 characters, const bool negative) const;
            void write_sign(str_t& it, const bool negative) const noexcept;
            void write_prefix(str_t& it) const noexcept;

            static bool format_string(str_t& it, const state_t& state, const cstr_t& str, const s32 str_length, const bool negative = false);

            inline constexpr s32 sign_width(const bool negative) const noexcept { return (!negative && sign().value <= Sign::kMinus) ? 0 : 1; }

            // Alternative format is valid for hexadecimal (including pointers), octal, binary and all floating point types.
            inline constexpr s32 prefix_width() const noexcept { return (!hash() || type_is_float()) ? 0 : type_is_integer_oct() ? 1 : 2; }

            char  m_fill_char = ' ';
            Type  m_type      = {Type::kNone};
            Flags m_flags     = {Flags::kEmpty};
            u8    m_width     = 0;
            s8    m_precision = -1;
            s8    m_index     = -1;
        };

        typedef void (*format_func_t)(u8 argType, u64 argValue, str_t& dst, state_t& format);

        // Arg defs
        template <typename T> struct arg_t
        {
            static inline u64           encode(T v) { return *((u64*)(&v)); }
            static inline T             decode(u64 v) { return *((T*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<bool>
        {
            static inline u64           encode(bool v) { return *((u64*)(&v)); }
            static inline bool          decode(u64 v) { return *((bool*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<char>
        {
            static inline u64           encode(char v) { return *((u64*)(&v)); }
            static inline char          decode(u64 v) { return *((char*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<s8>
        {
            static inline u64           encode(s8 v) { return *((u64*)(&v)); }
            static inline s8            decode(u64 v) { return *((s8*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<s16>
        {
            static inline u64           encode(s16 v) { return *((u64*)(&v)); }
            static inline s16           decode(u64 v) { return *((s16*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<s32>
        {
            static inline u64           encode(s32 v) { return *((u64*)(&v)); }
            static inline s32           decode(u64 v) { return *((s32*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<u8>
        {
            static inline u64           encode(u8 v) { return *((u64*)(&v)); }
            static inline u8            decode(u64 v) { return *((u8*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<u16>
        {
            static inline u64           encode(u16 v) { return *((u64*)(&v)); }
            static inline u16           decode(u64 v) { return *((u16*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<u32>
        {
            static inline u64           encode(u32 v) { return *((u64*)(&v)); }
            static inline u32           decode(u64 v) { return *((u32*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<s64>
        {
            static inline u64           encode(s64 v) { return *((u64*)(&v)); }
            static inline s64           decode(u64 v) { return *((s64*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<u64>
        {
            static inline u64           encode(u64 v) { return v; }
            static inline u64           decode(u64 v) { return v; }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<void*>
        {
            static inline u64           encode(void* v) { return reinterpret_cast<u64>(v); }
            static inline const void*   decode(u64 v) { return *((const void**)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<const void*>
        {
            static inline u64           encode(const void* v) { return reinterpret_cast<u64>(v); }
            static inline const void*   decode(u64 v) { return *((const void**)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<float>
        {
            static inline u64           encode(float v) { return *((u64*)(&v)); }
            static inline float         decode(u64 v) { return *((float*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<double>
        {
            static inline u64           encode(double v) { return *((u64*)(&v)); }
            static inline double        decode(u64 v) { return *((double*)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<char*>
        {
            static inline u64           encode(char* v) { return reinterpret_cast<u64>(v); }
            static inline const char*   decode(u64 v) { return *((const char**)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        template <> struct arg_t<const char*>
        {
            static inline u64           encode(const char* v) { return reinterpret_cast<u64>(v); }
            static inline const char*   decode(u64 v) { return *((const char**)(&v)); }
            static inline format_func_t formatter() { return nullptr; }
        };

        // Type defs

        template <typename T> struct typed
        {
            static const u8 value = kCustom;
        };

        template <> struct typed<bool>
        {
            static const u8 value = TypeId::kBool;
        };

        template <> struct typed<char>
        {
            static const u8 value = TypeId::kChar;
        };

        template <> struct typed<s8>
        {
            static const u8 value = TypeId::kInt8;
        };

        template <> struct typed<s16>
        {
            static const u8 value = TypeId::kInt16;
        };

        template <> struct typed<s32>
        {
            static const u8 value = TypeId::kInt32;
        };

        template <> struct typed<u8>
        {
            static const u8 value = TypeId::kUint8;
        };

        template <> struct typed<u16>
        {
            static const u8 value = TypeId::kUint16;
        };

        template <> struct typed<u32>
        {
            static const u8 value = TypeId::kUint32;
        };

        template <> struct typed<s64>
        {
            static const u8 value = TypeId::kInt64;
        };

        template <> struct typed<u64>
        {
            static const u8 value = TypeId::kUint64;
        };

        template <> struct typed<void*>
        {
            static const u8 value = TypeId::kPointer;
        };

        template <> struct typed<const void*>
        {
            static const u8 value = TypeId::kPointer;
        };

        template <> struct typed<float>
        {
            static const u8 value = TypeId::kFloat;
        };

        template <> struct typed<double>
        {
            static const u8 value = TypeId::kDouble;
        };

        template <> struct typed<char*>
        {
            static const u8 value = TypeId::kString;
        };

        template <> struct typed<const char*>
        {
            static const u8 value = TypeId::kString;
        };

        struct args_t
        {
            u64 const*           args;
            u8 const*            types;
            format_func_t const* funcs;
            s32                  size;
        };

        bool toStr(ascii::prune str, ascii::prune end, ascii::pcrune fmt, args_t const& args);

        template <typename... Args> bool toStr(ascii::prune str, s32 strMaxLen, ascii::pcrune fmt, Args... args)
        {
            const u8            types[]  = {typed<Args>::value...};
            const u64           values[] = {arg_t<Args>::encode(args)...};
            const format_func_t funcs[]  = {arg_t<Args>::formatter()...};

            args_t _args = {values, types, funcs, (s32)(sizeof(types) / sizeof(types[0]))};
            return toStr(str, &str[strMaxLen], fmt, _args);
        }

    } // namespace fmt
} // namespace ncore

#endif // __CBASE_STR_FMT_H__