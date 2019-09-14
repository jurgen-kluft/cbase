#ifndef __XBASE_256_BIT_SIGNED_INTEGER_H__
#define __XBASE_256_BIT_SIGNED_INTEGER_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef X_NO_CUSTOM_INT256

namespace xcore
{
	// Forward declares
	class __xint64;
	class __xuint64;
	class __xint128;
	class __xuint128;
	class __xuint256;

	class __xint256
	{
		enum ESettings
		{
			NUM_BITS = 256,
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
								__xint256();
		explicit				__xint256(s8 value);
		explicit				__xint256(u8 value);
		explicit				__xint256(s16 value);
		explicit				__xint256(u16 value);
		explicit				__xint256(s32 value);
		explicit				__xint256(u32 value);
		explicit				__xint256(const s64& value);
		explicit				__xint256(const u64& value);
		explicit				__xint256(const s128& value);
		explicit				__xint256(const u128& value);
								__xint256(const __xint256& value);
								__xint256(const __xuint256& value);
								__xint256(u32 b255_224, u32 b223_192, u32 b191_160, u32 b159_128, u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0);

		/** 
		 *  Arithmetic operators
		 */
		__xint256&				operator=(const __xint256& value);
		__xint256&				operator=(const __xuint256& value);
		__xint256& 				operator++();
		__xint256& 				operator--();
		__xint256	 			operator++(s32)											{ __xint256 r(*this); (*this)++; return r; }
		__xint256 				operator--(s32)											{ __xint256 r(*this); (*this)--; return r; }
		__xint256& 				operator*=(const __xint256& value);
		__xint256& 				operator/=(const __xint256& value);
		__xint256& 				operator+=(const __xint256& value);
		__xint256& 				operator-=(const __xint256& value);
		__xint256& 				operator%=(const __xint256& value);
		friend __xint256  		operator-(const __xint256& value);
		__xint256  				operator~() const;

		/**
		 *  Misc operators
		 */
		__xint256  				operator>>(s32 nShift) const;
		__xint256  				operator<<(s32 nShift) const;
		__xint256& 				operator>>=(s32 nShift);
		__xint256& 				operator<<=(s32 nShift);
		__xint256& 				operator^=(const __xint256& value);
		__xint256& 				operator|=(const __xint256& value);
		__xint256& 				operator&=(const __xint256& value);

	                         	/**
	                         	 *  Operators to convert back to basic types.
	                          	 */
								operator s32() const;
								operator u32() const;
								operator s64() const;
								operator u64() const;

		inline u32&				operator [] (s32 index)									{ return mUInt32[index]; }
		inline u32				operator [] (s32 index) const							{ return mUInt32[index]; }

		/**
		 *  Misc operators
		 */
		void 					SetBit(s32 nIndex, xbool value);
		xbool 					IsBitSet(s32 nIndex) const;

		void 					Negate();

		xbool 					IsNegative() const;
		xbool 					IsPositive() const;

		void 					Modulus(const __xint256& Divisor, __xint256& Quotient, __xint256& Remainder) const;
		s32						Compare(const __xint256& other) const;

		static const __xint256	Zero;
		static const __xint256	One;

	protected:
		void					TwosComplement();
		void					InverseTwosComplement();

		friend class __xuint256;

		/**
		 *  Actual member data variables
		 */
		union
		{
			struct
			{
				u32				m_MSB;
				u32				m_FSB;
				u32				m_ESB;
				u32				m_DSB;
				u32				m_CSB;
				u32				m_BSB;
				u32				m_ASB;
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

	inline __xint256	operator+(const __xint256& lhs, const __xint256& rhs) { __xint256 r(lhs); r+=rhs; return r; }
	inline __xint256	operator-(const __xint256& lhs, const __xint256& rhs) { __xint256 r(lhs); r-=rhs; return r; }
	inline __xint256  	operator*(const __xint256& lhs, const __xint256& rhs) { __xint256 r(lhs); r*=rhs; return r; }
	inline __xint256  	operator/(const __xint256& lhs, const __xint256& rhs) { __xint256 r(lhs); r/=rhs; return r; }
	inline __xint256  	operator%(const __xint256& lhs, const __xint256& rhs) { __xint256 r(lhs); r%=rhs; return r; }
	inline __xint256  	operator^(const __xint256& lhs, const __xint256& rhs) { __xint256 r(lhs); r^=rhs; return r; }
	inline __xint256  	operator|(const __xint256& lhs, const __xint256& rhs) { __xint256 r(lhs); r|=rhs; return r; }
	inline __xint256  	operator&(const __xint256& lhs, const __xint256& rhs) { __xint256 r(lhs); r&=rhs; return r; }

	inline __xint256	operator+(const __xint256& lhs, s32 rhs) { __xint256 r(lhs); r+=__xint256(rhs); return r; }
	inline __xint256	operator-(const __xint256& lhs, s32 rhs) { __xint256 r(lhs); r-=__xint256(rhs); return r; }
	inline __xint256  	operator*(const __xint256& lhs, s32 rhs) { __xint256 r(lhs); r*=__xint256(rhs); return r; }
	inline __xint256  	operator/(const __xint256& lhs, s32 rhs) { __xint256 r(lhs); r/=__xint256(rhs); return r; }
	inline __xint256  	operator%(const __xint256& lhs, s32 rhs) { __xint256 r(lhs); r%=__xint256(rhs); return r; }
	inline __xint256  	operator^(const __xint256& lhs, s32 rhs) { __xint256 r(lhs); r^=__xint256(rhs); return r; }
	inline __xint256  	operator|(const __xint256& lhs, s32 rhs) { __xint256 r(lhs); r|=__xint256(rhs); return r; }
	inline __xint256  	operator&(const __xint256& lhs, s32 rhs) { __xint256 r(lhs); r&=__xint256(rhs); return r; }

	inline xbool		operator<(const __xint256& lhs, const __xint256& rhs) { return xbool(lhs.Compare(rhs) == -1); }
	inline xbool		operator>(const __xint256& lhs, const __xint256& rhs) { return xbool(lhs.Compare(rhs) == 1); }
	inline xbool		operator<=(const __xint256& lhs, const __xint256& rhs) { return xbool(lhs.Compare(rhs) != 1); }
	inline xbool		operator>=(const __xint256& lhs, const __xint256& rhs) { return xbool(lhs.Compare(rhs) != -1); }
	inline xbool		operator==(const __xint256& lhs, const __xint256& rhs) { return xbool(lhs.Compare(rhs) == 0); }
	inline xbool		operator!=(const __xint256& lhs, const __xint256& rhs) { return xbool(lhs.Compare(rhs) != 0); }
};

#endif
#endif	///< __XBASE_256_BIT_SIGNED_INTEGER_H__
