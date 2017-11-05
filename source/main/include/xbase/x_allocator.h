/**
* @file Header file x_allocator.h
*/

#ifndef __XBASE_ALLOCATOR_H__
#define __XBASE_ALLOCATOR_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif


/**
 * xCore namespace
 */
namespace xcore
{
	class x_iallocator;
	class x_iallocator_visitor;

	/**
	 * This system allocator, will initialize console memory and use malloc/realloc/free
	 * For WII, you can call this twice, 1st=MEM1 allocator, 2nd=MEM2 allocator
	 * For N3DS, 1st=HeapMemory, 2nd=DeviceMemory. 
	 * For N3DS, user may override 's32 gGetHeapMemorySize()' which returns the heap size.
	 */
	extern x_iallocator*	gCreateSystemAllocator();

	/**
	 * This will create a basic first fit allocator.
	 * Be aware that the behavior (fragmentation, performance) of this allocator gets worse
	 * the more allocations and deallocations you do with different sizes.
	 */
	extern x_iallocator*	gCreateBasicAllocator(void* mem_begin, xsize_t mem_size, u32 default_minimum_size, u32 default_alignment);


	/// The allocator interface
	class x_iallocator
	{
	public:
		static x_iallocator* get_default();
		
		virtual const char*	name() const = 0;								///< The name of the allocator

		virtual void*		allocate(xsize_t size, u32 align) = 0;			///< Allocate memory with alignment
		virtual void*		reallocate(void* p, xsize_t size, u32 align) = 0;///< Reallocate memory
		virtual void		deallocate(void* p) = 0;						///< Deallocate/Free memory

		virtual void		release() = 0;									///< Release/Destruct this allocator

	protected:
		virtual				~x_iallocator() {}
	};

	#define XCORE_CLASS_PLACEMENT_NEW_DELETE														\
		void*	operator new(xcore::xsize_t num_bytes, void* mem)			{ return mem; }			\
		void	operator delete(void* mem, void* )							{ }						\
		void*	operator new(xcore::xsize_t num_bytes)						{ return NULL; }		\
		void	operator delete(void* mem)									{ }						

	#define XCORE_CLASS_NEW_DELETE(get_allocator_func, align)																				\
		void*	operator new(xcore::xsize_t num_bytes, void* mem)			{ return mem; }													\
		void	operator delete(void* mem, void* )							{ }																\
		void*	operator new(xcore::xsize_t num_bytes)						{ return get_allocator_func()->allocate(num_bytes, align); }	\
		void	operator delete(void* mem)									{ get_allocator_func()->deallocate(mem); }						

	#define XCORE_CLASS_ARRAY_NEW_DELETE(get_allocator_func, align)																			\
		void*	operator new[](xcore::xsize_t num_bytes)					{ return get_allocator_func()->allocate(num_bytes, align); }	\
		void	operator delete[](void* mem)								{ get_allocator_func()->deallocate(mem); }						

	template<class T>
	class x_cdtor_default
	{
	public:
		inline T*			construct(void* mem) const						{ return static_cast<T*>(mem); }
		inline T*			copy_construct(void* mem, T const* other) const	{ T* item = (T*)(mem); *item = *other; return item; }
		inline void			destruct(T* obj) const							{ }
	};

	template<class T>
	class x_cdtor_placement_new
	{
	public:
		inline T*			construct(void* mem) const						{ return new (mem) T(); }
		inline T*			copy_construct(void* mem, T const* other) const	{ return new (mem) T(*other); }
		inline void			destruct(T* obj) const							{ obj->~T(); }
	};


	template<typename T, typename C=x_cdtor_default<T> >
	class x_type_allocator
	{
		x_iallocator*		mAllocator;
	public:
							x_type_allocator(x_iallocator* heap) : mAllocator(heap) { }
							~x_type_allocator()								{ release(); }

		T*					allocate(u32 align=sizeof(void*))				{ void* mem = mAllocator->allocate(sizeof(T), align); C c; T* obj = c.construct(mem); return obj; }
		void				deallocate(T* p)								{ C c; c.destruct(p); mAllocator->deallocate(p); }

		void				release()										{ mAllocator = NULL; }
	};
	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif	///< __XBASE_ALLOCATOR_H__
