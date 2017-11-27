/**
* @file x_va_list.cpp
* x base variable argument
*/

#include "xbase/x_debug.h"
#include "xbase/x_va_list.h"
#include "xbase/x_string_utf.h"
#include "xbase/x_string_ascii.h"


/**
* string utilities
*/
#include "xbase/private/x_string_utils.h"

/**
 * xCore namespace
 */
namespace xcore
{
	const x_va	x_va::sEmpty;


	void		x_va::convertToTCharStr(char* str, char* end) const
	{
		u32 i = 0;
		switch (mType)
		{
		case TYPE_BOOL: { bool v = (*(bool*)mArg); ascii::to_string(str, end, end, v); } break;
		case TYPE_UCHAR: { uchar32 c = (*(uchar32*)mArg); utf::write(c, str); } break;
		case TYPE_UINT32: { u32 v = (*(u32*)mArg); ascii::to_string(str, end, end, v); } break;
		case TYPE_INT32: { s32 v = (*(u32*)mArg); ascii::to_string(str, end, end, v); } break;

		case TYPE_UINT8:
		case TYPE_INT8: { s32 v = (*(s8*)mArg); ascii::to_string(str, end, end, v); } break;
		case TYPE_UINT16:
		case TYPE_INT16: { s32 v = (*(s16*)mArg); ascii::to_string(str, end, end, v); } break;
		case TYPE_UINT64:
		case TYPE_INT64: { s64 v = (*(s64*)mArg); ascii::to_string(str, end, end, v); } break;
		case TYPE_FLOAT32: { f32 v = (*(f32*)mArg); ascii::to_string(str, end, end, v); } break;
		case TYPE_FLOAT64: { f64 v = (*(f64*)mArg); ascii::to_string(str, end, end, v); } break;
		case TYPE_PCTCHAR: { ascii::pcrune sstr = *(ascii::pcrune*)mArg; ascii::pcrune send = *(ascii::pcrune*)mArg2; utf::copy(sstr, send, str, end, utf::TERMINATOR_WRITE); } break;
		case TYPE_PCUCHAR8: { utf8::pcrune sstr = *(utf8::pcrune*)mArg; utf8::pcrune send = *(utf8::pcrune*)mArg2; utf::copy(sstr, send, str, end, utf::TERMINATOR_WRITE); } break;
		case TYPE_PCUCHAR32: { utf32::pcrune sstr = *(utf32::pcrune*)mArg; utf32::pcrune send = *(utf32::pcrune*)mArg2; utf::copy(sstr, send, str, end, utf::TERMINATOR_WRITE); } break;
		default:
			break; // Fall through
		};
	}

	void		x_va::convertToUChar8Str(uchar8* str, uchar8* end) const
	{
		u32 i = 0;
		switch (mType)
		{
		case TYPE_BOOL: { bool v = (*(bool*)mArg); utf8::to_string(str, end, end, v); } break;
		case TYPE_UCHAR: { uchar32 c = (*(uchar32*)mArg); utf::write(c, str); } break;
		case TYPE_UINT32: { u32 v = (*(u32*)mArg); utf8::to_string(str, end, end, v); } break;
		case TYPE_INT32: { s32 v = (*(u32*)mArg); utf8::to_string(str, end, end, v); } break;

		case TYPE_UINT8:
		case TYPE_INT8: { s32 v = (*(s8*)mArg); utf8::to_string(str, end, end, v); } break;
		case TYPE_UINT16:
		case TYPE_INT16: { s32 v = (*(s16*)mArg); utf8::to_string(str, end, end, v); } break;
		case TYPE_UINT64:
		case TYPE_INT64: { s64 v = (*(s64*)mArg); utf8::to_string(str, end, end, v); } break;
		case TYPE_FLOAT32: { f32 v = (*(f32*)mArg); utf8::to_string(str, end, end, v); } break;
		case TYPE_FLOAT64: { f64 v = (*(f64*)mArg); utf8::to_string(str, end, end, v); } break;
		case TYPE_PCTCHAR: { ascii::pcrune sstr = *(ascii::pcrune*)mArg; ascii::pcrune send = *(ascii::pcrune*)mArg2; utf::copy(sstr, send, str, end, utf::TERMINATOR_WRITE); } break;
		case TYPE_PCUCHAR8: { utf8::pcrune sstr = *(utf8::pcrune*)mArg; utf8::pcrune send = *(utf8::pcrune*)mArg2; utf::copy(sstr, send, str, end, utf::TERMINATOR_WRITE); } break;
		case TYPE_PCUCHAR32: { utf32::pcrune sstr = *(utf32::pcrune*)mArg; utf32::pcrune send = *(utf32::pcrune*)mArg2; utf::copy(sstr, send, str, end, utf::TERMINATOR_WRITE); } break;
		default:
			break; // Fall through
		};
	}
	
	void		x_va::convertToUChar32Str(uchar32* str, uchar32* end) const
	{
		u32 i = 0;
		switch (mType)
		{
		case TYPE_BOOL: { bool v = (*(bool*)mArg); utf32::to_string(str, end, end, v); } break;
		case TYPE_UCHAR: { uchar32 c = (*(uchar32*)mArg); utf::write(c, str); } break;
		case TYPE_UINT32: { u32 v = (*(u32*)mArg); utf32::to_string(str, end, end, v); } break;
		case TYPE_INT32: { s32 v = (*(u32*)mArg); utf32::to_string(str, end, end, v); } break;

		case TYPE_UINT8:
		case TYPE_INT8: { s32 v = (*(s8*)mArg); utf32::to_string(str, end, end, v); } break;
		case TYPE_UINT16:
		case TYPE_INT16: { s32 v = (*(s16*)mArg); utf32::to_string(str, end, end, v); } break;
		case TYPE_UINT64:
		case TYPE_INT64: { s64 v = (*(s64*)mArg); utf32::to_string(str, end, end, v); } break;
		case TYPE_FLOAT32: { f32 v = (*(f32*)mArg); utf32::to_string(str, end, end, v); } break;
		case TYPE_FLOAT64: { f64 v = (*(f64*)mArg); utf32::to_string(str, end, end, v); } break;
		case TYPE_PCTCHAR: { ascii::pcrune sstr = *(ascii::pcrune*)mArg; ascii::pcrune send = *(ascii::pcrune*)mArg2; utf::copy(sstr, send, str, end, utf::TERMINATOR_WRITE); } break;
		case TYPE_PCUCHAR8: { utf8::pcrune sstr = *(utf8::pcrune*)mArg; utf8::pcrune send = *(utf8::pcrune*)mArg2; utf::copy(sstr, send, str, end, utf::TERMINATOR_WRITE); } break;
		case TYPE_PCUCHAR32: { utf32::pcrune sstr = *(utf32::pcrune*)mArg; utf32::pcrune send = *(utf32::pcrune*)mArg2; utf::copy(sstr, send, str, end, utf::TERMINATOR_WRITE); } break;
		default:
			break; // Fall through
		};
	}

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
		case TYPE_UCHAR:
			//case TYPE_UINT:
			//case TYPE_INT:		{ i = (u32)(*(u32*)mArg); } break;
		case TYPE_UINT32:
		case TYPE_INT32: { i = (u32)(*(u32*)mArg); } break;

		case TYPE_UINT8:
		case TYPE_INT8: { i = (u32)(*(u8*)mArg);  } break;
		case TYPE_UINT16:
		case TYPE_INT16: { i = (u32)(*(u16*)mArg); } break;
		case TYPE_UINT64:
		case TYPE_INT64: { i = (u32)(*(u64*)mArg); } break;
		case TYPE_FLOAT32: { i = (u32)(*(f32*)mArg); } break;
		case TYPE_FLOAT64: { i = (u32)(*(f64*)mArg); } break;
		case TYPE_PCTCHAR: { ascii::pcrune str = *(ascii::pcrune*)mArg; ascii::pcrune end = *(ascii::pcrune*)mArg2; i = StrToU32(str, end); } break;
		case TYPE_PCUCHAR8: { utf8::pcrune str = *(utf8::pcrune*)mArg; utf8::pcrune end = *(utf8::pcrune*)mArg2; i = StrToU32(str, end); } break;
		case TYPE_PCUCHAR32: { utf32::pcrune str = *(utf32::pcrune*)mArg; utf32::pcrune end = *(utf32::pcrune*)mArg2; i = StrToU32(str, end); } break;
		default:
			break; // Fall through
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
			case TYPE_UCHAR:
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
			case TYPE_PCTCHAR: { ascii::pcrune str = *(ascii::pcrune*)mArg; ascii::pcrune end = *(ascii::pcrune*)mArg2; i = StrToS64(str, end, 10); } break;
			case TYPE_PCUCHAR8: { utf8::pcrune str = *(utf8::pcrune*)mArg; utf8::pcrune end = *(utf8::pcrune*)mArg2; i = StrToS64(str, end, 10); } break;
			case TYPE_PCUCHAR32: { utf32::pcrune str = *(utf32::pcrune*)mArg; utf32::pcrune end = *(utf32::pcrune*)mArg2; i = StrToS64(str, end, 10); } break;
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
			case TYPE_UCHAR:
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
			case TYPE_PCTCHAR: { ascii::pcrune str = *(ascii::pcrune*)mArg; ascii::pcrune end = *(ascii::pcrune*)mArg2; i = StrToF32(str, end); } break;
			case TYPE_PCUCHAR8: { utf8::pcrune str = *(utf8::pcrune*)mArg; utf8::pcrune end = *(utf8::pcrune*)mArg2; i = StrToF32(str, end); } break;
			case TYPE_PCUCHAR32: { utf32::pcrune str = *(utf32::pcrune*)mArg; utf32::pcrune end = *(utf32::pcrune*)mArg2; i = StrToF32(str, end); } break;
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
			case TYPE_UCHAR:
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

			case TYPE_PCTCHAR: { ascii::pcrune str = *(ascii::pcrune*)mArg; ascii::pcrune end = *(ascii::pcrune*)mArg2; i = StrToF64(str, end); } break;
			case TYPE_PCUCHAR8: { utf8::pcrune str = *(utf8::pcrune*)mArg; utf8::pcrune end = *(utf8::pcrune*)mArg2; i = StrToF64(str, end); } break;
			case TYPE_PCUCHAR32: { utf32::pcrune str = *(utf32::pcrune*)mArg; utf32::pcrune end = *(utf32::pcrune*)mArg2; i = StrToF64(str, end); } break;
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
			case TYPE_UCHAR:
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

			case TYPE_PCTCHAR: { ascii::pcrune str = *(ascii::pcrune*)mArg; ascii::pcrune end = *(ascii::pcrune*)mArg2; i = StrToBool(str, end); } break;
			case TYPE_PCUCHAR8: { utf8::pcrune str = *(utf8::pcrune*)mArg; utf8::pcrune end = *(utf8::pcrune*)mArg2; i = StrToBool(str, end); } break;
			case TYPE_PCUCHAR32: { utf32::pcrune str = *(utf32::pcrune*)mArg; utf32::pcrune end = *(utf32::pcrune*)mArg2; i = StrToBool(str, end); } break;

			default:			break; // Fall through
		};

		return i != 0;
	}

	uchar32			x_va::convertToUChar() const
	{
		uchar32 ch((u32)'?');
		switch (mType)
		{
			case TYPE_BOOL:		{ ch = (*(u32*)mArg)!=0 ? (u32)'y' : (u32)'n'; } break;
			case TYPE_UINT32:
			case TYPE_INT32:	
			case TYPE_UCHAR:	{ ch = (uchar32)(*(uchar*)mArg); } break;

			case TYPE_UINT8:
			case TYPE_INT8:		{ ch = (u8)(*(u8*)mArg); } break;
			case TYPE_UINT16:
			case TYPE_INT16:	{ ch = (u16)(*(u16*)mArg); } break;
			case TYPE_UINT64:
			case TYPE_INT64:	{ ch = (u32)(*(u64*)mArg); } break;

			case TYPE_FLOAT32:	
			case TYPE_FLOAT64:

			case TYPE_PCTCHAR: { utf::read(*(ascii::rune**)mArg, ch); } break;
			case TYPE_PCUCHAR8: { utf::read(*(utf8::rune**)mArg, ch); } break;
			case TYPE_PCUCHAR32: { utf::read(*(utf32::rune**)mArg, ch); } break;
			default:			break; // Fall through
		};

		return ch;
	}

	const uchar*		x_va::convertToUCharPointer() const
	{
		switch (mType)
		{
			case TYPE_PCTCHAR:
				{
					const uchar* p = *(const uchar**)mArg; 
					return p;
				}

			default:			break; // Fall through
		};

		return (const uchar*)"?";
	}

	const uchar8*		x_va::convertToUChar8Pointer() const
	{
		switch (mType)
		{
			case TYPE_PCUCHAR8:
			case TYPE_PCTCHAR:
				{
					const uchar8* p = *(const uchar8**)mArg; 
					return p;
				}

			default:			break; // Fall through
		};

		return (const uchar8*)"?";
	}

	const uchar32*		x_va::convertToUChar32Pointer() const
	{
		switch (mType)
		{
		case TYPE_PCUCHAR8:
		{
			const uchar32* p = *(const uchar32**)mArg;
			return p;
		}

		default:			break; // Fall through
		};

		return (const uchar32*)"?\0\0\0";
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
			case TYPE_UCHAR:	*((uchar*)mRef) = uchar((u32)'?'); break;
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
			case TYPE_UCHAR:	*((uchar*)mRef) = uchar((u32)'?'); break;
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
			case TYPE_UCHAR:	*((uchar*)mRef) = uchar((u32)'?'); break;
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
			case TYPE_UCHAR:	*((uchar*)mRef) = uchar((u32)'?'); break;
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
			case TYPE_UCHAR:	*((uchar*)mRef) = uchar((u32)'?'); break;
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

	x_va_r&					x_va_r::operator=(uchar32 rhs)
	{
		switch (mType)
		{
		case TYPE_BOOL:		*((xbool*)mRef) = rhs != 0 ? xTRUE : xFALSE; break;
		case TYPE_UCHAR:	*((uchar*)mRef) = (uchar)rhs; break;
		case TYPE_UINT32:	*((u32*)mRef) = (u32)rhs; break;
		case TYPE_INT32:	*((s32*)mRef) = (s32)rhs; break;
		case TYPE_UINT8:	*((u8*)mRef) = (u8)rhs; break;
		case TYPE_INT8:		*((s8*)mRef) = (s8)rhs; break;
		case TYPE_UINT16:	*((u16*)mRef) = (u16)rhs; break;
		case TYPE_INT16:	*((s16*)mRef) = (s16)rhs; break;
		case TYPE_UINT64:	*((u64*)mRef) = (u64)rhs; break;
		case TYPE_INT64:	*((s64*)mRef) = (s64)rhs; break;
		case TYPE_FLOAT32:	*((f32*)mRef) = (f32)rhs; break;
		case TYPE_FLOAT64:	*((f64*)mRef) = (f64)rhs; break;
		case TYPE_PTCHAR:    utf::write(rhs, ((uchar*)mRef)); break;
		case TYPE_PUCHAR8:   utf::write(rhs, ((uchar8*)mRef)); break;
		case TYPE_PUCHAR32:  ((uchar32*)mRef)[0] = rhs; break;
		default:			break;
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(bool rhs)
	{
		switch (mType)
		{
			case TYPE_BOOL:		*((xbool*)mRef) = xbool(rhs); break;
			case TYPE_UCHAR:	*((uchar*)mRef) = '?'; break;
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

	x_va_r&					x_va_r::operator=(const uchar* rhs)
	{
		switch (mType)
		{
		case TYPE_BOOL:		*((bool*)mRef) = StrToBool(rhs, NULL); break;
		case TYPE_UCHAR:	*((uchar8*)mRef) = (uchar8)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT32:	*((u32*)mRef) = (u32)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT32:	*((s32*)mRef) = (s32)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT8:	*((u8 *)mRef) = (u8 )StrToS64(rhs, NULL, 10); break;
		case TYPE_INT8:		*((s8 *)mRef) = (s8 )StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT16:	*((u16*)mRef) = (u16)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT16:	*((s16*)mRef) = (s16)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT64:	*((u64*)mRef) = (u64)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT64:	*((s64*)mRef) = (s64)StrToS64(rhs, NULL, 10); break;
		case TYPE_FLOAT32:	*((f32*)mRef) = (f32)StrToF32(rhs, NULL); break;
		case TYPE_FLOAT64:	*((f64*)mRef) = (f64)StrToF64(rhs, NULL); break;
		case TYPE_PTCHAR:	break;
		case TYPE_PUCHAR8:	break;
		case TYPE_PUCHAR32:	break;
		default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(const uchar8* rhs)
	{
		switch (mType)
		{
		case TYPE_BOOL:		*((bool*)mRef) = StrToBool(rhs, NULL); break;
		case TYPE_UCHAR:	*((uchar8*)mRef) = (uchar8)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT32:	*((u32*)mRef) = (u32)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT32:	*((s32*)mRef) = (s32)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT8:	*((u8 *)mRef) = (u8)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT8:		*((s8 *)mRef) = (s8)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT16:	*((u16*)mRef) = (u16)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT16:	*((s16*)mRef) = (s16)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT64:	*((u64*)mRef) = (u64)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT64:	*((s64*)mRef) = (s64)StrToS64(rhs, NULL, 10); break;
		case TYPE_FLOAT32:	*((f32*)mRef) = (f32)StrToF32(rhs, NULL); break;
		case TYPE_FLOAT64:	*((f64*)mRef) = (f64)StrToF64(rhs, NULL); break;
		case TYPE_PTCHAR:	break;
		case TYPE_PUCHAR8:	break;
		case TYPE_PUCHAR32:	break;
		default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(const uchar32* rhs)
	{
		switch (mType)
		{
		case TYPE_BOOL:		*((bool*)mRef) = StrToBool(rhs, NULL); break;
		case TYPE_UCHAR:	*((uchar8*)mRef) = (uchar8)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT32:	*((u32*)mRef) = (u32)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT32:	*((s32*)mRef) = (s32)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT8:	*((u8 *)mRef) = (u8)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT8:		*((s8 *)mRef) = (s8)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT16:	*((u16*)mRef) = (u16)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT16:	*((s16*)mRef) = (s16)StrToS64(rhs, NULL, 10); break;
		case TYPE_UINT64:	*((u64*)mRef) = (u64)StrToS64(rhs, NULL, 10); break;
		case TYPE_INT64:	*((s64*)mRef) = (s64)StrToS64(rhs, NULL, 10); break;
		case TYPE_FLOAT32:	*((f32*)mRef) = (f32)StrToF32(rhs, NULL); break;
		case TYPE_FLOAT64:	*((f64*)mRef) = (f64)StrToF64(rhs, NULL); break;
		case TYPE_PTCHAR:	break;
		case TYPE_PUCHAR8:	break;
		case TYPE_PUCHAR32:	break;
		default:			break; // Fall through
		};

		return *this;
	}

	x_va_r&					x_va_r::operator=(x_va const& rhs)
	{
		switch (mType)
		{
		case TYPE_BOOL:		*((bool*)mRef) = rhs; break;
		case TYPE_UCHAR:	*((uchar8*)mRef) = rhs; break;
		case TYPE_UINT32:	*((u32*)mRef) = rhs; break;
		case TYPE_INT32:	*((s32*)mRef) = rhs; break;
		case TYPE_UINT8:	*((u8 *)mRef) = rhs; break;
		case TYPE_INT8:		*((s8 *)mRef) = rhs; break;
		case TYPE_UINT16:	*((u16*)mRef) = rhs; break;
		case TYPE_INT16:	*((s16*)mRef) = rhs; break;
		case TYPE_UINT64:	*((u64*)mRef) = rhs; break;
		case TYPE_INT64:	*((s64*)mRef) = rhs; break;
		case TYPE_FLOAT32:	*((f32*)mRef) = rhs; break;
		case TYPE_FLOAT64:	*((f64*)mRef) = rhs; break;
		case TYPE_PTCHAR:	rhs.convertToTCharStr((ascii::prune)mRef, (ascii::prune)mRef2); break;
		case TYPE_PUCHAR8:	rhs.convertToUChar8Str((utf8::prune)mRef, (utf8::prune)mRef2); break;
		case TYPE_PUCHAR32:	rhs.convertToUChar32Str((utf32::prune)mRef, (utf32::prune)mRef2); break;
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