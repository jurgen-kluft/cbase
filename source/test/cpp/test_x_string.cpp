#include "xbase\x_target.h"
#include "xbase\x_types.h"
#include "xbase\x_string.h"

#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xstring)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		char	strBuffer1[512];

		UNITTEST_TEST(Constructors)
		{
			xstring str1(4);
			str1 = "aaaa";
			CHECK_TRUE(str1=="aaaa");
			CHECK_EQUAL(4, str1.getLength());

			xstring str2("a c style string");
			CHECK_TRUE(str2=="a c style string");
			CHECK_EQUAL(16, str2.getLength());

			xstring str3("a c style string", 9);
			CHECK_TRUE(str3=="a c style");
			CHECK_EQUAL(9, str3.getLength());

			xstring str4("12345", 5, "6789", 4);
			CHECK_TRUE(str4=="123456789");
			CHECK_EQUAL(9, str4.getLength());

			xstring str5(str4);
			CHECK_TRUE(str5=="123456789");
			CHECK_EQUAL(9, str5.getLength());

			xstring str6(xccstring("abcdefghijkl"));
			CHECK_TRUE(str6=="abcdefghijkl");
		}

		UNITTEST_TEST(Clear)
		{
			xstring str1(4);
			str1 = "aaaa";
			CHECK_TRUE(str1=="aaaa");
			str1.clear();
			CHECK_TRUE(str1=="");
			CHECK_TRUE(str1.getLength()==0);
		}

		UNITTEST_TEST(Length)
		{
			xstring str1(4);
			str1 = "aaaa";
			CHECK_TRUE(str1.getLength()==4);
			str1.clear();
			CHECK_TRUE(str1.getLength()==0);
			str1 += 'a';
			CHECK_TRUE(str1.getLength()==1);
			str1.clear();
			CHECK_TRUE(str1.getLength()==0);
			str1 += "abcdefghijklmnopqrstuvwxyz";
			CHECK_TRUE(str1.getLength()==26);
		}

		UNITTEST_TEST(IsEmpty)
		{
			xstring str1(4);
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
			xstring str1(4);
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
			xstring str1(4);
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
			xstring str1("aaaa");
			CHECK_FALSE(str1.isCapitalized());
			str1[0] = 'A';
			CHECK_TRUE(str1.isCapitalized());
		}

		UNITTEST_TEST(IsQuoted)
		{
			xstring str1("\"aaaa\"");
			CHECK_TRUE(str1.isQuoted());
			xstring str2("aaaa\"");
			CHECK_FALSE(str2.isQuoted());
			xstring str3("\"aaaa");
			CHECK_FALSE(str3.isQuoted());
		}

		UNITTEST_TEST(IsQuotedQuote)
		{
			xstring str1("\"aaaa\"");
			CHECK_TRUE(str1.isQuoted('\"'));
			xstring str2("|aaaa|");
			CHECK_TRUE(str2.isQuoted('|'));
			xstring str3("@aaaa@");
			CHECK_FALSE(str3.isQuoted('#'));
		}

		UNITTEST_TEST(IsDelimited)
		{
			xstring str1("<aaaa>");
			CHECK_TRUE(str1.isDelimited('<', '>'));
			xstring str2("|aaaa|");
			CHECK_TRUE(str2.isDelimited('|', '|'));
			xstring str3("{aaaa}");
			CHECK_FALSE(str3.isDelimited('[', ']'));
		}

		UNITTEST_TEST(GetAt)
		{
			xstring str1("<abcd>");
			CHECK_TRUE(str1.getAt(0)=='<');
			CHECK_TRUE(str1.getAt(1)=='a');
			CHECK_TRUE(str1.getAt(2)=='b');
			CHECK_TRUE(str1.getAt(3)=='c');
			CHECK_TRUE(str1.getAt(4)=='d');
			CHECK_TRUE(str1.getAt(5)=='>');
		}

		UNITTEST_TEST(FirstChar)
		{
			xstring str1("<abcd>");
			CHECK_TRUE(str1.firstChar()=='<');
		}

		UNITTEST_TEST(LastChar)
		{
			xstring str1("<abcd>");
			CHECK_TRUE(str1.lastChar()=='>');
		}

		UNITTEST_TEST(StartsWith)
		{
			xstring str1("<abcd>");
			CHECK_TRUE(str1.startsWith("<ab"));
			CHECK_FALSE(str1.startsWith("ab"));
			xstring str2("123456789ABCDEFGHIJ");
			CHECK_TRUE(str2.startsWith("123456789AB"));
			CHECK_FALSE(str2.startsWith("23456789AB"));
		}

		UNITTEST_TEST(EndsWith)
		{
			xstring str1("<abcd>");
			CHECK_TRUE(str1.endsWith("cd>"));
			CHECK_FALSE(str1.endsWith("ce>"));
			xstring str2("123456789ABCDEFGHIJ");
			CHECK_TRUE(str2.endsWith("DEFGHIJ"));
			CHECK_FALSE(str2.endsWith("DEFGHI"));
		}

		UNITTEST_TEST(Compare1)
		{
			xstring str1("bcd");
			CHECK_TRUE(str1.compare("bcd")==0);
			CHECK_TRUE(str1.compare("abcd")==1);
			CHECK_TRUE(str1.compare("cd")==-1);

			xstring str2("BCD");
			CHECK_TRUE(str2.compare("BCD")==0);
			CHECK_TRUE(str2.compare("ABCD")==1);
			CHECK_TRUE(str2.compare("CD")==-1);
		}

		UNITTEST_TEST(Compare2)
		{
			xstring str1("5678");
			CHECK_TRUE(str1.compare(xstring("5678"))==0);
			CHECK_TRUE(str1.compare(xstring("4567"))==1);
			CHECK_TRUE(str1.compare(xstring("6789"))==-1);

			xstring str2("[]{}");
			CHECK_TRUE(str2.compare(xstring("[]{}"))==0);
			CHECK_TRUE(str2.compare(xstring("===="))==1);
			CHECK_TRUE(str2.compare(xstring("{}[]"))==-1);
		}

		UNITTEST_TEST(CompareNoCase1)
		{
			xstring str1("bcd");
			CHECK_TRUE(str1.compareNoCase("bcd")==0);
			CHECK_TRUE(str1.compareNoCase("abcd")==1);
			CHECK_TRUE(str1.compareNoCase("cd")==-1);

			xstring str2("BCD");
			CHECK_TRUE(str2.compareNoCase("BCD")==0);
			CHECK_TRUE(str2.compareNoCase("ABCD")==1);
			CHECK_TRUE(str2.compareNoCase("CD")==-1);
		}

		UNITTEST_TEST(CompareNoCase2)
		{
			xstring str1("wxyz");
			CHECK_TRUE(str1.compareNoCase(xstring("wxyz"))==0);
			CHECK_TRUE(str1.compareNoCase(xstring("vwxy"))==1);
			CHECK_TRUE(str1.compareNoCase(xstring("xyz"))==-1);

			xstring str2("WXYZ");
			CHECK_TRUE(str2.compareNoCase(xstring("WXYZ"))==0);
			CHECK_TRUE(str2.compareNoCase(xstring("VWXY"))==1);
			CHECK_TRUE(str2.compareNoCase(xstring("XYZ"))==-1);
		}

		UNITTEST_TEST(IsEqual1)
		{
			xstring str1("bcd");
			CHECK_TRUE(str1.isEqual("bcd"));
			CHECK_FALSE(str1.isEqual("abcd"));
			CHECK_FALSE(str1.isEqual("cd"));

			xstring str2("BCD");
			CHECK_TRUE(str2.isEqual("BCD"));
			CHECK_FALSE(str2.isEqual("ABCD"));
			CHECK_FALSE(str2.isEqual("CD"));
		}

		UNITTEST_TEST(IsEqual2)
		{
			xstring str1("5678");
			CHECK_TRUE(str1.isEqual(xstring("5678")));
			CHECK_FALSE(str1.isEqual(xstring("4567")));
			CHECK_FALSE(str1.isEqual(xstring("6789")));

			xstring str2("[]{}");
			CHECK_TRUE(str2.isEqual(xstring("[]{}")));
			CHECK_FALSE(str2.isEqual(xstring("====")));
			CHECK_FALSE(str2.isEqual(xstring("{}[]")));
		}

		UNITTEST_TEST(IsEqualNoCase1)
		{
			xstring str1("bcd");
			CHECK_TRUE(str1.isEqualNoCase("bcd"));
			CHECK_TRUE(str1.isEqualNoCase("BCD"));
			CHECK_FALSE(str1.isEqualNoCase("abcd"));
			CHECK_FALSE(str1.isEqualNoCase("cd"));

			xstring str2("BCD");
			CHECK_TRUE(str2.isEqualNoCase("BCD"));
			CHECK_TRUE(str2.isEqualNoCase("bcd"));
			CHECK_FALSE(str2.isEqualNoCase("ABCD"));
			CHECK_FALSE(str2.isEqualNoCase("CD"));
		}

		UNITTEST_TEST(IsEqualNoCase2)
		{
			xstring str1("bcd");
			CHECK_TRUE(str1.isEqualNoCase(xstring("bcd")));
			CHECK_TRUE(str1.isEqualNoCase(xstring("BCD")));
			CHECK_FALSE(str1.isEqualNoCase(xstring("abcd")));
			CHECK_FALSE(str1.isEqualNoCase(xstring("cd")));

			xstring str2("BCD");
			CHECK_TRUE(str2.isEqualNoCase(xstring("BCD")));
			CHECK_TRUE(str2.isEqualNoCase(xstring("bcd")));
			CHECK_FALSE(str2.isEqualNoCase(xstring("ABCD")));
			CHECK_FALSE(str2.isEqualNoCase(xstring("CD")));
		}

		UNITTEST_TEST(Find1)
		{
			xstring str1("bcd");
			CHECK_TRUE(str1.find('d')==2);
			CHECK_TRUE(str1.find('e')==-1);

			xstring str2("BCD");
			CHECK_FALSE(str2.find('d')==2);
			CHECK_TRUE(str2.find('e')==-1);
		}

		UNITTEST_TEST(Find2)
		{
			xstring str1("bcd");
			CHECK_TRUE(str1.find("d")==2);
			CHECK_TRUE(str1.find("e")==-1);
			CHECK_TRUE(str1.find("cd")==1);
			CHECK_TRUE(str1.find("bcd")==0);

			xstring str2("BCD");
			CHECK_FALSE(str2.find("d")==2);
			CHECK_TRUE(str2.find("e")==-1);
			CHECK_FALSE(str2.find("cd")==1);
			CHECK_FALSE(str2.find("bcd")==0);
		}

		UNITTEST_TEST(FindNoCase1)
		{
			xstring str1("bcd");
			CHECK_TRUE(str1.findNoCase('d')==2);
			CHECK_TRUE(str1.findNoCase('e')==-1);

			xstring str2("BCD");
			CHECK_TRUE(str2.findNoCase('d')==2);
			CHECK_TRUE(str2.findNoCase('e')==-1);
		}

		UNITTEST_TEST(FindNoCase2)
		{
			xstring str1("bcd");
			CHECK_TRUE(str1.findNoCase("d")==2);
			CHECK_TRUE(str1.findNoCase("e")==-1);
			CHECK_TRUE(str1.findNoCase("cd")==1);
			CHECK_TRUE(str1.findNoCase("bcd")==0);

			xstring str2("BCD");
			CHECK_TRUE(str2.findNoCase("d")==2);
			CHECK_TRUE(str2.findNoCase("e")==-1);
			CHECK_TRUE(str2.findNoCase("cd")==1);
			CHECK_TRUE(str2.findNoCase("bcd")==0);
		}

		UNITTEST_TEST(FindInSubstr1)
		{
			xstring str1("bcd123456789efg");
			CHECK_EQUAL(-1, str1.find('e', 0, 12));
			CHECK_EQUAL( 6, str1.find('4', 3, 9));

			xstring str2("BCDE");
			CHECK_EQUAL( 3, str2.find('E', 1, 3));
		}

		UNITTEST_TEST(FindInSubstr2)
		{
			xstring str1("bcd123456789efg");
			CHECK_EQUAL(-1, str1.find("e", 0, 12));
			CHECK_EQUAL( 6, str1.find("456", 3, 9));

			xstring str2("BCDEFG");
			CHECK_EQUAL( 3, str2.find("E", 3, 3));
		}

		UNITTEST_TEST(FindNoCaseInSubstr1)
		{
			xstring str1("bcd123456789efg");
			CHECK_EQUAL(-1, str1.findNoCase('e', 0, 12));
			CHECK_EQUAL( 6, str1.findNoCase('4', 3, 9));

			xstring str2("BCDEFG");
			CHECK_EQUAL( 3, str2.findNoCase('E', 3, 3));
		}

		UNITTEST_TEST(FindNoCaseInSubstr2)
		{
			xstring str1("bcd123456789efg");
			CHECK_EQUAL(-1, str1.findNoCase("e", 0, 12));
			CHECK_EQUAL( 6, str1.findNoCase("456", 3, 9));

			xstring str2("BCDEFG");
			CHECK_EQUAL( 3, str2.findNoCase("E", 3, 3));
		}

		UNITTEST_TEST(RFind)
		{
			xstring str1("efg123454321efg");
			CHECK_EQUAL(12, str1.rfind('e'));
			CHECK_EQUAL(11, str1.rfind('1'));
			CHECK_EQUAL(-1, str1.rfind('h'));
			CHECK_EQUAL(-1, str1.rfind('G'));

			CHECK_EQUAL( 8, str1.rfind('4', 12));
			CHECK_EQUAL( 6, str1.rfind('4', 7));
		}

		UNITTEST_TEST(FindOneOf)
		{
			xstring str1("efg1234a4321efg");
			CHECK_EQUAL( 7, str1.findOneOf("klmna"));
			CHECK_EQUAL(-1, str1.findOneOf("klmnopq"));
		}

		UNITTEST_TEST(RFindOneOf)
		{
			xstring str1("efg1234a4321efg");
			CHECK_EQUAL(13, str1.rfindOneOf("kfmna"));
			CHECK_EQUAL(-1, str1.rfindOneOf("xyzw"));
		}

		UNITTEST_TEST(IndexOf)
		{
			xstring str1("efg123454321efg");
			CHECK_EQUAL( 0, str1.indexOf('e'));
			CHECK_EQUAL( 3, str1.indexOf('1'));
			CHECK_EQUAL(-1, str1.indexOf('h'));
			CHECK_EQUAL(-1, str1.indexOf('G'));

			CHECK_EQUAL( 8, str1.indexOf('4', 7));
			CHECK_EQUAL( 6, str1.indexOf('4', 1));
		}

		UNITTEST_TEST(LastIndexOf)
		{
			xstring str1("efg123454321efg");
			CHECK_EQUAL(12, str1.lastIndexOf('e'));
			CHECK_EQUAL(11, str1.lastIndexOf('1'));
			CHECK_EQUAL(-1, str1.lastIndexOf('h'));
			CHECK_EQUAL(-1, str1.lastIndexOf('G'));

			CHECK_EQUAL( 8, str1.lastIndexOf('4', 12));
			CHECK_EQUAL( 6, str1.lastIndexOf('4', 7));
		}

		UNITTEST_TEST(contains1)
		{
			xstring str1("efg123454321efg");
			CHECK_EQUAL(str1.contains('1'), true);
			CHECK_EQUAL(str1.contains('z'), false);
			CHECK_EQUAL(str1.contains('E'), false);
		}

		UNITTEST_TEST(contains2)
		{
			xstring str1("efg123454321efg");
			CHECK_EQUAL(str1.contains("123"), true);
			CHECK_EQUAL(str1.contains("456"), false);
			CHECK_EQUAL(str1.contains("12345", 3), true);
			CHECK_EQUAL(str1.contains("EF"), false);
			CHECK_EQUAL(str1.contains("EFG", 2), false);
		}

		UNITTEST_TEST(containsNoCase)
		{
			xstring str1("efg123454321efg");
			CHECK_EQUAL(str1.containsNoCase("EFG"), true);
			CHECK_EQUAL(str1.containsNoCase("456"), false);
			CHECK_EQUAL(str1.containsNoCase("EF"), true);
		}


		UNITTEST_TEST(repeat1)
		{
			xstring str1;
			str1.repeat("wiki", 3);
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");
			str1.clear();
			str1.repeat("wikiwiki", 3, 3);
			CHECK_EQUAL(str1.c_str(), "wikwikwik");
		}

		UNITTEST_TEST(repeat2)
		{
			xstring str1;
			str1.repeat(xstring("wiki"), 3);
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(setAt1)
		{
			xstring str1("wikiwiki");
			str1.setAt(4, 't');
			CHECK_EQUAL(str1.c_str(), "wikitiki");
		}

		UNITTEST_TEST(setAt2)
		{
			xstring str1("wiki2wiki");
			str1.setAt(4, "wiki", 4);
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(setAt3)
		{
			xstring str1("wiki22wiki");
			str1.setAt(4, 2, "wiki", 4);
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(replace1)
		{
			xstring str1("wiki2wiki");
			str1.replace((s32)4, "wiki");
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xstring str2("wiki2wiki");
			str2.replace((s32)4, "wiki", 4);
			CHECK_EQUAL(str2.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(replace2)
		{
			xstring str1("wiki2wiki");
			str1.replace(4, 1, "wiki");
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xstring str2("wiki22wiki");
			str2.replace(4, 2, "wiki", 4);
			CHECK_EQUAL(str2.c_str(), "wikiwikiwiki");
		}

		UNITTEST_TEST(replace3)
		{
			xstring str1("wikikokowiki");
			str1.replace("koko", "wiki");
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xstring str2("wikiwokowokowiki");
			str2.replace("woko", "wiki", 5);
			CHECK_EQUAL(str2.c_str(), "wikiwokowikiwiki");
		}

		UNITTEST_TEST(replace4)
		{
			xstring str1("wikiAwiki");
			str1.replace('A', "wiki");
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xstring str2("wikiBwikiB");
			str2.replace('B', "wiki", 5);
			CHECK_EQUAL(str2.c_str(), "wikiBwikiwiki");
		}

		UNITTEST_TEST(replace5)
		{
			xstring str1("wikiWIKIwiki");
			str1.replace("WIKI", '-');
			CHECK_EQUAL(str1.c_str(), "wiki-wiki");

			xstring str2("wikiTIKIwikiTIKI");
			str2.replace("TIKI", '-', 5);
			CHECK_EQUAL(str2.c_str(), "wikiTIKIwiki-");
		}

		UNITTEST_TEST(replace6)
		{
			xstring str1("wikiqikiwiki");
			str1.replace('q', 'w');
			CHECK_EQUAL(str1.c_str(), "wikiwikiwiki");

			xstring str2("wikiqikiwikiqiki");
			str2.replace('q', 'w', 5);
			CHECK_EQUAL(str2.c_str(), "wikiqikiwikiwiki");
		}


		UNITTEST_TEST(insert1)
		{
			xstring str1("iki");
			str1.insert('w');
			CHECK_EQUAL(str1.c_str(), "wiki");
		}

		UNITTEST_TEST(insert2)
		{
			xstring str1("iki");
			str1.insert("wikiw");
			CHECK_EQUAL(str1.c_str(), "wikiwiki");

			xstring str2("iki");
			str2.insert("wikiwiki", 5);
			CHECK_EQUAL(str2.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(insert3)
		{
			xstring str1("iki");
			str1.insert(xstring("wikiw"));
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(insert4)
		{
			xstring str1("wk");
			str1.insert(1, 'i');
			str1.insert(3, 'i');
			CHECK_EQUAL(str1.c_str(), "wiki");
		}

		UNITTEST_TEST(insert5)
		{
			xstring str1("wk");
			str1.insert(1, "i");
			str1.insert(3, "iwiki");
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(insert6)
		{
			xstring str1("wk");
			str1.insert(1, xstring("i"));
			str1.insert(3, xstring("iwiki"));
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(remove1)
		{
			xstring str1("wikitokowiki");
			str1.remove(4, 4);
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(remove2)
		{
			xstring str1("wikitobowiki");
			str1.remove("tob");
			CHECK_EQUAL(str1.c_str(), "wikiwiki");
		}

		UNITTEST_TEST(upper)
		{
			xstring str1("WikiWiki12");
			str1.upper();
			CHECK_EQUAL(str1.c_str(), "WIKIWIKI12");
		}

		UNITTEST_TEST(lower)
		{
			xstring str1("WikiWiki12");
			str1.lower();
			CHECK_EQUAL(str1.c_str(), "wikiwiki12");
		}

		UNITTEST_TEST(capitalize1)
		{
			xstring str1("wikiWiki12W");
			str1.capitalize();
			CHECK_EQUAL(str1.c_str(), "Wikiwiki12w");
		}

		UNITTEST_TEST(capitalize2)
		{
			xstring str1("wiKi wikI,tikI");
			str1.capitalize(" ,");
			CHECK_EQUAL(str1.c_str(), "Wiki Wiki,Tiki");
		}

		UNITTEST_TEST(trim1)
		{
			xstring str1(" wiki wiki ");
			str1.trim();
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimLeft1)
		{
			xstring str1(" wiki wiki ");
			str1.trimLeft();
			CHECK_EQUAL(str1.c_str(), "wiki wiki ");
		}

		UNITTEST_TEST(trimRight1)
		{
			xstring str1(" wiki wiki ");
			str1.trimRight();
			CHECK_EQUAL(str1.c_str(), " wiki wiki");
		}

		UNITTEST_TEST(trim2)
		{
			xstring str1("#wiki wiki#");
			str1.trim('#');
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimLeft2)
		{
			xstring str1("#wiki wiki#");
			str1.trimLeft('#');
			CHECK_EQUAL(str1.c_str(), "wiki wiki#");
		}

		UNITTEST_TEST(trimRight2)
		{
			xstring str1("#wiki wiki#");
			str1.trimRight('#');
			CHECK_EQUAL(str1.c_str(), "#wiki wiki");
		}

		UNITTEST_TEST(trim3)
		{
			xstring str1("<wiki wiki>");
			str1.trim("<>");
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimLeft3)
		{
			xstring str1("<!wiki wiki!>");
			str1.trimLeft("<!#");
			CHECK_EQUAL(str1.c_str(), "wiki wiki!>");
		}

		UNITTEST_TEST(trimRight3)
		{
			xstring str1("<!wiki wiki!>");
			str1.trimRight("<!#>");
			CHECK_EQUAL(str1.c_str(), "<!wiki wiki");
		}

		UNITTEST_TEST(trimQuotes1)
		{
			xstring str1("\"wiki wiki\"");
			str1.trimQuotes();
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimQuotes2)
		{
			xstring str1("$wiki wiki$");
			str1.trimQuotes('$');
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
		}

		UNITTEST_TEST(trimDelimiters)
		{
			xstring str1("<wiki wiki>");
			str1.trimDelimiters('<', '>');
			CHECK_EQUAL(str1.c_str(), "wiki wiki");
			xstring str2("{wiki wiki}");
			str2.trimDelimiters('<', '>');
			CHECK_EQUAL(str2.c_str(), "{wiki wiki}");
		}

		UNITTEST_TEST(reverse1)
		{
			xstring str1("wiki wiki");
			str1.reverse();
			CHECK_EQUAL(str1.c_str(), "ikiw ikiw");
		}

		UNITTEST_TEST(reverse2)
		{
			xstring str1("wiki wiki");
			str1.reverse(5,4);
			CHECK_EQUAL(str1.c_str(), "wiki ikiw");
		}


		UNITTEST_TEST(left)
		{
			xstring str1("leftpart of this string");
			xstring left1;
			str1.left(8, left1);
			CHECK_EQUAL(left1.c_str(), "leftpart");
			xcstring left2(strBuffer1, sizeof(strBuffer1));
			str1.left(8, left2);
			CHECK_EQUAL(left2.c_str(), "leftpart");
		}

		UNITTEST_TEST(right)
		{
			xstring str1("this string has a rightpart");
			xstring right;
			str1.right(9, right);
			CHECK_EQUAL(right.c_str(), "rightpart");
		}

		UNITTEST_TEST(mid)
		{
			xstring str1("this is the midpart of this string");
			xstring mid;
			str1.mid(12, mid, 7);
			CHECK_EQUAL(mid.c_str(), "midpart");
		}

		UNITTEST_TEST(substring1)
		{
			xstring str1("this is a subpart of this string");
			xstring sub;
			str1.substring(10,sub, 7);
			CHECK_EQUAL(sub.c_str(), "subpart");
		}

		UNITTEST_TEST(substring2)
		{
			xstring str1("this is a subpart of this string");
			xstring sub;
			str1.substring(10,sub);
			CHECK_EQUAL(sub.c_str(), "subpart of this string");
		}


		UNITTEST_TEST(splitOn1)
		{
			xstring str1("leftpart,right,part");
			xstring left,right;
			str1.splitOn(',', left, right);
			CHECK_EQUAL(left.c_str(), "leftpart");
			CHECK_EQUAL(right.c_str(), "right,part");
		}

		UNITTEST_TEST(splitOn2)
		{
			xstring str1("leftpart,right,part");
			xstring right;
			str1.splitOn(',', right);
			CHECK_EQUAL(str1.c_str(), "leftpart");
			CHECK_EQUAL(right.c_str(), "right,part");
		}

		UNITTEST_TEST(rsplitOn1)
		{
			xstring str1("left,part,rightpart");
			xstring left,right;
			str1.rsplitOn(',', left, right);
			CHECK_EQUAL(left.c_str(), "left,part");
			CHECK_EQUAL(right.c_str(), "rightpart");
		}

		UNITTEST_TEST(rsplitOn2)
		{
			xstring str1("left,part,rightpart");
			xstring right;
			str1.rsplitOn(',', right);
			CHECK_EQUAL(str1.c_str(), "left,part");
			CHECK_EQUAL(right.c_str(), "rightpart");
		}

		UNITTEST_TEST(split1)
		{
			xstring str1("leftpart,rightpart");
			xstring left;
			xstring right;
			str1.split(8, xTRUE, left, right);
			CHECK_EQUAL(left.c_str(), "leftpart");
			CHECK_EQUAL(right.c_str(), "rightpart");
			str1.split(8, xFALSE, left, right);
			CHECK_EQUAL(left.c_str(), "leftpart");
			CHECK_EQUAL(right.c_str(), ",rightpart");
		}

		UNITTEST_TEST(split2)
		{
			xstring str1("leftpart,rightpart");
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
			xstring str1("part1\npart2\npart3");
			
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
			xstring str1;
			str1.copy("a string");
			CHECK_EQUAL(str1.c_str(), "a string");
		}

		UNITTEST_TEST(copy2)
		{
			xstring str1;
			str1.copy("a string without tail", 8);
			CHECK_EQUAL(str1.c_str(), "a string");
		}

		UNITTEST_TEST(copy3)
		{
			xstring str1;
			str1.copy(xstring("a xstring"));
			CHECK_EQUAL(str1.c_str(), "a xstring");
		}

		UNITTEST_TEST(ConstIndexer)
		{
			xstring str1("indexer");
			CHECK_EQUAL('d', str1[2]);
			CHECK_EQUAL('r', str1[6]);
		}

		UNITTEST_TEST(MutableIndexer)
		{
			xstring str1("indexer");
			CHECK_EQUAL('d', str1[2]);
			str1[2] = 'z';
			CHECK_EQUAL('z', str1[2]);
			CHECK_EQUAL('r', str1[6]);
			str1[6] = 'a';
			CHECK_EQUAL('a', str1[6]);
		}

		UNITTEST_TEST(Assignment1)
		{
			xstring str1;
			str1 = "indexer1";
			CHECK_EQUAL(str1.c_str(), "indexer1");
		}
		
		UNITTEST_TEST(Assignment2)
		{
			xstring str1;
			str1 = xstring("indexer2");
			CHECK_EQUAL(str1.c_str(), "indexer2");
		}
		
		UNITTEST_TEST(Assignment3)
		{
			xstring str1;
			str1 = xstring_tmp("indexer3");
			CHECK_EQUAL(str1.c_str(), "indexer3");
		}

		UNITTEST_TEST(Assignment4)
		{
			xstring str1;
			str1 = xccstring("indexer4");
			CHECK_EQUAL(str1.c_str(), "indexer4");
		}

		UNITTEST_TEST(AddAssign1)
		{
			xstring str1;
			str1 += 'a';
			CHECK_EQUAL(str1.c_str(), "a");
			CHECK_EQUAL(1, str1.getLength());
			str1 += 'b';
			CHECK_EQUAL(str1.c_str(), "ab");
			CHECK_EQUAL(2, str1.getLength());
		}

		UNITTEST_TEST(AddAssign2)
		{
			xstring str1;
			str1 += "ab";
			CHECK_EQUAL(str1.c_str(), "ab");
			CHECK_EQUAL(2, str1.getLength());
			str1 += "01";
			CHECK_EQUAL(str1.c_str(), "ab01");
			CHECK_EQUAL(4, str1.getLength());
		}

		UNITTEST_TEST(AddAssign3)
		{
			xstring str1;
			str1 += xstring("1234");
			CHECK_EQUAL(str1.c_str(), "1234");
			CHECK_EQUAL(4, str1.getLength());
			str1 += xstring("5678");
			CHECK_EQUAL(str1.c_str(), "12345678");
			CHECK_EQUAL(8, str1.getLength());
		}

		UNITTEST_TEST(AddAssign4)
		{
			xstring str1;
			str1 += xstring_tmp("1234");
			CHECK_EQUAL(str1.c_str(), "1234");
			CHECK_EQUAL(4, str1.getLength());
			str1 += xstring_tmp("5678");
			CHECK_EQUAL(str1.c_str(), "12345678");
			CHECK_EQUAL(8, str1.getLength());
		}

		UNITTEST_TEST(AddAssign5)
		{
			xstring str1;
			str1 += xccstring("1234");
			CHECK_EQUAL(str1.c_str(), "1234");
			CHECK_EQUAL(4, str1.getLength());
			str1 += xccstring("5678");
			CHECK_EQUAL(str1.c_str(), "12345678");
			CHECK_EQUAL(8, str1.getLength());
		}
	}
}
UNITTEST_SUITE_END
