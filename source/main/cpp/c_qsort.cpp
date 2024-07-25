#include "cbase/c_qsort.h"

namespace ncore
{
    //----------------------------------------------------------------------------------------------------------------
    // Custom QuickSort
    namespace __qsort
    {
		template <typename T>
		static inline T minimum(T a, T b) { return (a) < (b) ? a : b; }

        static inline void sSwap(u8* a, u8* b, s32 n)
        {
            while (n > 0)
            {
                u8 t = *a;
                *a++ = *b;
                *b++ = t;
                --n;
            }
        }

        static inline void sSwap2(u16* a, u16* b, s32 n)
        {
            while (n > 0)
            {
                u16 t = *a;
                *a++  = *b;
                *b++  = t;
                --n;
            }
        }

        static inline void sSwap4(u32* a, u32* b, s32 n)
        {
            while (n > 0)
            {
                u32 t = *a;
                *a++  = *b;
                *b++  = t;
                --n;
            }
        }

        static inline void sSwap8(u64* a, u64* b, s32 n)
        {
            while (n > 0)
            {
                u64 t = *a;
                *a++  = *b;
                *b++  = t;
                --n;
            }
        }

        static inline u8* sMed3(u8* a, u8* b, u8* c, s32 (*cmp)(const void* const, const void* const, void*), void* data)
        {
            return cmp(a, b, data) < 0 ? (cmp(b, c, data) < 0 ? b : (cmp(a, c, data) < 0 ? c : a)) : (cmp(b, c, data) > 0 ? b : (cmp(a, c, data) < 0 ? a : c));
        }
        static inline u16* sMed3(u16* a, u16* b, u16* c, s32 (*cmp)(const void* const, const void* const, void*), void* data)
        {
            return cmp(a, b, data) < 0 ? (cmp(b, c, data) < 0 ? b : (cmp(a, c, data) < 0 ? c : a)) : (cmp(b, c, data) > 0 ? b : (cmp(a, c, data) < 0 ? a : c));
        }
        static inline u32* sMed3(u32* a, u32* b, u32* c, s32 (*cmp)(const void* const, const void* const, void*), void* data)
        {
            return cmp(a, b, data) < 0 ? (cmp(b, c, data) < 0 ? b : (cmp(a, c, data) < 0 ? c : a)) : (cmp(b, c, data) > 0 ? b : (cmp(a, c, data) < 0 ? a : c));
        }
        static inline u64* sMed3(u64* a, u64* b, u64* c, s32 (*cmp)(const void* const, const void* const, void*), void* data)
        {
            return cmp(a, b, data) < 0 ? (cmp(b, c, data) < 0 ? b : (cmp(a, c, data) < 0 ? c : a)) : (cmp(b, c, data) > 0 ? b : (cmp(a, c, data) < 0 ? a : c));
        }
    }  // namespace __qsort

	// Sort routine for element size other than 2, 4, or 8 bytes
    void g_qsortN(u8* a, s32 n, s32 es, s32 (*cmp)(const void*, const void*, void*), void* user_data)
    {
        u8 *pa, *pb, *pc, *pd, *pl, *pm, *pn;
        s32 d, r, swaptype, swap_cnt;
    loop:
        swap_cnt = 0;
        if (n < 7)
        {
            for (pm = (u8*)a + es; pm < (u8*)a + n * es; pm += es)
                for (pl = pm; pl > (u8*)a && cmp(pl - es, pl, user_data) > 0; pl -= es)
                    __qsort::sSwap(pl, pl - es, es);
            return;
        }
        pm = (u8*)a + (n / 2) * es;
        if (n > 7)
        {
            pl = (u8*)a;
            pn = (u8*)a + (n - 1) * es;
            if (n > 40)
            {
                d  = (n / 8) * es;
                pl = __qsort::sMed3(pl, pl + d, pl + 2 * d, cmp, user_data);
                pm = __qsort::sMed3(pm - d, pm, pm + d, cmp, user_data);
                pn = __qsort::sMed3(pn - 2 * d, pn - d, pn, cmp, user_data);
            }
            pm = __qsort::sMed3(pl, pm, pn, cmp, user_data);
        }
        __qsort::sSwap((u8*)a, pm, es);
        pa = pb = (u8*)a + es;
        pc = pd = (u8*)a + (n - 1) * es;
        for (;;)
        {
            while (pb <= pc && (r = cmp(pb, (u8*)a, user_data)) <= 0)
            {
                if (r == 0)
                {
                    swap_cnt = 1;
                    __qsort::sSwap(pa, pb, es);
                    pa += es;
                }
                pb += es;
            }

            while (pb <= pc && (r = cmp(pc, (u8*)a, user_data)) >= 0)
            {
                if (r == 0)
                {
                    swap_cnt = 1;
                    __qsort::sSwap(pc, pd, es);
                    pd -= es;
                }
                pc -= es;
            }

            if (pb > pc)
                break;

            __qsort::sSwap(pb, pc, es);
            swap_cnt = 1;
            pb += es;
            pc -= es;
        }

        if (swap_cnt == 0)  // Switch to insertion sort
        {
            for (pm = (u8*)a + es; pm < (u8*)a + n * es; pm += es)
                for (pl = pm; pl > (u8*)a && cmp(pl - es, pl, user_data) > 0; pl -= es)
                    __qsort::sSwap(pl, pl - es, es);
            return;
        }

        pn = (u8*)a + n * es;
        r  = (s32)(__qsort::minimum(pa - (u8*)a, pb - pa));
        //__qsort_VecSwap((u8*)a, pb - r, r);
        __qsort::sSwap((u8*)a, pb - r, r);
        r = (s32)(__qsort::minimum(pd - pc, pn - pd - es));
        //__qsort_VecSwap(pb, pn - r, r);
        __qsort::sSwap(pb, pn - r, r);

        if ((r = (s32)(pb - pa)) > es)
            g_qsortN((u8*)a, r / es, es, cmp, user_data);

        if ((r = (s32)(pd - pc)) > es)
        {
            // Iterate rather than recurse to save stack space
            a = pn - r;
            n = r / es;
            goto loop;
        }
    }

    void g_qsort2(u16* a, s32 n, s32 (*cmp)(const void*, const void*, void*), void* data)
    {
        u16 *pa, *pb, *pc, *pd, *pl, *pm, *pn;
        s32  d, r, swaptype, swap_cnt;
    loop:
        swap_cnt = 0;
        if (n < 7)
        {
            for (pm = (u16*)a + 1; pm < (u16*)a + n * 1; pm += 1)
                for (pl = pm; pl > (u16*)a && cmp(pl - 1, pl, data) > 0; pl -= 1)
                    __qsort::sSwap2(pl, pl - 1, 1);
            return;
        }
        pm = (u16*)a + (n / 2) * 1;
        if (n > 7)
        {
            pl = (u16*)a;
            pn = (u16*)a + (n - 1) * 1;
            if (n > 40)
            {
                d  = (n / 8) * 1;
                pl = __qsort::sMed3(pl, pl + d, pl + 2 * d, cmp, data);
                pm = __qsort::sMed3(pm - d, pm, pm + d, cmp, data);
                pn = __qsort::sMed3(pn - 2 * d, pn - d, pn, cmp, data);
            }
            pm = __qsort::sMed3(pl, pm, pn, cmp, data);
        }
        __qsort::sSwap2((u16*)a, pm, 1);
        pa = pb = (u16*)a + 1;
        pc = pd = (u16*)a + (n - 1) * 1;
        for (;;)
        {
            while (pb <= pc && (r = cmp(pb, (u16*)a, data)) <= 0)
            {
                if (r == 0)
                {
                    swap_cnt = 1;
                    __qsort::sSwap2(pa, pb, 1);
                    pa += 1;
                }
                pb += 1;
            }

            while (pb <= pc && (r = cmp(pc, (u16*)a, data)) >= 0)
            {
                if (r == 0)
                {
                    swap_cnt = 1;
                    __qsort::sSwap2(pc, pd, 1);
                    pd -= 1;
                }
                pc -= 1;
            }

            if (pb > pc)
                break;

            __qsort::sSwap2(pb, pc, 1);
            swap_cnt = 1;
            pb += 1;
            pc -= 1;
        }

        if (swap_cnt == 0)  // Switch to insertion sort
        {
            for (pm = (u16*)a + 1; pm < (u16*)a + n * 1; pm += 1)
                for (pl = pm; pl > (u16*)a && cmp(pl - 1, pl, data) > 0; pl -= 1)
                    __qsort::sSwap2(pl, pl - 1, 1);
            return;
        }

        pn = (u16*)a + n * 1;
        r  = (s32)(__qsort::minimum(pa - (u16*)a, pb - pa));
        __qsort::sSwap2((u16*)a, pb - r, r);
        r = (s32)(__qsort::minimum(pd - pc, pn - pd - 1));
        __qsort::sSwap2(pb, pn - r, r);

        if ((r = (s32)(pb - pa)) > 1)
            g_qsort2((u16*)a, r / 1, cmp, data);

        if ((r = (s32)(pd - pc)) > 1)
        {
            // Iterate rather than recurse to save stack space
            a = pn - r;
            n = r;
            goto loop;
        }
    }

    void g_qsort4(u32* a, s32 n, s32 (*cmp)(const void*, const void*, void*), void* data)
    {
        u32 *pa, *pb, *pc, *pd, *pl, *pm, *pn;
        s32  d, r, swaptype, swap_cnt;
    loop:
        swap_cnt = 0;
        if (n < 7)
        {
            for (pm = (u32*)a + 1; pm < (u32*)a + n * 1; pm += 1)
                for (pl = pm; pl > (u32*)a && cmp(pl - 1, pl, data) > 0; pl -= 1)
                    __qsort::sSwap4(pl, pl - 1, 1);
            return;
        }
        pm = (u32*)a + (n / 2) * 1;
        if (n > 7)
        {
            pl = (u32*)a;
            pn = (u32*)a + (n - 1) * 1;
            if (n > 40)
            {
                d  = (n / 8) * 1;
                pl = __qsort::sMed3(pl, pl + d, pl + 2 * d, cmp, data);
                pm = __qsort::sMed3(pm - d, pm, pm + d, cmp, data);
                pn = __qsort::sMed3(pn - 2 * d, pn - d, pn, cmp, data);
            }
            pm = __qsort::sMed3(pl, pm, pn, cmp, data);
        }
        __qsort::sSwap4((u32*)a, pm, 1);
        pa = pb = (u32*)a + 1;
        pc = pd = (u32*)a + (n - 1) * 1;
        for (;;)
        {
            while (pb <= pc && (r = cmp(pb, (u32*)a, data)) <= 0)
            {
                if (r == 0)
                {
                    swap_cnt = 1;
                    __qsort::sSwap4(pa, pb, 1);
                    pa += 1;
                }
                pb += 1;
            }

            while (pb <= pc && (r = cmp(pc, (u32*)a, data)) >= 0)
            {
                if (r == 0)
                {
                    swap_cnt = 1;
                    __qsort::sSwap4(pc, pd, 1);
                    pd -= 1;
                }
                pc -= 1;
            }

            if (pb > pc)
                break;

            __qsort::sSwap4(pb, pc, 1);
            swap_cnt = 1;
            pb += 1;
            pc -= 1;
        }

        if (swap_cnt == 0)  // Switch to insertion sort
        {
            for (pm = (u32*)a + 1; pm < (u32*)a + n * 1; pm += 1)
                for (pl = pm; pl > (u32*)a && cmp(pl - 1, pl, data) > 0; pl -= 1)
                    __qsort::sSwap4(pl, pl - 1, 1);
            return;
        }

        pn = (u32*)a + n * 1;
        r  = (s32)(__qsort::minimum(pa - (u32*)a, pb - pa));
        __qsort::sSwap4((u32*)a, pb - r, r);
        r = (s32)(__qsort::minimum(pd - pc, pn - pd - 1));
        __qsort::sSwap4(pb, pn - r, r);

        if ((r = (s32)(pb - pa)) > 1)
            g_qsort4((u32*)a, r / 1, cmp, data);

        if ((r = (s32)(pd - pc)) > 1)
        {
            // Iterate rather than recurse to save stack space
            a = pn - r;
            n = r;
            goto loop;
        }
    }

    void g_qsort8(u64* a, s32 n, s32 (*cmp)(const void*, const void*, void*), void* data)
    {
        u64 *pa, *pb, *pc, *pd, *pl, *pm, *pn;
        s32  d, r, swaptype, swap_cnt;
    loop:
        swap_cnt = 0;
        if (n < 7)
        {
            for (pm = (u64*)a + 1; pm < (u64*)a + n * 1; pm += 1)
                for (pl = pm; pl > (u64*)a && cmp(pl - 1, pl, data) > 0; pl -= 1)
                    __qsort::sSwap8(pl, pl - 1, 1);
            return;
        }
        pm = (u64*)a + (n / 2) * 1;
        if (n > 7)
        {
            pl = (u64*)a;
            pn = (u64*)a + (n - 1) * 1;
            if (n > 40)
            {
                d  = (n / 8) * 1;
                pl = __qsort::sMed3(pl, pl + d, pl + 2 * d, cmp, data);
                pm = __qsort::sMed3(pm - d, pm, pm + d, cmp, data);
                pn = __qsort::sMed3(pn - 2 * d, pn - d, pn, cmp, data);
            }
            pm = __qsort::sMed3(pl, pm, pn, cmp, data);
        }
        __qsort::sSwap8((u64*)a, pm, 1);
        pa = pb = (u64*)a + 1;
        pc = pd = (u64*)a + (n - 1) * 1;
        for (;;)
        {
            while (pb <= pc && (r = cmp(pb, (u64*)a, data)) <= 0)
            {
                if (r == 0)
                {
                    swap_cnt = 1;
                    __qsort::sSwap8(pa, pb, 1);
                    pa += 1;
                }
                pb += 1;
            }

            while (pb <= pc && (r = cmp(pc, (u64*)a, data)) >= 0)
            {
                if (r == 0)
                {
                    swap_cnt = 1;
                    __qsort::sSwap8(pc, pd, 1);
                    pd -= 1;
                }
                pc -= 1;
            }

            if (pb > pc)
                break;

            __qsort::sSwap8(pb, pc, 1);
            swap_cnt = 1;
            pb += 1;
            pc -= 1;
        }

        if (swap_cnt == 0)  // Switch to insertion sort
        {
            for (pm = (u64*)a + 1; pm < (u64*)a + n * 1; pm += 1)
                for (pl = pm; pl > (u64*)a && cmp(pl - 1, pl, data) > 0; pl -= 1)
                    __qsort::sSwap8(pl, pl - 1, 1);
            return;
        }

        pn = (u64*)a + n * 1;
        r  = (s32)(__qsort::minimum(pa - (u64*)a, pb - pa));
        __qsort::sSwap8((u64*)a, pb - r, r);
        r = (s32)(__qsort::minimum(pd - pc, pn - pd - 1));
        __qsort::sSwap8(pb, pn - r, r);

        if ((r = (s32)(pb - pa)) > 1)
            g_qsort8((u64*)a, r / 1, cmp, data);

        if ((r = (s32)(pd - pc)) > 1)
        {
            // Iterate rather than recurse to save stack space
            a = pn - r;
            n = r;
            goto loop;
        }
    }

    void g_qsort(void* a, s32 n, s32 es, s32 (*cmp)(const void*, const void*, void*), void* user_data)
	{
		switch (es)
		{
		case 2:
			g_qsort2((u16*)a, n, cmp, user_data);
			break;
		case 4:
			g_qsort4((u32*)a, n, cmp, user_data);
			break;
		case 8:
			g_qsort8((u64*)a, n, cmp, user_data);
			break;
		default:
			g_qsortN((u8*)a, n, es, cmp, user_data);
			break;
		}
	}

};  // namespace ncore
