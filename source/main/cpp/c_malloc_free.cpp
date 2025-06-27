#include "cbase/c_allocator.h"

namespace ncore
{
    // ncore, global new/delete, malloc/free

    void* malloc(u64 size, u16 align)
    {
        ASSERT(size > 0 && size < 0x7FFFFFFF);
        alloc_t* allocator = thread_context_t::current()->runtime_alloc();
        return allocator->allocate((u32)size, align);
    }

    void free(void* ptr)
    {
        alloc_t* allocator = thread_context_t::current()->runtime_alloc();
        return allocator->deallocate(ptr);
    }

}  // namespace ncore
