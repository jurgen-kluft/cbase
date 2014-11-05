 /**
 * Author:
 *     Virtuos Games
 * Summary:
 *     This file assists with cross platform development by
 *     providing a standard set of platform definitions with
 *     anticipated variations.
 * Description:
 *     This file assists with cross platform development by
 *     providing a standard set of platform definitions with
 *     anticipated variations. The selection of a platform and
 *     variation is based on a key macro definition.
 *     
 *     The target macro is provided in different ways depending on
 *     the development environment. But usually it is provided by
 *     the user when he sets the build configuration in the
 *     development environment.
 *     
 *     <table>
 *     Target Macro                \Description                           MFC Safe?
 *     --------------------------  -------------------------------------  ----------
 *     none                        Assumes TARGET_PC_EDITOR                    Y
 *     TARGET_PC_EDITOR_DEBUG      Editors                                     Y
 *     TARGET_PC_EDITOR_RELEASE    Editors                                     Y
 *     TARGET_PC_TOOL_DEBUG        Tools, command line, etc                    Y
 *     TARGET_PC_TOOL_RELEASE      Tools, command line, etc                    Y
 *     TARGET_PC_DEV_DEBUG         Windows PC DevKit Debug                     N
 *     TARGET_PC_DEV_RELEASE       Windows PC DevKit Release                   N
 *     TARGET_PC_DEV_FINAL         Windows PC DevKit Final                     N
 *     TARGET_PC_CLIENT_DEBUG      Windows PC "Debug Station" Debug            N
 *     TARGET_PC_CLIENT_RELEASE    Windows PC "Debug Station" Release          N
 *     TARGET_PC_CLIENT_FINAL      Windows PC "Debug Station" Final            N
 *     TARGET_PC_RETAIL_DEBUG      Windows PC Console Debug                    N
 *     TARGET_PC_RETAIL_RELEASE    Windows PC Console Release                  N
 *     TARGET_PC_RETAIL_FINAL      Windows PC Console Final                    N
 *     TARGET_PC_PROFILE_DEBUG     Windows PC Profile Debug                    N
 *     TARGET_PC_PROFILE_RELEASE   Windows PC Profile Release                  N
 *     TARGET_PC_TEST_DEBUG        Windows PC Test Debug                       N
 *     TARGET_PC_TEST_RELEASE      Windows PC Test Release                     N
 *     TARGET_PS3_DEV_DEBUG        Sony PlayStation 3 DevKit Debug               N
 *     TARGET_PS3_DEV_RELEASE      Sony PlayStation 3 DevKit Release             N
 *     TARGET_PS3_DEV_FINAL        Sony PlayStation 3 DevKit Final               N
 *     TARGET_PS3_CLIENT_DEBUG     Sony PlayStation 3 "Debug Station" Debug      N
 *     TARGET_PS3_CLIENT_RELEASE   Sony PlayStation 3 "Debug Station" Release    N
 *     TARGET_PS3_CLIENT_FINAL     Sony PlayStation 3 "Debug Station" Final      N
 *     TARGET_PS3_RETAIL_DEBUG     Sony PlayStation 3 console Debug              N
 *     TARGET_PS3_RETAIL_RELEASE   Sony PlayStation 3 console Release            N
 *     TARGET_PS3_RETAIL_FINAL     Sony PlayStation 3 console Final              N
 *     TARGET_PS3_PROFILE_DEBUG    Sony PlayStation 3 Profile Debug              N
 *     TARGET_PS3_PROFILE_RELEASE  Sony PlayStation 3 Profile Release            N
 *     TARGET_PS3_TEST_DEBUG       Sony PlayStation 3 Test Debug                 N
 *     TARGET_PS3_TEST_RELEASE     Sony PlayStation 3 Test Release               N
 *     TARGET_PSP_DEV_DEBUG        Sony PlayStation Portable DevKit Debug        N
 *     TARGET_PSP_DEV_RELEASE      Sony PlayStation Portable DevKit Release      N
 *     TARGET_PSP_DEV_FINAL        Sony PlayStation Portable DevKit Final        N
 *     TARGET_PSP_CLIENT_DEBUG     Sony PlayStation Portable "Debug Debug        N
 *     TARGET_PSP_CLIENT_RELEASE   Sony PlayStation Portable "Debug Release      N
 *     TARGET_PSP_CLIENT_FINAL     Sony PlayStation Portable "Debug Final        N
 *     TARGET_PSP_RETAIL_DEBUG     Sony PlayStation Portable Console Debug       N
 *     TARGET_PSP_RETAIL_RELEASE   Sony PlayStation Portable Console Release     N
 *     TARGET_PSP_RETAIL_FINAL     Sony PlayStation Portable Console Final       N
 *     TARGET_PSP_PROFILE_DEBUG    Sony PlayStation Portable Profile Debug       N
 *     TARGET_PSP_PROFILE_RELEASE  Sony PlayStation Portable Profile Release     N
 *     TARGET_PSP_TEST_DEBUG       Sony PlayStation Portable Test Debug          N
 *     TARGET_PSP_TEST_RELEASE     Sony PlayStation Portable Test Release        N
 *     TARGET_360_DEV_DEBUG        Microsoft X-Box 360 DevKit Debug              N
 *     TARGET_360_DEV_RELEASE      Microsoft X-Box 360 DevKit Release            N
 *     TARGET_360_DEV_FINAL        Microsoft X-Box 360 DevKit Final              N
 *     TARGET_360_CLIENT_DEBUG     Microsoft X-Box 360 "Debug Station" Debug     N
 *     TARGET_360_CLIENT_RELEASE   Microsoft X-Box 360 "Debug Station" Release   N
 *     TARGET_360_CLIENT_FINAL     Microsoft X-Box 360 "Debug Station" Final     N
 *     TARGET_360_RETAIL_DEBUG     Microsoft X-Box 360 Console Debug             N
 *     TARGET_360_RETAIL_RELEASE   Microsoft X-Box 360 Console Release           N
 *     TARGET_360_RETAIL_FINAL     Microsoft X-Box 360 Console Final             N
 *     TARGET_360_PROFILE_DEBUG    Microsoft X-Box 360 Profile Debug             N
 *     TARGET_360_PROFILE_RELEASE  Microsoft X-Box 360 Profile Release           N
 *     TARGET_360_TEST_DEBUG       Microsoft X-Box 360 Test Debug                N
 *     TARGET_360_TEST_RELEASE     Microsoft X-Box 360 Test Release              N
 *     TARGET_WII_DEV_DEBUG        Nintendo Wii DevKit Debug                     N
 *     TARGET_WII_DEV_RELEASE      Nintendo Wii DevKit Release                   N
 *     TARGET_WII_DEV_FINAL        Nintendo Wii DevKit Final                     N
 *     TARGET_WII_CLIENT_DEBUG     Nintendo Wii "Debug Station" Debug            N
 *     TARGET_WII_CLIENT_RELEASE   Nintendo Wii "Debug Station" Release          N
 *     TARGET_WII_CLIENT_FINAL     Nintendo Wii "Debug Station" Final            N
 *     TARGET_WII_RETAIL_DEBUG     Nintendo Wii Console Debug                    N
 *     TARGET_WII_RETAIL_RELEASE   Nintendo Wii Console Release                  N
 *     TARGET_WII_RETAIL_FINAL     Nintendo Wii Console Final                    N
 *     TARGET_WII_PROFILE_DEBUG    Nintendo Wii Profile Debug                    N
 *     TARGET_WII_PROFILE_RELEASE  Nintendo Wii Profile Release                  N
 *     TARGET_WII_TEST_DEBUG       Nintendo Wii Test Debug                       N
 *     TARGET_WII_TEST_RELEASE     Nintendo Wii Test Release                     N
 *     TARGET_3DS_DEV_DEBUG        Nintendo 3ds DevKit Debug                     N
 *     TARGET_3DS_DEV_RELEASE      Nintendo 3ds DevKit Release                   N
 *     TARGET_3DS_DEV_FINAL        Nintendo 3ds DevKit Final                     N
 *     TARGET_3DS_CLIENT_DEBUG     Nintendo 3ds "Debug Station" Debug            N
 *     TARGET_3DS_CLIENT_RELEASE   Nintendo 3ds "Debug Station" Release          N
 *     TARGET_3DS_CLIENT_FINAL     Nintendo 3ds "Debug Station" Final            N
 *     TARGET_3DS_RETAIL_DEBUG     Nintendo 3ds Console Debug                    N
 *     TARGET_3DS_RETAIL_RELEASE   Nintendo 3ds Console Release                  N
 *     TARGET_3DS_RETAIL_FINAL     Nintendo 3ds Console Final                    N
 *     TARGET_3DS_PROFILE_DEBUG    Nintendo 3ds Profile Debug                    N
 *     TARGET_3DS_PROFILE_RELEASE  Nintendo 3ds Profile Release                  N
 *     TARGET_3DS_TEST_DEBUG       Nintendo 3ds Test Debug                       N
 *     TARGET_3DS_TEST_RELEASE     Nintendo 3ds Test Release                     N
 *     </table>
 *     
 *     Targets which are "MFC safe" (and have _MFC in the macro)
 *     will disable the x_files version of operators new and delete.
 *     Graphic engines prepared for MFC configurations should not
 *     provide a main() function.
 *     
 *     When present, each of the primary target macros in the list
 *     above will, in turn, cause other secondary macros to be
 *     defined. These Secondary macros are set automatically for the
 *     user.
 *     
 *     
 *     
 *     <table>
 *     Secondary Macro   \Description
 *     ----------------  --------------------------------------
 *     TARGET_PC         Platform PC
 *     TARGET_PC_EDITOR  Platform PC Editor
 *     TARGET_MFC        MFC
 *     TARGET_PS3        Platform Sony PlayStation 3
 *     TARGET_PSP        Platform Sony PlayStation Portable
 *     TARGET_360        Platform Microsoft X-Box 360
 *     TARGET_WII        Platform Nintendo Wii
 *     TARGET_3DS        Platform Nintendo 3DS
 *     TARGET_DEVKIT     \on DevKit
 *     TARGET_CLIENT     \on "Debug Station"
 *     TARGET_RETAIL     \on Console
 *     TARGET_DEBUG      Debug
 *     TARGET_RELEASE    Release, including some Debug stuff
 *     TARGET_FINAL      Final, optimized codes, excluding all
 *                        the Debug stuff
 *     TARGET_TEST       Test (UnitTest, exceptions are enabled)
 *     </table>
 *     
 *     <table>
 *     Target Macro                Secondary Macros
 *     --------------------------  ---------------------------
 *     none                        TARGET_PC TARGET_DEBUG   TARGET_EDITOR TARGET_MFC
 *     TARGET_PC_EDITOR_DEBUG      TARGET_PC TARGET_DEBUG   TARGET_EDITOR TARGET_MFC
 *     TARGET_PC_EDITOR_RELEASE    TARGET_PC TARGET_RELEASE TARGET_EDITOR TARGET_MFC
 *     TARGET_PC_DEBUG             == TARGET_PC_DEV_DEBUG
 *     TARGET_PC_RELEASE           == TARGET_PC_DEV_RELEASE
 *     TARGET_PC_FINAL             == TARGET_PC_DEV_FINAL
 *     TARGET_PC_DEV_DEBUG         TARGET_PC TARGET_DEVKIT TARGET_DEBUG
 *     TARGET_PC_DEV_RELEASE       TARGET_PC TARGET_DEVKIT TARGET_RELASE
 *     TARGET_PC_DEV_FINAL         TARGET_PC TARGET_DEVKIT TARGET_FINAL
 *     TARGET_PC_CLIENT_DEBUG      TARGET_PC TARGET_CLIENT TARGET_DEBUG
 *     TARGET_PC_CLIENT_RELEASE    TARGET_PC TARGET_CLIENT TARGET_RELASE
 *     TARGET_PC_CLIENT_FINAL      TARGET_PC TARGET_CLIENT TARGET_FINAL
 *     TARGET_PC_RETAIL_DEBUG      TARGET_PC TARGET_RETAIL TARGET_DEBUG
 *     TARGET_PC_RETAIL_RELEASE    TARGET_PC TARGET_RETAIL TARGET_RELASE
 *     TARGET_PC_RETAIL_FINAL      TARGET_PC TARGET_RETAIL TARGET_FINAL
 *     TARGET_PC_TEST_DEBUG        TARGET_PC TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
 *     TARGET_PS3_TEST_RELEASE     TARGET_PC TARGET_DEVKIT TARGET_RELASE TARGET_TEST
 *     TARGET_PS3_DEV_DEBUG        TARGET_PS3 TARGET_DEVKIT TARGET_DEBUG
 *     TARGET_PS3_DEV_RELEASE      TARGET_PS3 TARGET_DEVKIT TARGET_RELASE
 *     TARGET_PS3_DEV_FINAL        TARGET_PS3 TARGET_DEVKIT TARGET_FINAL
 *     TARGET_PS3_CLIENT_DEBUG     TARGET_PS3 TARGET_CLIENT TARGET_DEBUG
 *     TARGET_PS3_CLIENT_RELEASE   TARGET_PS3 TARGET_CLIENT TARGET_RELASE
 *     TARGET_PS3_CLIENT_FINAL     TARGET_PS3 TARGET_CLIENT TARGET_FINAL
 *     TARGET_PS3_RETAIL_DEBUG     TARGET_PS3 TARGET_RETAIL TARGET_DEBUG
 *     TARGET_PS3_RETAIL_RELEASE   TARGET_PS3 TARGET_RETAIL TARGET_RELASE
 *     TARGET_PS3_RETAIL_FINAL     TARGET_PS3 TARGET_RETAIL TARGET_FINAL
 *     TARGET_PS3_TEST_DEBUG       TARGET_PS3 TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
 *     TARGET_PS3_TEST_RELEASE     TARGET_PS3 TARGET_DEVKIT TARGET_RELASE TARGET_TEST
 *     TARGET_PSP_DEV_DEBUG        TARGET_PSP TARGET_DEVKIT TARGET_DEBUG
 *     TARGET_PSP_DEV_RELEASE      TARGET_PSP TARGET_DEVKIT TARGET_RELASE
 *     TARGET_PSP_DEV_FINAL        TARGET_PSP TARGET_DEVKIT TARGET_FINAL
 *     TARGET_PSP_CLIENT_DEBUG     TARGET_PSP TARGET_CLIENT TARGET_DEBUG
 *     TARGET_PSP_CLIENT_RELEASE   TARGET_PSP TARGET_CLIENT TARGET_RELASE
 *     TARGET_PSP_CLIENT_FINAL     TARGET_PSP TARGET_CLIENT TARGET_FINAL
 *     TARGET_PSP_RETAIL_DEBUG     TARGET_PSP TARGET_RETAIL TARGET_DEBUG
 *     TARGET_PSP_RETAIL_RELEASE   TARGET_PSP TARGET_RETAIL TARGET_RELASE
 *     TARGET_PSP_RETAIL_FINAL     TARGET_PSP TARGET_RETAIL TARGET_FINAL
 *     TARGET_PSP_TEST_DEBUG       TARGET_PSP TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
 *     TARGET_PSP_TEST_RELEASE     TARGET_PSP TARGET_DEVKIT TARGET_RELASE TARGET_TEST
 *     TARGET_360_DEV_DEBUG        TARGET_360 TARGET_DEVKIT TARGET_DEBUG
 *     TARGET_360_DEV_RELEASE      TARGET_360 TARGET_DEVKIT TARGET_RELASE
 *     TARGET_360_DEV_FINAL        TARGET_360 TARGET_DEVKIT TARGET_FINAL
 *     TARGET_360_CLIENT_DEBUG     TARGET_360 TARGET_CLIENT TARGET_DEBUG
 *     TARGET_360_CLIENT_RELEASE   TARGET_360 TARGET_CLIENT TARGET_RELEASE
 *     TARGET_360_CLIENT_FINAL     TARGET_360 TARGET_CLIENT TARGET_FINAL
 *     TARGET_360_RETAIL_DEBUG     TARGET_360 TARGET_RETAIL TARGET_DEBUG
 *     TARGET_360_RETAIL_RELEASE   TARGET_360 TARGET_RETAIL TARGET_RELASE
 *     TARGET_360_RETAIL_FINAL     TARGET_360 TARGET_RETAIL TARGET_FINAL
 *     TARGET_360_TEST_DEBUG       TARGET_360 TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
 *     TARGET_360_TEST_RELEASE     TARGET_360 TARGET_DEVKIT TARGET_RELASE TARGET_TEST
 *     TARGET_WII_DEV_DEBUG        TARGET_WII TARGET_DEVKIT TARGET_DEBUG
 *     TARGET_WII_DEV_RELEASE      TARGET_WII TARGET_DEVKIT TARGET_RELASE
 *     TARGET_WII_DEV_FINAL        TARGET_WII TARGET_DEVKIT TARGET_FINAL
 *     TARGET_WII_CLIENT_DEBUG     TARGET_WII TARGET_CLIENT TARGET_DEBUG
 *     TARGET_WII_CLIENT_RELEASE   TARGET_WII TARGET_CLIENT TARGET_RELASE
 *     TARGET_WII_CLIENT_FINAL     TARGET_WII TARGET_CLIENT TARGET_FINAL
 *     TARGET_WII_RETAIL_DEBUG     TARGET_WII TARGET_RETAIL TARGET_DEBUG
 *     TARGET_WII_RETAIL_RELEASE   TARGET_WII TARGET_RETAIL TARGET_RELASE
 *     TARGET_WII_RETAIL_FINAL     TARGET_WII TARGET_RETAIL TARGET_FINAL
 *     TARGET_WII_TEST_DEBUG       TARGET_WII TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
 *     TARGET_WII_TEST_RELEASE     TARGET_WII TARGET_DEVKIT TARGET_RELASE TARGET_TEST
 *     TARGET_3DS_DEV_DEBUG        TARGET_3DS TARGET_DEVKIT TARGET_DEBUG
 *     TARGET_3DS_DEV_RELEASE      TARGET_3DS TARGET_DEVKIT TARGET_RELASE
 *     TARGET_3DS_DEV_FINAL        TARGET_3DS TARGET_DEVKIT TARGET_FINAL
 *     TARGET_3DS_CLIENT_DEBUG     TARGET_3DS TARGET_CLIENT TARGET_DEBUG
 *     TARGET_3DS_CLIENT_RELEASE   TARGET_3DS TARGET_CLIENT TARGET_RELASE
 *     TARGET_3DS_CLIENT_FINAL     TARGET_3DS TARGET_CLIENT TARGET_FINAL
 *     TARGET_3DS_RETAIL_DEBUG     TARGET_3DS TARGET_RETAIL TARGET_DEBUG
 *     TARGET_3DS_RETAIL_RELEASE   TARGET_3DS TARGET_RETAIL TARGET_RELASE
 *     TARGET_3DS_RETAIL_FINAL     TARGET_3DS TARGET_RETAIL TARGET_FINAL
 *     TARGET_3DS_TEST_DEBUG       TARGET_3DS TARGET_DEVKIT TARGET_DEBUG  TARGET_TEST
 *     TARGET_3DS_TEST_RELEASE     TARGET_3DS TARGET_DEVKIT TARGET_RELASE TARGET_TEST
 *     </table>
 *
 *
 *     A platform defines the high-level hardware but there is still one variable
 *     that we must specify and that is 32-bit or 64-bit. This has mainly an impact
 *     on sizeof(void*), for 32-bit this results in 4, meaning a pointer will occupy
 *     4 bytes on a 32-bit system. On a 64-bit system this will be 8 bytes.
 *     
 *     <table>
 *     Dat Size Macro     \Description              \Version   Platform
 *     -----------------  ------------------------  ---------  ---------
 *     X_32BIT            32-bit hardware systems   <2005		PC,PS2,PSP,PS3,XBOX,XBOX360,NDS,N3DS,WII
 *     X_64BIT            64-bit hardware systems   >2008       PC,XBOXONE,PS4
 *     </table>
 *
 *     Finally the user needs to define in the configuration which
 *     kind of build is he trying to do. X_DEBUG could be use even
 *     if the compiler is set in optimization.
 *     
 *     <table>
 *     Target Macro   \Description
 *     -------------  -------------------------------------------
 *     none           Ignores debugging features such asserts.
 *     X_DEBUG        Activates debugging features such asserts.
 *     </table>
 *     
 *     The target macro is provided in different ways depending on
 *     the compiler.
 *     
 *     <table>
 *     Compiler Macro     \Description              \Version   Platform
 *     -----------------  ------------------------  ---------  ---------
 *     COMPILER_MSVC      Microsoft Visual Studio   2005       PC
 *     COMPILER_MSVC      Microsoft Visual Studio   2008       PC
 *     COMPILER_MSVC      Microsoft Visual Studio   2010       PC
 *     COMPILER_MW_WII    Metrowerks                WII       WII
 *     COMPILER_GCC_PSP   GCC                        PSP       PSP
 *     COMPILER_SN_PSP    SNSystems                  PSP       PSP
 *     COMPILER_SN_PS3    SNSystems                  PS3       PS3
 *     </table>
 *     
 *     Other Macros provided to the user automatically are:
 *     
 *     <table>
 *     Target Macro      \Description
 *     ----------------  ---------------------------
 *     X_ALIGNMENT       alignment definition.
 *     X_BIG_ENDIAN      Endianness of the hardware
 *     X_LITTLE_ENDIAN   Endianness of the hardware
 *     </table>
 *     
 *     Finally there is a convenient platform enumeration that can
 *     be use with logical operations. ex: (PLATFORM_PC | PLATFORM_360)                                                                
 * ---------------------------------------------------------------------------------
 */

#ifndef __XBASE_TARGET_H__
#define __XBASE_TARGET_H__

/**
 * xCore namespace
 */
namespace xcore
{
	/**
	 *  
	 * Hardware enumeration
	 *
	 */
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

	/**
	 *  
	 *  Check for ambiguous or insufficiently qualified target specification.
	 *  
	 *  *** IF YOU GOT AN ERROR HERE ***, then you specified a target platform 
	 *  without sufficiently qualifying the target.
	 *
	 */
	#if !defined(TARGET_PS3_DEV_DEBUG) && !defined(TARGET_PS3_DEV_RELEASE) && !defined(TARGET_PS3_DEV_FINAL) && !defined(TARGET_PS3_CLIENT_DEBUG) && !defined(TARGET_PS3_CLIENT_RELEASE) && !defined(TARGET_PS3_CLIENT_FINAL) && !defined(TARGET_PS3_RETAIL_DEBUG) && !defined(TARGET_PS3_RETAIL_RELEASE) && !defined(TARGET_PS3_RETAIL_FINAL)      
		#ifdef TARGET_PS3
			#error x_target, error; Incorrect target specification.
			#error x_target, error; Use either TARGET_PS3_DEV, TARGET_PS3_CLIENT, or TARGET_PS3_RETAIL.
		#endif
	#else
		#undef TARGET_PS3
	#endif

	#if !defined(TARGET_360_DEV_DEBUG) && !defined(TARGET_360_DEV_RELEASE) && !defined(TARGET_360_DEV_FINAL) && !defined(TARGET_360_CLIENT_DEBUG) && !defined(TARGET_360_CLIENT_RELEASE) && !defined(TARGET_360_CLIENT_FINAL) && !defined(TARGET_360_RETAIL_DEBUG) && !defined(TARGET_360_RETAIL_RELEASE) && !defined(TARGET_360_RETAIL_FINAL)      
		#ifdef TARGET_360
			#error x_target, error; TARGET_360 is not a sufficient target specification.
			#error x_target, error; Use either TARGET_360_DEV, TARGET_360_CLIENT, or TARGET_360_RETAIL.
		#endif
	#else
		#undef TARGET_360
	#endif

	#if !defined(TARGET_WII_DEV_DEBUG) && !defined(TARGET_WII_DEV_RELEASE) && !defined(TARGET_WII_DEV_FINAL) && !defined(TARGET_WII_CLIENT_DEBUG) && !defined(TARGET_WII_CLIENT_RELEASE) && !defined(TARGET_WII_CLIENT_FINAL) && !defined(TARGET_WII_RETAIL_DEBUG) && !defined(TARGET_WII_RETAIL_RELEASE) && !defined(TARGET_WII_RETAIL_FINAL)      
		#ifdef TARGET_WII
			#error x_target, error; TARGET_WII is not a sufficient target specification.
			#error x_target, error; Use either PLATFORM_WII_DEV_DEBUG, PLATFORM_WII_DEV_RELEASE or PLATFORM_WII_DEV_FINAL
		#endif
	#else
		#undef TARGET_WII
	#endif

	#if !defined(TARGET_3DS_DEV_DEBUG) && !defined(TARGET_3DS_DEV_RELEASE) && !defined(TARGET_3DS_DEV_FINAL) && !defined(TARGET_3DS_CLIENT_DEBUG) && !defined(TARGET_3DS_CLIENT_RELEASE) && !defined(TARGET_3DS_CLIENT_FINAL) && !defined(TARGET_3DS_RETAIL_DEBUG) && !defined(TARGET_3DS_RETAIL_RELEASE) && !defined(TARGET_3DS_RETAIL_FINAL)      
		#ifdef TARGET_3DS
			#error x_target, error; TARGET_3DS is not a sufficient target specification.
			#error x_target, error; Use either PLATFORM_3DS_DEV_DEBUG, PLATFORM_3DS_DEV_RELEASE or PLATFORM_3DS_DEV_FINAL
		#endif
	#else
		#undef TARGET_3DS
	#endif
	
	#if !defined(TARGET_PSP_DEV_DEBUG) && !defined(TARGET_PSP_DEV_RELEASE) && !defined(TARGET_PSP_DEV_FINAL) && !defined(TARGET_PSP_CLIENT_DEBUG) && !defined(TARGET_PSP_CLIENT_RELEASE) && !defined(TARGET_PSP_CLIENT_FINAL) && !defined(TARGET_PSP_RETAIL_DEBUG) && !defined(TARGET_PSP_RETAIL_RELEASE) && !defined(TARGET_PSP_RETAIL_FINAL)      
		#ifdef TARGET_PSP
			#error x_target, error; TARGET_PSP is not a sufficient target specification.
			#error x_target, error; Use either TARGET_PSP_DEV, TARGET_PSP_CLIENT, or TARGET_PSP_RETAIL.
		#endif
	#else
		#undef TARGET_PSP
	#endif

	/**
	 *  Playstation 3 Targets
	 */

	#ifdef TARGET_PS3_TEST_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PS3
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
			#define TARGET_RETAIL
			#define TARGET_FINAL
			#define TARGET_PLATFORM X_PLATFORM_PS3
			#define VALID_TARGET
			#define X_DEBUG
		#endif
	#endif


	//------------------------------------------------------------------------------

	/**
	 *  Playstation Portable Targets
	 */

	#ifdef TARGET_PSP_TEST_DEBUG
		#ifdef VALID_TARGET
			#define MULTIPLE_TARGETS
		#else
			#define TARGET_PSP
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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
			#define TARGET_32BIT
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

	/**
	 *  Nintendo Wii Targets
	 */

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


	/**
	 *  Nintendo 3DS Targets
	 */

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

	/**
	 *  Microsoft X-Box 360 Targets
	 */

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

	/**
	 *  PC Targets
	 */

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

	/**
	 *
	 *  Make sure we found a proper target specification.  If you get a compilation 
	 *  error here, then your compilation environment is not specifying one of the
	 *  target macros.
	 *
	 */

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
			#error x_target, error; Target specification invalid or not found.
			#error x_target, error; The compilation environment must define one of the macros listed in x_targets.h
		#endif
	#endif

	/**
	 *
	 *  Make sure we have defined the architecture type, PLATFORM_32BIT or PLATFORM_64BIT
	 *
	 */
	#ifdef VALID_TARGET
		#if !defined(PLATFORM_32BIT) && !defined(PLATFORM_64BIT)
			// define it by checking sizeof(void*)
			#if !defined(_M_X64)
				#define TARGET_32BIT
			#elif defined(_M_X64)
				#define TARGET_64BIT
			#else
				#error x_target, error; Unknown target architecture type, only 32-bit or 64-bit are supported
			#endif
		#endif
	#endif

	/**
	 *
	 *  generate string containing the target description
	 *
	 *  TARGET_PC + TARGET_DEBUG + 
	 *
	 *
	 *
	 */
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
		#define TARGET_HW_STR			"_DEV"
	#endif
	#if defined(TARGET_RETAIL)
		#define TARGET_HW_STR			"_RETAIL"
	#endif
	#if defined(TARGET_CLIENT)
		#define TARGET_HW_STR			"_CLIENT"
	#endif

	#if defined(TARGET_32BIT)
		#define TARGET_BIT_STR			"_32"
	#elif defined(TARGET_64BIT)
		#define TARGET_BIT_STR			"_64"
	#else
		#define TARGET_BIT_STR			"_32"
	#endif

	#define	TARGET_FULL_DESCR_STR		TARGET_PLATFORM_STR TARGET_HW_STR TARGET_EDITOR_STR TARGET_CONFIG_FULL_STR TARGET_BIT_STR



	/**
	 *
	 *  Make sure we did not somehow get multiple target platform specifications.
	 *  *** IF YOU GOT AN ERROR HERE ***, then you have defined more than one of
	 *  the target specification macros.
	 *
	 */

	#ifdef MULTIPLE_TARGETS
	#pragma message(TARGET_FULL_DESCR_STR)
	#error x_target, error; Multiple target specification definition macros were detected.
	#error x_target, error; The compilation environment must define only one of the macros listed in x_targets.h
	#endif

	/**
	 *
	 *  Compiler Designation
	 *
	 */

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
			/**
			 * 1200      == VC++ 6.0
			 * 1200-1202 == VC++ 4
			 * 1300      == VC++ 7.0
			 * 1400      == VC++ 8.0
			 * 1500      == VC++ 9.0
			 * 1600      == VC++ 10.0 (Visual Studio 2010)
			 * 1700      == VC++ 11.0 (Visual Studio 2012)
			 * 1800      == VC++ 12.0 (Visual Studio 2013)
			 */
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
				#define COMPILER_VERSION            2010
			#elif _MSC_VER == 1700
				#define COMPILER_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2012
			#elif _MSC_VER == 1800
				#define COMPILER_MSVC
				#define COMPILER_DEFAULT
				#define COMPILER_VERSION            2013
			#else
				#error x_target, error; Unknown _MSVC_VER compiler ID
			#endif
		#else
			#error x_target, error; This compiler is not supported for TARGET_PC
		#endif
	#elif defined(TARGET_WII)
		#ifdef __CWCC__
			#define COMPILER_MW_WII
			#define COMPILER_DEFAULT
			#define COMPILER_VERSION            1
		#else
			#error x_target, error; This compiler is not supported for TARGET_WII
		#endif
	#elif defined(TARGET_3DS)
		#ifdef __arm__
			#define COMPILER_3DS
			#define COMPILER_DEFAULT
			#define COMPILER_VERSION            __ARMCC_VERSION
		#else
			#error x_target, error; This compiler is not supported for TARGET_3DS
		#endif
	#elif defined(TARGET_PSP)
		#ifdef __SNC__
			#define COMPILER_SN_PSP
			#define COMPILER_DEFAULT
			#define COMPILER_VERSION            1
		#elif defined(__GCC__)
			#error x_target, error; GCC for psp not defined
		#else
			#error x_target, error; This compiler is not supported for TARGET_PSP
		#endif
	#elif defined(TARGET_360)
		#if (_MSC_VER == 1400) || (_MSC_VER == 1500) || (_MSC_VER == 1600)
			#define COMPILER_X360
			#define COMPILER_DEFAULT
			#define COMPILER_VERSION            360
		#else
			#error x_target, error; The version of Microsoft Visual Studio compiler is not supported for TARGET_360
		#endif
	#elif defined(TARGET_PS3)
		#if defined(__SNC__) || defined(SPU) // It seems in xbase the GCC compiler is still being used for SPUs, but it has the attribute notation only
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
		#error x_target, error; This compiler is not supported for TARGET_UNKNOWN
	#endif

	#undef X_CHAR_BIT

	/// This one is for PC
	#if defined(COMPILER_MSVC)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

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

	#elif defined(COMPILER_MW_WII)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		#define X_CACHE_LINE_SIZE           32
		#define X_CHAR_BIT                  8
		#define X_USE_PRAGMA_ONCE
		#define f_inline                  inline
		#define f_noinline                __declspec(noinline)
		#define X_FORCE_INLINE			  inline
		#define X_STD_CALL                __stdcall
		#define X_C_DECL        
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

		#define X_OFFSET_OF(type, member)     (X_SIZE)&((((type *)0)->member))

		#define __GNU_C__

	#elif defined(COMPILER_3DS)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		#define X_CACHE_LINE_SIZE         32
		#define X_CHAR_BIT                8
		#define X_USE_PRAGMA_ONCE
		#define f_inline                  __forceinline
		#define f_noinline                __declspec(noinline)
		#define X_FORCE_INLINE			  inline
		#define X_STD_CALL                __stdcall
		#define X_C_DECL        
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
		#define X_LITTLE_ENDIAN           4321
		#define X_FLOAT                   float
		#define X_DOUBLE                  double
		#define X_ALIGNMENT_DEFAULT           4
		#define X_ALIGNMENT(a)            __attribute__((aligned(a)))

		#define X_ALIGN_BEGIN(a)          
		#define X_ALIGN_END(a)            __attribute__((aligned(a)))

		#define X_OFFSET_OF(type, member)     (X_SIZE)&reinterpret_cast<const volatile char&>((((type *)0)->member))

		#pragma anon_unions

	#elif defined(COMPILER_GCC_PS3)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		//#define X_NO_PARTIAL_TEMPLATE
		#define X_CACHE_LINE_SIZE               32
		#define X_CHAR_BIT                      8
		#define X_USE_PRAGMA_ONCE
		#define X_STD_CALL                    __stdcall
		#define X_C_DECL                      __cdecl
#ifdef SPU
		#define f_inline                      inline
#else
		#define f_inline                      __forceinline
#endif
		#define f_noinline
		#define X_FORCE_INLINE                inline
		#define X_FORCE_INLINE_SPU			  __forceinline
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

		#define X_OFFSET_OF(type, member)     (X_SIZE)&reinterpret_cast<const volatile char&>((((type *)0)->member))

		#define X_THREAD_LOCAL                __declspec(thread)

	#elif defined(COMPILER_SN_PS3)
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
#ifdef SPU									// because we dont REALLY want to force stuff to be inlined on SPU.. and the PS3 compiler excels at ignoring "inline"
		#define f_inline					  inline
		#define f_noinline                    __attribute__((noinline))
		#define X_FORCE_INLINE                inline
		#define X_FORCE_INLINE_SPU			  __attribute__((always_inline))		// this is just for those special cases where we really do want to inline on SPU..
#else
		#define f_inline					  __attribute__((always_inline))
		#define f_noinline                    __attribute__((noinline))
		#define X_FORCE_INLINE                __attribute__((always_inline))
		#define X_FORCE_INLINE_SPU			  __attribute__((always_inline))
#endif

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

		#define X_OFFSET_OF(type, member)     (X_SIZE)&reinterpret_cast<const volatile char&>((((type *)0)->member))
		
		#define X_THREAD_LOCAL                __declspec(thread)

	#elif defined(COMPILER_SN_PSP)
		#define X_NO_CUSTOM_INT64
		#define X_NO_CUSTOM_UINT64
		class __xint128;
		class __xuint128;
		class __xint256;
		class __xuint256;

		#define X_CACHE_LINE_SIZE           32
		#define X_CHAR_BIT                  8
		#define X_USE_PRAGMA_ONCE
		#define X_STD_CALL                  __stdcall
		#define X_C_DECL        
		#define f_inline                  inline
		#define f_noinline                __attribute__((noinline))
		#define X_FORCE_INLINE            __attribute__((always_inline))
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

		#define X_OFFSET_OF(type, member)     (X_SIZE)&reinterpret_cast<const volatile char&>((((type *)0)->member))

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

		#define X_CACHE_LINE_SIZE           32
		#define X_CHAR_BIT                  8
		#define X_USE_PRAGMA_ONCE
		#define X_IEEE_FLOATS
		#define X_STD_CALL                __stdcall
		#define X_C_DECL        
		#define f_inline                  inline
		#define f_noinline                
		#define X_FORCE_INLINE            inline
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

		#define X_OFFSET_OF(type, member)     (X_SIZE)&reinterpret_cast<const volatile char&>((((type *)0)->member))

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

		//#define X_NO_PARTIAL_TEMPLATE
		#define X_CACHE_LINE_SIZE          32
		#define X_CHAR_BIT                 8
		#define X_IEEE_FLOATS
		#define USE_PRAGMA_ONCE
		#define X_STD_CALL                 __stdcall
		#define X_C_DECL                   __cdecl
		#define f_inline                   __forceinline
		#define f_noinline                 __declspec(noinline)
		#define X_FORCE_INLINE            __forceinline
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

		#define X_ALIGN_BEGIN(a)			   __declspec(align(a))           
		#define X_ALIGN_END(a)                

		#define X_OFFSET_OF(type, member)  (X_SIZE)&reinterpret_cast<const volatile char&>((((type *)0)->member))

		#define X_THREAD_LOCAL             __declspec(thread)

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
		#error x_target, error; need to have X_MEMALIGN defined
	#endif
		X__PAD              = 0xffffffff
	};


	/**
		* Multi-threading configuration
	*/

	#if defined(TARGET_PC) || defined(TARGET_PS3) || defined(TARGET_360)
		#define TARGET_MULTI_CORE
	#else	
		#define TARGET_SINGLE_CORE
	#endif


	/**
	 *
	 *  Memory manager configuration 
	 *
	 */
	#if defined(TARGET_PC) || defined(TARGET_PS3) || defined(TARGET_360)
		#define TARGET_MULTITHREADED_MEMORY_MANAGER
	#endif
	/**
	 *
	 *  handle configuration specific options
	 *
	 */

	#if !defined(X_DEBUG)
		#undef X_ASSERT
	#else
		#ifndef X_ASSERT
			#define X_ASSERT
		#endif
	#endif

	#if !defined(SPU)
		#define X_FORCE_INLINE_SPU X_FORCE_INLINE
	#endif

	/**
	 *
	 * 64 bit constant support
	 *
	 */
	#ifndef X_CONSTANT_64
		/// Assume compiler needs/accepts standard prefix for int64 types (long long)
		#define    X_CONSTANT_64(x)            x##LL
	#endif

	#ifndef X_CONSTANT_U64
		/// Assume compiler needs/accepts standard prefix for uint64 types (unsigned long long)
		#define    X_CONSTANT_U64(x)            x##ULL
	#endif

	/**
	 *
	 * unsigned 32 bit constant support
	 *
	 */
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
	typedef					X_UINT64 			u64;
	typedef					X_UINT128			u128;
	typedef					X_UINT256			u256;
	typedef		signed      X_INT8   			s8;
	typedef		signed      X_INT16  			s16;
	typedef		signed      X_INT32  			s32;
	typedef					X_INT64  			s64;
	typedef					X_INT128 			s128;
	typedef					X_INT256 			s256;
	typedef					X_FLOAT  			f32;
	typedef					X_DOUBLE 			f64;
	typedef					X_BYTE   			xbyte;
	typedef					X_BOOL				xbool32;
	typedef					bool					xbool;
	typedef					X_SIZE				xsize_t;
	typedef					X_PTR_SIZED_INT		uptr;

	//==============================================================================
	// UTF 8, 16, 32
	struct uchar8
	{
		static const u32 null = 0;

		inline					uchar8() : c(null) {}
		inline					uchar8(char _c) : c(_c) {}
		inline explicit			uchar8(u8 _c) : c(_c) {}
		inline explicit			uchar8(u32 _c) : c(_c) {}

		inline bool				operator <  (const uchar8& _other) const		{ return c < _other.c; }
		inline bool				operator >  (const uchar8& _other) const		{ return c > _other.c; }
		inline bool				operator <= (const uchar8& _other) const		{ return c <= _other.c; }
		inline bool				operator >= (const uchar8& _other) const		{ return c >= _other.c; }
		inline bool				operator == (const uchar8& _other) const		{ return c == _other.c; }
		inline bool				operator != (const uchar8& _other) const		{ return c != _other.c; }

		u32		c; 
	};

	#pragma pack(push, 1)
	struct ustr8	// UTF-8 string
	{
		u8 c;
	};
	#pragma pack(pop)

	struct uchar16
	{
		static const u32 null = 0;

		inline					uchar16() : c(null) {}
		inline explicit			uchar16(u16 _c) : c(_c) {}
		inline explicit			uchar16(u32 _c) : c(_c) {}

		inline bool				operator <  (const uchar16& _other) const		{ return c < _other.c; }
		inline bool				operator >  (const uchar16& _other) const		{ return c > _other.c; }
		inline bool				operator <= (const uchar16& _other) const		{ return c <= _other.c; }
		inline bool				operator >= (const uchar16& _other) const		{ return c >= _other.c; }
		inline bool				operator == (const uchar16& _other) const		{ return c == _other.c; }
		inline bool				operator != (const uchar16& _other) const		{ return c != _other.c; }

		u32		c; 
	};

	#pragma pack(push, 2)
	struct ustr16	// UTF-16 string
	{
		u16 c;
	};
	#pragma pack(pop)

	struct uchar32
	{ 
		static const u32 null = 0;

		inline					uchar32() : c(null) {}
		inline explicit			uchar32(u32 _c) : c(_c) {}

		inline bool				operator <  (const uchar32& _other) const		{ return c < _other.c; }
		inline bool				operator >  (const uchar32& _other) const		{ return c > _other.c; }
		inline bool				operator <= (const uchar32& _other) const		{ return c <= _other.c; }
		inline bool				operator >= (const uchar32& _other) const		{ return c >= _other.c; }
		inline bool				operator == (const uchar32& _other) const		{ return c == _other.c; }
		inline bool				operator != (const uchar32& _other) const		{ return c != _other.c; }

		u32		c; 
	};

	struct ustr32	// UTF-32 string
	{
		u32 c;
	};

	//==============================================================================
	
	#define					XCAST(to, from)		((to)(from))

	//DOM-IGNORE-END
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------------------------------------------

	//==============================================================================
	// Min/Max values
	//==============================================================================

	const u8							xU8Min = (u8) 0x00;							///< minimum value of a u8.
	const u8							xU8Max = (u8) 0xFF;							///< maximum value of a u8.
	const s8							xS8Min = (s8) -0x80;							///< minimum value of a s8.
	const s8							xS8Max = (s8) 0x7F;							///< maximum value of a s8.

	const u16						xU16Min = (u16) 0x0000;						///< minimum value of a u16
	const u16						xU16Max = (u16) 0xFFFF;						///< maximum value of a u16.
	const s16						xS16Min = (s16) -0x8000;						///< minimum value of a s16.
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
	const f32                       xF32Eps = (f32) 0.0001f;						///< default epsilon generally good to check values in the range [0 - 1], normalisations, dot products and such.

	const f64						xF64Min = (f64) 2.2250738585072014e-308;		///< minimum value of a f64.
	const f64						xF64Max = (f64) 1.7976931348623158e+308;		///< maximum value of a f64.

	//==============================================================================
	//  HANDLE BOOL 
	//==============================================================================
	const xbool						xTrue  = true;								///< the value of True to true
	const xbool						xFalse = false;								///< the value of False to false
	const xbool						xTRUE  = true;								///< the value of True to true
	const xbool						xFALSE = false;								///< the value of False to false

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif
