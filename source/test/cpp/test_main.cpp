#include "cbase/c_base.h"
#include "cbase/c_allocator.h"
#include "cbase/c_console.h"
#include "cbase/c_context.h"

#include "cunittest/cunittest.h"
#include "cunittest/private/ut_ReportAssert.h"

UNITTEST_SUITE_LIST(xCoreUnitTest);

UNITTEST_SUITE_DECLARE(xCoreUnitTest, xinteger);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtypes);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xallocator);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbinary_search);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbitfield);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbtree);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, buffer_t);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, carray_t);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, darray_t);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xdouble);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xdtrie);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xendian);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xfloat);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, guid_t);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, hbb_t);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmap_and_set);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmemory_std);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, g_qsort);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xrange);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, singleton_t);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xslice);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsprintf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsscanf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_ascii);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_utf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtree);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, context_t);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xva);


#ifndef X_NO_CUSTOM_INT64
UNITTEST_SUITE_DECLARE(xCoreUnitTest, __xint64);
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

    int r = UNITTEST_SUITE_RUN(reporter, xCoreUnitTest);
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

