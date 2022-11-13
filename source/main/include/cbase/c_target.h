#ifndef __D_BASE_TARGET_H__
#define __D_BASE_TARGET_H__

namespace ncore
{
    // Hardware enumeration
    enum eplatform
    {
        PLATFORM_NONE = 0,
        PLATFORM_PC   = (1 << 0),
        PLATFORM_MAC  = (1 << 1),
        PLATFORM_ALL  = (1 << 15),
        PLATFORM_PAD  = 0xffffffff
    };

#if defined(TARGET_PC)
    #if defined(TARGET_DEBUG)
        #if defined(TARGET_DEV)
            #define TARGET_PC_DEV_DEBUG
        #elif defined(TARGET_RETAIL)
            #define TARGET_PC_RETAIL_DEBUG
        #elif defined(TARGET_TEST)
            #define TARGET_PC_TEST_DEBUG
        #else
            #error "TARGET_DEBUG must be defined with TARGET_DEV, TARGET_RETAIL or TARGET_TEST"
        #endif
    #elif defined(TARGET_RELEASE)
        #if defined(TARGET_DEV)
            #define TARGET_PC_DEV_RELEASE
        #elif defined(TARGET_RETAIL)
            #define TARGET_PC_RETAIL_RELEASE
        #elif defined(TARGET_TEST)
            #define TARGET_PC_TEST_RELEASE
        #else
            #error "TARGET_RELEASE must be defined with TARGET_DEV, TARGET_RETAIL or TARGET_TEST"
        #endif
    #elif defined(TARGET_FINAL)
        #if defined(TARGET_DEV)
            #define TARGET_PC_DEV_FINAL
        #elif defined(TARGET_RETAIL)
            #define TARGET_PC_RETAIL_FINAL
        #elif defined(TARGET_TEST)
            #define TARGET_PC_TEST_FINAL
        #else
            #error "TARGET_FINAL must be defined with TARGET_DEV, TARGET_RETAIL or TARGET_TEST"
        #endif
    #else
        #error "TARGET_DEBUG, TARGET_RELEASE or TARGET_FINAL must be defined"
    #endif
#elif defined(TARGET_MAC)
    #if defined(TARGET_DEBUG)
        #if defined(TARGET_DEV)
            #define TARGET_MAC_DEV_DEBUG
        #elif defined(TARGET_RETAIL)
            #define TARGET_MAC_RETAIL_DEBUG
        #elif defined(TARGET_TEST)
            #define TARGET_MAC_TEST_DEBUG
        #else
            #error "TARGET_DEBUG must be defined with TARGET_DEV, TARGET_RETAIL or TARGET_TEST"
        #endif
    #elif defined(TARGET_RELEASE)
        #if defined(TARGET_DEV)
            #define TARGET_MAC_DEV_RELEASE
        #elif defined(TARGET_RETAIL)
            #define TARGET_MAC_RETAIL_RELEASE
        #elif defined(TARGET_TEST)
            #define TARGET_MAC_TEST_RELEASE
        #else
            #error "TARGET_RELEASE must be defined with TARGET_DEV, TARGET_RETAIL or TARGET_TEST"
        #endif
    #elif defined(TARGET_FINAL)
        #if defined(TARGET_DEV)
            #define TARGET_MAC_DEV_FINAL
        #elif defined(TARGET_RETAIL)
            #define TARGET_MAC_RETAIL_FINAL
        #elif defined(TARGET_TEST)
            #define TARGET_MAC_TEST_FINAL
        #else
            #error "TARGET_FINAL must be defined with TARGET_DEV, TARGET_RETAIL or TARGET_TEST"
        #endif
    #else
        #error "TARGET_DEBUG, TARGET_RELEASE or TARGET_FINAL must be defined"
    #endif
#elif defined(TARGET_LINUX)
    #if defined(TARGET_DEBUG)
        #if defined(TARGET_DEV)
            #define TARGET_LINUD_DEV_DEBUG
        #elif defined(TARGET_RETAIL)
            #define TARGET_LINUD_RETAIL_DEBUG
        #elif defined(TARGET_TEST)
            #define TARGET_LINUD_TEST_DEBUG
        #else
            #error "TARGET_DEBUG must be defined with TARGET_DEV, TARGET_RETAIL or TARGET_TEST"
        #endif
    #elif defined(TARGET_RELEASE)
        #if defined(TARGET_DEV)
            #define TARGET_LINUD_DEV_RELEASE
        #elif defined(TARGET_RETAIL)
            #define TARGET_LINUD_RETAIL_RELEASE
        #elif defined(TARGET_TEST)
            #define TARGET_LINUD_TEST_RELEASE
        #else
            #error "TARGET_RELEASE must be defined with TARGET_DEV, TARGET_RETAIL or TARGET_TEST"
        #endif
    #elif defined(TARGET_FINAL)
        #if defined(TARGET_DEV)
            #define TARGET_LINUD_DEV_FINAL
        #elif defined(TARGET_RETAIL)
            #define TARGET_LINUD_RETAIL_FINAL
        #elif defined(TARGET_TEST)
            #define TARGET_LINUD_TEST_FINAL
        #else
            #error "TARGET_FINAL must be defined with TARGET_DEV, TARGET_RETAIL or TARGET_TEST"
        #endif
    #else
        #error "TARGET_DEBUG, TARGET_RELEASE or TARGET_FINAL must be defined"
    #endif
#else
    #error "TARGET_PC, TARGET_MAC or TARGET_LINUX must be defined"
#endif

#undef TARGET_PC
#undef TARGET_MAC
#undef TARGET_LINUX
#undef TARGET_DEBUG
#undef TARGET_RELEASE
#undef TARGET_FINAL
#undef TARGET_DEV
#undef TARGET_RETAIL
#undef TARGET_TEST

    //
    // Mac OS Targets
    //

#ifdef TARGET_MAC_TEST_DEBUG
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_DEBUG
#        define TARGET_TEST
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_TEST_DEV
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_DEV
#        define TARGET_TEST
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_TEST_RELEASE
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_RELEASE
#        define TARGET_TEST
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_MAC_DEV_DEBUG) || defined(TARGET_MAC_DEBUG)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_DEBUG
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_MAC_DEV_DEV) || defined(TARGET_MAC_DEV)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_DEV
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_MAC_DEV_RELEASE) || defined(TARGET_MAC_RELEASE)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_RELEASE
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_MAC_DEV_FINAL) || defined(TARGET_MAC_FINAL)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_FINAL
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_CLIENT_DEBUG
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_CLIENT
#        define TARGET_DEBUG
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_CLIENT_DEV
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_CLIENT
#        define TARGET_DEV
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_CLIENT_RELEASE
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_CLIENT
#        define TARGET_RELEASE
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_CLIENT_FINAL
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_CLIENT
#        define TARGET_RELEASE
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_RETAIL_DEBUG
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_RETAIL
#        define TARGET_DEBUG
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_RETAIL_DEV
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_RETAIL
#        define TARGET_DEV
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_RETAIL_RELEASE
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_RETAIL
#        define TARGET_RELEASE
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_MAC_RETAIL_FINAL
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_RETAIL
#        define TARGET_FINAL
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

//
// PC Targets
//
#if defined(TARGET_PC_TEST_DEBUG)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_DEBUG
#        define TARGET_TEST
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_TEST_DEV)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_DEV
#        define TARGET_TEST
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_TEST_RELEASE)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_RELEASE
#        define TARGET_TEST
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_DEBUG) || defined(TARGET_PC_DEV_DEBUG)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_DEBUG
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_DEV) || defined(TARGET_PC_DEV_DEV)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_DEV
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_RELEASE) || defined(TARGET_PC_DEV_RELEASE)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_RELEASE
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_FINAL) || defined(TARGET_PC_DEV_FINAL)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_RETAIL
#        define TARGET_FINAL
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_CLIENT_DEBUG)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_CLIENT
#        define TARGET_DEBUG
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_CLIENT_DEV)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_CLIENT
#        define TARGET_DEV
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_CLIENT_RELEASE)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_CLIENT
#        define TARGET_RELEASE
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_CLIENT_FINAL)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_CLIENT
#        define TARGET_FINAL
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_RETAIL_DEBUG)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_RETAIL
#        define TARGET_DEBUG
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_RETAIL_DEV)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_RETAIL
#        define TARGET_DEV
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#        define D_DEBUG
#    endif
#endif

    //------------------------------------------------------------------------------

#if defined(TARGET_PC_RETAIL_FINAL)
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_RETAIL
#        define TARGET_FINAL
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_PC_EDITOR_DEBUG
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_MFC
#        define TARGET_DEBUG
#        define TARGET_PLATFORM PLATFORM_PC
#        define TARGET_EDITOR
#        define VALID_TARGET
//#define D_USE_NATIVE_NEW_AND_DELETE
//#define __PLACEMENT_NEW_INLINE      // Tells MFC that we are dealing with the placement new/delete
#        define D_EXCEPTIONS
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_PC_EDITOR_DEV
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_MFC
#        define TARGET_DEV
#        define TARGET_PLATFORM PLATFORM_PC
#        define TARGET_EDITOR
#        define VALID_TARGET
//#define D_USE_NATIVE_NEW_AND_DELETE
//#define __PLACEMENT_NEW_INLINE      // Tells MFC that we are dealing with the placement new/delete
#        define D_EXCEPTIONS
#    endif
#endif

    //------------------------------------------------------------------------------

#ifdef TARGET_PC_EDITOR_RELEASE
#    ifdef VALID_TARGET
#        define MULTIPLE_TARGETS
#    else
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_DEVKIT
#        define TARGET_MFC
#        define TARGET_RELEASE
#        define TARGET_PLATFORM PLATFORM_PC
#        define TARGET_EDITOR
#        define VALID_TARGET
//#define D_USE_NATIVE_NEW_AND_DELETE
//#define __PLACEMENT_NEW_INLINE      // Tells MFC that we are dealing with the placement new/delete
#        define D_EXCEPTIONS
#    endif
#endif

    //  Make sure we found a proper target specification.  If you get a compilation
    //  error here, then your compilation environment is not specifying one of the
    //  target macros.
    //

#ifndef VALID_TARGET
#    ifdef _MSC_VER // and we are using the Microsoft compiler...
#        undef TARGET_PC
#        define TARGET_PC
#        define TARGET_64BIT
#        define TARGET_PLATFORM PLATFORM_PC
#        define VALID_TARGET

#        ifdef _DEBUG
#            define TARGET_DEBUG
#            define D_DEBUG
#        else
#            define TARGET_DEV
#        endif
#    elif defined(__APPLE__) && defined(__clang__)
#        undef TARGET_MAC
#        define TARGET_MAC
#        define TARGET_64BIT
#        define TARGET_PLATFORM PLATFORM_MAC
#        define VALID_TARGET

#        ifdef _DEBUG
#            define TARGET_DEBUG
#            define D_DEBUG
#        else
#            define TARGET_DEV
#        endif
#    else
#        error x_target, error; Target specification invalid or not found.
#        error x_target, error; The compilation environment must define one of the macros listed in x_targets.h
#    endif
#endif

//
//  Make sure we have defined the architecture type, TARGET_32BIT or TARGET_64BIT
//
#ifdef VALID_TARGET
#    if !defined(TARGET_32BIT) && !defined(TARGET_64BIT)
// define it by checking sizeof(void*)
#        if defined(_M_X64) || defined(__x86_64__) || defined(__LP64__)
#            define TARGET_64BIT
#        elif defined(__i386__)
#            define TARGET_32BIT
#        else
#            error x_target, error; Unknown target architecture type, only 32-bit or 64-bit are supported
#        endif
#    endif
#endif

    //  generate string containing the target description
    //
    //  TARGET_PC + TARGET_DEBUG +

#ifdef TARGET_PLATFORM_STR
#    undef TARGET_PLATFORM_STR
#endif

#if defined(TARGET_MAC)
#    define TARGET_PLATFORM_STR "MACOS"
#elif defined(TARGET_PC)
#    define TARGET_PLATFORM_STR "PC"
#elif defined(TARGET_LINUX)
#    define TARGET_PLATFORM_STR "LINUX"
#endif

#if defined(TARGET_DEBUG)
#    if defined(TARGET_TEST)
#        define TARGET_CONFIG_SHORT_STR "_TB"
#    else
#        define TARGET_CONFIG_SHORT_STR "_B"
#    endif
#endif
#if defined(TARGET_DEV)
#    if defined(TARGET_TEST)
#        define TARGET_CONFIG_SHORT_STR "_TD"
#    else
#        define TARGET_CONFIG_SHORT_STR "_D"
#    endif
#endif
#if defined(TARGET_RELEASE)
#    if defined(TARGET_TEST)
#        define TARGET_CONFIG_SHORT_STR "_TR"
#    else
#        define TARGET_CONFIG_SHORT_STR "_R"
#    endif
#endif
#if defined(TARGET_FINAL)
#    define TARGET_CONFIG_SHORT_STR "_F"
#endif

#if defined(TARGET_DEBUG)
#    if defined(TARGET_TEST)
#        define TARGET_CONFIG_FULL_STR "_TESTDEBUG"
#    else
#        define TARGET_CONFIG_FULL_STR "_DEBUG"
#    endif
#endif
#if defined(TARGET_DEV)
#    if defined(TARGET_TEST)
#        define TARGET_CONFIG_FULL_STR "_TESTDEV"
#    else
#        define TARGET_CONFIG_FULL_STR "_DEV"
#    endif
#endif
#if defined(TARGET_RELEASE)
#    if defined(TARGET_TEST)
#        define TARGET_CONFIG_FULL_STR "_TESTRELEASE"
#    else
#        define TARGET_CONFIG_FULL_STR "_RELEASE"
#    endif
#endif
#if defined(TARGET_FINAL)
#    define TARGET_CONFIG_FULL_STR "_FINAL"
#endif

#if defined(TARGET_EDITOR)
#    define TARGET_EDITOR_STR "_EDITOR"
#else
#    define TARGET_EDITOR_STR ""
#endif

#if defined(TARGET_DEVKIT)
#    define TARGET_HW_STR "_DEVKIT"
#endif
#if defined(TARGET_RETAIL)
#    define TARGET_HW_STR "_RETAIL"
#endif
#if defined(TARGET_CLIENT)
#    define TARGET_HW_STR "_CLIENT"
#endif
#if !defined(TARGET_HW_STR)
#    define TARGET_HW_STR ""
#endif

#if defined(TARGET_32BIT)
#    define TARGET_BIT_STR "_32"
#elif defined(TARGET_64BIT)
#    define TARGET_BIT_STR "_64"
#else
#    define TARGET_BIT_STR "_32"
#endif

#define TARGET_FULL_DESCR_STR TARGET_PLATFORM_STR TARGET_HW_STR TARGET_EDITOR_STR TARGET_CONFIG_FULL_STR TARGET_BIT_STR

    //
    //  Make sure we did not somehow get multiple target platform specifications.
    //  *** IF YOU GOT AN ERROR HERE ***, then you have defined more than one of
    //  the target specification macros.
    //

#ifdef MULTIPLE_TARGETS
#    pragma message(TARGET_FULL_DESCR_STR)
#    error error; Multiple target specification definition macros were detected.
#    error error; The compilation environment must define only one of the macros listed in x_targets.h
#endif

    //
    //  Operating System Designation
    //


    //
    //  Compiler Designation
    //

#undef COMPILER_MSVC
#undef COMPILER_CLANG

#undef COMPILER_DEFAULT
#undef COMPILER_VERSION

#ifdef TARGET_PC
#    define TARGET_OS_WINDOWS
#    ifdef _MSC_VER

    // 1200      == VC++ 6.0
    // 1200-1202 == VC++ 4
    // 1300      == VC++ 7.0
    // 1400      == VC++ 8.0
    // 1500      == VC++ 9.0
    // 1600      == VC++ 10.0 (Visual Studio 2010)
    // 1700      == VC++ 11.0 (Visual Studio 2012)
    // 1800      == VC++ 12.0 (Visual Studio 2013)

#        if _MSC_VER < 1400
#        elif _MSC_VER == 1400
#            define COMPILER_MSVC
#            define COMPILER_VERSION 2005
#        elif _MSC_VER == 1500
#            define COMPILER_MSVC
#            define COMPILER_DEFAULT
#            define COMPILER_VERSION 2008
#        elif _MSC_VER == 1600
#            define COMPILER_MSVC
#            define COMPILER_DEFAULT
#            define COMPILER_VERSION 2010
#        elif _MSC_VER == 1700
#            define COMPILER_MSVC
#            define COMPILER_DEFAULT
#            define COMPILER_VERSION 2012
#        elif _MSC_VER == 1800
#            define COMPILER_MSVC
#            define COMPILER_DEFAULT
#            define COMPILER_VERSION 2013
#        elif _MSC_VER >= 1900 && _MSC_VER < 1911
#            define COMPILER_MSVC
#            define COMPILER_DEFAULT
#            define COMPILER_VERSION 2015
#        elif _MSC_VER >= 1911
#            define COMPILER_MSVC
#            define COMPILER_DEFAULT
#            define COMPILER_VERSION 2017
#        else
#            error x_target, error; Unknown _MSVC_VER compiler version
#        endif
#    elif defined(__clang__)
#        define COMPILER_CLANG
#        define COMPILER_DEFAULT
#        define COMPILER_VERSION 7
#    else
#        error x_target, error; This compiler is not supported for TARGET_PC
#    endif
#elif defined(TARGET_MAC)
#    ifdef __clang__
#        define TARGET_OS_MAC
#        define COMPILER_CLANG
#        define COMPILER_DEFAULT
#        define COMPILER_VERSION 7
#    else
#        error x_target, error; This compiler is not supported for TARGET_MAC
#    endif
#else
#    error x_target, error; This compiler is not supported for TARGET_UNKNOWN
#endif

#undef D_CHAR_BIT

/// This one is for Windows; Microsoft Developer Studio Visual C/C++ Compiler
#if defined(COMPILER_MSVC)
#    define D_NO_CUSTOM_INT64
#    define D_NO_CUSTOM_UINT64
    class s128;
    class u128;
    class s256;
    class u256;

    template <bool> struct CompileTimeAssert;
    template <> struct CompileTimeAssert<true>
    {
        enum
        {
            OK = 0
        };
    };

//#define D_NO_PARTIAL_TEMPLATE
#    define D_CACHE_LINE_SIZE 32
#    define D_CHAR_BIT        8
#    define D_IEEE_FLOATS
#    define D_USE_PRAGMA_ONCE
#    define D_STD_CALL     __stdcall
#    define D_C_DECL       __cdecl
#    define f_inline       __forceinline
#    define f_noinline     __declspec(noinline)
#    define D_FORCE_INLINE __forceinline
#    define D_BOOL         signed long
#    define D_BYTE         unsigned char
#    define D_WCHAR        unsigned short
#    define D_INT8         char
#    define D_INT16        short
#    define D_INT32        int
#    define D_INT64        signed __int64
#    define D_UINT64       unsigned __int64
#    define D_INT128       s128
#    define D_UINT128      u128
#    define D_INT256       s256
#    define D_UINT256      u256
#    ifdef TARGET_64BIT
#        define D_SIZE              unsigned __int64
#        define D_INT               __int64
#        define D_PTR_SIZED_INT     __int64
#        define D_ALIGNMENT_DEFAULT 8
#    else
#        define D_SIZE              unsigned int
#        define D_INT               int
#        define D_PTR_SIZED_INT     int
#        define D_ALIGNMENT_DEFAULT 4
#    endif
#    define D_LITTLE_ENDIAN 4321
#    define D_FLOAT         float
#    define D_DOUBLE        double
#    define D_ALIGNMENT(a)  __declspec(align(a))

#    define D_ALIGN_BEGIN(a) __declspec(align(a))
#    define D_ALIGN_END(a)

#    define ALIGNOF(t) __alignof(t)

#    define D_OFFSET_OF(type, member) (D_SIZE) & reinterpret_cast<const volatile char&>((((type*)0)->member))

#    pragma inline_depth(255)

#    define D_THREAD_LOCAL __declspec(thread)
#    define D_FINAL        final

/// disable useless warnings
#    pragma warning(disable : 4800)

#elif defined(COMPILER_CLANG)

/// #pragma message "clang compiler"

#    define D_NO_CUSTOM_INT64
#    define D_NO_CUSTOM_UINT64
    class s128;
    class u128;
    class s256;
    class u256;

#    define __NO_PARTIAL_TEMPLATE__
#    define D_CACHE_LINE_SIZE 32
#    define D_CHAR_BIT        8
#    define D_USE_PRAGMA_ONCE
#    define D_STD_CALL         __stdcall
#    define D_C_DECL           __cdecl
#    define f_inline           __attribute__((always_inline))
#    define f_noinline         __attribute__((noinline))
#    define D_FORCE_INLINE     __attribute__((always_inline))
#    define D_FORCE_INLINE_SPU __attribute__((always_inline))

#    define D_BOOL    unsigned int
#    define D_BYTE    unsigned char
#    define D_WCHAR   unsigned short
#    define D_INT8    char
#    define D_INT16   short
#    define D_INT32   int
#    define D_INT64   signed long 
#    define D_UINT64  unsigned long 
#    define D_INT128  s128
#    define D_UINT128 u128
#    define D_INT256  s256
#    define D_UINT256 u256

    template <bool> struct CompileTimeAssert;
    template <> struct CompileTimeAssert<true>
    {
        enum
        {
            OK = 0
        };
    };

#    ifdef TARGET_64BIT
#        define D_SIZE              unsigned long 
#        define D_INT               signed long 
#        define D_PTR_SIZED_INT     signed long 
#        define D_ALIGNMENT_DEFAULT 8
#    else
#        define D_SIZE              unsigned int
#        define D_INT               signed int
#        define D_PTR_SIZED_INT     int
#        define D_ALIGNMENT_DEFAULT 4
#    endif

#    define D_LITTLE_ENDIAN 4321
#    define D_FLOAT         float
#    define D_DOUBLE        double
#    define D_ALIGNMENT(a)  __attribute__((aligned(a)))

#    define D_ALIGN_BEGIN(a)
#    define D_ALIGN_END(a) __attribute__((aligned(a)))

#    define D_OFFSET_OF(type, member) (D_SIZE) & reinterpret_cast<const volatile char&>((((type*)0)->member))

#    define D_ALIGNOF(t) __alignof(t)

#    define D_THREAD_LOCAL __declspec(thread)
#    define D_FINAL        final

#else
#    error x_target, error; no compiler selected
#endif

#if (!defined(D_BIG_ENDIAN) && !defined(D_LITTLE_ENDIAN))
#    error x_target, error; endian is not defined.
#endif

#if (defined(D_BIG_ENDIAN) && defined(D_LITTLE_ENDIAN))
#    error x_target, error; both endian specifications are defined!
#endif

    // Multi-threading configuration

#if defined(TARGET_PC) || defined(TARGET_MAC)
#    define TARGET_MULTI_CORE
#else
#    define TARGET_SINGLE_CORE
#endif

    //
    //  Memory manager configuration
    //

#if defined(TARGET_MULTI_CORE)
#    define TARGET_MULTITHREADED_MEMORY_MANAGER
#endif

    //
    //  handle configuration specific options
    //

#if !defined(D_DEBUG)
#    undef D_ASSERT
#else
#    ifndef D_ASSERT
#        define D_ASSERT
#    endif
#endif

    //
    // 64 bit constant support
    //

#ifndef D_CONSTANT_64
/// Assume compiler needs/accepts standard prefix for int64 types (long long)
#    define D_CONSTANT_64(x) x##LL
#endif

#ifndef D_CONSTANT_U64
/// Assume compiler needs/accepts standard prefix for uint64 types (unsigned long long)
#    define D_CONSTANT_U64(x) x##ULL
#endif

    //
    // unsigned 32 bit constant support
    //

#ifndef D_CONSTANT_S32
#    define D_CONSTANT_S32(x) x##L
#endif
#ifndef D_CONSTANT_U32
#    define D_CONSTANT_U32(x) x##UL
#endif

    //==============================================================================
    //
    // cross platform types
    //
    //==============================================================================
    // DOM-IGNORE-BEGIN
    //==============================================================================

    typedef unsigned D_INT8  u8;
    typedef unsigned D_INT16 u16;
    typedef unsigned D_INT32 u32;
    typedef D_UINT64         u64;
    typedef D_UINT128        u128;
    typedef D_UINT256        u256;
    typedef signed D_INT8    s8;
    typedef signed D_INT16   s16;
    typedef signed D_INT32   s32;
    typedef D_INT64          s64;
    typedef D_INT128         s128;
    typedef D_INT256         s256;
    typedef D_FLOAT          f32;
    typedef D_DOUBLE         f64;
    typedef D_SIZE           uint_t;
    typedef D_INT            int_t;
    typedef D_PTR_SIZED_INT  ptr_t;
    typedef D_WCHAR          wchar;
    
    //==============================================================================
    // ASCII + UTF 8, 16, 32
    typedef char uchar;
    typedef u8   uchar8;
    typedef u16  uchar16;
    typedef u32  uchar32;

    //==============================================================================
    // Interfaces: binary reader and writer
    //==============================================================================
    class writer_t
    {
    public:
        virtual ~writer_t() {}
        virtual s64 write(u8 const*, s64) = 0;
    };

    class reader_t
    {
    public:
        virtual ~reader_t() {}
        virtual s64 read(u8*, s64) = 0;
    };

    //==============================================================================
    // KB, MB, GB, TB values
    //==============================================================================
    const u64 cKB  = (u64)1024;
    const u64 cMB  = (u64)1024 * 1024;
    const u64 cGB  = (u64)1024 * 1024 * 1024;
    const u64 cTB  = (u64)1024 * 1024 * 1024 * 1024;

    //==============================================================================
    // Memory Alignment
    //==============================================================================

    enum ememalign
    {
        MEMALIGN_PC    = 8,
        MEMALIGN_MACOS = 8,
#if defined(TARGET_PC)
        MEMALIGN = MEMALIGN_PC,
#elif defined(TARGET_MAC)
        MEMALIGN = MEMALIGN_MACOS,
#else
#    error x_target, error unknown platform
#endif
        D__PAD = 0xffffffff
    };

    //==============================================================================
    // Min/Max values
    //==============================================================================

    const u8 cU8Min = (u8)0x00;  ///< minimum value of a u8.
    const u8 cU8Max = (u8)0xFF;  ///< maximum value of a u8.
    const s8 cS8Min = (s8)-0x80; ///< minimum value of a s8.
    const s8 cS8Max = (s8)0x7F;  ///< maximum value of a s8.

    const u16 cU16Min = (u16)0x0000;  ///< minimum value of a u16
    const u16 cU16Max = (u16)0xFFFF;  ///< maximum value of a u16.
    const s16 cS16Min = (s16)-0x8000; ///< minimum value of a s16.
    const s16 cS16Max = (s16)0x7FFF;  ///< maximum value of a s16.

    const u32 cU32Min = (u32)0x00000000; ///< minimum value of a u32.
    const u32 cU32Max = (u32)0xFFFFFFFF; ///< maximum value of a u32.
    const s32 cS32Min = (s32)0x80000000; ///< minimum value of a s32.
    const s32 cS32Max = (s32)0x7FFFFFFF; ///< maximum value of a s32.

    const u64 cU64Min = (u64)0x0000000000000000ull; ///< minimum value of a u64.
    const u64 cU64Max = (u64)0xFFFFFFFFFFFFFFFFull; ///< maximum value of a u64.
    const s64 cS64Min = (s64)0x8000000000000000ll;  ///< minimum value of a s64.
    const s64 cS64Max = (s64)0x7FFFFFFFFFFFFFFFll;  ///< maximum value of a s64.

    const f32 cF32Min = (f32)1.175494351e-38f; ///< minimum value of a f32.
    const f32 cF32Max = (f32)3.402823466e+38f; ///< maximum value of a f32.
    const f32 cF32Eps = (f32)0.0001f;          ///< default epsilon generally good to check values in the range [0 - 1], normalisations, dot products and such.

    const f64 cF64Min = (f64)2.2250738585072014e-308; ///< minimum value of a f64.
    const f64 cF64Max = (f64)1.7976931348623158e+308; ///< maximum value of a f64.

    //==============================================================================
    //  HANDLE BOOL
    //==============================================================================
    const bool True  = true;  ///< the value of True to true
    const bool False = false; ///< the value of False to false
    const bool TRUE  = true;  ///< the value of True to true
    const bool FALSE = false; ///< the value of False to false

    #define D_FOR_I(__from, __to) for (s32 i=__from; i<__to; ++i)
    #define D_FOR(__type, __var, __from, __to) for (__type __var=__from; __var<__to; ++__var)

}; // namespace ncore

#endif
