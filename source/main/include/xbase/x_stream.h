#ifndef __CBASE_STREAM_H__
#define __CBASE_STREAM_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
	class istream_t
	{
	public:
		inline bool canSeek() const { return vcanSeek(); }
		inline bool canRead() const { return vcanRead(); }
		inline bool canWrite() const { return vcanWrite(); }
		inline bool canZeroCopy() const { return vcanZeroCopy(); }	// Does this stream support zero-copy?

		inline u64  getLength() const { return vgetLength(); }
		inline void setLength(u64 length) { vsetLength(length); }
		inline s64  getPos() const { return vgetPos(); }
		inline s64  setPos(s64 pos) { return vsetPos(pos); }

		inline void close() { vclose(); }
		inline void flush() { vflush(); }

		inline s64 read(u8* data, s64 len) { return vread(data, len); }
		inline s64 read0(u8 const*& data, s64 len) { return vread0(data, len); }
		inline s64 write(u8 const* data, s64 len) { return vwrite(data, len); }

	protected:
		virtual bool vcanSeek() const = 0;
		virtual bool vcanRead() const = 0;
		virtual bool vcanWrite() const = 0;
		virtual bool vcanZeroCopy() const = 0;
		virtual void vflush() = 0;
		virtual void vclose() = 0;
		virtual u64  vgetLength() const = 0;
		virtual void vsetLength(u64 length) = 0;
		virtual s64  vsetPos(s64 pos) = 0;
		virtual s64  vgetPos() const = 0;
		virtual s64 vread(u8* buffer, s64 count) = 0;
		virtual s64 vread0(u8 const*& buffer, s64 count) = 0;
		virtual s64 vwrite(const u8* buffer, s64 count) = 0;
	};

} // namespace ncore

#endif // __CBASE_STREAM_H__