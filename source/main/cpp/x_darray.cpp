#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_debug.h"
#include "xbase/x_context.h"
#include "xbase/x_darray.h"

namespace xcore
{
    // typedef void* (*set_capacity_fn)(void* darray, u32 sizeof_item, u64 new_item_count);

	// 32 bytes
	// size of this structure needs to be power-of-2
    struct darray_header_t
    {
		set_capacity_fn m_set_cap_fn;
		void*           __dummy2__;
		u32             __dummy1__;
		u32             m_max_cap;
		u32             m_cur_cap;
		u32             m_size;
	};

    bool __set_capacity(void*& darray, u32 sizeof_item, u32 new_capacity)
    {
        darray_header_t* header = (darray_header_t*)((xbyte*)darray - sizeof(darray_header_t));

        if (new_capacity > header->m_max_cap)
            return false;

        alloc_t*  alloc     = context_t::runtime_alloc();
        u32 const allocsize = sizeof_item * new_capacity + sizeof(darray_header_t);
        void*     newmem = alloc->allocate(allocsize, sizeof(darray_header_t));

        darray_header_t* newheader = (darray_header_t*)((xbyte*)newmem);
        newheader->m_size          = header->m_size;
        newheader->m_cur_cap       = new_capacity;
        newheader->m_max_cap       = header->m_max_cap;
        newheader->m_set_cap_fn    = header->m_set_cap_fn;

        alloc->deallocate((void*)((xbyte*)darray - sizeof(darray_header_t)));

        darray = (void*)(newheader + 1);
        return true;
    }

    void* __create_darray(u32 sizeof_item, u32 initial_size, u32 initial_capacity, u32 maximum_capacity, set_capacity_fn set_capacity)
    {
        alloc_t*  alloc     = context_t::runtime_alloc();
        u32 const allocsize = sizeof_item * initial_capacity + (sizeof(darray_header_t));
        void*     darray    = alloc->allocate(allocsize, sizeof(darray_header_t));

        if (set_capacity == nullptr)
            set_capacity = __set_capacity;

        darray_header_t* header = (darray_header_t*)((xbyte*)darray);
        header->m_size          = initial_size;
        header->m_cur_cap       = initial_capacity;
        header->m_max_cap       = maximum_capacity;
        header->m_set_cap_fn    = set_capacity;

        return (void*)((xbyte*)darray + sizeof(darray_header_t));
    }

    void __destroy_darray(void*& darray)
    {
        alloc_t* alloc      = context_t::runtime_alloc();
        void*    darray_ptr = (xbyte*)darray - sizeof(darray_header_t);
        alloc->deallocate(darray_ptr);
		darray = nullptr;
    }

  	bool  __set_size(void* darray, u32 size)
    {
        darray_header_t* header = (darray_header_t*)((xbyte*)darray - sizeof(darray_header_t));
        if (size <= header->m_cur_cap)
        {
            header->m_size = size;
            return true;
        }
        return false;
    }


    bool __set_cap(void*& darray, u32 sizeof_item, u32 new_capacity)
    {
        darray_header_t* header = (darray_header_t*)((xbyte*)darray - sizeof(darray_header_t));
        return header->m_set_cap_fn(darray, sizeof_item, new_capacity);
    }

    bool __add_item(void*& darray, u32 sizeof_item, void* item)
    {
        darray_header_t* header = (darray_header_t*)((xbyte*)darray - sizeof(darray_header_t));

        if (header->m_size >= header->m_cur_cap)
        {
            u32 const new_capacity = ((header->m_size * 3) / 2);
            if (header->m_set_cap_fn(darray, sizeof_item, new_capacity))
                return false;
        }

        if (sizeof_item < 4)
        {
            x_memcpy((xbyte*)darray + header->m_size * sizeof_item, item, sizeof_item);
        }
        else
        {
            u32 const* src = (u32*)item;
            u32 const* end = (u32*)item + (sizeof_item >> 2);
            u32*       dst = (u32*)((xbyte*)darray + header->m_size * sizeof_item);
            while (src < end)
            {
                *dst++ = *src++;
            }
        }
        header->m_size += 1;
        return true;
    }

    bool __set_item(void*& darray, u32 sizeof_item, u32 index, void* item)
    {
        darray_header_t* header = (darray_header_t*)((xbyte*)darray - sizeof(darray_header_t));
        if (index > header->m_size)
            return false;

        if (sizeof_item < 4)
        {
            x_memcpy((xbyte*)darray + index * sizeof_item, item, sizeof_item);
        }
        else
        {
            u32 const* src = (u32*)item;
            u32 const* end = (u32*)item + (sizeof_item >> 2);
            u32*       dst = (u32*)((xbyte*)darray + index * sizeof_item);
            while (src < end)
            {
                *dst++ = *src++;
            }
        }
        return true;
    }

    void* __get_item(void* darray, u32 sizeof_item, u32 index)
    {
        darray_header_t* header = (darray_header_t*)((xbyte*)darray - sizeof(darray_header_t));
        if (index >= header->m_size)
            return nullptr;
        void* item = ((xbyte*)darray + index * sizeof_item);
        return item;
    }

    bool __equal_items(void* darray, u32 sizeof_item, u32 lhs_index, u32 rhs_index)
    {
        darray_header_t* header = (darray_header_t*)((xbyte*)darray - sizeof(darray_header_t));
        if (lhs_index >= header->m_size || rhs_index >= header->m_size)
            return nullptr;
        void const* lhs = ((xbyte const*)darray + lhs_index * sizeof_item);
        void const* rhs = ((xbyte const*)darray + rhs_index * sizeof_item);
        return x_memcmp(lhs, rhs, sizeof_item);
    }

    s32 __compare_items(void* darray, u32 sizeof_item, u32 lhs_index, u32 rhs_index)
    {
        darray_header_t* header = (darray_header_t*)((xbyte*)darray - sizeof(darray_header_t));
		ASSERT(lhs_index >= header->m_size || rhs_index >= header->m_size);
        void const* lhs = ((xbyte const*)darray + lhs_index * sizeof_item);
        void const* rhs = ((xbyte const*)darray + rhs_index * sizeof_item);
        return x_memcmp(lhs, rhs, sizeof_item);
    }

}; // namespace xcore
