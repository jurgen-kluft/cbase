#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_memory.h"

#include "xbase/x_hbb.h"

namespace xcore
{
    enum
    {
        AllBitsSet = 0xffffffff
    };

    // level n -> max-offset (max bits at that level)
    // level 0 -> 0          (32 bits)
    // level 1 -> 1          (1024 bits)
    // level 2 -> 33         (32768 bits)
    // level 3 -> 1057       (1048576 bits)
    // level 4 -> 33825      (33554432 bits)

    struct hbb_header_t
    {
        u32 m_maxbits;
        s16 m_numlevels;
        // u16   m_offset_level0;
        // u16   m_offset_level1;

        // if levels == 1 or levels == 2 then 'u16 m_offset_level[1]'
        u16 m_offset_level[3];

        inline u32* get_level_ptr(s16 level)
        {
            u32 offset;
            switch (level)
            {
                case 0: offset = 2 + 0; break;
                case 1: offset = 2 + 1; break;
                case 2:
                case 3:
                case 4: offset = m_offset_level[level - 2]; break;
                default: ASSERT(false); break;
            }
            return ((u32*)this) + offset;
        }
    };

    u32 g_sizeof_hbb(u32 maxbits)
    {
        ASSERT(maxbits <= (1 << 25));
        s32 total_num_dwords = 1;
        u32 level_num_dwords = ((maxbits + 31) / 32);
        s32 levels           = 1;
        while (level_num_dwords > 1)
        {
            total_num_dwords += level_num_dwords;
            level_num_dwords = ((level_num_dwords + 31) / 32);
            levels += 1;
        }
        switch (levels)
        {
            case 5: // fall through
            case 4: total_num_dwords += 3; break;
            case 3: total_num_dwords += 2; break;
            case 2: // fall through
            case 1: total_num_dwords += 2; break;
            default: ASSERT(false); break;
        }
        return total_num_dwords;
    }

    static u32 s_hbb_init(hbb_header_t* hdr, u32 maxbits)
    {
        ASSERT(maxbits <= (1 << 25));
        hdr->m_maxbits = maxbits;

        s8  levels           = 0;
        u32 levels_numdwords = 0;
        u16 size_per_level[] = {0, 0, 0, 0, 0, 0};
        while (maxbits > 1)
        {
            size_per_level[levels] = ((maxbits + 31) / 32);
            levels_numdwords += size_per_level[levels];
            maxbits = size_per_level[levels];
            levels += 1;
        }

        hdr->m_numlevels = levels;
        if (levels > 2)
        {
            hdr->m_offset_level[0] = (size_per_level[levels - 2] + 1 + 2);
            if (levels > 3)
            {
                hdr->m_offset_level[1] = (hdr->m_offset_level[0] + size_per_level[levels - 3]) + 1;
                if (levels > 4)
                    hdr->m_offset_level[2] = (hdr->m_offset_level[1] + size_per_level[levels - 4]);
            }
        }
        else
        {
            hdr->m_offset_level[0] = 0;
        }
        return levels_numdwords;
    }

    static void s_hbb_mask(hbb_header_t* hdr)
    {
        u32       maxbits  = hdr->m_maxbits;
        s16 const maxlevel = hdr->m_numlevels;
        s16       level    = maxlevel - 1;
        while (level >= 0)
        {
            u32 const numdwords = ((maxbits + 31) / 32);
            u32*      plevel    = hdr->get_level_ptr(level);
            plevel              = plevel + (numdwords - 1); // goto last dword

            // compute mask
            u32 const mask = 0xFFFFFFFF >> ((numdwords * 32) - maxbits);
            *plevel        = *plevel & mask;
            maxbits        = numdwords;
            level -= 1;
        }
    }

    void g_hbb_init(hbb_t hbb, u32 maxbits, s8 bits)
    {
        hbb_header_t* hdr     = (hbb_header_t*)hbb;
        u32 const     ndwords = s_hbb_init(hdr, maxbits);
        if (bits >= 0)
        {
            u32 fill = (bits != 0) ? 0xFFFFFFFF : 0;
            x_memset(hdr->get_level_ptr(0), fill, ndwords * sizeof(u32));
            if (bits != 0)
            {
                s_hbb_mask(hdr);
            }
        }
    }

    void g_hbb_init(hbb_t& hbb, u32 maxbits, s8 bits, alloc_t* alloc)
    {
        u32 const ndwords = g_sizeof_hbb(maxbits);
        hbb               = (u32*)alloc->allocate(ndwords * sizeof(u32), sizeof(u32));
        g_hbb_init(hbb, maxbits, bits);
    }

    void g_hbb_release(hbb_t& hbb, alloc_t* alloc)
    {
        if (alloc != nullptr)
        {
            alloc->deallocate(hbb);
            hbb = nullptr;
        }
    }

    void g_hbb_reset(hbb_t hbb, s8 bits)
    {
        hbb_header_t* hdr = (hbb_header_t*)hbb;
        g_hbb_init(hbb, hdr->m_maxbits, bits);
    }

    void g_hbb_resize(hbb_t& hbb, u32 maxbits, s8 bits, alloc_t* alloc)
    {
        if (hbb == nullptr)
        {
            g_hbb_init(hbb, maxbits, bits, alloc);
        }
        else
        {
            hbb_header_t* src = (hbb_header_t*)hbb;
            if (maxbits > src->m_maxbits)
            {
                hbb_t h = nullptr;
                g_hbb_init(h, maxbits, -1, alloc);

                u32 const fill = (bits != 0) ? 0xFFFFFFFF : 0;

                // Copy the level data
                hbb_header_t* dst         = (hbb_header_t*)h;
                s16           dst_level   = dst->m_numlevels - 1;
                s16           src_level   = src->m_numlevels - 1;
                u32           dst_maxbits = dst->m_maxbits;
                u32           src_maxbits = src->m_maxbits;
                while (src_level >= 0)
                {
                    // Number of words of the src level
                    u32 const  srcndwords = (src_maxbits + 31) / 32;
                    u32 const  dstndwords = (dst_maxbits + 31) / 32;
                    u32 const* srcdata    = src->get_level_ptr(src_level);
                    u32*       dstdata    = dst->get_level_ptr(dst_level);
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
                        u32*      dstdata    = dst->get_level_ptr(dst_level);
                        for (u32 i = 0; i < dstndwords; ++i)
                            dstdata[i] = fill;
                        dst_maxbits = dstndwords;
                        dst_level -= 1;
                    }

                    if (fill != 0)
                    {
                        s_hbb_mask(dst);
                    }

                    u32 const* srcdata = src->get_level_ptr(0);
                    dst_level          = (dst->m_numlevels - src->m_numlevels) - 1;
                    if (*srcdata == 0)
                    {
                        while (dst_level >= 0)
                        {
                            u32* dstdata = dst->get_level_ptr(dst_level);
                            *dstdata &= 0xFFFFFFFE;
                            dst_level -= 1;
                        }
                    }
                    else
                    {
                        // hmmm, we need to propogate up
                        while (dst_level >= 0)
                        {
                            u32* dstdata = dst->get_level_ptr(dst_level);
                            *dstdata |= 0x00000001;
                            dst_level -= 1;
                        }
                    }
                }

                g_hbb_release(hbb, alloc);
                hbb = h;
            }
            else if (maxbits < src->m_maxbits)
            {
                // TODO: Hmmm, we need to shrink, how do we do that?
            }
        }
    }

    void g_hbb_set(hbb_t hbb, u32 bit)
    {
        // set bit in full level, then avalanche up if necessary
        hbb_header_t* hdr = (hbb_header_t*)hbb;
        s16           i   = hdr->m_numlevels - 1;
        while (i >= 0)
        {
            u32*      level      = hdr->get_level_ptr(i);
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

    void g_hbb_clr(hbb_t hbb, u32 bit)
    {
        // clear bit in level 0, then avalanche up if necessary
        hbb_header_t* hdr = (hbb_header_t*)hbb;
        s16           i   = hdr->m_numlevels - 1;
        while (i >= 0)
        {
            u32*      level      = hdr->get_level_ptr(i);
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

    bool g_hbb_is_set(hbb_t hbb, u32 bit)
    {
        hbb_header_t* hdr        = (hbb_header_t*)hbb;
        u32 const*    level      = hdr->get_level_ptr(hdr->m_numlevels - 1);
        u32 const     dwordIndex = bit / 32;
        u32 const     dwordBit   = bit & 31;
        return ((level[dwordIndex] >> dwordBit) & 1) == 1;
    }

    bool g_hbb_is_empty(hbb_t hbb)
    {
        u32 const* level0 = hbb;
        return level0[0] == 0;
    }

    bool g_hbb_find(hbb_t hbb, u32& bit)
    {
        hbb_header_t* hdr      = (hbb_header_t*)hbb;
        s16 const     maxlevel = hdr->m_numlevels;

        // Start at top level and find a '1' bit and move down
        u32 dwordIndex = 0;
        u32 dwordBit   = 0;
        s16 i          = 0;
        while (i < maxlevel)
        {
            u32 const* level = hdr->get_level_ptr(i);
            dwordIndex       = (dwordIndex * 32) + dwordBit;
            u32 dword0       = level[dwordIndex];
            if (dword0 == 0)
                return false;
            dwordBit = xfindFirstBit(dword0);
            i += 1;
        }
        bit = (dwordIndex * 32) + dwordBit;
        return true;
    }

    bool g_hbb_upper(hbb_t hbb, u32 pivot, u32& bit)
    {
        hbb_header_t* hdr = (hbb_header_t*)hbb;
        if (pivot >= hdr->m_maxbits)
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

        s16 il = hdr->m_numlevels - 1;
        while (il >= 0)
        {
            u32 const* level = hdr->get_level_ptr(il);
            u32        w     = level[iw] & im;
            if (w != 0)
            {
                iw = (iw * 32) + xfindFirstBit(w);
                if (il == (hdr->m_numlevels - 1))
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

    bool g_hbb_lower(hbb_t hbb, u32 pivot, u32& bit)
    {
        hbb_header_t* hdr = (hbb_header_t*)hbb;
        if (pivot >= hdr->m_maxbits)
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

        s16 il = hdr->m_numlevels - 1;
        while (il >= 0)
        {
            u32 const* level = hdr->get_level_ptr(il);
            u32        w     = level[iw] & im;
            if (w != 0)
            {
                iw = (iw * 32) + xfindFirstBit(w);
                if (il == (hdr->m_numlevels - 1))
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

    hbb_iter_t g_hbb_iterator(hbb_t hbb, u32 start, u32 end)
    {
        hbb_iter_t iter;
        iter.m_cur = start;
        iter.m_end = end;
        iter.m_hbb = hbb;

        // Find the first set bit, starting from 'start'
        if (!g_hbb_is_set(hbb, start))
        {
            if (!g_hbb_upper(iter.m_hbb, start, iter.m_cur))
            {
                iter.m_cur = end;
            }
        }
        return iter;
    }

    void hbb_iter_t::next()
    {
        if (!g_hbb_upper(m_hbb, m_cur, m_cur))
            m_cur = m_end;
    }

}; // namespace xcore
