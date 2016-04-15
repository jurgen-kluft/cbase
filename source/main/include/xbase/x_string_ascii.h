#ifndef __XBASE_STRING_STD_H__
#define __XBASE_STRING_STD_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif


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
	typedef					char					rune;
	typedef					rune const*				pcrune;
	typedef					rune *					prune;

	/**
		*==============================================================================
		* string functions
		*==============================================================================
		*==============================================================================
		* TODO:
		*       * I need to explain all the different options for formated strings
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

	extern uchar32			ReadChar				(pcrune& c);
	extern s32				WriteChar				(uchar32 c, prune& dest, pcrune end);
	extern s32				LenInBytes				(uchar32 c);
	extern s32  			LenInBytes 				(pcrune str);									// Return the number of bytes that are in 'str'
	extern s32  			LenInChars 				(pcrune str, pcrune * end = NULL);				// Return the number of characters that are in 'str'

	extern s32				StrLen					(pcrune str, pcrune * end = NULL);				// Return length of string as number of characters
	extern prune			StrCopy    				(prune  dst, s32 dest_buffer_size, pcrune src);	// Copy 'src' to 'dst' but not more than 'dest_buffer_size'
	extern s32  			StrCmp    				(pcrune str1, pcrune str2);						// Compare str1 to str2
	extern s32  			StrCmpn    				(pcrune str1, pcrune str2, s32 count);			// Compare str1 to str2 for 'n' characters
	extern s32				StrCmpi   				(pcrune str1, pcrune str2);						// Compare str1 to str2, case insensitive
	extern s32				StrCmpin				(pcrune str1, pcrune str2, s32 count);			// Compare str1 to str2 for 'n' characters, case insensitive
	extern prune			StrCat 					(prune  dst, s32 dest_buffer_size, pcrune src);	// Concatenate strings, dst = dst + src

	extern f32  			StrToF32				(pcrune s, pcrune * scan_end);
	extern f64  			StrToF64				(pcrune s, pcrune * scan_end);
	extern s32				StrToS32  	  			(pcrune str);
	extern s32				StrToS32  	  			(pcrune str, s32 base);
	extern s32				StrToS32  	  			(pcrune str, pcrune * scanEnd, s32 base);
	extern s64				StrToS64  	  			(pcrune str);
	extern s64				StrToS64  	  			(pcrune str, s32 base);
	extern s64				StrToS64  	  			(pcrune str, s32 base, pcrune* scanEnd);
	extern s32				S32ToStr   	  			(s32 val, prune str, s32 str_buffer_size, s32 base);
	extern s32				U32ToStr   	  			(u32 val, prune str, s32 str_buffer_size, s32 base);
	extern s32				S64ToStr   	  			(s64 val, prune str, s32 str_buffer_size, s32 base);
	extern s32				U64ToStr   	  			(u64 val, prune str, s32 str_buffer_size, s32 base);
	extern s32				F32ToStr 	    		(f32 val, s32 numFractionalDigits, prune str, s32 str_buffer_size);
	extern s32				F64ToStr 	    		(f64 val, s32 numFractionalDigits, prune str, s32 str_buffer_size);

	inline bool				IsSpace  	  			(uchar32 c)		{ return((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
	inline bool				IsDigit  	  			(uchar32 c)		{ return((c >=  '0') && (c <= '9')); }
	inline bool				IsAlpha  	  			(uchar32 c)		{ return(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
	inline bool				IsUpper  	  			(uchar32 c)		{ return((c >=  'A') && (c <= 'Z')); }
	inline bool				IsLower  	  			(uchar32 c)		{ return((c >=  'a') && (c <= 'z')); }
	inline bool				IsHex    	  			(uchar32 c)		{ return(((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >=  '0') && (c <= '9'))); }

	inline uchar32			ToUpper    				(uchar32 c)		{ return((c >= 'a') && (c <= 'z'))? c + ('A' - 'a') : c; }
	inline uchar32			ToLower    				(uchar32 c)		{ return((c >= 'A') && (c <= 'Z'))? c + ('a' - 'A') : c; }
	inline uchar32			ToDigit    				(uchar32 c)		{ return ((c >=  '0') && (c <= '9')) ? (c - '0') : c; }
	inline uchar32			ToNumber   				(uchar32 c)		{ if (IsDigit(c)) return ToDigit(c); else if (c >= 'A' && c <= 'F') return(c - 'A' + 10); else if (c >= 'a' && c <= 'f') return(c - 'a' + 10); else return(c); }

	extern bool				IsNumber  				(uchar32 c, s32 base);
	extern bool				IsHex  					(pcrune str);
	extern bool				IsInt  					(pcrune str);
	extern bool				IsFloat					(pcrune str);
	extern bool				IsGUID 					(pcrune str);

	inline bool				EqualNoCase				(uchar32 a, uchar32 b)		{ if (a==b) return true; a = ToLower(a); b = ToLower(b); return (a==b); }

	/// Hash and CRC string functions
	extern u32 				Hash32 					(pcrune inStr, u32 range = 0xffffffff, u32 val = 0x811c9dc5);

	/// Start/End, First/Last
	extern bool				StartsWith				(pcrune inStr, uchar32 inStartChar);
	extern bool				StartsWith				(pcrune inStr, pcrune inStartStr);
	extern bool				StartsWith				(pcrune inStr, s32 inStrLen, pcrune inStartStr, s32 inStartStrLen=-1);
	extern bool				EndsWith				(pcrune inStr, uchar32 inEndChar);
	extern bool				EndsWith				(pcrune inStr, s32 inStrLen, uchar32 inEndChar);
	extern bool				EndsWith				(pcrune inStr, pcrune inEndStr);
	extern bool				EndsWith				(pcrune inStr, s32 inStrLen, pcrune inEndStr, s32 inEndStrLen=-1);

	extern uchar32 			FirstChar				(pcrune inStr);														///< Return first character of string, or 0 if string is empty
	extern uchar32 			LastChar				(pcrune inStr, s32 inStrLen=-1);									///< Return last character of string, or 0 if string is empty

	/// Comparison
	extern s32  			Compare					(pcrune inStr, s32 inStrLen, pcrune inOther, s32 inOtherLen);		///< Check if two strings are equal, taking case into account
	inline s32  			Compare					(pcrune inStr, s32 inStrLen, pcrune inOther)						{ return Compare(inStr, inStrLen, inOther, -1); }
	inline s32  			Compare					(pcrune inStr, pcrune inOther, s32 inOtherLen)						{ return Compare(inStr, -1, inOther, inOtherLen); }
	inline s32  			Compare					(pcrune inStr, pcrune inOther)										{ return Compare(inStr, -1, inOther, -1); }
	extern s32  			CompareNoCase			(pcrune inStr, s32 inStrLen, pcrune inOther, s32 inOtherLen);		///< Check if two strings are equal, not taking case into account
	inline s32  			CompareNoCase			(pcrune inStr, s32 inStrLen, pcrune inOther)						{ return CompareNoCase(inStr, inStrLen, inOther, -1); }
	inline s32  			CompareNoCase			(pcrune inStr, pcrune inOther, s32 inOtherLen)						{ return CompareNoCase(inStr, -1, inOther, inOtherLen); }
	inline s32  			CompareNoCase			(pcrune inStr, pcrune inOther)										{ return CompareNoCase(inStr, -1, inOther, -1); }
	extern bool				Equal					(pcrune inStr, s32 inStrLen, pcrune inOther, s32 inOtherLen);		///< Check if two strings are equal, taking case into account
	inline bool				Equal					(pcrune inStr, s32 inStrLen, pcrune inOther)						{ return Equal(inStr, inStrLen, inOther, -1); }
	inline bool				Equal					(pcrune inStr, pcrune inOther, s32 inOtherLen)						{ return Equal(inStr, -1, inOther, inOtherLen); }
	inline bool				Equal					(pcrune inStr, pcrune inOther)										{ return Equal(inStr, -1, inOther, -1); }
	extern bool				EqualNoCase				(pcrune inStr, s32 inStrLen, pcrune inOther, s32 inOtherLen);		///< Check if two strings are equal, not taking case into account
	inline bool				EqualNoCase				(pcrune inStr, s32 inStrLen, pcrune inOther)						{ return EqualNoCase(inStr, inStrLen, inOther, -1); }
	inline bool				EqualNoCase				(pcrune inStr, pcrune inOther, s32 inOtherLen)						{ return EqualNoCase(inStr, -1, inOther, inOtherLen); }
	inline bool				EqualNoCase				(pcrune inStr, pcrune inOther)										{ return EqualNoCase(inStr, -1, inOther, -1); }

	/// Find
	extern pcrune			Find					(pcrune inStr, uchar32 inChar, s32 inStrLen = -1);					///< Return position of first occurrence of <inChar> or -1 if not found
	extern pcrune			Find					(pcrune inStr, pcrune inFind, s32 inStrLen = -1);					///< Return position of first occurrence of <inString> or -1 if not found
	extern pcrune			FindNoCase				(pcrune inStr, uchar32 inChar, s32 inStrLen = -1);					///< Return position of first occurrence of <inChar> or -1 if not found
	extern pcrune			FindNoCase				(pcrune inStr, pcrune inFind, s32 inStrLen = -1);					///< Return position of first occurrence of <inString> or -1 if not found
	extern pcrune			FindInSubstr			(pcrune inStr, uchar32 inChar, s32 inPos = 0, s32 inStrLen = -1);	///< Return position of first occurrence of <inChar> after <inPos> or -1 if not found
	extern pcrune			FindInSubstr			(pcrune inStr, pcrune inFind, s32 inPos = 0, s32 inStrLen = -1);	///< Return position of first occurrence of <inString> after <inPos> or -1 if not found
	extern pcrune			FindNoCaseInSubstr		(pcrune inStr, uchar32 inChar, s32 inPos = 0, s32 inStrLen = -1);	///< Return position of first occurrence of <inChar> after <inPos> or -1 if not found
	extern pcrune			FindNoCaseInSubstr		(pcrune inStr, pcrune inFind, s32 inPos = 0, s32 inStrLen = -1);	///< Return position of first occurrence of <inString> after <inPos> or -1 if not found
	extern pcrune			FindOneOf				(pcrune inStr, pcrune inSet, s32 inPos = 0, s32 inStrLen = -1);		///< Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found

	/// Contains
	inline bool				Contains				(pcrune inStr, uchar32 inChar, s32 inPos = 0, s32 inStrLen = -1) 	{ return FindInSubstr(inStr, inChar, inPos, inStrLen) != NULL; } ///< Check if this string contains character <inChar>
	inline bool				Contains				(pcrune inStr, pcrune inString, s32 inPos = 0, s32 inStrLen = -1)	{ return FindInSubstr(inStr, inString, inPos, inStrLen) != NULL; } ///< Check if this string contains string <inString>
	inline bool				ContainsNoCase			(pcrune inStr, pcrune inString, s32 inPos = 0, s32 inStrLen = -1)	{ return FindNoCaseInSubstr(inStr, inString, inPos, inStrLen) != NULL; } ///< Check if this string contains <inString> without paying attention to case

	extern bool				IsUpper					(pcrune inStr, s32 inStrLen=-1);									///< Check if string is all upper case
	extern bool				IsLower					(pcrune inStr, s32 inStrLen=-1);									///< Check if string is all lower case
	extern bool				IsCapitalized			(pcrune inStr, s32 inStrLen=-1);									///< Check if string is capitalized ("Shanghai")
	extern bool				IsQuoted				(pcrune inStr, uchar32 quote='\"', s32 inStrLen=-1);				///< Check if string is enclosed in double quotes


	// CPrintf; Formatted print counting function, will return the number of characters needed.
	// SPrintf; Formatted print to "string".
	s32		CPrintf(pcrune formatStr, X_VA_ARGS_16_DEF);
	s32		SPrintf(prune buffer, s32 maxChars, pcrune formatStr, X_VA_ARGS_16_DEF);
	s32		VCPrintf(pcrune formatStr, const x_va_list& args);
	s32		VSPrintf(prune buffer, s32 maxChars, pcrune formatStr, const x_va_list& args);

	//  Printf
	//      Formatted print to "standard text output".  This is straight forward for
	//      text mode programs and is handled by xbase.  Graphical programs
	//      must register a function to handle this operation.

	s32		Printf(pcrune formatStr, X_VA_ARGS_16_DEF);
	s32		Printf(pcrune formatStr, const x_va_list& args);
	s32		Printf(pcrune str);

	//  SScanf
	//      Scan "string" to values.
	s32		SScanf(pcrune buf, pcrune fmt, X_VA_ARGS_16_DEF);
	s32		VSScanf(pcrune buf, pcrune fmt, const x_va_r_list& vr_args);

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif    ///< __X_STRING_STD_H__
