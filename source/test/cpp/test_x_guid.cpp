#include "xbase\x_guid.h"
#include "xbase\x_string_ascii.h"

#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xguid)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(constructor1)
		{
			xguid i;
			CHECK_EQUAL(true, i.isNull());

			xguid nill;
			nill.setNull();
			CHECK_EQUAL(true, nill.isNull());

			CHECK_TRUE(nill == i);
		}

		UNITTEST_TEST(constructor2)
		{
			xguid nill;
			nill.setNull();

			xguid i1(1,2,3,4);
			CHECK_EQUAL(false, i1.isNull());

			xguid i2(i1);
			CHECK_EQUAL(false, i2.isNull());

			CHECK_TRUE(i1 == i2);

			CHECK_FALSE(i1 == nill);
			CHECK_TRUE(i1 != nill);

			CHECK_FALSE(i2 == nill);
			CHECK_TRUE(i2 != nill);
		}

		UNITTEST_TEST(constructor3)
		{
			xguid i(1,2,3,4);
			CHECK_EQUAL(false, i.isNull());

			xguid nill;
			nill.setNull();

			CHECK_FALSE(i == nill);
			CHECK_TRUE(i != nill);
		}

		UNITTEST_TEST(setNull)
		{
			xguid i(1,2,3,4);
			CHECK_EQUAL(false, i.isNull());

			i.setNull();
			CHECK_EQUAL(true, i.isNull());
		}

		UNITTEST_TEST(isNull)
		{
			xguid i(1,2,3,4);
			CHECK_EQUAL(false, i.isNull());
			i.setNull();
			CHECK_EQUAL(true, i.isNull());

			xguid i2;
			CHECK_EQUAL(true, i2.isNull());
		}

		UNITTEST_TEST(set)
		{
			xguid i;
			i.set(1,2,3,4);

			u32 a,b,c,d;
			i.get(a,b,c,d);
			CHECK_EQUAL(1, a);
			CHECK_EQUAL(2, b);
			CHECK_EQUAL(3, c);
			CHECK_EQUAL(4, d);
		}

		UNITTEST_TEST(get)
		{
			xguid i1(1,2,3,4);

			u32 a,b,c,d;
			i1.get(a,b,c,d);
			CHECK_EQUAL(1, a);
			CHECK_EQUAL(2, b);
			CHECK_EQUAL(3, c);
			CHECK_EQUAL(4, d);

			xguid i2(11, 33, 44, 99);
			i2.get(a,b,c,d);
			CHECK_EQUAL(11, a);
			CHECK_EQUAL(33, b);
			CHECK_EQUAL(44, c);
			CHECK_EQUAL(99, d);
		}

		UNITTEST_TEST(assignment_operator)
		{
			xguid i1(1,2,3,4);
			xguid i2;
			CHECK_TRUE(i2.isNull());
			i2 = i1;
			CHECK_TRUE(i1 == i2);
			CHECK_TRUE(i2 == i1);
		}

		UNITTEST_TEST(equal_operator)
		{
			xguid i1(1,2,3,4);

			xguid i2;
			i2.set(1,2,3,4);

			CHECK_TRUE(i1 == i2);
			CHECK_TRUE(i2 == i1);

			i1.setNull();
			i2.setNull();
			CHECK_TRUE(i1 == i2);
			CHECK_TRUE(i2 == i1);
		}

		UNITTEST_TEST(not_equal_operator)
		{
			xguid i(1,2,3,4);
			CHECK_EQUAL(false, i.isNull());

			xguid nill;
			nill.setNull();

			CHECK_FALSE(i == nill);
			CHECK_TRUE(i != nill);
		}

		UNITTEST_TEST(toString)
		{
			xguid id(0x11335577, 0x22446688, 0x557799BB, 0x88AACCEE);

			char strBuffer[256];

			const char* guidStr = "11335577:22446688:557799BB:88AACCEE";
			id.toString(strBuffer, sizeof(strBuffer));
			CHECK_EQUAL(0, x_strCompare(strBuffer, guidStr));
		}

		UNITTEST_TEST(fromString)
		{
			const char* guidStr = "11335577:22446688:557799BB:88AACCEE";

			xguid id1;
			id1.fromString(guidStr);

			xguid id2(0x11335577, 0x22446688, 0x557799BB, 0x88AACCEE);

			CHECK_TRUE(id1 == id2);
		}
	}
}
UNITTEST_SUITE_END
