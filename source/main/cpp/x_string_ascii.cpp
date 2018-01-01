#include "xbase/x_string_ascii.h"
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_console.h"


//==============================================================================
// INCLUDES
//==============================================================================


/**
 * xCore namespace
 */
namespace xcore
{
	namespace ascii
	{

		/**
		*==============================================================================
		*==============================================================================
		* ASCII helper functions
		*==============================================================================
		*==============================================================================
		*/

		//------------------------------------------------------------------------------
		static inline uchar32	peek_char(pcrune str, s32* len = NULL)
		{
			rune c = *str;
			if (len != NULL)
				*len = 1;
			return c;
		}

		//------------------------------------------------------------------------------
		static uchar32			read_char(prune& str)
		{
			s32 l = 0;
			uchar32 c = peek_char(str, &l);
			if (c != 0)
				str += l;
			return c;
		}

		//------------------------------------------------------------------------------
		static uchar32			read_char(pcrune& str)
		{
			s32 l = 0;
			uchar32 c = peek_char(str, &l);
			str += l;
			return c;
		}

		//------------------------------------------------------------------------------
		static u8				sUTF8LC[] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
		static prune			write_char(uchar32 c, prune str, pcrune end)
		{
			if (str != NULL && str < end)
			{
				if (c > 0x7f)
					c = '?';
				*str++ = (rune)c;
			}
			return str;
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------

		static s32 len_in_bytes(uchar32 c)
		{
			if ((c & 0xffffff80) == 0x00)
			{
				return 1;
			}

			s32 l = 0;
			if ((c & 0xf8000000) == 0xf0000000) { l = 4; }
			else if ((c & 0xf00000) == 0xe00000) { l = 3; }
			else if ((c & 0xe000) == 0xc000) { l = 2; }
			return l;
		}

		static s32	len_in_chars(pcrune str, pcrune end)
		{
			s32 l = 0;
			if (end != NULL)
			{
				while (str < end)
				{
					read_char(str);
					l++;
				}
			}
			else
			{
				while (true)
				{
					uchar32 c = read_char(str);
					if (c == '\0')
						break;
					l++;
				}
			}
			return l;
		}

		void		copy(xuchars& _dest, xcuchars& _src, ECopyType type)
		{
			s32 len = 0;
			pcrune src = _src.m_str;
			pcrune src_end = _src.m_end;
			prune dest = _dest.m_str;
			prune dest_end = _dest.m_eos;
			while (dest < dest_end)
			{
				if (src_end != NULL && src == src_end)
				{
					if (type == COPY_AND_WRITE_MATCHING_TERMINATOR && *src_end == '\0')
						write_char('\0', dest, dest_end);
					break;
				}
				uchar32 const c = read_char(src);
				if (src_end == NULL && c == '\0')
				{
					if (type == COPY_AND_WRITE_MATCHING_TERMINATOR)
						write_char(c, dest, dest_end);
					break;
				}
				dest = write_char(c, dest, dest_end);
			}

			if (dest < dest_end && type == COPY_AND_WRITE_TERMINATOR)
				write_char('\0', dest, dest_end);
		}

		xcuchars		find(xcuchars& _str, xcuchars& _find, ECmpMode mode)
		{
			pcrune str_begin = _str.m_str;
			pcrune str_end = _str.m_end;
			pcrune find_begin = _find.m_str;
			pcrune find_end = _find.m_end;
			if (find_end != NULL && find_begin == find_end)
				return NULL;
			if (str_end != NULL && str_begin == str_end)
				return NULL;

			pcrune str_iter = str_begin;
			bool found = false;
			pcrune found_pos = NULL;
			while (true)
			{
				if (str_end != NULL && str_iter == str_end)
					break;

				found_pos = str_iter;
				uchar32 sc = read_char(str_iter);
				if (str_end == NULL && sc == '\0')
					break;

				pcrune str_find = str_iter;
				pcrune find_str = find_begin;

				uchar32 fc = read_char(find_str);
				found = is_equal(sc, fc, mode);
				while (found)
				{
					if ((find_end != NULL && find_str == find_end) || (find_end == NULL && *find_str == '\0'))
						break;
					if ((str_end != NULL && str_find == str_end) || (str_end == NULL && *str_find == '\0'))
					{
						found = false;
						break;
					}

					sc = read_char(str_find);
					fc = read_char(find_str);
					found = is_equal(sc, fc, mode);
				}

				if (found)
					break;
			}
			return found ? found_pos : NULL;
		}

		xcuchars	find(xcuchars& _str, uchar32 _c, ECmpMode mode)
		{
			pcrune str = _str.m_str;
			pcrune str_end = _str.m_end;
			while (true)
			{
				if (str_end != NULL && str == str_end)
					break;
				pcrune cur_pos = str;
				uchar32 c = read_char(str);
				if (str_end == NULL && c == '\0')
					break;
				if (is_equal(c, _c, mode))
					return xcuchars(cur_pos, str_end);
			}
			return xcuchars();
		}

		bool	contains(xcuchars& _str, uchar32 _c, ECmpMode mode)
		{
			xcuchars f = find(_str, _c, mode);
			return f.is_empty() == false;
		}

		xcuchars	find_one_of(xcuchars& _str, xcuchars& _set, ECmpMode mode)
		{
			pcrune str = _str.m_str;
			pcrune str_end = _str.m_end;
			while (true)
			{
				if (str_end != NULL && str == str_end)
					break;
				pcrune cur_pos = str;
				uchar32 c = read_char(str);
				if (str_end == NULL && c == '\0')
					break;
				if (contains(_set, c, mode))
					return xcuchars(cur_pos, str_end);
			}
			return xcuchars();
		}

		void	replace(xuchars& _str, xcuchars& _replace)
		{
			// The logic here is based on memory copy, we do not consider characters
			s32 const selected_len = (s32)((xbyte const*)_str.m_end - (xbyte const*)_str.m_str);
			s32 const replace_len = (s32)((xbyte const*)_replace.m_end - (xbyte const*)_replace.m_str);

			prune end = NULL;
			if (selected_len < replace_len)
			{
				// Move, expanding
				s32 move_len = replace_len - selected_len;
				if (move_len >((xbyte const*)_str.m_eos - (xbyte*)_str.m_end))
					move_len = (s32)(((xbyte const*)_str.m_eos - (xbyte*)_str.m_end));

				// No matter what, push out anything at the end!
				xbyte * dst = (xbyte*)((xbyte*)_str.m_end + ((xbyte const*)_str.m_eos - (xbyte*)_str.m_end) - 1);
				xbyte * src = (xbyte*)((xbyte*)dst - move_len);
				while (dst > (xbyte*)_str.m_end)
					*dst-- = *src--;

				end = (prune)((xbyte*)_str.m_str + selected_len + move_len);		// Update str_end
			}
			else if (selected_len > replace_len)
			{
				// Move, shrinking
				s32 move_len = selected_len - replace_len;
				xbyte* dst = (xbyte*)((xbyte*)_str.m_end - move_len);
				xbyte* src = (xbyte*)(_str.m_end);
				while (src < (xbyte const*)_str.m_eos)
					*dst++ = *src++;

				end = (prune)((xbyte*)_str.m_str + selected_len - move_len);		// Update str_end
			}
			else
			{
				end = (prune)(_str.m_str + selected_len);
			}

			// Replace
			xbyte const* src = (xbyte const*)_replace.m_str;
			xbyte const* src_end = (xbyte const*)_replace.m_str + replace_len;
			xbyte* dst = (xbyte*)_str.m_str;
			while (src < src_end)
				*dst++ = *src++;

			_str.m_end = end;
		}


		s32	compare(xcuchars& _lstr, xcuchars& _rstr, ECmpMode mode)
		{
			if (_lstr.m_str != _rstr.m_str && _lstr.m_end != _rstr.m_end)
			{
				pcrune lstr = _lstr.m_str;
				pcrune rstr = _rstr.m_str;
				do
				{
					bool const is_lstr_end = (lstr == _lstr.m_end);
					bool const is_rstr_end = (rstr == _rstr.m_end);
					if (is_lstr_end && is_rstr_end)
						break;
					if (is_lstr_end)
						return -1;
					if (is_rstr_end)
						return 1;

					uchar32 c1 = read_char(lstr);
					uchar32 c2 = read_char(rstr);
					if (mode == CASE_IGNORE)
					{
						c1 = to_lower(c1);
						c2 = to_lower(c2);
					}
					if (c1 < c2)
						return -1;
					if (c1 > c2)
						return 1;
				} while (true);
			}
			return 0;
		}

		//------------------------------------------------------------------------------
		void concatenate(xuchars& str, xcuchars& concat)
		{
			s32 l = 0;
			prune wptr = str.m_end;
			pcrune rptr = concat.m_str;
			pcrune rend = concat.m_end;
			while (wptr < str.m_eos)
			{
				if (rend != NULL && rptr == rend)
					break;
				prune front = wptr;
				uchar32 c = read_char(rptr);
				prune nwptr = write_char(c, wptr, str.m_eos);
				if (nwptr == wptr)
					break;
				wptr = nwptr;
			}
			str.m_end = wptr;
		}

		xcuchars	parse(xcuchars& str, bool& value)
		{
			xcuchars format("%b");
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		xcuchars	parse(xcuchars& str, s32& value, s32 base)
		{
			xcuchars format;
			switch (base)
			{
			case 16: format = "%x"; break;
			case 10: format = "%d"; break;
			case 8: format = "%o"; break;
			};
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		xcuchars	parse(xcuchars& str, u32& value, s32 base)
		{
			xcuchars format;
			switch (base)
			{
			case 16: format = "%x"; break;
			case 10: format = "%d"; break;
			case 8: format = "%o"; break;
			};
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		xcuchars	parse(xcuchars& str, s64& value, s32 base)
		{
			xcuchars format;
			switch (base)
			{
			case 16: format = "%x"; break;
			case 10: format = "%d"; break;
			case 8: format = "%o"; break;
			};
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		xcuchars	parse(xcuchars& str, u64& value, s32 base)
		{
			xcuchars format;
			switch (base)
			{
			case 16: format = "%x"; break;
			case 10: format = "%d"; break;
			case 8: format = "%o"; break;
			};
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		xcuchars	parse(xcuchars& str, f32& value)
		{
			xcuchars format("%f");
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		xcuchars	parse(xcuchars& str, f64& value)
		{
			xcuchars format("%f");
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		//------------------------------------------------------------------------------
		bool is_decimal(xcuchars& str)
		{
			pcrune src = str;
			uchar32 c = peek_char(src);
			if (c == '-')
			{
				read_char(src);
			}

			while (true)
			{
				if (str_end != NULL && src == str_end)
					break;
				uchar32 c = read_char(src);
				if (str_end == NULL && c == '\0')
					break;
				if (c < '0' || c > '9')
					return false;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool is_hexadecimal(xcuchars& str, bool with_prefix)
		{
			uchar32 c = 0;
			uchar32 p;
			s32 pos = 0;
			while (str < str_end)
			{
				p = c;
				c = read_char(str);
				if (c == 'x') {
					if (pos == 1 && with_prefix && p == '0') {
						// Ok, 0x.... prefix
					}
					else {
						return false;
					}
				}
				else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
					// Ok, digit or hex character
				}
				else {
					return false;
				}
				pos++;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool is_float(xcuchars& str)
		{
			static pcrune sFloatStr = (pcrune)"Ee.#QNABIF";

			// Does it have any other of the strange characters?
			pcrune src = str;
			for (; src<str_end; )
			{
				uchar32 c = read_char(src);
				if (c >= '0' && c <= '9')
				{
					continue;
				}

				pcrune float_str = sFloatStr;
				uchar32 f = read_char(float_str);;
				while (f != c)
				{
					f = read_char(float_str);
					if (f == '\0')
						break;
				}
				// Okay this is not a number
				if (f == '\0')
					return false;
				src++;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool is_GUID(xcuchars& str)
		{
			// Does it have only 0-9, a-f, A-F characters
			s32 n = 0;
			s32 l = 0;
			pcrune src = str;
			for (; src<str_end; )
			{
				uchar32 c = read_char(src);
				
				if (str_end == NULL && c == '\0') break;

				if (c >= '0' && c <= '9') { n++;  continue; }
				if (c >= 'A' && c <= 'F') { n++;  continue; }
				if (c >= 'a' && c <= 'f') { n++;  continue; }
				if (c == ':')
				{
					l++;
					if (l > 3)
						return false;
					continue;
				}
				return false;
			}

			// the length of the string must be 17 characters long (4*8 + 3 ':')
			return (n == (4 * 8) && l == 3);
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------

		void	to_string(xuchars& str, s32 val, s32 base)
		{
			xcuchars format;
			switch (base)
			{
			case 16: format = "%x"; break;
			case 10: format = "%d"; break;
			case 8: format = "%o"; break;
			};
			sprintf(str, format, x_va(val));
		}

		void	to_string(xuchars& str, u32 val, s32 base)
		{
			xcuchars format;
			switch (base)
			{
			case 16: format = "%x"; break;
			case 10: format = "%u"; break;
			case 8: format = "%o"; break;
			};
			sprintf(str, format, x_va(val));
		}

		void	to_string(xuchars& str, s64 val, s32 base)
		{
			xcuchars format;
			switch (base)
			{
			case 16: format = "%x"; break;
			case 10: format = "%d"; break;
			case 8: format = "%o"; break;
			};
			sprintf(str, format, x_va(val));
		}

		void	to_string(xuchars& str, u64 val, s32 base)
		{
			xcuchars format;
			switch (base)
			{
			case 16: format = "%x"; break;
			case 10: format = "%u"; break;
			case 8: format = "%o"; break;
			};
			sprintf(str, format, x_va(val));
		}

		void	to_string(xuchars& str, f32 val, s32 numFractionalDigits)
		{
			rune format_str[] = { '%', '.', '0', '2', 'f', '\0' };
			if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
			{
				format_str[2] = '0' + numFractionalDigits / 10;
				format_str[3] = '0' + numFractionalDigits % 10;
			}
			xcuchars format(format_str);
			sprintf(str, format, x_va(val));
		}

		void	to_string(xuchars& str, f64 val, s32 numFractionalDigits)
		{
			rune format_str[] = { '%', '.', '0', '2', 'f', '\0' };
			if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
			{
				format_str[2] = '0' + numFractionalDigits / 10;
				format_str[3] = '0' + numFractionalDigits % 10;
			}
			xcuchars format(format_str);
			sprintf(str, format, x_va(val));
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------

		bool		is_upper(xcuchars& _str)
		{
			const uchar* str = _str.m_str;
			while (str < _str.m_end)
			{
				uchar32 c = read_char(str);
				if (is_lower(c))
					return false;
			}
			return true;
		}

		bool		is_lower(xcuchars& _str)
		{
			const uchar* str = _str.m_str;
			while (str < _str.m_end)
			{
				uchar32 c = read_char(str);
				if (is_upper(c))
					return false;
			}
			return true;
		}

		bool		is_capitalized(xcuchars& _str)
		{
			bool b = true;
			const uchar* str = _str.m_str;
			while (str < _str.m_end)
			{
				uchar32 c = read_char(str);
				if (is_upper(c) != b)
					return false;
				b = (c == ' ');
			}
			return true;

		}

		bool		is_delimited(xcuchars& _str, rune delimit_left, rune delimit_right)
		{
			const uchar* str = _str.m_str;
			if (str < _str.m_end)
			{
				uchar32 char_left = read_char(str);
				if (char_left == delimit_left)
				{
					uchar32 char_right = 0;
					while (str < _str.m_end)
					{
						char_right = read_char(str);
					}
					return char_right == delimit_right;
				}
			}
			return false;
		}

		/**
		* Assumption: Letters A-Z and a-z are contiguous in the character set.
		* This is true for ASCII and UniCode.  (Not so for EBCDIC!)
		*/
		void to_upper(xuchars& _str)
		{
			ASSERT(str != NULL);
			prune p = _str.m_str;
			while (true)
			{
				if (p == _str.m_end)
					break;
				uchar32 c = peek_char(p);
				if ((c >= 'a') && (c <= 'z'))
				{
					c += ('A' - 'a');
					p = write_char(c, p, p + 4);
				}
				else 
				{
					read_char(p);
				}
			}
		}

		/**
		* Assumption: Letters A-Z and a-z are contiguous in the character set.
		* This is true for ASCII and UniCode.  (Not so for EBCDIC!)
		*/
		void to_lower(xuchars& _str)
		{
			ASSERT(str != NULL);
			prune p = _str.m_str;
			while (true)
			{
				if (p == _str.m_end)
					break;
				uchar32 c = peek_char(p);
				if ((c >= 'A') && (c <= 'Z'))
				{
					c = 'a' + (c - 'A');
					p = write_char(c, p, _str.m_end);
				}
				else 
				{
					read_char(p);
				}
			}
		}

		// ------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------

		bool		starts_with(xcuchars& str, uchar32 start_char)
		{
			uchar32 c = read_char(str.m_str);
			return start_char == c;
		}

		bool		starts_with(xcuchars& _str, xcuchars& start)
		{
			// Match from begin of string
			const uchar* str = _str.m_str;
			const uchar* str_end = _str.m_end;
			const uchar* start_str = start.m_str;
			const uchar* start_str_end = start.m_end;
			while (true)
			{
				if (start_str == start_str_end)
					break;
				uchar32 c1 = read_char(start_str);
				if (str == str_end)
					return false;
				uchar32 c2 = read_char(str);
				if (c1 != c2)
					return false;
			}
			// If matched all the way to the end of start then success
			return true;
		}

		bool		ends_with(xcuchars& _str, uchar32 end_char)
		{
			if (_str.m_end != NULL && has_fixed_size_rune())
			{
				uchar32 c;
				c = peek_char(_str.m_end - get_fixed_sizeof_rune());
				return end_char == c;
			}
			else
			{
				// Need to fast-forward to the last character
				const uchar* str = _str.m_str;
				const uchar* str_end = _str.m_end;
				
				uchar32 p, c;
				c = 0xffffffff;
				if (str_end != NULL)
				{
					while (true)
					{
						p = c;
						if (str_end != NULL && str == str_end)
							break;
						c = read_char(str);
					}
				}
				else
				{
					while (true)
					{
						p = c;
						c = read_char(str);
						if (c == '\0')
							break;
					}
				}
				return end_char == p;
			}
		}

		bool		ends_with(xcuchars& _str, xcuchars& _end)
		{
			if (has_fixed_size_rune())
			{
				// ASCII and UTF32 only
				pcrune srcstr = _str.m_str;
				pcrune srcptr = _str.m_str - get_fixed_sizeof_rune();
				pcrune endstr = _end.m_str;
				pcrune endptr = _end.m_end - get_fixed_sizeof_rune();
				while (true)
				{
					uchar32 c1 = peek_char(srcptr);
					uchar32 c2 = peek_char(endptr);
					if (c1 != c2)
						return false;
					if (srcptr == srcstr)
						return true;
					if (endptr == endstr)
						return true;
					srcptr -= get_fixed_sizeof_rune();
					endptr -= get_fixed_sizeof_rune();
				}
			}
			else
			{
				pcrune srcstr = _str.m_str;
				pcrune srcstr_end = _str.m_str - get_fixed_sizeof_rune();
				pcrune endstr = _end.m_str;
				pcrune endstr_end = _end.m_end;

				// First count the number of characters in 'endstr'
				s32 endstr_len = endstr_end - endstr;
				// Count the number of characters in 'srcstr'
				s32 srcstr_len = srcstr_end - srcstr;

				// Now we know where to start in 'srcstr'
				s32 srcstr_start = srcstr_len - endstr_len;
				if (srcstr_start >= 0)
				{
					// Move to start position in srcstr
					for (s32 i = 0; i < srcstr_start; ++i)
						read_char(srcstr);

					uchar32 sc = read_char(srcstr);
					uchar32 ec = read_char(endstr);
					while (sc == ec)
					{
						if (endstr == endstr_end)
							return true;
						if (srcstr == srcstr_end)
							return false;
						sc = read_char(srcstr);
						ec = read_char(endstr);
					}
				}
			}
			return false;
		}

		uchar32			first_char(xcuchars& str)
		{
			if (str.m_str == str.m_end)
				return '\0';
			uchar32 c = peek_char(str.m_str);
			return c;
		}

		uchar32			last_char(xcuchars& str)
		{
			if (has_fixed_size_rune())
			{
				if (str.m_end != NULL && str.m_end > str.m_str)
				{
					// @ASCII only
					return peek_char(str.m_end - get_fixed_sizeof_rune());
				}
				return '\0';
			}
			else
			{
				// Fast-forward to end of str, keep track of previous character
				uchar32 c, pc;
				pcrune iter = str.m_str;
				c = 0;
				while (true)
				{
					pc = c;
					if (str.m_end != NULL && iter == str.m_end)
						break;
					c = read_char(iter);
					if (str.m_end == NULL && c == '\0')
						break;
				}
				return pc;
			}
		}
	};	///< END ascii namespace
};	///< END xcore namespace

