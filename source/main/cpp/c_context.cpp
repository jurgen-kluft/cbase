#include "cbase/c_allocator.h"
#include "cbase/c_context.h"

#include "ccore/c_math.h"
#include "ccore/c_random.h"
#include "ccore/c_arena.h"

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
        arena_t*         m_arena;            //
        void*            m_slot0;            //
    };

    thread_local context_data_t* sThreadLocalContext = nullptr;

    static context_t s_get_context()
    {
        if (sThreadLocalContext == nullptr)
        {
            arena_t* arena = narena::new_arena(4 * cMB, 0);

            int_t commit = 0;
            commit += math::alignUp((s32)sizeof(arena_t), narena::alignment(arena));
            commit += math::alignUp((s32)sizeof(context_data_t), narena::alignment(arena));
            commit += math::alignUp((s32)sizeof(narena::aalloc_t), narena::alignment(arena));
            commit += math::alignUp((s32)sizeof(rand_t), narena::alignment(arena));
            narena::commit(arena, commit);

            context_data_t*   context_data = (context_data_t*)narena::alloc_and_zero(arena, sizeof(context_data_t));
            narena::aalloc_t* system_alloc = new (narena::alloc_and_zero(arena, sizeof(narena::aalloc_t))) narena::aalloc_t();
            rand_t*           rnd          = new (narena::alloc_and_zero(arena, sizeof(rand_t))) rand_t();
            rnd->reset((u64)arena);

            // TODO
            // - heap allocator
            // - frame allocator
            // - stack allocator

            system_alloc->m_vmem         = arena;
            context_data->m_arena        = arena;
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
            narena::destroy(sThreadLocalContext->m_arena);
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
