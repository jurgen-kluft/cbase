//==============================================================================
//  x_idx_allocator.h
//==============================================================================
#ifndef __XBASE_IDX_ALLOCATOR_H__
#define __XBASE_IDX_ALLOCATOR_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_allocator.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	///
	/// The indexed allocator interface
	///
	/// This allocator has an advantage over a heap allocator in that if you
	/// use two of these allocators at the same time you only have to store
	/// one index.
	/// The main constraint for this allocator is that the allocation size is 
	/// fixed.
	/// A simple implementation is an array with an embedded free-list, another
	/// one could be a pool allocator with fixed size blocks. So the index for
	/// this pool allocator would encode the block index and element index.
	/// Setting the maximum number of items lower as 65536 will basically give
	/// you indices in the 16 bit range, so then you only have to store 16 bits
	/// for referencing the item.
	///
	class x_indexer
	{
	public:
		virtual void*			to_ptr(u32 idx) const = 0;				///< Convert index to object pointer
		virtual u32				to_idx(void const* p) const = 0;		///< Convert object pointer to index
	};

	class x_iidx_allocator : public x_iallocator, public x_indexer
	{
	public:
		virtual void			init() = 0;								///< Initialize all resources
		virtual void			clear() = 0;							///< Release all resources

		virtual u32				size() const = 0;
		virtual u32				max_size() const = 0;

		virtual u32				iallocate(void*& p) = 0;				///< Allocate fixed size object and return array index of allocated object
		virtual void			ideallocate(u32 idx) = 0;				///< Deallocate object

	protected:
		virtual					~x_iidx_allocator() {}
	};

	template<typename T, typename C=x_cdtor_default<T> >
	class x_type_idx_allocator
	{
		x_iidx_allocator*	mAllocator;
	public:
							x_type_idx_allocator(x_iidx_allocator* array_allocator)	{ mAllocator = array_allocator; }
							~x_type_idx_allocator()		{ release(); }

		void				init()						{ mAllocator->init(); }
		void				clear()						{ mAllocator->clear(); }

		u32					size() const				{ return mAllocator->size(); }
		u32					max_size() const			{ return mAllocator->max_size(); }

		u32					iallocate(T*& p)			{ void* mem; u32 i = mAllocator->iallocate(mem); C c; p = c.construct(mem); return i; }
		void				ideallocate(u32 idx)		{ T* p = to_ptr(idx); C c; c.destruct(p); mAllocator->ideallocate(idx); }

		T*					to_ptr(u32 idx) const		{ void* p = mAllocator->to_ptr(idx); return (T*)p; }
		u32					to_idx(T* p) const			{ u32 i = mAllocator->to_idx((void*)p); return i; }

		void				release()					{ mAllocator->release(); mAllocator = NULL; }
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif	/// __XBASE_IDX_ALLOCATOR_H__
