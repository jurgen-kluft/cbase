#include "cbase/c_hash.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(hash)
{
    UNITTEST_FIXTURE(xxhash)
    {
#define SANITY_BUFFER_SIZE 2243
#define XXHASH_TEST(buffer, size, seed, expected)                        \
    {                                                                    \
        const u64 hash = nhash::datahash64((const u8*)buffer, size, seed); \
        CHECK_EQUAL(expected, hash);                                     \
    }

        static u8        sanityBuffer[SANITY_BUFFER_SIZE];
        static const u32 prime = 2654435761U;

        UNITTEST_FIXTURE_SETUP()
        {
            u32 byteGen = prime;
            int i;
            for (i = 0; i < SANITY_BUFFER_SIZE; i++)
            {
                sanityBuffer[i] = (u8)(byteGen >> 24);
                byteGen *= byteGen;
            }
        }

        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(sanity)
        {
            XXHASH_TEST(nullptr, 0, 0, 0xEF46DB3751D8E999ULL);
            XXHASH_TEST(nullptr, 0, prime, 0xAC75FDA2929B17EFULL);
            XXHASH_TEST(sanityBuffer, 1, 0, 0x4FCE394CC88952D8ULL);
            XXHASH_TEST(sanityBuffer, 1, prime, 0x739840CB819FA723ULL);
            XXHASH_TEST(sanityBuffer, 14, 0, 0xCFFA8DB881BC3A3DULL);
            XXHASH_TEST(sanityBuffer, 14, prime, 0x5B9611585EFCC9CBULL);
            XXHASH_TEST(sanityBuffer, 222, 0, 0x9DD507880DEBB03DULL);
            XXHASH_TEST(sanityBuffer, 222, prime, 0xDC515172B8EE0600ULL);
        }

    }
}
UNITTEST_SUITE_END
