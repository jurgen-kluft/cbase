#ifndef __X_BASE_TARGET_H__
#define __X_BASE_TARGET_H__

// Author:
//     Virtuos Games
// Summary:
//     This file assists with cross platform development by
//     providing a standard set of platform definitions with
//     anticipated variations.
// Description:
//     This file assists with cross platform development by
//     providing a standard set of platform definitions with
//     anticipated variations. The selection of a platform and
//     variation is based on a key macro definition.
//     The target macro is provided in different ways depending on
//     the development environment. But usually it is provided by
//     the user when he sets the build configuration in the
//     development environment.
//     <table>
//     Target Macro                \Description                           MFC Safe?
//     --------------------------  -------------------------------------  ----------
//     none                        Assumes TARGET_PC_EDITOR                    Y
//     TARGET_PC_EDITOR_DEBUG      Editors                                     Y
//     TARGET_PC_EDITOR_RELEASE    Editors                                     Y
//     TARGET_PC_TOOL_DEBUG        Tools, command line, etc                    Y
//     TARGET_PC_TOOL_RELEASE      Tools, command line, etc                    Y
//     TARGET_PC_DEV_DEBUG         Windows PC DevKit Debug                     N
//     TARGET_PC_DEV_RELEASE       Windows PC DevKit Release                   N
//     TARGET_PC_DEV_FINAL         Windows PC DevKit Final                     N
//     TARGET_PC_CLIENT_DEBUG      Windows PC "Debug Station" Debug            N
//     TARGET_PC_CLIENT_RELEASE    Windows PC "Debug Station" Release          N
//     TARGET_PC_CLIENT_FINAL      Windows PC "Debug Station" Final            N
//     TARGET_PC_RETAIL_DEBUG      Windows PC Console Debug                    N
//     TARGET_PC_RETAIL_RELEASE    Windows PC Console Release                  N
//     TARGET_PC_RETAIL_FINAL      Windows PC Console Final                    N
//     TARGET_PC_PROFILE_DEBUG     Windows PC Profile Debug                    N
//     TARGET_PC_PROFILE_RELEASE   Windows PC Profile Release                  N
//     TARGET_PC_TEST_DEBUG        Windows PC Test Debug                       N
//     TARGET_PC_TEST_RELEASE      Windows PC Test Release                     N
//     -------------------------------------------------------------------------
//     TARGET_MAC_EDITOR_DEBUG     Editors                                     N
//     TARGET_MAC_EDITOR_RELEASE   Editors                                     N
//     TARGET_MAC_TOOL_DEBUG       Tools, command line, etc                    N
//     TARGET_MAC_TOOL_RELEASE     Tools, command line, etc                    N
//     TARGET_MAC_DEV_DEBUG        Windows PC DevKit Debug                     N
//     TARGET_MAC_DEV_RELEASE      Windows PC DevKit Release                   N
//     TARGET_MAC_DEV_FINAL        Windows PC DevKit Final                     N
//     TARGET_MAC_CLIENT_DEBUG     Windows PC "Debug Station" Debug            N
//     TARGET_MAC_CLIENT_RELEASE   Windows PC "Debug Station" Release          N
//     TARGET_MAC_CLIENT_FINAL     Windows PC "Debug Station" Final            N
//     TARGET_MAC_RETAIL_DEBUG     Windows PC Console Debug                    N
//     TARGET_MAC_RETAIL_RELEASE   Windows PC Console Release                  N
//     TARGET_MAC_RETAIL_FINAL     Windows PC Console Final                    N
//     TARGET_MAC_PROFILE_DEBUG    Windows PC Profile Debug                    N
//     TARGET_MAC_PROFILE_RELEASE  Windows PC Profile Release                  N
//     TARGET_MAC_TEST_DEBUG       Windows PC Test Debug                       N
//     TARGET_MAC_TEST_RELEASE     Windows PC Test Release                     N
//     -------------------------------------------------------------------------
//     </table>
//     Targets which are "MFC safe" (and have _MFC in the macro)
//     will disable the x_files version of operators new and delete.
//     Graphic engines prepared for MFC configurations should not
//     provide a main() function.
//     When present, each of the primary target macros in the list
//     above will, in turn, cause other secondary macros to be
//     defined. These Secondary macros are set automatically for the
//     user.
//     <table>
//     Secondary Macro   \Description
//     ----------------  --------------------------------------
//     TARGET_PC         Platform PC
//     TARGET_PC_EDITOR  Platform PC Editor
//     TARGET_MFC        MFC
//     TARGET_MAC        Platform Mac OS
//     TARGET_DEVKIT     \on DevKit
//     TARGET_CLIENT     \on "Debug Station"
//     TARGET_RETAIL     \on Console
//     TARGET_DEBUG      Debug
//     TARGET_DEV        Development, optimized Debug
//     TARGET_RELEASE    Release, including some Debug stuff
//     TARGET_FINAL      Final, optimized codes, excluding all the Debug stuff
//     TARGET_TEST       Test (UnitTest, exceptions are enabled)
//     </table>
//     <table>
//     Target Macro                Secondary Macros
//     --------------------------  ---------------------------
//     none                        TARGET_PC TARGET_DEBUG   TARGET_EDITOR TARGET_MFC
//     TARGET_PC_EDITOR_DEBUG      TARGET_PC TARGET_DEBUG   TARGET_EDITOR TARGET_MFC
//     TARGET_PC_EDITOR_RELEASE    TARGET_PC TARGET_RELEASE TARGET_EDITOR TARGET_MFC
//     TARGET_PC_DEBUG             == TARGET_PC_DEV_DEBUG
//     TARGET_PC_DEV               == TARGET_PC_DEV_DEV
//     TARGET_PC_RELEASE           == TARGET_PC_DEV_RELEASE
//     TARGET_PC_FINAL             == TARGET_PC_DEV_FINAL
//     TARGET_PC_DEV_DEBUG         TARGET_PC TARGET_DEVKIT TARGET_DEBUG
//     TARGET_PC_DEV_DEV           TARGET_PC TARGET_DEVKIT TARGET_DEV
//     TARGET_PC_DEV_RELEASE       TARGET_PC TARGET_DEVKIT TARGET_RELEASE
//     TARGET_PC_DEV_FINAL         TARGET_PC TARGET_DEVKIT TARGET_FINAL
//     TARGET_PC_CLIENT_DEBUG      TARGET_PC TARGET_CLIENT TARGET_DEBUG
//     TARGET_PC_CLIENT_DEV        TARGET_PC TARGET_CLIENT TARGET_DEV
//     TARGET_PC_CLIENT_RELEASE    TARGET_PC TARGET_CLIENT TARGET_RELEASE
//     TARGET_PC_CLIENT_FINAL      TARGET_PC TARGET_CLIENT TARGET_FINAL
//     TARGET_PC_RETAIL_DEBUG      TARGET_PC TARGET_RETAIL TARGET_DEBUG
//     TARGET_PC_RETAIL_DEV        TARGET_PC TARGET_RETAIL TARGET_DEV
//     TARGET_PC_RETAIL_RELEASE    TARGET_PC TARGET_RETAIL TARGET_RELEASE
//     TARGET_PC_RETAIL_FINAL      TARGET_PC TARGET_RETAIL TARGET_FINAL
//     TARGET_PC_TEST_DEBUG        TARGET_PC TARGET_DEBUG  TARGET_TEST
//     TARGET_PC_TEST_DEV          TARGET_PC TARGET_DEV    TARGET_TEST
//     TARGET_PC_TEST_RELEASE      TARGET_PC TARGET_RELEASE TARGET_TEST
//     TARGET_MAC_DEV_DEBUG        TARGET_MAC TARGET_DEVKIT TARGET_DEBUG
//     TARGET_MAC_DEV_DEV          TARGET_MAC TARGET_DEVKIT TARGET_DEV
//     TARGET_MAC_DEV_RELEASE      TARGET_MAC TARGET_DEVKIT TARGET_RELEASE
//     TARGET_MAC_DEV_FINAL        TARGET_MAC TARGET_DEVKIT TARGET_FINAL
//     TARGET_MAC_CLIENT_DEBUG     TARGET_MAC TARGET_CLIENT TARGET_DEBUG
//     TARGET_MAC_CLIENT_DEV       TARGET_MAC TARGET_CLIENT TARGET_DEV
//     TARGET_MAC_CLIENT_RELEASE   TARGET_MAC TARGET_CLIENT TARGET_RELEASE
//     TARGET_MAC_CLIENT_FINAL     TARGET_MAC TARGET_CLIENT TARGET_FINAL
//     TARGET_MAC_RETAIL_DEBUG     TARGET_MAC TARGET_RETAIL TARGET_DEBUG
//     TARGET_MAC_RETAIL_DEV       TARGET_MAC TARGET_RETAIL TARGET_DEV
//     TARGET_MAC_RETAIL_RELEASE   TARGET_MAC TARGET_RETAIL TARGET_RELEASE
//     TARGET_MAC_RETAIL_FINAL     TARGET_MAC TARGET_RETAIL TARGET_FINAL
//     TARGET_MAC_TEST_DEBUG       TARGET_MAC TARGET_DEBUG  TARGET_TEST
//     TARGET_MAC_TEST_DEV         TARGET_MAC TARGET_DEV    TARGET_TEST
//     TARGET_MAC_TEST_RELEASE     TARGET_MAC TARGET_RELEASE TARGET_TEST
//     </table>
//     A platform defines the high-level hardware but there is still one variable
//     that we must specify and that is 32-bit or 64-bit. This has mainly an impact
//     on sizeof(void*), for 32-bit this results in 4, meaning a pointer will occupy
//     4 bytes on a 32-bit system. On a 64-bit system this will be 8 bytes.
//     <table>
//     Dat Size Macro     \Description              \Version   Platform
//     -----------------  ------------------------  ---------  ---------
//     X_32BIT            32-bit hardware systems   <2005		PC,PS2,PSP,PS3,XBOX,XBOX360,NDS,N3DS,WII
//     X_64BIT            64-bit hardware systems   >2008       PC,XBOXONE,PS4,SWITCH
//     </table>
//     Finally the user needs to define in the configuration which
//     kind of build is he trying to do. X_DEBUG could be use even
//     if the compiler is set in optimization.
//     <table>
//     Target Macro   \Description
//     -------------  -------------------------------------------
//     none           Ignores debugging features such asserts.
//     X_DEBUG        Activates debugging features such asserts.
//     </table>
//     The target macro is provided in different ways depending on
//     the compiler.
//     <table>
//     Compiler Macro     \Description              \Version   Platform
//     -----------------  ------------------------  ---------  ---------
//     COMPILER_WINDOWS_MSVC      Microsoft Visual Studio   2005       PC
//     COMPILER_WINDOWS_MSVC      Microsoft Visual Studio   2008       PC
//     COMPILER_WINDOWS_MSVC      Microsoft Visual Studio   2010       PC
//     COMPILER_WINDOWS_MSVC      Microsoft Visual Studio   2015       PC
//     COMPILER_WINDOWS_MSVC      Microsoft Visual Studio   2017       PC
//     COMPILER_WINDOWS_CLANG     CLang                     7.0        PC
// 
//     COMPILER_MACOS_CLANG       CLang                     7.0        MacOS
// 
//     </table>
//     Other Macros provided to the user automatically are:
//     <table>
//     Target Macro      \Description
//     ----------------  ---------------------------
//     X_ALIGNMENT       alignment definition.
//     X_BIG_ENDIAN      Endianness of the hardware
//     X_LITTLE_ENDIAN   Endianness of the hardware
//     </table>
//     Finally there is a convenient platform enumeration that can
//     be use with logical operations. ex: (PLATFORM_PC | PLATFORM_360)
// ---------------------------------------------------------------------------------
// 

namespace xcore
{
	// Hardware enumeration
	enum x_platform
	{
		X_PLATFORM_NONE             = 0,
		X_PLATFORM_PC               = (1<<0),
		X_PLATFORM_MAC              = (1<<1),
		X_PLATFORM_ALL              = (1<<15),
		X_PLATFORM_PAD              = 0xffffffff
	};

	#undef TARGET_PC
	#undef TARGET_MAC

	//
	//
	//  Check for ambiguous or insufficiently qualified target specification.
	//
	// >>> IF YOU GOT AN ERROR HERE <<<, then you specified a target platform
	//  without sufficiently qualifying the target.
	//
	///
	#if !defined(TARGET_MAC_DEV_DEBUG) && !defined(TARGET_MAC_DEV_DEV) && !defined(TARGET_MAC_DEV_RELEASE) && !defined(TARGET_MAC_DEV_FINAL) \
		&& !defined(TARGET_MAC_CLIENT_DEBUG) && !defined(TARGET_MAC_CLIENT_DEV) && !defined(TARGET_MAC_CLIENT_RELEASE) && !defined(TARGET_MAC_CLIENT_FINAL) \
			&& !defined(TARGET_MAC_RETAIL_DEBUG) && !defined(TARGET_MAC_RETAIL_DEV) && !defined(TARGET_MAC_RETAIL_RELEASE) && !defined(TARGET_MAC_RETAIL_FINAL)
		#ifdef TARGET_MAC
			#error x_target, error; Incorrect target specification.
			#error x_target, error; Use either TARGET_MAC_DEV, TARGET_MAC_CLIENT, or TARGET_MAC_RETAIL.
		#endif
	#else
		#undef TARGET_MAC
	#endif

	#if defined(PLATFORM_64BIT)
		#define TARGET_64BIT
	#elif defined(PLATFORM_32BIT)
		#define TARGET_32BIT
	#endif

	//
	// Mac OS Targets
	//

	#ifdef TARGET_MAC_TEST_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_TEST_DEV
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_DEVKIT
			#define TARGET_DEV
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_TEST_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_MAC_DEV_DEBUG) || defined(TARGET_MAC_DEBUG)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_MAC_DEV_DEV) || defined(TARGET_MAC_DEV)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_DEVKIT
			#define TARGET_DEV
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
		#endif
	#endif
	
	//------------------------------------------------------------------------------

	#if defined(TARGET_MAC_DEV_RELEASE) || defined(TARGET_MAC_RELEASE)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_MAC_DEV_FINAL) || defined(TARGET_MAC_FINAL)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_DEVKIT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_CLIENT_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_CLIENT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_CLIENT_DEV
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_CLIENT
			#define TARGET_DEV
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
		#endif
	#endif
	
	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_CLIENT_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_CLIENT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_CLIENT_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_CLIENT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_RETAIL_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_RETAIL
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_RETAIL_DEV
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_RETAIL
			#define TARGET_DEV
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
		#endif
	#endif
	
	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_RETAIL_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_RETAIL
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_MAC_RETAIL_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_MAC
			#define TARGET_64BIT
			#define TARGET_RETAIL
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif


	//
	// PC Targets
	//
	#if defined(TARGET_PC_TEST_DEBUG)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_TEST_DEV)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_DEVKIT
			#define TARGET_DEV
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_TEST_RELEASE)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_DEBUG) || defined(TARGET_PC_DEV_DEBUG)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_DEV) || defined(TARGET_PC_DEV_DEV)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_DEVKIT
			#define TARGET_DEV
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
		#endif
	#endif
	
	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_RELEASE) || defined(TARGET_PC_DEV_RELEASE)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_FINAL) || defined(TARGET_PC_DEV_FINAL)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_RETAIL
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_CLIENT_DEBUG)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_CLIENT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_CLIENT_DEV)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_CLIENT
			#define TARGET_DEV
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
		#endif
	#endif
	
	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_CLIENT_RELEASE)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_CLIENT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_CLIENT_FINAL)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_CLIENT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_RETAIL_DEBUG)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_RETAIL
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_RETAIL_DEV)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_RETAIL
			#define TARGET_DEV
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
			#define X_DEBUG
			#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_PC_RETAIL_FINAL)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_RETAIL
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PC_EDITOR_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_DEVKIT
			#define TARGET_MFC
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define TARGET_EDITOR
			#define VALID_TARGET
			//#define X_USE_NATIVE_NEW_AND_DELETE
			//#define __PLACEMENT_NEW_INLINE      // Tells MFC that we are dealing with the placement new/delete
			#define X_EXCEPTIONS
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PC_EDITOR_DEV
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_DEVKIT
			#define TARGET_MFC
			#define TARGET_DEV
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define TARGET_EDITOR
			#define VALID_TARGET
			//#define X_USE_NATIVE_NEW_AND_DELETE
			//#define __PLACEMENT_NEW_INLINE      // Tells MFC that we are dealing with the placement new/delete
			#define X_EXCEPTIONS
		#endif
	#endif
	
	//------------------------------------------------------------------------------

	#ifdef TARGET_PC_EDITOR_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_DEVKIT
			#define TARGET_MFC
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define TARGET_EDITOR
			#define VALID_TARGET
			//#define X_USE_NATIVE_NEW_AND_DELETE
			//#define __PLACEMENT_NEW_INLINE      // Tells MFC that we are dealing with the placement new/delete
			#define X_EXCEPTIONS
		#endif
	#endif

	//  Make sure we found a proper target specification.  If you get a compilation
	//  error here, then your compilation environment is not specifying one of the
	//  target macros.
	//

	#ifndef VALID_TARGET
		#ifdef _MSC_VER                         // and we are using the Microsoft compiler...
			#undef TARGET_PC
			#define TARGET_PC
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET

			#ifdef _DEBUG
				#define TARGET_DEBUG
				#define X_DEBUG
			#else
				#define TARGET_DEV
			#endif
		#elif defined(__APPLE__) && defined(__clang__)
			#undef TARGET_MAC
			#define TARGET_MAC
			#define TARGET_PLATFORM X_PLATFORM_MAC
			#define VALID_TARGET

			#ifdef _DEBUG
				#define TARGET_DEBUG
				#define X_DEBUG
			#else
				#define TARGET_DEV
			#endif
		#else
			#error x_target, error; Target specification invalid or not found.
			#error x_target, error; The compilation environment must define one of the macros listed in x_targets.h
		#endif
	#endif

	//
	//  Make sure we have defined the architecture type, TARGET_32BIT or TARGET_64BIT
	//
	#ifdef VALID_TARGET
		#if !defined(TARGET_32BIT) && !defined(TARGET_64BIT)
			// define it by checking sizeof(void*)
			#if defined(_M_X64) || defined(__x86_64__) || defined(__LP64__)
				#define TARGET_64BIT
	 		#elif defined(__i386__)
	 			#define TARGET_32BIT
	 		#else
				#error x_target, error; Unknown target architecture type, only 32-bit or 64-bit are supported
			#endif
		#endif
	#endif


	 //  generate string containing the target description
	 //
	 //  TARGET_PC + TARGET_DEBUG +

	#ifdef TARGET_PLATFORM_STR
		#undef TARGET_PLATFORM_STR
	#endif

	#if   defined(TARGET_MAC)
		#define TARGET_PLATFORM_STR	"MACOS"
	#elif defined(TARGET_PC)
		#define TARGET_PLATFORM_STR	"PC"
	#endif

	#if defined(TARGET_DEBUG)
		#if defined(TARGET_TEST)
			#define TARGET_CONFIG_SHORT_STR		"_TB"
		#else
			#define TARGET_CONFIG_SHORT_STR		"_B"
		#endif
	#endif
	#if defined(TARGET_DEV)
		#if defined(TARGET_TEST)
			#define TARGET_CONFIG_SHORT_STR		"_TD"
		#else
			#define TARGET_CONFIG_SHORT_STR		"_D"
		#endif
	#endif
	#if defined(TARGET_RELEASE)
		#if defined(TARGET_TEST)
			#define TARGET_CONFIG_SHORT_STR		"_TR"
		#else
			#define TARGET_CONFIG_SHORT_STR		"_R"
		#endif
	#endif
	#if defined(TARGET_FINAL)
		#define TARGET_CONFIG_SHORT_STR		"_F"
	#endif

	#if defined(TARGET_DEBUG)
		#if defined(TARGET_TEST)
			#define TARGET_CONFIG_FULL_STR		"_TESTDEBUG"
		#else
			#define TARGET_CONFIG_FULL_STR		"_DEBUG"
		#endif
	#endif
	#if defined(TARGET_DEV)
		#if defined(TARGET_TEST)
			#define TARGET_CONFIG_FULL_STR		"_TESTDEV"
		#else
			#define TARGET_CONFIG_FULL_STR		"_DEV"
		#endif
	#endif
	#if defined(TARGET_RELEASE)
		#if defined(TARGET_TEST)
			#define TARGET_CONFIG_FULL_STR		"_TESTRELEASE"
		#else
			#define TARGET_CONFIG_FULL_STR		"_RELEASE"
		#endif
	#endif
	#if defined(TARGET_FINAL)
		#define TARGET_CONFIG_FULL_STR		"_FINAL"
	#endif

	#if defined(TARGET_EDITOR)
		#define TARGET_EDITOR_STR		"_EDITOR"
	#else
		#define TARGET_EDITOR_STR		""
	#endif

	#if defined(TARGET_DEVKIT)
		#define TARGET_HW_STR			"_DEVKIT"
	#endif
	#if defined(TARGET_RETAIL)
		#define TARGET_HW_STR			"_RETAIL"
	#endif
	#if defined(TARGET_CLIENT)
		#define TARGET_HW_STR			"_CLIENT"
	#endif
	#if !defined(TARGET_HW_STR)
		#define TARGET_HW_STR			""
	#endif

	#if defined(TARGET_32BIT)
		#define TARGET_BIT_STR			"_32"
	#elif defined(TARGET_64BIT)
		#define TARGET_BIT_STR			"_64"
	#else
		#define TARGET_BIT_STR			"_32"
	#endif

	#define	TARGET_FULL_DESCR_STR		TARGET_PLATFORM_STR TARGET_HW_STR TARGET_EDITOR_STR TARGET_CONFIG_FULL_STR TARGET_BIT_STR


	 //
	 //  Make sure we did not somehow get multiple target platform specifications.
	 //  *** IF YOU GOT AN ERROR HERE ***, then you have defined more than one of
	 //  the target specification macros.
	 //

	#ifdef MULTIPLE_TARGETS
		#pragma message(TARGET_FULL_DESCR_STR)
		#error x_target, error; Multiple target specification definition macros were detected.
		#error x_target, error; The compilation environment must define only one of the macros listed in x_targets.h
	#endif

	 //
	 //  Compiler Designation
	 //

	#undef COMPILER_WINDOWS_MSVC
	#undef COMPILER_MACOS_CLANG
	
	#undef COMPILER_DEFAULT
	#undef COMPILER_VERSION

	#ifdef TARGET_PC
		#ifdef _MSC_VER

			 // 1200      == VC++ 6.0
			 // 1200-1202 == VC++ 4
			 // 1300      == VC++ 7.0
			 // 1400      == VC++ 8.0
			 // 1500      == VC++ 9.0
			 // 1600      == VC++ 10.0 (Visual Studio 2010)
			 // 1700      == VC++ 11.0 (Visual Studio 2012)
			 // 1800      == VC++ 12.0 (Visual Studio 2013)

			#if _MSC_VER < 1400
			#elif _MSC_VER == 1400
				#define COMPILER_WINDOWS_MSVC
				#define COMPILER_VERSION            2005
			#elif _MSC_VER == 1500
				#define COMPILER_WINDOWS_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2008
			#elif _MSC_VER == 1600
				#define COMPILER_WINDOWS_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2010
			#elif _MSC_VER == 1700
				#define COMPILER_WINDOWS_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2012
			#elif _MSC_VER == 1800
				#define COMPILER_WINDOWS_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2013
			#elif _MSC_VER >= 1900 && _MSC_VER < 1911
				#define COMPILER_WINDOWS_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2015
			#elif _MSC_VER >= 1911
				#define COMPILER_WINDOWS_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2017
			#else
				#error x_target, error; Unknown _MSVC_VER compiler version
			#endif
		#else
			#error x_target, error; This compiler is not supported for TARGET_PC
		#endif
	#elif defined(TARGET_MAC)
		#ifdef __clang__
			#define COMPILER_MACOS_CLANG
			#define COMPILER_DEFAULT
			#define COMPILER_VERSION            7
		#else
			#error x_target, error; This compiler is not supported for TARGET_MAC
		#endif
	#else
		#error x_target, error; This compiler is not supported for TARGET_UNKNOWN
	#endif

	#undef X_CHAR_BIT

	/// This one is for Windows; Microsoft Developer Studio Visual C/C++ Compiler
	#if defined(COMPILER_WINDOWS_MSVC)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		template <bool> struct xCompileTimeAssert; 
		template<> struct xCompileTimeAssert<true> { enum { OK = 0 }; };

		//#define X_NO_PARTIAL_TEMPLATE
		#define X_CACHE_LINE_SIZE             32
		#define X_CHAR_BIT                    8
		#define X_IEEE_FLOATS
		#define X_USE_PRAGMA_ONCE
		#define X_STD_CALL                    __stdcall
		#define X_C_DECL                      __cdecl
		#define f_inline                      __forceinline
		#define f_noinline                    __declspec(noinline)
		#define X_FORCE_INLINE                __forceinline
		#define X_BOOL                        signed long
		#define X_BYTE                        unsigned char
		#define X_WCHAR                       unsigned short
		#define X_INT8                        char
		#define X_INT16                       short
		#define X_INT32                       int
		#define X_INT64                       signed __int64
		#define X_UINT64                      unsigned __int64
		#define X_INT128                      __xint128
		#define X_UINT128                     __xuint128
		#define X_INT256                      __xint256
		#define X_UINT256                     __xuint256
#ifdef TARGET_64BIT
		#define X_SIZE                        unsigned __int64
		#define X_PTR_SIZED_INT               __int64
		#define X_ALIGNMENT_DEFAULT           8
#else
		#define X_SIZE                        unsigned int
		#define X_PTR_SIZED_INT               int
		#define X_ALIGNMENT_DEFAULT           4
#endif
		#define X_LITTLE_ENDIAN               4321
		#define X_FLOAT                       float
		#define X_DOUBLE                      double
		#define X_ALIGNMENT(a)                __declspec(align(a))

		#define X_ALIGN_BEGIN(a)              __declspec(align(a))
		#define X_ALIGN_END(a)

		#define X_OFFSET_OF(type, member)     (X_SIZE)&reinterpret_cast<const volatile char&>((((type *)0)->member))

		#pragma inline_depth(255)

		#define X_THREAD_LOCAL                __declspec(thread)

		/// disable useless warnings
		#pragma warning(disable:4800)

	#elif defined(COMPILER_MACOS_CLANG)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		#define __NO_PARTIAL_TEMPLATE__
		#define X_CACHE_LINE_SIZE             32
		#define X_CHAR_BIT                    8
		#define X_USE_PRAGMA_ONCE
		#define X_STD_CALL                    __stdcall
		#define X_C_DECL                      __cdecl
		#define f_inline					  __attribute__((always_inline))
		#define f_noinline                    __attribute__((noinline))
		#define X_FORCE_INLINE                __attribute__((always_inline))
		#define X_FORCE_INLINE_SPU			  __attribute__((always_inline))

		#define X_BOOL                        unsigned int
		#define X_BYTE                        unsigned char
		#define X_WCHAR                       unsigned short
		#define X_INT8                        char
		#define X_INT16                       short
		#define X_INT32                       int
		#define X_INT64                       signed long
		#define X_UINT64                      unsigned long
		#define X_INT128                      __xint128
		#define X_UINT128                     __xuint128
		#define X_INT256                      __xint256
		#define X_UINT256                     __xuint256

		template <bool> struct xCompileTimeAssert; 
		template<> struct xCompileTimeAssert<true> { enum { OK = 0 }; };


#ifdef TARGET_64BIT
		#define X_SIZE                        unsigned long
		#define X_PTR_SIZED_INT               signed long
		#define X_ALIGNMENT_DEFAULT           8
#else
		#define X_SIZE                        unsigned int
		#define X_PTR_SIZED_INT               int
		#define X_ALIGNMENT_DEFAULT           4
#endif

		static inline void TestTypeSizes()
		{
			xCompileTimeAssert<sizeof(X_SIZE) == sizeof(void*)>(); 
			xCompileTimeAssert<sizeof(X_PTR_SIZED_INT) == sizeof(void*)>(); 

			xCompileTimeAssert<sizeof(int) == 4>(); 
			xCompileTimeAssert<sizeof(short int) == 2>(); 
			xCompileTimeAssert<sizeof(long int) == 8>(); 

			xCompileTimeAssert<sizeof(X_BOOL) == 4>(); 
			xCompileTimeAssert<sizeof(X_INT8) == 1>(); 	
			xCompileTimeAssert<sizeof(X_INT16) == 2>(); 
			xCompileTimeAssert<sizeof(X_INT32) == 4>(); 
			xCompileTimeAssert<sizeof(X_INT64) == 8>(); 
			xCompileTimeAssert<sizeof(X_UINT64) == 8>(); 
		}
		
		#define X_LITTLE_ENDIAN               4321
		#define X_FLOAT                       float
		#define X_DOUBLE                      double
		#define X_ALIGNMENT(a)                __attribute__((aligned(a)))

		#define X_ALIGN_BEGIN(a)
		#define X_ALIGN_END(a)				  __attribute__((aligned(a)))

		#define X_OFFSET_OF(type, member)     (X_SIZE)&reinterpret_cast<const volatile char&>((((type *)0)->member))

		#define X_THREAD_LOCAL                __declspec(thread)

	#else
		#error x_target, error; no compiler selected
	#endif

	#if(!defined(X_BIG_ENDIAN) && !defined(X_LITTLE_ENDIAN))
		#error x_target, error; endian is not defined.
	#endif

	#if(defined(X_BIG_ENDIAN) &&  defined(X_LITTLE_ENDIAN))
		#error x_target, error; both endian specifications are defined!
	#endif

	enum x_memalign
	{
		X_MEMALIGN_PC               = 8,
		X_MEMALIGN_MACOS            = 8,
	#if defined(TARGET_PC)
		X_MEMALIGN			= X_MEMALIGN_PC,
	#elif defined(TARGET_MAC)
		X_MEMALIGN			= X_MEMALIGN_MACOS,
	#else
		#error x_target, error; need to have X_MEMALIGN defined
	#endif
		X__PAD              = 0xffffffff
	};


	// Multi-threading configuration


	#if defined(TARGET_PC) || defined(TARGET_MAC)
		#define TARGET_MULTI_CORE
	#else
		#define TARGET_SINGLE_CORE
	#endif


	 //
	 //  Memory manager configuration
	 //

	#if defined(TARGET_MULTI_CORE)
		#define TARGET_MULTITHREADED_MEMORY_MANAGER
	#endif

	 //
	 //  handle configuration specific options
	 //

	#if !defined(X_DEBUG)
		#undef X_ASSERT
	#else
		#ifndef X_ASSERT
			#define X_ASSERT
		#endif
	#endif

	 //
	 // 64 bit constant support
	 //

	#ifndef X_CONSTANT_64
		/// Assume compiler needs/accepts standard prefix for int64 types (long long)
		#define    X_CONSTANT_64(x)            x##LL
	#endif

	#ifndef X_CONSTANT_U64
		/// Assume compiler needs/accepts standard prefix for uint64 types (unsigned long long)
		#define    X_CONSTANT_U64(x)            x##ULL
	#endif


	 //
	 // unsigned 32 bit constant support
	 //

	#ifndef X_CONSTANT_S32
		#define X_CONSTANT_S32(x)		        x##L
	#endif
	#ifndef X_CONSTANT_U32
		#define X_CONSTANT_U32(x)		        x##UL
	#endif

	//==============================================================================
	//
	// cross platform types
	//
	//==============================================================================
	//DOM-IGNORE-BEGIN
	//==============================================================================

	#ifndef NULL
		#define NULL        0
	#endif

	typedef		unsigned		X_INT8   			u8;
	typedef		unsigned		X_INT16  			u16;
	typedef		unsigned		X_INT32  			u32;
	typedef						X_UINT64 			u64;
	typedef						X_UINT128			u128;
	typedef						X_UINT256			u256;
	typedef		signed      	X_INT8   			s8;
	typedef		signed      	X_INT16  			s16;
	typedef		signed      	X_INT32  			s32;
	typedef						X_INT64  			s64;
	typedef						X_INT128 			s128;
	typedef						X_INT256 			s256;
	typedef						X_FLOAT  			f32;
	typedef						X_DOUBLE 			f64;
	typedef						X_BYTE   			xbyte;
	typedef						X_BOOL				xbool32;
	typedef						bool				xbool;
	typedef						X_SIZE				xsize_t;
	typedef						X_PTR_SIZED_INT		uptr;

	//==============================================================================
	// UTF 8, 16, 32
	typedef		char			uchar;
	typedef		u8				uchar8;
	typedef		u16				uchar16;
	typedef		u32				uchar32;

	typedef		char*			astr;		// ASCII string
	typedef		char const*		acstr;		// ASCII string
	typedef		uchar8*			ustr8;		// UTF-8 string
	typedef		uchar8 const*	ucstr8;		// UTF-8 string
	typedef		uchar16*		ustr16;		// UTF-16 string
	typedef		uchar16 const*	ucstr16;	// UTF-16 string
	typedef		uchar32*		ustr32;		// UTF-32 string
	typedef		uchar32 const*	ucstr32;	// UTF-32 string

	//==============================================================================

	#define					XCAST(to, from)		((to)(from))

	//==============================================================================
	// Interfaces: binary reader and writer
	//==============================================================================
	class xwriter
	{
	public:
		virtual	~xwriter() {}
		virtual void 	write(xbyte const*, u32) = 0;
	};

	class xreader
	{
	public:
		virtual	~xreader() {}
		virtual s32 	read(xbyte*, u32) = 0;
	};

	//==============================================================================
	// Min/Max values
	//==============================================================================

	const u8						xU8Min = (u8) 0x00;							///< minimum value of a u8.
	const u8						xU8Max = (u8) 0xFF;							///< maximum value of a u8.
	const s8						xS8Min = (s8) -0x80;						///< minimum value of a s8.
	const s8						xS8Max = (s8) 0x7F;							///< maximum value of a s8.

	const u16						xU16Min = (u16) 0x0000;						///< minimum value of a u16
	const u16						xU16Max = (u16) 0xFFFF;						///< maximum value of a u16.
	const s16						xS16Min = (s16) -0x8000;					///< minimum value of a s16.
	const s16						xS16Max = (s16) 0x7FFF;						///< maximum value of a s16.

	const u32						xU32Min = (u32) 0x00000000;					///< minimum value of a u32.
	const u32						xU32Max = (u32) 0xFFFFFFFF;					///< maximum value of a u32.
	const s32						xS32Min = (s32) 0x80000000;					///< minimum value of a s32.
	const s32						xS32Max = (s32) 0x7FFFFFFF;					///< maximum value of a s32.

	const u64						xU64Min = (u64) 0x0000000000000000ull;		///< minimum value of a u64.
	const u64						xU64Max = (u64) 0xFFFFFFFFFFFFFFFFull;		///< maximum value of a u64.
	const s64						xS64Min = (s64) 0x8000000000000000ll;		///< minimum value of a s64.
	const s64						xS64Max = (s64) 0x7FFFFFFFFFFFFFFFll;		///< maximum value of a s64.

	const f32						xF32Min = (f32) 1.175494351e-38f;			///< minimum value of a f32.
	const f32						xF32Max = (f32) 3.402823466e+38f;			///< maximum value of a f32.
	const f32                       xF32Eps = (f32) 0.0001f;					///< default epsilon generally good to check values in the range [0 - 1], normalisations, dot products and such.

	const f64						xF64Min = (f64) 2.2250738585072014e-308;	///< minimum value of a f64.
	const f64						xF64Max = (f64) 1.7976931348623158e+308;	///< maximum value of a f64.

	//==============================================================================
	//  HANDLE BOOL
	//==============================================================================
	const xbool						xTrue  = true;								///< the value of True to true
	const xbool						xFalse = false;								///< the value of False to false
	const xbool						xTRUE  = true;								///< the value of True to true
	const xbool						xFALSE = false;								///< the value of False to false

};

#endif
