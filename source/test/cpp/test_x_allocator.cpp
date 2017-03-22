#include "xbase\x_allocator.h"
#include "xbase\x_debug.h"

#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;

UNITTEST_SUITE_BEGIN(xallocator)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		struct test_object1
		{
			test_object1():mInteger(1),mFloat(2.0){}
			s32	mInteger;
			f32	mFloat;
			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

        UNITTEST_TEST(_XCORE_CLASS_PLACEMENT_NEW_DELETE)
        {
			void* object1_tmp = gTestAllocator->allocate(sizeof(test_object1),4);
			test_object1* object1 = new(object1_tmp) test_object1;
			CHECK_NOT_NULL(object1);
			CHECK_TRUE(1 == object1->mInteger);
			CHECK_TRUE(2.0 == object1->mFloat);
			delete object1;
			CHECK_TRUE(1 == object1->mInteger);
			CHECK_TRUE(2.0 == object1->mFloat);
			gTestAllocator->deallocate(object1_tmp);
        }

		struct test_object2
		{
			test_object2():mInteger(3),mFloat(4.0){}
			s32	mInteger;
			f32	mFloat;
			static x_iallocator* get_allocator() { return gTestAllocator; }
			XCORE_CLASS_NEW_DELETE(get_allocator, 16)
		};

		UNITTEST_TEST(_XCORE_CLASS_NEW_DELETE)
		{
			test_object2* object2 = new test_object2;
			CHECK_NOT_NULL(object2);
			CHECK_TRUE(3 == object2->mInteger);
			CHECK_TRUE(4.0 == object2->mFloat);
			CHECK_TRUE((uptr)object2%4 == 0);
			delete object2;
		}

		struct test_object3
		{
			test_object3():mInteger(2),mFloat(3.0){}
			s32	mInteger;
			f32	mFloat;
			static x_iallocator* get_allocator() { return gTestAllocator; }
			XCORE_CLASS_ARRAY_NEW_DELETE(get_allocator, 32)
		};

		UNITTEST_TEST(_XCORE_CLASS_ARRAY_NEW_DELETE)
		{
			test_object3* object3 = new test_object3[3];
			CHECK_NOT_NULL(object3);
			CHECK_TRUE(object3[0].mInteger = 2);
			CHECK_TRUE(object3[0].mFloat = 3.0);
			CHECK_TRUE(object3[1].mInteger = 2);
			CHECK_TRUE(object3[1].mFloat = 3.0);
			CHECK_TRUE(object3[2].mInteger = 2);
			CHECK_TRUE(object3[2].mFloat = 3.0);
			CHECK_TRUE((uptr)object3%32 == 0);
			delete[] object3;
		}

		UNITTEST_TEST(basic_allocator_create)
		{
			u32 mem_size = 4 * 1024;
			void* mem_begin = gTestAllocator->allocate(mem_size, 4);

			x_iallocator* ba = gCreateBasicAllocator(mem_begin, mem_size, 16, 16);
			ba->release();

			gTestAllocator->deallocate(mem_begin);
		}

		UNITTEST_TEST(basic_allocator_alloc)
		{
			u32 mem_size = 4 * 1024;
			void* mem_begin = gTestAllocator->allocate(mem_size, 4);
			x_iallocator* ba = gCreateBasicAllocator(mem_begin, mem_size, 16, 16);

			void* mem1 = ba->allocate(12, 16);
			ba->deallocate(mem1);

			ba->release();
			gTestAllocator->deallocate(mem_begin);
		}

		UNITTEST_TEST(basic_allocator_alloc2_dealloc2)
		{
			u32 mem_size = 4 * 1024;
			void* mem_begin = gTestAllocator->allocate(mem_size, 4);
			x_iallocator* ba = gCreateBasicAllocator(mem_begin, mem_size, 16, 16);

			void* mem1 = ba->allocate(16, 16);
			void* mem2 = ba->allocate(32, 32);
			
			ba->deallocate(mem1);
			ba->deallocate(mem2);

			ba->release();
			gTestAllocator->deallocate(mem_begin);
		}

		UNITTEST_TEST(basic_allocator_alloc5_dealloc5)
		{
			u32 mem_size = 4 * 1024;
			void* mem_begin = gTestAllocator->allocate(mem_size, 4);
			x_iallocator* ba = gCreateBasicAllocator(mem_begin, mem_size, 16, 16);

			void* mem1 = ba->allocate(16, 16);
			void* mem2 = ba->allocate(32, 32);
			void* mem3 = ba->allocate(48, 64);
			void* mem4 = ba->allocate(128, 128);
			void* mem5 = ba->allocate(512, 256);

			ba->deallocate(mem4);
			ba->deallocate(mem2);
			ba->deallocate(mem5);
			ba->deallocate(mem3);
			ba->deallocate(mem1);

			ba->release();
			gTestAllocator->deallocate(mem_begin);
		}

		UNITTEST_TEST(basic_allocator_alloc_dealloc_stress_test)
		{
			u32 random_size_list_N = 100;
			s32 random_size_list[] = {
				 31		,174	,36		,242	,76
				,163	,205	,80		,84		,108
				,153	,53		,27		,134	,90
				,228	,54		,154	,121	,83
				,119	,183	,25		,201	,226
				,154	,184	,171	,149	,40
				,217	,84		,67		,114	,45
				,42		,202	,109	,27		,48
				,100	,79		,215	,227	,90
				,148	,128	,139	,70		,157
				,199	,157	,138	,40		,146
				,81		,223	,73		,109	,93
				,230	,76		,171	,127	,61
				,203	,100	,212	,228	,223
				,115	,171	,167	,59		,90
				,231	,221	,22		,117	,61
				,225	,60		,80		,85		,73
				,193	,248	,232	,184	,28
				,28		,233	,243	,30		,147
				,46		,33		,151	,71		,248
			};

			s32 random_i_list[] = {
				 35  ,43  ,03  ,74  ,79  ,67  ,13  ,73  ,22  , 6
				,78  ,60  ,98  ,10  ,42  ,12  ,40  ,86  ,18  , 5
				, 7  ,31  ,38  ,51  ,44  , 4  ,56  ,75  ,34  ,97
				,27  ,90  ,61  ,91  ,25  ,85  ,32  , 2  ,54  ,48
				,64  ,11  ,45  ,26  ,77  ,83  ,46  ,55  ,30  ,28
				,65  ,49  ,39  ,72  ,50  ,81  , 9  ,89  ,19  ,82
				, 1  ,70  ,41  ,69  ,95  ,62  ,63  ,59  ,37  ,15
				,68  ,33  ,71  ,58  ,23  ,57  ,96  ,36  ,66  ,53
				,80  ,20  ,92  ,16  ,24  ,84  ,87  ,93  , 8  ,76
				,52  ,14  ,99  ,21  ,94  ,17  ,29  , 0  ,88  ,47				
			};

			u32 mem_size = 32 * 1024;
			void* mem_begin = gTestAllocator->allocate(mem_size, 4);
			x_iallocator* ba = gCreateBasicAllocator(mem_begin, mem_size, 16, 16);

			void* mem[100];

			for (s32 i=0; i<100; ++i)
			{
				mem[i] = ba->allocate(random_size_list[i], 16);
				CHECK_NOT_NULL(mem[i]);
			}

			for (s32 i=0; i<100; ++i)
			{
				CHECK_NOT_NULL(mem[ random_i_list[i] ]);
				ba->deallocate(mem[ random_i_list[i] ]);
				mem[ random_i_list[i] ] = NULL;
			}

			ba->release();
			gTestAllocator->deallocate(mem_begin);

		}

	}
}
UNITTEST_SUITE_END
