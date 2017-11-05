#ifndef __XBASE_DEBUG_H__
#define __XBASE_DEBUG_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_va_list.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//==============================================================================
	// The debug API
	//==============================================================================
	class x_asserthandler
	{
	public:
		virtual	~x_asserthandler() {}

		enum EDebugFlags
		{
			XDB_FLAG_IGNORE = 1<<1,
		};
		virtual xbool	HandleAssert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString) = 0;

		static void		sRegisterHandler(x_asserthandler* handler);
	};

	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Main assert handler.
	// Arguments:
	//     flags                - Reference to a local copy of flags for the assert.
	//     fileName             - File name in which the assert happen
	//     lineNumber           - Line number where the assert happen
	//     exprString           - Expression of the assert
	//     messageString        - Additional string containing information about the 
	//                            assert.
	// Returns:
	//     xbool - TRUE when the program should be halted FALSE other wise
	// Description:
	//     This is the main entry of an assert, beyond this it will be dispatched to
	//     the x_debug object associated with the current thread.
	// See Also:
	//     ASSERTS XVERIFY
	//------------------------------------------------------------------------------
#ifdef X_ASSERT
	extern xbool		x_AssertHandler(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
#endif



	//==============================================================================
	// XBREAK & XNOP
	//==============================================================================
	// This two macros are use mainly for debugging. The nop is used more to
	// separate a block of code from another. Very useful when looking at the 
	// assembly code.
	//==============================================================================
	#ifdef XBREAK
		#undef XBREAK
	#endif

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Stop the program with a break point.
	// Description:
	//     The macro XBREAK will cause a debugger breakpoint if possible on any given 
	//     platform.  If a breakpoint cannot be caused, then a divide by zero will be 
	//     forced.  Note that the XBREAK macro is highly platform specific.  The 
	//     implementation of XBREAK on some platforms prevents it from being used 
	//     syntactically as an expression.  It can only be used as a statement.
	// See Also:
	//     NOP ASSERT
	//------------------------------------------------------------------------------
	#if defined(TARGET_PC) && defined(COMPILER_DEFAULT)
		#define XBREAK      { __debugbreak(); }
	#endif

	#if defined(TARGET_MACOS) && defined(COMPILER_DEFAULT)
		#define XBREAK      { __asm__ volatile("tw 31,1,1"); }
	#endif

	#if !defined(XBREAK)
		#error Unknown Platform/Compiler configuration for XBREAK
	#endif


	//------------------------------------------------------------------------------
	#ifdef XNOP
		#undef XNOP
	#endif

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Sets a XNOP operation in the code assembly.
	// Description:
	//     Sets a XNOP operation in the code assembly. This is commonly used for 
	//     debugging. By adding nops allows to see the assembly clearly in code.
	// See Also:
	//     BREAK
	//------------------------------------------------------------------------------

	#if defined(TARGET_PC) && defined(COMPILER_DEFAULT)
		#define XNOP            { __emit(0x00000000); }
	#endif

	#if defined(TARGET_MACOS) && defined(COMPILER_DEFAULT)
		#define XNOP            { __asm nop }
	#endif

	#if !defined(XNOP)
		#error Unknown Platform/Compiler configuration for XNOP
	#endif


	//==============================================================================
	//  Runtime validation support.  
	//==============================================================================
	//
	//  Most of the run-time validations are one form or another of an ASSERT.  So,
	//  for lack of a better name, the presence of the compile time macro X_ASSERT 
	//  activates the optional run-time validations.  (And, of course, the absence 
	//  of X_ASSERT deactivates them.)
	//
	//  The following macros and functions are all designed to perform validation of
	//  expected conditions at run-time.  Each takes an expression as the first 
	//  parameter.  The expression (expr) is expected to be TRUE whenever evaluated.
	//  
	//      ASSERT  (expr)
	//      ASSERTS (expr, message)
	//
	//      XVERIFY  (expr)
	//      XVERIFYS (expr, message)
	//  
	//  The macros ASSERT and ASSERTS completely evaporate in when X_ASSERT is not
	//  defined.  Macros XVERIFY and XVERIFYS, lacking X_ASSERT, still evaluate the 
	//  expression, but do not validate the result.  Consider:
	//  
	//      ASSERT(CriticalInitialization());    // EVIL without X_ASSERT!
	//      XVERIFY(CriticalInitialization());    // Safe without X_ASSERT.
	//  
	//  The ASSERTS and XVERIFYS macros accept a message string to assist problem
	//  diagnosis.  Upon a run-time failure, the message is displayed.  For example:
	//  
	//      ASSERTS(Radius >= 0.0f, "Radius must be non-negative.");
	//  
	//  To place formatted strings within ASSERTS and XVERIFYS, use the xstring_tmp class 
	//  from x_string.h.  For example:
	//  
	//      pFile = x_fopen(fileName, "rt");
	//      ASSERTS(pFile, xstring_tmp("Failed to open file '%s'.", fileName));
	//
	//  Available options:
	//
	//    - As previously mentioned, the macro X_ASSERT enables the validation 
	//      macros.  X_ASSERT should be always be present in debug configurations.
	//
	//==============================================================================
	#ifdef ASSERT
	#undef ASSERT
	#endif

	#ifdef ASSERTS
	#undef ASSERTS
	#endif

	#ifdef XVERIFY
	#undef XVERIFY
	#endif

	#ifdef XVERIFYS
	#undef XVERIFYS
	#endif

	#ifdef X_ASSERT
		template <bool> struct xCompileTimeAssert; 
		template<> struct xCompileTimeAssert<true> { enum { OK = 0 }; };

		#ifndef SPU

		#define XVERIFY(expr)                do { static xcore::u32 sAssertFlags = 0; if (!((expr)!=0) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, NULL))  XBREAK; } while (0)
		#define XVERIFYS(expr,str)           do { static xcore::u32 sAssertFlags = 0; if (!((expr)!=0) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, str))  XBREAK; } while (0)
		#define XBOUNDS(v,l,h)               do { static xcore::u32 sAssertFlags = 0; if (!((v)>=(l) && (v)<=(h)) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #v ">=" #l " && " #v "<=" #h, NULL))  XBREAK; } while (0)

		#define XASSERTCT(expr)              do { } while(xcore::xCompileTimeAssert<(expr)!=0>::OK);
		#define XASSERTSL(level,expr,str)    do { static xcore::u32 sAssertFlags = 0; if (xcore::x_ChooseDebugLevel(level) && ((expr)==false) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, str ))  XBREAK; } while (0)
		#define XASSERTL(level,expr)         do { static xcore::u32 sAssertFlags = 0; if (xcore::x_ChooseDebugLevel(level) && ((expr)==false) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, NULL))  XBREAK; } while (0)
		#define XASSERT(expr)                do { static xcore::u32 sAssertFlags = 0; if (!((expr)!=0) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, NULL))  XBREAK; } while (0)
		#define XASSERTS(expr,str)           do { static xcore::u32 sAssertFlags = 0; if (!((expr)!=0) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, str))  XBREAK; } while (0)
		
		#define ASSERTCT(expr)               XASSERTCT(expr)
		#define ASSERTSL(level,expr,str)     XASSERTSL(level,expr,str)
		#define ASSERTL(level,expr)          XASSERTL(level,expr)
		#define ASSERT(expr)                 XASSERT(expr)
		#define ASSERTS(expr,str)            XASSERTS(expr,str)

		#else
 
		#define XVERIFY(expr)                
		#define XVERIFYS(expr,str)           
		#define XBOUNDS(v,l,h)               

		#define XASSERTCT(expr)              
		#define XASSERTSL(level,expr,str)    
		#define XASSERTL(level,expr)         
		#define XASSERT(expr)                
		#define XASSERTS(expr,str)   

		#define ASSERTCT(expr)               XASSERTCT(expr)
		#define ASSERTSL(level,expr,str)     XASSERTSL(level,expr,str)
		#define ASSERTL(level,expr)          XASSERTL(level,expr)
		#define ASSERT(expr)                 XASSERT(expr)
		#define ASSERTS(expr,str)            XASSERTS(expr,str)

		#endif

	#else

		#if defined(TARGET_PC) && defined(COMPILER_DEFAULT)
			#define XASSERTCT(expr)              
			#define XASSERT(expr)               (void(0))
			#define XASSERTS(expr,str)          (void(0))
			#define XASSERTSL(level,expr,str)   (void(0))
			#define XASSERTL(level,expr)        (void(0))
			#define ASSERTCT(expr)              
			#define ASSERT(expr)                (void(0))
			#define ASSERTS(expr,str)           (void(0))
			#define ASSERTSL(level,expr,str)    (void(0))
			#define ASSERTL(level,expr)         (void(0))
		#elif defined(TARGET_MACOS) && defined(COMPILER_DEFAULT)
			#define XASSERTCT(expr)              
			#define XASSERT(expr)               (void(0))
			#define XASSERTS(expr,str)          (void(0))
			#define XASSERTSL(level,expr,str)   (void(0))
			#define XASSERTL(level,expr)        (void(0))
			#define ASSERTCT(expr)              
			#define ASSERT(expr)                (void(0))
			#define ASSERTS(expr,str)           (void(0))
			#define ASSERTSL(level,expr,str)    (void(0))
			#define ASSERTL(level,expr)         (void(0))
		#else
			#error Unknown target/compiler configuration for defining assert
		#endif

		#define XVERIFY(expr)				(expr)           
		#define XVERIFYS(expr,str)			(expr)
		#define XBOUNDS(v,l,h)               

	#endif


	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


#endif    /// __XBASE_DEBUG_H__
