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
	namespace ascii
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
	 *     atod64          - Converts a string value to integer base from a particular
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
	 *      atod32 atoi32 atoi64 atof32 atof64
	 *------------------------------------------------------------------------------
	 */
	s64 atod64(pcrune str, s32 base)
	{
		ASSERT(str != NULL);
		ASSERT(base > 2);
		ASSERT(base <= (26 + 26 + 10));

		// Skip whitespace.
		for (; *str == ' '; ++str)
			; // empty body

		rune c;         // Current character.
		c = *str++;		// Save sign indication.

		rune sign;        // If '-', then negative, otherwise positive.
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

	// <COMBINE atod64 >
	s32 atod32(pcrune str, s32 base)
	{
		return (s32)atod64(str, base);
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
	 *     _dtoa    - Converts value to a string in whatever base is passed
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
	s32 d64toa(u64 val, prune buff, s32 str_buffer_size, s32 base, bool hasNegative)
	{
		ASSERT(buff);
		ASSERT(base > 2);
		ASSERT(base <= (26 + 26 + 10));
		ASSERT(str_buffer_size > 1);

		prune p      = buff;
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
				*p = (rune)(cVal + '0');
				p++;
			}
			else if (cVal < (10 + 26))
			{
				*p = (rune)(cVal - 10 + 'a');
				p++;
			}
			else if (cVal < (10 + 26 + 26))
			{
				*p = (rune)(cVal - 10 - 26 + 'A');
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
			rune t = *p;
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
	 *     dtoa    - Converts value to a string in whatever base is passed
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
	 *     atod64 atod32 atoi32 atoi64 atof32 atof64
	 *------------------------------------------------------------------------------
	 */
	s32 d32toa(s32 val, prune str, s32 str_buffer_size, s32 base)
	{
		if (val < 0)
		{
			return d64toa(u64(u32(-val)), str, str_buffer_size, base, xTRUE);
		}
		else
		{
			return d64toa(u64(val), str, str_buffer_size, base, xFALSE);
		}
	}

	// <COMBINE dtoa >
	s32 d32toa(u32 val, prune str, s32 str_buffer_size, s32 base)
	{
		return d64toa(u64(val), str, str_buffer_size, base, xFALSE);
	}

	// <COMBINE dtoa >
	s32 d64toa(s64 val, prune str, s32 str_buffer_size, s32 base)
	{
		if (val < 0)
		{
			return  d64toa(u64(-val), str, str_buffer_size, base, xTRUE);
		}
		else
		{
			return  d64toa(u64(val), str, str_buffer_size, base, xFALSE);
		}
	}

	// <COMBINE dtoa >
	s32 d64toa(u64 val, prune str, s32 str_buffer_size, s32 base)
	{
		return d64toa(u64(val), str, str_buffer_size, base, xFALSE);
	}


	#define F32TOA_PSH(X)				(*(dest++)=(X))

	s32			f32toa(f32 val, s32 prec, prune str, s32 str_buffer_size, xbool positive_sign, rune format)
	{
		/* converts a floating point number to an ascii string */
		/* x is stored into str, which should be at least 30 chars long */
		int i, k;
		double y;

		s32 fstyle = 0;
		s32 ndig = ( prec<=0) ? 7 : (prec > 22 ? 23 : prec+1);
		if  (format == 'f' || format == 'F')
			fstyle = 1;

		prune start = str;
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
	s32			f32toa(f32 val, s32 prec, prune str, s32 str_buffer_size)
	{
		return f32toa(val, prec, str, str_buffer_size, true, 'f');
	}


	s32			f64toa(f64 val, s32 numFractionalDigits, prune str, s32 str_buffer_size)
	{
		rune format[8];
		Sprintf(format, sizeof(format)-1, "%%.%df", x_va(numFractionalDigits));
		s32 len = Sprintf(str, str_buffer_size-1, format, x_va(val));
		return len;
	}

	//------------------------------------------------------------------------------

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
	prune Copy(prune dest, s32 dest_buffer_size, pcrune src)
	{
		ASSERT(dest);
		ASSERT(src != NULL);
		ASSERT(dest_buffer_size >= 0);

		if (dest_buffer_size == 0)
			return dest;

		prune start = dest;
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

	s32 Len(pcrune str)
	{
		ASSERT(str != NULL);
		const rune *end = str;

		while (*end++) 
		{
			//Empty
		}
		return (s32)(end - str - 1);
	}
	
	//------------------------------------------------------------------------------

	s32 Cmp(pcrune str1, pcrune str2)
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

	s32	Cmpn(const rune * cs, const rune * ct, s32 n)
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

	s32 Cmpi(pcrune str1, pcrune str2)
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

	s32 Cmpin(const rune *s1, const rune *s2, s32 count)
	{
		s32 f, l;

		do 
		{
			if (((f = (rune)(*(s1++))) >= 'A') && (f <= 'Z'))
				f -= 'A' - 'a';
			if (((l = (rune)(*(s2++))) >= 'A') && (l <= 'Z'))
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
	 *     strscn    - scan the string for certain parttern
	 * Arguments:
	 *        parttern - the parttern need to be scanned
	 *        str      - source string        
	 * Returns:
	 *        pointer to the parttern of the source string
	 *------------------------------------------------------------------------------
	 */
	pcrune Str(pcrune mainStr, pcrune subStr)
	{
		prune mainString = (prune)mainStr;
		prune str1; 
		prune str2;

		if (!*subStr)
			return((prune)mainStr);

		while (*mainString)
		{
			str1 = mainString;
			str2 = (prune)subStr;

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

	pcrune		Chr(pcrune mainStr, rune c)
	{
		prune mainString = (prune)mainStr;

		while (*mainString)
		{
			if (*mainString == c)
				return(mainString);
			++mainString;
		}
		return NULL;
	}

	//------------------------------------------------------------------------------

	pcrune		ChrR(pcrune mainStr, rune c)
	{
		prune mainString = (prune)mainStr;

		s32 i = Len(mainString);
		while (--i >= 0)
		{
			if (mainString[i] == c)
				return(&mainString[i]);
		}
		return NULL;
	}

	//------------------------------------------------------------------------------

	pcrune        Scn(pcrune str, pcrune parttern)
	{
		const rune *scan;

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
	 *         rune *s; f64 x; s32 i; s64 li;
	 *
	 *           * Test of atof64
	 *          s = "  -2309.12E-15";     
	 *          x = atof64(s);
	 *          printf("atof64 test: \\"%s\\"; f32:  %e\\n", s, x);
	 *
	 *           * Test of atof32 
	 *          s = "7.8912654773d210";  
	 *          x = atof32(s);
	 *          printf("atof32 test: \\"%s\\"; f32:  %e\\n", s, x);
	 *
	 *           * Test of atoi 
	 *          s = "  -9885 pigs";      
	 *          i = atoi32(s);
	 *          printf("atoi32 test: \\"%s\\"; integer: %d\\n", s, i);
	 *
	 *           * Test of atol 
	 *          s = "98854 dollars";     
	 *          li = atoi64(s);
	 *          printf("atoi64 test: \\"%s\\"; long: %Ld\\n", s, li);
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
	 *     sprintf
	 *------------------------------------------------------------------------------
	 */
	s32 atoi32(pcrune str)
	{
		pcrune end;
		return atoi32(str, &end, 10);
	}

	//------------------------------------------------------------------------------
	s32 atoi32(pcrune str, pcrune* scanEnd, s32 base)
	{
		const rune *p = (const rune *)str;
		u32 res = 0;
		s32 negative = 0;

		// skip initial white spaces
		while (*p != '\0' && *p <= ' ')
			++p;

		if (p[0] == '\0')
		{
			if (scanEnd)
				*scanEnd = (const rune *)p;
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
	/// <COMBINE atoi32 >
	s64 atoi64(pcrune str)
	{
		pcrune end;
		return atoi64(str, 10, &end);
	}

	//------------------------------------------------------------------------------
	s64 atoi64(pcrune str, s32 base, pcrune* scanEnd)
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
		while (IsNumber(*str, base))
		{
			s64    oldresult;
			val = ToNumber(*str++);
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

	bool IsNumber(rune c, s32 base)
	{
		const rune max10 = (rune)((base > 9) ? '9' : ('0' + base - 1));

		if (c >= '0' && c <= max10)
			return xTRUE;

		if (base < 11)
			return xFALSE;

		const rune max = (rune)('A' + (base - 11));

		c = (rune)ToUpper(c);
		return (c >= 'A' && c <= max);
	}

	//------------------------------------------------------------------------------

	bool IsInt(pcrune str)
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

			// Okay this is not longer a number
			if (floatStr[l] == 0) return xFALSE;
		}

		return xTRUE;
	}

	//------------------------------------------------------------------------------

	bool IsHex(pcrune str)
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

	prune Cat(prune front, s32 maxChars, pcrune back)
	{
		ASSERT(front != NULL);
		ASSERT(back != NULL);

		prune p = front;
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
	f32  atof32(const rune *s, const rune **scan_end)
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
			*scan_end = (prune)s;

		return(result);
	}


	//------------------------------------------------------------------------------

	f64	 atof64(const rune *s, const rune **scan_end)
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
			*scan_end = (prune)s;

		return(result);
	}

	/**
	 * Assumption: Letters A-Z and a-z are contiguous in the character set.
	 * This is xTRUE for ASCII and UniCode.  (Not so for EBCDIC!)
	 */
	prune ToUpper(prune str)
	{
		ASSERT(str != NULL);

		prune p = str;
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
	prune ToLower(prune str)
	{
		ASSERT(str != NULL);

		prune p = str;
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
	 *     strHash - perform a 32 bit Fowler/Noll/Vo hash on a string
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
	 *     memHash
	 *------------------------------------------------------------------------------
	 */
	u32 Hash32(pcrune str, u32 range, u32 val)
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

	bool		StartsWith			(pcrune inStr, rune inStartChar)
	{
		return inStr[0] == inStartChar;
	}

	bool		StartsWith			(pcrune inStr, pcrune inStartStr)
	{
		// Match from begin of string
		for (s32 i=0; xTRUE; i++)
		{
			const rune c1 = inStartStr[i];
			if (c1 == '\0')
				break;
			const rune c2 = inStr[i];
			if (c1 != c2)
				return xFALSE;
		}

		// If matched all the way to the end of inStartStr[i] then success
		return xTRUE;
	}

	bool		StartsWith			(pcrune inStr, s32 inLen, pcrune inStartStr, s32 inStartLen)
	{
		if (inStartLen>inLen)
			return xFALSE;

		s32 minimumLen = x_intu::min(inLen, inStartLen==-1 ? inLen : inStartLen);
		for (s32 i=0; i<minimumLen; i++)
		{
			const rune c1 = inStartStr[i];
			if (c1 == '\0')
				return i>0;
			const rune c2 = inStr[i];
			if (c1 != c2)
				return xFALSE;
		}

		// If matched all the way to the end of inStartStr[i] then success
		return xTRUE;
	}


	bool		EndsWith			(pcrune inStr, rune inEndChar)
	{
		return EndsWith(inStr, -1, inEndChar);
	}

	bool		EndsWith			(pcrune inStr, s32 inLen, rune inEndChar)
	{
		if (inLen == -1)
			inLen = Len(inStr);
		return inLen>0 ? xbool(inStr[inLen-1] == inEndChar) : (xbool)xFALSE;
	}


	bool		EndsWith			(pcrune inStr, pcrune inEndStr)
	{
		return EndsWith(inStr, -1, inEndStr, -1);
	}

	bool		EndsWith			(pcrune inStr, s32 inLen, pcrune inEndStr, s32 inEndLength)
	{
		// Grab string lengths
		const s32 l0 = inLen==-1 ? Len(inStr) : inLen;
		const s32 l1 = inEndLength==-1 ? Len(inEndStr) : inEndLength;

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

	rune 		FirstChar			(pcrune inStr)
	{
		ASSERT(inStr!=NULL);
		return inStr[0];
	}

	rune 		LastChar			(pcrune inStr, s32 inLen)
	{
		ASSERT(inStr!=NULL);
		inLen = (inLen==-1) ? Len(inStr) : inLen;
		return inLen==0 ? '\0' : inStr[inLen-1];
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

	s32  		Compare			(pcrune inStr, s32 inLen, pcrune inOther, s32 inOtherLen)
	{
		if ( inLen == 0 && inOtherLen == 0 )
			return 0;

		if ( inLen == 0 && inOtherLen != 0 )
			return 1;

		if ( inLen != 0 && inOtherLen == 0 )
			return -1;

		if (inLen>0 && inOtherLen>0)
		{
			// 1/ Compare("aaaa", "aabb", 2, 2) -> return  0;
			// 2/ Compare("aaaa", "aabb", 3, 3) -> return -1;
			// 3/ Compare("aaaa", "aabb", 2, 4) -> return  0;
			// 4/ Compare("aaaa", "aabb", 4, 2) -> return  0;
			// 5/ Compare("aaaa", "aabb", 3, 4) -> return -1;
			// 6/ Compare("aaaa", "aabb", 4, 3) -> return -1;
			while ((*inStr == *inOther))
			{
				ASSERT(*inStr != '\0');
				ASSERT(*inOther != '\0');
				--inLen;
				--inOtherLen;
				if (inLen==0)	return (inOtherLen==0) ? 0 : -1;
				if (inOtherLen==0)	return 1;
				++inStr;
				++inOther;
			}
		}
		else if (inLen>0)
		{
			// 1/ Compare("aaaa", "aabb", 2) -> return  0;
			// 2/ Compare("aaaa", "aabb", 3) -> return -1;
			// 3/ Compare("aaaa", "aa"  , 3) -> return  0;
			// 4/ Compare("aaaa", "aabb", 4) -> return -1;
			// 5/ Compare("aaaa", "aabb", 3) -> return -1;

			while ((*inStr == *inOther))
			{
				ASSERT(*inStr != '\0');
				--inLen;
				if (*inOther=='\0')	break;
				if (inLen==0)	break;
				++inStr;
				++inOther;
			}
		}
		else if (inOtherLen>0)
		{
			// 1/ Compare("aaaa", "aabb", -1, 2) -> return  0;
			// 2/ Compare("aaaa", "aabb", -1, 3) -> return -1;
			// 4/ Compare("aaaa", "aabb", -1, 4) -> return -1;
			// 5/ Compare("aaaa", "aabb", -1, 3) -> return -1;

			inLen = inOtherLen;
			while ((*inStr == *inOther))
			{
				ASSERT(*inOther != '\0');
				--inLen;
				if (*inStr=='\0')	break;
				if (inLen==0)	break;
				++inStr;
				++inOther;
			}
		}
		else
		{
			// 1/ Compare("aaaa", "aa"  ) -> return  1;
			// 2/ Compare("aa"  , "aaaa") -> return -1;
			// 3/ Compare("ab"  , "aaaa") -> return  1;
			// 4/ Compare("aaaa", "aaaa") -> return  0;
			// 5/ Compare("aaaa", "aabb") -> return -1;
			// 6/ Compare("aabb", "aaaa") -> return  1;
			inLen=0;
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

	s32  		CompareNoCase		(pcrune inStr, s32 inLen, pcrune inOther, s32 inOtherLen)
	{
		if (inLen == 0 || inOtherLen == 0)
			return 0;

		if (inLen>0 && inOtherLen>0)
		{
			while (ToLower(*inStr) == ToLower(*inOther))
			{
				ASSERT(*inStr != '\0');
				ASSERT(*inOther != '\0');
				--inLen;
				--inOtherLen;
				if (inLen==0)	return (inOtherLen==0) ? 0 : -1;
				if (inOtherLen==0)	return 1;
				++inStr;
				++inOther;
			}
		}
		else if (inLen>0)
		{
			while (ToLower(*inStr) == ToLower(*inOther))
			{
				--inLen;
				if (*inOther=='\0')	break;
				if (inLen==0)	break;
				ASSERT(*inStr != '\0');
				++inStr;
				++inOther;
			}
		}
		else if (inOtherLen>0)
		{
			inLen = inOtherLen;
			while (ToLower(*inStr) == ToLower(*inOther))
			{
				--inLen;
				if (*inStr=='\0')	break;
				if (inLen==0)	break;
				ASSERT(*inOther != '\0');
				++inStr;
				++inOther;
			}
		}
		else
		{
			inLen=0;
			while (ToLower(*inStr) == ToLower(*inOther))
			{
				if (*inStr=='\0')	break;
				if (*inOther=='\0')	break;
				++inStr;
				++inOther;
			}
		}

		s32 result = ToLower(*inStr) - ToLower(*inOther);
		if (result==0)		return 0;
		else if (result<0)	return -1;
		else				return 1;
	}

	bool		Equal				(pcrune inStr, s32 inLen, pcrune inOther, s32 inOtherLen)
	{
		return Compare(inStr, inLen, inOther, inOtherLen) == 0;
	}

	bool		EqualNoCase			(pcrune inStr, s32 inLen, pcrune inOther, s32 inOtherLen)
	{
		return CompareNoCase(inStr, inLen, inOther, inOtherLen) == 0;
	}


	pcrune		Find(pcrune inStr, rune inChar, s32 inLen)
	{
		while (*inStr!='\0')
		{
			if (inLen==0)
				break;
			--inLen;
			if (*inStr == inChar)
				return inStr;
			++inStr;
		}
		return NULL;
	}

	pcrune		Find(pcrune inStr, pcrune inFind, s32 inLen)
	{
		ASSERT(inStr);
		// If we don't have a valid string then we didn't find anything
		if (inStr == NULL) 
			return NULL;

		pcrune mainString = inStr;
		while (*mainString!='\0')
		{
			if (inLen==0)
				break;
			--inLen;

			pcrune str1 = mainString;
			pcrune str2 = inFind;

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

	pcrune	RFind				(pcrune inStr, rune inFind, s32 inPos, s32 inLen)
	{
		rune find[2];
		find[0] = inFind;
		find[1] = '\0';
		return RFind(inStr, find, inPos, inLen);
	}

	pcrune	RFind				(pcrune inStr, pcrune inFind, s32 inPos, s32 inLen)
	{
		inLen = (inLen==-1) ? (Len(inStr)-inPos) : (inLen);
		if (inLen<=0)
			return NULL;

		if (inPos == -1)
			inPos = inLen-1;

		if ((inPos-(inLen-1)) >= 0)
		{
			const s32 findLen = Len(inFind);
			if (findLen > 0)
			{
				pcrune end = inStr + inPos - (inLen-1);
				pcrune pos = inStr + inPos;
				while (pos >= end)
				{
					s32 len = inLen;
					pcrune poss = pos;
					pcrune find = inFind + (findLen-1);
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
					--inLen;
					if (inLen==0)
						break;
				}

				return (pos >= end)? pos : NULL;
			}
		}
		return NULL;
	}

	pcrune	FindNoCase			(pcrune inStr, rune inChar, s32 inLen)
	{
		inChar = (rune)ToLower(inChar);
		while (*inStr!='\0')
		{
			if (inLen == 0)
				break;
			--inLen;

			if (ToLower(*inStr) == inChar)
				return inStr;
			++inStr;
		}
		return NULL;
	}

	pcrune	FindNoCase			(pcrune inStr, pcrune inFind, s32 inLen)
	{
		ASSERT(inStr);
		// If we don't have a valid string then we didn't find any thing
		if (inStr == NULL) 
			return NULL;

		pcrune mainString = inStr;
		while (*mainString)
		{
			if (inLen==0)
				break;
			--inLen;

			pcrune str1 = mainString;
			pcrune str2 = inFind;

			while (*str1 && *str2 && !(ToLower(*str1) - ToLower(*str2)))
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

	pcrune	FindInSubstr		(pcrune inStr, rune inFind, s32 inPos, s32 inLen)
	{
		ASSERT(inPos >= 0);
		pcrune findPos = Find(inStr + inPos, inFind, inLen);
		return findPos;
	}

	pcrune	FindInSubstr		(pcrune inStr, pcrune inFind, s32 inPos, s32 inLen)
	{
		ASSERT(inPos >= 0);
		pcrune findPos = Find(inStr + inPos, inFind, inLen);
		return findPos;
	}

	pcrune	FindNoCaseInSubstr	(pcrune inStr, rune inFind, s32 inPos, s32 inLen)
	{
		ASSERT(inPos >= 0);
		pcrune findPos = FindNoCase(inStr + inPos, inFind, inLen);
		return findPos;
	}

	pcrune	FindNoCaseInSubstr	(pcrune inStr, pcrune inFind, s32 inPos, s32 inLen)
	{
		ASSERT(inPos >= 0);
		pcrune findPos = FindNoCase(inStr + inPos, inFind, inLen);
		return findPos;
	}

	pcrune	FindOneOf			(pcrune inStr, pcrune inCharSet, s32 inPos, s32 inLen)
	{
		inLen = inLen==-1 ? (Len(inStr)) : (inLen);
		if (inLen<=0)
			return NULL;

		pcrune cur_pos = inStr + inPos;
		pcrune end_pos = inStr + inLen;
		while (cur_pos<end_pos)
		{
			if (Find(inCharSet, *cur_pos)!=NULL)
				return cur_pos;

			++cur_pos;
		}

		return NULL;
	}

	pcrune	RFindOneOf			(pcrune inStr, pcrune inCharSet, s32 inPos, s32 inLen)
	{
		inLen = inLen==-1 ? (Len(inStr)) : (inLen);
		if (inLen<=0)
			return NULL;

		if (inPos==-1)
			inPos = inLen - 1;

		ASSERT((inPos-(inLen-1)) >= 0);

		pcrune cur_pos = inStr + inPos;
		pcrune end_pos = inStr;
		while (cur_pos>=end_pos)
		{
			if (Find(inCharSet, *cur_pos)!=NULL)
				return cur_pos;
			--cur_pos;
			--inLen;
			if (inLen==0)
				break;
		}

		return NULL;
	}



	bool		IsUpper			(pcrune inStr, s32 inLen)
	{
		pcrune buffer = inStr;
		if (buffer == NULL)
			return xFALSE;

		bool isUpper = xTRUE;
		for (s32 i=0; isUpper; i++, --inLen)
		{
			rune c = buffer[i];
			if (c == 0 || inLen == 0)
			{
				break;
			}
			isUpper = isUpper && !IsLower(c);
		}

		return isUpper;
	}

	bool		IsLower			(pcrune inStr, s32 inLen)
	{
		pcrune buffer = inStr;
		if (buffer == NULL)
			return xFALSE;

		bool isLower = xTRUE;
		for (s32 i=0; isLower; i++, --inLen)
		{
			rune c = buffer[i];
			if (c == 0 || inLen==0)
			{
				break;
			}
			isLower = isLower && !IsUpper(c);
		}

		return isLower;
	}

	bool		IsCapitalized		(pcrune inStr, s32 inLen)
	{
		if (inStr[0]!='\0')
		{
			if (IsUpper(inStr[0]))
			{
				return IsLower(&inStr[1], inLen);
			}
		}
		return xFALSE;
	}

	bool		IsQuoted			(pcrune inStr, rune quote, s32 inLen)
	{
		if (inStr[0]!='\0' && inStr[1]!='\0' && inStr[0]==quote)
		{
			inLen = (inLen==-1) ? Len(inStr) : inLen;
			ASSERT(inLen>1);
			return inStr[inLen-1]==quote;
		}
		return xFALSE;
	}

	};
};

/**
 *  END xCore namespace
 */


 #endif // ifndef SPU