#include "ccore/c_target.h"

#include "cbase/c_allocator.h"
#include "ccore/c_vmem.h"

namespace ncore
{
    arena_t*         sSystemVmem;
    narena::aalloc_t sSystemAllocator;

    void g_init_system_alloc()
    {
        sSystemVmem             = narena::create(1 * cGB, 8 * cMB);
        sSystemAllocator.m_vmem = sSystemVmem;
    }

    void g_exit_system_alloc()
    {
        sSystemAllocator.m_vmem = nullptr;
        narena::destroy(sSystemVmem);
        sSystemVmem = nullptr;
    }

    alloc_t* g_get_system_alloc() { return &sSystemAllocator; }

};  // namespace ncore
