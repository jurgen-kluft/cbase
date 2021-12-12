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
        inline cbuffer_t()
            : m_len(0)
            , m_const((xbyte const*)&m_len)
        {
        }
        inline cbuffer_t(xsize_t len, xbyte const* data)
            : m_len(len)
            , m_const(data)
        {
        }
        cbuffer_t(buffer_t const& buffer);
        cbuffer_t(crunes_t const& str);

        inline xsize_t size() const { return m_len; }

        s32 compare(const cbuffer_t& other) const;

        bool operator==(const cbuffer_t& other) const;
        bool operator!=(const cbuffer_t& other) const;
        bool operator<(const cbuffer_t& other) const;
        bool operator<=(const cbuffer_t& other) const;
        bool operator>(const cbuffer_t& other) const;
        bool operator>=(const cbuffer_t& other) const;

        cbuffer_t operator()(xsize_t from, xsize_t to) const;

        binary_reader_t reader() const;

        static cbuffer_t from_ascii_string(const char* str);

        xsize_t      m_len;
        xbyte const* m_const;
    };

    class buffer_t
    {
    public:
        inline buffer_t()
            : m_len(0)
            , m_mutable((xbyte*)&m_len)
        {
        }
        inline buffer_t(xsize_t len, xbyte* data)
            : m_len(len)
            , m_mutable(data)
        {
            reset(0);
        }

        inline xsize_t size() const { return m_len; }
        inline xbyte*  data() { return m_mutable; }

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

        buffer_t operator()(xsize_t from, xsize_t to) const;

        binary_reader_t reader() const;
        binary_writer_t writer() const;

        xsize_t m_len;
        xbyte*  m_mutable;
    };

    template <xsize_t L> class xbytes : public buffer_t
    {
        enum
        {
            SIZE = (L + 3) / 4
        };

    public:
        u64 m_data[SIZE];
        inline xbytes()
            : buffer_t(SIZE * 4, (xbyte*)m_data)
        {
        }
        buffer_t buffer() const { return buffer_t(*this); }
    };

    //
    // Helper classes for reading and writing to buffers
    //

    class binary_reader_t
    {
    public:
        inline binary_reader_t()
            : m_len(0)
            , m_cursor(0)
            , m_buffer(nullptr)
        {
        }
        inline binary_reader_t(buffer_t const& b)
            : m_len(b.size())
            , m_cursor(0)
            , m_buffer(b.m_mutable)
        {
        }
        inline binary_reader_t(cbuffer_t const& b)
            : m_len(b.size())
            , m_cursor(0)
            , m_buffer(b.m_const)
        {
        }
        inline binary_reader_t(xbyte const* _buffer, xsize_t _len)
            : m_len(_len)
            , m_cursor(0)
            , m_buffer(_buffer)
        {
        }

        xsize_t size() const;
        xsize_t length() const;
        xsize_t remain() const;

        cbuffer_t get_current_buffer() const;
        bool      can_read(xsize_t number_of_bytes) const; // check if we still can read n number of bytes
        bool      at_end() const;
        bool      seek(xsize_t cursor);
        xsize_t   pos() const;
        void      reset();
        xsize_t   skip(xsize_t);
        xsize_t   read(bool&);
        xsize_t   read(u8&);
        xsize_t   read(s8&);
        xsize_t   read(u16&);
        xsize_t   read(s16&);
        xsize_t   read(u32&);
        xsize_t   read(s32&);
        xsize_t   read(u64&);
        xsize_t   read(s64&);
        xsize_t   read(f32&);
        xsize_t   read(f64&);
        bool      read_bool();
        u8        read_u8();
        s8        read_s8();
        u16       read_u16();
        u16       peek_u16() const;
        s16       read_s16();
        u32       read_u32();
        s32       read_s32();
        u64       read_u64();
        s64       read_s64();
        f32       read_f32();
        f64       read_f64();

        xsize_t read_data(buffer_t& buf);
        xsize_t view_data(xsize_t size, cbuffer_t& buf);
        xsize_t read_buffer(buffer_t& buf);
        xsize_t view_buffer(cbuffer_t& buf);
        xsize_t view_crunes(crunes_t& out_str);

    protected:
        xsize_t      m_len;
        xsize_t      m_cursor;
        xbyte const* m_buffer;
    };

    class binary_writer_t
    {
    public:
        inline binary_writer_t()
            : m_len(0)
            , m_cursor(0)
            , m_buffer()
        {
        }
        inline binary_writer_t(buffer_t const& _buffer)
            : m_len(_buffer.size())
            , m_cursor(0)
            , m_buffer(_buffer.m_mutable)
        {
        }
        inline binary_writer_t(xbyte* _buffer, xsize_t _len)
            : m_len(_len)
            , m_cursor(0)
            , m_buffer(_buffer)
        {
        }
        inline binary_writer_t(binary_writer_t const& other)
            : m_len(other.m_len)
            , m_cursor(other.m_cursor)
            , m_buffer(other.m_buffer)
        {
        }

        xsize_t size() const;
        xsize_t length() const;
        xsize_t remain() const;

        buffer_t get_full_buffer() const;
        buffer_t get_current_buffer() const;

        bool    can_write(xsize_t num_bytes = 0) const;
        bool    at_end() const;
        bool    seek(xsize_t cursor);
        xsize_t pos() const;

        void            reset();
        xsize_t         skip(xsize_t count);
        binary_writer_t reserve(xsize_t size); // For writing something in the future you can remember this place - size
        binary_writer_t range(xsize_t from, xsize_t to) const;

        xsize_t write(bool);
        xsize_t write(u8);
        xsize_t write(s8);
        xsize_t write(u16);
        xsize_t write(s16);
        xsize_t write(u32);
        xsize_t write(s32);
        xsize_t write(u64);
        xsize_t write(s64);
        xsize_t write(f32);
        xsize_t write(f64);

        xsize_t write_data(buffer_t const& cbuf);
        xsize_t write_data(cbuffer_t const& cbuf);
        xsize_t write_buffer(buffer_t const& cbuf);  // Will write [s32=Length][u8[]=Data]
        xsize_t write_buffer(cbuffer_t const& cbuf); // Will write [s32=Length][u8[]=Data]
        xsize_t write_string(crunes_t const& str);

        binary_writer_t& operator=(const binary_writer_t& other)
        {
            m_len    = other.m_len;
            m_cursor = other.m_cursor;
            m_buffer = other.m_buffer;
            return *this;
        }

    protected:
        xsize_t m_len;
        xsize_t m_cursor;
        xbyte*  m_buffer;
    };

    inline cbuffer_t::cbuffer_t(buffer_t const& buffer)
        : m_len(buffer.size())
        , m_const(buffer.m_mutable)
    {
    }

    inline s32 cbuffer_t::compare(const cbuffer_t& other) const
    {
        if (size() < other.size())
            return -1;
        if (size() > other.size())
            return 1;
        for (xsize_t i = 0; i < size(); ++i)
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

    inline cbuffer_t cbuffer_t::operator()(xsize_t from, xsize_t to) const
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
        for (xsize_t i = 0; i < size(); ++i)
            m_mutable[i] = fill;
    }

    inline void buffer_t::clear() { reset(0); }

    inline buffer_t buffer_t::operator()(xsize_t from, xsize_t to) const
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