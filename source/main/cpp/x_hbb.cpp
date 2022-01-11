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

    u32 sizeof_level(hbb_t hbb, u32 config, s8 level)
    {
        if (level == 0)
            return 1;

        ASSERT(level < (s8)(config & 0x1F));
        s32 size = 1;
        while (level >= 1)
        {
            config = config >> 5;
            size   = size * 32 - (config & 0x1F);
            level -= 1;
        }
        return size;
    }

	static inline u32 s_level_bits(u32 config, s8 level)
	{
		config = config >> 5;
		s32 numbits = 32 - (config & 0x1F);
		while (level > 0)
		{
			config = config >> 5;
			numbits = (numbits << 5) - (config & 0x1F);
			level -= 1;
		}
		return numbits;
	}

    static u32* s_level_ptr(hbb_t hbb, u32 config, s8 level)
    {
        if (level <= 1)
            return hbb.m_hbb + level;

        ASSERT(level < (s8)(config & 0x1F));
        s32 size   = 1;
        s32 offset = 1;
        while (level > 1)
        {
            config = config >> 5;
            size   = size * 32 - (config & 0x1F);
            offset += size;
            level -= 1;
        }
        return hbb.m_hbb + offset;
    }

    // level 3 = (15001 + 31) / 32 = 469 * 32 = 15008 - 15001 = rest 7
    // level 2 = (469 + 31) / 32 = 15 * 32 = 480 - 469 = rest 11
    // level 1 = (15 + 31) / 32 = 1 * 32 = 32 - 15 = rest 17
    // level 0 = 1
    u32 sizeof_hbb(u32 maxbits, u32& config)
    {
		ASSERT(maxbits <= (1<<25));
        config               = (((((maxbits + 31) / 32) * 32) - maxbits) & 0x1F);
        s32 number_of_levels = 1;
        s32 total_num_dwords = 1;
        u32 level_num_dwords = ((maxbits + 31) / 32);
        while (level_num_dwords > 1)
        {
            total_num_dwords += level_num_dwords;
            number_of_levels += 1;
            u32 const rest   = (((level_num_dwords + 31) / 32) * 32) - level_num_dwords;
            config           = (config << 5) | (rest & 0x1F);
            level_num_dwords = ((level_num_dwords + 31) / 32);
        }
        config = (config << 5) | (number_of_levels & 0x1F);
        return total_num_dwords;
    }

    void init(hbb_t hbb, u32 maxbits, u32 config)
    {
        // set the rest part of each level to '0'
        s8 const maxlevel = (config & 0x1F);

        for (s32 i = maxlevel - 1; i >= 0; --i)
        {
            u32 const numdwords = sizeof_level(hbb, config, i);
            u32*      plevel    = s_level_ptr(hbb, config, i);
            plevel              = plevel + (numdwords - 1); // goto last dword

            // compute mask
            u32 const mask = 0xFFFFFFFF >> ((numdwords * 32) - maxbits);
            *plevel        = *plevel & mask;
            maxbits        = numdwords;
        }
    }

    void init(hbb_t& hbb, u32 maxbits, u32& config, s8 bits, alloc_t* alloc)
    {
        u32 const ndwords = sizeof_hbb(maxbits, config);
        hbb.m_hbb         = (u32*)alloc->allocate(ndwords * 4, sizeof(u32));
        x_memset(hbb.m_hbb, bits == 0 ? 0 : 0xFFFFFFFF, ndwords * 4);
        if (bits == 1)
        {
            init(hbb, config, maxbits);
        }
    }

    void release(hbb_t& hbb, alloc_t* alloc)
    {
        if (alloc != nullptr)
        {
            alloc->deallocate(hbb.m_hbb);
            hbb.m_hbb = nullptr;
        }
    }

    void reset(hbb_t hbb, u32 config, u32 maxbits, s8 bits)
    {
        u32       c;
        u32 const size = sizeof_hbb(maxbits, c);
        ASSERT(c == config);
        x_memset(hbb.m_hbb, bits == 0 ? 0 : 0xFFFFFFFF, size * 4);
        if (bits == 1)
        {
            init(hbb, config, maxbits);
        }
    }

    void set(hbb_t hbb, u32 config, u32 maxbits, u32 bit)
    {
        // set bit in full level, then avalanche up if necessary
        s8 i = (config & 0x1F) - 1;
        while (i >= 0)
        {
            u32*      level      = s_level_ptr(hbb, config, i);
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

    void clr(hbb_t hbb, u32 config, u32 maxbits, u32 bit)
    {
        // clear bit in level 0, then avalanche up if necessary
        s8 i = (config & 0x1F) - 1;
        while (i >= 0)
        {
            u32*      level      = s_level_ptr(hbb, config, i);
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

    bool is_set(hbb_t hbb, u32 config, u32 maxbits, u32 bit)
    {
        u32 const* level      = s_level_ptr(hbb, config, (config & 0x1F) - 1);
        u32 const  dwordIndex = bit / 32;
        u32 const  dwordBit   = bit & 31;
        return ((level[dwordIndex] >> dwordBit) & 1) == 1;
    }

    bool is_empty(hbb_t hbb)
    {
        u32 const* level0 = hbb.m_hbb;
        return level0[0] == 0;
    }

    bool find(hbb_t hbb, u32 config, u32 maxbits, u32& bit)
    {
        s8 const maxlevel = config & 0x1F;

        // Start at top level and find a '1' bit and move down
        u32 dwordIndex = 0;
        u32 dwordBit   = 0;
        s32 i          = 0;
        while (i < maxlevel)
        {
            u32 const* level = s_level_ptr(hbb, config, i);
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

    bool upper(hbb_t hbb, u32 config, u32 maxbits, u32 pivot, u32& bit)
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

        s32 il = (config & 0x1F) - 1;
        while (il >= 0)
        {
            u32 const* level = s_level_ptr(hbb, config, il);
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

    bool lower(hbb_t hbb, u32 config, u32 maxbits, u32 pivot, u32& bit)
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

        s32 il = (config & 0x1F) - 1;
        while (il >= 0)
        {
            u32 const* level = s_level_ptr(hbb, config, il);
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

}; // namespace xcore
