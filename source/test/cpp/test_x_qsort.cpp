#include "xbase/x_qsort.h"

#include "xunittest/xunittest.h"

using namespace xcore;

static s32 s32_compare(const void* const a, const void* const b, void*)
{
	const xcore::s32* aa = (const xcore::s32*)a;
	const xcore::s32* bb = (const xcore::s32*)b;
	if (*aa < *bb)
		return -1;
	else if (*aa > *bb)
		return 1;
	return 0;
}

static s32 f32_compare(const void* const a, const void* const b, void*)
{
	const xcore::f32* aa = (const xcore::f32*)a;
	const xcore::f32* bb = (const xcore::f32*)b;
	if (*aa < *bb)
		return -1;
	else if (*aa > *bb)
		return 1;
	return 0;
}

UNITTEST_SUITE_BEGIN(g_qsort)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}



		UNITTEST_TEST(sort_s32)
		{
			xcore::s32 number_list[] =
			{
				20418437,89940142,35222807,15452193,39873965,57665325,25892441, 8795529, 2391726,11735384,
				94704581,89539275,97839662,43642375,86605077,56392381,83561822,34804288,51057740,26513622,
				45898144,91629822,75528490,50092159, 4428102,90834075,94564757,78467721,29248220,60437235,
				49094059,78501393,40469330,98927504,45939496,24333626,74765115,01162746,84472137,60756670,
				77416885,89363849,69293767,41257972,16360881,25410919,64540821,26084541,64276814,10367363,
				91322469,23978458,94403578,75149255,50233032,70315828,74761538,67276453,58166975,50370976,
				40218964,56377755,38854918,24906470,60564937,62452902,79326292,19724754,52525824,77604696,
				93641667,37720144,36194669,64086266,90701637,90751198,12407559,49757237,43191921,58161568,
				29822916,59362260,48436532, 7354940,47882312,18211779,77290120,31444388,65967189,82142358,
				  625909,15241912,98940339,69982586,44954660,35087976,28454552, 9612152,20659472,76181108,
				16336774,37027136,  978422,22499851,66946820,42727746,13317369,82220887,38824157, 9238429,
				62881795,21555435,16397278,98828691,45746016,94976910,35774012,26356933,33382049,16369721,
				27998504, 8127238,31398781,19448816,29177392,97011356,47148485,20763412,48130305,88311257,
				 9612956,42962822,84578847,31847224,76862290,69477389,96365726, 1018699,93736944,80747693,
				25592297,38723879,16115121,21605446,72957923,90235076, 3195122,44453770,22194130,33330701,
				77114095,74282683,14303992,23988805,14775657,18162041,49997339,96842207, 7406797,62782913,
				23554325,45340530,42556245,97339421,57248204,11141864,10186091,81801978,83056086,27835211,
				55380051,77844226,39720338,53136029,67734164,94087184,75420821,85656935,  229690,95273074,
				47971513,10730032,87339962,92212211,85290141,25099773,34062482,71467999,40682394,17799751,
				41100793,13014569,35745591,30845415,22231372,25965399,92075791,02076467,98044630,91943680,
				78235025,37357911,13679549,97043721,30093124,86478567,19493548,63036415,14905490, 5837347,
				77623653,95024157,26439572,66653493,42203128,72704222,50683353,32318111,59879133,46779583,
				32836164,48138006,52418999,66056820,43479057, 1949963,22702437,30813484,50143227,70892399,
				98020988,30621895,68118412,36675204,54984922,24104729,14684265,50909238,78559013,53622996,
				55080299,15321599,86966691,46446327,12201202,15986804,14911213,72444093,31161870,56214745,
				33724884,68692765,70877367,64728116, 2632895,45126151,93857030,16934737,93188643,85053739,
				45712825,81876592,93961330,24646309,75085731,33538764,40110417,85224655,16525454,16677283,
				12352412, 5578758,81339043,18607844,18848471,64523704,64196796,21016454,59427976,79996321,
				69495124,43485847,17519345,66644428,85166464,10394800,39572871,37438296,89853126,74654252
			};
			s32 n = sizeof(number_list)/sizeof(xcore::s32);
			g_qsort(number_list, n, sizeof(xcore::s32), s32_compare);
			for (s32 i=0; i<(n-1); ++i)
				CHECK_TRUE(number_list[i] <= number_list[i+1]);
		}

		UNITTEST_TEST(sort_f32)
		{
			xcore::f32 float_list[] =
			{
				0.20418437f,0.89940142f,0.35222807f,0.15452193f,0.39873965f,0.57665325f,0.25892441f,0.08795529f,0.02391726f,0.11735384f,
				0.94704581f,0.89539275f,0.97839662f,0.43642375f,0.86605077f,0.56392381f,0.83561822f,0.34804288f,0.51057740f,0.26513622f,
				0.45898144f,0.91629822f,0.75528490f,0.50092159f,0.04428102f,0.90834075f,0.94564757f,0.78467721f,0.29248220f,0.60437235f,
				0.49094059f,0.78501393f,0.40469330f,0.98927504f,0.45939496f,0.24333626f,0.74765115f,0.01162746f,0.84472137f,0.60756670f,
				0.77416885f,0.89363849f,0.69293767f,0.41257972f,0.16360881f,0.25410919f,0.64540821f,0.26084541f,0.64276814f,0.10367363f,
				0.91322469f,0.23978458f,0.94403578f,0.75149255f,0.50233032f,0.70315828f,0.74761538f,0.67276453f,0.58166975f,0.50370976f,
				0.40218964f,0.56377755f,0.38854918f,0.24906470f,0.60564937f,0.62452902f,0.79326292f,0.19724754f,0.52525824f,0.77604696f,
				0.93641667f,0.37720144f,0.36194669f,0.64086266f,0.90701637f,0.90751198f,0.12407559f,0.49757237f,0.43191921f,0.58161568f,
				0.29822916f,0.59362260f,0.48436532f,0.07354940f,0.47882312f,0.18211779f,0.77290120f,0.31444388f,0.65967189f,0.82142358f,
				0.00625909f,0.15241912f,0.98940339f,0.69982586f,0.44954660f,0.35087976f,0.28454552f,0.09612152f,0.20659472f,0.76181108f,
				0.16336774f,0.37027136f,0.00978422f,0.22499851f,0.66946820f,0.42727746f,0.13317369f,0.82220887f,0.38824157f,0.09238429f,
				0.62881795f,0.21555435f,0.16397278f,0.98828691f,0.45746016f,0.94976910f,0.35774012f,0.26356933f,0.33382049f,0.16369721f,
				0.27998504f,0.08127238f,0.31398781f,0.19448816f,0.29177392f,0.97011356f,0.47148485f,0.20763412f,0.48130305f,0.88311257f,
				0.09612956f,0.42962822f,0.84578847f,0.31847224f,0.76862290f,0.69477389f,0.96365726f,0.01018699f,0.93736944f,0.80747693f,
				0.25592297f,0.38723879f,0.16115121f,0.21605446f,0.72957923f,0.90235076f,0.03195122f,0.44453770f,0.22194130f,0.33330701f,
				0.77114095f,0.74282683f,0.14303992f,0.23988805f,0.14775657f,0.18162041f,0.49997339f,0.96842207f,0.07406797f,0.62782913f,
				0.23554325f,0.45340530f,0.42556245f,0.97339421f,0.57248204f,0.11141864f,0.10186091f,0.81801978f,0.83056086f,0.27835211f,
				0.55380051f,0.77844226f,0.39720338f,0.53136029f,0.67734164f,0.94087184f,0.75420821f,0.85656935f,0.00229690f,0.95273074f,
				0.47971513f,0.10730032f,0.87339962f,0.92212211f,0.85290141f,0.25099773f,0.34062482f,0.71467999f,0.40682394f,0.17799751f,
				0.41100793f,0.13014569f,0.35745591f,0.30845415f,0.22231372f,0.25965399f,0.92075791f,0.02076467f,0.98044630f,0.91943680f,
				0.78235025f,0.37357911f,0.13679549f,0.97043721f,0.30093124f,0.86478567f,0.19493548f,0.63036415f,0.14905490f,0.05837347f,
				0.77623653f,0.95024157f,0.26439572f,0.66653493f,0.42203128f,0.72704222f,0.50683353f,0.32318111f,0.59879133f,0.46779583f,
				0.32836164f,0.48138006f,0.52418999f,0.66056820f,0.43479057f,0.01949963f,0.22702437f,0.30813484f,0.50143227f,0.70892399f,
				0.98020988f,0.30621895f,0.68118412f,0.36675204f,0.54984922f,0.24104729f,0.14684265f,0.50909238f,0.78559013f,0.53622996f,
				0.55080299f,0.15321599f,0.86966691f,0.46446327f,0.12201202f,0.15986804f,0.14911213f,0.72444093f,0.31161870f,0.56214745f,
				0.33724884f,0.68692765f,0.70877367f,0.64728116f,0.02632895f,0.45126151f,0.93857030f,0.16934737f,0.93188643f,0.85053739f,
				0.45712825f,0.81876592f,0.93961330f,0.24646309f,0.75085731f,0.33538764f,0.40110417f,0.85224655f,0.16525454f,0.16677283f,
				0.12352412f,0.05578758f,0.81339043f,0.18607844f,0.18848471f,0.64523704f,0.64196796f,0.21016454f,0.59427976f,0.79996321f,
				0.69495124f,0.43485847f,0.17519345f,0.66644428f,0.85166464f,0.10394800f,0.39572871f,0.37438296f,0.89853126f,0.74654252f
			};
			s32 n = sizeof(float_list)/sizeof(xcore::f32);

			g_qsort(float_list, n, sizeof(xcore::f32), f32_compare);
			for (s32 i=0; i<(n-1); ++i)
				CHECK_TRUE(float_list[i] <= float_list[i+1]);
		}

		UNITTEST_TEST(sort3)
		{

		}
	}
}
UNITTEST_SUITE_END
