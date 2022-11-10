#include "cbase/private/c_int128.h"
//#ifndef D_NO_CUSTOM_INT128

#include "cbase/c_debug.h"
#include "cbase/c_limits.h"
#include "cbase/private/c_int64.h"
#include "cbase/private/c_uint64.h"
#include "cbase/private/c_uint128.h"

namespace ncore {

	const s128	s128::Zero;
	const s128	s128::One((u32)1);

	s128::s128()
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = 0;
	}


	s128::s128(s16 value)
	{
		if (value < 0)
		{
			*this = s128((u16)-value);
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

	s128::s128(u16 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;
	}

	s128::s128(s32 value)
	{
		if (value < 0)
		{
			*this = s128((u32)-value);
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

	s128::s128(u32 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;
	}

	s128::s128(const s64& value)
	{
		if (value < 0)
		{
			*this = s128(u64(-value));
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

	s128::s128(const s64& high, const s64& low)
	{
		if (high < 0)
		{
			*this = s128(u64(-high), u64(-low));
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

	s128::s128(const u64& value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = value>>32;
		m_LSB = (u32)value;
	}

	s128::s128(const u64& high, const u64& low)
	{
		m_MSB = high>>32;
		m_DSB = (u32)high;
		m_CSB = low>>32;
		m_LSB = (u32)low;
	}

	s128::s128(const s128& value)
	{
		*this = value;
	}

	s128::s128(const u128& value)
	{
		m_MSB = value.m_MSB;
		m_DSB = value.m_DSB;
		m_CSB = value.m_CSB;
		m_LSB = value.m_LSB;
	}

	s128::s128(u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0)
	{
		m_MSB = (b127_96);
		m_DSB = (b95_64);
		m_CSB = (b63_32);
		m_LSB = (b31_0);
	}

	s128& s128::operator=(const s128& value)
	{
		m_MSB = value.m_MSB;
		m_DSB = value.m_DSB;
		m_CSB = value.m_CSB;
		m_LSB = value.m_LSB;

		return *this;
	}

	s128 operator-(const s128& value)
	{
		s128 rVal(value);
		rVal.Negate();
		return rVal;
	}

	s128& s128::operator++()
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

	s128& s128::operator--()
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

	s128& s128::operator*=(const s128& value)
	{
		s128 A(*this);
		s128 B(value);

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

		s128 result;
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
					result += s128(r1) << s;
				s += 16;
				if (r2!=0)
					result += s128(r2) << s;
				if (r4!=0)
					result += s128(r4) << s;
				s += 16;
				if (r3!=0)
					result += s128(r3) << s;
			}
		}

		if ((bANegative && !bBNegative) || (!bANegative && bBNegative))
			result.Negate();

		*this = result;
		return *this;
	}

	s128& s128::operator/=(const s128& value)
	{
		s128 Remainder;
		s128 Quotient;
		Modulus(value, Quotient, Remainder);
		*this = Quotient;
		return *this;
	}

	s128& s128::operator+=(const s128& _value)
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

	s128& s128::operator-=(const s128& _value)
	{
		u64 t = ((u64)m_LSB) - ((u64)_value.m_LSB);
		s32 nCarry = ((t&D_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_LSB = (u32) (t);

		t = ((u64)m_CSB) - ((u64)_value.m_CSB) - nCarry;
		nCarry = ((t&D_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_CSB = (u32) (t);

		t = ((u64)m_DSB) - ((u64)_value.m_DSB) - nCarry;
		nCarry = ((t&D_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_DSB = (u32) (t);

		t = ((u64)m_MSB) - ((u64)_value.m_MSB) - nCarry;
		m_MSB = (u32) (t);

		return *this;
	}

	s128 operator~(const s128& value)
	{
		s128 rVal;
		rVal.m_MSB = ~value.m_MSB;
		rVal.m_DSB = ~value.m_DSB;
		rVal.m_CSB = ~value.m_CSB;
		rVal.m_LSB = ~value.m_LSB;
		return rVal;
	}

	void s128::Abs()
	{
		if (IsNegative())
			InverseTwosComplement();
	}

	void s128::Negate()
	{
		if (IsPositive())
			TwosComplement();
		else
			InverseTwosComplement();
	}

	bool s128::IsNegative() const
	{
		return bool((m_MSB & 0x80000000) != 0);
	}

	bool s128::IsPositive() const
	{
		return bool((m_MSB & 0x80000000) == 0);
	}

	void s128::TwosComplement()
	{
		m_MSB = ~m_MSB;
		m_DSB = ~m_DSB;
		m_CSB = ~m_CSB;
		m_LSB = ~m_LSB;
		operator++();
	}

	void s128::InverseTwosComplement()
	{
		operator--();
		m_MSB = ~m_MSB;
		m_DSB = ~m_DSB;
		m_CSB = ~m_CSB;
		m_LSB = ~m_LSB;
	}

	s128 s128::operator>>(s32 nShift) const
	{
		s128 rVal;
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

	s128 s128::operator<<(s32 nShift) const
	{
		s128 rVal;
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

	s128& s128::operator>>=(s32 nShift)
	{
		*this = (*this >> nShift);
		return *this;
	}

	s128& s128::operator<<=(s32 nShift)
	{
		*this = (*this << nShift);
		return *this;
	}

	bool s128::IsBitSet(s32 nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);

		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = nIndex >> 5;
		return bool((mUInt32[dwBitIndex] & dwBitMask) != 0);
	}

	void s128::SetBit(s32 nIndex, bool value)
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);

		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = nIndex >> 5;

		if (value)
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] | dwBitMask;
		else
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] & ~dwBitMask;
	}

	s128& s128::operator^=(const s128& value)
	{
		m_LSB ^= value.m_LSB;
		m_CSB ^= value.m_CSB;
		m_DSB ^= value.m_DSB;
		m_MSB ^= value.m_MSB;
		return *this;
	}

	s128& s128::operator|=(const s128& value)
	{
		m_LSB |= value.m_LSB;
		m_CSB |= value.m_CSB;
		m_DSB |= value.m_DSB;
		m_MSB |= value.m_MSB;
		return *this;
	}

	s128& s128::operator&=(const s128& value)
	{
		m_LSB &= value.m_LSB;
		m_CSB &= value.m_CSB;
		m_DSB &= value.m_DSB;
		m_MSB &= value.m_MSB;
		return *this;
	}

	s128& s128::operator%=(const s128& value)
	{
		s128 Remainder;
		s128 Quotient;
		Modulus(value, Quotient, Remainder);
		*this = Remainder;
		return *this;
	}

	void s128::Modulus(const s128& divisor, s128& Quotient, s128& Remainder) const
	{
		//Correctly handle negative values
		s128 tempDividend(*this);
		s128 tempDivisor(divisor);
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
				bool bBit = (Remainder >= tempDivisor);
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

	s32	s128::Compare(const s128& other) const
	{
		const bool thisPositive = IsPositive();
		const bool otherPositive = other.IsPositive();
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

	s128::operator s32() const
	{
		if (IsNegative())
		{
			s128 t(*this);
			t.Negate();
			return -t;
		}
		else
		{
			ASSERT(m_CSB == 0 && m_DSB == 0 && m_MSB == 0 && ((m_LSB & 0x80000000) == 0));
			return (s32) m_LSB;
		}
	}

	s128::operator u32() const
	{
		ASSERT(m_CSB == 0 && m_DSB == 0 && m_MSB == 0);
		ASSERT(!IsNegative());
		return m_LSB;
	}

	s128::operator s64() const
	{
		if (IsNegative())
		{
			s128 t(*this);
			t.Negate();
			return -t;
		}
		else
		{
			ASSERT(m_MSB == 0 && m_DSB == 0 && ((m_CSB & 0x80000000) == 0));
			return (((s64) m_CSB) << 32) + m_LSB;
		}
	}

	s128::operator u64() const
	{
		ASSERT(m_MSB == 0 && m_DSB == 0);
		ASSERT(!IsNegative());
		return (((u64) m_CSB) << 32) + m_LSB;
	}
};

//#endif
