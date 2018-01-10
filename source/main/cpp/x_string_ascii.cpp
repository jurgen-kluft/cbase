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
		static inline s32		peek_char(xuchars const& src, uchar32& out_c)
		{
			out_c = '\0';
			if (!src.is_empty())
			{
				out_c = src.m_str[0];
				return 1;
			}
			return 0;
		}
		static inline s32		peek_char(xcuchars const& src, uchar32& out_c)
		{
			out_c = '\0';
			if (!src.is_empty())
			{
				out_c = src.m_str[0];
				return 1;
			}
			return 0;
		}

		//------------------------------------------------------------------------------
		static bool				write_char(uchar32 c, xuchars& dst)
		{
			if (!dst.is_full())
			{
				if (c > 0x7f)
					c = '?';
				*dst.m_end++ = (uchar)c;
				*dst.m_end = '\0';
				return true;
			}
			return false;
		}

		//------------------------------------------------------------------------------
		static bool				read_char(runes& src, uchar32& out_c)
		{
			s32 const l = peek_char(src, out_c);
			src.m_str += l;
			return l > 0;
		}

		//------------------------------------------------------------------------------
		static bool				read_char(crunes& src, uchar32& out_c)
		{
			s32 l = peek_char(src, out_c);
			src.m_str += l;
			return l > 0;
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		s32		len(prune str, prune end)
		{
			s32 l = 0;
			while (*str != '\0' && (end == NULL || str < end))
			{
				str++;
				l++;
			}
			return l;
		}

		s32		len(pcrune str, pcrune end)
		{
			s32 l = 0;
			while (*str != '\0' && (end == NULL || str < end))
			{
				str++;
				l++;
			}
			return l;
		}

		prune			endof(prune str, prune end)
		{
			while (*str != '\0' && (end == NULL || str < end))
				str++;
			return str;
		}

		pcrune			endof(pcrune str, pcrune end)
		{
			while (*str != '\0' && (end == NULL || str < end))
				str++;
			return str;
		}


		void		copy(runes& _dest, crunes const& _src, ECopyType type)
		{
			crunes src = _src;
			_dest.m_end = _dest.m_str;
			while (_dest.m_end < _dest.m_eos)
			{
				uchar32 c;
				if (read_char(src, c)==false)
					break;
				write_char(c, _dest);
			}
		}

		void			concatenate(runes& str, crunes const& _concat)
		{
			crunes concat = _concat;
			uchar32 c;
			while (true)
			{
				if (!read_char(concat, c))
					break;
				if (!write_char(c, str))
					break;
			}
		}

		runes			find(runes const& _str, crunes const& _find, ECmpMode mode)
		{
			if (_str.is_empty())
				return runes();
			if (_find.is_empty())
				return runes();

			runes str_iter = _str;
			
			bool found = false;
			runes str_found;
			while (str_iter.is_empty() == false)
			{
				// Remember the current position because here is
				// where we start to check if the 'find' string
				// can be found at this particular position.
				str_found.clone(str_iter);

				uchar32 sc, fc;

				runes str_find(str_iter);
				crunes find_str = _find;

				found = true;
				while (found && read_char(find_str, fc))
				{
					if (!read_char(str_find, sc))
					{
						found = false;
						break;
					}
					found = is_equal(sc, fc, mode);
				}

				if (found)
				{
					str_found.m_end = str_find.m_str;
					break;
				}

				if (!read_char(str_iter, sc))
					break;
			}
			return found ? str_found : runes();
		}

		crunes			find(crunes const& _str, crunes const& _find, ECmpMode mode)
		{
			if (_str.is_empty())
				return crunes();
			if (_find.is_empty())
				return crunes();

			crunes str_iter = _str;

			bool found = false;
			crunes str_found;
			while (str_iter.is_empty() == false)
			{
				// Remember the current position because here is
				// where we start to check if the 'find' string
				// can be found at this particular position.
				str_found = str_iter;

				uchar32 sc, fc;

				crunes str_find = str_iter;
				crunes find_str = _find;

				found = true;
				while (found && read_char(find_str, fc))
				{
					if (!read_char(str_find, sc))
					{
						found = false;
						break;
					}
					found = is_equal(sc, fc, mode);
				}

				if (found)
				{
					str_found.m_end = str_find.m_str;
					break;
				}

				if (!read_char(str_iter, sc))
					break;
			}
			return found ? str_found : crunes();
		}

		runes	find(runes const& _str, uchar32 _c, ECmpMode mode)
		{
			runes str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				runes pos = str;
				if (!read_char(str, c))
					break;
				if (is_equal(c, _c, mode))
					return pos;
			}
			return runes();
		}

		crunes	find(crunes const& _str, uchar32 _c, ECmpMode mode)
		{
			crunes str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				crunes pos = str;
				if (!read_char(str, c))
					break;
				if (is_equal(c, _c, mode))
					return pos;
			}
			return crunes();
		}

		bool	contains(crunes const& _str, uchar32 _c, ECmpMode mode)
		{
			crunes f = find(_str, _c, mode);
			return f.is_empty() == false;
		}

		bool	contains(runes const& _str, uchar32 _c, ECmpMode mode)
		{
			runes f = find(_str, _c, mode);
			return f.is_empty() == false;
		}

		runes	find_one_of(runes const& _str, crunes const& _set, ECmpMode mode)
		{
			runes str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				// Remember the current position so that if we found
				// the char in the set we can return this position.
				runes pos = str;

				// Read next character from 'str'
				if (!read_char(str, c))
					break;

				// See if this char can be found in the 'set'
				crunes found = find(_set, c, mode);
				if (!found.is_empty())
					return pos;
			}
			// Nothing found, return an empty object
			return runes();
		}

		crunes	find_one_of(crunes const& _str, crunes const& _set, ECmpMode mode)
		{
			crunes str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				// Remember the current position so that if we found
				// the char in the set we can return this position.
				crunes pos = str;

				// Read next character from 'str'
				if (!read_char(str, c))
					break;

				// See if this char can be found in the 'set'
				crunes found = find(_set, c, mode);
				if (!found.is_empty())
					return pos;
			}
			// Nothing found, return an empty object
			return crunes();
		}


		void	replace(runes& _str, crunes const& _find, crunes const& _replace)
		{
			// search the 'find' string
			runes found = find(_str, _find);
			if (found.is_empty())
				return;

			// The logic here is based on memory copy, we do not consider characters
			s32 const selected_len = (s32)((xbyte const*)found.m_end - (xbyte const*)found.m_str);
			s32 const replace_len = (s32)((xbyte const*)_replace.m_end - (xbyte const*)_replace.m_str);

			prune end = NULL;
			if (selected_len < replace_len)
			{
				// Move, expanding
				s32 move_len = replace_len - selected_len;
				if (move_len >((xbyte const*)found.m_eos - (xbyte*)found.m_end))
					move_len = (s32)(((xbyte const*)found.m_eos - (xbyte*)found.m_end));

				// No matter what, push out anything at the end!
				xbyte * dst = (xbyte*)((xbyte*)found.m_end + ((xbyte const*)found.m_eos - (xbyte*)found.m_end) - 1);
				xbyte * src = (xbyte*)((xbyte*)dst - move_len);
				while (dst > (xbyte*)found.m_end)
					*dst-- = *src--;
				end = (prune)((xbyte*)_str.m_end + move_len);		// Update str_end
			}
			else if (selected_len > replace_len)
			{
				// Move, shrinking
				s32 move_len = selected_len - replace_len;
				xbyte* dst = (xbyte*)((xbyte*)found.m_end - move_len);
				xbyte* src = (xbyte*)(found.m_end);
				while (src < (xbyte const*)found.m_eos)
					*dst++ = *src++;
				end = (prune)((xbyte*)_str.m_end - move_len);		// Update str_end
			}
			else
			{
				end = (prune)(_str.m_end);
			}

			// Replace
			xbyte const* src = (xbyte const*)_replace.m_str;
			xbyte const* src_end = (xbyte const*)_replace.m_str + replace_len;
			xbyte* dst = (xbyte*)found.m_str;
			while (src < src_end)
				*dst++ = *src++;

			_str.m_end = end;
		}

		s32	compare(crunes const& _lstr, crunes const& _rstr, ECmpMode mode)
		{
			crunes lstr = _lstr;
			crunes rstr = _rstr;
			uchar32 lc=0, rc=0;
			while (true)
			{ 
				bool const lok = read_char(lstr, lc);
				bool const rok = read_char(rstr, rc);
				if (!lok && !rok)
					return 0;
				if (!lok)
					return -1;
				if (!rok)
					return 1;
				if (!is_equal(lc, rc, mode))
					break;
			}
			if (lc < rc)
				return -1;
			else if (lc > rc)
				return 1;
			return 0;
		}

		//------------------------------------------------------------------------------
		crunes	parse(crunes const& _str, bool& value)
		{
			crunes str = _str;
			rune format_str[] = { '%', 'b', '\0' };
			crunes format(format_str);
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes	parse(crunes const& _str, s32& value, s32 base)
		{
			crunes str = _str;
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str);
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes	parse(crunes const& _str, u32& value, s32 base)
		{
			crunes str = _str;
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str);
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes	parse(crunes const& _str, s64& value, s32 base)
		{
			crunes str = _str;
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str);
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes	parse(crunes const& _str, u64& value, s32 base)
		{
			crunes str = _str;
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str);
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes	parse(crunes const& _str, f32& value)
		{
			crunes str = _str;
			rune format_str[] = { '%', 'f', '\0' };
			crunes format(format_str);
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		crunes	parse(crunes const& _str, f64& value)
		{
			crunes str = _str;
			rune format_str[] = { '%', 'f', '\0' };
			crunes format(format_str);
			sscanf(str, format, x_va_r(&value));
			return str;
		}

		//------------------------------------------------------------------------------
		bool is_decimal(crunes const& _str)
		{
			crunes str = _str;
			uchar32 c;
			while (peek_char(str, c))
			{
				if (c != '-')
					break;
				read_char(str, c);
			}
			while (read_char(str, c))
			{
				if (c < '0' || c > '9')
					return false;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool is_hexadecimal(crunes const& _str, bool with_prefix)
		{
			crunes str = _str;

			uchar32 p,c;
			s32 pos = 0;
			while (read_char(str, c)) {
				if (c == 'x') {
					if (pos == 1 && with_prefix && p == '0') {
						// Ok, 0x.... prefix
					} else {
						return false;
					}
				} else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
					// Ok, digit or hex character
					p = c;
				} else {
					return false;
				}
				pos++;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool is_float(crunes const& _str)
		{
			rune f32chars_str[] = { 'E','e','.','#','Q','N','A','B','I','F', '\0' };
			crunes f32chars(f32chars_str);

			crunes str = _str;
			uchar32 c;
			while (read_char(str, c)) {
				if (c >= '0' && c <= '9')
					continue;
				if (!contains(f32chars, c, CASE_SENSITIVE))
					return false;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool is_GUID(crunes const& _str)
		{
			rune f32chars_str[] = { 'E','e','.','#','Q','N','A','B','I','F', '\0' };
			crunes f32chars(f32chars_str);

			crunes str = _str;

			// Does it have only 0-9, a-f, A-F characters
			s32 n = 0;
			s32 l = 0;
			uchar32 c;
			while (read_char(str, c)) {
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
			}
			// the length of the string must be 17 characters long (4*8 + 3 ':')
			return (n == (4 * 8) && l == 3);
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------

		void	to_string(runes& str, s32 val, s32 base)
		{
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str);
			sprintf(str, format, x_va(val));
		}

		void	to_string(runes& str, u32 val, s32 base)
		{
			rune format_str[] = { '%', 'u', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'u'; break;
			case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str);
			sprintf(str, format, x_va(val));
		}

		// *cFHbRXV[N]WWWR6FETY+

		void	to_string(runes& str, s64 val, s32 base)
		{
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str);
			sprintf(str, format, x_va(val));
		}

		void	to_string(runes& str, u64 val, s32 base)
		{
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			crunes format(format_str);
			sprintf(str, format, x_va(val));
		}

		void	to_string(runes& str, f32 val, s32 numFractionalDigits)
		{
			rune format_str[] = { '%', '.', '0', '2', 'f', '\0' };
			if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
			{
				format_str[2] = '0' + numFractionalDigits / 10;
				format_str[3] = '0' + numFractionalDigits % 10;
			}
			crunes format(format_str);
			sprintf(str, format, x_va(val));
		}

		void	to_string(runes& str, f64 val, s32 numFractionalDigits)
		{
			rune format_str[] = { '%', '.', '0', '2', 'f', '\0' };
			if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
			{
				format_str[2] = '0' + numFractionalDigits / 10;
				format_str[3] = '0' + numFractionalDigits % 10;
			}
			crunes format(format_str);
			sprintf(str, format, x_va(val));
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------

		bool		is_upper(crunes const& _str)
		{
			crunes str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				if (!read_char(str, c))
					break;
				if (is_lower(c))
					return false;
			}
			return true;
		}

		bool		is_lower(crunes const& _str)
		{
			crunes str = _str;
			uchar32 c;
			while (!str.is_empty())
			{
				if (!read_char(str, c))
					break;
				if (is_upper(c))
					return false;
			}
			return true;
		}

		bool		is_capitalized(crunes const& _str)
		{
			crunes str = _str;
			bool b = true;
			while (!str.is_empty())
			{
				uchar32 c;
				if (!read_char(str, c))
					break;
				if (is_upper(c) != b)
					return false;
				b = (c == ' ');
			}
			return true;
		}

		bool		is_delimited(crunes const& _str, rune delimit_left, rune delimit_right)
		{
			crunes str = _str;

			uchar32 c;
			if (read_char(str, c))
			{
				if (c == delimit_left)
				{
					while (!str.is_empty())
					{
						if (!read_char(str, c))
							break;
					}
					return c == delimit_right;
				}
			}
			return false;
		}

		/**
		* Assumption: Letters A-Z and a-z are contiguous in the character set.
		* This is true for ASCII and UniCode.  (Not so for EBCDIC!)
		*/
		void to_upper(runes& _str)
		{
			runes str = _str;
			uchar32 c;
			while (peek_char(str, c))
			{
				if ((c >= 'a') && (c <= 'z'))
				{
					c += ('A' - 'a');
				}
				*str.m_str++ = (uchar)c;
			}
		}

		/**
		* Assumption: Letters A-Z and a-z are contiguous in the character set.
		* This is true for ASCII and UniCode.  (Not so for EBCDIC!)
		*/
		void to_lower(runes& _str)
		{
			runes str = _str;
			uchar32 c;
			while (peek_char(str, c))
			{
				if ((c >= 'A') && (c <= 'Z'))
				{
					c += ('a' - 'A');
				}
				*str.m_str++ = (uchar)c;
			}
		}

		// ------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------

		bool		starts_with(crunes const& _str, uchar32 start_char)
		{
			crunes str = _str;
			uchar32 c = '\0';
			if (read_char(str, c))
				return start_char == c;
			return false;
		}

		bool		starts_with(crunes const& _str, crunes const& _start)
		{
			crunes lstr = _str;
			crunes rstr = _start;
			uchar32 lc, rc;
			while (read_char(rstr, rc))
			{
				if (!read_char(lstr, lc))
					return false;
				if (lc != rc)
					return false;;
			}
			return true;
		}

		bool		ends_with(crunes const& _str, uchar32 end_char)
		{
			crunes str = _str;
			if (!str.is_empty())
			{
				if (has_fixed_size_rune())
					str.m_str = str.m_end - 1;	

				// Need to fast-forward to the last character
				uchar32 p = 0xffffffff;
				uchar32 c = 0xffffffff;
				while (read_char(str, c))
					p = c;
				return end_char == p;
			}
			return false;
		}

		bool		ends_with(crunes const& _str, crunes const& _end)
		{
			crunes str = _str;
			crunes end = _end;
			if (!str.is_empty() && !end.is_empty())
			{
				if (str.size() < end.size())
					return false;

				if (has_fixed_size_rune())
					str.m_str = str.m_end - end.size();

				uchar32 c1, c2;
				while (read_char(str, c1) && read_char(end, c2)) {
					if (c1 != c2)
						return false;
				}
			}
			return str.is_empty() && end.is_empty();
		}

		uchar32			first_char(crunes const& str)
		{
			uchar32 c;
			if (peek_char(str, c))
				return c;
			return '\0';
		}

		uchar32			last_char(crunes const& _str)
		{
			crunes str = _str;
			if (has_fixed_size_rune())
			{
				str.m_str = str.m_end - 1;
			}
			// Fast-forward to end of str, keep track of previous character
			uchar32 c, pc = '\0';
			while (read_char(str, c))
				pc = c;
			return pc;
		}

	};	///< END ascii namespace
};	///< END xcore namespace

