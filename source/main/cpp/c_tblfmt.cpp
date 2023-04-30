#include "cbase/c_tblfmt.h"
#include "cbase/c_runes.h"

namespace ncore
{
    namespace fmt
    {
        // static const uchar32 BorderStyleNone[]   = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
        // static const uchar32 BorderStyleSingle[] = {'┌', '┬', '┐', '├', '┼', '┤', '└', '┴', '┘', '─', '│', ' '};
        // static const uchar32 BorderStyleDouble[] = {'╔', '╦', '╗', '╠', '╬', '╣', '╚', '╩', '╝', '═', '║', ' '};
        static const uchar32 BorderStyleNone[]   = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
        static const uchar32 BorderStyleSingle[] = {0x250C, 0x252C, 0x2510, 0x251C, 0x253C, 0x2524, 0x2514, 0x2534, 0x2518, 0x2500, 0x2502, ' '};
        static const uchar32 BorderStyleDouble[] = {0x2554, 0x2566, 0x2557, 0x2560, 0x256C, 0x2563, 0x255A, 0x2569, 0x255D, 0x2550, 0x2551, ' '};

        void tbl_init(tbl_state_t& state)
        {
            for (s32 i = 0; i < state.columns_size_; ++i)
            {
                state.borders_[i].value = Border::LSR;
            }
        }

        void tbl_format(tbl_state_t& state, args_t const& args) {}

        void tbl_cell(s8 column, tbl_state_t& state, args_t const& args) {}

        static inline uchar32 GetBorder(Border style, s32 column_border)
        {
            if ((style.value & Border::LSR) == 0)
                return ' ';

            const uchar32* borderChars = BorderStyleNone;
            if (style.value & Border::Single)
                borderChars = BorderStyleSingle;
            else if (style.value & Border::Double)
                borderChars = BorderStyleDouble;

            s32 offset = 0;
            if (style.value & Border::Middle)
                offset += 3;
            else if (style.value & Border::Bottom)
                offset += 6;

            if (style.value & ((Border::LSR & column_border) == Border::LSR))
                return borderChars[offset + 1];
            if (style.value & ((Border::LS_ & column_border) == Border::LS_))
                return borderChars[offset + 2];
            if (style.value & ((Border::_SR & column_border) == Border::_SR))
                return borderChars[offset];
            if (style.value & ((Border::_S_ & column_border) == Border::_S_))
                return borderChars[9];
            return borderChars[10];
        }

        static inline s32 BorderFlip(s32 border) { return (border & 2) | ((border & 1) << 2) | ((border & 4) >> 2); }

        void tbl_line(tbl_state_t& state, Border const* column_border, s32 column_border_count, Border line_style)
        {
            state.row_len_ = 0;

            // clear the row
            for (s32 i = 0; i < state.row_size_; ++i)
                state.row_[i] = ' ';

            // put in the column_border
            s32 offset = 0;

            // if the given column_border is null, then use the state's column_border
            if (column_border == nullptr)
            {
                column_border       = state.borders_;
                column_border_count = state.columns_size_;
            }

            // clamp the
            if (column_border_count > state.columns_size_)
                column_border_count = state.columns_size_;

            // Think of it as how the separator is attached to the vertical bar
            // _SR means that we have a vertical line with a right side horizontal line
            for (s32 i = 0; i < column_border_count; ++i)
            {
                // this defines if the column has a left and right border
                // it also defines if it has a separator horizontal character
                const s32 tabl_cell_borders = state.borders_[i].value & 7;
                const s32 user_cell_borders = (column_border[i].value & 7) & ((i > 0) ? ((BorderFlip(column_border[i - 1].value & 7) & Border::L__) | Border::_SR) : (Border::LS_));
                const s32 cell_borders      = user_cell_borders & tabl_cell_borders;

                // first write the (left) border of this cell
                state.row_[offset++] = GetBorder(line_style, BorderFlip(cell_borders));

                if (cell_borders & Border::_S_)
                {
                    s32           width = state.widths_[i] + 2;
                    const uchar32 sep   = GetBorder(line_style, cell_borders & Border::_S_);
                    while (width-- > 0)
                        state.row_[offset++] = sep;
                }
                else
                {
                    offset += state.widths_[i] + 2;  // skip the width (content) plus one spacing character
                }
            }

            // Finally write the right border
            if (column_border_count == state.columns_size_)
            {
                const s32 column_sep = column_border[state.columns_size_ - 1].value & 7;
                state.row_[offset++] = GetBorder(line_style, BorderFlip(column_sep) & Border::LS_);
            }
            state.row_[offset++] = '\n';
            state.row_len_       = offset;

            // And end the row with string terminators
            for (s32 i = offset; i < state.row_size_; ++i)
                state.row_[i] = '\0';
        }

        void tbl_flags(tbl_state_t& state, Flags const* flags, s32 count)
        {
            for (s32 i = 0; i < count; ++i)
            {
                state.flags_[i] = flags[i];
            }
        }

        void tbl_row_to_utf8(tbl_state_t& state, utf8::prune str, utf8::pcrune end)
        {
            utf8::prune   iter = str;
            while (iter < end)
                *iter++ = 0;

            // convert the utf32 row string to utf8
            runes_t        utf_8(str, 0, end - str, end - str, utf8::TYPE);
            crunes_t       utf_32((utf32::pcrune)state.row_, 0, state.row_len_, state.row_len_);
            runes_writer_t writer(utf_8);
            writer.write(utf_32);
        }

        void tbl_row_to_utf16(tbl_state_t& state, utf16::prune str, utf16::pcrune end)
        {
            utf16::prune   iter = str;
            while (iter < end)
                *iter++ = 0;

            // convert the utf32 row string to utf16
            runes_t        utf_16(str, 0, end - str, end - str, utf16::TYPE);
            crunes_t       utf_32((utf32::pcrune)state.row_, 0, state.row_len_, state.row_len_);
            runes_writer_t writer(utf_16);
            writer.write(utf_32);
        }
    }  // namespace fmt
}  // namespace ncore
