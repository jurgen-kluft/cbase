#include "xbase\x_debug.h"
#include "xbase\x_integer.h"
#include "xbase\x_memory_std.h"
#include "xbase\x_string_ascii.h"
#include "xbase\x_string_utf.h"


namespace xcore
{
	namespace utf
	{
		typedef u8  char8_t;
		typedef u16 char16_t;
		typedef u32 char32_t;

		typedef char32_t codepoint_type;

		inline bool validate_codepoint(char32_t c)
		{
            if (c < 0xd800) { return true; }
            if (c < 0xe000) { return false; }
            if (c < 0x110000) { return true; }
            return false;
        }

		struct utf8
		{
            static size_t read_length(char8_t c)
			{
                if ((c & 0x80) == 0x00) { return 1; }
                if ((c & 0xe0) == 0xc0) { return 2; }
                if ((c & 0xf0) == 0xe0) { return 3; }
                if ((c & 0xf8) == 0xf0) { return 4; }
                return 1;
            }

            static size_t write_length(char32_t c)
			{
                if (c <= 0x7f) { return 1; }
                if (c < 0x0800) { return 2; }
                if (c < 0xd800) { return 3; }
                if (c < 0xe000) { return 0; }
                if (c < 0x010000) { return 3; }
                if (c < 0x110000) { return 4; }
                return 0;
            }

            // responsible only for validating the utf8 encoded subsequence, not the codepoint it maps to
            static bool validate(char8_t const* first, char8_t const* last)
			{
                size_t len = last - first;
                char8_t lead = (char8_t)*first;
                switch (len)
				{
                    case 1:
                        if ((lead & 0x80) != 0x00) { return false; }
                        break;
                    case 2:
                        if ((lead & 0xe0) != 0xc0) { return false; }
                        break;
                    case 3:
                        if ((lead & 0xf0) != 0xe0) { return false; }
                        break;
                    case 4:
                        if ((lead & 0xf8) != 0xf0) { return false; }
                        break;
                    default:
                        return false;
                }

                for (size_t i = 1; i < len; ++i)
				{
                    char8_t c = static_cast<char8_t>(first[i]);
                    if ((c & 0xc0) != 0x80) { return false; }
                }

                // check for overlong encodings
                switch (len)
				{
                    case 2:
                        if (((char8_t)*first) <= 0xc1) { return false; }
                        break;
                    case 3:
                        if (((char8_t)*first) == 0xe0) { return false; }
                        break;
                    case 4:
                        if (((char8_t)*first) == 0xf0 && ((char8_t)first[1]) < 0x90) { return false; }
                        break;
                    default: break;
                }

                return true;
            }

            static char8_t * encode(codepoint_type c, char8_t * dest)
			{
                size_t len = write_length(c);

                char8_t res[4] = {};

                // loop to catch remaining
                for (size_t i = len; i != 1; --i)
				{
                    // select lower 6 bits
                    res[i-1] = (c & 0x3f) | 0x80;
                    c = c >> 6;
                }

                // switch on first byte
                switch (len)
				{
                    case 1: res[0] = c; break;
                    case 2: res[0] = c | 0xc0; break;
                    case 3: res[0] = c | 0xe0; break;
                    case 4: res[0] = c | 0xf0; break;
                    default:
                        assert(false && "bad utf8 codeunit");
                };

                for (size_t i = 0; i < len; ++i)
				{
                    *dest = res[i];
                    ++dest;
                }

                return dest;
            }

            static char32_t decode(char8_t const* c)
			{
                size_t len = read_length(static_cast<char8_t>(*c));

                char32_t res = 0;
                // switch on first byte
                switch (len)
				{
                    case 1: res = *c; break;
                    case 2: res = *c & 0x1f; break;
                    case 3: res = *c & 0x0f; break;
                    case 4: res = *c & 0x07; break;
                    default:
                        assert(false && "bad utf8 codeunit");
                };

                // then loop to catch remaining?
                for (size_t i = 1; i < len; ++i)
				{
                    res = (res << 6) | (c[i] & 0x3f);
                }
                return res;
            }
        };

        struct utf16
		{
            static size_t read_length(char16_t c)
			{
                if (c < 0xd800) { return 1; }
                if (c < 0xdc00) { return 2; }
                return 1;
            }

            static size_t write_length(char32_t c)
			{
                if (c < 0xd800) { return 1; }
                if (c < 0xe000) { return 0; }
                if (c < 0x010000) { return 1; }
                if (c < 0x110000) { return 2; }

                return 0;
            }

            static bool validate(char16_t const* first, char16_t const* last)
			{
                size_t len = last - first;
                switch (len)
				{
                    case 1:
                    {
                        char16_t lead = *first;
                        if (lead >= 0xd800 && lead < 0xe000) { return false; }
                        break;
                    }
                    case 2:
                    {
                        char16_t lead = first[0];
                        char16_t trail = first[1];
                        if (lead < 0xd800 || lead >= 0xdc00) { return false; }
                        if (trail < 0xdc00 || trail >= 0xe000) { return false; }
                        break;
                    }
                    default:
                        return false;
                }
                return true;
            }

            static char16_t* encode(char32_t c, char16_t* dest)
			{
                size_t len = write_length(c);

                if (len == 1)
				{
                    *dest = c;
                    ++dest;
                    return dest;
                }

                // 20-bit intermediate value
                size_t tmp = c - 0x10000;

                *dest = static_cast<char16_t>((tmp >> 10) + 0xd800);
                ++dest;
                *dest = static_cast<char16_t>((tmp & 0x03ff) + 0xdc00);
                ++dest;
                return dest;
            }

            static char32_t decode(char16_t const* c)
			{
                size_t len = read_length(*c);

                char16_t lead = *c;
                if (len == 1)
				{
                    return lead;
                }

                char32_t res = 0;
                // 10 most significant bits
                res = (lead - 0xd800) << 10;
                char16_t trail = c[1];
                // 10 least significant bits
                res +=  (trail - 0xdc00);
                return res + 0x10000;
            }
        };


		struct utf32
		{
            static size_t read_length(char32_t c) { return 1; }
            static size_t write_length(char32_t c)
			{
                if (c < 0xd800) { return 1; }
                if (c < 0xe000) { return 0; }
                if (c < 0x110000) { return 1; }
                return 0;
            }

            static bool validate(const char32_t* first, const char32_t* last)
			{
                // actually looking at the cp value is done by free validate function.
                return (last - first) == 1;
            }

            static char32_t* encode(char32_t c, char32_t* dest)
			{
                *dest = c;
                ++dest;
                return dest;
            }

            static char32_t decode(char32_t const* c)
			{
                return *c;
            }
        };

	}
};

/**
 *  END xCore namespace
 */
