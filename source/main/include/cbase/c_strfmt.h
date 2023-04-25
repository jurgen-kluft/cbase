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
        class BasicStringSpan;
        class ArgFormat;

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

        class BasicStringView;

        class BasicStringSpan
        {
            enum
            {
                Type_Ascii,
                Type_Utf8,
                Type_Utf16,
                Type_Utf32
            } m_type;

        public:
            BasicStringSpan() noexcept = delete;
            BasicStringSpan(char* first, char* last);
            BasicStringSpan(const BasicStringSpan&) noexcept = default;
            BasicStringSpan(BasicStringSpan&&) noexcept      = default;

            BasicStringSpan& operator=(const BasicStringSpan&) noexcept = default;

            bool at_end() const noexcept { return m_begin == m_end; }
            void write(uchar32 c = '\0');
            void skip(const uint_t n);
            void write(BasicStringView const& str);

        protected:
            char* m_begin;
            char* m_end;
        };

        class ArgFormatState
        {
        protected:
            friend class ArgFormat;

            enum Flags : u8
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

        public:
            using iterator       = char*;
            using const_iterator = const char*;

            enum class Align : u8
            {
                kNone    = kAlignNone,
                kLeft    = kAlignLeft,
                kRight   = kAlignRight,
                kCenter  = kAlignCenter,
                kNumeric = kAlignNumeric
            };

            enum class Sign : u8
            {
                kNone  = kSignNone,
                kMinus = kSignMinus,
                kPlus  = kSignPlus,
                kSpace = kSignSpace
            };

            enum class Type : u8
            {
                kNone,
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

            static void format_string(BasicStringSpan& it, ArgFormatState& state, const char* str, const char* end);
            static void format_string(BasicStringSpan& it, ArgFormatState& state, const BasicStringView& str);

            inline constexpr char fill_char() const noexcept { return m_fill_char; }
            inline constexpr Type type() const noexcept { return m_type; }
            inline constexpr int  width() const noexcept { return static_cast<int>(m_width); }
            inline constexpr int  precision() const noexcept { return static_cast<int>(m_precision); }
            inline constexpr int  index() const noexcept { return static_cast<int>(m_index); }

            inline constexpr Align align() const noexcept { return Align(m_flags & Flags::kAlignBitmask); }
            inline constexpr Sign  sign() const noexcept { return Sign(m_flags & Flags::kSignBitmask); }

            inline constexpr bool is_empty() const noexcept { return (m_flags & Flags::kEmpty) != 0; }
            inline constexpr bool hash() const noexcept { return (m_flags & Flags::kHash) != 0; }
            inline constexpr bool uppercase() const noexcept { return (m_flags & Flags::kUppercase) != 0; }

            inline constexpr bool type_is_none() const noexcept { return m_type == Type::kNone; }
            inline constexpr bool type_is_char() const noexcept { return m_type == Type::kChar; }
            inline constexpr bool type_is_string() const noexcept { return m_type == Type::kString; }
            inline constexpr bool type_is_pointer() const noexcept { return m_type == Type::kPointer; }

            inline constexpr bool type_is_integer() const noexcept { return m_type >= Type::kIntegerDec && m_type <= Type::kIntegerBin; }
            inline constexpr bool type_is_float() const noexcept { return m_type >= Type::kFloatFixed && m_type <= Type::kFloatGeneral; }
            inline constexpr bool type_is_numeric() const noexcept { return m_type >= Type::kIntegerDec && m_type <= Type::kFloatGeneral; }

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
                return m_type >= Type::kIntegerHex && m_type <= Type::kFloatGeneral;
            }

        protected:
            friend class Arg;
            
            // Writes the alignment (sign, prefix and fill before) for any
            // argument type. Returns the fill counter to write after argument.
            int write_alignment(BasicStringSpan& it, int digits, const bool negative) const;

            static void format_string(BasicStringSpan& it, const ArgFormatState& state, BasicStringView& str, const int str_length, const bool negative = false);

            inline constexpr int sign_width(const bool negative) const noexcept { return (!negative && sign() <= Sign::kMinus) ? 0 : 1; }

            // Alternative format is valid for hexadecimal (including pointers), octal, binary and all floating point types.
            inline constexpr int prefix_width() const noexcept { return (!hash() || type_is_float()) ? 0 : type_is_integer_oct() ? 1 : 2; }

            void write_sign(BasicStringSpan& it, const bool negative) const noexcept;
            void write_prefix(BasicStringSpan& it) const noexcept;

            char m_fill_char = ' ';
            Type m_type      = Type::kNone;
            u8   m_flags     = Flags::kEmpty;
            u8   m_width     = 0;
            s8   m_precision = -1;
            s8   m_index     = -1;
        };

        typedef void (*ArgFormatFunc)(u8 argType, u64 argValue, BasicStringSpan& dst, ArgFormatState& format);

        void FormatArg(u8 argType, u64 argValue, BasicStringSpan& dst, ArgFormatState& format);

        // Arg defs
        template <typename T> struct arg_t
        {
            static inline u64           encode(T v) { return *((u64*)(&v)); }
            static inline T             decode(u64 v) { return *((T*)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<bool>
        {
            static inline u64           encode(bool v) { return *((u64*)(&v)); }
            static inline bool          decode(u64 v) { return *((bool*)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<char>
        {
            static inline u64           encode(char v) { return *((u64*)(&v)); }
            static inline char          decode(u64 v) { return *((char*)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<s32>
        {
            static inline u64           encode(s32 v) { return *((u64*)(&v)); }
            static inline s32           decode(u64 v) { return *((s32*)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<u32>
        {
            static inline u64           encode(u32 v) { return *((u64*)(&v)); }
            static inline u32           decode(u64 v) { return *((u32*)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<s64>
        {
            static inline u64           encode(s64 v) { return *((u64*)(&v)); }
            static inline s64           decode(u64 v) { return *((s64*)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<u64>
        {
            static inline u64           encode(u64 v) { return v; }
            static inline u64           decode(u64 v) { return v; }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<void*>
        {
            static inline u64           encode(void* v) { return reinterpret_cast<u64>(v); }
            static inline const void*   decode(u64 v) { return *((const void**)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<const void*>
        {
            static inline u64           encode(const void* v) { return reinterpret_cast<u64>(v); }
            static inline const void*   decode(u64 v) { return *((const void**)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<float>
        {
            static inline u64           encode(float v) { return *((u64*)(&v)); }
            static inline float         decode(u64 v) { return *((float*)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<double>
        {
            static inline u64           encode(double v) { return *((u64*)(&v)); }
            static inline double        decode(u64 v) { return *((double*)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<char*>
        {
            static inline u64           encode(char* v) { return reinterpret_cast<u64>(v); }
            static inline const char*   decode(u64 v) { return *((const char**)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        template <> struct arg_t<const char*>
        {
            static inline u64           encode(const char* v) { return reinterpret_cast<u64>(v); }
            static inline const char*   decode(u64 v) { return *((const char**)(&v)); }
            static inline ArgFormatFunc formatter() { return FormatArg; }
        };

        // Type defs

        template <typename T> struct typed
        {
            static const u8 value = kCustom;
        };

        template <> struct typed<bool>
        {
            static const u8 value = kBool;
        };

        template <> struct typed<char>
        {
            static const u8 value = kChar;
        };

        template <> struct typed<s32>
        {
            static const u8 value = kInt32;
        };

        template <> struct typed<u32>
        {
            static const u8 value = kUint32;
        };

        template <> struct typed<s64>
        {
            static const u8 value = kInt64;
        };

        template <> struct typed<u64>
        {
            static const u8 value = kUint64;
        };

        template <> struct typed<void*>
        {
            static const u8 value = kPointer;
        };

        template <> struct typed<const void*>
        {
            static const u8 value = kPointer;
        };

        template <> struct typed<float>
        {
            static const u8 value = kFloat;
        };

        template <> struct typed<double>
        {
            static const u8 value = kDouble;
        };

        template <> struct typed<char*>
        {
            static const u8 value = kString;
        };

        template <> struct typed<const char*>
        {
            static const u8 value = kString;
        };

        struct args_t
        {
            u64 const*           args;
            u8 const*            types;
            ArgFormatFunc const* funcs;
            s32                  size;
        };

        ascii::prune toStr(ascii::prune str, ascii::prune end, ascii::pcrune fmt, args_t const& args);

        template <typename... Args> ascii::prune toStr(ascii::prune str, s32 strMaxLen, ascii::pcrune fmt, Args... args)
        {
            const u8            types[]  = {typed<Args>::value...};
            const u64           values[] = {arg_t<Args>::encode(args)...};
            const ArgFormatFunc funcs[]  = {arg_t<Args>::formatter()...};

            args_t _args = {values, types, funcs, (s32)(sizeof(types) / sizeof(types[0]))};
            return toStr(str, &str[strMaxLen], fmt, _args);
        }

    } // namespace fmt
} // namespace ncore

#endif // __CBASE_STR_FMT_H__