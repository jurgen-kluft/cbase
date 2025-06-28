#include "cbase/c_allocator.h"

namespace ncore
{
    // ncore, global new/delete, malloc/free

    void* malloc(u64 size, u16 align)
    {
        ASSERT(size > 0 && size < 0x7FFFFFFF);
        context_t c = g_current_context();
        alloc_t* allocator = c.heap_alloc();
        return allocator->allocate((u32)size, align);
    }

    void free(void* ptr)
    {
        context_t c = g_current_context();
        alloc_t* allocator = c.heap_alloc();
        return allocator->deallocate(ptr);
    }

}  // namespace ncore
