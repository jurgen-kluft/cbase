#ifndef __CBASE_TABLE_FMT_H__
#define __CBASE_TABLE_FMT_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif
#include "cbase/c_debug.h"
#include "cbase/c_runes.h"
#include "cbase/c_strfmt.h"

namespace ncore
{
    namespace fmt
    {
        struct table_t
        {
            static const u8 ___ = 0x0;
            static const u8 __R = 0x1;
            static const u8 _S_ = 0x2;
            static const u8 _SR = 0x3;
            static const u8 L__ = 0x4;
            static const u8 L_R = 0x5;
            static const u8 LS_ = 0x6;
            static const u8 LSR = 0x7;

            typedef s8         Color;
            static const Color Black   = 0;
            static const Color Red     = 1;
            static const Color Green   = 2;
            static const Color Yellow  = 3;
            static const Color Blue    = 4;
            static const Color Magenta = 5;
            static const Color Cyan    = 6;
            static const Color White   = 7;

            template <uint_t N>
            table_t(state_t (&columns)[N], char* row, uint_t row_size)
                : columns_(columns)
                , row_(row)
                , row_size_(row_size)
            {
            }

            void cell_color(s8 column, Color color)
            {
                // Take the argument and format it into the row buffer using
                // the column state information.
            }

            template <typename... Args> void header_values(Args&&... args)
            {
                const u8            types[]  = {typed<Args>::value...};
                const u64           values[] = {arg_t<Args>::encode(args)...};
                const format_func_t funcs[]  = {arg_t<Args>::formatter()...};

                // First we need to format the top border of the table and
                // print it to the row buffer.

                // Take each argument and format it into the row buffer using
                // the column state information.
                // Also take the left / right border style into account.

                // Then we need to format the bottom border of the header and
                // print it to the row buffer.
            }

            template <typename... Args> void row_values(Args&&... args)
            {
                const u8            types[]  = {typed<Args>::value...};
                const u64           values[] = {arg_t<Args>::encode(args)...};
                const format_func_t funcs[]  = {arg_t<Args>::formatter()...};

                // Take each argument and format it into the row buffer using
                // the column state information.
                // Also take the left / right border style into account.
            }

            template <typename T> void cell_value(s8 column, T const& arg)
            {
                const u8            types[]  = {typed<T>::value};
                const u64           values[] = {arg_t<T>::encode(arg)};
                const format_func_t funcs[]  = {arg_t<T>::formatter()};

                // Take the argument and format it into the row buffer using
                // the column state information.
            }

            void print()
            {
                // print where ?
            }

            template <typename... Args> void separator_line(Args&&... args)
            {
                const u8 borders[] = { arg... };

                // print a separator line

            }

            void final()
            {
                // Print the bottom border of the table.
            }

            static const char* const BorderStyleNone[];
            static const char* const BorderStyleSingle[];
            static const char* const BorderStyleDouble[];

            state_t* columns_;
            uint_t   column_count_;
            char*    row_;
            uint_t   row_size_;
        };

        const char* const table_t::BorderStyleNone[] = {" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "};
        const char* const table_t::BorderStyleSingle[] = {"┌", "┬", "┐", "├", "┼", "┤", "└", "┴", "┘", "─", "│", " "};
        const char* const table_t::BorderStyleDouble[] = {"╔", "╦", "╗", "╠", "╬", "╣", "╚", "╩", "╝", "═", "║", " "};


        // some thoughs on how to provide a simple API for the user to 'generate'
        // nicely formatted tables:
        //
        // 1. the user should be able to specify the number of columns
        // 2. the user should be able to specify the alignment of each column
        // 3. the user should be able to specify the padding of each column
        // 4. the user should be able to specify the header of each column
        // 5. the user should be able to specify the column widths of the table
        // 6. the user should be able to specify the border style of the table
        //
        // Note: Left/Right borders have a space character in front of them.
        //       Middle borders have a space character on both sides of them.
        // 
        // So the table with the 4 columns defined below as an example has
        // actually a total width of 93 characters:
        // 1 + 1 + 20 + 1 + 1 + 1 + 20 + 1 + 1 + 1 + 20 + 1 + 1 + 1 + 20 + 1 + 1 = 93
        inline void UserCase()
        {
            state_t columns[] = {
                state_t(20, TypeId::kString, Flags::kAlignCenter),
                state_t(20, TypeId::kString, Flags::kAlignCenter),
                state_t(20, TypeId::kFloat, Flags::kAlignCenter),
                state_t(20, TypeId::kFloat, Flags::kAlignCenter),
            };

            char row[256];

            fmt::table_t table(columns, row, sizeof(row));

            table.header_values("hello", "hello", "constant", "constant");
            table.print(); // console print ?

            table.row_values("world", "world", 3.1415f, 3.1415f);
            table.cell_value(1, "hello");      // still update/overwrite a cell
            table.cell_color(1, table_t::Red); // change the color of a cell
            table.print();                     // console print ?

            // row

            table.separator_line(table_t::LSR, table_t::LSR, table_t::L_R, table_t::L_R); // console print ?

            // row

            table.final(); // console print ?
        }

    } // namespace fmt
} // namespace ncore

#endif // __CBASE_TABLE_FMT_H__