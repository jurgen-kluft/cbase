#include "xbase/x_allocator.h"
#include "xbase/x_context.h"
#include "xbase/x_debug.h"

namespace xcore
{
    // For xbase what would we like to have per thread:
    // - A global heap allocator (in a multi-threaded environment this one should be thread-safe)
    // - A stack allocator (local to a function call)
    // - A local thread allocator (local to the thread)
    // - A random generator

    static s32 sThreadIndexCount = 0;
    thread_local s32 sThreadIndex;

    void** context_t::m_slots = nullptr;
    s32    context_t::m_max_num_threads = 0;
    s32    context_t::m_max_num_slots = 0;

    void context_t::init(s32 max_num_threads, s32 max_num_slots, alloc_t* allocator)
    {
        m_max_num_threads = max_num_threads;
        m_max_num_slots = max_num_slots;
        m_slots = (void**)allocator->allocate(max_num_threads * max_num_slots * sizeof(void*), sizeof(void*));
    }

    void context_t::exit(alloc_t* allocator)
    {
        m_max_num_threads = 0;
        m_max_num_slots = 0;
        allocator->deallocate(m_slots);
        m_slots = nullptr;
    }

    s32 context_t::register_thread()
    {
        // Is this thread-safe ?
        sThreadIndex = sThreadIndexCount++;
        return sThreadIndex;
    }

    void context_t::vset(s32 tidx, s32 slot, void* pvData) 
    { 
        ASSERT(tidx < m_max_num_threads);
        ASSERT(slot < m_max_num_slots);
        m_slots[tidx*m_max_num_slots + slot] = pvData; 
    }

    void context_t::vget(s32 tidx, s32 slot, void*& pvData) 
    { 
        ASSERT(tidx < m_max_num_threads);
        ASSERT(slot < m_max_num_slots);
        pvData = m_slots[tidx*m_max_num_slots + slot]; 
    }

    s32 context_t::thread_index()
    {
        return sThreadIndex;
    }


} // namespace xcore
