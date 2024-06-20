#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"
#include "cbase/c_integer.h"
#include "cbase/c_memory.h"

#include "cbase/c_hbb.h"

namespace ncore
{
    // level n -> offset                   (bits) (max bits at that level)
    // level 0 -> 0                        (0) (32 bits)
    // level 1 -> 1                        (0) (1_024 bits)
    // level 2 -> 32+1                     (5) (32_768 bits)
    // level 3 -> 1024+32+1                (11) (1_048_576 bits)
    // level 4 -> 32768+1024+32+1          (16) (33_554_432 bits)

    static inline u32 s_get_num_dwords(u32 maxbits, s8 numlevels)
    {
        u32       n      = 0;
        s8 const levels = numlevels;
        u32       len    = (maxbits + 31) >> 5;
        switch (levels)
        {
            case 4: n += len; len = (len + 31) >> 5;  // fall through
            case 3: n += len; len = (len + 31) >> 5;  // fall through
            case 2: n += len; len = (len + 31) >> 5;  // fall through
            case 1: n += len; len = (len + 31) >> 5;  // fall through
            case 0: n += 1; break;
        }
        return n;
    }

    u32 g_hbb_sizeof_data(u32 maxbits)
    {
        u32       n      = 0;
        s8 const levels = (s8)(math::mostSignificantBit(maxbits - 1) / 5);
        u32       len    = (maxbits + 31) >> 5;
        switch (levels)
        {
            case 4: n += len; len = (len + 31) >> 5;  // fall through
            case 3: n += len; len = (len + 31) >> 5;  // fall through
            case 2: n += len; len = (len + 31) >> 5;  // fall through
            case 1: n += len; len = (len + 31) >> 5;  // fall through
            case 0: n += 1; break;
        }
        return n;
    }

    static inline u32 s_get_lowest_level_offset(hbb_hdr_t const& hdr)
    {
        u32       offset = 0;
        u32 const count  = hdr.get_max_bits();
        s8 const levels = hdr.get_num_levels();
        u32       len    = (count + 31) >> 5;
        switch (levels)
        {
            case 4: len = (len + 31) >> 5; offset += len;  // fall through
            case 3: len = (len + 31) >> 5; offset += len;  // fall through
            case 2: len = (len + 31) >> 5; offset += len;  // fall through
            case 1: offset += 1; break;
        }
        return offset;
    }

    static inline void s_get_level_offsets(hbb_hdr_t const& hdr, u32 offsets[5])
    {
        s8 const levels     = hdr.get_num_levels();
        u32       len        = hdr.get_max_bits();
        u32       lengths[5] = {1, 0, 0, 0, 0};
        switch (levels)
        {
            case 4: len = (len + 31) >> 5; lengths[4] = len;  // fall through
            case 3: len = (len + 31) >> 5; lengths[3] = len;  // fall through
            case 2: len = (len + 31) >> 5; lengths[2] = len;  // fall through
            case 1: len = (len + 31) >> 5; lengths[1] = len;  // fall through
            case 0: break;
        }
        offsets[0] = 0;
        for (s8 i = 1; i <= levels; ++i)
            offsets[i] = offsets[i - 1] + lengths[i - 1];
    }

    //static inline u32 s_num_dwords(u32 bits) { return (bits + 31) / 32; }

    static void s_hbb_init(hbb_hdr_t& hdr_dst, u32 maxbits)
    {
        // We can have a maximum of 5 levels, each level holds 5 bits
        ASSERT(maxbits < (1 << 25));
        u8 const levels   = (u8)(math::mostSignificantBit(maxbits - 1) / 5);
        hdr_dst.m_maxbits = (maxbits << 4) | levels;
        hdr_dst.m_offset  = s_get_lowest_level_offset(hdr_dst);
    }

    static void s_hbb_mask(hbb_hdr_t const& hdr, hbb_data_t hbb)
    {
        u32      maxbits  = hdr.get_max_bits();
        s8 const maxlevel = hdr.get_num_levels();

        // Bits per level
        u32 numbits[5] = {0, 0, 0, 0, 0};
        u32 len        = maxbits;
        switch (maxlevel)
        {
            case 4: numbits[4] = len; len = (len + 31) >> 5;  // fall through
            case 3: numbits[3] = len; len = (len + 31) >> 5;  // fall through
            case 2: numbits[2] = len; len = (len + 31) >> 5;  // fall through
            case 1: numbits[1] = len; len = (len + 31) >> 5;  // fall through
            case 0: numbits[0] = len; break;
        }

        s8   level  = 0;
        u32* plevel = hbb;
        while (level <= maxlevel)
        {
            // compute mask
            u32 const mask = (u32)((u64)0xFFFFFFFF << (numbits[level] & 0x1F));
            *plevel        = *plevel | mask;
            level += 1;
            plevel += ((numbits[level] + 31) >> 5);
        }
    }

    void g_hbb_init(hbb_hdr_t& hdr, u32 maxbits) { s_hbb_init(hdr, maxbits); }

    void g_hbb_init(const hbb_hdr_t& hdr, hbb_data_t hbb, bool init_all_set)
    {
        u32 const fill = init_all_set ? 0xFFFFFFFF : 0;

        u32*      data    = hbb;
        const u32 ndwords = s_get_num_dwords(hdr.get_max_bits(), hdr.get_num_levels());
        for (u32 i = 0; i < ndwords; ++i)
            data[i] = fill;

        if (!init_all_set)
        {
            s_hbb_mask(hdr, hbb);
        }
    }

    void g_hbb_init(hbb_hdr_t const& hdr, hbb_data_t& hbb, bool init_all_set, alloc_t* alloc)
    {
        const u32 ndwords = s_get_num_dwords(hdr.get_max_bits(), hdr.get_num_levels());
        hbb               = (u32*)alloc->allocate(ndwords * sizeof(u32), sizeof(u32));
        g_hbb_init(hdr, hbb, init_all_set);
    }

    void g_hbb_release(hbb_data_t& hbb, alloc_t* alloc)
    {
        if (alloc != nullptr)
        {
            alloc->deallocate(hbb);
            hbb = nullptr;
        }
    }

    void g_hbb_reset(const hbb_hdr_t& hdr, hbb_data_t hbb, bool init_all_set) { g_hbb_init(hdr, hbb, init_all_set); }

    void g_hbb_resize(hbb_hdr_t& hdr_dst, hbb_data_t& hbb, u32 maxbits, bool init_all_set, alloc_t* alloc)
    {
        if (hbb == nullptr)
        {
            g_hbb_init(hdr_dst, hbb, init_all_set, alloc);
        }
        else
        {
            if (maxbits > hdr_dst.get_max_bits())
            {
                hbb_hdr_t hdr_src = hdr_dst;

                u32 src_offsets[5];
                s_get_level_offsets(hdr_src, src_offsets);

                hbb_data_t hbb_dst = nullptr;
                g_hbb_init(hdr_dst, maxbits);
                g_hbb_init(hdr_dst, hbb_dst, init_all_set, alloc);

                u32 dst_offsets[5];
                s_get_level_offsets(hdr_dst, dst_offsets);

                u32 const fill = init_all_set ? 0xFFFFFFFF : 0;

                // Copy the level data
                s8  dst_level   = hdr_dst.get_num_levels();
                s8  src_level   = hdr_src.get_num_levels();
                u32 dst_maxbits = hdr_dst.get_max_bits();
                u32 src_maxbits = hdr_src.get_max_bits();
                while (src_level >= 0)
                {
                    // Number of words of the hdr_src level
                    u32 const  srcndwords = (src_maxbits + 31) / 32;
                    u32 const  dstndwords = (dst_maxbits + 31) / 32;
                    u32 const* srcdata    = hbb + src_offsets[src_level];
                    u32*       dstdata    = hbb_dst + dst_offsets[dst_level];
                    for (u32 i = 0; i < srcndwords; ++i)
                        dstdata[i] = srcdata[i];

                    // Fill the rest
                    for (u32 i = srcndwords; i < dstndwords; ++i)
                        dstdata[i] = fill;

                    src_maxbits = srcndwords;
                    dst_maxbits = dstndwords;
                    src_level -= 1;
                    dst_level -= 1;
                }

                // Now it can happen that dst has more levels than hdr_src
                if (dst_level >= 0)
                {
                    while (dst_level >= 0)
                    {
                        u32 const dstndwords = (dst_maxbits + 31) / 32;
                        u32*      dstdata    = hbb_dst + dst_offsets[dst_level];
                        for (u32 i = 0; i < dstndwords; ++i)
                            dstdata[i] = fill;
                        dst_maxbits = dstndwords;
                        dst_level -= 1;
                    }

                    if (fill != 0)
                    {
                        s_hbb_mask(hdr_dst, hbb_dst);
                    }

                    u32 const* srcdata = hbb;
                    dst_level          = (hdr_dst.get_num_levels() - hdr_src.get_num_levels()) - 1;
                    if (*srcdata == 0)
                    {
                        while (dst_level >= 0)
                        {
                            u32* dstdata = hbb_dst + dst_offsets[dst_level];
                            *dstdata &= 0xFFFFFFFE;
                            dst_level -= 1;
                        }
                    }
                    else
                    {
                        // hmmm, we need to propogate up
                        while (dst_level >= 0)
                        {
                            u32* dstdata = hbb_dst + dst_offsets[dst_level];
                            *dstdata |= 0x00000001;
                            dst_level -= 1;
                        }
                    }
                }

                g_hbb_release(hbb, alloc);
                hbb = hbb_dst;
            }
            else if (maxbits < hdr_dst.get_max_bits())
            {
                // TODO: Hmmm, we need to shrink, how do we do that?
            }
        }
    }

    void g_hbb_set(const hbb_hdr_t& hdr, hbb_data_t hbb, u32 bit)
    {
        // set bit in full level, then avalanche up if necessary
        u32 level_offset    = hdr.m_offset;
        u32 level_numdwords = (hdr.get_max_bits() + 31) >> 5;

        s8 i = hdr.get_num_levels();
        while (i >= 0)
        {
            ASSERT((i >= 2 && level_offset >= 2) || (i == 1 && level_offset == 1) || (i == 0 && level_offset == 0));
            u32*      level      = hbb + level_offset;
            u32 const dwordIndex = bit / 32;
            u32 const dwordBit   = 1 << (bit & 31);
            u32 const dword0     = level[dwordIndex];
            u32 const dword1     = dword0 | dwordBit;
            level[dwordIndex]    = dword1;

            bool const avalanche = (dword0 != dword1 && dword0 == 0);
            if (!avalanche)
                break;

            i -= 1;
            bit = bit >> 5;

            level_numdwords = (level_numdwords + 31) >> 5;
            level_offset -= level_numdwords;
        }
    }

    void g_hbb_clr(const hbb_hdr_t& hdr, hbb_data_t hbb, u32 bit)
    {
        // clear bit in level 0, then avalanche up if necessary
        u32 level_offset    = hdr.m_offset;
        u32 level_numdwords = (hdr.get_max_bits() + 31) >> 5;

        s8 i = hdr.get_num_levels();
        while (i >= 0)
        {
            ASSERT((i >= 2 && level_offset >= 2) || (i == 1 && level_offset == 1) || (i == 0 && level_offset == 0));
            u32* const level      = hbb + level_offset;
            u32 const  dwordIndex = bit / 32;
            u32 const  dwordBit   = 1 << (bit & 31);
            u32 const  dword0     = level[dwordIndex];
            u32 const  dword1     = dword0 & ~dwordBit;

            level[dwordIndex] = dword1;

            bool const avalanche = (dword0 != dword1 && dword1 == 0);
            if (!avalanche)
                break;

            i -= 1;
            bit = bit >> 5;

            level_numdwords = (level_numdwords + 31) >> 5;
            level_offset -= level_numdwords;
        }
    }

    bool g_hbb_is_set(const hbb_hdr_t& hdr, hbb_data_t const hbb, u32 bit)
    {
        u32 const* level      = hbb + hdr.m_offset;
        u32 const  dwordIndex = bit / 32;
        u32 const  dwordBit   = bit & 31;
        return ((level[dwordIndex] >> dwordBit) & 1) == 1;
    }

    bool g_hbb_is_empty(const hbb_hdr_t& hdr, hbb_data_t const hbb)
    {
        u32 const* level0 = hbb;
        return level0[0] == 0;
    }

    bool g_hbb_find(const hbb_hdr_t& hdr, hbb_data_t const hbb, u32& bit)
    {
        s8 const maxlevel = hdr.get_num_levels();

        // Start at top level and find a '1' bit and move down
        u32 offsets[5];
        s_get_level_offsets(hdr, offsets);

        u32 dwordIndex = 0;
        u32 dwordBit   = 0;
        s8  i          = 0;
        while (i <= maxlevel)
        {
            u32 const* level = hbb + offsets[i];
            dwordIndex       = (dwordIndex * 32) + dwordBit;
            u32 dword0       = level[dwordIndex];
            if (dword0 == 0)
                return false;
            dwordBit = (u32)math::findFirstBit(dword0);
            i += 1;
        }
        bit = (dwordIndex * 32) + dwordBit;
        return true;
    }

    bool g_hbb_upper(const hbb_hdr_t& hdr, hbb_data_t const hbb, u32 pivot, u32& bit)
    {
        if (pivot >= hdr.get_max_bits())
            return false;

        // Start at bottom level and move up finding a 'set' bit
        u32 iw = (pivot / 32);  // The index of a 32-bit word in bottom level
        u32 ib = (pivot & 31);  // The bit number in that 32-bit word
        u32 im;

        ib += 1;  // Search next bit and beyond
        if (ib == 32)
        {
            iw += 1;
            ib = 0;
            im = 0xffffffff;
        }
        else
        {
            im = ~((1 << ib) - 1);
        }

        s8 const ml = hdr.get_num_levels();
        s8       il = ml;

        u32 level_offsets[5] = {0, 1, 0, 0, 0};
        u32 level_numdwords  = (hdr.get_max_bits() + 31) >> 5;
        level_offsets[il] = hdr.m_offset;
        while (il >= 0 && il <= ml)
        {
            u32 const* level = hbb + level_offsets[il];
            u32        w     = level[iw] & im;
            if (w != 0)
            {
                iw = (iw * 32) + (u32)math::findFirstBit(w);
                if (il == ml)
                {
                    bit = iw;
                    return true;
                }
                else
                {
                    // Go down
                    il += 1;
                    ib = 0;
                    im = 0xffffffff;
                }
            }
            else
            {
                // Go up a level and move one unit in the direction of upper
                il -= 1;
                ib = (iw & 31);
                iw = (iw / 32);
                ib += 1;
                if (ib == 32)
                {
                    iw += 1;
                    ib = 0;
                }
                im = ~((1 << ib) - 1);

                level_numdwords = (level_numdwords + 31) >> 5;
                level_offsets[il] = level_offsets[il + 1] - level_numdwords;
            }
        };
        return false;
    }

    bool g_hbb_lower(const hbb_hdr_t& hdr, hbb_data_t const hbb, u32 pivot, u32& bit)
    {
        if (pivot >= hdr.get_max_bits())
            return false;

        // Start at bottom level and move up finding a 'set' bit
        u32 iw = (pivot / 32);  // The index of a 32-bit word in level 0
        u32 ib = (pivot & 31);  // The bit number in that 32-bit word
        u32 im;
        if (ib == 0)
        {
            iw -= 1;
            im = 0xffffffff;
        }
        else
        {
            im = ((1 << ib) - 1);
        }

        s8 const ml = hdr.get_num_levels();
        s8       il = ml;

        u32 level_offsets[5] = {0, 1, 0, 0, 0};
        u32 level_numdwords  = (hdr.get_max_bits() + 31) >> 5;
        level_offsets[il] = hdr.m_offset; 
        while (il >= 0 && il <= ml)
        {
            u32 const* level = hbb + level_offsets[il];
            u32        w     = level[iw] & im;
            if (w != 0)
            {
                iw = (iw * 32) + (u32)math::findFirstBit(w);
                if (il == ml)
                {
                    bit = iw;
                    return true;
                }
                else
                {
                    // Go down
                    il += 1;
                    im = 0xffffffff;
                }
            }
            else
            {
                // Go up a level and move one unit in the direction of lower
                il -= 1;
                ib = (iw & 31);
                iw = (iw / 32);
                if (ib == 0)
                {
                    iw -= 1;
                    im = 0xffffffff;
                }
                else
                {
                    im = ((1 << ib) - 1);
                }

                level_numdwords = (level_numdwords + 31) >> 5;
                level_offsets[il] = level_offsets[il + 1] - level_numdwords;
            }
        };
        return false;
    }

    hbb_iter_t g_hbb_iterator(const hbb_hdr_t& hdr, hbb_data_t hbb, u32 start, u32 end)
    {
        hbb_iter_t iter;
        iter.m_cur = start;
        iter.m_end = end;
        iter.m_hbb = hbb;
        iter.m_hdr = hdr;

        // Find the first set bit, starting from 'start'
        if (!g_hbb_is_set(hdr, hbb, start))
        {
            if (!g_hbb_upper(iter.m_hdr, iter.m_hbb, start, iter.m_cur))
            {
                iter.m_cur = end;
            }
        }
        return iter;
    }

    void hbb_iter_t::next()
    {
        if (!g_hbb_upper(m_hdr, m_hbb, m_cur, m_cur))
            m_cur = m_end;
    }

};  // namespace ncore
