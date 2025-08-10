#include "ccore/c_target.h"
#include "ccore/c_allocator.h"
#include "ccore/c_debug.h"
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
