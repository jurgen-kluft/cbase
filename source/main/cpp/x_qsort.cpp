//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_qsort.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//----------------------------------------------------------------------------------------------------------------
	// Custom QuickSort
	namespace __xqsort
	{
		static inline xbyte*			sMed3(xbyte*, xbyte*, xbyte*, s32(*)(const void* const, const void* const, u32), u32);
		static inline void				sSwapFunc(xbyte*, xbyte*, s32, s32);

		#define __xqsort_MIN(a, b)	(a) < (b) ? a : b

		#define __xqsort_SwapCode(TYPE, parmi, parmj, n) { 		\
			u32 i = (n) / sizeof (TYPE); 						\
			register TYPE *pi = (TYPE *) (parmi); 				\
			register TYPE *pj = (TYPE *) (parmj); 				\
			do { 												\
				register TYPE	t = *pi;						\
				*pi++ = *pj;									\
				*pj++ = t;										\
			} while (--i > 0);									\
		}

		// Determine the swap functionality to use
		// single byte copy
		// 4-byte copy
		#define __xqsort_SWAPINIT(a, es) swaptype = (((xbyte*)a - (xbyte*)0) % sizeof(u32) || es % sizeof(u32)) ? 2 : es == sizeof(u32)? 0 : 1;

		static inline void sSwapFunc(xbyte* a, xbyte* b, s32 n, s32 swaptype)
		{
			if (swaptype <= 1) 
				__xqsort_SwapCode(u32, a, b, n)
			else
				__xqsort_SwapCode(xbyte, a, b, n)
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

		static inline xbyte* sMed3(xbyte* a, xbyte* b, xbyte* c,
			s32 (*cmp)(const void* const, const void* const, u32), u32 data)
		{
			return cmp(a, b, data) < 0 ? (cmp(b, c, data) < 0 ? b : (cmp(a, c, data) < 0 ? c : a )) : (cmp(b, c, data) > 0 ? b : (cmp(a, c, data) < 0 ? a : c ));
		}
	}

	void xqsort(void *a /*element_array*/,
		s32 n /*element_count*/,
		s32 es /*element_size*/,
		s32 (*cmp)(const void* const, const void* const, u32),
		u32 data)
	{
		xbyte *pa, *pb, *pc, *pd, *pl, *pm, *pn;
		s32 d, r, swaptype, swap_cnt;

	loop:	
		__xqsort_SWAPINIT(a, es);
		swap_cnt = 0;
		if (n < 7)
		{
			for (pm = (xbyte*) a + es; pm < (xbyte*) a + n * es; pm += es)
				for (pl = pm; pl > (xbyte*) a && cmp(pl - es, pl, data) > 0; pl -= es)
					__xqsort_Swap(pl, pl - es);
			return;
		}
		pm = (xbyte*) a + (n / 2) * es;
		if (n > 7)
		{
			pl = (xbyte*) a;
			pn = (xbyte*) a + (n - 1) * es;
			if (n > 40)
			{
				d = (n / 8) * es;
				pl = __xqsort::sMed3(pl, pl + d, pl + 2 * d, cmp, data);
				pm = __xqsort::sMed3(pm - d, pm, pm + d, cmp, data);
				pn = __xqsort::sMed3(pn - 2 * d, pn - d, pn, cmp, data);
			}
			pm = __xqsort::sMed3(pl, pm, pn, cmp, data);
		}
		__xqsort_Swap((xbyte*) a, pm);
		pa = pb = (xbyte*) a + es;
		pc = pd = (xbyte*) a + (n - 1) * es;
		for (;;)
		{
			while (pb <= pc && (r = cmp(pb, (xbyte*)a, data)) <= 0)
			{
				if (r == 0)
				{
					swap_cnt = 1;
					__xqsort_Swap(pa, pb);
					pa += es;
				}
				pb += es;
			}

			while (pb <= pc && (r = cmp(pc, (xbyte*)a, data)) >= 0)
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
			for (pm = (xbyte*) a + es; pm < (xbyte*) a + n * es; pm += es)
				for (pl = pm; pl > (xbyte*) a && cmp(pl - es, pl, data) > 0; pl -= es)
					__xqsort_Swap(pl, pl - es);
			return;
		}

		pn = (xbyte*) a + n * es;
		r = (s32)(__xqsort_MIN(pa - (xbyte*)a, pb - pa));
		__xqsort_VecSwap((xbyte*)a, pb - r, r);
		r = (s32)(__xqsort_MIN(pd - pc, pn - pd - es));
		__xqsort_VecSwap(pb, pn - r, r);

		if ((r = (s32)(pb - pa)) > es)
			xqsort((xbyte*)a, r / es, es, cmp, data);
	
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
	// END xCore namespace
	//==============================================================================
};

