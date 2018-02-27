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

}

#endif	///< __XCORE_BUFFER_H__