#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_allocator.h"
#include "cbase/c_memory.h"
#include "cbase/c_integer.h"

#include "cbase/c_duomap.h"

namespace ncore
{
    static void clear_level(u32 level_bits, u32* level, u32 df)
    {
        u32 const w = level_bits >> 5;
        for (u32 i = 0; i < w; i++)
            level[i] = df;
    }

    binmap_t::config_t duomap_t::compute(u32 count) { return binmap_t::config_t::compute(count); }
    u32                duomap_t::sizeof_data(u32 count) { return binmap_t::config_t::sizeof_data(count); }

    void duomap_t::release(alloc_t* allocator)
    {
        // Note: We share the lowest level between both binmaps
        switch (levels())
        {
            case 3: allocator->deallocate(m_l[1]);
            case 2: allocator->deallocate(m_l[0]);
            case 1: break;
        }
        reset();

        m_binmap0.release(allocator);
    }

    void duomap_t::init_all_free()
    {
        u32 const count = size();
        config_t  cfg   = config_t::compute(count);
        m_binmap0.init_all_free(cfg, m_binmap0.m_l[0], m_binmap0.m_l[1], m_binmap0.m_l[2]);

        s8 const l = cfg.m_levels;
        switch (l)
        {
            case 3: binmap_t::clear_levelN(cfg.m_lnlen[1], m_l[1], 1, 0);
            case 2: binmap_t::clear_levelN(cfg.m_lnlen[0], m_l[0], 1, 0);
            case 1: binmap_t::clear_level0(cfg, m_l0, 1, 0);
            case 0: break;
        }
    }

    void duomap_t::init_all_used()
    {
        config_t cfg = config_t::compute(size());
        m_binmap0.init_all_used(cfg, m_binmap0.m_l[0], m_binmap0.m_l[1], m_binmap0.m_l[2]);

        // Clear our own levels but not the shared one
        s8 const l = cfg.m_levels;
        switch (l)
        {
            case 3: binmap_t::clear_levelN(cfg.m_lnlen[1], m_l[1], 1, 1);
            case 2: binmap_t::clear_levelN(cfg.m_lnlen[0], m_l[0], 1, 1);
            case 1: binmap_t::clear_level0(cfg, m_l0, 1, 1);
            case 0: break;
        }

        m_set = cfg.m_count;
    }

    void duomap_t::init_all_free(u32 count, alloc_t* allocator)
    {
        reset();

        config_t cfg = config_t::compute(count);
        switch (cfg.m_levels)
        {
            case 3: m_binmap0.m_l[2] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[2]);
            case 2: m_binmap0.m_l[1] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[1]);
            case 1: m_binmap0.m_l[0] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[0]);
        }
        m_binmap0.init_all_free(cfg, m_binmap0.m_l[0], m_binmap0.m_l[1], m_binmap0.m_l[2]);

        switch (cfg.m_levels)
        {
            case 3: m_l[1] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[1]); binmap_t::clear_levelN(cfg.m_lnlen[1], m_l[1], 1, 0);
            case 2: m_l[0] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[0]); binmap_t::clear_levelN(cfg.m_lnlen[0], m_l[0], 1, 0);
            case 1: binmap_t::clear_level0(cfg, m_l0, 1, 0);
        }
    }

    void duomap_t::init_all_used(u32 count, alloc_t* allocator)
    {
        reset();

        config_t cfg = config_t::compute(count);
        switch (cfg.m_levels)
        {
            case 3: m_binmap0.m_l[2] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[2]);
            case 2: m_binmap0.m_l[1] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[1]);
            case 1: m_binmap0.m_l[0] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[0]);
        }
        m_binmap0.init_all_used(cfg, m_binmap0.m_l[0], m_binmap0.m_l[1], m_binmap0.m_l[2]);

        switch (cfg.m_levels)
        {
            case 3: m_l[1] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[1]); binmap_t::clear_levelN(cfg.m_lnlen[1], m_l[1], 1, 1);
            case 2: m_l[0] = (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[0]); binmap_t::clear_levelN(cfg.m_lnlen[0], m_l[0], 1, 1);
            case 1: binmap_t::clear_level0(cfg, m_l0, 1, 1);
        }

        m_set = cfg.m_count;
    }

    void duomap_t::init_all_free(config_t const& cfg, u32* bm0l1, u32* bm0l2, u32* bm0l3, u32* bm1l1, u32* bm1l2)
    {
        reset();

        m_binmap0.m_l[2] = bm0l3;
        m_binmap0.m_l[1] = bm0l2;
        m_binmap0.m_l[0] = bm0l1;
        m_binmap0.init_all_free(cfg, m_binmap0.m_l[0], m_binmap0.m_l[1], m_binmap0.m_l[2]);

        m_l[1] = bm1l2;
        m_l[0] = bm1l1;
        binmap_t::clear_levelN(cfg.m_lnlen[1], m_l[1], 1, 0);
        binmap_t::clear_levelN(cfg.m_lnlen[0], m_l[0], 1, 0);
        binmap_t::clear_level0(cfg, m_l0, 1, 0);
    }

    void duomap_t::init_all_used(config_t const& cfg, u32* bm0l1, u32* bm0l2, u32* bm0l3, u32* bm1l1, u32* bm1l2)
    {
        reset();

        m_binmap0.m_l[2] = bm0l3;
        m_binmap0.m_l[1] = bm0l2;
        m_binmap0.m_l[0] = bm0l1;
        m_binmap0.init_all_used(cfg, m_binmap0.m_l[0], m_binmap0.m_l[1], m_binmap0.m_l[2]);

        m_l[1] = bm1l2;
        m_l[0] = bm1l1;
        binmap_t::clear_levelN(cfg.m_lnlen[1], m_l[1], 1, 1);
        binmap_t::clear_levelN(cfg.m_lnlen[0], m_l[0], 1, 1);
        binmap_t::clear_level0(cfg, m_l0, 1, 1);

        m_set = cfg.m_count;
    }

    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // lazy init and tick functions
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------

    void duomap_t::set_used(u32 bit)
    {
        s8 l = levels();
        if (l > 0)
        {
            u32 wdb = m_l[l][bit >> 5];
            if ((wdb & (1 << (bit & 31))) != 0)
                return;

            // We are sure we are going to set a bit
            m_set += 1;

            u32 wi = bit;
            for (--l; l >= 0; --l)
            {
                u32 const bi         = (u32)1 << (wi & 31);
                wi                   = wi >> 5;
                u32 const wd         = m_binmap0.m_l[l][wi];
                u32 const wc         = wd | bi;
                m_binmap0.m_l[l][wi] = wc;
                if (wc != 0xffffffff)  // If all bits are not set yet -> early out
                    goto do_binmap1;
            }

            m_binmap0.m_l0 = m_binmap0.m_l0 | (1 << (wi & 31));

        do_binmap1:

            if (wdb != 0)  // If all bits at the lowest level where not 0 -> early out
                return;

            // We are going one level up already
            wi = bit >> 5;
            // propagate the set bit to the upper levels
            for (l = levels() - 2; l >= 0; --l)
            {
                u32 const bi = (u32)1 << (wi & 31);
                wi           = wi >> 5;
                u32 wd       = m_l[l][wi];
                m_l[l][wi]   = wd | bit;
                if (wd != 0)  // If all bits where not 0 -> early out
                    return;
            }

            m_l0 = m_l0 | (1 << (wi & 31));
        }
        else
        {
            u32 const bi = (u32)1 << (bit & 31);
            u32 const l0 = m_l0 | bi;
            if (l0 == m_l0)
                return;
            m_l0           = l0;
            m_binmap0.m_l0 = m_binmap0.m_l0 | (1 << (bit & 31));
            m_set += 1;
        }
    }

    void duomap_t::set_free(u32 bit)
    {
        s8 l = levels();
        if (l > 0)
        {
            u32 const wdb = m_l[l][bit >> 5];
            if ((wdb & (1 << (bit & 31))) == 0)
                return;

            // We are sure we are going to clear a bit
            m_set -= 1;

            u32 wi = bit;
            for (--l; l >= 0; --l)
            {
                u32 const bi         = (u32)1 << (wi & 31);
                wi                   = wi >> 5;
                u32 const wd         = m_binmap0.m_l[l][wi];
                m_binmap0.m_l[l][wi] = wd & ~bi;
                if (wd != 0xffffffff)  // If not all bits where set before  -> early out
                    goto do_binmap1;
            }

            m_binmap0.m_l0 = m_binmap0.m_l0 & ~(1 << (wi & 31));

        do_binmap1:

            if (wdb != 0)  // If at the lowest level we still have some bits set -> early out
                return;

            // We are going one level up already
            wi = bit >> 5;
            // propagate the bit to the upper levels
            for (l = levels() - 2; l >= 0; --l)
            {
                u32 const bi = (u32)1 << (wi & 31);
                wi           = wi >> 5;
                u32 const wd = m_l[l][wi] & ~bi;
                m_l[l][wi]   = wd;
                if (wd != 0)  // If there are still bits set -> early out
                    return;
            }

            m_l0 = m_l0 & ~(1 << (wi & 31));
        }
        else
        {
            u32 const bi = (u32)1 << (bit & 31);
            u32 const l0 = m_l0 & ~bi;
            if (l0 == m_l0)
                return;
            m_l0           = l0;
            m_binmap0.m_l0 = m_binmap0.m_l0 & ~bi;
            m_set -= 1;
        }
    }

    bool duomap_t::get(u32 bit) const { return m_binmap0.get(bit); }

    s32 duomap_t::find_free() const { return m_binmap0.find(); }
    s32 duomap_t::find_free_and_set_used()
    {
        s32 const bit = m_binmap0.find();
        if (bit >= 0)
            set_used(bit);
        return bit;
    }

    s32 duomap_t::find_free_upper() const { return m_binmap0.find_upper(); }
    s32 duomap_t::find_free_upper_and_set_used()
    {
        s32 const bit = m_binmap0.find_upper_and_set();
        if (bit < 0)
            return -1;

        // We found a '0' bit in binmap0 and we have set it to '1', we need to
        // track this '1' bit here.
        m_set += 1;

        // check the word the bit was set in, if that word was 0 we need to propagate it up
        s8 l = levels();
        if (l > 0)
        {
            u32 wdb = m_l[l][bit >> 5] & ~(1 << (bit & 31));
            if (wdb != 0)  // If all bits at the lowest level where not 0 -> early out
                return bit;

            // We are going one level up already
            u32 wi = bit >> 5;
            // propagate the set bit to the upper levels
            for (l = levels() - 2; l >= 0; --l)
            {
                u32 const bi = (u32)1 << (wi & 31);
                wi           = wi >> 5;
                u32 wd       = m_l[l][wi];
                m_l[l][wi]   = wd | bit;
                if (wd != 0)  // If all bits where not 0 -> early out
                    return bit;
            }

            m_l0 = m_l0 | (1 << (wi & 31));
        }
        else
        {
            m_l0 = m_l0 | (1 << (bit & 31));
        }

        return bit;
    }

    s32 duomap_t::upper_free(u32 pivot) const { return m_binmap0.upper(pivot); }
    s32 duomap_t::lower_free(u32 pivot) const { return m_binmap0.lower(pivot); }

    s32 duomap_t::find_used() const
    {
        if (m_l0 == 0)
            return -1;

        s8       i  = 0;
        s8 const l  = levels() - 1;
        u32      wi = 0;
        s8       bi = math::findFirstBit(m_l0);
        ASSERT(bi >= 0 && bi < 32);

        for (; i <= l; ++i)
        {
            wi = (wi << 5) + bi;
            bi = (i < l) ? math::findFirstBit(m_l[i][wi]) : math::findFirstBit(m_binmap0.m_l[i][wi]);
            ASSERT(bi >= 0 && bi < 32);
        }

        u32 const bit = (wi << 5) + bi;
        return (bit < size()) ? bit : -1;
    }

    s32 duomap_t::find_used_and_set_free()
    {
        s32 const bit = find_used();
        if (bit >= 0)
            set_free(bit);
        return bit;
    }

    s32 duomap_t::find_used_upper() const
    {
        if (m_l0 == 0)
            return -1;

        u32 const l  = levels() - 1;
        u32       wi = 0;
        s8        bi = 31 - math::findLastBit(m_l0);
        ASSERT(bi >= 0 && bi < 32);
        for (s8 i = 0; i <= l; ++i)
        {
            wi           = (wi << 5) + bi;
            u32 const wc = i < l ? m_l[i][wi] : m_binmap0.m_l[i][wi];
            ASSERT(wc != 0);
            bi = 31 - math::findLastBit(wc);
            ASSERT(bi >= 0 && bi < 32);
        }

        u32 const bin = (wi << 5) + bi;
        return (bin < size()) ? bin : -1;
    }

    s32 duomap_t::find_used_upper_and_set_free()
    {
        s32 const bit = find_used_upper();
        if (bit >= 0)
            set_free(bit);
        return bit;
    }

    s32 duomap_t::upper_used(u32 pivot) const
    {
        if (pivot >= size())
            return -1;

        // Start at bottom level and move up finding a '1' bit
        u32 iw = (pivot / 32);  // The index of a 32-bit word in bottom level
        u32 ib = (pivot & 31);  // The bit number in that 32-bit word

        s8 const ml = levels();
        s8       il = ml;

        u32 nwpl = (size() + 31) >> 5;  // Number of words for the lowest level
        while (il >= 0 && il <= ml)
        {
            u32 const* level = il == 0 ? &m_l0 : (il < ml ? m_l[il - 1] : m_binmap0.m_l[il - 1]);
            u32 const  w     = level[iw] & (0xffffffff << ib);
            if (w != 0)
            {
                iw = (iw * 32) + math::findFirstBit(w);
                if (il == ml)
                    return iw;
                il += 1;  // Go down one level
                ib = 0;
            }
            else
            {
                // move one unit in the direction of upper
                iw += 1;
                if (il == ml && iw >= nwpl)
                    break;
                ib = (iw & 31);
                iw = (iw / 32);
                il -= 1;  // Go up one level
            }
        };
        return -1;
    }

    s32 duomap_t::lower_used(u32 pivot) const
    {
        if (pivot >= size())
            return -1;

        // Start at bottom level and move up finding a 'set' bit
        u32 iw = (pivot / 32);  // The index of a 32-bit word in level 0
        u32 ib = (pivot & 31);  // The bit number in that 32-bit word

        s8 const ml = levels();
        s8       il = ml;

        while (il >= 0 && il <= ml)
        {
            u32 const* level = il == 0 ? &m_l0 : (il < ml ? m_l[il - 1] : m_binmap0.m_l[il - 1]);
            u32 const  w     = level[iw] & (0xffffffff >> (31 - ib));
            if (w != 0)
            {
                iw = (iw * 32) + (u32)math::findFirstBit(w);
                if (il == ml)
                    return iw;
                il += 1;  // Go down one level
                ib = 31;
            }
            else
            {
                // move one unit in the direction of lower
                iw -= 1;
                ib = (iw & 31);
                iw = (iw / 32);
                il -= 1;  // Go up one level
            }
        };
        return -1;
    }

    void duomap_t::iter_free_t::begin()
    {
        // Find the first free bit, starting from and including 'start'
        m_cur = m_bm->upper(m_cur);
        if (m_cur < 0)
            m_cur = m_end;
    }

    void duomap_t::iter_free_t::next()
    {
        m_cur = m_bm->upper(m_cur + 1);
        if (m_cur < 0)
            m_cur = m_end;
    }

    void duomap_t::iter_used_t::begin()
    {
        // Find the first free bit, starting from and including 'start'
        m_cur = m_bm->upper_used(m_cur);
        if (m_cur < 0)
            m_cur = m_end;
    }

    void duomap_t::iter_used_t::next()
    {
        m_cur = m_bm->upper_used(m_cur + 1);
        if (m_cur < 0)
            m_cur = m_end;
    }

}  // namespace ncore
