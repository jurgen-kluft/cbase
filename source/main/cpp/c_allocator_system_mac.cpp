#include "ccore/c_target.h"
#ifdef TARGET_MAC

#include <stdlib.h>
#include <cstring>
#include <stdio.h>

#include "cbase/c_memory.h"
#include "cbase/c_integer.h"
#include "cbase/c_allocator.h"

namespace ncore
{
    // void* mac_aligned_malloc(size_t size, size_t alignment)
    // {
    //     if (alignment < 2*(sizeof(void*)))
    //     {
    //         alignment = 2*(sizeof(void*));
    //     }
    //     if (size < (sizeof(void*)))
    //     {
    //         size = (sizeof(void*));
    //     }


    //     void* p1; // original block
    //     if ((p1 = malloc(size + 2*alignment)) == nullptr)
    //         return nullptr;

    //     void** p2; // aligned block
    //     p2     = (void**)(((ptr_t)(p1) & ~(alignment - 1)));
    //     p2     = (void**)(((ptr_t)(p2) + 2*alignment));
    //     p2[-2] = (void*)size;
    //     p2[-1] = p1;
    //     return (void*)p2;
    // }

    // void mac_aligned_free(void* ptr) { free(((void**)ptr)[-1]); }

    // void* mac_aligned_realloc(void* ptr, size_t size, size_t alignment)
    // {
    //     size_t old_size = (size_t)((void**)ptr)[-2];
    //     void*  new_ptr  = mac_aligned_malloc(size, alignment);
    //     memcpy(new_ptr, ptr, math::min(size, old_size));
    //     mac_aligned_free(ptr);
    //     return new_ptr;
    // }

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
            alignment = math::alignUp(alignment, mDefaultAlignment);

            void* ptr;
            posix_memalign(&ptr, alignment, size);
            ++mAllocationCount;
            return ptr;
        }

        virtual void v_deallocate(void* ptr)
        {
            --mAllocationCount;
            free(ptr);
        }

        s32 mInitialized;
        u32 mDefaultAlignment;
        u64 mAllocationCount;
    };

    allocator_macos_system sSystemAllocator;

    void init_system_alloc()
    {
        if (!sSystemAllocator.isInitialized())
        {
            sSystemAllocator.init();
        }
    }
    
    void exit_system_alloc()
    {
        if (sSystemAllocator.isInitialized())
        {
            sSystemAllocator.exit();
        }
    }

    alloc_t* get_system_alloc() { return &sSystemAllocator; }

}; // namespace ncore

#endif
