#include "xbase/x_tls.h"

#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(tls_t)
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
			tls_t::set<0, OurTlsObject>(&gInstance);
		}

		UNITTEST_TEST(get)
		{
			OurTlsObject * obj;
			tls_t::get<0, OurTlsObject>(obj);
			CHECK_EQUAL(1, obj->mInteger);
			CHECK_EQUAL(2.0f, obj->mFloat);
		}

	}
}
UNITTEST_SUITE_END
