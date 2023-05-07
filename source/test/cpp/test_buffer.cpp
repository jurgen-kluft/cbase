#include "cbase/c_carray.h"
#include "cbase/c_buffer.h"
#include "cbase/c_runes.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_buffer_t)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(test_buffer32)
        {
            sbuffer_t<32> buf1;
            sbuffer_t<32> buf2;
            buf1.reset(1);
            buf2.reset(2);

            CHECK_TRUE(buf1 != buf2);
            CHECK_FALSE(buf1 == buf2);

            CHECK_TRUE(buf1 < buf2);
            CHECK_TRUE(buf2 > buf1);

            buf1 = buf2;
            CHECK_TRUE(buf1 == buf2);
            CHECK_FALSE(buf1 != buf2);
        }

        UNITTEST_TEST(test_binary_reader)
        {
            sbuffer_t<2048>   buffer;
            binary_writer_t writer = buffer.writer();

            CHECK_EQUAL(0, writer.skip(16));
            CHECK_EQUAL(16, writer.write(false));
            CHECK_EQUAL(17, writer.write((u8)1));
            CHECK_EQUAL(18, writer.write((s8)1));
            CHECK_EQUAL(19, writer.write((u16)2));
            CHECK_EQUAL(21, writer.write((s16)3));
            CHECK_EQUAL(23, writer.write((u32)4));
            CHECK_EQUAL(27, writer.write((s32)5));
            CHECK_EQUAL(31, writer.write((u64)6));
            CHECK_EQUAL(39, writer.write((s64)7));
            CHECK_EQUAL(47, writer.write((f32)8.0));
            CHECK_EQUAL(51, writer.write((f64)9.0));
			CHECK_EQUAL(59, writer.pos());

            const char* cctext = "this is the buffer";
            cbuffer_t    text(ascii::strlen(cctext), (const u8*)cctext);

            sbuffer_t<32> bytes;
            bytes.writer().write_data(text);
            writer.write_data(text);
            writer.write_data(bytes);

            binary_reader_t reader = (buffer.reader());

            bool the_bool;
            u8   the_u8;
            s8   the_s8;
            u16  the_u16;
            s16  the_s16;
            u32  the_u32;
            s32  the_s32;
            u64  the_u64;
            s64  the_s64;
            f32  the_f32;
            f64  the_f64;

            CHECK_EQUAL(0, reader.skip(16));
            CHECK_EQUAL(16, reader.read(the_bool));
            CHECK_EQUAL(17, reader.read(the_u8));
            CHECK_EQUAL(18, reader.read(the_s8));
            CHECK_EQUAL(19, reader.read(the_u16));
            CHECK_EQUAL(21, reader.read(the_s16));
            CHECK_EQUAL(23, reader.read(the_u32));
            CHECK_EQUAL(27, reader.read(the_s32));
            CHECK_EQUAL(31, reader.read(the_u64));
            CHECK_EQUAL(39, reader.read(the_s64));
            CHECK_EQUAL(47, reader.read(the_f32));
            CHECK_EQUAL(51, reader.read(the_f64));
            CHECK_EQUAL(59, reader.pos());

            CHECK_EQUAL(false, the_bool);
            CHECK_EQUAL(1, the_u8);
            CHECK_EQUAL(1, the_s8);
            CHECK_EQUAL(2, the_u16);
            CHECK_EQUAL(3, the_s16);
            CHECK_EQUAL(4, the_u32);
            CHECK_EQUAL(5, the_s32);
            CHECK_EQUAL(6, the_u64);
            CHECK_EQUAL(7, the_s64);
            CHECK_EQUAL(8.0, the_f32);
            CHECK_EQUAL(9.0, the_f64);

            s32         len = ascii::strlen(cctext);
            sbuffer_t<32> rdata;
            buffer_t    rdatalen = rdata(0, len);
            reader.read_data(rdatalen);
			CHECK_EQUAL(rdatalen.size(), len);

            cbuffer_t rviewdata;
            reader.view_data(32, rviewdata);
			CHECK_EQUAL(rviewdata.size(), 32);

        }
    }
}
UNITTEST_SUITE_END
