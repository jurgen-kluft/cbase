inline void		xbuffer::reset()
{
	for (u32 i = 0; i < size(); ++i)
		m_data[i] = 0;
}

inline s32			xbuffer::compare(const xbuffer& other) const
{
	if (size() < other.size())
		return -1;
	if (size() > other.size())
		return 1;
	for (u32 i = 0; i < size(); ++i)
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
	u32 i = 0;
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
	for (u32 i = 0; i < size(); ++i)
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
	for (u32 i = 0; i < size(); ++i)
	{
		if (m_data[i] != other.m_data[i])
			return true;
	}
	return false;
}

inline xbyte		xbuffer::operator [] (int i) const
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
	for (u32 i = 0; i < size(); ++i)
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
	for (u32 i = 0; i < size(); ++i)
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
	for (u32 i = 0; i < size(); ++i)
	{
		if (m_data[i] != other.m_data[i])
			return true;
	}
	return false;
}

inline xbyte		xcbuffer::operator [] (int i) const
{
	ASSERT(i >= 0 && i < (s32)size());
	return m_data[i];
}


