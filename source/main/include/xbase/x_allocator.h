#ifndef __XBASE_ALLOCATOR_H__
#define __XBASE_ALLOCATOR_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_debug.h"
#include "xbase/x_memory.h"

namespace xcore
{
	class xbuffer;

	class xalloc
	{
	public:
		static void			init_system();
		static xalloc*		get_system();
		
		virtual void*		allocate(xsize_t size, u32 align) = 0;			// Allocate memory with alignment
		virtual void		deallocate(void* p) = 0;						// Deallocate/Free memory

		virtual void		release() = 0;

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

	protected:
		virtual				~xalloc() {}
	};

	// fixed-size allocator
	class xfsa
	{
	public:
		virtual u32			size() const = 0;
		virtual void*		allocate() = 0;
		virtual void		deallocate(void*) = 0;

		virtual void		release() = 0;

		template<typename T, typename... Args>
		T*			construct(Args... args)
		{
			ASSERT(sizeof(T) <= size());
			void * mem = allocate();
			T* object = new (mem) T(args...);
			return object;
		}

		template<typename T>
		void		destruct(T* p)
		{
			p->~T();
			deallocate(p);
		}

	protected:
		virtual				~xfsa() {}
	};

	// The dexer interface, 'pointer to index' and 'index to pointer'
	class xdexer
	{
	public:
		virtual void*		idx2ptr(u32 index) const = 0;
		virtual u32			ptr2idx(void* ptr) const = 0;
	};

	class xfsadexed : public xfsa, public xdexer
	{
	public:
		xfsadexed() {}

	protected:
		virtual			~xfsadexed() {}
	};

	// Global new and delete
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

	class xalloc_stack : public xalloc
	{
		static inline xbyte* align_ptr(xbyte* ptr, uptr align) { return (xbyte*)(((uptr)ptr + (align-1)) & ~(align-1)); }

		xbyte*			m_base;
		xbyte*			m_ptr;
		xbyte*			m_end;
		s64				m_cnt;

	public:
						xalloc_stack(xbuffer& storage);

		virtual void*	allocate(xsize_t size, u32 align)
		{
			if (m_ptr < m_end && align_ptr(m_ptr+size, align) <= m_end)
			{
				xbyte* storage = m_ptr;
				m_ptr = align_ptr(m_ptr + size, sizeof(void*));
				m_cnt += 1;
				return storage;
			}
			return nullptr;
		}

		virtual void	deallocate(void* p)
		{
			if (p != nullptr)
			{
				ASSERT(m_cnt > 0);
				m_cnt -= 1;
				if (m_cnt == 0)
					m_ptr = m_base;
			}
		}

		virtual void	release() { m_base=nullptr; m_ptr=nullptr; m_end=nullptr; }
	};

	// Allocate a single object in-place
	class xallocinplace : public xalloc
	{
		xbyte*				m_data;
		u64					m_size;
	public:
		inline				xallocinplace(xbyte* data, u64 size) : m_data(data), m_size(size) {}
		virtual void*		allocate(xsize_t size, u32 align);
		virtual void		deallocate(void* p);
		virtual void		release();
	};

	template<u32 L>
	class xinplace
	{
		enum { SIZE = (L+3)/4 };
		u64				m_memory[SIZE];
	public:
		inline			xinplace() {}
		xallocinplace	allocator() const { return xallocinplace((xbyte*)m_memory, (u64)SIZE); }

		template<class T>
		inline T*		object() { return static_cast<T*>(m_memory); }
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

};

#endif	///< __XBASE_ALLOCATOR_H__
