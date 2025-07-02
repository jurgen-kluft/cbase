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
