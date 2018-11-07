#ifndef __XBASE_SLICE_H__
#define __XBASE_SLICE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	struct slice_data;

	//==============================================================================
	// Slice
	// A reference counted slice owning a memory block with a view/window (from,to).
	//==============================================================================
	struct slice
	{
						slice();
						slice(xalloc* allocator, u32 item_count, u32 item_size);
						slice(slice_data* data, s32 from, s32 to);

		static void		alloc(slice& slice, xalloc* allocator, u32 item_count, u32 item_size);

		u32				size() const;
		u32				refcnt() const;

		void			resize(u32 count);

		slice			view(u32 from, u32 to) const;

		slice			obtain() const;
		void			release();

		void*			get_at(s32 index);
		void const*		get_at(s32 index) const;

		slice_data*		mData;
		u32				mFrom;
		u32				mTo;
	};


	// ----------------------------------------------------------------------------------------
	//   SLICE REFERENCE COUNTED DATA
	// ----------------------------------------------------------------------------------------

	struct slice_data
	{
		slice_data();
		slice_data(u32 item_count, u32 item_size);
		slice_data(xbyte* data, u32 item_count, u32 item_size);

		static slice_data	sNull;

		slice_data* 		incref();
		slice_data* 		incref() const;
		slice_data*			decref();

		slice_data*			resize(s32 from, s32 to);

		static slice_data*	alloc(xalloc* allocator, u32& to_itemcount, u32& to_itemsize);

		mutable s32			mRefCount;
		u32					mItemCount;					/// Count of total items
		u32					mItemSize;					/// Size of one item
		u32					mDummy;
		xalloc*		mAllocator;
		xbyte*				mData;
	};
}

#endif
