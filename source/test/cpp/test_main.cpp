#include "cbase/c_base.h"
#include "cbase/c_allocator.h"
#include "cbase/c_console.h"
#include "cbase/c_context.h"

#include "cunittest/cunittest.h"
#include "cunittest/private/ut_ReportAssert.h"

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

namespace ncore
{
    // Our own assert handler
    class UnitTestAssertHandler : public ncore::asserthandler_t
    {
    public:
        UnitTestAssertHandler()
        {
            NumberOfAsserts = 0;
        }

        virtual bool	handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
        {
            UnitTest::reportAssert(exprString, fileName, lineNumber);
            NumberOfAsserts++;
            return false;
        }


        ncore::s32		NumberOfAsserts;
    };

    class UnitTestAllocator : public UnitTest::Allocator
    {
        ncore::alloc_t*	mAllocator;
    public:
                        UnitTestAllocator(ncore::alloc_t* allocator)	{ mAllocator = allocator; }
        virtual void*	Allocate(uint_t size)								{ return mAllocator->allocate((u32)size, sizeof(void*)); }
        virtual uint_t	Deallocate(void* ptr)								{ return mAllocator->deallocate(ptr); }
    };

    class TestAllocator : public alloc_t
    {
        alloc_t*		mAllocator;
    public:
                            TestAllocator(alloc_t* allocator) : mAllocator(allocator) { }

        virtual void*		v_allocate(u32 size, u32 alignment)
        {
            UnitTest::IncNumAllocations();
            return mAllocator->allocate(size, alignment);
        }

        virtual u32			v_deallocate(void* mem)
        {
            UnitTest::DecNumAllocations();
            return mAllocator->deallocate(mem);
        }

        virtual void		v_release()
        {
            mAllocator = nullptr;
        }
    };
}

ncore::alloc_t* gTestAllocator = nullptr;
ncore::UnitTestAssertHandler gAssertHandler;

bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
    cbase::init();

#ifdef TARGET_DEBUG
    ncore::context_t::set_assert_handler(&gAssertHandler);
#endif
    ncore::console->write("Configuration: ");
    ncore::console->setColor(ncore::console_t::YELLOW);
    ncore::console->writeLine(TARGET_FULL_DESCR_STR);
    ncore::console->setColor(ncore::console_t::NORMAL);

    ncore::alloc_t* systemAllocator = ncore::context_t::system_alloc();
    ncore::UnitTestAllocator unittestAllocator( systemAllocator );
    UnitTest::SetAllocator(&unittestAllocator);

    ncore::TestAllocator testAllocator(systemAllocator);
    gTestAllocator = &testAllocator;
    ncore::context_t::set_system_alloc(&testAllocator);

    int r = UNITTEST_SUITE_RUN(reporter, cUnitTest);
    if (UnitTest::GetNumAllocations()!=0)
    {
        reporter.reportFailure(__FILE__, __LINE__, "cunittest", "memory leaks detected!");
        r = -1;
    }

    gTestAllocator->release();

    UnitTest::SetAllocator(nullptr);
    ncore::context_t::set_system_alloc(systemAllocator);

    cbase::exit();
    return r==0;
}

