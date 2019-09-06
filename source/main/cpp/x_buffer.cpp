#include "xbase/x_target.h"
#include "xbase/x_base.h"
#include "xbase/x_buffer.h"
#include "xbase/x_runes.h"

namespace xcore
{
	/// ---------------------------------------------------------------------------------------
	/// Unaligned Data Reading/Writing
	/// ---------------------------------------------------------------------------------------
	inline u8			read_u8(xbyte const* ptr) { return (u8)*ptr; }
	inline s8			read_s8(xbyte const* ptr) { return (s8)*ptr; }
	inline u16			read_u16(xbyte const* ptr) { u16 b0 = *ptr++; u16 b1 = *ptr++; return (u16)((b1 << 8) | b0); }
	inline s16			read_s16(xbyte const* ptr) { u16 b0 = *ptr++; u16 b1 = *ptr++; return (s16)((b1 << 8) | b0); }
	inline u32			read_u32(xbyte const* ptr) { u32 b0 = *ptr++; u32 b1 = *ptr++; u32 b2 = *ptr++; u32 b3 = *ptr++; return (u32)((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0)); }
	inline s32			read_s32(xbyte const* ptr) { u32 b0 = *ptr++; u32 b1 = *ptr++; u32 b2 = *ptr++; u32 b3 = *ptr++; return (s32)((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0)); }
	inline f32			read_f32(xbyte const* ptr) { u32 b0 = *ptr++; u32 b1 = *ptr++; u32 b2 = *ptr++; u32 b3 = *ptr++; u32 u = ((b3 << 24) | (b2 << 16) | (b1 << 8) | (b0)); return *((f32*)&u); }
	inline u64			read_u64(xbyte const* ptr) { u64 l0 = read_u32(ptr); u64 l1 = read_u32(ptr + 4); return (u64)((l1 << 32) | l0); }
	inline s64			read_s64(xbyte const* ptr) { u64 l0 = read_u32(ptr); u64 l1 = read_u32(ptr + 4); return (s64)((l1 << 32) | l0); }
	inline f64			read_f64(xbyte const* ptr) { u64 l0 = read_u32(ptr); u64 l1 = read_u32(ptr + 4); u64 ll = (u64)((l1 << 32) | l0); return *((f64*)&ll); }

	inline void			write_u8(xbyte * ptr, u8  b) { *ptr = b; }
	inline void			write_s8(xbyte * ptr, s8  b) { *ptr = b; }
	inline void			write_u16(xbyte * ptr, u16 b) { for (s32 i = 0; i < sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_s16(xbyte * ptr, s16 b) { for (s32 i = 0; i < sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_u32(xbyte * ptr, u32 b) { for (s32 i = 0; i < sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_s32(xbyte * ptr, s32 b) { for (s32 i = 0; i < sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_f32(xbyte * ptr, f32 f) { u32 b = *((u32*)&f); for (s32 i = 0; i < sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_u64(xbyte * ptr, u64 b) { for (s32 i = 0; i < sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_s64(xbyte * ptr, s64 b) { for (s32 i = 0; i < sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_f64(xbyte * ptr, f64 f) { u64 b = *((u64*)&f); for (s32 i = 0; i < sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }


	xcbuffer	xcbuffer::from_ascii_string(const char* str)
	{
		u32 len = ascii::len(str, nullptr);
		xcbuffer buffer(len, (xbyte const*)str);
		return buffer;
	}


	/// ---------------------------------------------------------------------------------------
	/// Binary Reader
	/// ---------------------------------------------------------------------------------------
	inline bool			_can_read(u32 _buffer_size, u32 _cursor, u32 _num_bytes)
	{
		return (_cursor + _num_bytes) <= _buffer_size;
	}

	u32					xbinary_reader::size() const
	{
		if (len_ == 0) return 0;
		return len_ - cursor_;
	}

	u32					xbinary_reader::len() const
	{
		return len_;
	}

	xcbuffer             xbinary_reader::get_current_buffer() const
	{
		return xcbuffer(size(), buffer_ + cursor_);
	}

	bool				xbinary_reader::can_read(u32 number_of_bytes) const
	{
		return _can_read(len_, cursor_, number_of_bytes);
	}

	void				xbinary_reader::reset()
	{
		cursor_ = 0;
	}

	u32				xbinary_reader::skip(s32 c)
	{
		if (_can_read(len_, cursor_, c))
		{
			cursor_ += c;
			return (u32)c;
		}
		return 0;
	}

	u32					xbinary_reader::read(bool& b)
	{
		if (_can_read(len_, cursor_, 1))
		{
			xbyte const* ptr = buffer_ + cursor_;
			cursor_ += 1;
			b = read_u8(ptr) != 0;
			return 1;
		}
		return 0;
	}

	u32					xbinary_reader::read(u8  & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			xbyte const* ptr = buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_u8(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(s8  & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			xbyte const* ptr = buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_s8(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(u16 & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			xbyte const* ptr = buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_u16(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(s16 & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			xbyte const* ptr = buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_s16(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(u32 & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			xbyte const* ptr = buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_u32(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(s32 & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			xbyte const* ptr = buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_s32(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(u64 & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			xbyte const* ptr = buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_u64(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(s64 & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_s64(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(f32 & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_f32(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(f64 & b)
	{
		if (_can_read(len_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_ + cursor_;
			cursor_ += sizeof(b);
			b = read_f64(ptr);
			return sizeof(b);
		}
		return 0;
	}

	bool				xbinary_reader::read_data(u32 size, xbuffer& buf)
	{
		if (_can_read(len_, cursor_, size))
		{
			xcbuffer to_read(size, buffer_ + cursor_);
			buf.writer().write_data(to_read);
			cursor_ += size;
			return true;
		}
		return false;
	}

	bool				xbinary_reader::view_data(u32 size, xcbuffer& buf)
	{
		if (!_can_read(len_, cursor_, size))
		{
			buf.m_const = NULL;
			buf.m_len = 0;
			return false;
		}
		buf.m_const = buffer_ + cursor_;
		buf.m_len = size;
		cursor_ += size;
		return true;
	}

	bool				xbinary_reader::view_string(u32 _size, const char*& out_str, const char*& out_end)
	{
		if (size() == 0) 
			return false;
		u32 strlen = 0;
		char const* str = (char const*)buffer_ + cursor_;
		while (cursor_ < len())
		{
			if (str[strlen] == '\0')
				break;
			++strlen;
		}
		if (str[strlen] != '\0')
		{
			return false;
		}
		out_str = str;
		out_end = str + strlen;
		cursor_ += strlen;
		return true;
	}

	/// ---------------------------------------------------------------------------------------
	/// Binary Writer
	/// ---------------------------------------------------------------------------------------
	inline bool			_can_write(u32 _buffer_size, u32 _cursor, u32 _num_bytes)
	{
		return (_cursor + _num_bytes) <= _buffer_size;
	}

	u32                 xbinary_writer::size() const
	{
		return len_ - cursor_;
	}

	u32                 xbinary_writer::len() const
	{
		return len_;
	}

	xbuffer				xbinary_writer::get_full_buffer() const
	{
		xbuffer buffer;
		buffer.m_mutable = buffer_;
		buffer.m_len = len_;
		return buffer;
	}

	xbuffer				xbinary_writer::get_current_buffer() const
	{
		xbuffer buffer;
		buffer.m_mutable = buffer_ + cursor_;
		buffer.m_len = size();
		return buffer;
	}

	bool				xbinary_writer::can_write(u32 num_bytes) const
	{
		return _can_write(len_, cursor_, num_bytes);
	}

	void				xbinary_writer::reset()
	{
		cursor_ = 0;
	}

	u32				xbinary_writer::skip(s32 c)
	{
		if (_can_write(len_, cursor_, c))
		{
			cursor_ += c;
			return (u32)c;
		}
		return 0;
	}

	u32					xbinary_writer::write(bool b)
	{
		if (_can_write(len_, cursor_, 1))
		{
			write_u8(buffer_ + cursor_, b ? 1 : 0);
			cursor_ += 1;
			return 1;
		}
		return 0;
	}

	u32					xbinary_writer::write(u8   b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_u8(buffer_ + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(s8   b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_u8(buffer_ + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(u16  b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_u16(buffer_ + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(s16  b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_s16(buffer_ + cursor_, b);
			cursor_ += sizeof(b);
		}
		return sizeof(b);
	}

	u32					xbinary_writer::write(u32  b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_u32(buffer_ + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(s32  b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_s32((xbyte*)buffer_ + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(u64  b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_u64((xbyte*)buffer_ + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(s64  b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_s64((xbyte*)buffer_ + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(f32  b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_f32((xbyte*)buffer_ + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(f64  b)
	{
		if (_can_write(len_, cursor_, sizeof(b)))
		{
			write_f64((xbyte*)buffer_ + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}


	u32					xbinary_writer::write_data(xbuffer const& buf)
	{
		if (_can_write(len_, cursor_, (u32)buf.size()))
		{
			xbyte* dst = (xbyte*)buffer_ + cursor_;
			xbyte const* src = buf.m_mutable;
			u32 const n = (u32)buf.size();
			for (u32 i = 0; i < n; i++)
				*dst++ = *src++;
			cursor_ += n;
			return n;
		}
		return 0;
	}

	u32					xbinary_writer::write_data(xcbuffer const& buf)
	{
		if (_can_write(len_, cursor_, (u32)buf.size()))
		{
			xbyte* dst = (xbyte*)buffer_ + cursor_;
			xbyte const* src = buf.m_const;
			u32 const n = (u32)buf.size();
			for (u32 i = 0; i < n; i++)
				*dst++ = *src++;
			cursor_ += n;
			return n;
		}
		return 0;
	}

	u32					xbinary_writer::write_string(char const* str, char const* end)
	{
		u32 const size = (u32)(end - str);
		if (_can_write(len_, cursor_, size))
		{
			char* dststr = (char*)buffer_ + cursor_;
			ascii::runes dst(dststr, dststr, dststr + size);
			ascii::crunes src(str, end);
			utf::copy(src, dst);
			cursor_ += size;
			return size;
		}
		return 0;
	}

	xbinary_writer		xbinary_writer::reserve(u32 length)
	{
		xbinary_writer writer;
		if (_can_write(len_, cursor_, length))
		{
			writer.len_    = length;
			writer.cursor_ = cursor_;
			writer.buffer_ = buffer_;
			return writer;
		}
		return writer;
	}

}


