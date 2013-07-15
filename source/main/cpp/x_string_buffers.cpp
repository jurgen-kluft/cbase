//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_types.h"
#include "xbase\x_debug.h"
#include "xbase\x_integer.h"
#include "xbase\x_string_std.h"
#include "xbase\x_memory_std.h"
#include "xbase\x_allocator.h"
#include "xbase\x_string.h"
#include "xbase\private\x_string_buffers.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	static x_iallocator*	sStringHeapAllocator = NULL;
	static x_iallocator*	sStringTempAllocator = NULL;

	void	xstring::sSetAllocator(x_iallocator* allocator)
	{
		sStringHeapAllocator = allocator;
	}

	void	xstring_tmp::sSetAllocator(x_iallocator* allocator)
	{
		sStringTempAllocator = allocator;
	}

	//==============================================================================
	//==============================================================================
	//==============================================================================
	// xstring_buffer_tmp
	//==============================================================================
	//==============================================================================
	//==============================================================================
	xstring_buffer_tmp::xstring_buffer_tmp()
	:mString(NULL)
	,mLength(0)
	,mMaxLength(0)
	{
	}

	xstring_buffer_tmp::xstring_buffer_tmp(s32 length)
	:mLength(0)
	,mMaxLength(length)
	{
		mString = (char*)sStringTempAllocator->allocate(length + 2, X_ALIGNMENT_DEFAULT);
		terminateWithZero();
	}

	xstring_buffer_tmp::xstring_buffer_tmp(const char* str)
	:mLength(x_strlen(str))
	,mMaxLength(mLength)
	{
		mString = (char*)sStringTempAllocator->allocate(mLength + 2, X_ALIGNMENT_DEFAULT);
		x_memcpy(mString, str, mLength);
		terminateWithZero();
	}

	xstring_buffer_tmp::xstring_buffer_tmp(s32 length, const char* str)
	:mLength(length)
	,mMaxLength(length)
	{
		mString = (char*)sStringTempAllocator->allocate(mLength + 2, X_ALIGNMENT_DEFAULT);
		x_memcpy(mString, str, mLength);
		terminateWithZero();
	}

	xstring_buffer_tmp::xstring_buffer_tmp(s32 lengthA, const char* strA, s32 lengthB, const char* strB)
		: mLength(lengthA + lengthB)
		, mMaxLength(lengthA + lengthB)
	{
		mString = (char*)sStringTempAllocator->allocate(mLength + 2, X_ALIGNMENT_DEFAULT);
		x_memcpy(mString, strA, lengthA);
		x_memcpy(mString + lengthA, strB, lengthB);
		terminateWithZero();
	}

	xstring_buffer_tmp::xstring_buffer_tmp(const char* formatString, const x_va_list& args)
	{
		ASSERT(formatString);
		mLength = x_vcprintf(formatString, args);
		mString = (char*)sStringTempAllocator->allocate(mLength + 2, X_ALIGNMENT_DEFAULT);
		x_vsprintf(mString, mLength, formatString, args);
		mMaxLength = mLength;
		terminateWithZero();
	}

	xstring_buffer_tmp::xstring_buffer_tmp(const char* formatString, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(formatString);
		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
		mLength = x_vcprintf(formatString, args);
		mString = (char*)sStringTempAllocator->allocate(mLength + 2, X_ALIGNMENT_DEFAULT);
		x_vsprintf(mString, mLength, formatString, args);
		mMaxLength = mLength;
		terminateWithZero();
	}

	xstring_buffer_tmp::~xstring_buffer_tmp()
	{
		sStringTempAllocator->deallocate(mString);
		mString = NULL;
	}



	//------------------------------------------------------------------------------
	static inline char* xAllocString(s32 length, s32& outLength)
	{
		char* alloc = NULL;

		//
		// handle the allocation 
		//
		alloc = (char*)sStringHeapAllocator->allocate(length, X_ALIGNMENT_DEFAULT);
		outLength = length;

		return alloc;
	}

	//------------------------------------------------------------------------------
	static inline void xReleaseString(char* str, s32 length)
	{
		ASSERT(str);

		//
		// Handle the free
		//
		sStringHeapAllocator->deallocate(str);
	}

	//==============================================================================
	//==============================================================================
	//==============================================================================
	// xstring_buffer_heap
	//==============================================================================
	//==============================================================================
	//==============================================================================
	class xstring_buffer_heap_u
	{
	public:
		typedef					xstring_buffer_heap_object										__object;

		inline static s32		sGetRequiredBufferSize(s32 inLength)							{ return 12 + (inLength+2)*sizeof(char); }	///< Returns the required size of the buffer to hold a string of <inLen> characters including 2 x the '\0' terminator.
		inline static s32		sGreedyLength(s32 inLength)										{ return inLength*5/4 + 8; }				///< Return the allocated (greedy) length of a string of length <inLength>

		static __object*		sAlloc(s32 inLength);											///< Allocate memory for this string
		static __object*		sAllocAndSet(s32 inLength);										///< Allocate memory for this string and set length
		static __object*		sRealloc(__object* inBuffer, s32 inLength, s32 inGreedyLength);	///< Reallocate this string
		static void				sDestroy(__object* inBuffer);									///< Destroy this buffer
		static __object*		sSeperate(__object* inBuffer);									///< Make unique
		static __object*		sShrink(__object* inBuffer, s32 inGreedyLen);					///< Shrink buffer to match actual (greedy) string length. Pass -1 to let it determine greedy size based on current string length.
		static __object*		sGetEmptyStringBuffer();										///< The empty string buffer
	};

	//------------------------------------------------------------------------------

	xstring_buffer_heap_object* xstring_buffer_heap_u::sAlloc(s32 inLength)
	{
		xstring_buffer_heap_object* buffer;
		if (inLength==0)
		{
			buffer = xstring_buffer_heap_u::sGetEmptyStringBuffer();
		}
		else
		{
			s32 maxLength;
			buffer = (xstring_buffer_heap_object*)xAllocString(sGetRequiredBufferSize(inLength), maxLength);
			buffer->mRefCount = 0;
			buffer->mLength = 0;
			buffer->mMaxLength = maxLength - sGetRequiredBufferSize(0);
		}
		buffer->bind();
		return buffer;
	}

	//------------------------------------------------------------------------------

	xstring_buffer_heap_object* xstring_buffer_heap_u::sAllocAndSet(s32 inLength)
	{
		xstring_buffer_heap_object* o = sAlloc(inLength);
		o->mLength = inLength;
		return o;
	}

	//------------------------------------------------------------------------------

	xstring_buffer_heap_object* xstring_buffer_heap_u::sRealloc(xstring_buffer_heap_object* inBuffer, s32 inLength, s32 inGreedyLength)
	{
		ASSERT(inLength>0);
		ASSERT((inGreedyLength==-1) || (inLength<=inGreedyLength));

		// If the user didn't supply a greedy length then we just allocate the required size
		const s32 length = inGreedyLength == -1 ? inLength : inGreedyLength;

		// Check if our buffer is unique
		if (inBuffer->mRefCount == 1)
		{
			ASSERT(inBuffer != sGetEmptyStringBuffer());

			// Reallocate buffer
			if (inBuffer->mMaxLength < length)
			{
				s32 maxLength;
				xstring_buffer_heap_object* new_buffer = (xstring_buffer_heap_object*)xAllocString(sGetRequiredBufferSize(length), maxLength);

				// Copy entire old buffer to new buffer
				x_memcpy(new_buffer, inBuffer, sGetRequiredBufferSize(inBuffer->mLength));
				sDestroy(inBuffer);
				inBuffer = new_buffer;
				inBuffer->mMaxLength = maxLength - sGetRequiredBufferSize(0);
			}
		}
		else // Buffer is shared, must leave old untouched
		{
			s32 maxLength;
			xstring_buffer_heap_object *new_buffer = (xstring_buffer_heap_object *)xAllocString(sGetRequiredBufferSize(length), maxLength);

			// Copy entire old buffer to new buffer
			x_memcpy(new_buffer, inBuffer, sGetRequiredBufferSize(inBuffer->mLength));
			inBuffer->unbind();
			inBuffer = new_buffer;
			inBuffer->mMaxLength = maxLength - sGetRequiredBufferSize(0);
			inBuffer->mRefCount	= 1;
		}
		return inBuffer;
	}

	//------------------------------------------------------------------------------

	void xstring_buffer_heap_u::sDestroy(xstring_buffer_heap_object* inBuffer)
	{
		xReleaseString((char*)inBuffer, sGetRequiredBufferSize(inBuffer->mMaxLength));
	}


	//------------------------------------------------------------------------------


	xstring_buffer_heap_object*	xstring_buffer_heap_u::sGetEmptyStringBuffer()
	{
		static xstring_buffer_heap_object	sEmptyBuffer(1,0,0);
		static xstring_buffer_heap_object*	sEmpty = &sEmptyBuffer;
		return sEmpty;
	}

	//------------------------------------------------------------------------------

	xstring_buffer_heap_object* xstring_buffer_heap_u::sSeperate(xstring_buffer_heap_object* inBuffer)
	{
		// If we're the only one referencing the buffer, the buffer is unique
		if (inBuffer->mRefCount>1)
		{
			// Create a copy of the buffer
			const s32 length = inBuffer->mLength;
			s32 buffer_size = sGetRequiredBufferSize(length);
			s32 max_buffer_size;
			xstring_buffer_heap_object *new_buffer = (xstring_buffer_heap_object *)xAllocString(buffer_size, max_buffer_size);
			x_memcpy(new_buffer, inBuffer, buffer_size);
			inBuffer->unbind();
			inBuffer = new_buffer;
			inBuffer->mMaxLength = max_buffer_size-sGetRequiredBufferSize(0);
			inBuffer->mRefCount	= 1;
		}
		return inBuffer;
	}

	//------------------------------------------------------------------------------

	xstring_buffer_heap_object* xstring_buffer_heap_u::sShrink(xstring_buffer_heap_object* inBuffer, s32 inGreedyLen)
	{
		ASSERTS(inBuffer!=NULL, "Can only be used on existing buffers.");
		if (inBuffer==NULL)
			return inBuffer;

		const s32 greedyLength = (inGreedyLen==-1 || inGreedyLen<inBuffer->mLength) ? sGreedyLength(inBuffer->mLength) : inGreedyLen;

		s32 max_buffer_size;
		xstring_buffer_heap_object* buffer = (xstring_buffer_heap_object*)xAllocString(sGetRequiredBufferSize(greedyLength), max_buffer_size);
		x_memcpy(buffer, inBuffer, sGetRequiredBufferSize(x_intu::min(inBuffer->mLength, greedyLength)));
		sDestroy(inBuffer);
		inBuffer = buffer;
		inBuffer->mMaxLength = max_buffer_size - sGetRequiredBufferSize(0);
		return inBuffer;
	}


	//------------------------------------------------------------------------------

	void xstring_buffer_heap_object::unbind()
	{ 
		if (--mRefCount <= 0) 
			xstring_buffer_heap_u::sDestroy(this);
	}

	//------------------------------------------------------------------------------
	bool xstring_buffer_heap_object::isEmptyStringBuffer() const
	{
		return xstring_buffer_heap_u::sGetEmptyStringBuffer() == this;
	}


	//------------------------------------------------------------------------------
	xstring_buffer_heap::~xstring_buffer_heap()
	{
		mBuffer->unbind();
		mBuffer = NULL;
	}

	//------------------------------------------------------------------------------

	xstring_buffer_heap::xstring_buffer_heap()
	:mBuffer(xstring_buffer_heap_u::sGetEmptyStringBuffer())
	{
		mBuffer->bind();
	}
	//------------------------------------------------------------------------------

	xstring_buffer_heap::xstring_buffer_heap(s32 len)
	:mBuffer(xstring_buffer_heap_u::sAlloc(len))
	{
	}

	//------------------------------------------------------------------------------

	xstring_buffer_heap::xstring_buffer_heap(s32 len, const char* str)
	:mBuffer(xstring_buffer_heap_u::sAllocAndSet(len))
	{
		x_memcpy(getPointer(), str , len * sizeof(char));
		terminateWithZero();
	}

	//------------------------------------------------------------------------------

	xstring_buffer_heap::xstring_buffer_heap(s32 lenA, const char* strA, s32 lenB, const char* strB)
	:mBuffer(xstring_buffer_heap_u::sAllocAndSet(lenA+lenB))
	{
		char* p = getPointer(); 
		x_memcpy(p     , strA, lenA * sizeof(char));
		x_memcpy(p+lenA, strB, lenB * sizeof(char));
		terminateWithZero();
	}

	//------------------------------------------------------------------------------

	xstring_buffer_heap::xstring_buffer_heap(const char* formatString, const x_va_list& args)
	{
		xstring_tmp tmp(formatString, args);

		mBuffer = xstring_buffer_heap_u::sAllocAndSet(tmp.getLength());
		x_memcpy(getPointer(), tmp.c_str(), tmp.getLength()); 
		terminateWithZero();
	}

	//------------------------------------------------------------------------------

	xstring_buffer_heap::xstring_buffer_heap(const char* formatString, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(formatString);
		xstring_tmp tmp(formatString, v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);

		mBuffer = xstring_buffer_heap_u::sAllocAndSet(tmp.getLength());
		x_memcpy(getPointer(), tmp.c_str(), tmp.getLength()); 
		terminateWithZero();
	}

	//------------------------------------------------------------------------------

	void xstring_buffer_heap::resize(s32 inLength)
	{
		if (mBuffer!=NULL)
		{
			mBuffer = xstring_buffer_heap_u::sRealloc(mBuffer, inLength, xstring_buffer_heap_u::sGreedyLength(inLength));
		}
		else
		{
			mBuffer = xstring_buffer_heap_u::sAlloc(inLength);
		}
	}

	//------------------------------------------------------------------------------

	void xstring_buffer_heap::unique()
	{
		mBuffer = xstring_buffer_heap_u::sSeperate(mBuffer);
	}

	//------------------------------------------------------------------------------

	void xstring_buffer_heap::setEmpty()
	{
		mBuffer = xstring_buffer_heap_u::sGetEmptyStringBuffer();
		mBuffer->bind();
	}

//==============================================================================
// END xCore namespace
//==============================================================================
};
