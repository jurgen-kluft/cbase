#include "cbase/c_target.h"
#ifdef TARGET_MAC

#include <stdlib.h>
#include <cstring>
#include <stdio.h>

#include "cbase/c_memory.h"
#include "cbase/c_integer.h"
#include "cbase/c_allocator.h"

namespace ncore
{
    void* mac_aligned_malloc(size_t size, size_t alignment)
    {
        if (alignment < 2*(sizeof(void*)))
        {
            alignment = 2*(sizeof(void*));
        }
        if (size < (sizeof(void*)))
        {
            size = (sizeof(void*));
        }

        void* p1; // original block
        if ((p1 = malloc(size + 2*alignment)) == nullptr)
            return nullptr;

        void** p2; // aligned block
        p2     = (void**)(((ptr_t)(p1) & ~(alignment - 1)));
        p2     = (void**)(((ptr_t)(p2) + 2*alignment));
        p2[-2] = (void*)size;
        p2[-1] = p1;
        return (void*)p2;
    }

    void mac_aligned_free(void* ptr) { free(((void**)ptr)[-1]); }

    void* mac_aligned_realloc(void* ptr, size_t size, size_t alignment)
    {
        size_t old_size = (size_t)((void**)ptr)[-2];
        void*  new_ptr  = mac_aligned_malloc(size, alignment);
        memcpy(new_ptr, ptr, math::min(size, old_size));
        mac_aligned_free(ptr);
        return new_ptr;
    }

    class x_allocator_macos_system : public alloc_t
    {
    public:
        x_allocator_macos_system()
            : mInitialized(0)
            , mDefaultAlignment(4)
            , mAllocationCount(0)
        {
        }

        void init()
        {
            mInitialized      = 1;
            mAllocationCount  = 0;
            mDefaultAlignment = 4;
        }

        void exit()
        {
            ASSERTS(mAllocationCount == 0, "ERROR: System Allocator is being released but still has allocations that are not freed");
            mInitialized      = 0;
            mDefaultAlignment = 0;
            mAllocationCount  = 0;
        }

        bool isInitialized() { return mInitialized == 1; }

        virtual void* v_allocate(u32 size, u32 alignment)
        {
            void* ptr = mac_aligned_malloc(size, alignment);
            ++mAllocationCount;
            return ptr;
        }

        virtual u32 v_deallocate(void* ptr)
        {
            --mAllocationCount;
            mac_aligned_free(ptr);
            return 0;
        }

        virtual void v_release() {}

        s32 mInitialized;
        s32 mDefaultAlignment;
        u64 mAllocationCount;
    };

    x_allocator_macos_system sSystemAllocator;

    void alloc_t::init_system()
    {
        if (!sSystemAllocator.isInitialized())
        {
            sSystemAllocator.init();
        }
    }
    
    void alloc_t::exit_system()
    {
        if (sSystemAllocator.isInitialized())
        {
            sSystemAllocator.exit();
        }
    }

    alloc_t* alloc_t::get_system() { return &sSystemAllocator; }

}; // namespace ncore

#endif
