#ifndef __CBASE_DUOMAP_H__
#define __CBASE_DUOMAP_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_binmap.h"

namespace ncore
{
    class alloc_t;

    // Hierarchical Bin Map, keeps track of both '1' and '0' bits in a hierarchical manner.
    // The lowest level has all the bits according to 'count', the next level has 1 bit for each 32 bits in the level below.
    // This advantage of using this duomap instead of two binmaps, is that this duomap shares the lowest level with the binmap.
    // Note: Tracks up to 2^20 bits
    struct duomap_t  // 32 bytes
    {
        typedef binmap_t::config_t config_t;

        // This will output the number of bits in each level and return the number of levels
        static config_t compute(u32 count);
        static u32      sizeof_data(u32 count);

        inline void reset()
        {
            m_binmap0.reset();
            m_l0   = 0;
            m_set  = 0;
            m_l[0] = m_l[1] = m_l[2] = nullptr;
        }

        void release(alloc_t* allocator);


        void init_all_free(config_t const& cfg, u32* bm0l1, u32* bm0l2, u32* bm0l3, u32* bm1l1, u32* bm1l2);
        void init_all_free();
        void init_all_free(config_t const& cfg, alloc_t* allocator);
        void init_all_free_lazy();
        void init_all_free_lazy(config_t const& cfg, u32* bm0l1, u32* bm0l2, u32* bm0l3, u32* bm1l1, u32* bm1l2);  // Do not not clear the levels, only the ends
        void init_all_free_lazy(config_t const& cfg, alloc_t* allocator);
        void tick_all_free_lazy(u32 bit);

        void init_all_used();
        void init_all_used(config_t const& cfg, alloc_t* allocator);
        void init_all_used(config_t const& cfg, u32* bm0l1, u32* bm0l2, u32* bm0l3, u32* bm1l1, u32* bm1l2);
        // void init_all_used_lazy();
        // void init_all_used_lazy(config_t const& cfg, u32* bm0l1, u32* bm0l2, u32* bm0l3, u32* bm1l1, u32* bm1l2);  // Do not not clear the levels, only the ends
        // void init_all_used_lazy(config_t const& cfg, alloc_t* allocator);
        // void tick_all_used_lazy(u32 bit);  // Progressive lazy initialization

        void set_free(u32 bit);
        void set_used(u32 bit);
        bool get(u32 bit) const;
        bool is_free(u32 bit) const { return !get(bit); }
        bool is_used(u32 bit) const { return get(bit); }

        s32 find_free() const;               // Finds the first free bit and returns the bit index
        s32 find_free_and_set_used();        // Finds the first free bit and sets it to used and returns the bit index
        s32 find_free_upper() const;         // Finds the last free bit and returns the bit index
        s32 find_free_upper_and_set_used();  // Finds the last free bit and sets it to used and returns the bit index

        s32 upper_free(u32 pivot) const;  // Finds the first free bit greater than the pivot
        s32 lower_free(u32 pivot) const;  // Finds the first free bit less than the pivot (high to low)

        s32 find_used() const;
        s32 find_used_and_set_free();
        s32 find_used_upper() const;
        s32 find_used_upper_and_set_free();

        s32 upper_used(u32 pivot) const;
        s32 lower_used(u32 pivot) const;

        inline u32 size() const { return m_binmap0.size(); }
        inline s8  levels() const { return m_binmap0.levels(); }
        inline s32 num_used() const { return m_set; }
        inline s32 num_free() const { return size() - m_set; }

        struct iter_free_t
        {
            iter_free_t(duomap_t* bm)
                : m_bm(&bm->m_binmap0)
                , m_cur(0)
                , m_end(bm->size())
            {
            }

            iter_free_t(duomap_t* bm, u32 start)
                : m_bm(&bm->m_binmap0)
                , m_cur(start)
                , m_end(bm->size())
            {
            }

            iter_free_t(duomap_t* bm, u32 start, u32 end)
                : m_bm(&bm->m_binmap0)
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

        struct iter_used_t
        {
            iter_used_t(duomap_t* bm)
                : m_bm(bm)
                , m_cur(0)
                , m_end(bm->size())
            {
            }

            iter_used_t(duomap_t* bm, u32 start)
                : m_bm(bm)
                , m_cur(start)
                , m_end(bm->size())
            {
            }

            iter_used_t(duomap_t* bm, u32 start, u32 end)
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
            duomap_t* m_bm;
            u32       m_cur;
            u32       m_end;
        };

        binmap_t m_binmap0;  // The binmap tracking '0' bits
        u32*     m_l[2];     // We are tracking '1' bits
        u32      m_l0;       // The top level
        u32      m_set;      // Number of '1' bits, also tells us the number of '0' bits
    };

};  // namespace ncore

#endif  /// __C_HIERARCHICAL_BIT_BUFFER_H__
