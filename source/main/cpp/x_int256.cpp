/**
* @file HwInt256.cpp
* Core custom 256 bit signed integer
*/

#include "xbase/x_target.h"
#ifndef X_NO_CUSTOM_INT256
#include "xbase/x_limits.h"
#include "xbase/private/x_int64.h"
#include "xbase/private/x_uint64.h"
#include "xbase/private/x_int128.h"
#include "xbase/private/x_uint128.h"
#include "xbase/private/x_int256.h"
#include "xbase/private/x_uint256.h"

namespace xcore {

	const __xint256	__xint256::Zero;
	const __xint256	__xint256::One((u32)1);


	__xint256::__xint256()
	{
		m_MSB = 0;
		m_FSB = 0;
		m_ESB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_BSB = 0;
		m_ASB = 0;
		m_LSB = 0;
	}


	__xint256::__xint256(s16 value)
	{
		if (value < 0)
		{
			*this = __xint256((u16)-value);
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_FSB = 0;
			m_ESB = 0;
			m_DSB = 0;
			m_CSB = 0;
			m_BSB = 0;
			m_ASB = 0;
			m_LSB = value;
		}
	}

	__xint256::__xint256(u16 value)
	{
		m_MSB = 0;
		m_FSB = 0;
		m_ESB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_BSB = 0;
		m_ASB = 0;
		m_LSB = value;
	}

	__xint256::__xint256(s32 value)
	{
		if (value < 0)
		{
			*this = __xint256((u32)-value);
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_FSB = 0;
			m_ESB = 0;
			m_DSB = 0;
			m_CSB = 0;
			m_BSB = 0;
			m_ASB = 0;
			m_LSB = value;
		}
	}

	__xint256::__xint256(u32 value)
	{
		m_MSB = 0;
		m_FSB = 0;
		m_ESB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_BSB = 0;
		m_ASB = 0;
		m_LSB = value;
	}

	__xint256::__xint256(const s64& value)
	{
		if (value < 0)
		{
			*this = __xint256(u64(-value));
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_FSB = 0;
			m_ESB = 0;
			m_DSB = 0;
			m_CSB = 0;
			m_BSB = 0;
			m_ASB = value>>32;
			m_LSB = value&0xFFFFFFFF;
		}
	}

	__xint256::__xint256(const u64& value)
	{
		m_MSB = 0;
		m_FSB = 0;
		m_ESB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_BSB = 0;
		m_ASB = value>>32;
		m_LSB = value&0xFFFFFFFF;
	}

	__xint256::__xint256(const s128& value)
	{
		if (value < 0)
		{
			*this = __xint256(u128(-value));
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_FSB = 0;
			m_ESB = 0;
			m_DSB = 0;
			m_CSB = value>>96;
			m_BSB = value>>64;
			m_ASB = value>>32;
			m_LSB = ((s32)value);
		}
	}

	__xint256::__xint256(const u128& value)
	{
		m_MSB = 0;
		m_FSB = 0;
		m_ESB = 0;
		m_DSB = 0;
		m_CSB = value>>96;
		m_BSB = value>>64;
		m_ASB = value>>32;
		m_LSB = ((s32)value);
	}

	__xint256::__xint256(const __xint256& value)
	{
		*this = value;
	}

	__xint256::__xint256(const __xuint256& value)
	{
		*this = value;
	}

	__xint256::__xint256(u32 b255_224, u32 b223_192, u32 b191_160, u32 b159_128, u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0)
		:m_MSB(b255_224)
		,m_FSB(b223_192)
		,m_ESB(b191_160)
		,m_DSB(b159_128)
		,m_CSB(b127_96)
		,m_BSB(b95_64)
		,m_ASB(b63_32)
		,m_LSB(b31_0)
	{
	}

	__xint256& __xint256::operator=(const __xint256& value)
	{
		m_MSB = value.m_MSB;
		m_FSB = value.m_FSB;
		m_ESB = value.m_ESB;
		m_DSB = value.m_DSB;
		m_CSB = value.m_CSB;
		m_BSB = value.m_BSB;
		m_ASB = value.m_ASB;
		m_LSB = value.m_LSB;
		return *this;
	}

	__xint256& __xint256::operator=(const __xuint256& value)
	{
		m_MSB = value.m_MSB;
		m_FSB = value.m_FSB;
		m_ESB = value.m_ESB;
		m_DSB = value.m_DSB;
		m_CSB = value.m_CSB;
		m_BSB = value.m_BSB;
		m_ASB = value.m_ASB;
		m_LSB = value.m_LSB;
		return *this;
	}

	__xint256 operator-(const __xint256& value)
	{
		__xint256 rVal(value);
		rVal.operator~();
		return rVal;
	}

	__xint256& __xint256::operator++()
	{
		*this = *this + One;
		return *this;
	}

	__xint256& __xint256::operator--()
	{
		*this = *this - One;
		return *this;
	}

	__xint256& __xint256::operator*=(const __xint256& value)
	{
		__xint256 A(*this);
		__xint256 B(value);

		// Correctly handle negative values
		bool bANegative = false;
		bool bBNegative = false;
		if (A.IsNegative())
		{
			bANegative = true;
			A.Negate();
		}
		if (B.IsNegative())
		{
			bBNegative = true;
			B.Negate();
		}

		__xint256 result;
		for (s32 i=0; i<NUM_INT32; i++)
		{
			for (s32 j=0; (i+j)<NUM_INT32; j++)
			{
				u32 ai = mUInt32[NUM_INT32-j-1];
				u32 bi = B.mUInt32[NUM_INT32-i-1];

				u32 r1 = (ai&0x0000FFFF)       * (bi&0x0000FFFF);
				u32 r2 = ((ai&0xFFFF0000)>>16) * (bi&0x0000FFFF);
				u32 r3 = ((ai&0xFFFF0000)>>16) * ((bi&0xFFFF0000)>>16);
				u32 r4 = (ai&0x0000FFFF) * ((bi&0xFFFF0000)>>16);

				s32 s = (i+j)*32;
				if (r1!=0)
					result += __xint256(r1) << s;
				s += 16;
				if (r2!=0)
					result += __xint256(r2) << s;
				if (r4!=0)
					result += __xint256(r4) << s;
				s += 16;
				if (r3!=0)
					result += __xint256(r3) << s;
			}
		}

		if ((bANegative && !bBNegative) || (!bANegative && bBNegative))
			result.Negate();

		*this = result;
		return *this;
	}

	__xint256& __xint256::operator/=(const __xint256& value)
	{
		__xint256 Remainder;
		__xint256 Quotient;
		Modulus(value, Quotient, Remainder);
		*this = Quotient;
		return *this;
	}

	__xint256& __xint256::operator+=(const __xint256& _value)
	{
		u64 t = ((u64)m_LSB) + ((u64)_value.m_LSB);
		s32 nCarry = (t > 0xFFFFFFFF);
		m_LSB = (u32) (t);
		
		t = ((u64)m_ASB) + ((u64)_value.m_ASB) + nCarry;
		nCarry = (t > 0xFFFFFFFF);
		m_ASB = (u32) (t);

		t = ((u64)m_BSB) + ((u64)_value.m_BSB) + nCarry;
		nCarry = (t > 0xFFFFFFFF);
		m_BSB = (u32) (t);

		t = ((u64)m_CSB) + ((u64)_value.m_CSB) + nCarry;
		nCarry = (t > 0xFFFFFFFF);
		m_CSB = (u32) (t);

		t = ((u64)m_DSB) + ((u64)_value.m_DSB) + nCarry;
		nCarry = (t > 0xFFFFFFFF);
		m_DSB = (u32) (t);

		t = ((u64)m_ESB) + ((u64)_value.m_ESB) + nCarry;
		nCarry = (t > 0xFFFFFFFF);
		m_ESB = (u32) (t);

		t = ((u64)m_FSB) + ((u64)_value.m_FSB) + nCarry;
		nCarry = (t > 0xFFFFFFFF);
		m_FSB = (u32) (t);

		t = ((u64)m_MSB) + ((u64)_value.m_MSB) + nCarry;
		m_MSB = (u32) (t);
		return *this;
	}

	__xint256& __xint256::operator-=(const __xint256& _value)
	{
		u64 t = ((u64)m_LSB) - ((u64)_value.m_LSB);
		s32 nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_LSB = (u32) (t);

		t = ((u64)m_ASB) - ((u64)_value.m_ASB) - nCarry;
		nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_ASB = (u32) (t);

		t = ((u64)m_BSB) - ((u64)_value.m_BSB) - nCarry;
		nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_BSB = (u32) (t);

		t = ((u64)m_CSB) - ((u64)_value.m_CSB) - nCarry;
		nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_CSB = (u32) (t);

		t = ((u64)m_DSB) - ((u64)_value.m_DSB) - nCarry;
		nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_DSB = (u32) (t);

		t = ((u64)m_ESB) - ((u64)_value.m_ESB) - nCarry;
		nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_ESB = (u32) (t);

		t = ((u64)m_FSB) - ((u64)_value.m_FSB) - nCarry;
		nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_FSB = (u32) (t);

		t = ((u64)m_MSB) - ((u64)_value.m_MSB) - nCarry;
		m_MSB = (u32) (t);

		return *this;
	}

	__xint256 __xint256::operator~() const
	{
		__xint256 rVal;

		rVal.m_MSB = ~m_MSB;
		rVal.m_FSB = ~m_FSB;
		rVal.m_ESB = ~m_ESB;
		rVal.m_DSB = ~m_DSB;
		rVal.m_CSB = ~m_CSB;
		rVal.m_BSB = ~m_BSB;
		rVal.m_ASB = ~m_ASB;
		rVal.m_LSB = ~m_LSB;

		return rVal;
	}

	void __xint256::Negate()
	{
		if (IsPositive())
			TwosComplement();
		else
			InverseTwosComplement();
	}

	xbool __xint256::IsNegative() const
	{
		return xbool((m_MSB & 0x80000000) != 0);
	}

	xbool __xint256::IsPositive() const
	{
		return xbool((m_MSB & 0x80000000) == 0);
	}

	void __xint256::TwosComplement()
	{
		m_MSB = ~m_MSB;
		m_FSB = ~m_FSB;
		m_ESB = ~m_ESB;
		m_DSB = ~m_DSB;
		m_CSB = ~m_CSB;
		m_BSB = ~m_BSB;
		m_ASB = ~m_ASB;
		m_LSB = ~m_LSB;
		operator++();
	}

	void __xint256::InverseTwosComplement()
	{
		operator--();
		m_MSB = ~m_MSB;
		m_FSB = ~m_FSB;
		m_ESB = ~m_ESB;
		m_DSB = ~m_DSB;
		m_CSB = ~m_CSB;
		m_BSB = ~m_BSB;
		m_ASB = ~m_ASB;
		m_LSB = ~m_LSB;
	}

	__xint256 __xint256::operator>>(s32 nShift) const
	{
		__xint256 rVal;

		if (nShift > 0)
		{
			if (nShift > NUM_BITS)
				nShift = NUM_BITS;

			const s32 n = (nShift >> 5);
			if ((nShift&0x1F) == 0)
			{
				s32 i = 0;
				while (i<n)
					rVal.mUInt32[i++] = 0;
				s32 j = 0;
				while (i<NUM_UINT32)
					rVal.mUInt32[i++] = mUInt32[j++];
			}
			else
			{
				const s32 s1 = nShift - n*32;
				const s32 s2 = 32 - s1;

				s32 i = 0;
				while (i<n)
					rVal.mUInt32[i++] = 0;
				s32 j = 0;
				rVal.mUInt32[i++] = mUInt32[j++] >> s1;
				while (i<NUM_UINT32)
				{
					rVal.mUInt32[i++] = (mUInt32[j] >> s1) | (mUInt32[j-1] << s2);
					++j;
				}
			}
		}
		else if (nShift < 0)
			rVal.operator<<(-nShift);
		else
			rVal = *this;

		return rVal;
	}

	__xint256 __xint256::operator<<(s32 nShift) const
	{
		__xint256 rVal;

		if (nShift > 0)
		{
			if (nShift > NUM_BITS)
				nShift = NUM_BITS;

			s32 n = (nShift >> 5);
			if ((nShift&0x1F) == 0)
			{
				s32 i = NUM_UINT32 - 1;
				while (n>0)
				{
					rVal.mUInt32[i] = 0;
					--i;
					--n;
				}

				s32 j = NUM_UINT32 - 1;
				while (i>=0)
				{
					rVal.mUInt32[i] = mUInt32[j];
					--i;
					--j;
				}
			}
			else
			{
				const s32 s1 = nShift - n*32;
				const s32 s2 = 32 - s1;

				s32 i = NUM_UINT32 - 1;
				while (n>0)
				{
					rVal.mUInt32[i] = 0;
					--i;
					--n;
				}

				s32 j = NUM_UINT32 - 1;
				rVal.mUInt32[i] = mUInt32[j] << s1;
				--i;
				--j;

				while (i>=0)
				{
					rVal.mUInt32[i] = (mUInt32[j] << s1) | (mUInt32[j+1] >> s2);
					--i;
					--j;
				}
			}
		}
		else if (nShift < 0)
			rVal.operator>>(-nShift);
		else
			rVal = *this;

		return rVal;
	}

	__xint256& __xint256::operator>>=(s32 nShift)
	{
		*this = (*this >> nShift);
		return *this;
	}

	__xint256& __xint256::operator<<=(s32 nShift)
	{
		*this = (*this << nShift);
		return *this;
	}

	xbool __xint256::IsBitSet(s32 nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);

		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = nIndex >> 5;
		return xbool((mUInt32[dwBitIndex] & dwBitMask) != 0);
	}

	void __xint256::SetBit(s32 nIndex, xbool value)
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);

		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = nIndex >> 5;

		if (value)
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] | dwBitMask;
		else
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] & ~dwBitMask;
	}

	__xint256& __xint256::operator^=(const __xint256& value)
	{
		for (s32 i=0; i<NUM_INT32; ++i)
			mUInt32[i] = mUInt32[i] ^ value.mUInt32[i];
		return *this;
	}

	__xint256& __xint256::operator|=(const __xint256& value)
	{
		for (s32 i=0; i<NUM_INT32; ++i)
			mUInt32[i] = mUInt32[i] | value.mUInt32[i];
		return *this;
	}

	__xint256& __xint256::operator&=(const __xint256& value)
	{
		for (s32 i=0; i<NUM_INT32; ++i)
			mUInt32[i] = mUInt32[i] & value.mUInt32[i];
		return *this;
	}

	__xint256& __xint256::operator%=(const __xint256& value)
	{
		__xint256 Remainder;
		__xint256 Quotient;
		Modulus(value, Quotient, Remainder);
		*this = Remainder;
		return *this;
	}

	void __xint256::Modulus(const __xint256& divisor, __xint256& Quotient, __xint256& Remainder) const
	{
		//Correctly handle negative values
		__xint256 tempDividend(*this);
		__xint256 tempDivisor(divisor);

		bool bDividendNegative = false;
		bool bDivisorNegative = false;
		if (tempDividend.IsNegative())
		{
			bDividendNegative = true;
			tempDividend.Negate();
		}
		if (tempDivisor.IsNegative())
		{
			bDivisorNegative = true;
			tempDivisor.Negate();
		}

		//Handle the special case's
		if (tempDivisor == Zero)
		{
			//force a Divide by Zero exception
			ASSERTS(false, "Division by zero");
		}
		else if (tempDividend == Zero)
		{
			Quotient = Zero;
			Remainder = Zero;
		}
		else
		{
			Remainder = Zero;
			for (s32 i=0; i<NUM_BITS; i++)
			{
				Remainder += tempDividend.IsBitSet(i) ? One : Zero;
				xbool bBit = (Remainder >= tempDivisor);
				Quotient.SetBit(i, bBit);
				if (bBit)
					Remainder -= tempDivisor;

				if ((i!=(NUM_BITS-1)) && Remainder!=Zero)
					Remainder <<= 1;
			}
		}

		if ((bDividendNegative && !bDivisorNegative) || (!bDividendNegative && bDivisorNegative))
		{
			//Ensure the following formula applies for negative dividends
			//dividend = divisor * Quotient + Remainder
			Quotient.Negate();
		}
	}

	s32	__xint256::Compare(const __xint256& other) const
	{
		if (this == &other)
			return 0;

		const xbool thisPositive = IsPositive();
		const xbool otherPositive = other.IsPositive();
		if (thisPositive != otherPositive)
		{
			return thisPositive ? 1 : -1;	// If this is positive and other is negative then 1, else -1
		}
		else
		{
			for (s32 i=0; i<NUM_INT32; i++)
			{
				if (mUInt32[i] != other.mUInt32[i])
				{
					if (mUInt32[i] != other.mUInt32[i])
						return (mUInt32[i] < other.mUInt32[i]) ? (thisPositive ? -1 : 1) : (thisPositive ? 1 : -1);
				}
			}
		}

		// No differences could be found!
		return 0;
	}

	__xint256::operator s32() const
	{
		if (IsNegative())
		{
			__xint256 t(*this);
			t.Negate();
			return -t;
		}
		else
		{
			ASSERT(m_ASB == 0 && m_BSB == 0 && m_CSB == 0 && m_DSB == 0 && m_ESB == 0 && m_FSB == 0 && m_MSB == 0 && ((m_LSB & 0x80000000) == 0));
			return (s32) m_LSB;
		}
	}

	__xint256::operator u32() const
	{
		ASSERT(m_ASB == 0 && m_BSB == 0 && m_CSB == 0 && m_DSB == 0 && m_ESB == 0 && m_FSB == 0 && m_MSB == 0);
		ASSERT(!IsNegative());
		return m_LSB;
	}

	__xint256::operator s64() const
	{
		if (IsNegative())
		{
			__xint256 t(*this);
			t.Negate();
			return -t;
		}
		else
		{
			ASSERT(m_BSB == 0 && m_CSB == 0 && m_DSB == 0 && m_ESB == 0 && m_FSB == 0 && m_MSB == 0 && ((m_ASB & 0x80000000) == 0));
			return (((s64) m_ASB) << 32) + m_LSB;
		}
	}

	__xint256::operator u64() const
	{
		ASSERT(m_ASB == 0 && m_BSB == 0 && m_CSB == 0 && m_DSB == 0 && m_ESB == 0 && m_FSB == 0 && m_MSB == 0);
		ASSERT(!IsNegative());
		return (((u64) m_ASB) << 32) + m_LSB;
	}

};

#endif
