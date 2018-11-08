#ifndef __XBASE_ALLOCATOR_H__
#define __XBASE_ALLOCATOR_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_memory.h"

/**
 * xCore namespace
 */
namespace xcore
{
	class xalloc;
	class xalloc_visitor;

	/**
	 * This system allocator, will initialize console memory and use malloc/realloc/free
	 */
	extern xalloc*	gCreateSystemAllocator();

	/**
	 * This will create a basic first fit allocator.
	 * Be aware that the behavior (fragmentation, performance) of this allocator gets worse
	 * the more allocations and deallocations you do with different sizes.
	 */
	extern xalloc*	gCreateBasicAllocator(void* mem_begin, xsize_t mem_size, u32 default_minimum_size, u32 default_alignment);

	/// The allocator interface
	class xalloc
	{
	public:
		static xalloc*		get_default();
		
		virtual void*		allocate(xsize_t size, u32 align) = 0;			///< Allocate memory with alignment
		virtual void		deallocate(void* p) = 0;						///< Deallocate/Free memory

		virtual void		release() = 0;

	protected:
		virtual				~xalloc() {}
	};

	class xsystemheap
	{
	public:
		void*	allocate(xcore::u32 size, xcore::u32 alignment)
		{
			return xalloc::get_default()->allocate(size, alignment);
		}
		void	deallocate(void* p)
		{
			return xalloc::get_default()->deallocate(p);
		}
	};

	template<typename T, typename MEM, typename... Args>
	T*			xnew(Args... args)
	{
		MEM m;
		void * mem = m.allocate(sizeof(T), sizeof(void*));
		T* object = new (mem) T(args...);
		return object;
	}

	template<typename MEM, typename T>
	void		xdelete(T* p)
	{
		MEM m;
		p->~T();
		m.deallocate(p);
	}

	class xheap
	{
		xalloc*		m_allocator;
	public:
		inline	xheap(xalloc* allocator) : m_allocator(allocator) {}

		void*	allocate(xcore::u32 size, xcore::u32 alignment)
		{
			return m_allocator->allocate(size, alignment);
		}
		void	deallocate(void* p)
		{
			return m_allocator->deallocate(p);
		}
	};

	template<typename T, typename... Args>
	T*			xnew(xheap& heap, Args... args)
	{
		void * mem = heap.allocate(sizeof(T), sizeof(void*));
		T* object = new (mem) T(args...);
		return object;
	}

	template<typename T>
	void		xdelete(xheap& heap, T* p)
	{
		p->~T();
		heap.deallocate(p);
	}


	// Example:
	// 	xheap::sAllocator = systemAllocator;
	// 	test_object* test = xnew<test_object, xsystemheap>(200, 1000.0f);
	// 	xdelete<xheap>(test);


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


	template<typename T, typename C= x_cdtor_placement_new<T> >
	class x_type_allocator
	{
		xalloc*				mAllocator;
	public:
							x_type_allocator(xalloc* heap) : mAllocator(heap) { }
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
