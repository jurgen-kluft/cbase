#include "ccore/c_target.h"
#ifdef TARGET_PC

#ifdef TARGET_DEBUG
#define USE_MALLOC_DBG
#endif

#ifdef USE_MALLOC_DBG
#    include <crtdbg.h>
#endif

#include "cbase/c_allocator.h"

namespace ncore
{
    class allocator_win32_system : public alloc_t
    {
    public:
        allocator_win32_system()
            : mInitialized(0)
            , mDefaultAlignment(sizeof(void*))
            , mAllocationCount(0)
        {
        }
        ~allocator_win32_system() {}

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
#ifdef COMPILER_MSVC
#ifdef USE_MALLOC_DBG
            void* mem = _aligned_malloc_dbg(size, alignment, nullptr, 0);
#else
            void* mem = _aligned_malloc(size, alignment);
#endif
#else
            void* mem = nullptr;
#endif
            ++mAllocationCount;
            return mem;
        }

        virtual void v_deallocate(void* ptr)
        {
            --mAllocationCount;
#ifdef COMPILER_MSVC
#ifdef USE_MALLOC_DBG
            _aligned_free_dbg(ptr);
#else
            _aligned_free(ptr);
#endif
#endif
        }

        s32 mInitialized;
        s32 mDefaultAlignment;
        u64 mAllocationCount;
    };

    allocator_win32_system sSystemAllocator;
    void                     init_system_alloc()
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
