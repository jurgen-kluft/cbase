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

    //==============================================================================
    // A context per thread
    //==============================================================================
    class context_t
    {
    public:
        enum
        {
            ASSERT_HANDLER    = 0,
            SYSTEM_ALLOCATOR  = 1,
            RUNTIME_ALLOCATOR = 2,
            FRAME_ALLOCATOR   = 3,
            LOCAL_ALLOCATOR   = 4,
            RANDOM_GENERATOR  = 5,
            EMPTY_SLOT_0      = 6,
            EMPTY_SLOT_1      = 7,
            EMPTY_SLOT_2      = 8,
        };

        static void init(s32 max_num_threads, s32 max_num_slots, alloc_t* allocator);
        static void exit(alloc_t* allocator);

        static s32 register_thread();
        static s32 thread_index();
        static s32 max_threads();

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
            get<alloc_t>(tidx, SYSTEM_ALLOCATOR, a);
            return a;
        }

        static alloc_t* runtime_alloc()
        {
            s32 const tidx = thread_index();
            alloc_t*  a;
            get<alloc_t>(tidx, RUNTIME_ALLOCATOR, a);
            return a;
        }

        static alloc_t* frame_allocator()
        {
            s32 const tidx = thread_index();
            alloc_t*  a;
            get<alloc_t>(tidx, FRAME_ALLOCATOR, a);
            return a;
        }

        static alloc_t* local_alloc()
        {
            s32 const tidx = thread_index();
            alloc_t*  a;
            get<alloc_t>(tidx, LOCAL_ALLOCATOR, a);
            return a;
        }

        static alloc_t* global_alloc() { return runtime_alloc(); }   // application life-time allocations
        static alloc_t* heap_alloc() { return runtime_alloc(); }     // long life-time allocations
        static alloc_t* frame_alloc() { return frame_allocator(); }  // specific duration tagged allocations
        static alloc_t* stack_alloc() { return local_alloc(); }      // function life-time allocations

        class gpu_alloc_t;
        static gpu_alloc_t* gpu_alloc() { return nullptr; }  // GPU resource allocations?

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
            set<alloc_t>(tidx, SYSTEM_ALLOCATOR, a);
        }

        static void set_runtime_alloc(alloc_t* a)
        {
            s32 const tidx = thread_index();
            set<alloc_t>(tidx, RUNTIME_ALLOCATOR, a);
        }

        static void set_frame_alloc(alloc_t* a)
        {
            s32 const tidx = thread_index();
            set<alloc_t>(tidx, FRAME_ALLOCATOR, a);
        }

        static void set_local_alloc(alloc_t* a)
        {
            s32 const tidx = thread_index();
            set<alloc_t>(tidx, LOCAL_ALLOCATOR, a);
        }

        static void set_random(random_t* r)
        {
            s32 const tidx = thread_index();
            set<random_t>(tidx, RANDOM_GENERATOR, r);
        }

        template <class T>
        inline static void set(s32 tidx, s32 slot, T* inData)
        {
            vset(tidx, slot, (void*)inData);
        }
        template <class T>
        inline static void get(s32 tidx, s32 slot, T*& outData)
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
}  // namespace ncore

#endif  // __CBASE_THREAD_CONTEXT_H__
