#include "xbase\x_target.h"
#ifdef TARGET_PC

#include <string>

#include "xbase\x_memory_std.h"
#include "xbase\x_integer.h"
#include "xbase\x_allocator.h"

namespace xcore
{
	class x_allocator_win32_system : public x_iallocator
	{
	public:
		x_allocator_win32_system()
			: mInitialized(0)
			, mDefaultAlignment(4)
			, mAllocationCount(0)
		{
		}

		void					init() 
		{
			mInitialized = 1;
			mAllocationCount = 0;
			mDefaultAlignment = 4;
		}

		bool					isInitialized()
		{
			return mInitialized == 1;
		}

		virtual const char*		name() const
		{
			return "win32 system allocator";
		}

		virtual void*			allocate(xsize_t size, u32 alignment)
		{
			void* mem = _aligned_malloc(size, alignment);
			++mAllocationCount;
			return mem;
		}

		virtual void*			reallocate(void* ptr, xsize_t size, u32 alignment)
		{
			if (ptr == NULL)
				return allocate(size, alignment);

			void* new_ptr = _aligned_realloc(ptr, size, alignment);
			return new_ptr;
		}

		virtual void			deallocate(void* ptr)
		{
			--mAllocationCount;
			_aligned_free(ptr);
		}

		virtual void			release()
		{
			ASSERTS(mAllocationCount==0, "ERROR: System Allocator is being released but still has allocations that are not freed");
			mInitialized = 0;
			mDefaultAlignment = 0;
			mAllocationCount = 0;
		}

		void*					operator new(xsize_t num_bytes, void* mem)		{ return mem; }
		void					operator delete(void* pMem, void* )			{ }

		s32						mInitialized;
		s32						mDefaultAlignment;
		u64						mAllocationCount;
	};

	x_iallocator*		gCreateSystemAllocator()
	{
		static x_allocator_win32_system sSystemAllocator;
		if (!sSystemAllocator.isInitialized())
		{
			sSystemAllocator.init();
		}
		return &sSystemAllocator;
	}

};

#endif
