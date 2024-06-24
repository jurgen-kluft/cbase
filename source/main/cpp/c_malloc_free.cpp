#include "cbase/c_allocator.h"

namespace ncore
{
    // ncore, global new/delete, malloc/free

    void* malloc(u64 size, u16 align)
    {
        alloc_t* allocator = context_t::runtime_alloc();
        return allocator->allocate (size, align);
    }

    void  free(void* ptr)
    {
        alloc_t* allocator = context_t::runtime_alloc();
        return allocator->deallocate(ptr);
    }

}
