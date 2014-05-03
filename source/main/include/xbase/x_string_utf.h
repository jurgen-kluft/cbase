#ifndef __XBASE_STRING_UTF_H__
#define __XBASE_STRING_UTF_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_integer.h"

/**
* xCore namespace
*/
namespace xcore
{
	namespace utf
	{
		s32  		strlen		(ustr8 const* _str, ustr8 const** _end = NULL);
		s32  		strlen		(ustr16 const* _str, ustr16 const** _end = NULL);

		u32			numBytes	(uchar8 _c);
		u32			numBytes	(uchar16 _c);

		s32			read		(ustr8  const* _str, uchar8 & _out);
		s32			read		(ustr16 const* _str, uchar16& _out);

		s32			read		(ustr8  const* _str, ustr8  const* _src_end, uchar8 & _out);
		s32			read		(ustr16 const* _str, ustr16 const* _src_end, uchar16& _out);

		s32			write		(ustr8  * _str, uchar8  _char);
		s32			write		(ustr16 * _str, uchar16 _char);

		s32			write		(ustr8  * _str, uchar8  _char, ustr8  const* _src_end);
		s32			write		(ustr16 * _str, uchar16 _char, ustr16 const* _src_end);

		bool		isLegal		(uchar8  _c);
		bool		isLegal		(uchar16 _c);
		bool		isLegal		(const ustr8 * _src, const ustr8 * _src_end = NULL);
		bool		isLegal		(const ustr16* _src, const ustr16* _src_end = NULL);

		bool		convert		(uchar16 _from, uchar8  & _to);
		bool		convert		(uchar8  _from, uchar16 & _to);

		s32			convert		(ustr8  const* _src, ustr8  const* _src_end, ustr16*& _dst, ustr16 const* _dst_end);
		s32			convert		(ustr16 const* _src, ustr16 const* _src_end, ustr8 *& _dst, ustr8  const* _dst_end);

		namespace internal
		{
			inline bool		is_trail	(u8 c)	{ return ((c>>6)==0x2); }
		}
	}	// utf  
};	// xCore namespace



namespace xcore
{
	namespace utf
	{
		// This represents the byte position in a UTF-8 string, this object is to make the API
		// clear for the user to indicate which position is a byte position and which a character position.
		struct ubpos8
		{
			inline			ubpos8() : bpos_(0xffffffff)					{ }
			inline			ubpos8(const ubpos8& _p) : bpos_(_p.bpos_)		{ }
			inline explicit	ubpos8(u32 _bpos) : bpos_(_bpos)				{ }

			inline bool		is_empty() const								{ return bpos_ == 0xffffffff; }

			inline u32		bpos() const									{ return bpos_; }

							operator u32() const							{ return bpos_; }

			ubpos8&			operator =  (ubpos8 const& _p)					{ bpos_ = _p.bpos_; return *this; }

			ubpos8&			operator ++ ()									{ bpos_++; return *this; }
			ubpos8			operator ++ (s32)								{ ubpos8 i(*this); bpos_++; return i; }

			bool			operator == (const ubpos8& p) const				{ return (bpos_==p.bpos_); }
			bool			operator != (const ubpos8& p) const				{ return (bpos_!=p.bpos_); }
			bool			operator <  (const ubpos8& p) const				{ return (bpos_ <p.bpos_); }
			bool			operator <= (const ubpos8& p) const				{ return (bpos_<=p.bpos_); }
			bool			operator >  (const ubpos8& p) const				{ return (bpos_ >p.bpos_); }
			bool			operator >= (const ubpos8& p) const				{ return (bpos_>=p.bpos_); }

			ubpos8			operator +  (u32 i) const						{ ubpos8 p(*this); p.bpos_+=i; return p; }
			ubpos8			operator -  (u32 i) const						{ ubpos8 p(*this); p.bpos_-=i; return p; }
			ubpos8&			operator += (u32 i)								{ bpos_+=i; return *this; }
			ubpos8&			operator -= (u32 i)								{ bpos_-=i; return *this; }

			ubpos8			operator +  (uchar8 c) const					{ ubpos8 p(*this); const u32 n = numBytes(c); p.bpos_+=n; return p; }
			ubpos8			operator -  (uchar8 c) const					{ ubpos8 p(*this); const u32 n = numBytes(c); p.bpos_-=n; return p; }
			ubpos8&			operator += (uchar8 c)							{ const u32 n = numBytes(c); bpos_+=n; return *this; }
			ubpos8&			operator -= (uchar8 c)							{ const u32 n = numBytes(c); bpos_-=n; return *this; }

		private:
			u32				bpos_;
		};

		// This represents the character position in a UTF-8 string, this object is to make the API
		// clear for the user to indicate a position to be a character position.
		struct ucpos8
		{
			inline			ucpos8() : cpos_(0xffffffff)					{ }
			inline			ucpos8(const ucpos8& _p) : cpos_(_p.cpos_)		{ }
			inline explicit	ucpos8(u32 _cpos) : cpos_(_cpos)				{ }

			inline bool		is_empty() const								{ return cpos_ == 0xffffffff; }

			inline u32		cpos() const									{ return cpos_; }

							operator u32() const							{ return cpos_; }

			ucpos8&			operator =  (ucpos8 const& _p)					{ cpos_ = _p.cpos_; return *this; }

			ucpos8&			operator ++ ()									{ cpos_++; return *this; }
			ucpos8			operator ++ (s32)								{ ucpos8 i(*this); cpos_++; return i; }

			bool			operator == (const ucpos8& p) const				{ return (cpos_==p.cpos_); }
			bool			operator != (const ucpos8& p) const				{ return (cpos_!=p.cpos_); }
			bool			operator <  (const ucpos8& p) const				{ return (cpos_ <p.cpos_); }
			bool			operator <= (const ucpos8& p) const				{ return (cpos_<=p.cpos_); }
			bool			operator >  (const ucpos8& p) const				{ return (cpos_ >p.cpos_); }
			bool			operator >= (const ucpos8& p) const				{ return (cpos_>=p.cpos_); }

			ucpos8			operator +  (u32 i) const						{ ucpos8 p(*this); p.cpos_+=i; return p; }
			ucpos8			operator -  (u32 i) const						{ ucpos8 p(*this); p.cpos_-=i; return p; }
			ucpos8&			operator += (u32 i)								{ cpos_+=i; return *this; }
			ucpos8&			operator -= (u32 i)								{ cpos_-=i; return *this; }

			ucpos8			operator +  (uchar8 c) const					{ ucpos8 p(*this); const u32 n = numBytes(c); p.cpos_+=n; return p; }
			ucpos8			operator -  (uchar8 c) const					{ ucpos8 p(*this); const u32 n = numBytes(c); p.cpos_-=n; return p; }
			ucpos8&			operator += (uchar8 c)							{ const u32 n = numBytes(c); cpos_+=n; return *this; }
			ucpos8&			operator -= (uchar8 c)							{ const u32 n = numBytes(c); cpos_-=n; return *this; }

		private:
			u32				cpos_;
		};

		// This represents a position in a UTF-8 string where the byte position
		// and character position are different.
		struct upos8
		{
			inline			upos8() : cpos_(0xffffffff), bpos_(0xffffffff) { }
			inline			upos8(const upos8& _p) : cpos_(_p.cpos_), bpos_(_p.bpos_) { }
			inline			upos8(ucpos8 _cpos, ubpos8 _bpos) : cpos_(_cpos), bpos_(_bpos) { }

			inline bool		is_empty() const								{ return cpos_ == bpos_ && cpos_ == 0xffffffff; }
			inline bool		is_valid() const								{ return cpos_ <= bpos_; }

			inline ucpos8	cpos() const									{ return ucpos8(cpos_); }
			inline ubpos8	bpos() const									{ return ubpos8(bpos_); }

			static upos8	begin()											{ return upos8(ucpos8(0), ubpos8(0)); }
			static upos8	at(ustr8 const* _str, ucpos8 _cpos)
			{
				ubpos8 bpos(0);
				ucpos8 cpos(0);
				while (cpos!=_cpos)
				{
					const u32 n = numBytes(uchar8(_str->c));
					_str += n;
					bpos += n;
					cpos += 1;
				}
				return upos8(cpos, bpos);
			}

			upos8&			operator  = (upos8 const& _p)					{ cpos_ = _p.cpos_; bpos_ = _p.bpos_; return *this; }

			bool			operator == (const upos8& _pos) const			{ return (cpos_)==(_pos.cpos_) && (bpos_==_pos.bpos_); }
			bool			operator != (const upos8& _pos) const			{ return (cpos_)!=(_pos.cpos_) && (bpos_!=_pos.bpos_); }
			bool			operator <  (const upos8& _pos) const			{ return (cpos_) <(_pos.cpos_) && (bpos_ <_pos.bpos_); }
			bool			operator <= (const upos8& _pos) const			{ return (cpos_)<=(_pos.cpos_) && (bpos_<=_pos.bpos_); }
			bool			operator >  (const upos8& _pos) const			{ return (cpos_) >(_pos.cpos_) && (bpos_ >_pos.bpos_); }
			bool			operator >= (const upos8& _pos) const			{ return (cpos_)>=(_pos.cpos_) && (bpos_>=_pos.bpos_); }

			upos8			operator +  (uchar8 c) const					{ upos8 p(*this); const u32 n = numBytes(c); p.bpos_+=n; ++p.cpos_; return p; }
			upos8			operator -  (uchar8 c) const					{ upos8 p(*this); const u32 n = numBytes(c); p.bpos_-=n; --p.cpos_; return p; }
			upos8&			operator += (uchar8 c)							{ const u32 n = numBytes(c); bpos_+=n; ++cpos_; return *this; }
			upos8&			operator -= (uchar8 c)							{ const u32 n = numBytes(c); bpos_-=n; --cpos_; return *this; }

			upos8			operator +  (upos8 _p) const					{ upos8 p(*this); p.cpos_+=_p.cpos_; p.bpos_+=_p.bpos_; return p; }
			upos8			operator -  (upos8 _p) const					{ upos8 p(*this); p.cpos_-=_p.cpos_; p.bpos_+=_p.bpos_; return p; }
			upos8&			operator += (upos8 _p)							{ cpos_+=_p.cpos_; bpos_+=_p.bpos_; return *this; }
			upos8&			operator -= (upos8 _p)							{ cpos_-=_p.cpos_; bpos_-=_p.bpos_; return *this; }

		private:
			u32				cpos_;	// character position
			u32				bpos_;	// byte position
		};


		// This represents a length/distance in a UTF-8 string where the byte position
		// and character position are different.
		struct ulen8
		{
			inline			ulen8() : clen_(0xffffffff), blen_(0xffffffff) { }
			inline			ulen8(const ulen8& _p) : clen_(_p.clen_), blen_(_p.blen_) { }
			inline			ulen8(ucpos8 _cend, ubpos8 _bend) : clen_(_cend-ucpos8(0)), blen_(_bend-ubpos8(0)) { }
			inline			ulen8(ucpos8 _cbegin, ubpos8 _bbegin, ucpos8 _cend, ubpos8 _bend) : clen_(_cend-_cbegin), blen_(_bend-_bbegin) { }
			inline			ulen8(upos8 _end) : clen_(_end.cpos() - ucpos8(0)), blen_(_end.bpos() - ubpos8(0)) { }
			inline			ulen8(upos8 _begin, upos8 _end) : clen_(_end.cpos() - _begin.cpos()), blen_(_end.bpos() - _begin.bpos()) { }

			inline bool		is_empty() const								{ return clen_ == blen_ && clen_ == 0xffffffff; }
			inline bool		is_valid() const								{ return clen_ <= blen_; }

			inline ucpos8	clen() const									{ return ucpos8(clen_); }
			inline ubpos8	blen() const									{ return ubpos8(blen_); }

			static ulen8	strlen(const ustr8* _str)
			{
				const ustr8* end = 0;
				s32 const len = utf::strlen(_str, &end);
				return ulen8(ucpos8(len), ubpos8((u32)(end-_str)));
			}

			static ulen8	at(ustr8 const* _str, ucpos8 _cpos)
			{
				ubpos8 bpos(0);
				ucpos8 cpos(0);
				while (cpos!=_cpos)
				{
					const u32 n = numBytes(uchar8(_str->c));
					_str += n;
					bpos += n;
					cpos += 1;
				}
				upos8 end(cpos,bpos);
				return ulen8(end);
			}

			ulen8&			operator = (ulen8 const& _p)					{ clen_ = _p.clen_; blen_ = _p.blen_; return *this; }

			bool			operator == (const ulen8& _pos) const			{ return (clen_)==(_pos.clen_) && (blen_==_pos.blen_); }
			bool			operator != (const ulen8& _pos) const			{ return (clen_)!=(_pos.clen_) && (blen_!=_pos.blen_); }
			bool			operator <  (const ulen8& _pos) const			{ return (clen_) <(_pos.clen_) && (blen_ <_pos.blen_); }
			bool			operator <= (const ulen8& _pos) const			{ return (clen_)<=(_pos.clen_) && (blen_<=_pos.blen_); }
			bool			operator >  (const ulen8& _pos) const			{ return (clen_) >(_pos.clen_) && (blen_ >_pos.blen_); }
			bool			operator >= (const ulen8& _pos) const			{ return (clen_)>=(_pos.clen_) && (blen_>=_pos.blen_); }

			ulen8			operator +  (uchar8 c) const					{ ulen8 p(*this); const u32 n = numBytes(c); p.blen_+=n; ++p.clen_; return p; }
			ulen8			operator -  (uchar8 c) const					{ ulen8 p(*this); const u32 n = numBytes(c); p.blen_-=n; --p.clen_; return p; }
			ulen8&			operator += (uchar8 c)							{ const u32 n = numBytes(c); blen_+=n; ++clen_; return *this; }
			ulen8&			operator -= (uchar8 c)							{ const u32 n = numBytes(c); blen_-=n; --clen_; return *this; }

			ulen8			operator +  (ulen8 _p) const					{ ulen8 p(*this); p.clen_+=_p.clen_; p.blen_+=_p.blen_; return p; }
			ulen8			operator -  (ulen8 _p) const					{ ulen8 p(*this); p.clen_-=_p.clen_; p.blen_+=_p.blen_; return p; }
			ulen8&			operator += (ulen8 _p)							{ clen_+=_p.clen_; blen_+=_p.blen_; return *this; }
			ulen8&			operator -= (ulen8 _p)							{ clen_-=_p.clen_; blen_-=_p.blen_; return *this; }

		private:
			u32				clen_;	// character position
			u32				blen_;	// byte position
		};

		// Forward declare
		class ucptr8;

		class uptr8
		{
			struct xuptr8
			{
								xuptr8(ustr8* _str) : str_(_str)			{ }

				xuptr8&			operator = (xuptr8 const& _ptr)				{ uchar8 ch; utf::read(_ptr.str_, ch); utf::write(str_, ch); return *this; }
				xuptr8&			operator = (char _ch)						{ utf::write(str_, uchar8(_ch)); return *this; }
				xuptr8&			operator = (uchar8 _ch)						{ utf::write(str_, _ch); return *this; }
								operator uchar8 ()							{ uchar8 ch; utf::read(str_, ch); return ch; }

				ustr8*			str_;
			};

		public:
			inline			uptr8(ustr8* _str) : str_(_str) { }
			inline			uptr8(const uptr8& _ptr) : str_(_ptr.str_) { }

			ulen8			strlen() const
			{
				ulen8 len = ulen8::strlen(str_);
				return len;
			}

			ulen8			copy_char_to(uchar8& c) const
			{
				s32 const n = utf::read(str_, c); 
				return ulen8(ucpos8(1), ubpos8(n));
			}

			ulen8			copy_char_to(uptr8& _ptr) const
			{
				uchar8 ch;
				s32 const n = utf::read(str_, ch);
				utf::write(_ptr.str_, ch);
				return ulen8(ucpos8(1), ubpos8(n));
			}

			ulen8			copy_char_from(uchar8 c)
			{
				s32 const n = utf::write(str_, c); 
				return ulen8(ucpos8(1), ubpos8(n));
			}

			ulen8			copy_char_from(uptr8 const& _ptr)
			{
				uchar8 ch;
				s32 const n = utf::read(_ptr.str_, ch);
				utf::write(str_, ch);
				return ulen8(ucpos8(1), ubpos8(n));
			}

			ulen8			copy_char_from(ucptr8 const& _ptr);

			bool			at_end() const								{ return str_->c == 0; }

			xuptr8			operator* ()								{ return xuptr8(str_); }

			uptr8&			operator++ ()								{ u32 const n=numBytes(uchar8(str_->c)); str_+=n; return *this; }
			uptr8			operator++ (s32)							{ uptr8 i(str_); u32 const n=numBytes(uchar8(str_->c)); str_+=n; return i; }

			uptr8&			operator = (uchar8 _c)						{ utf::write(str_, _c); return *this; }
			uptr8&			operator = (ustr8* _str)					{ str_ = _str; return *this; }
			uptr8&			operator = (uptr8 const& _p)				{ str_ = _p.str_; return *this; }

			uptr8			operator + (uchar8 c) const					{ return uptr8(str_ + numBytes(c)); }
			uptr8			operator - (uchar8 c) const					{ return uptr8(str_ - numBytes(c)); }
			uptr8&			operator +=(uchar8 c)						{ str_ += numBytes(c); return *this; }
			uptr8&			operator -=(uchar8 c)						{ str_ -= numBytes(c); return *this; }

			uptr8			operator + (ulen8 n) const					{ uptr8 p(str_ + n.blen()); return p; }
			uptr8			operator - (ulen8 n) const					{ uptr8 p(str_ - n.blen()); return p; }
			uptr8&			operator -=(ulen8 n)						{ str_ -= n.blen(); return *this; }
			uptr8&			operator +=(ulen8 n)						{ str_ += n.blen(); return *this; }

			uptr8			operator + (u32 n) const					{ uptr8 p(str_); while (n>0) { p.str_+=utf::numBytes(uchar8(p.str_->c)); --n; } return p; }
			uptr8			operator - (u32 n) const					{ uptr8 p(str_); while (n>0) { --p.str_; while (internal::is_trail(p.str_->c)) --p.str_; --n; } return p; }
			uptr8&			operator +=(u32 n)							{ while (n>0) { str_+=utf::numBytes(uchar8(str_->c)); --n; } return *this; }
			uptr8&			operator -=(u32 n)							{ while (n>0) { --str_; while (internal::is_trail(str_->c)) --str_; --n; } }

			bool			operator == (const uptr8& _iter) const		{ return (str_) == (_iter.str_); }
			bool			operator != (const uptr8& _iter) const		{ return (str_) != (_iter.str_); }
			bool			operator <  (const uptr8& _iter) const		{ return (str_) < (_iter.str_); }
			bool			operator <= (const uptr8& _iter) const		{ return (str_) <= (_iter.str_); }
			bool			operator >  (const uptr8& _iter) const		{ return (str_) > (_iter.str_); }
			bool			operator >= (const uptr8& _iter) const		{ return (str_) >= (_iter.str_); }

			ustr8*			str() const									{ return str_; }

		private:
			ustr8*			str_;
		};



		class ucptr8
		{
		public:
			inline			ucptr8(ustr8 const* _str) : str_(_str) { }
			inline			ucptr8(const ucptr8& _ptr) : str_(_ptr.str_) { }

			ulen8			strlen() const
			{
				ulen8 len = ulen8::strlen(str_);
				return len;
			}

			ulen8			copy_char_to(uchar8& c) const
			{
				s32 const n = utf::read(str_, c); 
				return ulen8(ucpos8(1), ubpos8(n));
			}

			ulen8			copy_char_to(ustr8* _ptr) const
			{
				uchar8 ch;
				s32 const n = utf::read(str_, ch);
				utf::write(_ptr, ch);
				return ulen8(ucpos8(1), ubpos8(n));
			}

			bool			at_end() const								{ return str_->c == 0; }

			uchar8			operator* ()								{ uchar8 ch; read(str_, ch); return ch; }

			ucptr8&			operator++ ()								{ u32 const n=numBytes(uchar8(str_->c)); str_+=n; return *this; }
			ucptr8			operator++ (s32)							{ ucptr8 i(str_); u32 const n=numBytes(uchar8(str_->c)); str_+=n; return i; }

			ucptr8&			operator = (ustr8 const* _str)				{ str_ = _str; return *this; }
			ucptr8&			operator = (ucptr8 const& _p)				{ str_ = _p.str_; return *this; }

			ucptr8			operator + (uchar8 c) const					{ return ucptr8(str_ + numBytes(c)); }
			ucptr8			operator - (uchar8 c) const					{ return ucptr8(str_ - numBytes(c)); }
			ucptr8&			operator +=(uchar8 c)						{ str_ += numBytes(c); return *this; }
			ucptr8&			operator -=(uchar8 c)						{ str_ -= numBytes(c); return *this; }

			ucptr8			operator + (ulen8 n) const					{ ucptr8 p(str_ + n.blen()); return p; }
			ucptr8			operator - (ulen8 n) const					{ ucptr8 p(str_ - n.blen()); return p; }
			ucptr8&			operator -=(ulen8 n)						{ str_ -= n.blen(); return *this; }
			ucptr8&			operator +=(ulen8 n)						{ str_ += n.blen(); return *this; }

			ucptr8			operator + (u32 n) const					{ ucptr8 p(str_); while (n>0) { p.str_+=utf::numBytes(uchar8(p.str_->c)); --n; } return p; }
			ucptr8			operator - (u32 n) const					{ ucptr8 p(str_); while (n>0) { --p.str_; while (internal::is_trail(p.str_->c)) --p.str_; --n; } return p; }
			ucptr8&			operator +=(u32 n)							{ while (n>0) { str_+=utf::numBytes(uchar8(str_->c)); --n; } return *this; }
			ucptr8&			operator -=(u32 n)							{ while (n>0) { --str_; while (internal::is_trail(str_->c)) --str_; --n; } }

			bool			operator == (const uptr8& _p) const			{ return (str_) == (_p.str()); }
			bool			operator != (const uptr8& _p) const			{ return (str_) != (_p.str()); }
			bool			operator <  (const uptr8& _p) const			{ return (str_)  < (_p.str()); }
			bool			operator <= (const uptr8& _p) const			{ return (str_) <= (_p.str()); }
			bool			operator >  (const uptr8& _p) const			{ return (str_)  > (_p.str()); }
			bool			operator >= (const uptr8& _p) const			{ return (str_) >= (_p.str()); }

			bool			operator == (const ucptr8& _p) const		{ return (str_) == (_p.str_); }
			bool			operator != (const ucptr8& _p) const		{ return (str_) != (_p.str_); }
			bool			operator <  (const ucptr8& _p) const		{ return (str_) <  (_p.str_); }
			bool			operator <= (const ucptr8& _p) const		{ return (str_) <= (_p.str_); }
			bool			operator >  (const ucptr8& _p) const		{ return (str_) >  (_p.str_); }
			bool			operator >= (const ucptr8& _p) const		{ return (str_) >= (_p.str_); }

			ustr8 const*	str() const									{ return str_; }

		private:
			ustr8 const*	str_;
		};

		inline ulen8		uptr8::copy_char_from(ucptr8 const& _ptr)
		{
			uchar8 ch;
			s32 const n = utf::read(_ptr.str(), ch);
			utf::write(str_, ch);
			return ulen8(ucpos8(1), ubpos8(n));
		}


		inline s32  		strlen    			(ustr8 const* _str, ustr8 const** _end)	// We assume the string is a valid UTF8 string!
		{
			s32 len = 0;
			ustr8 const* e = (_end != NULL) ? *_end : NULL;
			ustr8 const* s = _str;
			if (e != NULL)
			{
				while (s < e)
				{
					if ((s->c & 0xc0) != 0x80) len++;
					s++;
				}
			}
			else
			{
				while (s->c != 0)
				{
					if ((s->c & 0xc0) != 0x80) len++;
					s++;
				}
				if (_end != NULL)
					*_end = s;
			}
			return len;
		}

		inline s32  		strlen    			(ustr16 const* _str, ustr16 const** _end)
		{
			s32 len = 0;
			ustr16 const* e = (_end != NULL) ? *_end : NULL;
			if (e != NULL)
			{
				ustr16 const* s = _str;
				while (s < e)
				{
					++len;
					u16 ch = s->c;
					++s;
					if (ch >= 0xD800 && ch <= 0xDBFF)
					{
						if (s >= e)
							return -1;	// Invalid string range

						ch = s->c;
						++s;
						if (ch < 0xDC00 || ch > 0xDFFF)
							return -1;	// Invalid UTF-16
					}
				}
			}
			else
			{
				ustr16 const* s = _str;
				while (s->c != 0)
				{
					++len;
					u16 ch = s->c;
					++s;
					if (ch >= 0xD800 && ch <= 0xDBFF)
					{
						if (s->c == 0)
							return -1;	// Invalid string range

						ch = s->c;
						++s;
						if (ch < 0xDC00 || ch > 0xDFFF)
							return -1;	// Invalid UTF-16
					}
				}
			}
			return len;
		}

		inline u32			numBytes			(uchar8 _c)		// We assume the character is a valid UTF8 character!
		{
			u32 n;
			u8 c = _c.c & 0xFF;
			if (_c.c < 0x80)	// ASCII
			{
				n=1;
			}
			else
			{
				switch (0xF0 & _c.c)
				{
				case 0xE0: n=3; break;
				case 0xF0: n=4; break;
				default:   n=2; break;
				}
			}
			return n;
		}

		inline u32			numBytes			(uchar16 _c)
		{
			u16 const ch = _c.c & 0xFF;
			if (ch >= 0xD800 && ch <= 0xDBFF)
				return 4;
			return 2;
		}

		inline s32			read		(ustr8  const* _str, uchar8 & _out)
		{
			uchar8 c((u32)_str->c);
			const s32 n = numBytes(c);
			switch (n)
			{
			case 4: c.c=c.c|((u32)_str[3].c << 24);
			case 3: c.c=c.c|((u32)_str[2].c << 16);
			case 2: c.c=c.c|((u32)_str[1].c <<  8);
			}
			_out = c;
			return n;
		}

		inline s32			read		(ustr8  const* _str, ustr8  const* _src_end, uchar8 & _out)
		{
			if (_str >= _src_end)
				return 0;

			uchar8 c((u32)_str->c);
			const s32 n = numBytes(c);
			if ((_str+n)<=_src_end)
			{
				switch (n)
				{
				case 4: c.c=c.c|((u32)_str[3].c << 24);
				case 3: c.c=c.c|((u32)_str[2].c << 16);
				case 2: c.c=c.c|((u32)_str[1].c <<  8);
				}
				_out = c;
				return n;
			}
			else
			{
				return 0;
			}
		}

		inline s32			read		(ustr16 const* _str, uchar16& _out)
		{
			uchar16 c((u32)_str->c);
			_str++;

			const s32 n = numBytes(c);
			if (n==2)
			{
				c.c = c.c | ((u32)_str->c << 16);
				_str++;
			}
			_out = c;
			return n;
		}

		inline s32			read		(ustr16 const* _str, ustr16 const* _src_end, uchar16& _out)
		{
			if (_str >= _src_end)
				return 0;

			uchar16 c((u32)_str->c);
			_str++;

			const s32 n = numBytes(c);
			if (n==2)
			{
				if (_str >= _src_end)
					return -1;
				c.c = c.c | ((u32)_str->c << 16);
				_str++;
			}
			_out = c;
			return n;
		}

		inline s32			write		(ustr8* _dst, uchar8 _char)
		{
			u32 const n = numBytes(_char);
			{
				u32 c = _char.c;
				switch (n)
				{
				case 4: _dst->c = c&0xFF; c>>=8; ++_dst;
				case 3: _dst->c = c&0xFF; c>>=8; ++_dst;
				case 2: _dst->c = c&0xFF; c>>=8; ++_dst;
				case 1: _dst->c = c&0xFF; 
				};
				return n;
			}
		}

		inline s32			write		(ustr8* _dst, uchar8 _char, ustr8 const* _dst_end)
		{
			u32 const n = numBytes(_char);
			if ((_dst+n) <= _dst_end)
			{
				u32 c = _char.c;
				switch (n)
				{
				case 4: _dst->c = c&0xFF; c>>=8; ++_dst;
				case 3: _dst->c = c&0xFF; c>>=8; ++_dst;
				case 2: _dst->c = c&0xFF; c>>=8; ++_dst;
				case 1: _dst->c = c&0xFF; 
				};
				return n;
			}
			else
			{
				return 0;
			}
		}

		inline s32			write		(ustr16* _dst, uchar16 _char)
		{
			u32 const n = numBytes(_char);
			{
				u32 c = _char.c;
				switch (n)
				{
				case 4: _dst->c = c&0xFFFF; c>>=16; ++_dst;
				case 2: _dst->c = c&0xFFFF; 
				};
				return n;
			}
		}

		inline s32			write		(ustr16* _dst, uchar16 _char, ustr16 const* _dst_end)
		{
			u32 const n = numBytes(_char);
			if ((_dst+n) <= _dst_end)
			{
				u32 c = _char.c;
				switch (n)
				{
				case 4: _dst->c = c&0xFFFF; c>>=16; ++_dst;
				case 2: _dst->c = c&0xFFFF; 
				};
				return n;
			}
			else
			{
				return 0;
			}
		}

		inline bool			isLegal				(uchar8  _c)
		{
			u8 const a = _c.c & 0xFF;
			if (a<0x80)		// ASCII, 0 - 127
				return true;

			u8 b;
			switch (0xF0 & a)
			{
			case 0xF0: // 4
				{
					b = (_c.c >> 24) & 0xFF;
					if (b<0x80 || b>0xBF) return false;
				};
			case 0xE0: // 3 
				{
					b = (_c.c >> 16) & 0xFF;
					if (b<0x80 || b>0xBF) return false;
				};
			default:   // 2
				{
					b = (_c.c >> 8) & 0xFF;
					if (b<0x80 || b>0xBF) return false;
				};
			}

			switch (a)
			{
				// no fall-through in this inner switch
			case 0xE0: if (b < 0xA0) return false; break;
			case 0xED: if (b > 0x9F) return false; break;
			case 0xF0: if (b < 0x90) return false; break;
			case 0xF4: if (b > 0x8F) return false; break;
			default:   if (b < 0x80) return false;
			}

			return true;
		}

		inline bool			isLegal				(uchar16 _c)
		{
			u16 const ch1 = _c.c & 0xFFFF;
			if (ch1 >= 0xD800 && ch1 <= 0xDBFF)
			{
				u16 const ch2 = ((_c.c>>16) & 0xFFFF);
				return (ch2 >= 0xDC00 && ch2 <= 0xDFFF);
			}
			else
			{
				return !(ch1 >= 0xDC00 && ch1 <= 0xDFFF);
			}
		}

		inline bool			isLegal		(const ustr8 * _src, const ustr8 * _src_end)
		{
			const ustr8* s = _src;
			uchar8 ch;
			do
			{
				s32 n = read(s, _src_end, ch);
				if (n<=0)
					return false;
				if (!isLegal(ch))
					return false;
				s += n;
			} while(s<_src_end);
			return true;
		}

		inline bool			isLegal		(const ustr16* _src, const ustr16* _src_end)
		{
			const ustr16* s = _src;
			uchar16 ch;
			do
			{
				s32 n = read(s, _src_end, ch);
				if (n<=0)
					return false;
				if (!isLegal(ch))
					return false;
				s += n;
			} while(s<_src_end);
			return true;
		}

		static const s32 halfShift  = 10; // used for shifting by 10 bits

		static const u32 halfBase = 0x00010000;
		static const u32 halfMask = 0x000003FF;

		static const u32 byteMask = 0xBF;
		static const u32 byteMark = 0x80; 

		inline bool			convert		(uchar16 _from, uchar8& _to)
		{
			_to.c = '?';

			u16 ch = _from.c & 0xFFFF;
			// If we have a surrogate pair, convert to u32 first
			if (ch >= 0xD800 && ch <= 0xDBFF)
			{
				u32 ch2 = (_from.c>>16) & 0xFFFF;

				// If it's a low surrogate, convert to u32
				if (ch2 >= 0xDC00 && ch2 <= 0xDFFF)
					ch = ((ch - 0xD800) << halfShift) + (ch2 - 0xDC00) + halfBase;
				else 
					return false;	// it's an unpaired high surrogate
			}
			else
			{
				if (ch >= 0xDC00 && ch <= 0xDFFF)
					return false;
			}

			// Figure out how many bytes the result will require
			u32 bytesToWrite;
			if      (ch < (u32)0x80)     { bytesToWrite = 1;}
			else if (ch < (u32)0x800)    { bytesToWrite = 2; }
			else if (ch < (u32)0x10000)  { bytesToWrite = 3; }
			else if (ch < (u32)0x110000) { bytesToWrite = 4; }
			else                         { return false; }

			static const u8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

			switch (bytesToWrite)
			{ // note: everything falls through.
			case 4: _to.c = (u8)((ch | byteMark) & byteMask); _to.c = _to.c << 8; ch >>= 6;
			case 3: _to.c = (u8)((ch | byteMark) & byteMask); _to.c = _to.c << 8; ch >>= 6;
			case 2: _to.c = (u8)((ch | byteMark) & byteMask); _to.c = _to.c << 8; ch >>= 6;
			case 1: _to.c = (u8) (ch | firstByteMark[bytesToWrite]);
			}

			return true;
		}

		inline bool			convert		(uchar8  _from, uchar16& _to)
		{
			_to.c = '?';

			// Do this check whether lenient or strict
			if (!isLegal(_from))
				return false;

			u16 const bytesToRead = numBytes(_from);

			u32 ch8 = _from.c;
			u32 ch16 = 0;

			switch (bytesToRead)
			{
			case 4: ch16 += ch8 & 0xFF; ch8=ch8>>8; ch16<<=6;
			case 3: ch16 += ch8 & 0xFF; ch8=ch8>>8; ch16<<=6;
			case 2: ch16 += ch8 & 0xFF; ch8=ch8>>8; ch16<<=6;
			case 1: ch16 += ch8 & 0xFF; break;
			default:
				return false;
				break;
			}
			static const u32 offsetsFromUTF8[5] = { 0/*UNUSED*/, 0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL };
			ch16 -= offsetsFromUTF8[bytesToRead];

			if (ch16 <= 0x0000FFFF)	// Target is a character <= 0xFFFF
			{	
				if (ch16 >= 0xD800 && ch16 <= 0xDFFF)	// UTF-16 surrogate values are illegal
					return false;
				_to.c = ch16;
			} 
			else if (ch16 > 0x0010FFFF)
			{
				return false;
			}
			else
			{
				ch16 -= halfBase;
				_to.c = (u16)((ch16 >> halfShift) + 0xD800);
				_to.c = _to.c | (((ch16 & halfMask) + 0xDC00)<<16);
			}
			return true;
		}

		inline s32			convert		(ustr8  const* _src, ustr8  const* _src_end, ustr16*& _dst, ustr16 const* _dst_end)
		{
			s32 numChars = 0;
			ustr8 const* src = _src;
			while (src < _src_end)
			{
				uchar8 ch8;
				s32 const srcn = read(src, _src_end, ch8);
				if (srcn<=0)
					return -1;

				uchar16 ch16;
				if (!convert(ch8, ch16))
					return -1;

				bool terminate = false;
				if (ch8.c == 0)
				{
					terminate = true;
				}
				else if ((_dst_end - _dst) == 1)
				{
					terminate = true;
				}
				else if ((_dst_end - _dst) == 2)
				{
					terminate = (numBytes(ch16)==4);
				}

				if (terminate)
				{
					ch16.c = '\0';
					s32 const dstn = write(_dst, ch16, _dst_end);
					if (dstn<=0)
						return -1;
					return numChars;
				}

				s32 const dstn = write(_dst, ch16, _dst_end);
				if (dstn<0)
					return -1;
				_dst += dstn;
				++numChars;
			}
			return numChars;
		}

		inline s32			convert		(ustr16 const* _src, ustr16 const* _src_end, ustr8 *& _dst, ustr8  const* _dst_end)
		{
			s32 numChars = 0;
			ustr16 const* src = _src;
			while (src < _src_end)
			{
				uchar16 ch16;
				s32 const srcn = read(src, _src_end, ch16);
				if (srcn<=0)
					return -1;

				uchar8 ch8;
				if (!convert(ch16, ch8))
					return -1;

				bool terminate = false;
				if (ch8.c == 0)
				{
					terminate = true;
				}
				else 
				{
					const u32 n = (u32)(_dst_end - _dst);
					if (n == 1)
					{
						terminate = true;
					}
					else if (n <= 4)
					{
						terminate = (numBytes(ch8)>=n);
					}
				}
				if (terminate)
				{
					ch8.c = '\0';
					s32 const dstn = write(_dst, ch8, _dst_end);
					if (dstn<=0)
						return -1;
					return numChars;
				}

				s32 const dstn = write(_dst, ch8, _dst_end);
				if (dstn<=0)
					return -1;
				_dst += dstn;
				++numChars;
			}
			return numChars;
		}
	}

	namespace utf
	{
		/**
			* FORMATTED STRING FUNCTIONS
			*==============================================================================
			*
			*  x_cprintf    
			*
			*      Formatted print counting function, will return the number of characters needed.
			*
			*  x_sprintf    
			*
			*      Formatted print to "string".
			*
			*==============================================================================
		*/
		s32		x_cprintf		(                            const ustr8* formatStr, const x_va& v1             , const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, 
																					const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty, const x_va& v11=x_va::sEmpty, const x_va& v12=x_va::sEmpty, const x_va& v13=x_va::sEmpty, const x_va& v14=x_va::sEmpty, const x_va& v15=x_va::sEmpty, const x_va& v16=x_va::sEmpty);
		s32		x_sprintf		(ustr8* buffer, s32 maxChars, const ustr8* formatStr, const x_va& v1             , const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, 
																					const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty, const x_va& v11=x_va::sEmpty, const x_va& v12=x_va::sEmpty, const x_va& v13=x_va::sEmpty, const x_va& v14=x_va::sEmpty, const x_va& v15=x_va::sEmpty, const x_va& v16=x_va::sEmpty);
		s32		x_vcprintf		(                            const ustr8* formatStr, const x_va_list& args);
		s32		x_vsprintf		(ustr8* buffer, s32 maxChars, const ustr8* formatStr, const x_va_list& args);

		/**
			* FORMATTED STRING FUNCTIONS
			*==============================================================================
			*
			*  x_printf    
			*
			*      Formatted print to "standard text output".  This is straight forward for
			*      text mode programs and is handled by xbase.  Graphical programs
			*      must register a function to handle this operation.
			*
			*==============================================================================
		*/

		s32		x_printf   		(const ustr8* formatStr, const x_va& v1             , const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, 
														const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty, const x_va& v11=x_va::sEmpty, const x_va& v12=x_va::sEmpty, const x_va& v13=x_va::sEmpty, const x_va& v14=x_va::sEmpty, const x_va& v15=x_va::sEmpty, const x_va& v16=x_va::sEmpty);
		s32		x_printf		(const ustr8* formatStr, const x_va_list& args)	;
		s32		x_printf		(const ustr8* str);

	}
}

#endif    ///< __XBASE_STRING_UTF_H__

