/**
* @file x_va_list.cpp
* x base variable argument
*/

#include "xbase\x_debug.h"
#include "xbase\x_string_utf.h"
#include "xbase\x_va_list.h"

/**
 * xCore namespace
 */
namespace xcore
{
	const x_va	x_va::sEmpty;

	s8			x_va::convertToInt8() const
	{
		u32 i = convertToUInt32();
		return (s8)i;
	}

	u8			x_va::convertToUInt8() const
	{
		u32 i = convertToUInt32();
		return (u8)i;
	}

	s16			x_va::convertToInt16() const
	{
		u32 i = convertToUInt32();
		return (s16)i;
	}

	u16			x_va::convertToUInt16() const
	{
		u32 i = convertToUInt32();
		return (u16)i;
	}

	s32			x_va::convertToInt32() const
	{
		u32 i = convertToUInt32();
		return (s32)i;
	}

	u32			x_va::convertToUInt32() const
	{
		u32 i = 0;
		switch (mType)
		{
			case TYPE_BOOL:
			case TYPE_UCHAR8:
			//case TYPE_UINT:
			//case TYPE_INT:		{ i = (u32)(*(u32*)mArg); } break;
			case TYPE_UINT32:
			case TYPE_INT32:	{ i = (u32)(*(u32*)mArg); } break;

			case TYPE_UINT8:
			case TYPE_INT8:		{ i = (u32)(*(u8*)mArg);  } break;
			case TYPE_UINT16:
			case TYPE_INT16:	{ i = (u32)(*(u16*)mArg); } break;
			case TYPE_UINT64:
			case TYPE_INT64:	{ i = (u32)(*(u64*)mArg); } break;
			case TYPE_FLOAT32:	{ i = (u32)(*(f32*)mArg); } break;
			case TYPE_FLOAT64:	{ i = (u32)(*(f64*)mArg); } break;
			case TYPE_PCTCHAR:
			case TYPE_PCUSTR8:
			default:			break; // Fall through
		};

		return i;
	}

	s64			x_va::convertToInt64() const
	{
		u64 i = convertToUInt64();
		return (s64)i;
	}

	u64			x_va::convertToUInt64() const
	{
		u64 i = 0;
		switch (mType)
		{
			case TYPE_BOOL:
			case TYPE_UCHAR8:
			//case TYPE_UINT:
			//case TYPE_INT:		{ i = (u64)(*(u32*)mArg);  } break;
			case TYPE_UINT32:
			case TYPE_INT32:	{ i = (u64)(*(u32*)mArg);  } break;

			case TYPE_UINT8:
			case TYPE_INT8:		{ i = (u64)(*(u8*)mArg);   } break;
			case TYPE_UINT16:
			case TYPE_INT16:	{ i = (u64)(*(u16*)mArg);  } break;
			case TYPE_UINT64:
			case TYPE_INT64:	{ i = (u64)(*(u64*)mArg);  } break;
			case TYPE_FLOAT32:	{ i = (u64)(*(f32*)mArg);  } break;
			case TYPE_FLOAT64:	{ i = (u64)(*(f64*)mArg);  } break;
			case TYPE_PCTCHAR:
			case TYPE_PCUSTR8:
			default:			break; // Fall through
		};

		return i;
	}

	f32			x_va::convertToFloat() const
	{
		f32 i = 0.0f;
		switch (mType)
		{
			case TYPE_BOOL:
			case TYPE_UCHAR8:
			//case TYPE_UINT:
			//case TYPE_INT:		{ i = (f32)(*(u32*)mArg); } break;
			case TYPE_UINT32:
			case TYPE_INT32:	{ i = (f32)(*(u32*)mArg); } break;

			case TYPE_UINT8:
			case TYPE_INT8:		{ i = (f32)(*(u8*)mArg);  } break;
			case TYPE_UINT16:
			case TYPE_INT16:	{ i = (f32)(*(u16*)mArg); } break;
			case TYPE_UINT64:
			case TYPE_INT64:	{ i = (f32)(*(u64*)mArg); } break;

			case TYPE_FLOAT32:	{ i = (f32)(*(f32*)mArg); } break;
			case TYPE_FLOAT64:	{ i = (f32)(*(f64*)mArg); } break;
			case TYPE_PCTCHAR:
			case TYPE_PCUSTR8:
			default:			break; // Fall through
		};

		return i;
	}

	f64			x_va::convertToDouble() const
	{
		f64 i = 0.0;
		switch (mType)
		{
			case TYPE_BOOL:
			case TYPE_UCHAR8:
			//case TYPE_UINT:
			//case TYPE_INT:		{ i = (f64)(*(u32*)mArg); } break;
			case TYPE_UINT32:
			case TYPE_INT32:	{ i = (f64)(*(u32*)mArg); } break;

			case TYPE_UINT8:
			case TYPE_INT8:		{ i = (f64)(*(u8*)mArg);  } break;
			case TYPE_UINT16:
			case TYPE_INT16:	{ i = (f64)(*(u16*)mArg); } break;
			case TYPE_UINT64:
			case TYPE_INT64:	{ i = (f64)(*(u64*)mArg); } break;

			case TYPE_FLOAT32:	{ i = (f64)(*(f32*)mArg); } break;
			case TYPE_FLOAT64:	{ i = (f64)(*(f64*)mArg); } break;

			case TYPE_PCTCHAR:
			case TYPE_PCUSTR8:
			default:			break; // Fall through
		};

		return i;
	}

	bool			x_va::convertToBool() const
	{
		u32 i = 0;
		switch (mType)
		{
			case TYPE_BOOL:
			case TYPE_UCHAR8:
			//case TYPE_UINT:
			//case TYPE_INT:		{ i = (u32)(*(u32*)mArg); } break;
			case TYPE_UINT32:
			case TYPE_INT32:	{ i = (u32)(*(u32*)mArg); } break;

			case TYPE_UINT8:
			case TYPE_INT8:		{ i = (u32)(*(u8*)mArg);  } break;
			case TYPE_UINT16:
			case TYPE_INT16:	{ i = (u32)(*(u16*)mArg); } break;
			case TYPE_UINT64:
			case TYPE_INT64:	{ i = (u32)(*(u64*)mArg); } break;
			case TYPE_FLOAT32:	{ i = (u32)(*(f32*)mArg); } break;
			case TYPE_FLOAT64:	{ i = (u32)(*(f64*)mArg); } break;

#ifdef TARGET_32BIT
			case TYPE_PCTCHAR:
			case TYPE_PCUSTR8:	{ i = (u32)(*(u32*)mArg); } break;
#else
			case TYPE_PCTCHAR:
			case TYPE_PCUSTR8:	{ i = (u32)(*(u64*)mArg); } break;
#endif
			default:			break; // Fall through
		};

		return i != 0;
	}

	uchar8			x_va::convertToUchar8() const
	{
		uchar8 ch((u32)'?');
		switch (mType)
		{
			case TYPE_BOOL:		{ ch.c = (*(u32*)mArg)!=0 ? (u32)'y' : (u32)'n'; } break;
			case TYPE_UINT32:
			case TYPE_INT32:	
			case TYPE_UCHAR8:	{ ch.c = (u32)(*(u32*)mArg); } break;

			case TYPE_UINT8:
			case TYPE_INT8:		{ ch.c = (u8)(*(u8*)mArg); } break;
			case TYPE_UINT16:
			case TYPE_INT16:	{ ch.c = (u16)(*(u16*)mArg); } break;
			case TYPE_UINT64:
			case TYPE_INT64:	{ ch.c = (u32)(*(u64*)mArg); } break;

			case TYPE_FLOAT32:	
			case TYPE_FLOAT64:

			case TYPE_PCTCHAR:
			case TYPE_PCUSTR8:
			default:			break; // Fall through
		};

		if (false == xcore::utf::isLegal(ch))
			ch.c = '?';

		return ch;
	}

	const char*			x_va::convertToCharPointer() const
	{
		switch (mType)
		{
			case TYPE_PCTCHAR:
				{
					const char* p = *(const char**)mArg; 
					return p;
				}

			default:			break; // Fall through
		};

		return "?";
	}

	const ustr8*		x_va::convertToUStr8Pointer() const
	{
		switch (mType)
		{
			case TYPE_PCUSTR8:
			case TYPE_PCTCHAR:
				{
					const ustr8* p = *(const ustr8**)mArg; 
					return p;
				}

			default:			break; // Fall through
		};

		return (const ustr8*)"?";
	}

	x_va_r	x_va_r::sEmpty;


	x_va_r&					x_va_r::operator=(s8 rhs)
	{
		switch (mType)
		{
			case TYPE_BOOL:		*((xbool*)mRef) = rhs!=0 ? xTRUE : xFALSE; break;
			//case TYPE_UINT:		*((u32*)mRef) = rhs; break;
			//case TYPE_INT:		*((s32*)mRef) = rhs; break;
			case TYPE_UINT32:	*((u32*)mRef) = rhs; break;
			case TYPE_INT32:	*((s32*)mRef) = rhs; break;
			case TYPE_UINT8:	*((u8*)mRef) = rhs; break;
			case TYPE_INT8:		*((s8*)mRef) = rhs; break;
			case TYPE_UINT16:	*((u16*)mRef) = rhs; break;
			case TYPE_INT16:	*((s16*)mRef) = rhs; break;
			case TYPE_UINT64:	*((u64*)mRef) = rhs; break;
			case TYPE_INT64:	*((s64*)mRef) = rhs; break;
			case TYPE_FLOAT32:	*((f32*)mRef) = (f32)rhs; break;
			case TYPE_FLOAT64:	*((f64*)mRef) = (f64)rhs; break;
			default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(u8 rhs)
	{
		*this = (s8)rhs;
		return *this;
	}

	x_va_r&					x_va_r::operator=(s16 rhs)
	{
		switch (mType)
		{
			case TYPE_BOOL:		*((xbool*)mRef) = rhs!=0 ? xTRUE : xFALSE; break;
			case TYPE_UCHAR8:	*((uchar8*)mRef) = uchar8((u32)'?'); break;
			case TYPE_UINT32:	*((u32*)mRef) = rhs; break;
			case TYPE_INT32:	*((s32*)mRef) = rhs; break;
			case TYPE_UINT8:	*((u8*)mRef) = (u8)rhs; break;
			case TYPE_INT8:		*((s8*)mRef) = (s8)rhs; break;
			case TYPE_UINT16:	*((u16*)mRef) = rhs; break;
			case TYPE_INT16:	*((s16*)mRef) = rhs; break;
			case TYPE_UINT64:	*((u64*)mRef) = rhs; break;
			case TYPE_INT64:	*((s64*)mRef) = rhs; break;
			case TYPE_FLOAT32:	*((f32*)mRef) = (f32)rhs; break;
			case TYPE_FLOAT64:	*((f64*)mRef) = (f64)rhs; break;
			default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(u16 rhs)
	{
		*this = (s16)rhs;
		return *this;
	}

	x_va_r&					x_va_r::operator=(s32 rhs)
	{
		switch (mType)
		{
			case TYPE_BOOL:		*((xbool*)mRef) = rhs!=0 ? xTRUE : xFALSE; break;
			case TYPE_UCHAR8:	*((uchar8*)mRef) = uchar8((u32)'?'); break;
			case TYPE_UINT32:	*((u32*)mRef) = rhs; break;
			case TYPE_INT32:	*((s32*)mRef) = rhs; break;
			case TYPE_UINT8:	*((u8*)mRef) = (u8)rhs; break;
			case TYPE_INT8:		*((s8*)mRef) = (s8)rhs; break;
			case TYPE_UINT16:	*((u16*)mRef) = (u16)rhs; break;
			case TYPE_INT16:	*((s16*)mRef) = (s16)rhs; break;
			case TYPE_UINT64:	*((u64*)mRef) = rhs; break;
			case TYPE_INT64:	*((s64*)mRef) = rhs; break;
			case TYPE_FLOAT32:	*((f32*)mRef) = (f32)rhs; break;
			case TYPE_FLOAT64:	*((f64*)mRef) = (f64)rhs; break;
			default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(u32 rhs)
	{
		*this = (s32)rhs;
		return *this;
	}

	x_va_r&					x_va_r::operator=(s64 rhs)
	{
		switch (mType)
		{
			case TYPE_BOOL:		*((xbool*)mRef) = rhs!=0 ? xTRUE : xFALSE; break;
			case TYPE_UCHAR8:	*((uchar8*)mRef) = uchar8((u32)'?'); break;
			case TYPE_UINT32:	*((u32*)mRef) = (u32)rhs; break;
			case TYPE_INT32:	*((s32*)mRef) = (s32)rhs; break;
			case TYPE_UINT8:	*((u8*)mRef) = (u8)rhs; break;
			case TYPE_INT8:		*((s8*)mRef) = (s8)rhs; break;
			case TYPE_UINT16:	*((u16*)mRef) = (u16)rhs; break;
			case TYPE_INT16:	*((s16*)mRef) = (s16)rhs; break;
			case TYPE_UINT64:	*((u64*)mRef) = rhs; break;
			case TYPE_INT64:	*((s64*)mRef) = rhs; break;
			case TYPE_FLOAT32:	*((f32*)mRef) = (f32)rhs; break;
			case TYPE_FLOAT64:	*((f64*)mRef) = (f64)rhs; break;
			default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(u64 rhs)
	{
		*this = (s64)rhs;
		return *this;
	}

	x_va_r&					x_va_r::operator=(f32 rhs)
	{
		switch (mType)
		{
			case TYPE_BOOL:		*((xbool*)mRef) = rhs!=0 ? xTRUE : xFALSE; break;
			case TYPE_UCHAR8:	*((uchar8*)mRef) = uchar8((u32)'?'); break;
			case TYPE_UINT32:	*((u32*)mRef) = (u32)rhs; break;
			case TYPE_INT32:	*((s32*)mRef) = (s32)rhs; break;
			case TYPE_UINT8:	*(( u8*)mRef) = ( u8)rhs; break;
			case TYPE_INT8:		*(( s8*)mRef) = ( s8)rhs; break;
			case TYPE_UINT16:	*((u16*)mRef) = (u16)rhs; break;
			case TYPE_INT16:	*((s16*)mRef) = (s16)rhs; break;
			case TYPE_UINT64:	*((u64*)mRef) = (u64)rhs; break;
			case TYPE_INT64:	*((s64*)mRef) = (s64)rhs; break;
			case TYPE_FLOAT32:	*((f32*)mRef) = (f32)rhs; break;
			case TYPE_FLOAT64:	*((f64*)mRef) = (f64)rhs; break;
			default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(f64 rhs)
	{
		switch (mType)
		{
			case TYPE_BOOL:		*((xbool*)mRef) = rhs!=0 ? xTRUE : xFALSE; break;
			case TYPE_UCHAR8:	*((uchar8*)mRef) = uchar8((u32)'?'); break;
			case TYPE_UINT32:	*((u32*)mRef) = (u32)rhs; break;
			case TYPE_INT32:	*((s32*)mRef) = (s32)rhs; break;
			case TYPE_UINT8:	*(( u8*)mRef) = ( u8)rhs; break;
			case TYPE_INT8:		*(( s8*)mRef) = ( s8)rhs; break;
			case TYPE_UINT16:	*((u16*)mRef) = (u16)rhs; break;
			case TYPE_INT16:	*((s16*)mRef) = (s16)rhs; break;
			case TYPE_UINT64:	*((u64*)mRef) = (u64)rhs; break;
			case TYPE_INT64:	*((s64*)mRef) = (s64)rhs; break;
			case TYPE_FLOAT32:	*((f32*)mRef) = (f32)rhs; break;
			case TYPE_FLOAT64:	*((f64*)mRef) = (f64)rhs; break;
			default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(bool rhs)
	{
		switch (mType)
		{
			case TYPE_BOOL:		*((xbool*)mRef) = xbool(rhs); break;
			case TYPE_UCHAR8:	*((uchar8*)mRef) = uchar8((u32)'?'); break;
			case TYPE_UINT32:	*((u32*)mRef) = rhs ? 1 : 0; break;
			case TYPE_INT32:	*((s32*)mRef) = rhs ? 1 : 0; break;
			case TYPE_UINT8:	*((u8*)mRef) = rhs ? 1 : 0; break;
			case TYPE_INT8:		*((s8*)mRef) = rhs ? 1 : 0; break;
			case TYPE_UINT16:	*((u16*)mRef) = rhs ? 1 : 0; break;
			case TYPE_INT16:	*((s16*)mRef) = rhs ? 1 : 0; break;
			case TYPE_UINT64:	*((u64*)mRef) = rhs ? 1 : 0; break;
			case TYPE_INT64:	*((s64*)mRef) = rhs ? 1 : 0; break;
			case TYPE_FLOAT32:	*((f32*)mRef) = rhs ? 1.0f : 0.0f; break;
			case TYPE_FLOAT64:	*((f64*)mRef) = rhs ? 1.0 : 0.0; break;
			default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(uchar8 rhs)
	{
		switch (mType)
		{
			case TYPE_BOOL:		*((xbool*)mRef) = rhs.c ? 1 : 0; break;
			case TYPE_UINT32:	*((u32*)mRef) = rhs.c; break;
			case TYPE_INT32:	*((s32*)mRef) = (s32)rhs.c; break;
			case TYPE_UINT8:	*((u8*)mRef)  = (u8 )rhs.c; break;
			case TYPE_INT8:		*((s8*)mRef)  = (s8 )rhs.c; break;
			case TYPE_UINT16:	*((u16*)mRef) = (u16)rhs.c; break;
			case TYPE_INT16:	*((s16*)mRef) = (s16)rhs.c; break;
			case TYPE_UINT64:	*((u64*)mRef) = (u32)rhs.c; break;
			case TYPE_INT64:	*((s64*)mRef) = (u32)rhs.c; break;
			case TYPE_FLOAT32:	*((f32*)mRef) = (f32)rhs.c; break;
			case TYPE_FLOAT64:	*((f64*)mRef) = (f64)rhs.c; break;
			default:			break; // Fall through
		};

		return *this;
	}

//==============================================================================
// END xCore namespace
//==============================================================================
};
/**
 *  END xCore namespace
 */