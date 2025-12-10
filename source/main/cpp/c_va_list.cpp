#include "ccore/c_debug.h"
#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"

namespace ncore
{
    const va_t va_t::sEmpty;

    va_t::va_t(const char* inVar)
        : mArg3(TYPE_PCRUNES)
    {
        mArg = (ptr_t)inVar;
        mArg3 |= ascii::TYPE;
        mArg2 = (u32)ascii::strlen(inVar);
    }

    va_t::va_t(const wchar_t* inVar)
        : mArg3(TYPE_PCRUNES)
    {
        mArg = (ptr_t)inVar;
        mArg3 |= ascii::TYPE;
        mArg2             = 0;
        const wchar_t* it = inVar;
        while (*it != L'\0')
        {
            ++it;
            ++mArg2;
        }
    }

    va_t::va_t(crunes_t const& str)
        : mArg3(TYPE_PCRUNES)
    {
        mArg3 |= (str.m_type);
        switch (str.m_type)
        {
            case ascii::TYPE: mArg = (ptr_t)(&str.m_ascii[str.m_str]); break;
            case utf8::TYPE: mArg = (ptr_t)(&str.m_utf8[str.m_str]); break;
            case utf16::TYPE: mArg = (ptr_t)(&str.m_utf16[str.m_str]); break;
            case utf32::TYPE: mArg = (ptr_t)(&str.m_utf32[str.m_str]); break;
            default: break;
        }
        mArg2 = str.m_end - str.m_str;
    }

    char va_t::specifier() const
    {
        switch (mArg3 & TYPE_MASK)
        {
            case TYPE_BOOL: return 'b';
            case TYPE_UINT32: return 'u';
            case TYPE_INT32: return 'd';
            case TYPE_UINT8: return 'u';
            case TYPE_INT8: return 'd';
            case TYPE_UINT16: return 'u';
            case TYPE_INT16: return 'd';
            case TYPE_UINT64: return 'u';
            case TYPE_INT64: return 'd';
            case TYPE_FLOAT32: return 'f';
            case TYPE_FLOAT64: return 'f';
            case TYPE_PCRUNES: return 's';
            default: return 's';
        }
    }

    void va_t::convertToRunes(runes_t& str) const
    {
        switch (mArg3 & TYPE_MASK)
        {
            case TYPE_BOOL:
            {
                bool v = (*(bool*)&mArg);
                nrunes::to_string(str, v);
            }
            break;
            case TYPE_UINT32:
            {
                u32 v = (*(u32*)&mArg);
                nrunes::to_string(str, v);
            }
            break;
            case TYPE_INT32:
            {
                s32 v = (*(s32*)&mArg);
                nrunes::to_string(str, v);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                s32 v = (*(s8*)&mArg);
                nrunes::to_string(str, v);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                s32 v = (*(s16*)&mArg);
                nrunes::to_string(str, v);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                s64 v = (*(s64*)&mArg);
                nrunes::to_string(str, v);
            }
            break;
            case TYPE_FLOAT32:
            {
                f32 v = (*(f32*)&mArg);
                nrunes::to_string(str, v);
            }
            break;
            case TYPE_FLOAT64:
            {
                f64 v = (*(f64*)&mArg);
                nrunes::to_string(str, v);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch = ascii::make_crunes((const char*)mArg, 0, mArg2, mArg2, (u32)mArg3 & 0x0F);
                nrunes::copy(ch, str);
            }
            break;
            default: break;  // Fall through
        };
    }

    s8 va_t::convertToInt8() const
    {
        u32 i = convertToUInt32();
        return (s8)i;
    }

    u8 va_t::convertToUInt8() const
    {
        u32 i = convertToUInt32();
        return (u8)i;
    }

    s16 va_t::convertToInt16() const
    {
        s32 i = convertToInt32();
        return (s16)i;
    }

    u16 va_t::convertToUInt16() const
    {
        u32 i = convertToUInt32();
        return (u16)i;
    }

    s32 va_t::convertToInt32() const
    {
        s32 i = (s32)convertToInt64();
        return i;
    }

    u32 va_t::convertToUInt32() const { return (u32)convertToUInt64(); }

    s64 va_t::convertToInt64() const
    {
        s64 i = 0;
        switch (mArg3 & TYPE_MASK)
        {
            case TYPE_BOOL:
            case TYPE_UINT32:
            {
                i = (s64)(*(u32*)&mArg);
            }
            break;
            case TYPE_INT32:
            {
                i = (s64)(*(u32*)&mArg);
            }
            break;

            case TYPE_INT8:
            {
                i = (s64)(*(s8*)&mArg);
            }
            break;
            case TYPE_UINT8:
            {
                i = (s64)(*(u8*)&mArg);
            }
            break;
            case TYPE_INT16:
            {
                i = (s64)(*(s16*)&mArg);
            }
            break;
            case TYPE_UINT16:
            {
                i = (s64)(*(u16*)&mArg);
            }
            break;
            case TYPE_UINT64:
            {
                i = (s64)(*(u64*)&mArg);
            }
            break;
            case TYPE_INT64:
            {
                i = (*(s64*)&mArg);
            }
            break;
            case TYPE_FLOAT32:
            {
                i = (s64)(*(f32*)&mArg);
            }
            break;
            case TYPE_FLOAT64:
            {
                i = (s64)(*(f64*)&mArg);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch = ascii::make_crunes((const char*)mArg, 0, mArg2, mArg2, (u32)mArg3 & 0x0F);
                nrunes::parse(ch, i);
            }
            break;
            default: break;  // Fall through
        };

        return i;
    }

    u64 va_t::convertToUInt64() const
    {
        u64 i = 0;
        switch (mArg3 & TYPE_MASK)
        {
            case TYPE_BOOL:
            case TYPE_UINT32:
            case TYPE_INT32:
            {
                i = (u64)(*(u32*)&mArg);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                i = (u64)(*(u8*)&mArg);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                i = (u64)(*(u16*)&mArg);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                i = (u64)(*(u64*)&mArg);
            }
            break;
            case TYPE_FLOAT32:
            {
                i = (u64)(*(f32*)&mArg);
            }
            break;
            case TYPE_FLOAT64:
            {
                i = (u64)(*(f64*)&mArg);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch = ascii::make_crunes((const char*)mArg, 0, mArg2, mArg2, (u32)mArg3 & 0x0F);
                nrunes::parse(ch, i);
            }
            break;
            default: break;  // Fall through
        };

        return i;
    }

    f32 va_t::convertToFloat() const { return (f32)convertToDouble(); }

    f64 va_t::convertToDouble() const
    {
        f64 i = 0.0;
        switch (mArg3 & TYPE_MASK)
        {
            case TYPE_BOOL:
            case TYPE_UINT32:
            case TYPE_INT32:
            {
                i = (f64)(*(u32*)&mArg);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                i = (f64)(*(u8*)&mArg);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                i = (f64)(*(u16*)&mArg);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                i = (f64)(*(u64*)&mArg);
            }
            break;

            case TYPE_FLOAT32:
            {
                i = (f64)(*(f32*)&mArg);
            }
            break;
            case TYPE_FLOAT64:
            {
                i = (f64)(*(f64*)&mArg);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch = ascii::make_crunes((const char*)mArg, 0, mArg2, mArg2, (u32)mArg3 & 0x0F);
                nrunes::parse(ch, i);
            }
            break;
            default: break;  // Fall through
        };

        return i;
    }

    bool va_t::convertToBool() const
    {
        u32 i = 0;
        switch (mArg3 & TYPE_MASK)
        {
            case TYPE_BOOL:
            case TYPE_UINT32:
            case TYPE_INT32:
            {
                i = (u32)(*(u32*)&mArg);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                i = (u32)(*(u8*)&mArg);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                i = (u32)(*(u16*)&mArg);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                i = (u32)(*(u64*)&mArg);
            }
            break;
            case TYPE_FLOAT32:
            {
                i = (u32)(*(f32*)&mArg);
            }
            break;
            case TYPE_FLOAT64:
            {
                i = (u32)(*(f64*)&mArg);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch = ascii::make_crunes((const char*)mArg, 0, mArg2, mArg2, (u32)mArg3 & 0x0F);
                bool     b  = false;
                nrunes::parse(ch, b);
                i = b ? 1 : 0;
            }
            break;

            default: break;  // Fall through
        };

        return i != 0;
    }

    crunes_t va_t::convertToCRunes() const
    {
        switch (mArg3 & TYPE_MASK)
        {
            case TYPE_PCRUNES:
            {
                crunes_t ch = ascii::make_crunes((const char*)mArg, 0, mArg2, mArg2, (u32)mArg3 & 0x0F);
                return ch;
            }
            default: break;  // Fall through
        };
        return crunes_t();
    }

    va_r_t va_r_t::sEmpty;

    va_r_t& va_r_t::operator=(s8 rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef) = rhs != 0 ? true : false; break;
            case TYPE_UINT32: *((u32*)mRef) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_AINT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_UINT8: *((u8*)mRef) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef) = (u64)rhs; break;
            case TYPE_INT64: *((s64*)mRef) = (s64)rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef) = (f64)rhs; break;
            default: break;  // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(u8 rhs)
    {
        *this = (s8)rhs;
        return *this;
    }

    va_r_t& va_r_t::operator=(s16 rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef) = (bool)rhs != 0 ? true : false; break;
            case TYPE_UINT32: *((u32*)mRef) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_AINT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_UINT8: *((u8*)mRef) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef) = (u64)rhs; break;
            case TYPE_INT64: *((s64*)mRef) = (s64)rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef) = (f64)rhs; break;
            default: break;  // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(u16 rhs)
    {
        *this = (s16)rhs;
        return *this;
    }

    va_r_t& va_r_t::operator=(s32 rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef) = (bool)rhs != 0 ? true : false; break;
            case TYPE_UINT32: *((u32*)mRef) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_AINT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_UINT8: *((u8*)mRef) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef) = (u64)rhs; break;
            case TYPE_INT64: *((s64*)mRef) = (s64)rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef) = (f64)rhs; break;
            default: break;  // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(u32 rhs)
    {
        *this = (s32)rhs;
        return *this;
    }

    va_r_t& va_r_t::operator=(s64 rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef) = (bool)rhs != 0 ? true : false; break;
            case TYPE_UINT32: *((u32*)mRef) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_AINT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_UINT8: *((u8*)mRef) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef) = (u64)rhs; break;
            case TYPE_INT64: *((s64*)mRef) = rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef) = (f64)rhs; break;
            default: break;  // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(u64 rhs)
    {
        *this = (s64)rhs;
        return *this;
    }

    va_r_t& va_r_t::operator=(f32 rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef) = rhs != 0 ? true : false; break;
            case TYPE_UINT32: *((u32*)mRef) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_AINT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_UINT8: *((u8*)mRef) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef) = (u64)rhs; break;
            case TYPE_INT64: *((s64*)mRef) = (s64)rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef) = (f64)rhs; break;
            default: break;  // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(f64 rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef) = rhs != 0 ? true : false; break;
            case TYPE_UINT32: *((u32*)mRef) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_AINT32: *((s32*)mRef) = (s32)rhs; break;
            case TYPE_UINT8: *((u8*)mRef) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef) = (u64)rhs; break;
            case TYPE_INT64: *((s64*)mRef) = (s64)rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef) = (f64)rhs; break;
            default: break;  // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(bool rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef) = bool(rhs); break;
            case TYPE_UINT32: *((u32*)mRef) = rhs ? 1 : 0; break;
            case TYPE_INT32: *((s32*)mRef) = rhs ? 1 : 0; break;
            case TYPE_AINT32: *((s32*)mRef) = rhs ? 1 : 0; break;
            case TYPE_UINT8: *((u8*)mRef) = rhs ? 1 : 0; break;
            case TYPE_INT8: *((s8*)mRef) = rhs ? 1 : 0; break;
            case TYPE_UINT16: *((u16*)mRef) = rhs ? 1 : 0; break;
            case TYPE_INT16: *((s16*)mRef) = rhs ? 1 : 0; break;
            case TYPE_UINT64: *((u64*)mRef) = rhs ? 1 : 0; break;
            case TYPE_INT64: *((s64*)mRef) = rhs ? 1 : 0; break;
            case TYPE_FLOAT32: *((f32*)mRef) = rhs ? 1.0f : 0.0f; break;
            case TYPE_FLOAT64: *((f64*)mRef) = rhs ? 1.0 : 0.0; break;
            default: break;  // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(const crunes_t& rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: nrunes::parse(rhs, *((bool*)mRef)); break;
            case TYPE_UINT32: nrunes::parse(rhs, *((u32*)mRef)); break;
            case TYPE_INT32: nrunes::parse(rhs, *((s32*)mRef)); break;
            case TYPE_AINT32: nrunes::parse(rhs, *((s32*)mRef)); break;
            case TYPE_UINT8: nrunes::parse(rhs, *((u8*)mRef)); break;
            case TYPE_INT8: nrunes::parse(rhs, *((s8*)mRef)); break;
            case TYPE_UINT16: nrunes::parse(rhs, *((u16*)mRef)); break;
            case TYPE_INT16: nrunes::parse(rhs, *((s16*)mRef)); break;
            case TYPE_UINT64: nrunes::parse(rhs, *((u64*)mRef)); break;
            case TYPE_INT64: nrunes::parse(rhs, *((s64*)mRef)); break;
            case TYPE_FLOAT32: nrunes::parse(rhs, *((f32*)mRef)); break;
            case TYPE_FLOAT64: nrunes::parse(rhs, *((f64*)mRef)); break;
            case TYPE_ASCII_STR:
            {
                runes_t dst = ascii::make_runes((char*)mRef, 0, 0, mCap, ascii::TYPE);
                nrunes::copy(rhs, dst);
            }
            break;
            case TYPE_USC2_STR:
            {
                runes_t dst = ucs2::make_runes((ucs2::prune)mRef, 0, 0, mCap, ucs2::TYPE);
                nrunes::copy(rhs, dst);
            }
            break;
            case TYPE_UTF8_STR:
            {
                runes_t dst = utf16::make_runes((utf16::prune)mRef, 0, 0, mCap, utf8::TYPE);
                nrunes::copy(rhs, dst);
            }
            break;
            case TYPE_UTF16_STR:
            {
                runes_t dst = utf16::make_runes((utf16::prune)mRef, 0, 0, mCap, utf16::TYPE);
                nrunes::copy(rhs, dst);
            }
            break;
            case TYPE_UTF32_STR:
            {
                runes_t dst = utf32::make_runes((utf32::prune)mRef, 0, 0, mCap, utf32::TYPE);
                nrunes::copy(rhs, dst);
            }
            break;
            default: break;  // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(va_t const& rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef) = rhs; break;
            case TYPE_UINT32: *((u32*)mRef) = rhs; break;
            case TYPE_INT32: *((s32*)mRef) = rhs; break;
            case TYPE_AINT32: *((s32*)mRef) = rhs; break;
            case TYPE_UINT8: *((u8*)mRef) = rhs; break;
            case TYPE_INT8: *((s8*)mRef) = rhs; break;
            case TYPE_UINT16: *((u16*)mRef) = rhs; break;
            case TYPE_INT16: *((s16*)mRef) = rhs; break;
            case TYPE_UINT64: *((u64*)mRef) = rhs; break;
            case TYPE_INT64: *((s64*)mRef) = rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef) = rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef) = rhs; break;
            case TYPE_ASCII_STR:
            {
                runes_t dst = ascii::make_runes((char*)mRef, 0, 0, mCap, ascii::TYPE);
                rhs.convertToRunes(dst);
            }
            break;
            case TYPE_USC2_STR:
            {
                runes_t dst = ucs2::make_runes((ucs2::prune)mRef, 0, 0, mCap, ucs2::TYPE);
                rhs.convertToRunes(dst);
            }
            break;
            case TYPE_UTF8_STR:
            {
                runes_t dst = utf16::make_runes((utf16::prune)mRef, 0, 0, mCap, utf8::TYPE);
                rhs.convertToRunes(dst);
            }
            break;
            case TYPE_UTF16_STR:
            {
                runes_t dst = utf16::make_runes((utf16::prune)mRef, 0, 0, mCap, utf16::TYPE);
                rhs.convertToRunes(dst);
            }
            break;
            case TYPE_UTF32_STR:
            {
                runes_t dst = utf32::make_runes((utf32::prune)mRef, 0, 0, mCap, utf32::TYPE);
                rhs.convertToRunes(dst);
            }
            break;
            default: break;  // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator+=(s32 rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef) = rhs != 0 ? true : false; break;
            case TYPE_UINT32: *((u32*)mRef) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef) = rhs; break;
            case TYPE_AINT32:
                if (mSize < mCap)
                    *((s32*)mRef + mSize++) = rhs;
                break;
            case TYPE_UINT8: *((u8*)mRef) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef) = (u64)rhs; break;
            case TYPE_INT64: *((s64*)mRef) = (s64)rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef) = (f64)rhs; break;
            default: break;  // Fall through
        };

        return *this;
    }

};  // namespace ncore
