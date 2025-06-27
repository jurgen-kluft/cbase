#include "cbase/c_allocator.h"
#include "cbase/c_thread_context.h"
// #include "ccore/c_debug.h"

namespace ncore
{
    // Thread context is a global context that holds per-thread data.
    struct thread_context_data_t
    {
        asserthandler_t* m_assert_handler;   // assert handler for this thread
        alloc_t*         m_system_alloc;     // application life-time allocations
        alloc_t*         m_runtime_alloc;    // system life-time allocations
        alloc_t*         m_frame_allocator;  // specific duration tagged allocations
        alloc_t*         m_local_alloc;      // function/temporary life-time allocations
        random_t*        m_random;           //
        void*            m_slot0;            //
        void*            m_slot1;            //
    };

    thread_local thread_context_t* sThreadLocalContext = nullptr;

    static thread_context_t* s_get_context()
    {
        if (sThreadLocalContext == nullptr)
        {
            // create system allocator using virtual memory
        }
        return sThreadLocalContext;
    }
    static void s_release_context()
    {
        if (sThreadLocalContext != nullptr)
        {
            // release the thread context

            sThreadLocalContext = nullptr;
        }
    }

    thread_context_t* thread_context_t::current() { return s_get_context(); }
    void              thread_context_t::release() { s_release_context(); }

    asserthandler_t* thread_context_t::assert_handler()
    {
        thread_context_t* ctx = s_get_context();
        return ctx->m_data->m_assert_handler;
    }
    alloc_t* thread_context_t::system_alloc()
    {
        thread_context_t* ctx = s_get_context();
        return ctx->m_data->m_system_alloc;
    }
    alloc_t* thread_context_t::runtime_alloc()
    {
        thread_context_t* ctx = s_get_context();
        return ctx->m_data->m_runtime_alloc;
    }
    alloc_t* thread_context_t::frame_allocator()
    {
        thread_context_t* ctx = s_get_context();
        return ctx->m_data->m_frame_allocator;
    }
    alloc_t* thread_context_t::local_alloc()
    {
        thread_context_t* ctx = s_get_context();
        return ctx->m_data->m_local_alloc;
    }
    random_t* thread_context_t::random()
    {
        thread_context_t* ctx = s_get_context();
        return ctx->m_data->m_random;
    }
    void* thread_context_t::slot0()
    {
        thread_context_t* ctx = s_get_context();
        return ctx->m_data->m_slot0;
    }
    void* thread_context_t::slot1()
    {
        thread_context_t* ctx = s_get_context();
        return ctx->m_data->m_slot1;
    }

    void thread_context_t::set_assert_handler(asserthandler_t* p)
    {
        thread_context_t* ctx         = s_get_context();
        ctx->m_data->m_assert_handler = p;
    }
    void thread_context_t::set_system_alloc(alloc_t* a)
    {
        thread_context_t* ctx       = s_get_context();
        ctx->m_data->m_system_alloc = a;
    }
    void thread_context_t::set_runtime_alloc(alloc_t* a)
    {
        thread_context_t* ctx        = s_get_context();
        ctx->m_data->m_runtime_alloc = a;
    }
    void thread_context_t::set_frame_alloc(alloc_t* a)
    {
        thread_context_t* ctx          = s_get_context();
        ctx->m_data->m_frame_allocator = a;
    }
    void thread_context_t::set_local_alloc(alloc_t* a)
    {
        thread_context_t* ctx      = s_get_context();
        ctx->m_data->m_local_alloc = a;
    }
    void thread_context_t::set_random(random_t* r)
    {
        thread_context_t* ctx = s_get_context();
        ctx->m_data->m_random = r;
    }
    void thread_context_t::set_slot0(void* p)
    {
        thread_context_t* ctx = s_get_context();
        ctx->m_data->m_slot0  = p;
    }
    void thread_context_t::set_slot1(void* p)
    {
        thread_context_t* ctx = s_get_context();
        ctx->m_data->m_slot1  = p;
    }

}  // namespace ncore
