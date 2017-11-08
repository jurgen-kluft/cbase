#include "xbase/x_tls.h"

#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xtls)
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
			xtls<0, OurTlsObject> slot;
			gInstance.mInteger = 1;
			gInstance.mFloat = 2.0f;
			slot.set(&gInstance);
		}

		UNITTEST_TEST(get)
		{
			xtls<0, OurTlsObject> slot;
			OurTlsObject const* obj;
			slot.get(obj);
			CHECK_EQUAL(1, obj->mInteger);
			CHECK_EQUAL(2.0f, obj->mFloat);
		}

	}
}
UNITTEST_SUITE_END
