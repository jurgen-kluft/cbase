#ifndef __CBASE_BINMAP_H__
#define __CBASE_BINMAP_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    class alloc_t;

    // Hierarchical Bin Map, keeps track of free bits in a hierarchical way.
    // Note: Tracks free bits
    // Note: Tracks up to 2^20 bits
    struct binmap_t  // 32 bytes
    {
        struct config_t
        {
            u32 m_count;
            u32 m_l0len;
            u32 m_lnlen[3];  // The level lengths
            s32 m_levels;

            static config_t compute(u32 count);
            static u32      sizeof_data(u32 count);
        };

        // This will output the number of bits in each level and return the number of levels
        static void clear_level0(config_t const& cfg, u32& l0, s8 btv, s8 cv);
        static void clear_levelN(s8 lnlen, u32* lndata, s8 btv, s8 cv);
        static void clear_levels(config_t const& cfg, u32& l0, u32* l1, u32* l2, u32* l3, s8 btv, s8 cv);

        inline void reset()
        {
            m_count = 0;
            m_l0    = 0;
            m_l[0] = m_l[1] = m_l[2] = nullptr;
        }

        void release(alloc_t* allocator);

        void init_all_free();
        void init_all_used();

        void init_all_free(config_t const& cfg, alloc_t* allocator);
        void init_all_used(config_t const& cfg, alloc_t* allocator);

        void init_all_free(config_t const& cfg, u32* l1, u32* l2, u32* l3);
        void init_all_used(config_t const& cfg, u32* l1, u32* l2, u32* l3);

        void init_all_free_lazy();
        void init_all_free_lazy(config_t const& cfg, u32* l1, u32* l2, u32* l3);  // Do not not clear the levels, only the ends
        void init_all_free_lazy(config_t const& cfg, alloc_t* allocator);
        void tick_all_free_lazy(u32 bit);

        void init_all_used_lazy();
        void init_all_used_lazy(config_t const& cfg, u32* l1, u32* l2, u32* l3);  // Do not not clear the levels, only the ends
        void init_all_used_lazy(config_t const& cfg, alloc_t* allocator);
        void tick_all_used_lazy(u32 bit);  // Progressive lazy initialization

        void set_free(u32 bit);
        void set_used(u32 bit);
        bool get(u32 bit) const;
        bool is_free(u32 bit) const { return !get(bit); }
        bool is_used(u32 bit) const { return get(bit); }

        s32 find() const;          // Finds the first free bit and returns the bit index
        s32 find_and_set();        // Finds the first free bit and sets it to used and returns the bit index
        s32 find_upper() const;    // Finds the last free bit and returns the bit index
        s32 find_upper_and_set();  // Finds the last free bit and sets it to used and returns the bit index

        s32 upper(u32 pivot) const;  // Finds the first free bit greater than the pivot
        s32 lower(u32 pivot) const;  // Finds the first free bit less than the pivot (high to low)

        // Finds the set bit from the given bit, this is useful for iterating over the set bits.
        // Using this as an iterator means you need to increment the returned bit by one to use it in the
        // next call to find_set.
        // Note: This function only moves over the lowest level and is thus not taking advantage of the
        //       hierarchical structure. This means that it is not efficient for large sparse bitmaps.
        s32 find_set(u32 bit) const;

        inline u32 size() const { return m_count & 0x0FFFFFFF; }
        inline s8  levels() const { return m_count >> 28; }

        struct iter_t
        {
            iter_t(binmap_t* bm)
                : m_bm(bm)
                , m_cur(0)
                , m_end(bm->size())
            {
            }

            iter_t(binmap_t* bm, u32 start)
                : m_bm(bm)
                , m_cur(start)
                , m_end(bm->size())
            {
            }

            iter_t(binmap_t* bm, u32 start, u32 end)
                : m_bm(bm)
                , m_cur(start)
                , m_end(end)
            {
            }

            inline bool end() const { return !(m_cur < m_end); }
            inline u32  get() const { return m_cur; }

            void begin();
            void next();

        private:
            binmap_t* m_bm;
            u32       m_cur;
            u32       m_end;
        };

        u32  m_count;  // 0xF0000000 = number of levels, 0x0FFFFFFF = number of bits
        u32  m_l0;     // Level 0 is 32 bits
        u32* m_l[3];   // Separate the allocation of level data (better allocation sizes)
    };

};  // namespace ncore

#endif  /// __C_HIERARCHICAL_BIT_BUFFER_H__
