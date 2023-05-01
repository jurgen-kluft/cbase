#ifndef __CBASE_TABLE_FMT_H__
#define __CBASE_TABLE_FMT_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_debug.h"
#include "cbase/c_runes.h"
#include "cbase/c_strfmt.h"
#include "cbase/c_va_list.h"

namespace ncore
{
    namespace fmt
    {
        struct Border
        {
            enum EBorder
            {
                None   = 0x00,
                ___    = 0x00,
                __R    = 0x01,
                _S_    = 0x02,
                _SR    = 0x03,
                L__    = 0x04,
                L_R    = 0x05,
                LS_    = 0x06,
                LSR    = 0x07,
                Middle = 0x08,
                Single = 0x10,
                Double = 0x20,
                Top    = 0x40,
                Bottom = 0x80,
            };

            inline Border() : value((u8)LSR | (u8)Single) {}
            inline Border(EBorder value) : value((u8)value) {}
            inline Border(s32 value) : value((u8)value) {}

            u8 value;
        };
        struct CellColor
        {
            enum EColor
            {
                Black   = 0,
                Red     = 1,
                Green   = 2,
                Yellow  = 3,
                Blue    = 4,
                Magenta = 5,
                Cyan    = 6,
                White   = 7,
            };
            u8 value;
        };

        struct tbl_state_t
        {
            s8*      widths_;
            Flags*   flags_;
            s8*      colors_;
            Border*  borders_;
            uchar32* row_;
            s32      row_len_;
            s16      columns_size_;
            s16      row_size_;
        };

        void tbl_init(tbl_state_t& state);
        void tbl_format(tbl_state_t& state, va_t const* argv, s32 argc);
        void tbl_cell(s8 column, tbl_state_t& state, va_t const* argv, s32 argc);
        void tbl_line(tbl_state_t& state, Border const* borders, s32 borders_count, Border style);
        void tbl_flags(tbl_state_t& state, Flags const* flags, s32 count);
        void tbl_row_to_utf8(tbl_state_t& state, utf8::prune str, utf8::pcrune end);
        void tbl_row_to_utf16(tbl_state_t& state, utf16::prune str, utf16::pcrune end);

        // A table formatter (helper) that uses a (given) fixed size array to store a table row.
        // 
        template <uint_t C, uint_t W>
        struct table_t
        {
            table_t()
            {
                state_.widths_       = widths_;
                state_.flags_        = flags_;
                state_.colors_       = colors_;
                state_.borders_      = borders_;
                state_.row_          = row_;
                state_.columns_size_ = C;
                state_.row_size_     = W;
                tbl_init(state_);
            }

            inline s32 columns() const { return state_.columns_size_; }

            void cell_color(s8 column, CellColor::EColor color)
            {
                if (column < state_.columns_size_)
                    colors_[column] = (s8)color;
            }

            void row_to_utf8(utf8::prune str, utf8::pcrune end) { tbl_row_to_utf8(state_, str, end); }
            void row_to_utf16(utf16::prune str, utf16::pcrune end) { tbl_row_to_utf16(state_, str, end); }

            template <typename... Args>
            void row(Args&&... _args)
            {
                const va_t argv[] = {va_t(_args)...};
                const s32  argc   = sizeof(argv) / sizeof(argv[0]);
                tbl_format(state_, argv, argc);
            }

            template <typename T>
            void cell_replace(s8 column, T arg, Flags flags = Flags::None)
            {
                const va_t argv[] = {va_t(arg)};
                const s32  argc   = 1;
                tbl_cell(column, state_, argv, argc);
            }

            template <typename T>
            void cell_overlay(s8 column, T arg, Flags flags = Flags::None)
            {
                const va_t argv[] = {va_t(arg)};
                const s32  argc   = 1;
                tbl_cell(column, state_, argv, argc);
            }

            void top() { tbl_line(state_, nullptr, 0, Border{Border::Single | Border::Top | Border::LSR}); }
            void line() { tbl_line(state_, nullptr, 0, Border{Border::Single | Border::Middle | Border::LSR}); }
            void bottom() { tbl_line(state_, nullptr, 0, Border{Border::Single | Border::Bottom | Border::LSR}); }

            template <typename... Args>
            void flags(Args&&... args)
            {
                const Flags flags[] = {Flags(args)...};
                tbl_flags(state_, flags, sizeof(flags) / sizeof(flags[0]));
            }

            template <typename... Args>
            void widths(Args&&... args)
            {
                const s8 widths[] = {(s8)(args)...};
                for (s32 i = 0; i < sizeof(widths) / sizeof(widths[0]); ++i)
                    widths_[i] = widths[i];
            }

            template <typename... Args>
            void top(Args&&... args)
            {
                const Border bd[] = {Border(args)...};
                const s32    bc   = sizeof(bd) / sizeof(bd[0]);
                tbl_line(state_, bd, bc, {Border::Single | Border::Top});
            }

            template <typename... Args>
            void line(Args&&... args)
            {
                const Border bd[] = {Border(args)...};
                const s32    bc   = sizeof(bd) / sizeof(bd[0]);
                tbl_line(state_, bd, bc, {Border::Single | Border::Middle});
            }

            template <typename... Args>
            void bottom(Args&&... args)
            {
                const Border bd[] = {Border(args)...};
                const s32    bc   = sizeof(bd) / sizeof(bd[0]);
                tbl_line(state_, bd, bc, {Border::Single | Border::Bottom});
            }

            const char* str_utf8()
            {
                row_to_utf8(row_utf8_, &row_utf8_[W * 3 - 3]);
                return ((const char*)row_utf8_);
            }

            s8      widths_[C];
            s8      colors_[C];
            Flags   flags_[C];
            Border  borders_[C];
            uchar32 row_[W];
            uchar8  row_utf8_[W * 3];
            tbl_state_t state_;
        };

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
        // So a table with the 4 columns defined actually has a total width of 93 characters:
        // 1 + 1 + 20 + 1 + 1 + 1 + 20 + 1 + 1 + 1 + 20 + 1 + 1 + 1 + 20 + 1 + 1 = 93

    }  // namespace fmt
}  // namespace ncore

#endif  // __CBASE_TABLE_FMT_H__