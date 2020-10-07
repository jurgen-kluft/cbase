#ifndef __XBASE_RUNES_H__
#define __XBASE_RUNES_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
#undef printf

    namespace ascii
    {
        typedef uchar        rune;
        typedef uchar*       prune;
        typedef const uchar* pcrune;
        static const rune    TERMINATOR = '\0';
        struct runes;
        struct crunes;
    } // namespace ascii
    namespace utf8
    {
        typedef uchar8        rune;
        typedef uchar8*       prune;
        typedef const uchar8* pcrune;
        static const rune     TERMINATOR = '\0';

        struct runes;
        struct crunes;
    } // namespace utf8
    namespace utf16
    {
        typedef uchar16        rune;
        typedef uchar16*       prune;
        typedef const uchar16* pcrune;
        static const rune      TERMINATOR = '\0';

        struct runes;
        struct crunes;
    } // namespace utf16
    namespace utf32
    {
        typedef uchar32        rune;
        typedef uchar32*       prune;
        typedef const uchar32* pcrune;
        static const rune      TERMINATOR = '\0';

        struct runes;
        struct crunes;
    } // namespace utf32

    // NOTE:
    //   UTF string manipulation is the easiest with UTF-32 since it is the only UTF format that
    //   has a fixed rune size and thus is easier to index directly than UTF-8 or UTF-16.
    namespace utf
    {
        // ASCII, UTF-8, UTF-16, UTF-32 conversions
        uchar32 peek(ascii::crunes const& str);
        uchar32 peek(utf8::crunes const& str);
        uchar32 peek(utf16::crunes const& str);
        uchar32 peek(utf32::crunes const& str);

        s32 compare(ascii::pcrune stra, ascii::pcrune strb);
        s32 compare(utf32::pcrune stra, ascii::pcrune strb);
        s32 compare(ascii::pcrune stra, utf32::pcrune strb);
        s32 compare(utf32::pcrune stra, utf32::pcrune strb);

        s32 compare(ascii::crunes const& stra, utf32::pcrune strb);

        uchar32 read(ascii::rune const*& str, ascii::rune const* end);
        uchar32 read(utf8::rune const*& str, utf8::rune const* end);
        uchar32 read(utf16::rune const*& str, utf16::rune const* end);
        uchar32 read(utf32::rune const*& str, utf32::rune const* end);

        uchar32 read(ascii::rune*& str, ascii::rune const* end);
        uchar32 read(utf8::rune*& str, utf8::rune const* end);
        uchar32 read(utf16::rune*& str, utf16::rune const* end);
        uchar32 read(utf32::rune*& str, utf32::rune const* end);

        uchar32 read(ascii::crunes& str);
        uchar32 read(utf8::crunes& str);
        uchar32 read(utf16::crunes& str);
        uchar32 read(utf32::crunes& str);

        void write(uchar32 c, uchar*& str, const uchar* end);
        void write(uchar32 c, uchar8*& str, const uchar8* end);
        void write(uchar32 c, uchar16*& str, const uchar16* end);
        void write(uchar32 c, uchar32*& str, const uchar32* end);

        bool write(uchar32 c, ascii::runes& str);
        bool write(uchar32 c, utf8::runes& str);
        bool write(uchar32 c, utf16::runes& str);
        bool write(uchar32 c, utf32::runes& str);

        void copy(char const* sstr, ascii::runes& dstr, bool write_terminator = true);
        void copy(char const* sstr, utf16::runes& dstr, bool write_terminator = true);
        void copy(char const* sstr, utf32::runes& dstr, bool write_terminator = true);

        void copy(ascii::crunes const& sstr, ascii::runes& dstr, bool write_terminator = true);
        void copy(utf16::crunes const& sstr, ascii::runes& dstr, bool write_terminator = true);
        void copy(utf32::crunes const& sstr, ascii::runes& dstr, bool write_terminator = true);

        void copy(ascii::crunes const& sstr, utf16::runes& dstr, bool write_terminator = true);
        void copy(utf16::crunes const& sstr, utf16::runes& dstr, bool write_terminator = true);
        void copy(utf32::crunes const& sstr, utf16::runes& dstr, bool write_terminator = true);

        void copy(ascii::crunes const& sstr, utf32::runes& dstr, bool write_terminator = true);
        void copy(utf16::crunes const& sstr, utf32::runes& dstr, bool write_terminator = true);
        void copy(utf32::crunes const& sstr, utf32::runes& dstr, bool write_terminator = true);

        s32 size(uchar32 c);

        bool is_eos(uchar const* str);
        bool is_eos(uchar8 const* str);
        bool is_eos(uchar16 const* str);
        bool is_eos(uchar32 const* str);

        bool can_read(ascii::runes const& str);
        bool can_read(utf8::runes const& str);
        bool can_read(utf16::runes const& str);
        bool can_read(utf32::runes const& str);
        bool can_read(ascii::crunes const& str);
        bool can_read(utf8::crunes const& str);
        bool can_read(utf16::crunes const& str);
        bool can_read(utf32::crunes const& str);

        bool can_write(ascii::runes const& str);
        bool can_write(utf8::runes const& str);
        bool can_write(utf16::runes const& str);
        bool can_write(utf32::runes const& str);

        bool read_is_crln(ascii::runes const& str);
        bool read_is_crln(utf8::runes const& str);
        bool read_is_crln(utf16::runes const& str);
        bool read_is_crln(utf32::runes const& str);
        bool read_is_crln(ascii::crunes const& str);
        bool read_is_crln(utf8::crunes const& str);
        bool read_is_crln(utf16::crunes const& str);
        bool read_is_crln(utf32::crunes const& str);
    }; // namespace utf

    namespace ascii
    {
        struct runes
        {
            inline runes()
                : m_str(nullptr)
                , m_end(nullptr)
                , m_eos(nullptr)
            {
            }
            inline runes(prune _str, prune _end, prune _eos)
                : m_str(_str)
                , m_end(_end)
                , m_eos(_eos)
            {
                *m_eos = TERMINATOR;
            }
            inline runes(runes const& other)
                : m_str(other.m_str)
                , m_end(other.m_end)
                , m_eos(other.m_eos)
            {
            }

            s32 size() const { return (s32)(m_end - m_str); }
            s32 cap() const { return (s32)(m_eos - m_str); }

            bool is_empty() const { return m_str == m_end; }
            bool is_valid() const { return m_str != nullptr && m_end <= m_eos; }
            void reset() { m_end = m_str; }
            void clear()
            {
                if (m_str != nullptr)
                {
                    m_end    = m_str;
                    m_end[0] = TERMINATOR;
                }
            }
            void term()
            {
                if (m_end < m_eos)
                    *m_end = TERMINATOR;
            }

            void concatenate(rune c)
            {
                if (m_end < m_eos)
                    *m_end++ = c;
            }

            void concatenate(const rune* str)
            {
                while (m_end < m_eos && *str != TERMINATOR)
                    *m_end++ = *str++;
            }

            runes& operator=(runes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                m_eos = other.m_eos;
                return *this;
            }

            runes& operator+=(const rune* str)
            {
                concatenate(str);
                return *this;
            }

            runes& operator+=(rune c)
            {
                if (m_end < m_eos)
                {
                    *m_end++ = c;
                }
                return *this;
            }

            prune m_str; // Begin of string in memory and begin of character stream
            prune m_end; // End of character/text stream
            prune m_eos; // End of string in memory, points to a TERMINATOR
        };
        struct crunes
        {
            inline crunes()
                : m_str("\0")
                , m_cur(m_str)
                , m_end(m_str)
            {
            }
            inline crunes(pcrune _str)
                : m_str(_str)
                , m_cur(_str)
                , m_end(_str)
            {
                while (*m_end != TERMINATOR)
                    ++m_end;
            }
            inline crunes(pcrune _str, pcrune _end)
                : m_str(_str)
                , m_cur(_str)
                , m_end(_end)
            {
                if (m_end == nullptr)
                {
                    m_end = m_str;
                    while (*m_end != TERMINATOR)
                        ++m_end;
                }
            }
            inline crunes(runes const& other)
                : m_str(other.m_str)
                , m_cur(other.m_str)
                , m_end(other.m_end)
            {
            }
            inline crunes(crunes const& other)
                : m_str(other.m_str)
                , m_cur(other.m_str)
                , m_end(other.m_end)
            {
            }

            s32 size() const { return (s32)(s64)(m_end - m_str); }

            bool is_valid() const { return m_str != nullptr && m_cur <= m_end; }
            bool is_empty() const { return m_str == m_end; }
            void reset() { m_cur = m_str; }
            void clear()
            {
                m_str = "\0";
                m_cur = m_str;
                m_end = m_str;
            }
            crunes& operator=(crunes const& other)
            {
                m_str = other.m_str;
                m_cur = other.m_cur;
                m_end = other.m_end;
                return *this;
            }
            pcrune m_str;
            pcrune m_cur;
            pcrune m_end;
        };

        template <s32 L> class runez : public runes
        {
        public:
            enum
            {
                SIZE = L + 1
            };
            rune m_run[SIZE];
            inline runez()
                : runes(m_run, m_run, &m_run[SIZE - 1])
            {
            }

            inline runez(const char* str)
                : runes(m_run, m_run, &m_run[SIZE - 1])
            {
                concatenate(str);
                term();
            }
        };

        template <s32 L> class crunez : public crunes
        {
        public:
            enum
            {
                SIZE = L + 1
            };
            rune m_run[SIZE];

            inline crunez(uchar32 c)
                : crunes(m_run, &m_run[SIZE - 1])
            {
                runes run(m_run, m_run, &m_run[SIZE - 1]);
                run.concatenate((rune)c);
                run.term();
                m_end = run.m_end;
            }
            inline crunez(const char* str)
                : crunes(m_run, &m_run[SIZE - 1])
            {
                runes run(m_run, m_run, &m_run[SIZE - 1]);
                run.concatenate(str);
                run.term();
                m_end = run.m_end;
            }
        };

        class alloc
        {
        public:
            virtual runes allocate(s32 len, s32 cap) = 0;
            virtual void  deallocate(runes& slice)   = 0;
        };
    } // namespace ascii

    namespace utf8
    {
        struct runes
        {
            inline runes()
                : m_str(nullptr)
                , m_end(nullptr)
                , m_eos(nullptr)
            {
            }
            inline runes(prune _str, prune _end, prune _eos)
                : m_str(_str)
                , m_end(_end)
                , m_eos(_eos)
            {
            }
            inline runes(runes const& other)
                : m_str(other.m_str)
                , m_end(other.m_end)
                , m_eos(other.m_eos)
            {
            }

            bool is_empty() const { return m_str == m_end; }
            void reset() { m_end = m_str; }

            void clear()
            {
                if (m_str != nullptr)
                {
                    m_end    = m_str;
                    m_end[0] = TERMINATOR;
                }
            }
            runes& operator=(runes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                m_eos = other.m_eos;
                return *this;
            }
            prune m_str;
            prune m_end;
            prune m_eos;
        };

        struct crunes
        {
            inline crunes()
                : m_str((pcrune) "\0\0")
                , m_end(m_str)
            {
            }
            inline crunes(pcrune _str)
                : m_str(_str)
                , m_end(nullptr)
            {
                if (m_end == nullptr)
                {
                    m_end = m_str;
                    while (utf::read(m_end, nullptr)) {}
                }
            }
            inline crunes(pcrune _str, pcrune _end)
                : m_str(_str)
                , m_end(_end)
            {
                if (m_end == nullptr)
                {
                    m_end = m_str;
                    while (utf::read(m_end, nullptr)) {}
                }
            }

            inline crunes(runes const& other)
                : m_str(other.m_str)
                , m_end(other.m_end)
            {
            }
            inline crunes(crunes const& other)
                : m_str(other.m_str)
                , m_end(other.m_end)
            {
            }

            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                m_str = (pcrune) "\0\0";
                m_end = m_str;
            }
            crunes& operator=(crunes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                return *this;
            }
            pcrune m_str;
            pcrune m_end;
        };

    } // namespace utf8

    namespace utf16
    {
        struct runes
        {
            inline runes()
                : m_str(nullptr)
                , m_end(nullptr)
                , m_eos(nullptr)
            {
            }
            inline runes(prune _str, prune _end, prune _eos)
                : m_str(_str)
                , m_end(_end)
                , m_eos(_eos)
            {
            }
            inline runes(runes const& other)
                : m_str(other.m_str)
                , m_end(other.m_end)
                , m_eos(other.m_eos)
            {
            }

            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                if (m_str != nullptr)
                {
                    m_end    = m_str;
                    m_end[0] = TERMINATOR;
                }
            }

            runes& operator=(runes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                m_eos = other.m_eos;
                return *this;
            }

            bool operator==(runes const& other) const
            {
                pcrune s1 = other.m_str;
                pcrune s2 = m_str;
                while (s1 < other.m_end && s2 < m_end)
                {
                    if (*s1++ != *s2++)
                        return false;
                }
                return (s1 == other.m_end && s2 == m_end);
            }

            prune m_str;
            prune m_end;
            prune m_eos;
        };
        struct crunes
        {
            inline crunes()
                : m_str(nullptr)
                , m_cur(nullptr)
                , m_end(nullptr)
            {
            }
            inline crunes(pcrune _str)
                : m_str(_str)
                , m_cur(_str)
                , m_end(_str)
            {
                while (*m_end != TERMINATOR)
                    ++m_end;
            }
            inline crunes(pcrune _str, pcrune _end)
                : m_str(_str)
                , m_cur(_str)
                , m_end(_end)
            {
                if (m_end == nullptr)
                {
                    m_end = m_str;
                    while (*m_end != TERMINATOR)
                        ++m_end;
                }
            }
            inline crunes(runes const& other)
                : m_str(other.m_str)
                , m_cur(other.m_str)
                , m_end(other.m_end)
            {
            }
            inline crunes(crunes const& other)
                : m_str(other.m_str)
                , m_cur(other.m_str)
                , m_end(other.m_end)
            {
            }
            s32  size() const { return (s32)(m_end - m_str); }
            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                m_str = (pcrune) "\0\0\0\0";
                m_cur = m_str;
                m_end = m_str;
            }
            crunes& operator=(crunes const& other)
            {
                m_str = other.m_str;
                m_cur = other.m_cur;
                m_end = other.m_end;
                return *this;
            }
            pcrune m_str;
            pcrune m_cur;
            pcrune m_end;
        };

        class alloc
        {
        public:
            virtual runes allocate(s32 len, s32 cap) = 0;
            virtual void  deallocate(runes& slice)   = 0;
        };

    } // namespace utf16

    namespace utf32
    {
        struct runes
        {
            inline runes()
                : m_str(nullptr)
                , m_end(nullptr)
                , m_eos(nullptr)
            {
            }
            inline runes(prune _str, prune _end, prune _eos)
                : m_str(_str)
                , m_end(_end)
                , m_eos(_eos)
            {
            }
            inline runes(runes const& other)
                : m_str(other.m_str)
                , m_end(other.m_end)
                , m_eos(other.m_eos)
            {
            }

            s32 size() const { return (s32)(m_end - m_str); }
            s32 cap() const { return (s32)(m_eos - m_str); }

            bool is_empty() const { return m_str == m_end; }
            bool is_valid() const { return m_str != nullptr && m_end <= m_eos; }

            void reset() { m_end = m_str; }
            void clear()
            {
                if (m_str != nullptr)
                {
                    m_end    = m_str;
                    m_end[0] = TERMINATOR;
                }
            }

            runes& operator=(runes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                m_eos = other.m_eos;
                return *this;
            }

            prune m_str;
            prune m_end;
            prune m_eos;
        };

        struct crunes
        {
            inline crunes()
                : m_str(nullptr)
                , m_cur(nullptr)
                , m_end(nullptr)
            {
            }
            inline crunes(pcrune _str)
                : m_str(_str)
                , m_cur(_str)
                , m_end(_str)
            {
                while (*m_end != TERMINATOR)
                    ++m_end;
            }
            inline crunes(pcrune _str, pcrune _end)
                : m_str(_str)
                , m_cur(_str)
                , m_end(_end)
            {
                if (m_end == nullptr)
                {
                    m_end = m_str;
                    if (m_end != nullptr)
                    {
                        while (*m_end != TERMINATOR)
                            ++m_end;
                    }
                }
            }
            inline crunes(runes const& other)
                : m_str(other.m_str)
                , m_cur(other.m_str)
                , m_end(other.m_end)
            {
            }
            inline crunes(crunes const& other)
                : m_str(other.m_str)
                , m_cur(other.m_str)
                , m_end(other.m_end)
            {
            }

            s32  size() const { return (s32)(m_end - m_str); }
            bool is_valid() const { return m_str != nullptr && m_cur <= m_end; }
            bool is_empty() const { return m_str == m_end; }
            void reset() { m_cur = m_str; }
            void clear()
            {
                m_str = (pcrune) "\0\0\0\0";
                m_cur = m_str;
                m_end = m_str;
            }
            crunes& operator=(crunes const& other)
            {
                m_str = other.m_str;
                m_cur = other.m_cur;
                m_end = other.m_end;
                return *this;
            }
            pcrune m_str;
            pcrune m_cur;
            pcrune m_end;
        };

        template <s32 L> class runez : public runes
        {
        public:
            enum
            {
                SIZE = L + 1
            };
            rune m_run[SIZE];

            inline runez()
                : runes(m_run, m_run, &m_run[SIZE - 1])
            {
                m_eos[0] = TERMINATOR;
            }
            inline runez(const char* str)
                : runes(m_run, m_run, &m_run[SIZE - 1])
            {
                m_eos[0] = TERMINATOR;
                while (*str != ascii::TERMINATOR && m_end < m_eos)
                {
                    *m_end++ = (*str++);
                }
                *m_end = TERMINATOR;
            }
        };

        template <s32 L> class crunez : public crunes
        {
        public:
            enum
            {
                SIZE = L + 1
            };
            rune m_run[SIZE];

            inline crunez(uchar32 c)
                : crunes(m_run, &m_run[SIZE - 1])
            {
                prune dst = m_run;
                utf::write(c, dst, m_end);
                utf::write(TERMINATOR, dst, m_end);
                m_end = &m_run[1];
            }
            inline crunez(const char* str)
                : crunes(m_run, &m_run[SIZE - 1])
            {
                runes dst(m_run, m_run, &m_run[SIZE - 1]);
                utf::copy(str, dst, true);
                m_end = dst.m_end;
            }
        };

        class alloc
        {
        public:
            virtual ~alloc() {}
            virtual runes allocate(s32 len, s32 cap) = 0;
            virtual void  deallocate(runes& slice)   = 0;
        };
    } // namespace utf32
} // namespace xcore

namespace xcore
{
    // NOTE: Identical to utf32 namespace
    namespace ascii
    {
        s32 len(pcrune str, pcrune end);

        prune  endof(prune str, pcrune eos);
        pcrune endof(pcrune str, pcrune eos);

        crunes find(crunes const& _str, uchar32 _c, bool inCaseSensitive = true);
        runes  find(runes const& _str, uchar32 _c, bool inCaseSensitive = true);
        crunes findLast(crunes const& _str, uchar32 _c, bool inCaseSensitive = true);
        runes  findLast(runes const& _str, uchar32 _c, bool inCaseSensitive = true);

        crunes find(crunes const& str, crunes const& find, bool inCaseSensitive = true);
        runes  find(runes const& str, crunes const& find, bool inCaseSensitive = true);
        crunes findLast(crunes const& str, crunes const& find, bool inCaseSensitive = true);
        runes  findLast(runes const& str, crunes const& find, bool inCaseSensitive = true);

        crunes findOneOf(crunes const& str, crunes const& set, bool inCaseSensitive = true);
        runes  findOneOf(runes const& str, crunes const& set, bool inCaseSensitive = true);

        // -------------------------------------------------------------------------------
        // crunes, functions
        crunes findSelectUntil(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findSelectUntil(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        crunes findLastSelectUntil(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findLastSelectUntil(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        crunes findSelectUntilIncluded(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findSelectUntilIncluded(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        crunes findLastSelectUntilIncluded(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findLastSelectUntilIncluded(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        crunes findSelectAfter(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findSelectAfter(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        crunes findLastSelectAfter(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findLastSelectAfter(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        crunes selectBetween(const crunes& inStr, uchar32 inLeft, uchar32 inRight);
        crunes selectNextBetween(const crunes& inStr, const crunes& inSelection, uchar32 inLeft, uchar32 inRight);

        crunes selectBetweenLast(const crunes& inStr, uchar32 inLeft, uchar32 inRight);
        crunes selectPreviousBetween(const crunes& inStr, const crunes& inSelection, uchar32 inLeft, uchar32 inRight);

        crunes selectUntilEndExcludeSelection(const crunes& inStr, const crunes& inSelection);
        crunes selectUntilEndIncludeSelection(const crunes& inStr, const crunes& inSelection);

        crunes selectOverlap(const crunes& inStr, const crunes& inRight);

        s32 compare(crunes const& str1, crunes const& str2, bool inCaseSensitive = true);

        // -------------------------------------------------------------------------------
        // runes, functions
        runes findSelectUntil(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectUntil(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectUntil(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectUntil(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findSelectUntilIncluded(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectUntilIncluded(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectUntilIncluded(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectUntilIncluded(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        runes findSelectAfter(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectAfter(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectAfter(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectAfter(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        runes selectBetween(const runes& inStr, uchar32 inLeft, uchar32 inRight);
        runes selectNextBetween(const runes& inStr, const runes& inSelection, uchar32 inLeft, uchar32 inRight);

        runes selectBetweenLast(const runes& inStr, uchar32 inLeft, uchar32 inRight);
        runes selectPreviousBetween(const runes& inStr, const runes& inSelection, uchar32 inLeft, uchar32 inRight);

        runes selectUntilEndExcludeSelection(const runes& inStr, const runes& inSelection);
        runes selectUntilEndIncludeSelection(const runes& inStr, const runes& inSelection);

        crunes selectBeforeExcludeSelection(const crunes& inStr, const crunes& inSelection);

        runes selectOverlap(const runes& inStr, const runes& inRight);

        s32 compare(runes const& str1, runes const& str2, bool inCaseSensitive = true);

        // -------------------------------------------------------------------------------
        // parse/from_string, to_string
        crunes parse(crunes const& str, bool& value);
        crunes parse(crunes const& str, s32& value, s32 base = 10);
        crunes parse(crunes const& str, u32& value, s32 base = 10);
        crunes parse(crunes const& str, s64& value, s32 base = 10);
        crunes parse(crunes const& str, u64& value, s32 base = 10);
        crunes parse(crunes const& str, f32& value);
        crunes parse(crunes const& str, f64& value);

        void to_string(runes& str, s32 val, s32 base = 10);
        void to_string(runes& str, u32 val, s32 base = 10);
        void to_string(runes& str, s64 val, s32 base = 10);
        void to_string(runes& str, u64 val, s32 base = 10);
        void to_string(runes& str, f32 val, s32 num_fractional_digits = 4);
        void to_string(runes& str, f64 val, s32 num_fractional_digits = 4);

        bool is_decimal(crunes const& str);
        bool is_hexadecimal(crunes const& str, bool with_prefix = false);
        bool is_float(crunes const& str);
        bool is_GUID(crunes const& str);

        inline bool is_space(uchar32 c) { return ((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
        inline bool is_upper(uchar32 c) { return ((c >= 'A') && (c <= 'Z')); }
        inline bool is_lower(uchar32 c) { return ((c >= 'a') && (c <= 'z')); }
        inline bool is_alpha(uchar32 c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
        inline bool is_digit(uchar32 c) { return ((c >= '0') && (c <= '9')); }
        inline bool is_hexa(uchar32 c) { return (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

        void           to_upper(runes& str);
        void           to_lower(runes& str);
        inline uchar32 to_upper(uchar32 c) { return ((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
        inline uchar32 to_lower(uchar32 c) { return ((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
        inline u32     to_digit(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
        inline u32     to_number(uchar32 c)
        {
            if (is_digit(c))
                return to_digit(c);
            else if (c >= 'A' && c <= 'F')
                return (c - 'A' + 10);
            else if (c >= 'a' && c <= 'f')
                return (c - 'a' + 10);
            else
                return (c);
        }

        inline bool is_equal(uchar32 a, uchar32 b) { return (a == b); }
        inline bool is_equalfold(uchar32 a, uchar32 b)
        {
            a = to_lower(a);
            b = to_lower(b);
            return (a == b);
        }

        bool    is_upper(crunes const& str);
        bool    is_lower(crunes const& str);
        bool    is_capitalized(crunes const& str);
        bool    is_delimited(crunes const& str, uchar32 delimit_left = '\"', uchar32 delimit_right = '\"');
        bool    is_quoted(crunes const& str, uchar32 quote = '\"');
        bool    starts_with(crunes const& str, uchar32 start);
        bool    starts_with(crunes const& str, crunes const& start);
        bool    ends_with(crunes const& str, uchar32 end_char);
        bool    ends_with(crunes const& str, crunes const& end);
        uchar32 first_char(crunes const& str);
        uchar32 last_char(crunes const& str);

        void removeSelection(runes& str, crunes const& sel);
        void keepOnlySelection(runes& str, crunes const& sel);

        void replaceSelection(runes& str, crunes const& sel, crunes const& replace);
        void replaceSelection(runes& str, crunes const& sel, crunes const& replace, alloc* allocator, s32 size_alignment);

        void findReplace(runes& str, uchar32 find, uchar32 replace, bool inCaseSensitive = true);
        void findReplace(runes& str, crunes const& find, crunes const& replace, bool inCaseSensitive = true);

        void insert(runes& str, crunes const& insert);
        void insert(runes& str, crunes const& insert, alloc* allocator, s32 size_alignment);

        void insert(runes& str, crunes const& sel, crunes const& insert);
        void insert(runes& str, crunes const& sel, crunes const& insert, alloc* allocator, s32 size_alignment);

        void trim(runes&);                                      // Trim whitespace from left and right side
        void trimLeft(runes&);                                  // Trim whitespace from left side
        void trimRight(runes&);                                 // Trim whitespace from right side
        void trim(runes&, rune inChar);                         // Trim characters in <inCharSet> from left and right side
        void trimLeft(runes&, rune inChar);                     // Trim character <inChar> from left side
        void trimRight(runes&, rune inChar);                    // Trim character <inChar> from right side
        void trim(runes&, crunes const& inCharSet);             // Trim characters in <inCharSet> from left and right side
        void trimLeft(runes&, crunes const& inCharSet);         // Trim characters in <inCharSet> from left side
        void trimRight(runes&, crunes const& inCharSet);        // Trim characters in <inCharSet> from right side
        void trimQuotes(runes&);                                // Trim double quotes from left and right side
        void trimQuotes(runes&, rune quote);                    // Trim double quotes from left and right side
        void trimDelimiters(runes&, rune inLeft, rune inRight); // Trim delimiters from left and right side

        void trim(crunes&);                                      // Trim whitespace from left and right side
        void trimLeft(crunes&);                                  // Trim whitespace from left side
        void trimRight(crunes&);                                 // Trim whitespace from right side
        void trim(crunes&, rune inChar);                         // Trim characters in <inCharSet> from left and right side
        void trimLeft(crunes&, rune inChar);                     // Trim character <inChar> from left side
        void trimRight(crunes&, rune inChar);                    // Trim character <inChar> from right side
        void trim(crunes&, crunes const& inCharSet);             // Trim characters in <inCharSet> from left and right side
        void trimLeft(crunes&, crunes const& inCharSet);         // Trim characters in <inCharSet> from left side
        void trimRight(crunes&, crunes const& inCharSet);        // Trim characters in <inCharSet> from right side
        void trimQuotes(crunes&);                                // Trim double quotes from left and right side
        void trimQuotes(crunes&, rune quote);                    // Trim double quotes from left and right side
        void trimDelimiters(crunes&, rune inLeft, rune inRight); // Trim delimiters from left and right side

        void copy(const crunes& src, runes& dst);
        void copy(const crunes& src, runes& dst, alloc* allocator, s32 size_alignment);

        void concatenate(runes& str, const crunes& concat);
        void concatenate(runes& str, const crunes& concat, alloc* allocator, s32 size_alignment);
        void concatenate(runes& str, const crunes& concat1, const crunes& concat2, alloc* allocator, s32 size_alignment);
    } // namespace ascii

    inline bool operator==(const ascii::crunes& lhs, const ascii::crunes& rhs) { return ascii::compare(lhs, rhs) == 0; }
    inline bool operator!=(const ascii::crunes& lhs, const ascii::crunes& rhs) { return ascii::compare(lhs, rhs) != 0; }
    inline bool operator==(const ascii::runes& lhs, const ascii::runes& rhs) { return ascii::compare(lhs, rhs) == 0; }
    inline bool operator!=(const ascii::runes& lhs, const ascii::runes& rhs) { return ascii::compare(lhs, rhs) != 0; }

    // NOTE: Identical to ascii namespace
    namespace utf32
    {
        s32 len(pcrune str, pcrune end);

        prune  endof(prune str, pcrune eos);
        pcrune endof(pcrune str, pcrune eos);

        crunes find(crunes const& _str, uchar32 _c, bool inCaseSensitive = true);
        runes  find(runes const& _str, uchar32 _c, bool inCaseSensitive = true);
        crunes findLast(crunes const& _str, uchar32 _c, bool inCaseSensitive = true);
        runes  findLast(runes const& _str, uchar32 _c, bool inCaseSensitive = true);

        crunes find(crunes const& str, crunes const& find, bool inCaseSensitive = true);
        runes  find(runes const& str, crunes const& find, bool inCaseSensitive = true);
        crunes findLast(crunes const& str, crunes const& find, bool inCaseSensitive = true);
        runes  findLast(runes const& str, crunes const& find, bool inCaseSensitive = true);

        crunes findOneOf(crunes const& str, crunes const& set, bool inCaseSensitive = true);
        runes  findOneOf(runes const& str, crunes const& set, bool inCaseSensitive = true);

        // -------------------------------------------------------------------------------
        // crunes, functions
        crunes findSelectUntil(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findSelectUntil(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        crunes findLastSelectUntil(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findLastSelectUntil(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        crunes findSelectUntilIncluded(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findSelectUntilIncluded(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        crunes findLastSelectUntilIncluded(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findLastSelectUntilIncluded(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        crunes findSelectAfter(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findSelectAfter(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        crunes findLastSelectAfter(const crunes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        crunes findLastSelectAfter(const crunes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        crunes selectBetween(const crunes& inStr, uchar32 inLeft, uchar32 inRight);
        crunes selectNextBetween(const crunes& inStr, const crunes& inSelection, uchar32 inLeft, uchar32 inRight);

        crunes selectBetweenLast(const crunes& inStr, uchar32 inLeft, uchar32 inRight);
        crunes selectPreviousBetween(const crunes& inStr, const crunes& inSelection, uchar32 inLeft, uchar32 inRight);

        crunes selectUntilEndExcludeSelection(const crunes& inStr, const crunes& inSelection);
        crunes selectUntilEndIncludeSelection(const crunes& inStr, const crunes& inSelection);

        crunes selectBeforeExcludeSelection(const crunes& inStr, const crunes& inSelection);

        crunes selectOverlap(const crunes& inStr, const crunes& inRight);

        s32 compare(crunes const& str1, crunes const& str2, bool inCaseSensitive = true);

        // -------------------------------------------------------------------------------
        // runes, functions
        runes findSelectUntil(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectUntil(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectUntil(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectUntil(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findSelectUntilIncluded(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectUntilIncluded(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectUntilIncluded(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectUntilIncluded(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        runes findSelectAfter(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectAfter(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectAfter(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectAfter(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        runes selectBetween(const runes& inStr, uchar32 inLeft, uchar32 inRight);
        runes selectNextBetween(const runes& inStr, const runes& inSelection, uchar32 inLeft, uchar32 inRight);

        runes selectBetweenLast(const runes& inStr, uchar32 inLeft, uchar32 inRight);
        runes selectPreviousBetween(const runes& inStr, const runes& inSelection, uchar32 inLeft, uchar32 inRight);

        runes selectUntilEndExcludeSelection(const runes& inStr, const runes& inSelection);
        runes selectUntilEndIncludeSelection(const runes& inStr, const runes& inSelection);

        runes selectOverlap(const runes& inStr, const runes& inRight);

        s32 compare(runes const& str1, runes const& str2, bool inCaseSensitive = true);

        // -------------------------------------------------------------------------------
        // parse/from_string, to_string
        crunes parse(crunes const& str, bool& value);
        crunes parse(crunes const& str, s32& value, s32 base = 10);
        crunes parse(crunes const& str, u32& value, s32 base = 10);
        crunes parse(crunes const& str, s64& value, s32 base = 10);
        crunes parse(crunes const& str, u64& value, s32 base = 10);
        crunes parse(crunes const& str, f32& value);
        crunes parse(crunes const& str, f64& value);

        void to_string(runes& str, s32 val, s32 base = 10);
        void to_string(runes& str, u32 val, s32 base = 10);
        void to_string(runes& str, s64 val, s32 base = 10);
        void to_string(runes& str, u64 val, s32 base = 10);
        void to_string(runes& str, f32 val, s32 num_fractional_digits = 4);
        void to_string(runes& str, f64 val, s32 num_fractional_digits = 4);

        bool is_decimal(crunes const& str);
        bool is_hexadecimal(crunes const& str, bool with_prefix = false);
        bool is_float(crunes const& str);
        bool is_GUID(crunes const& str);

        inline bool is_space(uchar32 c) { return ((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
        inline bool is_upper(uchar32 c) { return ((c >= 'A') && (c <= 'Z')); }
        inline bool is_lower(uchar32 c) { return ((c >= 'a') && (c <= 'z')); }
        inline bool is_alpha(uchar32 c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
        inline bool is_digit(uchar32 c) { return ((c >= '0') && (c <= '9')); }
        inline bool is_hexa(uchar32 c) { return (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }

        void           to_upper(runes& str);
        void           to_lower(runes& str);
        inline uchar32 to_upper(uchar32 c) { return ((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
        inline uchar32 to_lower(uchar32 c) { return ((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
        inline u32     to_digit(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
        inline u32     to_number(uchar32 c)
        {
            if (is_digit(c))
                return to_digit(c);
            else if (c >= 'A' && c <= 'F')
                return (c - 'A' + 10);
            else if (c >= 'a' && c <= 'f')
                return (c - 'a' + 10);
            else
                return (c);
        }

        inline bool is_equal(uchar32 a, uchar32 b) { return (a == b); }

        inline bool is_equalfold(uchar32 a, uchar32 b)
        {
            a = to_lower(a);
            b = to_lower(b);
            return (a == b);
        }

        bool    is_upper(crunes const& str);
        bool    is_lower(crunes const& str);
        bool    is_capitalized(crunes const& str);
        bool    is_delimited(crunes const& str, uchar32 delimit_left = '\"', uchar32 delimit_right = '\"');
        bool    is_quoted(crunes const& str, uchar32 quote = '\"');
        bool    starts_with(crunes const& str, uchar32 start);
        bool    starts_with(crunes const& str, crunes const& start);
        bool    ends_with(crunes const& str, uchar32 end);
        bool    ends_with(crunes const& str, crunes const& end);
        uchar32 first_char(crunes const& str);
        uchar32 last_char(crunes const& str);

        void removeSelection(runes& str, crunes const& sel);
        void keepOnlySelection(runes& str, crunes const& sel);
        void replaceSelection(runes& str, crunes const& sel, crunes const& replace);
        void replaceSelection(runes& str, crunes const& sel, crunes const& replace, alloc* allocator, s32 size_alignment);
        void findReplace(runes& str, uchar32 find, uchar32 replace, bool inCaseSensitive = true);
        void findReplace(runes& str, crunes const& find, crunes const& replace, bool inCaseSensitive = true);
        void insert(runes& str, crunes const& insert);
        void insert(runes& str, crunes const& insert, alloc* allocator, s32 size_alignment);
        void insert(runes& str, crunes const& sel, crunes const& insert);
        void insert(runes& str, crunes const& sel, crunes const& insert, alloc* allocator, s32 size_alignment);

        void trim(runes&);                                      // Trim whitespace from left and right side
        void trimLeft(runes&);                                  // Trim whitespace from left side
        void trimRight(runes&);                                 // Trim whitespace from right side
        void trim(runes&, rune inChar);                         // Trim characters in <inCharSet> from left and right side
        void trimLeft(runes&, rune inChar);                     // Trim character <inChar> from left side
        void trimRight(runes&, rune inChar);                    // Trim character <inChar> from right side
        void trim(runes&, crunes const& inCharSet);             // Trim characters in <inCharSet> from left and right side
        void trimLeft(runes&, crunes const& inCharSet);         // Trim characters in <inCharSet> from left side
        void trimRight(runes&, crunes const& inCharSet);        // Trim characters in <inCharSet> from right side
        void trimQuotes(runes&);                                // Trim double quotes from left and right side
        void trimQuotes(runes&, rune quote);                    // Trim double quotes from left and right side
        void trimDelimiters(runes&, rune inLeft, rune inRight); // Trim delimiters from left and right side

        void trim(crunes&);                                      // Trim whitespace from left and right side
        void trimLeft(crunes&);                                  // Trim whitespace from left side
        void trimRight(crunes&);                                 // Trim whitespace from right side
        void trim(crunes&, rune inChar);                         // Trim characters in <inCharSet> from left and right side
        void trimLeft(crunes&, rune inChar);                     // Trim character <inChar> from left side
        void trimRight(crunes&, rune inChar);                    // Trim character <inChar> from right side
        void trim(crunes&, crunes const& inCharSet);             // Trim characters in <inCharSet> from left and right side
        void trimLeft(crunes&, crunes const& inCharSet);         // Trim characters in <inCharSet> from left side
        void trimRight(crunes&, crunes const& inCharSet);        // Trim characters in <inCharSet> from right side
        void trimQuotes(crunes&);                                // Trim double quotes from left and right side
        void trimQuotes(crunes&, rune quote);                    // Trim double quotes from left and right side
        void trimDelimiters(crunes&, rune inLeft, rune inRight); // Trim delimiters from left and right side

        void copy(const crunes& src, runes& dst);
        void copy(const crunes& src, runes& dst, alloc* allocator, s32 size_alignment);

        void concatenate(runes& str, const crunes& concat);
        void concatenate(runes& str, const crunes& concat, alloc* allocator, s32 size_alignment);
        void concatenate(runes& str, const crunes& concat1, const crunes& concat2, alloc* allocator, s32 size_alignment);
    }; // namespace utf32

    // utf32 specific functions, different from 'namespace ascii'
    namespace utf32
    {
        void copy(const ascii::crunes& src, runes& dst, alloc* allocator, s32 size_alignment);
    }

    inline bool operator==(const utf32::crunes& lhs, const utf32::crunes& rhs) { return utf32::compare(lhs, rhs) == 0; }
    inline bool operator!=(const utf32::crunes& lhs, const utf32::crunes& rhs) { return utf32::compare(lhs, rhs) != 0; }
    inline bool operator==(const utf32::runes& lhs, const utf32::runes& rhs) { return utf32::compare(lhs, rhs) == 0; }
    inline bool operator!=(const utf32::runes& lhs, const utf32::runes& rhs) { return utf32::compare(lhs, rhs) != 0; }

}; // namespace xcore

#endif ///< __XBASE_RUNES_H__
