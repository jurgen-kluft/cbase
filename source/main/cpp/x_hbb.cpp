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
        u16 m_numlevels;
        // u16   m_offset_level0;
        // u16   m_offset_level1;
        u16 m_offset_level[3];

        inline u32* get_level_ptr(u8 level)
        {
            u32 offset;
            switch (level)
            {
                case 0: offset = 0; break;
                case 1: offset = 1; break;
                case 2:
                case 3:
                case 4: offset = m_offset_level[level - 2]; break;
                default: ASSERT(false); break;
            }
            return ((u32*)this) + offset + (sizeof(hbb_header_t) / sizeof(u32));
        }
    };

    // level 3 = (15001 + 31) / 32 = 469 * 32 = 15008 - 15001 = rest 7
    // level 2 = (469 + 31) / 32 = 15 * 32 = 480 - 469 = rest 11
    // level 1 = (15 + 31) / 32 = 1 * 32 = 32 - 15 = rest 17
    // level 0 = 1
    u32 g_sizeof_hbb(u32 maxbits)
    {
        ASSERT(maxbits <= (1 << 25));
        s32 number_of_levels = 1;
        s32 total_num_dwords = 1;
        u32 level_num_dwords = ((maxbits + 31) / 32);
        while (level_num_dwords > 1)
        {
            total_num_dwords += level_num_dwords;
            number_of_levels += 1;
            u32 const rest   = (((level_num_dwords + 31) / 32) * 32) - level_num_dwords;
            level_num_dwords = ((level_num_dwords + 31) / 32);
        }
        return (sizeof(hbb_header_t) / sizeof(u32)) + total_num_dwords;
    }

    static u32 s_hbb_init(hbb_header_t* hdr, u32 maxbits)
    {
        ASSERT(maxbits <= (1 << 25));
        hdr->m_maxbits = maxbits;

        s8  levels                = 0;
        u32 levels_numdwords      = 0;
        u16 numdwords_per_level[] = {0, 0, 0, 0, 0, 0};
        while (maxbits > 1)
        {
            numdwords_per_level[levels] = ((maxbits + 31) / 32);
            levels_numdwords += numdwords_per_level[levels];
            maxbits = numdwords_per_level[levels];
            levels += 1;
        }

        hdr->m_numlevels       = levels;
        hdr->m_offset_level[0] = (levels > 2) ? numdwords_per_level[levels - 3] : 0;
        hdr->m_offset_level[1] = (levels > 3) ? numdwords_per_level[levels - 4] : 0;
        hdr->m_offset_level[2] = (levels > 4) ? numdwords_per_level[levels - 5] : 0;

        return levels_numdwords;
    }

    void g_hbb_init(hbb_t hbb, u32 maxbits, s8 bits)
    {
        hbb_header_t* hdr     = (hbb_header_t*)hbb;
        u32 const     ndwords = s_hbb_init(hdr, maxbits);

        u32 fill = (bits != 0) ? 0xFFFFFFFF : 0;
        x_memset((u8*)hbb + sizeof(hbb_header_t), fill, ndwords * sizeof(u32));
        if (bits != 0)
        { // set the rest part of each level to '0'
            s8 const maxlevel = hdr->m_numlevels;
            s16      level    = 0;
            while (level < maxlevel)
            {
                u32 const numdwords = ((maxbits + 31) / 32);
                u32*      plevel    = hdr->get_level_ptr(level);
                plevel              = plevel + (numdwords - 1); // goto last dword

                // compute mask
                u32 const mask = 0xFFFFFFFF >> ((numdwords * 32) - maxbits);
                *plevel        = *plevel & mask;
                maxbits        = numdwords;
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

    void g_hbb_set(hbb_t hbb, u32 bit)
    {
        // set bit in full level, then avalanche up if necessary
        hbb_header_t* hdr = (hbb_header_t*)hbb;
        s8            i   = hdr->m_numlevels - 1;
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
        s8            i   = hdr->m_numlevels - 1;
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
        s8 const      maxlevel = hdr->m_numlevels;

        // Start at top level and find a '1' bit and move down
        u32 dwordIndex = 0;
        u32 dwordBit   = 0;
        s32 i          = 0;
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
        // Start at bottom level and move up finding a 'set' bit
        u32 iw = (pivot / 32); // The index of a 32-bit word in level 0
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

        hbb_header_t* hdr = (hbb_header_t*)hbb;
        s32           il  = hdr->m_numlevels - 1;
        while (il >= 0)
        {
            u32 const* level = hdr->get_level_ptr(il);
            u32        w     = level[iw] & im;
            if (w != 0)
            {
                if (il == 0)
                {
                    bit = (iw * 32) + xfindFirstBit(w);
                    return true;
                }
                else
                {
                    // Go down
                    il += 1;
                    iw = (iw * 32) + xfindFirstBit(w);
                    ib = 0;
                    im = 0xffffffff;
                }
            }
            else
            {
                // Go up a level and move one unit in the direction of upper
                il -= 1;
                iw = (iw / 32);
                ib = (iw & 31);
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
        if (pivot == 0)
            return false;

        // Start at bottom level and move up finding a 'set' bit
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

        hbb_header_t* hdr = (hbb_header_t*)hbb;
        s32           il  = hdr->m_numlevels - 1;
        while (il >= 0)
        {
            u32 const* level = hdr->get_level_ptr(il);
            u32        w     = level[iw] & im;
            if (w != 0)
            {
                if (il == 0)
                {
                    bit = (iw * 32) + xfindFirstBit(w);
                    return true;
                }
                else
                {
                    // Go down
                    il += 1;
                    iw = (iw * 32) + xfindFirstBit(w);
                    im = 0xffffffff;
                }
            }
            else
            {
                // Go up a level and move one unit in the direction of lower
                il -= 1;
                iw = (iw / 32);
                ib = (iw & 31);
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
        iter.m_end = end;
        iter.m_cur = start;
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
        u32 index;
        if (!g_hbb_upper(m_hbb, m_cur, index))
        {
            m_cur = m_end;
        }
        else
        {
            m_cur = index;
        }
    }

}; // namespace xcore
