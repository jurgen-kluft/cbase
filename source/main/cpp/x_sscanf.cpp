#include "xbase/x_target.h"
#include "xbase/x_runes.h"
#include "xbase/x_va_list.h"

#ifndef SPU

// Shared code
#include "xbase/private/x_printf.h"

namespace xcore
{
	static bool SearchUntilOneOf(CharReader* reader, CharReader* foo)
	{
		uchar32 c = reader->Peek();
		while (c != '\0')
		{
			uchar32 f = foo->Read();
			while (f != '\0')
			{
				if (f == c)
					return true;
				f = foo->Read();
			}
			foo->Reset();
			reader->Skip();
			c = reader->Peek();
		}
		return false;
	}

	static bool	MatchBoolStr(CharReader* str, bool& boolean)
	{
		bool bval = false;
		const char* bstr = NULL;
		uchar32 c = str->Peek();
		if (utf32::is_equalfold(c, 't'))
		{
			bstr = "true";
			bval = true;
		}
		else if (utf32::is_equalfold(c, 'f'))
		{
			bstr = "false";
		}
		else if (utf32::is_equalfold(c, 'y'))
		{
			bstr = "yes";
			bval = true;
		}
		else if (utf32::is_equalfold(c, 'n'))
		{
			bstr = "no";
		}
		else if (utf32::is_equalfold(c, 'o'))
		{
			str->Read();
			c = str->Peek();
			if (utf32::is_equalfold(c, 'f'))
			{
				bstr = "ff";
				bval = false;
			}
			else if (utf32::is_equalfold(c, 'n'))
			{
				bval = true;
				bstr = "n";
			}
		}

		boolean = bval;
		if (bstr != NULL)
		{
			while (*bstr != '\0')
			{
				uchar32 bc = *bstr++;
				uchar32 sc = str->Read();
				if (sc == '\0')
					return false;
				if (utf32::is_equalfold(bc, sc) == false)
					return false;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 *------------------------------------------------------------------------------
	 * Summary:
	 *     atod64          - Converts a string value to integer base from a particular
	 * Arguments:
	 *        reader       - Source string encoded with a particular base
	 *        Base         - Base of the numeric string
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
	s64 StrToS64(CharReader* reader, s32 base)
	{
		ASSERT(reader != NULL);
		ASSERT(base > 2);
		ASSERT(base <= (26 + 26 + 10));

		// Skip whitespace.
		while (reader->Peek() == ' ')
		{
			reader->Read();
		}

		uchar32 c;				// Current character.
		c = reader->Peek();		// Save sign indication.
		uchar32 sign = c;		// If '-', then negative, otherwise positive.

		// Skip sign.
		if ((c == '-') || (c == '+'))
		{
			c = reader->Read();
		}

		s64  total = 0;   // Current total.

		// Decode the rest of the string
		while (true)
		{
			c = reader->Peek();

			s32  validBase = 0;
			if ((c >= '0') && (c <= '9'))
			{
				validBase = c - '0';
			}
			else if (base==16 && (c >= 'a') && (c <= 'f'))
			{
				validBase = c - 'a' + 10;
			}
			else if (base == 16 && (c >= 'A') && (c <= 'F'))
			{
				validBase = c - 'A' + 10;
			}
			else if (c == '_' || c == ':')
			{
				// Ignore
				reader->Skip();
				continue;
			}
			else
			{
				// Negate the total if negative.
				if (sign == '-')
					total = -total;

				// Any other character is bad news
				return total;
			}

			reader->Skip();

			ASSERT(validBase >= 0);
			ASSERT(validBase < base);

			// Accumulate digit.
			total = (base * total) + validBase;
		}

		// No way to get here
		return total;
	}

	// <COMBINE atod64 >
	s32 StrToS32(CharReader* reader, s32 base)
	{
		return (s32)StrToS64(reader, base);
	}

	//------------------------------------------------------------------------------

	f64	 StrToF64(CharReader* reader)
	{
		// Evaluate sign 
		s32 sign = 1;
		if (reader->Peek() == '-')
		{
			sign = -1;
			reader->Read();
		}

		// Skip trailing zeros 
		while (reader->Peek() == '0')
			reader->Read();

		// Convert integer part 
		f64 result = 0;
		f64 value;
		while (true)
		{
			uchar32 c = reader->Peek();
			if (c >= '0' && c <= '9')
			{
				value = c - '0';
				result *= 10.0;
				result += value;
				reader->Read();
			}
			else
			{
				break;
			}
		}

		// Detect floating point & mantissa 
		f64 mantissa = 0;
		f64 divisor = 1;
		if (reader->Peek() == '.')
		{
			reader->Read();

			while (true)
			{
				uchar32 c = reader->Peek();
				if (c >= '0' && c <= '9')
				{
					value = c - '0';
					mantissa *= 10.0;
					mantissa += value;
					divisor *= 10.0;
					reader->Read();
				}
				else
				{
					break;
				}
			}
		}

		mantissa /= divisor;
		// Adjust result 
		result += mantissa;
		// Adjust sign 
		result *= sign;
		// Detect exponent 
		u32 power = 0;

		uchar32 c = reader->Peek();
		if (c == 'e' || c == 'E')
		{
			reader->Read();

			c = reader->Peek();
			if (c == '-')
			{
				sign = -1;
				reader->Read();
			}
			else if (c == '+')
			{
				sign = 1;
				reader->Read();
			}
			else
				sign = 1;

			while (c <= '9' && c >= '0')
			{
				u32 v = c - '0';
				power *= 10;
				power += v;
				reader->Read();
			}
		}

		// Adjust result on exponent sign 
		if (sign > 0)
			for (u32 i = 0; i < power; i++)
				result *= 10.0;
		else
			for (u32 i = 0; i < power; i++)
				result /= 10.0;

		return(result);
	}

	/**
	 *Parameters

	 *	str
	 *		C string that the function processes as its source to retrieve the data.
	 *	format
	 *		C string that contains one or more of the following items:

	 *	Whitespace character:
	 *							The function will read and ignore any whitespace characters (this includes blank spaces
	 *							and the newline and tab characters) which are encountered before the next non-whitespace
	 *							character. This includes any quantity of whitespace characters, or none.

	 *	Non-whitespace character, except percentage signs (%):
	 *							Any character that is not either a whitespace character (blank, newline or tab) or part
	 *							of a format specifier (which begin with a % character) causes the function to read the
	 *							next character from str, compare it to this non-whitespace character and if it matches,
	 *							it is discarded and the function continues with the next character of format and str.
	 *							If the character does not match, the function fails and returns.

	 *	Format specifiers:
	 *							A sequence formed by an initial percentage sign (%) indicates a format specifier, which
	 *							is used to specify the type and format of the data to be retrieved from the str string
	 *							and stored in the locations pointed by the additional arguments.
	 *							A format specifier follows this prototype: [=%[ *][width][modifiers]type=]

	 *							where:

	 *							 *            An optional starting asterisk indicates that the data is to be retrieved from
	 *										the str string but ignored, i.e. it is not stored in the corresponding argument.
	 *							width        Specifies the maximum number of characters to be read in the current reading operation
	 *							modifiers    Specifies a size different from s32 (in the case of d, i and n), unsigned s32 (in
	 *										the case of o, u and x) or f32 (in the case of e, f and g) for the data pointed by
	 *										the corresponding additional argument:
	 *										h : short s32 (for d, i and n), or unsigned short s32 (for o, u and x)
	 *										l : long s32 (for d, i and n), or unsigned long s32 (for o, u and x), or f64 (for e, f and g)
	 *										L : long f64 (for e, f and g)
	 *
	 *							type        A character specifying the type of data to be read and how it is expected to be read.
	 *										See next table.

	 *	fscanf type specifiers: type Qualifying Input Type of argument
	 *							c            Single character: Reads the next character. If a width different from 1 is
	 *										specified, the function reads width characters and stores them in the successive
	 *										locations of the array passed as argument. No null character is appended at the end.
	 *								type    char  *

	 *							d            Decimal integer: Number optionally preceeded with a + or - sign.
	 *								type    s32  *

	 *							e,E,f,g,G    Floating point: Decimal number containing a decimal point, optionally preceeded by a
	 *										+ or - sign and optionally folowed by the e or E character and a decimal number.
	 *										Two examples of valid entries are -732.103 and 7.12e4

	 *								type    f32  *

	 *							o            Octal integer.
	 *								type    s32  *

	 *							s            String of characters. This will read subsequent characters until a whitespace is found
	 *										(whitespace characters are considered to be blank, newline and tab).
	 *								type    char  *

	 *							u            Unsigned decimal integer.
	 *								type    unsigned s32  *

	 *							x,X            Hexadecimal integer.
	 *								type    s32  *

	 *	additional arguments
	 *							The function expects a sequence of references as additional arguments, each one pointing to
	 *							an object of the type specified by their corresponding %-tag within the format string, in the
	 *							same order.
	 */


	enum ESScanf
	{
		RESPECT_WIDTH = 1,    ///< use fixed width
		ADD_PLUS = 2,    ///< use + for positive
		SPACE_PAD = 4,    ///< use padding
		ZERO_PAD = 8,
		LEFT_PAD = 16,

		INT16_SIZE = 2,
		INT32_SIZE = 4,
		INT64_SIZE = 8,
	};

	s32 VSScanf(CharReader* reader, CharReader* fmt, const x_va_r_list& vr_args)
	{
		s32 i = 0;
		s32 w = 0;
		s32 flag = 0;
		s32 l = 0;
		s32 parsing = 0;
		s32 scanned = 0;
		s32 suppress = 0;

		while (fmt->Peek() != '\0')
		{
			if (fmt->Peek() != '%' && !parsing)
			{
				fmt->Read();
			}
			else
			{
				if (fmt->Peek() == '%')
				{
					fmt->Skip();

					parsing = 1;
					//size = INT32_SIZE;
					suppress = 0;
					w = 0;
					flag = 0;
					l = 0;
				}

				switch (fmt->Peek())
				{
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case '0':
					if (parsing == 1)
					{
						w = StrToS32(fmt, 10);
						flag |= SPACE_PAD;
					}
					break;
				case 'c':
				{
					fmt->Skip();

					uchar32 c = reader->Read();
					x_va_r  r = vr_args[i++];
					r = c;
					scanned++;
					parsing = 0;
				} break;
				case 's':
				{
					fmt->Skip();

					x_va_r r = vr_args[i++];

					u32 i = 0;
					while (reader->Peek() != 0 && reader->Peek() == ' ')
						reader->Read();

					uchar* ascii_str = (uchar*)r;
					AsciiBuffer ascii_str_buffer(ascii_str, ascii_str != NULL ? (ascii_str + r.var()) : ascii_str);
					CharWriterToAsciiBuffer ascii_str_writer(ascii_str_buffer);

					uchar32* utf32_str = (uchar32*)r;
					Utf32Buffer utf32_str_buffer(utf32_str, r.var());
					CharWriterToUtf32Buffer utf32_str_writer(utf32_str_buffer);

					CharWriter* str_writer = &ascii_str_writer;
					if (r.isPUChar32())
						str_writer = &utf32_str_writer;

					l = 0;
					while (reader->Peek() != 0 && reader->Peek() != ' ')
					{
						if (!(flag & SPACE_PAD))
						{
							str_writer->Write(reader->Read());
						}
						else if (l < w)
						{
							str_writer->Write(reader->Read());
							l++;
						}
						reader->Read();
					}

					scanned++;
					parsing = 0;
				} break;

				case 'B':
				case 'b':
				case 'Y':
				case 'y':
				{
					fmt->Skip();
					x_va_r r = vr_args[i++];
					bool boolean;
					MatchBoolStr(reader, boolean);
					r = boolean;
					scanned++;
				} break;

				case 'i':
				case 'd':
				{
					fmt->Skip();

					utf32::rune decimalChars[] = {'1','2','3','4','5','6','7','8','9','0','-','+', 0};
					CharReaderFromUtf32Buffer foo(decimalChars, decimalChars + 12);

					while (reader->Peek() != 0 && reader->Peek() == ' ')
						reader->Read();

					s64 n1 = 0;
					if (SearchUntilOneOf(reader, &foo))
					{
						n1 = StrToS64(reader, 10);
					}

					if (!suppress)
					{
						x_va_r r = vr_args[i++];
						r = n1;
						scanned++;
					}
					else
					{
						i++;
					}

					parsing = 0;
				} break;
				case 'u':
				{
					fmt->Skip();

					utf32::rune decimalChars[] = {'1','2','3','4','5','6','7','8','9','0', 0};
					CharReaderFromUtf32Buffer foo(decimalChars, decimalChars + 10);

					while (reader->Peek() != 0 && reader->Peek() == ' ')
						reader->Read();

					s64 n2 = 0;
					if (SearchUntilOneOf(reader, &foo))
					{
						n2 = StrToS64(reader, 10);
					}

					if (!suppress)
					{
						x_va_r r = vr_args[i++];
						r = n2;
						scanned++;
					}
					else
					{
						i++;
					}

					parsing = 0;
				} break;
				case 'o':
				{
					fmt->Skip();

					utf32::rune decimalChars[] = {'1','2','3','4','5','6','7','0', 0};
					CharReaderFromUtf32Buffer foo(decimalChars, decimalChars + 8);

					while (reader->Peek() != 0 && reader->Peek() == ' ')
						reader->Read();

					s64 n2 = 0;
					if (SearchUntilOneOf(reader, &foo))
					{
						n2 = StrToS64(reader, 8);
					}

					if (!suppress)
					{
						x_va_r r = vr_args[i++];
						r = n2;
						scanned++;
					}
					else
					{
						i++;
					}

					parsing = 0;
				} break;
				case 'x':
				case 'X':
				{
					fmt->Skip();

					utf32::rune decimalChars[] = {'1','2','3','4','5','6','7','8','9','0','x','a','b','c','d','e','f','A','B','C','D','E','F', 0};
					CharReaderFromUtf32Buffer foo(decimalChars, decimalChars + 23);

					while (reader->Peek() != 0 && reader->Peek() == ' ')
						reader->Read();

					if (w == 0)
					{
						u32 const varsize = vr_args[i].sizeInBytes();
						w = varsize * 2;
					}

					u64 n2 = 0;
					if (SearchUntilOneOf(reader, &foo))
					{
						if (w == 2)
						{
							u32 const strl = 2;
							uchar32 str[strl + 1];
							str[strl] = '\0';
							for (s32 j = 0; j < strl; ++j)
								str[j] = reader->Read();
							CharReaderFromUtf32Buffer str_reader(str, NULL);
							n2 = StrToS64(&str_reader, 16);
						}
						else if (w == 4)
						{
							u32 const strl = 4;
							uchar32 str[strl + 1];
							str[strl] = '\0';
							for (s32 j = 0; j < strl; ++j)
							{
								uchar32 c = reader->Read();
								str[j] = c;
								if (c == '\0')
									break;
							}
							CharReaderFromUtf32Buffer str_reader(str, NULL);
							n2 = StrToS64(&str_reader, 16);
						}
						else if (w == 8)
						{
							u32 const strl = 8;
							uchar32 str[strl + 1];
							str[strl] = '\0';
							for (s32 j = 0; j < strl; ++j)
							{
								uchar32 c = reader->Read();
								str[j] = c;
								if (c == '\0')
									break;
							}
							CharReaderFromUtf32Buffer str_reader(str, NULL);
							n2 = StrToS64(&str_reader, 16);
						}
						else // if (w == 16)
						{
							u32 const strl = 16;
							uchar32 str[strl + 1];
							str[strl] = '\0';
							for (s32 j = 0; j < strl; ++j)
							{
								uchar32 c = reader->Read();
								str[j] = c;
								if (c == '\0')
									break;
							}
							CharReaderFromUtf32Buffer str_reader(str, NULL);
							n2 = (u64)StrToS64(&str_reader, 16);
						}
					}

					if (!suppress)
					{
						x_va_r r = vr_args[i++];
						r = n2;
						scanned++;
					}
					else
					{
						i++;
					}

					parsing = 0;
				} break;
				case 'f':
				case 'g':
				case 'G':
				case 'e':
				case 'E':
				{
					fmt->Skip();

					utf32::rune decimalChars[] = {'1','2','3','4','5','6','7','8','9','0','.','e','+','-', 0};
					CharReaderFromUtf32Buffer foo(decimalChars, decimalChars + 14);

					while (reader->Peek() != 0 && reader->Peek() == ' ')
						reader->Read();

					f64 n3 = 0;
					if (SearchUntilOneOf(reader, &foo))
					{
						n3 = StrToF64(reader);
					}

					if (!suppress)
					{
						x_va_r r = vr_args[i++];
						r = n3;
						scanned++;
					}
					else
					{
						i++;
					}

					parsing = 0;
				} break;
				case 'l':
				case 'L':
					fmt->Skip();
					//size = INT64_SIZE;
					break;
				case 'h':
				case 'n':
					fmt->Skip();
					//size = INT16_SIZE;
					break;
				case '*':
					fmt->Skip();
					suppress = 1;
					break;
				default:
					parsing = 0;
					fmt->Skip();
					break;
				}
			}
		}

		return scanned;
	}

	namespace ascii
	{
		s32 sscanf(crunes & str, crunes const& fmt, X_VA_R_ARGS_16)
		{
			x_va_r_list vr_args(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16);
			CharReaderFromAsciiBuffer buf_reader(str.m_str, str.m_end);
			CharReaderFromAsciiBuffer fmt_reader(fmt.m_str, fmt.m_end);
			s32 scanned = VSScanf(&buf_reader, &fmt_reader, vr_args);
			str.m_str = buf_reader.mPtr;
			return scanned;
		}

		s32 vsscanf(crunes & str, crunes const& fmt, const x_va_r_list& vr_args)
		{
			CharReaderFromAsciiBuffer buf_reader(str.m_str, str.m_end);
			CharReaderFromAsciiBuffer fmt_reader(fmt.m_str, fmt.m_end);
			s32 scanned = VSScanf(&buf_reader, &fmt_reader, vr_args);
			str.m_str = buf_reader.mPtr;
			return scanned;
		}
	}

	namespace utf32
	{
		s32 sscanf(crunes & str, crunes const& fmt, X_VA_R_ARGS_16)
		{
			x_va_r_list vr_args(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16);
			CharReaderFromUtf32Buffer buf_reader(str.m_str, str.m_end);
			CharReaderFromUtf32Buffer fmt_reader(fmt.m_str, fmt.m_end);
			s32 scanned = VSScanf(&buf_reader, &fmt_reader, vr_args);
			str.m_str = buf_reader.mPtr;
			return scanned;
		}

		s32 vsscanf(crunes & str, crunes const& fmt, const x_va_r_list& vr_args)
		{
			CharReaderFromUtf32Buffer buf_reader(str.m_str, str.m_end);
			CharReaderFromUtf32Buffer fmt_reader(fmt.m_str, fmt.m_end);
			s32 scanned = VSScanf(&buf_reader, &fmt_reader, vr_args);
			str.m_str = buf_reader.mPtr;
			return scanned;
		}
	}

};
/**
 *  END xCore namespace
 */

#endif // ifndef SPU
