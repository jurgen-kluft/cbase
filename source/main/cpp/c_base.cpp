#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_console.h"
#include "cbase/c_debug.h"
#include "cbase/c_random.h"
#include "cbase/c_runes.h"
#include "cbase/c_context.h"

namespace ncore
{
    void* g_malloc(u32 size, u32 align)
    {
        context_t thread    = g_current_context();
        alloc_t*  allocator = thread.heap_alloc();
        return allocator->allocate(size, align);
    }

    void g_free(void* ptr)
    {
        context_t thread    = g_current_context();
        alloc_t*  allocator = thread.heap_alloc();
        allocator->deallocate(ptr);
    }
}  // namespace ncore

namespace cbase
{
    void init()
    {
        ncore::g_init_system_alloc();
        ncore::alloc_t* system_allocator = ncore::g_get_system_alloc();

        ncore::console_t::init_default_console();
        ncore::context_t context = ncore::g_current_context();

        // The assert handler, system and string allocator are thread safe
        ncore::asserthandler_t* assert_handler = gSetAssertHandler();

        ncore::stack_alloc_t* stack_allocator = nullptr;
        // TODO create stack allocator

        ncore::alloc_t* heap_allocator = nullptr;
        // TODO create heap allocator

        ncore::frame_alloc_t* frame_allocator = nullptr;
        // TODO create frame allocator

        ncore::random_t* random_generator = new (system_allocator) ncore::wyrand_t();
        random_generator->reset((ncore::s64)random_generator);  // randomize the seed

        context.set_assert_handler(assert_handler);
        context.set_system_alloc(system_allocator);
        context.set_heap_alloc(heap_allocator);
        context.set_frame_alloc(frame_allocator);
        context.set_stack_alloc(stack_allocator);
        context.set_random(random_generator);
    }

    void exit()
    {
        ncore::alloc_t* system_alloc = ncore::g_get_system_alloc();

        ncore::g_exit_system_alloc();
        system_alloc = nullptr;

        ncore::context_t context = ncore::g_current_context();
        context.set_assert_handler(nullptr);
        ncore::g_release_context();
    }

};  // namespace cbase
