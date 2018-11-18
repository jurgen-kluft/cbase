#ifndef __XBASE_ALLOCATOR_H__
#define __XBASE_ALLOCATOR_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_memory.h"

namespace xcore
{
	/// The allocator interface
	class xalloc
	{
	public:
		static xalloc*		get_system();
		
		virtual void*		allocate(xsize_t size, u32 align) = 0;			///< Allocate memory with alignment
		virtual void		deallocate(void* p) = 0;						///< Deallocate/Free memory

		virtual void		release() = 0;

	protected:
		virtual				~xalloc() {}
	};

	template<typename T, typename... Args>
	T*			xnew(Args... args)
	{
		void * mem = xalloc::get_system()->allocate(sizeof(T), sizeof(void*));
		T* object = new (mem) T(args...);
		return object;
	}

	template<typename T>
	void		xdelete(T* p)
	{
		p->~T();
		xalloc::get_system()->deallocate(p);
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

		template<typename T, typename... Args>
		T*			construct(Args... args)
		{
			void * mem = allocate(sizeof(T), sizeof(void*));
			T* object = new (mem) T(args...);
			return object;
		}

		template<typename T>
		void		destruct(T* p)
		{
			p->~T();
			deallocate(p);
		}
	};


	// Example:
	// 	xheap heap(systemAllocator);
	// 	test_object* test = xnew<test_object>(heap, 200, 1000.0f);
	// 	xdelete<>(heap, test);


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

};

#endif	///< __XBASE_ALLOCATOR_H__
