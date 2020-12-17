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
    class buffer_t;
    class binary_reader_t;
    class binary_writer_t;
	struct crunes_t;

    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------
    class cbuffer_t
    {
    public:
        inline cbuffer_t() : m_len(0), m_const((xbyte const*)&m_len) {}
        inline cbuffer_t(u32 len, xbyte const* data) : m_len(len), m_const(data) {}
        cbuffer_t(buffer_t const& buffer);
        cbuffer_t(crunes_t const& str);

        inline u32 size() const { return m_len; }

        s32 compare(const cbuffer_t& other) const;

        bool operator==(const cbuffer_t& other) const;
        bool operator!=(const cbuffer_t& other) const;
        bool operator<(const cbuffer_t& other) const;
        bool operator<=(const cbuffer_t& other) const;
        bool operator>(const cbuffer_t& other) const;
        bool operator>=(const cbuffer_t& other) const;

        cbuffer_t operator()(u32 from, u32 to) const;

        binary_reader_t reader() const;

        static cbuffer_t from_ascii_string(const char* str);

        u32          m_len;
        xbyte const* m_const;
    };

    class buffer_t
    {
    public:
        inline buffer_t() : m_len(0), m_mutable((xbyte*)&m_len) {}
        inline buffer_t(u32 len, xbyte* data) : m_len(len), m_mutable(data) { reset(0); }

        inline u32 size() const { return m_len; }

        void reset(xbyte fill);
        void clear();

        s32 compare(const cbuffer_t& other) const
        {
            cbuffer_t cthis(*this);
            return cthis.compare(other);
        }

        bool operator==(const cbuffer_t& other) const
        {
            cbuffer_t cthis(*this);
            return cthis == other;
        }
        bool operator!=(const cbuffer_t& other) const
        {
            cbuffer_t cthis(*this);
            return cthis != other;
        }
        bool operator<(const cbuffer_t& other) const
        {
            cbuffer_t cthis(*this);
            return cthis < other;
        }
        bool operator<=(const cbuffer_t& other) const
        {
            cbuffer_t cthis(*this);
            return cthis <= other;
        }
        bool operator>(const cbuffer_t& other) const
        {
            cbuffer_t cthis(*this);
            return cthis > other;
        }
        bool operator>=(const cbuffer_t& other) const
        {
            cbuffer_t cthis(*this);
            return cthis >= other;
        }

        bool operator==(const buffer_t& other) const
        {
            cbuffer_t cother(other);
            cbuffer_t cthis(*this);
            return cthis == cother;
        }
        bool operator!=(const buffer_t& other) const
        {
            cbuffer_t cother(other);
            cbuffer_t cthis(*this);
            return cthis != cother;
        }
        bool operator<(const buffer_t& other) const
        {
            cbuffer_t cother(other);
            cbuffer_t cthis(*this);
            return cthis < cother;
        }
        bool operator<=(const buffer_t& other) const
        {
            cbuffer_t cother(other);
            cbuffer_t cthis(*this);
            return cthis <= cother;
        }
        bool operator>(const buffer_t& other) const
        {
            cbuffer_t cother(other);
            cbuffer_t cthis(*this);
            return cthis > cother;
        }
        bool operator>=(const buffer_t& other) const
        {
            cbuffer_t cother(other);
            cbuffer_t cthis(*this);
            return cthis >= cother;
        }

        buffer_t operator()(u32 from, u32 to) const;

        binary_reader_t reader() const;
        binary_writer_t writer() const;

        u32    m_len;
        xbyte* m_mutable;
    };

    template <u32 L> class xbytes : public buffer_t
    {
        enum
        {
            SIZE = (L + 3) / 4
        };

    public:
        u64 m_data[SIZE];
        inline xbytes() : buffer_t(SIZE * 4, (xbyte*)m_data) {}
        buffer_t buffer() const { return buffer_t(*this); }
    };

    //
    // Helper classes for reading and writing to buffers
    //

    class binary_reader_t
    {
    public:
        inline binary_reader_t(buffer_t const& b) : len_(b.size()), cursor_(0), buffer_(b.m_mutable) {}
        inline binary_reader_t(cbuffer_t const& b) : len_(b.size()), cursor_(0), buffer_(b.m_const) {}
        inline binary_reader_t(xbyte const* _buffer, u32 _len) : len_(_len), cursor_(0), buffer_(_buffer) {}

        u32      size() const;
        u32      len() const;
        cbuffer_t get_current_buffer() const;
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
        s32      read_data(buffer_t& buf);
        s32      view_data(u32 size, cbuffer_t& buf);
		s32      read_buffer(buffer_t& buf);
		s32      view_buffer(cbuffer_t& buf);
		s32      view_crunes(crunes_t& out_str);

    protected:
        u32          len_;
        u32          cursor_;
        xbyte const* buffer_;
    };

    class binary_writer_t
    {
    public:
        inline binary_writer_t() : len_(0), cursor_(0), buffer_() {}
        inline binary_writer_t(buffer_t const& _buffer) : len_(_buffer.size()), cursor_(0), buffer_(_buffer.m_mutable) {}
        inline binary_writer_t(xbyte* _buffer, u32 _len) : len_(_len), cursor_(0), buffer_(_buffer) {}

        u32 size() const;
        u32 len() const;

        buffer_t get_full_buffer() const;
        buffer_t get_current_buffer() const;

        bool can_write(u32 num_bytes = 0) const;
        bool at_end() const;
        bool seek(u32 cursor);
        u32  pos() const;

        void           reset();
        s32            skip(s32 count);
        binary_writer_t reserve(u32 size); // For writing something in the future you can remember this place - size

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

        s32 write_data(buffer_t const& cbuf);
        s32 write_data(cbuffer_t const& cbuf);
        s32 write_buffer(buffer_t const& cbuf);	// Will write [s32=Length][u8[]=Data]
        s32 write_buffer(cbuffer_t const& cbuf);	// Will write [s32=Length][u8[]=Data]
        s32 write_string(crunes_t const& str);

    protected:
        u32    len_;
        u32    cursor_;
        xbyte* buffer_;
    };

    inline cbuffer_t::cbuffer_t(buffer_t const& buffer) : m_len(buffer.size()), m_const(buffer.m_mutable) {}

    inline s32 cbuffer_t::compare(const cbuffer_t& other) const
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

    inline bool cbuffer_t::operator==(const cbuffer_t& other) const { return compare(other) == 0; }
    inline bool cbuffer_t::operator!=(const cbuffer_t& other) const { return compare(other) != 0; }
    inline bool cbuffer_t::operator<(const cbuffer_t& other) const { return compare(other) < 0; }
    inline bool cbuffer_t::operator<=(const cbuffer_t& other) const { return compare(other) <= 0; }
    inline bool cbuffer_t::operator>(const cbuffer_t& other) const { return compare(other) > 0; }
    inline bool cbuffer_t::operator>=(const cbuffer_t& other) const { return compare(other) >= 0; }

    inline cbuffer_t cbuffer_t::operator()(u32 from, u32 to) const
    {
        if (from < 0 || from >= to || from >= size())
            return cbuffer_t();
        if (to >= size())
            return cbuffer_t();
        return cbuffer_t(to - from, m_const + from);
    }

    inline binary_reader_t cbuffer_t::reader() const { return binary_reader_t(m_const, size()); }

    inline void buffer_t::reset(xbyte fill)
    {
        for (u32 i = 0; i < size(); ++i)
            m_mutable[i] = fill;
    }

    inline void buffer_t::clear() { reset(0); }

    inline buffer_t buffer_t::operator()(u32 from, u32 to) const
    {
        if (from < 0 || from >= to || from >= size())
            return buffer_t();
        if (to >= size())
            return buffer_t();
        return buffer_t(to - from, m_mutable + from);
    }

    inline binary_reader_t buffer_t::reader() const { return binary_reader_t(m_mutable, size()); }
    inline binary_writer_t buffer_t::writer() const { return binary_writer_t(m_mutable, size()); }
} // namespace xcore

#endif ///< __XCORE_BUFFER_H__