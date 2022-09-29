#ifndef __CBASE_128_BIT_UNSIGNED_INTEGER_H__
#define __CBASE_128_BIT_UNSIGNED_INTEGER_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef D_NO_CUSTOM_UINT128


/**
 * xCore namespace
 */
namespace ncore
{

	// Forward declares
	class __xint64;
	class __xuint64;
	class __xint128;


	class __xuint128
	{
		enum ESettings
		{
			NUM_BITS = 128,
			NUM_INT8 = NUM_BITS / 8,
			NUM_INT16 = NUM_BITS / 16,
			NUM_INT32 = NUM_BITS / 32,
			NUM_UINT8 = NUM_BITS / 8,
			NUM_UINT16 = NUM_BITS / 16,
			NUM_UINT32 = NUM_BITS / 32,
		};

		enum EConstants
		{
			MIN,
			MAX,
		};

								__xuint128(EConstants c);

	public:
								/** 
								 *  Constructors
								 */
								__xuint128();
		explicit				__xuint128(s16 value);
		explicit				__xuint128(u16 value);
		explicit				__xuint128(s32 value);
		explicit				__xuint128(u32 value);
		explicit				__xuint128(const s64& value);
		explicit				__xuint128(const u64& value);
		explicit				__xuint128(const u64& high, const u64& low);
		explicit				__xuint128(const s128& value);
		explicit				__xuint128(u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0);
								__xuint128(const __xuint128& value);

		/** 
		 *  Assignment operators
		 */
		__xuint128&				operator=(s8 value);
		__xuint128&				operator=(u8 value);
		__xuint128&				operator=(s16 value);
		__xuint128&				operator=(u16 value);
		__xuint128&				operator=(s32 value);
		__xuint128&				operator=(u32 value);
		__xuint128&				operator=(const __xuint128& value);

		/** 
		 *  Arithmetic operators
		 */
		__xuint128& 			operator++();
		__xuint128& 			operator--();
		__xuint128	 			operator++(s32)											{ __xuint128 r(*this); ++r; return r; }
		__xuint128 				operator--(s32)											{ __xuint128 r(*this); --r; return r; }
		__xuint128& 			operator*=(const __xuint128& value);
		__xuint128& 			operator/=(const __xuint128& value);
		__xuint128& 			operator+=(const __xuint128& value);
		__xuint128& 			operator-=(const __xuint128& value);
		__xuint128& 			operator%=(const __xuint128& value);
		__xuint128  			operator~() const;

		/**
		 *  Misc operators
		 */
		__xuint128  			operator>>(s32 nShift) const;
		__xuint128  			operator<<(s32 nShift) const;
		__xuint128& 			operator>>=(s32 nShift);
		__xuint128& 			operator<<=(s32 nShift);
		__xuint128& 			operator^=(const __xuint128& value);
		__xuint128& 			operator|=(const __xuint128& value);
		__xuint128& 			operator&=(const __xuint128& value);

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

		void 					Modulus(const __xuint128& Divisor, __xuint128& Quotient, __xuint128& Remainder) const;
		s32						Compare(const __xuint128& other) const;

		static const __xuint128	Zero;
		static const __xuint128	One;
		static const __xuint128	Max;

	protected:
		friend class __xint128;

		/**
		 *  Actual member data variables
		 */
		union
		{
			struct
			{
				u32				m_MSB;
				u32				m_DSB;
				u32				m_CSB;
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


	inline __xuint128		operator+(const __xuint128& lhs, const __xuint128& rhs) { __xuint128 r(lhs); r+=rhs; return r; }
	inline __xuint128		operator-(const __xuint128& lhs, const __xuint128& rhs) { __xuint128 r(lhs); r-=rhs; return r; }
	inline __xuint128		operator*(const __xuint128& lhs, const __xuint128& rhs) { __xuint128 r(lhs); r*=rhs; return r; }
	inline __xuint128		operator/(const __xuint128& lhs, const __xuint128& rhs) { __xuint128 r(lhs); r/=rhs; return r; }
	inline __xuint128		operator%(const __xuint128& lhs, const __xuint128& rhs) { __xuint128 r(lhs); r%=rhs; return r; }
	inline __xuint128		operator^(const __xuint128& lhs, const __xuint128& rhs) { __xuint128 r(lhs); r^=rhs; return r; }
	inline __xuint128		operator|(const __xuint128& lhs, const __xuint128& rhs) { __xuint128 r(lhs); r|=rhs; return r; }
	inline __xuint128		operator&(const __xuint128& lhs, const __xuint128& rhs) { __xuint128 r(lhs); r&=rhs; return r; }


	inline __xuint128		operator+(const __xuint128& lhs, const s32& rhs) { __xuint128 r(lhs); r+=__xuint128(rhs); return r; }
	inline __xuint128		operator-(const __xuint128& lhs, const s32& rhs) { __xuint128 r(lhs); r-=__xuint128(rhs); return r; }
	inline __xuint128		operator*(const __xuint128& lhs, const s32& rhs) { __xuint128 r(lhs); r*=__xuint128(rhs); return r; }
	inline __xuint128		operator/(const __xuint128& lhs, const s32& rhs) { __xuint128 r(lhs); r/=__xuint128(rhs); return r; }
	inline __xuint128		operator%(const __xuint128& lhs, const s32& rhs) { __xuint128 r(lhs); r%=__xuint128(rhs); return r; }
	inline __xuint128		operator^(const __xuint128& lhs, const s32& rhs) { __xuint128 r(lhs); r^=__xuint128(rhs); return r; }
	inline __xuint128		operator|(const __xuint128& lhs, const s32& rhs) { __xuint128 r(lhs); r|=__xuint128(rhs); return r; }
	inline __xuint128		operator&(const __xuint128& lhs, const s32& rhs) { __xuint128 r(lhs); r&=__xuint128(rhs); return r; }

	inline __xuint128		operator+(const __xuint128& lhs, const u32& rhs) { __xuint128 r(lhs); r+=__xuint128(rhs); return r; }
	inline __xuint128		operator-(const __xuint128& lhs, const u32& rhs) { __xuint128 r(lhs); r-=__xuint128(rhs); return r; }
	inline __xuint128		operator*(const __xuint128& lhs, const u32& rhs) { __xuint128 r(lhs); r*=__xuint128(rhs); return r; }
	inline __xuint128		operator/(const __xuint128& lhs, const u32& rhs) { __xuint128 r(lhs); r/=__xuint128(rhs); return r; }
	inline __xuint128		operator%(const __xuint128& lhs, const u32& rhs) { __xuint128 r(lhs); r%=__xuint128(rhs); return r; }
	inline __xuint128		operator^(const __xuint128& lhs, const u32& rhs) { __xuint128 r(lhs); r^=__xuint128(rhs); return r; }
	inline __xuint128		operator|(const __xuint128& lhs, const u32& rhs) { __xuint128 r(lhs); r|=__xuint128(rhs); return r; }
	inline __xuint128		operator&(const __xuint128& lhs, const u32& rhs) { __xuint128 r(lhs); r&=__xuint128(rhs); return r; }

	inline __xuint128		operator+(const __xuint128& lhs, const s64& rhs) { __xuint128 r(lhs); r+=__xuint128(rhs); return r; }
	inline __xuint128		operator-(const __xuint128& lhs, const s64& rhs) { __xuint128 r(lhs); r-=__xuint128(rhs); return r; }
	inline __xuint128		operator*(const __xuint128& lhs, const s64& rhs) { __xuint128 r(lhs); r*=__xuint128(rhs); return r; }
	inline __xuint128		operator/(const __xuint128& lhs, const s64& rhs) { __xuint128 r(lhs); r/=__xuint128(rhs); return r; }
	inline __xuint128		operator%(const __xuint128& lhs, const s64& rhs) { __xuint128 r(lhs); r%=__xuint128(rhs); return r; }
	inline __xuint128		operator^(const __xuint128& lhs, const s64& rhs) { __xuint128 r(lhs); r^=__xuint128(rhs); return r; }
	inline __xuint128		operator|(const __xuint128& lhs, const s64& rhs) { __xuint128 r(lhs); r|=__xuint128(rhs); return r; }
	inline __xuint128		operator&(const __xuint128& lhs, const s64& rhs) { __xuint128 r(lhs); r&=__xuint128(rhs); return r; }

	inline __xuint128		operator+(const __xuint128& lhs, const u64& rhs) { __xuint128 r(lhs); r+=__xuint128(rhs); return r; }
	inline __xuint128		operator-(const __xuint128& lhs, const u64& rhs) { __xuint128 r(lhs); r-=__xuint128(rhs); return r; }
	inline __xuint128		operator*(const __xuint128& lhs, const u64& rhs) { __xuint128 r(lhs); r*=__xuint128(rhs); return r; }
	inline __xuint128		operator/(const __xuint128& lhs, const u64& rhs) { __xuint128 r(lhs); r/=__xuint128(rhs); return r; }
	inline __xuint128		operator%(const __xuint128& lhs, const u64& rhs) { __xuint128 r(lhs); r%=__xuint128(rhs); return r; }
	inline __xuint128		operator^(const __xuint128& lhs, const u64& rhs) { __xuint128 r(lhs); r^=__xuint128(rhs); return r; }
	inline __xuint128		operator|(const __xuint128& lhs, const u64& rhs) { __xuint128 r(lhs); r|=__xuint128(rhs); return r; }
	inline __xuint128		operator&(const __xuint128& lhs, const u64& rhs) { __xuint128 r(lhs); r&=__xuint128(rhs); return r; }

	inline __xuint128		operator&(const __xuint128& lhs, u8 rhs)	{ u32 r = (u32)lhs; return __xuint128(r&rhs); }
	inline __xuint128		operator&(const __xuint128& lhs, u16 rhs)	{ u32 r = (u32)lhs; return __xuint128(r&rhs); }
	inline __xuint128		operator&(const __xuint128& lhs, u32 rhs)	{ u32 r = (u32)lhs; return __xuint128(r&rhs); }
	inline __xuint128		operator&(const __xuint128& lhs, u64 rhs)	{ u64 r = (u64)lhs; return __xuint128(r&rhs); }
	inline __xuint128		operator&(const __xuint128& lhs, s8 rhs)	{ s32 r = (s32)lhs; return __xuint128(r&rhs); }
	inline __xuint128		operator&(const __xuint128& lhs, s16 rhs)	{ s32 r = (s32)lhs; return __xuint128(r&rhs); }
	inline __xuint128		operator&(const __xuint128& lhs, s32 rhs)	{ s32 r = (s32)lhs; return __xuint128(r&rhs); }
	inline __xuint128		operator&(const __xuint128& lhs, s64 rhs)	{ s64 r = (s64)lhs; return __xuint128(r&rhs); }

	inline u8  				operator&(u8  lhs, const __xuint128& rhs)	{ u32 r = rhs; return r&lhs; }
	inline u16 				operator&(u16 lhs, const __xuint128& rhs)	{ u32 r = rhs; return r&lhs; }
	inline u32 				operator&(u32 lhs, const __xuint128& rhs)	{ u32 r = rhs; return r&lhs; }
	inline u64 				operator&(u64 lhs, const __xuint128& rhs)	{ u64 r = rhs; return r&lhs; }
	inline s8  				operator&(s8  lhs, const __xuint128& rhs)	{ s32 r = rhs; return r&lhs; }
	inline s16 				operator&(s16 lhs, const __xuint128& rhs)	{ s32 r = rhs; return r&lhs; }
	inline s32 				operator&(s32 lhs, const __xuint128& rhs)	{ s32 r = rhs; return r&lhs; }
	inline s64 				operator&(s64 lhs, const __xuint128& rhs)	{ s64 r = rhs; return r&lhs; }

	inline bool				operator<(const __xuint128& lhs, const __xuint128& rhs) { return lhs.Compare(rhs) == -1; }
	inline bool				operator>(const __xuint128& lhs, const __xuint128& rhs) { return lhs.Compare(rhs) == 1; }
	inline bool				operator<=(const __xuint128& lhs, const __xuint128& rhs) { return lhs.Compare(rhs) != 1; }
	inline bool				operator>=(const __xuint128& lhs, const __xuint128& rhs) { return lhs.Compare(rhs) != -1; }
	inline bool				operator==(const __xuint128& lhs, const __xuint128& rhs) { return lhs.Compare(rhs) == 0; }
	inline bool				operator!=(const __xuint128& lhs, const __xuint128& rhs) { return lhs.Compare(rhs) != 0; }

	inline bool				operator==(const __xuint128& lhs, s32 rhs) { return lhs.Compare(__xuint128(rhs)) == 0; }
	inline bool				operator!=(const __xuint128& lhs, s32 rhs) { return lhs.Compare(__xuint128(rhs)) != 0; }

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif
#endif	///< __CBASE_128_BIT_UNSIGNED_INTEGER_H__
