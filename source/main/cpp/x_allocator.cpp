#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_base.h"
#include "xbase/x_buffer.h"

namespace xcore
{
	xalloc_stack::xalloc_stack(xbuffer& storage)
		: m_base(storage.m_mutable)
		, m_ptr(storage.m_mutable)
		, m_end(storage.m_mutable + storage.m_len)
		, m_cnt(0)
	{
	}

	void*		xallocinplace::allocate(xsize_t size, u32 align)
	{
		ASSERT(size <= m_size);
		return m_data;
	}

	void		xallocinplace::deallocate(void* p)
	{
	}

	void		xallocinplace::release()
	{
	}


};