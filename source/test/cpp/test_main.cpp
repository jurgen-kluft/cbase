#include "cbase/c_base.h"
#include "cbase/c_allocator.h"
#include "cbase/c_console.h"
#include "cbase/c_context.h"

#include "cunittest/cunittest.h"

UNITTEST_SUITE_LIST(cUnitTest);
UNITTEST_SUITE_DECLARE(cUnitTest, test_integer);
UNITTEST_SUITE_DECLARE(cUnitTest, test_types);
UNITTEST_SUITE_DECLARE(cUnitTest, test_allocator);
UNITTEST_SUITE_DECLARE(cUnitTest, test_binary_search);
UNITTEST_SUITE_DECLARE(cUnitTest, test_bitfield);
UNITTEST_SUITE_DECLARE(cUnitTest, test_btree);
UNITTEST_SUITE_DECLARE(cUnitTest, test_buffer_t);
UNITTEST_SUITE_DECLARE(cUnitTest, test_carray_t);
UNITTEST_SUITE_DECLARE(cUnitTest, test_darray_t);
UNITTEST_SUITE_DECLARE(cUnitTest, test_double);
UNITTEST_SUITE_DECLARE(cUnitTest, test_dtrie);
UNITTEST_SUITE_DECLARE(cUnitTest, test_endian);
UNITTEST_SUITE_DECLARE(cUnitTest, test_float);
UNITTEST_SUITE_DECLARE(cUnitTest, test_guid_t);
UNITTEST_SUITE_DECLARE(cUnitTest, test_hbb_t);
UNITTEST_SUITE_DECLARE(cUnitTest, test_map_and_set);
UNITTEST_SUITE_DECLARE(cUnitTest, test_memory_std);
UNITTEST_SUITE_DECLARE(cUnitTest, test_qsort);
UNITTEST_SUITE_DECLARE(cUnitTest, test_range);
UNITTEST_SUITE_DECLARE(cUnitTest, test_singleton_t);
UNITTEST_SUITE_DECLARE(cUnitTest, test_slice);
UNITTEST_SUITE_DECLARE(cUnitTest, test_span);
UNITTEST_SUITE_DECLARE(cUnitTest, test_sprintf);
UNITTEST_SUITE_DECLARE(cUnitTest, test_sscanf);
UNITTEST_SUITE_DECLARE(cUnitTest, test_string_ascii);
UNITTEST_SUITE_DECLARE(cUnitTest, test_string_utf);
UNITTEST_SUITE_DECLARE(cUnitTest, test_tree);
UNITTEST_SUITE_DECLARE(cUnitTest, test_context_t);
UNITTEST_SUITE_DECLARE(cUnitTest, test_va);

#ifndef D_NO_CUSTOM_INT64
UNITTEST_SUITE_DECLARE(cUnitTest, test_int64);
#endif // X_NO_CUSTOM_INT64

#include "test_main.cxx"
