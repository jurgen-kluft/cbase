#include "xbase/x_base.h"
#include "xbase/x_allocator.h"
#include "xbase/x_console.h"

#include "xunittest/xunittest.h"
#include "xunittest/private/ut_ReportAssert.h"

UNITTEST_SUITE_LIST(xCoreUnitTest);

UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtypes);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xallocator);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbinary_search);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbitfield);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbuffer);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xcarray);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xcontainers);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xdouble);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xendian);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xfloat);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xguid);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xinteger);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmemory_std);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xqsort);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xrange);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsingleton);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xslice);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsprintf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsscanf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_ascii);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_utf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbtree);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtls);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xva);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xhibitset);

#ifndef X_NO_CUSTOM_INT64
UNITTEST_SUITE_DECLARE(xCoreUnitTest, __xint64);
#endif // X_NO_CUSTOM_INT64

namespace xcore
{
	// Our own assert handler
	class UnitTestAssertHandler : public xcore::xasserthandler
	{
	public:
		UnitTestAssertHandler()
		{
			NumberOfAsserts = 0;
		}

		virtual xcore::xbool	handle_assert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
		{
			UnitTest::reportAssert(exprString, fileName, lineNumber);
			NumberOfAsserts++;
			return false;
		}


		xcore::s32		NumberOfAsserts;
	};

	class UnitTestAllocator : public UnitTest::Allocator
	{
		xcore::xalloc*	mAllocator;
	public:
						UnitTestAllocator(xcore::xalloc* allocator)	{ mAllocator = allocator; }
		virtual void*	Allocate(xsize_t size)								{ return mAllocator->allocate((u32)size, sizeof(void*)); }
		virtual void	Deallocate(void* ptr)								{ mAllocator->deallocate(ptr); }
	};

	class TestAllocator : public xalloc
	{
		xalloc*		mAllocator;
	public:
							TestAllocator(xalloc* allocator) : mAllocator(allocator) { }

		virtual const char*	name() const										{ return "xbase unittest test heap allocator"; }

		virtual void*		allocate(xsize_t size, u32 alignment)
		{
			UnitTest::IncNumAllocations();
			return mAllocator->allocate(size, alignment);
		}

		virtual void		deallocate(void* mem)
		{
			UnitTest::DecNumAllocations();
			mAllocator->deallocate(mem);
		}

		virtual void		release()
		{
			mAllocator->release();
			mAllocator = NULL;
		}
	};
}

xcore::xalloc* gTestAllocator = NULL;
xcore::UnitTestAssertHandler gAssertHandler;


bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
	xbase::x_Init();

#ifdef TARGET_DEBUG
	xcore::xasserthandler::sRegisterHandler(&gAssertHandler);
#endif

	xcore::xalloc* systemAllocator = xcore::xalloc::get_system();
	xcore::UnitTestAllocator unittestAllocator( systemAllocator );
	UnitTest::SetAllocator(&unittestAllocator);

	xcore::console->write("Configuration: ");
	xcore::console->writeLine(TARGET_FULL_DESCR_STR);

	xcore::TestAllocator testAllocator(systemAllocator);
	gTestAllocator = &testAllocator;

	int r = UNITTEST_SUITE_RUN(reporter, xCoreUnitTest);
	if (UnitTest::GetNumAllocations()!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, "xunittest", "memory leaks detected!");
		r = -1;
	}

	gTestAllocator->release();

	UnitTest::SetAllocator(NULL);

	xbase::x_Exit();
	return r==0;
}

