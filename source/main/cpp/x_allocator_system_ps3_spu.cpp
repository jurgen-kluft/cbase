#include "xbase\x_target.h"
#include "xbase\x_debug.h"
#ifdef SPU

#include "xbase\x_allocator.h"
#include <spu_printf.h>
#include <new>
#include <stdlib.h>

namespace xcore
{


	xcore::s32 gSPUProgramSize = 0;
	xcore::s32 gSPUStackSize = 0;

	const xcore::u32 gSPUMemSize = 256000;


	void gSetSPUConfig(xcore::s32 programSize, xcore::s32 stackSize)
	{
		gSPUProgramSize = programSize;
		gSPUStackSize = stackSize;
	}
	/*
	x_iallocator*		gCreateSystemAllocator()
	{

		

		void *memstart = (void*)gSPUProgramSize;
		return gCreateBasicAllocator(memstart, gSPUMemSize - gSPUProgramSize - gSPUStackSize, X_MEMALIGN, X_MEMALIGN);
	}
			*/





	class x_allocator_ps3_system : public x_iallocator
	{
	public:
		x_allocator_ps3_system()
			: mInitialized(0)
			, mDefaultAlignment(4)
			, mAllocationCount(0)
		{
		}

		void			init() 
		{
			mInitialized = 1;
			mDefaultAlignment = 4;
			mAllocationCount = 0;
		}

		bool			isInitialized()
		{
			return mInitialized == 1;
		}

		virtual const char*		name() const
		{
			return "PS3 system allocator";
		}

		virtual void*			allocate(u32 size, u32 alignment)
		{
			++mAllocationCount;
			void* p = memalign(alignment, size);
			return p;
		}

		virtual void*			reallocate(void* ptr, u32 size, u32 alignment)
		{
			if (ptr == NULL)
				return allocate(size, alignment);

			void* p = reallocalign(ptr, size, alignment);
			return p;
		}

		virtual void			deallocate(void* ptr)
		{
			--mAllocationCount;
			free(ptr);
		}

		virtual void			release()
		{
			ASSERTS(mAllocationCount==0, "ERROR: System Allocator is being released but still has allocations that are not freed");
			mDefaultAlignment = 4;
			mInitialized = 0;
			mAllocationCount = 0;
		}

	protected:
		s32						mInitialized;
		u32						mDefaultAlignment;
		u64						mAllocationCount;
	};

	x_iallocator*		gCreateSystemAllocator()
	{
		static x_allocator_ps3_system sPs3SystemAllocator;
		if (!sPs3SystemAllocator.isInitialized())
		{
			sPs3SystemAllocator.init();
		}
		return &sPs3SystemAllocator;
	}

};








#endif
