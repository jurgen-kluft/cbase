#include "xbase/x_debug.h"
#include "xbase/x_va_list.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

namespace ncore
{
    const va_t va_t::sEmpty;

    va_t::va_t(const char* inVar) : mType(TYPE_PCRUNES)
    {
        mArg[0] = ascii::TYPE;
        mArg[1] = (u64)inVar;
        mArg[2] = (u64)inVar + ascii::strlen(inVar);
    }

    va_t::va_t(crunes_t const& str) : mType(TYPE_PCRUNES)
    {
        mArg[0] = str.m_type;
        mArg[1] = (u64)str.m_ascii.m_str;
        mArg[2] = (u64)str.m_ascii.m_end;
    }

    void va_t::convertToRunes(runes_t& str) const
    {
        u32 i = 0;
        switch (mType)
        {
            case TYPE_BOOL:
            {
                bool v = (*(bool*)mArg);
                to_string(str, v);
            }
            break;
            case TYPE_UINT32:
            {
                u32 v = (*(u32*)mArg);
                to_string(str, v);
            }
            break;
            case TYPE_INT32:
            {
                s32 v = (*(u32*)mArg);
                to_string(str, v);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                s32 v = (*(s8*)mArg);
                to_string(str, v);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                s32 v = (*(s16*)mArg);
                to_string(str, v);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                s64 v = (*(s64*)mArg);
                to_string(str, v);
            }
            break;
            case TYPE_FLOAT32:
            {
                f32 v = (*(f32*)mArg);
                to_string(str, v);
            }
            break;
            case TYPE_FLOAT64:
            {
                f64 v = (*(f64*)mArg);
                to_string(str, v);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch((ascii::pcrune)mArg[1], (ascii::pcrune)mArg[2]);
                ch.m_type = (s32)mArg[0];
                copy(ch, str);
            }
            break;
            default: break; // Fall through
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
        u32 i = convertToUInt32();
        return (s16)i;
    }

    u16 va_t::convertToUInt16() const
    {
        u32 i = convertToUInt32();
        return (u16)i;
    }

    s32 va_t::convertToInt32() const
    {
        u32 i = convertToUInt32();
        return (s32)i;
    }

    u32 va_t::convertToUInt32() const
    {
        u32 i = 0;
        switch (mType)
        {
            case TYPE_BOOL:
            case TYPE_UINT32:
            case TYPE_INT32:
            {
                i = (u32)(*(u32*)mArg);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                i = (u32)(*(u8*)mArg);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                i = (u32)(*(u16*)mArg);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                i = (u32)(*(u64*)mArg);
            }
            break;
            case TYPE_FLOAT32:
            {
                i = (u32)(*(f32*)mArg);
            }
            break;
            case TYPE_FLOAT64:
            {
                i = (u32)(*(f64*)mArg);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch((ascii::pcrune)mArg[1], (ascii::pcrune)mArg[2]);
                ch.m_type = (s32)mArg[0];
                parse(ch, i);
            }
            break;
            default: break; // Fall through
        };

        return i;
    }

    s64 va_t::convertToInt64() const
    {
        u64 i = convertToUInt64();
        return (s64)i;
    }

    u64 va_t::convertToUInt64() const
    {
        u64 i = 0;
        switch (mType)
        {
            case TYPE_BOOL:
            case TYPE_UINT32:
            case TYPE_INT32:
            {
                i = (u64)(*(u32*)mArg);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                i = (u64)(*(u8*)mArg);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                i = (u64)(*(u16*)mArg);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                i = (u64)(*(u64*)mArg);
            }
            break;
            case TYPE_FLOAT32:
            {
                i = (u64)(*(f32*)mArg);
            }
            break;
            case TYPE_FLOAT64:
            {
                i = (u64)(*(f64*)mArg);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch((ascii::pcrune)mArg[1], (ascii::pcrune)mArg[2]);
                ch.m_type = (s32)mArg[0];
                parse(ch, i);
            }
            break;
            default: break; // Fall through
        };

        return i;
    }

    f32 va_t::convertToFloat() const
    {
        f32 i = 0.0f;
        switch (mType)
        {
            case TYPE_BOOL:
            case TYPE_UINT32:
            case TYPE_INT32:
            {
                i = (f32)(*(u32*)mArg);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                i = (f32)(*(u8*)mArg);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                i = (f32)(*(u16*)mArg);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                i = (f32)(*(u64*)mArg);
            }
            break;

            case TYPE_FLOAT32:
            {
                i = (f32)(*(f32*)mArg);
            }
            break;
            case TYPE_FLOAT64:
            {
                i = (f32)(*(f64*)mArg);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch((ascii::pcrune)mArg[1], (ascii::pcrune)mArg[2]);
                ch.m_type = (s32)mArg[0];
                parse(ch, i);
            }
            break;
            default: break; // Fall through
        };

        return i;
    }

    f64 va_t::convertToDouble() const
    {
        f64 i = 0.0;
        switch (mType)
        {
            case TYPE_BOOL:
            case TYPE_UINT32:
            case TYPE_INT32:
            {
                i = (f64)(*(u32*)mArg);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                i = (f64)(*(u8*)mArg);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                i = (f64)(*(u16*)mArg);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                i = (f64)(*(u64*)mArg);
            }
            break;

            case TYPE_FLOAT32:
            {
                i = (f64)(*(f32*)mArg);
            }
            break;
            case TYPE_FLOAT64:
            {
                i = (f64)(*(f64*)mArg);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch((ascii::pcrune)mArg[1], (ascii::pcrune)mArg[2]);
                ch.m_type = (s32)mArg[0];
                parse(ch, i);
            }
            break;
            default: break; // Fall through
        };

        return i;
    }

    bool va_t::convertToBool() const
    {
        u32 i = 0;
        switch (mType)
        {
            case TYPE_BOOL:
            case TYPE_UINT32:
            case TYPE_INT32:
            {
                i = (u32)(*(u32*)mArg);
            }
            break;

            case TYPE_UINT8:
            case TYPE_INT8:
            {
                i = (u32)(*(u8*)mArg);
            }
            break;
            case TYPE_UINT16:
            case TYPE_INT16:
            {
                i = (u32)(*(u16*)mArg);
            }
            break;
            case TYPE_UINT64:
            case TYPE_INT64:
            {
                i = (u32)(*(u64*)mArg);
            }
            break;
            case TYPE_FLOAT32:
            {
                i = (u32)(*(f32*)mArg);
            }
            break;
            case TYPE_FLOAT64:
            {
                i = (u32)(*(f64*)mArg);
            }
            break;
            case TYPE_PCRUNES:
            {
                crunes_t ch((ascii::pcrune)mArg[1], (ascii::pcrune)mArg[2]);
                ch.m_type = (s32)mArg[0];
                bool b    = false;
                parse(ch, b);
                i = b ? 1 : 0;
            }
            break;

            default: break; // Fall through
        };

        return i != 0;
    }

    crunes_t va_t::convertToCRunes() const
    {
        switch (mType)
        {
            case TYPE_PCRUNES:
            {
                crunes_t ch((ascii::pcrune)mArg[1], (ascii::pcrune)mArg[2]);
                ch.m_type = (s32)mArg[0];
                return ch;
            }
            default: break; // Fall through
        };
        return crunes_t();
    }

    va_r_t va_r_t::sEmpty;

    va_r_t& va_r_t::operator=(s8 rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef[0]) = rhs != 0 ? xTRUE : False; break;
            // case TYPE_UINT:		*((u32*)mRef[0]) = rhs; break;
            // case TYPE_INT:		*((s32*)mRef[0]) = rhs; break;
            case TYPE_UINT32: *((u32*)mRef[0]) = rhs; break;
            case TYPE_INT32: *((s32*)mRef[0]) = rhs; break;
            case TYPE_AINT32: *((s32*)mRef[0]) = rhs; break;
            case TYPE_UINT8: *((u8*)mRef[0]) = rhs; break;
            case TYPE_INT8: *((s8*)mRef[0]) = rhs; break;
            case TYPE_UINT16: *((u16*)mRef[0]) = rhs; break;
            case TYPE_INT16: *((s16*)mRef[0]) = rhs; break;
            case TYPE_UINT64: *((u64*)mRef[0]) = rhs; break;
            case TYPE_INT64: *((s64*)mRef[0]) = rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef[0]) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef[0]) = (f64)rhs; break;
            default: break; // Fall through
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
            case TYPE_BOOL: *((bool*)mRef[0]) = rhs != 0 ? xTRUE : False; break;
            case TYPE_UINT32: *((u32*)mRef[0]) = rhs; break;
            case TYPE_INT32: *((s32*)mRef[0]) = rhs; break;
            case TYPE_AINT32: *((s32*)mRef[0]) = rhs; break;
            case TYPE_UINT8: *((u8*)mRef[0]) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef[0]) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef[0]) = rhs; break;
            case TYPE_INT16: *((s16*)mRef[0]) = rhs; break;
            case TYPE_UINT64: *((u64*)mRef[0]) = rhs; break;
            case TYPE_INT64: *((s64*)mRef[0]) = rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef[0]) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef[0]) = (f64)rhs; break;
            default: break; // Fall through
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
            case TYPE_BOOL: *((bool*)mRef[0]) = rhs != 0 ? xTRUE : False; break;
            case TYPE_UINT32: *((u32*)mRef[0]) = rhs; break;
            case TYPE_INT32: *((s32*)mRef[0]) = rhs; break;
            case TYPE_AINT32: *((s32*)mRef[0]) = rhs; break;
            case TYPE_UINT8: *((u8*)mRef[0]) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef[0]) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef[0]) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef[0]) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef[0]) = rhs; break;
            case TYPE_INT64: *((s64*)mRef[0]) = rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef[0]) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef[0]) = (f64)rhs; break;
            default: break; // Fall through
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
            case TYPE_BOOL: *((bool*)mRef[0]) = rhs != 0 ? xTRUE : False; break;
            case TYPE_UINT32: *((u32*)mRef[0]) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef[0]) = (s32)rhs; break;
            case TYPE_AINT32: *((s32*)mRef[0]) = (s32)rhs; break;
            case TYPE_UINT8: *((u8*)mRef[0]) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef[0]) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef[0]) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef[0]) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef[0]) = rhs; break;
            case TYPE_INT64: *((s64*)mRef[0]) = rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef[0]) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef[0]) = (f64)rhs; break;
            default: break; // Fall through
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
            case TYPE_BOOL: *((bool*)mRef[0]) = rhs != 0 ? xTRUE : False; break;
            case TYPE_UINT32: *((u32*)mRef[0]) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef[0]) = (s32)rhs; break;
            case TYPE_AINT32: *((s32*)mRef[0]) = (s32)rhs; break;
            case TYPE_UINT8: *((u8*)mRef[0]) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef[0]) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef[0]) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef[0]) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef[0]) = (u64)rhs; break;
            case TYPE_INT64: *((s64*)mRef[0]) = (s64)rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef[0]) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef[0]) = (f64)rhs; break;
            default: break; // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(f64 rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef[0]) = rhs != 0 ? xTRUE : False; break;
            case TYPE_UINT32: *((u32*)mRef[0]) = (u32)rhs; break;
            case TYPE_INT32: *((s32*)mRef[0]) = (s32)rhs; break;
            case TYPE_AINT32: *((s32*)mRef[0]) = (s32)rhs; break;
            case TYPE_UINT8: *((u8*)mRef[0]) = (u8)rhs; break;
            case TYPE_INT8: *((s8*)mRef[0]) = (s8)rhs; break;
            case TYPE_UINT16: *((u16*)mRef[0]) = (u16)rhs; break;
            case TYPE_INT16: *((s16*)mRef[0]) = (s16)rhs; break;
            case TYPE_UINT64: *((u64*)mRef[0]) = (u64)rhs; break;
            case TYPE_INT64: *((s64*)mRef[0]) = (s64)rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef[0]) = (f32)rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef[0]) = (f64)rhs; break;
            default: break; // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(bool rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef[0]) = bool(rhs); break;
            case TYPE_UINT32: *((u32*)mRef[0]) = rhs ? 1 : 0; break;
            case TYPE_INT32: *((s32*)mRef[0]) = rhs ? 1 : 0; break;
            case TYPE_AINT32: *((s32*)mRef[0]) = rhs ? 1 : 0; break;
            case TYPE_UINT8: *((u8*)mRef[0]) = rhs ? 1 : 0; break;
            case TYPE_INT8: *((s8*)mRef[0]) = rhs ? 1 : 0; break;
            case TYPE_UINT16: *((u16*)mRef[0]) = rhs ? 1 : 0; break;
            case TYPE_INT16: *((s16*)mRef[0]) = rhs ? 1 : 0; break;
            case TYPE_UINT64: *((u64*)mRef[0]) = rhs ? 1 : 0; break;
            case TYPE_INT64: *((s64*)mRef[0]) = rhs ? 1 : 0; break;
            case TYPE_FLOAT32: *((f32*)mRef[0]) = rhs ? 1.0f : 0.0f; break;
            case TYPE_FLOAT64: *((f64*)mRef[0]) = rhs ? 1.0 : 0.0; break;
            default: break; // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(const crunes_t& rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: parse(rhs, *((bool*)mRef[0])); break;
            case TYPE_UINT32: parse(rhs, *((u32*)mRef[0])); break;
            case TYPE_INT32: parse(rhs, *((s32*)mRef[0])); break;
            case TYPE_AINT32: parse(rhs, *((s32*)mRef[0])); break;
            case TYPE_UINT8: parse(rhs, *((u8*)mRef[0])); break;
            case TYPE_INT8: parse(rhs, *((s8*)mRef[0])); break;
            case TYPE_UINT16: parse(rhs, *((u16*)mRef[0])); break;
            case TYPE_INT16: parse(rhs, *((s16*)mRef[0])); break;
            case TYPE_UINT64: parse(rhs, *((u64*)mRef[0])); break;
            case TYPE_INT64: parse(rhs, *((s64*)mRef[0])); break;
            case TYPE_FLOAT32: parse(rhs, *((f32*)mRef[0])); break;
            case TYPE_FLOAT64: parse(rhs, *((f64*)mRef[0])); break;
            case TYPE_PRUNES: break;
            default: break; // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator=(va_t const& rhs)
    {
        switch (mType)
        {
            case TYPE_BOOL: *((bool*)mRef[0]) = rhs; break;
            case TYPE_UINT32: *((u32*)mRef[0]) = rhs; break;
            case TYPE_INT32: *((s32*)mRef[0]) = rhs; break;
            case TYPE_AINT32: *((s32*)mRef[0]) = rhs; break;
            case TYPE_UINT8: *((u8*)mRef[0]) = rhs; break;
            case TYPE_INT8: *((s8*)mRef[0]) = rhs; break;
            case TYPE_UINT16: *((u16*)mRef[0]) = rhs; break;
            case TYPE_INT16: *((s16*)mRef[0]) = rhs; break;
            case TYPE_UINT64: *((u64*)mRef[0]) = rhs; break;
            case TYPE_INT64: *((s64*)mRef[0]) = rhs; break;
            case TYPE_FLOAT32: *((f32*)mRef[0]) = rhs; break;
            case TYPE_FLOAT64: *((f64*)mRef[0]) = rhs; break;
            case TYPE_PRUNES: rhs.convertToRunes(*(runes_t*)mRef[0]); break;
            default: break; // Fall through
        };

        return *this;
    }

    va_r_t& va_r_t::operator+=(s32 rhs)
    {
        switch (mType)
        {
        case TYPE_BOOL: *((bool*)mRef[0]) = rhs != 0 ? xTRUE : False; break;
        case TYPE_UINT32: *((u32*)mRef[0]) = rhs; break;
        case TYPE_INT32: *((s32*)mRef[0]) = rhs; break;
        case TYPE_AINT32: if (mSize < mCap) *((s32*)mRef[0] + mSize++) = rhs; break;
        case TYPE_UINT8: *((u8*)mRef[0]) = (u8)rhs; break;
        case TYPE_INT8: *((s8*)mRef[0]) = (s8)rhs; break;
        case TYPE_UINT16: *((u16*)mRef[0]) = (u16)rhs; break;
        case TYPE_INT16: *((s16*)mRef[0]) = (s16)rhs; break;
        case TYPE_UINT64: *((u64*)mRef[0]) = rhs; break;
        case TYPE_INT64: *((s64*)mRef[0]) = rhs; break;
        case TYPE_FLOAT32: *((f32*)mRef[0]) = (f32)rhs; break;
        case TYPE_FLOAT64: *((f64*)mRef[0]) = (f64)rhs; break;
        default: break; // Fall through
        };

        return *this;
    }

}; // namespace ncore
