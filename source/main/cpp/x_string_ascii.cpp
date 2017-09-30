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
	static rune			ReadChar(prune& str)
	{
		s32 l = 0;
		rune c = PeekChar(str, &l);
		str += l;
		return c;
	}

	//------------------------------------------------------------------------------
	static rune			ReadChar(pcrune& str)
	{
		s32 l = 0;
		rune c = PeekChar(str, &l);
		str += l;
		return c;
	}
	//------------------------------------------------------------------------------
	s32						StrLen(pcrune str, pcrune* end)
	{
		ASSERT(str != NULL);
		s32 len = 0;
		while (true) {
			uchar32 c = ReadChar(str);
			if (c == 0)
				break;
			len++;
		}
		if (end != NULL)
			*end = str;
		return len;
	}

	//------------------------------------------------------------------------------

	static s32				LenInBytes(uchar32 c)
	{
		if ((c & 0xffffff80) == 0x00) {
			return 1;
		}

		s32 l = 0;
		if ((c & 0xf8000000) == 0xf0000000) { l = 4; }
		else if ((c & 0xf00000) == 0xe00000) { l = 3; }
		else if ((c & 0xe000) == 0xc000) { l = 2; }
		return l;
	}

	//------------------------------------------------------------------------------

	static s32				LenInChars(pcrune str, pcrune * end)
	{
		return StrLen(str, end);
	}

	//------------------------------------------------------------------------------

	static u8				sUTF8LC[] = { 0, 0, 0xc0, 0xe0, 0xf0 };
	static s32				WriteChar(uchar32 c, prune& str, pcrune end)
	{
		s32 len = 0;
		if (c <= 0x7f) { len = 1; }
		else if (c < 0x0800) { len = 2; }
		else if (c < 0xd800) { len = 3; }
		else if (c < 0xe000) { len = 0; }
		else if (c < 0x010000) { len = 3; }
		else if (c < 0x110000) { len = 4; }

		if ((str + len) > end)
			return 0;

		if (len == 1) {
			*str++ = (rune)c;
			return 1;
		}

		uchar8 res[4];
		switch (len) {
		case 4: res[3] = (c & 0x3f) | 0x80; c = c >> 6;
		case 3: res[2] = (c & 0x3f) | 0x80; c = c >> 6;
		case 2: res[1] = (c & 0x3f) | 0x80; c = c >> 6;
		default: len = 0;
		};
		res[0] = (rune)c | sUTF8LC[len];

		if (str != NULL) {
			for (s32 i = 0; i < len; ++i) {
				*str++ = res[i];
			}
		}
		return len;
	}

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     strcpy   - copy two strings
	 * Arguments:
	 *        dest    - destination string
	 *        maxChar - number of characters need to be copied (excluding trailing zero)
	 *        str     - source string        
	 * Returns:
	 *        pointer to the destination string
	 *------------------------------------------------------------------------------
	 */
	prune StrCopy(prune dest, pcrune dest_end, pcrune src, pcrune src_end)
	{
		ASSERT(dest != NULL && dest_end != NULL);
		ASSERT(src != NULL && src_end != NULL);

		prune dst = dest;
		while (dst < dest_end)
		{
			if (dst == dest_end)
				break;
			if (src == src_end)
				break;
			*dst++ = *src++;
		}
		return dest;
	}


	//------------------------------------------------------------------------------

	s32	StrCmpn(pcrune str1, pcrune str1_end, pcrune str2, pcrune str2_end)
	{
		ASSERT(str1);
		ASSERT(str2);

		if (str1 == str2)
			return 0;

		do {
			if (str1 == str1_end && str2 == str2_end)
				break;

			if (str1 == str1_end)
				return -1;
			if (str2 == str2_end)
				return 1;

			rune c1 = *(str1);
			rune c2 = *(str2);
			if (c1 < c2)
				return -1;
			if (c1 > c2)
				return 1;
			str1++;
			str2++;
		} while (true);

		return 0;
	}

	//------------------------------------------------------------------------------
	static uchar32	ToUpper(uchar32 c) { return((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
	static uchar32	ToLower(uchar32 c) { return((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
	static uchar32	ToDigit(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
	static bool		IsUpper(uchar32 c) { return((c >= 'A') && (c <= 'Z')); }
	static bool		IsLower(uchar32 c) { return((c >= 'a') && (c <= 'z')); }
	static bool		IsAlpha(uchar32 c) { return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
	static bool		IsDigit(uchar32 c) { return((c >= '0') && (c <= '9')); }
	static uchar32	ToNumber(uchar32 c) { if (IsDigit(c)) return ToDigit(c); else if (c >= 'A' && c <= 'F') return(c - 'A' + 10); else if (c >= 'a' && c <= 'f') return(c - 'a' + 10); else return(c); }

	//------------------------------------------------------------------------------
	s32 StrCmpi(pcrune str1, pcrune str2)
	{
		ASSERT(str1);
		ASSERT(str2);
		if (str1 == str2) 
			return 0;
		do {
			uchar32 c1 = ReadChar(str1);
			uchar32 c2 = ReadChar(str2);
			if (c1 == 0 || c2 == 0)
				break;
			c1 = ToLower(c1);
			c2 = ToLower(c2);
			if (c1 < c2) return -1;
			if (c1 > c2) return 1;
		} while (true);
		return 0;
	}

	//------------------------------------------------------------------------------

	s32 StrCmpin(const rune *str1, const rune *str2, s32 n)
	{
		ASSERT(str1);
		ASSERT(str2);
		if (n == 0 || str1 == str2)
			return 0;
		do {
			uchar32 c1 = ReadChar(str1);
			uchar32 c2 = ReadChar(str2);
			if (c1 == 0 || c2 == 0)
				break;
			c1 = ToLower(c1);
			c2 = ToLower(c2);
			if (c1 < c2) return -1;
			if (c1 > c2) return 1;
		} while (--n > 0);
		return 0;
	}

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     strscn    - scan the string for certain pattern
	 * Arguments:
	 *        parttern - the pattern need to be scanned
	 *        str      - source string        
	 * Returns:
	 *        pointer to the pattern of the source string
	 *------------------------------------------------------------------------------
	 */
	pcrune StrStr(pcrune mainStr, pcrune subStr)
	{
		if (!*subStr)
			return((prune)mainStr);

		while (true) {
			pcrune str1 = mainStr;
			pcrune str2 = subStr;

			while (true) {
				uchar32 c1 = ReadChar(str1);
				uchar32 c2 = ReadChar(str2);
				if (c2 == 0)
					return mainStr;
				if (c1 != c2)
					break;
			}
			uchar32 c = ReadChar(mainStr);
			if (c == 0)
				break;
		}

		return NULL;
	}

	//------------------------------------------------------------------------------

	pcrune		StrChr(pcrune mainStr, uchar32 c)
	{
		while (true) {
			pcrune str = mainStr;
			uchar32 ch = ReadChar(mainStr);
			if (ch == 0)
				break;
			if (ch == c)
				return(str);
		}
		return NULL;
	}



	//------------------------------------------------------------------------------

	bool IsNumber(uchar32 c, s32 base)
	{
		const uchar32 max10 = (uchar32)((base > 9) ? '9' : ('0' + base - 1));

		if (c >= '0' && c <= max10)
			return true;

		if (base < 11)
			return false;

		const uchar32 max = (uchar32)('A' + (base - 11));

		c = ToUpper(c);
		return (c >= 'A' && c <= max);
	}

	//------------------------------------------------------------------------------

	bool IsInt(pcrune str)
	{
		s32 i = 0;
		if (str[0] == '-') {
			i = 1;
		}
		for (; str[i]; i++) {
			if (str[i] < '0' || str[i] > '9') return false;
		}
		return true;
	}

	//------------------------------------------------------------------------------

	bool IsFloat(pcrune str)
	{
		static pcrune floatStr = "Ee.#QNABIF";

		// Does it have any other of the strange characters?
		for (s32 i = 0; str[i]; i++)
		{
			if (str[i] >= '0' && str[i] <= '9') 
				continue;

			s32 l;
			for (l = 0; (str[i] != floatStr[l]) && floatStr[l]; l++);

			// Okay this is not a number
			if (floatStr[l] == 0) 
				return false;
		}

		return true;
	}

	//------------------------------------------------------------------------------

	bool IsHex(pcrune str)
	{
		for (s32 i = 0; str[i] != 0; i++)
		{
			if (str[i] >= '0' && str[i] <= '9') continue;
			if (str[i] >= 'A' && str[i] <= 'F') continue;
			if (str[i] >= 'a' && str[i] <= 'f') continue;
			return false;
		}
		return true;
	}

	//------------------------------------------------------------------------------

	bool IsGUID(pcrune str)
	{
		s32 i;
		for (i = 0; str[i]; i++)
		{
			s32 nCol = 0;
			if (str[i] >= '0' && str[i] <= '9') continue;
			if (str[i] >= 'A' && str[i] <= 'F') continue;
			if (str[i] >= 'a' && str[i] <= 'f') continue;
			if (str[i] == ':') 
			{
				nCol++;
				if (nCol > 1)
					return false;
				continue;
			}
			return false;
		}

		// the length of the string must be 17 characters long (16 + 1 for the':')
		if (i != 17) 
			return false;

		return true;
	}

	//------------------------------------------------------------------------------

	prune StrCat(prune front, pcrune end, pcrune back)
	{
		ASSERT(front != NULL);
		ASSERT(end != NULL);
		ASSERT(back != NULL);

		prune wptr = front;
		s32 l = 0;
		pcrune rptr = wptr;
		while (*rptr != 0)
		{
			ReadChar(rptr);
			++l;
		}
		wptr += (rptr - wptr);

		while (wptr < end)
		{
			uchar32 c = ReadChar(back);
			if (c == 0)
				break;
			if (WriteChar(c, wptr, end) == 0)
				break;
		}
		return front;
	}

	/**
	 * Assumption: Letters A-Z and a-z are contiguous in the character set.
	 * This is true for ASCII and UniCode.  (Not so for EBCDIC!)
	 */
	prune ToUpper(prune str)
	{
		ASSERT(str != NULL);
		prune p = str;
		while (true)
		{
			uchar32 c = PeekChar(p);
			if (c == 0)
				break;
			if ((c >= 'a') && (c <= 'z')) {
				c += ('A' - 'a');
				WriteChar(c, p, p + 4);
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
	prune ToLower(prune str)
	{
		ASSERT(str != NULL);
		prune p = str;
		while (true)
		{
			uchar32 c = PeekChar(p);
			if (c == 0)
				break;
			if ((c >= 'A') && (c <= 'Z')) {
				c = 'a' + (c - 'A');
				WriteChar(c, p, p + 4);
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

	bool		StartsWith			(pcrune inStr, uchar32 inStartChar)
	{
		uchar32 c = ReadChar(inStr);
		return c == inStartChar;
	}

	bool		StartsWith			(pcrune inStr, pcrune inStartStr)
	{
		// Match from begin of string
		while(true) {
			uchar32 c1 = ReadChar(inStartStr);
			if (c1 == '\0')
				break;
			uchar32 c2 = ReadChar(inStr);
			if (c1 != c2)
				return false;
		}
		// If matched all the way to the end of inStartStr then success
		return true;
	}

	bool		StartsWith			(pcrune inStr, s32 inLen, pcrune inStartStr, s32 inStartLen)
	{
		if (inStartLen>inLen)
			return false;

		s32 minimumLen = x_intu::min(inLen, inStartLen==-1 ? inLen : inStartLen);
		for (s32 i=0; i<minimumLen; i++)
		{
			uchar32 c1 = ReadChar(inStartStr);
			if (c1 == '\0')
				return i>0;
			uchar32 c2 = ReadChar(inStr);
			if (c1 != c2)
				return false;
		}
		// If matched all the way to the end of inStartStr[i] then success
		return true;
	}

	bool		EndsWith			(pcrune str, pcrune str_end, uchar32 inEndChar)
	{
		if (str == NULL || str_end <= str)
			return false;
		rune c = *(str_end - 1);
		return inEndChar == c;
	}

	bool		EndsWith			(pcrune str, pcrune str_end, pcrune str2, pcrune str2_end)
	{
		pcrune strL = str_end - 1;
		pcrune strR = str2_end - 1;
		while (strL >= str && strR >= str2)
		{
			rune c1 = *strL;
			rune c2 = *strR;
			if (c1 != c2)
				return false;
			if (strR == str2)
				return true;
			strL -= 1;
			strR -= 1;
		}
		return false;
	}

	rune			FirstChar			(pcrune str, pcrune str_end)
	{
		ASSERT(str != NULL);
		if (str == str_end)
			return '\0';
		rune c = *str;
		return c;
	}

	rune			LastChar			(pcrune str, pcrune str_end)
	{
		if ((str_end - 1) >= str)
			return *(str_end - 1);
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

	s32  		Compare_Internal	(pcrune str, pcrune str_end, pcrune other, pcrune other_end, bool inIgnoreCase=false)
	{
		rune c1 = '\0';
		rune c2 = '\0';

		// 1/ Compare("aaaa", "aabb", 2, 2) -> return  0;
		// 2/ Compare("aaaa", "aabb", 3, 3) -> return -1;
		// 3/ Compare("aaaa", "aabb", 2, 4) -> return  0;
		// 4/ Compare("aaaa", "aabb", 4, 2) -> return  0;
		// 5/ Compare("aaaa", "aabb", 3, 4) -> return -1;
		// 6/ Compare("aaaa", "aabb", 4, 3) -> return -1;

		s32 result = -1;
		while (true)
		{
			c1 = ReadChar(str);
			c2 = ReadChar(other);
			if (c1 == '\0')
				break;
			if (c2 == '\0')
				break;

			if (inIgnoreCase) {
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

	pcrune		Find(pcrune inStr, uchar32 inChar, s32 inLen)
	{
		uchar32 c = 0;
		do {
			if (inLen==0)
				break;
			--inLen;
			pcrune str = inStr;
			c = ReadChar(inStr);
			if (c == inChar)
				return str;
		} while (c != 0);
		return NULL;
	}

	pcrune		Find(pcrune str, pcrune str_end, pcrune find, pcrune find_end)
	{
		pcrune str1 = str;
		while (str1 < str_end) {
			if (*str1 == *find)
			{
				pcrune str2 = str1;
				pcrune find2 = find;
				while (true) {
					if (find2 == find_end)
						return str1;
					if (str2 == str_end)
						break;
					rune c2 = *str2;
					rune f2 = *find2;
					if (c2 != f2)
						break;
					str2++;
					find2++;
				}
			}
			str1++;
		}
		return NULL;
	}

	pcrune	FindNoCase			(pcrune str, pcrune str_end, rune find)
	{
		find = ToLower(find);
		while (str < str_end)
		{
			rune c = *str;
			if (ToLower(c) == find)
				return str;
			str++;
		}
		return NULL;
	}

	pcrune	FindNoCase			(pcrune str, pcrune str_end, pcrune find, pcrune find_end)
	{
		pcrune str1 = str;
		while (str1 < str_end) {
			rune const c = *str1;
			pcrune str2 = str1;
			pcrune find2 = find;
			while (true) {
				if (find2 == find_end)
					return str1;
				if (str2 == str_end)
					break;
				rune const c1 = ToLower(*str2++);
				rune const c2 = ToLower(*find2++);
				if (c1 != c2)
					break;
			}
			str1++;
		}
		return NULL;
	}

	pcrune	FindInSubstr		(pcrune str, pcrune str_end, rune find)
	{
		while (str < str_end) {
			if (*str == find)
				return str;
			str++;
		}
		return NULL;
	}

	pcrune	FindInSubstr_Internal(pcrune str, pcrune str_end, pcrune find, pcrune find_end, bool case_sensitive)
	{
		if (str == str_end)
			return NULL;
		if (find == find_end)
			return NULL;

		while (str < str_end) {
			rune c = *str;
			if (c == *find)
			{
				pcrune fnd = find;
				pcrune sub = str;
				do
				{
					if (fnd == find_end)
						return str;
					if (sub == str_end)
						break;
					if (case_sensitive)
					{
						if (*sub++ != *fnd++)
							break;
					}
					else
					{
						if (ToLower(*sub++) != ToLower(*fnd++))
							break;
					}
				} while (true);
			}
			str++;
		}
		return NULL;
	}

	pcrune	FindInSubstr		(pcrune str, pcrune str_end, pcrune find, pcrune find_end)
	{
		return FindInSubstr_Internal(str, str_end, find, find_end, true);
	}

	pcrune	FindNoCaseInSubstr	(pcrune str, pcrune str_end, pcrune find, pcrune find_end)
	{
		return FindInSubstr_Internal(str, str_end, find, find_end, false);
	}

	pcrune	FindOneOf			(pcrune str, pcrune str_end, pcrune charset, pcrune charset_end)
	{
		pcrune cur_pos = str;
		pcrune end_pos = str_end;
		while (cur_pos < end_pos)
		{
			rune c = *cur_pos;
			if (FindInSubstr(charset, charset_end, c) != NULL)
				return cur_pos;
			cur_pos++;
		}
		return NULL;
	}

	bool		IsUpper			(pcrune str, pcrune str_end)
	{
		bool isUpper = true;
		while (str < str_end)
		{
			rune c = *str++;
			isUpper = isUpper && !IsLower(c);
		}
		return isUpper;
	}

	bool		IsLower			(pcrune str, pcrune str_end)
	{
		bool isLower = true;
		while (str < str_end)
		{
			rune c = *str++;
			isLower = isLower && !IsUpper(c);
		}
		return isLower;
	}

	bool		IsCapitalized		(pcrune str, pcrune str_end)
	{
		if (str < str_end)
		{
			rune c = *str++;
			if (IsUpper(c))
			{
				c = *str++;
				return IsLower(c);
			}
		}
		return false;
	}

	bool		IsQuoted			(pcrune str, pcrune str_end, rune quote)
	{
		if (str < (str_end - 1))
		{
			rune c1 = *str;
			rune c2 = *(str_end - 1);
			return c1 == c2 && c1 == quote;
		}
		return false;
	}






};

/**
 *  END xCore namespace
 */


 #endif // ifndef SPU