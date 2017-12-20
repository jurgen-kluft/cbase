// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------


inline void			xbuffer::reset()
{
	for (u32 i = 0; i < size(); ++i)
		m_data[i] = 0;
}

inline void			xbuffer::clear()
{
	for (u32 i = 0; i < size(); ++i)
		m_data[i] = 0;
}

inline xbuffer		xbuffer::buffer() const 
{
	return buffer_at(0); 
}

inline xbuffer		xbuffer::buffer_at(u64 idx) const
{ 
	ASSERT(idx < m_len); 
	return xbuffer(size() - idx, (xbyte*)&m_data[idx]); 
}

inline xcbuffer		xbuffer::cbuffer() const 
{ 
	return cbuffer_at(0); 
}

inline xcbuffer		xbuffer::cbuffer_at(u64 idx) const
{ 
	ASSERT(idx < m_len); 
	return xcbuffer(size() - idx, (xbyte*)&m_data[idx]); 
}

inline void			xbuffer::write(const xbuffer& other) 
{ 
	write_at(0, other.cbuffer()); 
}

inline void			xbuffer::write(const xcbuffer& other) 
{ 
	write_at(0, other); 
}

inline void			xbuffer::write_at(u64 idx, const xcbuffer& other)
{
	ASSERT(idx < m_len);
	xbyte* dst = &m_data[idx];
	u64 dst_size = m_len - idx;
	for (u64 i = 0; i < other.size() && i < dst_size; i++)
		dst[i] = other[i];
}


inline s32			xbuffer::compare(const xbuffer& other) const
{
	if (size() < other.size())
		return -1;
	if (size() > other.size())
		return 1;
	for (u64 i = 0; i < size(); ++i)
	{
		if (m_data[i] < other.m_data[i])
			return -1;
		else if (m_data[i] > other.m_data[i])
			return 1;
	}
	return 0;
}

inline xbuffer&	xbuffer::operator = (const xbuffer& other)
{
	u64 i = 0;
	for (; i < size() && i < other.size(); ++i)
		m_data[i] = other.m_data[i];
	for (; i < size(); ++i)
		m_data[i] = 0;
	return *this;
}

inline xbuffer&	xbuffer::operator = (const xcbuffer& other)
{
	u64 i = 0;
	for (; i < size() && i < other.size(); ++i)
		m_data[i] = other.m_data[i];
	for (; i < size(); ++i)
		m_data[i] = 0;
	return *this;
}

inline bool		xbuffer::operator == (const xbuffer& other) const
{
	if (size() < other.size())
		return false;
	if (size() > other.size())
		return false;
	for (u64 i = 0; i < size(); ++i)
	{
		if (m_data[i] != other.m_data[i])
			return false;
	}
	return true;
}

inline bool		xbuffer::operator != (const xbuffer& other) const
{
	if (size() < other.size())
		return true;
	if (size() > other.size())
		return true;
	for (u64 i = 0; i < size(); ++i)
	{
		if (m_data[i] != other.m_data[i])
			return true;
	}
	return false;
}

inline xbyte&		xbuffer::operator [] (u64 i)
{
	ASSERT(i >= 0 && i < (s32)size());
	return m_data[i];
}

inline xbyte		xbuffer::operator [] (u64 i) const
{
	ASSERT(i >= 0 && i < (s32)size());
	return m_data[i];
}




inline s32			xcbuffer::compare(const xcbuffer& other) const
{
	if (size() < other.size())
		return -1;
	if (size() > other.size())
		return 1;
	for (u64 i = 0; i < size(); ++i)
	{
		if (m_data[i] < other.m_data[i])
			return -1;
		else if (m_data[i] > other.m_data[i])
			return 1;
	}
	return 0;
}

inline bool		xcbuffer::operator == (const xcbuffer& other) const
{
	if (size() < other.size())
		return false;
	if (size() > other.size())
		return false;
	for (u64 i = 0; i < size(); ++i)
	{
		if (m_data[i] != other.m_data[i])
			return false;
	}
	return true;
}

inline bool		xcbuffer::operator != (const xcbuffer& other) const
{
	if (size() < other.size())
		return true;
	if (size() > other.size())
		return true;
	for (u64 i = 0; i < size(); ++i)
	{
		if (m_data[i] != other.m_data[i])
			return true;
	}
	return false;
}

inline xbyte		xcbuffer::operator [] (u64 i) const
{
	ASSERT(i >= 0 && i < size());
	return m_data[i];
}

