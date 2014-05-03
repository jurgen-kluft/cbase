#include "xbase\x_base.h"
#include "xbase\x_allocator.h"
#include "xbase\x_console.h"

#include "xunittest\xunittest.h"
#include "xunittest\private\ut_ReportAssert.h"

UNITTEST_SUITE_LIST(xCoreUnitTest);


// SPU-only test list

#ifdef SPU
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsingleton);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xallocator);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbinary_search);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xqsort);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbitfield);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xendian);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xfloat);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xinteger);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmemory_std);
#endif

#ifndef SPU
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xallocator);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbinary_search);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xcarray);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbitfield);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xdouble);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xendian);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xfloat);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xguid);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xinteger);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xqsort);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xrbtree15);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsingleton);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsprintf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsscanf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmemory_std);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtree);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtypes);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtls);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xutf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xva);
#endif

#ifndef X_NO_CUSTOM_INT64
UNITTEST_SUITE_DECLARE(xCoreUnitTest, __xint64);
#endif // X_NO_CUSTOM_INT64

namespace xcore
{
	// Our own assert handler
	class UnitTestAssertHandler : public xcore::x_asserthandler
	{
	public:
		UnitTestAssertHandler()
		{
			NumberOfAsserts = 0;
		}

		virtual xcore::xbool	HandleAssert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
		{
			UnitTest::reportAssert(exprString, fileName, lineNumber);
			NumberOfAsserts++;
			return false;
		}


		xcore::s32		NumberOfAsserts;
	};

	class UnitTestAllocator : public UnitTest::Allocator
	{
		xcore::x_iallocator*	mAllocator;
	public:
						UnitTestAllocator(xcore::x_iallocator* allocator)	{ mAllocator = allocator; }
		virtual void*	Allocate(xsize_t size)								{ return mAllocator->allocate((u32)size, 4); }
		virtual void	Deallocate(void* ptr)								{ mAllocator->deallocate(ptr); }
	};

	class TestAllocator : public x_iallocator
	{
		x_iallocator*		mAllocator;
	public:
							TestAllocator(x_iallocator* allocator) : mAllocator(allocator) { }

		virtual const char*	name() const										{ return "xbase unittest test heap allocator"; }

		virtual void*		allocate(xsize_t size, u32 alignment)
		{
			UnitTest::IncNumAllocations();
			return mAllocator->allocate(size, alignment);
		}

		virtual void*		reallocate(void* mem, xsize_t size, u32 alignment)
		{
			if (mem==NULL)
				return allocate(size, alignment);
			else 
				return mAllocator->reallocate(mem, size, alignment);
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

xcore::x_iallocator* gTestAllocator = NULL;
xcore::UnitTestAssertHandler gAssertHandler;

bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
#ifdef SPU
	xcore::s32 progSize;
	xcore::s32 stackSize;

	::getProgramAndStackSizeForSPU(&progSize, &stackSize);

	xcore::gSetSPUConfig(progSize, stackSize);
#endif

	xbase::x_Init();

#ifdef TARGET_DEBUG
	xcore::x_asserthandler::sRegisterHandler(&gAssertHandler);
#endif

	xcore::x_iallocator* systemAllocator = xcore::x_iallocator::default();
	xcore::UnitTestAllocator unittestAllocator( systemAllocator );
	UnitTest::SetAllocator(&unittestAllocator);
	
	xcore::xconsole::write("Configuration: ");
	xcore::xconsole::writeLine(TARGET_FULL_DESCR_STR);

	xcore::TestAllocator testAllocator(systemAllocator);
	gTestAllocator = &testAllocator;

	int r = UNITTEST_SUITE_RUN(reporter, xCoreUnitTest);

	gTestAllocator->release();

	UnitTest::SetAllocator(NULL);

	xbase::x_Exit();
	return r==0;
}

