#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_console.h"
#include "cbase/c_debug.h"
#include "cbase/c_random.h"
#include "cbase/c_runes.h"
#include "cbase/c_thread_context.h"

namespace ncore
{
    void* g_malloc(u32 size, u32 align)
    {
        thread_context_t* thread = thread_context_t::current();
        alloc_t* allocator = thread->system_alloc();
        return allocator->allocate(size, align);
    }

    void g_free(void* ptr)
    {
        thread_context_t* thread = thread_context_t::current();
        alloc_t* allocator = thread->system_alloc();
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
        ncore::thread_context_t* context = ncore::thread_context_t::current();

        // The assert handler, system and string allocator are thread safe
        ncore::asserthandler_t* assert_handler = gSetAssertHandler();
        {
            //ncore::byte* buffer_data = (ncore::byte*)system_allocator->allocate((ncore::u32)temporary_allocator_size);
            ncore::byte* buffer_data = nullptr;

            ncore::fixed_buffer_alloc_t* stack_allocator = system_allocator->construct<ncore::fixed_buffer_alloc_t>();
            stack_allocator->setup(buffer_data, ncore::cMB * 8);

            ncore::random_t* random_generator = system_allocator->construct<ncore::wyrand_t>();
            random_generator->reset((ncore::s64)random_generator);  // randomize the seed

            context->set_assert_handler(assert_handler);
            context->set_runtime_alloc(system_allocator);
            context->set_frame_alloc(system_allocator);
            context->set_local_alloc(stack_allocator);
            context->set_random(random_generator);
        }
    }

    void exit()
    {
        ncore::alloc_t* system_alloc = ncore::g_get_system_alloc();

        ncore::g_exit_system_alloc();
        system_alloc = nullptr;

        ncore::thread_context_t* context = ncore::thread_context_t::current();
        context->set_assert_handler(nullptr);
        ncore::thread_context_t::release();
    }

};  // namespace cbase
