#ifndef __XCORE_STRING_BUFFERS_H__
#define __XCORE_STRING_BUFFERS_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_types.h"
#include "xbase\x_debug.h"

#include "xbase\x_string_std.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	//
	// Summary:
	//     Here are all the different xstring buffers that are used. They are used
	//     as a 'type' in a template architecture. The main interface can be seen
	//     by looking at xstring_buffer_const.
	//
	// Note:
	//     All string buffers make sure that they are terminated with 2 '\0' chars.
	//
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Description:
	//     The xstring_buffer_char is a helper class that points to a buffer that 
	//     was passed by the user, no alloc, realloc or free is being done.
	//     This represents a string which is mutable but not able to resize.
	//------------------------------------------------------------------------------
	class xstring_buffer_char
	{
	public:
		xstring_buffer_char()
			: mString(NULL)
			, mLength(0)
			, mMaxLength(0)
		{
		}

		xstring_buffer_char(void* buffer, s32 bufferSize)
			: mString((char*)buffer)
			, mLength(0)
			, mMaxLength(bufferSize-2)
		{
			terminateWithZero();
		}

		xstring_buffer_char(void* buffer, s32 bufferSize, const char* str)
			: mString((char*)buffer)
			, mMaxLength(bufferSize-2)
		{ 
			copyFrom(str, mMaxLength);
		}

		xstring_buffer_char(void* buffer, s32 bufferSize, const char* formatString, const x_va_list& args)
			: mString((char*)buffer)
			, mMaxLength(bufferSize-2)
		{ 
			mLength = x_vsprintf(mString, mMaxLength, formatString, args);
		}

		xstring_buffer_char(void* buffer, s32 bufferSize, const char* formatString, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
			: mString((char*)buffer)
			, mLength(0)
			, mMaxLength(bufferSize-2)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
			mLength = x_vsprintf(mString, mMaxLength, formatString, args);
		}

		inline				~xstring_buffer_char()
		{
			mString = NULL;
		}

		inline s32			getLength() const									{ return mLength; }
		inline s32			getMaxLength() const								{ return mMaxLength; }

		inline bool			isEmpty() const										{ return mLength == 0; }

		inline char*		getPointer()										{ return mString; }
		inline const char*	getPointer() const									{ return mString; }

		inline xbool		isGrowable() const									{ return xFALSE; }
		inline void			terminateWithZero()									{ mString[mLength] = '\0'; mString[mLength+1] = '\0'; }

		// Reference counting interface
		void				bind()												{ }
		void				unbind()											{ }

		// Dynamic buffer interface
		void				setLength(s32 inLength)								{ mLength = inLength; }
		void				resize(s32)								            { }
		void				unique()											{ }
		void				setEmpty()											{ mLength = 0; }

		void				copyFrom(const char* str, s32 length)
		{
			mLength = 0; 
			while (mLength<length && mLength<mMaxLength && *str!='\0')
				mString[mLength++] = *str++;
			terminateWithZero();
		}

		char*				mString;
		s32					mLength;
		s32					mMaxLength;
	};

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Description:
	//     The xstring_buffer_const_char is a helper class that points to a system 
	//     'c' string, no alloc, realloc or free is being done.
	//     This represents a string which is not mutable and not able to resize.
	//------------------------------------------------------------------------------
	class xstring_buffer_const_char
	{
	public:
		inline				xstring_buffer_const_char()
			: mString(NULL)
			, mLength(0)
		{
		}

		inline 				xstring_buffer_const_char(const char* str)
			: mString(str)
			, mLength(x_strlen(str))
		{
		}

		inline 				xstring_buffer_const_char(s32 len, const char* str)
			: mString(str)
			, mLength(len)
		{
		}

		inline				~xstring_buffer_const_char()
		{
			mString = NULL;
		}


		inline s32			getLength() const									{ return mLength; }

		inline bool			isEmpty() const										{ return mLength == 0; }

		inline const char*	getPointer()										{ return mString; }
		inline const char*	getPointer() const									{ return mString; }

		inline xbool		isGrowable() const									{ return xFALSE; }

		const char*			mString;
		s32					mLength;
	};

	
	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Description:
	//     The xstring_buffer_tmp class is a helper class to alloc and free a custom 
	//     temp based piece of memory. This represents a string which is mutable but
	//     not able to resize.
	//------------------------------------------------------------------------------
	class xstring_buffer_tmp
	{
	public:
							xstring_buffer_tmp();
							xstring_buffer_tmp(s32 length);
							xstring_buffer_tmp(const char* str);
							xstring_buffer_tmp(s32 length, const char* str);
							xstring_buffer_tmp(s32 lengthA, const char* strA, s32 lengthB, const char* strB);
							xstring_buffer_tmp(const char* formatString, const x_va_list& args);
							xstring_buffer_tmp(const char* formatString, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10);
							~xstring_buffer_tmp();

		inline s32			getLength() const									{ return mLength; }
		inline s32			getMaxLength() const								{ return mMaxLength; }

		inline bool			isEmpty() const										{ return mLength == 0; }

		inline char*		getPointer()										{ return mString; }
		inline const char*	getPointer() const									{ return mString; }

		inline xbool		isGrowable() const									{ return xFALSE; }

		inline void			terminateWithZero()									{ mString[mLength] = '\0'; mString[mLength+1] = '\0'; }

		// Reference counting interface
		void				bind()												{ }
		void				unbind()											{ }

		// Dynamic buffer interface
		void				setLength(s32 inLength)								{ mLength = inLength; }
		void				resize(s32)								            { }
		void				unique()											{ }
		void				setEmpty()											{ mLength = 0; }

	protected:
		char*				mString;
		s32					mLength;
		s32					mMaxLength;
	};

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Description:
	//     The xstring_buffer class is a helper class to alloc, realloc, separate 
	//     and destroy a reference counted string buffer.
	//     The bind/unbind functions is the 'reference counting' interface and is
	//     used to connect to a given buffer or disconnect from your own buffer. 
	//     When disconnecting the buffer determines if it needs to destroy itself.
	//     This represents a string which is mutable and able to grow/shrink.
	//------------------------------------------------------------------------------
	class xstring_buffer_heap_object
	{
	public:
		inline					xstring_buffer_heap_object(s32 refCount, s32 length, s32 maxLength) : mRefCount(refCount), mLength(length), mMaxLength(maxLength)
		{ 
			mData[0]='\0';
			mData[1]='\0';
			mData[2]='h'; 
			mData[3]='e'; 
			mData[4]='a'; 
			mData[5]='p'; 
			mData[6]='\0';
			mData[7]='\0';
		}

		inline void				bind()												{ ++mRefCount; }		///< Add a reference to this class
		void					unbind();											///< Remove a reference to this class, if no references are left the object is destructed

		bool					isEmptyStringBuffer() const;

		s32						mRefCount;											///< Reference count
		mutable s32				mLength;											///< Data length
		mutable s32				mMaxLength;											///< Number characters allocated for the text buffer (excluding the terminating zero)
		char					mData[8];											///< String data (allocated with sAlloc so that it is large enough to fit the string)
	};


	class xstring_buffer_heap
	{
	public:
								xstring_buffer_heap();
								xstring_buffer_heap(s32 len);
								xstring_buffer_heap(s32 len, const char* str);
								xstring_buffer_heap(s32 lenA, const char* strA, s32 lenB, const char* strB);
								xstring_buffer_heap(const char* formatString, const x_va_list& args);
								xstring_buffer_heap(const char* formatString, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10);
								~xstring_buffer_heap();

		inline s32				getLength() const									{ return mBuffer->mLength; }
		inline s32				getMaxLength() const								{ return mBuffer->mMaxLength; }

		inline bool				isEmpty() const										{ return mBuffer->mLength == 0; }

		inline char*			getPointer()										{ return &mBuffer->mData[0]; }
		inline const char*		getPointer() const									{ return &mBuffer->mData[0]; }

		inline bool				isGrowable() const									{ return true; }

		inline void				terminateWithZero()									{ mBuffer->mData[mBuffer->mLength] = '\0'; mBuffer->mData[mBuffer->mLength+1] = '\0'; }

		// Reference counting interface
		void					bind()												{ mBuffer->bind(); }					///< Add a reference to this class
		void					unbind()											{ mBuffer->unbind(); mBuffer=NULL; }	///< Remove a reference to this class, if no references are left the object is destructed

		// Dynamic buffer interface
		inline void				setLength(s32 inLength)								{ mBuffer->mLength = inLength; }
		void					resize(s32 inLength);
		void					unique();
		void					setEmpty();

	private:
		xstring_buffer_heap_object*	mBuffer;
	};


//==============================================================================
// END xCore namespace
//==============================================================================
};

//==============================================================================
// END
//==============================================================================
#endif    /// __XCORE_STRING_BUFFERS_H__
