#ifndef __CBASE_STR_H__
#define __CBASE_STR_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_debug.h"
#include "cbase/c_va_list.h"

namespace ncore
{
    /*
        Thoughts and notes on a string type that is based on arena allocation.
        The behavior is what is called copy-on-write, so you are not able to
        directly modify the string data, but instead you create a new string
        data when modifying it.

        Note: What if a string is always a node in a linked list of pieces?
              This way appending strings is cheap, and only when you need
              a contiguous string you flatten the pieces into a single string.
              Also operations like replace, insert, do not require copying
              strings, but instead just require manipulating the list.

        Note: A hard limit on the length of a string.
              If a string comes in that is longer than the limit, it is split
              into multiple pieces. This way operations on pieces are always
              bounded in time and space.
              We need to keep linked list nodes as small as possible, so
              that they fit into CPU caches.
              If you limit a piece of string to say 256 bytes, then you could
              also store the length of the string in the first byte.

        Multiple arenas:
        - Arena only holding str8_t structs, no actual string data.
        - Arena holding 'const char*' string information
        - Arena holding string data only
          - 256, 128, 64, 32 ?
    */

    struct str8_t
    {
        u8* data;      // string data (not null terminated!)
        u32 previous;  // index of previous str8_t in arena, or INVALID_INDEX
        u32 next;      // index of next str8_t in arena, or INVALID_INDEX
    };

    struct iter8_t
    {
        str8_t* str;
        u32     index;
    };

    iter8_t str_begin(str8_t* str);
    uchar32 str_read(iter8_t& it);
    bool    str_at_end(iter8_t const& it);

    str8_t* str_make(u32 len = 0);
    str8_t* str_make(const char* cstr, u32 len);
    void    str_free(str8_t* str);

    template <const char, uint_t N>
    str8_t* str_lit(T (&)[N])
    {
        return str_make(T, N - 1);
    }

    str8_t* str_append(str8_t** strs, u32 count);
    str8_t* str_append(str8_t* strA, const char* str);
    str8_t* str_append(str8_t* strA, str8_t* strB);
    str8_t* str_append(str8_t* strA, str8_t* strB, str8_t* strC);

    str8_t* str_join(str8_t** strs, u32 count, uchar32 sep = ',');
    str8_t* str_join(str8_t* strA, str8_t* strB, uchar32 sep = ',');
    str8_t* str_join(str8_t* strA, str8_t* strB, str8_t* strC, uchar32 sep = ',');

    str8_t* str_view(str8_t* str, u32 from, u32 to);

    str8_t* str_upper(str8_t* str);
    str8_t* str_lower(str8_t* str);

    str8_t* str_trim(str8_t* str);
    str8_t* str_trim_left(str8_t* str);
    str8_t* str_trim_right(str8_t* str);

    str8_t* str_format_internal(str8_t* format, va_t* argv, s32 argc);

    template <typename... Args>
    inline str8_t* str_format(str8_t* format, Args&&... _args)
    {
        const va_t argv[] = {_args...};
        return str_format_internal(format, argv, DARRAYSIZE(argv));
    }

    void str_flatten(str8_t* str, char* buffer, u32 buffer_size);

}  // namespace ncore

#endif  // __CBASE_STR_H__
