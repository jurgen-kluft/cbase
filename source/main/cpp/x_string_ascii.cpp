#include "xbase\x_debug.h"
#include "xbase\x_integer.h"
#include "xbase\x_string_ascii.h"

#ifdef SPU

	 
namespace xcore
 {
	 namespace ascii
	 {
		 s32		Printf(pcrune formatStr, VA_ARGS_16)
		 {
			 return 0;
		 }
		 s32		Printf(pcrune formatStr, const va_list& args) { return 0; }
		 s32		Printf(pcrune str) { return 0; }
	 }
 };	


#else


//==============================================================================
// INCLUDES
//==============================================================================


/**
 * xCore namespace
 */
namespace xcore
{
	/** 
	 *==============================================================================
	 *==============================================================================
	 * ASCII functions
	 *==============================================================================
	 *==============================================================================
     */

	/**
	 *------------------------------------------------------------------------------
	 *DOM-IGNORE-BEGIN
	 *------------------------------------------------------------------------------
	 */


	//------------------------------------------------------------------------------
	static uchar32			PeekChar(pcrune str, s32* len = NULL)
	{
		rune c = *str;
		if ((c & 0x80) == 0x00) {
			if (len != NULL)
				*len = 1;
			return (uchar32)c;
		}

		s32 l = 0;
		if ((c & 0xe0) == 0xc0) { l = 2; }
		else if ((c & 0xf0) == 0xe0) { l = 3; }
		else if ((c & 0xf8) == 0xf0) { l = 4; }

		uchar32 c32 = 0;
		for (s32 i = 0; i<l; i++) {
			c = str[i];
			c32 = c32 << 8;
			c32 = c32 | c;
		}
		if (len != NULL)
			*len = l;
		return c32;
	}

	//------------------------------------------------------------------------------
	static uchar32			ReadChar(prune& str)
	{
		s32 l = 0;
		uchar32 c = PeekChar(str, &l);
		str += l;
		return c;
	}

	//------------------------------------------------------------------------------
	static uchar32			ReadChar(pcrune& str)
	{
		s32 l = 0;
		uchar32 c = PeekChar(str, &l);
		str += l;
		return c;
	}

	//------------------------------------------------------------------------------
	static u8				sUTF8LC[] = { 0, 0, 0xc0, 0xe0, 0xf0 };
	static s32				WRITEMODE_ASCII = 0;
	static s32				WRITEMODE_UTF8 = 1;
	static s32				WriteChar(uchar32 c, prune& str, pcrune end, s32 write_mode = WRITEMODE_ASCII)
	{
		s32 len;
		if (c <= 0x7f) { len = 1; }
		else if (c < 0x0800) { len = 2; }
		else if (c < 0xd800) { len = 3; }
		else if (c < 0xe000) { len = 0; }
		else if (c < 0x010000) { len = 3; }
		else if (c < 0x110000) { len = 4; }

		if (len == 0)
		{
			return 0;
		}
		if ((str + len) > end)
		{	
			return 0;
		}

		if (str != NULL)
		{
			if (len > 1 && write_mode = WRITEMODE_ASCII)
			{	// Force ASCII character '?'
				c = (rune)'?';
				len = 1;
			}
			u32 mask = 0x0F0E0C00;
			switch (len)
			{
				case 4: str[3] = (c & 0x3f) | 0x80; c = c >> 6;
				case 3: str[2] = (c & 0x3f) | 0x80; c = c >> 6;
				case 2: str[1] = (c & 0x3f) | 0x80; c = c >> 6;
				case 1: str[0] = (rune)c | (0xFF & (mask >> (len*4)));
			};
		}
		return len;
	}

	namespace ucstr
	{
		prune Len(prune str, s32* str_len)
		{
			ASSERT(str != NULL);

			s32 len = 0;
			while (true) 
			{
				uchar32 c = ReadChar(str);
				if (c == 0)
					break;
				len++;
			}
			if (str_len != NULL)
				*str_len = len;
			return str;
		}
		prune Len(prune str, pcrune str_eos, s32* str_len)
		{
			ASSERT(str != NULL);

			s32 len = 0;
			while (str < str_eos) 
			{
				uchar32 c = ReadChar(str);
				if (c == 0)
					break;
				len++;
			}
			if (str_len != NULL)
				*str_len = len;
			return str;
		}

		pcrune Len(pcrune str, s32* str_len)
		{
			ASSERT(str != NULL);

			s32 len = 0;
			while (true) 
			{
				uchar32 c = ReadChar(str);
				if (c == 0)
					break;
				len++;
			}
			if (str_len != NULL)
				*str_len = len;
			return str;
		}
		pcrune Len(pcrune str, pcrune str_eos, s32* str_len)
		{
			ASSERT(str != NULL);

			s32 len = 0;
			while (str < str_eos) 
			{
				uchar32 c = ReadChar(str);
				if (c == 0)
					break;
				len++;
			}
			if (str_len != NULL)
				*str_len = len;
			return str;
		}

		static s32 LenInBytes(uchar32 c)
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

		static s32 LenInChars(pcrune str, pcrune * end)
		{
			return StrLen(str, end);
		}

		prune Copy(prune dest, pcrune dest_end, pcrune src, pcrune src_end)
		{
			ASSERT(dest != NULL && dest_end != NULL);
			ASSERT(src != NULL && src_end != NULL);

			s32 len = 0;
			prune dst = dest;
			while (dst < dest_end)
			{
				if (src == src_end)
					break;
				uchar32 const c = ReadChar(src);
				WriteChar(c, dst, dest_end);
			}
			return dst;
		}
		
		bool		Find(pcrune find, pcrune find_end, pcrune& str_begin, pcrune str_end)
		{
			if (find == find_end)
				return false;
			if (str_begin == str_end)
				return false;
		
			pcrune src = str_begin;
			bool found = false;
			pcrune found_pos;
			while (true)
			{
				found_pos = src;
				if (src == str_end)
					break;

				uchar32 sc = ReadChar(src);
				
				pcrune find_src = src;
				pcrune find_str = find;
				uchar32 fc = ReadChar(find_str):
				bool found = sc == fc;
				while (found)
				{
					if (find_str == find_end)
						break;
					if (find_src == str_end)
					{
						found = false;
						break;
					}

					sc = ReadChar(find_src);
					fc = ReadChar(find_str):
					found = (sc == fc);
				}

				if (found)
				{
					str_begin = found_pos;
					break;
				}
			}
			return found;
		}

		void	Replace(prune str_begin, prune str_end, pcrune str_eos, pcrune replace_str, pcrune replace_end)
		{
			// The logic here is based on memory copy, we do not consider characters
			s32 const selected_len = str_end - str_begin;
			s32 const replace_len = replace_end - replace_str;
			if (selected_len < replace_len)
			{
				// Move, increasing
				s32 move_len = replace_len - selected_len;
				if (move_len > (str_eos - str_end))
					move_len = (str_eos - str_end);

				// No matter what, push out anything at the end!
				rune * dst = str_end + (str_eos - str_end) - 1;
				rune * src = dst - move_len;

				while (dst > str_end)
					*dst-- = *src--;
			}
			else if (selected_len > replace_len)
			{
				// Move, reducing
				s32 move_len = selected_len - replace_len;

				rune * dst = str_end - move_len;
				rune * src = str_end;

				while (src < str_eos)
					*dst++ = *src++;
			}

			// Replace
			pcrune src = replace_str;
			pcrune end = replace_str + replace_len;
			rune* dst = str_begin;
			while (src < end)
				*dst++ = *src++;
		}

		const s32	STRCMPMODE_CASE_SENSITIVE = 0;
		const s32	STRCMPMODE_CASE_IGNORE = 1;

		static s32	Cmp_Internal(pcrune str1, pcrune str1_end, pcrune str2, pcrune str2_end, s32 mode = STRCMPMODE_CASE_SENSITIVE)
		{
			ASSERT(str1);
			ASSERT(str2);

			if (str1 != str2)
			{
				do {
					if (str1 == str1_end)
						return -1;
					if (str2 == str2_end)
						return 1;

					uchar32 c1 = ReadChar(str1);
					uchar32 c2 = ReadChar(str2);
					if (mode == STRCMPMODE_CASE_IGNORE)
					{
						c1 = ToLower(c1);
						c2 = ToLower(c2);
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
		s32	Compare_Case_Sensitive(pcrune str1, pcrune str1_end, pcrune str2, pcrune str2_end)
		{
			return StrCmp_Internal(str1, str1_end, str2, str2_end);
		}

		//------------------------------------------------------------------------------
		s32 Compare_Case_Ignore(pcrune str1, pcrune str1_end, pcrune str2, pcrune str2_end)
		{
			return StrCmp_Internal(str1, str1_end, str2, str2_end, STRCMPMODE_CASE_IGNORE);
		}

		//------------------------------------------------------------------------------
		prune Concatenate(prune front, pcrune front_end, pcrune front_eos, pcrune back, pcrune back_end)
		{
			ASSERT(front != NULL);
			ASSERT(front_end != NULL);
			ASSERT(back != NULL);
			ASSERT(back_end != NULL);
			
			s32 l = 0;
			prune wptr = front + (front_end - front);
			while (wptr < front_eos)
			{
				if (back == back_end)
					break;
				uchar32 c = ReadChar(back);
				if (WriteChar(c, wptr, front_eos, WRITEMODE_ASCII) == 0)
					break;
			}
			return front;
		}

		pcrune	ToValue(pcrune str, pcrune str_end, bool& value)
		{
			rune const format_str[] = { '%', 'b' };
			pcrune format_str_end = format_str + sizeof(format_str);
			pcrune s = SScanf(str, str_end, format_str, format_str_end, x_va_r_list(x_va_r(&value)));
			return s;
		}

		pcrune	ToValue(pcrune str, pcrune str_end, s32& value, s32 base)
		{
			rune format_str[] = { '%', 'd' };
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			pcrune format_str_end = format_str + sizeof(format_str);
			pcrune s = SScanf(str, str_end, format_str, format_str_end, x_va_r_list(x_va_r(&value)));
			return s;
		}

		pcrune	ToValue(pcrune str, pcrune str_end, s64& value, s32 base)
		{
			rune format_str[] = { '%', 'd' };
			switch (base)
			{
				case 16: format_str[1] = 'x';
				case 10: format_str[1] = 'd';
				case 8: format_str[1] = 'o';
			};
			pcrune format_str_end = format_str + sizeof(format_str);
			pcrune s = SScanf(str, str_end, format_str, format_str_end, x_va_r_list(x_va_r(&value)));
			return s;
		}

		pcrune	ToValue(pcrune str, pcrune str_end, f32& value)
		{
			rune format_str[] = { '%', 'f' };
			pcrune format_str_end = format_str + sizeof(format_str);
			pcrune s = SScanf(str, str_end, format_str, format_str_end, x_va_r_list(x_va_r(&value)));
			return s;
		}

		pcrune	ToValue(pcrune str, pcrune str_end, f64& value)
		{
			rune format_str[] = { '%', 'f' };
			pcrune format_str_end = format_str + sizeof(format_str);
			pcrune s = SScanf(str, str_end, format_str, format_str_end, x_va_r_list(x_va_r(&value)));
			return s;
		}


		//------------------------------------------------------------------------------
		bool IsDecimal(pcrune str, pcrune str_end)
		{
			pcrune src = str;
			uchar32 c = PeekChar(src);
			if (c == '-')
			{
				ReadChar(src);
			}

			for (; src < str_end; )
			{
				c = ReadChar(src);
				if (c < '0' || c > '9')
					return false;
			}
			return true;
		}

		//------------------------------------------------------------------------------
		bool IsHexadecimal(pcrune str, pcrune str_end, bool with_prefix)
		{
			uchar32 c = 0;
			uchar32 p;
			s32 pos = 0;
			while (str < str_end)
			{
				p = c;
				c = ReadChar(str);
				if (c == 'x') {
					if (pos == 1 && with_prefix && p == '0') {
						// Ok, 0x.... prefix
					} else {
						return false;
					}
				} else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
					// Ok, digit or hex character
				} else {
					return false;
				}
				pos++;
			}
			return true;
		}
		
		//------------------------------------------------------------------------------
		bool IsFloat(pcrune str, pcrune str_end)
		{
			static pcrune sFloatStr = "Ee.#QNABIF";

			// Does it have any other of the strange characters?
			pcrune src = str;
			for ( ; src<str_end; )
			{
				uchar32 c = ReadChar(src);
				if (c >= '0' && c <= '9') 
				{
					continue;
				}

				pcrune float_str = sFloatStr;
				uchar32 f = ReadChar(float_str);;
				while (f != c)
				{
					f = ReadChar(float_str);
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
		bool IsGUID(pcrune str, pcrune str_end)
		{
			// Does it have only 0-9, a-f, A-F characters
			s32 n = 0;
			s32 l = 0;
			pcrune src = str;
			for ( ; src<str_end; n++)
			{
				uchar32 c = ReadChar(src);
				if (c >= '0' && c <= '9') continue;
				if (c >= 'A' && c <= 'F') continue;
				if (c >= 'a' && c <= 'f') continue;
				if (c == ':') 
				{
					l++;
					if (l > 1)
						return false;
					continue;
				}
				return false;
			}

			// the length of the string must be 17 characters long (16 + 1 for the':')
			return (n == 17);
		}

		prune	ToString(s32 val, prune str, pcrune str_end, pcrune str_eos, s32 base)
		{
			rune format_str[] = {'%', 'd'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			pcrune format_str_end = format_str + sizeof(format_str);
			prune end = SPrintf(str, str_eos, format_str, format_str_end, x_va(val));
			return end;
		}

		prune	ToString(u32 val, prune str, pcrune str_end, pcrune str_eos, s32 base)
		{
			rune format_str[] = {'%', 'u'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'u'; break;
				case 8: format_str[1] = 'o'; break;
			};
			pcrune format_str_end = format_str + sizeof(format_str);
			prune end = SPrintf(str, str_eos, format_str, format_str_end, x_va(val));
			return end;
		}

		prune	ToString(s64 val, prune str, pcrune str_end, pcrune str_eos, s32 base)
		{
			rune format_str[] = {'%', 'd'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'd'; break;
				case 8: format_str[1] = 'o'; break;
			};
			pcrune format_str_end = format_str + sizeof(format_str);
			prune end = SPrintf(str, str_eos, format_str, format_str_end, x_va(val));
			return end;
		}

		prune	ToString(u64 val, prune str, pcrune str_end, pcrune str_eos, s32 base)
		{
			rune format_str[] = {'%', 'u'};
			switch (base)
			{
				case 16: format_str[1] = 'x'; break;
				case 10: format_str[1] = 'u'; break;
				case 8: format_str[1] = 'o'; break;
			};
			pcrune format_str_end = format_str + sizeof(format_str);
			prune end = SPrintf(str, str_eos, format_str, format_str_end, x_va(val));
			return end;
		}

		prune	ToString(f32 val, s32 numFractionalDigits, prune str, pcrune str_end, pcrune str_eos)
		{
			rune format_str[] = {'%', 'f'};
			pcrune format_str_end = format_str + sizeof(format_str);
			prune end = SPrintf(str, str_eos, format_str, format_str_end, x_va(val));
			return end;
		}

		prune	ToString(f64 val, s32 numFractionalDigits, prune str, pcrune str_end, pcrune str_eos)
		{
			rune format_str[] = {'%', 'f'};
			pcrune format_str_end = format_str + sizeof(format_str);
			prune end = SPrintf(str, str_eos, format_str, format_str_end, x_va(val));
			return end;
		}


		bool		IsUpper(pcrune str, pcrune str_end)
		{
			while (str < str_end)
			{
				uchar32 c = ReadChar(str);
				if (IsLower(c))
					return false;
			}
			return true;
		}

		bool		IsLower(pcrune str, pcrune str_end)
		{
			while (str < str_end)
			{
				uchar32 c = ReadChar(str);
				if (IsUpper(c))
					return false;
			}
			return true;
		}

		bool		IsCapitalized(pcrune str, pcrune str_end)
		{
			bool is_upper = true;
			while (str < str_end)
			{
				uchar32 c = ReadChar(str);
				if (IsUpper(c) == is_upper)
					return false;
				is_upper = false;
			}
			return true;

		}

		bool		IsDelimited(pcrune str, pcrune end, rune delimit_left, rune delimit_right)
		{
			if (str < end)
			{
				uchar32 char_left = ReadChar(str);
				if (char_left == delimit_left)
				{
					uchar32 char_right;
					while (str < end)
					{
						char_right = ReadChar(str);
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
		prune ToUpper(prune str, pcrune str_end)
		{
			ASSERT(str != NULL);
			prune p = str;
			while (true)
			{
				if (p == str_end)
					break;
				uchar32 c = PeekChar(p);
				if ((c >= 'a') && (c <= 'z'))
				{
					c += ('A' - 'a');
					WriteChar(c, p, p + 4, WRITEMODE_ASCII);
				} else {
					ReadChar(p);
				}
			}
			return str;
		}

		/**
		 * Assumption: Letters A-Z and a-z are contiguous in the character set.
		 * This is true for ASCII and UniCode.  (Not so for EBCDIC!)
		 */
		prune ToLower(prune str, pcrune str_end)
		{
			ASSERT(str != NULL);
			prune p = str;
			while (true)
			{
				if (p == str_end)
					break;
				uchar32 c = PeekChar(p);
				if ((c >= 'A') && (c <= 'Z'))
				{
					c = 'a' + (c - 'A');
					WriteChar(c, p, p + 4, WRITEMODE_ASCII);
				} else {
					ReadChar(p);
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

		bool		StartsWith(pcrune str, pcrune str_end, uchar32 start_char)
		{
			uchar32 c = ReadChar(str);
			return start_char == c;
		}

		bool		StartsWith			(pcrune str, pcrune str_end, pcrune start_str, pcrune start_str_end)
		{
			// Match from begin of string
			while(true) 
			{
				if (start_str == start_str_end)
					break;
				uchar32 c1 = ReadChar(start_str);
				if (str == str_end)
					break;
				uchar32 c2 = ReadChar(str);
				if (c1 != c2)
					return false;
			}
			// If matched all the way to the end of inStartStr then success
			return true;
		}

		bool		EndsWith			(pcrune str, pcrune str_end, uchar32 end_char)
		{
			if (str == NULL || str_end <= str)
				return false;
			// ASCII only
			rune c = PeekChar(str_end - 1);
			return end_char == c;
		}

		bool		EndsWith(pcrune srcstr, pcrune srcstr_end, pcrune endstr, pcrune endstr_end)
		{
			// ASCII only
			pcrune srcptr = srcstr_end - 1;
			pcrune endptr = endstr_end - 1;
			while (true)
			{
				s32 l1, l2;
				rune c1 = PeekChar(srcptr, &l1);
				rune c2 = PeekChar(endptr, &l2);
				if (c1 != c2)
					return false;
				if (srcptr == srcstr)
					return true;
				if (endptr == endstr)
					return true;
				srcptr -= l1;
				endptr -= l2;
			}
			return false;
		}

		uchar32			FirstChar(pcrune str, pcrune str_end)
		{
			ASSERT(str != NULL);
			ASSERT(str_end != NULL);
			if (str == str_end)
				return '\0';
			uchar32 c = PeekChar(str);
			return c;
		}

		uchar32			LastChar(pcrune str, pcrune str_end)
		{
			ASSERT(str != NULL);
			ASSERT(str_end != NULL);
			if (str_end > str)
				return PeekChar(str_end - 1);
			return '\0';
		}

		/**
		 *------------------------------------------------------------------------------
		* Author:
		*     Jurgen Kluft
		* Summary:
		*     String compare, returning 0, 1 or -1
		* Arguments:
		*     inStr                - The main string
		*     inOther              - The 'compare' string to compare to the main string
		*     inLen             - The whole(-1) or sub length of the main string
		*     inOtherLen           - The whole(-1) or sub length of the 'compare' string
		* Returns:
		*     s32 - 0 when inStr==inOther, 1 when inStr>inOther, -1 when inStr<inOther
		* Description:
		*     This function can handle a bit more than traditional string compare, it can
		*     identify the following cases:
		*
		*     1) Main string sub length, 'Compare' string sub length
		*			Required: need to supply both inLen or inOtherLen
		*			Action:   strings are compared until inLen==0 or inOtherLen==0
		*
		*     2) Main string sub length, 'Compare' string total length = -1
		*			Required: need to supply inLen, no need to supply inOtherLen=-1
		*			Action:   strings are compared until inLen==0 or 'Compare' string encounters '\0'
		*
		*     3) Main string total length = -1, 'Compare' string sub length
		*			Required: need to supply inLen with -1, need to supply inOtherLen
		*			Action:   strings are compared until inOtherLen==0 or 'Compare' string encounters '\0'
		*
		*     4) Main string total length = -1, 'Compare' string total length = -1
		*			Required: no need to supply either inLen or inOtherLen
		*			Action:   strings are compared until one of the strings encounters '\0'
		*

		* See Also:
		*     SetAssertHandler
		*------------------------------------------------------------------------------
		*/

		s32  		Compare_Internal	(pcrune str, pcrune str_end, pcrune other, pcrune other_end, s32 cmp_mode = STRCMPMODE_CASE_SENSITIVE)
		{
			uchar32 c1 = '\0';
			uchar32 c2 = '\0';

			// 1/ Compare("aaaa", "aabb", 2, 2) -> return  0;
			// 2/ Compare("aaaa", "aabb", 3, 3) -> return -1;
			// 3/ Compare("aaaa", "aabb", 2, 4) -> return  0;
			// 4/ Compare("aaaa", "aabb", 4, 2) -> return  0;
			// 5/ Compare("aaaa", "aabb", 3, 4) -> return -1;
			// 6/ Compare("aaaa", "aabb", 4, 3) -> return -1;

			s32 result = -1;
			while (true)
			{
				if (str == str_end)
					break;
				if (other == other_end)
					break;

				c1 = ReadChar(str);
				c2 = ReadChar(other);

				if (cmp_mode == STRCMPMODE_CASE_IGNORE)
				{
					c1 = ToLower(c1);
					c2 = ToLower(c2);
				}

				if (c1 != c2)
					break;
			}
			
			if (c1 == c2)
				return 0;

			result = c1 < c2 ? -1 : 1;
			return result;
		}

		s32  		Compare				(pcrune str, pcrune str_end, pcrune other, pcrune other_end)
		{
			return Compare_Internal(str, str_end, other, other_end, false);
		}

		s32  		CompareNoCase		(pcrune str, pcrune str_end, pcrune other, pcrune other_end)
		{
			return Compare_Internal(str, str_end, other, other_end, true);
		}

		pcrune		Find_Internal(pcrune str, pcrune str_end, pcrune find, pcrune find_end, s32 find_mode = STRCMPMODE_CASE_SENSITIVE)
		{
			pcrune str1 = str;
			while (str1 < str_end) 
			{
				if (*str1 == *find)
				{
					pcrune str2 = str1;
					pcrune find2 = find;
					while (true) 
					{
						if (find2 == find_end)
							return str1;
						if (str2 == str_end)
							break;
						uchar32 c2 = PeekChar(str2);
						uchar32 f2 = PeekChar(find2);
						if (find_mode = STRCMPMODE_CASE_IGNORE)
						{
							c2 = ToLower(c2);
							f2 = ToLower(f2);
						}
						if (c2 != f2)
							break;
						ReadChar(str2);
						ReadChar(find2);
					}
				}
				str1++;
			}
			return NULL;
		}

		pcrune		Find(pcrune str, pcrune str_end, uchar32 find_char)
		{
			uchar32 c = 0;
			do 
			{
				pcrune cur_pos = str;
				c = ReadChar(str);
				if (c == find_char)
					return cur_pos;
			} while (str < str_end);
			return NULL;
		}

		pcrune	Find(pcrune str, pcrune str_end, pcrune find, pcrune find_end)
		{
			return Find_Internal(str, str_end, find, find_end, STRCMPMODE_CASE_SENSITIVE);
		}

		pcrune	FindNoCase(pcrune str, pcrune str_end, uchar32 find)
		{
			return Find_Internal(str, str_end, find, find_end, STRCMPMODE_CASE_IGNORE);
		}

		pcrune	FindOneOf(pcrune str, pcrune str_end, pcrune charset, pcrune charset_end)
		{
			pcrune cur_pos = str;
			pcrune end_pos = str_end;
			while (cur_pos < end_pos)
			{
				uchar32 c = PeekChar(cur_pos);
				if (Find(charset, charset_end, c) != NULL)
					return cur_pos;
				cur_pos++;
			}
			return NULL;
		}

		bool		IsUpper(pcrune str, pcrune str_end)
		{
			bool isUpper = true;
			while (str < str_end)
			{
				uchar32 c = ReadChar(str);
				isUpper = isUpper && !IsLower(c);
			}
			return isUpper;
		}

		bool		IsLower(pcrune str, pcrune str_end)
		{
			bool isLower = true;
			while (str < str_end)
			{
				uchar32 c = ReadChar(str);
				isLower = isLower && !IsUpper(c);
			}
			return isLower;
		}

		bool		IsCapitalized(pcrune str, pcrune str_end)
		{
			if (str < str_end)
			{
				uchar32 c = ReadChar(str);
				if (IsUpper(c))
				{
					c = ReadChar(str);
					return IsLower(c);
				}
			}
			return false;
		}

		bool		IsDelimited(pcrune str, pcrune str_end, uchar32 left, uchar32 right)
		{
			return StartsWith(str, str_end, left) && EndsWith(str, str_end, right);
		}
	}

	xcstr& xcstr::select(xccstr const& from, xccstr const& to)
	{
		if (from.mStr == to.mStr && from.mStr_Begin <= to.mStr_Begin)
		{
			mStr       = from.mStr;
			mStr_Begin = from.mStr_Begin;
			mStr_End   = to.mStr_Begin;
			mStr_Eos   = to.mStr_Eos;
		}
		return *this;
	}

	xcstr& xcstr::select_until_one_of(xccstr const& one_of)
	{
		prune fpos = ucstr::FindOneOf(mStr_Begin, mStr_End, one_of.mStr_Begin, one_of.mStr_End);
		if (fpos != NULL)
			mStr_End = fpos;
		return *this;
	}

	xcstr& xcstr::select_delimited(rune left_char, rune right_char)
	{
		rune left_str[] = { left_char, '\0' };
		prune left_end = left_str + 1;
		prune fpos = ucstr::FindOneOf(mStr_Begin, mStr_End, left_str, left_end);
		if (fpos != NULL && fpos < mStr_End)
		{
			ReadChar(fpos);
			mStr_Begin = fpos;

			rune right_str[] = { right_char, '\0' };
			prune right_end = right_str + 1;
	
			fpos = ucstr::FindOneOf(fpos, mStr_End, right_str, right_end);
			if (fpos == NULL)
			{
				reset();
			}
			else
			{
				mStr_End = fpos;
			}
		}
		return *this;
	}
	





};

/**
 *  END xCore namespace
 */


 #endif // ifndef SPU