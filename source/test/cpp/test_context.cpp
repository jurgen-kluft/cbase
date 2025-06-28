#include "cbase/c_context.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(context)
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
            context_t context = g_current_context();
			context.set_slot0((void*)&gInstance);
		}

		UNITTEST_TEST(get)
		{
            context_t context = g_current_context();
			OurTlsObject* obj = (OurTlsObject*)context.slot0();
			CHECK_EQUAL(1, obj->mInteger);
			CHECK_EQUAL(2.0f, obj->mFloat);
		}

	}
}
UNITTEST_SUITE_END
