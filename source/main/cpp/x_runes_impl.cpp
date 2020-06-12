#ifndef XNAMESPACE_NAME
	#define XNAMESPACE_NAME utfxx
	#include "xbase/x_debug.h"
	#include "xbase/x_integer.h"
	#include "xbase/x_console.h"
	#include "xbase/x_va_list.h"
	#include "xbase/x_runes.h"
	#include "xbase/x_printf.h"
#else
namespace xcore
{
    namespace XNAMESPACE_NAME
    {
        runes crunes_to_runes(runes const& str, crunes const& sel)
        {
            runes r(str);
            r.m_str += (sel.m_str - str.m_str);
            r.m_end += (sel.m_end - str.m_end);
            return r;
        }

        runes  nothing_found(runes const& str) { return runes(str.m_str, str.m_str, str.m_str); }
        crunes nothing_found(crunes const& str) { return crunes(str.m_str, str.m_str); }

        s32 len(pcrune str, pcrune end)
        {
            end = endof(str, end);
            return s32(end - str);
        }

        prune endof(prune str, pcrune eos)
        {
            prune end = str;
            if (eos == nullptr)
            {
                while (*end != TERMINATOR)
                    end++;
            }
            else
            {
                while (*end != TERMINATOR && end < eos)
                    end++;
            }
            return end;
        }

        pcrune endof(pcrune str, pcrune eos)
        {
            pcrune end = str;
            if (eos == nullptr)
            {
                while (*end != TERMINATOR)
                    end++;
            }
            else
            {
                while (*end != TERMINATOR && end < eos)
                    end++;
            }
            return end;
        }

        crunes find(crunes const& _str, uchar32 _c, bool _casesensitive)
        {
            pcrune str = _str.m_str;
            while (str < _str.m_end)
            {
                pcrune  pos = str;
                uchar32 c   = *str++;

                bool const equal = _casesensitive ? is_equal(c, _c) : is_equalfold(c, _c);
                if (equal)
                {
                    return crunes(pos, pos + 1);
                }
            }
            return nothing_found(_str);
        }

        runes find(runes const& _str, uchar32 _c, bool _casesensitive)
        {
            crunes str(_str);
            crunes csel = find(str, _c, _casesensitive);
            return crunes_to_runes(_str, csel);
        }

        crunes findLast(crunes const& _str, uchar32 _c, bool _casesensitive)
        {
            pcrune str = _str.m_end;
            while (str > _str.m_str)
            {
                pcrune  pos = str;
                uchar32 c   = *--str;

                bool const equal = _casesensitive ? is_equal(c, _c) : is_equalfold(c, _c);
                if (equal)
                {
                    return crunes(pos, pos + 1);
                }
            }
            return nothing_found(_str);
        }

        runes findLast(runes const& _str, uchar32 _c, bool _casesensitive)
        {
            crunes str(_str);
            crunes csel = findLast(str, _c, _casesensitive);
            return crunes_to_runes(_str, csel);
        }

        bool contains(runes const& _str, uchar32 _c, bool _casesensitive = true)
        {
            runes pos = find(_str, _c, _casesensitive);
            return !pos.is_empty();
        }

        bool contains(crunes const& _str, uchar32 _c, bool _casesensitive = true)
        {
            crunes pos = find(_str, _c, _casesensitive);
            return !pos.is_empty();
        }

        crunes find(crunes const& _str, crunes const& _find, bool _casesensitive)
        {
            if (_str.is_empty())
                return nothing_found(_str);
            if (_find.is_empty())
                return nothing_found(_str);
            if (_find.size() > _str.size())
                return nothing_found(_str);

            pcrune str_cursor = _str.m_str;
            pcrune str_end    = _str.m_end - _find.size();

            while (str_cursor <= str_end)
            {
                crunes    str_sel(str_cursor, str_cursor + _find.size());
                s32 const cmp = compare(str_sel, _find, _casesensitive);
                if (cmp == 0)
                {
                    return str_sel;
                }
                str_cursor += 1;
            }
            return nothing_found(_str);
        }

        runes find(runes const& _str, crunes const& _find, bool _casesensitive)
        {
            crunes str(_str);
            crunes csel = find(str, _find, _casesensitive);
            if (!csel.is_empty())
            {
                runes sel = crunes_to_runes(_str, csel);
                return sel;
            }
            return nothing_found(_str);
        }

        crunes findLast(crunes const& _str, crunes const& _find, bool _casesensitive)
        {
            if (_str.is_empty())
                return nothing_found(_str);
            if (_find.is_empty())
                return nothing_found(_str);
            if (_find.size() > _str.size())
                return nothing_found(_str);

            pcrune str_cursor = _str.m_end - _find.size();
            while (str_cursor >= _str.m_str)
            {
                crunes    str_sel(str_cursor, str_cursor + _find.size());
                s32 const cmp = compare(str_sel, _find, _casesensitive);
                if (cmp == 0)
                {
                    return str_sel;
                }
                str_cursor -= 1;
            }
            return nothing_found(_str);
        }

        runes findLast(runes const& _str, crunes const& _find, bool _casesensitive)
        {
            crunes str(_str);
            crunes csel = findLast(str, _find, _casesensitive);
            if (!csel.is_empty())
            {
                runes sel = crunes_to_runes(_str, csel);
                return sel;
            }
            return nothing_found(_str);
        }

        crunes findOneOf(crunes const& _str, crunes const& _any, bool _casesensitive)
        {
            pcrune cursor = _str.m_str;
            while (cursor < _str.m_end)
            {
                pcrune  pos = cursor;
                uchar32 c   = *cursor++;

                // See if this char can be found in the 'set'
                crunes found = find(_any, c, _casesensitive);
                if (!found.is_empty())
                    return crunes(pos, pos + 1);
            }
            return nothing_found(_str);
        }

        runes findOneOf(runes const& _str, crunes const& _any, bool _casesensitive)
        {
            crunes str(_str);
            crunes csel = findOneOf(str, _any, _casesensitive);
            if (!csel.is_empty())
            {
                runes sel = crunes_to_runes(_str, csel);
                return sel;
            }
            return nothing_found(_str);
        }

        // ------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------
        crunes findSelectUntil(const crunes& _str, uchar32 _c, bool _casesensitive)
        {
            crunez<4> _find(_c);
            return findSelectUntil(_str, _find, _casesensitive);
        }

        crunes findSelectUntil(const crunes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes sel = find(_str, _find, _casesensitive);
            if (sel.is_empty())
                return nothing_found(_str);
            return crunes(_str.m_str, sel.m_str);
        }

        crunes findLastSelectUntil(const crunes& _str, uchar32 _c, bool _casesensitive)
        {
            crunez<4> _find(_c);
            return findLastSelectUntil(_str, _find, _casesensitive);
        }

        crunes findLastSelectUntil(const crunes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes sel = findLast(_str, _find, _casesensitive);
            if (sel.is_empty())
                return nothing_found(_str);
            return crunes(_str.m_str, sel.m_str);
        }

        crunes findSelectUntilIncluded(const crunes& _str, uchar32 _c, bool _casesensitive)
        {
            crunez<4> _find(_c);
            return findSelectUntilIncluded(_str, _find, _casesensitive);
        }

        crunes findSelectUntilIncluded(const crunes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes sel = find(_str, _find, _casesensitive);
            if (sel.is_empty())
                return nothing_found(_str);
            return crunes(_str.m_str, sel.m_end);
        }

        crunes findLastSelectUntilIncluded(const crunes& _str, uchar32 _c, bool _casesensitive)
        {
            crunez<4> _find(_c);
            return findLastSelectUntilIncluded(_str, _find, _casesensitive);
        }

        crunes findLastSelectUntilIncluded(const crunes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes sel = findLast(_str, _find, _casesensitive);
            if (sel.is_empty())
                return nothing_found(_str);
            return crunes(_str.m_str, sel.m_end);
        }

        crunes selectAfterSelection(const crunes& _str, const crunes& _sel)
        {
            if (!_sel.is_empty())
            {
                crunes sel;
                sel.m_str = _sel.m_end;
                sel.m_end = _str.m_end;
                sel.m_cur = _sel.m_end;
                return sel;
            }
            return nothing_found(_str);
        }

        crunes findSelectAfter(const crunes& _str, uchar32 _find, bool _casesensitive)
        {
            crunes sel = findSelectUntilIncluded(_str, _find, _casesensitive);
            return selectAfterSelection(_str, sel);
        }

        crunes findSelectAfter(const crunes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes sel = findSelectUntilIncluded(_str, _find, _casesensitive);
            return selectAfterSelection(_str, sel);
        }

        crunes findLastSelectAfter(const crunes& _str, uchar32 _find, bool _casesensitive)
        {
            crunes sel = findLastSelectUntilIncluded(_str, _find, _casesensitive);
            return selectAfterSelection(_str, sel);
        }

        crunes findLastSelectAfter(const crunes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes sel = findLastSelectUntilIncluded(_str, _find, _casesensitive);
            return selectAfterSelection(_str, sel);
        }

        crunes selectBetween(const crunes& _str, uchar32 _left, uchar32 _right)
        {
            crunes l = find(_str, _left);
            if (!l.is_empty())
            {
                pcrune pstr = l.m_end;
                if (pstr < _str.m_end)
                {
                    crunes str(pstr, _str.m_end);
                    crunes r = find(str, _right);
                    if (!r.is_empty())
                    {
                        return crunes(pstr, r.m_str);
                    }
                }
            }
            return nothing_found(_str);
        }

        crunes selectNextBetween(const crunes& _str, const crunes& _selection, uchar32 _left, uchar32 _right)
        {
            crunes str;
            str.m_str  = _selection.m_str;
            str.m_end  = _str.m_end;
            str.m_cur  = _selection.m_str;
            crunes sel = selectBetween(str, _left, _right);
            return sel;
        }

        crunes selectBetweenLast(const crunes& _str, uchar32 _left, uchar32 _right)
        {
            crunes r = findLast(_str, _right);
            if (!r.is_empty())
            {
                crunes s(_str.m_str, r.m_str);
                crunes l = findLast(s, _left);
                if (!l.is_empty())
                {
                    return crunes(l.m_end, r.m_str);
                }
            }
            return nothing_found(_str);
        }

        crunes selectPreviousBetween(const crunes& _str, const crunes& _selection, uchar32 _left, uchar32 _right)
        {
            crunes str;
            str.m_str  = _str.m_str;
            str.m_end  = _selection.m_end;
            str.m_cur  = _str.m_str;
            crunes sel = selectBetweenLast(str, _left, _right);
            return sel;
        }

        crunes selectUntilEndExcludeSelection(const crunes& _str, const crunes& _selection)
        {
            crunes sel;
            sel.m_str = _selection.m_end;
            sel.m_end = _str.m_end;
            sel.m_cur = _selection.m_end;
            return sel;
        }

        crunes selectUntilEndIncludeSelection(const crunes& _str, const crunes& _selection)
        {
            crunes sel;
            sel.m_str = _selection.m_str;
            sel.m_end = _str.m_end;
            sel.m_cur = _selection.m_str;
            return sel;
        }

        crunes selectOverlap(const crunes& lstr, const crunes& rstr)
        {
			// Find the last character of 'lstr' in 'rstr'
            // When found start to expand the string in 'lstr' to the left
            // while keep checking if 'rstr' also contains that character
			
			ASSERT(false);	// TODO

            return lstr;
        }

        s32 compare(crunes const& _lstr, crunes const& _rstr, bool _casesensitive)
        {
            if (_lstr.size() < _rstr.size())
                return -1;
            if (_lstr.size() > _rstr.size())
                return 1;
            if (_lstr.size() == 0)
				return 0;

            pcrune lstr = _lstr.m_str;
            pcrune rstr = _rstr.m_str;

			uchar32 lc, rc;
            if (_casesensitive)
            {
				do
				{
					lc = *lstr++;
					rc = *rstr++;
				} while (lc == rc && lstr < _lstr.m_end);
			}
			else
			{
				do
				{
                    lc = utf32::to_lower(*lstr++);
                    rc = utf32::to_lower(*rstr++);
				} while (lc == rc && lstr < _lstr.m_end);
			}
			if (lc < rc) return -1;
			else if (lc > rc) return 1;
            return 0;
        }

        // ------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------

        runes findSelectUntil(const runes& _str, uchar32 _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findSelectUntil(const runes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findLastSelectUntil(const runes& _str, uchar32 _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findLastSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findLastSelectUntil(const runes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findLastSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findSelectUntilIncluded(const runes& _str, uchar32 _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findSelectUntilIncluded(const runes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findLastSelectUntilIncluded(const runes& _str, uchar32 _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findLastSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findLastSelectUntilIncluded(const runes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findLastSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes selectAfterSelection(const runes& _str, const runes& _sel)
        {
            crunes cstr(_str);
            crunes found = selectAfterSelection(cstr, _sel);
            return crunes_to_runes(_str, found);
        }

        runes findSelectAfter(const runes& _str, uchar32 _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findSelectAfter(const runes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findLastSelectAfter(const runes& _str, uchar32 _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findLastSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes findLastSelectAfter(const runes& _str, const crunes& _find, bool _casesensitive)
        {
            crunes cstr(_str);
            crunes found = findLastSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes selectBetween(const runes& _str, uchar32 _left, uchar32 _right)
        {
            crunes cstr(_str);
            crunes found = selectBetween(cstr, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes selectNextBetween(const runes& _str, const runes& _selection, uchar32 _left, uchar32 _right)
        {
            crunes cstr(_str);
            crunes found = selectNextBetween(cstr, _selection, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes selectBetweenLast(const runes& _str, uchar32 _left, uchar32 _right)
        {
            crunes cstr(_str);
            crunes found = selectBetweenLast(cstr, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes selectPreviousBetween(const runes& _str, const runes& _selection, uchar32 _left, uchar32 _right)
        {
            crunes cstr(_str);
            crunes found = selectPreviousBetween(cstr, _selection, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes selectUntilEndExcludeSelection(const runes& _str, const runes& _selection)
        {
            crunes cstr(_str);
            crunes found = selectUntilEndExcludeSelection(cstr, _selection);
            return crunes_to_runes(_str, found);
        }

        runes selectUntilEndIncludeSelection(const runes& _str, const runes& _selection)
        {
            crunes cstr(_str);
            crunes found = selectUntilEndIncludeSelection(cstr, _selection);
            return crunes_to_runes(_str, found);
        }

        runes selectOverlap(const runes& _str, const runes& _rstr)
        {
            crunes cstr(_str);
            crunes found = selectOverlap(cstr, _rstr);
            return crunes_to_runes(_str, found);
        }

        s32 compare(runes const& _lstr, runes const& _rstr, bool _casesensitive)
        {
            crunes clstr(_lstr);
            crunes crstr(_rstr);
            return compare(clstr, crstr, _casesensitive);
        }

        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------

        crunes parse(crunes const& _str, bool& value)
        {
            crunes str          = _str;
            rune   format_str[] = {'%', 'b', TERMINATOR};
            crunes format(format_str, format_str + 2);
            sscanf(str, format, x_va_r(&value));
            return str;
        }

        crunes parse(crunes const& _str, s32& value, s32 base)
        {
            crunes str          = _str;
            rune   format_str[] = {'%', 'd', TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes format(format_str, format_str + 2);
            sscanf(str, format, x_va_r(&value));
            return str;
        }

        crunes parse(crunes const& _str, u32& value, s32 base)
        {
            crunes str          = _str;
            rune   format_str[] = {'%', 'd', TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes format(format_str, format_str + 2);
            sscanf(str, format, x_va_r(&value));
            return str;
        }

        crunes parse(crunes const& _str, s64& value, s32 base)
        {
            crunes str          = _str;
            rune   format_str[] = {'%', 'd', TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes format(format_str, format_str + 2);
            sscanf(str, format, x_va_r(&value));
            return str;
        }

        crunes parse(crunes const& _str, u64& value, s32 base)
        {
            crunes str          = _str;
            rune   format_str[] = {'%', 'd', TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes format(format_str, format_str + 2);
            sscanf(str, format, x_va_r(&value));
            return str;
        }

        crunes parse(crunes const& _str, f32& value)
        {
            crunes str          = _str;
            rune   format_str[] = {'%', 'f', TERMINATOR};
            crunes format(format_str, format_str + 2);
            sscanf(str, format, x_va_r(&value));
            return str;
        }

        crunes parse(crunes const& _str, f64& value)
        {
            crunes str          = _str;
            rune   format_str[] = {'%', 'f', TERMINATOR};
            crunes format(format_str, format_str + 2);
            sscanf(str, format, x_va_r(&value));
            return str;
        }

        //------------------------------------------------------------------------------
        bool is_decimal(crunes const& _str)
        {
            pcrune str = _str.m_str;
            while (str < _str.m_end)
            {
                uchar32 c = *str;
                if (c != '-')
                    break;
                str++;
            }
            while (str < _str.m_end)
            {
                uchar32 c = *str++;
                if (c < '0' || c > '9')
                    return false;
            }
            return true;
        }

        //------------------------------------------------------------------------------
        bool is_hexadecimal(crunes const& _str, bool with_prefix)
        {
            pcrune  str = _str.m_str;
            uchar32 p;
            s32     pos = 0;
            while (str < _str.m_end)
            {
                uchar32 c = *str++;
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
        bool is_float(crunes const& _str)
        {
            rune   f32chars_str[] = {'E', 'e', '.', '#', 'Q', 'N', 'A', 'B', 'I', 'F', 0};
            crunes f32chars(f32chars_str, endof(f32chars_str, NULL));

            pcrune str = _str.m_str;
            while (str < _str.m_end)
            {
                uchar32 c = *str++;
                if (c >= '0' && c <= '9')
                    continue;
                if (!contains(f32chars, c))
                    return false;
            }
            return true;
        }

        //------------------------------------------------------------------------------
        bool is_GUID(crunes const& _str)
        {
            rune   f32chars_str[] = {'E', 'e', '.', '#', 'Q', 'N', 'A', 'B', 'I', 'F', 0};
            crunes f32charslen(f32chars_str, endof(f32chars_str, NULL));

            pcrune str = _str.m_str;

            // Does it have only 0-9, a-f, A-F characters
            s32 n = 0;
            s32 l = 0;
            while (str < _str.m_end)
            {
                uchar32 c = *str++;
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

        void to_string(runes& str, s32 val, s32 base)
        {
            rune format_str[] = {'%', 'd', TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes format(format_str, format_str + 2);
            sprintf(str, format, x_va(val));
        }

        void to_string(runes& str, u32 val, s32 base)
        {
            rune format_str[] = {'%', 'u', TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'u'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes format(format_str, format_str + 2);
            sprintf(str, format, x_va(val));
        }

        // *cFHbRXV[N]WWWR6FETY+

        void to_string(runes& str, s64 val, s32 base)
        {
            rune format_str[] = {'%', 'd', TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes format(format_str, format_str + 2);
            sprintf(str, format, x_va(val));
        }

        void to_string(runes& str, u64 val, s32 base)
        {
            rune format_str[] = {'%', 'd', TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes format(format_str, format_str + 2);
            sprintf(str, format, x_va(val));
        }

        void to_string(runes& str, f32 val, s32 numFractionalDigits)
        {
            rune format_str[] = {'%', '.', '0', '2', 'f', TERMINATOR};
            if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
            {
                format_str[2] = '0' + numFractionalDigits / 10;
                format_str[3] = '0' + numFractionalDigits % 10;
            }
            crunes format(format_str, format_str + 5);
            sprintf(str, format, x_va(val));
        }

        void to_string(runes& str, f64 val, s32 numFractionalDigits)
        {
            rune format_str[] = {'%', '.', '0', '2', 'f', TERMINATOR};
            if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
            {
                format_str[2] = '0' + numFractionalDigits / 10;
                format_str[3] = '0' + numFractionalDigits % 10;
            }
            crunes format(format_str, format_str + 5);
            sprintf(str, format, x_va(val));
        }

        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------

        bool is_upper(crunes const& _str)
        {
            pcrune str = _str.m_str;
            while (str < _str.m_end)
            {
                uchar32 c = *str++;
                if (is_lower(c))
                    return false;
            }
            return true;
        }

        bool is_lower(crunes const& _str)
        {
            pcrune str = _str.m_str;
            while (str < _str.m_end)
            {
                uchar32 c = *str++;
                if (is_upper(c))
                    return false;
            }
            return true;
        }

        bool is_capitalized(crunes const& _str)
        {
            pcrune str = _str.m_str;
            bool   b   = true;
            while (str < _str.m_end)
            {
                uchar32 c = *str++;
                if (is_upper(c) != b)
                    return false;
                b = (c == ' ');
            }
            return true;
        }

		bool    is_quoted(crunes const& str, uchar32 quote) 
		{
			return is_delimited(str, quote, quote); 
		}

        bool is_delimited(crunes const& _str, uchar32 delimit_left, uchar32 delimit_right)
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
        void to_upper(runes& _str)
        {
            prune str = _str.m_str;
            while (str < _str.m_end)
            {
                uchar32 c = *str;
                if ((c >= 'a') && (c <= 'z'))
                {
                    c += ('A' - 'a');
                    *str++ = c;
                }
                else
                {
                    str++;
                }
            }
        }

        /**
         * Assumption: Letters A-Z and a-z are contiguous in the character set.
         * This is true for ASCII and UniCode.  (Not so for EBCDIC!)
         */
        void to_lower(runes& _str)
        {
            prune str = _str.m_str;
            while (str < _str.m_end)
            {
                uchar32 c = *str;
                if ((c >= 'A') && (c <= 'Z'))
                {
                    c += ('a' - 'A');
                    *str++ = c;
                }
                else
                {
                    str++;
                }
            }
        }

        // ------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------

        bool starts_with(crunes const& _str, uchar32 start_char)
        {
            pcrune str = _str.m_str;
            if (str < _str.m_end)
                return start_char == *str;
            return false;
        }

        bool starts_with(crunes const& _str, crunes const& _start)
        {
            if (_start.size() > _str.size())
                return false;

            pcrune lstr = _str.m_str;
            pcrune sstr = _start.m_str;
            while (lstr < _str.m_end && sstr < _start.m_end)
            {
                uchar32 lc = *lstr++;
                uchar32 rc = *sstr++;
                if (lc != rc)
                    return false;
            }
            return lstr <= _str.m_end && sstr == _start.m_end;
        }

        bool ends_with(crunes const& _str, uchar32 end_char)
        {
            if (_str.m_end > _str.m_str)
            {
                pcrune str = _str.m_end - 1;
                return end_char == *str;
            }
            return false;
        }

        bool ends_with(crunes const& _str, crunes const& _end)
        {
            if (_end.size() > _str.size())
                return false;
            pcrune lstr = _str.m_end - _end.size();
            pcrune estr = _end.m_str;
            while (lstr < _str.m_end && estr < _end.m_end)
            {
                uchar32 lc = *lstr++;
                uchar32 rc = *estr++;
                if (lc != rc)
                    return false;
            }
            return lstr == _str.m_end && estr == _end.m_end;
        }

        uchar32 first_char(crunes const& _str)
        {
            pcrune str = _str.m_str;
            if (str < _str.m_end)
                return *str;
            return TERMINATOR;
        }

        uchar32 last_char(crunes const& _str)
        {
            uchar32 c = TERMINATOR;
            if (_str.m_str < _str.m_end)
            {
                pcrune str = _str.m_end - 1;
                return *str;
            }
            return c;
        }

        // ------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------

        void removeSelection(runes& str, crunes const& selection)
        {
            prune  dst = (prune)selection.m_str;
            prune  src = (prune)selection.m_end;
            pcrune end = str.m_end;
            while (src < end)
            {
                *dst++ = *src++;
            }
            *dst      = TERMINATOR;
            str.m_end = dst;
        }

        void keepOnlySelection(runes& str, crunes const& keep)
        {
            prune  dst = (prune)str.m_str;
            prune  src = (prune)keep.m_str;
            pcrune end = keep.m_end;
            while (src < end)
            {
                *dst++ = *src++;
            }
            *dst      = TERMINATOR;
            str.m_end = dst;
        }

        void replaceSelection(runes& _str, crunes const& _selection, crunes const& replace)
        {
            runes str = crunes_to_runes(_str, _selection);

            // The logic here is based on memory copy, we do not consider characters
            s32 const selected_len = (s32)((xbyte const*)str.m_end - (xbyte const*)str.m_str);
            s32 const replace_len  = (s32)((xbyte const*)replace.m_end - (xbyte const*)replace.m_str);

            prune end = NULL;
            if (selected_len < replace_len)
            {
                // Move, expanding
                s32 move_len = replace_len - selected_len;
                if (move_len > ((xbyte const*)str.m_eos - (xbyte*)str.m_end))
                    move_len = (s32)(((xbyte const*)str.m_eos - (xbyte*)str.m_end));

                // No matter what, push out anything at the end!
                xbyte* dst = (xbyte*)((xbyte*)str.m_end + ((xbyte const*)str.m_eos - (xbyte*)str.m_end) - 1);
                xbyte* src = (xbyte*)((xbyte*)dst - move_len);
                while (dst > (xbyte*)str.m_end)
                    *dst-- = *src--;

                end = (prune)((xbyte*)_str.m_end + move_len); // Update str_end
            }
            else if (selected_len > replace_len)
            {
                // Move, shrinking
                s32    move_len = selected_len - replace_len;
                xbyte* dst      = (xbyte*)((xbyte*)str.m_end - move_len);
                xbyte* src      = (xbyte*)(str.m_end);
                while (src < (xbyte const*)str.m_eos)
                    *dst++ = *src++;

                end = (prune)((xbyte*)_str.m_end - move_len); // Update str_end
            }
            else
            {
                end = (prune)(_str.m_end);
            }

            // Replace
            xbyte const* src     = (xbyte const*)replace.m_str;
            xbyte const* src_end = (xbyte const*)replace.m_str + replace_len;
            xbyte*       dst     = (xbyte*)str.m_str;
            while (src < src_end)
                *dst++ = *src++;

            _str.m_end = end;
        }

        static inline bool isIn(runes const& str, crunes const& selection)
        {
            return (selection.m_str >= str.m_str && selection.m_str < str.m_end && selection.m_end <= str.m_end);
        }

        crunes relocate(runes const& from, runes const& to, crunes const& selection)
        {
            if (isIn(from, selection))
            {
                crunes nselection(to);
                s32    stroffset = (s32)(selection.m_str - from.m_str);
                s32    endoffset = (s32)(selection.m_end - from.m_str);
                nselection.m_end = nselection.m_str + endoffset;
                nselection.m_str = nselection.m_str + stroffset;
                return nselection;
            }
            return crunes();
        }

        void replaceSelection(runes& str, crunes const& selection, crunes const& replace, alloc* allocator, s32 size_alignment)
        {
            s64 const diff = replace.size() - selection.size();
            if (diff > 0)
            {
                // Do we need to allocate additional space for @str ?
                // This will invalidate @selection
                if ((str.size() + diff) > str.cap())
                {
                    runes nstr = allocator->allocate(0, (str.size() + diff + size_alignment) & ~(size_alignment - 1));
                    copy(str, nstr);

                    // Relocate selection
                    crunes nselection = relocate(str, nstr, selection);

                    // Deallocate original str
                    allocator->deallocate(str);
                    str = nstr;
                }
            }
            replaceSelection(str, selection, replace);
        }

        void findReplace(runes& _str, uchar32 _find, uchar32 _replace, bool _casesensitive)
        {
            runez<3> find;
            utf::write(_find, find.m_end, find.m_eos);
            utf::write(TERMINATOR, find.m_end, find.m_eos);

            runez<3> replace;
            utf::write(_replace, replace.m_end, replace.m_eos);
            utf::write(TERMINATOR, replace.m_end, replace.m_eos);

            findReplace(_str, find, replace, _casesensitive);
        }

        void findReplace(runes& _str, crunes const& _find, crunes const& _replace, bool _casesensitive)
        {
            // Find
            runes selected = find(_str, _find, _casesensitive);
            if (selected.is_empty())
                return;
            replaceSelection(_str, selected, _replace);
        }

        void insert(runes& str, crunes const& insert) {}
        void insert(runes& str, crunes const& insert, alloc* allocator, s32 size_alignment) {}
        void insert(runes& str, crunes const& selection, crunes const& insert) {}
        void insert(runes& str, crunes const& selection, crunes const& insert, alloc* allocator, s32 size_alignment) {}

        void trim(runes& str)
        {
            rune   charseta[] = {' ', '\t', TERMINATOR};
            crunes charset(charseta, &charseta[2]);
            trimLeft(str, charseta);
            trimRight(str, charseta);
        }

        void trimLeft(runes& str)
        {
            rune   charseta[] = {' ', '\t', TERMINATOR};
            crunes charset(charseta, &charseta[2]);
            trimLeft(str, charseta);
        }

        void trimRight(runes& str)
        {
            rune   charseta[] = {' ', '\t', TERMINATOR};
            crunes charset(charseta, &charseta[2]);
            trimRight(str, charseta);
        }

        void trim(runes& str, rune _c)
        {
            rune   charseta[] = {_c, TERMINATOR};
            crunes charset(charseta, &charseta[1]);
            trimLeft(str, charseta);
            trimRight(str, charseta);
        }

        void trimLeft(runes& str, rune _c)
        {
            rune charset[] = {_c, TERMINATOR};
            trimLeft(str, charset);
        }

        void trimRight(runes& str, rune _c)
        {
            rune charset[2] = {_c, TERMINATOR};
            trimLeft(str, charset);
        }

        void trim(runes& str, crunes const& _charset)
        {
            trimLeft(str, _charset);
            trimRight(str, _charset);
        }

        void trimLeft(runes& str, crunes const& _charset)
        {
            bool trim = true;
            while (str.m_str < str.m_end && trim)
            {
                pcrune charptr = _charset.m_str;
                trim           = false;
                while (charptr < _charset.m_end)
                {
                    rune c = charptr[0];
                    if (str.m_str[0] == c)
                    {
                        trim = true;
                        str.m_str++;
                        break;
                    }
                    charptr += 1;
                }
            }
        }

        void trimRight(runes& str, crunes const& _charset)
        {
            bool trim = true;
            while (str.m_end > str.m_str && trim)
            {
                pcrune charptr = _charset.m_str;
                trim           = false;
                while (charptr < _charset.m_end)
                {
                    rune c = charptr[0];
                    if (str.m_end[-1] == c)
                    {
                        trim = true;
                        str.m_end -= 1;
                        break;
                    }
                    charptr += 1;
                }
            }
        }

        void trimQuotes(runes& str)
        {
            rune   charseta[] = {'\'', '"', TERMINATOR};
            crunes charset(charseta, &charseta[2]);
            trim(str, charseta);
        }

        void trimQuotes(runes& str, rune quote)
        {
            rune   charseta[] = {quote, TERMINATOR};
            crunes charset(charseta, &charseta[1]);
            trim(str, charseta);
        }

        void trimDelimiters(runes& str, rune _left, rune _right)
        {
            trimLeft(str, _left);
            trimRight(str, _right);
        }



        void trim(crunes& str)
        {
            rune   charseta[] = {' ', '\t', TERMINATOR};
            crunes charset(charseta, &charseta[2]);
            trimLeft(str, charseta);
            trimRight(str, charseta);
        }

        void trimLeft(crunes& str)
        {
            rune   charseta[] = {' ', '\t', TERMINATOR};
            crunes charset(charseta, &charseta[2]);
            trimLeft(str, charseta);
        }

        void trimRight(crunes& str)
        {
            rune   charseta[] = {' ', '\t', TERMINATOR};
            crunes charset(charseta, &charseta[2]);
            trimRight(str, charseta);
        }

        void trim(crunes& str, rune _c)
        {
            rune   charseta[] = {_c, TERMINATOR};
            crunes charset(charseta, &charseta[1]);
            trimLeft(str, charseta);
            trimRight(str, charseta);
        }

        void trimLeft(crunes& str, rune _c)
        {
            rune charset[] = {_c, TERMINATOR};
            trimLeft(str, charset);
        }

        void trimRight(crunes& str, rune _c)
        {
            rune charset[2] = {_c, TERMINATOR};
            trimLeft(str, charset);
        }

        void trim(crunes& str, crunes const& _charset)
        {
            trimLeft(str, _charset);
            trimRight(str, _charset);
        }

        void trimLeft(crunes& str, crunes const& _charset)
        {
            bool trim = true;
            while (str.m_str < str.m_end && trim)
            {
                pcrune charptr = _charset.m_str;
                trim           = false;
                while (charptr < _charset.m_end)
                {
                    rune c = charptr[0];
                    if (str.m_str[0] == c)
                    {
                        trim = true;
                        str.m_str++;
                        break;
                    }
                    charptr += 1;
                }
            }
        }

        void trimRight(crunes& str, crunes const& _charset)
        {
            bool trim = true;
            while (str.m_end > str.m_str && trim)
            {
                pcrune charptr = _charset.m_str;
                trim           = false;
                while (charptr < _charset.m_end)
                {
                    rune c = charptr[0];
                    if (str.m_end[-1] == c)
                    {
                        trim = true;
                        str.m_end -= 1;
                        break;
                    }
                    charptr += 1;
                }
            }
        }

        void trimQuotes(crunes& str)
        {
            rune   charseta[] = {'\'', '"', TERMINATOR};
            crunes charset(charseta, &charseta[2]);
            trim(str, charseta);
        }

        void trimQuotes(crunes& str, rune quote)
        {
            rune   charseta[] = {quote, TERMINATOR};
            crunes charset(charseta, &charseta[1]);
            trim(str, charseta);
        }

        void trimDelimiters(crunes& str, rune _left, rune _right)
        {
            trimLeft(str, _left);
            trimRight(str, _right);
        }



        void resize(runes& str, s32 cap, alloc* allocator, s32 size_alignment)
        {
            runes nstr = allocator->allocate(0, cap);
			if (str.is_valid())
			{
				copy(str, nstr);
				allocator->deallocate(str);
			}
            str = nstr;
        }

        void copy(crunes const& src, runes& dst)
        {
            dst.m_end = dst.m_str;
			concatenate(dst, src);
        }

        void concatenate(runes& str, crunes const& con)
        {
            pcrune src = con.m_str;
            prune& dst = str.m_end;
            while (dst < str.m_eos)
            {
                if (src == con.m_end)
                {
                    *dst = TERMINATOR;
                    break;
                }
                *dst++ = *src++;
            }
        }

        void copy(const crunes& src, runes& dst, alloc* allocator, s32 size_alignment)
		{
            dst.m_end = dst.m_str;
			concatenate(dst, src, allocator, size_alignment);
		}

        void concatenate(runes& str, const crunes& concat, alloc* allocator, s32 size_alignment)
        {
            s32 const required = str.size() + concat.size();
            if (required > str.cap())
            {
                resize(str, required, allocator, size_alignment);
            }
            concatenate(str, concat);
        }

        void concatenate(runes& str, const crunes& concat1, const crunes& concat2, alloc* allocator, s32 size_alignment)
        {
            s32 const required = str.size() + concat1.size() + concat2.size();
            if (required > str.cap())
            {
                resize(str, required, allocator, size_alignment);
            }
            concatenate(str, concat1);
            concatenate(str, concat2);
        }
    }

} // namespace xcore

#endif