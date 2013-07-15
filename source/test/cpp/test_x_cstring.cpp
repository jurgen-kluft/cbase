#include "xbase\x_target.h"
#include "xbase\x_types.h"
#include "xbase\x_string.h"

#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xcstring)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		char	sStrBuffer1[34];
		char	sStrBuffer2[34];
		char	sStrBuffer3[34];
		char	sStrBuffer4[34];
		char	sStrBuffer5[34];
		char	sStrBuffer6[34];

		UNITTEST_TEST(Constructors)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "aaaa";
			CHECK_TRUE(str1=="aaaa");

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "a c style string");
			CHECK_TRUE(str2=="a c style string");

			xcstring str4(sStrBuffer4, sizeof(sStrBuffer4), "%s%s", "12345", "6789");
			CHECK_TRUE(str4=="123456789");

			xcstring str5(sStrBuffer5, sizeof(sStrBuffer5), str4);
			CHECK_TRUE(str5=="123456789");

			xcstring str6(sStrBuffer6, sizeof(sStrBuffer6), xccstring("abcdefghijkl"));
			CHECK_TRUE(str6=="abcdefghijkl");
		}

		UNITTEST_TEST(Clear)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "aaaa";
			CHECK_TRUE(str1=="aaaa");
			str1.clear();
			CHECK_TRUE(str1=="");
			CHECK_TRUE(str1.getLength()==0);
		}

		UNITTEST_TEST(Length)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "aaaa";
			CHECK_EQUAL(4, str1.getLength());
			str1.clear();
			CHECK_EQUAL(0, str1.getLength());
			str1 += 'a';
			CHECK_EQUAL(1, str1.getLength());
			str1.clear();
			CHECK_EQUAL(0, str1.getLength());
			str1 += "abcdefghijklmnopqrstuvwxyz";
			CHECK_EQUAL(26, str1.getLength());
		}

		UNITTEST_TEST(IsEmpty)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "aaaa";
			CHECK_TRUE(!str1.isEmpty());
			str1.clear();
			CHECK_TRUE(str1.isEmpty());
			str1 += 'a';
			CHECK_TRUE(!str1.isEmpty());
			str1.clear();
			CHECK_TRUE(str1.isEmpty());
			str1 += "abcdefghijklmnopqrstuvwxyz";
			CHECK_TRUE(!str1.isEmpty());
		}

		UNITTEST_TEST(IsUpper)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "aaaa";
			CHECK_TRUE(!str1.isUpper());
			str1 = "AAb";
			CHECK_TRUE(!str1.isUpper());
			str1.clear();
			CHECK_TRUE(str1.isUpper());
			str1 += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			CHECK_TRUE(str1.isUpper());
		}

		UNITTEST_TEST(IsLower)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "aaaa";
			CHECK_TRUE(str1.isLower());
			str1 = "AAb";
			CHECK_TRUE(!str1.isLower());
			str1.clear();
			CHECK_TRUE(str1.isLower());
			str1 += "abcdefghijklmnopqrstuvwxyz";
			CHECK_TRUE(str1.isLower());
		}

		UNITTEST_TEST(IsCapatilized)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "aaaa");
			CHECK_FALSE(str1.isCapitalized());
			str1[0] = 'A';
			CHECK_TRUE(str1.isCapitalized());
		}

		UNITTEST_TEST(IsQuoted)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "\"aaaa\"");
			CHECK_TRUE(str1.isQuoted());
			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "aaaa\"");
			CHECK_FALSE(str2.isQuoted());
			xcstring str3(sStrBuffer3, sizeof(sStrBuffer3), "\"aaaa");
			CHECK_FALSE(str3.isQuoted());
		}

		UNITTEST_TEST(IsQuotedQuote)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "\"aaaa\"");
			CHECK_TRUE(str1.isQuoted('\"'));
			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "|aaaa|");
			CHECK_TRUE(str2.isQuoted('|'));
			xcstring str3(sStrBuffer3, sizeof(sStrBuffer3), "@aaaa@");
			CHECK_FALSE(str3.isQuoted('#'));
		}

		UNITTEST_TEST(IsDelimited)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<aaaa>");
			CHECK_TRUE(str1.isDelimited('<', '>'));
			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "|aaaa|");
			CHECK_TRUE(str2.isDelimited('|', '|'));
			xcstring str3(sStrBuffer3, sizeof(sStrBuffer3), "{aaaa}");
			CHECK_FALSE(str3.isDelimited('[', ']'));
		}

		UNITTEST_TEST(GetAt)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<abcd>");
			CHECK_TRUE(str1.getAt(0)=='<');
			CHECK_TRUE(str1.getAt(1)=='a');
			CHECK_TRUE(str1.getAt(2)=='b');
			CHECK_TRUE(str1.getAt(3)=='c');
			CHECK_TRUE(str1.getAt(4)=='d');
			CHECK_TRUE(str1.getAt(5)=='>');
		}

		UNITTEST_TEST(FirstChar)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<abcd>");
			CHECK_TRUE(str1.firstChar()=='<');
		}

		UNITTEST_TEST(LastChar)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<abcd>");
			CHECK_TRUE(str1.lastChar()=='>');
		}

		UNITTEST_TEST(StartsWith)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<abcd>");
			CHECK_TRUE(str1.startsWith("<ab"));
			CHECK_FALSE(str1.startsWith("ab"));
			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "123456789ABCDEFGHIJ");
			CHECK_TRUE(str2.startsWith("123456789AB"));
			CHECK_FALSE(str2.startsWith("23456789AB"));
		}

		UNITTEST_TEST(EndsWith)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<abcd>");
			CHECK_TRUE(str1.endsWith("cd>"));
			CHECK_FALSE(str1.endsWith("ce>"));
			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "123456789ABCDEFGHIJ");
			CHECK_TRUE(str2.endsWith("DEFGHIJ"));
			CHECK_FALSE(str2.endsWith("DEFGHI"));
		}

		UNITTEST_TEST(Compare1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd");
			CHECK_TRUE(str1.compare("bcd")==0);
			CHECK_TRUE(str1.compare("abcd")==1);
			CHECK_TRUE(str1.compare("cd")==-1);

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCD");
			CHECK_TRUE(str2.compare("BCD")==0);
			CHECK_TRUE(str2.compare("ABCD")==1);
			CHECK_TRUE(str2.compare("CD")==-1);
		}

		UNITTEST_TEST(Compare2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "5678");
			CHECK_TRUE(str1.compare(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "5678"))==0);
			CHECK_TRUE(str1.compare(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "4567"))==1);
			CHECK_TRUE(str1.compare(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "6789"))==-1);

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "[]{}");
			CHECK_TRUE(str2.compare(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "[]{}"))==0);
			CHECK_TRUE(str2.compare(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "===="))==1);
			CHECK_TRUE(str2.compare(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "{}[]"))==-1);
		}

		UNITTEST_TEST(CompareNoCase1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd");
			CHECK_TRUE(str1.compareNoCase("bcd")==0);
			CHECK_TRUE(str1.compareNoCase("abcd")==1);
			CHECK_TRUE(str1.compareNoCase("cd")==-1);

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCD");
			CHECK_TRUE(str2.compareNoCase("BCD")==0);
			CHECK_TRUE(str2.compareNoCase("ABCD")==1);
			CHECK_TRUE(str2.compareNoCase("CD")==-1);
		}

		UNITTEST_TEST(CompareNoCase2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wxyz");
			CHECK_TRUE(str1.compareNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "wxyz"))==0);
			CHECK_TRUE(str1.compareNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "vwxy"))==1);
			CHECK_TRUE(str1.compareNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "xyz"))==-1);

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "WXYZ");
			CHECK_TRUE(str2.compareNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "WXYZ"))==0);
			CHECK_TRUE(str2.compareNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "VWXY"))==1);
			CHECK_TRUE(str2.compareNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "XYZ"))==-1);
		}

		UNITTEST_TEST(IsEqual1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd");
			CHECK_TRUE(str1.isEqual("bcd"));
			CHECK_FALSE(str1.isEqual("abcd"));
			CHECK_FALSE(str1.isEqual("cd"));

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCD");
			CHECK_TRUE(str2.isEqual("BCD"));
			CHECK_FALSE(str2.isEqual("ABCD"));
			CHECK_FALSE(str2.isEqual("CD"));
		}

		UNITTEST_TEST(IsEqual2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "5678");
			CHECK_TRUE(str1.isEqual(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "5678")));
			CHECK_FALSE(str1.isEqual(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "4567")));
			CHECK_FALSE(str1.isEqual(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "6789")));

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "[]{}");
			CHECK_TRUE(str2.isEqual(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "[]{}")));
			CHECK_FALSE(str2.isEqual(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "====")));
			CHECK_FALSE(str2.isEqual(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "{}[]")));
		}

		UNITTEST_TEST(IsEqualNoCase1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd");
			CHECK_TRUE(str1.isEqualNoCase("bcd"));
			CHECK_TRUE(str1.isEqualNoCase("BCD"));
			CHECK_FALSE(str1.isEqualNoCase("abcd"));
			CHECK_FALSE(str1.isEqualNoCase("cd"));

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCD");
			CHECK_TRUE(str2.isEqualNoCase("BCD"));
			CHECK_TRUE(str2.isEqualNoCase("bcd"));
			CHECK_FALSE(str2.isEqualNoCase("ABCD"));
			CHECK_FALSE(str2.isEqualNoCase("CD"));
		}

		UNITTEST_TEST(IsEqualNoCase2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd");
			CHECK_TRUE(str1.isEqualNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "bcd")));
			CHECK_TRUE(str1.isEqualNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "BCD")));
			CHECK_FALSE(str1.isEqualNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "abcd")));
			CHECK_FALSE(str1.isEqualNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "cd")));

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCD");
			CHECK_TRUE(str2.isEqualNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "BCD")));
			CHECK_TRUE(str2.isEqualNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "bcd")));
			CHECK_FALSE(str2.isEqualNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "ABCD")));
			CHECK_FALSE(str2.isEqualNoCase(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "CD")));
		}

		UNITTEST_TEST(Find1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd");
			CHECK_TRUE(str1.find('d')==2);
			CHECK_TRUE(str1.find('e')==-1);

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCD");
			CHECK_FALSE(str2.find('d')==2);
			CHECK_TRUE(str2.find('e')==-1);
		}

		UNITTEST_TEST(Find2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd");
			CHECK_TRUE(str1.find("d")==2);
			CHECK_TRUE(str1.find("e")==-1);
			CHECK_TRUE(str1.find("cd")==1);
			CHECK_TRUE(str1.find("bcd")==0);

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCD");
			CHECK_FALSE(str2.find("d")==2);
			CHECK_TRUE(str2.find("e")==-1);
			CHECK_FALSE(str2.find("cd")==1);
			CHECK_FALSE(str2.find("bcd")==0);
		}

		UNITTEST_TEST(FindNoCase1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd");
			CHECK_TRUE(str1.findNoCase('d')==2);
			CHECK_TRUE(str1.findNoCase('e')==-1);

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCD");
			CHECK_TRUE(str2.findNoCase('d')==2);
			CHECK_TRUE(str2.findNoCase('e')==-1);
		}

		UNITTEST_TEST(FindNoCase2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd");
			CHECK_TRUE(str1.findNoCase("d")==2);
			CHECK_TRUE(str1.findNoCase("e")==-1);
			CHECK_TRUE(str1.findNoCase("cd")==1);
			CHECK_TRUE(str1.findNoCase("bcd")==0);

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCD");
			CHECK_TRUE(str2.findNoCase("d")==2);
			CHECK_TRUE(str2.findNoCase("e")==-1);
			CHECK_TRUE(str2.findNoCase("cd")==1);
			CHECK_TRUE(str2.findNoCase("bcd")==0);
		}

		UNITTEST_TEST(FindInSubstr1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd123456789efg");
			CHECK_EQUAL(-1, str1.find('e', 0, 12));
			CHECK_EQUAL( 6, str1.find('4', 3, 9));

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCDE");
			CHECK_EQUAL( 3, str2.find('E', 1, 3));
		}

		UNITTEST_TEST(FindInSubstr2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd123456789efg");
			CHECK_EQUAL(-1, str1.find("e", 0, 12));
			CHECK_EQUAL( 6, str1.find("456", 3, 9));

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCDEFG");
			CHECK_EQUAL( 3, str2.find("E", 3, 3));
		}

		UNITTEST_TEST(FindNoCaseInSubstr1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd123456789efg");
			CHECK_EQUAL(-1, str1.findNoCase('e', 0, 12));
			CHECK_EQUAL( 6, str1.findNoCase('4', 3, 9));

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCDEFG");
			CHECK_EQUAL( 3, str2.findNoCase('E', 3, 3));
		}

		UNITTEST_TEST(FindNoCaseInSubstr2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "bcd123456789efg");
			CHECK_EQUAL(-1, str1.findNoCase("e", 0, 12));
			CHECK_EQUAL( 6, str1.findNoCase("456", 3, 9));

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "BCDEFG");
			CHECK_EQUAL( 3, str2.findNoCase("E", 3, 3));
		}

		UNITTEST_TEST(RFind)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "efg123454321efg");
			CHECK_EQUAL(12, str1.rfind('e'));
			CHECK_EQUAL(11, str1.rfind('1'));
			CHECK_EQUAL(-1, str1.rfind('h'));
			CHECK_EQUAL(-1, str1.rfind('G'));

			CHECK_EQUAL( 8, str1.rfind('4', 12));
			CHECK_EQUAL( 6, str1.rfind('4', 7));
		}

		UNITTEST_TEST(FindOneOf)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "efg1234a4321efg");
			CHECK_EQUAL( 7, str1.findOneOf("klmna"));
			CHECK_EQUAL(-1, str1.findOneOf("klmnopq"));
		}

		UNITTEST_TEST(RFindOneOf)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "efg1234a4321efg");
			CHECK_EQUAL(13, str1.rfindOneOf("kfmna"));
			CHECK_EQUAL(-1, str1.rfindOneOf("xyzw"));
		}

		UNITTEST_TEST(IndexOf)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "efg123454321efg");
			CHECK_EQUAL( 0, str1.indexOf('e'));
			CHECK_EQUAL( 3, str1.indexOf('1'));
			CHECK_EQUAL(-1, str1.indexOf('h'));
			CHECK_EQUAL(-1, str1.indexOf('G'));

			CHECK_EQUAL( 8, str1.indexOf('4', 7));
			CHECK_EQUAL( 6, str1.indexOf('4', 1));
		}

		UNITTEST_TEST(LastIndexOf)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "efg123454321efg");
			CHECK_EQUAL(12, str1.lastIndexOf('e'));
			CHECK_EQUAL(11, str1.lastIndexOf('1'));
			CHECK_EQUAL(-1, str1.lastIndexOf('h'));
			CHECK_EQUAL(-1, str1.lastIndexOf('G'));

			CHECK_EQUAL( 8, str1.lastIndexOf('4', 12));
			CHECK_EQUAL( 6, str1.lastIndexOf('4', 7));
		}

		UNITTEST_TEST(contains1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "efg123454321efg");
			CHECK_EQUAL(str1.contains('1'), true);
			CHECK_EQUAL(str1.contains('z'), false);
			CHECK_EQUAL(str1.contains('E'), false);
		}

		UNITTEST_TEST(contains2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "efg123454321efg");
			CHECK_EQUAL(str1.contains("123"), true);
			CHECK_EQUAL(str1.contains("456"), false);
			CHECK_EQUAL(str1.contains("12345", 3), true);
			CHECK_EQUAL(str1.contains("EF"), false);
			CHECK_EQUAL(str1.contains("EFG", 2), false);
		}

		UNITTEST_TEST(containsNoCase)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "efg123454321efg");
			CHECK_EQUAL(str1.containsNoCase("EFG"), true);
			CHECK_EQUAL(str1.containsNoCase("456"), false);
			CHECK_EQUAL(str1.containsNoCase("EF"), true);
		}


		UNITTEST_TEST(repeat1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1.repeat("wiki", 3);
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");
			str1.clear();
			str1.repeat("wikiwiki", 3, 3);
			CHECK_EQUAL(str1.c_str(), "wikwikwik");
		}

		UNITTEST_TEST(repeat2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1.repeat(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "wiki"), 3);
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(setAt1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wikiwiki");
			str1.setAt(4, 't');
			CHECK_EQUAL(str1.c_str(), "wikitiki");
		}

		UNITTEST_TEST(setAt2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wiki2wiki");
			str1.setAt(4, "wiki", 4);
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(setAt3)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wiki22wiki");
			str1.setAt(4, 2, "wiki", 4);
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(replace1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wiki2wiki");
			str1.replace((s32)4, "wiki");
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "wiki2wiki");
			str2.replace((s32)4, "wiki", 4);
			CHECK_EQUAL(str2.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(replace2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wiki2wiki");
			str1.replace(4, 1, "wiki");
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "wiki22wiki");
			str2.replace(4, 2, "wiki", 4);
			CHECK_EQUAL(str2.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(replace3)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wikikokowiki");
			str1.replace("koko", "wiki");
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "wikiwokowokowiki");
			str2.replace("woko", "wiki", 5);
			CHECK_EQUAL(str2.c_str(), "wikiwokowikiwiki");
		}

		UNITTEST_TEST(replace4)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wikiAwiki");
			str1.replace('A', "wiki");
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "wikiBwikiB");
			str2.replace('B', "wiki", 5);
			CHECK_EQUAL(str2.c_str(), "wikiBwikiwiki");
		}

		UNITTEST_TEST(replace5)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wikiWIKIwiki");
			str1.replace("WIKI", '-');
			CHECK_EQUAL(str1.c_str(), "wiki-wiki");

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "wikiTIKIwikiTIKI");
			str2.replace("TIKI", '-', 5);
			CHECK_EQUAL(str2.c_str(), "wikiTIKIwiki-");
		}

		UNITTEST_TEST(replace6)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wikiqikiwiki");
			str1.replace('q', 'w');
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "wikiqikiwikiqiki");
			str2.replace('q', 'w', 5);
			CHECK_EQUAL(str2.c_str(), "wikiqikiwikiwiki");
		}

		UNITTEST_TEST(replaceAnyWith)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wi#iqi*iwi|i");
			s32 c;
			c = str1.replaceAnyWith("#*|", 'k');
			CHECK_EQUAL(str1.c_str(), "wikiqikiwiki");
			CHECK_EQUAL(c, 3);
			c = str1.replaceAnyWith("q", 'w', 6);
			CHECK_EQUAL(str1.c_str(), "wikiqikiwiki");
			CHECK_EQUAL(c, 0);
			c = str1.replaceAnyWith("q", 'w', 4, 1);
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");
			CHECK_EQUAL(c, 1);
		}

		UNITTEST_TEST(insert1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "iki");
			str1.insert('w');
			CHECK_EQUAL(str1.c_str(), "wiki");
		}

		UNITTEST_TEST(insert2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "iki";
			str1.insert("wikiw");
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
			CHECK_EQUAL(8, str1.getLength());

			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2));
			str2 = "iki";
			CHECK_EQUAL(3, str2.getLength());
			str2.insert("wikiwiki", 5);
			CHECK_EQUAL(str2.c_str(), "wikiwiki");
			CHECK_EQUAL(8, str2.getLength());
		}

		UNITTEST_TEST(insert3)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "iki";
			CHECK_EQUAL(3, str1.getLength());
			str1.insert(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "wikiw"));
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
			CHECK_EQUAL(8, str1.getLength());
		}

		UNITTEST_TEST(insert4)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "wk";
			str1.insert(1, 'i');
			str1.insert(3, 'i');
			CHECK_EQUAL(str1.c_str(), "wiki");
		}

		UNITTEST_TEST(insert5)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "wk";
			str1.insert(1, "i");
			str1.insert(3, "iwiki");
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(insert6)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1 = "wk";
			str1.insert(1, xcstring(sStrBuffer6, sizeof(sStrBuffer6), "i"));
			str1.insert(3, xcstring(sStrBuffer6, sizeof(sStrBuffer6), "iwiki"));
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(remove1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wikitokowiki");
			str1.remove(4, 4);
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(remove2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wikitobowiki");
			str1.remove("tob");
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(upper)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "WikiWiki12");
			str1.upper();
			CHECK_EQUAL(str1.c_str(), "WIKIWIKI12");
		}

		UNITTEST_TEST(lower)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "WikiWiki12");
			str1.lower();
			CHECK_EQUAL(str1.c_str(), "wikiwiki12");
		}

		UNITTEST_TEST(capitalize1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wikiWiki12W");
			str1.capitalize();
			CHECK_EQUAL(str1.c_str(), "Wikiwiki12w");
		}

		UNITTEST_TEST(capitalize2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wiKi wikI,tikI");
			str1.capitalize(" ,");
			CHECK_EQUAL(str1.c_str(), "Wiki Wiki,Tiki");
		}

		UNITTEST_TEST(trim1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), " wiki wiki ");
			str1.trim();
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimLeft1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), " wiki wiki ");
			str1.trimLeft();
			CHECK_EQUAL(str1.c_str(), "wiki wiki ");
		}

		UNITTEST_TEST(trimRight1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), " wiki wiki ");
			str1.trimRight();
			CHECK_EQUAL(str1.c_str(), " wiki wiki");
		}

		UNITTEST_TEST(trim2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "#wiki wiki#");
			str1.trim('#');
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimLeft2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "#wiki wiki#");
			str1.trimLeft('#');
			CHECK_EQUAL(str1.c_str(), "wiki wiki#");
		}

		UNITTEST_TEST(trimRight2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "#wiki wiki#");
			str1.trimRight('#');
			CHECK_EQUAL(str1.c_str(), "#wiki wiki");
		}

		UNITTEST_TEST(trim3)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<wiki wiki>");
			str1.trim("<>");
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimLeft3)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<!wiki wiki!>");
			str1.trimLeft("<!#");
			CHECK_EQUAL(str1.c_str(), "wiki wiki!>");
		}

		UNITTEST_TEST(trimRight3)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<!wiki wiki!>");
			str1.trimRight("<!#>");
			CHECK_EQUAL(str1.c_str(), "<!wiki wiki");
		}

		UNITTEST_TEST(trimQuotes1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "\"wiki wiki\"");
			str1.trimQuotes();
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimQuotes2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "$wiki wiki$");
			str1.trimQuotes('$');
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimDelimiters)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "<wiki wiki>");
			str1.trimDelimiters('<', '>');
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
			xcstring str2(sStrBuffer2, sizeof(sStrBuffer2), "{wiki wiki}");
			str2.trimDelimiters('<', '>');
			CHECK_EQUAL(str2.c_str(), "{wiki wiki}");
		}

		UNITTEST_TEST(reverse1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wiki wiki");
			str1.reverse();
			CHECK_EQUAL(str1.c_str(), "ikiw ikiw");
		}

		UNITTEST_TEST(reverse2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "wiki wiki");
			str1.reverse(5,4);
			CHECK_EQUAL(str1.c_str(), "wiki ikiw");
		}


		UNITTEST_TEST(left)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "leftpart of this string");
			xstring left;
			str1.left(8, left);
			CHECK_EQUAL(left.c_str(), "leftpart");
		}

		UNITTEST_TEST(right)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "this string has a rightpart");
			xstring right;
			str1.right(9, right);
			CHECK_EQUAL(right.c_str(), "rightpart");
		}

		UNITTEST_TEST(mid)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "this is the midpart of this string");
			xstring mid;
			str1.mid(12,mid, 7);
			CHECK_EQUAL(mid.c_str(), "midpart");
		}

		UNITTEST_TEST(substring1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "this is a subpart of this string");
			xstring sub;
			str1.substring(10,sub,7);
			CHECK_EQUAL(sub.c_str(), "subpart");
		}

		UNITTEST_TEST(substring2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "this is a subpart of this string");
			xstring sub;
			str1.substring(10, sub);
			CHECK_EQUAL("subpart of this string", sub.c_str());
		}


		UNITTEST_TEST(splitOn1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "leftpart,right,part");
			xstring left,right;
			str1.splitOn(',', left, right);
			CHECK_EQUAL(left.c_str(), "leftpart");
			CHECK_EQUAL(right.c_str(), "right,part");
		}

		UNITTEST_TEST(splitOn2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "leftpart,right,part");
			xstring right;
			str1.splitOn(',', right);
			CHECK_EQUAL(str1.c_str(), "leftpart");
			CHECK_EQUAL(right.c_str(), "right,part");
		}

		UNITTEST_TEST(rsplitOn1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "left,part,rightpart");
			xstring left,right;
			str1.rsplitOn(',', left, right);
			CHECK_EQUAL(left.c_str(), "left,part");
			CHECK_EQUAL(right.c_str(), "rightpart");
		}

		UNITTEST_TEST(rsplitOn2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "left,part,rightpart");
			xstring right;
			str1.rsplitOn(',', right);
			CHECK_EQUAL(str1.c_str(), "left,part");
			CHECK_EQUAL(right.c_str(), "rightpart");
		}

		UNITTEST_TEST(split1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "leftpart,rightpart");
			xstring left;
			xstring right;
			str1.split(8, xTRUE, left, right);
			CHECK_EQUAL(left.c_str(), "leftpart");
			CHECK_EQUAL(right.c_str(), "rightpart");
			str1.split(8, xFALSE, left, right);
			CHECK_EQUAL(left.c_str() , "leftpart"  );
			CHECK_EQUAL(right.c_str(), ",rightpart");
		}

		UNITTEST_TEST(split2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "leftpart,rightpart");
			xstring right;
			str1.split(8, xTRUE, right);
			CHECK_EQUAL(str1.c_str(), "leftpart");
			CHECK_EQUAL(right.c_str(), "rightpart");
			str1 = "leftpart,rightpart";
			str1.split(8, xFALSE, right);
			CHECK_EQUAL(str1.c_str(), "leftpart");
			CHECK_EQUAL(right.c_str(), ",rightpart");
		}

		UNITTEST_TEST(indexToRowCol)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "part1\npart2\npart3");
			
			s32 row,col;
			str1.indexToRowCol(40, row, col);
			CHECK_EQUAL(-1, row);
			CHECK_EQUAL(-1, col);
			str1.indexToRowCol(7, row, col);
			CHECK_EQUAL(2, row);
			CHECK_EQUAL(2, col);
			str1.indexToRowCol(12, row, col);
			CHECK_EQUAL(3, row);
			CHECK_EQUAL(1, col);
		}

		UNITTEST_TEST(copy1)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1.copy("a string");
			CHECK_EQUAL(str1.c_str(), "a string");
		}

		UNITTEST_TEST(copy2)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1.copy("a string without tail", 8);
			CHECK_EQUAL(str1.c_str(), "a string");
		}

		UNITTEST_TEST(copy3)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1));
			str1.copy(xcstring(sStrBuffer6, sizeof(sStrBuffer6), "a xcstring"));
			CHECK_EQUAL(str1.c_str(), "a xcstring");
		}

		UNITTEST_TEST(operator_plus_assign)
		{
			xcstring str1(sStrBuffer1, sizeof(sStrBuffer1), "test");
			str1 += "2";
			CHECK_EQUAL(str1.c_str(), "test2");
		}
	}
}
UNITTEST_SUITE_END
