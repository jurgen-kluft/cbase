#ifndef __XCORE_BUFFER_H__
#define __XCORE_BUFFER_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_debug.h"

namespace xcore
{

	class xbuffer
	{
	public:
		inline		xbuffer(u32 len, xbyte* data) : m_len(len), m_data(data) { reset(); }

		u32			size() const { return m_len; }
		void		reset();

		s32			compare(const xbuffer& other) const;

		xbuffer&	operator = (const xbuffer& other);
		bool		operator == (const xbuffer& other) const;
		bool		operator != (const xbuffer& other) const;

		xbyte		operator [] (int i) const;

		s32			m_len;
		xbyte*		m_data;
	};

	class xcbuffer
	{
	public:
		inline		xcbuffer(u32 len, xbyte const* data) : m_len(len), m_data(data) {}

		u32			size() const { return m_len; }

		s32			compare(const xcbuffer& other) const;

		bool		operator == (const xcbuffer& other) const;
		bool		operator != (const xcbuffer& other) const;

		xbyte		operator [] (int i) const;

		s32			m_len;
		xbyte const*m_data;
	};

	#define XBUFFER(name, len)													\
	class name																	\
	{																			\
	public:																		\
		inline		name() { reset(0); }										\
		inline		name(xbyte b) { reset(b); }									\
		inline		name(name const& other) { *this = other; }					\
		inline		name(xbuffer const& other) { *this = other; }				\
		u32			size() const { return len; }								\
		xbuffer		buffer() { return xbuffer(size(), m_data); }				\
		xcbuffer	cbuffer() const { return xcbuffer(size(), m_data); }		\
		void		reset(xbyte fill) {											\
			for (u32 i = 0; i < size(); ++i)									\
				m_data[i] = fill;												\
		}																		\
		s32			compare(const name& other) const {							\
			for (u32 i = 0; i < size(); ++i) {									\
				if (m_data[i] < other.m_data[i])								\
					return -1;													\
				else if (m_data[i] > other.m_data[i])							\
					return 1;													\
			}																	\
			return 0;															\
		}																		\
		name&		operator = (const xbuffer& other) {							\
			u32 i = 0;															\
			for (; i < size() && i<other.size(); ++i)							\
				m_data[i] = other.m_data[i];									\
			for (; i < size(); ++i)												\
				m_data[i] = 0;													\
			return *this;														\
		}																		\
		name&		operator = (const name& other) {							\
			for (u32 i = 0; i < size(); ++i)									\
				m_data[i] = other.m_data[i];									\
			return *this;														\
		}																		\
		bool		operator == (const name& other) const {						\
			for (u32 i = 0; i < size(); ++i) {									\
				if (m_data[i] != other.m_data[i])								\
					return false;												\
			}																	\
			return true;														\
		}																		\
		bool		operator != (const name& other) const {						\
			for (u32 i = 0; i < size(); ++i) {									\
				if (m_data[i] != other.m_data[i])								\
					return true;												\
			}																	\
			return false;														\
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
		xbyte		operator [] (int i) const {									\
			ASSERT(i >= 0 && i < (s32)size());									\
			return m_data[i];													\
		}																		\
		xbyte		m_data[len];												\
	}

	// Inline functions
	#include "xbase/private/x_buffer.h"

	XBUFFER(xbuffer16, 16);
	XBUFFER(xbuffer32, 32);
	XBUFFER(xbuffer64, 64);
	XBUFFER(xbuffer128, 128);
	XBUFFER(xbuffer256, 256);
	XBUFFER(xbuffer512, 512);
	XBUFFER(xbuffer1024, 1024);
}

#endif	///< __XCORE_BUFFER_H__