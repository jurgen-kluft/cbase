#include "ccore/c_target.h"

#include "cbase/c_allocator.h"
#include "ccore/c_arena.h"

namespace ncore
{
    arena_t*         sSystemArena = nullptr;
    narena::aalloc_t sSystemAllocator;

    void g_init_system_alloc()
    {
        sSystemArena             = narena::new_arena(1 * cGB, 8 * cMB);
        sSystemAllocator.m_arena = sSystemArena;
    }

    void g_exit_system_alloc()
    {
        sSystemAllocator.m_arena = nullptr;
        narena::destroy(sSystemArena);
        sSystemArena = nullptr;
    }

    alloc_t* g_get_system_alloc() { return &sSystemAllocator; }

};  // namespace ncore
