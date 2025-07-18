#ifndef __CCORE_BUFFER_H__
#define __CCORE_BUFFER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"
#include "cbase/c_memory.h"
#include "ccore/c_debug.h"

namespace ncore
{
    class buffer_t;
    class binary_reader_t;
    class binary_writer_t;

    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------
    class cbuffer_t
    {
    public:
        inline cbuffer_t()
            : m_begin(nullptr)
            , m_end(nullptr)
        {
        }
        inline cbuffer_t(u8 const* data, u8 const* end)
            : m_begin(data)
            , m_end(end)
        {
        }
        cbuffer_t(buffer_t const& buffer);

        inline int_t size() const { return m_end - m_begin; }

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

        void copy_to(buffer_t& dst) const;

        binary_reader_t reader() const;

        u8 const* m_begin;
        u8 const* m_end;
    };

    class buffer_t
    {
    public:
        inline buffer_t()
            : m_begin(nullptr)
            , m_end(nullptr)
            , m_cap(nullptr)
        {
        }
        inline explicit buffer_t(u8* data, u8* end)
            : m_begin(data)
            , m_end(end)
            , m_cap(end)
        {
            reset(0);
        }
        inline explicit buffer_t(u8* data, u8* end, u8* cap)
            : m_begin(data)
            , m_end(end)
            , m_cap(cap)
        {
            reset(0);
        }

        inline int_t size() const { return m_end - m_begin; }
        inline int_t cap() const { return m_cap - m_begin; }
        inline u8*   data() { return m_begin; }

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

        void copy_from(const cbuffer_t& other);

        binary_reader_t reader() const;
        binary_writer_t writer() const;

        u8* m_begin;
        u8* m_end;
        u8* m_cap;
    };

    template <u32 L>
    class memory_t
    {
    public:
        u8 m_data[L];
        inline memory_t() {}

        inline u32 size() const { return L; }

        buffer_t  buffer() const { return buffer_t((u8*)m_data, (u8*)m_data + size()); }
        cbuffer_t cbuffer() const { return cbuffer_t((u8 const*)m_data, (u8 const*)m_data + size()); }

        void reset(u8 fill);

        s32 compare(const memory_t& other) const;

        bool operator==(const memory_t& other) const;
        bool operator!=(const memory_t& other) const;
        bool operator<(const memory_t& other) const;
        bool operator<=(const memory_t& other) const;
        bool operator>(const memory_t& other) const;
        bool operator>=(const memory_t& other) const;
    };

    //
    // Helper class for reading types from binary data
    //
    class binary_reader_t
    {
    public:
        inline binary_reader_t()
            : m_buffer()
            , m_cursor(nullptr)
        {
        }
        inline binary_reader_t(u8 const* begin, u8 const* end)
            : m_buffer(begin, end)
            , m_cursor(begin)
        {
        }
        inline binary_reader_t(buffer_t& buffer)
            : m_buffer(buffer)
            , m_cursor(buffer.m_begin)
        {
        }
        inline binary_reader_t(cbuffer_t& buffer)
            : m_buffer(buffer)
            , m_cursor(buffer.m_begin)
        {
        }

        int_t size() const;
        int_t length() const;
        int_t remain() const;

        cbuffer_t get_current_buffer() const;
        bool      can_read(int_t number_of_bytes) const;  // check if we still can read n number of bytes
        bool      at_end() const;
        bool      seek(int_t cursor);
        int_t     pos() const;
        void      reset();
        int_t     skip(int_t);

        u16 peek_u16() const;

        bool read_bool();
        u8   read_u8();
        s8   read_s8();
        u16  read_u16();
        s16  read_s16();
        u32  read_u32();
        s32  read_s32();
        u64  read_u64();
        s64  read_s64();
        f32  read_f32();
        f64  read_f64();

        void read(bool& value) { value = read_bool(); }
        void read(u8& value) { value = read_u8(); }
        void read(s8& value) { value = read_s8(); }
        void read(u16& value) { value = read_u16(); }
        void read(s16& value) { value = read_s16(); }
        void read(u32& value) { value = read_u32(); }
        void read(s32& value) { value = read_s32(); }
        void read(u64& value) { value = read_u64(); }
        void read(s64& value) { value = read_s64(); }
        void read(f32& value) { value = read_f32(); }
        void read(f64& value) { value = read_f64(); }

        void read_data(buffer_t& buf);
        void view_data(u32 size, cbuffer_t& buf);
        void read_buffer(buffer_t& buf);
        void view_buffer(cbuffer_t& buf);
        void view_string(const char*& out_str, const char*& out_str_end, s8& str_type);

    protected:
        cbuffer_t m_buffer;
        u8 const* m_cursor;
    };

    //
    // Helper class for writing types and strings to binary data
    //

    class binary_writer_t
    {
    public:
        inline binary_writer_t()
            : m_buffer()
            , m_cursor(nullptr)
        {
        }
        inline binary_writer_t(u8* _buffer, u8* _end)
            : m_buffer(_buffer, _end)
            , m_cursor(_buffer)
        {
        }
        inline binary_writer_t(u8* _buffer, u8* _end, u8* _cap)
            : m_buffer(_buffer, _end, _cap)
            , m_cursor(_buffer)
        {
        }
        inline binary_writer_t(const buffer_t& buffer)
            : m_buffer(buffer)
            , m_cursor(buffer.m_begin)
        {
        }
        inline binary_writer_t(binary_writer_t const& other)
            : m_buffer(other.m_buffer)
            , m_cursor(other.m_cursor)
        {
        }

        int_t size() const;
        int_t length() const;
        int_t remain() const;

        buffer_t get_full_buffer() const;
        buffer_t get_current_buffer() const;

        bool  can_write(int_t num_bytes = 0) const;
        bool  at_end() const;
        bool  seek(int_t cursor);
        int_t pos() const;

        void            reset();
        int_t           skip(int_t count);
        binary_writer_t reserve(int_t size);  // For writing something in the future you can remember this place - size
        binary_writer_t range(int_t from, int_t to) const;

        void write(bool);
        void write(u8);
        void write(s8);
        void write(u16);
        void write(s16);
        void write(u32);
        void write(s32);
        void write(u64);
        void write(s64);
        void write(f32);
        void write(f64);

        template <typename T>
        void write(T* ptr)
        {
            write((u64)ptr);
        }
        template <typename T>
        void write(T const* ptr)
        {
            write((u64)ptr);
        }

        void write_data(buffer_t const& cbuf);
        void write_data(cbuffer_t const& cbuf);
        void write_buffer(buffer_t const& cbuf);   // Will write [s32=Length][u8[]=Data]
        void write_buffer(cbuffer_t const& cbuf);  // Will write [s32=Length][u8[]=Data]
        void write_string(const char* str, const char* str_end, s8 str_type);

        binary_writer_t& operator=(const binary_writer_t& other)
        {
            m_buffer = other.m_buffer;
            m_cursor = other.m_cursor;
            return *this;
        }

        buffer_t m_buffer;
        u8*      m_cursor;
    };

    inline cbuffer_t::cbuffer_t(buffer_t const& buffer)
        : m_begin(buffer.m_begin)
        , m_end(buffer.m_end)
    {
    }

    inline s32 compare_buffers(u8 const* buf1, u8 const* end1, u8 const* buf2, u8 const* end2)
    {
        if ((end1 - buf1) == (end2 - buf2))
        {
            while (*buf1 == *buf2)
            {
                if (buf1 == end1)
                    return 0;
                ++buf1;
                ++buf2;
            }
            if (*buf1 < *buf2)
                return -1;
        }
        else
        {
            if ((end1 - buf1) < (end2 - buf2))
                return -1;
        }
        return 1;
    }

    inline s32 buffer_t::compare(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end); }

    inline bool buffer_t::operator==(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) == 0; }
    inline bool buffer_t::operator!=(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) != 0; }
    inline bool buffer_t::operator<(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) < 0; }
    inline bool buffer_t::operator<=(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) <= 0; }
    inline bool buffer_t::operator>(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) > 0; }
    inline bool buffer_t::operator>=(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) >= 0; }
    inline bool buffer_t::operator==(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) == 0; }
    inline bool buffer_t::operator!=(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) != 0; }
    inline bool buffer_t::operator<(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) < 0; }
    inline bool buffer_t::operator<=(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) <= 0; }
    inline bool buffer_t::operator>(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) > 0; }
    inline bool buffer_t::operator>=(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) >= 0; }
    inline s32  cbuffer_t::compare(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end); }
    inline bool cbuffer_t::operator==(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) == 0; }
    inline bool cbuffer_t::operator!=(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) != 0; }
    inline bool cbuffer_t::operator<(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) < 0; }
    inline bool cbuffer_t::operator<=(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) <= 0; }
    inline bool cbuffer_t::operator>(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) > 0; }
    inline bool cbuffer_t::operator>=(const cbuffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) >= 0; }

    inline bool      cbuffer_t::operator==(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) == 0; }
    inline bool      cbuffer_t::operator!=(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) != 0; }
    inline bool      cbuffer_t::operator<(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) < 0; }
    inline bool      cbuffer_t::operator<=(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) <= 0; }
    inline bool      cbuffer_t::operator>(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) > 0; }
    inline bool      cbuffer_t::operator>=(const buffer_t& other) const { return compare_buffers(m_begin, m_end, other.m_begin, other.m_end) >= 0; }
    inline cbuffer_t cbuffer_t::operator()(u32 from, u32 to) const
    {
        u8 const* begin = m_begin + from;
        u8 const* end   = m_end + to;
        if (begin >= m_begin && end <= m_end && begin <= end)
            return cbuffer_t(begin, end);
        return cbuffer_t();
    }

    inline binary_reader_t cbuffer_t::reader() const { return binary_reader_t(m_begin, m_end); }

    inline void buffer_t::reset(u8 fill)
    {
        for (u32 i = 0; i < size(); ++i)
            m_begin[i] = fill;
    }

    inline void buffer_t::clear() { reset(0); }

    inline buffer_t buffer_t::operator()(u32 from, u32 to) const
    {
        u8* begin = m_begin + from;
        u8* end   = m_begin + to;
        if (begin >= m_begin && end <= m_end && begin <= end)
            return buffer_t(begin, end);
        return buffer_t();
    }

    inline binary_reader_t buffer_t::reader() const { return binary_reader_t(m_begin, m_end); }
    inline binary_writer_t buffer_t::writer() const { return binary_writer_t(m_begin, m_end, m_cap); }

    template <u32 L>
    void memory_t<L>::reset(u8 fill)
    {
        for (u32 i = 0; i < size(); ++i)
            m_data[i] = fill;
    }

    template <u32 L>
    s32 memory_t<L>::compare(const memory_t& other) const
    {
        if (size() < other.size())
            return -1;
        if (size() > other.size())
            return 1;
        for (u32 i = 0; i < size(); ++i)
        {
            if (m_data[i] < other.m_data[i])
                return -1;
            if (m_data[i] > other.m_data[i])
                return 1;
        }
        return 0;
    }

    template <u32 L>
    bool memory_t<L>::operator==(const memory_t& other) const
    {
        return compare(other) == 0;
    }
    template <u32 L>
    bool memory_t<L>::operator!=(const memory_t& other) const
    {
        return compare(other) != 0;
    }
    template <u32 L>
    bool memory_t<L>::operator<(const memory_t& other) const
    {
        return compare(other) < 0;
    }
    template <u32 L>
    bool memory_t<L>::operator<=(const memory_t& other) const
    {
        return compare(other) <= 0;
    }
    template <u32 L>
    bool memory_t<L>::operator>(const memory_t& other) const
    {
        return compare(other) > 0;
    }
    template <u32 L>
    bool memory_t<L>::operator>=(const memory_t& other) const
    {
        return compare(other) >= 0;
    }

}  // namespace ncore
#endif  ///< __CCORE_BUFFEbegin
