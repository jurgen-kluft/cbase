#include "cbase/c_allocator.h"
#include "cbase/c_context.h"
#include "cbase/c_random.h"

#include "ccore/c_math.h"
#include "ccore/c_vmem.h"

namespace ncore
{
    // context data is a global context that holds thread data.
    struct context_data_t
    {
        asserthandler_t* m_assert_handler;   // assert handler for this thread
        alloc_t*         m_system_alloc;     // application life-time allocations
        alloc_t*         m_heap_alloc;       // system life-time allocations
        frame_alloc_t*   m_frame_allocator;  // specific duration tagged allocations
        stack_alloc_t*   m_stack_alloc;      // function/temporary life-time allocations
        random_t*        m_random;           //
        vmem_arena_t*    m_vmem_arena;       //
        void*            m_slot0;            //
    };

    thread_local context_data_t* sThreadLocalContext = nullptr;

    static context_t s_get_context()
    {
        if (sThreadLocalContext == nullptr)
        {
            vmem_arena_t arena;
            arena.reserved(32 * cMB);

            int_t commit = 0;
            commit += math::g_alignUp((s32)sizeof(vmem_arena_t), arena.m_alignment);
            commit += math::g_alignUp((s32)sizeof(context_data_t), arena.m_alignment);
            commit += math::g_alignUp((s32)sizeof(vmem_alloc_t), arena.m_alignment);
            commit += math::g_alignUp((s32)sizeof(wyrand_t), arena.m_alignment);
            arena.committed(commit);

            vmem_arena_t*   vmem_arena   = (vmem_arena_t*)arena.commit_and_zero(sizeof(vmem_arena_t));
            context_data_t* context_data = (context_data_t*)arena.commit_and_zero(sizeof(context_data_t));
            vmem_alloc_t*   system_alloc = new (arena.commit_and_zero(sizeof(vmem_alloc_t))) vmem_alloc_t();
            wyrand_t*       rnd          = new (arena.commit_and_zero(sizeof(wyrand_t))) wyrand_t();
            rnd->reset((u64)arena.m_base);

            *vmem_arena                  = arena;
            system_alloc->m_vmem         = vmem_arena;
            context_data->m_vmem_arena   = vmem_arena;
            context_data->m_system_alloc = system_alloc;
            context_data->m_random       = rnd;

            sThreadLocalContext = context_data;
        }
        return context_t{sThreadLocalContext};
    }

    static void s_release_context()
    {
        if (sThreadLocalContext != nullptr)
        {
            vmem_arena_t a = *sThreadLocalContext->m_vmem_arena;
            a.release();
            sThreadLocalContext = nullptr;
        }
    }

    context_t g_current_context() { return s_get_context(); }
    void      g_release_context() { s_release_context(); }

    asserthandler_t* context_t::assert_handler()
    {
        context_data_t* ctx = sThreadLocalContext;
        return ctx->m_assert_handler;
    }
    alloc_t* context_t::system_alloc()
    {
        context_data_t* ctx = sThreadLocalContext;
        return ctx->m_system_alloc;
    }
    alloc_t* context_t::heap_alloc()
    {
        context_data_t* ctx = sThreadLocalContext;
        return ctx->m_heap_alloc;
    }
    frame_alloc_t* context_t::frame_allocator()
    {
        context_data_t* ctx = sThreadLocalContext;
        return ctx->m_frame_allocator;
    }
    stack_alloc_t* context_t::stack_alloc()
    {
        context_data_t* ctx = sThreadLocalContext;
        return ctx->m_stack_alloc;
    }
    random_t* context_t::random()
    {
        context_data_t* ctx = sThreadLocalContext;
        return ctx->m_random;
    }
    void* context_t::slot0()
    {
        context_data_t* ctx = sThreadLocalContext;
        return ctx->m_slot0;
    }

    void context_t::set_assert_handler(asserthandler_t* p)
    {
        context_data_t* ctx   = sThreadLocalContext;
        ctx->m_assert_handler = p;
    }
    void context_t::set_system_alloc(alloc_t* a)
    {
        context_data_t* ctx = sThreadLocalContext;
        ctx->m_system_alloc = a;
    }
    void context_t::set_heap_alloc(alloc_t* a)
    {
        context_data_t* ctx = sThreadLocalContext;
        ctx->m_heap_alloc   = a;
    }
    void context_t::set_frame_alloc(frame_alloc_t* a)
    {
        context_data_t* ctx    = sThreadLocalContext;
        ctx->m_frame_allocator = a;
    }
    void context_t::set_stack_alloc(stack_alloc_t* a)
    {
        context_data_t* ctx = sThreadLocalContext;
        ctx->m_stack_alloc  = a;
    }
    void context_t::set_random(random_t* r)
    {
        context_data_t* ctx = sThreadLocalContext;
        ctx->m_random       = r;
    }
    void context_t::set_slot0(void* p)
    {
        context_data_t* ctx = sThreadLocalContext;
        ctx->m_slot0        = p;
    }

}  // namespace ncore
