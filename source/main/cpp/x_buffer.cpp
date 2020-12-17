#include "xbase/x_target.h"
#include "xbase/x_base.h"
#include "xbase/x_buffer.h"
#include "xbase/x_runes.h"

namespace xcore
{
    //==============================================================================
    // Integer cast and overflow detection
    template <class FROM, class TO> TO xcast(FROM f) { return f; }

    template <> u32 xcast(u64 f)
    {
        ASSERT(f < 0x100000000UL);
        return (u32)f;
    }
    template <> s32 xcast(s64 f)
    {
        ASSERT(f >= xS32Min && f <= xS32Max);
        return (s32)f;
    }

    namespace xuadrw
    {
        /// ---------------------------------------------------------------------------------------
        /// Unaligned Data Reading/Writing
        /// ---------------------------------------------------------------------------------------
        static inline u8  read_u8(xbyte const* ptr) { return (u8)*ptr; }
        static inline s8  read_s8(xbyte const* ptr) { return (s8)*ptr; }
        static inline u16 read_u16(xbyte const* ptr)
        {
            u16 b0 = *ptr++;
            u16 b1 = *ptr++;
            return (u16)((b1 << 8) | b0);
        }
        static inline s16 read_s16(xbyte const* ptr)
        {
            u16 b0 = *ptr++;
            u16 b1 = *ptr++;
            return (s16)((b1 << 8) | b0);
        }
        static inline u32 read_u32(xbyte const* ptr)
        {
            u32 b0 = *ptr++;
            u32 b1 = *ptr++;
            u32 b2 = *ptr++;
            u32 b3 = *ptr++;
            return (u32)((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0));
        }
        static inline s32 read_s32(xbyte const* ptr)
        {
            u32 b0 = *ptr++;
            u32 b1 = *ptr++;
            u32 b2 = *ptr++;
            u32 b3 = *ptr++;
            return (s32)((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0));
        }
        static inline f32 read_f32(xbyte const* ptr)
        {
            u32 b0 = *ptr++;
            u32 b1 = *ptr++;
            u32 b2 = *ptr++;
            u32 b3 = *ptr++;
            u32 u  = ((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0));
            return *((f32*)&u);
        }
        static inline u64 read_u64(xbyte const* ptr)
        {
            u64 l0 = read_u32(ptr);
            u64 l1 = read_u32(ptr + 4);
            return (u64)((l1 << 32) | l0);
        }
        static inline s64 read_s64(xbyte const* ptr)
        {
            u64 l0 = read_u32(ptr);
            u64 l1 = read_u32(ptr + 4);
            return (s64)((l1 << 32) | l0);
        }
        static inline f64 read_f64(xbyte const* ptr)
        {
            u64 l0 = read_u32(ptr);
            u64 l1 = read_u32(ptr + 4);
            u64 ll = (u64)((l1 << 32) | l0);
            return *((f64*)&ll);
        }

        static inline void write_u8(xbyte* ptr, u8 b) { *ptr = b; }
        static inline void write_s8(xbyte* ptr, s8 b) { *ptr = b; }
        static inline void write_u16(xbyte* ptr, u16 b)
        {
            for (s32 i = 0; i < sizeof(b); ++i)
            {
                ptr[i] = (u8)b;
                b      = b >> 8;
            }
        }
        static inline void write_s16(xbyte* ptr, s16 b)
        {
            for (s32 i = 0; i < sizeof(b); ++i)
            {
                ptr[i] = (u8)b;
                b      = b >> 8;
            }
        }
        static inline void write_u32(xbyte* ptr, u32 b)
        {
            for (s32 i = 0; i < sizeof(b); ++i)
            {
                ptr[i] = (u8)b;
                b      = b >> 8;
            }
        }
        static inline void write_s32(xbyte* ptr, s32 b)
        {
            for (s32 i = 0; i < sizeof(b); ++i)
            {
                ptr[i] = (u8)b;
                b      = b >> 8;
            }
        }
        static inline void write_f32(xbyte* ptr, f32 f)
        {
            u32 b = *((u32*)&f);
            for (s32 i = 0; i < sizeof(b); ++i)
            {
                ptr[i] = (u8)b;
                b      = b >> 8;
            }
        }
        static inline void write_u64(xbyte* ptr, u64 b)
        {
            for (s32 i = 0; i < sizeof(b); ++i)
            {
                ptr[i] = (u8)b;
                b      = b >> 8;
            }
        }
        static inline void write_s64(xbyte* ptr, s64 b)
        {
            for (s32 i = 0; i < sizeof(b); ++i)
            {
                ptr[i] = (u8)b;
                b      = b >> 8;
            }
        }
        static inline void write_f64(xbyte* ptr, f64 f)
        {
            u64 b = *((u64*)&f);
            for (s32 i = 0; i < sizeof(b); ++i)
            {
                ptr[i] = (u8)b;
                b      = b >> 8;
            }
        }
    } // namespace xuadrw

    xcbuffer::xcbuffer(crunes_t const& str)
    {
        m_const          = (xbyte const*)str.m_runes.m_ascii.m_str;
        xbyte const* end = (xbyte const*)str.m_runes.m_ascii.m_end;
        m_len            = xcast<size_t, u32>(end - m_const);
    }

    xcbuffer xcbuffer::from_ascii_string(const char* str)
    {
        s32      len = ascii::strlen((ascii::pcrune)str, nullptr);
        xcbuffer buffer(len, (xbyte const*)str);
        return buffer;
    }

    /// ---------------------------------------------------------------------------------------
    /// Binary Reader
    /// ---------------------------------------------------------------------------------------
    inline bool _can_read(u32 _buffer_size, u32 _cursor, u32 _num_bytes) { return (_cursor + _num_bytes) <= _buffer_size; }

    u32      xbinary_reader::size() const { return cursor_; }
    u32      xbinary_reader::len() const { return len_; }
    xcbuffer xbinary_reader::get_current_buffer() const { return xcbuffer(size(), buffer_ + cursor_); }
    bool     xbinary_reader::can_read(u32 number_of_bytes) const { return _can_read(len_, cursor_, number_of_bytes); }
    bool     xbinary_reader::at_end() const { return cursor_ == len_; }
    bool     xbinary_reader::seek(u32 cursor)
    {
        if (cursor <= len_)
        {
            cursor_ = cursor;
            return true;
        }
        return false;
    }

    u32  xbinary_reader::pos() const { return cursor_; }
    void xbinary_reader::reset() { cursor_ = 0; }

    s32 xbinary_reader::skip(s32 c)
    {
        s32 const offset = (s32)cursor_;
        if (_can_read(len_, cursor_, c))
        {
            cursor_ += c;
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(bool& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, 1))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += 1;
            b = xuadrw::read_u8(ptr) != 0;
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(u8& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_u8(ptr);
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(s8& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_s8(ptr);
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(u16& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_u16(ptr);
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(s16& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_s16(ptr);
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(u32& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_u32(ptr);
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(s32& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_s32(ptr);
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(u64& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_u64(ptr);
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(s64& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            u8 const* ptr = (u8 const*)buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_s64(ptr);
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(f32& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            u8 const* ptr = (u8 const*)buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_f32(ptr);
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::read(f64& b)
    {
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            u8 const* ptr = (u8 const*)buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_f64(ptr);
            return offset;
        }
        return -1;
    }

    // Direct Read

    bool xbinary_reader::read_bool()
    {
        u32 const offset = cursor_;
        bool      b      = false;
        if (_can_read(len_, cursor_, 1))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += 1;
            b = xuadrw::read_u8(ptr) != 0;
        }
        return b;
    }

    u8 xbinary_reader::read_u8()
    {
        u32 const offset = cursor_;
        u8        b      = 0;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_u8(ptr);
        }
        return b;
    }

    s8 xbinary_reader::read_s8()
    {
        s8        b      = 0;
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_s8(ptr);
        }
        return b;
    }

    u16 xbinary_reader::read_u16()
    {
        u16       b      = 0;
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_u16(ptr);
        }
        return b;
    }

    s16 xbinary_reader::read_s16()
    {
        s16       b      = 0;
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_s16(ptr);
        }
        return b;
    }

    u32 xbinary_reader::read_u32()
    {
        u32       b      = 0;
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_u32(ptr);
        }
        return b;
    }

    s32 xbinary_reader::read_s32()
    {
        s32       b      = 0;
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_s32(ptr);
        }
        return b;
    }

    u64 xbinary_reader::read_u64()
    {
        u64       b      = 0;
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            xbyte const* ptr = buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_u64(ptr);
        }
        return b;
    }

    s64 xbinary_reader::read_s64()
    {
        s64       b      = 0;
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            u8 const* ptr = (u8 const*)buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_s64(ptr);
        }
        return b;
    }

    f32 xbinary_reader::read_f32()
    {
        f32       b      = 0;
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            u8 const* ptr = (u8 const*)buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_f32(ptr);
        }
        return b;
    }

    f64 xbinary_reader::read_f64()
    {
        f64       b      = 0;
        u32 const offset = cursor_;
        if (_can_read(len_, cursor_, sizeof(b)))
        {
            u8 const* ptr = (u8 const*)buffer_ + cursor_;
            cursor_ += sizeof(b);
            b = xuadrw::read_f64(ptr);
        }
        return b;
    }

    // End Direct Read

    s32 xbinary_reader::read_data(xbuffer& buf)
    {
        u32 const size = buf.size();
        if (_can_read(len_, cursor_, size))
        {
            s32 const offset = (s32)cursor_;
            xcbuffer  to_read(size, buffer_ + cursor_);
            buf.writer().write_data(to_read);
            cursor_ += size;
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::view_data(u32 size, xcbuffer& buf)
    {
        s32 const offset = (s32)cursor_;
        buf.m_const      = buffer_ + cursor_;
        buf.m_len        = size;
        cursor_ += size;
        return offset;
    }

    s32 xbinary_reader::read_buffer(xbuffer& buf)
    {
        u32 const size = read_u32();
        if (_can_read(len_, cursor_, size))
        {
            s32 const offset = (s32)cursor_;
            xcbuffer  to_read(size, buffer_ + cursor_);
            buf.writer().write_data(to_read);
            cursor_ += size;
            return offset;
        }
        return -1;
    }

    s32 xbinary_reader::view_buffer(xcbuffer& buf)
    {
        s32 const offset = (s32)cursor_;
        u32 const size   = read_u32();
        buf.m_const      = buffer_ + cursor_;
        buf.m_len        = size;
        cursor_ += size;
        return offset;
    }

    s32 xbinary_reader::view_crunes(crunes_t& out_str)
    {
        if (size() == 0)
            return -1;
        s32 const offset              = (s32)cursor_;
        out_str.m_type                = read_s32();
        s32 const strlen              = read_s32();
        out_str.m_runes.m_ascii.m_bos = (char const*)(buffer_ + cursor_);
        out_str.m_runes.m_ascii.m_str = (out_str.m_runes.m_ascii.m_bos);
        out_str.m_runes.m_ascii.m_end = (out_str.m_runes.m_ascii.m_bos + strlen);
        out_str.m_runes.m_ascii.m_eos = (out_str.m_runes.m_ascii.m_bos + strlen);
        cursor_ += strlen;
        return offset;
    }

    /// ---------------------------------------------------------------------------------------
    /// Binary Writer
    /// ---------------------------------------------------------------------------------------
    inline bool _can_write(u32 _buffer_size, u32 _cursor, u32 _num_bytes) { return (_cursor + _num_bytes) <= _buffer_size; }

    u32 xbinary_writer::size() const { return cursor_; }
    u32 xbinary_writer::len() const { return len_; }

    xbuffer xbinary_writer::get_full_buffer() const
    {
        xbuffer buffer;
        buffer.m_mutable = buffer_;
        buffer.m_len     = len_;
        return buffer;
    }

    xbuffer xbinary_writer::get_current_buffer() const
    {
        xbuffer buffer;
        buffer.m_mutable = buffer_ + cursor_;
        buffer.m_len     = size();
        return buffer;
    }

    bool xbinary_writer::can_write(u32 num_bytes) const { return _can_write(len_, cursor_, num_bytes); }

    bool xbinary_writer::at_end() const { return cursor_ == len_; }

    bool xbinary_writer::seek(u32 cursor)
    {
        if (cursor <= len_)
        {
            cursor_ = cursor;
            return true;
        }
        return false;
    }

    u32 xbinary_writer::pos() const { return cursor_; }

    void xbinary_writer::reset() { cursor_ = 0; }

    s32 xbinary_writer::skip(s32 c)
    {
        s32 const offset = (s32)cursor_;
        if (_can_write(len_, cursor_, c))
        {
            cursor_ += c;
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(bool b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, 1))
        {
            xuadrw::write_u8(buffer_ + cursor_, b ? 1 : 0);
            cursor_ += 1;
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(u8 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_u8(buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(s8 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_u8(buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(u16 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_u16(buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(s16 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_s16(buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(u32 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_u32(buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(s32 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_s32((xbyte*)buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(u64 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_u64((xbyte*)buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(s64 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_s64((xbyte*)buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(f32 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_f32((xbyte*)buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write(f64 b)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, sizeof(b)))
        {
            xuadrw::write_f64((xbyte*)buffer_ + cursor_, b);
            cursor_ += sizeof(b);
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write_data(xbuffer const& buf)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, (u32)buf.size()))
        {
            u32 const    n   = (u32)buf.size();
            xbyte*       dst = (xbyte*)buffer_ + cursor_;
            xbyte const* src = buf.m_mutable;
            for (u32 i = 0; i < n; i++)
                *dst++ = *src++;
            cursor_ += n;
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write_data(xcbuffer const& buf)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, (u32)buf.size()))
        {
            u32 const n = (u32)buf.size();

            xbyte*       dst = (xbyte*)buffer_ + cursor_;
            xbyte const* src = buf.m_const;
            for (u32 i = 0; i < n; i++)
                *dst++ = *src++;
            cursor_ += n;
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write_buffer(xbuffer const& buf)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, (u32)buf.size()))
        {
            u32 const n = (u32)buf.size();
            write(n);
            xbyte*       dst = (xbyte*)buffer_ + cursor_;
            xbyte const* src = buf.m_mutable;
            for (u32 i = 0; i < n; i++)
                *dst++ = *src++;
            cursor_ += n;
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write_buffer(xcbuffer const& buf)
    {
        u32 const offset = cursor_;
        if (_can_write(len_, cursor_, (u32)buf.size()))
        {
            u32 const n = (u32)buf.size();
            write(n);
            xbyte*       dst = (xbyte*)buffer_ + cursor_;
            xbyte const* src = buf.m_const;
            for (u32 i = 0; i < n; i++)
                *dst++ = *src++;
            cursor_ += n;
            return offset;
        }
        return -1;
    }

    s32 xbinary_writer::write_string(crunes_t const& str)
    {
        u32 const   offset = cursor_;
        char const* srcstr = str.m_runes.m_ascii.m_str;
        char const* srcend = str.m_runes.m_ascii.m_end;
        s32 const   size   = (u32)(srcend - srcstr);
        if (_can_write(len_, cursor_, 2 + 2 + size))
        {
            write(str.m_type);
            write(size);

            char* dststr = (char*)buffer_ + cursor_;
            while (srcstr < srcend)
            {
                *dststr++ = *srcstr++;
            }
            cursor_ += size;
            return offset;
        }
        return -1;
    }

    xbinary_writer xbinary_writer::reserve(u32 length)
    {
        xbinary_writer writer;
        if (_can_write(len_, cursor_, length))
        {
            writer.len_    = length;
            writer.cursor_ = cursor_;
            writer.buffer_ = buffer_;
        }
        return writer;
    }

} // namespace xcore
