#include "cbase/c_tblfmt.h"

namespace ncore
{
    namespace fmt
    {
        static const char* const BorderStyleNone   = "            ";
        static const char* const BorderStyleSingle = "┌┬┐├┼┤└┴┘─│ ";
        static const char* const BorderStyleDouble = "╔╦╗╠╬╣╚╩╝═║ ";

        void tbl_format(tbl_state_t& state, u8* arg_types, u64* arg_values, format_func_t* arg_formatters, s32 argc)
        {


        }

        void tbl_cell(s8 column, tbl_state_t& state, u8* arg_types, u64* arg_values, format_func_t* arg_formatters, s32 argc)
        {


        }

        void tbl_line(tbl_state_t& state, u8 const* separators, s32 separator_count, Border style)
        {


        }

        void tbl_flags(tbl_state_t& state, Flags const* flags, s32 count)
        {
            for (s32 i = 0; i < count; ++i)
            {
                state.flags_[i] = flags[i];
            }
        }
    }
}
