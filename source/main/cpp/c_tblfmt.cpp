#include "cbase/c_tblfmt.h"

namespace ncore
{
    namespace fmt
    {
        static const char* const BorderStyleNone   = "            ";
        static const char* const BorderStyleSingle = "┌┬┐├┼┤└┴┘─│ ";
        static const char* const BorderStyleDouble = "╔╦╗╠╬╣╚╩╝═║ ";

        void tbl_init(tbl_state_t& state)
        {
            for (s32 i = 0; i < state.columns_size_; ++i)
            {
                state.borders_[i] = Border::LSR;
            }
            state.borders_[0] = Border::_SR;
            state.borders_[state.columns_size_ - 1] = Border::LS_;
        }

        void tbl_format(tbl_state_t& state, args_t const& args) {}

        void tbl_cell(s8 column, tbl_state_t& state, args_t const& args) {}

        static inline char GetBorder(Border style, u8 column_border)
        {
            if ((style.value & Border::LSR) == 0)
                return ' ';

            const char* borderChars = BorderStyleNone;
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
            return borderChars[10];
        }

        static inline u8 BorderFlip(u8 border) { return (border & 2) | ((border & 1) << 2) | ((border & 4) >> 2); }

        void tbl_line(tbl_state_t& state, Border::EBorder const* column_border, s32 column_border_count, Border line_style)
        {
            // clear the row
            for (s32 i = 0; i < state.row_size_; ++i)
                state.row_[i] = ' ';

            // put in the column_border
            s32 offset = 0;

            // clamp the
            if (column_border_count > state.columns_size_)
                column_border_count = state.columns_size_;

            // Think of it as how the separator is attached to the vertical bar
            // _SR means that we have a vertical line with a right side horizontal line
            for (s32 i = 0; i < column_border_count; ++i)
            {
                // this defines if the column has a left and right border
                // it also defines if it has a separator horizontal character
                const u8 tbl_left = state.borders_[i];
                const u8 cell_left = column_border[i] & ((i>0) ? (BorderFlip(column_border[i-1])) : (Border::LSR));

                // first write the (left) border
                state.row_[offset++] = GetBorder(line_style, BorderFlip(cell_left) & tbl_left);
                state.row_[offset++] = ' ';

                offset += state.widths_[i] + 1;
            }

            // Finally write the right border
            if (column_border_count == state.columns_size_)
            {
                const u8 border = Border::LS_;
                const u8 column_sep = column_border[state.columns_size_ - 1];
                state.row_[offset++] = GetBorder(line_style, BorderFlip(column_sep) & border);
            }
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
    }  // namespace fmt
}  // namespace ncore
