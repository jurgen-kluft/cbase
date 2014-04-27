#ifndef __XBASE_UTF8_UNCHECKED_H__
#define __XBASE_UTF8_UNCHECKED_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/private/x_utf8_core.h"

namespace xcore
{
	namespace utf8
	{
		namespace unchecked 
		{
			template <typename octet_iterator>
			octet_iterator append(u32 cp, octet_iterator result)
			{
				if (cp < 0x80)                        // one octet
					*(result++) = static_cast<u8>(cp);  
				else if (cp < 0x800) {                // two octets
					*(result++) = static_cast<u8>((cp >> 6)          | 0xc0);
					*(result++) = static_cast<u8>((cp & 0x3f)        | 0x80);
				}
				else if (cp < 0x10000) {              // three octets
					*(result++) = static_cast<u8>((cp >> 12)         | 0xe0);
					*(result++) = static_cast<u8>(((cp >> 6) & 0x3f) | 0x80);
					*(result++) = static_cast<u8>((cp & 0x3f)        | 0x80);
				}
				else {                                // four octets
					*(result++) = static_cast<u8>((cp >> 18)         | 0xf0);
					*(result++) = static_cast<u8>(((cp >> 12) & 0x3f)| 0x80);
					*(result++) = static_cast<u8>(((cp >> 6) & 0x3f) | 0x80);
					*(result++) = static_cast<u8>((cp & 0x3f)        | 0x80);
				}
				return result;
			}

			template <typename octet_iterator>
			u32 next(octet_iterator& it)
			{
				u32 cp = utf8::internal::mask8(*it);
				typename std::iterator_traits<octet_iterator>::difference_type length = utf8::internal::sequence_length(it);
				switch (length) {
				case 1:
					break;
				case 2:
					it++;
					cp = ((cp << 6) & 0x7ff) + ((*it) & 0x3f);
					break;
				case 3:
					++it; 
					cp = ((cp << 12) & 0xffff) + ((utf8::internal::mask8(*it) << 6) & 0xfff);
					++it;
					cp += (*it) & 0x3f;
					break;
				case 4:
					++it;
					cp = ((cp << 18) & 0x1fffff) + ((utf8::internal::mask8(*it) << 12) & 0x3ffff);                
					++it;
					cp += (utf8::internal::mask8(*it) << 6) & 0xfff;
					++it;
					cp += (*it) & 0x3f; 
					break;
				}
				++it;
				return cp;        
			}

			template <typename octet_iterator>
			u32 peek_next(octet_iterator it)
			{
				return utf8::unchecked::next(it);    
			}

			template <typename octet_iterator>
			u32 prior(octet_iterator& it)
			{
				while (utf8::internal::is_trail(*(--it))) ;
				octet_iterator temp = it;
				return utf8::unchecked::next(temp);
			}

			// Deprecated in versions that include prior, but only for the sake of consistency (see utf8::previous)
			template <typename octet_iterator>
			inline u32 previous(octet_iterator& it)
			{
				return utf8::unchecked::prior(it);
			}

			template <typename octet_iterator, typename distance_type>
			void advance (octet_iterator& it, distance_type n)
			{
				for (distance_type i = 0; i < n; ++i)
					utf8::unchecked::next(it);
			}

			template <typename octet_iterator>
			typename std::iterator_traits<octet_iterator>::difference_type
				distance (octet_iterator first, octet_iterator last)
			{
				typename std::iterator_traits<octet_iterator>::difference_type dist;
				for (dist = 0; first < last; ++dist) 
					utf8::unchecked::next(first);
				return dist;
			}

			template <typename u16bit_iterator, typename octet_iterator>
			octet_iterator utf16to8 (u16bit_iterator start, u16bit_iterator end, octet_iterator result)
			{       
				while (start != end) {
					u32 cp = utf8::internal::mask16(*start++);
					// Take care of surrogate pairs first
					if (utf8::internal::is_lead_surrogate(cp)) {
						u32 trail_surrogate = utf8::internal::mask16(*start++);
						cp = (cp << 10) + trail_surrogate + internal::SURROGATE_OFFSET;
					}
					result = utf8::unchecked::append(cp, result);
				}
				return result;         
			}

			template <typename u16bit_iterator, typename octet_iterator>
			u16bit_iterator utf8to16 (octet_iterator start, octet_iterator end, u16bit_iterator result)
			{
				while (start < end) {
					u32 cp = utf8::unchecked::next(start);
					if (cp > 0xffff) { //make a surrogate pair
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
					result = utf8::unchecked::append(*(start++), result);

				return result;
			}

			template <typename octet_iterator, typename u32bit_iterator>
			u32bit_iterator utf8to32 (octet_iterator start, octet_iterator end, u32bit_iterator result)
			{
				while (start < end)
					(*result++) = utf8::unchecked::next(start);

				return result;
			}

			// The iterator class
			template <typename octet_iterator>
			class iterator : public std::iterator <std::bidirectional_iterator_tag, u32>
			{ 
				octet_iterator it;
			public:
				iterator () {}
				explicit iterator (const octet_iterator& octet_it): it(octet_it) {}
				// the default "big three" are OK
				octet_iterator base () const { return it; }
				u32 operator * () const
				{
					octet_iterator temp = it;
					return utf8::unchecked::next(temp);
				}
				bool operator == (const iterator& rhs) const 
				{ 
					return (it == rhs.it);
				}
				bool operator != (const iterator& rhs) const
				{
					return !(operator == (rhs));
				}
				iterator& operator ++ () 
				{
					::std::advance(it, utf8::internal::sequence_length(it));
					return *this;
				}
				iterator operator ++ (int)
				{
					iterator temp = *this;
					::std::advance(it, utf8::internal::sequence_length(it));
					return temp;
				}  
				iterator& operator -- ()
				{
					utf8::unchecked::prior(it);
					return *this;
				}
				iterator operator -- (int)
				{
					iterator temp = *this;
					utf8::unchecked::prior(it);
					return temp;
				}
			}; // class iterator

		} // namespace utf8::unchecked
	} // namespace utf8 
} // namespace xcore 

#endif // __XBASE_UTF8_UNCHECKED_H__
