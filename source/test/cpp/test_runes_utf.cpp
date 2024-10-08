#include "cbase/c_allocator.h"
#include "cbase/c_carray.h"
#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"
#include "cbase/c_runes.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_string_utf)
{
    UNITTEST_FIXTURE(utf8)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_TEST(type_size_checks)
        {
            CHECK_EQUAL(1, sizeof(uchar));
            CHECK_EQUAL(1, sizeof(uchar8));
            CHECK_EQUAL(2, sizeof(uchar16));
            CHECK_EQUAL(4, sizeof(uchar32));
            CHECK_EQUAL(1, sizeof(ascii::rune));
            CHECK_EQUAL(1, sizeof(utf8::rune));
            CHECK_EQUAL(2, sizeof(utf16::rune));
            CHECK_EQUAL(4, sizeof(utf32::rune));
        }

        UNITTEST_TEST(peek_ascii)
        {
            const ascii::rune str[] = {0x66, 0x72, (uchar)0xC3, (uchar)0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src = make_crunes(str);
            nrunes::reader_t  reader(src);
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
        }

        UNITTEST_TEST(peek_utf8)
        {
            const utf8::rune str[] = {0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t         src = make_crunes(str);
            nrunes::reader_t reader(src);
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
        }

        UNITTEST_TEST(peek_utf16)
        {
            const utf16::rune str[] = {0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src = make_crunes(str);
            nrunes::reader_t  reader(src);
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
        }

        UNITTEST_TEST(peek_utf32)
        {
            const utf32::rune str[] = {0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src = make_crunes(str);
            nrunes::reader_t  reader(src);
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
        }

        UNITTEST_TEST(read_ascii)
        {
            const ascii::rune str[] = {0x66, 0x72, (uchar)0xC3, (uchar)0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src = make_crunes(str);
            nrunes::reader_t  reader(src);
            CHECK_EQUAL('f', reader.read());
            CHECK_EQUAL('r', reader.read());
            CHECK_EQUAL(0xC3, reader.read());
            CHECK_EQUAL(0xA9, reader.read());
            CHECK_EQUAL('n', reader.read());
            CHECK_EQUAL('c', reader.read());
            CHECK_EQUAL('h', reader.read());
            CHECK_EQUAL(ascii::TERMINATOR, reader.read());
            CHECK_EQUAL(ascii::TERMINATOR, reader.read());
        }

        UNITTEST_TEST(read_utf8)
        {
            const utf8::rune str[] = {0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t         src = make_crunes(str);
            nrunes::reader_t reader(src);
            CHECK_EQUAL('f', reader.read());
            CHECK_EQUAL('r', reader.read());
            CHECK_EQUAL(0xE9, reader.read());
            CHECK_EQUAL('n', reader.read());
            CHECK_EQUAL('c', reader.read());
            CHECK_EQUAL('h', reader.read());
            CHECK_EQUAL(cEOS, reader.read());
            CHECK_EQUAL(cEOS, reader.read());
        }

        UNITTEST_TEST(read_utf16)
        {
            //\u{66}\u{72}\u{e9}\u{6e}\u{63}\u{68}
            const utf16::rune str[] = {0x66, 0x72, 0xE9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src = make_crunes(str);
            nrunes::reader_t  reader(src);
            CHECK_EQUAL('f', reader.read());
            CHECK_EQUAL('r', reader.read());
            CHECK_EQUAL(0xE9, reader.read());
            CHECK_EQUAL('n', reader.read());
            CHECK_EQUAL('c', reader.read());
            CHECK_EQUAL('h', reader.read());
            CHECK_EQUAL(cEOS, reader.read());
            CHECK_EQUAL(cEOS, reader.read());
        }

        UNITTEST_TEST(read_utf32)
        {
            const utf32::rune str[] = {0x66, 0x72, 0xE9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src = make_crunes(str);
            nrunes::reader_t  reader(src);
            CHECK_EQUAL('f', reader.read());
            CHECK_EQUAL('r', reader.read());
            CHECK_EQUAL(0xE9, reader.read());
            CHECK_EQUAL('n', reader.read());
            CHECK_EQUAL('c', reader.read());
            CHECK_EQUAL('h', reader.read());
            CHECK_EQUAL(cEOS, reader.read());
            CHECK_EQUAL(cEOS, reader.read());
        }

        UNITTEST_TEST(copy_ascii_to_utf32)
        {
            crunes_t src = make_crunes("test");
            CHECK_EQUAL('t', src.m_ascii[src.m_str]);
            CHECK_EQUAL('e', src.m_ascii[src.m_str + 1]);

            utf32::rune dst_runes[16];
            runes_t     dst = make_runes(dst_runes, 0, 0, 16);
            nrunes::copy(src, dst);

            CHECK_EQUAL('t', dst.m_utf32[dst.m_str].r);
        }

        UNITTEST_TEST(read1_utf8)
        {
            crunes_t         str1 = make_crunes((utf8::pcrune) "test");
            crunes_t         str11 = make_crunes((ascii::pcrune) "test");
            nrunes::reader_t reader1(str1);
            for (s32 i = 0; i < 5; ++i)
            {
                uchar32 ch = reader1.read();
                CHECK_EQUAL(str11.m_utf8[str11.m_str + i].r, ch);
            }

            const utf8::rune str2[] = {0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch

            crunes_t         str = make_crunes(str2, nullptr);
            nrunes::reader_t reader(str);
            uchar32          c;
            c = reader.read();
            c = reader.read();
            c = reader.read();

            str =  make_crunes(str2);
            nrunes::reader_t reader2(str);
            for (s32 i = 0; i < 6; ++i)
            {
                c = reader2.read();
                if (i == 2)
                {
                    CHECK_EQUAL(0xe9, c);
                }
                else if (i > 2)
                {
                    CHECK_EQUAL(c, (uchar32)str2[i + 1].r);
                }
                else
                {
                    CHECK_EQUAL(c, (uchar32)str2[i].r);
                }
            }
        }
    }

    UNITTEST_FIXTURE(utf32)
    {
        UNITTEST_TEST(type_size_checks)
        {
            CHECK_EQUAL(1, sizeof(uchar));
            CHECK_EQUAL(1, sizeof(uchar8));
            CHECK_EQUAL(2, sizeof(uchar16));
            CHECK_EQUAL(4, sizeof(uchar32));
        }
    }
}
UNITTEST_SUITE_END
