#include "xbase\x_target.h"
#include "xbase\x_types.h"
#include "xbase\x_allocator.h"
#include "xbase\x_string.h"

#include "xunittest\xunittest.h"

UNITTEST_SUITE_LIST(xCoreUnitTest);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xdouble);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xendian);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xfloat);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xguid);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xinteger);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_std);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_tmp);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xcstring);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xccstring);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmemory_std);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xqsort);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, x_binary_search);

#ifndef X_NO_CUSTOM_INT64
UNITTEST_SUITE_DECLARE(xCoreUnitTest, __xint64);
#endif // X_NO_CUSTOM_INT64


namespace xcore
{
	class TestHeapAllocator : public x_iallocator
	{
	public:
		TestHeapAllocator(xcore::x_iallocator* allocator)
			: mAllocator(allocator)
			, mNumAllocations(0)
		{
		}

		xcore::x_iallocator*	mAllocator;
		s32						mNumAllocations;

		virtual const char*	name() const
		{
			return "xbase unittest test heap allocator";
		}

		virtual void*		allocate(u32 size, u32 alignment)
		{
			++mNumAllocations;
			return mAllocator->allocate(size, alignment);
		}

		virtual void*		reallocate(void* mem, u32 size, u32 alignment)
		{
			return mAllocator->reallocate(mem, size, alignment);
		}

		virtual void		deallocate(void* mem)
		{
			--mNumAllocations;
			mAllocator->deallocate(mem);
		}

		virtual void		release()
		{
		}
	};
}

class UnitTestAllocator : public UnitTest::Allocator
{
public:
	xcore::x_iallocator*	mAllocator;
	int						mNumAllocations;

	UnitTestAllocator(xcore::x_iallocator* allocator)
		: mNumAllocations(0)
	{
		mAllocator = allocator;
	}

	virtual void*	Allocate(int size)
	{
		++mNumAllocations;
		return mAllocator->allocate(size, 4);
	}
	virtual void	Deallocate(void* ptr)
	{
		--mNumAllocations;
		mAllocator->deallocate(ptr);
	}
};

bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
	xcore::x_iallocator* systemAllocator = xcore::gCreateSystemAllocator();
	UnitTestAllocator unittestAllocator( systemAllocator );
	UnitTest::SetAllocator(&unittestAllocator);

	xcore::TestHeapAllocator stringHeapAllocator(systemAllocator);
	xcore::xstring::sSetAllocator(&stringHeapAllocator);
	xcore::TestHeapAllocator stringTmpAllocator(systemAllocator);
	xcore::xstring_tmp::sSetAllocator(&stringTmpAllocator);
	
	int r = UNITTEST_SUITE_RUN(reporter, xCoreUnitTest);
	if (unittestAllocator.mNumAllocations!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, "xunittest", "memory leaks detected!");
		r = -1;
	}
	if (stringHeapAllocator.mNumAllocations!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, "xbase::xstring", "memory leaks detected!");
		r = -1;
	}
	if (stringTmpAllocator.mNumAllocations!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, "xbase::xstring_tmp", "memory leaks detected!");
		r = -1;
	}

	xcore::xstring::sSetAllocator(NULL);
	xcore::xstring_tmp::sSetAllocator(NULL);

	UnitTest::SetAllocator(NULL);
	systemAllocator->release();
	return r==0;
}

