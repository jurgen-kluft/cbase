#ifndef __XBASE_DEBUG_H__
#define __XBASE_DEBUG_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_types.h"
#include "xbase\x_va_list.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//==============================================================================
	// BREAK & NOP
	//==============================================================================
	// This two macros are use mainly for debugging. The nop is used more to
	// separate a block of code from another. Very useful when looking at the 
	// assembly code.
	//==============================================================================
	#ifdef BREAK
		#undef BREAK
	#endif

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Stop the program with a break point.
	// Description:
	//     The macro BREAK will cause a debugger breakpoint if possible on any given 
	//     platform.  If a breakpoint cannot be caused, then a divide by zero will be 
	//     forced.  Note that the BREAK macro is highly platform specific.  The 
	//     implementation of BREAK on some platforms prevents it from being used 
	//     syntactically as an expression.  It can only be used as a statement.
	// See Also:
	//     NOP ASSERT
	//------------------------------------------------------------------------------
	#if defined(TARGET_PC) && defined(COMPILER_DEFAULT)
		#define BREAK      { __debugbreak(); }
	#endif

	#if defined(TARGET_360) && defined(COMPILER_DEFAULT)
		#define BREAK      {__debugbreak(); }
	#endif

	#if defined(TARGET_PS3) && defined(COMPILER_DEFAULT)
		#define BREAK      { __asm__ volatile("tw 31,1,1"); }
	#endif

	#if defined(TARGET_WII) && defined(COMPILER_DEFAULT)
		#define BREAK      { register unsigned int reg0; register unsigned int reg1; asm { mfmsr reg0; ori reg1, reg0, 0x400; mtmsr reg1; mtmsr reg0; } }
	#endif

	#if defined(TARGET_3DS) && defined(COMPILER_DEFAULT)
		#define BREAK      { __breakpoint(0xBA5E); }
	#endif

	#if defined(TARGET_PSP) && defined(COMPILER_DEFAULT)
		#define BREAK      { asm("break"); }
	#endif

	#if !defined(BREAK)
		#error Unknown Platform/Compiler configuration for BREAK
	#endif


	//------------------------------------------------------------------------------
	#ifdef NOP
		#undef NOP
	#endif

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Sets a NOP operation in the code assembly.
	// Description:
	//     Sets a NOP operation in the code assembly. This is commonly used for 
	//     debugging. By adding nops allows to see the assembly clearly in code.
	// See Also:
	//     BREAK
	//------------------------------------------------------------------------------

	#if defined(TARGET_PC) && defined(COMPILER_DEFAULT)
		#define NOP            { __emit(0x00000000); }
	#endif

	#if defined(TARGET_360) && defined(COMPILER_DEFAULT)
		#define NOP            { __asm nop }
	#endif

	#if defined(TARGET_PSP) && defined(COMPILER_DEFAULT)
		#define NOP            { _asm { nop } }
	#endif

	#if defined(TARGET_PS3) && defined(COMPILER_DEFAULT)
		#define NOP            { __asm nop }
	#endif

	#if defined(TARGET_WII) && defined(COMPILER_DEFAULT)
		#define NOP            { __asm nop }
	#endif

	#if defined(TARGET_3DS) && defined(COMPILER_DEFAULT)
		#define NOP            { void __nop(void); }
	#endif

	#if !defined(NOP)
		#error Unknown Platform/Compiler configuration for NOP
	#endif

	//==============================================================================
	// ASSERT/XVERIFY
	//==============================================================================

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Assert handler function type declaration.
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
	//     If the user wants to overwrite the default assert handler his function must
	//     be of this type. 
	// See Also:
	//     ASSERTS XVERIFY x_SetAssertHandler x_GetAssertHandler
	//------------------------------------------------------------------------------
	#ifdef X_ASSERT
	typedef xbool        xassert_fn(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
	extern xbool		x_AssertHandler(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
	#endif

	//------------------------------------------------------------------------------

	// Button = 0, 1, 2, 3
	// -------+-----------
	// WII    = A, B, 1, 2
	// 360    = A, B, X, Y
	// PC     = A, B, X, Y
	// PSP    = CIRCLE, CROSS, SQUARE, TRIANGLE
	// PS3    = CIRCLE, CROSS, SQUARE, TRIANGLE
	#ifdef X_ASSERT
	typedef void		xassert_input_fn(xbool& _button0, xbool& _button1, xbool& _button2, xbool& _button3);		
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

		#define ASSERTCT(expr)               do { } while(xcore::xCompileTimeAssert<(expr)!=0>::OK);
		#define ASSERTSL(level,expr,str)     do { static xcore::u32 sAssertFlags = 0; if (xcore::x_ChooseDebugLevel(level) && ((expr)==false) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, str ))  BREAK; } while (0)
		#define ASSERTL(level,expr)          do { static xcore::u32 sAssertFlags = 0; if (xcore::x_ChooseDebugLevel(level) && ((expr)==false) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, NULL))  BREAK; } while (0)
		#define ASSERT(expr)                 do { static xcore::u32 sAssertFlags = 0; if (!((expr)!=0) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, NULL))  BREAK; } while (0)
		#define ASSERTS(expr,str)            do { static xcore::u32 sAssertFlags = 0; if (!((expr)!=0) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, str))  BREAK; } while (0)
		#define XVERIFY(expr)                do { static xcore::u32 sAssertFlags = 0; if (!((expr)!=0) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, NULL))  BREAK; } while (0)
		#define XVERIFYS(expr,str)           do { static xcore::u32 sAssertFlags = 0; if (!((expr)!=0) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #expr, str))  BREAK; } while (0)
		#define XBOUNDS(v,l,h)               do { static xcore::u32 sAssertFlags = 0; if (!((v)>=(l) && (v)<=(h)) && xcore::x_AssertHandler(sAssertFlags, __FILE__, __LINE__, #v ">=" #l " && " #v "<=" #h, NULL))  BREAK; } while (0)

		#else

		#define ASSERTCT(expr)              
		#define ASSERTSL(level,expr,str)    
		#define ASSERTL(level,expr)         
		#define ASSERT(expr)                
		#define ASSERTS(expr,str)           
		#define XVERIFY(expr)                
		#define XVERIFYS(expr,str)           
		#define XBOUNDS(v,l,h)               


		#endif

	#else

		#if defined(TARGET_PC) && defined(COMPILER_DEFAULT)
			#define ASSERTCT(expr)              
			#define ASSERT(expr)                (void(0))
			#define ASSERTS(expr,str)           (void(0))
			#define ASSERTSL(level,expr,str)    (void(0))
			#define ASSERTL(level,expr)         (void(0))
		#elif defined(TARGET_360) && defined(COMPILER_DEFAULT)
			#define ASSERTCT(expr)              
			#define ASSERT(expr)                (void(0))
			#define ASSERTS(expr,str)           (void(0))
			#define ASSERTSL(level,expr,str)    (void(0))
			#define ASSERTL(level,expr)         (void(0))
		#elif defined(TARGET_PS3) && defined(COMPILER_DEFAULT)
			#define ASSERTCT(expr)              
			#define ASSERT(expr)                (void(0))
			#define ASSERTS(expr,str)           (void(0))
			#define ASSERTSL(level,expr,str)    (void(0))
			#define ASSERTL(level,expr)         (void(0))
		#elif defined(TARGET_WII) && defined(COMPILER_DEFAULT)
			#define ASSERTCT(expr)              
			#define ASSERT(expr)                (void(0))
			#define ASSERTS(expr,str)           (void(0))
			#define ASSERTSL(level,expr,str)    (void(0))
			#define ASSERTL(level,expr)         (void(0))
		#elif defined(TARGET_3DS) && defined(COMPILER_DEFAULT)
			#define ASSERTCT(expr)              
			#define ASSERT(expr)                (void(0))
			#define ASSERTS(expr,str)           (void(0))
			#define ASSERTSL(level,expr,str)    (void(0))
			#define ASSERTL(level,expr)         (void(0))
		#elif defined(TARGET_PSP) && defined(COMPILER_DEFAULT)
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

	//------------------------------------------------------------------------------
	#ifdef X_ASSERT

	extern void			x_SetAssertHandler(xassert_fn* Function);
	extern xassert_fn*	x_GetAssertHandler(void);

	#endif

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


#endif    /// __XBASE_DEBUG_H__
