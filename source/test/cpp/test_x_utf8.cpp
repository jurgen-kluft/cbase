#include "xbase\x_carray.h"
#include "xbase\x_va_list.h"
#include "xbase\x_utf8.h"
#include "xunittest\xunittest.h"

using namespace xcore;
using namespace utf8;

UNITTEST_SUITE_BEGIN(xutf8)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(x_strcpy)
		{
			//append
			unsigned char u[5] = {0,0,0,0,0};

			append(0x0448, u);
			CHECK_TRUE (u[0] == 0xd1 && u[1] == 0x88 && u[2] == 0 && u[3] == 0 && u[4] == 0);

			append(0x65e5, u);
			CHECK_TRUE (u[0] == 0xe6 && u[1] == 0x97 && u[2] == 0xa5 && u[3] == 0 && u[4] == 0);

			append(0x3044, u);
			CHECK_TRUE (u[0] == 0xe3 && u[1] == 0x81 && u[2] == 0x84 && u[3] == 0 && u[4] == 0);

			append(0x10346, u);
			CHECK_TRUE (u[0] == 0xf0 && u[1] == 0x90 && u[2] == 0x8d && u[3] == 0x86 && u[4] == 0);


			//next
			const char* twochars = "\xe6\x97\xa5\xd1\x88";
			const char* w = twochars;
			int cp = next(w, twochars + 6);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == twochars + 3);

			const char* threechars = "\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88";
			w = threechars;
			cp = next(w, threechars + 9);
			CHECK_TRUE (cp == 0x10346);
			CHECK_TRUE (w == threechars + 4);
			cp = next(w, threechars + 9);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == threechars + 7);
			cp = next(w, threechars + 9);
			CHECK_TRUE (cp == 0x0448);
			CHECK_TRUE (w == threechars + 9);

			//peek_next
			const char* const cw = twochars;
			cp = peek_next(cw, cw + 6);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (cw == twochars);

			//prior
			w = twochars + 3;
			cp = prior (w, twochars);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == twochars);

			w = threechars + 9;
			cp = prior(w, threechars);
			CHECK_TRUE (cp == 0x0448);
			CHECK_TRUE (w == threechars + 7);
			cp = prior(w, threechars);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == threechars + 4);
			cp = prior(w, threechars);
			CHECK_TRUE (cp == 0x10346);
			CHECK_TRUE (w == threechars); 

			//previous (deprecated)
			w = twochars + 3;
			cp = previous (w, twochars - 1);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == twochars);

			w = threechars + 9;
			cp = previous(w, threechars - 1);
			CHECK_TRUE (cp == 0x0448);
			CHECK_TRUE (w == threechars + 7);
			cp = previous(w, threechars -1);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == threechars + 4);
			cp = previous(w, threechars - 1);
			CHECK_TRUE (cp == 0x10346);
			CHECK_TRUE (w == threechars); 

			// advance
			w = twochars;
			advance (w, 2, twochars + 6);
			CHECK_TRUE (w == twochars + 5);

			// distance
			size_t dist = utf8::distance(twochars, twochars + 5);
			CHECK_TRUE (dist == 2);

			// utf32to8
			int utf32string[] = {0x448, 0x65E5, 0x10346, 0};
			char utf8result_data[64];
			xcarray<char> utf8result(utf8result_data, sizeof(utf8result_data));
			
			utf8result = utf32to8(utf32string, utf32string + 3, utf8result);
			CHECK_EQUAL(9, utf8result.size());

			// try it with the return value;
			char* utf8_end = utf32to8(utf32string, utf32string + 3, &utf8result[0]);
			CHECK_TRUE (utf8_end == &utf8result[0] + 9);

			//utf8to32
			int utf32result_data[64];
			xcarray<int> utf32result(utf32result_data, sizeof(utf32result_data));
			utf32result = utf8to32(twochars, twochars + 5, utf32result);
			CHECK_TRUE (utf32result.size() == 2);
			// try it with the return value;
			int* utf32_end = utf8to32(twochars, twochars + 5, &utf32result[0]);
			CHECK_TRUE (utf32_end == &utf32result[0] + 2);

			//utf16to8
			u16 utf16string[] = {0x41, 0x0448, 0x65e5, 0xd834, 0xdd1e};
			utf8result.clear();
			utf8result = utf16to8(utf16string, utf16string + 5, utf8result);
			CHECK_TRUE (utf8result.size() == 10);
			// try it with the return value;
			utf8_end = utf16to8 (utf16string, utf16string + 5, &utf8result[0]);
			CHECK_TRUE (utf8_end == &utf8result[0] + 10);

			//utf8to16
			char utf8_with_surrogates[] = "\xe6\x97\xa5\xd1\x88\xf0\x9d\x84\x9e";
			u16 utf16result_data[64];
			xcarray<u16> utf16result(utf16result_data, sizeof(utf16result_data));
			utf16result = utf8to16(utf8_with_surrogates, utf8_with_surrogates + 9, utf16result);
			CHECK_TRUE (utf16result.size() == 4);
			CHECK_TRUE (utf16result[2] == 0xd834);
			CHECK_TRUE (utf16result[3] == 0xdd1e);
			// try it with the return value;
			unsigned short* utf16_end = utf8to16 (utf8_with_surrogates, utf8_with_surrogates + 9, &utf16result[0]);
			CHECK_TRUE (utf16_end == &utf16result[0] + 4);

			//find_invalid
			char utf_invalid[] = "\xe6\x97\xa5\xd1\x88\xfa";
			char* invalid = find_invalid(utf_invalid, utf_invalid + 6);
			CHECK_TRUE (invalid == utf_invalid + 5);

			//is_valid
			bool bvalid = is_valid(utf_invalid, utf_invalid + 6);
			CHECK_TRUE (bvalid == false);
			bvalid = is_valid(utf8_with_surrogates, utf8_with_surrogates + 9);
			CHECK_TRUE (bvalid == true);

			//starts_with_bom
			unsigned char byte_order_mark[] = {0xef, 0xbb, 0xbf};
			bool bbom = starts_with_bom(byte_order_mark, byte_order_mark + sizeof(byte_order_mark));
			CHECK_TRUE (bbom == true);
			bool no_bbom = starts_with_bom(threechars, threechars + sizeof(threechars));
			CHECK_TRUE (no_bbom == false);

			//replace_invalid
			char invalid_sequence[] = "a\x80\xe0\xa0\xc0\xaf\xed\xa0\x80z";
			char replace_invalid_result_data[64];
			xcarray<char> replace_invalid_result(replace_invalid_result_data, sizeof(replace_invalid_result_data));
			replace_invalid (invalid_sequence, invalid_sequence + sizeof(invalid_sequence), replace_invalid_result, '?');
			bvalid = is_valid(replace_invalid_result.begin(), replace_invalid_result.end());
			CHECK_TRUE (bvalid);
			const char* fixed_invalid_sequence = "a????z";
			CHECK_TRUE (std::equal(replace_invalid_result.begin(), replace_invalid_result.begin() + sizeof(fixed_invalid_sequence), fixed_invalid_sequence));

			// iterator
			utf8::iterator<const char*> it(threechars, threechars, threechars + 9);
			utf8::iterator<const char*> it2 = it;
			CHECK_TRUE (it2 == it);
			CHECK_TRUE (*it == 0x10346);
			CHECK_TRUE (*(++it) == 0x65e5);
			CHECK_TRUE ((*it++) == 0x65e5);
			CHECK_TRUE (*it == 0x0448);
			CHECK_TRUE (it != it2);
			utf8::iterator<const char*> endit (threechars + 9, threechars, threechars + 9);  
			CHECK_TRUE (++it == endit);
			CHECK_TRUE (*(--it) == 0x0448);
			CHECK_TRUE ((*it--) == 0x0448);
			CHECK_TRUE (*it == 0x65e5);
			CHECK_TRUE (--it == utf8::iterator<const char*>(threechars, threechars, threechars + 9));
			CHECK_TRUE (*it == 0x10346);

			//////////////////////////////////////////////////////////
			//// Unchecked variants
			//////////////////////////////////////////////////////////

			//append
			memset(u, 0, 5);
			append(0x0448, u);
			CHECK_TRUE (u[0] == 0xd1 && u[1] == 0x88 && u[2] == 0 && u[3] == 0 && u[4] == 0);

			append(0x65e5, u);
			CHECK_TRUE (u[0] == 0xe6 && u[1] == 0x97 && u[2] == 0xa5 && u[3] == 0 && u[4] == 0);

			append(0x10346, u);
			CHECK_TRUE (u[0] == 0xf0 && u[1] == 0x90 && u[2] == 0x8d && u[3] == 0x86 && u[4] == 0);

			//next
			w = twochars;
			cp = unchecked::next(w);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == twochars + 3);

			w = threechars;
			cp = unchecked::next(w);
			CHECK_TRUE (cp == 0x10346);
			CHECK_TRUE (w == threechars + 4);
			cp = unchecked::next(w);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == threechars + 7);
			cp = unchecked::next(w);
			CHECK_TRUE (cp == 0x0448);
			CHECK_TRUE (w == threechars + 9);

			//peek_next
			cp = unchecked::peek_next(cw);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (cw == twochars);


			//previous (calls prior internally)

			w = twochars + 3;
			cp = unchecked::previous (w);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == twochars);

			w = threechars + 9;
			cp = unchecked::previous(w);
			CHECK_TRUE (cp == 0x0448);
			CHECK_TRUE (w == threechars + 7);
			cp = unchecked::previous(w);
			CHECK_TRUE (cp == 0x65e5);
			CHECK_TRUE (w == threechars + 4);
			cp = unchecked::previous(w);
			CHECK_TRUE (cp == 0x10346);
			CHECK_TRUE (w == threechars); 

			// advance
			w = twochars;
			unchecked::advance (w, 2);
			CHECK_TRUE (w == twochars + 5);

			// distance
			dist = unchecked::distance(twochars, twochars + 5);
			CHECK_TRUE (dist == 2);

			// utf32to8
			utf8result.clear();
			utf8result = unchecked::utf32to8(utf32string, utf32string + 3, utf8result);
			CHECK_EQUAL (9, utf8result.size());
			// try it with the return value;
			utf8_end = utf32to8(utf32string, utf32string + 3, &utf8result[0]);
			CHECK_TRUE(utf8_end == &utf8result[0] + 9);

			//utf8to32
			utf32result.clear();
			utf32result = unchecked::utf8to32(twochars, twochars + 5, utf32result);
			CHECK_EQUAL (2, utf32result.size());
			// try it with the return value;
			utf32_end = utf8to32(twochars, twochars + 5, &utf32result[0]);
			CHECK_TRUE (utf32_end == &utf32result[0] + 2);

			//utf16to8
			utf8result.clear();
			utf8result = unchecked::utf16to8(utf16string, utf16string + 5, utf8result);
			CHECK_EQUAL (10, utf8result.size());
			// try it with the return value;
			utf8_end = utf16to8 (utf16string, utf16string + 5, &utf8result[0]);
			CHECK_TRUE (utf8_end == &utf8result[0] + 10);

			//utf8to16
			utf16result.clear();
			utf16result = unchecked::utf8to16(utf8_with_surrogates, utf8_with_surrogates + 9, utf16result);
			CHECK_TRUE (utf16result.size() == 4);
			CHECK_TRUE (utf16result[2] == 0xd834);
			CHECK_TRUE (utf16result[3] == 0xdd1e);
			// try it with the return value;
			utf16_end = utf8to16 (utf8_with_surrogates, utf8_with_surrogates + 9, &utf16result[0]);
			CHECK_TRUE (utf16_end == &utf16result[0] + 4);
	
			// iterator
			utf8::unchecked::iterator<const char*> un_it(threechars);
			utf8::unchecked::iterator<const char*> un_it2 = un_it;
			CHECK_TRUE (un_it2 == un_it);
			CHECK_TRUE (*un_it == 0x10346);
			CHECK_TRUE (*(++un_it) == 0x65e5);
			CHECK_TRUE ((*un_it++) == 0x65e5);
			CHECK_TRUE (un_it != un_it2);
			CHECK_TRUE (*un_it == 0x0448);
			utf8::unchecked::iterator<const char*> un_endit (threechars + 9);  
			CHECK_TRUE (++un_it == un_endit);
			CHECK_TRUE (*(--un_it) == 0x0448);
			CHECK_TRUE ((*un_it--) == 0x0448);
			CHECK_TRUE (*un_it == 0x65e5);
			CHECK_TRUE (--un_it == utf8::unchecked::iterator<const char*>(threechars));
			CHECK_TRUE (*un_it == 0x10346);

		}
	}
}
UNITTEST_SUITE_END
