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
              We need to keep linked list nodes as small as possible, since
              there will be many of them.
              If you limit a piece of string to say 256 bytes, then you could
              also store the length of the string in the first byte.

        Multiple arenas:
        - Arena only holding str8_t structs, no actual string data.
        - Arena holding 'const char*' string information
        - Arena holding string data only
          - 1024, 512, 256, 128, 64, 32, 16 ?

        More control?:
        What about having the user create the whole system in an arena, then
        the user can destroy the whole string system at once by calling
        narena::destroy on the arena.
    */

    struct arena_t;
    struct str8_t;

    struct iter8_t
    {
        str8_t* str;
        u32     index;
    };

    // String iteration, character by character
    iter8_t str_begin(str8_t* str);
    uchar32 str_read(iter8_t& it);
    bool    str_at_end(iter8_t const& it);

    // String creation and destruction
    str8_t* str_make(u32 len = 0);
    str8_t* str_make(const char* cstr, u32 len);
    void    str_free(str8_t* str);

    template <typename T, uint_t N>
    str8_t* str_literal(const T (&literal)[N])
    {
        static_assert(sizeof(T) == 1, "Only char literals are supported");
        return str_make((const char*)literal, N - 1);
    }

    str8_t* str_append(str8_t** strs, u32 count);
    str8_t* str_append(str8_t* strA, const char* str);
    str8_t* str_append(str8_t* strA, str8_t* strB);
    str8_t* str_append(str8_t* strA, str8_t* strB, str8_t* strC);

    str8_t* str_join(str8_t** strs, u32 count, uchar32 sep = ',');
    str8_t* str_join(str8_t* strA, str8_t* strB, uchar32 sep = ',');
    str8_t* str_join(str8_t* strA, str8_t* strB, str8_t* strC, uchar32 sep = ',');

    str8_t* str_view(str8_t* str, u32 from, u32 to);
    str8_t* str_find(str8_t* str, uchar32 ch);
    str8_t* str_find(str8_t* str, str8_t* substr);
    str8_t* str_rfind(str8_t* str, uchar32 ch);
    str8_t* str_rfind(str8_t* str, str8_t* substr);

    bool str_equal(str8_t* strA, str8_t* strB);
    s32  str_compare(str8_t* strA, str8_t* strB);
    bool str_starts_with(str8_t* str, str8_t* prefix);
    bool str_ends_with(str8_t* str, str8_t* suffix);

    str8_t* str_upper(str8_t* str);
    str8_t* str_lower(str8_t* str);

    str8_t* str_replace(str8_t* str, uchar32 oldChar, uchar32 newChar);
    str8_t* str_replace(str8_t* str, str8_t* oldStr, str8_t* newStr);
    str8_t* str_insert_before(str8_t* str, str8_t* insert);
    str8_t* str_insert_after(str8_t* str, str8_t* insert);

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
