#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_allocator.h"
#include "cbase/c_memory.h"
#include "cbase/c_integer.h"

#include "cbase/c_binmap.h"

namespace ncore
{
    // l0 :  32 =  1  * u32
    // l1 :  1K =  32 * u32
    // l2 : 32K =  1K * u32
    // l3 :  1M = 32K * u32

    binmap_t::config_t binmap_t::config_t::compute(u32 count)
    {
        ASSERT(count > 0 && count <= 1 * 1024 * 1024);  // maximum count is 1 Million (5 bits + 5 bits + 5 bits + 5 bits = 20 bits = 1 M)
        config_t cfg;
        cfg.m_levels   = (math::mostSignificantBit(count - 1) / 5);  // We can have a maximum of 4 levels, each level holds 5 bits
        cfg.m_count    = count;
        cfg.m_l0len    = 0;
        cfg.m_lnlen[0] = cfg.m_lnlen[1] = cfg.m_lnlen[2] = 0;
        u32 len                                          = count;
        switch (cfg.m_levels)
        {
            case 3: cfg.m_lnlen[2] = len; len = (len + 31) >> 5;  // fall through
            case 2: cfg.m_lnlen[1] = len; len = (len + 31) >> 5;  // fall through
            case 1: cfg.m_lnlen[0] = len; len = (len + 31) >> 5;  // fall through
        }
        cfg.m_l0len = len;
        return cfg;
    }

    u32 binmap_t::config_t::sizeof_data(u32 count)
    {
        config_t c = compute(count);
        return sizeof(binmap_t) + ((c.m_lnlen[0] + c.m_lnlen[1] + c.m_lnlen[2]) >> 3);
    }

    // btv = bit tracking, either 0 or 1, this binmap should always pass 0
    // cv = clear value, either 0 or 1(0xffffffff)
    static void s_clear_level(u32 level_bits, u32* level, s8 btv, s8 cv)
    {
        ASSERT(btv == 0 || btv == 1);
        ASSERT(cv == 0 || cv == 1);

        if (level == nullptr)
            return;

        u32 const w = level_bits >> 5;
        u32 const c = cv & 1 ? 0xffffffff : 0;
        for (u32 i = 0; i < w; i++)
            level[i] = c;

        u32 const n = level_bits & (32 - 1);
        if (n != 0)
        {
            switch (btv)
            {
                case 0: level[w] = cv == 1 ? 0xffffffff : 0xffffffff << n; break;
                case 1: level[w] = cv == 0 ? 0 : ~(0xffffffff >> n); break;
            }
        }
    }

    void binmap_t::clear_level0(config_t const& cfg, u32& l0, s8 btv, s8 cv)
    {
        switch ((btv << 1) | cv)
        {
            case 0: l0 = (u32)((u64)0xffffffff << cfg.m_l0len); break;
            case 1: l0 = 0xffffffff; break;
            case 2: l0 = 0; break;
            case 3: l0 = (u32)(~((u64)0xffffffff << cfg.m_l0len)); break;
        }
    }

    void binmap_t::clear_levelN(s8 lnlen, u32* lndata, s8 btv, s8 cv) { s_clear_level(lnlen, lndata, btv, cv); }

    void binmap_t::clear_levels(config_t const& cfg, u32& l0, u32* l1, u32* l2, u32* l3, s8 btv, s8 cv)
    {
        switch (cfg.m_levels)
        {
            case 3: s_clear_level(cfg.m_lnlen[2], l3, btv, cv);
            case 2: s_clear_level(cfg.m_lnlen[1], l2, btv, cv);
            case 1: s_clear_level(cfg.m_lnlen[0], l1, btv, cv);
            case 0: clear_level0(cfg, l0, btv, cv);
        }
    }

    void binmap_t::init_all_free()
    {
        u32 const count = size();
        config_t  cfg   = config_t::compute(count);
        init_all_free(cfg, m_l[0], m_l[1], m_l[2]);
    }

    void binmap_t::init_all_used()
    {
        u32 const count = size();
        config_t  cfg   = config_t::compute(count);
        init_all_used(cfg, m_l[0], m_l[1], m_l[2]);
    }

    void binmap_t::init_all_free(u32 count, alloc_t* allocator)
    {
        config_t cfg = config_t::compute(count);
        u32*     d1  = (cfg.m_lnlen[0] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[0]) : nullptr;
        u32*     d2  = (cfg.m_lnlen[1] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[1]) : nullptr;
        u32*     d3  = (cfg.m_lnlen[2] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[2]) : nullptr;
        init_all_free(cfg, d1, d2, d3);
    }

    void binmap_t::init_all_used(u32 count, alloc_t* allocator)
    {
        config_t cfg = config_t::compute(count);
        u32*     d1  = (cfg.m_lnlen[0] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[0]) : nullptr;
        u32*     d2  = (cfg.m_lnlen[1] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[1]) : nullptr;
        u32*     d3  = (cfg.m_lnlen[2] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[2]) : nullptr;
        init_all_used(cfg, d1, d2, d3);
    }

    void binmap_t::release(alloc_t* allocator)
    {
        switch (levels())
        {
            case 3: allocator->deallocate(m_l[2]);
            case 2: allocator->deallocate(m_l[1]);
            case 1: allocator->deallocate(m_l[0]);
        }
        reset();
    }

    void binmap_t::init_all_free_lazy() { m_l0 = 0xffffffff; }

    void binmap_t::init_all_free_lazy(config_t const& cfg, u32* l1, u32* l2, u32* l3)
    {
        ASSERT((cfg.m_ln[3] == 0 || l3 != nullptr) && (cfg.m_ln[2] == 0 || l2 != nullptr) && (cfg.m_ln[1] == 0 || l1 != nullptr) && (cfg.m_ln[0] > 0));

        m_l[2]  = l3;
        m_l[1]  = l2;
        m_l[0]  = l1;
        m_l0    = 0xffffffff;
        m_count = (cfg.m_levels << 28) | cfg.m_count;
    }

    void binmap_t::init_all_free_lazy(u32 count, alloc_t* allocator)
    {
        config_t cfg = config_t::compute(count);
        u32*     d1  = (cfg.m_lnlen[0] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[0]) : nullptr;
        u32*     d2  = (cfg.m_lnlen[1] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[1]) : nullptr;
        u32*     d3  = (cfg.m_lnlen[2] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[2]) : nullptr;
        init_all_free_lazy(cfg, d1, d2, d3);
    }

    void binmap_t::tick_all_free_lazy(u32 bit)
    {
        u32 wi = bit;
        for (s8 l = levels() - 1; l >= 0; --l)
        {
            const u32 li = wi & (32 - 1);
            wi           = wi >> 5;
            const u32 wd = (li == 0) ? 0xffffffff : m_l[l][wi];
            const u32 bi = (u32)1 << li;
            m_l[l][wi]   = wd & ~bi;
            if (wd != 0xffffffff)
                return;
        }
        m_l0 = m_l0 & ~((u32)1 << (wi & (32 - 1)));
    }

    void binmap_t::init_all_used_lazy() { m_l0 = 0xffffffff; }

    void binmap_t::init_all_used_lazy(config_t const& cfg, u32* l1, u32* l2, u32* l3)
    {
        ASSERT((cfg.m_ln[3] == 0 || l3 != nullptr) && (cfg.m_ln[2] == 0 || l2 != nullptr) && (cfg.m_ln[1] == 0 || l1 != nullptr) && (cfg.m_ln[0] > 0));

        m_l[2]  = l3;
        m_l[1]  = l2;
        m_l[0]  = l1;
        m_l0    = 0xffffffff;
        m_count = (cfg.m_levels << 28) | cfg.m_count;
    }

    void binmap_t::init_all_used_lazy(u32 count, alloc_t* allocator)
    {
        config_t cfg = config_t::compute(count);
        u32*     d1  = (cfg.m_lnlen[0] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[0]) : nullptr;
        u32*     d2  = (cfg.m_lnlen[1] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[1]) : nullptr;
        u32*     d3  = (cfg.m_lnlen[2] > 0) ? (u32*)allocator->allocate(sizeof(u32) * cfg.m_lnlen[2]) : nullptr;
        init_all_used_lazy(cfg, d1, d2, d3);
    }

    void binmap_t::tick_all_used_lazy(u32 bit)
    {
        u32 wi = bit;
        for (s8 l = levels() - 1; l >= 0; --l)
        {
            const u32 li = wi & (32 - 1);
            wi           = wi >> 5;
            u32 wd       = li == 0 ? 0xfffffffe : m_l[l][wi];
            wd |= (u32)1 << li;
            m_l[l][wi] = wd;
            if (wd != 0xffffffff)
                return;
        }
        m_l0 = m_l0 | ((u32)1 << (wi & (32 - 1)));
    }

    void binmap_t::init_all_free(config_t const& cfg, u32* l1, u32* l2, u32* l3)
    {
        ASSERT((cfg.m_ln[3] == 0 || l3 != nullptr) && (cfg.m_ln[2] == 0 || l2 != nullptr) && (cfg.m_ln[1] == 0 || l1 != nullptr) && (cfg.m_ln[0] > 0));
        m_count = (cfg.m_levels << 28) | cfg.m_count;
        m_l[2]  = l3;
        m_l[1]  = l2;
        m_l[0]  = l1;
        clear_levels(cfg, m_l0, m_l[0], m_l[1], m_l[2], 0, 0);
    }

    void binmap_t::init_all_used(config_t const& cfg, u32* l1, u32* l2, u32* l3)
    {
        ASSERT((cfg.m_ln[3] == 0 || l3 != nullptr) && (cfg.m_ln[2] == 0 || l2 != nullptr) && (cfg.m_ln[1] == 0 || l1 != nullptr) && (cfg.m_ln[0] > 0));
        m_count = (cfg.m_levels << 28) | cfg.m_count;
        m_l[2]  = l3;
        m_l[1]  = l2;
        m_l[0]  = l1;
        clear_levels(cfg, m_l0, m_l[0], m_l[1], m_l[2], 0, 1);
    }

    // Note: We are tracking 'empty' place where we can set a bit
    void binmap_t::set_used(u32 bit)
    {
        u32 wi = bit;
        for (s8 l = levels() - 1; l >= 0; --l)
        {
            u32 const bi = (u32)1 << (wi & (32 - 1));
            wi           = wi >> 5;
            u32 wd       = m_l[l][wi];
            wd |= bi;
            m_l[l][wi] = wd;
            // If all bits are not set yet -> early out
            // Which means there are some empty places
            if (wd != 0xffffffff)
                return;
        }
        m_l0 = m_l0 | (1 << (wi & (32 - 1)));
    }

    void binmap_t::set_free(u32 bit)
    {
        u32 wi = bit;
        for (s8 l = levels() - 1; l >= 0; --l)
        {
            u32 const bi = (u32)1 << (wi & (32 - 1));
            wi           = wi >> 5;
            const u32 wd = m_l[l][wi];
            m_l[l][wi]   = wd & ~bi;
            // There are already where some empty places -> early out
            if (wd != 0xFFFFFFFF)
                return;
        }
        m_l0 = m_l0 & ~(1 << (wi & (32 - 1)));
    }

    bool binmap_t::get(u32 bit) const
    {
        u32 const l  = levels();
        u32 const bi = (u32)1 << (bit & (32 - 1));
        if (l == 0)
            return (m_l0 & bi) != 0;
        u32 const wi = bit >> 5;
        u32 const wd = m_l[l - 1][wi];
        return (wd & bi) != 0;
    }

    s32 binmap_t::find() const
    {
        if (m_l0 == 0xffffffff)
            return -1;

        u32 const l  = levels();
        u32       wi = 0;
        s8        bi = math::findFirstBit(~m_l0);
        ASSERT(bi >= 0 && bi < 32);
        for (u32 i = 0; i < l; ++i)
        {
            wi = (wi << 5) + bi;
            bi = math::findFirstBit(~m_l[i][wi]);
            ASSERT(bi >= 0 && bi < 32);
        }

        u32 const bin = (wi << 5) + bi;
        return (bin < size()) ? bin : -1;
    }

    s32 binmap_t::find_and_set()
    {
        s32 const bi = find();
        if (bi >= 0)
            set_used(bi);
        return bi;
    }

    s32 binmap_t::find_upper() const
    {
        if (m_l0 == 0xffffffff)
            return -1;

        u32 const l  = levels();
        u32       wi = 0;
        ASSERT(~m_l0 != 0);
        s8 bi = 31 - math::findLastBit(~m_l0);
        ASSERT(bi >= 0 && bi < 32);
        for (u32 i = 0; i < l; ++i)
        {
            wi = (wi << 5) + bi;
            ASSERT(~m_l[i][wi] != 0);
            bi = 31 - math::findLastBit(~m_l[i][wi]);
            ASSERT(bi >= 0 && bi < 32);
        }

        u32 const bin = (wi << 5) + bi;
        return (bin < size()) ? bin : -1;
    }

    s32 binmap_t::find_upper_and_set()
    {
        s32 const bi = find_upper();
        if (bi >= 0)
            set_used(bi);
        return bi;
    }

    s32 binmap_t::upper(u32 pivot) const
    {
        if (pivot >= size())
            return -1;

        // Start at bottom level and move up finding an empty place
        u32 iw = (pivot / 32);  // The index of a 32-bit word in bottom level
        u32 ib = (pivot & 31);  // The bit number in that 32-bit word

        s8 const ml = levels();
        s8       il = ml;

        u32 nwpl = (size() + 31) >> 5;  // Number of words per level, lowest level
        while (il >= 0 && il <= ml)
        {
            u32 const* level = il == 0 ? &m_l0 : m_l[il - 1];
            u32 const  w     = (~level[iw]) & (0xffffffff << ib);
            if (w != 0)
            {
                iw = (iw * 32) + math::findFirstBit(w);
                if (il == ml)
                    return iw < size() ? iw : -1;
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
                iw = (iw >> 5);
                il -= 1;  // Go up one level
            }
        };
        return -1;
    }

    // Find a '0' bit lower but including pivot
    s32 binmap_t::lower(u32 pivot) const
    {
        if (pivot >= size())
            return -1;

        u32 iw = (pivot / 32);  // The index of a 32-bit word in level 0
        u32 ib = (pivot & 31);  // The bit number in that 32-bit word

        s8 const ml = levels();
        s8       il = ml;
        while (il >= 0 && il <= ml)
        {
            u32 const* level = il == 0 ? &m_l0 : m_l[il - 1];
            u32 const  w     = (~level[iw]) & (0xffffffff >> (31 - ib));
            if (w != 0)
            {
                iw = (iw * 32) + (u32)math::findFirstBit(w);
                if (il == ml)
                    return iw < size() ? iw : -1;
                il += 1;  // Go down one level
                ib = 31;
            }
            else
            {
                // move one unit in the direction of lower
                if (iw == 0)
                    break;
                iw -= 1;
                ib = (iw & 31);
                iw = (iw >> 5);
                il -= 1;  // Go up one level
            }
        };
        return -1;
    }

    s32 binmap_t::find_set(u32 bit) const
    {
        u32 const maxbits = size();
        if (bit >= maxbits)
            return -1;

        u32 const  iw    = bit / 32;
        u32 const  ml    = levels();
        u32 const* level = (ml == 0) ? &m_l0 : m_l[ml - 1];

        {
            u32 const ib = bit & 31;
            u32 const w  = level[iw] & (0xffffffff << ib);
            if (w != 0)
            {
                s32 const found_bit = (s32)((iw * 32) + math::findFirstBit(w));
                return (found_bit < maxbits) ? found_bit : -1;
            }
        }

        {
            u32 const nw = (maxbits + 31) >> 5;
            for (s32 i = iw + 1; i < nw; ++i)
            {
                u32 const w = level[i];
                if (w != 0)
                {
                    s32 const found_bit = (s32)((i * 32) + math::findFirstBit(w));
                    return (found_bit < maxbits) ? found_bit : -1;
                }
            }
        }

        return -1;
    }

    void binmap_t::iter_t::begin()
    {
        // Find the first free bit, starting from and including 'start'
        m_cur = m_bm->upper(m_cur);
        if (m_cur < 0)
            m_cur = m_end;
    }

    void binmap_t::iter_t::next()
    {
        m_cur = m_bm->upper(m_cur + 1);
        if (m_cur < 0)
            m_cur = m_end;
    }

}  // namespace ncore
