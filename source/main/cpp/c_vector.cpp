#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_vector.h"
#include "ccore/c_limits.h"
#include "cbase/c_context.h"
#include "ccore/c_arena.h"

namespace ncore
{
    namespace nvector
    {
        arena_t* g_alloc_vmem_arena(s32 reserved, s32 committed, s32 item_size)
        {
            arena_t* a = narena::new_arena((int_t)reserved * item_size, (int_t)committed * item_size);
            return a;
        }

        void g_free_vmem_arena(arena_t*& arena)
        {
            if (arena != nullptr)
            {
                narena::destroy(arena);
            }
        }

        bool g_set_capacity(arena_t* arena, s32& length, s32 new_capacity, s32 item_size)
        {
            if (new_capacity > length)
            {
                s32 reserved = g_get_reserved(arena, item_size);
                if (new_capacity > reserved)
                {
                    return false;
                }

                // Set the new committed size
                narena::commit(arena, new_capacity * item_size);
                length = new_capacity;
                return true;
            }
            return false;
        }

        s32 g_get_reserved(arena_t* arena, s32 item_size) { return (arena->m_reserved_pages * (1 << arena->m_page_size_shift)) / item_size; }

        void* g_vmem_arena_allocate(arena_t* arena, s32 size, s32 alignment)
        {
            if (size > 0)
            {
                ASSERT(alignment > 0 && (alignment & (alignment - 1)) == 0);  // Ensure alignment is a power of two
                return narena::alloc(arena, size, alignment);
            }
            return nullptr;
        }

    }  // namespace nvector

};  // namespace ncore
