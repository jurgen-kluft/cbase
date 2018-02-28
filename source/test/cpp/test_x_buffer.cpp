#include "xbase/x_carray.h"
#include "xbase/x_buffer.h"
#include "xbase/x_string_ascii.h"
#include "xunittest/xunittest.h"

using namespace xcore;


UNITTEST_SUITE_BEGIN(xbuffer)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test_xbuffer32)
		{
			xbytes32 buf1(1);
			xbytes32 buf2(2);

			CHECK_TRUE(buf1 != buf2);
			CHECK_FALSE(buf1 == buf2);

			CHECK_TRUE(buf1 < buf2);
			CHECK_TRUE(buf2 > buf1);

			buf1 = buf2;
			CHECK_TRUE(buf1 == buf2);
			CHECK_FALSE(buf1 != buf2);
		}

		UNITTEST_TEST(test_xbinary_reader)
		{
			xbyte data[2048];
			xbuffer buffer(2048, data);

			xbinary_writer writer(buffer);
			writer.skip(16);
			writer.write(false);
			writer.write((u8)1);
			writer.write(( s8)1);
			writer.write((u16)2);
			writer.write((s16)3);
			writer.write((u32)4);
			writer.write((s32)5);
			writer.write((u64)6);
			writer.write((s64)7);
			writer.write((f32)8.0);
			writer.write((f64)9.0);

			const char* cctext = "this is the buffer";
			xcuchars chars(cctext);
			xcbuffer text(ascii::len(cctext, NULL), (xbyte const*)cctext);

			xbytes32 bytes;
			bytes = text;
			writer.write_data(text);
			writer.write_data(bytes.buffer());
			writer.write_string(cctext);

			xbinary_reader reader(buffer.cbuffer());
			
			reader.skip(16);

			bool the_bool;
			u8  the_u8 ;
			s8  the_s8 ;
			u16 the_u16;
			s16 the_s16;
			u32 the_u32;
			s32 the_s32;
			u64 the_u64;
			s64 the_s64;
			f32 the_f32;
			f64 the_f64;

			reader.read(the_bool);
			reader.read(the_u8);
			reader.read(the_s8);
			reader.read(the_u16);
			reader.read(the_s16);
			reader.read(the_u32);
			reader.read(the_s32);
			reader.read(the_u64);
			reader.read(the_s64);
			reader.read(the_f32);
			reader.read(the_f64);

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

			xuchars32 rtext;
			reader.read_data(rtext.buffer());
			xcbuffer rview;
			reader.view_data(ascii::len(cctext, NULL), rview);
			xcuchars rviewtext;
			reader.view_string(rviewtext);
		}
	}
}
UNITTEST_SUITE_END
