#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_runes.h"
#include "xbase/x_va_list.h"

#ifndef SPU

// Shared code

namespace xcore
{
    // runes_reader_t -> rune_reader_t
    // CharWriter -> rune_writer_t

    static bool SearchUntilOneOf(irunes_reader_t* reader, irunes_reader_t* foo)
    {
        uchar32 c = reader->peek();
        while (c != '\0')
        {
            uchar32 f = foo->read();
            while (f != '\0')
            {
                if (f == c)
                    return true;
                f = foo->read();
            }
            foo->reset();
            reader->skip();
            c = reader->peek();
        }
        return false;
    }

    static bool MatchBoolStr(irunes_reader_t* str, bool& boolean)
    {
        bool        bval = false;
        const char* bstr = NULL;
        uchar32     c    = str->peek();
        if (is_equalfold(c, 't'))
        {
            bstr = "true";
            bval = true;
        }
        else if (is_equalfold(c, 'f'))
        {
            bstr = "false";
        }
        else if (is_equalfold(c, 'y'))
        {
            bstr = "yes";
            bval = true;
        }
        else if (is_equalfold(c, 'n'))
        {
            bstr = "no";
        }
        else if (is_equalfold(c, 'o'))
        {
            str->read();
            c = str->peek();
            if (is_equalfold(c, 'f'))
            {
                bstr = "ff";
                bval = false;
            }
            else if (is_equalfold(c, 'n'))
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
                uchar32 sc = str->read();
                if (sc == '\0')
                    return false;
                if (is_equalfold(bc, sc) == false)
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
    s64 StrToS64(irunes_reader_t* reader, s32 base)
    {
        ASSERT(reader != NULL);
        ASSERT(base > 2);
        ASSERT(base <= (26 + 26 + 10));

        // skip whitespace.
        while (reader->peek() == ' ')
        {
            reader->read();
        }

        uchar32 c;                     // Current character.
        c            = reader->peek(); // Save sign indication.
        uchar32 sign = c;              // If '-', then negative, otherwise positive.

        // skip sign.
        if ((c == '-') || (c == '+'))
        {
            c = reader->read();
        }

        s64 total = 0; // Current total.

        // Decode the rest of the string
        while (true)
        {
            c = reader->peek();

            s32 validBase = 0;
            if ((c >= '0') && (c <= '9'))
            {
                validBase = c - '0';
            }
            else if (base == 16 && (c >= 'a') && (c <= 'f'))
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
                reader->skip();
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

            reader->skip();

            ASSERT(validBase >= 0);
            ASSERT(validBase < base);

            // Accumulate digit.
            total = (base * total) + validBase;
        }

        // No way to get here
        return total;
    }

    // <COMBINE atod64 >
    s32 StrToS32(irunes_reader_t* reader, s32 base) { return (s32)StrToS64(reader, base); }

    //------------------------------------------------------------------------------

    f64 StrToF64(irunes_reader_t* reader)
    {
        // Evaluate sign
        s32 sign = 1;
        if (reader->peek() == '-')
        {
            sign = -1;
            reader->read();
        }

        // skip trailing zeros
        while (reader->peek() == '0')
            reader->read();

        // Convert integer part
        f64 result = 0;
        f64 value;
        while (true)
        {
            uchar32 c = reader->peek();
            if (c >= '0' && c <= '9')
            {
                value = c - '0';
                result *= 10.0;
                result += value;
                reader->read();
            }
            else
            {
                break;
            }
        }

        // Detect floating point & mantissa
        f64 mantissa = 0;
        f64 divisor  = 1;
        if (reader->peek() == '.')
        {
            reader->read();

            while (true)
            {
                uchar32 c = reader->peek();
                if (c >= '0' && c <= '9')
                {
                    value = c - '0';
                    mantissa *= 10.0;
                    mantissa += value;
                    divisor *= 10.0;
                    reader->read();
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

        uchar32 c = reader->peek();
        if (c == 'e' || c == 'E')
        {
            reader->read();

            c = reader->peek();
            if (c == '-')
            {
                sign = -1;
                reader->read();
            }
            else if (c == '+')
            {
                sign = 1;
                reader->read();
            }
            else
                sign = 1;

            while (c <= '9' && c >= '0')
            {
                u32 v = c - '0';
                power *= 10;
                power += v;
                reader->read();
            }
        }

        // Adjust result on exponent sign
        if (sign > 0)
            for (u32 i = 0; i < power; i++)
                result *= 10.0;
        else
            for (u32 i = 0; i < power; i++)
                result /= 10.0;

        return (result);
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
        RESPECT_WIDTH = 1, ///< use fixed width
        ADD_PLUS      = 2, ///< use + for positive
        SPACE_PAD     = 4, ///< use padding
        ZERO_PAD      = 8,
        LEFT_PAD      = 16,

        INT16_SIZE = 2,
        INT32_SIZE = 4,
        INT64_SIZE = 8,
    };

    s32 VSScanf(irunes_reader_t* reader, irunes_reader_t* fmt, const x_va_r_list& vr_args)
    {
        s32 i        = 0;
        s32 w        = 0;
        s32 flag     = 0;
        s32 l        = 0;
        s32 parsing  = 0;
        s32 scanned  = 0;
        s32 suppress = 0;

        while (fmt->peek() != '\0')
        {
            if (fmt->peek() != '%' && !parsing)
            {
                fmt->read();
            }
            else
            {
                if (fmt->peek() == '%')
                {
                    fmt->skip();

                    parsing = 1;
                    // size = INT32_SIZE;
                    suppress = 0;
                    w        = 0;
                    flag     = 0;
                    l        = 0;
                }

                switch (fmt->peek())
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
                        fmt->skip();

                        uchar32 c = reader->read();
                        va_r_t  r = vr_args[i++];
                        r         = c;
                        scanned++;
                        parsing = 0;
                    }
                    break;
                    case 's':
                    {
                        fmt->skip();

                        va_r_t r = vr_args[i++];

                        u32 i = 0;
                        while (reader->peek() != 0 && reader->peek() == ' ')
                            reader->read();

						runes_t* runes = r.getRunes();
						if (runes != nullptr)
						{
							runes_writer_t str_writer(*runes);

							l = 0;
							while (reader->peek() != 0 && reader->peek() != ' ')
							{
								if (!(flag & SPACE_PAD))
								{
									str_writer.write(reader->read());
								}
								else if (l < w)
								{
									str_writer.write(reader->read());
									l++;
								}
								reader->read();
							}
						}

                        scanned++;
                        parsing = 0;
                    }
                    break;

                    case 'B':
                    case 'b':
                    case 'Y':
                    case 'y':
                    {
                        fmt->skip();
                        va_r_t r = vr_args[i++];
                        bool   boolean;
                        MatchBoolStr(reader, boolean);
                        r = boolean;
                        scanned++;
                    }
                    break;

                    case 'i':
                    case 'd':
                    {
                        fmt->skip();

                        utf32::rune    allDecimalChars[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', 0};
						runes_reader_t decimalChars(allDecimalChars, allDecimalChars + 12);

                        while (reader->peek() != 0 && reader->peek() == ' ')
                            reader->read();

                        s64 n1 = 0;
                        if (SearchUntilOneOf(reader, &decimalChars))
                        {
                            n1 = StrToS64(reader, 10);
                        }

                        if (!suppress)
                        {
                            va_r_t r = vr_args[i++];
                            r        = n1;
                            scanned++;
                        }
                        else
                        {
                            i++;
                        }

                        parsing = 0;
                    }
                    break;
                    case 'u':
                    {
                        fmt->skip();

                        utf32::rune    allDecimalChars[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', 0};
						runes_reader_t decimalChars(allDecimalChars, allDecimalChars + 10);

                        while (reader->peek() != 0 && reader->peek() == ' ')
                            reader->read();

                        s64 n2 = 0;
                        if (SearchUntilOneOf(reader, &decimalChars))
                        {
                            n2 = StrToS64(reader, 10);
                        }

                        if (!suppress)
                        {
                            va_r_t r = vr_args[i++];
                            r        = n2;
                            scanned++;
                        }
                        else
                        {
                            i++;
                        }

                        parsing = 0;
                    }
                    break;
                    case 'o':
                    {
                        fmt->skip();

						utf32::rune    allDecimalChars[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', 0};
						runes_reader_t octalChars(allDecimalChars, allDecimalChars + 8);

                        while (reader->peek() != 0 && reader->peek() == ' ')
                            reader->read();

                        s64 n2 = 0;
                        if (SearchUntilOneOf(reader, &octalChars))
                        {
                            n2 = StrToS64(reader, 8);
                        }

                        if (!suppress)
                        {
                            va_r_t r = vr_args[i++];
                            r        = n2;
                            scanned++;
                        }
                        else
                        {
                            i++;
                        }

                        parsing = 0;
                    }
                    break;
                    case 'x':
                    case 'X':
                    {
                        fmt->skip();

                        utf32::rune    allHexChars[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'x', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F', 0};
						runes_reader_t hexChars(allHexChars, allHexChars + 23);

                        while (reader->peek() != 0 && reader->peek() == ' ')
                            reader->read();

                        if (w == 0)
                        {
                            u32 const varsize = vr_args[i].sizeInBytes();
                            w                 = varsize * 2;
                        }

                        u64 n2 = 0;
                        if (SearchUntilOneOf(reader, &hexChars))
                        {
                            if (w == 2)
                            {
                                u32 const strl = 2;
                                uchar32   str[strl + 1];
                                str[strl] = '\0';
                                for (s32 j = 0; j < strl; ++j)
                                    str[j] = reader->read();
								runes_reader_t str_reader(str, str + strl);
                                n2 = StrToS64(&str_reader, 16);
                            }
                            else if (w == 4)
                            {
                                u32 const strl = 4;
                                uchar32   str[strl + 1];
                                str[strl] = '\0';
                                for (s32 j = 0; j < strl; ++j)
                                {
                                    uchar32 c = reader->read();
                                    str[j]    = c;
                                    if (c == '\0')
                                        break;
                                }
								runes_reader_t str_reader(str, str + strl);
								n2 = StrToS64(&str_reader, 16);
                            }
                            else if (w == 8)
                            {
                                u32 const strl = 8;
                                uchar32   str[strl + 1];
                                str[strl] = '\0';
                                for (s32 j = 0; j < strl; ++j)
                                {
                                    uchar32 c = reader->read();
                                    str[j]    = c;
                                    if (c == '\0')
                                        break;
                                }
								runes_reader_t str_reader(str, str + strl);
								n2 = StrToS64(&str_reader, 16);
                            }
                            else // if (w == 16)
                            {
                                u32 const strl = 16;
                                uchar32   str[strl + 1];
                                str[strl] = '\0';
                                for (s32 j = 0; j < strl; ++j)
                                {
                                    uchar32 c = reader->read();
                                    str[j]    = c;
                                    if (c == '\0')
                                        break;
                                }
								runes_reader_t str_reader(str, str + strl);
								n2 = (u64)StrToS64(&str_reader, 16);
                            }
                        }

                        if (!suppress)
                        {
                            va_r_t r = vr_args[i++];
                            r        = n2;
                            scanned++;
                        }
                        else
                        {
                            i++;
                        }

                        parsing = 0;
                    }
                    break;
                    case 'f':
                    case 'g':
                    case 'G':
                    case 'e':
                    case 'E':
                    {
                        fmt->skip();

                        utf32::rune    allFloatChars[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', 'e', '+', '-', 0};
						runes_reader_t floatChars(allFloatChars, allFloatChars + 14);

                        while (reader->peek() != 0 && reader->peek() == ' ')
                            reader->read();

                        f64 n3 = 0;
                        if (SearchUntilOneOf(reader, &floatChars))
                        {
                            n3 = StrToF64(reader);
                        }

                        if (!suppress)
                        {
                            va_r_t r = vr_args[i++];
                            r        = n3;
                            scanned++;
                        }
                        else
                        {
                            i++;
                        }

                        parsing = 0;
                    }
                    break;
                    case 'l':
                    case 'L':
                        fmt->skip();
                        // size = INT64_SIZE;
                        break;
                    case 'h':
                    case 'n':
                        fmt->skip();
                        // size = INT16_SIZE;
                        break;
                    case '*':
                        fmt->skip();
                        suppress = 1;
                        break;
                    default:
                        parsing = 0;
                        fmt->skip();
                        break;
                }
            }
        }

        return scanned;
    }

    s32 sscanf(crunes_t & str, crunes_t const& fmt, X_VA_R_ARGS_16)
    {
        x_va_r_list    vr_args(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16);
        runes_reader_t buf_reader(str);
        runes_reader_t fmt_reader(fmt);
        s32            scanned = VSScanf(&buf_reader, &fmt_reader, vr_args);
		str = buf_reader.get_current();
		return scanned;
    }

    s32 vsscanf(crunes_t & str, crunes_t const& fmt, const x_va_r_list& vr_args)
    {
        runes_reader_t buf_reader(str);
        runes_reader_t fmt_reader(fmt);
        s32            scanned = VSScanf(&buf_reader, &fmt_reader, vr_args);
		str = buf_reader.get_current();
        return scanned;
    }

}; // namespace xcore

#endif // ifndef SPU
