#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_vector.h"
#include "cbase/c_limits.h"
#include "cbase/c_context.h"
#include "ccore/c_vmem.h"

namespace ncore
{
    namespace nvector
    {
        vmem_arena_t* g_alloc_vmem_arena(u32 reserved, u32 committed, s32 item_size)
        {
            vmem_arena_t a;
            a.reserved(reserved * item_size);
            a.committed(sizeof(vmem_arena_t) + (committed * item_size));
            vmem_arena_t* arena = (vmem_arena_t*)a.commit_and_zero(sizeof(vmem_arena_t));
            a.commit(committed * item_size);
            *arena = a;
            return arena;
        }

        void g_free_vmem_arena(vmem_arena_t*& arena)
        {
            if (arena != nullptr)
            {
                arena->release();
            }
        }

        bool g_set_capacity(vmem_arena_t* arena, u32& length, u32 new_capacity, s32 item_size)
        {
            if (new_capacity > length)
            {
                u32 reserved = g_get_reserved(arena, item_size);
                if (new_capacity > reserved)
                {
                    return false;
                }

                // Set the new committed size
                arena->committed(new_capacity * item_size);
                length = new_capacity;
                return true;
            }
            return false;
        }

        u32 g_get_reserved(vmem_arena_t* arena, s32 item_size) { return (arena->m_reserved_pages * (1 << arena->m_page_size_shift)) / item_size; }

        void g_vmem_arena_reserved(vmem_arena_t* arena, u32 reserved_size)
        {
            ASSERT(reserved_size > 0);
            arena->reserved(reserved_size);
        }

        void* g_vmem_arena_allocate(vmem_arena_t* arena, u32 size, s32 alignment)
        {
            ASSERT(size > 0);
            ASSERT(alignment > 0 && (alignment & (alignment - 1)) == 0);  // Ensure alignment is a power of two
            return arena->commit(size, alignment);
        }

    }  // namespace narray

};  // namespace ncore
