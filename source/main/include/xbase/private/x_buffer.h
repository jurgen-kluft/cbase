
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
inline				xchars::xchars(uchar* str)
	: m_str(str)
	, m_end(str)
	, m_eos(str)
{
	while (*m_end != '\0')
		++m_end;
	m_eos = m_end;
}
inline				xchars::xchars(uchar* str, uchar* end)
	: m_str(str)
	, m_end(end)
	, m_eos(end)
{
}

inline				xchars::xchars(uchar* str, uchar* end, uchar* eos)
	: m_str(str)
	, m_end(end)
	, m_eos(eos)
{
}

inline				xchars::xchars(uchar* str, uchar* eos, const xchars& other)
	: m_str(str)
	, m_end(str)
	, m_eos(eos)
{
	copy(other);
}

inline u32			xchars::size() const { return m_end - m_str; }

inline void			xchars::reset()
{
	m_str[0] = '\0';
	m_end = m_str;
}

inline void			xchars::clear()
{
	m_str[0] = '\0';
	m_end = m_str;
	uchar* str = m_str;
	while (str < m_eos)
		*str++ = '\0';
}

inline s32			xchars::copy(const xchars& _other)
{
	xcchars other(_other.m_str, _other.m_end);
	return copy(other);
}

inline s32			xchars::copy(const xcchars& other)
{
	uchar const* src = other.m_str;
	uchar * dst = m_str;

	while (dst < m_eos && src <= other.m_end)
	{
		*dst++ = *src++;
	}

	m_end = dst;
	if (dst < m_eos)
		m_end--;

	return m_end - m_str;
}

inline s32			xchars::compare(const xchars& _other) const
{
	xcchars other(_other.m_str, _other.m_end);
	return compare(other);
}

inline s32			xchars::compare(const xcchars& other) const
{
	if (size() < other.size())
		return -1;
	if (size() > other.size())
		return 1;
	for (u32 i = 0; i < size(); ++i)
	{
		if (m_str[i] < other.m_str[i])
			return -1;
		else if (m_str[i] > other.m_str[i])
			return 1;
	}
	return 0;
}

inline xchars&	xchars::operator = (const xchars& other)
{
	copy(other);
	return *this;
}

inline xchars&	xchars::operator = (const xcchars& other)
{
	copy(other);
	return *this;
}

inline bool		xchars::operator == (const xchars& other) const
{
	return compare(other) == 0;
}

inline bool		xchars::operator != (const xchars& other) const
{
	return compare(other) != 0;
}

inline bool		xchars::operator == (const xcchars& other) const
{
	return compare(other) == 0;
}

inline bool		xchars::operator != (const xcchars& other) const
{
	return compare(other) != 0;
}

inline uchar		xchars::operator [] (int i) const
{
	if (i >= 0 && i < size())
		return m_str[i];
	return '\0';
}



inline s32			xcchars::compare(const xchars& _other) const
{
	xcchars other(_other.m_str, _other.m_end);
	return compare(other);
}

inline s32			xcchars::compare(const xcchars& other) const
{
	if (size() < other.size())
		return -1;
	if (size() > other.size())
		return 1;
	for (u32 i = 0; i < size(); ++i)
	{
		if (m_str[i] < other.m_str[i])
			return -1;
		else if (m_str[i] > other.m_str[i])
			return 1;
	}
	return 0;
}

inline bool		xcchars::operator == (const xchars& other) const
{
	return compare(other) == 0;
}

inline bool		xcchars::operator != (const xchars& other) const
{
	return compare(other) != 0;
}

inline bool		xcchars::operator == (const xcchars& other) const
{
	return compare(other) == 0;
}

inline bool		xcchars::operator != (const xcchars& other) const
{
	return compare(other) != 0;
}

inline uchar		xcchars::operator [] (int i) const
{
	if (i >= 0 && i < size())
		return m_str[i];
	return '\0';
}


// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------


inline void			xbuffer::reset()
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

inline xbuffer&	xbuffer::operator = (const xcbuffer& other)
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

