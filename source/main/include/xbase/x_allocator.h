/**
* @file Header file x_allocator.h
*/

#ifndef __XBASE_ALLOCATOR_H__
#define __XBASE_ALLOCATOR_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_memory_std.h"

/**
 * xCore namespace
 */
namespace xcore
{
	class x_iallocator;
	class x_iallocator_visitor;

	/**
	 * This system allocator, will initialize console memory and use malloc/realloc/free
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

	class xallocator
	{
		x_iallocator*	m_allocator;

	public:
		inline			xallocator() : m_allocator(NULL) {}
		inline			xallocator(x_iallocator* all) : m_allocator(all) {}

		bool			can_alloc() const { return m_allocator != NULL; }

		void*			allocate(xsize_t size, u32 align) 
		{
			return m_allocator->allocate(size, align); 
		}

		void*			reallocate(void* p, xsize_t size, u32 align)
		{
			return m_allocator->reallocate(p, size, align);
		}

		void			deallocate(void* p)
		{
			m_allocator->deallocate(p);
		}

		void*			allocate_and_clear(xsize_t size, u32 align, u32 clr)
		{
			void* mem = m_allocator->allocate(size, align);
			xmem_utils::memset(mem, clr, (u32)size);
			return mem;
		}

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
