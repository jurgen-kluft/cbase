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
        // some thoughs on how to provide a simple API for the user to 'generate'
        // nicely formatted tables:
        //
        // 1. the user should be able to specify the number of columns
        // 2. the user should be able to specify the alignment of each column
        // 3. the user should be able to specify the padding of each column
        // 4. the user should be able to specify the header of each column
        // 5. the user should be able to specify the column widths of the table
        // 6. the user should be able to specify the border style of the table

        struct table_t
        {
            template <uint_t N>
            table_t(state_t (&columns)[N], char* row, uint_t row_size)
                : columns_(columns)
                , row_(row)
                , row_size_(row_size)
            {
            }

            template <typename... Args> 
            void header(Args&&... args)
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

            template <typename... Args> 
            void row(Args&&... args)
            {
                const u8            types[]  = {typed<Args>::value...};
                const u64           values[] = {arg_t<Args>::encode(args)...};
                const format_func_t funcs[]  = {arg_t<Args>::formatter()...};

                // Take each argument and format it into the row buffer using
                // the column state information.
                // Also take the left / right border style into account.
                
            }

            void final()
            {
                // Print the bottom border of the table.
            }

            state_t* columns_;
            uint_t   column_count_;
            char*    row_;
            uint_t   row_size_;
        };

        inline void UserCase()
        {
            state_t columns[] = {
                state_t(20, fmt::Type::kString, fmt::Flags::kAlignCenter),
                state_t(20, fmt::Type::kString, fmt::Flags::kAlignCenter),
                state_t(20, fmt::Type::kString, fmt::Flags::kAlignCenter),
                state_t(20, fmt::Type::kString, fmt::Flags::kAlignCenter),
            };

            char row[256];

            fmt::table_t table(columns, row, sizeof(row));

            table.header("hello", "hello", "hello", "hello");
            table.row("world", "world", "world", "world");
            // row
            // row
            table.final();
        }

    } // namespace fmt
} // namespace ncore

#endif // __CBASE_TABLE_FMT_H__