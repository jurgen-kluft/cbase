#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_console.h"
#include "cbase/c_context.h"
#include "cbase/c_debug.h"
#include "cbase/c_random.h"
#include "cbase/c_runes.h"

namespace ncore
{
    void* g_malloc(u32 size, u32 align)
    {
        alloc_t* allocator = context_t::system_alloc();
        return allocator->allocate(size, align);
    }

    void g_free(void* ptr)
    {
        alloc_t* allocator = context_t::system_alloc();
        allocator->deallocate(ptr);
    }
}  // namespace ncore

namespace cbase
{
    void init(ncore::s32 number_of_threads, ncore::s32 temporary_allocator_size)
    {
        ncore::g_init_system_alloc();
        ncore::alloc_t* system_allocator = ncore::g_get_system_alloc();

        ncore::console_t::init_default_console();
        ncore::context_t::init(number_of_threads, 16, system_allocator);

        ncore::context_t::register_thread();  // Should be called once from a created thread

        // The assert handler, system and string allocator are thread safe
        ncore::asserthandler_t* assert_handler = gSetAssertHandler();
        for (ncore::s32 i = 0; i < number_of_threads; i++)
        {
            ncore::byte* buffer_data = (ncore::byte*)system_allocator->allocate((ncore::u32)temporary_allocator_size);

            ncore::fixed_buffer_alloc_t* stack_allocator = system_allocator->construct<ncore::fixed_buffer_alloc_t>();
            stack_allocator->setup(buffer_data, temporary_allocator_size);

            ncore::random_t* random_generator = system_allocator->construct<ncore::wyrand_t>();
            random_generator->reset((ncore::s64)random_generator);  // randomize the seed

            ncore::context_t::set(i, ncore::context_t::ASSERT_HANDLER, assert_handler);
            ncore::context_t::set(i, ncore::context_t::SYSTEM_ALLOCATOR, system_allocator);
            ncore::context_t::set(i, ncore::context_t::RUNTIME_ALLOCATOR, system_allocator);
            ncore::context_t::set(i, ncore::context_t::FRAME_ALLOCATOR, system_allocator);
            ncore::context_t::set(i, ncore::context_t::LOCAL_ALLOCATOR, stack_allocator);
            ncore::context_t::set(i, ncore::context_t::RANDOM_GENERATOR, random_generator);
        }
    }

    void exit()
    {
        ncore::alloc_t* system_alloc = ncore::g_get_system_alloc();

        ncore::s32 number_of_threads = ncore::context_t::max_threads();
        for (ncore::s32 i = 0; i < number_of_threads; i++)
        {
            ncore::alloc_t* system_allocator = nullptr;
            ncore::context_t::set(i, ncore::context_t::SYSTEM_ALLOCATOR, system_allocator);

            ncore::alloc_t* runtime_allocator = nullptr;
            ncore::context_t::set(i, ncore::context_t::RUNTIME_ALLOCATOR, runtime_allocator);

            ncore::alloc_t* frame_allocator = nullptr;
            ncore::context_t::set(i, ncore::context_t::FRAME_ALLOCATOR, frame_allocator);

            ncore::fixed_buffer_alloc_t* stack_allocator;
            ncore::context_t::get(i, ncore::context_t::LOCAL_ALLOCATOR, stack_allocator);

            ncore::byte* buffer_data = stack_allocator->data();
            system_alloc->deallocate(buffer_data);
            system_alloc->destruct(stack_allocator);

            ncore::random_t* random_generator;
            ncore::context_t::get(i, ncore::context_t::RANDOM_GENERATOR, random_generator);

            system_alloc->destruct(random_generator);
        }

        ncore::context_t::exit(system_alloc);
        ncore::g_exit_system_alloc();
        system_alloc = nullptr;

        ncore::context_t::set_assert_handler(nullptr);
    }

};  // namespace cbase
