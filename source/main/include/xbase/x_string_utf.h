#ifndef __XBASE_STRING_UTF_H__
#define __XBASE_STRING_UTF_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_integer.h"

/**
* xCore namespace
*/
namespace xcore
{
	// UTF string manipulation is using UTF-32 since it is the only UTF format that has a fixed rune size
	// and thus is easier to manipulate directly than UTF-8 or UTF-16
	namespace UTF
	{
		// From UTF-8 or UTF-16 to UTF-32
		s32			convert		(ustr8  const* _src, ustr8  const* _src_end, ustr32*& _dst, u32 _dstmaxleninbytes);
		s32			convert		(ustr16 const* _src, ustr16 const* _src_end, ustr32*& _dst, u32 _dstmaxleninbytes);

		// From UTF-32 to UTF-8 or UTF-16
		s32			convert		(ustr32 const* _src, u32 _srclen, ustr8 *& _dst, u32 _dstmaxleninbytes);
		s32			convert		(ustr32 const* _src, u32 _srclen, ustr16*& _dst, u32 _dstmaxleninbytes);

	}	// UTF
};	// xCore namespace


#endif    ///< __XBASE_STRING_UTF_H__
