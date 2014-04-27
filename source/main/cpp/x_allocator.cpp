#include "xbase\x_target.h"
#include "xbase\x_allocator.h"
#include "xbase\x_debug.h"
#include "xbase\x_integer.h"
#include "xbase\x_memory_std.h"

namespace xcore
{
	// Simple first fit allocator
	//
	// This allocator has 2 weaknesses:
	// 1. Has to scan through a list to find a free node
	// 2. This is a first fit allocator and the consequence of 
	//    this is that fragmentation will be high when doing a 
	//    lot of allocations and deallocations
	//
	// Minimum size and alignment is 16 bytes.
	//
	namespace ffa
	{
		struct node
		{
			node*			mPrev;
			node*			mNext;
			u32				mSize;
			u32				mFlags;

			u32				size() const				{ return mSize; }
			bool			used() const				{ return (mFlags&1)==1; }
			
			void			set_used()					{ mFlags = mFlags |  1; }
			void			set_not_used()				{ mFlags = mFlags & ~1; }

			void*			to_ptr() const				{ return (void*)(this + 1); }
			static node*	from_ptr(void* ptr)			{ node* n = (node*)ptr; n = n - 1; return n; }
		};

		struct root
		{
			inline	root() : mTail(NULL)				{ mTail = NULL; mMinSize = 16; mMinAlign = 16;}

			node*	mTail;

			xsize_t	mMinSize;
			u32		mMinAlign;

			xsize_t	min_size(xsize_t size) const		{ return x_intu::max(size, mMinSize); }
			u32		min_align(u32 align) const			{ return x_intu::max(align, mMinAlign); }
		};

		static void*	ptr_align(void* ptr, u32 alignment)
		{
			return (void*)(((X_PTR_SIZED_INT)ptr + (X_PTR_SIZED_INT)(alignment-1)) & ~((X_PTR_SIZED_INT)(alignment-1)));
		}

		static void		init(root *s, void *mem, xsize_t size, u32 default_minimum_size, u32 default_alignment)
		{
			s->mTail         = (node*)mem;
			s->mMinSize      = x_intu::max( (xcore::u32)sizeof(node), default_minimum_size);
			s->mMinAlign     = x_intu::max( (xcore::u32)sizeof(node), default_alignment);
			s->mMinAlign     = x_intu::ceilPower2(s->mMinAlign);

			s->mTail->mPrev  = NULL;
			s->mTail->mNext  = NULL;
			s->mTail->mSize  = (u32)(size - sizeof(node));
			s->mTail->mFlags = 0;
		}

		static inline 
		xsize_t			compute_size(node* cell, xsize_t size, u32 alignment)
		{
			node* aligned_cell = node::from_ptr(ptr_align(cell->to_ptr(), alignment));
			u32 offset = (u32)((xbyte*)aligned_cell - (xbyte*)cell);
			return offset + size;
		}

		static inline
		u32				align_node(node*& cell, u32 alignment)
		{
			node* aligned_cell = node::from_ptr(ptr_align(cell->to_ptr(), alignment));
			u32 const offset = (u32)((xbyte*)aligned_cell - (xbyte*)cell);
			if (offset != 0)
			{
				node* cell_prev = cell->mPrev;
				node* cell_next = cell->mNext;
				u32 cell_size   = cell->mSize;
				u32 cell_flags  = cell->mFlags;
				aligned_cell->mPrev  = cell_prev;
				aligned_cell->mNext  = cell_next;
				aligned_cell->mSize  = cell_size - offset;
				aligned_cell->mFlags = cell_flags;
				cell = aligned_cell;
			}
			return offset;
		}

		static void*	allocate(root *s, xsize_t size, u32 alignment)
		{
			node *c1, *c2, *c3;

			size      = s->min_size(size);
			size      = x_intu::alignUp((s32)size, s->mMinAlign);

			alignment = s->min_align(alignment);

			// Search a free node (first-fit)
			c1 = s->mTail;
			while (true)
			{
				if (!c1->used())
				{
					if (compute_size(c1, size, alignment) <= c1->mSize)
						break;
				}

				if (c1->mNext == NULL)
					return NULL;

				c1 = c1->mNext;
			}

			// c2 might 'move' due to the alignment request and this 
			// means that 'c1->mNext' and 'c1->mSize' need to be updated
			// or that we need to create an additional node to handle
			// the size that is left
			c2 = c1;
			u32 const c2_offset = align_node(c2, alignment);
			if (c2_offset > 0)
			{
				if (c2_offset <= (s->mMinSize + sizeof(node)))
				{
					if (s->mTail == c1)
						s->mTail = c2;

					c1 = c2->mPrev;
					if (c1 != NULL)
					{
						// Update the size and next pointer of the previous node
						c1->mSize += c2_offset;
						c1->mNext  = c2;
					}
				}
				else
				{
					c1->mNext = c2;
					c1->mPrev = c2->mPrev;
					c2->mPrev = c1;
					c1->mSize = c2_offset - (sizeof(node));
				}
			}

			if (c2->mSize > (sizeof(node) + s->mMinSize + size))
			{	// split new cell
				c3 = (node*)(xmem_utils::ptr_add(c2->to_ptr(), (s32)size));
				c3->mSize   = c2->mSize - (u32)size - sizeof(node);
				c3->mFlags  = 0;
				c3->mNext   = c2->mNext;
				c3->mPrev   = c2;

				c2->mNext   = c3;
				c2->mSize   = (u32)size;
				c2->set_used();
			}
			else
			{	// use the entire cell
				c2->set_used();
			}

			return c2->to_ptr();
		}

		static void		deallocate(root *s, void *ptr)
		{
			node* c2 = node::from_ptr(ptr);

			// Remove the 'used' flag
			c2->set_not_used();

			// Merging:
			node* c1 = c2->mPrev;
			node* c3 = c2->mNext;
			if (c1!=NULL && !c1->used())
			{
				c1->mSize += c2->mSize + sizeof(node);
				c1->mNext  = c3;
				if (c3 != NULL)
					c3->mPrev = c1;
				c2 = c1;
			}
			if (c3!=NULL && !c3->used())
			{
				c2->mSize += c3->mSize + sizeof(node);
				c2->mNext  = c3->mNext;

				node* c4 = c3->mNext;
				if (c4!=NULL)
					c4->mPrev = c2;
			}
		}

		static void*	reallocate(root* s, void* old_ptr, xsize_t new_size, u32 alignment)
		{
			void* p = NULL;
			if (old_ptr==NULL)
			{
				p = allocate(s, new_size, alignment);
			}
			else if (new_size == 0)
			{
				deallocate(s, old_ptr);
			}
			else
			{
				node* c2 = node::from_ptr(old_ptr);

				// Don't do anything when the new size request is smaller
				if (new_size <= c2->mSize)
				{
					p = old_ptr;
				}
				else
				{
					// Here we have to allocate to fulfill the new size request
					p = allocate(s, new_size, alignment);

				}
			}
			return NULL;
		}

	}


	class x_allocator_basic : public x_iallocator
	{
	public:
		x_allocator_basic()
			: mAllocationCount(0)
		{
		}

		void					init(void* mem_begin, u32 mem_size, u32 default_minimum_size, u32 default_alignment) 
		{
			mAllocationCount  = 0;
			ffa::init(&mRoot, mem_begin, mem_size, default_minimum_size, default_alignment);
		}

		virtual const char*		name() const
		{
			return "Simple first-fit allocator";
		}

		virtual void*			allocate(xsize_t size, u32 alignment)
		{
			++mAllocationCount;
			void* p = ffa::allocate(&mRoot, size, alignment);
			
			ASSERTS(p != NULL, "allocation failed, ERROR!!\n");
			return p;
		}

		virtual void*			reallocate(void* ptr, xsize_t size, u32 alignment)
		{
			void* p = ffa::allocate(&mRoot, size, alignment);

			ffa::node* ptr_node = ffa::node::from_ptr(ptr);
			x_memcpy(p, ptr, ptr_node->mSize < (u32)size ? ptr_node->mSize : (u32)size);

			ffa::deallocate(&mRoot, ptr);

			return p;
		}

		virtual void			deallocate(void* ptr)
		{
			--mAllocationCount;
			ffa::deallocate(&mRoot, ptr);
		}

		virtual void			release()
		{
			ASSERTS(mAllocationCount==0, "ERROR: Simple first-fit allocator is being released but still has allocations that are not freed");
			mAllocationCount = 0;
			ASSERT(mRoot.mTail->mPrev == NULL);
			ASSERT(mRoot.mTail->mNext == NULL);
			ASSERT(mRoot.mTail->used() == false);
		}

		void*		operator new(xcore::xsize_t num_bytes, void* mem)			{ return mem; }
		void		operator delete(void* mem, void* )							{ }
		void		operator delete(void* mem)									{ }						

	protected:
		ffa::root	mRoot;
		u32			mAllocationCount;
	};

	x_iallocator*		gCreateBasicAllocator(void* mem_begin, u32 mem_size, u32 default_minimum_size, u32 default_alignment)
	{
		x_allocator_basic* ba = new (mem_begin) x_allocator_basic();
		mem_begin = xmem_utils::ptr_add(mem_begin, sizeof(x_allocator_basic));
		mem_size -= sizeof(x_allocator_basic);
		ba->init(mem_begin, mem_size, default_minimum_size, default_alignment);
		return ba;
	}


};


