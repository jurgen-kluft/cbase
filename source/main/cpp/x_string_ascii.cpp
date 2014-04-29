#include "xbase\x_debug.h"
#include "xbase\x_integer.h"
#include "xbase\x_string_ascii.h"

#ifdef SPU

	 
namespace xcore
 {
	 s32		x_printf   				(const char* formatStr, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, 
																const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16) { return 0; }
	 s32		x_printf				(const char* formatStr, const x_va_list& args)		{ return 0; }
	 s32		x_printf				(const char* str)									{ return 0; }
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
	 * ANSI functions
	 *==============================================================================
	 *==============================================================================
	 *==============================================================================
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     x_atod64          - Converts a string value to integer base from a particular
	 * Arguments:
	 *        str            - Source string encoded with a particular base
	 *        Base           - Base of the numeric string
	 * Returns:
	 *        Actual integer number
	 * Description:
	 *      Converts a string that has been encoded into an integer of a particular base
	 *      into a actual atomic integer of a particular size (32vs64)bits. If the
	 *      string contains '_' or ':' characters they will be ignore.
	 * See Also:
	 *      x_atod32 x_atoi32 x_atoi64 x_atof32 x_atof64
	 *------------------------------------------------------------------------------
	 */
	s64 x_atod64(const char* str, s32 base)
	{
		ASSERT(str != NULL);
		ASSERT(base > 2);
		ASSERT(base <= (26 + 26 + 10));

		// Skip whitespace.
		for (; *str == ' '; ++str)
			; // empty body

		char c;         // Current character.
		// Save sign indication.
		c = *str++;

		char sign;        // If '-', then negative, otherwise positive.
		sign = c;

		// Skip sign.
		if ((c == '-') || (c == '+'))
		{
			c = *str++;
		}

		s64  total = 0;   // Current total.
		s32  validBase;
		// Decode the rest of the string
		for (;;c = *str++)
		{
			if ((c >= '0') && (c <= '9'))  
			{
				validBase = c - '0';
			}
			else if ((c >= 'a') && (c <= 'z'))  
			{
				validBase = c - 'a' + 10;
			}
			else if ((c >= 'A') && (c <= 'Z'))  
			{
				validBase = c - 'A' + 10 + 26;
			}
			else if (c == '_' || c == ':')  
			{
				// Ignore
				continue;
			}
			else
			{
				// Negate the total if negative.
				if (sign == '-') 
					total = -total;

				// Any other character is bad news
				//return total;
				break;
			}

			ASSERT(validBase >= 0);
			ASSERT(validBase <  base);

			// Accumulate digit.
			total = (base * total) + validBase;
		}

		// No way to get here
		return total;
	}

	// <COMBINE x_atod64 >
	s32 x_atod32(const char* str, s32 base)
	{
		return (s32)x_atod64(str, base);
	}

	/**
	 *------------------------------------------------------------------------------
	 *DOM-IGNORE-BEGIN
	 *------------------------------------------------------------------------------
	 */

	/**
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     _x_dtoa    - Converts value to a string in whatever base is passed
	 * Arguments:
	 *      Val             - Number to be converted
	 *      buff            - Destination String 
	 *      sizeOfBuff      - Size of the destination string
	 *      Base            - Numeric base that we are converting to.
	 *      hasNegative     - The number will has minus or not 
	 * Returns:
	 *        length of the final string
	 * Description:
	 *      Converts any 32 bit number into a string which contains the number 
	 *      represented in a particular base. The base for instance could be 16
	 *      for hex numbers.
	 *------------------------------------------------------------------------------
	 */
	s32 _x_dtoa(u64 val, char* buff, s32 str_buffer_size, s32 base, bool hasNegative)
	{
		ASSERT(buff);
		ASSERT(base > 2);
		ASSERT(base <= (26 + 26 + 10));
		ASSERT(str_buffer_size > 1);

		char* p      = buff;
		s32   length = 0;  
		s32   i      = 0;
		if (hasNegative)
		{
			*p++ = '-';
			i++;
			length++;
		}

		do 
		{
			u32 cVal = (u32)(val % base);
			val /= base;

			// convert to ascii and store 
			if (cVal < 10)
			{
				*p = (char)(cVal + '0');
				p++;
			}
			else if (cVal < (10 + 26))
			{
				*p = (char)(cVal - 10 + 'a');
				p++;
			}
			else if (cVal < (10 + 26 + 26))
			{
				*p = (char)(cVal - 10 - 26 + 'A');
				p++;
			}
			else
			{
				// The base is too big
				ASSERT(0);
			}

			length++;

		} while (val > 0 && length < str_buffer_size);

		// Check if we run out of space
		if (length >= str_buffer_size)
		{
			buff[0] = 0;
			return 0;
		}

		//  terminate string; 
		*p = 0;

		// Reverse string order
		for (p--; p > &buff[i] ; i++, p--)
		{
			char t = *p;
			*p = buff[i];
			buff[i] = t;
		}

		return length;
	}
	///< DOM-IGNORE-END

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     x_dtoa    - Converts value to a string in whatever base is passed
	 * Arguments:
	 *      Val             - Number to be converted
	 *      str             - Destination String 
	 *      SizeOfString    - Size of the destination string
	 *      Base            - Numeric base that we are converting to.
	 * Returns:
	 *        length of the final string
	 * Description:
	 *      Converts any 32 bit number into a string which contains the number 
	 *      represented in a particular base. The base for instance could be 16
	 *      for hex numbers.
	 * See Also:
	 *     x_atod64 x_atod32 x_atoi32 x_atoi64 x_atof32 x_atof64
	 *------------------------------------------------------------------------------
	 */
	s32 x_dtoa(s32 val, char* str, s32 str_buffer_size, s32 base)
	{
		if (val < 0)
		{
			return _x_dtoa(u64(u32(-val)), str, str_buffer_size, base, xTRUE);
		}
		else
		{
			return _x_dtoa(u64(val), str, str_buffer_size, base, xFALSE);
		}
	}

	// <COMBINE x_dtoa >
	s32 x_dtoa(u32 val, char* str, s32 str_buffer_size, s32 base)
	{
		return _x_dtoa(u64(val), str, str_buffer_size, base, xFALSE);
	}

	// <COMBINE x_dtoa >
	s32 x_dtoa(s64 val, char* str, s32 str_buffer_size, s32 base)
	{
		if (val < 0)
		{
			return  _x_dtoa(u64(-val), str, str_buffer_size, base, xTRUE);
		}
		else
		{
			return  _x_dtoa(u64(val), str, str_buffer_size, base, xFALSE);
		}
	}

	// <COMBINE x_dtoa >
	s32 x_dtoa(u64 val, char* str, s32 str_buffer_size, s32 base)
	{
		return _x_dtoa(u64(val), str, str_buffer_size, base, xFALSE);
	}


	#define F32TOA_PSH(X)				(*(dest++)=(X))

	s32			x_f32toa(f32 val, s32 prec, char* str, s32 str_buffer_size, xbool positive_sign, char format)
	{
		/* converts a floating point number to an ascii string */
		/* x is stored into str, which should be at least 30 chars long */
		int i, k;
		double y;

		s32 fstyle = 0;
		s32 ndig = ( prec<=0) ? 7 : (prec > 22 ? 23 : prec+1);
		if  (format == 'f' || format == 'F')
			fstyle = 1;

		char* start = str;
		f32 x = val;

		/* print in e format unless last arg is 'f' */
		s32 ie = 0;

		/* if x negative, write minus and reverse */
		if ( x < 0)
		{
			*str++ = '-';
			x = -x;
		}

		/* put x in range 1 <= x < 10 */
		if (x > 0.0f)
		{
			while (x < 1.0)
			{
				x *= 10.0f;
				ie--;
			}
		}
		while (x >= 10.0f)
		{
			x = x / 10.0f;
			ie++;
		}

		/* in f format, number of digits is related to size */
		if (fstyle) 
			ndig =+ ie;

		/* round. x is between 1 and 10 and ndig will be printed to
		right of decimal point so rounding is ... */
		for (y = i = 1; i < ndig; i++)
			y = y/10.0;

		x += (f32)(y / 2.0);

		/* repair rounding disasters */
		if (x >= 10.0f)
		{
			x = 1.0; 
			ie++;
		}

		/* now loop.  put out a digit (obtain by multiplying by
		10, truncating, subtracting) until enough digits out */
		/* if fstyle, and leading zeros, they go out special */
		if (fstyle && ie<0)
		{
			*str++ = '0'; 
			*str++ = '.';
			if (ndig < 0) 
				ie = ie-ndig; /* limit zeros if underflow */
			for (i = -1; i > ie; i--)
				*str++ = '0';
		}
		for (i=0; i < ndig; i++)
		{
			k = (s32)x;
			*str++ = k + '0';
			if (i == (fstyle ? ie : 0)) /* where is decimal point */
				*str++ = '.';
			x =- (f32)(y = k);
			x *= 10.0f;
		}

		/* now, in estyle,  put out exponent if not zero */
		if (!fstyle && ie != 0)
		{
			*str++ = 'E';
			if (ie < 0)
			{
				ie = -ie;
				*str++ = '-';
			}
			for (k=100; k > ie; k /= 10);
			for (; k > 0; k /= 10)
			{
				*str++ = ie/k + '0';
				ie = ie%k;
			}
		}
		*str = '\0';
		return (s32)(str - start);
	}
	s32			x_f32toa(f32 val, s32 prec, char* str, s32 str_buffer_size)
	{
		return x_f32toa(val, prec, str, str_buffer_size, true, 'f');
	}


	s32			x_f64toa(f64 val, s32 numFractionalDigits, char* str, s32 str_buffer_size)
	{
		char format[8];
		x_sprintf(format, sizeof(format)-1, "%%.%df", x_va(numFractionalDigits));
		s32 len = x_sprintf(str, str_buffer_size-1, format, x_va(val));
		return len;
	}

	//------------------------------------------------------------------------------

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     x_strcpy   - copy two strings
	 * Arguments:
	 *        dest    - destination string
	 *        maxChar - number of characters need to be copied (excluding trailing zero)
	 *        str     - source string        
	 * Returns:
	 *        pointer to the destination string
	 *------------------------------------------------------------------------------
	 */
	char* x_strcpy(char* dest, s32 dest_buffer_size, const char* src)
	{
		ASSERT(dest);
		ASSERT(src != NULL);
		ASSERT(dest_buffer_size >= 0);

		if (dest_buffer_size == 0)
			return dest;

		char* start = dest;
		while (true)
		{
			if (dest_buffer_size==1 || *src=='\0')
			{
				*dest = '\0';
				break;
			}
			--dest_buffer_size;
			*dest++ = *src++;
		}
		return start;
	}

	//------------------------------------------------------------------------------

	s32 x_strlen(const char* str)
	{
		ASSERT(str != NULL);
		const char *end = str;

		while (*end++) 
		{
			//Empty
		}
		return (s32)(end - str - 1);
	}
	
	//------------------------------------------------------------------------------

	s32 x_strcmp(const char* str1, const char* str2)
	{
		s32 result = 0;

		ASSERT(str1);
		ASSERT(str2);

		if (str1 == str2) return 0;

		while (!((result = ((s32)*str1) - ((s32)*str2)) != 0) && *str1)
			++str1, ++str2;

		if (result==0)		return 0;
		else if (result<0)	return -1;
		else				return 1;
	}

	//------------------------------------------------------------------------------

	s32	x_strncmp(const char * cs, const char * ct, s32 n)
	{
		if (n == 0) 
			return 0;

		while (--n && (*cs == *ct))
		{
			if (*cs == 0) 
				return 0;
			++cs; ++ct;
		}

		s32 result = *cs - *ct;
		if (result==0)		return 0;
		else if (result<0)	return -1;
		else				return 1;
	}

	//------------------------------------------------------------------------------

	s32 x_stricmp(const char* str1, const char* str2)
	{
		s32 f, l;

		ASSERT(str1);
		ASSERT(str2);

		if (str1 == str2) return 0;

		do
		{
			if (((l = (s32)(*(str1++))) >= 'A') && (l <= 'Z'))
				l -= ('A' - 'a');

			if (((f = (s32)(*(str2++))) >= 'A') && (f <= 'Z'))
				f -= ('A' - 'a');

		} while (f && (f == l));

		s32 result = l - f;
		if (result==0)		return 0;
		else if (result<0)	return -1;
		else				return 1;
	}

	//------------------------------------------------------------------------------

	s32 x_strnicmp(const char *s1, const char *s2, s32 count)
	{
		s32 f, l;

		do 
		{
			if (((f = (unsigned char)(*(s1++))) >= 'A') && (f <= 'Z'))
				f -= 'A' - 'a';
			if (((l = (unsigned char)(*(s2++))) >= 'A') && (l <= 'Z'))
				l -= 'A' - 'a';
		} while (--count && f && (f == l));

		s32 result = f - l;
		if (result==0)		return 0;
		else if (result<0)	return -1;
		else				return 1;
	}

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     x_strscn    - scan the string for certain parttern
	 * Arguments:
	 *        parttern - the parttern need to be scanned
	 *        str      - source string        
	 * Returns:
	 *        pointer to the parttern of the source string
	 *------------------------------------------------------------------------------
	 */
	char* x_strstr(const char* mainStr, const char* subStr)
	{
		char* mainString = (char*)mainStr;
		char* str1; 
		char* str2;

		if (!*subStr)
			return((char*)mainStr);

		while (*mainString)
		{
			str1 = mainString;
			str2 = (char*)subStr;

			while (*str1 && *str2 && !(*str1 - *str2))
			{
				str1++;
				str2++;
			}

			if (!*str2)
				return(mainString);

			mainString++;
		}

		return NULL;
	}

	//------------------------------------------------------------------------------

	char*		x_strchr    			(const char* mainStr, char c)
	{
		char* mainString = (char*)mainStr;

		while (*mainString)
		{
			if (*mainString == c)
				return(mainString);
			++mainString;
		}
		return NULL;
	}

	//------------------------------------------------------------------------------

	char*		x_strrchr    			(const char* mainStr, char c)
	{
		char* mainString = (char*)mainStr;

		s32 i = x_strlen(mainString);
		while (--i >= 0)
		{
			if (mainString[i] == c)
				return(&mainString[i]);
		}
		return NULL;
	}

	//------------------------------------------------------------------------------

	const char*        x_strscn(const char* str, const char* parttern)
	{
		const char *scan;

		while (*str != 0)
		{
			scan = parttern;
			while (*scan != 0)
			{
				if (*str == *scan)
				{
					return str;
				}
				else
				{
					scan++;
				}
			}
			str++;
		}
		return NULL;
	}

	//------------------------------------------------------------------------------

	char* x_stristr(const char* mainStr, const char* subStr)
	{
		char* mainString = (char*)mainStr;
		char* str1; 
		char* str2;

		if (!*subStr)
			return((char*)mainStr);

		while (*mainString)
		{
			str1 = mainString;
			str2 = (char*)subStr;

			while (*str1 && *str2 && !(x_tolower(*str1) - x_tolower(*str2)))
			{
				str1++;
				str2++;
			}

			if (!*str2)
				return(mainString);

			mainString++;
		}

		return NULL ;
	}


	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     Converts a string to a numeric value
	 * Arguments:
	 *        str     - String to be converted. 
	 * Returns:
	 *      Each function returns the value produced by interpreting the input characters as a number. 
	 *      The return value is 0 if the input cannot be converted to a value of that type. 
	 *      The return value is undefined in case of overflow.
	 * Description:
	 *      These functions convert a character string to a f64-precision, floating-point value 
	 *      (atof64 and atof32), an integer, long integer value (atoi32, atoi64).
	 *
	 *<P>   The input string is a sequence of characters that can be interpreted as a numerical value 
	 *      of the specified type. The function stops reading the input string at the first character 
	 *      that it cannot recognize as part of a number. This character may be the null character 
	 *      ('\\0' or L'\\0') terminating the string.
	 *
	 *<P>   The string argument to atof32 and atof64 has the following form:
	 *
	 *<P>   [whitespace] [sign] [digits] [.digits] [ {d | D | e | E }[sign]digits]
	 *
	 *<P>   A whitespace consists of space or tab characters, which are ignored; sign is either plus (+) or minus (?; 
	 *      and digits are one or more decimal digits. If no digits appear before the decimal point, at least one must 
	 *      appear after the decimal point. The decimal digits may be followed by an exponent, which consists 
	 *      of an introductory letter (d, D, e, or E) and an optionally signed decimal integer.
	 *
	 *<P>   atoi32, and atoi64 do not recognize decimal points or exponents. 
	 *      The string argument for these functions has the form:
	 *
	 *<P>   [whitespace] [sign]digits
	 *
	 *<P>   where whitespace, sign, and digits are as described for atof32 and atof64.
	 *
	 * Example:
	 * <CODE>
	 *      s32 main(void)
	 *      {
	 *         char *s; f64 x; s32 i; s64 li;
	 *
	 *           * Test of atof64
	 *          s = "  -2309.12E-15";     
	 *          x = x_atof64(s);
	 *          x_printf("atof64 test: \\"%s\\"; f32:  %e\\n", s, x);
	 *
	 *           * Test of atof32 
	 *          s = "7.8912654773d210";  
	 *          x = x_atof32(s);
	 *          x_printf("atof32 test: \\"%s\\"; f32:  %e\\n", s, x);
	 *
	 *           * Test of atoi 
	 *          s = "  -9885 pigs";      
	 *          i = x_atoi32(s);
	 *          x_printf("atoi32 test: \\"%s\\"; integer: %d\\n", s, i);
	 *
	 *           * Test of atol 
	 *          s = "98854 dollars";     
	 *          li = x_atoi64(s);
	 *          x_printf("atoi64 test: \\"%s\\"; long: %Ld\\n", s, li);
	 *      }
	 * </CODE>
	 *
	 * <TABLE>
	 *      The Output
	 *      -----------------------------------------------------
	 *      atof64 test: "  -2309.12E-15"; f32:  -2.309120e-012
	 *      atof32 test: "7.8912654773d210"; f32:  7.891265e + 210
	 *      atoi32 test: "  -9885 pigs"; integer: -9885
	 *      atoi64 test: "98854 dollars"; long: 98854
	 * </TABLE>
	 * See Also:
	 *     x_sprintf
	 *------------------------------------------------------------------------------
	 */
	s32 x_atoi32(const char* str)
	{
		ASSERT(str != NULL);

		// Skip whitespace.
		for (; *str == ' '; ++str)
			; // empty body

		// Save sign indication.
		char c = *str++;          // Current character.
		char sign = c;            // If '-', then negative, otherwise positive.

		// Skip sign.
		if((c == '-') || (c == '+'))
		{
			c = *str++;
		}

		s32 total = 0;    // Current total.

		while ((c >= '0') && (c <= '9'))
		{
			// Accumulate digit.
			total = (10 * total) + (c - '0');

			// get next char.
			c = *str++;
		}

		// Negate the total if negative.
		if (sign == '-') 
			total = -total;

		return total;
	}

	//------------------------------------------------------------------------------
	s32 x_atoi32(const char* str, const char** scanEnd, s32 base)
	{
		const char *p = (const char *)str;
		u32 res = 0;
		s32 negative = 0;

		// skip initial white spaces
		while (*p != '\0' && *p <= ' ')
			++p;

		if (p[0] == '\0')
		{
			if (scanEnd)
				*scanEnd = (const char *)p;
			return 0;
		}

		// check sign
		if ((p[0] == '+') || (p[0] == '-'))
		{
			negative = p[0] == '-' ? 1 : 0;
			++p;
		}

		// figure out base if not given
		if (base == 0)
		{
			if (p[0] != '0')
			{
				base = 10;
			} 
			else
			{
				if ((p[1] == 'x') || (p[1] == 'X'))
				{
					base = 16;
					p += 2;
				}
				else
				{
					base = 8;
					++p;
				}
			}
		}
		else
		{
			// 0x/0X is allowed for hex even when base is given
			if ((base == 16) && (p[0] == '0'))
			{
				if ((p[1] == 'x') || (p[1] == 'X'))
				{
					p += 2;
				}
			}
		}

		for (; p[0]; ++p)
		{
			u32 val = p[0];

			if (val >= 'a')
				val -= 'a' - 'A';

			if (val >= 'A')
			{
				val = val - 'A' + 10;
			} 
			else 
			{
				if (val < '0' || val > '9') 
					break;

				val = val - '0';
			}

			if (val >= (u32)base) 
				break;

			res = res * (u32)base + val;
		}

		if (scanEnd)
			*scanEnd = p;

		return negative ? -((s32)res) : (s32)res;
	}

	//------------------------------------------------------------------------------
	/// <COMBINE x_atoi32 >
	s64 x_atoi64(const char* str)
	{
		ASSERT(str != NULL);

		// Skip whitespace.
		for (; *str == ' '; ++str)
			; // empty body

		// Save sign indication.
		char c = *str++;            // Current character.
		char sign = c;              // If '-', then negative, otherwise positive.

		// Skip sign.
		if((c == '-') || (c == '+'))
		{
			c = *str++;
		}

		s64 total = 0;      // Current total.

		while ((c >= '0') && (c <= '9'))
		{
			// Accumulate digit.
			total = (10 * total) + (c - '0');

			// get next char.
			c = *str++;
		}

		// Negate the total if negative.
		if (sign == '-') 
			total = -total;

		return total;
	}

	//------------------------------------------------------------------------------
	s64 x_atoi64(const char* str, s32 base, const char** scanEnd)
	{
		// Evaluate sign
		s32 sign = 1;
		if (*str == '-')
		{
			sign = -1;
			str++;
		}
		else if (*str == '+')
		{
			str++;
		}

		// Skip trailing zeros
		while (*str == '0')
			str++;

		s32 val = 0;
		s32 overflow = 0;
		s64    result = 0;
		// Convert number
		while (x_isnumber(*str, base))
		{
			s64    oldresult;
			val = x_tonumber(*str++);
			if (val > base || val < 0)
				return 0;

			oldresult = result;
			result *= base;
			result += val;

			// Detect overflow
			if (oldresult > result)
				overflow = 1;
		}
		if (scanEnd != 0)
			*scanEnd = str;

		if (overflow)
			result = -1;

		result *= sign;
		return result;
	}

	//------------------------------------------------------------------------------

	bool x_isnumber(char c, s32 base)
	{
		const char max10 = (char)((base > 9) ? '9' : ('0' + base - 1));

		if (c >= '0' && c <= max10)
			return xTRUE;

		if (base < 11)
			return xFALSE;

		const char max = (char)('A' + (base - 11));

		c = (char)x_toupper(c);
		return (c >= 'A' && c <= max);
	}

	//------------------------------------------------------------------------------

	bool x_isstrint(const char* str)
	{
		if (str[0] == '-')
		{
			if (str[1] < '0' || str[1] > '9')
				return xFALSE;

			for (s32 i = 2; str[i]; i++)
			{
				if (str[i] < '0' || str[i] > '9') return xFALSE;
			}
		}
		else
		{
			for (s32 i = 0; str[i]; i++)
			{
				if (str[i] < '0' || str[i] > '9') return xFALSE;
			}

		}

		return xTRUE;
	}

	//------------------------------------------------------------------------------

	bool x_isstrfloat(const char* str)
	{
		static const char* floatStr = "Ee.#QNABIF";

		// Does it have any other of the strange characters?
		for (s32 i = 0; str[i]; i++)
		{
			if (str[i] >= '0' && str[i] <= '9') 
				continue;

			s32 l;
			for (l = 0; (str[i] != floatStr[l]) && floatStr[l]; l++);

			// Okay this is not longer a number
			if (floatStr[l] == 0) return xFALSE;
		}

		return xTRUE;
	}

	//------------------------------------------------------------------------------

	bool x_isstrhex(const char* str)
	{
		for (s32 i = 0; str[i]; i++)
		{
			if (str[i] >= '0' && str[i] <= '9') continue;
			if (str[i] >= 'A' && str[i] <= 'F') continue;
			if (str[i] >= 'a' && str[i] <= 'f') continue;
			return xFALSE;
		}

		return xTRUE;
	}

	//------------------------------------------------------------------------------

	bool x_isstrguid(const char* str)
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
				if (nCol > 1) return xFALSE;
				continue;
			}
			return xFALSE;
		}

		// the length of the string must be 17 characters long (16 + 1 for the':')
		if (i != 17) return xFALSE;

		return xTRUE;
	}

	//------------------------------------------------------------------------------
	// simple_pow
	//------------------------------------------------------------------------------
	/*
	static f32 simple_pow(f32 x, s32 p)
	{
		if (p == 0) return 1.0f;
		if (x == 0.0f) return 0.0f;

		if (p < 0)
		{
			p = -p;
			x = 1.0f / x;
		}

		f32 r = 1.0f;
		// TODO:while?
		for (;;)
		{
			if (p & 1) r *= x;
			if ((p >>= 1) == 0) return r;
			x *= x;
		}
	}
	*/
	//------------------------------------------------------------------------------
	// This implementation will handle scientific notation such as "1.23e + 7".
	// <COMBINE x_atoi32 >
	f32 x_atof32(const char* s)
	{
		const char* s_end;
		return x_strtof(s, &s_end);
	}

	//------------------------------------------------------------------------------
	// <COMBINE x_atoi32 >

	f64 x_atof64(const char* s)
	{
		const char* s_end;
		return x_strtod(s, &s_end);
	}

	//------------------------------------------------------------------------------

	void* x_memchr(void* buf, s32 chr, s32 count)
	{
		xbyte* p;
		xbyte  c;

		ASSERT(buf != NULL);
		ASSERT(count >= 0);

		p = (xbyte*)buf;
		c = (xbyte)chr;

		while (count && (*p != c))
		{
			p++;
			count--;
		}

		return count ? (void*)p : NULL;
	}

	//------------------------------------------------------------------------------

	char* x_strcat(char* front, s32 maxChars, const char* back)
	{
		ASSERT(front != NULL);
		ASSERT(back != NULL);

		char* p = front;
		while (*p) 
		{
			p++;
		}
		ASSERT(maxChars > (s32)(p - front));

		while ((*p++ = *back++) != 0)
		{
			ASSERT(maxChars > (s32)(p - front));
		}

		ASSERT(maxChars >= (s32)(p - front));

		return front;
	}


	//------------------------------------------------------------------------------
	f32  x_strtof(const char *s, const char **scan_end)
	{
		// Evaluate sign 
		s32 sign = 1;
		if (*s == '-')
		{
			sign = -1;
			s++;
		}

		// Skip trailing zeros 
		while (*s == '0')
			s++;

		// Convert integer part 
		f32 result = 0;
		f32 value;
		while (*s <= '9' && *s >= '0')
		{
			value = (f32)(*s++ - '0');
			result *= 10.0f;
			result += value;
		}

		// Detect floating point & mantissa 
		f32 mantissa = 0;
		f32 divisor = 1;
		if (*s == '.')
		{
			s++;
			while (*s <= '9' && *s >= '0')
			{
				value = (f32)(*s++ - '0');
				mantissa *= 10.0f;
				mantissa += value;
				divisor *= 10.0f;
			}
		}
		mantissa /= divisor;
		// Adjust result 
		result += mantissa;
		// Adjust sign 
		result *= sign;
		// Detect exponent 
		u16 power = 0;
		if (*s == 'e' || *s == 'E')
		{
			s++;
			if (*s == '-')
			{
				sign = -1;
				s++;
			}
			else if (*s == '+')
			{
				sign = 1;
				s++;
			}
			else
				sign = 1;

			while (*s <= '9' && *s >= '0') 
			{
				u16 v = (*s++ - '0');
				power *= 10;
				power += v;
			}
		}

		// Adjust result on exponent sign 
		if (sign > 0)
			for (s32 i = 0; i < power; i++)
				result *= 10.0f;
		else 
			for (s32 i = 0; i < power; i++)
				result /= 10.0f;

		if (scan_end != 0)
			*scan_end = (char*)s;

		return(result);
	}


	//------------------------------------------------------------------------------

	f64	 x_strtod(const char *s, const char **scan_end)
	{
		// Evaluate sign 
		s32 sign = 1;
		if (*s == '-')
		{
			sign = -1;
			s++;
		}

		// Skip trailing zeros 
		while (*s == '0')
			s++;

		// Convert integer part 
		f64 result = 0;
		f64 value;
		while (*s <= '9' && *s >= '0')
		{
			value = *s++ - '0';
			result *= 10.0;
			result += value;
		}

		// Detect floating point & mantissa 
		f64 mantissa = 0;
		f64 divisor = 1;
		if (*s == '.')
		{
			s++;
			while (*s <= '9' && *s >= '0')
			{
				value = *s++ - '0';
				mantissa *= 10.0;
				mantissa += value;
				divisor *= 10.0;
			}
		}
		mantissa /= divisor;
		// Adjust result 
		result += mantissa;
		// Adjust sign 
		result *= sign;
		// Detect exponent 
		u16 power = 0;
		if (*s == 'e' || *s == 'E')
		{
			s++;
			if (*s == '-')
			{
				sign = -1;
				s++;
			}
			else if (*s == '+')
			{
				sign = 1;
				s++;
			}
			else
				sign = 1;

			while (*s <= '9' && *s >= '0') 
			{
				u16 v = *s++ - '0';
				power *= 10;
				power += v;
			}
		}

		// Adjust result on exponent sign 
		if (sign > 0)
			for (s32 i = 0; i < power; i++)
				result *= 10.0;
		else 
			for (s32 i = 0; i < power; i++)
				result /= 10.0;

		if (scan_end != 0)
			*scan_end = (char*)s;

		return(result);
	}

	/**
	 * Assumption: Letters A-Z and a-z are contiguous in the character set.
	 * This is xTRUE for ASCII and UniCode.  (Not so for EBCDIC!)
	 */
	char* x_strtoupper(char* str)
	{
		ASSERT(str != NULL);

		char* p = str;
		while (*p != '\0')
		{
			if ((*p >= 'a') && (*p <= 'z'))
				*p += ('A' - 'a');
			p++;
		}

		return str;
	}

	/**
	 * Assumption: Letters A-Z and a-z are contiguous in the character set.
	 * This is xTRUE for ASCII and UniCode.  (Not so for EBCDIC!)
	 */
	char* x_strtolower(char* str)
	{
		ASSERT(str != NULL);

		char* p = str;
		while (*p != '\0')
		{
			if ((*p >= 'A') && (*p <= 'Z'))
				*p += ('a' - 'A');
			p++;
		}

		return str;
	}


	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     x_strHash - perform a 32 bit Fowler/Noll/Vo hash on a string
	 * Arguments:
	 *        str     - string to hash
	 *        val     - previous hash value or nothing if first call
	 * Returns:
	 *        32 bit hash as a static hash type
	 * Description:
	 *      FNV hashes are designed to be fast while maintaining a low
	 *      collision rate. The FNV speed allows one to quickly hash lots
	 *      of data while maintaining a reasonable collision rate.  See:
	 *      http: *www.isthe.com/chongo/tech/comp/fnv/index.html
	 *      for more details as well as other forms of the FNV hash.
	 * See Also:
	 *     x_memHash
	 *------------------------------------------------------------------------------
	 */
	u32 x_strHash(const char* str, u32 range, u32 val)
	{
		const u8* s = (const u8*)str;    // unsigned string 

		//
		// FNV-1 hash each octet in the buffer
		//
		while (*s) 
		{
			//.. multiply by the 32 bit FNV magic prime mod 2^32 
			val *= 0x01000193;

			// xor the bottom with the current octet 
			val ^= (s32)*s++;
		}

		// Do we need to compute a different range?
		if (range != 0xffffffff)
		{
			const u32 retryLevel = (0xffffffff / range) * range;
			while (val >= retryLevel) 
			{
				val = (val * ((u32)16777619)) + (X_CONSTANT_U32(2166136261));
			}
			val %= range;
		}

		// return our new hash value 
		return val;
	}

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     x_strIHash - perform a 32 bit Fowler/Noll/Vo hash on a string( ignore upper case or down case)
	 * Arguments:
	 *        str     - string to hash
	 *        val     - previous hash value or nothing if first call
	 * Returns:
	 *        32 bit hash as a static hash type
	 * Description:
	 *      FNV hashes are designed to be fast while maintaining a low
	 *      collision rate. The FNV speed allows one to quickly hash lots
	 *      of data while maintaining a reasonable collision rate.  See:
	 *      http: *www.isthe.com/chongo/tech/comp/fnv/index.html
	 *      for more details as well as other forms of the FNV hash.
	 * See Also:
	 *     x_memHash
	 *------------------------------------------------------------------------------
	 */
	u32 x_strIHash (const char* str, u32 range, u32 val)
	{
		const u8* s = (const u8*)str;    // unsigned string 

		//
		// FNV-1 hash each octet in the buffer
		//
		while (*s) 
		{
			//.. multiply by the 32 bit FNV magic prime mod 2^32 
			val *= 0x01000193;

			// xor the bottom with the current octet 
			val ^= (s32)x_tolower(*s);

			// move to next character
			s++;
		}

		// Do we need to compute a different range?
		if (range != 0xffffffff)
		{
			const u32 retryLevel = (0xffffffff / range) * range;
			while (val >= retryLevel) 
			{
				val = (val * ((u32)16777619)) + (X_CONSTANT_U32(2166136261));
			}
			val %= range;
		}

		// return our new hash value 
		return val;
	}

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     x_strCRC - calculate and return crc of a string
	 * Arguments:
	 *        str     - string to calculate
	 *        crcSum  - provided to affect crc calculate
	 * Returns:
	 *        32 bit crc of the string
	 * See Also:
	 *     x_memCrc
	 *------------------------------------------------------------------------------
	 */

	bool		x_strStartsWith			(const char* inStr, char inStartChar)
	{
		return inStr[0] == inStartChar;
	}

	bool		x_strStartsWith			(const char* inStr, const char* inStartStr)
	{
		// Match from begin of string
		for (s32 i=0; xTRUE; i++)
		{
			const char c1 = inStartStr[i];
			if (c1 == '\0')
				break;
			const char c2 = inStr[i];
			if (c1 != c2)
				return xFALSE;
		}

		// If matched all the way to the end of inStartStr[i] then success
		return xTRUE;
	}

	bool		x_strStartsWith			(const char* inStr, s32 inStrLen, const char* inStartStr, s32 inStartStrLen)
	{
		if (inStartStrLen>inStrLen)
			return xFALSE;

		s32 minimumLen = x_intu::min(inStrLen, inStartStrLen==-1 ? inStrLen : inStartStrLen);
		for (s32 i=0; i<minimumLen; i++)
		{
			const char c1 = inStartStr[i];
			if (c1 == '\0')
				return i>0;
			const char c2 = inStr[i];
			if (c1 != c2)
				return xFALSE;
		}

		// If matched all the way to the end of inStartStr[i] then success
		return xTRUE;
	}


	bool		x_strEndsWith			(const char* inStr, char inEndChar)
	{
		return x_strEndsWith(inStr, -1, inEndChar);
	}

	bool		x_strEndsWith			(const char* inStr, s32 inStrLen, char inEndChar)
	{
		if (inStrLen == -1)
			inStrLen = x_strlen(inStr);
		return inStrLen>0 ? xbool(inStr[inStrLen-1] == inEndChar) : (xbool)xFALSE;
	}


	bool		x_strEndsWith			(const char* inStr, const char* inEndStr)
	{
		return x_strEndsWith(inStr, -1, inEndStr, -1);
	}

	bool		x_strEndsWith			(const char* inStr, s32 inStrLen, const char* inEndStr, s32 inEndStrLength)
	{
		// Grab string lengths
		const s32 l0 = inStrLen==-1 ? x_strlen(inStr) : inStrLen;
		const s32 l1 = inEndStrLength==-1 ? x_strlen(inEndStr) : inEndStrLength;

		if (l0 < l1)
			return xFALSE;

		// Match from end of string
		s32 index = l0-l1;
		for (s32 i=0; i<l1; i++)
		{
			if (inStr[index+i] != inEndStr[i])
				return xFALSE;
		}
		return xTRUE;
	}

	char 		x_strFirstChar			(const char* inStr)
	{
		ASSERT(inStr!=NULL);
		return inStr[0];
	}

	char 		x_strLastChar			(const char* inStr, s32 inStrLen)
	{
		ASSERT(inStr!=NULL);
		inStrLen = (inStrLen==-1) ? x_strlen(inStr) : inStrLen;
		return inStrLen==0 ? '\0' : inStr[inStrLen-1];
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
	 *     inStrLen             - The whole(-1) or sub length of the main string
	 *     inOtherLen           - The whole(-1) or sub length of the 'compare' string
	 * Returns:
	 *     s32 - 0 when inStr==inOther, 1 when inStr>inOther, -1 when inStr<inOther
	 * Description:
	 *     This function can handle a bit more than traditional string compare, it can
	 *     identify the following cases:
	 *
	 *     1) Main string sub length, 'Compare' string sub length
	 *			Required: need to supply both inStrLen or inOtherLen
	 *			Action:   strings are compared until inStrLen==0 or inOtherLen==0
	 *
	 *     2) Main string sub length, 'Compare' string total length = -1
	 *			Required: need to supply inStrLen, no need to supply inOtherLen=-1
	 *			Action:   strings are compared until inStrLen==0 or 'Compare' string encounters '\0'
	 *
	 *     3) Main string total length = -1, 'Compare' string sub length
	 *			Required: need to supply inStrLen with -1, need to supply inOtherLen
	 *			Action:   strings are compared until inOtherLen==0 or 'Compare' string encounters '\0'
	 *
	 *     4) Main string total length = -1, 'Compare' string total length = -1
	 *			Required: no need to supply either inStrLen or inOtherLen
	 *			Action:   strings are compared until one of the strings encounters '\0'
	 *

	 * See Also:
	 *     x_SetAssertHandler
	 *------------------------------------------------------------------------------
	 */

	s32  		x_strCompare			(const char* inStr, s32 inStrLen, const char* inOther, s32 inOtherLen)
	{
		if ( inStrLen == 0 && inOtherLen == 0 )
			return 0;

		if ( inStrLen == 0 && inOtherLen != 0 )
			return 1;

		if ( inStrLen != 0 && inOtherLen == 0 )
			return -1;

		if (inStrLen>0 && inOtherLen>0)
		{
			// 1/ x_strCompare("aaaa", "aabb", 2, 2) -> return  0;
			// 2/ x_strCompare("aaaa", "aabb", 3, 3) -> return -1;
			// 3/ x_strCompare("aaaa", "aabb", 2, 4) -> return  0;
			// 4/ x_strCompare("aaaa", "aabb", 4, 2) -> return  0;
			// 5/ x_strCompare("aaaa", "aabb", 3, 4) -> return -1;
			// 6/ x_strCompare("aaaa", "aabb", 4, 3) -> return -1;
			while ((*inStr == *inOther))
			{
				ASSERT(*inStr != '\0');
				ASSERT(*inOther != '\0');
				--inStrLen;
				--inOtherLen;
				if (inStrLen==0)	return (inOtherLen==0) ? 0 : -1;
				if (inOtherLen==0)	return 1;
				++inStr;
				++inOther;
			}
		}
		else if (inStrLen>0)
		{
			// 1/ x_strCompare("aaaa", "aabb", 2) -> return  0;
			// 2/ x_strCompare("aaaa", "aabb", 3) -> return -1;
			// 3/ x_strCompare("aaaa", "aa"  , 3) -> return  0;
			// 4/ x_strCompare("aaaa", "aabb", 4) -> return -1;
			// 5/ x_strCompare("aaaa", "aabb", 3) -> return -1;

			while ((*inStr == *inOther))
			{
				ASSERT(*inStr != '\0');
				--inStrLen;
				if (*inOther=='\0')	break;
				if (inStrLen==0)	break;
				++inStr;
				++inOther;
			}
		}
		else if (inOtherLen>0)
		{
			// 1/ x_strCompare("aaaa", "aabb", -1, 2) -> return  0;
			// 2/ x_strCompare("aaaa", "aabb", -1, 3) -> return -1;
			// 4/ x_strCompare("aaaa", "aabb", -1, 4) -> return -1;
			// 5/ x_strCompare("aaaa", "aabb", -1, 3) -> return -1;

			inStrLen = inOtherLen;
			while ((*inStr == *inOther))
			{
				ASSERT(*inOther != '\0');
				--inStrLen;
				if (*inStr=='\0')	break;
				if (inStrLen==0)	break;
				++inStr;
				++inOther;
			}
		}
		else
		{
			// 1/ x_strCompare("aaaa", "aa"  ) -> return  1;
			// 2/ x_strCompare("aa"  , "aaaa") -> return -1;
			// 3/ x_strCompare("ab"  , "aaaa") -> return  1;
			// 4/ x_strCompare("aaaa", "aaaa") -> return  0;
			// 5/ x_strCompare("aaaa", "aabb") -> return -1;
			// 6/ x_strCompare("aabb", "aaaa") -> return  1;
			inStrLen=0;
			while ((*inStr == *inOther))
			{
				if (*inStr=='\0')	break;
				if (*inOther=='\0')	break;
				++inStr;
				++inOther;
			}
		}

		s32 result = *inStr - *inOther;
		if (result==0)		return 0;
		else if (result<0)	return -1;
		else				return 1;
	}

	s32  		x_strCompareNoCase		(const char* inStr, s32 inStrLen, const char* inOther, s32 inOtherLen)
	{
		if (inStrLen == 0 || inOtherLen == 0)
			return 0;

		if (inStrLen>0 && inOtherLen>0)
		{
			while (x_tolower(*inStr) == x_tolower(*inOther))
			{
				ASSERT(*inStr != '\0');
				ASSERT(*inOther != '\0');
				--inStrLen;
				--inOtherLen;
				if (inStrLen==0)	return (inOtherLen==0) ? 0 : -1;
				if (inOtherLen==0)	return 1;
				++inStr;
				++inOther;
			}
		}
		else if (inStrLen>0)
		{
			while (x_tolower(*inStr) == x_tolower(*inOther))
			{
				--inStrLen;
				if (*inOther=='\0')	break;
				if (inStrLen==0)	break;
				ASSERT(*inStr != '\0');
				++inStr;
				++inOther;
			}
		}
		else if (inOtherLen>0)
		{
			inStrLen = inOtherLen;
			while (x_tolower(*inStr) == x_tolower(*inOther))
			{
				--inStrLen;
				if (*inStr=='\0')	break;
				if (inStrLen==0)	break;
				ASSERT(*inOther != '\0');
				++inStr;
				++inOther;
			}
		}
		else
		{
			inStrLen=0;
			while (x_tolower(*inStr) == x_tolower(*inOther))
			{
				if (*inStr=='\0')	break;
				if (*inOther=='\0')	break;
				++inStr;
				++inOther;
			}
		}

		s32 result = x_tolower(*inStr) - x_tolower(*inOther);
		if (result==0)		return 0;
		else if (result<0)	return -1;
		else				return 1;
	}

	bool		x_strEqual				(const char* inStr, s32 inStrLen, const char* inOther, s32 inOtherLen)
	{
		return x_strCompare(inStr, inStrLen, inOther, inOtherLen) == 0;
	}

	bool		x_strEqualNoCase		(const char* inStr, s32 inStrLen, const char* inOther, s32 inOtherLen)
	{
		return x_strCompareNoCase(inStr, inStrLen, inOther, inOtherLen) == 0;
	}


	const char*	x_strFind				(const char* inStr, char inChar, s32 inStrLen)
	{
		while (*inStr!='\0')
		{
			if (inStrLen==0)
				break;
			--inStrLen;
			if (*inStr == inChar)
				return inStr;
			++inStr;
		}
		return NULL;
	}

	const char*	x_strFind				(const char* inStr, const char* inFind, s32 inStrLen)
	{
		ASSERT(inStr);
		// If we don't have a valid string then we didn't find anything
		if (inStr == NULL) 
			return NULL;

		const char* mainString = inStr;
		while (*mainString!='\0')
		{
			if (inStrLen==0)
				break;
			--inStrLen;

			const char* str1 = mainString;
			const char* str2 = inFind;

			while (*str1!='\0' && *str2!='\0' && (*str1==*str2))
			{
				str1++;
				str2++;
			}

			if (!*str2)
				return mainString;

			mainString++;
		}

		return NULL;
	}

	const char*	x_strRFind				(const char* inStr, char inFind, s32 inPos, s32 inStrLen)
	{
		char find[2];
		find[0] = inFind;
		find[1] = '\0';
		return x_strRFind(inStr, find, inPos, inStrLen);
	}

	const char*	x_strRFind				(const char* inStr, const char* inFind, s32 inPos, s32 inStrLen)
	{
		inStrLen = (inStrLen==-1) ? (x_strlen(inStr)-inPos) : (inStrLen);
		if (inStrLen<=0)
			return NULL;

		if (inPos == -1)
			inPos = inStrLen-1;

		if ((inPos-(inStrLen-1)) >= 0)
		{
			const s32 findLen = x_strlen(inFind);
			if (findLen > 0)
			{
				const char* end = inStr + inPos - (inStrLen-1);
				const char* pos = inStr + inPos;
				while (pos >= end)
				{
					s32 len = inStrLen;
					const char* poss = pos;
					const char* find = inFind + (findLen-1);
					while (poss >= end && *poss == *find)
					{
						if (find == inFind)
							return poss;
						--find;
						--poss;
						--len;
						if (len==0)
							break;
					}

					--pos;
					--inStrLen;
					if (inStrLen==0)
						break;
				}

				return (pos >= end)? pos : NULL;
			}
		}
		return NULL;
	}

	const char*	x_strFindNoCase			(const char* inStr, char inChar, s32 inStrLen)
	{
		inChar = (char)x_tolower(inChar);
		while (*inStr!='\0')
		{
			if (inStrLen == 0)
				break;
			--inStrLen;

			if (x_tolower(*inStr) == inChar)
				return inStr;
			++inStr;
		}
		return NULL;
	}

	const char*	x_strFindNoCase			(const char* inStr, const char* inFind, s32 inStrLen)
	{
		ASSERT(inStr);
		// If we don't have a valid string then we didn't find any thing
		if (inStr == NULL) 
			return NULL;

		const char* mainString = inStr;
		while (*mainString)
		{
			if (inStrLen==0)
				break;
			--inStrLen;

			const char* str1 = mainString;
			const char* str2 = inFind;

			while (*str1 && *str2 && !(x_tolower(*str1) - x_tolower(*str2)))
			{
				str1++;
				str2++;
			}

			if (!*str2)
				return mainString;

			mainString++;
		}

		return NULL;
	}

	const char*	x_strFindInSubstr		(const char* inStr, char inFind, s32 inPos, s32 inStrLen)
	{
		ASSERT(inPos >= 0);
		const char* findPos = x_strFind(inStr + inPos, inFind, inStrLen);
		return findPos;
	}

	const char*	x_strFindInSubstr		(const char* inStr, const char* inFind, s32 inPos, s32 inStrLen)
	{
		ASSERT(inPos >= 0);
		const char* findPos = x_strFind(inStr + inPos, inFind, inStrLen);
		return findPos;
	}

	const char*	x_strFindNoCaseInSubstr	(const char* inStr, char inFind, s32 inPos, s32 inStrLen)
	{
		ASSERT(inPos >= 0);
		const char* findPos = x_strFindNoCase(inStr + inPos, inFind, inStrLen);
		return findPos;
	}

	const char*	x_strFindNoCaseInSubstr	(const char* inStr, const char* inFind, s32 inPos, s32 inStrLen)
	{
		ASSERT(inPos >= 0);
		const char* findPos = x_strFindNoCase(inStr + inPos, inFind, inStrLen);
		return findPos;
	}

	const char*	x_strFindOneOf			(const char* inStr, const char* inCharSet, s32 inPos, s32 inStrLen)
	{
		inStrLen = inStrLen==-1 ? (x_strlen(inStr)) : (inStrLen);
		if (inStrLen<=0)
			return NULL;

		const char* cur_pos = inStr + inPos;
		const char* end_pos = inStr + inStrLen;
		while (cur_pos<end_pos)
		{
			if (x_strFind(inCharSet, *cur_pos)!=NULL)
				return cur_pos;

			++cur_pos;
		}

		return NULL;
	}

	const char*	x_strRFindOneOf			(const char* inStr, const char* inCharSet, s32 inPos, s32 inStrLen)
	{
		inStrLen = inStrLen==-1 ? (x_strlen(inStr)) : (inStrLen);
		if (inStrLen<=0)
			return NULL;

		if (inPos==-1)
			inPos = inStrLen - 1;

		ASSERT((inPos-(inStrLen-1)) >= 0);

		const char* cur_pos = inStr + inPos;
		const char* end_pos = inStr;
		while (cur_pos>=end_pos)
		{
			if (x_strFind(inCharSet, *cur_pos)!=NULL)
				return cur_pos;
			--cur_pos;
			--inStrLen;
			if (inStrLen==0)
				break;
		}

		return NULL;
	}



	bool		x_strIsUpper			(const char* inStr, s32 inStrLen)
	{
		const char* buffer = inStr;
		if (buffer == NULL)
			return xFALSE;

		bool isUpper = xTRUE;
		for (s32 i=0; isUpper; i++, --inStrLen)
		{
			char c = buffer[i];
			if (c == 0 || inStrLen == 0)
			{
				break;
			}
			isUpper = isUpper && !x_islower(c);
		}

		return isUpper;
	}

	bool		x_strIsLower			(const char* inStr, s32 inStrLen)
	{
		const char* buffer = inStr;
		if (buffer == NULL)
			return xFALSE;

		bool isLower = xTRUE;
		for (s32 i=0; isLower; i++, --inStrLen)
		{
			char c = buffer[i];
			if (c == 0 || inStrLen==0)
			{
				break;
			}
			isLower = isLower && !x_isupper(c);
		}

		return isLower;
	}

	bool		x_strIsCapitalized		(const char* inStr, s32 inStrLen)
	{
		if (inStr[0]!='\0')
		{
			if (x_isupper(inStr[0]))
			{
				return x_strIsLower(&inStr[1], inStrLen);
			}
		}
		return xFALSE;
	}

	bool		x_strIsQuoted			(const char* inStr, char quote, s32 inStrLen)
	{
		if (inStr[0]!='\0' && inStr[1]!='\0' && inStr[0]==quote)
		{
			inStrLen = (inStrLen==-1) ? x_strlen(inStr) : inStrLen;
			ASSERT(inStrLen>1);
			return inStr[inStrLen-1]==quote;
		}
		return xFALSE;
	}





	 s32		x_printf   				(const char* formatStr, const x_va& v1             , const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, 
																	const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16) { return xconsole::write(formatStr,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16); }
	 s32		x_printf				(const char* formatStr, const x_va_list& args)		{ return xconsole::write(formatStr, args); }
	 s32		x_printf				(const char* str)									{ return xconsole::write(str); }


};

/**
 *  END xCore namespace
 */


 #endif // ifndef SPU