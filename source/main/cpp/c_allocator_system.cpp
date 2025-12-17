#include "ccore/c_target.h"

#include "cbase/c_allocator.h"
#include "ccore/c_vmem.h"

namespace ncore
{
    arena_t       sSystemVmem;
    arena_alloc_t sSystemAllocator;

    void g_init_system_alloc()
    {
        sSystemVmem.reserved(1 * cGB);
        sSystemVmem.committed(8 * cMB);
        sSystemAllocator.m_vmem = &sSystemVmem;
    }

    void g_exit_system_alloc()
    {
        sSystemAllocator.m_vmem = nullptr;
        sSystemVmem.release();
    }

    alloc_t* g_get_system_alloc() { return &sSystemAllocator; }

};  // namespace ncore
