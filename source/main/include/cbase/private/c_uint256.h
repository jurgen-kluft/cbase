/**
* @file Head file x_uint256.h
*
* xCore custom 256 bit unsigned integer.
*/

#ifndef __CBASE_256_BIT_UNSIGNED_INTEGER_H__
#define __CBASE_256_BIT_UNSIGNED_INTEGER_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef D_NO_CUSTOM_UINT256


/**
 * xCore namespace
 */
namespace ncore
{
	// Forward declares
	class __xint64;
	class __xuint64;
	class __xint128;
	class __xuint128;
	class __xint256;


	class __xuint256
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
								__xuint256();
		explicit				__xuint256(s8 value);
		explicit				__xuint256(u8 value);
		explicit				__xuint256(s16 value);
		explicit				__xuint256(u16 value);
		explicit				__xuint256(s32 value);
		explicit				__xuint256(u32 value);
		explicit				__xuint256(const s64& value);
		explicit				__xuint256(const u64& value);
		explicit				__xuint256(const s128& value);
		explicit				__xuint256(const u128& value);
								__xuint256(const __xint256& value);
								__xuint256(const __xuint256& value);
								__xuint256(u32 b255_224, u32 b223_192, u32 b191_160, u32 b159_128, u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0);

		/** 
		 *  Assignment operators
		 */
		__xuint256&				operator=(const __xint256& value);
		__xuint256&				operator=(const __xuint256& value);

		/** 
		 *  Arithmetic operators
		 */
		__xuint256& 			operator++();
		__xuint256& 			operator--();
		__xuint256	 			operator++(s32)											{ __xuint256 r(*this); r++; return r; }
		__xuint256 				operator--(s32)											{ __xuint256 r(*this); r--; return r; }
		__xuint256& 			operator*=(const __xuint256& value);
		__xuint256& 			operator/=(const __xuint256& value);
		__xuint256& 			operator+=(const __xuint256& value);
		__xuint256& 			operator-=(const __xuint256& value);
		__xuint256& 			operator%=(const __xuint256& value);
		__xuint256  			operator~() const;

		/**
		 *  Misc operators
		 */
		__xuint256  			operator>>(s32 nShift) const;
		__xuint256  			operator<<(s32 nShift) const;
		__xuint256& 			operator>>=(s32 nShift);
		__xuint256& 			operator<<=(s32 nShift);
		__xuint256& 			operator^=(const __xuint256& value);
		__xuint256& 			operator|=(const __xuint256& value);
		__xuint256& 			operator&=(const __xuint256& value);

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
		 *  Misc. Functions
		 */
		void 					SetBit(s32 nIndex, bool value);
		bool 					IsBitSet(s32 nIndex) const;

		void 					Modulus(const __xuint256& Divisor, __xuint256& Quotient, __xuint256& Remainder) const;
		s32						Compare(const __xuint256& other) const;

		static const __xuint256	Zero;
		static const __xuint256	One;

	protected:
		friend class __xint256;

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


	inline __xuint256		operator+(const __xuint256& lhs, const __xuint256& rhs) { __xuint256 r(lhs); r+=rhs; return r; }
	inline __xuint256		operator-(const __xuint256& lhs, const __xuint256& rhs) { __xuint256 r(lhs); r-=rhs; return r; }
	inline __xuint256  		operator*(const __xuint256& lhs, const __xuint256& rhs) { __xuint256 r(lhs); r*=rhs; return r; }
	inline __xuint256  		operator/(const __xuint256& lhs, const __xuint256& rhs) { __xuint256 r(lhs); r/=rhs; return r; }
	inline __xuint256  		operator%(const __xuint256& lhs, const __xuint256& rhs) { __xuint256 r(lhs); r%=rhs; return r; }
	inline __xuint256  		operator^(const __xuint256& lhs, const __xuint256& rhs) { __xuint256 r(lhs); r^=rhs; return r; }
	inline __xuint256  		operator|(const __xuint256& lhs, const __xuint256& rhs) { __xuint256 r(lhs); r|=rhs; return r; }
	inline __xuint256  		operator&(const __xuint256& lhs, const __xuint256& rhs) { __xuint256 r(lhs); r&=rhs; return r; }

	inline __xuint256		operator+(const __xuint256& lhs, s32 rhs) { __xuint256 r(lhs); r+=__xuint256(rhs); return r; }
	inline __xuint256		operator-(const __xuint256& lhs, s32 rhs) { __xuint256 r(lhs); r-=__xuint256(rhs); return r; }
	inline __xuint256  		operator*(const __xuint256& lhs, s32 rhs) { __xuint256 r(lhs); r*=__xuint256(rhs); return r; }
	inline __xuint256  		operator/(const __xuint256& lhs, s32 rhs) { __xuint256 r(lhs); r/=__xuint256(rhs); return r; }
	inline __xuint256  		operator%(const __xuint256& lhs, s32 rhs) { __xuint256 r(lhs); r%=__xuint256(rhs); return r; }
	inline __xuint256  		operator^(const __xuint256& lhs, s32 rhs) { __xuint256 r(lhs); r^=__xuint256(rhs); return r; }
	inline __xuint256  		operator|(const __xuint256& lhs, s32 rhs) { __xuint256 r(lhs); r|=__xuint256(rhs); return r; }
	inline __xuint256  		operator&(const __xuint256& lhs, s32 rhs) { __xuint256 r(lhs); r&=__xuint256(rhs); return r; }

	inline bool			operator<(const __xuint256& lhs, const __xuint256& rhs) { return bool(lhs.Compare(rhs) == -1); }
	inline bool			operator>(const __xuint256& lhs, const __xuint256& rhs) { return bool(lhs.Compare(rhs) == 1); }
	inline bool			operator<=(const __xuint256& lhs, const __xuint256& rhs) { return bool(lhs.Compare(rhs) != 1); }
	inline bool			operator>=(const __xuint256& lhs, const __xuint256& rhs) { return bool(lhs.Compare(rhs) != -1); }
	inline bool			operator==(const __xuint256& lhs, const __xuint256& rhs) { return bool(lhs.Compare(rhs) == 0); }
	inline bool			operator!=(const __xuint256& lhs, const __xuint256& rhs) { return bool(lhs.Compare(rhs) != 0); }

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif
#endif	///< __CBASE_256_BIT_UNSIGNED_INTEGER_H__
