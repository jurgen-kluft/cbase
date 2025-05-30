#include "ccore/c_target.h"
#ifdef TARGET_MAC

#    include <stdlib.h>
#    include <cstring>
#    include <stdio.h>

#    include "cbase/c_memory.h"
#    include "cbase/c_integer.h"
#    include "cbase/c_allocator.h"
#    include "cbase/c_context.h"

namespace ncore
{
    class allocator_macos_system : public alloc_t
    {
    public:
        allocator_macos_system()
            : mInitialized(0)
            , mDefaultAlignment(sizeof(void*))
            , mAllocationCount(0)
        {
        }

        void init()
        {
            mInitialized      = 1;
            mAllocationCount  = 0;
            mDefaultAlignment = sizeof(void*);
        }

        void exit()
        {
            ASSERTS(mAllocationCount == 0, "ERROR: System Allocator is being released but still has allocations that are not freed");
            mInitialized      = 0;
            mDefaultAlignment = sizeof(void*);
            mAllocationCount  = 0;
        }

        bool isInitialized() { return mInitialized == 1; }

        virtual void* v_allocate(u32 size, u32 alignment)
        {
            alignment = math::g_alignUp(alignment, mDefaultAlignment);

            void* ptr;
            posix_memalign(&ptr, alignment, size);
            ++mAllocationCount;
            return ptr;
        }

        virtual void v_deallocate(void* ptr)
        {
            --mAllocationCount;
            ::free(ptr);
        }

        s32 mInitialized;
        u32 mDefaultAlignment;
        u64 mAllocationCount;
    };

    allocator_macos_system sSystemAllocator;

    void g_init_system_alloc()
    {
        if (!sSystemAllocator.isInitialized())
        {
            sSystemAllocator.init();
        }
    }

    void g_exit_system_alloc()
    {
        if (sSystemAllocator.isInitialized())
        {
            sSystemAllocator.exit();
        }
    }

    alloc_t* g_get_system_alloc() { return &sSystemAllocator; }

};  // namespace ncore

#endif
