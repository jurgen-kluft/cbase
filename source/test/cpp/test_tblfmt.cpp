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
            fmt::table_t<4, 256> table;   // 4 columns (~2 KB of stack memory)

            table.widths(20, 20, 20, 20);  // set the widths of the columns

            table.top();
            UnitTest::Stdout::Trace(table.str_utf8());

            table.flags(fmt::Flags::AlignLeft, fmt::Flags::AlignRight, fmt::Flags::AlignRight, fmt::Flags::AlignRight);
            table.row("hello", "hello", "constant", "constant");
            UnitTest::Stdout::Trace(table.str_utf8());

            table.line();
            UnitTest::Stdout::Trace(table.str_utf8());

            table.row(2, 3, 4, 5);
            UnitTest::Stdout::Trace(table.str_utf8());

            table.row("world", "world", 3.1415f, 3.1415f);
            table.cell_replace(1, "hello", fmt::Flags::AlignLeft);    // overwrite a cell
            table.cell_overlay(1, "world!", fmt::Flags::AlignRight);  // overlay a cell
            table.cell_color(1, fmt::CellColor::Red);                 // change the color of a cell

            UnitTest::Stdout::Trace(table.str_utf8());

            // row

            table.row();
            UnitTest::Stdout::Trace(table.str_utf8());

            table.bottom();
            UnitTest::Stdout::Trace(table.str_utf8());

        }
    }
}
UNITTEST_SUITE_END
