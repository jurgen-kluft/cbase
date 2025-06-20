#include "ccore/c_target.h"
#include "cbase/c_buffer.h"

namespace ncore
{
    namespace nuadrw
    {
        /// ---------------------------------------------------------------------------------------
        /// Unaligned Data Reading/Writing
        /// ---------------------------------------------------------------------------------------
        static inline u8  read_u8(u8 const* ptr) { return (u8)*ptr; }
        static inline s8  read_s8(u8 const* ptr) { return (s8)*ptr; }
        static inline u16 read_u16(u8 const* ptr)
        {
            const u16 b0 = *ptr++;
            const u16 b1 = *ptr++;
            return (u16)((b1 << 8) | b0);
        }
        static inline s16 read_s16(u8 const* ptr)
        {
            u16 const c = read_u16(ptr);
            return *((s16*)&c);
        }
        static inline u32 read_u32(u8 const* ptr)
        {
            const u32 b0 = *ptr++;
            const u32 b1 = *ptr++;
            const u32 b2 = *ptr++;
            const u32 b3 = *ptr++;
            return (u32)((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0));
        }
        static inline s32 read_s32(u8 const* ptr)
        {
            u32 const c = read_u32(ptr);
            return *((s32*)&c);
        }
        static inline f32 read_f32(u8 const* ptr)
        {
            u32 u = read_u32(ptr);
            return *((f32*)&u);
        }
        static inline u64 read_u64(u8 const* ptr)
        {
            const u32 b0 = *ptr++;
            const u32 b1 = *ptr++;
            const u32 b2 = *ptr++;
            const u32 b3 = *ptr++;
            const u32 b4 = *ptr++;
            const u32 b5 = *ptr++;
            const u32 b6 = *ptr++;
            const u32 b7 = *ptr++;
            return (u64)(((u64)b7 << 56) | ((u64)b6 << 48) | ((u64)b5 << 40) | ((u64)b4 << 32) | ((u64)b3 << 24) | ((u64)b2 << 16) | ((u64)b1 << 8) | (u64)b0);
        }
        static inline s64 read_s64(u8 const* ptr)
        {
            u64 const c = read_u64(ptr);
            return *((s64*)&c);
        }
        static inline f64 read_f64(u8 const* ptr)
        {
            u64 const c = read_u64(ptr);
            return *((f64*)&c);
        }

        static inline void write_u8(u8* ptr, u8 b) { *ptr = b; }
        static inline void write_s8(u8* ptr, s8 b) { *ptr = (u8)b; }
        static inline void write_u16(u8* ptr, u16 b)
        {
            ptr[0] = (u8)(b);
            b      = b >> 8;
            ptr[1] = (u8)(b);
        }
        static inline void write_s16(u8* ptr, s16 b)
        {
            u16 const c = *((u16*)&b);
            write_u16(ptr, c);
        }
        static inline void write_u32(u8* ptr, u32 b)
        {
            ptr[0] = (u8)(b);
            b      = b >> 8;
            ptr[1] = (u8)(b);
            b      = b >> 8;
            ptr[2] = (u8)(b);
            b      = b >> 8;
            ptr[3] = (u8)(b);
        }
        static inline void write_s32(u8* ptr, s32 b)
        {
            u32 const c = *((u32*)&b);
            write_u32(ptr, c);
        }
        static inline void write_f32(u8* ptr, f32 f)
        {
            u32 const c = *((u32*)&f);
            write_u32(ptr, c);
        }
        static inline void write_u64(u8* ptr, u64 b)
        {
            ptr[0] = (u8)(b);
            b      = b >> 8;
            ptr[1] = (u8)(b);
            b      = b >> 8;
            ptr[2] = (u8)(b);
            b      = b >> 8;
            ptr[3] = (u8)(b);
            b      = b >> 8;
            ptr[4] = (u8)(b);
            b      = b >> 8;
            ptr[5] = (u8)(b);
            b      = b >> 8;
            ptr[6] = (u8)(b);
            b      = b >> 8;
            ptr[7] = (u8)(b);
        }
        static inline void write_s64(u8* ptr, s64 b)
        {
            u64 const c = *((u64*)&b);
            write_u64(ptr, c);
        }
        static inline void write_f64(u8* ptr, f64 f)
        {
            u64 const c = *((u64*)&f);
            write_u64(ptr, c);
        }
    }  // namespace nuadrw

    /// ---------------------------------------------------------------------------------------
    /// Binary Reader
    /// ---------------------------------------------------------------------------------------

    int_t     binary_reader_t::size() const { return m_buffer.m_end - m_buffer.m_begin; }
    int_t     binary_reader_t::length() const { return m_buffer.m_end - m_buffer.m_begin; }
    int_t     binary_reader_t::remain() const { return m_buffer.m_end - m_cursor; }
    cbuffer_t binary_reader_t::get_current_buffer() const { return cbuffer_t(m_buffer.m_begin, m_cursor); }
    bool      binary_reader_t::can_read(int_t number_of_bytes) const { return (m_cursor + number_of_bytes) <= m_buffer.m_end; }
    bool      binary_reader_t::at_end() const { return m_cursor == m_buffer.m_end; }
    bool      binary_reader_t::seek(int_t pos)
    {
        u8 const* cursor = m_buffer.m_begin + pos;
        if (cursor >= m_buffer.m_begin && cursor <= m_buffer.m_end)
        {
            m_cursor = cursor;
            return true;
        }
        return false;
    }

    int_t binary_reader_t::pos() const { return m_cursor - m_buffer.m_begin; }
    void  binary_reader_t::reset() { m_cursor = 0; }

    int_t binary_reader_t::skip(int_t c)
    {
        int_t const offset = m_cursor - m_buffer.m_begin;
        if (can_read(c))
        {
            m_cursor += c;
            return offset;
        }
        return 0;
    }

    // Peek

    u16 binary_reader_t::peek_u16() const
    {
        u16 b = 0;
        if (can_read(sizeof(b)))
        {
            u8 const* ptr = m_cursor;
            b             = nuadrw::read_u16(ptr);
        }
        return b;
    }

    // Direct Read

    bool binary_reader_t::read_bool()
    {
        bool b = false;
        if (can_read(sizeof(u8)))
        {
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
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
            u8 const* ptr = m_cursor;
            m_cursor += sizeof(b);
            b = nuadrw::read_f64(ptr);
        }
        return b;
    }

    // End Direct Read

    void binary_reader_t::read_data(buffer_t& buf)
    {
        int_t const size = buf.size();
        if (can_read(size))
        {
            cbuffer_t to_read(m_cursor, m_cursor + size);
            buf.writer().write_data(to_read);
            m_cursor += size;
        }
    }

    void binary_reader_t::view_data(u32 size, cbuffer_t& buf)
    {
        buf.m_begin = m_cursor;
        buf.m_end   = m_cursor + size;
        m_cursor += size;
    }

    void binary_reader_t::read_buffer(buffer_t& buf)
    {
        int_t const size = read_u32();
        if (can_read(size))
        {
            cbuffer_t to_read(m_cursor, m_cursor + size);
            buf.writer().write_data(to_read);
            m_cursor += size;
        }
    }

    void binary_reader_t::view_buffer(cbuffer_t& buf)
    {
        int_t const size = read_u32();
        buf.m_begin      = m_cursor;
        buf.m_end        = m_cursor + size;
        m_cursor += size;
    }

    void binary_reader_t::view_string(const char*& out_str, const char*& out_str_end, s8& out_str_type)
    {
        if (size() == 0)
            return;
        u32 const datalen = read_u32();
        out_str_type      = read_s8();
        out_str           = (char const*)(m_cursor);
        out_str_end       = (char const*)(m_cursor + datalen);
        m_cursor += datalen;
    }

    /// ---------------------------------------------------------------------------------------
    /// Binary Writer
    /// ---------------------------------------------------------------------------------------
    int_t binary_writer_t::size() const { return m_cursor - m_buffer.m_begin; }
    int_t binary_writer_t::length() const { return m_buffer.m_end - m_buffer.m_begin; }
    int_t binary_writer_t::remain() const { return m_buffer.m_end - m_cursor; }

    buffer_t binary_writer_t::get_full_buffer() const { return buffer_t(m_buffer.m_begin, m_buffer.m_end); }

    buffer_t binary_writer_t::get_current_buffer() const { return buffer_t(m_buffer.m_begin, m_cursor); }

    bool binary_writer_t::can_write(int_t num_bytes) const { return ((m_cursor + num_bytes) <= m_buffer.m_end); }
    bool binary_writer_t::at_end() const { return m_cursor == m_buffer.m_end; }

    bool binary_writer_t::seek(int_t pos)
    {
        u8* cursor = m_cursor + pos;
        if (cursor <= m_buffer.m_end)
        {
            m_cursor = cursor;
            return true;
        }
        return false;
    }

    int_t binary_writer_t::pos() const { return m_cursor - m_buffer.m_begin; }
    void  binary_writer_t::reset() { m_cursor = m_buffer.m_begin; }

    int_t binary_writer_t::skip(int_t c)
    {
        if (can_write(c))
        {
            int_t const offset = m_cursor - m_buffer.m_begin;
            m_cursor += c;
            return offset;
        }
        return -1;
    }

    binary_writer_t binary_writer_t::reserve(int_t len)
    {
        binary_writer_t writer;
        if (can_write(len))
        {
            writer.m_buffer.m_begin  = m_cursor;
            writer.m_cursor = m_cursor;
            writer.m_buffer.m_end    = m_cursor + len;
            m_cursor += len;
        }
        return writer;
    }

    binary_writer_t binary_writer_t::range(int_t from, int_t to) const
    {
        ASSERT(from < to);
        ASSERT((m_cursor + from) < m_buffer.m_end);
        ASSERT((m_cursor + to) < m_buffer.m_end);

        binary_writer_t writer;
        writer.m_buffer.m_begin  = m_cursor + from;
        writer.m_cursor = m_cursor + from;
        writer.m_buffer.m_end    = m_cursor + to;
        return writer;
    }

    void binary_writer_t::write(bool b) { write(b ? (u8)1 : (u8)0); }

    void binary_writer_t::write(u8 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_u8(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write(s8 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_s8(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write(u16 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_u16(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write(s16 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_s16(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write(u32 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_u32(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write(s32 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_s32(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write(u64 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_u64(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write(s64 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_s64(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write(f32 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_f32(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write(f64 b)
    {
        if (can_write(sizeof(b)))
        {
            nuadrw::write_f64(m_cursor, b);
            m_cursor += sizeof(b);
        }
    }

    void binary_writer_t::write_data(buffer_t const& buf)
    {
        if (can_write(buf.size()))
        {
            int_t const n   = buf.size();
            u8*         dst = m_cursor;
            u8 const*   src = buf.m_begin;
            for (u32 i = 0; i < n; i++)
                *dst++ = *src++;
            m_cursor += n;
        }
    }

    void binary_writer_t::write_data(cbuffer_t const& buf)
    {
        if (can_write(buf.size()))
        {
            int_t const n   = buf.size();
            u8*         dst = m_cursor;
            u8 const*   src = buf.m_begin;
            for (u32 i = 0; i < n; i++)
                *dst++ = *src++;
            m_cursor += n;
        }
    }

    void binary_writer_t::write_buffer(buffer_t const& buf)
    {
        if (can_write(4 + (u32)buf.size()))
        {
            int_t const n = buf.size();
            write(n);
            u8*       dst = m_cursor;
            u8 const* src = buf.m_begin;
            for (u32 i = 0; i < n; i++)
                *dst++ = *src++;
            m_cursor += n;
        }
    }

    void binary_writer_t::write_buffer(cbuffer_t const& buf)
    {
        if (can_write(4 + buf.size()))
        {
            int_t const n = buf.size();
            write((u32)n);
            u8*       dst = m_cursor;
            u8 const* src = buf.m_begin;
            for (u32 i = 0; i < n; i++)
                *dst++ = *src++;
            m_cursor += n;
        }
    }

    void binary_writer_t::write_string(const char* str, const char* str_end, s8 str_type)
    {
        char const* srcstr   = str;
        char const* srcend   = str_end;
        u32 const   datasize = (u32)(srcend - srcstr);
        if (can_write(4 + 1 + datasize))
        {
            write((u32)datasize);
            write(str_type);

            char* dststr = (char*)m_cursor;
            while (srcstr < srcend)
            {
                *dststr++ = *srcstr++;
            }
            m_cursor += datasize;
        }
    }

}  // namespace ncore
