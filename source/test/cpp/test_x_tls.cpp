#include "xbase\x_tls.h"

#include "xunittest\xunittest.h"

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

        UNITTEST_TEST(Instance)
        {
			xtls<OurTlsObject>& ourInstance = xtls<OurTlsObject>::Instance();
		}

		UNITTEST_TEST(Release)
		{
			xtls<OurTlsObject>::Release();
		}

	}
}
UNITTEST_SUITE_END
