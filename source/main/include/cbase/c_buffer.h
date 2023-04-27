#ifndef __CCORE_BUFFER_H__
#define __CCORE_BUFFER_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"
#include "cbase/c_memory.h"
#include "cbase/c_debug.h"

namespace ncore
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
            , m_const((u8 const*)&m_len)
        {
        }
        inline cbuffer_t(s32 len, u8 const* data)
            : m_len(len)
            , m_const(data)
        {
        }
        cbuffer_t(buffer_t const& buffer);

        inline u32 size() const { return m_len; }

        s32 compare(const cbuffer_t& other) const;

        bool operator==(const cbuffer_t& other) const;
        bool operator!=(const cbuffer_t& other) const;
        bool operator<(const cbuffer_t& other) const;
        bool operator<=(const cbuffer_t& other) const;
        bool operator>(const cbuffer_t& other) const;
        bool operator>=(const cbuffer_t& other) const;

        bool operator==(const buffer_t& other) const;
        bool operator!=(const buffer_t& other) const;
        bool operator<(const buffer_t& other) const;
        bool operator<=(const buffer_t& other) const;
        bool operator>(const buffer_t& other) const;
        bool operator>=(const buffer_t& other) const;

        cbuffer_t operator()(u32 from, u32 to) const;

        binary_reader_t reader() const;

        u8 const* m_const;
        s32       m_len;
    };

    class buffer_t
    {
    public:
        inline buffer_t()
            : m_len(0)
            , m_mutable((u8*)&m_len)
        {
        }
        inline buffer_t(u32 len, u8* data)
            : m_len(len)
            , m_mutable(data)
        {
            reset(0);
        }

        inline u32 size() const { return m_len; }
        inline u8* data() { return m_mutable; }

        void reset(u8 fill);
        void clear();

        s32 compare(const cbuffer_t& other) const;

        bool operator==(const cbuffer_t& other) const;
        bool operator!=(const cbuffer_t& other) const;
        bool operator<(const cbuffer_t& other) const;
        bool operator<=(const cbuffer_t& other) const;
        bool operator>(const cbuffer_t& other) const;
        bool operator>=(const cbuffer_t& other) const;

        bool operator==(const buffer_t& other) const;
        bool operator!=(const buffer_t& other) const;
        bool operator<(const buffer_t& other) const;
        bool operator<=(const buffer_t& other) const;
        bool operator>(const buffer_t& other) const;
        bool operator>=(const buffer_t& other) const;

        buffer_t operator()(u32 from, u32 to) const;

        binary_reader_t reader() const;
        binary_writer_t writer() const;

        u8* m_mutable;
        u32 m_len;
    };

    template <u32 L> class u8s : public buffer_t
    {
        enum
        {
            SIZE = (L + 3) / 4
        };

    public:
        u64 m_data[SIZE];
        inline u8s()
            : buffer_t(SIZE * 4, (u8*)m_data)
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
        inline binary_reader_t(u8 const* _buffer, u32 _len)
            : m_len(_len)
            , m_cursor(0)
            , m_buffer(_buffer)
        {
        }

        u32 size() const;
        u32 length() const;
        u32 remain() const;

        cbuffer_t get_current_buffer() const;
        bool      can_read(u32 number_of_bytes) const; // check if we still can read n number of bytes
        bool      at_end() const;
        bool      seek(u32 cursor);
        u32       pos() const;
        void      reset();
        u32       skip(u32);
        u32       read(bool&);
        u32       read(u8&);
        u32       read(s8&);
        u32       read(u16&);
        u32       read(s16&);
        u32       read(u32&);
        u32       read(s32&);
        u32       read(u64&);
        u32       read(s64&);
        u32       read(f32&);
        u32       read(f64&);
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

        u32 read_data(buffer_t& buf);
        u32 view_data(u32 size, cbuffer_t& buf);
        u32 read_buffer(buffer_t& buf);
        u32 view_buffer(cbuffer_t& buf);
        u32 view_crunes(crunes_t& out_str);

    protected:
        u32       m_len;
        u32       m_cursor;
        u8 const* m_buffer;
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
        inline binary_writer_t(u8* _buffer, s32 _len)
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

        u32 size() const;
        u32 length() const;
        u32 remain() const;

        buffer_t get_full_buffer() const;
        buffer_t get_current_buffer() const;

        bool can_write(u32 num_bytes = 0) const;
        bool at_end() const;
        bool seek(u32 cursor);
        u32  pos() const;

        void            reset();
        u32             skip(u32 count);
        binary_writer_t reserve(u32 size); // For writing something in the future you can remember this place - size
        binary_writer_t range(u32 from, u32 to) const;

        u32 write(bool);
        u32 write(u8);
        u32 write(s8);
        u32 write(u16);
        u32 write(s16);
        u32 write(u32);
        u32 write(s32);
        u32 write(u64);
        u32 write(s64);
        u32 write(f32);
        u32 write(f64);

        u32 write_data(buffer_t const& cbuf);
        u32 write_data(cbuffer_t const& cbuf);
        u32 write_buffer(buffer_t const& cbuf);  // Will write [s32=Length][u8[]=Data]
        u32 write_buffer(cbuffer_t const& cbuf); // Will write [s32=Length][u8[]=Data]
        u32 write_string(crunes_t const& str);

        binary_writer_t& operator=(const binary_writer_t& other)
        {
            m_len    = other.m_len;
            m_cursor = other.m_cursor;
            m_buffer = other.m_buffer;
            return *this;
        }

    protected:
        u32 m_len;
        u32 m_cursor;
        u8* m_buffer;
    };

    inline cbuffer_t::cbuffer_t(buffer_t const& buffer)
        : m_len(buffer.size())
        , m_const(buffer.m_mutable)
    {
    }

    inline s32 compare_buffers(u8 const* buf1, u32 len1, u8 const* buf2, u32 len2)
    {
        if (len1 == len2)
        {
            len1 = 0;
            while (len1 < len2 && buf1[len1] == buf2[len1])
                ++len1;
            if (len1 == len2)
                return 0;
            if (buf1[len1] < buf2[len1])
                return -1;
        }
        else
        {
            if (len1 < len2)
                return -1;
        }
        return 1;
    }

    inline s32  buffer_t::compare(const cbuffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_const, other.m_len); }

    inline bool buffer_t::operator==(const buffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_mutable, other.m_len) == 0; }
    inline bool buffer_t::operator!=(const buffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_mutable, other.m_len) != 0; }
    inline bool buffer_t::operator<(const buffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_mutable, other.m_len) < 0; }
    inline bool buffer_t::operator<=(const buffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_mutable, other.m_len) <= 0; }
    inline bool buffer_t::operator>(const buffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_mutable, other.m_len) > 0; }
    inline bool buffer_t::operator>=(const buffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_mutable, other.m_len) >= 0; }

    inline bool buffer_t::operator==(const cbuffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_const, other.m_len) == 0; }
    inline bool buffer_t::operator!=(const cbuffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_const, other.m_len) != 0; }
    inline bool buffer_t::operator<(const cbuffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_const, other.m_len) < 0; }
    inline bool buffer_t::operator<=(const cbuffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_const, other.m_len) <= 0; }
    inline bool buffer_t::operator>(const cbuffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_const, other.m_len) > 0; }
    inline bool buffer_t::operator>=(const cbuffer_t& other) const { return compare_buffers(m_mutable, m_len, other.m_const, other.m_len) >= 0; }

    inline s32  cbuffer_t::compare(const cbuffer_t& other) const { return compare_buffers(m_const, m_len, other.m_const, other.m_len); }

    inline bool cbuffer_t::operator==(const cbuffer_t& other) const { return compare_buffers(m_const, m_len, other.m_const, other.m_len) == 0; }
    inline bool cbuffer_t::operator!=(const cbuffer_t& other) const { return compare_buffers(m_const, m_len, other.m_const, other.m_len) != 0; }
    inline bool cbuffer_t::operator<(const cbuffer_t& other) const { return compare_buffers(m_const, m_len, other.m_const, other.m_len) < 0; }
    inline bool cbuffer_t::operator<=(const cbuffer_t& other) const { return compare_buffers(m_const, m_len, other.m_const, other.m_len) <= 0; }
    inline bool cbuffer_t::operator>(const cbuffer_t& other) const { return compare_buffers(m_const, m_len, other.m_const, other.m_len) > 0; }
    inline bool cbuffer_t::operator>=(const cbuffer_t& other) const { return compare_buffers(m_const, m_len, other.m_const, other.m_len) >= 0; }

    inline bool cbuffer_t::operator==(const buffer_t& other) const { return compare_buffers(m_const, m_len, other.m_mutable, other.m_len) == 0; }
    inline bool cbuffer_t::operator!=(const buffer_t& other) const { return compare_buffers(m_const, m_len, other.m_mutable, other.m_len) != 0; }
    inline bool cbuffer_t::operator<(const buffer_t& other) const { return compare_buffers(m_const, m_len, other.m_mutable, other.m_len) < 0; }
    inline bool cbuffer_t::operator<=(const buffer_t& other) const { return compare_buffers(m_const, m_len, other.m_mutable, other.m_len) <= 0; }
    inline bool cbuffer_t::operator>(const buffer_t& other) const { return compare_buffers(m_const, m_len, other.m_mutable, other.m_len) > 0; }
    inline bool cbuffer_t::operator>=(const buffer_t& other) const { return compare_buffers(m_const, m_len, other.m_mutable, other.m_len) >= 0; }

    inline cbuffer_t cbuffer_t::operator()(u32 from, u32 to) const
    {
        if (from < 0 || from >= to || from >= size())
            return cbuffer_t();
        if (to >= size())
            return cbuffer_t();
        return cbuffer_t(to - from, m_const + from);
    }

    inline binary_reader_t cbuffer_t::reader() const { return binary_reader_t(m_const, size()); }

    inline void buffer_t::reset(u8 fill)
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
} // namespace ncore

#endif ///< __CCORE_BUFFER_H__