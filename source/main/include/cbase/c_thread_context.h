#ifndef __CBASE_THREAD_CONTEXT_H__
#define __CBASE_THREAD_CONTEXT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    class alloc_t;
    class asserthandler_t;
    class stack_alloc_t;
    class random_t;

    struct thread_context_data_t;

    class thread_context_t
    {
    public:
        static thread_context_t* current();  // returns the current thread context
        static void              release();  // releases the current thread context

        asserthandler_t* assert_handler();   // assert handler for this thread
        alloc_t*         system_alloc();     // application life-time allocations
        alloc_t*         runtime_alloc();    // system life-time allocations
        alloc_t*         frame_allocator();  // specific duration tagged allocations
        alloc_t*         local_alloc();      // function/temporary life-time allocations
        random_t*        random();           // random number generator
        void*            slot0();            // user defined slot 0
        void*            slot1();            // user defined slot 1

        void set_assert_handler(asserthandler_t* p);
        void set_system_alloc(alloc_t* a);
        void set_runtime_alloc(alloc_t* a);
        void set_frame_alloc(alloc_t* a);
        void set_local_alloc(alloc_t* a);
        void set_random(random_t* r);
        void set_slot0(void* p);
        void set_slot1(void* p);

        thread_context_data_t* m_data;
    };

}  // namespace ncore

#endif  // __CBASE_THREAD_CONTEXT_H__
