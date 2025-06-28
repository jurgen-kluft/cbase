#ifndef __CBASE_CONTEXT_H__
#define __CBASE_CONTEXT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    class asserthandler_t;
    class random_t;
    class alloc_t;
    class stack_alloc_t;
    class frame_alloc_t;

    struct context_data_t;
    struct context_t
    {
        asserthandler_t* assert_handler();   // assert handler for this thread
        alloc_t*         system_alloc();     // application life-time allocations
        alloc_t*         heap_alloc();       // system life-time allocations
        frame_alloc_t*   frame_allocator();  // specific duration tagged allocations
        stack_alloc_t*   stack_alloc();      // function/temporary life-time allocations
        random_t*        random();           // random number generator
        void*            slot0();            // user defined slot 0

        void set_assert_handler(asserthandler_t* p);
        void set_system_alloc(alloc_t* a);
        void set_heap_alloc(alloc_t* a);
        void set_frame_alloc(frame_alloc_t* a);
        void set_stack_alloc(stack_alloc_t* a);
        void set_random(random_t* r);
        void set_slot0(void* p);

        context_data_t* m_data;
    };

    context_t g_current_context();  // returns the current thread context
    void      g_release_context();  // releases the current thread context

}  // namespace ncore

#endif  // __CBASE_THREAD_CONTEXT_H__
