#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_base.h"
#include "xbase/x_buffer.h"

namespace xcore
{
	alloc_stack_t::alloc_stack_t(buffer_t& storage)
		: m_base(storage.m_mutable)
		, m_ptr(storage.m_mutable)
		, m_end(storage.m_mutable + storage.m_len)
		, m_cnt(0)
	{
	}

	void*		allocinplace_t::v_allocate(u32 size, u32 align)
	{
		ASSERT(size <= m_size);
		void* ptr = m_data;
		m_data = (xbyte*)((u64)m_data + (u64)(align-1) & ~((u64)align - 1));
		m_data = m_data + (size + (align-1) & ~(align - 1));
		ASSERT(m_data <= (m_base + m_size));
		return ptr;
	}

	u32	 	    allocinplace_t::v_deallocate(void* p)
	{
		return 0;
	}

	void		allocinplace_t::v_release()
	{
	}

	static inline u32		get_item_idx(void* array_item, void* item, u32 sizeof_item)
	{
		u32 const index = (u32)(((u64)item - (u64)array_item) / sizeof_item);
		return index;
	}

	static inline xbyte*	get_item_ptr(void* array_item, u32 index, u32 sizeof_item)
	{
		return (xbyte*)array_item + (index * sizeof_item);
	}


	dexed_array_t::dexed_array_t(void* array_item, u32 sizeof_item, u32 countof_item)
		: m_data(array_item)
		, m_sizeof(sizeof_item)
		, m_countof(countof_item)
	{
	}

	void*		dexed_array_t::v_idx2ptr(u32 index) const
	{
		if (index == 0xffffffff)
			return nullptr;
		ASSERT(index < m_countof);
		return get_item_ptr(m_data, index, m_sizeof);
	}

	u32			dexed_array_t::v_ptr2idx(void* ptr) const
	{
		if (ptr == nullptr)
			return 0xffffffff;
		u32 const i = get_item_idx(m_data, ptr, m_sizeof);
		ASSERT(i < m_countof);
		return i;
	}


	fsadexed_array_t::fsadexed_array_t()
        : m_data(nullptr)
        , m_sizeof(0)
        , m_countof(0)
        , m_freelist(0xffffffff)
        , m_freeindex(0xffffffff)
	{
	}

	fsadexed_array_t::fsadexed_array_t(void* array_item, u32 sizeof_item, u32 countof_item)
		: m_data(array_item)
		, m_sizeof(sizeof_item)
		, m_countof(countof_item)
		, m_freelist(0xffffffff)
		, m_freeindex(0)
	{
		ASSERT(m_sizeof >= sizeof(u32));	// Can only deal with items that are 4 bytes or more
	}

	u32			fsadexed_array_t::v_size() const { return m_sizeof; }

	void*		fsadexed_array_t::v_allocate()
	{
		u32 freeitem = 0xffffffff;
		if (m_freelist != 0xffffffff)
		{
			freeitem = m_freelist;
			m_freelist = *(u32*)get_item_ptr(m_data, m_freelist, m_sizeof);
		}
		else if (m_freeindex < m_countof)
		{
			freeitem = m_freeindex++;
		}
		
		if (freeitem == 0xffffffff)
			return nullptr;

		return get_item_ptr(m_data, freeitem, m_sizeof);
	}

	u32		fsadexed_array_t::v_deallocate(void* p)
	{
		u32 const idx = ptr2idx(p);
		u32* item = (u32*)p;
		*item = m_freelist;
		m_freelist = idx;
		return m_sizeof;
	}

	void*		fsadexed_array_t::v_idx2ptr(u32 index) const
	{
		if (index == 0xffffffff)
			return nullptr;
		ASSERT(index < m_freeindex);
		return get_item_ptr(m_data, index, m_sizeof);
	}

	u32			fsadexed_array_t::v_ptr2idx(void* ptr) const
	{
		if (ptr == nullptr)
			return 0xffffffff;
		u32 const i = get_item_idx(m_data, ptr, m_sizeof);
		ASSERT(i < m_freeindex);
		return i;
	}

	void		fsadexed_array_t::v_release()
	{
	}
};