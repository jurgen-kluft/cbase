
//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_string.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{

	//==============================================================================
	// xstring
	//==============================================================================

	//------------------------------------------------------------------------------

	xstring::xstring(void)
	{
	}

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Arguments:
	//     str - The string to use to construct this xstring
	// Returns:
	//     none
	// Summary:
	//     Constructor
	// Description:
	//     Construct an xstring from a 'const char*'
	// See Also:
	//     Other constructors
	//------------------------------------------------------------------------------
	xstring::xstring(const char* str)
	:__base(x_strlen(str), str)
	{
	}

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Arguments:
	//     str - The string to use to construct this xstring
	//     len - The length of 'str' to use
	// Returns:
	//     none
	// Summary:
	//     Constructor
	// Description:
	//     Construct an xstring from 'len' number of characters of 'str' 
	// See Also:
	//     Other constructors
	//------------------------------------------------------------------------------
	xstring::xstring(const char* str, s32 len)
	:__base(len, str)
	{
	}

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Arguments:
	//     strA - The string to use to construct the first part
	//     lenA - The length of 'strA' to use
	//     strB - The string to use concatenate with the first part
	//     lenB - The length of 'strB' to use
	// Returns:
	//     none
	// Summary:
	//     Algorithmic Constructor
	// Description:
	//     Construct an xstring from 2 strings with a defined length
	// See Also:
	//     Other constructors
	//------------------------------------------------------------------------------
	xstring::xstring(const char* strA, s32 lenA, const char* strB, s32 lenB)
	:__base(lenA, strA, lenB, strB)
	{
	}

	//------------------------------------------------------------------------------

	xstring::xstring(s32 length)
	:__base(length)
	{
	}

	//------------------------------------------------------------------------------

	xstring::xstring(const xstring& str)
	:__base(str)
	{
	}

	//------------------------------------------------------------------------------

	xstring::xstring(const xcstring& str)
	:__base(str)
	{
	}

	//------------------------------------------------------------------------------
	xstring::xstring(const xccstring& str)
	:__base(str)
	{
	}

	//------------------------------------------------------------------------------

	xstring xstring::sBuildFromFormat(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		xstring outStr;
		outStr.appendFormatV(format, x_va_list(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10));
		return outStr;
	}

	//------------------------------------------------------------------------------

	s32 xstring::format(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(format);

		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
		return formatV(format, args);
	}

	//------------------------------------------------------------------------------

	s32 xstring::formatV(const char* format, const x_va_list& args)
	{
		ASSERT(format);

		xstring_tmp temp(format, args);
		const s32 length = temp.getLength();

		copy(temp.c_str(), length);

		return length;
	}

	//------------------------------------------------------------------------------

	s32 xstring::appendFormat(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(format);

		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
		return appendFormatV(format, args);
	}

	//------------------------------------------------------------------------------

	s32 xstring::appendFormatV(const char* format, const x_va_list& args)
	{
		ASSERT(format);

		// Compute the string properties
		const xstring_tmp temp(format, args);
		const s32 length = temp.getLength();

		concat(temp.c_str(), length);
		return getLength();
	}

	//------------------------------------------------------------------------------

	char&               xstring::operator [](s32 inIndex)
	{
		ASSERT(inIndex<getLength()+1);
		return w_str()[inIndex];
	}

	//------------------------------------------------------------------------------

	const char&         xstring::operator [](s32 inIndex) const
	{
		ASSERT(inIndex<getLength()+1);
		return c_str()[inIndex];
	}


	//------------------------------------------------------------------------------

	xstring&			xstring::operator =(const char* inStr)
	{
		copy(inStr, x_strlen(inStr));
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&            xstring::operator =(const xstring& inStr)
	{
		copy(inStr.c_str(), inStr.getLength());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&            xstring::operator =(const xstring_tmp& inStr)
	{
		copy(inStr.c_str(), inStr.getLength());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&            xstring::operator =(const xcstring& inStr)
	{
		copy(inStr.c_str(), inStr.getLength());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&            xstring::operator =(const xccstring& inStr)
	{
		copy(inStr.c_str(), inStr.getLength());
		return *this;
	}


	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(char inRHS)
	{
		concat(&inRHS, 1);
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const char* inRHS)
	{
		concat(inRHS);
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const xstring& inRHS)
	{
		concat(inRHS.c_str(), inRHS.getLength());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const xstring_tmp& inRHS)
	{
		concat(inRHS.c_str(), inRHS.getLength());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const xcstring& inRHS)
	{
		concat(inRHS.c_str(), inRHS.getLength());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const xccstring& inRHS)
	{
		concat(inRHS.c_str(), inRHS.getLength());
		return *this;
	}





	//------------------------------------------------------------------------------

	xstring_tmp::xstring_tmp(s32 maxLength)
		: __base(maxLength)
	{
	}

	//------------------------------------------------------------------------------

	xstring_tmp::xstring_tmp(const char* str)
		: __base(str)
	{
	}

	//------------------------------------------------------------------------------

	xstring_tmp::xstring_tmp(const char* str, s32 strLen)
		: __base(strLen, str)
	{
	}

	//------------------------------------------------------------------------------

	xstring_tmp::xstring_tmp(const char* strA, s32 strLenA, const char* strB, s32 strLenB)
		: __base(strLenA, strA, strLenB, strB)
	{
	}

	//------------------------------------------------------------------------------

	xstring_tmp::xstring_tmp(const char* formatString, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
		: __base(formatString,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
	{
	}

	//------------------------------------------------------------------------------

	xstring_tmp::xstring_tmp(const char* formatString, const x_va_list& args)
		: __base(formatString,args)
	{
	}

	//------------------------------------------------------------------------------
	xstring_tmp::xstring_tmp(const xstring& other)
		: __base(other.getLength(), other.c_str())
	{
	}

	//------------------------------------------------------------------------------
	xstring_tmp::xstring_tmp(const xcstring& other)
		: __base(other.getLength(), other.c_str())
	{
	}

	//------------------------------------------------------------------------------
	xstring_tmp::xstring_tmp(const xccstring& other)
		: __base(other.getLength(), other.c_str())
	{
	}

	//------------------------------------------------------------------------------
	xstring_tmp::xstring_tmp(const xstring_tmp& other)
		: __base(other.getLength(), other.c_str())
	{
	}

	//------------------------------------------------------------------------------

	char				xstring_tmp::operator[] (s32 inIndex) const
	{
		ASSERT(inIndex<getLength());
		return mBuffer.getPointer()[inIndex];
	}

	//------------------------------------------------------------------------------

	char&				xstring_tmp::operator[]	(s32 inIndex)
	{
		ASSERT(inIndex<getLength());
		return mBuffer.getPointer()[inIndex];
	}


	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator=(const xstring& inRHS)
	{
		mBuffer.setLength(0);
		*this += inRHS;
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator=(const xcstring& inRHS)
	{
		mBuffer.setLength(0);
		*this += inRHS;
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator=(const xccstring& inRHS)
	{
		mBuffer.setLength(0);
		*this += inRHS;
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator=(const xstring_tmp& inRHS)
	{
		mBuffer.setLength(0);
		*this += inRHS;
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator=(char inRHS)
	{
		mBuffer.setLength(0);
		mBuffer.getPointer()[0] = inRHS;
		mBuffer.setLength(1);
		terminateWithZero();
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator=(const char* inRHS)
	{
		mBuffer.setLength(0);
		s32 l = 0;
		while (l<mBuffer.getMaxLength() && *inRHS!='\0')
		{
			mBuffer.getPointer()[l] = *inRHS++;
			++l;
		}
		mBuffer.setLength(l);
		terminateWithZero();
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator+=(char inRHS)
	{
		char str[3];
		str[0] = inRHS;
		str[1] = '\0';
		str[2] = '\0';
		*this += str;
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator+=(const char* inRHS)
	{
		s32 l = getLength(); 
		while (l<mBuffer.getMaxLength() && *inRHS!='\0')
		{
			mBuffer.getPointer()[l] = *inRHS++;
			++l;
		}
		mBuffer.setLength(l);
		terminateWithZero();
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator+=(const xstring& inRHS)
	{
		s32 l = getLength(); 
		while (l<mBuffer.getMaxLength() && l<inRHS.getLength())
		{
			mBuffer.getPointer()[l] = inRHS[l];
			++l;
		}
		mBuffer.setLength(l);
		terminateWithZero();
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator+=(const xstring_tmp& inRHS)
	{
		s32 l = getLength(); 
		while (l<mBuffer.getMaxLength() && l<inRHS.getLength())
		{
			mBuffer.getPointer()[l] = inRHS[l];
			++l;
		}
		mBuffer.setLength(l);
		terminateWithZero();
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator+=(const xcstring& inRHS)
	{
		s32 l = getLength(); 
		while (l<mBuffer.getMaxLength() && l<inRHS.getLength())
		{
			mBuffer.getPointer()[l] = inRHS[l];
			++l;
		}
		mBuffer.setLength(l);
		terminateWithZero();
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring_tmp&		xstring_tmp::operator+=(const xccstring& inRHS)
	{
		s32 l = getLength(); 
		while (l<mBuffer.getMaxLength() && l<inRHS.getLength())
		{
			mBuffer.getPointer()[l] = inRHS[l];
			++l;
		}
		mBuffer.setLength(l);
		terminateWithZero();
		return *this;
	}


	//------------------------------------------------------------------------------

	s32 xcstring::format(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(format);

		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
		return formatV(format, args);
	}

	//------------------------------------------------------------------------------

	s32 xcstring::formatV(const char* format, const x_va_list& args)
	{
		ASSERT(format);

		xstring_tmp temp(format, args);
		const s32 length = temp.getLength();

		copy(temp.c_str(), length);

		return length;
	}

	//------------------------------------------------------------------------------

	s32 xcstring::appendFormat(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(format);

		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
		return appendFormatV(format, args);
	}

	//------------------------------------------------------------------------------

	s32 xcstring::appendFormatV(const char* format, const x_va_list& args)
	{
		ASSERT(format);

		// Compute the string properties
		const xstring_tmp temp(format, args);
		const s32 length = temp.getLength();

		concat(temp.c_str(), length);
		return getLength();
	}

//==============================================================================
// END xCore namespace
//==============================================================================
};
