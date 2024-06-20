//==============================================================================
// INCLUDES
//==============================================================================
#include "cbase/c_qsort.h"

//==============================================================================
// ccore namespace
//==============================================================================
namespace ncore
{
	//----------------------------------------------------------------------------------------------------------------
	// Custom QuickSort
	namespace __xqsort
	{
		static inline u8*			sMed3(u8*, u8*, u8*, s32(*)(const void* const, const void* const, void*), void*);
		static inline void			sSwapFunc(u8*, u8*, s32, s32);

		#define __xqsort_MIN(a, b)	(a) < (b) ? a : b

		#define __xqsort_SwapCode(TYPE, parmi, parmj, n) { 		\
			s32 i = (n) / (s32)sizeof (TYPE); 						\
			TYPE *pi = (TYPE *) (parmi); 						\
			TYPE *pj = (TYPE *) (parmj); 						\
			do { 												\
				TYPE	t = *pi;								\
				*pi++ = *pj;									\
				*pj++ = t;										\
			} while (--i > 0);									\
		}

		// Determine the swap functionality to use
		// single byte copy
		// 4-byte copy
		#define __xqsort_SWAPINIT(a, es) swaptype = ((u64)a % sizeof(u32) || (es) % (s32)sizeof(u32)) ? 2 : (es) == (s32)sizeof(u32)? 0 : 1

		static inline void sSwapFunc(u8* a, u8* b, s32 n, s32 swaptype)
		{
			if (swaptype <= 1)
				__xqsort_SwapCode(u32, a, b, n)
			else
				__xqsort_SwapCode(u8, a, b, n)
		}

		#define __xqsort_Swap(a, b)							\
			if (swaptype == 0) {							\
				u32 t = *(u32 *)(a);						\
				*(u32 *)(a) = *(u32 *)(b);					\
				*(u32 *)(b) = t;							\
			}												\
			else {											\
				__xqsort::sSwapFunc(a, b, es, swaptype);	\
			}


		#define __xqsort_VecSwap(a, b, n) 	if ((n) > 0) __xqsort::sSwapFunc(a, b, n, swaptype)

		static inline u8* sMed3(u8* a, u8* b, u8* c, s32 (*cmp)(const void* const, const void* const, void*), void*data)
		{
			return cmp(a, b, data) < 0 ? (cmp(b, c, data) < 0 ? b : (cmp(a, c, data) < 0 ? c : a )) : (cmp(b, c, data) > 0 ? b : (cmp(a, c, data) < 0 ? a : c ));
		}
	}

	void g_qsort(void *a, s32 n, s32 es, s32 (*cmp)(const void*, const void*, void*), void* data)
	{
		u8 *pa, *pb, *pc, *pd, *pl, *pm, *pn;
		s32 d, r, swaptype, swap_cnt;
	loop:
		__xqsort_SWAPINIT(a, es);
        swap_cnt = 0;
		if (n < 7)
		{
			for (pm = (u8*) a + es; pm < (u8*) a + n * es; pm += es)
				for (pl = pm; pl > (u8*) a && cmp(pl - es, pl, data) > 0; pl -= es)
					__xqsort_Swap(pl, pl - es);
			return;
		}
		pm = (u8*) a + (n / 2) * es;
		if (n > 7)
		{
			pl = (u8*) a;
			pn = (u8*) a + (n - 1) * es;
			if (n > 40)
			{
				d = (n / 8) * es;
				pl = __xqsort::sMed3(pl, pl + d, pl + 2 * d, cmp, data);
				pm = __xqsort::sMed3(pm - d, pm, pm + d, cmp, data);
				pn = __xqsort::sMed3(pn - 2 * d, pn - d, pn, cmp, data);
			}
			pm = __xqsort::sMed3(pl, pm, pn, cmp, data);
		}
		__xqsort_Swap((u8*) a, pm);
		pa = pb = (u8*) a + es;
		pc = pd = (u8*) a + (n - 1) * es;
		for (;;)
		{
			while (pb <= pc && (r = cmp(pb, (u8*)a, data)) <= 0)
			{
				if (r == 0)
				{
					swap_cnt = 1;
					__xqsort_Swap(pa, pb);
					pa += es;
				}
				pb += es;
			}

			while (pb <= pc && (r = cmp(pc, (u8*)a, data)) >= 0)
			{
				if (r == 0)
				{
					swap_cnt = 1;
					__xqsort_Swap(pc, pd);
					pd -= es;
				}
				pc -= es;
			}

			if (pb > pc)
				break;

			__xqsort_Swap(pb, pc);
			swap_cnt = 1;
			pb += es;
			pc -= es;
		}

		if (swap_cnt == 0)	// Switch to insertion sort
		{
			for (pm = (u8*) a + es; pm < (u8*) a + n * es; pm += es)
				for (pl = pm; pl > (u8*) a && cmp(pl - es, pl, data) > 0; pl -= es)
					__xqsort_Swap(pl, pl - es);
			return;
		}

		pn = (u8*) a + n * es;
		r = (s32)(__xqsort_MIN(pa - (u8*)a, pb - pa));
		__xqsort_VecSwap((u8*)a, pb - r, r);
		r = (s32)(__xqsort_MIN(pd - pc, pn - pd - es));
		__xqsort_VecSwap(pb, pn - r, r);

		if ((r = (s32)(pb - pa)) > es)
			g_qsort((u8*)a, r / es, es, cmp, data);

		if ((r = (s32)(pd - pc)) > es)
		{
			// Iterate rather than recurse to save stack space
			a = pn - r;
			n = r / es;
			goto loop;
		}
	}

	#undef __xqsort_VecSwap
	#undef __xqsort_Swap
	#undef __xqsort_SWAPINIT
	#undef __xqsort_SwapCode
	#undef __xqsort_MIN

	//==============================================================================
	// END ccore namespace
	//==============================================================================
};

