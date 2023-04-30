#include "cbase/c_tblfmt.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_tblfmt)
{
    UNITTEST_FIXTURE(tblfmt)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(simple)
        {
            char            row[256];    // 256 characters should be enough for most tables
            fmt::table_t<4> table(row);  // 4 columns

            table.top();
            // printf(row);                                        // console print ?

            table.flags(fmt::Flags::AlignLeft, fmt::Flags::AlignRight, fmt::Flags::AlignRight, fmt::Flags::AlignRight);
            table.row("hello", "hello", "constant", "constant");
            // printf(row);

            table.line();
            // printf(row);                                        // console print ?

            table.row("world", "world", 3.1415f, 3.1415f);
            table.cell_replace(1, "hello", fmt::Flags::AlignLeft);    // overwrite a cell
            table.cell_overlay(1, "world!", fmt::Flags::AlignRight);  // overlay a cell
            table.cell_color(1, fmt::CellColor::Red);                 // change the color of a cell

            // printf(row);                                        // console print ?

            // row

            table.line();
            // printf(row);                                        // console print ?

            // or if you want to override the separators for the line:
            table.line(fmt::Border::LSR, fmt::Border::LSR, fmt::Border::L_R, fmt::Border::L_R);
            // printf(row);                                        // console print ?

            // row

            table.bottom();
            // printf(row);                                        // console print ?

            // or if you want to override the separators for the bottom line:
            table.bottom(fmt::Border::LSR, fmt::Border::LS_, fmt::Border::_S_, fmt::Border::_SR);            

        }
    }
}
UNITTEST_SUITE_END
