#ifndef __XBASE_LIMITS_H__
#define __XBASE_LIMITS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"


/**
 * xCore namespace
 */
namespace xcore
{
	/**
	 *  MACROS  
	 */

	#define X_U8_MIN                      (0)
	#define X_U8_MAX                      (255)                               

	#define X_U16_MIN                     (0)
	#define X_U16_MAX                     (65535)                               

	#define X_U32_MIN                     (0)
	#define X_U32_MAX                     (X_CONSTANT_U32(4294967295))

	#define X_U64_MIN                     (X_CONSTANT_U64(0))
	#define X_U64_MAX                     (X_CONSTANT_U64(18446744073709551615))

	#define X_S8_MIN                     -(128)
	#define X_S8_MAX                      (127)                           

	#define X_S16_MIN                    -(32768)
	#define X_S16_MAX                     (32767)                            

	#define X_S32_MIN                     (0x80000000L)
	#define X_S32_MAX                     (2147483647)

	#define X_S64_MIN                     (0x8000000000000000LL)
	#define X_S64_MAX                     (X_CONSTANT_64(9223372036854775807))

	#define X_F32_MIN                     -3.402823466e+38f
	#define X_F32_MAX                     3.402823466e+38f

	#define X_F64_MIN                     -1.7976931348623158e+308
	#define X_F64_MAX                     1.7976931348623158e+308

	#define X_XBYTE_MIN                   X_U8_MIN
	#define X_XBYTE_MAX                   X_U8_MAX

	#define X_XWCHAR_MIN                  U16_MIN
	#define X_XWCHAR_MAX                  U16_MAX


	/**
	 * Author:
	 *     Virtuos Games
	 * Description:
	 *     class xlimits is a template class, which use the C++ class template specialization. 
	 *     The idea of C++ class template specialization is similar to function template
	 *     overloading. This can make the template code for certain data types to be fixed.
	 *
	 * Example:
	 *<CODE>
	 *      template<class T>
	 *      bool isInRange(T inNum)
	 *      {
	 *          if ((inNum >= xlimits<T>::minimum()) && (inNum <= xlimits<T>::maximum()))
	 *          {
	 *                return true;
	 *            }
	 *            return false;
	 *         }      
	 *      
	 *</CODE>
	 *
	 */
	template<typename T>
	class xlimits { };

	#undef min
	#undef max

	template<>
	class xlimits<u8>
	{
	public:
		typedef u8 xtype;

		static xtype minimum() { return X_U8_MIN; }
		static xtype maximum() { return X_U8_MAX; }
	};

	template<>
	class xlimits<u16>
	{
	public:
		typedef u16 xtype;

		static xtype minimum() { return X_U16_MIN; }
		static xtype maximum() { return X_U16_MAX; }
	};

	template<>
	class xlimits<u32>
	{
	public:
		typedef u32 xtype;

		static xtype minimum() { return X_U32_MIN; }
		static xtype maximum() { return X_U32_MAX; }
	};

	template<>
	class xlimits<u64>
	{
	public:
		typedef u64 xtype;

		static xtype minimum() { return X_U64_MIN; }
		static xtype maximum() { return X_U64_MAX; }
	};

	template<>
	class xlimits<s8>
	{
	public:
		typedef s8 xtype;

		static xtype minimum() { return X_S8_MIN; }
		static xtype maximum() { return X_S8_MAX; }
	};

	template<>
	class xlimits<s16>
	{
	public:
		typedef s16 xtype;

		static xtype minimum() { return X_S16_MIN; }
		static xtype maximum() { return X_S16_MAX; }
	};

	template<>
	class xlimits<s32>
	{
	public:
		typedef s32 xtype;

		static xtype minimum() { return X_S32_MIN; }
		static xtype maximum() { return X_S32_MAX; }
	};

	template<>
	class xlimits<s64>
	{
	public:
		typedef s64 xtype;

		static xtype minimum() { return X_S64_MIN; }
		static xtype maximum() { return X_S64_MAX; }
	};

	template<>
	class xlimits<f32>
	{
	public:
		typedef f32 xtype;

		static xtype minimum() { return X_F32_MIN; }
		static xtype maximum() { return X_F32_MAX; }
	};

	template<>
	class xlimits<f64>
	{
	public:
		typedef f64 xtype;

		static xtype minimum() { return X_F64_MIN; }
		static xtype maximum() { return X_F64_MAX; }
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif ///< END __XBASE_LIMITS_H__
