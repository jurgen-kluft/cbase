#include "xbase/x_target.h"
#ifdef TARGET_PC

#include <string>

#include "xbase/x_memory.h"
#include "xbase/x_integer.h"
#include "xbase/x_allocator.h"

namespace xcore
{
    class x_allocator_win32_system : public alloc_t
    {
    public:
        x_allocator_win32_system()
            : mInitialized(0)
            , mDefaultAlignment(sizeof(void*))
            , mAllocationCount(0)
        {
        }

        void init()
        {
            mInitialized      = 1;
            mAllocationCount  = 0;
            mDefaultAlignment = 4;
        }

        bool isInitialized() { return mInitialized == 1; }

        virtual void* v_allocate(u32 size, u32 alignment)
        {
#ifdef TARGET_DEBUG
            void* mem = _aligned_malloc_dbg(size, alignment, NULL, 0);
#else
            void* mem = _aligned_malloc(size, alignment);
#endif
            ++mAllocationCount;
            return mem;
        }

        virtual u32 v_deallocate(void* ptr)
        {
            --mAllocationCount;
#ifdef TARGET_DEBUG
            _aligned_free_dbg(ptr);
#else
            _aligned_free(ptr);
#endif
			return 0;
        }

        virtual void v_release()
        {
            ASSERTS(mAllocationCount == 0, "ERROR: System Allocator is being released but still has allocations that are not freed");
            mInitialized      = 0;
            mDefaultAlignment = 0;
            mAllocationCount  = 0;
        }

        void* operator new(xsize_t num_bytes, void* mem) { return mem; }
        void  operator delete(void* pMem, void*) {}

        s32 mInitialized;
        s32 mDefaultAlignment;
        u64 mAllocationCount;
    };

    x_allocator_win32_system sSystemAllocator;
    void                     alloc_t::init_system()
    {
        if (sSystemAllocator.isInitialized())
        {
            sSystemAllocator.init();
        }
    }

    alloc_t* alloc_t::get_system() { return &sSystemAllocator; }

}; // namespace xcore

#endif
