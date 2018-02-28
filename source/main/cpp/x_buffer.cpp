#include "xbase/x_target.h"
#include "xbase/x_base.h"
#include "xbase/x_buffer.h"
#include "xbase/x_chars.h"

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
	inline void			write_u16(xbyte * ptr, u16 b) { for (s32 i = 0; i<sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_s16(xbyte * ptr, s16 b) { for (s32 i = 0; i<sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_u32(xbyte * ptr, u32 b) { for (s32 i = 0; i<sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_s32(xbyte * ptr, s32 b) { for (s32 i = 0; i<sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_f32(xbyte * ptr, f32 f) { u32 b = *((u32*)&f); for (s32 i = 0; i<sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_u64(xbyte * ptr, u64 b) { for (s32 i = 0; i<sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_s64(xbyte * ptr, s64 b) { for (s32 i = 0; i<sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }
	inline void			write_f64(xbyte * ptr, f64 f) { u64 b = *((u64*)&f); for (s32 i = 0; i<sizeof(b); ++i) { ptr[i] = (u8)b; b = b >> 8; } }


	/// ---------------------------------------------------------------------------------------
	/// Message Reader
	/// ---------------------------------------------------------------------------------------
	inline bool			_can_read(xcbuffer const& _buffer, u32 _cursor, u32 _num_bytes)
	{
		return (_cursor + _num_bytes) <= _buffer.size();
	}

	u32					xbinary_reader::get_size() const
	{
		if (buffer_.size() == 0) return 0;
		return (u32)buffer_.size() - cursor_;
	}

	bool				xbinary_reader::can_read(u32 number_of_bytes) const
	{
		return _can_read(buffer_, cursor_, number_of_bytes);
	}

	void				xbinary_reader::reset()
	{
		cursor_ = 0;
	}

	void				xbinary_reader::skip(s32 c)
	{
		if (_can_read(buffer_, cursor_, c))
		{
			cursor_ += c;
		}
	}

	u32					xbinary_reader::read(bool& b)
	{
		if (_can_read(buffer_, cursor_, 1))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += 1;
			b = read_u8(ptr) != 0;
			return 1;
		}
		return 0;
	}

	u32					xbinary_reader::read(u8  & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_u8(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(s8  & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_s8(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(u16 & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_u16(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(s16 & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_s16(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(u32 & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_u32(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(s32 & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_s32(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(u64 & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_u64(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(s64 & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_s64(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(f32 & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_f32(ptr);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_reader::read(f64 & b)
	{
		if (_can_read(buffer_, cursor_, sizeof(b)))
		{
			u8 const* ptr = (u8 const*)buffer_.m_data + cursor_;
			cursor_ += sizeof(b);
			b = read_f64(ptr);
			return sizeof(b);
		}
		return 0;
	}

	bool				xbinary_reader::read_data(xbuffer& buf)
	{
		if (!_can_read(buffer_, cursor_, (u32)buf.size()))
		{
			return false;
		}
		xcbuffer src(buf.size(), (xbyte const*)buffer_.m_data + cursor_);
		buf.write(src);
		cursor_ += (u32)buf.size();
		return true;
	}

	bool				xbinary_reader::view_data(u32 size, xcbuffer& buf)
	{
		if (!_can_read(buffer_, cursor_, size))
		{
			buf.m_data = NULL;
			buf.m_len = 0;
			return false;
		}

		buf.m_data = (xbyte const*)buffer_.m_data + cursor_;
		buf.m_len = size;

		cursor_ += size;
		return true;
	}

	bool				xbinary_reader::view_string(xcuchars& outstr)
	{
		if (buffer_.size() == 0) return false;
		return false;

		u32 strlen = 0;
		char const* str = (char const*)buffer_.m_data + cursor_;
		while (cursor_ < buffer_.size())
		{
			if (str[strlen] == '\0')
				break;
			++strlen;
		}

		if (str[strlen] != '\0')
		{
			return false;
		}

		outstr = xcuchars(str, str + strlen);
		cursor_ += strlen;
		return true;
	}

	/// ---------------------------------------------------------------------------------------
	/// Message Writer
	/// ---------------------------------------------------------------------------------------
	inline bool			_can_write(xbuffer const& _buffer, u32 _cursor, u32 _num_bytes)
	{
		return (_cursor + _num_bytes) <= _buffer.size();
	}

	xbuffer				xbinary_writer::get_current_buffer() const
	{
		xbuffer buffer;
		buffer = buffer_;
		buffer.m_len = cursor_;
		return buffer;
	}

	bool				xbinary_writer::can_write(u32 num_bytes) const
	{
		return _can_write(buffer_, cursor_, num_bytes);
	}

	void				xbinary_writer::reset()
	{
		cursor_ = 0;
	}

	void				xbinary_writer::skip(s32 c)
	{
		if (_can_write(buffer_, cursor_, c))
		{
			cursor_ += c;
		}
	}

	u32					xbinary_writer::write(bool b)
	{
		if (_can_write(buffer_, cursor_, 1))
		{
			write_u8((xbyte*)buffer_.m_data + cursor_, b ? 1 : 0);
			cursor_ += 1;
			return 1;
		}
		return 0;
	}

	u32					xbinary_writer::write(u8   b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_u8((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(s8   b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_u8((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(u16  b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_u16((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(s16  b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_s16((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
		}
		return sizeof(b);
	}

	u32					xbinary_writer::write(u32  b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_u32((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(s32  b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_s32((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(u64  b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_u64((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(s64  b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_s64((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(f32  b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_f32((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}

	u32					xbinary_writer::write(f64  b)
	{
		if (_can_write(buffer_, cursor_, sizeof(b)))
		{
			write_f64((xbyte*)buffer_.m_data + cursor_, b);
			cursor_ += sizeof(b);
			return sizeof(b);
		}
		return 0;
	}


	u32					xbinary_writer::write_data(xbuffer const& buf)
	{
		if (_can_write(buffer_, cursor_, (u32)buf.size()))
		{
			xbyte* dst = (xbyte*)buffer_.m_data + cursor_;
			for (u32 i = 0; i<buf.size(); i++)
				dst[i] = buf[i];
			cursor_ += (u32)buf.size();
			return (u32)buf.size();
		}
		return 0;
	}

	u32					xbinary_writer::write_data(xcbuffer const& buf)
	{
		if (_can_write(buffer_, cursor_, (u32)buf.size()))
		{
			xbyte* dst = (xbyte*)buffer_.m_data + cursor_;
			for (u32 i = 0; i<buf.size(); i++)
				dst[i] = buf[i];
			cursor_ += (u32)buf.size();
			return (u32)buf.size();
		}
		return 0;
	}

	u32					xbinary_writer::write_string(xcuchars const& str)
	{
		if (_can_write(buffer_, cursor_, str.size()))
		{
			char* dststr = (char*)buffer_.m_data + cursor_;
			xuchars dst(dststr, dststr, (char*)buffer_.m_data + buffer_.m_len - 1);
			dst.copy(str);
			cursor_ += str.size();
			return str.size();
		}
		return 0;
	}

	xbinary_writer		xbinary_writer::operator () (s32 length) const
	{
		xbinary_writer writer;
		if (_can_write(buffer_, cursor_, length))
		{
			writer.cursor_ = cursor_;
			writer.buffer_ = buffer_;
			writer.buffer_.m_len = length;
			return writer;
		}
		return writer;
	}

}


