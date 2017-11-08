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
			if (has_fixed_size_rune())
			{
				rune c = *str;
				if (len != NULL)
					*len = get_fixed_sizeof_rune();
				return c;
			}
			else
			{
				rune c = *str;
				if ((c & 0x80) == 0x00)
				{
					if (len != NULL)
						*len = 1;
					return (uchar32)c;
				}

				s32 l = 0;
				if ((c & 0xe0) == 0xc0) { l = 2; }
				else if ((c & 0xf0) == 0xe0) { l = 3; }
				else if ((c & 0xf8) == 0xf0) { l = 4; }

				uchar32 c32 = 0;
				for (s32 i = 0; i<l; i++)
				{
					c = str[i];
					c32 = c32 << 8;
					c32 = c32 | c;
				}

				if (len != NULL)
					*len = l;

				return c32;
			}
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
		static prune			write_char(uchar32 c, prune str, pcrune end, EWriteMode write_mode)
		{
			if (sizeof(rune) == 4 && write_mode == WRITEMODE_UTF32)
			{
				if (str != NULL)
					*str++ = c;
				return str;
			}
			else if (sizeof(rune) == 1 && write_mode == WRITEMODE_ASCII)
			{
				if (str != NULL && str < end)
				{
					if (c > 0x7f)
						c = '?';
					*str++ = c;
				}
				return str;
			}

			// UTF8

			s32 len = 0;
			if (c <= 0x7f) { len = 1; }
			else if (c < 0x0800) { len = 2; }
			else if (c < 0xd800) { len = 3; }
			else if (c < 0xe000) { len = 0; }
			else if (c < 0x010000) { len = 3; }
			else if (c < 0x110000) { len = 4; }

			if (str != NULL && len > 0)
			{
				if ((str + len) <= end)
				{
					u32 const mask = 0x0F0E0C00;
					switch (len)
					{
					case 4: str[3] = (c & 0x3f) | 0x80; c = c >> 6;
					case 3: str[2] = (c & 0x3f) | 0x80; c = c >> 6;
					case 2: str[1] = (c & 0x3f) | 0x80; c = c >> 6;
					case 1: str[0] = (rune)c | (sUTF8LC[len]);
					};

					str += len;
				}
			}
			return str;
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------

		pcrune len(pcrune str)
		{
			ASSERT(str != NULL);
			while (true)
			{
				pcrune end = str;
				uchar32 c = read_char(str);
				if (c == '\0')
				{
					str = end;
					break;
				}
			}
			return str;
		}

		pcrune len(pcrune str, s32* str_len)
		{
			ASSERT(str != NULL);

			s32 len = 0;
			while (true)
			{
				pcrune end = str;
				uchar32 c = read_char(str);
				if (c == 0)
				{
					str = end;
					break;
				}
				len++;
			}
			if (str_len != NULL)
				*str_len = len;
			return str;
		}

		pcrune len(pcrune str, pcrune str_eos, s32* str_len)
		{
			ASSERT(str != NULL);

			s32 len = 0;
			while (str < str_eos)
			{
				uchar32 c = read_char(str);
				if (c == 0)
					break;
				len++;
			}
			if (str_len != NULL)
				*str_len = len;
			return str;
		}

		s32	 size(pcrune str)
		{
			s32 l = 0;
			pcrune end = len(str, &l);
			return l;
		}


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

		prune		copy(prune dest, pcrune dest_end, pcrune src, pcrune src_end, ECopyType type)
		{
			ASSERT(dest != NULL && dest_end != NULL);
			ASSERT(src != NULL);

			s32 len = 0;
			prune dst = dest;
			while (dst < dest_end)
			{
				if (src_end != NULL && src == src_end)
				{
					if (type == COPY_AND_WRITE_MATCHING_TERMINATOR && *src_end == '\0')
						write_char('\0', dst, dest_end, get_writemode());
					break;
				}
				uchar32 const c = read_char(src);
				if (src_end == NULL && c == '\0')
				{
					if (type == COPY_AND_WRITE_MATCHING_TERMINATOR)
						write_char(c, dst, dest_end, get_writemode());
					break;
				}
				dst = write_char(c, dst, dest_end, get_writemode());
			}

			if (dst < dest_end && type == COPY_AND_WRITE_TERMINATOR)
				write_char('\0', dst, dest_end, get_writemode());

			return dst;
		}

		pcrune		find(pcrune str_begin, pcrune str_end, pcrune find_begin, pcrune find_end, ECmpMode mode)
		{
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

		pcrune		find(pcrune str, pcrune str_end, uchar32 find_char, ECmpMode mode)
		{
			uchar32 c = 0;
			if (mode == CASE_SENSITIVE)
			{
				while (true)
				{
					if (str_end != NULL && (str == str_end))
						break;
					pcrune cur_pos = str;
					c = read_char(str);
					if (str_end == NULL && (c == '\0'))
						break;
					if (c == find_char)
						return cur_pos;
				};
			}
			else
			{
				while (true)
				{
					if (str_end != NULL && (str == str_end))
						break;
					pcrune cur_pos = str;
					c = read_char(str);
					if (str_end == NULL && (c == '\0'))
						break;
					if (is_equal(c, find_char, mode))
						return cur_pos;
				};
			}
			return NULL;
		}

		pcrune		find_one_of(pcrune str, pcrune str_end, pcrune charset, pcrune charset_end, ECmpMode mode)
		{
			pcrune cur_pos = str;
			while (true)
			{
				if (str_end != NULL && cur_pos == str_end)
					break;
				uchar32 c = peek_char(cur_pos);
				if (str_end == NULL && c == '\0')
					break;
				if (find(charset, charset_end, c, mode) != NULL)
					return cur_pos;
				cur_pos++;
			}
			return NULL;
		}

		prune	replace(prune str_begin, pcrune str_end, pcrune str_eos, pcrune replace_str, pcrune replace_end)
		{
			// The logic here is based on memory copy, we do not consider characters
			s32 const selected_len = (s32)((xbyte const*)str_end - (xbyte const*)str_begin);
			s32 const replace_len = (s32)((xbyte const*)replace_end - (xbyte const*)replace_str);

			prune end = NULL;
			if (selected_len < replace_len)
			{
				// Move, increasing
				s32 move_len = replace_len - selected_len;
				if (move_len >((xbyte const*)str_eos - (xbyte*)str_end))
					move_len = (s32)(((xbyte const*)str_eos - (xbyte*)str_end));

				// No matter what, push out anything at the end!
				xbyte * dst = (xbyte*)((xbyte*)str_end + ((xbyte const*)str_eos - (xbyte*)str_end) - 1);
				xbyte * src = (xbyte*)((xbyte*)dst - move_len);
				while (dst > (xbyte*)str_end)
					*dst-- = *src--;

				end = (prune)(str_begin + selected_len + move_len);		// Update str_end
			}
			else if (selected_len > replace_len)
			{
				// Move, reducing
				s32 move_len = selected_len - replace_len;
				xbyte* dst = (xbyte*)((xbyte*)str_end - move_len);
				xbyte* src = (xbyte*)(str_end);
				while (src < (xbyte const*)str_eos)
					*dst++ = *src++;

				end = (prune)(str_begin + selected_len - move_len);		// Update str_end
			}
			else
			{
				end = (prune)(str_begin + selected_len);
			}

			// Replace
			xbyte const* src = (xbyte const*)replace_str;
			xbyte const* src_end = (xbyte const*)replace_str + replace_len;
			xbyte* dst = (xbyte*)str_begin;
			while (src < src_end)
				*dst++ = *src++;

			return end;
		}


		s32	compare(pcrune lstr, pcrune lstr_end, pcrune rstr, pcrune rstr_end, ECmpMode mode)
		{
			ASSERT(lstr);
			ASSERT(rstr);

			if (lstr != rstr)
			{
				do
				{
					bool const is_lstr_end = (lstr == lstr_end) || (lstr_end == NULL && *lstr == '\0');
					bool const is_rstr_end = (rstr == rstr_end) || (rstr_end == NULL && *rstr == '\0');
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


		s32	compare(pcrune lstr, pcrune rstr, ECmpMode mode)
		{
			return compare(lstr, NULL, rstr, NULL, mode);
		}

		//------------------------------------------------------------------------------
		prune concatenate(prune front, pcrune front_end, pcrune front_eos, pcrune back, pcrune back_end)
		{
			ASSERT(front != NULL);
			ASSERT(front_end != NULL);
			ASSERT(back != NULL);

			s32 l = 0;
			prune wptr = pos(front, front_end);
			while (wptr < front_eos)
			{
				if (back_end != NULL && back == back_end)
					break;
				front = wptr;
				uchar32 c = read_char(back);
				prune nwptr = write_char(c, wptr, front_eos, get_writemode());
				if (nwptr == wptr)
					break;
				wptr = nwptr;
				if (back_end == NULL && c == 0)
					break;
			}
			return front;
		}

		s32		parse(pcrune str, pcrune str_end, bool& value)
		{
			rune const format_str[] = { '%', 'b', '\0' };
			s32 s = sscanf(str, str_end, format_str, NULL, x_va_r(&value));
			return s;
		}

		s32		parse(pcrune str, pcrune str_end, s32& value, s32 base)
		{
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			s32 s = sscanf(str, str_end, format_str, NULL, x_va_r(&value));
			return s;
		}

		s32		parse(pcrune str, pcrune str_end, u32& value, s32 base)
		{
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			s32 s = sscanf(str, str_end, format_str, NULL, x_va_r(&value));
			return s;
		}

		s32		parse(pcrune str, pcrune str_end, s64& value, s32 base)
		{
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			s32 s = sscanf(str, str_end, format_str, NULL, x_va_r(&value));
			return s;
		}

		s32		parse(pcrune str, pcrune str_end, u64& value, s32 base)
		{
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			s32 s = sscanf(str, str_end, format_str, NULL, x_va_r(&value));
			return s;
		}

		s32		parse(pcrune str, pcrune str_end, f32& value)
		{
			rune format_str[] = { '%', 'f', '\0' };
			s32 s = sscanf(str, str_end, format_str, NULL, x_va_r(&value));
			return s;
		}

		s32		parse(pcrune str, pcrune str_end, f64& value)
		{
			rune format_str[] = { '%', 'f', '\0' };
			s32 s = sscanf(str, str_end, format_str, NULL, x_va_r(&value));
			return s;
		}

		//------------------------------------------------------------------------------
		bool is_decimal(pcrune str, pcrune str_end)
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
		bool is_hexadecimal(pcrune str, pcrune str_end, bool with_prefix)
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
		bool is_float(pcrune str, pcrune str_end)
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
		bool is_GUID(pcrune str, pcrune str_end)
		{
			// Does it have only 0-9, a-f, A-F characters
			s32 n = 0;
			s32 l = 0;
			pcrune src = str;
			for (; src<str_end; n++)
			{
				uchar32 c = read_char(src);
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

		prune	to_string(prune str, prune str_end, pcrune str_eos, s32 val, s32 base)
		{
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			pcrune format_str_end = format_str + 2 * get_fixed_sizeof_rune();
			s32 len = vsprintf(str, str_eos, (pcrune)format_str, format_str_end, x_va(val));
			return str + len;
		}

		prune	to_string(prune str, prune str_end, pcrune str_eos, u32 val, s32 base)
		{
			rune format_str[] = { '%', 'u', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'u'; break;
			case 8: format_str[1] = 'o'; break;
			};
			pcrune format_str_end = format_str + 2 * get_fixed_sizeof_rune();
			s32 len = sprintf(str, str_eos, format_str, format_str_end, x_va(val));
			return str + len;
		}

		prune	to_string(prune str, prune str_end, pcrune str_eos, s64 val, s32 base)
		{
			rune format_str[] = { '%', 'd', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'd'; break;
			case 8: format_str[1] = 'o'; break;
			};
			pcrune format_str_end = format_str + 2 * get_fixed_sizeof_rune();
			s32 len = sprintf(str, str_eos, format_str, format_str_end, x_va(val));
			return str + len;
		}

		prune	to_string(prune str, prune str_end, pcrune str_eos, u64 val, s32 base)
		{
			rune format_str[] = { '%', 'u', '\0' };
			switch (base)
			{
			case 16: format_str[1] = 'x'; break;
			case 10: format_str[1] = 'u'; break;
			case 8: format_str[1] = 'o'; break;
			};
			pcrune format_str_end = format_str + sizeof(format_str);
			s32 len = sprintf(str, str_eos, format_str, format_str_end, x_va(val));
			return str + len;
		}

		prune	to_string(prune str, prune str_end, pcrune str_eos, f32 val, s32 numFractionalDigits)
		{
			rune format_str[] = { '%', 'f', '\0' };
			pcrune format_str_end = format_str + 2 * get_fixed_sizeof_rune();
			s32 len = sprintf(str, str_eos, format_str, format_str_end, x_va(val));
			return str + len;
		}

		prune	to_string(prune str, prune str_end, pcrune str_eos, f64 val, s32 numFractionalDigits)
		{
			rune format_str[] = { '%', 'f', '\0' };
			pcrune format_str_end = format_str + 2 * get_fixed_sizeof_rune();
			s32 len = sprintf(str, str_eos, format_str, format_str_end, x_va(val));
			return str + len;
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------

		bool		is_upper(pcrune str, pcrune str_end)
		{
			if (str_end != NULL)
			{
				while (str < str_end)
				{
					uchar32 c = read_char(str);
					if (is_lower(c))
						return false;
				}
			}
			else
			{
				while (true)
				{
					uchar32 c = read_char(str);
					if (c == '\0')
						break;
					if (is_lower(c))
						return false;
				}
			}
			return true;
		}

		bool		is_lower(pcrune str, pcrune str_end)
		{
			if (str_end != NULL)
			{
				while (str < str_end)
				{
					uchar32 c = read_char(str);
					if (is_upper(c))
						return false;
				}
			}
			else
			{
				while (true)
				{
					uchar32 c = read_char(str);
					if (c == '\0')
						break;
					if (is_upper(c))
						return false;
				}
			}
			return true;
		}

		bool		is_capitalized(pcrune str, pcrune str_end)
		{
			bool b = true;
			if (str_end != NULL)
			{
				while (str < str_end)
				{
					uchar32 c = read_char(str);
					if (is_upper(c) == b)
						return false;
					b = false;
				}
			}
			else
			{
				while (true)
				{
					uchar32 c = read_char(str);
					if (c == '\0')
						break;
					if (is_upper(c) == b)
						return false;
					b = false;
				}
			}
			return true;

		}

		bool		is_delimited(pcrune str, pcrune end, rune delimit_left, rune delimit_right)
		{
			if (end != NULL)
			{
				if (str < end)
				{
					uchar32 char_left = read_char(str);
					if (char_left == delimit_left)
					{
						uchar32 char_right;
						while (str < end)
						{
							char_right = read_char(str);
						}
						return char_right == delimit_right;
					}
				}
			}
			else
			{
				uchar32 char_left = read_char(str);
				if (char_left == delimit_left)
				{
					uchar32 char_right;
					while (true)
					{
						uchar32 c = read_char(str);
						if (c == '\0')
							break;
						char_right = c;
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
		prune to_upper(prune str, pcrune str_end)
		{
			ASSERT(str != NULL);
			prune p = str;
			while (true)
			{
				if (p == str_end)
					break;
				uchar32 c = peek_char(p);
				if ((c >= 'a') && (c <= 'z'))
				{
					c += ('A' - 'a');
					p = write_char(c, p, p + 4, get_writemode());
				}
				else 
				{
					read_char(p);
				}
			}
			return str;
		}

		/**
		* Assumption: Letters A-Z and a-z are contiguous in the character set.
		* This is true for ASCII and UniCode.  (Not so for EBCDIC!)
		*/
		prune to_lower(prune str, pcrune str_end)
		{
			ASSERT(str != NULL);
			prune p = str;
			while (true)
			{
				if (p == str_end)
					break;
				uchar32 c = peek_char(p);
				if ((c >= 'A') && (c <= 'Z'))
				{
					c = 'a' + (c - 'A');
					p = write_char(c, p, str_end, get_writemode());
				}
				else 
				{
					read_char(p);
				}
			}
			return str;
		}

		/**
		*------------------------------------------------------------------------------
		* Author:
		*     Virtuos Games
		* Summary:
		*     strCRC - calculate and return crc of a string
		* Arguments:
		*        str     - string to calculate
		*        crcSum  - provided to affect crc calculate
		* Returns:
		*        32 bit crc of the string
		* See Also:
		*     memCrc
		*------------------------------------------------------------------------------
		*/

		bool		starts_with(pcrune str, pcrune str_end, uchar32 start_char)
		{
			uchar32 c = read_char(str);
			return start_char == c;
		}

		bool		starts_with(pcrune str, pcrune str_end, pcrune start_str, pcrune start_str_end)
		{
			// Match from begin of string
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
			// If matched all the way to the end of inStartStr then success
			return true;
		}

		bool		ends_with(pcrune str, pcrune str_end, uchar32 end_char)
		{
			if (str == NULL)
				return false;

			if (str_end != NULL && has_fixed_size_rune())
			{
				uchar32 c;
				c = peek_char(str_end - get_fixed_sizeof_rune());
				return end_char == c;
			}
			else
			{
				// Need to fast-forward to the last character
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

		bool		ends_with(pcrune srcstr, pcrune srcstr_end, pcrune endstr, pcrune endstr_end)
		{
			if (has_fixed_size_rune())
			{
				// ASCII and UTF32 only
				pcrune srcptr = srcstr_end - get_fixed_sizeof_rune();
				pcrune endptr = endstr_end - get_fixed_sizeof_rune();
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
				// First count the number of characters in 'enstr'
				s32 endstr_len = 0;
				endstr_end = len(endstr, endstr_end, &endstr_len);
				// Count the number of characters in 'srcstr'
				s32 srcstr_len;
				srcstr_end = len(srcstr, srcstr_end, &srcstr_len);

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
						if (srcstr == srcstr_end || endstr == endstr_end)
							return true;
						sc = read_char(srcstr);
						ec = read_char(endstr);
					}
				}
			}
			return false;
		}

		uchar32			first_char(pcrune str, pcrune str_end)
		{
			ASSERT(str != NULL);
			ASSERT(str_end != NULL);
			if (str_end != NULL && str == str_end)
				return '\0';
			uchar32 c = peek_char(str);
			return c;
		}

		uchar32			last_char(pcrune str, pcrune str_end)
		{
			ASSERT(str != NULL);

			if (has_fixed_size_rune())
			{
				if (str_end != NULL && str_end > str)
				{
					// @ASCII only
					return peek_char(str_end - get_fixed_sizeof_rune());
				}
				else if (str_end == NULL)
				{
					// Fast-forward to end of str
					pcrune str_end = len(str);
					return peek_char(str_end - get_fixed_sizeof_rune());
				}

				return '\0';
			}
			else
			{
				// Fast-forward to end of str, keep track of previous character
				uchar32 c, pc;
				pcrune iter = str;
				c = 0;
				while (true)
				{
					pc = c;
					if (str_end != NULL && iter == str_end)
						break;
					c = read_char(iter);
					if (str_end == NULL && c == '\0')
						break;
				}
				return pc;
			}
		}
	};	///< END ascii namespace
};	///< END xcore namespace

