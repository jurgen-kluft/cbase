#include "xbase\x_target.h"
#ifdef TARGET_WII
#include <revolution\mem.h>

#include "xbase\x_debug.h"
#include "xbase\x_integer.h"
#include "xbase\x_memory_std.h"
#include "xbase\x_allocator.h"

namespace xcore
{
	class x_allocator_wii_system : public x_iallocator
	{
		const char*				mName;
		s32						mDefaultAlignment;
		MEMHeapHandle			mExpHeapHandle;
		MEMAllocator			mMemAllocator;
		void*					mArenaLo;
		void*					mArenaHi;
		u64						mAllocationCount;

		struct header
		{
			void*	real_ptr;
			u32		real_size;
		};

	public:
		x_allocator_wii_system(const char* name) 
			: mName(name)
			, mDefaultAlignment(8)
			, mArenaLo(NULL)
			, mArenaHi(NULL)
			, mAllocationCount(0)
		{
			mDefaultAlignment = xcore::x_intu::ceilPower2(sizeof(header));
		}

		virtual const char*	name() const
		{
			return mName;
		}

		bool				isInitialized() const
		{
			return mArenaLo!=NULL || mArenaHi!=NULL;
		}

		bool				initMem1()
		{
			// Heap on MEM1
			mArenaLo = OSGetMEM1ArenaLo();
			mArenaHi = OSGetMEM1ArenaHi();
	    
			mExpHeapHandle = MEMCreateExpHeap(mArenaLo, (u32)mArenaHi - (u32)mArenaLo);
			if ( mExpHeapHandle == MEM_HEAP_INVALID_HANDLE )
			{
				// MEM1 heap allocation error
				mArenaLo = NULL;
				mArenaHi = NULL;
				return false;
			}
			OSSetMEM1ArenaLo(mArenaHi);
			MEMInitAllocatorForExpHeap(&mMemAllocator, mExpHeapHandle, mDefaultAlignment);

			return true;
		}

		bool				initMem2()
		{
			// Heap on MEM2
			mArenaLo = OSGetMEM2ArenaLo();
			mArenaHi = OSGetMEM2ArenaHi();
	    
			mExpHeapHandle = MEMCreateExpHeap(mArenaLo, (u32)mArenaHi - (u32)mArenaLo);
			if ( mExpHeapHandle == MEM_HEAP_INVALID_HANDLE )
			{
				// MEM2 heap allocation error
				mArenaLo = NULL;
				mArenaHi = NULL;
				return false;
			}
			OSSetMEM2ArenaLo(mArenaHi);
			MEMInitAllocatorForExpHeap(&mMemAllocator, mExpHeapHandle, mDefaultAlignment);

			return true;
		}

		inline
		u32					recalc_alignment(s32 alignment)
		{
			return xcore::x_intu::max(alignment, mDefaultAlignment);
		}

		inline
		u32					recalc_size(s32 size, s32 alignment)
		{
			// We have to be sure that our header fits into the space created by alignment.
			// This means that we have to request an allocation size that includes twice
			// the requested alignment.
			return size + recalc_alignment(alignment)*2;
		}

		header*				get_header(void* ptr)
		{
			header* _header = (header*)((u32)ptr - sizeof(header));
			return _header;
		}

		void*				set_header(void* ptr, s32 size, s32 requested_size, s32 requested_alignment)
		{
			u32 alignment = recalc_alignment(requested_alignment);
			// MEMAllocFromAllocator will return us a pointer that likely is 
			// not aligned according to our requirement, so we have to make sure 
			// we really align up
			void* new_ptr = (void*)(xcore::x_intu::alignUp((u32)ptr, alignment));

			// We can advance again with the alignment size and making sure that we
			// now really have space for our header.
			new_ptr = (void*)((u32)new_ptr + alignment);

			ASSERT((u32)new_ptr > (u32)ptr);
			ASSERT(((u32)new_ptr - (u32)ptr) >= sizeof(header));
			ASSERT((((u32)new_ptr - (u32)ptr) + requested_size) <= size);

			header* _header = get_header(new_ptr);
			_header->real_ptr = ptr;
			_header->real_size = requested_size;
			return new_ptr;
		}

		virtual void*		allocate(u32 size, u32 alignment)
		{
			s32 new_size = recalc_size(size, alignment);
			void* mem = MEMAllocFromAllocator(&mMemAllocator, new_size);
			++mAllocationCount;
			return set_header(mem, new_size, size, alignment);
		}

		virtual void*		reallocate(void* ptr, u32 size, u32 alignment)
		{
			if (ptr == NULL)
				return allocate(size, alignment);

			header* _header = get_header(ptr);
			s32 new_size = recalc_size(size, alignment);
			void* mem = allocate(size, alignment);
			x_memcpy(mem, ptr, _header->real_size);
			deallocate(ptr);
			return mem;
		}

		virtual void		deallocate(void* ptr)
		{
			header* _header = get_header(ptr);
			MEMFreeToAllocator(&mMemAllocator, _header->real_ptr);
			--mAllocationCount;
		}

		virtual void		release()
		{
			ASSERTS(mAllocationCount==0, "ERROR: System Allocator is being released but still has allocations that are not freed");
			mArenaLo = NULL;
			mArenaHi = NULL;
		}
	};

	x_iallocator*		gCreateSystemAllocator()
	{
		static x_allocator_wii_system	sWiiMem1SystemAllocator("WII MEM1 system allocator");
		static x_allocator_wii_system	sWiiMem2SystemAllocator("WII MEM2 system allocator");

		if (sWiiMem1SystemAllocator.isInitialized() == false)
		{
			if (sWiiMem1SystemAllocator.initMem1() == true)
				return &sWiiMem1SystemAllocator;
		}
		else if (sWiiMem2SystemAllocator.isInitialized() == false)
		{
			if (sWiiMem2SystemAllocator.initMem2() == true)
				return &sWiiMem2SystemAllocator;
		}

		// System failure
		return NULL;
	}

};

#endif
