// ---------------------------------------------------------------------------------
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
//     
//     The target macro is provided in different ways depending on
//     the development environment. But usually it is provided by
//     the user when he sets the build configuration in the
//     development environment.
//     
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
//     TARGET_PS3_DEV_DEBUG        Sony PlayStation 3 DevKit Debug               N
//     TARGET_PS3_DEV_RELEASE      Sony PlayStation 3 DevKit Release             N
//     TARGET_PS3_DEV_FINAL        Sony PlayStation 3 DevKit Final               N
//     TARGET_PS3_CLIENT_DEBUG     Sony PlayStation 3 "Debug Station" Debug      N
//     TARGET_PS3_CLIENT_RELEASE   Sony PlayStation 3 "Debug Station" Release    N
//     TARGET_PS3_CLIENT_FINAL     Sony PlayStation 3 "Debug Station" Final      N
//     TARGET_PS3_RETAIL_DEBUG     Sony PlayStation 3 console Debug              N
//     TARGET_PS3_RETAIL_RELEASE   Sony PlayStation 3 console Release            N
//     TARGET_PS3_RETAIL_FINAL     Sony PlayStation 3 console Final              N
//     TARGET_PS3_PROFILE_DEBUG    Sony PlayStation 3 Profile Debug              N
//     TARGET_PS3_PROFILE_RELEASE  Sony PlayStation 3 Profile Release            N
//     TARGET_PS3_TEST_DEBUG       Sony PlayStation 3 Test Debug                 N
//     TARGET_PS3_TEST_RELEASE     Sony PlayStation 3 Test Release               N
//     TARGET_PSP_DEV_DEBUG        Sony PlayStation Portable DevKit Debug        N
//     TARGET_PSP_DEV_RELEASE      Sony PlayStation Portable DevKit Release      N
//     TARGET_PSP_DEV_FINAL        Sony PlayStation Portable DevKit Final        N
//     TARGET_PSP_CLIENT_DEBUG     Sony PlayStation Portable "Debug Debug        N
//     TARGET_PSP_CLIENT_RELEASE   Sony PlayStation Portable "Debug Release      N
//     TARGET_PSP_CLIENT_FINAL     Sony PlayStation Portable "Debug Final        N
//     TARGET_PSP_RETAIL_DEBUG     Sony PlayStation Portable Console Debug       N
//     TARGET_PSP_RETAIL_RELEASE   Sony PlayStation Portable Console Release     N
//     TARGET_PSP_RETAIL_FINAL     Sony PlayStation Portable Console Final       N
//     TARGET_PSP_PROFILE_DEBUG    Sony PlayStation Portable Profile Debug       N
//     TARGET_PSP_PROFILE_RELEASE  Sony PlayStation Portable Profile Release     N
//     TARGET_PSP_TEST_DEBUG       Sony PlayStation Portable Test Debug          N
//     TARGET_PSP_TEST_RELEASE     Sony PlayStation Portable Test Release        N
//     TARGET_360_DEV_DEBUG        Microsoft X-Box 360 DevKit Debug              N
//     TARGET_360_DEV_RELEASE      Microsoft X-Box 360 DevKit Release            N
//     TARGET_360_DEV_FINAL        Microsoft X-Box 360 DevKit Final              N
//     TARGET_360_CLIENT_DEBUG     Microsoft X-Box 360 "Debug Station" Debug     N
//     TARGET_360_CLIENT_RELEASE   Microsoft X-Box 360 "Debug Station" Release   N
//     TARGET_360_CLIENT_FINAL     Microsoft X-Box 360 "Debug Station" Final     N
//     TARGET_360_RETAIL_DEBUG     Microsoft X-Box 360 Console Debug             N
//     TARGET_360_RETAIL_RELEASE   Microsoft X-Box 360 Console Release           N
//     TARGET_360_RETAIL_FINAL     Microsoft X-Box 360 Console Final             N
//     TARGET_360_PROFILE_DEBUG    Microsoft X-Box 360 Profile Debug             N
//     TARGET_360_PROFILE_RELEASE  Microsoft X-Box 360 Profile Release           N
//     TARGET_360_TEST_DEBUG       Microsoft X-Box 360 Test Debug                N
//     TARGET_360_TEST_RELEASE     Microsoft X-Box 360 Test Release              N
//     TARGET_WII_DEV_DEBUG        Nintendo Wii DevKit Debug                     N
//     TARGET_WII_DEV_RELEASE      Nintendo Wii DevKit Release                   N
//     TARGET_WII_DEV_FINAL        Nintendo Wii DevKit Final                     N
//     TARGET_WII_CLIENT_DEBUG     Nintendo Wii "Debug Station" Debug            N
//     TARGET_WII_CLIENT_RELEASE   Nintendo Wii "Debug Station" Release          N
//     TARGET_WII_CLIENT_FINAL     Nintendo Wii "Debug Station" Final            N
//     TARGET_WII_RETAIL_DEBUG     Nintendo Wii Console Debug                    N
//     TARGET_WII_RETAIL_RELEASE   Nintendo Wii Console Release                  N
//     TARGET_WII_RETAIL_FINAL     Nintendo Wii Console Final                    N
//     TARGET_WII_PROFILE_DEBUG    Nintendo Wii Profile Debug                    N
//     TARGET_WII_PROFILE_RELEASE  Nintendo Wii Profile Release                  N
//     TARGET_WII_TEST_DEBUG       Nintendo Wii Test Debug                       N
//     TARGET_WII_TEST_RELEASE     Nintendo Wii Test Release                     N
//     TARGET_3DS_DEV_DEBUG        Nintendo 3ds DevKit Debug                     N
//     TARGET_3DS_DEV_RELEASE      Nintendo 3ds DevKit Release                   N
//     TARGET_3DS_DEV_FINAL        Nintendo 3ds DevKit Final                     N
//     TARGET_3DS_CLIENT_DEBUG     Nintendo 3ds "Debug Station" Debug            N
//     TARGET_3DS_CLIENT_RELEASE   Nintendo 3ds "Debug Station" Release          N
//     TARGET_3DS_CLIENT_FINAL     Nintendo 3ds "Debug Station" Final            N
//     TARGET_3DS_RETAIL_DEBUG     Nintendo 3ds Console Debug                    N
//     TARGET_3DS_RETAIL_RELEASE   Nintendo 3ds Console Release                  N
//     TARGET_3DS_RETAIL_FINAL     Nintendo 3ds Console Final                    N
//     TARGET_3DS_PROFILE_DEBUG    Nintendo 3ds Profile Debug                    N
//     TARGET_3DS_PROFILE_RELEASE  Nintendo 3ds Profile Release                  N
//     TARGET_3DS_TEST_DEBUG       Nintendo 3ds Test Debug                       N
//     TARGET_3DS_TEST_RELEASE     Nintendo 3ds Test Release                     N
//     </table>
//     
//     Targets which are "MFC safe" (and have _MFC in the macro)
//     will disable the x_files version of operators new and delete.
//     Graphic engines prepared for MFC configurations should not
//     provide a main() function.
//     
//     When present, each of the primary target macros in the list
//     above will, in turn, cause other secondary macros to be
//     defined. These Secondary macros are set automatically for the
//     user.
//     
//     
//     
//     <table>
//     Secondary Macro   \Description
//     ----------------  --------------------------------------
//     TARGET_PC         Platform PC
//     TARGET_PC_EDITOR  Platform PC Editor
//     TARGET_MFC        MFC
//     TARGET_PS3        Platform Sony PlayStation 3
//     TARGET_PSP        Platform Sony PlayStation Portable
//     TARGET_360        Platform Microsoft X-Box 360
//     TARGET_WII        Platform Nintendo Wii
//     TARGET_3DS        Platform Nintendo 3DS
//     TARGET_DEVKIT     \on DevKit
//     TARGET_CLIENT     \on "Debug Station"
//     TARGET_RETAIL     \on Console
//     TARGET_DEBUG      Debug
//     TARGET_RELEASE    Release, including some Debug stuff
//     TARGET_FINAL      Final, optimized codes, excluding all
//                        the Debug stuff
//     TARGET_TEST       Test (UnitTest, exceptions are enabled)
//     </table>
//     
//     <table>
//     Target Macro                Secondary Macros
//     --------------------------  ---------------------------
//     none                        TARGET_PC TARGET_DEBUG   TARGET_EDITOR TARGET_MFC
//     TARGET_PC_EDITOR_DEBUG      TARGET_PC TARGET_DEBUG   TARGET_EDITOR TARGET_MFC
//     TARGET_PC_EDITOR_RELEASE    TARGET_PC TARGET_RELEASE TARGET_EDITOR TARGET_MFC
//     TARGET_PC_DEBUG             == TARGET_PC_DEV_DEBUG
//     TARGET_PC_RELEASE           == TARGET_PC_DEV_RELEASE
//     TARGET_PC_FINAL             == TARGET_PC_DEV_FINAL
//     TARGET_PC_DEV_DEBUG         TARGET_PC TARGET_DEVKIT TARGET_DEBUG
//     TARGET_PC_DEV_RELEASE       TARGET_PC TARGET_DEVKIT TARGET_RELASE
//     TARGET_PC_DEV_FINAL         TARGET_PC TARGET_DEVKIT TARGET_FINAL
//     TARGET_PC_CLIENT_DEBUG      TARGET_PC TARGET_CLIENT TARGET_DEBUG
//     TARGET_PC_CLIENT_RELEASE    TARGET_PC TARGET_CLIENT TARGET_RELASE
//     TARGET_PC_CLIENT_FINAL      TARGET_PC TARGET_CLIENT TARGET_FINAL
//     TARGET_PC_RETAIL_DEBUG      TARGET_PC TARGET_RETAIL TARGET_DEBUG
//     TARGET_PC_RETAIL_RELEASE    TARGET_PC TARGET_RETAIL TARGET_RELASE
//     TARGET_PC_RETAIL_FINAL      TARGET_PC TARGET_RETAIL TARGET_FINAL
//     TARGET_PC_TEST_DEBUG        TARGET_PC TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
//     TARGET_PS3_TEST_RELEASE     TARGET_PC TARGET_DEVKIT TARGET_RELASE TARGET_TEST
//     TARGET_PS3_DEV_DEBUG        TARGET_PS3 TARGET_DEVKIT TARGET_DEBUG
//     TARGET_PS3_DEV_RELEASE      TARGET_PS3 TARGET_DEVKIT TARGET_RELASE
//     TARGET_PS3_DEV_FINAL        TARGET_PS3 TARGET_DEVKIT TARGET_FINAL
//     TARGET_PS3_CLIENT_DEBUG     TARGET_PS3 TARGET_CLIENT TARGET_DEBUG
//     TARGET_PS3_CLIENT_RELEASE   TARGET_PS3 TARGET_CLIENT TARGET_RELASE
//     TARGET_PS3_CLIENT_FINAL     TARGET_PS3 TARGET_CLIENT TARGET_FINAL
//     TARGET_PS3_RETAIL_DEBUG     TARGET_PS3 TARGET_RETAIL TARGET_DEBUG
//     TARGET_PS3_RETAIL_RELEASE   TARGET_PS3 TARGET_RETAIL TARGET_RELASE
//     TARGET_PS3_RETAIL_FINAL     TARGET_PS3 TARGET_RETAIL TARGET_FINAL
//     TARGET_PS3_TEST_DEBUG       TARGET_PS3 TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
//     TARGET_PS3_TEST_RELEASE     TARGET_PS3 TARGET_DEVKIT TARGET_RELASE TARGET_TEST
//     TARGET_PSP_DEV_DEBUG        TARGET_PSP TARGET_DEVKIT TARGET_DEBUG
//     TARGET_PSP_DEV_RELEASE      TARGET_PSP TARGET_DEVKIT TARGET_RELASE
//     TARGET_PSP_DEV_FINAL        TARGET_PSP TARGET_DEVKIT TARGET_FINAL
//     TARGET_PSP_CLIENT_DEBUG     TARGET_PSP TARGET_CLIENT TARGET_DEBUG
//     TARGET_PSP_CLIENT_RELEASE   TARGET_PSP TARGET_CLIENT TARGET_RELASE
//     TARGET_PSP_CLIENT_FINAL     TARGET_PSP TARGET_CLIENT TARGET_FINAL
//     TARGET_PSP_RETAIL_DEBUG     TARGET_PSP TARGET_RETAIL TARGET_DEBUG
//     TARGET_PSP_RETAIL_RELEASE   TARGET_PSP TARGET_RETAIL TARGET_RELASE
//     TARGET_PSP_RETAIL_FINAL     TARGET_PSP TARGET_RETAIL TARGET_FINAL
//     TARGET_PSP_TEST_DEBUG       TARGET_PSP TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
//     TARGET_PSP_TEST_RELEASE     TARGET_PSP TARGET_DEVKIT TARGET_RELASE TARGET_TEST
//     TARGET_360_DEV_DEBUG        TARGET_360 TARGET_DEVKIT TARGET_DEBUG
//     TARGET_360_DEV_RELEASE      TARGET_360 TARGET_DEVKIT TARGET_RELASE
//     TARGET_360_DEV_FINAL        TARGET_360 TARGET_DEVKIT TARGET_FINAL
//     TARGET_360_CLIENT_DEBUG     TARGET_360 TARGET_CLIENT TARGET_DEBUG
//     TARGET_360_CLIENT_RELEASE   TARGET_360 TARGET_CLIENT TARGET_RELEASE
//     TARGET_360_CLIENT_FINAL     TARGET_360 TARGET_CLIENT TARGET_FINAL
//     TARGET_360_RETAIL_DEBUG     TARGET_360 TARGET_RETAIL TARGET_DEBUG
//     TARGET_360_RETAIL_RELEASE   TARGET_360 TARGET_RETAIL TARGET_RELASE
//     TARGET_360_RETAIL_FINAL     TARGET_360 TARGET_RETAIL TARGET_FINAL
//     TARGET_360_TEST_DEBUG       TARGET_360 TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
//     TARGET_360_TEST_RELEASE     TARGET_360 TARGET_DEVKIT TARGET_RELASE TARGET_TEST
//     TARGET_WII_DEV_DEBUG        TARGET_WII TARGET_DEVKIT TARGET_DEBUG
//     TARGET_WII_DEV_RELEASE      TARGET_WII TARGET_DEVKIT TARGET_RELASE
//     TARGET_WII_DEV_FINAL        TARGET_WII TARGET_DEVKIT TARGET_FINAL
//     TARGET_WII_CLIENT_DEBUG     TARGET_WII TARGET_CLIENT TARGET_DEBUG
//     TARGET_WII_CLIENT_RELEASE   TARGET_WII TARGET_CLIENT TARGET_RELASE
//     TARGET_WII_CLIENT_FINAL     TARGET_WII TARGET_CLIENT TARGET_FINAL
//     TARGET_WII_RETAIL_DEBUG     TARGET_WII TARGET_RETAIL TARGET_DEBUG
//     TARGET_WII_RETAIL_RELEASE   TARGET_WII TARGET_RETAIL TARGET_RELASE
//     TARGET_WII_RETAIL_FINAL     TARGET_WII TARGET_RETAIL TARGET_FINAL
//     TARGET_WII_TEST_DEBUG       TARGET_WII TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
//     TARGET_WII_TEST_RELEASE     TARGET_WII TARGET_DEVKIT TARGET_RELASE TARGET_TEST
//     TARGET_3DS_DEV_DEBUG        TARGET_3DS TARGET_DEVKIT TARGET_DEBUG
//     TARGET_3DS_DEV_RELEASE      TARGET_3DS TARGET_DEVKIT TARGET_RELASE
//     TARGET_3DS_DEV_FINAL        TARGET_3DS TARGET_DEVKIT TARGET_FINAL
//     TARGET_3DS_CLIENT_DEBUG     TARGET_3DS TARGET_CLIENT TARGET_DEBUG
//     TARGET_3DS_CLIENT_RELEASE   TARGET_3DS TARGET_CLIENT TARGET_RELASE
//     TARGET_3DS_CLIENT_FINAL     TARGET_3DS TARGET_CLIENT TARGET_FINAL
//     TARGET_3DS_RETAIL_DEBUG     TARGET_3DS TARGET_RETAIL TARGET_DEBUG
//     TARGET_3DS_RETAIL_RELEASE   TARGET_3DS TARGET_RETAIL TARGET_RELASE
//     TARGET_3DS_RETAIL_FINAL     TARGET_3DS TARGET_RETAIL TARGET_FINAL
//     TARGET_3DS_TEST_DEBUG       TARGET_3DS TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
//     TARGET_3DS_TEST_RELEASE     TARGET_3DS TARGET_DEVKIT TARGET_RELASE TARGET_TEST
//     </table>
//     
//     Finally the user needs to define in the configuration which
//     kind of build is he trying to do. X_DEBUG could be use even
//     if the compiler is set in optimization.
//     
//     <table>
//     Target Macro   \Description
//     -------------  -------------------------------------------
//     none           Ignores debugging features such asserts.
//     X_DEBUG        Activates debugging features such asserts.
//     </table>
//     
//     The target macro is provided in different ways depending on
//     the compiler.
//     
//     <table>
//     Compiler Macro     \Description              \Version   Platform
//     -----------------  ------------------------  ---------  ---------
//     COMPILER_MSVC      Microsoft Visual Studio   2005       PC
//     COMPILER_MSVC      Microsoft Visual Studio   2008       PC
//     COMPILER_MSVC      Microsoft Visual Studio   2010       PC
//     COMPILER_MW_WII    Metrowerks                WII       WII
//     COMPILER_GCC_PSP   GCC                        PSP       PSP
//     COMPILER_SN_PSP    SNSystems                  PSP       PSP
//     COMPILER_SN_PS3    SNSystems                  PS3       PS3
//     </table>
//     
//     Other Macros provided to the user automatically are:
//     
//     <table>
//     Target Macro      \Description
//     ----------------  ---------------------------
//     X_ALIGNMENT       alignment definition.
//     X_BIG_ENDIAN      Endianness of the hardware
//     X_LITTLE_ENDIAN   Endianness of the hardware
//     </table>
//     
//     Finally there is a convenient platform enumeration that can
//     be use with logical operations. ex: (PLATFORM_PC | PLATFORM_360)                                                                
// ---------------------------------------------------------------------------------

#ifndef __XBASE_TARGET_H__
#define __XBASE_TARGET_H__

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//==============================================================================
	//  
	// Hardware enumeration
	//
	//==============================================================================
	enum x_platform
	{
		X_PLATFORM_NONE             = 0,
		X_PLATFORM_PC               = (1<<0),
		X_PLATFORM_PS3              = (1<<1),
		X_PLATFORM_PSP              = (1<<2),
		X_PLATFORM_360              = (1<<3),
		X_PLATFORM_WII              = (1<<4),
		X_PLATFORM_3DS              = (1<<5),
		X_PLATFORM_ALL              = (1<<15),
		X_PLATFORM_PAD              = 0xffffffff
	};

	#undef TARGET_PC
	#undef TARGET_PS3
	#undef TARGET_PSP
	#undef TARGET_360
	#undef TARGET_NDS
	#undef TARGET_WII
	#undef TARGET_3DS

	//==============================================================================
	//  
	//  Check for ambiguous or insufficiently qualified target specification.
	//  
	//  *** IF YOU GOT AN ERROR HERE ***, then you specified a target platform 
	//  without sufficiently qualifying the target.
	//
	//==============================================================================
	#if !defined(TARGET_PS3_DEV_DEBUG) && !defined(TARGET_PS3_DEV_RELEASE) && !defined(TARGET_PS3_DEV_FINAL) && !defined(TARGET_PS3_CLIENT_DEBUG) && !defined(TARGET_PS3_CLIENT_RELEASE) && !defined(TARGET_PS3_CLIENT_FINAL) && !defined(TARGET_PS3_RETAIL_DEBUG) && !defined(TARGET_PS3_RETAIL_RELEASE) && !defined(TARGET_PS3_RETAIL_FINAL)      
		#ifdef TARGET_PS3
			#error Incorrect target specification.
			#error Use either TARGET_PS3_DEV, TARGET_PS3_CLIENT, or TARGET_PS3_RETAIL.
		#endif
	#else
		#undef TARGET_PS3
	#endif

	#if !defined(TARGET_360_DEV_DEBUG) && !defined(TARGET_360_DEV_RELEASE) && !defined(TARGET_360_DEV_FINAL) && !defined(TARGET_360_CLIENT_DEBUG) && !defined(TARGET_360_CLIENT_RELEASE) && !defined(TARGET_360_CLIENT_FINAL) && !defined(TARGET_360_RETAIL_DEBUG) && !defined(TARGET_360_RETAIL_RELEASE) && !defined(TARGET_360_RETAIL_FINAL)      
		#ifdef TARGET_360
			#error TARGET_360 is not a sufficient target specification.
			#error Use either TARGET_360_DEV, TARGET_360_CLIENT, or TARGET_360_RETAIL.
		#endif
	#else
		#undef TARGET_360
	#endif

	#if !defined(TARGET_WII_DEV_DEBUG) && !defined(TARGET_WII_DEV_RELEASE) && !defined(TARGET_WII_DEV_FINAL) && !defined(TARGET_WII_CLIENT_DEBUG) && !defined(TARGET_WII_CLIENT_RELEASE) && !defined(TARGET_WII_CLIENT_FINAL) && !defined(TARGET_WII_RETAIL_DEBUG) && !defined(TARGET_WII_RETAIL_RELEASE) && !defined(TARGET_WII_RETAIL_FINAL)      
		#ifdef TARGET_WII
			#error TARGET_WII is not a sufficient target specification.
			#error Use either PLATFORM_WII_DEV_DEBUG, PLATFORM_WII_DEV_RELEASE or PLATFORM_WII_DEV_FINAL
		#endif
	#else
		#undef TARGET_WII
	#endif

	#if !defined(TARGET_3DS_DEV_DEBUG) && !defined(TARGET_3DS_DEV_RELEASE) && !defined(TARGET_3DS_DEV_FINAL) && !defined(TARGET_3DS_CLIENT_DEBUG) && !defined(TARGET_3DS_CLIENT_RELEASE) && !defined(TARGET_3DS_CLIENT_FINAL) && !defined(TARGET_3DS_RETAIL_DEBUG) && !defined(TARGET_3DS_RETAIL_RELEASE) && !defined(TARGET_3DS_RETAIL_FINAL)      
		#ifdef TARGET_3DS
			#error TARGET_3DS is not a sufficient target specification.
			#error Use either PLATFORM_3DS_DEV_DEBUG, PLATFORM_3DS_DEV_RELEASE or PLATFORM_3DS_DEV_FINAL
		#endif
	#else
		#undef TARGET_3DS
	#endif
	
	#if !defined(TARGET_PSP_DEV_DEBUG) && !defined(TARGET_PSP_DEV_RELEASE) && !defined(TARGET_PSP_DEV_FINAL) && !defined(TARGET_PSP_CLIENT_DEBUG) && !defined(TARGET_PSP_CLIENT_RELEASE) && !defined(TARGET_PSP_CLIENT_FINAL) && !defined(TARGET_PSP_RETAIL_DEBUG) && !defined(TARGET_PSP_RETAIL_RELEASE) && !defined(TARGET_PSP_RETAIL_FINAL)      
		#ifdef TARGET_PSP
			#error TARGET_PSP is not a sufficient target specification.
			#error Use either TARGET_PSP_DEV, TARGET_PSP_CLIENT, or TARGET_PSP_RETAIL.
		#endif
	#else
		#undef TARGET_PSP
	#endif

	//==============================================================================
	//  Playstation 3 Targets
	//==============================================================================

	#ifdef TARGET_PS3_TEST_DEBUG
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_DEVKIT
            #define TARGET_DEBUG
			#define TARGET_TEST
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
            #define X_DEBUG
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_TEST_RELEASE
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_DEVKIT
            #define TARGET_RELEASE
			#define TARGET_TEST
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_DEV_DEBUG
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_DEVKIT
            #define TARGET_DEBUG
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
            #define X_DEBUG
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_DEV_RELEASE
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_DEVKIT
            #define TARGET_RELEASE
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_DEV_FINAL
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_DEVKIT
            #define TARGET_FINAL
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
            #define X_DEBUG
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_CLIENT_DEBUG
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_CLIENT
            #define TARGET_DEBUG
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
            #define X_DEBUG
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_CLIENT_RELEASE
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_CLIENT
            #define TARGET_RELEASE
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
            #define X_DEBUG
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_CLIENT_FINAL
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_CLIENT
            #define TARGET_RELEASE
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
            #define X_DEBUG
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_RETAIL_DEBUG
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_RETAIL
            #define TARGET_DEBUG
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
            #define X_DEBUG
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_RETAIL_RELEASE
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_RETAIL
            #define TARGET_RELEASE
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
            #define X_DEBUG
        #endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PS3_RETAIL_FINAL
        #ifdef VALID_TARGET
            #define MULTIPLE_TARGETS
        #else
            #define TARGET_PS3
            #define TARGET_RETAIL
            #define TARGET_FINAL
            #define TARGET_PLATFORM X_PLATFORM_PS3
            #define VALID_TARGET
            #define X_DEBUG
        #endif
	#endif

	//------------------------------------------------------------------------------

	//==============================================================================
	//  Playstation Portable Targets
	//==============================================================================

	#ifdef TARGET_PSP_TEST_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_TEST_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_DEV_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_DEV_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_DEV_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_DEVKIT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_CLIENT_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_CLIENT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_CLIENT_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_CLIENT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_CLIENT_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_CLIENT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_RETAIL_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_RETAIL
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_RETAIL_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_RETAIL
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_PSP_RETAIL_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_RETAIL
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_PSP
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	//==============================================================================
	//  Nintendo Wii Targets
	//==============================================================================

	#ifdef TARGET_WII_TEST_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_TEST_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_DEV_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_DEV_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_DEV_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_DEVKIT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_CLIENT_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_CLIENT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_CLIENT_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_CLIENT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_CLIENT_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_CLIENT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_RETAIL_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_RETAIL
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_RETAIL_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_RETAIL
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_WII_RETAIL_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_WII
			#define TARGET_RETAIL
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_WII
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------


	//==============================================================================
	//  Nintendo 3DS Targets
	//==============================================================================

	#if defined(TARGET_3DS_TEST_DEBUG)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_TEST_RELEASE)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_DEV_DEBUG)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_DEV_RELEASE)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_DEV_FINAL)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_DEVKIT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_CLIENT_DEBUG)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_CLIENT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_CLIENT_RELEASE)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_CLIENT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_CLIENT_FINAL)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_CLIENT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_RETAIL_DEBUG)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_RETAIL
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_RETAIL_RELEASE)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_RETAIL
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#if defined(TARGET_3DS_RETAIL_FINAL)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_3DS
			#define TARGET_RETAIL
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_3DS
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	//==============================================================================
	//  Microsoft X-Box 360 Targets
	//==============================================================================

	#ifdef TARGET_360_TEST_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
            #define X_DEBUG        
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_TEST_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_TEST
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_DEV_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_DEVKIT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
            #define X_DEBUG        
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_DEV_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_DEVKIT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_DEV_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_DEVKIT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_CLIENT_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_CLIENT
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_CLIENT_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_CLIENT
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_CLIENT_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_CLIENT
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_RETAIL_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_RETAIL
			#define TARGET_DEBUG
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
            #define X_DEBUG
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_RETAIL_RELEASE
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_RETAIL
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
		#endif
	#endif

	//------------------------------------------------------------------------------

	#ifdef TARGET_360_RETAIL_FINAL
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_360
			#define TARGET_RETAIL
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_360
			#define VALID_TARGET
		#endif
	#endif

	//==============================================================================
	//  PC Targets
	//==============================================================================

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

	#if defined(TARGET_PC_RETAIL_RELEASE)
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PC
			#define TARGET_RETAIL
			#define TARGET_RELEASE
			#define TARGET_PLATFORM X_PLATFORM_PC
			#define VALID_TARGET
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
	        #define X_USE_NATIVE_NEW_AND_DELETE
			#define __PLACEMENT_NEW_INLINE      // Tells MFC that we are dealing with the placement new/delete
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
	        #define X_USE_NATIVE_NEW_AND_DELETE
			#define __PLACEMENT_NEW_INLINE      // Tells MFC that we are dealing with the placement new/delete
			#define X_EXCEPTIONS
		#endif
	#endif

	//==============================================================================
	//
	//  Make sure we found a proper target specification.  If you get a compilation 
	//  error here, then your compilation environment is not specifying one of the
	//  target macros.
	//
	//==============================================================================

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
                #define TARGET_RELEASE
            #endif

        #else 
	        #error Target specification invalid or not found.
	        #error The compilation environment must define one of the macros listed in x_targets.hpp.
        #endif
    #endif


	//==============================================================================
	//
	//  generate string containing the target description
	//
	//  TARGET_PC + TARGET_DEBUG + 
	//
	//
	//
	//==============================================================================
	#ifdef TARGET_PLATFORM_STR
		#undef TARGET_PLATFORM_STR
	#endif

	#if   defined(TARGET_360)
		#define TARGET_PLATFORM_STR	"X360"
	#elif defined(TARGET_PSP)
		#define TARGET_PLATFORM_STR	"PSP"
	#elif defined(TARGET_PS3)
		#define TARGET_PLATFORM_STR	"PS3"
	#elif defined(TARGET_WII)
		#define TARGET_PLATFORM_STR	"WII"
	#elif defined(TARGET_PC)
		#define TARGET_PLATFORM_STR	"PC"
	#endif

	#if defined(TARGET_DEBUG)
		#define TARGET_CONFIG_SHORT_STR		"_D"
	#endif
	#if defined(TARGET_RELEASE)
		#define TARGET_CONFIG_SHORT_STR		"_R"
	#endif
	#if defined(TARGET_FINAL)
		#define TARGET_CONFIG_SHORT_STR		"_F"
	#endif

	#if defined(TARGET_DEBUG)
		#define TARGET_CONFIG_FULL_STR		"_DEBUG"
	#endif
	#if defined(TARGET_RELEASE)
		#define TARGET_CONFIG_FULL_STR		"_RELEASE"
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
		#define TARGET_HW_STR			"_DEV"
	#endif
	#if defined(TARGET_RETAIL)
		#define TARGET_HW_STR			"_RETAIL"
	#endif
	#if defined(TARGET_CLIENT)
		#define TARGET_HW_STR			"_CLIENT"
	#endif

	#define	TARGET_FULL_DESCR_STR		TARGET_PLATFORM_STR TARGET_HW_STR TARGET_EDITOR_STR TARGET_CONFIG_FULL_STR



	//==============================================================================
	//
	//  Make sure we did not somehow get multiple target platform specifications.
	//  *** IF YOU GOT AN ERROR HERE ***, then you have defined more than one of
	//  the target specification macros.
	//
	//==============================================================================

	#ifdef MULTIPLE_TARGETS
	#pragma message(TARGET_FULL_DESCR_STR)
	#error Multiple target specification definition macros were detected.
	#error The compilation environment must define only one of the macros listed in x_targets.hpp.
	#endif

	//==============================================================================
	//
	//  Compiler Designation
	//
	//==============================================================================

	#undef COMPILER_MSVC
	#undef COMPILER_X360
	#undef COMPILER_MW_WII
	#undef COMPILER_3DS
	#undef COMPILER_GCC_PSP
	#undef COMPILER_SN_PSP
	#undef COMPILER_SN_PS2
	#undef COMPILER_SN_PS3
    #undef COMPILER_GCC_PS3
	#undef COMPILER_DEFAULT
	#undef COMPILER_VERSION

	#ifdef TARGET_PC
		#ifdef _MSC_VER
			// 1200      == VC++ 6.0
			// 1200-1202 == eVC++4
			// 1300      == VC++ 7.0
			// 1400      == VC++ 8.0
			// 1500      == VC++ 9.0
			#if _MSC_VER < 1400
			#elif _MSC_VER == 1400
				#define COMPILER_MSVC
				#define COMPILER_VERSION            2005
			#elif _MSC_VER == 1500
				#define COMPILER_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2008
			#elif _MSC_VER == 1600
				#define COMPILER_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2008
			#else
				#error wrong compiler ID
			#endif
		#else
			#error This compiler is not supported for TARGET_PC
		#endif
	#elif defined(TARGET_WII)
		#ifdef __CWCC__
			#define COMPILER_MW_WII
			#define COMPILER_DEFAULT
			#define COMPILER_VERSION            1
		#else
			#error This compiler is not supported for TARGET_WII
		#endif
	#elif defined(TARGET_3DS)
		#ifdef __arm__
			#define COMPILER_3DS
			#define COMPILER_DEFAULT
			#define COMPILER_VERSION            __ARMCC_VERSION
		#else
			#error This compiler is not supported for TARGET_3DS
		#endif
	#elif defined(TARGET_PSP)
		#ifdef __SNC__
			#define COMPILER_SN_PSP
			#define COMPILER_DEFAULT
			#define COMPILER_VERSION            1
		#elif defined(__GCC__)
			#error GCC for psp not defined
		#else
			#error This compiler is not supported for TARGET_PSP
		#endif
	#elif defined(TARGET_360)
		#if (_MSC_VER == 1400) || (_MSC_VER == 1500) || (_MSC_VER == 1600)
			#define COMPILER_X360
			#define COMPILER_DEFAULT
			#define COMPILER_VERSION            360
		#else
			#error The version of Microsoft Visual Studio compiler is not supported for TARGET_360
		#endif
    #elif defined(TARGET_PS3)
        #if defined(__SNC__)
            #define COMPILER_SN_PS3
            #define COMPILER_DEFAULT
            #define COMPILER_VERSION            1
        #elif defined(__GCC__)
            #define COMPILER_GCC_PS3
            #define COMPILER_DEFAULT
            #define COMPILER_VERSION            1
        #else
            #error PS3 complier not found
        #endif
	#else
		#error This compiler is not supported for TARGET_UNKNOWN
	#endif

	#undef X_CHAR_BIT

	//This one is for PC
	#if defined(COMPILER_MSVC)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		#define __NO_PARTIAL_TEMPLATE__
		#define UNALIGNED_OK
		#define CACHE_LINE_SIZE               32
		#define X_CHAR_BIT                      8
		#define IEEE_FLOATS
		#define USE_PRAGMA_ONCE
		#define STD_CALL                      __stdcall
		#define C_DECL                        __cdecl
		#define f_inline                      __forceinline
		#define f_noinline                   __declspec(noinline)
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
		#define X_SIZE                        unsigned int
		#define X_PTR_SIZED_INT               int
		#define X_LITTLE_ENDIAN               4321
		#define X_FLOAT                       float
		#define X_DOUBLE                      double
		#define X_ALIGNMENT_DEFAULT           4
		#define X_ALIGNMENT(a)                __declspec(align(a))

		#define X_ALIGN_BEGIN(a)              __declspec(align(a))
		#define X_ALIGN_END(a)                

		#pragma inline_depth(255)

		// LWSYNC & THREAD_LOCAL
		#define X_LWSYNC_MSVC
		#define X_LWSYNC
		#define X_THREAD_LOCAL                __declspec(thread)
		#define X_FORCE_INLINE                _forceinline

		#ifdef _WIN64
			#define X_TARGET_64BIT
		#else
			#define X_TARGET_32BIT
		#endif
		
		// disable useless warnings
		#pragma warning(disable:4800)

	#elif defined(COMPILER_MW_WII)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		#define UNALIGNED_OK
		#define CACHE_LINE_SIZE           32
		#define X_CHAR_BIT                  8
		#define f_inline                  inline
		#define f_noinline                __declspec(noinline)
		#define STD_CALL                  __stdcall
		#define C_DECL        
		#define X_BOOL                    signed long
		#define X_BYTE                    unsigned char
		#define X_WCHAR                   unsigned short
		#define X_INT8                    char
		#define X_INT16                   short
		#define X_INT32                   int
		#define X_INT64                   signed long long
		#define X_UINT64                  unsigned long long
		#define X_INT128                  __xint128
		#define X_UINT128                 __xuint128   
		#define X_INT256                  __xint256
		#define X_UINT256                 __xuint256
		#define X_SIZE                    unsigned long
		#define X_PTR_SIZED_INT           int
		#define X_BIG_ENDIAN              1234
		#define X_FLOAT                   float
		#define X_DOUBLE                  double
		#define X_ALIGNMENT_DEFAULT           4
		#define X_ALIGNMENT(a)            __attribute__((aligned(a)))

		#define X_ALIGN_BEGIN(a)          
		#define X_ALIGN_END(a)            __attribute__((aligned(a)))
		#define __GNU_C__

		#define USE_PRAGMA_ONCE
	#elif defined(COMPILER_3DS)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		#define UNALIGNED_OK
		#define CACHE_LINE_SIZE           32
		#define X_CHAR_BIT                  8
		#define f_inline                  __forceinline
		#define f_noinline                __declspec(noinline)
		#define STD_CALL                  __stdcall
		#define C_DECL        
		#define X_BOOL                    signed long
		#define X_BYTE                    unsigned char
		#define X_WCHAR                   unsigned short
		#define X_INT8                    char
		#define X_INT16                   short
		#define X_INT32                   int
		#define X_INT64                   signed __int64
		#define X_UINT64                  unsigned __int64
		#define X_INT128                  __xint128
		#define X_UINT128                 __xuint128   
		#define X_INT256                  __xint256
		#define X_UINT256                 __xuint256
		#define X_SIZE                    unsigned int
		#define X_PTR_SIZED_INT           int
		#define X_BIG_ENDIAN              1234
		#define X_FLOAT                   float
		#define X_DOUBLE                  double
		#define X_ALIGNMENT_DEFAULT           4
		#define X_ALIGNMENT(a)            __attribute__((aligned(a)))

		#define X_ALIGN_BEGIN(a)          
		#define X_ALIGN_END(a)            __attribute__((aligned(a)))
		#define __GNU_C__

		#define USE_PRAGMA_ONCE
		#pragma anon_unions
    #elif defined(COMPILER_GCC_PS3)
        #define X_NO_CUSTOM_INT64
        #define X_NO_CUSTOM_UINT64
        class __xint128;
        class __xuint128;
        class __xint256;
        class __xuint256;

        #define __NO_PARTIAL_TEMPLATE__
        #define UNALIGNED_OK
        #define CACHE_LINE_SIZE               32
        #define X_CHAR_BIT                      8
        #define USE_PRAGMA_ONCE
        #define STD_CALL                      __stdcall
        #define C_DECL                        __cdecl
        #define f_inline                      __forceinline
        #define f_noinline
        #define X_BOOL                        signed long
        #define X_BYTE                        unsigned char
        #define X_WCHAR                       unsigned short
        #define X_INT8                        char
        #define X_INT16                       short
        #define X_INT32                       int
        #define X_INT64                       signed long long
        #define X_UINT64                      unsigned long long
        #define X_INT128                      __xint128
        #define X_UINT128                     __xuint128   
        #define X_INT256                      __xint256
        #define X_UINT256                     __xuint256
#ifdef SPU
        #define X_SIZE                        long unsigned int
#else
		#define X_SIZE                        unsigned int
#endif
        #define X_PTR_SIZED_INT               int
        #define X_BIG_ENDIAN                  1234
        #define X_FLOAT                       float
        #define X_DOUBLE                      double
		#define X_ALIGNMENT_DEFAULT           4
        #define X_ALIGNMENT(a)                __attribute__((aligned(a)))

		#define X_ALIGN_BEGIN(a)              
		#define X_ALIGN_END(a)				  __attribute__((aligned(a)))              
		#define __GNU_C__

        // LWSYNC & THREAD_LOCAL
        #define X_LWSYNC_PS3
        #define X_LWSYNC
        #define X_THREAD_LOCAL                __declspec(thread)
        #define X_FORCE_INLINE                _forceinline

	#elif defined(COMPILER_SN_PS3)
        #define X_NO_CUSTOM_INT64
        #define X_NO_CUSTOM_UINT64
        class __xint128;
        class __xuint128;
        class __xint256;
        class __xuint256;

        #define __NO_PARTIAL_TEMPLATE__
        #define UNALIGNED_OK
        #define CACHE_LINE_SIZE               32
        #define X_CHAR_BIT                      8
        #define USE_PRAGMA_ONCE
        #define STD_CALL                      __stdcall
        #define C_DECL                        __cdecl
        #define f_inline					  __attribute__((always_inline))
        #define f_noinline
        #define X_BOOL                        signed long
        #define X_BYTE                        unsigned char
        #define X_WCHAR                       unsigned short
        #define X_INT8                        char
        #define X_INT16                       short
        #define X_INT32                       int
        #define X_INT64                       signed long long
        #define X_UINT64                      unsigned long long
        #define X_INT128                      __xint128
        #define X_UINT128                     __xuint128   
        #define X_INT256                      __xint256
        #define X_UINT256                     __xuint256
#ifdef SPU
		#define X_SIZE                        long unsigned int
#else
		#define X_SIZE                        unsigned int
#endif
        #define X_PTR_SIZED_INT               int
        #define X_BIG_ENDIAN                  1234
        #define X_FLOAT                       float
        #define X_DOUBLE                      double
		#define X_ALIGNMENT_DEFAULT           4
        #define X_ALIGNMENT(a)                __attribute__((aligned(a)))

		#define X_ALIGN_BEGIN(a)              
		#define X_ALIGN_END(a)				  __attribute__((aligned(a)))              
		#define __GNU_C__

        // LWSYNC & THREAD_LOCAL
        #define X_LWSYNC_PS3
        #define X_LWSYNC
        #define X_THREAD_LOCAL                __declspec(thread)
        #define X_FORCE_INLINE                _forceinline

	#elif defined(COMPILER_SN_PSP)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		#define UNALIGNED_OK
		#define CACHE_LINE_SIZE           32
		#define X_CHAR_BIT                  8
		#define f_inline                  inline
		#define STD_CALL                  __stdcall
		#define C_DECL        
		#define f_noinline                __attribute__((noinline))
		#define X_BOOL                    signed long
		#define X_BYTE                    unsigned char
		#define X_WCHAR                   unsigned short
		#define X_INT8                    char
		#define X_INT16                   short
		#define X_INT32                   int
		#define X_INT64                   signed long long
		#define X_UINT64                  unsigned long long
		#define X_INT128                  __xint128
		#define X_UINT128                 __xuint128   
		#define X_INT256                  __xint256
		#define X_UINT256                 __xuint256
		#define X_SIZE                    unsigned int
		#define X_PTR_SIZED_INT           int
		#define X_LITTLE_ENDIAN           4321
		#define X_FLOAT                   float
		#define X_DOUBLE		          double
		#define X_ALIGNMENT_DEFAULT       4
		#define X_ALIGNMENT(a)            __attribute__((aligned(a)))

		#define X_ALIGN_BEGIN(a)
		#define X_ALIGN_END(a)            __attribute__((aligned(a)))               

		#pragma diag_suppress=817
		#pragma diag_suppress=68
		#pragma diag_suppress=382
		#pragma diag_suppress=613
		#pragma diag_suppress=178
		#pragma diag_suppress=187
		#pragma diag_suppress=47
		#pragma diag_suppress=1055
		#pragma diag_suppress=552
		#pragma diag_suppress=129
		#pragma diag_suppress=112
		#pragma diag_suppress=428

	#elif defined(COMPILER_GCC_PSP)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		#define UNALIGNED_OK
		#define CACHE_LINE_SIZE           32
		#define X_CHAR_BIT                  8
		#define IEEE_FLOATS
		#define f_inline                  inline
		#define STD_CALL                  __stdcall
		#define C_DECL        
		#define X_BOOL                    signed int
		#define X_BYTE                    unsigned char
		#define X_WCHAR                   unsigned short
		#define X_INT8                    char
		#define X_INT16                   short
		#define X_INT32                   long
		#define X_INT64                   signed long long
		#define X_UINT64                  unsigned long long
		#define X_INT128                  __xint128
		#define X_UINT128                 __xuint128   
		#define X_INT256                  __xint256
		#define X_UINT256                 __xuint256
		#define X_SIZE                    unsigned int
		#define X_PTR_SIZED_INT           int
		#define X_LITTLE_ENDIAN           4321
		#define X_FLOAT                   float
		#define X_ALIGNMENT_DEFAULT       4
		#define X_ALIGNMENT(a)            __attribute__((aligned(a)))

		#define X_ALIGN_BEGIN(a)          
		#define X_ALIGN_END(a)			  __attribute__((aligned(a)))

		#define __GNU_C__

	#elif defined(COMPILER_X360)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		//#define X_NO_CUSTOM_INT128
		//#define X_NO_CUSTOM_UINT128
        class __xint128;
        class __xuint128;
		class __xint256;
		class __xuint256;

		#define __NO_PARTIAL_TEMPLATE__
		#define UNALIGNED_OK
		#define CACHE_LINE_SIZE             32
		#define X_CHAR_BIT                    8
		#define IEEE_FLOATS
		#define USE_PRAGMA_ONCE
		#define STD_CALL                   __stdcall
		#define C_DECL                     __cdecl
		#define f_inline                   __forceinline
        #define f_noinline                 __declspec(noinline)
		#define X_BOOL                     signed long
		#define X_BYTE                     unsigned char
		#define X_WCHAR                    unsigned short
		#define X_INT8                     char
		#define X_INT16                    short
		#define X_INT32                    int
		#define X_INT64                    signed __int64
		#define X_UINT64                   unsigned __int64
		#define X_INT128                   __xint128
		#define X_UINT128                  __xuint128   
		#define X_INT256                   __xint256
		#define X_UINT256                  __xuint256
		#define X_SIZE                     unsigned int
		#define X_PTR_SIZED_INT            int
		#define X_BIG_ENDIAN               1234
		#define X_FLOAT                    float
        #define X_DOUBLE                   double
		#define X_ALIGNMENT_DEFAULT        4
		#define X_ALIGNMENT(a)             __declspec(align(a))


		#define X_ALIGN_BEGIN(a)		   __declspec(align(a))           
		#define X_ALIGN_END(a)                

		// LWSYNC & THREAD_LOCAL
		#define X_LWSYNC_PS3
		#define X_LWSYNC                  __lwsync();
		#define X_THREAD_LOCAL            __declspec(thread)
		#define X_FORCE_INLINE            __forceinline

	#else
		#error No Correct complier selected
	#endif

	#if(!defined(X_BIG_ENDIAN) && !defined(X_LITTLE_ENDIAN))
		#error Endian is not defined.
	#endif

	#if(defined(X_BIG_ENDIAN) &&  defined(X_LITTLE_ENDIAN))
		#error Both Endian specifications are defined!
	#endif

	enum x_memalign
	{
		X_MEMALIGN_PC               = 8,
		X_MEMALIGN_PSP              = 8,
		X_MEMALIGN_PS3              = 8,
		X_MEMALIGN_360              = 8,
		X_MEMALIGN_WII              = 8,
		X_MEMALIGN_3DS              = 8,
	#if defined(TARGET_PC)
		X_MEMALIGN			= X_MEMALIGN_PC,
	#elif defined(TARGET_PSP)
		X_MEMALIGN			= X_MEMALIGN_PSP,
	#elif defined(TARGET_PS3)
		X_MEMALIGN			= X_MEMALIGN_PS3,
	#elif defined(TARGET_WII)
		X_MEMALIGN			= X_MEMALIGN_WII,
	#elif defined(TARGET_360)
		X_MEMALIGN			= X_MEMALIGN_360,
	#elif defined(TARGET_3DS)
		X_MEMALIGN			= X_MEMALIGN_3DS,
	#else
		#error Need to have X_MEMALIGN!
	#endif
		X__PAD              = 0xffffffff
	};


	//==============================================================================
	//
	//  Memory manager configuration 
	//
	//==============================================================================
	#if defined(TARGET_PC) || defined(TARGET_PS3) || defined(TARGET_360)
		#define TARGET_MULTITHREADED_MEMORY_MANAGER
	#endif
	//==============================================================================
	//
	//  handle configuration specific options
	//
	//==============================================================================

	#if !defined(X_DEBUG)
		#undef X_ASSERT
	#else
		#ifndef X_ASSERT
			#define X_ASSERT
		#endif
	#endif

	//==============================================================================
	//
	// 64 bit constant support
	//
	//==============================================================================
	#ifndef X_CONSTANT_64
		// Assume compiler needs/accepts standard prefix for int64 types (long long)
		#define    X_CONSTANT_64(x)            x##LL
	#endif

	#ifndef X_CONSTANT_U64
		// Assume compiler needs/accepts standard prefix for uint64 types (unsigned long long)
		#define    X_CONSTANT_U64(x)            x##ULL
	#endif

    //==============================================================================
    //
    // unsigned 32 bit constant support
    //
    //==============================================================================
    #ifndef X_CONSTANT_S32
        #define X_CONSTANT_S32(x)		        x##L
    #endif
    #ifndef X_CONSTANT_U32
        #define X_CONSTANT_U32(x)		        x##UL
    #endif
	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END
//==============================================================================
#endif
