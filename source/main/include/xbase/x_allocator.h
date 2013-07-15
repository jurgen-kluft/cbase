//==============================================================================
//  x_allocator.h
//==============================================================================
#ifndef __XBASE_ALLOCATOR_H__
#define __XBASE_ALLOCATOR_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_types.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class x_iallocator;
	class x_iallocator_visitor;

	/// This system allocator, will initialize console memory and use malloc/realloc/free
	/// For WII, you can call this twice, 1st=MEM1 allocator, 2nd=MEM2 allocator
	/// For N3DS, 1st=HeapMemory, 2nd=DeviceMemory. 
	/// For N3DS, user may override 's32 gGetHeapMemorySize()' which returns the heap size.
	extern x_iallocator*		gCreateSystemAllocator();
	
	/// The allocator interface
	class x_iallocator
	{
	public:
		virtual const char*		name() const = 0;										///< The name of the allocator

		virtual void*			allocate(u32 size, u32 align) = 0;						///< Allocate memory with alignment
		virtual void*			reallocate(void* p, u32 size, u32 align) = 0;			///< Reallocate memory
		virtual void			deallocate(void* p) = 0;								///< Deallocate/Free memory

		virtual void			release() = 0;											///< Release/Destruct this allocator

	protected:
		virtual					~x_iallocator() {}
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif	/// __XBASE_ALLOCATOR_H__
