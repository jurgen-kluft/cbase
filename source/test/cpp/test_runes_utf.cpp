#include "cbase/c_allocator.h"
#include "cbase/c_carray.h"
#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"
#include "cbase/c_runes.h"

#include "cunittest/cunittest.h"

namespace ncore
{
    struct string8
    {
        u32     m_len;
        utf8::rune m_str[512];

        template <typename... Args>
        string8(Args... args)
            : m_len(0)
        {
            const u8 str[] = {args...};
            append(str, sizeof(str) / sizeof(str[0]));
            m_str[m_len].r = '\0';
        }

        void append(const u8* s, size_t l)
        {
            while (l > 0 && m_len < g_array_size(m_str) - 1)
            {
                m_str[m_len++].r = *s++;
                --l;
            }
        }
    };

    struct string16
    {
        u32 m_len;
        utf16::rune m_str[512];

        template <typename... Args>
        string16(Args... args)
            : m_len(0)
        {
            const int str[] = {args...};
            append(str, sizeof(str) / sizeof(str[0]));
            m_str[m_len].r = '\0';
        }

        void append(const int* s, size_t l)
        {
            while (l > 0 && m_len < g_array_size(m_str) - 1)
            {
                m_str[m_len++].r = (u16)*s++;
                --l;
            }
        }
    };

    struct string32
    {
        u32         m_len;
        utf32::rune m_str[512];

        template <typename... Args>
        string32(Args... args)
            : m_len(0)
        {
            const int str[] = {args...};
            append(str, sizeof(str) / sizeof(str[0]));
            m_str[m_len].r = '\0';
        }

        void append(const int* s, size_t l)
        {
            while (l > 0 && m_len < g_array_size(m_str) - 1)
            {
                m_str[m_len++].r = (u32)*s++;
                --l;
            }
        }
    };

    struct unicode_tuple final
    {
        const char*    test_name;
        string8        str8;
        string16       str16;
        string32       str32;
        wchar_t const* strw;
    };

    struct supported_tuple final
    {
        const char* test_name;
        string8     str8;
        string32    str32;
    };

    unicode_tuple const unicode_test_data[] = {
      {"test 1", {(u8)'\x24'}, {0x0024}, {0x00000024}, L"$"},
      {"test 2", {(u8)'\xC2', (u8)'\xA2'}, {0x00A2}, {0x000000A2}, L"¢"},
      {"test 3", {(u8)'\xE2', (u8)'\x82', (u8)'\xAC'}, {0x20AC}, {0x000020AC}, L"€"},
      {"test 4", {(u8)'\xF0', (u8)'\x90', (u8)'\x8D', (u8)'\x88'}, {0xD800, 0xDF48}, {0x00010348}, L"𐍈"},
      {"test 5", {(u8)'\xF0', (u8)'\xA4', (u8)'\xAD', (u8)'\xA2'}, {0xD852, 0xDF62}, {0x00024B62}, L"𤭢"},
      {"test 6", {(u8)'\xF0', (u8)'\x90', (u8)'\x90', (u8)'\xB7'}, {0xD801, 0xDC37}, {0x00010437}, L"𐐷"},
      {"test 7", {(u8)'\xEF', (u8)'\xAB', (u8)'\x82'}, {0xFAC2}, {0x0000FAC2}, L"輸"},
      {"test 8",
       {(u8)'\xD0', (u8)'\xAE', (u8)'\xD0', (u8)'\xBD', (u8)'\xD0', (u8)'\xB8', (u8)'\xD0', (u8)'\xBA', (u8)'\xD0', (u8)'\xBE', (u8)'\xD0', (u8)'\xB4'},
       {0x042E, 0x043D, 0x0438, 0x043A, 0x043E, 0x0434},
       {0x0000042E, 0x0000043D, 0x00000438, 0x0000043A, 0x0000043E, 0x00000434},
       L"Юникод"},

      {"test 9",
       {(u8)'\xC5', (u8)'\xAA', (u8)'\x6E', (u8)'\xC4', (u8)'\xAD', (u8)'\x63', (u8)'\xC5', (u8)'\x8D', (u8)'\x64', (u8)'\x65', (u8)'\xCC', (u8)'\xBD'},
       {0x016A, 0x006E, 0x012D, 0x0063, 0x014D, 0x0064, 0x0065, 0x033D},
       {0x0000016A, 0x0000006E, 0x0000012D, 0x00000063, 0x0000014D, 0x00000064, 0x00000065, 0x0000033D},
       L"Ūnĭcōde̽"},

      {"test 10",
       {(u8)'\xE0', (u8)'\xA4', (u8)'\xAF', (u8)'\xE0', (u8)'\xA5', (u8)'\x82', (u8)'\xE0', (u8)'\xA4', (u8)'\xA8', (u8)'\xE0', (u8)'\xA4',
        (u8)'\xBF', (u8)'\xE0', (u8)'\xA4', (u8)'\x95', (u8)'\xE0', (u8)'\xA5', (u8)'\x8B', (u8)'\xE0', (u8)'\xA4', (u8)'\xA1'},
       {0x092F, 0x0942, 0x0928, 0x093F, 0x0915, 0x094B, 0x0921},
       {0x0000092F, 0x00000942, 0x00000928, 0x0000093F, 0x00000915, 0x0000094B, 0x00000921},
       L"यूनिकोड"},

      {"test 11", {(u8)'\x41', (u8)'\xE2', (u8)'\x89', (u8)'\xA2', (u8)'\xCE', (u8)'\x91', (u8)'\x2E'}, {0x0041, 0x2262, 0x0391, 0x002E}, {0x00000041, 0x00002262, 0x00000391, 0x0000002E}, L"A≢Α."},

      {"test 12", {(u8)'\xED', (u8)'\x95', (u8)'\x9C', (u8)'\xEA', (u8)'\xB5', (u8)'\xAD', (u8)'\xEC', (u8)'\x96', (u8)'\xB4'}, {0xD55C, 0xAD6D, 0xC5B4}, {0x0000D55C, 0x0000AD6D, 0x0000C5B4}, L"한국어"},

      {"test 13", {(u8)'\xE6', (u8)'\x97', (u8)'\xA5', (u8)'\xE6', (u8)'\x9C', (u8)'\xAC', (u8)'\xE8', (u8)'\xAA', (u8)'\x9E'}, {0x65E5, 0x672C, 0x8A9E}, {0x000065E5, 0x0000672C, 0x00008A9E}, L"日本語"},

      {"test 14",
       {(u8)'\xE1', (u8)'\x9B', (u8)'\x81', (u8)'\xE1', (u8)'\x9A', (u8)'\xB3', (u8)'\xE1', (u8)'\x9B', (u8)'\xAB', (u8)'\xE1', (u8)'\x9B', (u8)'\x97', (u8)'\xE1', (u8)'\x9A', (u8)'\xA8', (u8)'\xE1', (u8)'\x9A', (u8)'\xB7',
        (u8)'\xE1', (u8)'\x9B', (u8)'\xAB', (u8)'\xE1', (u8)'\x9A', (u8)'\xB7', (u8)'\xE1', (u8)'\x9B', (u8)'\x9A', (u8)'\xE1', (u8)'\x9A', (u8)'\xA8', (u8)'\xE1', (u8)'\x9B', (u8)'\x8B', (u8)'\xE1', (u8)'\x9B', (u8)'\xAB',
        (u8)'\xE1', (u8)'\x9B', (u8)'\x96', (u8)'\xE1', (u8)'\x9A', (u8)'\xA9', (u8)'\xE1', (u8)'\x9B', (u8)'\x8F', (u8)'\xE1', (u8)'\x9A', (u8)'\xAA', (u8)'\xE1', (u8)'\x9A', (u8)'\xBE', (u8)'\xE1', (u8)'\x9B', (u8)'\xAB',
        (u8)'\xE1', (u8)'\x9A', (u8)'\xA9', (u8)'\xE1', (u8)'\x9A', (u8)'\xBE', (u8)'\xE1', (u8)'\x9B', (u8)'\x9E', (u8)'\xE1', (u8)'\x9B', (u8)'\xAB', (u8)'\xE1', (u8)'\x9A', (u8)'\xBB', (u8)'\xE1', (u8)'\x9B', (u8)'\x81',
        (u8)'\xE1', (u8)'\x9B', (u8)'\x8F', (u8)'\xE1', (u8)'\x9B', (u8)'\xAB', (u8)'\xE1', (u8)'\x9A', (u8)'\xBE', (u8)'\xE1', (u8)'\x9B', (u8)'\x96', (u8)'\xE1', (u8)'\x9B', (u8)'\xAB', (u8)'\xE1', (u8)'\x9A', (u8)'\xBB',
        (u8)'\xE1', (u8)'\x9B', (u8)'\x96', (u8)'\xE1', (u8)'\x9A', (u8)'\xAA', (u8)'\xE1', (u8)'\x9A', (u8)'\xB1', (u8)'\xE1', (u8)'\x9B', (u8)'\x97', (u8)'\xE1', (u8)'\x9B', (u8)'\x81', (u8)'\xE1', (u8)'\x9A', (u8)'\xAA',
        (u8)'\xE1', (u8)'\x9A', (u8)'\xA7', (u8)'\xE1', (u8)'\x9B', (u8)'\xAB', (u8)'\xE1', (u8)'\x9B', (u8)'\x97', (u8)'\xE1', (u8)'\x9B', (u8)'\x96', (u8)'\xE1', (u8)'\x9B', (u8)'\xAC'},
       {0x16C1, 0x16B3, 0x16EB, 0x16D7, 0x16A8, 0x16B7, 0x16EB, 0x16B7, 0x16DA, 0x16A8, 0x16CB, 0x16EB, 0x16D6, 0x16A9, 0x16CF, 0x16AA, 0x16BE, 0x16EB, 0x16A9, 0x16BE, 0x16DE,
        0x16EB, 0x16BB, 0x16C1, 0x16CF, 0x16EB, 0x16BE, 0x16D6, 0x16EB, 0x16BB, 0x16D6, 0x16AA, 0x16B1, 0x16D7, 0x16C1, 0x16AA, 0x16A7, 0x16EB, 0x16D7, 0x16D6, 0x16EC},
       {0x000016C1, 0x000016B3, 0x000016EB, 0x000016D7, 0x000016A8, 0x000016B7, 0x000016EB, 0x000016B7, 0x000016DA, 0x000016A8, 0x000016CB, 0x000016EB, 0x000016D6, 0x000016A9,
        0x000016CF, 0x000016AA, 0x000016BE, 0x000016EB, 0x000016A9, 0x000016BE, 0x000016DE, 0x000016EB, 0x000016BB, 0x000016C1, 0x000016CF, 0x000016EB, 0x000016BE, 0x000016D6,
        0x000016EB, 0x000016BB, 0x000016D6, 0x000016AA, 0x000016B1, 0x000016D7, 0x000016C1, 0x000016AA, 0x000016A7, 0x000016EB, 0x000016D7, 0x000016D6, 0x000016EC},
       L"ᛁᚳ᛫ᛗᚨᚷ᛫ᚷᛚᚨᛋ᛫ᛖᚩᛏᚪᚾ᛫ᚩᚾᛞ᛫ᚻᛁᛏ᛫ᚾᛖ᛫ᚻᛖᚪᚱᛗᛁᚪᚧ᛫ᛗᛖ᛬"},

      {"test 15",
       {(u8)'\xE1', (u8)'\x9A', (u8)'\x9B', (u8)'\xE1', (u8)'\x9A', (u8)'\x9B', (u8)'\xE1', (u8)'\x9A', (u8)'\x89', (u8)'\xE1', (u8)'\x9A', (u8)'\x91', (u8)'\xE1', (u8)'\x9A', (u8)'\x85', (u8)'\xE1', (u8)'\x9A', (u8)'\x94',
        (u8)'\xE1', (u8)'\x9A', (u8)'\x89', (u8)'\xE1', (u8)'\x9A', (u8)'\x89', (u8)'\xE1', (u8)'\x9A', (u8)'\x94', (u8)'\xE1', (u8)'\x9A', (u8)'\x8B', (u8)'\xE1', (u8)'\x9A', (u8)'\x80', (u8)'\xE1', (u8)'\x9A', (u8)'\x94',
        (u8)'\xE1', (u8)'\x9A', (u8)'\x88', (u8)'\xE1', (u8)'\x9A', (u8)'\x94', (u8)'\xE1', (u8)'\x9A', (u8)'\x80', (u8)'\xE1', (u8)'\x9A', (u8)'\x8D', (u8)'\xE1', (u8)'\x9A', (u8)'\x82', (u8)'\xE1', (u8)'\x9A', (u8)'\x90',
        (u8)'\xE1', (u8)'\x9A', (u8)'\x85', (u8)'\xE1', (u8)'\x9A', (u8)'\x91', (u8)'\xE1', (u8)'\x9A', (u8)'\x80', (u8)'\xE1', (u8)'\x9A', (u8)'\x85', (u8)'\xE1', (u8)'\x9A', (u8)'\x94', (u8)'\xE1', (u8)'\x9A', (u8)'\x8B',
        (u8)'\xE1', (u8)'\x9A', (u8)'\x8C', (u8)'\xE1', (u8)'\x9A', (u8)'\x93', (u8)'\xE1', (u8)'\x9A', (u8)'\x85', (u8)'\xE1', (u8)'\x9A', (u8)'\x90', (u8)'\xE1', (u8)'\x9A', (u8)'\x9C'},
       {0x169B, 0x169B, 0x1689, 0x1691, 0x1685, 0x1694, 0x1689, 0x1689, 0x1694, 0x168B, 0x1680, 0x1694, 0x1688, 0x1694, 0x1680, 0x168D, 0x1682, 0x1690, 0x1685, 0x1691, 0x1680, 0x1685, 0x1694, 0x168B, 0x168C, 0x1693, 0x1685, 0x1690, 0x169C},
       {0x0000169B, 0x0000169B, 0x00001689, 0x00001691, 0x00001685, 0x00001694, 0x00001689, 0x00001689, 0x00001694, 0x0000168B, 0x00001680, 0x00001694, 0x00001688, 0x00001694, 0x00001680,
        0x0000168D, 0x00001682, 0x00001690, 0x00001685, 0x00001691, 0x00001680, 0x00001685, 0x00001694, 0x0000168B, 0x0000168C, 0x00001693, 0x00001685, 0x00001690, 0x0000169C},
       L"᚛᚛ᚉᚑᚅᚔᚉᚉᚔᚋ ᚔᚈᚔ ᚍᚂᚐᚅᚑ ᚅᚔᚋᚌᚓᚅᚐ᚜"},

      {"test 16",
       {(u8)'\xE2', (u8)'\xA0', (u8)'\x8A', (u8)'\xE2', (u8)'\xA0', (u8)'\x80', (u8)'\xE2', (u8)'\xA0', (u8)'\x89', (u8)'\xE2', (u8)'\xA0', (u8)'\x81', (u8)'\xE2', (u8)'\xA0', (u8)'\x9D', (u8)'\xE2', (u8)'\xA0', (u8)'\x80', (u8)'\xE2',
        (u8)'\xA0', (u8)'\x91', (u8)'\xE2', (u8)'\xA0', (u8)'\x81', (u8)'\xE2', (u8)'\xA0', (u8)'\x9E', (u8)'\xE2', (u8)'\xA0', (u8)'\x80', (u8)'\xE2', (u8)'\xA0', (u8)'\x9B', (u8)'\xE2', (u8)'\xA0', (u8)'\x87', (u8)'\xE2', (u8)'\xA0',
        (u8)'\x81', (u8)'\xE2', (u8)'\xA0', (u8)'\x8E', (u8)'\xE2', (u8)'\xA0', (u8)'\x8E', (u8)'\xE2', (u8)'\xA0', (u8)'\x80', (u8)'\xE2', (u8)'\xA0', (u8)'\x81', (u8)'\xE2', (u8)'\xA0', (u8)'\x9D', (u8)'\xE2', (u8)'\xA0', (u8)'\x99',
        (u8)'\xE2', (u8)'\xA0', (u8)'\x80', (u8)'\xE2', (u8)'\xA0', (u8)'\x8A', (u8)'\xE2', (u8)'\xA0', (u8)'\x9E', (u8)'\xE2', (u8)'\xA0', (u8)'\x80', (u8)'\xE2', (u8)'\xA0', (u8)'\x99', (u8)'\xE2', (u8)'\xA0', (u8)'\x95', (u8)'\xE2',
        (u8)'\xA0', (u8)'\x91', (u8)'\xE2', (u8)'\xA0', (u8)'\x8E', (u8)'\xE2', (u8)'\xA0', (u8)'\x9D', (u8)'\xE2', (u8)'\xA0', (u8)'\x9E', (u8)'\xE2', (u8)'\xA0', (u8)'\x80', (u8)'\xE2', (u8)'\xA0', (u8)'\x93', (u8)'\xE2', (u8)'\xA0',
        (u8)'\xA5', (u8)'\xE2', (u8)'\xA0', (u8)'\x97', (u8)'\xE2', (u8)'\xA0', (u8)'\x9E', (u8)'\xE2', (u8)'\xA0', (u8)'\x80', (u8)'\xE2', (u8)'\xA0', (u8)'\x8D', (u8)'\xE2', (u8)'\xA0', (u8)'\x91'},
       {0x280A, 0x2800, 0x2809, 0x2801, 0x281D, 0x2800, 0x2811, 0x2801, 0x281E, 0x2800, 0x281B, 0x2807, 0x2801, 0x280E, 0x280E, 0x2800, 0x2801, 0x281D, 0x2819,
        0x2800, 0x280A, 0x281E, 0x2800, 0x2819, 0x2815, 0x2811, 0x280E, 0x281D, 0x281E, 0x2800, 0x2813, 0x2825, 0x2817, 0x281E, 0x2800, 0x280D, 0x2811},
       {0x0000280A, 0x00002800, 0x00002809, 0x00002801, 0x0000281D, 0x00002800, 0x00002811, 0x00002801, 0x0000281E, 0x00002800, 0x0000281B, 0x00002807, 0x00002801, 0x0000280E, 0x0000280E, 0x00002800, 0x00002801, 0x0000281D, 0x00002819,
        0x00002800, 0x0000280A, 0x0000281E, 0x00002800, 0x00002819, 0x00002815, 0x00002811, 0x0000280E, 0x0000281D, 0x0000281E, 0x00002800, 0x00002813, 0x00002825, 0x00002817, 0x0000281E, 0x00002800, 0x0000280D, 0x00002811},
       L"⠊⠀⠉⠁⠝⠀⠑⠁⠞⠀⠛⠇⠁⠎⠎⠀⠁⠝⠙⠀⠊⠞⠀⠙⠕⠑⠎⠝⠞⠀⠓⠥⠗⠞⠀⠍⠑"},

      {"test 17",
       {(u8)'\xD8', (u8)'\xA3', (u8)'\xD9', (u8)'\x86', (u8)'\xD8', (u8)'\xA7', (u8)'\x20', (u8)'\xD9', (u8)'\x82', (u8)'\xD8', (u8)'\xA7', (u8)'\xD8', (u8)'\xAF', (u8)'\xD8', (u8)'\xB1', (u8)'\x20', (u8)'\xD8', (u8)'\xB9',
        (u8)'\xD9', (u8)'\x84', (u8)'\xD9', (u8)'\x89', (u8)'\x20', (u8)'\xD8', (u8)'\xA3', (u8)'\xD9', (u8)'\x83', (u8)'\xD9', (u8)'\x84', (u8)'\x20', (u8)'\xD8', (u8)'\xA7', (u8)'\xD9', (u8)'\x84', (u8)'\xD8', (u8)'\xB2',
        (u8)'\xD8', (u8)'\xAC', (u8)'\xD8', (u8)'\xA7', (u8)'\xD8', (u8)'\xAC', (u8)'\x20', (u8)'\xD9', (u8)'\x88', (u8)'\x20', (u8)'\xD9', (u8)'\x87', (u8)'\xD8', (u8)'\xB0', (u8)'\xD8', (u8)'\xA7', (u8)'\x20', (u8)'\xD9',
        (u8)'\x84', (u8)'\xD8', (u8)'\xA7', (u8)'\x20', (u8)'\xD9', (u8)'\x8A', (u8)'\xD8', (u8)'\xA4', (u8)'\xD9', (u8)'\x84', (u8)'\xD9', (u8)'\x85', (u8)'\xD9', (u8)'\x86', (u8)'\xD9', (u8)'\x8A', (u8)'\x2E'},
       {0x0623, 0x0646, 0x0627, 0x0020, 0x0642, 0x0627, 0x062F, 0x0631, 0x0020, 0x0639, 0x0644, 0x0649, 0x0020, 0x0623, 0x0643, 0x0644, 0x0020, 0x0627, 0x0644, 0x0632,
        0x062C, 0x0627, 0x062C, 0x0020, 0x0648, 0x0020, 0x0647, 0x0630, 0x0627, 0x0020, 0x0644, 0x0627, 0x0020, 0x064A, 0x0624, 0x0644, 0x0645, 0x0646, 0x064A, 0x002E},
       {0x00000623, 0x00000646, 0x00000627, 0x00000020, 0x00000642, 0x00000627, 0x0000062F, 0x00000631, 0x00000020, 0x00000639, 0x00000644, 0x00000649, 0x00000020, 0x00000623, 0x00000643, 0x00000644, 0x00000020, 0x00000627, 0x00000644, 0x00000632,
        0x0000062C, 0x00000627, 0x0000062C, 0x00000020, 0x00000648, 0x00000020, 0x00000647, 0x00000630, 0x00000627, 0x00000020, 0x00000644, 0x00000627, 0x00000020, 0x0000064A, 0x00000624, 0x00000644, 0x00000645, 0x00000646, 0x0000064A, 0x0000002E},
       L"أنا قادر على أكل الزجاج و هذا لا يؤلمني."},

      {"test 18",
       {(u8)'\xE1', (u8)'\x80', (u8)'\x80', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE1', (u8)'\x80', (u8)'\x9A', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE1', (u8)'\x80', (u8)'\x9D', (u8)'\xE1', (u8)'\x80', (u8)'\x94', (u8)'\xE1',
        (u8)'\x80', (u8)'\xB9', (u8)'\xE2', (u8)'\x80', (u8)'\x8C', (u8)'\xE1', (u8)'\x80', (u8)'\x90', (u8)'\xE1', (u8)'\x80', (u8)'\xB1', (u8)'\xE1', (u8)'\x80', (u8)'\xAC', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE2', (u8)'\x80',
        (u8)'\x8C', (u8)'\xE1', (u8)'\x81', (u8)'\x8A', (u8)'\xE1', (u8)'\x80', (u8)'\x80', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE1', (u8)'\x80', (u8)'\x9A', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE1', (u8)'\x80', (u8)'\x9D',
        (u8)'\xE1', (u8)'\x80', (u8)'\x94', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE2', (u8)'\x80', (u8)'\x8C', (u8)'\xE1', (u8)'\x80', (u8)'\x99', (u8)'\x20', (u8)'\xE1', (u8)'\x80', (u8)'\x99', (u8)'\xE1', (u8)'\x80', (u8)'\xB9',
        (u8)'\xE1', (u8)'\x80', (u8)'\x9A', (u8)'\xE1', (u8)'\x80', (u8)'\x80', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE2', (u8)'\x80', (u8)'\x8C', (u8)'\xE1', (u8)'\x80', (u8)'\x85', (u8)'\xE1', (u8)'\x80', (u8)'\xAC', (u8)'\xE1',
        (u8)'\x80', (u8)'\xB8', (u8)'\xE1', (u8)'\x80', (u8)'\x94', (u8)'\xE1', (u8)'\x80', (u8)'\xAF', (u8)'\xE1', (u8)'\x80', (u8)'\xAD', (u8)'\xE1', (u8)'\x80', (u8)'\x84', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE2', (u8)'\x80',
        (u8)'\x8C', (u8)'\xE1', (u8)'\x80', (u8)'\x9E', (u8)'\xE1', (u8)'\x80', (u8)'\x8A', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE2', (u8)'\x80', (u8)'\x8C', (u8)'\xE1', (u8)'\x81', (u8)'\x8B', (u8)'\x20', (u8)'\xE1', (u8)'\x81',
        (u8)'\x8E', (u8)'\xE1', (u8)'\x80', (u8)'\x80', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE1', (u8)'\x80', (u8)'\x9B', (u8)'\xE1', (u8)'\x80', (u8)'\xB1', (u8)'\xE1', (u8)'\x80', (u8)'\xAC', (u8)'\xE1', (u8)'\x80', (u8)'\x84',
        (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE2', (u8)'\x80', (u8)'\x8C', (u8)'\xE1', (u8)'\x80', (u8)'\xB7', (u8)'\x20', (u8)'\xE1', (u8)'\x80', (u8)'\x91', (u8)'\xE1', (u8)'\x80', (u8)'\xAD', (u8)'\xE1', (u8)'\x80', (u8)'\x81',
        (u8)'\xE1', (u8)'\x80', (u8)'\xAF', (u8)'\xE1', (u8)'\x80', (u8)'\xAD', (u8)'\xE1', (u8)'\x80', (u8)'\x80', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE2', (u8)'\x80', (u8)'\x8C', (u8)'\xE1', (u8)'\x80', (u8)'\x99', (u8)'\xE1',
        (u8)'\x80', (u8)'\xB9', (u8)'\xE1', (u8)'\x80', (u8)'\x9F', (u8)'\xE1', (u8)'\x80', (u8)'\xAF', (u8)'\x20', (u8)'\xE1', (u8)'\x80', (u8)'\x99', (u8)'\xE1', (u8)'\x80', (u8)'\x9B', (u8)'\xE1', (u8)'\x80', (u8)'\xB9', (u8)'\xE1',
        (u8)'\x80', (u8)'\x9F', (u8)'\xE1', (u8)'\x80', (u8)'\xAD', (u8)'\xE1', (u8)'\x80', (u8)'\x95', (u8)'\xE1', (u8)'\x80', (u8)'\xAC', (u8)'\xE1', (u8)'\x81', (u8)'\x8B'},
       {0x1000, 0x1039, 0x101A, 0x1039, 0x101D, 0x1014, 0x1039, 0x200C, 0x1010, 0x1031, 0x102C, 0x1039, 0x200C, 0x104A, 0x1000, 0x1039, 0x101A, 0x1039, 0x101D, 0x1014, 0x1039, 0x200C, 0x1019, 0x0020, 0x1019, 0x1039,
        0x101A, 0x1000, 0x1039, 0x200C, 0x1005, 0x102C, 0x1038, 0x1014, 0x102F, 0x102D, 0x1004, 0x1039, 0x200C, 0x101E, 0x100A, 0x1039, 0x200C, 0x104B, 0x0020, 0x104E, 0x1000, 0x1039, 0x101B, 0x1031, 0x102C, 0x1004,
        0x1039, 0x200C, 0x1037, 0x0020, 0x1011, 0x102D, 0x1001, 0x102F, 0x102D, 0x1000, 0x1039, 0x200C, 0x1019, 0x1039, 0x101F, 0x102F, 0x0020, 0x1019, 0x101B, 0x1039, 0x101F, 0x102D, 0x1015, 0x102C, 0x104B},
       {0x00001000, 0x00001039, 0x0000101A, 0x00001039, 0x0000101D, 0x00001014, 0x00001039, 0x0000200C, 0x00001010, 0x00001031, 0x0000102C, 0x00001039, 0x0000200C, 0x0000104A, 0x00001000, 0x00001039, 0x0000101A, 0x00001039, 0x0000101D, 0x00001014,
        0x00001039, 0x0000200C, 0x00001019, 0x00000020, 0x00001019, 0x00001039, 0x0000101A, 0x00001000, 0x00001039, 0x0000200C, 0x00001005, 0x0000102C, 0x00001038, 0x00001014, 0x0000102F, 0x0000102D, 0x00001004, 0x00001039, 0x0000200C, 0x0000101E,
        0x0000100A, 0x00001039, 0x0000200C, 0x0000104B, 0x00000020, 0x0000104E, 0x00001000, 0x00001039, 0x0000101B, 0x00001031, 0x0000102C, 0x00001004, 0x00001039, 0x0000200C, 0x00001037, 0x00000020, 0x00001011, 0x0000102D, 0x00001001, 0x0000102F,
        0x0000102D, 0x00001000, 0x00001039, 0x0000200C, 0x00001019, 0x00001039, 0x0000101F, 0x0000102F, 0x00000020, 0x00001019, 0x0000101B, 0x00001039, 0x0000101F, 0x0000102D, 0x00001015, 0x0000102C, 0x0000104B},
       L"က္ယ္ဝန္‌တော္‌၊က္ယ္ဝန္‌မ မ္ယက္‌စားနုိင္‌သည္‌။ ၎က္ရောင္‌့ ထိခုိက္‌မ္ဟု မရ္ဟိပာ။"},

      {"test 19",
       {(u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x80', (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x81', (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x82', (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x83',
        (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x84', (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x85', (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x86', (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x87',
        (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x88', (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x89', (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x8A', (u8)'\xF0', (u8)'\x9F', (u8)'\xA0', (u8)'\x8B'},
       {0xD83E, 0xDC00, 0xD83E, 0xDC01, 0xD83E, 0xDC02, 0xD83E, 0xDC03, 0xD83E, 0xDC04, 0xD83E, 0xDC05, 0xD83E, 0xDC06, 0xD83E, 0xDC07, 0xD83E, 0xDC08, 0xD83E, 0xDC09, 0xD83E, 0xDC0A, 0xD83E, 0xDC0B},
       {0x0001F800, 0x0001F801, 0x0001F802, 0x0001F803, 0x0001F804, 0x0001F805, 0x0001F806, 0x0001F807, 0x0001F808, 0x0001F809, 0x0001F80A, 0x0001F80B},
       L"🠀🠁🠂🠃🠄🠅🠆🠇🠈🠉🠊🠋"},

      {"test 20",
       {(u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x80', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x81', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x82', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x83', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x84',
        (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x85', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x86', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x87', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x88', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x89',
        (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x8A', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x8B', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x8C', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x8D', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x8E',
        (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x8F', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x90', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x91', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x92', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x93',
        (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x94', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x95', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x96', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x97', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x98',
        (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x99', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x9A', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x9B', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x9C', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x9D',
        (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x9E', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\x9F', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA0', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA1', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA2',
        (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA3', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA4', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA5', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA6', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA7',
        (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA8', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xA9', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xAA', (u8)'\xF0', (u8)'\x9F', (u8)'\x80', (u8)'\xAB'},
       {0xD83C, 0xDC00, 0xD83C, 0xDC01, 0xD83C, 0xDC02, 0xD83C, 0xDC03, 0xD83C, 0xDC04, 0xD83C, 0xDC05, 0xD83C, 0xDC06, 0xD83C, 0xDC07, 0xD83C, 0xDC08, 0xD83C, 0xDC09, 0xD83C, 0xDC0A, 0xD83C, 0xDC0B, 0xD83C, 0xDC0C, 0xD83C, 0xDC0D, 0xD83C, 0xDC0E,
        0xD83C, 0xDC0F, 0xD83C, 0xDC10, 0xD83C, 0xDC11, 0xD83C, 0xDC12, 0xD83C, 0xDC13, 0xD83C, 0xDC14, 0xD83C, 0xDC15, 0xD83C, 0xDC16, 0xD83C, 0xDC17, 0xD83C, 0xDC18, 0xD83C, 0xDC19, 0xD83C, 0xDC1A, 0xD83C, 0xDC1B, 0xD83C, 0xDC1C, 0xD83C, 0xDC1D,
        0xD83C, 0xDC1E, 0xD83C, 0xDC1F, 0xD83C, 0xDC20, 0xD83C, 0xDC21, 0xD83C, 0xDC22, 0xD83C, 0xDC23, 0xD83C, 0xDC24, 0xD83C, 0xDC25, 0xD83C, 0xDC26, 0xD83C, 0xDC27, 0xD83C, 0xDC28, 0xD83C, 0xDC29, 0xD83C, 0xDC2A, 0xD83C, 0xDC2B},
       {0x0001F000, 0x0001F001, 0x0001F002, 0x0001F003, 0x0001F004, 0x0001F005, 0x0001F006, 0x0001F007, 0x0001F008, 0x0001F009, 0x0001F00A, 0x0001F00B, 0x0001F00C, 0x0001F00D, 0x0001F00E,
        0x0001F00F, 0x0001F010, 0x0001F011, 0x0001F012, 0x0001F013, 0x0001F014, 0x0001F015, 0x0001F016, 0x0001F017, 0x0001F018, 0x0001F019, 0x0001F01A, 0x0001F01B, 0x0001F01C, 0x0001F01D,
        0x0001F01E, 0x0001F01F, 0x0001F020, 0x0001F021, 0x0001F022, 0x0001F023, 0x0001F024, 0x0001F025, 0x0001F026, 0x0001F027, 0x0001F028, 0x0001F029, 0x0001F02A, 0x0001F02B},
       L"🀀🀁🀂🀃🀄🀅🀆🀇🀈🀉🀊🀋🀌🀍🀎🀏🀐🀑🀒🀓🀔🀕🀖🀗🀘🀙🀚🀛🀜🀝🀞🀟🀠🀡🀢🀣🀤🀥🀦🀧🀨🀩🀪🀫"},

      {"test 21", {(u8)'\xED', (u8)'\x9F', (u8)'\xBF'}, {0xD7FF}, {utf::UTF16_MIN_SURROGATE - 1 /* utf16::min_surrogate - 1*/}, L"\U0000D7FF"},
      {"test 22", {(u8)'\xEE', (u8)'\x80', (u8)'\x80'}, {0xE000}, {utf::UTF16_MAX_SURROGATE + 1 /*utf::utf16::max_surrogate + 1*/}, L"\U0000E000"},
      {"test 23", {(u8)'\xF0', (u8)'\x90', (u8)'\x80', (u8)'\x80'}, {0xD800, 0xDC00}, {0x00010000}, L"\U00010000"},
      {"test 24", {(u8)'\xF4', (u8)'\x8F', (u8)'\xBF', (u8)'\xBF'}, {0xDBFF, 0xDFFF}, {utf::UTF_MAX_CODEPOINT}, L"\U0010FFFF"},
    };

    supported_tuple const supported_test_data[] = {
      {"test 25", {(u8)'\xFA', (u8)'\x95', (u8)'\xA9', (u8)'\xB6', (u8)'\x83'}, {0x02569D83}},
      {"test 26", {(u8)'\xFC', (u8)'\x95', (u8)'\xA9', (u8)'\xB6', (u8)'\x83', (u8)'\xAC'}, {0x15A760EC}},
      {"test 27", {(u8)'\xFD', (u8)'\x95', (u8)'\xA9', (u8)'\xB6', (u8)'\x83', (u8)'\xAC'}, {0x55A760EC}},
      {"test 28", {(u8)'\xFD', (u8)'\xBF', (u8)'\xBF', (u8)'\xBF', (u8)'\xBF', (u8)'\xBF'}, {utf::UTF_MAX_CODEPOINT}},
    };
}  // namespace ncore

using namespace ncore;

UNITTEST_SUITE_BEGIN(runes_and_utf)
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

        UNITTEST_TEST(verify_conversion)
        {
            const u32   len      = 1024;
            utf8::prune utf8     = (utf8::prune)Allocator->allocate(len);
            utf8::prune utf8_end = utf8 + len;

            utf16::prune utf16     = (utf16::prune)Allocator->allocate(len);
            utf16::prune utf16_end = utf16 + len;

            utf32::prune utf32     = (utf32::prune)Allocator->allocate(len);
            utf32::prune utf32_end = utf32 + len;

            size_t num_test = sizeof(unicode_test_data) / sizeof(unicode_test_data[0]);
            for (size_t i = 0; i < num_test; ++i)
            {
                const unicode_tuple& test = unicode_test_data[i];

                {  // utf-8 -> utf-16, compare with str16
                    u32 c8  = 0;
                    u32 c16 = 0;
                    utf::convert(test.str8.m_str, c8, test.str8.m_len, utf16, c16, len);
                    utf16[c16].r = '\0';

                    // compare utf16
                    CHECK_EQUAL(test.str16.m_len, c16);
                    CHECK_EQUAL(0, utf16::compare(test.str16.m_str, utf16));
                }

                {  // utf-8 -> utf-32, compare with str32
                    u32 c8  = 0;
                    u32 c32 = 0;
                    utf::convert(test.str8.m_str, c8, test.str8.m_len, utf32, c32, len);
                    utf32[c32].r = '\0';

                    // compare utf32
                    CHECK_EQUAL(test.str32.m_len, c32);
                    CHECK_EQUAL(0, utf32::compare(test.str32.m_str, utf32));
                }

                { // utf-16 -> utf-8, compare with str8
                    u32 c16 = 0;
                    u32 c8  = 0;
                    utf::convert(test.str16.m_str, c16, test.str16.m_len, utf8, c8, len);
                    utf8[c8].r = '\0';

                    // compare utf8
                    CHECK_EQUAL_T(test.str8.m_len, c8, test.test_name);
                    CHECK_EQUAL_T(0, utf8::compare(test.str8.m_str, utf8), test.test_name);
                }

                { // utf-16 -> utf-32, compare with str32
                    u32 c16 = 0;
                    u32 c32 = 0;
                    utf::convert(test.str16.m_str, c16, test.str16.m_len, utf32, c32, len);
                    utf32[c32].r = '\0';

                    // compare utf32
                    CHECK_EQUAL(test.str32.m_len, c32);
                    CHECK_EQUAL(0, utf32::compare(test.str32.m_str, utf32));
                }

                { // utf-32 -> utf-8, compare with str8
                    u32 c32 = 0;
                    u32 c8  = 0;
                    utf::convert(test.str32.m_str, c32, test.str32.m_len, utf8, c8, len);
                    utf8[c8].r = '\0';

                    // compare utf8
                    CHECK_EQUAL(test.str8.m_len, c8);
                    CHECK_EQUAL(0, utf8::compare(test.str8.m_str, utf8));
                }

                { // utf-32 -> utf-16, compare with str16
                    u32 c32 = 0;
                    u32 c16 = 0;
                    utf::convert(test.str32.m_str, c32, test.str32.m_len, utf16, c16, len);
                    utf16[c16].r = '\0';

                    // compare utf16
                    CHECK_EQUAL(test.str16.m_len, c16);
                    CHECK_EQUAL(0, utf16::compare(test.str16.m_str, utf16));
                }
            }

            Allocator->deallocate(utf8);
            Allocator->deallocate(utf16);
            Allocator->deallocate(utf32);
        }

        UNITTEST_TEST(peek_ascii)
        {
            const ascii::rune str[] = {0x66, 0x72, (uchar)0xC3, (uchar)0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src   = make_crunes(str);
            nrunes::reader_t  reader(src);
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
        }

        UNITTEST_TEST(peek_utf8)
        {
            const utf8::rune str[] = {0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t         src   = make_crunes(str);
            nrunes::reader_t reader(src);
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
        }

        UNITTEST_TEST(peek_utf16)
        {
            const utf16::rune str[] = {0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src   = make_crunes(str);
            nrunes::reader_t  reader(src);
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
        }

        UNITTEST_TEST(peek_utf32)
        {
            const utf32::rune str[] = {0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src   = make_crunes(str);
            nrunes::reader_t  reader(src);
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
            CHECK_EQUAL('f', reader.peek());
        }

        UNITTEST_TEST(read_ascii)
        {
            const ascii::rune str[] = {0x66, 0x72, (uchar)0xC3, (uchar)0xA9, 0x6E, 0x63, 0x68, 0x0};  // fr�nch
            crunes_t          src   = make_crunes(str);
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
            crunes_t         src   = make_crunes(str);
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
            crunes_t          src   = make_crunes(str);
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
            crunes_t          src   = make_crunes(str);
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
            crunes_t         str1  = make_crunes((utf8::pcrune) "test");
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

            str = make_crunes(str2);
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
