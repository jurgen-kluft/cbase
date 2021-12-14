#ifndef __XBASE_THREAD_CONTEXT_H__
#define __XBASE_THREAD_CONTEXT_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
    class asserthandler_t;
    class alloc_t;
    class runes_alloc_t;
    class stack_alloc_t;
    class random_t;

    //==============================================================================
    //
    // The thread context
    //
    //==============================================================================
    class context_t
    {
    public:
        enum
        {
            ASSERT_HANDLER   = 0,
            HEAP_ALLOCATOR   = 1,
            STACK_ALLOCATOR  = 2,
            STRING_ALLOCATOR = 3,
            RANDOM_GENERATOR = 4,
        };

        static void init(s32 max_num_threads, s32 max_num_slots, alloc_t* allocator);
        static void exit(alloc_t* allocator);

        static s32  register_thread();
        static s32  thread_index();

        static asserthandler_t* assert_handler()
        {
            s32 const        tidx = thread_index();
            asserthandler_t* h;
            get<asserthandler_t>(tidx, ASSERT_HANDLER, h);
            return h;
        }

        static alloc_t* system_alloc()
        {
            s32 const tidx = thread_index();
            alloc_t*  a;
            get<alloc_t>(tidx, HEAP_ALLOCATOR, a);
            return a;
        }

        static runes_alloc_t* string_alloc()
        {
            s32 const      tidx = thread_index();
            runes_alloc_t* a;
            get<runes_alloc_t>(tidx, STRING_ALLOCATOR, a);
            return a;
        }

        static alloc_t* stack_alloc()
        {
            s32 const tidx = thread_index();
            alloc_t*  a;
            get<alloc_t>(tidx, STACK_ALLOCATOR, a);
            return a;
        }

        static random_t* random()
        {
            s32 const tidx = thread_index();
            random_t* r;
            get<random_t>(tidx, RANDOM_GENERATOR, r);
            return r;
        }

        static void set_assert_handler(asserthandler_t* p)
        {
            s32 const tidx = thread_index();
            set<asserthandler_t>(tidx, ASSERT_HANDLER, p);
        }

        static void set_system_alloc(alloc_t* a)
        {
            s32 const tidx = thread_index();
            set<alloc_t>(tidx, HEAP_ALLOCATOR, a);
        }

        static void set_string_alloc(runes_alloc_t* a)
        {
            s32 const tidx = thread_index();
            set<runes_alloc_t>(tidx, STRING_ALLOCATOR, a);
        }

        static void set_stack_alloc(alloc_t* a)
        {
            s32 const tidx = thread_index();
            set<alloc_t>(tidx, STACK_ALLOCATOR, a);
        }

        static void set_random(random_t* r)
        {
            s32 const tidx = thread_index();
            set<random_t>(tidx, RANDOM_GENERATOR, r);
        }

        template <class T> inline static void set(s32 tidx, s32 slot, T* inData) { vset(tidx, slot, reinterpret_cast<void*>(inData)); }
        template <class T> inline static void get(s32 tidx, s32 slot, T*& outData)
        {
            void* p = nullptr;
            vget(tidx, slot, p);
            outData = (T*)p;
        }

    protected:
        static void vset(s32 tidx, s32 SLOT, void* inData);
        static void vget(s32 tidx, s32 SLOT, void*& outData);

        static void** m_slots;
        static s32    m_max_num_threads;
        static s32    m_max_num_slots;
    };
} // namespace xcore

#endif // __XBASE_THREAD_CONTEXT_H__
