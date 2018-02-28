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
	class xcbuffer;

	template<typename T>
	class xcuchars_t;
	typedef	xcuchars_t<char> xcuchars;

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	class xbuffer
	{
	public:
		static xbuffer	allocate(s64 size, x_iallocator* a);
		static void		reallocate(xbuffer& buf, s64 size, x_iallocator* a);
		static void		deallocate(xbuffer& buf, x_iallocator* a);

		inline		xbuffer() : m_len(0), m_data((xbyte*)&m_len) { }
		inline		xbuffer(s64 len, xbyte* data) : m_len(len), m_data(data) { reset(); }

		s64			size() const					{ return m_len; }

		void		reset();
		void		clear();

		xbuffer		buffer() const;
		xbuffer		buffer_at(s64 idx) const;

		xcbuffer	cbuffer() const;
		xcbuffer	cbuffer_at(s64 idx) const;

		void		write(const xbuffer& other);
		void		write(const xcbuffer& other);

		void		write_at(s64 idx, const xcbuffer& other);

		s32			compare(const xbuffer& other) const;

		xbuffer&	operator = (const xbuffer& other);
		xbuffer&	operator = (const xcbuffer& other);

		bool		operator == (const xbuffer& other) const;
		bool		operator != (const xbuffer& other) const;

		xbyte&		operator [] (s64 i);
		xbyte		operator [] (s64 i) const;


		s64			m_len;
		xbyte*		m_data;
	};

	class xcbuffer
	{
	public:
		inline		xcbuffer() : m_len(0), m_data(NULL) {}
		inline		xcbuffer(s64 len, xbyte const* data) : m_len(len), m_data(data) {}

		s64			size() const { return m_len; }

		s32			compare(const xcbuffer& other) const;

		bool		operator == (const xcbuffer& other) const;
		bool		operator != (const xcbuffer& other) const;

		xcbuffer	operator () (s32 from, s32 to) const;
		xbyte		operator [] (s64 i) const;

		s64			m_len;
		xbyte const*m_data;
	};

	#define XBYTES(name, len)													\
	class name																	\
	{																			\
	public:																		\
		inline		name() { reset(0); }										\
		inline		name(xbyte b) { reset(b); }									\
		inline		name(name const& other) { *this = other; }					\
		inline		name(xbuffer const& other) { *this = other; }				\
		s64			size() const { return len; }								\
		xbuffer		buffer() { return xbuffer(size(), m_data); }				\
		xcbuffer	cbuffer() const { return xcbuffer(size(), m_data); }		\
		void		reset(xbyte fill) {											\
			for (s64 i = 0; i < size(); ++i)									\
				m_data[i] = fill;												\
		}																		\
		void		clear() {													\
			reset(0);															\
		}																		\
		s32			compare(const name& other) const {							\
			for (s64 i = 0; i < size(); ++i) {									\
				if (m_data[i] < other.m_data[i])								\
					return -1;													\
				else if (m_data[i] > other.m_data[i])							\
					return 1;													\
			}																	\
			return 0;															\
		}																		\
		name&		operator = (const xcbuffer& other) {						\
			s64 i = 0;															\
			for (; i < size() && i < other.size(); ++i)							\
				m_data[i] = other.m_data[i];									\
			for (; i < size(); ++i)												\
				m_data[i] = 0;													\
			return *this;														\
		}																		\
		name&		operator = (const name& other) {							\
			for (s64 i=0; i < size(); ++i)										\
				m_data[i] = other.m_data[i];									\
			return *this;														\
		}																		\
		bool		operator == (const name& other) const {						\
			return compare(other) == 0;											\
		}																		\
		bool		operator != (const name& other) const {						\
			return compare(other) != 0;											\
		}																		\
		bool		operator > (const name& other) const {						\
			return compare(other) == 1;											\
		}																		\
		bool		operator >= (const name& other) const {						\
			return compare(other) >= 0;											\
		}																		\
		bool		operator < (const name& other) const {						\
			return compare(other) == -1;										\
		}																		\
		bool		operator <= (const name& other) const {						\
			return compare(other) <= 0;											\
		}																		\
		xbyte&		operator [] (s64 i) {										\
			ASSERT(i >= 0 && i < size());										\
			return m_data[i];													\
		}																		\
		xbyte		operator [] (s64 i) const {									\
			ASSERT(i >= 0 && i < size());										\
			return m_data[i];													\
		}																		\
		xbyte		m_data[len];												\
	}

	// Inline functions
	#include "xbase/private/x_buffer.h"

	XBYTES(xbytes8, 8);
	XBYTES(xbytes16, 16);
	XBYTES(xbytes20, 20);
	XBYTES(xbytes32, 32);
	XBYTES(xbytes64, 64);
	XBYTES(xbytes128, 128);
	XBYTES(xbytes256, 256);
	XBYTES(xbytes512, 512);
	XBYTES(xbytes1024, 1024);

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