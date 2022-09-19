#include "cbase/c_target.h"
#include "cbase/c_base.h"
#include "cbase/c_buffer.h"
#include "cbase/c_runes.h"

namespace ncore
{
    //==============================================================================
    // Integer cast and overflow detection
    template <class FROM, class TO> TO ccast(FROM f) { return f; }

    template <> u32 ccast(u64 f)
    {
        ASSERT(f < 0x100000000UL);
        return (u32)f;
    }
    template <> s32 ccast(s64 f)
    {
        ASSERT(f >= cS32Min && f <= cS32Max);
        return (s32)f;
    }

    namespace nuadrw
    {
        /// ---------------------------------------------------------------------------------------
        /// Unaligned Data Reading/Writing
        /// ---------------------------------------------------------------------------------------
        static inline u8  read_u8(u8 const* ptr) { return (u8)*ptr; }
        static inline s8  read_s8(u8 const* ptr) { return (s8)*ptr; }
        static inline u16 read_u16(u8 const* ptr)
        {
            u16 b0 = *ptr++;
            u16 b1 = *ptr++;
            return (u16)((b1 << 8) | b0);
        }
        static inline s16 read_s16(u8 const* ptr)
        {
            u16 b0 = *ptr++;
            u16 b1 = *ptr++;
            return (s16)((b1 << 8) | b0);
        }
        static inline u32 read_u32(u8 const* ptr)
        {
            u32 b0 = *ptr++;
            u32 b1 = *ptr++;
            u32 b2 = *ptr++;
            u32 b3 = *ptr++;
            return (u32)((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0));
        }
        static inline s32 read_s32(u8 const* ptr)
        {
            u32 b0 = *ptr++;
            u32 b1 = *ptr++;
            u32 b2 = *ptr++;
            u32 b3 = *ptr++;
            return (s32)((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0));
        }
        static inline f32 read_f32(u8 const* ptr)
        {
            u32 b0 = *ptr++;
            u32 b1 = *ptr++;
            u32 b2 = *ptr++;
            u32 b3 = *ptr++;
            u32 u  = ((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0));
            return *((f32*)&u);
        }
        static inline u64 read_u64(u8 const* ptr)
        {
            u64 l0 = read_u32(ptr);
            u64 l1 = read_u32(ptr + 4);
            return (u64)((l1 << 32) | l0);
        }
        static inline s64 read_s64(u8 const* ptr)
        {
            u64 l0 = read_u32(ptr);
            u64 l1 = read_u32(ptr + 4);
            return (s64)((l1 << 32) | l0);
        }
        static inline f64 read_f64(u8 const* ptr)
        {
            u64 l0 = read_u32(ptr);
            u64 l1 = read_u32(ptr + 4);
            u64 ll = (u64)((l1 << 32) | l0);
            return *((f64*)&ll);
        }

        static inline void write_u8(u8* ptr, u8 b) { *ptr = b; }
        static inline void write_s8(u8* ptr, s8 b) { *ptr = b; }
        static inline void write_u16(u8* ptr, u16 b)
        {
            ptr[0] = (u8)(b);
            ptr[1] = (u8)(b >> 8);
        }
        static inline void write_s16(u8* ptr, s16 b)
        {
            ptr[0] = (u8)(b);
            ptr[1] = (u8)(b >> 8);
        }
        static inline void write_u32(u8* ptr, u32 b)
        {
            ptr[0] = (u8)(b);
            ptr[1] = (u8)(b >> 8);
            ptr[2] = (u8)(b >> 16);
            ptr[3] = (u8)(b >> 24);
        }
        static inline void write_s32(u8* ptr, s32 b)
        {
            u32 const c = *((u32*)&b);
            ptr[0]      = (u8)(c);
            ptr[1]      = (u8)(c >> 8);
            ptr[2]      = (u8)(c >> 16);
            ptr[3]      = (u8)(c >> 24);
        }
        static inline void write_f32(u8* ptr, f32 f)
        {
            u32 const c = *((u32*)&f);
            ptr[0]      = (u8)(c);
            ptr[1]      = (u8)(c >> 8);
            ptr[2]      = (u8)(c >> 16);
            ptr[3]      = (u8)(c >> 24);
        }
        static inline void write_u64(u8* ptr, u64 b)
        {
            write_u16(ptr, (u32)((b >> 0) & 0xFFFFFFFF));
            write_u16(ptr + 4, (u32)((b >> 32) & 0xFFFFFFFF));
        }
        static inline void write_s64(u8* ptr, s64 b)
        {
            u64 const c = *((u64*)&b);
            write_u32(ptr, (u32)((c >> 0) & 0xFFFFFFFF));
            write_u32(ptr + 4, (u32)((c >> 32) & 0xFFFFFFFF));
        }
        static inline void write_f64(u8* ptr, f64 f)
        {
            u64 const c = *((u64*)&f);
            write_u32(ptr, (u32)((c >> 0) & 0xFFFFFFFF));
            write_u32(ptr + 4, (u32)((c >> 32) & 0xFFFFFFFF));
        }
    } // namespace nuadrw

    cbuffer_t::cbuffer_t(crunes_t const& str)
    {
        m_const          = (u8 const*)str.m_ascii.m_str;
        u8 const* end = (u8 const*)str.m_ascii.m_end;
        m_len            = (end - m_const);
    }

    cbuffer_t cbuffer_t::from_ascii_string(const char* str)
    {
        s32       len = ascii::strlen((ascii::pcrune)str, nullptr);
        cbuffer_t buffer((uint_t)len, (u8 const*)str);
        return buffer;
    }

    /// ---------------------------------------------------------------------------------------
    /// Binary Reader
    /// ---------------------------------------------------------------------------------------

    uint_t   binary_reader_t::size() const { return m_cursor; }
    uint_t   binary_reader_t::length() const { return m_len; }
    uint_t   binary_reader_t::remain() const { return m_len - m_cursor; }
    cbuffer_t binary_reader_t::get_current_buffer() const { return cbuffer_t(size(), m_buffer + m_cursor); }
    bool      binary_reader_t::can_read(uint_t number_of_bytes) const { return (m_cursor + number_of_bytes) <= m_len; }
    bool      binary_reader_t::at_end() const { return m_cursor == m_len; }
    bool      binary_reader_t::seek(uint_t cursor)
    {
        if (cursor <= m_len)
        {
            m_cursor = cursor;
            return true;
        }
        return false;
    }

    uint_t binary_reader_t::pos() const { return m_cursor; }
    void    binary_reader_t::reset() { m_cursor = 0; }

    uint_t binary_reader_t::skip(uint_t c)
    {
        uint_t const offset = m_cursor;
        if (can_read(m_len))
        {
            m_cursor += c;
            return offset;
        }
        return 0;
    }

    uint_t binary_reader_t::read(bool& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(u8)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += 1;
            b = nuadrw::read_u8(ptr) != 0;
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(u8& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_u8(ptr);
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(s8& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_s8(ptr);
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(u16& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_u16(ptr);
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(s16& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_s16(ptr);
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(u32& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_u32(ptr);
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(s32& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_s32(ptr);
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(u64& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_u64(ptr);
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(s64& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = (u8 const*)m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_s64(ptr);
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(f32& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = (u8 const*)m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_f32(ptr);
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::read(f64& b)
    {
        uint_t const offset = m_cursor;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = (u8 const*)m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_f64(ptr);
            return offset;
        }
        return -1;
    }

    // Peek

    u16 binary_reader_t::peek_u16() const
    {
        u16 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            b                = nuadrw::read_u16(ptr);
        }
        return b;
    }

    // Direct Read

    bool binary_reader_t::read_bool()
    {
        bool b = false;
        if (can_read(m_len))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += 1;
            b = nuadrw::read_u8(ptr) != 0;
        }
        return b;
    }

    u8 binary_reader_t::read_u8()
    {
        u8 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_u8(ptr);
        }
        return b;
    }

    s8 binary_reader_t::read_s8()
    {
        s8 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_s8(ptr);
        }
        return b;
    }

    u16 binary_reader_t::read_u16()
    {
        u16 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_u16(ptr);
        }
        return b;
    }

    s16 binary_reader_t::read_s16()
    {
        s16 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_s16(ptr);
        }
        return b;
    }

    u32 binary_reader_t::read_u32()
    {
        u32 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_u32(ptr);
        }
        return b;
    }

    s32 binary_reader_t::read_s32()
    {
        s32 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_s32(ptr);
        }
        return b;
    }

    u64 binary_reader_t::read_u64()
    {
        u64 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_u64(ptr);
        }
        return b;
    }

    s64 binary_reader_t::read_s64()
    {
        s64 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = (u8 const*)m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_s64(ptr);
        }
        return b;
    }

    f32 binary_reader_t::read_f32()
    {
        f32 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = (u8 const*)m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_f32(ptr);
        }
        return b;
    }

    f64 binary_reader_t::read_f64()
    {
        f64 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = (u8 const*)m_buffer + m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_f64(ptr);
        }
        return b;
    }

    // End Direct Read

    uint_t binary_reader_t::read_data(buffer_t& buf)
    {
        uint_t const size = buf.size();
        if (can_read(size))
        {
            uint_t const offset = m_cursor;
            cbuffer_t to_read(size, m_buffer + m_cursor);
            buf.writer().write_data(to_read);
            m_cursor += size;
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::view_data(uint_t size, cbuffer_t& buf)
    {
        uint_t const offset = m_cursor;
        buf.m_const      = m_buffer + m_cursor;
        buf.m_len        = size;
        m_cursor += size;
        return offset;
    }

    uint_t binary_reader_t::read_buffer(buffer_t& buf)
    {
        uint_t const size = read_u32();
        if (can_read(size))
        {
            uint_t const offset = m_cursor;
            cbuffer_t to_read(size, m_buffer + m_cursor);
            buf.writer().write_data(to_read);
            m_cursor += size;
            return offset;
        }
        return -1;
    }

    uint_t binary_reader_t::view_buffer(cbuffer_t& buf)
    {
        uint_t const offset = m_cursor;
        uint_t const size   = read_u32();
        buf.m_const      = m_buffer + m_cursor;
        buf.m_len        = size;
        m_cursor += size;
        return offset;
    }

    uint_t binary_reader_t::view_crunes(crunes_t& out_str)
    {
        if (size() == 0)
            return -1;
        uint_t const offset          = m_cursor;
        out_str.m_type                = read_s32();
        s64 const strlen              = read_s64();
        out_str.m_ascii.m_bos = (char const*)(m_buffer + m_cursor);
        out_str.m_ascii.m_str = (out_str.m_ascii.m_bos);
        out_str.m_ascii.m_end = (out_str.m_ascii.m_bos + strlen);
        out_str.m_ascii.m_eos = (out_str.m_ascii.m_bos + strlen);
        m_cursor += strlen;
        return offset;
    }

    /// ---------------------------------------------------------------------------------------
    /// Binary Writer
    /// ---------------------------------------------------------------------------------------
    uint_t binary_writer_t::size() const { return m_cursor; }
    uint_t binary_writer_t::length() const { return m_len; }
    uint_t binary_writer_t::remain() const { return m_len - m_cursor; }

    buffer_t binary_writer_t::get_full_buffer() const
    {
        buffer_t buffer;
        buffer.m_mutable = m_buffer;
        buffer.m_len     = m_len;
        return buffer;
    }

    buffer_t binary_writer_t::get_current_buffer() const
    {
        buffer_t buffer;
        buffer.m_mutable = m_buffer + m_cursor;
        buffer.m_len     = size();
        return buffer;
    }

    bool binary_writer_t::can_write(uint_t num_bytes) const { return ((m_cursor + num_bytes) <= m_len); }
    bool binary_writer_t::at_end() const { return m_cursor == m_len; }

    bool binary_writer_t::seek(uint_t cursor)
    {
        if (cursor <= m_len)
        {
            m_cursor = cursor;
            return true;
        }
        return false;
    }

    uint_t binary_writer_t::pos() const { return m_cursor; }

    void binary_writer_t::reset() { m_cursor = 0; }

    uint_t binary_writer_t::skip(uint_t c)
    {
        uint_t const offset = m_cursor;
        if (can_write(c))
        {
            m_cursor += c;
            return offset;
        }
        return -1;
    }

    binary_writer_t binary_writer_t::reserve(uint_t len)
    {
        binary_writer_t writer;
        if (can_write(len))
        {
            writer.m_len    = len;
            writer.m_cursor = m_cursor;
            writer.m_buffer = m_buffer;
            m_cursor += len;
        }
        return writer;
    }

    binary_writer_t binary_writer_t::range(uint_t from, uint_t to) const
    {
        ASSERT(from < to);
        ASSERT(from < m_len);
        ASSERT(to < m_len);

        binary_writer_t writer;
        uint_t const       len = to - from;
        writer.m_len        = len;
        writer.m_cursor     = 0;
        writer.m_buffer     = m_buffer + from;
        return writer;
    }

    uint_t binary_writer_t::write(bool b) { return write(b ? (u8)1 : (u8)0); }

    uint_t binary_writer_t::write(u8 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_u8(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write(s8 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_s8(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write(u16 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_u16(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write(s16 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_s16(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write(u32 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_u32(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write(s32 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_s32(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write(u64 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_u64(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write(s64 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_s64(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write(f32 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_f32(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write(f64 b)
    {
        uint_t const offset = m_cursor;
        if (can_write(sizeof(b)))
        {
            nuadrw::write_f64(m_buffer + m_cursor, b);
            m_cursor += sizeof(b);
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write_data(buffer_t const& buf)
    {
        uint_t const offset = m_cursor;
        if (can_write(buf.size()))
        {
            uint_t const    n   = buf.size();
            u8*       dst = m_buffer + m_cursor;
            u8 const* src = buf.m_mutable;
            for (uint_t i = 0; i < n; i++)
                *dst++ = *src++;
            m_cursor += n;
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write_data(cbuffer_t const& buf)
    {
        uint_t const offset = m_cursor;
        if (can_write(buf.size()))
        {
            uint_t const    n   = buf.size();
            u8*       dst = (u8*)m_buffer + m_cursor;
            u8 const* src = buf.m_const;
            for (uint_t i = 0; i < n; i++)
                *dst++ = *src++;
            m_cursor += n;
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write_buffer(buffer_t const& buf)
    {
        uint_t const offset = m_cursor;
        if (can_write((u32)buf.size()))
        {
            uint_t const n = buf.size();
            write(n);
            u8*       dst = (u8*)m_buffer + m_cursor;
            u8 const* src = buf.m_mutable;
            for (uint_t i = 0; i < n; i++)
                *dst++ = *src++;
            m_cursor += n;
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write_buffer(cbuffer_t const& buf)
    {
        uint_t const offset = m_cursor;
        if (can_write(buf.size()))
        {
            uint_t const n = buf.size();
            write(n);
            u8*       dst = (u8*)m_buffer + m_cursor;
            u8 const* src = buf.m_const;
            for (uint_t i = 0; i < n; i++)
                *dst++ = *src++;
            m_cursor += n;
            return offset;
        }
        return -1;
    }

    uint_t binary_writer_t::write_string(crunes_t const& str)
    {
        uint_t const   offset = m_cursor;
        char const* srcstr = str.m_ascii.m_str;
        char const* srcend = str.m_ascii.m_end;
        uint_t const   size   = (srcend - srcstr);
        if (can_write(4 + 8 + size))
        {
            write(str.m_type);
            write(size);

            char* dststr = (char*)m_buffer + m_cursor;
            while (srcstr < srcend)
            {
                *dststr++ = *srcstr++;
            }
            m_cursor += size;
            return offset;
        }
        return -1;
    }

} // namespace ncore