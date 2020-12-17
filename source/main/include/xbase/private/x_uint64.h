/**
* @file Head file x_uint64.h
*
* xCore custom 64 bit unsigned integer.
*/

#ifndef __XBASE_64_BIT_UNSIGNED_INTEGER_H__
#define __XBASE_64_BIT_UNSIGNED_INTEGER_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef X_NO_CUSTOM_UINT64


/**
 * xCore namespace
 */
namespace xcore
{

	// Forward declares
	class __xint64;


	/**
	 * 64 bit unsigned integer
	 */
	class __xuint64
	{
		enum ESettings
		{
			NUM_BITS = 64,
			NUM_INT8 = NUM_BITS / 8,
			NUM_INT16 = NUM_BITS / 16,
			NUM_INT32 = NUM_BITS / 32,
			NUM_UINT8 = NUM_BITS / 8,
			NUM_UINT16 = NUM_BITS / 16,
			NUM_UINT32 = NUM_BITS / 32,
		};

	public:
								/** 
								 *  Constructors
								 */
							__xuint64();
		explicit			__xuint64(s8 _value);
		explicit			__xuint64(u8 _value);
		explicit			__xuint64(s16 _value);
		explicit			__xuint64(u16 _value);
		explicit			__xuint64(s32 _value);
		explicit			__xuint64(u32 _value);
		explicit			__xuint64(const s64& _value);
		explicit			__xuint64(const u64& _value);
							__xuint64(u32 msb, u32 lsb);

		/** 
		 *  Assignment operators
		 */
		__xuint64&			operator=(char _value)									{ *this = __xuint64(_value); return *this; }
		__xuint64&			operator=(s16 _value)									{ *this = __xuint64(_value); return *this; }
		__xuint64&			operator=(s32 _value)									{ *this = __xuint64(_value); return *this; }
		__xuint64&			operator=(unsigned char _value)							{ *this = __xuint64(_value); return *this; }
		__xuint64&			operator=(u16 _value)									{ *this = __xuint64(_value); return *this; }
		__xuint64&			operator=(u32 _value)									{ *this = __xuint64(_value); return *this; }
		__xuint64&			operator=(const u64& _value)							{ *this = __xuint64(_value); return *this; }
		__xuint64&			operator=(const s64& _value)							{ *this = __xuint64(_value); return *this; }

		/** 
		 *  Arithmetic operators
		 */
		__xuint64& 			operator++();
		__xuint64& 			operator--();
		__xuint64	 		operator++(s32);
		__xuint64 			operator--(s32);
		__xuint64& 			operator*=(const __xuint64& _value);
		__xuint64& 			operator/=(const __xuint64& _value);
		__xuint64& 			operator+=(const __xuint64& _value);
		__xuint64& 			operator-=(const __xuint64& _value);
		__xuint64& 			operator%=(const __xuint64& _value);
		__xuint64  			operator~() const;
		/** 
		 *  Bit shift operators
		 */
		__xuint64  			operator>>(s32 nShift) const;
		__xuint64  			operator<<(s32 nShift) const;
		__xuint64& 			operator>>=(s32 nShift);
		__xuint64& 			operator<<=(s32 nShift);

		/** 
		 *  Bit operators
		 */
		__xuint64& 			operator^=(const __xuint64& _value);
		__xuint64& 			operator|=(const __xuint64& _value);
		__xuint64& 			operator&=(const __xuint64& _value);

	                        /**
	                         *  Operators to convert back to basic types.
	                         */
							operator bool() const;
							operator s32() const;
							operator u32() const;
							operator s64() const;
							operator u64() const;

		inline u32&			operator [] (s32 index)									{ return mUInt32[index]; }
		inline u32			operator [] (s32 index) const							{ return mUInt32[index]; }

		/**
		 *  Misc. Functions
		 */
		void 				SetBit(s32 nIndex, bool _value);
		bool 				IsBitSet(s32 nIndex) const;

		void 				Modulus(const __xuint64& Divisor, __xuint64& Quotient, __xuint64& Remainder) const;
		s32					Compare(const __xuint64& other) const;

		static const __xuint64	Zero;
		static const __xuint64	One;
		static const __xuint64	Max;

	protected:
		friend class __xint64;

		/**
		 *  Actual member data variables
		 */
		union
		{
			struct
			{
				u32				m_MSB;
				u32				m_LSB;
			};
			s8				mInt8[NUM_INT8];
			s16				mInt16[NUM_INT16];
			s32				mInt32[NUM_INT32];
			u8				mUInt8[NUM_UINT8];
			u16				mUInt16[NUM_UINT16];
			u32				mUInt32[NUM_UINT32];
		};
	};

	inline __xuint64		operator+(const __xuint64& lhs, const __xuint64& rhs)		{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(const __xuint64& lhs, const __xuint64& rhs)		{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(const __xuint64& lhs, const __xuint64& rhs)		{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(const __xuint64& lhs, const __xuint64& rhs)		{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(const __xuint64& lhs, const __xuint64& rhs)		{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(const __xuint64& lhs, const __xuint64& rhs)		{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(const __xuint64& lhs, const __xuint64& rhs)		{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(const __xuint64& lhs, const __xuint64& rhs)		{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(const __xuint64& lhs, u8 rhs)						{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(const __xuint64& lhs, u8 rhs)						{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(const __xuint64& lhs, u8 rhs)						{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(const __xuint64& lhs, u8 rhs)						{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(const __xuint64& lhs, u8 rhs)						{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(const __xuint64& lhs, u8 rhs)						{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(const __xuint64& lhs, u8 rhs)						{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(const __xuint64& lhs, u8 rhs)						{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(const __xuint64& lhs, u16 rhs)					{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(const __xuint64& lhs, u16 rhs)					{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(const __xuint64& lhs, u16 rhs)					{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(const __xuint64& lhs, u16 rhs)					{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(const __xuint64& lhs, u16 rhs)					{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(const __xuint64& lhs, u16 rhs)					{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(const __xuint64& lhs, u16 rhs)					{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(const __xuint64& lhs, u16 rhs)					{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(const __xuint64& lhs, u32 rhs)					{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(const __xuint64& lhs, u32 rhs)					{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(const __xuint64& lhs, u32 rhs)					{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(const __xuint64& lhs, u32 rhs)					{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(const __xuint64& lhs, u32 rhs)					{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(const __xuint64& lhs, u32 rhs)					{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(const __xuint64& lhs, u32 rhs)					{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(const __xuint64& lhs, u32 rhs)					{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(u32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(u32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(u32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(u32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(u32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(u32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(u32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(u32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(const __xuint64& lhs, s8 rhs)						{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(const __xuint64& lhs, s8 rhs)						{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(const __xuint64& lhs, s8 rhs)						{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(const __xuint64& lhs, s8 rhs)						{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(const __xuint64& lhs, s8 rhs)						{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(const __xuint64& lhs, s8 rhs)						{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(const __xuint64& lhs, s8 rhs)						{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(const __xuint64& lhs, s8 rhs)						{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(s8 lhs, const __xuint64& rhs)						{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(s8 lhs, const __xuint64& rhs)						{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(s8 lhs, const __xuint64& rhs)						{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(s8 lhs, const __xuint64& rhs)						{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(s8 lhs, const __xuint64& rhs)						{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(s8 lhs, const __xuint64& rhs)						{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(s8 lhs, const __xuint64& rhs)						{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(s8 lhs, const __xuint64& rhs)						{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(const __xuint64& lhs, s16 rhs)					{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(const __xuint64& lhs, s16 rhs)					{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(const __xuint64& lhs, s16 rhs)					{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(const __xuint64& lhs, s16 rhs)					{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(const __xuint64& lhs, s16 rhs)					{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(const __xuint64& lhs, s16 rhs)					{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(const __xuint64& lhs, s16 rhs)					{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(const __xuint64& lhs, s16 rhs)					{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(s16 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(s16 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(s16 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(s16 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(s16 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(s16 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(s16 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(s16 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(const __xuint64& lhs, s32 rhs)					{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(const __xuint64& lhs, s32 rhs)					{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(const __xuint64& lhs, s32 rhs)					{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(const __xuint64& lhs, s32 rhs)					{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(const __xuint64& lhs, s32 rhs)					{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(const __xuint64& lhs, s32 rhs)					{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(const __xuint64& lhs, s32 rhs)					{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(const __xuint64& lhs, s32 rhs)					{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator+(s32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r+=rhs; return r; }
	inline __xuint64		operator-(s32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r-=rhs; return r; }
	inline __xuint64  		operator*(s32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r*=rhs; return r; }
	inline __xuint64  		operator/(s32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r/=rhs; return r; }
	inline __xuint64  		operator%(s32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r%=rhs; return r; }
	inline __xuint64  		operator^(s32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r^=rhs; return r; }
	inline __xuint64  		operator|(s32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r|=rhs; return r; }
	inline __xuint64  		operator&(s32 lhs, const __xuint64& rhs)					{ __xuint64 r(lhs); r&=rhs; return r; }

	inline __xuint64		operator<<(const __xuint64& lhs, s8 rhs)					{ return lhs << (s32)rhs; }
	inline __xuint64		operator>>(const __xuint64& lhs, s8 rhs)					{ return lhs >> (s32)rhs; }
	inline __xuint64		operator<<(const __xuint64& lhs, s16 rhs)					{ return lhs << (s32)rhs; }
	inline __xuint64		operator>>(const __xuint64& lhs, s16 rhs)					{ return lhs >> (s32)rhs; }

	inline bool			operator<(const __xuint64& lhs, const __xuint64& rhs)		{ return lhs.Compare(rhs) == -1; }
	inline bool			operator>(const __xuint64& lhs, const __xuint64& rhs)		{ return lhs.Compare(rhs) == 1; }
	inline bool			operator<=(const __xuint64& lhs, const __xuint64& rhs)		{ return lhs.Compare(rhs) != 1; }
	inline bool			operator>=(const __xuint64& lhs, const __xuint64& rhs)		{ return lhs.Compare(rhs) != -1; }
	inline bool			operator==(const __xuint64& lhs, const __xuint64& rhs)		{ return lhs.Compare(rhs) == 0; }
	inline bool			operator!=(const __xuint64& lhs, const __xuint64& rhs)		{ return lhs.Compare(rhs) != 0; }

	inline bool			operator<(const __xuint64& lhs, s32 rhs)					{ return lhs.Compare(__xuint64(rhs)) == -1; }
	inline bool			operator>(const __xuint64& lhs, s32 rhs)					{ return lhs.Compare(__xuint64(rhs)) == 1; }
	inline bool			operator<=(const __xuint64& lhs, s32 rhs)					{ return lhs.Compare(__xuint64(rhs)) != 1; }
	inline bool			operator>=(const __xuint64& lhs, s32 rhs)					{ return lhs.Compare(__xuint64(rhs)) != -1; }
	inline bool			operator==(const __xuint64& lhs, s32 rhs)					{ return lhs.Compare(__xuint64(rhs)) == 0; }
	inline bool			operator!=(const __xuint64& lhs, s32 rhs)					{ return lhs.Compare(__xuint64(rhs)) != 0; }

	inline bool			operator< (s32 lhs, const __xuint64& rhs)					{ return __xuint64(lhs).Compare(rhs) == -1; }
	inline bool			operator> (s32 lhs, const __xuint64& rhs)					{ return __xuint64(lhs).Compare(rhs) == 1; }
	inline bool			operator<=(s32 lhs, const __xuint64& rhs)					{ return __xuint64(lhs).Compare(rhs) != 1; }
	inline bool			operator>=(s32 lhs, const __xuint64& rhs)					{ return __xuint64(lhs).Compare(rhs) != -1; }
	inline bool			operator==(s32 lhs, const __xuint64& rhs)					{ return __xuint64(lhs).Compare(rhs) == 0; }
	inline bool			operator!=(s32 lhs, const __xuint64& rhs)					{ return __xuint64(lhs).Compare(rhs) != 0; }

	inline bool			operator<(const __xuint64& lhs, u32 rhs)					{ return lhs.Compare(__xuint64(rhs)) == -1; }
	inline bool			operator>(const __xuint64& lhs, u32 rhs)					{ return lhs.Compare(__xuint64(rhs)) == 1; }
	inline bool			operator<=(const __xuint64& lhs, u32 rhs)					{ return lhs.Compare(__xuint64(rhs)) != 1; }
	inline bool			operator>=(const __xuint64& lhs, u32 rhs)					{ return lhs.Compare(__xuint64(rhs)) != -1; }
	inline bool			operator==(const __xuint64& lhs, u32 rhs)					{ return lhs.Compare(__xuint64(rhs)) == 0; }
	inline bool			operator!=(const __xuint64& lhs, u32 rhs)					{ return lhs.Compare(__xuint64(rhs)) != 0; }


	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif

#endif	///< __XBASE_64_BIT_UNSIGNED_INTEGER_H__
