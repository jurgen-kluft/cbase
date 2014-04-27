#ifndef __XBASE_STRING_STD_H__
#define __XBASE_STRING_STD_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

/**
 * TODO
 *==============================================================================
 * - Convert char to xtchar	(1 byte char)
 * - Add support for xwchar (2 byte char)
 */

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_va_list.h"
#include "xbase\x_integer.h"
#include "xbase\x_console.h"


/**
 * xCore namespace
 */
namespace xcore
{
	/**
	 *==============================================================================
	 * string functions
	 *==============================================================================
	 *==============================================================================
	 * TODO: 
	 *       * I need to explain all the different options for formated strings
	 *       * inline things properly
	 *
	 *  Type Field Characters
	 *  Character       Output format
	 *  =========       --------------------------------------------------------
	 *     %%            a percent sign
	 *     %b,%B,%#b     a boolean as true/false (%b=lower-case true/false, %B=upper-case TRUE/FALSE, %#b/%B#=capital-case True/False)
	 *     %c            a character with the given number
	 *     %s            a string
	 *     %d            a signed integer, in decimal
	 *     %u            an unsigned integer, in decimal
	 *     %o            an unsigned integer, in octal
	 *     %x            an unsigned integer, in hexadecimal (lower case)
	 *     %e            a floating-point number, in scientific notation
	 *     %f            a floating-point number, in fixed decimal notation
	 *     %g            a floating-point number, in %e or %f notation
	 *     %X            like %x, but using upper-case letters
	 *     %E            like %e, but using an upper-case "E"
	 *     %G            like %g, but with an upper-case "E" (if applicable)
	 *     %p            a pointer (outputs the Perl value's address in hexadecimal)
	 *     %n            special: *stores* the number of characters output so far into the next variable in the parameter list
	 *     %y,%Y,%#y     a boolean as yes/no (%y=lower-case yes/no, %Y=upper-case YES/NO, %#y/%#Y=capital-case Yes/No)
	 *
	 *  Size flags
	 *  Character    Output
	 *  =========    ------------------------------------------------------------
	 *   h           interpret integer as s16 or u16
	 *   l           interpret integer as s32 or u32
	 *   q, L        interpret integer as s64 or u64
	 *   L           interpret floating point as higher precision adds a few extra floating point numbers
	 *
	 * Examples:
	 *
	 *      x_printf("%Lf %Lf %Lf", Vec.X, Vec.Y, Vec.Z);  // Note that we use the higher precision floating point representation
	 *      x_printf("%Ld" (s64)123);                      // This is a way to print an s64 bit number
	 *      x_printf("%d" 123);                            // Nothing special here printing an 32bit integer
	 *      x_printf("%f" 123.0);                          // Nothing special here printing a floating point number
	 *      x_printf("%+010.4f", 123.456);                 // The printf works like you will expect
	 *
	 */




	extern char*			x_strcpy    			(char* dest, s32 dest_buffer_size, const char* src);
	extern s32  			x_strlen    			(const char* str);
	extern s32  			x_wstrlen    		(const xwchar* str);
	extern s32  			x_strcmp    			(const char* str1, const char* str2);
	extern s32  			x_strncmp    		(const char* str1, const char* str2, s32 count);
	extern s32			x_stricmp   			(const char* str1, const char* str2);
	extern s32			x_strnicmp			(const char *str1, const char *str2, s32 count);
	extern char*			x_stristr   			(const char* str1, const char* str2);
	extern char*			x_strstr    			(const char* str1, const char* str2);
	extern char*			x_strchr    			(const char* str1, char c);
	extern char*			x_strrchr    		(const char* str1, char c);
	extern const char*	x_strscn    			(const char* str, const char* pattern);
	extern void*			x_memchr    			(void* buf, s32 chr, s32 count);
	extern char*			x_strcat    			(char* dest, s32 dest_buffer_size, const char* back);

	extern f32  			x_strtof				(const char *s, const char **scan_end);
	extern f64  			x_strtod				(const char *s, const char **scan_end);
	extern s32			x_atoi32    			(const char* str);
	extern s32			x_atod32    			(const char* str, s32 base);
	extern s32			x_atoi32    			(const char* str, const char** scanEnd, s32 base);
	extern s64			x_atoi64    			(const char* str);
	extern s64			x_atod64    			(const char* str, s32 base);
	extern s64			x_atoi64    			(const char* str, s32 base, const char** scanEnd);
	extern f32			x_atof32    			(const char* str);
	extern f64			x_atof64    			(const char* str);
	extern s32			x_dtoa      			(s32 val, char* str, s32 str_buffer_size, s32 base);
	extern s32			x_dtoa      			(u32 val, char* str, s32 str_buffer_size, s32 base);
	extern s32			x_dtoa      			(s64 val, char* str, s32 str_buffer_size, s32 base);
	extern s32			x_dtoa      			(u64 val, char* str, s32 str_buffer_size, s32 base);
	extern s32			x_f32toa      		(f32 val, s32 numFractionalDigits, char* str, s32 str_buffer_size);
	extern s32			x_f64toa      		(f64 val, s32 numFractionalDigits, char* str, s32 str_buffer_size);

	inline bool			x_isspace    		(s32 c)					{ return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
	inline bool			x_isdigit    		(s32 c)					{ return((c >=  '0') && (c <= '9')); }
	inline bool			x_isalpha    		(s32 c)					{ return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
	inline bool			x_isupper    		(s32 c)					{ return((c >=  'A') && (c <= 'Z')); }
	inline bool			x_islower    		(s32 c)					{ return((c >=  'a') && (c <= 'z')); }
	inline bool			x_ishex      		(s32 c)					{ return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >=  '0') && (c <= '9'))); }

	inline s32			x_toupper    		(s32 c)					{ return((c >= 'a') && (c <= 'z'))? c + ('A' - 'a') : c; }
	inline s32			x_tolower    		(s32 c)					{ return((c >= 'A') && (c <= 'Z'))? c + ('a' - 'A') : c; }
	inline s32			x_todigit    		(s32 c)					{ return ((c >=  '0') && (c <= '9')) ? (c - '0') : c; }
	inline s32			x_tonumber   		(s32 c)					{ if (x_isdigit(c)) return x_todigit(c); else if (c >= 'A' && c <= 'F') return(c - 'A' + 10); else if (c >= 'a' && c <= 'f') return(c - 'a' + 10); else return(c); }

	extern bool			x_isnumber  			(char c, s32 base);
	extern bool			x_isstrhex  			(const char* str);
	extern bool			x_isstrint  			(const char* str);
	extern bool			x_isstrfloat			(const char* str);
	extern bool			x_isstrguid 			(const char* str);

	inline bool			x_isEqualNoCase		(char a, char b)		{ if (a==b) return true; a = x_tolower(a); b = x_tolower(b); return (a==b); }


	/// UTF8 and UTF16
	extern u32			x_strAsciiToUTF8		( char* szDst, const char* szSrc );
	extern u32			x_strUTF16ToUTF8		( char* szDst, const xwchar* wszSrc );

	/// Hash and CRC string functions
	extern u32 			x_strHash   			(const char* inStr, u32 range = 0xffffffff, u32 val = 0x811c9dc5);
	extern u32 			x_strIHash  			(const char* inStr, u32 range = 0xffffffff, u32 val = 0x811c9dc5);

	/// Start/End, First/Last
	extern bool			x_strStartsWith			(const char* inStr, char inStartChar);
	extern bool			x_strStartsWith			(const char* inStr, const char* inStartStr);
	extern bool			x_strStartsWith			(const char* inStr, s32 inStrLen, const char* inStartStr, s32 inStartStrLen=-1);
	extern bool			x_strEndsWith			(const char* inStr, char inEndChar);
	extern bool			x_strEndsWith			(const char* inStr, s32 inStrLen, char inEndChar);
	extern bool			x_strEndsWith			(const char* inStr, const char* inEndStr);
	extern bool			x_strEndsWith			(const char* inStr, s32 inStrLen, const char* inEndStr, s32 inEndStrLen=-1);

	extern char 			x_strFirstChar			(const char* inStr);														///< Return first character of string, or 0 if string is empty
	extern char 			x_strLastChar			(const char* inStr, s32 inStrLen=-1);										///< Return last character of string, or 0 if string is empty

	/// Comparison
	extern s32  			x_strCompare				(const char* inStr, s32 inStrLen, const char* inOther, s32 inOtherLen);		///< Check if two strings are equal, taking case into account
	inline s32  			x_strCompare				(const char* inStr, s32 inStrLen, const char* inOther)						{ return x_strCompare(inStr, inStrLen, inOther, -1); }
	inline s32  			x_strCompare				(const char* inStr, const char* inOther, s32 inOtherLen)						{ return x_strCompare(inStr, -1, inOther, inOtherLen); }
	inline s32  			x_strCompare				(const char* inStr, const char* inOther)										{ return x_strCompare(inStr, -1, inOther, -1); }
	extern s32  			x_strCompareNoCase		(const char* inStr, s32 inStrLen, const char* inOther, s32 inOtherLen);		///< Check if two strings are equal, not taking case into account
	inline s32  			x_strCompareNoCase		(const char* inStr, s32 inStrLen, const char* inOther)						{ return x_strCompareNoCase(inStr, inStrLen, inOther, -1); }
	inline s32  			x_strCompareNoCase		(const char* inStr, const char* inOther, s32 inOtherLen)						{ return x_strCompareNoCase(inStr, -1, inOther, inOtherLen); }
	inline s32  			x_strCompareNoCase		(const char* inStr, const char* inOther)										{ return x_strCompareNoCase(inStr, -1, inOther, -1); }
	extern bool			x_strEqual				(const char* inStr, s32 inStrLen, const char* inOther, s32 inOtherLen);		///< Check if two strings are equal, taking case into account
	inline bool			x_strEqual				(const char* inStr, s32 inStrLen, const char* inOther)						{ return x_strEqual(inStr, inStrLen, inOther, -1); }
	inline bool			x_strEqual				(const char* inStr, const char* inOther, s32 inOtherLen)						{ return x_strEqual(inStr, -1, inOther, inOtherLen); }
	inline bool			x_strEqual				(const char* inStr, const char* inOther)										{ return x_strEqual(inStr, -1, inOther, -1); }
	extern bool			x_strEqualNoCase			(const char* inStr, s32 inStrLen, const char* inOther, s32 inOtherLen);		///< Check if two strings are equal, not taking case into account
	inline bool			x_strEqualNoCase			(const char* inStr, s32 inStrLen, const char* inOther)						{ return x_strEqualNoCase(inStr, inStrLen, inOther, -1); }
	inline bool			x_strEqualNoCase			(const char* inStr, const char* inOther, s32 inOtherLen)						{ return x_strEqualNoCase(inStr, -1, inOther, inOtherLen); }
	inline bool			x_strEqualNoCase			(const char* inStr, const char* inOther)										{ return x_strEqualNoCase(inStr, -1, inOther, -1); }

	/// Find
	extern const char*	x_strFind				(const char* inStr, char inChar, s32 inStrLen = -1);							///< Return position of first occurrence of <inChar> or -1 if not found
	extern const char*	x_strFind				(const char* inStr, const char* inFind, s32 inStrLen = -1);					///< Return position of first occurrence of <inString> or -1 if not found
	extern const char*	x_strRFind				(const char* inStr, char inChar, s32 inPos = -1, s32 inStrLen = -1);			///< Return position of last occurrence of <inChar> on or before <inPos> or -1 if not found
	extern const char*	x_strRFind				(const char* inStr, const char* inFind, s32 inPos = -1, s32 inStrLen = -1);	///< Return position of last occurrence of <inChar> on or before <inPos> or -1 if not found
	extern const char*	x_strFindNoCase			(const char* inStr, char inChar, s32 inStrLen = -1);							///< Return position of first occurrence of <inChar> or -1 if not found
	extern const char*	x_strFindNoCase			(const char* inStr, const char* inFind, s32 inStrLen = -1);					///< Return position of first occurrence of <inString> or -1 if not found
	extern const char*	x_strFindInSubstr		(const char* inStr, char inChar, s32 inPos = 0, s32 inStrLen = -1);			///< Return position of first occurrence of <inChar> after <inPos> or -1 if not found
	extern const char*	x_strFindInSubstr		(const char* inStr, const char* inFind, s32 inPos = 0, s32 inStrLen = -1);	///< Return position of first occurrence of <inString> after <inPos> or -1 if not found
	extern const char*	x_strFindNoCaseInSubstr	(const char* inStr, char inChar, s32 inPos = 0, s32 inStrLen = -1);			///< Return position of first occurrence of <inChar> after <inPos> or -1 if not found
	extern const char*	x_strFindNoCaseInSubstr	(const char* inStr, const char* inFind, s32 inPos = 0, s32 inStrLen = -1);	///< Return position of first occurrence of <inString> after <inPos> or -1 if not found
	extern const char*	x_strFindOneOf			(const char* inStr, const char* inSet, s32 inPos = 0, s32 inStrLen = -1);	///< Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found
	extern const char*	x_strRFindOneOf			(const char* inStr, const char* inSet, s32 inPos = -1, s32 inStrLen = -1);	///< Return position of last occurrence of a character in <inCharSet> after <inPos> or -1 if not found

	/// Contains
	inline bool			x_strContains			(const char* inStr, char inChar, s32 inPos = 0, s32 inStrLen = -1) 			{ return x_strFindInSubstr(inStr, inChar, inPos, inStrLen) != NULL; } ///< Check if this string contains character <inChar>
	inline bool			x_strContains			(const char* inStr, const char* inString, s32 inPos = 0, s32 inStrLen = -1)	{ return x_strFindInSubstr(inStr, inString, inPos, inStrLen) != NULL; } ///< Check if this string contains string <inString>
	inline bool			x_strContainsNoCase		(const char* inStr, const char* inString, s32 inPos = 0, s32 inStrLen = -1)	{ return x_strFindNoCaseInSubstr(inStr, inString, inPos, inStrLen) != NULL; } ///< Check if this string contains <inString> without paying attention to case

	/// IndexOf
	inline s32 			x_strIndexOf				(const char* inStr, char inChar, s32 inPos = 0, s32 inStrLen = -1)			{ const char* str = x_strFindInSubstr(inStr, inChar, inPos, inStrLen); return str==NULL ? -1 : (s32)(str-inStr); }
	inline s32 			x_strIndexOf				(const char* inStr, const char* inString, s32 inPos = 0, s32 inStrLen = -1)	{ const char* str = x_strFindInSubstr(inStr, inString, inPos, inStrLen); return str==NULL ? -1 : (s32)(str-inStr); }
	inline s32 			x_strLastIndexOf			(const char* inStr, char inChar, s32 inPos = 0, s32 inStrLen = -1)			{ const char* str = x_strRFind(inStr, inChar, inPos, inStrLen); return str==NULL ? -1 : (s32)(str-inStr); }

	extern bool			x_strIsUpper				(const char* inStr, s32 inStrLen=-1);										///< Check if string is all upper case
	extern bool			x_strIsLower				(const char* inStr, s32 inStrLen=-1);										///< Check if string is all lower case
	extern bool			x_strIsCapitalized		(const char* inStr, s32 inStrLen=-1);										///< Check if string is capitalized ("Shanghai")
	extern bool			x_strIsQuoted			(const char* inStr, char quote='\"', s32 inStrLen=-1);						///< Check if string is enclosed in f64 quotes


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
	extern s32		x_cprintf   			(                            const char* formatStr, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, 
																							const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty, const x_va& v11=x_va::sEmpty, const x_va& v12=x_va::sEmpty, const x_va& v13=x_va::sEmpty, const x_va& v14=x_va::sEmpty, const x_va& v15=x_va::sEmpty, const x_va& v16=x_va::sEmpty);
	extern s32		x_sprintf   			(char* buffer, s32 maxChars, const char* formatStr, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, 
																							const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty, const x_va& v11=x_va::sEmpty, const x_va& v12=x_va::sEmpty, const x_va& v13=x_va::sEmpty, const x_va& v14=x_va::sEmpty, const x_va& v15=x_va::sEmpty, const x_va& v16=x_va::sEmpty);
	extern s32		x_vcprintf  			(                            const char* formatStr, const x_va_list& args);
	extern s32		x_vsprintf  			(char* buffer, s32 maxChars, const char* formatStr, const x_va_list& args);

	/**
	 * FORMATTED STRING FUNCTIONS
	 *==============================================================================
	 *
	 *  x_printf    
	 *
	 *      Formatted print to "standard text output".  This is straight forward for
	 *      text mode programs and is handled by the xBase.  Graphical programs
	 *      must register a function to handle this operation.
	 *
	 *==============================================================================
	 */


	s32		x_printf   			(const char* formatStr, const x_va& v1             , const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, 
														const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty, const x_va& v11=x_va::sEmpty, const x_va& v12=x_va::sEmpty, const x_va& v13=x_va::sEmpty, const x_va& v14=x_va::sEmpty, const x_va& v15=x_va::sEmpty, const x_va& v16=x_va::sEmpty);
	s32		x_printf				(const char* formatStr, const x_va_list& args)	;
	s32		x_printf				(const char* str);
		


	/**
	 * STRING SCAN FUNCTIONS
	 *==============================================================================
	 *
	 *  x_sscanf  
	 *
	 *      Scan "string" to values.
	 *
	 *==============================================================================
	 */

	extern s32		x_sscanf		(const char *buf, const char *fmt, const x_va_r& v1=x_va_r::sEmpty, const x_va_r& v2=x_va_r::sEmpty, const x_va_r& v3=x_va_r::sEmpty, const x_va_r& v4=x_va_r::sEmpty, const x_va_r& v5=x_va_r::sEmpty, const x_va_r& v6=x_va_r::sEmpty, const x_va_r& v7=x_va_r::sEmpty, const x_va_r& v8=x_va_r::sEmpty,
																	const x_va_r& v9=x_va_r::sEmpty, const x_va_r& v10=x_va_r::sEmpty, const x_va_r& v11=x_va_r::sEmpty, const x_va_r& v12=x_va_r::sEmpty, const x_va_r& v13=x_va_r::sEmpty, const x_va_r& v14=x_va_r::sEmpty, const x_va_r& v15=x_va_r::sEmpty, const x_va_r& v16=x_va_r::sEmpty);
	extern s32		x_vsscanf	(const char *buf, const char *fmt, const x_va_r_list& vr_args);

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */
#endif    ///< __X_STRING_STD_H__
