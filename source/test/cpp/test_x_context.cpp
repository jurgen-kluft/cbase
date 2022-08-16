#include "xbase/x_context.h"

#include "xunittest/xunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(context_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		struct OurTlsObject
		{
			s32		mInteger;
			f32		mFloat;
		};
		OurTlsObject	gInstance;

        UNITTEST_TEST(set)
        {
			gInstance.mInteger = 1;
			gInstance.mFloat = 2.0f;
			context_t::set(0, 5, &gInstance);
		}

		UNITTEST_TEST(get)
		{
			OurTlsObject* obj;
			context_t::get(0, 5, obj);
			CHECK_EQUAL(1, obj->mInteger);
			CHECK_EQUAL(2.0f, obj->mFloat);
		}

	}
}
UNITTEST_SUITE_END
