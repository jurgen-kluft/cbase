/**
* @file HwInt128.cpp
* Core custom 128 bit signed integer
*/

#include "xbase\x_target.h"
#ifndef X_NO_CUSTOM_INT128

#include "xbase\x_limits.h"
#include "xbase\private\x_int64.h"
#include "xbase\private\x_uint64.h"
#include "xbase\private\x_uint128.h"

#include "xbase\private\x_int128.h"

namespace xcore {

	const __xint128	__xint128::Zero;
	const __xint128	__xint128::One((u32)1);


	__xint128::__xint128()
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = 0;
	}


	__xint128::__xint128(s16 value)
	{
		if (value < 0)
		{
			*this = __xint128((u16)-value);
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_DSB = 0;
			m_CSB = 0;
			m_LSB = value;
		}
	}

	__xint128::__xint128(u16 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;
	}

	__xint128::__xint128(s32 value)
	{
		if (value < 0)
		{
			*this = __xint128((u32)-value);
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_DSB = 0;
			m_CSB = 0;
			m_LSB = value;
		}
	}

	__xint128::__xint128(u32 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;
	}

	__xint128::__xint128(const s64& value)
	{
		if (value < 0)
		{
			*this = __xint128(u64(-value));
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_DSB = 0;
			m_CSB = value>>32;
			m_LSB = (u32)value;
		}
	}

	__xint128::__xint128(const s64& high, const s64& low)
	{
		if (high < 0)
		{
			*this = __xint128(u64(-high), u64(-low));
			TwosComplement();
		}
		else
		{
			m_MSB = high>>32;
			m_DSB = (u32)high;
			m_CSB = low>>32;
			m_LSB = (u32)low;
		}
	}

	__xint128::__xint128(const u64& value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = value>>32;
		m_LSB = (u32)value;
	}

	__xint128::__xint128(const u64& high, const u64& low)
	{
		m_MSB = high>>32;
		m_DSB = (u32)high;
		m_CSB = low>>32;
		m_LSB = (u32)low;
	}

	__xint128::__xint128(const __xint128& value)
	{
		*this = value;
	}

	__xint128::__xint128(const __xuint128& value)
	{
		m_MSB = value.m_MSB;
		m_DSB = value.m_DSB;
		m_CSB = value.m_CSB;
		m_LSB = value.m_LSB;
	}

	__xint128::__xint128(u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0)
	{
		m_MSB = (b127_96);
		m_DSB = (b95_64);
		m_CSB = (b63_32);
		m_LSB = (b31_0);
	}

	__xint128& __xint128::operator=(const __xint128& value)
	{
		m_MSB = value.m_MSB;
		m_DSB = value.m_DSB;
		m_CSB = value.m_CSB;
		m_LSB = value.m_LSB;

		return *this;
	}

	__xint128 operator-(const __xint128& value)
	{
		__xint128 rVal(value);
		rVal.Negate();
		return rVal;
	}

	__xint128& __xint128::operator++()
	{
		++m_LSB;
		if (m_LSB==0)
		{
			++m_CSB;
			if (m_CSB==0)
			{
				++m_DSB;
				if (m_DSB==0)
				{
					++m_MSB;
				}
			}
		}
		return *this;
	}

	__xint128& __xint128::operator--()
	{
		--m_LSB;
		if (m_LSB==0)
		{
			--m_CSB;
			if (m_CSB==0)
			{
				--m_DSB;
				if (m_DSB==0)
				{
					--m_MSB;
				}
			}
		}
		return *this;
	}

	__xint128& __xint128::operator*=(const __xint128& value)
	{
		__xint128 A(*this);
		__xint128 B(value);

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

		__xint128 result;
		for (s32 i=0; i<NUM_INT32; i++)
		{
			for (s32 j=0; (i+j)<NUM_INT32; j++)
			{
				u32 ai = A.mUInt32[NUM_INT32-j-1];
				u32 bi = B.mUInt32[NUM_INT32-i-1];

				u32 r1 = (ai&0x0000FFFF)       * (bi&0x0000FFFF);
				u32 r2 = ((ai&0xFFFF0000)>>16) * (bi&0x0000FFFF);
				u32 r3 = ((ai&0xFFFF0000)>>16) * ((bi&0xFFFF0000)>>16);
				u32 r4 = (ai&0x0000FFFF) * ((bi&0xFFFF0000)>>16);

				s32 s = (i+j)*32;
				if (r1!=0)
					result += __xint128(r1) << s;
				s += 16;
				if (r2!=0)
					result += __xint128(r2) << s;
				if (r4!=0)
					result += __xint128(r4) << s;
				s += 16;
				if (r3!=0)
					result += __xint128(r3) << s;
			}
		}

		if ((bANegative && !bBNegative) || (!bANegative && bBNegative))
			result.Negate();

		*this = result;
		return *this;
	}

	__xint128& __xint128::operator/=(const __xint128& value)
	{
		__xint128 Remainder;
		__xint128 Quotient;
		Modulus(value, Quotient, Remainder);
		*this = Quotient;
		return *this;
	}

	__xint128& __xint128::operator+=(const __xint128& _value)
	{
		u64 t = ((u64)m_LSB) + ((u64)_value.m_LSB);
		s32 nCarry = (t > 0xFFFFFFFF);
		m_LSB = (u32) (t);

		t = ((u64)m_CSB) + ((u64)_value.m_CSB) + nCarry;
		nCarry = (t > 0xFFFFFFFF);
		m_CSB = (u32) (t);

		t = ((u64)m_DSB) + ((u64)_value.m_DSB) + nCarry;
		nCarry = (t > 0xFFFFFFFF);
		m_DSB = (u32) (t);

		t = ((u64)m_MSB) + ((u64)_value.m_MSB) + nCarry;
		m_MSB = (u32) (t);
		return *this;
	}

	__xint128& __xint128::operator-=(const __xint128& _value)
	{
		u64 t = ((u64)m_LSB) - ((u64)_value.m_LSB);
		s32 nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_LSB = (u32) (t);

		t = ((u64)m_CSB) - ((u64)_value.m_CSB) - nCarry;
		nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_CSB = (u32) (t);

		t = ((u64)m_DSB) - ((u64)_value.m_DSB) - nCarry;
		nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_DSB = (u32) (t);

		t = ((u64)m_MSB) - ((u64)_value.m_MSB) - nCarry;
		m_MSB = (u32) (t);

		return *this;
	}

	__xint128 operator~(const __xint128& value)
	{
		__xint128 rVal;
		rVal.m_MSB = ~value.m_MSB;
		rVal.m_DSB = ~value.m_DSB;
		rVal.m_CSB = ~value.m_CSB;
		rVal.m_LSB = ~value.m_LSB;
		return rVal;
	}

	void __xint128::Abs()
	{
		if (IsNegative())
			InverseTwosComplement();
	}

	void __xint128::Negate()
	{
		if (IsPositive())
			TwosComplement();
		else
			InverseTwosComplement();
	}

	xbool __xint128::IsNegative() const
	{
		return xbool((m_MSB & 0x80000000) != 0);
	}

	xbool __xint128::IsPositive() const
	{
		return xbool((m_MSB & 0x80000000) == 0);
	}

	void __xint128::TwosComplement()
	{
		m_MSB = ~m_MSB;
		m_DSB = ~m_DSB;
		m_CSB = ~m_CSB;
		m_LSB = ~m_LSB;
		operator++();
	}

	void __xint128::InverseTwosComplement()
	{
		operator--();
		m_MSB = ~m_MSB;
		m_DSB = ~m_DSB;
		m_CSB = ~m_CSB;
		m_LSB = ~m_LSB;
	}

	__xint128 __xint128::operator>>(s32 nShift) const
	{
		__xint128 rVal;
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

	__xint128 __xint128::operator<<(s32 nShift) const
	{
		__xint128 rVal;
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

	__xint128& __xint128::operator>>=(s32 nShift)
	{
		*this = (*this >> nShift);
		return *this;
	}

	__xint128& __xint128::operator<<=(s32 nShift)
	{
		*this = (*this << nShift);
		return *this;
	}

	xbool __xint128::IsBitSet(s32 nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);

		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = nIndex >> 5;
		return xbool((mUInt32[dwBitIndex] & dwBitMask) != 0);
	}

	void __xint128::SetBit(s32 nIndex, xbool value)
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);

		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = nIndex >> 5;

		if (value)
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] | dwBitMask;
		else
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] & ~dwBitMask;
	}

	__xint128& __xint128::operator^=(const __xint128& value)
	{
		m_LSB ^= value.m_LSB;
		m_CSB ^= value.m_CSB;
		m_DSB ^= value.m_DSB;
		m_MSB ^= value.m_MSB;
		return *this;
	}

	__xint128& __xint128::operator|=(const __xint128& value)
	{
		m_LSB |= value.m_LSB;
		m_CSB |= value.m_CSB;
		m_DSB |= value.m_DSB;
		m_MSB |= value.m_MSB;
		return *this;
	}

	__xint128& __xint128::operator&=(const __xint128& value)
	{
		m_LSB &= value.m_LSB;
		m_CSB &= value.m_CSB;
		m_DSB &= value.m_DSB;
		m_MSB &= value.m_MSB;
		return *this;
	}

	__xint128& __xint128::operator%=(const __xint128& value)
	{
		__xint128 Remainder;
		__xint128 Quotient;
		Modulus(value, Quotient, Remainder);
		*this = Remainder;
		return *this;
	}

	void __xint128::Modulus(const __xint128& divisor, __xint128& Quotient, __xint128& Remainder) const
	{
		//Correctly handle negative values
		__xint128 tempDividend(*this);
		__xint128 tempDivisor(divisor);
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

	s32	__xint128::Compare(const __xint128& other) const
	{
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

	__xint128::operator s32() const
	{
		if (IsNegative())
		{
			__xint128 t(*this);
			t.Negate();
			return -t;
		}
		else
		{
			ASSERT(m_CSB == 0 && m_DSB == 0 && m_MSB == 0 && ((m_LSB & 0x80000000) == 0));
			return (s32) m_LSB;
		}
	}

	__xint128::operator u32() const
	{
		ASSERT(m_CSB == 0 && m_DSB == 0 && m_MSB == 0);
		ASSERT(!IsNegative());
		return m_LSB;
	}

	__xint128::operator s64() const
	{
		if (IsNegative())
		{
			__xint128 t(*this);
			t.Negate();
			return -t;
		}
		else
		{
			ASSERT(m_MSB == 0 && m_DSB == 0 && ((m_CSB & 0x80000000) == 0));
			return (((s64) m_CSB) << 32) + m_LSB;
		}
	}

	__xint128::operator u64() const
	{
		ASSERT(m_MSB == 0 && m_DSB == 0);
		ASSERT(!IsNegative());
		return (((u64) m_CSB) << 32) + m_LSB;
	}
};

#endif
