#ifndef __XCORE_BUFFER_H__
#define __XCORE_BUFFER_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_allocator.h"
#include "xbase/x_memory.h"
#include "xbase/x_debug.h"

namespace xcore
{
    class xbuffer;
    class xbinary_reader;
    class xbinary_writer;
	struct crunes_t;

    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------
    class xcbuffer
    {
    public:
        inline xcbuffer() : m_len(0), m_const((xbyte const*)&m_len) {}
        inline xcbuffer(u32 len, xbyte const* data) : m_len(len), m_const(data) {}
        xcbuffer(xbuffer const& buffer);
        xcbuffer(crunes_t const& str);

        inline u32 size() const { return m_len; }

        s32 compare(const xcbuffer& other) const;

        bool operator==(const xcbuffer& other) const;
        bool operator!=(const xcbuffer& other) const;
        bool operator<(const xcbuffer& other) const;
        bool operator<=(const xcbuffer& other) const;
        bool operator>(const xcbuffer& other) const;
        bool operator>=(const xcbuffer& other) const;

        xcbuffer operator()(u32 from, u32 to) const;

        xbinary_reader reader() const;

        static xcbuffer from_ascii_string(const char* str);

        u32          m_len;
        xbyte const* m_const;
    };

    class xbuffer
    {
    public:
        inline xbuffer() : m_len(0), m_mutable((xbyte*)&m_len) {}
        inline xbuffer(u32 len, xbyte* data) : m_len(len), m_mutable(data) { reset(0); }

        inline u32 size() const { return m_len; }

        void reset(xbyte fill);
        void clear();

        s32 compare(const xcbuffer& other) const
        {
            xcbuffer cthis(*this);
            return cthis.compare(other);
        }

        bool operator==(const xcbuffer& other) const
        {
            xcbuffer cthis(*this);
            return cthis == other;
        }
        bool operator!=(const xcbuffer& other) const
        {
            xcbuffer cthis(*this);
            return cthis != other;
        }
        bool operator<(const xcbuffer& other) const
        {
            xcbuffer cthis(*this);
            return cthis < other;
        }
        bool operator<=(const xcbuffer& other) const
        {
            xcbuffer cthis(*this);
            return cthis <= other;
        }
        bool operator>(const xcbuffer& other) const
        {
            xcbuffer cthis(*this);
            return cthis > other;
        }
        bool operator>=(const xcbuffer& other) const
        {
            xcbuffer cthis(*this);
            return cthis >= other;
        }

        bool operator==(const xbuffer& other) const
        {
            xcbuffer cother(other);
            xcbuffer cthis(*this);
            return cthis == cother;
        }
        bool operator!=(const xbuffer& other) const
        {
            xcbuffer cother(other);
            xcbuffer cthis(*this);
            return cthis != cother;
        }
        bool operator<(const xbuffer& other) const
        {
            xcbuffer cother(other);
            xcbuffer cthis(*this);
            return cthis < cother;
        }
        bool operator<=(const xbuffer& other) const
        {
            xcbuffer cother(other);
            xcbuffer cthis(*this);
            return cthis <= cother;
        }
        bool operator>(const xbuffer& other) const
        {
            xcbuffer cother(other);
            xcbuffer cthis(*this);
            return cthis > cother;
        }
        bool operator>=(const xbuffer& other) const
        {
            xcbuffer cother(other);
            xcbuffer cthis(*this);
            return cthis >= cother;
        }

        xbuffer operator()(u32 from, u32 to) const;

        xbinary_reader reader() const;
        xbinary_writer writer() const;

        u32    m_len;
        xbyte* m_mutable;
    };

    template <u32 L> class xbytes : public xbuffer
    {
        enum
        {
            SIZE = (L + 3) / 4
        };

    public:
        u64 m_data[SIZE];
        inline xbytes() : xbuffer(SIZE * 4, (xbyte*)m_data) {}
        xbuffer buffer() const { return xbuffer(*this); }
    };

    //
    // Helper classes for reading and writing to buffers
    //

    class xbinary_reader
    {
    public:
        inline xbinary_reader(xbuffer const& b) : len_(b.size()), cursor_(0), buffer_(b.m_mutable) {}
        inline xbinary_reader(xcbuffer const& b) : len_(b.size()), cursor_(0), buffer_(b.m_const) {}
        inline xbinary_reader(xbyte const* _buffer, u32 _len) : len_(_len), cursor_(0), buffer_(_buffer) {}

        u32      size() const;
        u32      len() const;
        xcbuffer get_current_buffer() const;
        bool     can_read(u32 number_of_bytes) const; // check if we still can read n number of bytes
        bool     at_end() const;
        bool     seek(u32 cursor);
        u32      pos() const;
        void     reset();
        s32      skip(s32);
        s32      read(bool&);
        s32      read(u8&);
        s32      read(s8&);
        s32      read(u16&);
        s32      read(s16&);
        s32      read(u32&);
        s32      read(s32&);
        s32      read(u64&);
        s32      read(s64&);
        s32      read(f32&);
        s32      read(f64&);
        bool     read_bool();
        u8       read_u8();
        s8       read_s8();
        u16      read_u16();
        s16      read_s16();
        u32      read_u32();
        s32      read_s32();
        u64      read_u64();
        s64      read_s64();
        f32      read_f32();
        f64      read_f64();
        s32      read_data(xbuffer& buf);
        s32      view_data(u32 size, xcbuffer& buf);
		s32      read_buffer(xbuffer& buf);
		s32      view_buffer(xcbuffer& buf);
		s32      view_crunes(crunes_t& out_str);

    protected:
        u32          len_;
        u32          cursor_;
        xbyte const* buffer_;
    };

    class xbinary_writer
    {
    public:
        inline xbinary_writer() : len_(0), cursor_(0), buffer_() {}
        inline xbinary_writer(xbuffer const& _buffer) : len_(_buffer.size()), cursor_(0), buffer_(_buffer.m_mutable) {}
        inline xbinary_writer(xbyte* _buffer, u32 _len) : len_(_len), cursor_(0), buffer_(_buffer) {}

        u32 size() const;
        u32 len() const;

        xbuffer get_full_buffer() const;
        xbuffer get_current_buffer() const;

        bool can_write(u32 num_bytes = 0) const;
        bool at_end() const;
        bool seek(u32 cursor);
        u32  pos() const;

        void           reset();
        s32            skip(s32 count);
        xbinary_writer reserve(u32 size); // For writing something in the future you can remember this place - size

        s32 write(bool);
        s32 write(u8);
        s32 write(s8);
        s32 write(u16);
        s32 write(s16);
        s32 write(u32);
        s32 write(s32);
        s32 write(u64);
        s32 write(s64);
        s32 write(f32);
        s32 write(f64);

        s32 write_data(xbuffer const& cbuf);
        s32 write_data(xcbuffer const& cbuf);
        s32 write_buffer(xbuffer const& cbuf);	// Will write [s32=Length][u8[]=Data]
        s32 write_buffer(xcbuffer const& cbuf);	// Will write [s32=Length][u8[]=Data]
        s32 write_string(crunes_t const& str);

    protected:
        u32    len_;
        u32    cursor_;
        xbyte* buffer_;
    };

    inline xcbuffer::xcbuffer(xbuffer const& buffer) : m_len(buffer.size()), m_const(buffer.m_mutable) {}

    inline s32 xcbuffer::compare(const xcbuffer& other) const
    {
        if (size() < other.size())
            return -1;
        if (size() > other.size())
            return 1;
        for (s64 i = 0; i < size(); ++i)
        {
            if (m_const[i] < other.m_const[i])
                return -1;
            else if (m_const[i] > other.m_const[i])
                return 1;
        }
        return 0;
    }

    inline bool xcbuffer::operator==(const xcbuffer& other) const { return compare(other) == 0; }
    inline bool xcbuffer::operator!=(const xcbuffer& other) const { return compare(other) != 0; }
    inline bool xcbuffer::operator<(const xcbuffer& other) const { return compare(other) < 0; }
    inline bool xcbuffer::operator<=(const xcbuffer& other) const { return compare(other) <= 0; }
    inline bool xcbuffer::operator>(const xcbuffer& other) const { return compare(other) > 0; }
    inline bool xcbuffer::operator>=(const xcbuffer& other) const { return compare(other) >= 0; }

    inline xcbuffer xcbuffer::operator()(u32 from, u32 to) const
    {
        if (from < 0 || from >= to || from >= size())
            return xcbuffer();
        if (to >= size())
            return xcbuffer();
        return xcbuffer(to - from, m_const + from);
    }

    inline xbinary_reader xcbuffer::reader() const { return xbinary_reader(m_const, size()); }

    inline void xbuffer::reset(xbyte fill)
    {
        for (u32 i = 0; i < size(); ++i)
            m_mutable[i] = fill;
    }

    inline void xbuffer::clear() { reset(0); }

    inline xbuffer xbuffer::operator()(u32 from, u32 to) const
    {
        if (from < 0 || from >= to || from >= size())
            return xbuffer();
        if (to >= size())
            return xbuffer();
        return xbuffer(to - from, m_mutable + from);
    }

    inline xbinary_reader xbuffer::reader() const { return xbinary_reader(m_mutable, size()); }
    inline xbinary_writer xbuffer::writer() const { return xbinary_writer(m_mutable, size()); }
} // namespace xcore

#endif ///< __XCORE_BUFFER_H__