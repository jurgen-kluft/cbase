#ifndef __XCORE_BUFFER_H__
#define __XCORE_BUFFER_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
	#pragma once
#endif

#include "xbase/x_allocator.h"
#include "xbase/x_memory.h"
#include "xbase/x_debug.h"

namespace xcore
{
	class xbuffer;
	class xbinary_reader;
	class xbinary_writer;

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	class xcbuffer
	{
	public:
		inline			xcbuffer() : m_len(0), m_const((xbyte const*) & m_len) { }
		inline			xcbuffer(u32 len, xbyte const* data) : m_len(len), m_const(data) { }
						xcbuffer(xbuffer const& buffer);

		inline u32		size() const                                    { return m_len; }

		s32				compare(const xcbuffer& other) const;

		bool			operator == (const xcbuffer& other) const;
		bool			operator != (const xcbuffer& other) const;
		bool			operator < (const xcbuffer& other) const;
		bool			operator <= (const xcbuffer& other) const;
		bool			operator > (const xcbuffer& other) const;
		bool			operator >= (const xcbuffer& other) const;

		xcbuffer		operator()(u32 from, u32 to) const;

		xbinary_reader	reader() const;

		u32				m_len;
		xbyte const*	m_const;
	};

	class xbuffer
	{
	public:
		inline      	xbuffer() : m_len(0), m_mutable((xbyte*) & m_len) { }
		inline      	xbuffer(u32 len, xbyte* data) : m_len(len), m_mutable(data) { reset(0); }

		inline u32  	size() const                                    { return m_len; }

		void        	reset(xbyte fill);
		void        	clear();

		s32         	compare(const xcbuffer& other) const			{ xcbuffer cthis(*this); return cthis.compare(other); }

		bool        	operator == (const xcbuffer& other) const		{ xcbuffer cthis(*this); return cthis == other; }
		bool        	operator != (const xcbuffer& other) const		{ xcbuffer cthis(*this); return cthis != other; }
		bool        	operator <  (const xcbuffer& other) const		{ xcbuffer cthis(*this); return cthis <  other; }
		bool        	operator <= (const xcbuffer& other) const		{ xcbuffer cthis(*this); return cthis <= other; }
		bool        	operator >  (const xcbuffer& other) const		{ xcbuffer cthis(*this); return cthis >  other; }
		bool        	operator >= (const xcbuffer& other) const		{ xcbuffer cthis(*this); return cthis >= other; }

		bool        	operator == (const xbuffer& other) const		{ xcbuffer cother(other); xcbuffer cthis(*this); return cthis == cother; }
		bool        	operator != (const xbuffer& other) const		{ xcbuffer cother(other); xcbuffer cthis(*this); return cthis != cother; }
		bool        	operator <  (const xbuffer& other) const		{ xcbuffer cother(other); xcbuffer cthis(*this); return cthis <  cother; }
		bool        	operator <= (const xbuffer& other) const		{ xcbuffer cother(other); xcbuffer cthis(*this); return cthis <= cother; }
		bool        	operator >  (const xbuffer& other) const		{ xcbuffer cother(other); xcbuffer cthis(*this); return cthis >  cother; }
		bool        	operator >= (const xbuffer& other) const		{ xcbuffer cother(other); xcbuffer cthis(*this); return cthis >= cother; }

		xbuffer     	operator()(u32 from, u32 to) const;

		xbinary_reader	reader() const;
		xbinary_writer	writer() const;

		u32         	m_len;
		xbyte*      	m_mutable;
	};

	template<u32 L>
	class xbytes : public xbuffer
	{
		enum {SIZE = L};
	public:
		xbyte			m_data[SIZE];
		inline			xbytes() : xbuffer(SIZE, m_data) {}
		xbuffer			buffer() const { return xbuffer(*this); }
	};

	//
	// Helper classes for reading and writing to buffers
	//

	class xbinary_reader
	{
	public:
		inline              xbinary_reader(xbyte const* _buffer, u32 _len) : len_(_len), cursor_(0), buffer_(_buffer) {}

		u32                 size() const;
		u32                 len() const;

		xcbuffer            get_current_buffer() const;

		bool                can_read(u32 number_of_bytes) const;        // check if we still can read n number of bytes

		void                reset();
		void                skip(s32);

		u32                 read(bool&);
		u32                 read(u8&);
		u32                 read(s8&);
		u32                 read(u16&);
		u32                 read(s16&);
		u32                 read(u32&);
		u32                 read(s32&);
		u32                 read(u64&);
		u32                 read(s64&);
		u32                 read(f32&);
		u32                 read(f64&);

		bool				read_data(u32 size, xbuffer& buf);

		bool                view_data(u32 size, xcbuffer& buf);
		bool                view_string(u32 size, const char*& str, const char*& end);

	protected:
		u32					len_;
		u32                 cursor_;
		xbyte const*		buffer_;
	};

	class xbinary_writer
	{
	public:
		inline              xbinary_writer() : cursor_(0), buffer_() { }
		inline              xbinary_writer(xbyte* _buffer, u32 _len) : cursor_(0), buffer_(_buffer) { }

		u32                 size() const;
		u32                 len() const;

		xbuffer             get_full_buffer() const;
		xbuffer             get_current_buffer() const;

		bool                can_write(u32 num_bytes = 0) const;

		void                reset();
		void                skip(s32);

		u32                 write(bool);
		u32                 write(u8);
		u32                 write(s8);
		u32                 write(u16);
		u32                 write(s16);
		u32                 write(u32);
		u32                 write(s32);
		u32                 write(u64);
		u32                 write(s64);
		u32                 write(f32);
		u32                 write(f64);

		u32                 write_data(xbuffer const& cbuf);
		u32                 write_data(xcbuffer const& cbuf);
		u32                 write_string(char const* str, char const* end);

		xbinary_writer      reserve(u32 size);		// For writing something in the future you can remember this place - size

	protected:
		u32					len_;
		u32                 cursor_;
		xbyte*				buffer_;
	};


	inline				xcbuffer::xcbuffer(xbuffer const& buffer) 
		: m_len(buffer.size())
		, m_const(buffer.m_mutable) 
	{
	}

	inline s32          xcbuffer::compare(const xcbuffer& other) const
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

	inline bool     xcbuffer::operator == (const xcbuffer& other) const
	{
		return compare(other) == 0;
	}

	inline bool     xcbuffer::operator != (const xcbuffer& other) const
	{
		return compare(other) != 0;
	}

	inline bool     xcbuffer::operator < (const xcbuffer& other) const
	{
		return compare(other) == -1;
	}

	inline bool     xcbuffer::operator <= (const xcbuffer& other) const
	{
		return compare(other) <= 0;
	}

	inline bool     xcbuffer::operator > (const xcbuffer& other) const
	{
		return compare(other) == 1;
	}

	inline bool     xcbuffer::operator >= (const xcbuffer& other) const
	{
		return compare(other) >= 0;
	}

	inline xcbuffer     xcbuffer::operator()(u32 from, u32 to) const
	{
		if (from < 0 || from >= to || from >= size())
			return xcbuffer();
		if (to >= size())
			return xcbuffer();
		return xcbuffer(to - from, m_const + from);
	}

	inline xbinary_reader	xcbuffer::reader() const
	{
		return xbinary_reader(m_const, size());
	}




	inline void         xbuffer::reset(xbyte fill)
	{
		for (u32 i = 0; i < size(); ++i)
			m_mutable[i] = fill;
	}

	inline void         xbuffer::clear()
	{
		for (u32 i = 0; i < size(); ++i)
			m_mutable[i] = 0;
	}

	inline xbuffer      xbuffer::operator()(u32 from, u32 to) const
	{
		if (from < 0 || from >= to || from >= size())
			return xbuffer();
		if (to >= size())
			return xbuffer();
		return xbuffer(to - from, m_mutable + from);
	}

	inline xbinary_reader xbuffer::reader() const
	{
		return xbinary_reader(m_mutable, size());
	}

	inline xbinary_writer xbuffer::writer() const
	{
		return xbinary_writer(m_mutable, size());
	}

}

#endif  ///< __XCORE_BUFFER_H__