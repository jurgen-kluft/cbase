#ifndef __CBASE_DOUBLE_CONVERT_H__
#define __CBASE_DOUBLE_CONVERT_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    struct DoubleConvert
    {
        enum EDoubleConvert
        {
            FLAG_HAVE_WIDTH    = 0x0001,
            FLAG_LEFT_JUSTIFY  = 0x0002,
            FLAG_SHARP         = 0x0004,
            FLAG_PRINT_PLUS    = 0x0008,
            FLAG_SPACE_IF_PLUS = 0x0010,
            FLAG_PAD_WITH_ZERO = 0x0020,
            FLAG_UPPERCASE     = 0x0040,
            DEFAULT_PRECISION  = 6,
        };
        u8 value;
    };

    //  Print IEEE 754 floating-point double precision value to string
    //
    //  @param  outbuf            Address of variable with a pointer to output buffer filled by the function.
    //                            On entry, this variable is initialized by caller.
    //                            On exit, this variable points to the end of printed string.
    //
    //  @param  outbuf_size       Size of output buffer filled by the function.
    //                            On entry, this variable is initialized by caller to maximum allowed size.
    //                            On exit, this variable contains size of unused portion of the output buffer.
    //
    //  @param  value             Input value (IEEE 754 floating-point double precision).
    //
    //  @param  format_char       Format char. Either 'e', or 'f', or 'g'. Refer to printf(3) manual for details.
    //
    //  @param  format_flags      Any combination of the above (DCONVSTR_FLAG_*).
    //
    //  @param  format_width      Format width. Used only if DCONVSTR_FLAG_HAVE_WIDTH bit is set in flags.
    //                            Refer to printf(3) manual for details.
    //
    //  @param  format_precision  Format precision. Set it to DCONVSTR_DEFAULT_PRECISION if unsure.
    //                            Refer to printf(3) manual for details.
    //
    //  @returns  1  if value was successfully converted to string.
    //            0  if there is not enough room in buffer or internal error happened during conversion.
    //
    //
    s32 dconvstr_print(char** outbuf, s32* outbuf_size, double value, s32 format_char, u32 format_flags, s32 format_width, s32 format_precision);

    //  Convert string to IEEE 754 floating-point double precision value
    //
    //  @param  input          Input buffer, C-style string. Filled by caller.
    //
    //  @param  input_end      Address of pointer to end of scanned value in input buffer.
    //                         Filled by function if address is not NULL.
    //
    //  @param  output         Conversion result (IEEE 754 floating-point double precision).
    //                         Set to 0.0 if string in input buffer has syntax errors.
    //
    //  @param  output_erange  Address of overflow/underflow flag variable, filled by function.
    //                         0  if there is no overflow/underflow condition
    //                         1  if there is overflow/underflow condition: strtod(3) would set errno = ERANGE
    //
    //  @returns  1  if there were no internal errors
    //            0  if there was internal error during conversion.
    //
    //  In general, interface of this function is similar to strtod(3), except for returning overflow
    //  condition instead of setting errno. If you want just to convert C-style string to double with
    //  error checking, then set input_end != NULL and use ( ret_value != 0 )&&( **input_end == 0 )
    //  condition as an indication of successful conversion.
    //
    s32 dconvstr_scan(const char* input, const char** input_end, double* output, s32* output_erange);

} // namespace ncore

#endif // __CBASE_DOUBLE_CONVERT_H__