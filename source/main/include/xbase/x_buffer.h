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

	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	class xbuffer
	{
	public:
		static xbuffer	allocate(s64 size, x_iallocator* a);
		static void		reallocate(xbuffer& buf, s64 size, x_iallocator* a);
		static void		deallocate(xbuffer& buf, x_iallocator* a);

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

	private:
		inline		xbuffer() : m_len(0), m_data(NULL) { }

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
		u64			size() const { return len; }								\
		xbuffer		buffer() { return xbuffer(size(), m_data); }				\
		xcbuffer	cbuffer() const { return xcbuffer(size(), m_data); }		\
		void		reset(xbyte fill) {											\
			for (u64 i = 0; i < size(); ++i)									\
				m_data[i] = fill;												\
		}																		\
		void		clear() {													\
			reset(0);															\
		}																		\
		s32			compare(const name& other) const {							\
			for (u64 i = 0; i < size(); ++i) {									\
				if (m_data[i] < other.m_data[i])								\
					return -1;													\
				else if (m_data[i] > other.m_data[i])							\
					return 1;													\
			}																	\
			return 0;															\
		}																		\
		name&		operator = (const xcbuffer& other) {						\
			u64 i = 0;															\
			for (; i < size() && i < other.size(); ++i)							\
				m_data[i] = other.m_data[i];									\
			for (; i < size(); ++i)												\
				m_data[i] = 0;													\
			return *this;														\
		}																		\
		name&		operator = (const name& other) {							\
			for (u64 i=0; i < size(); ++i)										\
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
		xbyte&		operator [] (u64 i) {										\
			ASSERT(i >= 0 && i < size());										\
			return m_data[i];													\
		}																		\
		xbyte		operator [] (u64 i) const {									\
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

}

#endif	///< __XCORE_BUFFER_H__