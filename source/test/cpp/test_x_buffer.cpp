#include "xbase/x_carray.h"
#include "xbase/x_buffer.h"
#include "xbase/x_runes.h"
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
			xbytes<32> buf1;
			xbytes<32> buf2;
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

		UNITTEST_TEST(test_xbinary_reader)
		{
			xbytes<2048> buffer;
			xbinary_writer writer = buffer.writer();

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
			ascii::crunes chars(cctext);
			xcbuffer text(chars.size(), (xbyte const*)cctext);

			xbytes<32> bytes;
			bytes.writer().write_data(text);
			writer.write_data(text);
			writer.write_data(bytes);
			writer.write_string(cctext, ascii::endof(cctext, NULL));

			xbinary_reader reader= (buffer.reader());
			
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

			s32 len = chars.size();
			xbytes<32> rdata;
			xbuffer rdatalen = rdata(0, len);
			reader.read_data(len, rdatalen);

			xcbuffer rviewdata;
			reader.view_data(32, rviewdata);
			
			const char* rviewstr;
			const char* rviewend;
			reader.view_string(len, rviewstr, rviewend);
			ascii::crunes rchars(rviewstr, rviewend);
			CHECK_TRUE(chars == rchars);
		}
	}
}
UNITTEST_SUITE_END
