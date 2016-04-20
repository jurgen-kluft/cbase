#include "xbase\x_target.h"
#include "xbase\x_string_ascii.h"
#include "xbase\x_va_list.h"

#ifndef SPU

/**
 * xCore namespace
 */
namespace xcore
{
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
		RESPECT_WIDTH    = 1,    ///< use fixed width
		ADD_PLUS        = 2,    ///< use + for positive
		SPACE_PAD        = 4,    ///< use padding
		ZERO_PAD        = 8,
		LEFT_PAD         = 16,

		INT16_SIZE        = 2,
		INT32_SIZE        = 4,
		INT64_SIZE        = 8,
	};

	s32 VSScanf(const char *buf, const char *fmt, const x_va_r_list& vr_args)
	{
		s32 i        = 0;
		s32 w        = 0;
		s32 flag     = 0;
		s32 l        = 0;
		s32 parsing  = 0;
		s32 scanned  = 0;
		//s32 size     = 0;
		s32 suppress = 0;
	    
		const char *base = buf;
	    
		while (*fmt != 0) 
		{
			if (*fmt != '%' && !parsing) 
			{
				fmt++;
			} 
			else
			{
				if (*fmt == '%')
				{
					fmt++;
					parsing = 1;
					//size = INT32_SIZE;
					suppress = 0;
					w = 0;
					flag = 0;
					l = 0;
				}
	            
				switch(*fmt)
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
							w = StrToS32(fmt,&base,10);
							flag |= SPACE_PAD;
							fmt = base-1;
						}
						break;
					case 'c':
						{
							char    c = *buf++;
							x_va_r  r = vr_args[i++];
							r = (u8)c;
							scanned++;
							parsing = 0;
						} break;
					case 's':
						{
							x_va_r r = vr_args[i++];

							char* str = (char*)r;
							
							const u32 str_len = r.var();
							u32 i = 0;

							while (*buf != 0 && IsSpace(*buf))
							{
								buf++;
							}
	                        
							l = 0;
							while (*buf != 0 && !IsSpace(*buf))
							{
								if (!(flag & SPACE_PAD)) 
								{
									if (i < str_len)
										str[i++] = *buf;
								} 
								else if (l < w)
								{
									if (i < str_len)
										str[i++] = *buf;
									l++;
								}
								buf++;
							}

							scanned++;
							parsing = 0;
						} break;
					case 'i':
					case 'd':
						{
							buf = Find(buf, "1234567890-+");
							s64 n1 = StrToS64(buf, 10, &base);
							buf = base;
	                        
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
							buf = Find(buf, "1234567890");
							s64 n2 = StrToS64(buf, 10, &base);
							buf = base;
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
							buf = Find(buf, "1234567890xabcdefABCDEF");

							u32 const varsize = vr_args[i].sizeInBytes();
							if (w==0)
								w = varsize * 2;

							u64 n2 = 0;
							if (w==2)
							{
								u32 const strl = 2;
								char str[strl + 1];
								str[strl]= '\0';
								for (s32 j=0; j<strl; ++j)
									str[j] = buf[j];
								n2 = (u64)StrToS64(str, 16, &base);
								buf += (base - str);
							}
							else if (w == 4)
							{
								u32 const strl = 4;
								char str[strl + 1];
								str[strl]= '\0';
								for (s32 j=0; j<strl; ++j)
									str[j] = buf[j];
								n2 = (u64)StrToS64(str, 16, &base);
								buf += (base - str);
							}
							else if (w == 8)
							{
								u32 const strl = 8;
								char str[strl + 1];
								str[strl]= '\0';
								for (s32 j=0; j<strl; ++j)
									str[j] = buf[j];
								n2 = (u64)StrToS64(str, 16, &base);
								buf += (base - str);
							}
							else // if (w == 16)
							{
								u32 const strl = 16;
								char str[strl + 1];
								str[strl]= '\0';
								for (s32 j=0; j<strl; ++j)
									str[j] = buf[j];
								n2 = (u64)StrToS64(str, 16, &base);
								buf += (base - str);
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
							buf = Find(buf, "1234567890.e+-");
							//TODO: delete an arguments to fix compiling error
							f64 n3 = StrToF64(buf, &base);
							buf = base;
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
						//size = INT64_SIZE;
						break;
					case 'h':
					case 'n':
						//size = INT16_SIZE;
						break;
					case '*':
						suppress = 1;
						break;
					default:
						parsing = 0;
						break;
				}
				fmt++;
			}
		}
	    
		return scanned;
	}

	s32 SScanf(const char *buf, const char *fmt, const x_va_r_list& vr_args)
	{
		return VSScanf(buf, fmt, vr_args);
	}

	s32 SScanf(const char *buf, const char *fmt, X_VA_R_ARGS_16)
	{
		x_va_r_list vr_args;
		vr_args.add(v1);
		vr_args.add(v2);
		vr_args.add(v3);
		vr_args.add(v4);
		vr_args.add(v5);
		vr_args.add(v6);
		vr_args.add(v7);
		vr_args.add(v8);
		vr_args.add(v9);
		vr_args.add(v10);
		vr_args.add(v11);
		vr_args.add(v12);
		vr_args.add(v13);
		vr_args.add(v14);
		vr_args.add(v15);
		vr_args.add(v16);

		return VSScanf(buf, fmt, vr_args);
	}
};
/**
 *  END xCore namespace
 */

#endif // ifndef SPU
