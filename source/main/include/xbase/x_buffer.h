#ifndef __XCORE_BUFFER_H__
#define __XCORE_BUFFER_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_allocator.h"
#include "xbase/x_memory_std.h"
#include "xbase/x_debug.h"

namespace xcore
{
	class xbuffer;

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	class xcbuffer
	{
	public:
		inline		xcbuffer() : m_len(0), m_const((xbyte const*)&m_len) { }
		inline		xcbuffer(s64 len, xbyte const* data) : m_len(len), m_const(data) { }

		inline s64	size() const									{ return m_len; }

		s32			compare(const xcbuffer& other) const;

		bool		operator == (const xcbuffer& other) const;
		bool		operator != (const xcbuffer& other) const;
		bool		operator <  (const xcbuffer& other) const;
		bool		operator <= (const xcbuffer& other) const;
		bool		operator >  (const xcbuffer& other) const;
		bool		operator >= (const xcbuffer& other) const;

		xcbuffer	operator () (s64 from, s64 to) const;

		s64			m_len;
		xbyte const* m_const;
	};

	class xbuffer : public xcbuffer
	{
	public:
		inline		xbuffer() : xcbuffer(), m_mutable((xbyte*)&m_len) { }
		inline		xbuffer(s64 len, xbyte* data) : xcbuffer(len, data), m_mutable(data) { reset(0); }

		void		reset(xbyte fill);
		void		clear();

		xbuffer&	operator = (const xcbuffer& other);
		xbuffer		operator () (s64 from, s64 to) const;

		xbyte*		m_mutable;
	};

	template<s64 L>
	class xbytes : public xbuffer
	{
		enum {SIZE=L};
	public:
		xbyte		m_data[SIZE];

		inline		xbytes() : xbuffer(SIZE, m_data) {}

		xbytes<L>&	operator = (const xcbuffer& other) { this->xbuffer::operator=(other); return *this; }
		xbuffer		operator () (s64 from, s64 to) const { return this->xbuffer::operator()(from, to); }
	};

	// Inline functions
	#include "xbase/private/x_buffer.h"

	//
	// Helper classes for reading and writing to buffers
	//

	class xbinary_reader
	{
	public:
		inline				xbinary_reader(xcbuffer const& _buffer) : cursor_(0), buffer_(_buffer) {}

		u32					get_size() const;

		bool				can_read(u32 number_of_bytes) const;		// check if we still can read n number of bytes

		void				reset();
		void				skip(s32);

		u32					read(bool&);
		u32					read(u8  &);
		u32					read(s8  &);
		u32					read(u16 &);
		u32					read(s16 &);
		u32					read(u32 &);
		u32					read(s32 &);
		u32					read(u64 &);
		u32					read(s64 &);
		u32					read(f32 &);
		u32					read(f64 &);

		bool				read_data(xbuffer& buf);
		bool				view_data(u32 size, xcbuffer& buf);
		bool				view_string(u32 size, const char*& str, const char*& end);

	protected:
		u32					cursor_;
		xcbuffer			buffer_;
	};

	class xbinary_writer
	{
	public:
		inline				xbinary_writer() : cursor_(0), buffer_() { }
		inline				xbinary_writer(xbuffer& _buffer) : cursor_(0), buffer_(_buffer) { }

		xbuffer				get_current_buffer() const;

		bool				can_write(u32 num_bytes = 0) const;

		void				reset();
		void				skip(s32);

		u32					write(bool);
		u32					write(u8);
		u32					write(s8);
		u32					write(u16);
		u32					write(s16);
		u32					write(u32);
		u32					write(s32);
		u32					write(u64);
		u32					write(s64);
		u32					write(f32);
		u32					write(f64);

		u32					write_data(xbuffer const& cbuf);
		u32					write_data(xcbuffer const& cbuf);
		u32					write_string(char const* str, char const* end);

		xbinary_writer		operator () (s32 length) const;		// For writing something in the future you can remember this place - length

	protected:
		u32					cursor_;
		xbuffer				buffer_;
	};


	inline s32			xcbuffer::compare(const xcbuffer& other) const
	{
		if (size() < other.size())
			return -1;
		if (size() > other.size())
			return 1;
		for (s64 i = 0; i < size(); ++i)
		{
			if (m_const[i] < other.m_const[i])
				return -1;
			else if (m_const[i] > other.m_const[i])
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
		for (s64 i = 0; i < size(); ++i)
		{
			if (m_const[i] != other.m_const[i])
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
		for (s64 i = 0; i < size(); ++i)
		{
			if (m_const[i] != other.m_const[i])
				return true;
		}
		return false;
	}

	inline bool		xcbuffer::operator < (const xcbuffer& other) const
	{
		if (size() < other.size())
			return true;
		if (size() > other.size())
			return false;
		for (s64 i = 0; i < size(); ++i)
		{
			if (m_const[i] >= other.m_const[i])
				return false;
		}
		return true;
	}

	inline bool		xcbuffer::operator <= (const xcbuffer& other) const
	{
		if (size() < other.size())
			return true;
		if (size() > other.size())
			return false;
		for (s64 i = 0; i < size(); ++i)
		{
			if (m_const[i] > other.m_const[i])
				return false;
		}
		return true;
	}

	inline bool		xcbuffer::operator > (const xcbuffer& other) const
	{
		if (size() < other.size())
			return false;
		if (size() > other.size())
			return true;
		for (s64 i = 0; i < size(); ++i)
		{
			if (m_const[i] <= other.m_const[i])
				return false;
		}
		return true;
	}

	inline bool		xcbuffer::operator >= (const xcbuffer& other) const
	{
		if (size() < other.size())
			return false;
		if (size() > other.size())
			return true;
		for (s64 i = 0; i < size(); ++i)
		{
			if (m_const[i] < other.m_const[i])
				return false;
		}
		return true;
	}

	inline xcbuffer		xcbuffer::operator () (s64 from, s64 to) const
	{
		if (from < 0 || from >= to || from >= size())
			return xcbuffer();
		if (to >= size())
			return xcbuffer();
		return xcbuffer(to - from, m_const + from);
	}








	inline void			xbuffer::reset(xbyte fill)
	{
		for (u32 i = 0; i < size(); ++i)
			m_mutable[i] = fill;
	}

	inline void			xbuffer::clear()
	{
		for (u32 i = 0; i < size(); ++i)
			m_mutable[i] = 0;
	}

	inline xbuffer&	xbuffer::operator = (const xcbuffer& other)
	{
		s64 i = 0;
		for (; i < size() && i < other.size(); ++i)
			m_mutable[i] = other.m_const[i];
		for (; i < size(); ++i)
			m_mutable[i] = 0;
		return *this;
	}

	inline xbuffer		xbuffer::operator () (s64 from, s64 to) const
	{
		if (from < 0 || from >= to || from >= size())
			return xbuffer();
		if (to >= size())
			return xbuffer();
		return xbuffer(to - from, m_mutable + from);
	}

}

#endif	///< __XCORE_BUFFER_H__