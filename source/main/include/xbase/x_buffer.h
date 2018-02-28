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

	template<typename T>
	class xcuchars_t;
	typedef	xcuchars_t<char> xcuchars;

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	class xbuffer
	{
	public:
		inline		xbuffer() : m_len(0), m_data((xbyte*)&m_len) { }
		inline		xbuffer(s64 len, xbyte* data) : m_len(len), m_data(data) { reset(0); }

		s64			size() const { return m_len; }

		void		reset(xbyte fill);
		void		clear();

		s32			compare(const xbuffer& other) const;

		xbuffer&	operator = (const xbuffer& other);

		bool		operator == (const xbuffer& other) const;
		bool		operator != (const xbuffer& other) const;

		xbyte&		operator [] (s64 i);
		xbyte		operator [] (s64 i) const;

		xbuffer		operator () (s64 from, s64 to) const;

		s64			m_len;
		xbyte*		m_data;
	};

	template<s64 L>
	class xbytes : public xbuffer
	{
		enum {SIZE=L};
	public:
		xbyte		m_data[len];
		inline		name() : xbuffer(len, m_data) {}
	};

	// Inline functions
	#include "xbase/private/x_buffer.h"

	//
	// Helper classes for reading and writing to buffers
	//

	class xbinary_reader
	{
	public:
		inline				xbinary_reader(xcbuffer& _buffer) : cursor_(0), buffer_(_buffer) {}

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
		bool				view_string(xcuchars& str);

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
		u32					write_string(xcuchars const& str);

		xbinary_writer		operator () (s32 length) const;		// For writing something in the future you can remember this place - length

	protected:
		u32					cursor_;
		xbuffer				buffer_;
	};

}

#endif	///< __XCORE_BUFFER_H__