#ifndef __C_HIERARCHICAL_BIT_BUFFER_H__
#define __C_HIERARCHICAL_BIT_BUFFER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    class alloc_t;

    // Number of bits and how much memory they consume
    // 32 bits
    //  = 4 bytes
    // 1 K bits
    //  = 128+4 byte
    // 32 K bits
    //  = 4 K + 128 + 4 bytes
    // 1 M bits
    //  = 128 K + 4 K + 128 + 4
    // 32 M bits
    //  = 4 M + 128 K + 4 K + 128 + 4

    // NOTE: Maximum number of bits we can manage = 32 M bits

    typedef u32* hbb_data_t;

    // NOTE: The header is separate from the data so that we can re-use this for many hbb_data_t instances
    struct hbb_hdr_t  // 8 bytes
    {
        hbb_hdr_t()
            : m_maxbits(0)
            , m_offset(0)
        {
        }
        hbb_hdr_t(u32 maxbits, u8 numlevels, u32 offset_to_lowest_level)
            : m_maxbits((maxbits << 4) | numlevels)
            , m_offset(offset_to_lowest_level)
        {
        }

        inline u32 get_max_bits() const { return m_maxbits >> 4; }
        inline u8  get_num_levels() const { return m_maxbits & 0x0F; } // Actually this is the number of levels - 1

        u32 m_maxbits;  // u32 m_maxbits : 25, m_numlevels : 4;
        u32 m_offset;  // Offset to the lowest level
    };

    struct hbb_iter_t
    {
        hbb_hdr_t   m_hdr;
        hbb_data_t  m_hbb;
        u32         m_cur;
        u32         m_end;
        inline bool end() const { return !(m_cur < m_end); }
        void        next();
    };

    // A convenient wrapper for hbb_hdr_t and hbb_data_t
    struct hbb_t
    {
        hbb_hdr_t  m_hdr;
        hbb_data_t m_hbb;

        hbb_t()
            : m_hbb(0)
        {
        }

        void       init(u32 maxbits, bool init_all_set, hbb_data_t data);
        void       init(u32 maxbits, bool init_all_set, alloc_t* alloc);
        void       init(u32 maxbits, hbb_data_t data) { init(maxbits, true, data); }
        void       reset(bool init_all_set);
        void       resize(u32 maxbits, bool init_all_set, alloc_t* alloc);
        void       release(alloc_t* alloc);
        hbb_iter_t iterator(u32 start, u32 end);  // Iterate over all '1' bits from start to end
        void       set(u32 bit);
        void       clr(u32 bit);
        bool       is_set(u32 bit);
        bool       is_empty();
        bool       find(u32& bit);
        bool       upper(u32 pivot, u32& bit);  // First 1 equal to or greater than @pivot
        bool       lower(u32 pivot, u32& bit);  // First 1 equal to or lesser than @pivot
    };

    extern u32        g_hbb_sizeof_data(u32 maxbits);  // NOTE: sizeof returns the number of u32 to allocate (not including the header)
    extern void       g_hbb_init(hbb_hdr_t& hdr, u32 maxbits);
    extern void       g_hbb_init(hbb_hdr_t const& hdr, hbb_data_t hbb, bool init_all_set);
    extern void       g_hbb_init(hbb_hdr_t const& hdr, hbb_data_t& hbb, bool init_all_set, alloc_t* alloc);
    extern void       g_hbb_reset(hbb_hdr_t const& hdr, hbb_data_t hbb, bool init_all_set);
    extern void       g_hbb_resize(hbb_hdr_t& hdr, hbb_data_t& hbb, u32 maxbits, bool init_all_set, alloc_t* alloc);
    extern void       g_hbb_release(hbb_data_t& hbb, alloc_t* alloc);
    extern hbb_iter_t g_hbb_iterator(hbb_hdr_t const& hdr, hbb_data_t hbb, u32 start, u32 end);  // Iterate over all '1' bits from start to end
    extern void       g_hbb_set(hbb_hdr_t const& hdr, hbb_data_t hbb, u32 bit);
    extern void       g_hbb_clr(hbb_hdr_t const& hdr, hbb_data_t hbb, u32 bit);
    extern bool       g_hbb_is_set(hbb_hdr_t const& hdr, hbb_data_t const hbb, u32 bit);
    extern bool       g_hbb_is_empty(hbb_hdr_t const& hdr, hbb_data_t const hbb);
    extern bool       g_hbb_find(hbb_hdr_t const& hdr, hbb_data_t const hbb, u32& bit);
    extern bool       g_hbb_upper(hbb_hdr_t const& hdr, hbb_data_t const hbb, u32 pivot, u32& bit);  // First 1 equal to or greater than @pivot
    extern bool       g_hbb_lower(hbb_hdr_t const& hdr, hbb_data_t const hbb, u32 pivot, u32& bit);  // First 1 equal to or lesser than @pivot

    // hbb_t implementation
    inline void hbb_t::init(u32 maxbits, bool init_all_set, hbb_data_t data)
    {
        m_hbb = data;
        g_hbb_init(m_hdr, maxbits);
        g_hbb_init(m_hdr, m_hbb, init_all_set);
    }
    inline void hbb_t::init(u32 maxbits, bool init_all_set, alloc_t* alloc)
    {
        g_hbb_init(m_hdr, maxbits);
        g_hbb_init(m_hdr, m_hbb, init_all_set, alloc);
    }
    inline void       hbb_t::reset(bool init_all_set) { g_hbb_reset(m_hdr, m_hbb, init_all_set); }
    inline void       hbb_t::resize(u32 maxbits, bool init_all_set, alloc_t* alloc) { g_hbb_resize(m_hdr, m_hbb, maxbits, init_all_set, alloc); }
    inline void       hbb_t::release(alloc_t* alloc) { g_hbb_release(m_hbb, alloc); }
    inline hbb_iter_t hbb_t::iterator(u32 start, u32 end) { return g_hbb_iterator(m_hdr, m_hbb, start, end); }
    inline void       hbb_t::set(u32 bit) { g_hbb_set(m_hdr, m_hbb, bit); }
    inline void       hbb_t::clr(u32 bit) { g_hbb_clr(m_hdr, m_hbb, bit); }
    inline bool       hbb_t::is_set(u32 bit) { return g_hbb_is_set(m_hdr, m_hbb, bit); }
    inline bool       hbb_t::is_empty() { return g_hbb_is_empty(m_hdr, m_hbb); }
    inline bool       hbb_t::find(u32& bit) { return g_hbb_find(m_hdr, m_hbb, bit); }
    inline bool       hbb_t::upper(u32 pivot, u32& bit) { return g_hbb_upper(m_hdr, m_hbb, pivot, bit); }
    inline bool       hbb_t::lower(u32 pivot, u32& bit) { return g_hbb_lower(m_hdr, m_hbb, pivot, bit); }

};  // namespace ncore

#endif  /// __C_HIERARCHICAL_BIT_BUFFER_H__