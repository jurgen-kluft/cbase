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

    static inline u32 get_level_offset(hbb_hdr_t const& hdr, s8 level)
    {
        switch (level)
        {
            case 0: return 0; break;
            case 1: return 1; break;
            case 2: return (hdr.m_offset32 & 0x1f) + 2; break;
            case 3: return (hdr.m_offset32 >> 5); break;
            case 4: return hdr.m_offset4; break;
            default: break;
        }
        return 0xffffffff;
    }

    static u32 s_hbb_init(hbb_hdr_t& hdr, u32 maxbits)
    {
        ASSERT(maxbits <= (1 << 25));
        hdr.m_maxbits_numlevels = maxbits << 4;

        s8  levels           = 0;
        u16 size_per_level[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        while (maxbits > 1)
        {
            maxbits = ((maxbits + 31) / 32);
            size_per_level[5 + levels] = maxbits; // simulate we have 5 levels
            levels += 1;
        }
        for (s32 i = 5 + levels - 1; i >= 5; --i)
            size_per_level[i] += size_per_level[i + 1];

        hdr.m_maxbits_numlevels |= (levels & 0xf);
        hdr.m_offset32 = ((size_per_level[levels + 2]) << 5) | ((size_per_level[levels + 3] - 2) & 0x1f);
        hdr.m_offset4  = size_per_level[levels + 1];
        return size_per_level[5];
    }

    static void s_hbb_mask(hbb_hdr_t const& hdr, hbb_data_t hbb)
    {
        u32      maxbits  = hdr.get_max_bits();
        s8 const maxlevel = hdr.get_num_levels();
        s8       level    = maxlevel - 1;
        while (level >= 0)
        {
            u32 const numdwords = ((maxbits + 31) / 32);
            u32*      plevel    = hbb + get_level_offset(hdr, level);
            plevel              = plevel + (numdwords - 1); // goto last dword

            // compute mask
            u32 const mask = 0xFFFFFFFF >> ((numdwords * 32) - maxbits);
            *plevel        = *plevel & mask;
            maxbits        = numdwords;
            level -= 1;
        }
    }

    u32 g_hbb_sizeof_data(u32 maxbits)
    {
        hbb_hdr_t hdr;
        return g_hbb_init(hdr, maxbits);
    }

    u32 g_hbb_init(hbb_hdr_t& hdr, u32 maxbits) { return s_hbb_init(hdr, maxbits); }

    void g_hbb_init(const hbb_hdr_t& hdr, hbb_data_t hbb, s8 bits)
    {
        const u32 ndwords = hdr.get_num_dwords();
        if (bits >= 0)
        {
            u32 const fill = (bits != 0) ? 0xFFFFFFFF : 0;
            u32*      data = hbb + get_level_offset(hdr, 0);
            for (u32 i = 0; i < ndwords; ++i)
                data[i] = fill;

            if (bits != 0)
            {
                s_hbb_mask(hdr, hbb);
            }
        }
    }

    void g_hbb_init(hbb_hdr_t const& hdr, hbb_data_t& hbb, s8 bits, alloc_t* alloc)
    {
        const u32 ndwords = hdr.get_num_dwords();
        hbb               = (u32*)alloc->allocate(ndwords * sizeof(u32), sizeof(u32));
        g_hbb_init(hdr, hbb, bits);
    }

    void g_hbb_release(hbb_data_t& hbb, alloc_t* alloc)
    {
        if (alloc != nullptr)
        {
            alloc->deallocate(hbb);
            hbb = nullptr;
        }
    }

    void g_hbb_reset(const hbb_hdr_t& hdr, hbb_data_t hbb, s8 bits) { g_hbb_init(hdr, hbb, bits); }

    void g_hbb_resize(hbb_hdr_t& hdr, hbb_data_t& hbb, u32 maxbits, s8 bits, alloc_t* alloc)
    {
        if (hbb == nullptr)
        {
            g_hbb_init(hdr, hbb, bits, alloc);
        }
        else
        {
            if (maxbits > hdr.get_max_bits())
            {
                hbb_hdr_t src = hdr;

                hbb_data_t h = nullptr;
                g_hbb_init(hdr, maxbits);
                g_hbb_init(hdr, h, -1, alloc);

                u32 const fill = (bits != 0) ? 0xFFFFFFFF : 0;

                // Copy the level data
                s8  dst_level   = hdr.get_num_levels() - 1;
                s8  src_level   = src.get_num_levels() - 1;
                u32 dst_maxbits = hdr.get_max_bits();
                u32 src_maxbits = src.get_max_bits();
                while (src_level >= 0)
                {
                    // Number of words of the src level
                    u32 const  srcndwords = (src_maxbits + 31) / 32;
                    u32 const  dstndwords = (dst_maxbits + 31) / 32;
                    u32 const* srcdata    = hbb + get_level_offset(src, src_level);
                    u32*       dstdata    = h + get_level_offset(hdr, dst_level);
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

                // Now it can happen that dst has more levels than src
                if (dst_level >= 0)
                {
                    while (dst_level >= 0)
                    {
                        u32 const dstndwords = (dst_maxbits + 31) / 32;
                        u32*      dstdata    = h + get_level_offset(hdr, dst_level);
                        for (u32 i = 0; i < dstndwords; ++i)
                            dstdata[i] = fill;
                        dst_maxbits = dstndwords;
                        dst_level -= 1;
                    }

                    if (fill != 0)
                    {
                        s_hbb_mask(hdr, h);
                    }

                    u32 const* srcdata = hbb + get_level_offset(src, 0);
                    dst_level          = (hdr.get_num_levels() - src.get_num_levels()) - 1;
                    if (*srcdata == 0)
                    {
                        while (dst_level >= 0)
                        {
                            u32* dstdata = h + get_level_offset(hdr, dst_level);
                            *dstdata &= 0xFFFFFFFE;
                            dst_level -= 1;
                        }
                    }
                    else
                    {
                        // hmmm, we need to propogate up
                        while (dst_level >= 0)
                        {
                            u32* dstdata = h + get_level_offset(hdr, dst_level);
                            *dstdata |= 0x00000001;
                            dst_level -= 1;
                        }
                    }
                }

                g_hbb_release(hbb, alloc);
                hbb = h;
            }
            else if (maxbits < hdr.get_max_bits())
            {
                // TODO: Hmmm, we need to shrink, how do we do that?
            }
        }
    }

    void g_hbb_set(const hbb_hdr_t& hdr, hbb_data_t hbb, u32 bit)
    {
        // set bit in full level, then avalanche up if necessary
        s8 i = hdr.get_num_levels() - 1;
        while (i >= 0)
        {
            u32*      level      = hbb + get_level_offset(hdr, i);
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
        }
    }

    void g_hbb_clr(const hbb_hdr_t& hdr, hbb_data_t hbb, u32 bit)
    {
        // clear bit in level 0, then avalanche up if necessary
        s8 i = hdr.get_num_levels() - 1;
        while (i >= 0)
        {
            u32*      level      = hbb + get_level_offset(hdr, i);
            u32 const dwordIndex = bit / 32;
            u32 const dwordBit   = 1 << (bit & 31);
            u32 const dword0     = level[dwordIndex];
            u32 const dword1     = dword0 & ~dwordBit;

            level[dwordIndex] = dword1;

            bool const avalanche = (dword0 != dword1 && dword1 == 0);
            if (!avalanche)
                break;

            i -= 1;
            bit = bit >> 5;
        }
    }

    bool g_hbb_is_set(const hbb_hdr_t& hdr, hbb_data_t const hbb, u32 bit)
    {
        u32 const* level      = hbb + get_level_offset(hdr, hdr.get_num_levels() - 1);
        u32 const  dwordIndex = bit / 32;
        u32 const  dwordBit   = bit & 31;
        return ((level[dwordIndex] >> dwordBit) & 1) == 1;
    }

    bool g_hbb_is_empty(const hbb_hdr_t& hdr, hbb_data_t const hbb)
    {
        u32 const* level0 = hbb + get_level_offset(hdr, 0);
        return level0[0] == 0;
    }

    bool g_hbb_find(const hbb_hdr_t& hdr, hbb_data_t const hbb, u32& bit)
    {
        s8 const maxlevel = hdr.get_num_levels();

        // Start at top level and find a '1' bit and move down
        u32 dwordIndex = 0;
        u32 dwordBit   = 0;
        s8  i          = 0;
        while (i < maxlevel)
        {
            u32 const* level = hbb + get_level_offset(hdr, i);
            dwordIndex       = (dwordIndex * 32) + dwordBit;
            u32 dword0       = level[dwordIndex];
            if (dword0 == 0)
                return false;
            dwordBit = math::findFirstBit(dword0);
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
        u32 iw = (pivot / 32); // The index of a 32-bit word in bottom level
        u32 ib = (pivot & 31); // The bit number in that 32-bit word
        u32 im;

        ib += 1; // Search next bit and beyond
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

        s8 il = hdr.get_num_levels() - 1;
        while (il >= 0)
        {
            u32 const* level = hbb + get_level_offset(hdr, il);
            u32        w     = level[iw] & im;
            if (w != 0)
            {
                iw = (iw * 32) + math::findFirstBit(w);
                if (il == (hdr.get_num_levels() - 1))
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
            }
        };
        return false;
    }

    bool g_hbb_lower(const hbb_hdr_t& hdr, hbb_data_t const hbb, u32 pivot, u32& bit)
    {
        if (pivot >= hdr.get_max_bits())
            return false;

        // Start at bottom level and move up leve finding a 'set' bit
        u32 iw = (pivot / 32); // The index of a 32-bit word in level 0
        u32 ib = (pivot & 31); // The bit number in that 32-bit word
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

        s8 il = hdr.get_num_levels() - 1;
        while (il >= 0)
        {
            u32 const* level = hbb + get_level_offset(hdr, il);
            u32        w     = level[iw] & im;
            if (w != 0)
            {
                iw = (iw * 32) + math::findFirstBit(w);
                if (il == (hdr.get_num_levels() - 1))
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

}; // namespace ncore
