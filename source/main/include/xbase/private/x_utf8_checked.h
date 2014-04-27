#ifndef __XBASE_UTF8_CHECKED_H__
#define __XBASE_UTF8_CHECKED_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/private/x_utf8_core.h"

namespace xcore
{
	namespace utf8
	{
		template <typename octet_iterator>
		octet_iterator append(u32 cp, octet_iterator result)
		{
			if (!utf8::internal::is_code_point_valid(cp))
			{
				*(result++) = '?';
			}
			else
			{
				if (cp < 0x80)                        // one octet
					*(result++) = static_cast<u8>(cp);
				else if (cp < 0x800) {                // two octets
					*(result++) = static_cast<u8>((cp >> 6)            | 0xc0);
					*(result++) = static_cast<u8>((cp & 0x3f)          | 0x80);
				}
				else if (cp < 0x10000) {              // three octets
					*(result++) = static_cast<u8>((cp >> 12)           | 0xe0);
					*(result++) = static_cast<u8>(((cp >> 6) & 0x3f)   | 0x80);
					*(result++) = static_cast<u8>((cp & 0x3f)          | 0x80);
				}
				else {                                // four octets
					*(result++) = static_cast<u8>((cp >> 18)           | 0xf0);
					*(result++) = static_cast<u8>(((cp >> 12) & 0x3f)  | 0x80);
					*(result++) = static_cast<u8>(((cp >> 6) & 0x3f)   | 0x80);
					*(result++) = static_cast<u8>((cp & 0x3f)          | 0x80);
				}
			}
			return result;
		}

		template <typename octet_iterator, typename output_iterator>
		output_iterator replace_invalid(octet_iterator start, octet_iterator end, output_iterator out, u32 replacement)
		{
			while (start != end) 
			{
				octet_iterator sequence_start = start;
				internal::utf_error err_code = utf8::internal::validate_next(start, end);
				switch (err_code) 
				{
				case internal::UTF8_OK :
					for (octet_iterator it = sequence_start; it != start; ++it)
						*out++ = *it;
					break;
				case internal::NOT_ENOUGH_ROOM:
					return out;
				case internal::INVALID_LEAD:
					out = utf8::append (replacement, out);
					++start;
					break;
				case internal::INCOMPLETE_SEQUENCE:
				case internal::OVERLONG_SEQUENCE:
				case internal::INVALID_CODE_POINT:
					out = utf8::append (replacement, out);
					++start;
					// just one replacement mark for the sequence
					while (start != end && utf8::internal::is_trail(*start))
						++start;
					break;
				}
			}
			return out;
		}

		template <typename octet_iterator, typename output_iterator>
		inline output_iterator replace_invalid(octet_iterator start, octet_iterator end, output_iterator out)
		{
			static const u32 replacement_marker = utf8::internal::mask16(0xfffd);
			return utf8::replace_invalid(start, end, out, replacement_marker);
		}

		template <typename octet_iterator>
		u32 next(octet_iterator& it, octet_iterator end)
		{
			u32 cp = 0;
			internal::utf_error err_code = utf8::internal::validate_next(it, end, cp);
			switch (err_code) {
			case internal::UTF8_OK :
				break;
			case internal::NOT_ENOUGH_ROOM :
				cp = '?';
				break;
			case internal::INVALID_LEAD :
			case internal::INCOMPLETE_SEQUENCE :
			case internal::OVERLONG_SEQUENCE :
				cp = '?';
				break;
			case internal::INVALID_CODE_POINT :
				cp = '?';
				break;
			}
			return cp;
		}

		template <typename octet_iterator>
		u32 peek_next(octet_iterator it, octet_iterator end)
		{
			return utf8::next(it, end);
		}

		template <typename octet_iterator>
		u32 prior(octet_iterator& it, octet_iterator start)
		{
			// can't do much if it == start
			if (it == start)
				return 0;

			octet_iterator end = it;
			// Go back until we hit either a lead octet or start
			while (utf8::internal::is_trail(*(--it)))
			{
				if (it == start)
					return 0; // error - no lead byte in the sequence
			}
			return utf8::peek_next(it, end);
		}

		/// Deprecated in versions that include "prior"
		template <typename octet_iterator>
		u32 previous(octet_iterator& it, octet_iterator pass_start)
		{
			octet_iterator end = it;
			while (utf8::internal::is_trail(*(--it)))
			{
				if (it == pass_start)
					return 0; // error - no lead byte in the sequence
			}
			octet_iterator temp = it;
			return utf8::next(temp, end);
		}

		template <typename octet_iterator, typename distance_type>
		void advance (octet_iterator& it, distance_type n, octet_iterator end)
		{
			for (distance_type i = 0; i < n; ++i)
				utf8::next(it, end);
		}

		template <typename octet_iterator>
		typename std::iterator_traits<octet_iterator>::difference_type
			distance (octet_iterator first, octet_iterator last)
		{
			typename std::iterator_traits<octet_iterator>::difference_type dist;
			for (dist = 0; first < last; ++dist)
				utf8::next(first, last);
			return dist;
		}

		template <typename u16bit_iterator, typename octet_iterator>
		octet_iterator utf16to8 (u16bit_iterator start, u16bit_iterator end, octet_iterator result)
		{
			while (start != end)
			{
				u32 cp = utf8::internal::mask16(*start++);
				// Take care of surrogate pairs first
				if (utf8::internal::is_lead_surrogate(cp)) 
				{
					if (start != end) 
					{
						u32 trail_surrogate = utf8::internal::mask16(*start++);
						if (utf8::internal::is_trail_surrogate(trail_surrogate))
							cp = (cp << 10) + trail_surrogate + internal::SURROGATE_OFFSET;
						else
							cp = '?';
					}
					else
					{
						cp = '?';
					}
				}
				// Lone trail surrogate
				else if (utf8::internal::is_trail_surrogate(cp))
					cp = '?';

				result = utf8::append(cp, result);
			}
			return result;
		}

		template <typename u16bit_iterator, typename octet_iterator>
		u16bit_iterator utf8to16 (octet_iterator start, octet_iterator end, u16bit_iterator result)
		{
			while (start != end)
			{
				u32 cp = utf8::next(start, end);
				if (cp > 0xffff)
				{ //make a surrogate pair
					*result++ = static_cast<u16>((cp >> 10)   + internal::LEAD_OFFSET);
					*result++ = static_cast<u16>((cp & 0x3ff) + internal::TRAIL_SURROGATE_MIN);
				}
				else
					*result++ = static_cast<u16>(cp);
			}
			return result;
		}

		template <typename octet_iterator, typename u32bit_iterator>
		octet_iterator utf32to8 (u32bit_iterator start, u32bit_iterator end, octet_iterator result)
		{
			while (start != end)
				result = utf8::append(*(start++), result);

			return result;
		}

		template <typename octet_iterator, typename u32bit_iterator>
		u32bit_iterator utf8to32 (octet_iterator start, octet_iterator end, u32bit_iterator result)
		{
			while (start != end)
				(*result++) = utf8::next(start, end);

			return result;
		}

		// The iterator class
		template <typename octet_iterator>
		class iterator : public std::iterator <std::bidirectional_iterator_tag, u32> 
		{
			octet_iterator it;
			octet_iterator range_start;
			octet_iterator range_end;
		
		public:
			iterator () {}
			explicit iterator (const octet_iterator& octet_it, const octet_iterator& range_start, const octet_iterator& range_end) 
				: it(octet_it), range_start(range_start), range_end(range_end)
			{
				if (it < range_start)
					it = range_start;
				else if (it > range_end)
					it = range_end;
			}
			// the default "big three" are OK
			octet_iterator base () const { return it; }
			u32 operator * () const
			{
				octet_iterator temp = it;
				return utf8::next(temp, range_end);
			}
			bool operator == (const iterator& rhs) const
			{
				if (range_start != rhs.range_start || range_end != rhs.range_end)
					return false;
				return (it == rhs.it);
			}
			bool operator != (const iterator& rhs) const
			{
				return !(operator == (rhs));
			}
			iterator& operator ++ ()
			{
				utf8::next(it, range_end);
				return *this;
			}
			iterator operator ++ (int)
			{
				iterator temp = *this;
				utf8::next(it, range_end);
				return temp;
			}
			iterator& operator -- ()
			{
				utf8::prior(it, range_start);
				return *this;
			}
			iterator operator -- (int)
			{
				iterator temp = *this;
				utf8::prior(it, range_start);
				return temp;
			}
		}; // class iterator
	} // namespace utf8
}  // namespace xcore

#endif //__XBASE_UTF8_CHECKED_H__
