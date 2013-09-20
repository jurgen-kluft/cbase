#include "xbase\x_target.h"
#ifdef TARGET_3DS

#include <nn\fnd.h>
#include <nn\init.h>
#include <nn\os.h>
#include <nn\err.h>

#include "xbase\x_allocator.h"
#include "xbase\x_debug.h"

namespace xcore
{
	__weak void gGetHeapMemorySize(s32& outHeapSize);

	extern "C" void nninitStartUp()
	{
		s32 heapSize = 40*1024*1024;
		gGetHeapMemorySize(heapSize);

		const size_t assignment   = nn::os::GetAppMemorySize();
		const size_t currentUsing = nn::os::GetUsingMemorySize();

		const size_t available = assignment - currentUsing;
		size_t deviceSize;
		if (heapSize <= 0)
		{
			deviceSize = available/2;
			heapSize = available - deviceSize;
		}
		else
		{
			deviceSize = available - heapSize;
		}

		nn::os::SetupHeapForMemoryBlock(heapSize);
		nn::Result result = nn::os::SetDeviceMemorySize( deviceSize );
		NN_ERR_THROW_FATAL( result );
	}

	class x_allocator_3ds_system : public x_iallocator
	{
		const char*					mName;
		s32							mDefaultAlignment;
		void*						mHeapAddress;
		u32							mHeapSize;
		nn::fnd::ExpHeap			mSystemHeap;
		u64							mAllocationCount;

	public:
		x_allocator_3ds_system(const char* name) 
			: mName(name)
			, mDefaultAlignment(4)
			, mHeapAddress(NULL)
			, mHeapSize(0)
			, mAllocationCount(0)
		{
		}

		virtual const char*	name() const
		{
			return mName;
		}

		bool isInitialized() const
		{
			return mHeapAddress != NULL;
		}

		bool initHeap()
		{
			mHeapAddress = (void*)nn::os::GetHeapAddress();
			mHeapSize = nn::os::GetHeapSize();
			mSystemHeap.Initialize(nn::os::GetHeapAddress(), nn::os::GetHeapSize(), nn::os::ALLOCATE_OPTION_LINEAR);

			mAllocationCount = 0;
			return true;
		}

		bool initDeviceMem()
		{
			mHeapAddress = (void*)nn::os::GetDeviceMemoryAddress();
			mHeapSize = nn::os::GetDeviceMemorySize();
			mSystemHeap.Initialize(nn::os::GetDeviceMemoryAddress(), nn::os::GetDeviceMemorySize(), nn::os::ALLOCATE_OPTION_LINEAR);

			mAllocationCount = 0;
			return true;
		}

		virtual void*			allocate(u32 size, u32 alignment)
		{
			++mAllocationCount;
			return mSystemHeap.Allocate(size, alignment);
		}

		virtual void*			reallocate(void* ptr, u32 size, u32 alignment)
		{
			if (ptr == NULL)
				return allocate(size, alignment);

			size_t newsize = mSystemHeap.ResizeBlock(ptr, size);
			ASSERTS(newsize > 0, "Reallocate failed!");
			return ptr;
		}

		virtual void			deallocate(void* ptr)
		{
			--mAllocationCount;
			mSystemHeap.Free(ptr);
		}

		virtual void		release()
		{
			ASSERTS(mAllocationCount==0, "ERROR: System Allocator is being released but still has allocations that are not freed");
			mSystemHeap.Invalidate();
			mHeapAddress = NULL;
			mHeapSize = 0;
		}
	};

	x_iallocator*		gCreateSystemAllocator()
	{
		static x_allocator_3ds_system sN3dsHeapMemoryAllocator("N3DS Heap memory allocator");
		static x_allocator_3ds_system sN3dsDeviceMemoryAllocator("N3DS Device memory allocator");

		if (sN3dsHeapMemoryAllocator.isInitialized() == false)
		{
			if (sN3dsHeapMemoryAllocator.initHeap() == true)
				return &sN3dsHeapMemoryAllocator;
		}
		else if (sN3dsDeviceMemoryAllocator.isInitialized() == false)
		{
			if (sN3dsDeviceMemoryAllocator.initDeviceMem() == true)
				return &sN3dsDeviceMemoryAllocator;
		}

		// System failure
		return NULL;
	}

};

#endif
