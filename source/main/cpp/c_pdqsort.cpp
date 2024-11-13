#include "ccore/c_target.h"

namespace ncore
{
    // *****************************************************************************
    // * pdqsort.c -- A public domain implementation of an iterative version of    *
    // * the QuickSort algorithm with "Median of Three" pivot selection,           *
    // * "end recursion removal", and the use of an Insertion Sort for small       *
    // * partitions. Many people have "improved" C. A. R. Hoare's original         *
    // * QuickSort algorithm. This implementation follows Robert Sedgewick's       *
    // * ("Algorithms in C", Robert Sedgewick, Addison-Wesley, 1990,               *
    // * ISBN 0-201-51425-7) improvements.                                         *
    // *                                                                           *
    // * An iterative qsort() is faster and requires less stack space than the     *
    // * recursive implementation. If "end recursion removal" is employed, the     *
    // * maximum number of stack entries required will be no more than log to the  *
    // * base 2 of the number of elements in the array being sorted.               *
    // *                                                                           *
    // * Sedgewick states that the use of an Insertion Sort for partitions smaller *
    // * than M elements, where M is a constant between 5 and 25, results in       *
    // * about a 20% improvement in the efficiency of the sort.                    *
    // *****************************************************************************

    //  ****************************************************************************
    //  * Swap() -- The routine used by the main qsort() routine to swap elements  *
    //  * of the array when necessary. To save time, Swap() will swap in integer   *
    //  * units until there is less than a full integer left. Then it will swap    *
    //  * in byte units until the entire element has been swapped.                 *
    //  ****************************************************************************

    static void Swap(void *a, void *b, u32 intCount, u32 byteCount)
    {
        u32 tmp;
        u32 i;

        for (i = 0; i < intCount; i++)
        {
            tmp         = *((u32 *)a);
            *((u32 *)a) = *((u32 *)b);
            *((u32 *)b) = tmp;
            ((u32 *&)a)++;
            ((u32 *&)b)++;
        }
        for (i = 0; i < byteCount; i++)
        {
            tmp        = *((u8 *)a);
            *((u8 *)a) = *((u8 *)b);
            *((u8 *)b) = (u8)tmp;
            ((u8 *&)a)++;
            ((u8 *&)b)++;
        }
    }

    const int M = 8;  // The threshold for Insertion

    //  *****************************************************************************
    //  * The main qsort() routine. This implementation is fully compatible with    *
    //  * the standard qsort() routine in the ANSI C run time libraries.            *
    //  *                                                                           *
    //  * base is the base address of the array to be sorted, nelem is the number of*
    //  * entries in the array to be sorted, width is the width of a single element *
    //  * in bytes, and fcmp is a pointer to a function that performs the comparison*
    //  * between elements of the array.                                            *
    //  *                                                                           *
    //  * The pointer L scans partitions from the Left, the pointer R scans         *
    //  * partitions from the right, Limit is a pointer that serves as a sentinel   *
    //  * to stop the scan on the right (the pivot element is stored at base and    *
    //  * serves as the left end sentinel). InsertThresh is the M constant          *
    //  * converted to a pointer for comparisons.                                   *
    //  *****************************************************************************
    void qsort(void *_base, u32 nelem, u32 width, int (*fcmp)(const void *a, const void *b))
    {
        char    *base = (char *)_base;
        char    *L, *R, *Limit;
        unsigned InsertThresh;

        //  ***************************************************************************
        //  * The stack used to push the bounds of the larger partition. Thirty-two   *
        //  * entries will allow for sorting an array with a little over 4 million    *
        //  * entries!                                                                *
        //  ***************************************************************************
        struct
        {
            char *Base;
            char *Limit;
        } Stack[32], *Sptr;

        // Initialization
        const u32 IntCount  = width >> 2;
        const u32 ByteCount = width & 3;

        InsertThresh = M * width;
        Sptr         = Stack;
        Limit        = (char *)base + nelem * width;

        while (true)
        {
            while ((Limit - base) > InsertThresh)
            {
                L = (char *)base + width;
                R = Limit - width;

                //  *************************************************************************
                //  * The following code implements Sedgewick's "Median of Three" selection *
                //  * for the pivot element. The central element is selected as the first   *
                //  * tentative pivot                                                       *
                //  *************************************************************************

                Swap(((unsigned)(Limit - base) >> 1) - ((((unsigned)(Limit - (char *)base) >> 1)) % width) + (char *)base, base, IntCount, ByteCount);
                if ((*fcmp)(L, R) > 0)
                    Swap(L, R, IntCount, ByteCount);
                if ((*fcmp)(base, R) > 0)
                    Swap(base, R, IntCount, ByteCount);
                if ((*fcmp)(L, base) > 0)
                    Swap(L, base, IntCount, ByteCount);

                //  *************************************************
                //  * The following code performs the partitioning  *
                //  *************************************************

                while (1)
                {
                    while ((*fcmp)((L += width), base) < 0) {}
                    while ((*fcmp)((R -= width), base) > 0) {}
                    if (L > R)
                        break;
                    Swap(L, R, IntCount, ByteCount);
                }
                Swap(base, R, IntCount, ByteCount);

                // **************************************************************************
                // * The following code performs the "end recursion removal". This is       *
                // * accomplished by stacking the larger partition and immediately sorting  *
                // * the smaller.                                                           *
                // **************************************************************************

                if (R - base > Limit - L)
                {
                    Sptr->Base  = base;
                    Sptr->Limit = R;
                    base        = L;
                }
                else
                {
                    Sptr->Base  = L;
                    Sptr->Limit = Limit;
                    Limit       = R;
                }
                Sptr++;
            }

            // *****************************************************************************
            // * The following code is the Insertion Sort used to sort partitions smaller  *
            // * the M elements.                                                           *
            // *****************************************************************************

            L = (char *)base + width;
            while (L < Limit)
            {
                R = L;
                while (R > base && (*fcmp)(R - width, R) > 0)
                {
                    Swap(R - width, R, IntCount, ByteCount);
                    R -= width;
                }
                L += width;
            }

            // ****************************************************************************
            // * If there are any partitions left on the stack, pop one off and sort it.  *
            // * Otherwise, the qsort is finished.                                        *
            // ****************************************************************************
            if (Sptr <= Stack)
                break;

            --Sptr;
            base  = Sptr->Base;
            Limit = Sptr->Limit;
        }
    }

}  // namespace ncore
