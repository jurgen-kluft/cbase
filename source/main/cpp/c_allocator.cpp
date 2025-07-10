#include "ccore/c_target.h"
#include "cbase/c_allocator.h"


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
