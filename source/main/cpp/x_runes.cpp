#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_console.h"
#include "xbase/x_va_list.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

namespace xcore
{
    namespace utf
    {
        static s32 countof_runes(ascii::pcrune str, ascii::pcrune end);
        static s32 countof_runes(utf8::pcrune str, utf8::pcrune end);
        static s32 countof_runes(utf16::pcrune str, utf16::pcrune end);
        static s32 countof_runes(utf32::pcrune str, utf32::pcrune end);

        static ascii::prune  endof(ascii::prune str, ascii::pcrune eos);
        static ascii::pcrune endof(ascii::pcrune str, ascii::pcrune eos);
        static utf8::prune   endof(utf8::prune str, utf8::pcrune eos);
        static utf8::pcrune  endof(utf8::pcrune str, utf8::pcrune eos);
        static utf16::prune  endof(utf16::prune str, utf16::pcrune eos);
        static utf16::pcrune endof(utf16::pcrune str, utf16::pcrune eos);
        static utf32::prune  endof(utf32::prune str, utf32::pcrune eos);
        static utf32::pcrune endof(utf32::pcrune str, utf32::pcrune eos);

        // UTF sequence sizes
        static s32 sequence_sizeof_utf8(uchar32 c);
        static s32 sequence_sizeof_utf16(uchar32 c);

        // Peek
        static uchar32 peek(runes_t const& str, runes_t::ptr_t const& ptr);
        static uchar32 peek(ascii::runes_t const& str, runes_t::ptr_t const& ptr);
        static uchar32 peek(utf8::runes_t const& str, runes_t::ptr_t const& ptr);
        static uchar32 peek(utf16::runes_t const& str, runes_t::ptr_t const& ptr);
        static uchar32 peek(utf32::runes_t const& str, runes_t::ptr_t const& ptr);

        static uchar32 peek(crunes_t const& str, crunes_t::ptr_t const& ptr);
        static uchar32 peek(ascii::crunes_t const& str, crunes_t::ptr_t const& ptr);
        static uchar32 peek(utf8::crunes_t const& str, crunes_t::ptr_t const& ptr);
        static uchar32 peek(utf16::crunes_t const& str, crunes_t::ptr_t const& ptr);
        static uchar32 peek(utf32::crunes_t const& str, crunes_t::ptr_t const& ptr);

        // Skip
        static bool skip(ascii::rune*& str, ascii::rune const* begin, ascii::rune const* end, s32 count);
        static bool skip(utf8::rune*& str, utf8::rune const* begin, utf8::rune const* end, s32 count);
        static bool skip(utf16::rune*& str, utf16::rune const* begin, utf16::rune const* end, s32 count);
        static bool skip(utf32::rune*& str, utf32::rune const* begin, utf32::rune const* end, s32 count);

        static bool skip(ascii::rune const*& str, ascii::rune const* begin, ascii::rune const* end, s32 count);
        static bool skip(utf8::rune const*& str, utf8::rune const* begin, utf8::rune const* end, s32 count);
        static bool skip(utf16::rune const*& str, utf16::rune const* begin, utf16::rune const* end, s32 count);
        static bool skip(utf32::rune const*& str, utf32::rune const* begin, utf32::rune const* end, s32 count);

        // Compare
        static s32 compare(ascii::pcrune stra, ascii::pcrune strb);
        static s32 compare(ascii::pcrune stra, utf32::pcrune strb);
        static s32 compare(utf32::pcrune stra, utf32::pcrune strb);
        static s32 compare(utf32::pcrune stra, ascii::pcrune strb);
        static s32 compare(ascii::crunes_t const& cstra, utf32::pcrune strb);

        // Read
        static uchar32 read(ascii::pcrune& str, ascii::pcrune end);
        static uchar32 read(utf8::pcrune& str, utf8::pcrune end);
        static uchar32 read(utf16::pcrune& str, utf16::pcrune end);
        static uchar32 read(utf32::pcrune& str, utf32::pcrune end);

        static uchar32 read(ascii::prune& str, ascii::prune end);
        static uchar32 read(utf8::prune& str, utf8::prune end);
        static uchar32 read(utf16::prune& str, utf16::prune end);
        static uchar32 read(utf32::prune& str, utf32::prune end);

        static uchar32 read(ascii::crunes_t const& str, ascii::pcrune& cursor);
        static uchar32 read(utf8::crunes_t const& str, utf8::pcrune& cursor);
        static uchar32 read(utf16::crunes_t const& str, utf16::pcrune& cursor);
        static uchar32 read(utf32::crunes_t const& str, utf32::pcrune& cursor);

        static uchar32 read(ascii::runes_t const& str, ascii::prune& cursor);
        static uchar32 read(utf8::runes_t const& str, utf8::prune& cursor);
        static uchar32 read(utf16::runes_t const& str, utf16::prune& cursor);
        static uchar32 read(utf32::runes_t const& str, utf32::prune& cursor);

        static uchar32 read(runes_t const& str, runes_t::ptr_t& cursor);
        static uchar32 read(crunes_t const& str, crunes_t::ptr_t& cursor);

        // Write
        static bool write(uchar32 c, ascii::prune& dest, ascii::pcrune end);
        static bool write(uchar32 c, utf8::prune& dest, utf8::pcrune end);
        static bool write(uchar32 c, utf16::prune& dest, utf16::pcrune end);
        static bool write(uchar32 c, utf32::prune& dst, utf32::pcrune end);
        static bool write(uchar32 c, ascii::runes_t const& dst, ascii::prune& cursor);
        static bool write(uchar32 c, utf8::runes_t const& dst, utf8::prune& cursor);
        static bool write(uchar32 c, utf16::runes_t const& dst, utf16::prune& cursor);
        static bool write(uchar32 c, utf32::runes_t const& dst, utf32::prune& cursor);

        // Terminate
        static bool terminate_str(ascii::runes_t& dst);
        static bool terminate_str(utf8::runes_t& dst);
        static bool terminate_str(utf16::runes_t& dst);
        static bool terminate_str(utf32::runes_t& dst);

        // const char* -> ascii / utf16 / utf32
        static void copy(char const* sstr, ascii::runes_t& dstr, bool write_terminator);
        static void copy(char const* sstr, utf16::runes_t& dstr, bool write_terminator);
        static void copy(char const* sstr, utf32::runes_t& dstr, bool write_terminator);

        // -> ASCII
        static void copy(ascii::crunes_t const& sstr, ascii::runes_t& dstr, bool write_terminator);
        static void copy(utf16::crunes_t const& sstr, ascii::runes_t& dstr, bool write_terminator);
        static void copy(utf32::crunes_t const& sstr, ascii::runes_t& dstr, bool write_terminator);

        // -> UTF-16
        static void copy(ascii::crunes_t const& sstr, utf16::runes_t& dstr, bool write_terminator);
        static void copy(utf16::crunes_t const& sstr, utf16::runes_t& dstr, bool write_terminator);
        static void copy(utf32::crunes_t const& sstr, utf16::runes_t& dstr, bool write_terminator);

        // -> UTF-32
        static void copy(ascii::crunes_t const& sstr, utf32::runes_t& dstr, bool write_terminator);
        static void copy(utf16::crunes_t const& sstr, utf32::runes_t& dstr, bool write_terminator);
        static void copy(utf32::crunes_t const& sstr, utf32::runes_t& dstr, bool write_terminator);

        static bool is_eos(uchar const* str);
        static bool is_eos(uchar8 const* str);
        static bool is_eos(uchar16 const* str);
        static bool is_eos(uchar32 const* str);

        static bool can_read(ascii::runes_t const& str, ascii::pcrune cursor);
        static bool can_read(utf8::runes_t const& str, utf8::pcrune cursor);
        static bool can_read(utf16::runes_t const& str, utf16::pcrune cursor);
        static bool can_read(utf32::runes_t const& str, utf32::pcrune cursor);

        static bool can_read(ascii::crunes_t const& str, ascii::pcrune cursor);
        static bool can_read(utf8::crunes_t const& str, utf8::pcrune cursor);
        static bool can_read(utf16::crunes_t const& str, utf16::pcrune cursor);
        static bool can_read(utf32::crunes_t const& str, utf32::pcrune cursor);

        static bool can_write(ascii::runes_t const& str, ascii::prune cursor);
        static bool can_write(utf8::runes_t const& str, utf8::prune cursor);
        static bool can_write(utf16::runes_t const& str, utf16::prune cursor);
        static bool can_write(utf32::runes_t const& str, utf32::prune cursor);

        static bool read_is_crln(ascii::runes_t const& str, ascii::prune cursor);
        static bool read_is_crln(utf8::runes_t const& str, utf8::prune cursor);
        static bool read_is_crln(utf16::runes_t const& str, utf16::prune cursor);
        static bool read_is_crln(utf32::runes_t const& str, utf32::prune cursor);
        static bool read_is_crln(ascii::crunes_t const& str, ascii::pcrune cursor);
        static bool read_is_crln(utf8::crunes_t const& str, utf8::pcrune cursor);
        static bool read_is_crln(utf16::crunes_t const& str, utf16::pcrune cursor);
        static bool read_is_crln(utf32::crunes_t const& str, utf32::pcrune cursor);

        s32 countof_runes(ascii::pcrune str, ascii::pcrune end)
        {
            end = endof(str, end);
            return s32(end - str);
        }

        s32 countof_runes(utf8::pcrune str, utf8::pcrune end)
        {
            s32 count = 0;
            utf8::pcrune iter = str;
            while (iter < end)
            {
                uchar32 c = iter[0];
                s32 const l = sequence_sizeof_utf8(c);
                count += 1;
                iter += l;
            }
            return count;
        }

        s32 countof_runes(utf16::pcrune str, utf16::pcrune end)
        {
            s32 count = 0;
            utf16::pcrune iter = str;
            while (iter < end)
            {
                uchar32 c = iter[0];
                s32 const l = sequence_sizeof_utf16(c);
                count += 1;
                iter += l;
            }
            return count;
        }

        s32 countof_runes(utf32::pcrune str, utf32::pcrune end)
        {
            end = endof(str, end);
            return s32(end - str);
        }

        ascii::prune endof(ascii::prune str, ascii::pcrune eos)
        {
            ascii::prune end = str;
            if (eos == nullptr)
            {
                while (*end != ascii::TERMINATOR)
                    end++;
            }
            else
            {
                while (*end != ascii::TERMINATOR && end < eos)
                    end++;
            }
            return end;
        }

        ascii::pcrune endof(ascii::pcrune str, ascii::pcrune eos)
        {
            ascii::pcrune end = str;
            if (eos == nullptr)
            {
                while (*end != ascii::TERMINATOR)
                    end++;
            }
            else
            {
                while (*end != ascii::TERMINATOR && end < eos)
                    end++;
            }
            return end;
        }

        utf8::prune endof(utf8::prune str, utf8::pcrune eos)
        {
            utf8::prune end = str;
            if (eos == nullptr)
            {
                while (*end != utf8::TERMINATOR)
                {
                    utf8::rune c = *end;
                    s32 const l = sequence_sizeof_utf8(c);
                    end += l;
                }
            }
            else
            {
                while (*end != utf8::TERMINATOR && end < eos)
                {
                    utf8::rune c = *end;
                    s32 const l = sequence_sizeof_utf8(c);
                    end += l;
                }
            }
            return end;
        }

        utf8::pcrune endof(utf8::pcrune str, utf8::pcrune eos)
        {
            utf8::pcrune end = str;
            if (eos == nullptr)
            {
                while (*end != utf8::TERMINATOR)
                {
                    utf8::rune c = *end;
                    s32 const l = sequence_sizeof_utf8(c);
                    end += l;
                }
            }
            else
            {
                while (*end != utf8::TERMINATOR && end < eos)
                {
                    utf8::rune c = *end;
                    s32 const l = sequence_sizeof_utf8(c);
                    end += l;
                }
            }
            return end;
        }

        utf16::prune endof(utf16::prune str, utf16::pcrune eos)
        {
            utf16::prune end = str;
            if (eos == nullptr)
            {
                while (*end != utf16::TERMINATOR)
                {
                    utf16::rune c = *end;
                    s32 const l = sequence_sizeof_utf16(c);
                    end += l;
                }
            }
            else
            {
                while (*end != utf16::TERMINATOR && end < eos)
                {
                    utf16::rune c = *end;
                    s32 const l = sequence_sizeof_utf16(c);
                    end += l;
                }
            }
            return end;
        }

        utf16::pcrune endof(utf16::pcrune str, utf16::pcrune eos)
        {
            utf16::pcrune end = str;
            if (eos == nullptr)
            {
                while (*end != utf16::TERMINATOR)
                {
                    utf16::rune c = *end;
                    s32 const l = sequence_sizeof_utf16(c);
                    end += l;
                }
            }
            else
            {
                while (*end != utf16::TERMINATOR && end < eos)
                {
                    utf16::rune c = *end;
                    s32 const l = sequence_sizeof_utf16(c);
                    end += l;
                }
            }
            return end;
        }

        utf32::prune endof(utf32::prune str, utf32::pcrune eos)
        {
            utf32::prune end = str;
            if (eos == nullptr)
            {
                while (*end != utf32::TERMINATOR)
                    end++;
            }
            else
            {
                while (*end != utf32::TERMINATOR && end < eos)
                    end++;
            }
            return end;
        }

        utf32::pcrune endof(utf32::pcrune str, utf32::pcrune eos)
        {
            utf32::pcrune end = str;
            if (eos == nullptr)
            {
                while (*end != utf32::TERMINATOR)
                    end++;
            }
            else
            {
                while (*end != utf32::TERMINATOR && end < eos)
                    end++;
            }
            return end;
        }

        s32 sequence_sizeof_utf8(uchar32 c)
        {
            if (c < 0x80)
                return 1;
            else if ((c >> 5) == 0x6)
                return 2;
            else if ((c >> 4) == 0xe)
                return 3;
            else if ((c >> 3) == 0x1e)
                return 4;
            return 0;
        }

        s32 sequence_sizeof_utf16(uchar32 c)
        {
            if (c < 0xd800)
                return 1;
            else if (c < 0xdc00)
                return 2;
            return 1;
        }

        uchar32 peek(runes_t const& str, runes_t::ptr_t const& ptr)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return peek(str.m_runes.m_ascii, ptr);
                case utf8::TYPE: return peek(str.m_runes.m_utf8, ptr);
                case utf16::TYPE: return peek(str.m_runes.m_ascii, ptr);
                case utf32::TYPE: return peek(str.m_runes.m_ascii, ptr);
                default: ASSERT(false); break;
            }
            return '\0';
        }

        uchar32 peek(ascii::runes_t const& str, runes_t::ptr_t const& ptr)
        {
            ascii::pcrune src = ptr.m_ptr.m_ascii;
            return read(src, str.m_end);
        }

        uchar32 peek(utf8::runes_t const& str, runes_t::ptr_t const& ptr)
        {
            utf8::pcrune src = ptr.m_ptr.m_utf8;
            return read(src, str.m_end);
        }

        uchar32 peek(utf16::runes_t const& str, runes_t::ptr_t const& ptr)
        {
            utf16::pcrune src = ptr.m_ptr.m_utf16;
            return read(src, str.m_end);
        }

        uchar32 peek(utf32::runes_t const& str, runes_t::ptr_t const& ptr)
        {
            utf32::pcrune src = ptr.m_ptr.m_utf32;
            return read(src, str.m_end);
        }

        uchar32 peek(crunes_t const& str, crunes_t::ptr_t const& ptr)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return peek(str.m_runes.m_ascii, ptr);
                case utf8::TYPE: return peek(str.m_runes.m_utf8, ptr);
                case utf16::TYPE: return peek(str.m_runes.m_ascii, ptr);
                case utf32::TYPE: return peek(str.m_runes.m_ascii, ptr);
                default: ASSERT(false); break;
            }
            return '\0';
        }

        uchar32 peek(ascii::crunes_t const& str, crunes_t::ptr_t const& ptr)
        {
            ascii::pcrune src = ptr.m_ptr.m_ascii;
            return read(src, str.m_end);
        }

        uchar32 peek(utf8::crunes_t const& str, crunes_t::ptr_t const& ptr)
        {
            utf8::pcrune src = ptr.m_ptr.m_utf8;
            return read(src, str.m_end);
        }

        uchar32 peek(utf16::crunes_t const& str, crunes_t::ptr_t const& ptr)
        {
            utf16::pcrune src = ptr.m_ptr.m_utf16;
            return read(src, str.m_end);
        }

        uchar32 peek(utf32::crunes_t const& str, crunes_t::ptr_t const& ptr)
        {
            utf32::pcrune src = ptr.m_ptr.m_utf32;
            return read(src, str.m_end);
        }

        bool skip(ascii::rune*& str, ascii::rune const* begin, ascii::rune const* end, s32 count)
        {
            ascii::rune const* cursor = str;
            bool               result = skip(cursor, begin, end, count);
            str += (cursor - str);
            return result;
        }

        bool skip(utf8::rune*& str, utf8::rune const* begin, utf8::rune const* end, s32 count)
        {
            utf8::rune const* cursor = str;
            bool              result = skip(cursor, begin, end, count);
            str += (cursor - str);
            return result;
        }

        bool skip(utf16::rune*& str, utf16::rune const* begin, utf16::rune const* end, s32 count)
        {
            utf16::rune const* cursor = str;
            bool               result = skip(cursor, begin, end, count);
            str += (cursor - str);
            return result;
        }

        bool skip(utf32::rune*& str, utf32::rune const* begin, utf32::rune const* end, s32 count)
        {
            utf32::rune const* cursor = str;
            bool               result = skip(cursor, begin, end, count);
            str += (cursor - str);
            return result;
        }

        bool skip(ascii::rune const*& str, ascii::rune const* begin, ascii::rune const* end, s32 count)
        {
            if (count < 0)
            {
                ASSERT(begin != nullptr);
                while ((str > begin) && count < 0)
                {
                    str -= 1;
                    uchar32 c = *str;
                    if (c == ascii::TERMINATOR)
                        break;
                    count += 1;
                }
            }
            else
            {
                uchar32 c = *str;
                while (c != ascii::TERMINATOR && (end == NULL || str < end) && count > 0)
                {
                    str++;
                    count -= 1;
                }
            }
            return count == 0;
        }

        bool skip(utf8::rune const*& str, utf8::rune const* begin, utf8::rune const* end, s32 count)
        {
            ASSERT(begin != nullptr);
            ASSERT(end != nullptr);
            if (count < 0)
            {
                // UTF-8 backwards iteration can be quite slow, since we need to iterate forwards to find
                // (str - count)
                utf8::pcrune iter  = begin;
                utf8::pcrune prev1 = iter;
                utf8::pcrune prev2 = nullptr;

                // First step forwards no matter the count, until we reach the end
                s32 const forward_count = -count;
                while (skip(iter, begin, str, forward_count))
                {
                    prev2 = prev1;
                    prev1 = iter;
                }
                if (prev2 != nullptr)
                {
                    // Then step forwards by 1
                    if (forward_count > 1)
                    {
                        prev1 = prev2;
                        do
                        {
                            prev2 = prev1;
                            skip(prev1, begin, str, 1);
                            iter = prev1;
                        } while (skip(iter, begin, str, forward_count));
                    }
                    str   = prev2;
                    count = 0;
                }
            }
            else
            {
                uchar32 c = *str;
                while (c != utf8::TERMINATOR && count > 0)
                {
                    s32 const l = sequence_sizeof_utf8(c);
                    if ((str + l) > end)
                        return false;

                    switch (l)
                    {
                        case 0:
                            c = '?'; // Illegal character in utf8, replace with '?'
                            str++;
                            break;
                        case 1: 
                            c = *str++;
                            break;
                        case 2:
                            str++;
                            c = ((c << 6) & 0x7ff) + ((str[0]) & 0x3f);
                            str++;
                            break;
                        case 3:
                            str++;
                            c = ((c << 12) & 0xffff) + (((str[0]) << 6) & 0xfff);
                            str++;
                            c += (str[0]) & 0x3f;
                            str++;
                            break;
                        case 4:
                            str++;
                            c = ((c << 18) & 0x1fffff) + (((str[0]) << 12) & 0x3ffff);
                            str++;
                            c += ((str[0]) << 6) & 0xfff;
                            str++;
                            c += (str[0]) & 0x3f;
                            str++;
                            break;
                    }
                    count -= 1;
                }
            }
            return count == 0;
        }

        bool skip(utf16::rune const*& str, utf16::rune const* begin, utf16::rune const* end, s32 count)
        {
            if (count < 0)
            {
                // UTF-16 backwards iteration can be quite slow, since we need to iterate forwards to find
                // (str - count)
                utf16::pcrune iter  = begin;
                utf16::pcrune prev1 = iter;
                utf16::pcrune prev2 = nullptr;

                // First step forwards no matter the count, until we reach the end
                s32 const forward_count = -count;
                while (skip(iter, begin, str, forward_count))
                {
                    prev2 = prev1;
                    prev1 = iter;
                }
                if (prev2 != nullptr)
                {
                    // Then step forwards by 1
                    if (forward_count > 1)
                    {
                        prev1 = prev2;
                        do
                        {
                            prev2 = prev1;
                            skip(prev1, begin, str, 1);
                            iter = prev1;
                        } while (skip(iter, begin, str, forward_count));
                    }
                    str   = prev2;
                    count = 0;
                }
            }
            else
            {
                uchar32 c = *str;
                while (c != utf16::TERMINATOR && (end == NULL || str < end) && count > 0)
                {
                    s32 const l = sequence_sizeof_utf16(c);
                    if ((str + l) <= end)
                    {
                        c = 0;
                        for (s32 i = 0; i < l; i++)
                        {
                            c = c << 16;
                            c = c | str[i];
                        }
                        str += l;
                    }
                    count -= 1;
                }
            }
            return count == 0;
        }

        bool skip(utf32::rune const*& str, utf32::rune const* begin, utf32::rune const* end, s32 count)
        {
            if (count < 0)
            {
                ASSERT(begin != nullptr);
                while ((str > begin) && count < 0)
                {
                    str -= 1;
                    uchar32 c = *str;
                    if (c == utf32::TERMINATOR)
                        break;
                    count += 1;
                }
            }
            else
            {
                uchar32 c = *str;
                while (c != utf32::TERMINATOR && (end == NULL || str < end) && count > 0)
                {
                    str++;
                    count -= 1;
                }
            }
            return count == 0;
        }

        s32 compare(ascii::pcrune stra, ascii::pcrune strb)
        {
            uchar32 ca = *stra;
            uchar32 cb = *strb;
            while (ca != ascii::TERMINATOR && cb != ascii::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (ca == cb)
                return 0;
            if (ca == ascii::TERMINATOR)
                return -1;
            return 1;
        }

        s32 compare(ascii::pcrune stra, utf32::pcrune strb)
        {
            uchar32 ca = *stra;
            uchar32 cb = *strb;
            while (ca != ascii::TERMINATOR && cb != utf32::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (ca == cb)
                return 0;
            if (ca == ascii::TERMINATOR)
                return -1;
            return 1;
        }

        s32 compare(utf32::pcrune stra, utf32::pcrune strb)
        {
            uchar32 ca = *stra;
            uchar32 cb = *strb;
            while (ca != utf32::TERMINATOR && cb != utf32::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (ca == cb)
                return 0;
            if (ca == utf32::TERMINATOR)
                return -1;
            return 1;
        }

        s32 compare(utf32::pcrune stra, ascii::pcrune strb)
        {
            uchar32 ca = *stra;
            uchar32 cb = *strb;
            while (ca != utf32::TERMINATOR && cb != ascii::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (ca == cb)
                return 0;
            if (ca == utf32::TERMINATOR)
                return -1;
            return 1;
        }

        s32 compare(ascii::crunes_t const& cstra, utf32::pcrune strb)
        {
            ascii::pcrune stra = cstra.m_str;
            uchar32       ca   = *stra;
            uchar32       cb   = *strb;
            while (stra < cstra.m_end && cb != utf32::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (stra == cstra.m_end)
                return 0;
            if (ascii::TERMINATOR < cb)
                return -1;
            return 1;
        }

        // read

        uchar32 read(ascii::pcrune& str, ascii::pcrune end)
        {
            uchar32 c = ((uchar32)*str) & 0xFF;
            if (c != utf32::TERMINATOR && (end == NULL || str < end))
            {
                str++;
            }
            return c;
        }

        uchar32 read(utf8::pcrune& str, utf8::pcrune end)
        {
            uchar32 c = *str;
            if (c != utf32::TERMINATOR)
            {
                s32 const l = sequence_sizeof_utf8((uchar8)c);
                if ((str + l) > end)
                    return c;

                switch (l)
                {
                    case 0:
                        c = '?'; // Illegal character in utf8, replace with '?'
                        break;
                    case 1: break;
                    case 2:
                        str++;
                        c = ((c << 6) & 0x7ff) + ((str[0]) & 0x3f);
                        break;
                    case 3:
                        str++;
                        c = ((c << 12) & 0xffff) + (((str[0]) << 6) & 0xfff);
                        ++str;
                        c += (str[0]) & 0x3f;
                        break;
                    case 4:
                        str++;
                        c = ((c << 18) & 0x1fffff) + (((str[0]) << 12) & 0x3ffff);
                        ++str;
                        c += ((str[0]) << 6) & 0xfff;
                        ++str;
                        c += (str[0]) & 0x3f;
                        break;
                }
                str++;
            }
            return c;
        }

        uchar32 read(utf16::pcrune& str, utf16::pcrune end)
        {
            uchar32 out_c;
            uchar16 c = *str;
            if (c == utf16::TERMINATOR || (end != NULL && str >= end))
            {
                out_c = utf32::TERMINATOR;
                return out_c;
            }

            s32 l = 0;
            if (c < 0xd800)
            {
                l = 1;
            }
            else if (c < 0xdc00)
            {
                l = 2;
            }

            if (end == NULL || (str + l) <= end)
            {
                // @TODO: This needs to be fixed since the case where end==nullptr needs to be handled correctly
                out_c = 0;
                for (s32 i = 0; i < l; i++)
                {
                    c     = str[i];
                    out_c = out_c << 16;
                    out_c = out_c | c;
                }
                str += l;
            }
            else
            {
                out_c = '\0';
            }
            return out_c;
        }

        uchar32 read(utf32::pcrune& str, utf32::pcrune end)
        {
            uchar32 c = *str;
            if (c != utf32::TERMINATOR && (end == NULL || str < end))
            {
                str++;
            }
            return c;
        }

        uchar32 read(ascii::prune& str, ascii::prune end)
        {
            ascii::pcrune p = str;
            uchar32       c = read(p, (ascii::pcrune)end);
            str += (p - str);
            return c;
        }

        uchar32 read(utf8::prune& str, utf8::prune end)
        {
            utf8::pcrune p = str;
            uchar32      c = read(p, (utf8::pcrune)end);
            str += (p - str);
            return c;
        }

        uchar32 read(utf16::prune& str, utf16::prune end)
        {
            utf16::pcrune p = str;
            uchar32       c = read(p, (utf16::pcrune)end);
            str += (p - str);
            return c;
        }

        uchar32 read(utf32::prune& str, utf32::prune end)
        {
            utf32::pcrune p = str;
            uchar32       c = read(p, (utf32::pcrune)end);
            str += (p - str);
            return c;
        }

        uchar32 read(ascii::crunes_t const& str, ascii::pcrune& cursor) { return read(cursor, str.m_end); }
        uchar32 read(utf8::crunes_t const& str, utf8::pcrune& cursor) { return read(cursor, str.m_end); }
        uchar32 read(utf16::crunes_t const& str, utf16::pcrune& cursor) { return read(cursor, str.m_end); }
        uchar32 read(utf32::crunes_t const& str, utf32::pcrune& cursor) { return read(cursor, str.m_end); }

        uchar32 read(ascii::runes_t const& str, ascii::prune& cursor) { return read(cursor, str.m_end); }
        uchar32 read(utf8::runes_t const& str, utf8::prune& cursor) { return read(cursor, str.m_end); }
        uchar32 read(utf16::runes_t const& str, utf16::prune& cursor) { return read(cursor, str.m_end); }
        uchar32 read(utf32::runes_t const& str, utf32::prune& cursor) { return read(cursor, str.m_end); }

        uchar32 read(runes_t const& str, runes_t::ptr_t& cursor)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return read(cursor.m_ptr.m_ascii, str.m_runes.m_ascii.m_end);
                case utf8::TYPE: return read(cursor.m_ptr.m_utf8, str.m_runes.m_utf8.m_end);
                case utf16::TYPE: return read(cursor.m_ptr.m_utf16, str.m_runes.m_utf16.m_end);
                case utf32::TYPE: return read(cursor.m_ptr.m_utf32, str.m_runes.m_utf32.m_end);
            }
            return '\0';
        }
        uchar32 read(crunes_t const& str, crunes_t::ptr_t& cursor)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return read(str.m_runes.m_ascii, cursor.m_ptr.m_ascii);
                case utf8::TYPE: return read(str.m_runes.m_utf8, cursor.m_ptr.m_utf8);
                case utf16::TYPE: return read(str.m_runes.m_utf16, cursor.m_ptr.m_utf16);
                case utf32::TYPE: return read(str.m_runes.m_utf32, cursor.m_ptr.m_utf32);
            }
            return '\0';
        }

        // ####### WRITE ########

        bool write(uchar32 c, ascii::prune& cursor, ascii::pcrune end)
        {
            if (c > 0x7f)
            {
                c = '?';
            }
            if (cursor < end)
            {
                cursor[0] = (uchar)c;
                cursor++;
                return true;
            }
            return false;
        }

        bool write(uchar32 cp, utf8::prune& cursor, utf8::pcrune end)
        {
            if (cp < 0x80)
            { // one octet
                if (cursor < end)
                {
                    *(cursor++) = static_cast<uchar8>(cp);
                    return true;
                }
            }
            else if (cp < 0x800)
            { // two octets
                if ((cursor + 1) < end)
                {
                    *(cursor++) = static_cast<uchar8>((cp >> 6) | 0xc0);
                    *(cursor++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            else if (cp < 0x10000)
            { // three octets
                if ((cursor + 2) < end)
                {
                    *(cursor++) = static_cast<uchar8>((cp >> 12) | 0xe0);
                    *(cursor++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    *(cursor++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            else
            { // four octets
                if ((cursor + 3) < end)
                {
                    *(cursor++) = static_cast<uchar8>((cp >> 18) | 0xf0);
                    *(cursor++) = static_cast<uchar8>(((cp >> 12) & 0x3f) | 0x80);
                    *(cursor++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    *(cursor++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            return false;
        }

        bool write(uchar32 rune, utf16::prune& cursor, utf16::pcrune end)
        {
            s32 len = 0;
            if (rune < 0xd800)
            {
                len = 1;
            }
            else if (rune < 0xe000)
            {
                len = 0;
            }
            else if (rune < 0x010000)
            {
                len = 1;
            }
            else if (rune < 0x110000)
            {
                len = 2;
            }

            if (cursor != NULL && len > 0)
            {
                if (len == 1 && cursor < end)
                {
                    *cursor++ = (uchar16)rune;
                    return true;
                }
                else if ((cursor + 1) < end)
                {
                    // 20-bit intermediate value
                    u32 const iv = rune - 0x10000;
                    *cursor++    = static_cast<uchar16>((iv >> 10) + 0xd800);
                    *cursor++    = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
                    return true;
                }
            }
            return false;
        }

        bool write(uchar32 c, utf32::prune& cursor, utf32::pcrune end)
        {
            if (cursor < end)
            {
                cursor[0] = c;
                cursor++;
                return true;
            }
            return false;
        }

        bool write(uchar32 c, ascii::runes_t const& dst, ascii::prune& cursor)
        {
            if (can_write(dst, cursor))
            {
                return write(c, cursor, dst.m_eos);
            }
            return false;
        }

        bool write(uchar32 c, utf8::runes_t const& dst, utf8::prune& cursor)
        {
            if (can_write(dst, cursor))
            {
                return write(c, cursor, dst.m_eos);
            }
            return false;
        }

        bool write(uchar32 c, utf16::runes_t const& dst, utf16::prune& cursor)
        {
            if (can_write(dst, cursor))
            {
                return write(c, cursor, dst.m_eos);
            }
            return false;
        }

        bool write(uchar32 c, utf32::runes_t const& dst, utf32::prune& cursor)
        {
            if (can_write(dst, cursor))
            {
                return write(c, cursor, dst.m_eos);
            }
            return false;
        }

        bool terminate_str(ascii::runes_t& dst)
        {
            ascii::prune cursor = dst.m_end;
            if (can_write(dst, cursor))
            {
                uchar32 c = ascii::TERMINATOR;
                return write(c, cursor, dst.m_eos);
            }
            return false;
        }

        bool terminate_str(utf8::runes_t& dst)
        {
            utf8::prune cursor = dst.m_end;
            if (can_write(dst, cursor))
            {
                uchar32 c = utf8::TERMINATOR;
                return write(c, cursor, dst.m_eos);
            }
            return false;
        }

        bool terminate_str(utf16::runes_t& dst)
        {
            utf16::prune cursor = dst.m_end;
            if (can_write(dst, cursor))
            {
                uchar32 c = utf16::TERMINATOR;
                return write(c, cursor, dst.m_eos);
            }
            return false;
        }

        bool terminate_str(utf32::runes_t& dst)
        {
            utf32::prune cursor = dst.m_end;
            if (can_write(dst, cursor))
            {
                uchar32 c = utf32::TERMINATOR;
                return write(c, cursor, dst.m_eos);
            }
            return false;
        }

        // const char* -> ascii / utf16 / utf32
        void copy(char const* sstr, ascii::runes_t& dstr, bool write_terminator)
        {
            ascii::prune cursor = dstr.m_str;
            while (*sstr != '\0')
            {
                uchar32 c = *sstr++;
                if (!write(c, cursor, dstr.m_eos))
                    break;
            }
            if (dstr.m_end < cursor)
                dstr.m_end = cursor;
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(char const* sstr, utf16::runes_t& dstr, bool write_terminator)
        {
            utf16::prune cursor = dstr.m_str;
            while (*sstr != '\0')
            {
                uchar32 c = *sstr++;
                if (!write(c, cursor, dstr.m_eos))
                    break;
            }
            if (dstr.m_end < cursor)
                dstr.m_end = cursor;
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(char const* sstr, utf32::runes_t& dstr, bool write_terminator)
        {
            utf32::prune cursor = dstr.m_str;
            while (*sstr != '\0')
            {
                uchar32 c = *sstr++;
                if (!write(c, cursor, dstr.m_eos))
                    break;
            }
            if (dstr.m_end < cursor)
                dstr.m_end = cursor;
            if (write_terminator)
                terminate_str(dstr);
        }

        // -> ASCII

        void copy(ascii::crunes_t const& sstr, ascii::runes_t& dstr, bool write_terminator)
        {
            ascii::crunes_t src     = sstr;
            ascii::pcrune   scursor = sstr.m_str;
            ascii::prune    dcursor = dstr.m_str;
            while (can_read(src, scursor))
            {
                uchar32 c = read(src, scursor);
                if (!write(c, dcursor, dstr.m_eos))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf16::crunes_t const& sstr, ascii::runes_t& dstr, bool write_terminator)
        {
            utf16::crunes_t src     = sstr;
            utf16::pcrune   scursor = sstr.m_str;
            ascii::prune    dcursor = dstr.m_str;
            while (can_read(src, scursor))
            {
                uchar32 c = read(src, scursor);
                if (!write(c, dcursor, dstr.m_eos))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf32::crunes_t const& sstr, ascii::runes_t& dstr, bool write_terminator)
        {
            utf32::crunes_t src     = sstr;
            utf32::pcrune   scursor = sstr.m_str;
            ascii::prune    dcursor = dstr.m_str;
            while (can_read(src, scursor))
            {
                uchar32 c = read(src, scursor);
                if (!write(c, dcursor, dstr.m_eos))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        // -> UTF-16

        void copy(ascii::crunes_t const& sstr, utf16::runes_t& dstr, bool write_terminator)
        {
            ascii::crunes_t src     = sstr;
            ascii::pcrune   scursor = sstr.m_str;
            utf16::prune    dcursor = dstr.m_str;
            while (can_read(src, scursor))
            {
                uchar32 c = read(src, scursor);
                if (!write(c, dcursor, dstr.m_eos))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf16::crunes_t const& sstr, utf16::runes_t& dstr, bool write_terminator)
        {
            utf16::crunes_t src     = sstr;
            utf16::pcrune   scursor = sstr.m_str;
            utf16::prune    dcursor = dstr.m_str;
            while (can_read(src, scursor))
            {
                uchar32 c = read(src, scursor);
                if (!write(c, dcursor, dstr.m_eos))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf32::crunes_t const& sstr, utf16::runes_t& dstr, bool write_terminator)
        {
            utf32::crunes_t src     = sstr;
            utf32::pcrune   scursor = sstr.m_str;
            utf16::prune    dcursor = dstr.m_str;
            while (can_read(src, scursor))
            {
                uchar32 c = read(src, scursor);
                if (!write(c, dcursor, dstr.m_eos))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        // -> UTF-32

        void copy(ascii::crunes_t const& sstr, utf32::runes_t& dstr, bool write_terminator)
        {
            ascii::crunes_t src     = sstr;
            ascii::pcrune   scursor = sstr.m_str;
            utf32::prune    dcursor = dstr.m_str;
            while (can_read(src, scursor))
            {
                uchar32 c = read(src, scursor);
                if (!write(c, dcursor, dstr.m_eos))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf16::crunes_t const& sstr, utf32::runes_t& dstr, bool write_terminator)
        {
            utf16::crunes_t src     = sstr;
            utf16::pcrune   scursor = sstr.m_str;
            utf32::prune    dcursor = dstr.m_str;
            while (can_read(src, scursor))
            {
                uchar32 c = read(src, scursor);
                if (!write(c, dcursor, dstr.m_eos))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf32::crunes_t const& sstr, utf32::runes_t& dstr, bool write_terminator)
        {
            utf32::crunes_t src     = sstr;
            utf32::pcrune   scursor = sstr.m_str;
            utf32::prune    dcursor = dstr.m_str;
            while (can_read(src, scursor))
            {
                uchar32 c = read(src, scursor);
                if (!write(c, dcursor, dstr.m_eos))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        static inline s32 rune_size(uchar32 c)
        {
            s32 len = 0;
            if (c <= 0x7f)
            {
                len = 1;
            }
            else if (c < 0x0800)
            {
                len = 2;
            }
            else if (c < 0xd800)
            {
                len = 3;
            }
            else if (c < 0xe000)
            {
            }
            else if (c < 0x010000)
            {
                len = 3;
            }
            else if (c < 0x110000)
            {
                len = 4;
            }

            return len;
        }

        bool is_eos(uchar const* str) { return str[0] == '\0'; }
        bool is_eos(uchar8 const* str) { return str[0] == '\0'; }
        bool is_eos(uchar16 const* str) { return str[0] == '\0'; }
        bool is_eos(uchar32 const* str) { return str[0] == '\0'; }

        bool can_read(ascii::runes_t const& str, ascii::pcrune cursor) { return cursor >= str.m_str && cursor < str.m_end; }
        bool can_read(utf8::runes_t const& str, utf8::pcrune cursor) { return cursor >= str.m_str && cursor < str.m_end; }
        bool can_read(utf16::runes_t const& str, utf16::pcrune cursor) { return cursor >= str.m_str && cursor < str.m_end; }
        bool can_read(utf32::runes_t const& str, utf32::pcrune cursor) { return cursor >= str.m_str && cursor < str.m_end; }
        bool can_read(ascii::crunes_t const& str, ascii::pcrune cursor) { return cursor >= str.m_str && cursor < str.m_end; }
        bool can_read(utf8::crunes_t const& str, utf8::pcrune cursor) { return cursor >= str.m_str && cursor < str.m_end; }
        bool can_read(utf16::crunes_t const& str, utf16::pcrune cursor) { return cursor >= str.m_str && cursor < str.m_end; }
        bool can_read(utf32::crunes_t const& str, utf32::pcrune cursor) { return cursor >= str.m_str && cursor < str.m_end; }

        bool can_write(ascii::runes_t const& str, ascii::prune cursor)
        {
            ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
            return cursor >= str.m_str && cursor < str.m_eos;
        }

        bool can_write(utf8::runes_t const& str, utf8::prune cursor)
        {
            ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
            return cursor >= str.m_str && cursor < str.m_eos;
        }

        bool can_write(utf16::runes_t const& str, utf16::prune cursor)
        {
            ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
            return cursor >= str.m_str && cursor < str.m_eos;
        }

        bool can_write(utf32::runes_t const& str, utf32::prune cursor)
        {
            ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
            return cursor >= str.m_str && cursor < str.m_eos;
        }

        bool read_is_crln(ascii::runes_t const& str, ascii::prune cursor)
        {
            ASSERT(str.m_end != nullptr);
            return (cursor + 1) < str.m_end && (cursor[0] == '\r' && cursor[1] == '\n');
        }

        bool read_is_crln(utf8::runes_t const& str, utf8::prune cursor)
        {
            ASSERT(str.m_end != nullptr);
            return (cursor + 1) < str.m_end && (cursor[0] == '\r' && cursor[1] == '\n');
        }

        bool read_is_crln(utf16::runes_t const& str, utf16::prune cursor)
        {
            ASSERT(str.m_end != nullptr);
            return (cursor + 1) < str.m_end && (cursor[0] == '\r' && cursor[1] == '\n');
        }

        bool read_is_crln(utf32::runes_t const& str, utf32::prune cursor)
        {
            ASSERT(str.m_end != nullptr);
            return (cursor + 1) < str.m_end && (cursor[0] == '\r' && cursor[1] == '\n');
        }

        bool read_is_crln(ascii::crunes_t const& str, ascii::pcrune cursor)
        {
            ASSERT(str.m_end != nullptr);
            return (cursor + 1) < str.m_end && (cursor[0] == '\r' && cursor[1] == '\n');
        }

        bool read_is_crln(utf8::crunes_t const& str, utf8::pcrune cursor)
        {
            ASSERT(str.m_end != nullptr);
            return (cursor + 1) < str.m_end && (cursor[0] == '\r' && cursor[1] == '\n');
        }

        bool read_is_crln(utf16::crunes_t const& str, utf16::pcrune cursor)
        {
            ASSERT(str.m_end != nullptr);
            return (cursor + 1) < str.m_end && (cursor[0] == '\r' && cursor[1] == '\n');
        }

        bool read_is_crln(utf32::crunes_t const& str, utf32::pcrune cursor)
        {
            ASSERT(str.m_end != nullptr);
            return (cursor + 1) < str.m_end && (cursor[0] == '\r' && cursor[1] == '\n');
        }

    } // namespace utf

    namespace ascii
    {
        s32 strlen(pcrune str, pcrune eos)
        {
            ascii::pcrune end = str;
            if (eos == nullptr)
            {
                while (*end != ascii::TERMINATOR)
                    end++;
            }
            else
            {
                while (*end != ascii::TERMINATOR && end < eos)
                    end++;
            }
            return (s32)(end - str);
        }
    } // namespace ascii

    typedef runes_t::ptr_t  rptr_t;
    typedef crunes_t::ptr_t cptr_t;

    // runes_t::ptr_t functions
    static rptr_t  get_begin(runes_t const& str);
    static rptr_t  get_end(runes_t const& str);
    static rptr_t  get_eos(runes_t const& str);
    static runes_t select(runes_t const& str, rptr_t const& from, rptr_t const& to);
    static bool    forwards(runes_t const& str, rptr_t& cursor, s32 step = 1);
    static bool    backwards(runes_t const& str, rptr_t& cursor, s32 step = 1);
    static uchar32 peek(runes_t const& str, rptr_t const& cursor);
    static uchar32 read(runes_t const& str, rptr_t& cursor);
    static bool    write(runes_t const& str, rptr_t& cursor, uchar32 c);
    static bool    is_valid(runes_t const& str, rptr_t const& cursor);

    // crunes_t::ptr_t functions
    static cptr_t   get_begin(crunes_t const& str);
    static cptr_t   get_end(crunes_t const& str);
    static cptr_t   get_eos(crunes_t const& str);
    static crunes_t select(crunes_t const& str, cptr_t const& from, cptr_t const& to);
    static crunes_t select(crunes_t const& str, cptr_t const& from, u32 len);
    static bool     forwards(crunes_t const& str, cptr_t& cursor, s32 step = 1);
    static bool     backwards(crunes_t const& str, cptr_t& cursor, s32 step = 1);
    static uchar32  peek(crunes_t const& str, cptr_t const& cursor);
    static uchar32  read(crunes_t const& str, cptr_t& cursor);
    static bool     write(crunes_t const& str, cptr_t& cursor, uchar32 c);
    static bool     is_valid(crunes_t const& str, cptr_t const& cursor);

    static runes_t crunes_to_runes(runes_t const& str, crunes_t const& sel)
    {
        runes_t r(str);
        r.m_runes.m_ascii.m_str += (sel.m_runes.m_ascii.m_str - str.m_runes.m_ascii.m_str);
        r.m_runes.m_ascii.m_end += (sel.m_runes.m_ascii.m_end - str.m_runes.m_ascii.m_end);
        return r;
    }

    static inline runes_t nothing_found(runes_t const& str)
    {
        return runes_t(str.m_runes.m_ascii.m_str, str.m_runes.m_ascii.m_str, str.m_runes.m_ascii.m_str, str.m_type);
    }
    static inline crunes_t nothing_found(crunes_t const& str)
    {
        return crunes_t(str.m_runes.m_ascii.m_str, str.m_runes.m_ascii.m_str, str.m_runes.m_ascii.m_str, str.m_type);
    }

    crunes_t find(crunes_t const& _str, uchar32 _c, bool _casesensitive)
    {
        cptr_t iter = get_begin(_str);
        cptr_t end  = get_end(_str);
        while (iter < end)
        {
            uchar32    c     = peek(_str, iter);
            bool const equal = _casesensitive ? is_equal(c, _c) : is_equalfold(c, _c);
            if (equal)
            {
                cptr_t begin = iter;
                forwards(_str, iter);
                return select(_str, begin, iter);
            }
            forwards(_str, iter);
        }
        return nothing_found(_str);
    }

    runes_t find(runes_t const& _str, uchar32 _c, bool _casesensitive)
    {
        crunes_t str(_str);
        crunes_t csel = find(str, _c, _casesensitive);
        return crunes_to_runes(_str, csel);
    }

    crunes_t findLast(crunes_t const& _str, uchar32 _c, bool _casesensitive)
    {
        cptr_t iter  = get_end(_str);
        cptr_t begin = get_begin(_str);
        while (iter > begin)
        {
            cptr_t end = iter;
            backwards(_str, iter);

            uchar32    c     = peek(_str, iter);
            bool const equal = _casesensitive ? is_equal(c, _c) : is_equalfold(c, _c);
            if (equal)
            {
                return select(_str, iter, end);
            }
        }
        return nothing_found(_str);
    }

    runes_t findLast(runes_t const& _str, uchar32 _c, bool _casesensitive)
    {
        crunes_t str(_str);
        crunes_t csel = findLast(str, _c, _casesensitive);
        return crunes_to_runes(_str, csel);
    }

    bool contains(runes_t const& _str, uchar32 _c, bool _casesensitive = true)
    {
        runes_t pos = find(_str, _c, _casesensitive);
        return !pos.is_empty();
    }

    bool contains(crunes_t const& _str, uchar32 _c, bool _casesensitive = true)
    {
        crunes_t pos = find(_str, _c, _casesensitive);
        return !pos.is_empty();
    }

    crunes_t find(crunes_t const& _str, crunes_t const& _find, bool _casesensitive)
    {
        if (_str.is_empty())
            return nothing_found(_str);
        if (_find.is_empty())
            return nothing_found(_str);
        if (_find.size() > _str.size())
            return nothing_found(_str);

        cptr_t iterb = get_begin(_str);
        cptr_t itere = iterb;
        forwards(_str, itere, _find.size());
        cptr_t end = get_end(_str);
        while (itere <= end)
        {
            crunes_t  sel = select(_str, iterb, itere);
            s32 const cmp = compare(sel, _find, _casesensitive);
            if (cmp == 0)
            {
                return sel;
            }
            forwards(_str, iterb, 1);
            if (!forwards(_str, itere, 1))
                break;
        }
        return nothing_found(_str);
    }

    runes_t find(runes_t const& _str, crunes_t const& _find, bool _casesensitive)
    {
        crunes_t str(_str);
        crunes_t csel = find(str, _find, _casesensitive);
        if (!csel.is_empty())
        {
            runes_t sel = crunes_to_runes(_str, csel);
            return sel;
        }
        return nothing_found(_str);
    }

    crunes_t findLast(crunes_t const& _str, crunes_t const& _find, bool _casesensitive)
    {
        if (_str.is_empty())
            return nothing_found(_str);
        if (_find.is_empty())
            return nothing_found(_str);
        if (_find.size() > _str.size())
            return nothing_found(_str);

        cptr_t begin = get_begin(_str);
        cptr_t itere = get_end(_str);
        cptr_t iterb = itere;
        backwards(_str, iterb, _find.size());
        while (iterb >= begin)
        {
            crunes_t  sel = select(_str, iterb, itere);
            s32 const cmp = compare(sel, _find, _casesensitive);
            if (cmp == 0)
            {
                return sel;
            }
            if (!backwards(_str, iterb, 1))
                break;
            backwards(_str, itere, 1);
        }
        return nothing_found(_str);
    }

    runes_t findLast(runes_t const& _str, crunes_t const& _find, bool _casesensitive)
    {
        crunes_t str(_str);
        crunes_t csel = findLast(str, _find, _casesensitive);
        if (!csel.is_empty())
        {
            runes_t sel = crunes_to_runes(_str, csel);
            return sel;
        }
        return nothing_found(_str);
    }

    crunes_t findOneOf(crunes_t const& _str, crunes_t const& _any, bool _casesensitive)
    {
        cptr_t iter = get_begin(_str);
        cptr_t end  = get_end(_str);
        while (iter < end)
        {
            cptr_t  pos = iter;
            uchar32 c   = read(_str, iter);

            // See if this char can be found in the 'set'
            crunes_t found = find(_any, c, _casesensitive);
            if (!found.is_empty())
                return select(_str, pos, iter);
        }
        return nothing_found(_str);
    }

    runes_t findOneOf(runes_t const& _str, crunes_t const& _any, bool _casesensitive)
    {
        crunes_t str(_str);
        crunes_t csel = findOneOf(str, _any, _casesensitive);
        if (!csel.is_empty())
        {
            runes_t sel = crunes_to_runes(_str, csel);
            return sel;
        }
        return nothing_found(_str);
    }

    // ------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------
    crunes_t findSelectUntil(const crunes_t& _str, uchar32 _c, bool _casesensitive)
    {
        crunez_t<utf32::rune, 4> _find(_c);
        return findSelectUntil(_str, _find, _casesensitive);
    }

    crunes_t findSelectUntil(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = find(_str, _find, _casesensitive);
        if (sel.is_empty())
            return nothing_found(_str);
        return crunes_t(_str.m_runes.m_ascii.m_bos, _str.m_runes.m_ascii.m_str, sel.m_runes.m_ascii.m_str,
                        _str.m_runes.m_ascii.m_eos, _str.m_type);
    }

    crunes_t findLastSelectUntil(const crunes_t& _str, uchar32 _c, bool _casesensitive)
    {
        crunez_t<utf32::rune, 4> _find(_c);
        return findLastSelectUntil(_str, _find, _casesensitive);
    }

    crunes_t findLastSelectUntil(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = findLast(_str, _find, _casesensitive);
        if (sel.is_empty())
            return nothing_found(_str);
        return crunes_t(_str.m_runes.m_ascii.m_bos, _str.m_runes.m_ascii.m_str, sel.m_runes.m_ascii.m_str,
                        _str.m_runes.m_ascii.m_eos, _str.m_type);
    }

    crunes_t findSelectUntilIncluded(const crunes_t& _str, uchar32 _c, bool _casesensitive)
    {
        crunez_t<utf32::rune, 4> _find(_c);
        return findSelectUntilIncluded(_str, _find, _casesensitive);
    }

    crunes_t findSelectUntilIncluded(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = find(_str, _find, _casesensitive);
        if (sel.is_empty())
            return nothing_found(_str);
        return crunes_t(_str.m_runes.m_ascii.m_bos, _str.m_runes.m_ascii.m_str, sel.m_runes.m_ascii.m_end,
                        _str.m_runes.m_ascii.m_eos, _str.m_type);
    }

    crunes_t findLastSelectUntilIncluded(const crunes_t& _str, uchar32 _c, bool _casesensitive)
    {
        crunez_t<utf32::rune, 4> _find(_c);
        return findLastSelectUntilIncluded(_str, _find, _casesensitive);
    }

    crunes_t findLastSelectUntilIncluded(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = findLast(_str, _find, _casesensitive);
        if (sel.is_empty())
            return nothing_found(_str);
        return crunes_t(_str.m_runes.m_ascii.m_bos, _str.m_runes.m_ascii.m_str, sel.m_runes.m_ascii.m_end,
                        _str.m_runes.m_ascii.m_eos, _str.m_type);
    }

    crunes_t selectAfterSelection(const crunes_t& _str, const crunes_t& _sel)
    {
        if (!_sel.is_empty())
        {
            crunes_t sel;
            sel.m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
            sel.m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
            sel.m_runes.m_ascii.m_str = _sel.m_runes.m_ascii.m_end;
            sel.m_runes.m_ascii.m_end = _str.m_runes.m_ascii.m_end;
            return sel;
        }
        return nothing_found(_str);
    }

    crunes_t findSelectAfter(const crunes_t& _str, uchar32 _find, bool _casesensitive)
    {
        crunes_t sel = findSelectUntilIncluded(_str, _find, _casesensitive);
        return selectAfterSelection(_str, sel);
    }

    crunes_t findSelectAfter(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = findSelectUntilIncluded(_str, _find, _casesensitive);
        return selectAfterSelection(_str, sel);
    }

    crunes_t findLastSelectAfter(const crunes_t& _str, uchar32 _find, bool _casesensitive)
    {
        crunes_t sel = findLastSelectUntilIncluded(_str, _find, _casesensitive);
        return selectAfterSelection(_str, sel);
    }

    crunes_t findLastSelectAfter(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = findLastSelectUntilIncluded(_str, _find, _casesensitive);
        return selectAfterSelection(_str, sel);
    }

    crunes_t selectBetween(const crunes_t& _str, uchar32 _left, uchar32 _right)
    {
        crunes_t l = find(_str, _right);
        if (!l.is_empty())
        {
            crunes_t after = selectAfterExclude(_str, l);
            crunes_t r     = findLast(after, _left);
            if (!l.is_empty())
            {
                return crunes_t(_str.m_runes.m_ascii.m_bos, l.m_runes.m_ascii.m_end, r.m_runes.m_ascii.m_str,
                                _str.m_runes.m_ascii.m_eos, _str.m_type);
            }
        }
        return nothing_found(_str);
    }

    crunes_t selectNextBetween(const crunes_t& _str, const crunes_t& _selection, uchar32 _left, uchar32 _right)
    {
        crunes_t str;
        str.m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
        str.m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
        str.m_runes.m_ascii.m_end = _str.m_runes.m_ascii.m_end;
        str.m_runes.m_ascii.m_str = _selection.m_runes.m_ascii.m_end;
        return selectBetween(str, _left, _right);
    }

    crunes_t selectBetweenLast(const crunes_t& _str, uchar32 _left, uchar32 _right)
    {
        crunes_t r = findLast(_str, _right);
        if (!r.is_empty())
        {
            crunes_t before = selectBeforeExclude(_str, r);
            crunes_t l      = findLast(before, _left);
            if (!l.is_empty())
            {
                return crunes_t(_str.m_runes.m_ascii.m_bos, l.m_runes.m_ascii.m_end, r.m_runes.m_ascii.m_str,
                                _str.m_runes.m_ascii.m_eos, _str.m_type);
            }
        }
        return nothing_found(_str);
    }

    crunes_t selectPreviousBetween(const crunes_t& _str, const crunes_t& _selection, uchar32 _left, uchar32 _right)
    {
        crunes_t str;
        str.m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
        str.m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
        str.m_runes.m_ascii.m_str = _str.m_runes.m_ascii.m_str;
        str.m_runes.m_ascii.m_end = _selection.m_runes.m_ascii.m_str;
        str.m_type                = _str.m_type;
        return selectBetweenLast(str, _left, _right);
    }

    crunes_t selectBeforeExclude(const crunes_t& _str, const crunes_t& _selection)
    {
        crunes_t str;
        str.m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
        str.m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
        str.m_runes.m_ascii.m_str = _str.m_runes.m_ascii.m_str;
        str.m_runes.m_ascii.m_end = _selection.m_runes.m_ascii.m_str;
        str.m_type                = _str.m_type;
        return str;
    }

    crunes_t selectBeforeInclude(const crunes_t& _str, const crunes_t& _selection)
    {
        crunes_t str;
        str.m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
        str.m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
        str.m_runes.m_ascii.m_str = _str.m_runes.m_ascii.m_str;
        str.m_runes.m_ascii.m_end = _selection.m_runes.m_ascii.m_end;
        str.m_type                = _str.m_type;
        return str;
    }

    crunes_t selectAfterExclude(const crunes_t& _str, const crunes_t& _selection)
    {
        crunes_t sel;
        sel.m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
        sel.m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
        sel.m_runes.m_ascii.m_str = _selection.m_runes.m_ascii.m_end;
        sel.m_runes.m_ascii.m_end = _str.m_runes.m_ascii.m_end;
        sel.m_type                = _str.m_type;
        return sel;
    }

    crunes_t selectAfterInclude(const crunes_t& _str, const crunes_t& _selection)
    {
        crunes_t sel;
        sel.m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
        sel.m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
        sel.m_runes.m_ascii.m_str = _selection.m_runes.m_ascii.m_str;
        sel.m_runes.m_ascii.m_end = _str.m_runes.m_ascii.m_end;
        sel.m_type                = _str.m_type;
        return sel;
    }

    crunes_t selectOverlap(const crunes_t& lstr, const crunes_t& rstr)
    {
        // Find the last character of 'lstr' in 'rstr'
        // When found start to expand the string in 'lstr' to the left
        // while keep checking if 'rstr' also contains that character

        ASSERT(false); // TODO

        return lstr;
    }

    s32 compare(crunes_t const& _lstr, crunes_t const& _rstr, bool _casesensitive)
    {
        if (_lstr.size() < _rstr.size())
            return -1;
        if (_lstr.size() > _rstr.size())
            return 1;
        if (_lstr.size() == 0)
            return 0;

        cptr_t  lcursor = get_begin(_lstr);
        cptr_t  rcursor = get_begin(_rstr);
        cptr_t  lend    = get_end(_lstr);
        cptr_t  rend    = get_end(_rstr);
        uchar32 lc, rc;
        if (_casesensitive)
        {
            do
            {
                lc = read(_lstr, lcursor);
                rc = read(_rstr, rcursor);
            } while (lc == rc && lcursor < lend && rcursor < rend);
        }
        else
        {
            do
            {
                lc = read(_lstr, lcursor);
                rc = read(_rstr, rcursor);
                lc = to_lower(lc);
                rc = to_lower(rc);
            } while (lc == rc && lcursor < lend && rcursor < rend);
        }
        if (lc < rc)
            return -1;
        else if (lc > rc)
            return 1;
        return 0;
    }

    // ------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------

    runes_t findSelectUntil(const runes_t& _str, uchar32 _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findSelectUntil(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findSelectUntil(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findSelectUntil(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findLastSelectUntil(const runes_t& _str, uchar32 _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findLastSelectUntil(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findLastSelectUntil(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findLastSelectUntil(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findSelectUntilIncluded(const runes_t& _str, uchar32 _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findSelectUntilIncluded(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findSelectUntilIncluded(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findSelectUntilIncluded(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findLastSelectUntilIncluded(const runes_t& _str, uchar32 _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findLastSelectUntilIncluded(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findLastSelectUntilIncluded(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findLastSelectUntilIncluded(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t selectAfterSelection(const runes_t& _str, const runes_t& _sel)
    {
        crunes_t cstr(_str);
        crunes_t found = selectAfterSelection(cstr, _sel);
        return crunes_to_runes(_str, found);
    }

    runes_t findSelectAfter(const runes_t& _str, uchar32 _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findSelectAfter(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findSelectAfter(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findSelectAfter(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findLastSelectAfter(const runes_t& _str, uchar32 _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findLastSelectAfter(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t findLastSelectAfter(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t cstr(_str);
        crunes_t found = findLastSelectAfter(cstr, _find, _casesensitive);
        return crunes_to_runes(_str, found);
    }

    runes_t selectBetween(const runes_t& _str, uchar32 _left, uchar32 _right)
    {
        crunes_t cstr(_str);
        crunes_t found = selectBetween(cstr, _left, _right);
        return crunes_to_runes(_str, found);
    }

    runes_t selectNextBetween(const runes_t& _str, const runes_t& _selection, uchar32 _left, uchar32 _right)
    {
        crunes_t cstr(_str);
        crunes_t found = selectNextBetween(cstr, _selection, _left, _right);
        return crunes_to_runes(_str, found);
    }

    runes_t selectBetweenLast(const runes_t& _str, uchar32 _left, uchar32 _right)
    {
        crunes_t cstr(_str);
        crunes_t found = selectBetweenLast(cstr, _left, _right);
        return crunes_to_runes(_str, found);
    }

    runes_t selectPreviousBetween(const runes_t& _str, const runes_t& _selection, uchar32 _left, uchar32 _right)
    {
        crunes_t cstr(_str);
        crunes_t found = selectPreviousBetween(cstr, _selection, _left, _right);
        return crunes_to_runes(_str, found);
    }

    runes_t selectBeforeExclude(const runes_t& _str, const runes_t& _selection)
    {
        crunes_t cstr(_str);
        crunes_t found = selectBeforeExclude(cstr, _selection);
        return crunes_to_runes(_str, found);
    }

    runes_t selectBeforeInclude(const runes_t& _str, const runes_t& _selection)
    {
        crunes_t cstr(_str);
        crunes_t found = selectBeforeInclude(cstr, _selection);
        return crunes_to_runes(_str, found);
    }

    runes_t selectAfterExclude(const runes_t& _str, const runes_t& _selection)
    {
        crunes_t cstr(_str);
        crunes_t found = selectAfterExclude(cstr, _selection);
        return crunes_to_runes(_str, found);
    }

    runes_t selectAfterInclude(const runes_t& _str, const runes_t& _selection)
    {
        crunes_t cstr(_str);
        crunes_t found = selectAfterInclude(cstr, _selection);
        return crunes_to_runes(_str, found);
    }

    runes_t selectOverlap(const runes_t& _str, const runes_t& _rstr)
    {
        crunes_t cstr(_str);
        crunes_t found = selectOverlap(cstr, _rstr);
        return crunes_to_runes(_str, found);
    }

    s32 compare(runes_t const& _lstr, runes_t const& _rstr, bool _casesensitive)
    {
        crunes_t clstr(_lstr);
        crunes_t crstr(_rstr);
        return compare(clstr, crstr, _casesensitive);
    }

    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------

    crunes_t parse(crunes_t const& _str, bool& value)
    {
        crunes_t    str          = _str;
        ascii::rune format_str[] = {'%', 'b', ascii::TERMINATOR};
        crunes_t    format(format_str, format_str + 2);
        sscanf(str, format, va_r_t(&value));
        return str;
    }

    crunes_t parse(crunes_t const& _str, s8& value, s32 base)
    {
        s64      value64;
        crunes_t adv = parse(_str, value64, base);
        value        = (s8)value64;
        return adv;
    }

    crunes_t parse(crunes_t const& _str, s16& value, s32 base)
    {
        s64      value64;
        crunes_t adv = parse(_str, value64, base);
        value        = (s16)value64;
        return adv;
    }

    crunes_t parse(crunes_t const& _str, s32& value, s32 base)
    {
        s64      value64;
        crunes_t adv = parse(_str, value64, base);
        value        = (s32)value64;
        return adv;
    }

    crunes_t parse(crunes_t const& _str, u8& value, s32 base)
    {
        u64      value64;
        crunes_t adv = parse(_str, value64, base);
        value        = (u8)value64;
        return adv;
    }

    crunes_t parse(crunes_t const& _str, u16& value, s32 base)
    {
        u64      value64;
        crunes_t adv = parse(_str, value64, base);
        value        = (u16)value64;
        return adv;
    }

    crunes_t parse(crunes_t const& _str, u32& value, s32 base)
    {
        u64      value64;
        crunes_t adv = parse(_str, value64, base);
        value        = (u16)value64;
        return adv;
    }

    crunes_t parse(crunes_t const& _str, s64& value, s32 base)
    {
        crunes_t    str          = _str;
        ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
        switch (base)
        {
            case 16: format_str[1] = 'x'; break;
            case 10: format_str[1] = 'd'; break;
            case 8: format_str[1] = 'o'; break;
        };
        crunes_t format(format_str, format_str + 2);
        sscanf(str, format, va_r_t(&value));
        return str;
    }

    crunes_t parse(crunes_t const& _str, u64& value, s32 base)
    {
        crunes_t    str          = _str;
        ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
        switch (base)
        {
            case 16: format_str[1] = 'x'; break;
            case 10: format_str[1] = 'd'; break;
            case 8: format_str[1] = 'o'; break;
        };
        crunes_t format(format_str, format_str + 2);
        sscanf(str, format, va_r_t(&value));
        return str;
    }

    crunes_t parse(crunes_t const& _str, f32& value)
    {
        crunes_t    str          = _str;
        ascii::rune format_str[] = {'%', 'f', ascii::TERMINATOR};
        crunes_t    format(format_str, format_str + 2);
        sscanf(str, format, va_r_t(&value));
        return str;
    }

    crunes_t parse(crunes_t const& _str, f64& value)
    {
        crunes_t    str          = _str;
        ascii::rune format_str[] = {'%', 'f', ascii::TERMINATOR};
        crunes_t    format(format_str, format_str + 2);
        sscanf(str, format, va_r_t(&value));
        return str;
    }

    //------------------------------------------------------------------------------
    bool is_decimal(crunes_t const& _str)
    {
        cptr_t iter = get_begin(_str);
        cptr_t end  = get_end(_str);
        while (iter < end)
        {
            uchar32 c = peek(_str, iter);
            if (c != '-')
                break;
            read(_str, iter);
        }
        while (iter < end)
        {
            uchar32 c = read(_str, iter);
            if (c < '0' || c > '9')
                return false;
        }
        return true;
    }

    //------------------------------------------------------------------------------
    bool is_hexadecimal(crunes_t const& _str, bool with_prefix)
    {
        cptr_t iter = get_begin(_str);
        cptr_t end  = get_end(_str);

        uchar32 p;
        s32     pos = 0;
        while (iter < end)
        {
            uchar32 c = read(_str, iter);
            if (c == 'x')
            {
                if (pos == 1 && with_prefix && p == '0')
                {
                    // Ok, 0x.... prefix
                }
                else
                {
                    return false;
                }
            }
            else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                // Ok, digit or hex character
                p = c;
            }
            else
            {
                return false;
            }
            pos++;
        }
        return true;
    }

    //------------------------------------------------------------------------------
    bool is_float(crunes_t const& _str)
    {
        ascii::rune f32chars_str[] = {'E', 'e', '.', '#', 'Q', 'N', 'A', 'B', 'I', 'F', 0};
        crunes_t    f32chars(f32chars_str, &f32chars_str[10]);

        cptr_t iter = get_begin(_str);
        cptr_t end  = get_end(_str);
        while (iter < end)
        {
            uchar32 c = read(_str, iter);
            if (c >= '0' && c <= '9')
                continue;
            if (!contains(f32chars, c))
                return false;
        }
        return true;
    }

    //------------------------------------------------------------------------------
    bool is_GUID(crunes_t const& _str)
    {
        cptr_t iter = get_begin(_str);
        cptr_t end  = get_end(_str);

        // Does it have only 0-9, a-f, A-F characters
        s32 n = 0;
        s32 l = 0;
        while (iter < end)
        {
            uchar32 c = read(_str, iter);
            if (c >= '0' && c <= '9')
            {
                n++;
                continue;
            }
            if (c >= 'A' && c <= 'F')
            {
                n++;
                continue;
            }
            if (c >= 'a' && c <= 'f')
            {
                n++;
                continue;
            }
            if (c == ':')
            {
                l++;
                if (l > 3)
                    return false;
                continue;
            }
        }
        // the length of the string must be 17 characters long (4*8 + 3 ':')
        return (n == (4 * 8) && l == 3);
    }

    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------

    void to_string(runes_t& str, s32 val, s32 base)
    {
        ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
        switch (base)
        {
            case 16: format_str[1] = 'x'; break;
            case 10: format_str[1] = 'd'; break;
            case 8: format_str[1] = 'o'; break;
        };
        crunes_t format(format_str, format_str + 2);
        sprintf(str, format, va_t(val));
    }

    void to_string(runes_t& str, u32 val, s32 base)
    {
        ascii::rune format_str[] = {'%', 'u', ascii::TERMINATOR};
        switch (base)
        {
            case 16: format_str[1] = 'x'; break;
            case 10: format_str[1] = 'u'; break;
            case 8: format_str[1] = 'o'; break;
        };
        crunes_t format(format_str, format_str + 2);
        sprintf(str, format, va_t(val));
    }

    // *cFHbRXV[N]WWWR6FETY+

    void to_string(runes_t& str, s64 val, s32 base)
    {
        ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
        switch (base)
        {
            case 16: format_str[1] = 'x'; break;
            case 10: format_str[1] = 'd'; break;
            case 8: format_str[1] = 'o'; break;
        };
        crunes_t format(format_str, format_str + 2);
        sprintf(str, format, va_t(val));
    }

    void to_string(runes_t& str, u64 val, s32 base)
    {
        ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
        switch (base)
        {
            case 16: format_str[1] = 'x'; break;
            case 10: format_str[1] = 'd'; break;
            case 8: format_str[1] = 'o'; break;
        };
        crunes_t format(format_str, format_str + 2);
        sprintf(str, format, va_t(val));
    }

    void to_string(runes_t& str, f32 val, s32 numFractionalDigits)
    {
        ascii::rune format_str[] = {'%', '.', '0', '2', 'f', ascii::TERMINATOR};
        if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
        {
            format_str[2] = '0' + numFractionalDigits / 10;
            format_str[3] = '0' + numFractionalDigits % 10;
        }
        crunes_t format(format_str, format_str + 5);
        sprintf(str, format, va_t(val));
    }

    void to_string(runes_t& str, f64 val, s32 numFractionalDigits)
    {
        ascii::rune format_str[] = {'%', '.', '0', '2', 'f', ascii::TERMINATOR};
        if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
        {
            format_str[2] = '0' + numFractionalDigits / 10;
            format_str[3] = '0' + numFractionalDigits % 10;
        }
        crunes_t format(format_str, format_str + 5);
        sprintf(str, format, va_t(val));
    }

    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------

    bool is_upper(crunes_t const& _str)
    {
        cptr_t iter = get_begin(_str);
        cptr_t end  = get_end(_str);
        while (iter < end)
        {
            uchar32 c = read(_str, iter);
            if (is_lower(c))
                return false;
        }
        return true;
    }

    bool is_lower(crunes_t const& _str)
    {
        cptr_t iter = get_begin(_str);
        cptr_t end  = get_end(_str);
        while (iter < end)
        {
            uchar32 c = read(_str, iter);
            if (is_upper(c))
                return false;
        }
        return true;
    }

    bool is_capitalized(crunes_t const& _str)
    {
        bool   b    = true;
        cptr_t iter = get_begin(_str);
        cptr_t end  = get_end(_str);
        while (iter < end)
        {
            uchar32 c = read(_str, iter);
            if (is_upper(c) != b)
                return false;
            b = (c == ' ');
        }
        return true;
    }

    bool is_quoted(crunes_t const& str, uchar32 quote) { return is_delimited(str, quote, quote); }

    bool is_delimited(crunes_t const& _str, uchar32 delimit_left, uchar32 delimit_right)
    {
        uchar32 first = first_char(_str);
        if (first == delimit_left)
        {
            uchar32 last = last_char(_str);
            return last == delimit_right;
        }
        return false;
    }

    /**
     * Assumption: Letters A-Z and a-z are contiguous in the character set.
     * This is true for ASCII and UniCode.  (Not so for EBCDIC!)
     */
    void to_upper(runes_t& _str)
    {
        rptr_t iter = get_begin(_str);
        rptr_t end  = get_end(_str);
        while (iter < end)
        {
            uchar32 c = peek(_str, iter);
            if ((c >= 'a') && (c <= 'z'))
            {
                c += ('A' - 'a');
            }
            write(_str, iter, c);
        }
    }

    /**
     * Assumption: Letters A-Z and a-z are contiguous in the character set.
     * This is true for ASCII and UniCode.  (Not so for EBCDIC!)
     */
    void to_lower(runes_t& _str)
    {
        rptr_t iter = get_begin(_str);
        rptr_t end  = get_end(_str);
        while (iter < end)
        {
            uchar32 c = peek(_str, iter);
            if ((c >= 'A') && (c <= 'Z'))
            {
                c += ('a' - 'A');
            }
            write(_str, iter, c);
        }
    }

    // ------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------

    bool starts_with(crunes_t const& _str, uchar32 start_char)
    {
        cptr_t  iter = get_begin(_str);
        uchar32 c    = peek(_str, iter);
        return start_char == c;
    }

    bool starts_with(crunes_t const& _str, crunes_t const& _start)
    {
        if (_start.size() > _str.size())
            return false;

        cptr_t liter = get_begin(_str);
        cptr_t lend  = get_end(_str);
        cptr_t riter = get_begin(_start);
        cptr_t rend  = get_end(_start);
        while (liter < lend && riter < rend)
        {
            uchar32 lc = read(_str, liter);
            uchar32 rc = read(_start, riter);
            if (lc != rc)
                return false;
        }
        return liter <= lend && riter == rend;
    }

    bool ends_with(crunes_t const& _str, uchar32 end_char)
    {
        cptr_t iter = get_end(_str);
        cptr_t last = iter;
        backwards(_str, last, 1);
        if (is_valid(_str, last))
        {
            uchar32 c = read(_str, last);
            return end_char == c;
        }
        return false;
    }

    bool ends_with(crunes_t const& _str, crunes_t const& _end)
    {
        if (_end.size() > _str.size())
            return false;

        cptr_t lend  = get_end(_str);
        cptr_t liter = lend;
        backwards(_str, liter, _end.size());
        cptr_t rend  = get_end(_end);
        cptr_t riter = get_begin(_end);

        while (liter < lend && riter < rend)
        {
            uchar32 lc = read(_str, liter);
            uchar32 rc = read(_end, riter);
            if (lc != rc)
                return false;
        }
        return liter == lend && riter == rend;
    }

    uchar32 first_char(crunes_t const& _str)
    {
        cptr_t  iter = get_begin(_str);
        uchar32 c    = peek(_str, iter);
        return c;
    }

    uchar32 last_char(crunes_t const& _str)
    {
        cptr_t iter = get_end(_str);
        cptr_t last = iter;
        backwards(_str, last, 1);
        uchar32 c = peek(_str, last);
        return c;
    }

    // ------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------

    void removeSelection(runes_t& str, crunes_t const& selection)
    {
        //@note: treated as a mem move
        xbyte* dst = (xbyte*)selection.m_runes.m_ascii.m_str;
        xbyte* src = (xbyte*)selection.m_runes.m_ascii.m_end;
        xbyte* end = (xbyte*)str.m_runes.m_ascii.m_end;
        while (src < end)
        {
            *dst++ = *src++;
        }
        str.m_runes.m_ascii.m_end = (ascii::prune)dst;
    }

    void keepOnlySelection(runes_t& str, crunes_t const& keep)
    {
        //@note: treated as a mem move
        xbyte*       dst = (xbyte*)str.m_runes.m_ascii.m_str;
        xbyte*       src = (xbyte*)keep.m_runes.m_ascii.m_str;
        xbyte const* end = (xbyte const*)keep.m_runes.m_ascii.m_end;
        while (src < end)
        {
            *dst++ = *src++;
        }
        str.m_runes.m_ascii.m_end = (ascii::prune)dst;
    }

    void replaceSelection(runes_t& _str, crunes_t const& _selection, crunes_t const& replace)
    {
        runes_t str = crunes_to_runes(_str, _selection);

        // The logic here is based on memory copy, we do not consider characters
        s32 const selected_len = (s32)((xbyte const*)str.m_runes.m_ascii.m_end - (xbyte const*)str.m_runes.m_ascii.m_str);
        s32 const replace_len =
            (s32)((xbyte const*)replace.m_runes.m_ascii.m_end - (xbyte const*)replace.m_runes.m_ascii.m_str);

        xbyte* end = NULL;
        if (selected_len < replace_len)
        {
            // Move, expanding
            s32 move_len = replace_len - selected_len;
            if (move_len > ((xbyte const*)str.m_runes.m_ascii.m_eos - (xbyte*)str.m_runes.m_ascii.m_end))
                move_len = (s32)(((xbyte const*)str.m_runes.m_ascii.m_eos - (xbyte*)str.m_runes.m_ascii.m_end));

            // No matter what, push out anything at the end!
            xbyte* dst = (xbyte*)((xbyte*)str.m_runes.m_ascii.m_end +
                                  ((xbyte const*)str.m_runes.m_ascii.m_eos - (xbyte*)str.m_runes.m_ascii.m_end) - 1);
            xbyte* src = (xbyte*)((xbyte*)dst - move_len);
            while (dst > (xbyte*)str.m_runes.m_ascii.m_end)
                *dst-- = *src--;

            end = (xbyte*)_str.m_runes.m_ascii.m_end + move_len; // Update str_end
        }
        else if (selected_len > replace_len)
        {
            // Move, shrinking
            s32    move_len = selected_len - replace_len;
            xbyte* dst      = (xbyte*)((xbyte*)str.m_runes.m_ascii.m_end - move_len);
            xbyte* src      = (xbyte*)(str.m_runes.m_ascii.m_end);
            while (src < (xbyte const*)str.m_runes.m_ascii.m_eos)
                *dst++ = *src++;

            end = (xbyte*)_str.m_runes.m_ascii.m_end - move_len; // Update str_end
        }
        else
        {
            end = (xbyte*)_str.m_runes.m_ascii.m_end;
        }

        // Replace
        xbyte const* src     = (xbyte const*)replace.m_runes.m_ascii.m_str;
        xbyte const* src_end = (xbyte const*)replace.m_runes.m_ascii.m_str + replace_len;
        xbyte*       dst     = (xbyte*)str.m_runes.m_ascii.m_str;
        while (src < src_end)
            *dst++ = *src++;

        _str.m_runes.m_ascii.m_end = (ascii::prune)end;
    }

    static inline bool isIn(runes_t const& str, crunes_t const& selection)
    {
        return (selection.m_runes.m_ascii.m_str >= str.m_runes.m_ascii.m_str &&
                selection.m_runes.m_ascii.m_str < str.m_runes.m_ascii.m_end &&
                selection.m_runes.m_ascii.m_end <= str.m_runes.m_ascii.m_end);
    }

    static crunes_t relocate(runes_t const& from, runes_t const& to, crunes_t const& selection)
    {
        if (isIn(from, selection))
        {
            crunes_t nselection(to);
            s32      stroffset               = (s32)(selection.m_runes.m_ascii.m_str - from.m_runes.m_ascii.m_str);
            s32      endoffset               = (s32)(selection.m_runes.m_ascii.m_end - from.m_runes.m_ascii.m_str);
            nselection.m_runes.m_ascii.m_end = nselection.m_runes.m_ascii.m_str + endoffset;
            nselection.m_runes.m_ascii.m_str = nselection.m_runes.m_ascii.m_str + stroffset;
            return nselection;
        }
        return crunes_t();
    }

    void replaceSelection(runes_t& str, crunes_t const& selection, crunes_t const& replace, runes_alloc_t* allocator,
                          s32 size_alignment)
    {
        s64 const diff = replace.size() - selection.size();
        if (diff > 0)
        {
            // Do we need to allocate additional space for @str ?
            // This will invalidate @selection
            if ((str.size() + diff) > str.cap())
            {
                runes_t nstr = allocator->allocate(0, (str.size() + diff + size_alignment) & ~(size_alignment - 1), str.m_type);
                copy(str, nstr);

                // Relocate selection
                crunes_t nselection = relocate(str, nstr, selection);

                // Deallocate original str
                allocator->deallocate(str);
                str = nstr;
            }
        }
        replaceSelection(str, selection, replace);
    }

    void findReplace(runes_t& _str, uchar32 _find, uchar32 _replace, bool _casesensitive)
    {
        runez_t<utf32::rune, 3> find(_find);
        runez_t<utf32::rune, 3> replace(_replace);
        findReplace(_str, find, replace, _casesensitive);
    }

    void findReplace(runes_t& _str, crunes_t const& _find, crunes_t const& _replace, bool _casesensitive)
    {
        runes_t selected = find(_str, _find, _casesensitive);
        if (selected.is_empty())
            return;
        replaceSelection(_str, selected, _replace);
    }

    void insert(runes_t& str, crunes_t const& insert) {}
    void insert(runes_t& str, crunes_t const& insert, runes_alloc_t* allocator, s32 size_alignment) {}
    void insert(runes_t& str, crunes_t const& selection, crunes_t const& insert) {}
    void insert(runes_t& str, crunes_t const& selection, crunes_t const& insert, runes_alloc_t* allocator, s32 size_alignment)
    {
    }

    void trim(runes_t& str)
    {
        utf32::rune charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t    charset(charseta, &charseta[2]);
        trimLeft(str, charseta);
        trimRight(str, charseta);
    }

    void trimLeft(runes_t& str)
    {
        utf32::rune charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t    charset(charseta, &charseta[2]);
        trimLeft(str, charseta);
    }

    void trimRight(runes_t& str)
    {
        utf32::rune charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t    charset(charseta, &charseta[2]);
        trimRight(str, charseta);
    }

    void trim(runes_t& str, uchar32 _c)
    {
        utf32::rune charseta[] = {_c, utf32::TERMINATOR};
        crunes_t    charset(charseta, &charseta[1]);
        trimLeft(str, charseta);
        trimRight(str, charseta);
    }

    void trimLeft(runes_t& str, uchar32 _c)
    {
        utf32::rune charset[] = {_c, utf32::TERMINATOR};
        trimLeft(str, charset);
    }

    void trimRight(runes_t& str, uchar32 _c)
    {
        utf32::rune charset[2] = {_c, utf32::TERMINATOR};
        trimLeft(str, charset);
    }

    void trim(runes_t& str, crunes_t const& _charset)
    {
        trimLeft(str, _charset);
        trimRight(str, _charset);
    }

    void trimLeft(runes_t& _str, crunes_t const& _charset)
    {
        bool         trim = true;
        rptr_t       iter = get_begin(_str);
        rptr_t const end  = get_end(_str);
        cptr_t const cend = get_end(_charset);
        rptr_t       l    = get_begin(_str);
        while (iter < end && trim)
        {
            uchar32 c1    = read(_str, iter);
            cptr_t  citer = get_begin(_charset);
            trim          = false;
            while (citer < cend)
            {
                uchar32 c2 = read(_charset, citer);
                if (c1 == c2)
                {
                    trim = true;
                    l    = iter;
                    break;
                }
            }
        }
        _str = select(_str, l, end);
    }

    void trimRight(runes_t& _str, crunes_t const& _charset)
    {
        bool         trim  = true;
        rptr_t       iter  = get_end(_str);
        rptr_t const begin = get_begin(_str);
        cptr_t const cend  = get_end(_charset);
        rptr_t       r     = iter;
        while (iter > begin && trim)
        {
            backwards(_str, iter, 1);
            uchar32 c1    = peek(_str, iter);
            cptr_t  citer = get_begin(_charset);
            trim          = false;
            while (citer < cend)
            {
                uchar32 c2 = peek(_charset, citer);
                if (c1 == c2)
                {
                    trim = true;
                    r    = iter;
                    break;
                }
            }
        }
        _str = select(_str, begin, r);
    }

    void trimQuotes(runes_t& str)
    {
        uchar32  charseta[] = {'\'', '"', utf32::TERMINATOR};
        crunes_t charset(charseta, &charseta[2]);
        trim(str, charseta);
    }

    void trimQuotes(runes_t& str, uchar32 quote)
    {
        uchar32  charseta[] = {quote, utf32::TERMINATOR};
        crunes_t charset(charseta, &charseta[1]);
        trim(str, charseta);
    }

    void trimDelimiters(runes_t& str, uchar32 _left, uchar32 _right)
    {
        trimLeft(str, _left);
        trimRight(str, _right);
    }

    void trim(crunes_t& str)
    {
        uchar32  charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t charset(charseta, &charseta[2]);
        trimLeft(str, charseta);
        trimRight(str, charseta);
    }

    void trimLeft(crunes_t& str)
    {
        uchar32  charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t charset(charseta, &charseta[2]);
        trimLeft(str, charseta);
    }

    void trimRight(crunes_t& str)
    {
        uchar32  charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t charset(charseta, &charseta[2]);
        trimRight(str, charseta);
    }

    void trim(crunes_t& str, uchar32 _c)
    {
        uchar32  charseta[] = {_c, utf32::TERMINATOR};
        crunes_t charset(charseta, &charseta[1]);
        trimLeft(str, charseta);
        trimRight(str, charseta);
    }

    void trimLeft(crunes_t& str, uchar32 _c)
    {
        uchar32 charset[] = {_c, utf32::TERMINATOR};
        trimLeft(str, charset);
    }

    void trimRight(crunes_t& str, uchar32 _c)
    {
        uchar32 charset[2] = {_c, utf32::TERMINATOR};
        trimLeft(str, charset);
    }

    void trim(crunes_t& str, crunes_t const& _charset)
    {
        trimLeft(str, _charset);
        trimRight(str, _charset);
    }

    void trimLeft(crunes_t& _str, crunes_t const& _charset)
    {
        bool         trim = true;
        cptr_t       iter = get_begin(_str);
        cptr_t const end  = get_end(_str);
        cptr_t const cend = get_end(_charset);
        cptr_t       l    = get_begin(_str);
        while (iter < end && trim)
        {
            uchar32 c1    = read(_str, iter);
            cptr_t  citer = get_begin(_charset);
            trim          = false;
            while (citer < cend)
            {
                uchar32 c2 = read(_charset, citer);
                if (c1 == c2)
                {
                    trim = true;
                    l    = iter;
                    break;
                }
            }
        }
        _str = select(_str, l, end);
    }

    void trimRight(crunes_t& _str, crunes_t const& _charset)
    {
        bool         trim  = true;
        cptr_t       iter  = get_end(_str);
        cptr_t const begin = get_begin(_str);
        cptr_t const cend  = get_end(_charset);
        cptr_t       r     = iter;
        while (iter > begin && trim)
        {
            backwards(_str, iter, 1);
            uchar32 c1    = peek(_str, iter);
            cptr_t  citer = get_begin(_charset);
            trim          = false;
            while (citer < cend)
            {
                uchar32 c2 = read(_charset, citer);
                if (c1 == c2)
                {
                    trim = true;
                    r    = iter;
                    break;
                }
            }
        }
        _str = select(_str, begin, r);
    }

    void trimQuotes(crunes_t& str)
    {
        uchar32  charseta[] = {'\'', '"', utf32::TERMINATOR};
        crunes_t charset(charseta, &charseta[2]);
        trim(str, charseta);
    }

    void trimQuotes(crunes_t& str, uchar32 quote)
    {
        uchar32  charseta[] = {quote, utf32::TERMINATOR};
        crunes_t charset(charseta, &charseta[1]);
        trim(str, charseta);
    }

    void trimDelimiters(crunes_t& str, uchar32 _left, uchar32 _right)
    {
        trimLeft(str, _left);
        trimRight(str, _right);
    }

    void resize(runes_t& str, s32 cap, runes_alloc_t* allocator, s32 size_alignment)
    {
        runes_t nstr = allocator->allocate(0, cap, str.m_type);
        if (str.is_valid())
        {
            copy(str, nstr);
            allocator->deallocate(str);
        }
        str = nstr;
    }

    void copy(crunes_t const& _src, runes_t& _dst)
    {
        cptr_t siter  = get_begin(_src);
        cptr_t send   = get_end(_src);
        rptr_t dbegin = get_begin(_dst);
        rptr_t diter  = get_begin(_dst);
        rptr_t dend   = get_eos(_dst);
        while (diter < dend && siter < send)
        {
            uchar32 c = read(_src, siter);
            if (c == 0)
                break;
            write(_dst, diter, c);
        }
        _dst = select(_dst, dbegin, diter);
    }

    void concatenate(runes_t& _dst, crunes_t const& _con)
    {
        cptr_t siter  = get_begin(_con);
        cptr_t send   = get_end(_con);
        rptr_t dbegin = get_begin(_dst);
        rptr_t diter  = get_end(_dst);
        rptr_t dend   = get_eos(_dst);
        while (diter < dend && siter < send)
        {
            uchar32 c = read(_con, siter);
            write(_dst, diter, c);
        }
        _dst = select(_dst, dbegin, diter);
    }

    void copy(const crunes_t& src, runes_t& dst, runes_alloc_t* allocator, s32 size_alignment)
    {
        s32 const required = src.size();
        if (required > dst.cap())
        {
            allocator->deallocate(dst);
            resize(dst, required, allocator, size_alignment);
        }
        copy(src, dst);
    }

    void concatenate(runes_t& str, const crunes_t& concat, runes_alloc_t* allocator, s32 size_alignment)
    {
        s32 const required = str.size() + concat.size();
        if (required > str.cap())
        {
            resize(str, required, allocator, size_alignment);
        }
        concatenate(str, concat);
    }

    void concatenate(runes_t& str, const crunes_t& concat1, const crunes_t& concat2, runes_alloc_t* allocator,
                     s32 size_alignment)
    {
        s32 const required = str.size() + concat1.size() + concat2.size();
        if (required > str.cap())
        {
            resize(str, required, allocator, size_alignment);
        }
        concatenate(str, concat1);
        concatenate(str, concat2);
    }

    // ---------------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------------
    // rptr_t functions
    rptr_t get_begin(runes_t const& str)
    {
        rptr_t c;
        c.m_ptr.m_ascii = str.m_runes.m_ascii.m_str;
        return c;
    }
    rptr_t get_end(runes_t const& str)
    {
        rptr_t c;
        c.m_ptr.m_ascii = str.m_runes.m_ascii.m_end;
        return c;
    }
    rptr_t get_eos(runes_t const& str)
    {
        rptr_t c;
        c.m_ptr.m_ascii = str.m_runes.m_ascii.m_eos;
        return c;
    }
    runes_t select(runes_t const& str, rptr_t const& from, rptr_t const& to)
    {
        runes_t r;
        r.m_type = str.m_type;
        r.m_runes.m_ascii.m_bos = str.m_runes.m_ascii.m_bos;
        r.m_runes.m_ascii.m_str = from.m_ptr.m_ascii;
        r.m_runes.m_ascii.m_end = to.m_ptr.m_ascii;
        r.m_runes.m_ascii.m_eos = str.m_runes.m_ascii.m_eos;
        return r;
    }
    bool forwards(runes_t const& str, rptr_t& cursor, s32 step)
    {
        switch (str.m_type)
        {
            case ascii::TYPE:
                return utf::skip(cursor.m_ptr.m_ascii, str.m_runes.m_ascii.m_str, str.m_runes.m_ascii.m_end, step);
            case utf8::TYPE: return utf::skip(cursor.m_ptr.m_utf8, str.m_runes.m_utf8.m_str, str.m_runes.m_utf8.m_end, step);
            case utf16::TYPE:
                return utf::skip(cursor.m_ptr.m_utf16, str.m_runes.m_utf16.m_str, str.m_runes.m_utf16.m_end, step);
            case utf32::TYPE:
                return utf::skip(cursor.m_ptr.m_utf32, str.m_runes.m_utf32.m_str, str.m_runes.m_utf32.m_end, step);
        }
        return false;
    }
    bool backwards(runes_t const& str, rptr_t& cursor, s32 step)
    {
        switch (str.m_type)
        {
            case ascii::TYPE:
                return utf::skip(cursor.m_ptr.m_ascii, str.m_runes.m_ascii.m_str, str.m_runes.m_ascii.m_end, -step);
            case utf8::TYPE: return utf::skip(cursor.m_ptr.m_utf8, str.m_runes.m_utf8.m_str, str.m_runes.m_utf8.m_end, -step);
            case utf16::TYPE:
                return utf::skip(cursor.m_ptr.m_utf16, str.m_runes.m_utf16.m_str, str.m_runes.m_utf16.m_end, -step);
            case utf32::TYPE:
                return utf::skip(cursor.m_ptr.m_utf32, str.m_runes.m_utf32.m_str, str.m_runes.m_utf32.m_end, -step);
        }
        return false;
    }
    uchar32 peek(runes_t const& str, rptr_t const& cursor)
    {
        switch (str.m_type)
        {
            case ascii::TYPE: return utf::peek(str.m_runes.m_ascii, cursor);
            case utf8::TYPE: return utf::peek(str.m_runes.m_utf8, cursor);
            case utf16::TYPE: return utf::peek(str.m_runes.m_utf16, cursor);
            case utf32::TYPE: return utf::peek(str.m_runes.m_utf32, cursor);
        }
        return '\0';
    }
    uchar32 read(runes_t const& str, rptr_t& cursor)
    {
        switch (str.m_type)
        {
            case ascii::TYPE: return utf::read(cursor.m_ptr.m_ascii, str.m_runes.m_ascii.m_end);
            case utf8::TYPE: return utf::read(cursor.m_ptr.m_utf8, str.m_runes.m_utf8.m_end);
            case utf16::TYPE: return utf::read(cursor.m_ptr.m_utf16, str.m_runes.m_utf16.m_end);
            case utf32::TYPE: return utf::read(cursor.m_ptr.m_utf32, str.m_runes.m_utf32.m_end);
        }
        return '\0';
    }
    bool write(runes_t const& str, rptr_t& cursor, uchar32 c)
    {
        switch (str.m_type)
        {
            case ascii::TYPE: return utf::write(c, cursor.m_ptr.m_ascii, str.m_runes.m_ascii.m_eos); break;
            case utf32::TYPE: return utf::write(c, cursor.m_ptr.m_utf32, str.m_runes.m_utf32.m_eos); break;
            default: return false;
        }
        return true;
    }

    bool is_valid(runes_t const& str, rptr_t const& cursor)
    {
        return cursor.m_ptr.m_ascii >= str.m_runes.m_ascii.m_str && cursor.m_ptr.m_ascii < str.m_runes.m_ascii.m_end;
    }

    // ---------------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------------
    // cptr_t functions
    cptr_t get_begin(crunes_t const& str)
    {
        cptr_t c;
        c.m_ptr.m_ascii = str.m_runes.m_ascii.m_str;
        return c;
    }
    cptr_t get_end(crunes_t const& str)
    {
        cptr_t c;
        c.m_ptr.m_ascii = str.m_runes.m_ascii.m_end;
        return c;
    }
    cptr_t get_eos(crunes_t const& str)
    {
        cptr_t c;
        c.m_ptr.m_ascii = str.m_runes.m_ascii.m_eos;
        return c;
    }
    crunes_t select(crunes_t const& str, cptr_t const& from, cptr_t const& to)
    {
        crunes_t r;
        r.m_type = str.m_type;
        r.m_runes.m_ascii.m_bos = str.m_runes.m_ascii.m_bos;
        r.m_runes.m_ascii.m_str = from.m_ptr.m_ascii;
        r.m_runes.m_ascii.m_end = to.m_ptr.m_ascii;
        r.m_runes.m_ascii.m_eos = str.m_runes.m_ascii.m_eos;
        return r;
    }

    crunes_t select(crunes_t const& str, cptr_t const& from, u32 len)
    {
        crunes_t r;
        r.m_type = str.m_type;
        r.m_runes.m_ascii.m_bos = str.m_runes.m_ascii.m_bos;
        r.m_runes.m_ascii.m_eos = str.m_runes.m_ascii.m_eos;
        r.m_runes.m_ascii.m_str = from.m_ptr.m_ascii;
        r.m_runes.m_ascii.m_end = from.m_ptr.m_ascii;
        if (str.m_type == ascii::TYPE)
        {
            r.m_runes.m_ascii.m_end += len;
        }
        else if (str.m_type == utf32::TYPE)
        {
            r.m_runes.m_utf32.m_end += len;
        }
        return r;
    }

    bool forwards(crunes_t const& str, cptr_t& cursor, s32 step)
    {
        switch (str.m_type)
        {
            case ascii::TYPE:
                return utf::skip(cursor.m_ptr.m_ascii, str.m_runes.m_ascii.m_str, str.m_runes.m_ascii.m_end, step);
            case utf8::TYPE: return utf::skip(cursor.m_ptr.m_utf8, str.m_runes.m_utf8.m_str, str.m_runes.m_utf8.m_end, step);
            case utf16::TYPE:
                return utf::skip(cursor.m_ptr.m_utf16, str.m_runes.m_utf16.m_str, str.m_runes.m_utf16.m_end, step);
            case utf32::TYPE:
                return utf::skip(cursor.m_ptr.m_utf32, str.m_runes.m_utf32.m_str, str.m_runes.m_utf32.m_end, step);
        }
        return false;
    }
    bool backwards(crunes_t const& str, cptr_t& cursor, s32 step)
    {
        switch (str.m_type)
        {
            case ascii::TYPE:
                return utf::skip(cursor.m_ptr.m_ascii, str.m_runes.m_ascii.m_str, str.m_runes.m_ascii.m_end, -step);
            case utf8::TYPE: return utf::skip(cursor.m_ptr.m_utf8, str.m_runes.m_utf8.m_str, str.m_runes.m_utf8.m_end, -step);
            case utf16::TYPE:
                return utf::skip(cursor.m_ptr.m_utf16, str.m_runes.m_utf16.m_str, str.m_runes.m_utf16.m_end, -step);
            case utf32::TYPE:
                return utf::skip(cursor.m_ptr.m_utf32, str.m_runes.m_utf32.m_str, str.m_runes.m_utf32.m_end, -step);
        }
        return false;
    }

    uchar32 peek(crunes_t const& str, cptr_t const& cursor)
    {
        uchar32 c = '\0';
        switch (str.m_type)
        {
            case ascii::TYPE: c = utf::peek(str.m_runes.m_ascii, cursor.m_ptr.m_ascii); break;
            case utf8::TYPE: c = utf::peek(str.m_runes.m_utf8, cursor.m_ptr.m_utf8); break;
            case utf16::TYPE: c = utf::peek(str.m_runes.m_utf16, cursor.m_ptr.m_utf16); break;
            case utf32::TYPE: c = utf::peek(str.m_runes.m_utf32, cursor.m_ptr.m_utf32); break;
        }

        return c;
    }
    uchar32 read(crunes_t const& str, cptr_t& cursor)
    {
        uchar32 c = '\0';
        switch (str.m_type)
        {
            case ascii::TYPE: c = utf::read(cursor.m_ptr.m_ascii, str.m_runes.m_ascii.m_end); break;
            case utf8::TYPE: c = utf::read(cursor.m_ptr.m_utf8, str.m_runes.m_utf8.m_end); break;
            case utf16::TYPE: c = utf::read(cursor.m_ptr.m_utf16, str.m_runes.m_utf16.m_end); break;
            case utf32::TYPE: c = utf::read(cursor.m_ptr.m_utf32, str.m_runes.m_utf32.m_end); break;
        }

        return c;
    }

    bool write(crunes_t const& str, cptr_t& cursor, uchar32 c) { return false; }
    bool is_valid(crunes_t const& str, cptr_t const& cursor)
    {
        return cursor.m_ptr.m_ascii >= str.m_runes.m_ascii.m_str && cursor.m_ptr.m_ascii < str.m_runes.m_ascii.m_end;
    }

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------

    runes_t::runes_t() : m_type(ascii::TYPE)
    {
        m_runes.m_ascii.m_bos = nullptr;
        m_runes.m_ascii.m_str = m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_end = m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_eos = m_runes.m_ascii.m_bos;
    }

    runes_t::runes_t(runes_t const& other) : m_type(other.m_type) { m_runes.m_ascii = other.m_runes.m_ascii; }

    runes_t::runes_t(ascii::prune _str, ascii::prune _end, s32 _type) : m_type(_type)
    {
        m_runes.m_ascii.m_bos = _str;
        m_runes.m_ascii.m_str = _str;
        m_runes.m_ascii.m_end = _end;
        m_runes.m_ascii.m_eos = _end;
    }
    runes_t::runes_t(ascii::prune _str, ascii::prune _end, ascii::prune _eos, s32 _type) : m_type(_type)
    {
        m_runes.m_ascii.m_bos = _str;
        m_runes.m_ascii.m_str = _str;
        m_runes.m_ascii.m_end = _end;
        m_runes.m_ascii.m_eos = _eos;
    }
    runes_t::runes_t(ascii::prune _bos, ascii::prune _str, ascii::prune _end, ascii::prune _eos, s32 _type) : m_type(_type)
    {
        m_runes.m_ascii.m_bos = _bos;
        m_runes.m_ascii.m_str = _str;
        m_runes.m_ascii.m_end = _end;
        m_runes.m_ascii.m_eos = _eos;
    }

    runes_t::runes_t(utf16::prune _str, utf16::prune _end, s32 _type) : m_type(_type)
    {
        m_runes.m_utf16.m_bos = _str;
        m_runes.m_utf16.m_str = _str;
        m_runes.m_utf16.m_eos = _end;
        m_runes.m_utf16.m_end = _end;
    }
    runes_t::runes_t(utf16::prune _str, utf16::prune _end, utf16::prune _eos, s32 _type) : m_type(_type)
    {
        m_runes.m_utf16.m_bos = _str;
        m_runes.m_utf16.m_str = _str;
        m_runes.m_utf16.m_end = _end;
        m_runes.m_utf16.m_eos = _eos;
    }
    runes_t::runes_t(utf16::prune _bos, utf16::prune _str, utf16::prune _end, utf16::prune _eos, s32 _type) : m_type(_type)
    {
        m_runes.m_utf16.m_bos = _bos;
        m_runes.m_utf16.m_str = _str;
        m_runes.m_utf16.m_end = _end;
        m_runes.m_utf16.m_eos = _eos;
    }

    runes_t::runes_t(utf32::prune _str, utf32::prune _end, s32 _type) : m_type(_type)
    {
        m_runes.m_utf32.m_bos = _str;
        m_runes.m_utf32.m_str = _str;
        m_runes.m_utf32.m_eos = _end;
        m_runes.m_utf32.m_end = _end;
    }
    runes_t::runes_t(utf32::prune _str, utf32::prune _end, utf32::prune _eos, s32 _type) : m_type(_type)
    {
        m_runes.m_utf32.m_bos = _str;
        m_runes.m_utf32.m_str = _str;
        m_runes.m_utf32.m_end = _end;
        m_runes.m_utf32.m_eos = _eos;
    }
    runes_t::runes_t(utf32::prune _bos, utf32::prune _str, utf32::prune _end, utf32::prune _eos, s32 _type) : m_type(_type)
    {
        m_runes.m_utf32.m_bos = _bos;
        m_runes.m_utf32.m_str = _str;
        m_runes.m_utf32.m_end = _end;
        m_runes.m_utf32.m_eos = _eos;
    }

    ascii::prune runes_t::str()
    {
        if (m_type == ascii::TYPE)
            return m_runes.m_ascii.m_str;
        return nullptr;
    }
    utf16::prune runes_t::str16()
    {
        if (m_type == utf16::TYPE)
            return m_runes.m_utf16.m_str;
        return nullptr;
    }
    utf32::prune runes_t::str32()
    {
        if (m_type == utf32::TYPE)
            return m_runes.m_utf32.m_str;
        return nullptr;
    }
    ascii::prune runes_t::end()
    {
        if (m_type == ascii::TYPE)
            return m_runes.m_ascii.m_end;
        return nullptr;
    }
    utf16::prune runes_t::end16()
    {
        if (m_type == utf16::TYPE)
            return m_runes.m_utf16.m_end;
        return nullptr;
    }
    utf32::prune runes_t::end32()
    {
        if (m_type == utf32::TYPE)
            return m_runes.m_utf32.m_end;
        return nullptr;
    }

    s32 runes_t::size() const
    {
        s32 size = 0;
        switch (m_type)
        {
            case ascii::TYPE: size = (s32)((m_runes.m_ascii.m_end - m_runes.m_ascii.m_str)); break;
            case utf32::TYPE: size = (s32)((m_runes.m_utf32.m_end - m_runes.m_utf32.m_str)); break;
            case utf16::TYPE:
            {
                utf16::prune str = m_runes.m_utf16.m_str;
                while (*str != 0)
                {
                    size += 1;
                    utf::read(m_runes.m_utf16, str);
                }
                break;
            }
            case utf8::TYPE:
            {
                utf8::prune str = m_runes.m_utf8.m_str;
                while (*str != 0)
                {
                    size += 1;
                    utf::read(m_runes.m_utf8, str);
                }
                break;
            }
        }
        return size;
    }
    s32 runes_t::cap() const
    {
        s32 cap = 0;
        switch (m_type)
        {
            case ascii::TYPE: cap = (s32)((m_runes.m_ascii.m_eos - m_runes.m_ascii.m_str)); break;
            case utf16::TYPE: cap = (s32)((m_runes.m_utf16.m_eos - m_runes.m_utf16.m_str)); break;
            case utf32::TYPE: cap = (s32)((m_runes.m_utf32.m_eos - m_runes.m_utf32.m_str)); break;
        }
        return cap;
    }

    bool runes_t::is_empty() const { return size() == 0; }
    bool runes_t::is_valid() const { return m_runes.m_ascii.m_end < m_runes.m_ascii.m_eos; }
    bool runes_t::is_nil() const { return m_runes.m_ascii.m_bos == nullptr; }
    void runes_t::reset() { m_runes.m_ascii.m_end = m_runes.m_ascii.m_str; }
    void runes_t::clear()
    {
        m_runes.m_ascii.m_str = m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_end = m_runes.m_ascii.m_bos;
    }
    void runes_t::term()
    {
        switch (m_type)
        {
            case ascii::TYPE: *m_runes.m_ascii.m_end++ = ascii::TERMINATOR; break;
            case utf32::TYPE: *m_runes.m_utf32.m_end++ = utf32::TERMINATOR; break;
            case utf16::TYPE: *m_runes.m_utf16.m_end++ = utf16::TERMINATOR; break;
            case utf8::TYPE: *m_runes.m_utf8.m_end++ = utf8::TERMINATOR; break;
        }
    }

    runes_t& runes_t::operator+=(const ascii::crunes_t& str)
    {
        crunes_t cstr(str);
        concatenate(*this, cstr);
        return *this;
    }
    runes_t& runes_t::operator+=(const utf32::crunes_t& str)
    {
        crunes_t cstr(str);
        concatenate(*this, cstr);
        return *this;
    }
    runes_t& runes_t::operator+=(ascii::rune c)
    {
        ascii::rune str[2];
        str[0] = c;
        str[1] = ascii::TERMINATOR;
        crunes_t cstr(str, 1);
        concatenate(*this, cstr);
        return *this;
    }
    runes_t& runes_t::operator+=(utf32::rune c)
    {
        utf32::rune str[2];
        str[0] = c;
        str[1] = utf32::TERMINATOR;
        crunes_t cstr(str, 1);
        concatenate(*this, cstr);
        return *this;
    }
    runes_t& runes_t::operator=(runes_t const& other)
    {
        m_type  = other.m_type;
        m_runes = other.m_runes;
        return *this;
    }

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------
    crunes_t::crunes_t() : m_type(ascii::TYPE)
    {
        m_runes.m_ascii.m_bos = nullptr;
        m_runes.m_ascii.m_str = m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_end = m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_eos = m_runes.m_ascii.m_bos;
    }
    crunes_t::crunes_t(runes_t const& _str) : m_type(_str.m_type)
    {
        m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_str = _str.m_runes.m_ascii.m_str;
        m_runes.m_ascii.m_end = _str.m_runes.m_ascii.m_end;
        m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
    }
    crunes_t::crunes_t(crunes_t const& _str) : m_type(_str.m_type)
    {
        m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_str = _str.m_runes.m_ascii.m_str;
        m_runes.m_ascii.m_end = _str.m_runes.m_ascii.m_end;
        m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
    }
    crunes_t::crunes_t(crunes_t const& _str, ptr_t const& from, ptr_t const& to) : m_type(_str.m_type)
    {
        ASSERT(from >= _str.m_runes.m_ascii.m_bos && from < _str.m_runes.m_ascii.m_eos);
        ASSERT(to >= _str.m_runes.m_ascii.m_bos && to < _str.m_runes.m_ascii.m_eos);
        ASSERT(from <= to);
        m_runes.m_ascii.m_bos = _str.m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_str = from.m_ptr.m_ascii;
        m_runes.m_ascii.m_end = to.m_ptr.m_ascii;
        m_runes.m_ascii.m_eos = _str.m_runes.m_ascii.m_eos;
    }
    crunes_t::crunes_t(ascii::crunes_t const& _str) : m_type(ascii::TYPE)
    {
        m_runes.m_ascii.m_bos = _str.m_bos;
        m_runes.m_ascii.m_str = _str.m_str;
        m_runes.m_ascii.m_end = _str.m_end;
        m_runes.m_ascii.m_eos = _str.m_eos;
    }
    crunes_t::crunes_t(ascii::pcrune _str) : m_type(ascii::TYPE)
    {
        m_runes.m_ascii.m_bos = _str;
        m_runes.m_ascii.m_str = _str;
        m_runes.m_ascii.m_end = utf::endof(_str, nullptr);
        m_runes.m_ascii.m_eos = m_runes.m_ascii.m_end;
    }
    crunes_t::crunes_t(ascii::pcrune _str, u32 len) : m_type(ascii::TYPE)
    {
        m_runes.m_ascii.m_bos = _str;
        m_runes.m_ascii.m_str = _str;
        m_runes.m_ascii.m_end = _str + len;
        m_runes.m_ascii.m_eos = m_runes.m_ascii.m_end;
    }
    crunes_t::crunes_t(ascii::pcrune _str, ascii::pcrune _end) : m_type(ascii::TYPE)
    {
        m_runes.m_ascii.m_bos = _str;
        m_runes.m_ascii.m_str = _str;
        m_runes.m_ascii.m_end = _end;
        m_runes.m_ascii.m_eos = _end;
    }
    crunes_t::crunes_t(ascii::pcrune _str, ascii::pcrune _end, ascii::pcrune _eos, s32 _type) : m_type(_type)
    {
        m_runes.m_ascii.m_bos = _str;
        m_runes.m_ascii.m_str = _str;
        m_runes.m_ascii.m_end = _end;
        m_runes.m_ascii.m_eos = _eos;
    }
    crunes_t::crunes_t(ascii::pcrune _bos, ascii::pcrune _str, ascii::pcrune _end, ascii::pcrune _eos, s32 _type)
        : m_type(_type)
    {
        m_runes.m_ascii.m_bos = _bos;
        m_runes.m_ascii.m_str = _str;
        m_runes.m_ascii.m_end = _end;
        m_runes.m_ascii.m_eos = _eos;
    }

    crunes_t::crunes_t(utf8::crunes_t const& _str) : m_type(utf8::TYPE)
    {
        m_runes.m_utf8.m_bos = _str.m_bos;
        m_runes.m_utf8.m_str = _str.m_str;
        m_runes.m_utf8.m_end = _str.m_end;
        m_runes.m_utf8.m_eos = _str.m_eos;
    }
    crunes_t::crunes_t(utf8::pcrune _str) : m_type(utf8::TYPE)
    {
        m_runes.m_utf8.m_bos = _str;
        m_runes.m_utf8.m_str = _str;
        m_runes.m_utf8.m_end = utf::endof(_str, nullptr);
        m_runes.m_utf8.m_eos = m_runes.m_utf8.m_end;
    }
    crunes_t::crunes_t(utf8::pcrune _str, u32 len) : m_type(utf8::TYPE)
    {
        m_runes.m_utf8.m_bos = _str;
        m_runes.m_utf8.m_str = _str;
        m_runes.m_utf8.m_end = _str;
        utf::skip(m_runes.m_utf8.m_end, _str, nullptr, len);
        m_runes.m_utf8.m_eos = m_runes.m_utf8.m_end;
    }
    crunes_t::crunes_t(utf8::pcrune _str, utf8::pcrune _end) : m_type(utf8::TYPE)
    {
        m_runes.m_utf8.m_bos = _str;
        m_runes.m_utf8.m_str = _str;
        m_runes.m_utf8.m_end = _end;
        m_runes.m_utf8.m_eos = _end;
    }
    crunes_t::crunes_t(utf8::pcrune _str, utf8::pcrune _end, utf8::pcrune _eos) : m_type(utf8::TYPE)
    {
        m_runes.m_utf8.m_bos = _str;
        m_runes.m_utf8.m_str = _str;
        m_runes.m_utf8.m_end = _end;
        m_runes.m_utf8.m_eos = _eos;
    }
    crunes_t::crunes_t(utf8::pcrune _bos, utf8::pcrune _str, utf8::pcrune _end, utf8::pcrune _eos) : m_type(utf8::TYPE)
    {
        m_runes.m_utf8.m_bos = _bos;
        m_runes.m_utf8.m_str = _str;
        m_runes.m_utf8.m_end = _end;
        m_runes.m_utf8.m_eos = _eos;
    }

    crunes_t::crunes_t(utf16::crunes_t const& _str) : m_type(utf16::TYPE)
    {
        m_runes.m_utf16.m_bos = _str.m_bos;
        m_runes.m_utf16.m_str = _str.m_str;
        m_runes.m_utf16.m_end = _str.m_end;
        m_runes.m_utf16.m_eos = _str.m_eos;
    }
    crunes_t::crunes_t(utf16::pcrune _str) : m_type(utf16::TYPE)
    {
        m_runes.m_utf16.m_bos = _str;
        m_runes.m_utf16.m_str = _str;
        m_runes.m_utf16.m_end = utf::endof(_str, nullptr);
        m_runes.m_utf16.m_eos = m_runes.m_utf16.m_end;
    }
    crunes_t::crunes_t(utf16::pcrune _str, u32 len) : m_type(utf16::TYPE)
    {
        m_runes.m_utf16.m_bos = _str;
        m_runes.m_utf16.m_str = _str;
        m_runes.m_utf16.m_end = _str;
        utf::skip(m_runes.m_utf16.m_end, _str, nullptr, len);
        m_runes.m_utf16.m_eos = m_runes.m_utf16.m_end;
    }
    crunes_t::crunes_t(utf16::pcrune _str, utf16::pcrune _end) : m_type(utf16::TYPE)
    {
        m_runes.m_utf16.m_bos = _str;
        m_runes.m_utf16.m_str = _str;
        m_runes.m_utf16.m_end = _end;
        m_runes.m_utf16.m_eos = _end;
    }
    crunes_t::crunes_t(utf16::pcrune _str, utf16::pcrune _end, utf16::pcrune _eos) : m_type(utf16::TYPE)
    {
        m_runes.m_utf16.m_bos = _str;
        m_runes.m_utf16.m_str = _str;
        m_runes.m_utf16.m_end = _end;
        m_runes.m_utf16.m_eos = _eos;
    }
    crunes_t::crunes_t(utf16::pcrune _bos, utf16::pcrune _str, utf16::pcrune _end, utf16::pcrune _eos) : m_type(utf16::TYPE)
    {
        m_runes.m_utf16.m_bos = _bos;
        m_runes.m_utf16.m_str = _str;
        m_runes.m_utf16.m_end = _end;
        m_runes.m_utf16.m_eos = _eos;
    }

    crunes_t::crunes_t(utf32::crunes_t const& _str) : m_type(utf32::TYPE)
    {
        m_runes.m_utf32.m_bos = _str.m_bos;
        m_runes.m_utf32.m_str = _str.m_str;
        m_runes.m_utf32.m_end = _str.m_end;
        m_runes.m_utf32.m_eos = _str.m_eos;
    }
    crunes_t::crunes_t(utf32::pcrune _str) : m_type(utf32::TYPE)
    {
        m_runes.m_utf32.m_bos = _str;
        m_runes.m_utf32.m_str = _str;
        m_runes.m_utf32.m_end = utf::endof(_str, nullptr);
        m_runes.m_utf32.m_eos = m_runes.m_utf32.m_end;
    }
    crunes_t::crunes_t(utf32::pcrune _str, u32 len) : m_type(utf32::TYPE)
    {
        m_runes.m_utf32.m_bos = _str;
        m_runes.m_utf32.m_str = _str;
        m_runes.m_utf32.m_end = _str + len;
        m_runes.m_utf32.m_eos = m_runes.m_utf32.m_end;
    }
    crunes_t::crunes_t(utf32::pcrune _str, utf32::pcrune _end) : m_type(utf32::TYPE)
    {
        m_runes.m_utf32.m_bos = _str;
        m_runes.m_utf32.m_str = _str;
        m_runes.m_utf32.m_eos = _end;
        m_runes.m_utf32.m_end = _end;
    }
    crunes_t::crunes_t(utf32::pcrune _str, utf32::pcrune _end, utf32::pcrune _eos) : m_type(utf32::TYPE)
    {
        m_runes.m_utf32.m_bos = _str;
        m_runes.m_utf32.m_str = _str;
        m_runes.m_utf32.m_end = _end;
        m_runes.m_utf32.m_eos = _eos;
    }
    crunes_t::crunes_t(utf32::pcrune _bos, utf32::pcrune _str, utf32::pcrune _end, utf32::pcrune _eos) : m_type(utf32::TYPE)
    {
        m_runes.m_utf32.m_bos = _bos;
        m_runes.m_utf32.m_str = _str;
        m_runes.m_utf32.m_end = _end;
        m_runes.m_utf32.m_eos = _eos;
    }

    crunes_t& crunes_t::operator=(crunes_t const& other)
    {
        m_type                = other.m_type;
        m_runes.m_ascii.m_bos = other.m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_str = other.m_runes.m_ascii.m_str;
        m_runes.m_ascii.m_end = other.m_runes.m_ascii.m_end;
        m_runes.m_ascii.m_eos = other.m_runes.m_ascii.m_eos;
        return *this;
    }

    s32 crunes_t::size() const
    {
        s32 size = 0;
        switch (m_type)
        {
            case ascii::TYPE: size = (s32)((m_runes.m_ascii.m_end - m_runes.m_ascii.m_str)); break;
            case utf32::TYPE: size = (s32)((m_runes.m_utf32.m_end - m_runes.m_utf32.m_str)); break;
            case utf16::TYPE:
            case utf8::TYPE:
                break; // This is problematic, since we need to count the actual runes since a rune can span more than one utf
                       // value.
        }
        return size;
    }
    bool crunes_t::is_empty() const { return m_runes.m_ascii.m_end == m_runes.m_ascii.m_str; }
    bool crunes_t::is_valid() const { return m_runes.m_ascii.m_end < m_runes.m_ascii.m_eos; }
    bool crunes_t::is_nil() const { return m_runes.m_ascii.m_bos == nullptr; }
    void crunes_t::reset() { m_runes.m_ascii.m_end = m_runes.m_ascii.m_str; }
    void crunes_t::clear()
    {
        m_runes.m_ascii.m_str = m_runes.m_ascii.m_bos;
        m_runes.m_ascii.m_end = m_runes.m_ascii.m_bos;
    }

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------
    runes_reader_t::runes_reader_t() {}

    runes_reader_t::runes_reader_t(ascii::pcrune str)
    {
        ascii::pcrune str_end = str;
        while (*str_end != '\0')
            str_end++;
        m_runes  = crunes_t(str, str, str_end, str_end);
        m_cursor = str;
    }
    runes_reader_t::runes_reader_t(ascii::pcrune str, u32 len)
    {
        ascii::pcrune str_end = str + len;
        m_runes               = crunes_t(str, str, str_end, str_end);
        m_cursor              = str;
    }
    runes_reader_t::runes_reader_t(ascii::pcrune str, ascii::pcrune str_end)
    {
        m_runes  = crunes_t(str, str, str_end, str_end);
        m_cursor = str;
    }
    runes_reader_t::runes_reader_t(utf32::pcrune str, utf32::pcrune str_end)
    {
        m_runes  = crunes_t(str, str, str_end, str_end);
        m_cursor = str;
    }
    runes_reader_t::runes_reader_t(crunes_t const& runes)
    {
        m_runes.m_type                = runes.m_type;
        m_runes.m_runes.m_ascii.m_bos = runes.m_runes.m_ascii.m_str;
        m_runes.m_runes.m_ascii.m_eos = runes.m_runes.m_ascii.m_eos;
        m_runes.m_runes.m_ascii.m_str = runes.m_runes.m_ascii.m_str;
        m_runes.m_runes.m_ascii.m_end = runes.m_runes.m_ascii.m_end;
        m_cursor                      = m_runes.m_runes.m_ascii.m_str;
    }

    crunes_t runes_reader_t::get_source() const
    {
        crunes_t str;
        str.m_type                = m_runes.m_type;
        str.m_runes.m_ascii.m_bos = m_runes.m_runes.m_ascii.m_str;
        str.m_runes.m_ascii.m_eos = m_runes.m_runes.m_ascii.m_eos;
        str.m_runes.m_ascii.m_str = m_runes.m_runes.m_ascii.m_str;
        str.m_runes.m_ascii.m_end = m_runes.m_runes.m_ascii.m_end;
        return str;
    }

    crunes_t runes_reader_t::get_current() const
    {
        crunes_t str;
        str.m_type                = m_runes.m_type;
        str.m_runes.m_ascii.m_bos = m_runes.m_runes.m_ascii.m_str;
        str.m_runes.m_ascii.m_eos = m_runes.m_runes.m_ascii.m_eos;
        str.m_runes.m_ascii.m_str = m_cursor.m_ptr.m_ascii;
        str.m_runes.m_ascii.m_end = m_runes.m_runes.m_ascii.m_end;
        return str;
    }

    bool runes_reader_t::at_end() const { return m_cursor.m_ptr.m_ascii >= m_runes.m_runes.m_ascii.m_end; }
    bool runes_reader_t::valid() const { return m_cursor.m_ptr.m_ascii < m_runes.m_runes.m_ascii.m_end; }

    void runes_reader_t::reset() { m_cursor.m_ptr.m_ascii = m_runes.m_runes.m_ascii.m_str; }

    uchar32 runes_reader_t::peek() const
    {
        uchar32 c = '\0';
        if (!at_end())
        {
            switch (m_runes.m_type)
            {
                case ascii::TYPE: c = (*m_cursor.m_ptr.m_ascii) & 0xff; break;
                case utf8::TYPE: c = utf::peek(m_runes.m_runes.m_utf8, m_cursor); break;
                case utf16::TYPE: c = utf::peek(m_runes.m_runes.m_utf16, m_cursor); break;
                case utf32::TYPE: c = *m_cursor.m_ptr.m_utf32; break;
            }
        }
        return c;
    }

    uchar32 runes_reader_t::read()
    {
        uchar32 c = '\0';
        if (!at_end())
        {
            switch (m_runes.m_type)
            {
                case ascii::TYPE: c = (*m_cursor.m_ptr.m_ascii++) & 0xff; break;
                case utf8::TYPE: c = utf::read(m_runes.m_runes.m_utf8, m_cursor.m_ptr.m_utf8); break;
                case utf16::TYPE: c = utf::read(m_runes.m_runes.m_utf16, m_cursor.m_ptr.m_utf16); break;
                case utf32::TYPE: c = *m_cursor.m_ptr.m_utf32++; break;
            }
        }
        return c;
    }

    void runes_reader_t::skip(s32 c)
    {
        switch (m_runes.m_type)
        {
            case ascii::TYPE:
                while (c > 0 && !at_end())
                {
                    m_cursor.m_ptr.m_ascii += 1;
                    c -= 1;
                }
                break;

            case utf8::TYPE:
                utf::skip(m_cursor.m_ptr.m_utf8, m_runes.m_runes.m_utf8.m_str, m_runes.m_runes.m_utf8.m_end, c);
                break;

            case utf16::TYPE:
                utf::skip(m_cursor.m_ptr.m_utf16, m_runes.m_runes.m_utf16.m_str, m_runes.m_runes.m_utf16.m_end, c);
                break;

            case utf32::TYPE:
                while (c > 0 && !at_end())
                {
                    m_cursor.m_ptr.m_utf32 += 1;
                    c -= 1;
                }
                break;
        }
    }

    runes_reader_t runes_reader_t::select(crunes_t::ptr_t const& from, crunes_t::ptr_t to) const
    {
        runes_reader_t reader;
        reader.m_runes.m_type                = m_runes.m_type;
        reader.m_runes.m_runes.m_ascii.m_bos = m_runes.m_runes.m_ascii.m_str;
        reader.m_runes.m_runes.m_ascii.m_eos = m_runes.m_runes.m_ascii.m_eos;
        reader.m_runes.m_runes.m_ascii.m_str = from.m_ptr.m_ascii;
        reader.m_runes.m_runes.m_ascii.m_end = to.m_ptr.m_ascii;
        reader.m_cursor                      = from;
        return reader;
    }

    runes_raw_writer_t::runes_raw_writer_t(ascii::prune str, ascii::prune str_end)
    {
        m_runes                = runes_t(str, str, str, str_end, ascii::TYPE);
        m_cursor.m_ptr.m_ascii = str;
        m_count                = 0;
    }
    runes_raw_writer_t::runes_raw_writer_t(utf32::prune str, utf32::prune str_end)
    {
        m_runes                = runes_t(str, str, str, str_end, utf32::TYPE);
        m_cursor.m_ptr.m_utf32 = str;
        m_count                = 0;
    }
    runes_raw_writer_t::runes_raw_writer_t(runes_t const& runes) : m_runes(runes)
    {
        m_cursor.m_ptr.m_ascii = m_runes.m_runes.m_ascii.m_str;
        m_count                = 0;
    }

    runes_t runes_raw_writer_t::get_destination() const
    {
        runes_t str;
        str.m_type                = m_runes.m_type;
        str.m_runes.m_ascii.m_bos = m_runes.m_runes.m_ascii.m_bos;
        str.m_runes.m_ascii.m_eos = m_runes.m_runes.m_ascii.m_eos;
        str.m_runes.m_ascii.m_str = m_runes.m_runes.m_ascii.m_str;
        str.m_runes.m_ascii.m_end = m_runes.m_runes.m_ascii.m_end;
        return str;
    }

    runes_t runes_raw_writer_t::get_current() const
    {
        runes_t str;
        str.m_type                = m_runes.m_type;
        str.m_runes.m_ascii.m_bos = m_runes.m_runes.m_ascii.m_bos;
        str.m_runes.m_ascii.m_eos = m_runes.m_runes.m_ascii.m_eos;
        str.m_runes.m_ascii.m_str = m_runes.m_runes.m_ascii.m_str;
        str.m_runes.m_ascii.m_end = m_cursor.m_ptr.m_ascii;
        return str;
    }

    void runes_raw_writer_t::reset()
    {
        m_cursor.m_ptr.m_ascii = m_runes.m_runes.m_ascii.m_str;
        m_count                = 0;
    }

    s32  runes_raw_writer_t::count() const
    {
        return m_count;
    }

    static bool at_end(runes_t::ptr_t const& p, runes_t const& str)
    {
        return p.m_ptr.m_ascii >= str.m_runes.m_ascii.m_eos;
    }

    bool runes_raw_writer_t::write(uchar32 c)
    {
        if (!at_end(m_cursor, m_runes))
        {
            switch (m_runes.m_type)
            {
                case ascii::TYPE:
                    if (!at_end(m_cursor, m_runes))
                    {
                        m_cursor.m_ptr.m_ascii[0] = c;
                        m_cursor.m_ptr.m_ascii += 1;
                        m_count += 1;
                        return true;
                    }
                    break;
                case utf8::TYPE:
                    if (!at_end(m_cursor, m_runes))
                    {
                        utf::write(c, m_runes.m_runes.m_utf8, m_cursor.m_ptr.m_utf8);
                        m_count += 1;
                        return true;
                    }
                    break;
                case utf16::TYPE:
                    if (!at_end(m_cursor, m_runes))
                    {
                        utf::write(c, m_runes.m_runes.m_utf16, m_cursor.m_ptr.m_utf16);
                        m_count += 1;
                        return true;
                    }
                    break;
                case utf32::TYPE:
                    if (!at_end(m_cursor, m_runes))
                    {
                        m_cursor.m_ptr.m_utf32[0] = c;
                        m_cursor.m_ptr.m_utf32 += 1;
                        m_count += 1;
                        return true;
                    }
                    break;
            }
        }
        return false;
    }

    bool runes_raw_writer_t::write(crunes_t const& str)
    {
        if (!at_end(m_cursor, m_runes))
        {
            runes_reader_t reader(str);
            switch (m_runes.m_type)
            {
                case ascii::TYPE:
                    while (!reader.at_end() && !at_end(m_cursor, m_runes))
                    {
                        uchar32 c = reader.read();
                        //@note: use utf::write
                        m_cursor.m_ptr.m_ascii[0] = (ascii::rune)c;
                        m_cursor.m_ptr.m_ascii += 1;
                        m_count += 1;
                    }
                    return true;
                case utf8::TYPE:
                    while (!reader.at_end() && !at_end(m_cursor, m_runes))
                    {
                        uchar32 c = reader.read();
                        utf::write(c, m_runes.m_runes.m_utf8, m_cursor.m_ptr.m_utf8);
                        m_count += 1;
                    }
                    return true;
                case utf16::TYPE:
                    while (!reader.at_end() && !at_end(m_cursor, m_runes))
                    {
                        uchar32 c = reader.read();
                        utf::write(c, m_runes.m_runes.m_utf16, m_cursor.m_ptr.m_utf16);
                        m_count += 1;
                    }
                    return true;
                case utf32::TYPE:
                    while (!reader.at_end() && !at_end(m_cursor, m_runes))
                    {
                        uchar32 c = reader.read();
                        //@note: use utf::write
                        m_cursor.m_ptr.m_utf32[0] = c;
                        m_cursor.m_ptr.m_utf32 += 1;
                        m_count += 1;
                    }
                    return true;
            }
        }
        return false;
    }

    void runes_raw_writer_t::flush()
    {

    }

} // namespace xcore
