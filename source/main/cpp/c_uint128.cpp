#include "cbase/private/c_uint128.h"
#ifndef D_NO_CUSTOM_UINT128

#include "cbase/c_debug.h"
#include "cbase/c_limits.h"
#include "cbase/private/c_int64.h"
#include "cbase/private/c_uint64.h"
#include "cbase/private/c_int128.h"

namespace ncore 
{
	const u128	u128::Zero;
	const u128	u128::One((u32)1);
	const u128	u128::Max(u128::MAX);

	u128::u128(EConstants c)
	{
		switch (c)
		{
		case MIN:
			m_MSB = 0;
			m_DSB = 0;
			m_CSB = 0;
			m_LSB = 0;
			break;
		case MAX:
			m_MSB = limits_t<u32>::maximum();
			m_DSB = limits_t<u32>::maximum();
			m_CSB = limits_t<u32>::maximum();
			m_LSB = limits_t<u32>::maximum();
			break;
		}
	}

	u128::u128()
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = 0;
	}


	u128::u128(s16 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;
	}

	u128::u128(u16 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;
	}

	u128::u128(s32 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;
	}

	u128::u128(u32 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;
	}


	u128::u128(const s64& value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = value>>32;
		m_LSB = (s32)value;
	}

	u128::u128(const u64& value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = value>>32;
		m_LSB = (u32)value;
	}

	u128::u128(const u64& high, const u64& low)
	{
		m_MSB = high>>32;
		m_DSB = (u32)low;
		m_CSB = low>>32;
		m_LSB = (u32)low;
	}

	u128::u128(const s128& value)
	{
		m_MSB = value.m_MSB;
		m_DSB = value.m_DSB;
		m_CSB = value.m_CSB;
		m_LSB = value.m_LSB;
	}

	u128::u128(const u128& value)
	{
		m_MSB = value.m_MSB;
		m_DSB = value.m_DSB;
		m_CSB = value.m_CSB;
		m_LSB = value.m_LSB;
	}

	u128::u128(u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0)
		:m_MSB(b127_96)
		,m_DSB(b95_64)
		,m_CSB(b63_32)
		,m_LSB(b31_0)
	{
	}

	u128& u128::operator=(s8 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;

		return *this;
	}

	u128& u128::operator=(u8 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;

		return *this;
	}

	u128& u128::operator=(s16 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;

		return *this;
	}

	u128& u128::operator=(u16 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;

		return *this;
	}

	u128& u128::operator=(s32 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;

		return *this;
	}
	u128& u128::operator=(u32 value)
	{
		m_MSB = 0;
		m_DSB = 0;
		m_CSB = 0;
		m_LSB = value;

		return *this;
	}

	u128& u128::operator=(const u128& value)
	{
		m_MSB = value.m_MSB;
		m_DSB = value.m_DSB;
		m_CSB = value.m_CSB;
		m_LSB = value.m_LSB;

		return *this;
	}

	u128& u128::operator++()
	{
		*this = *this + One;
		return *this;
	}

	u128& u128::operator--()
	{
		*this = *this - One;
		return *this;
	}

	u128& u128::operator*=(const u128& value)
	{
		u128 A(*this);
		u128 B(value);

		u128 result;
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
					result += u128(r1) << s;
				s += 16;
				if (r2!=0)
					result += u128(r2) << s;
				if (r4!=0)
					result += u128(r4) << s;
				s += 16;
				if (r3!=0)
					result += u128(r3) << s;
			}
		}
		*this = result;
		return *this;
	}

	u128& u128::operator/=(const u128& value)
	{
		u128 Remainder;
		u128 Quotient;
		Modulus(value, Quotient, Remainder);
		*this = Quotient;
		return *this;
	}

	u128& u128::operator+=(const u128& _value)
	{
		u64 t = ((u64)m_LSB) + ((u64)_value.m_LSB);
		s32 nCarry = (t > 0xFFFFFFFF);
		m_LSB = (u32) (t);

		t = ((u64)m_CSB) + ((u64)_value.m_CSB);
		nCarry = (t > 0xFFFFFFFF);
		m_CSB = (u32) (t);

		t = ((u64)m_DSB) + ((u64)_value.m_DSB);
		nCarry = (t > 0xFFFFFFFF);
		m_DSB = (u32) (t);

		t = ((u64)m_MSB) + ((u64)_value.m_MSB) + nCarry;
		m_MSB = (u32) (t);
		return *this;
	}

	u128& u128::operator-=(const u128& _value)
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

	u128 u128::operator~() const
	{
		u128 rVal;

		rVal.m_MSB = ~m_MSB;
		rVal.m_DSB = ~m_DSB;
		rVal.m_CSB = ~m_CSB;
		rVal.m_LSB = ~m_LSB;

		return rVal;
	}

	u128 u128::operator>>(s32 nShift) const
	{
		u128 rVal;
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

	u128 u128::operator<<(s32 nShift) const
	{
		u128 rVal;
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

	u128& u128::operator>>=(s32 nShift)
	{
		*this = (*this >> nShift);
		return *this;
	}

	u128& u128::operator<<=(s32 nShift)
	{
		*this = (*this << nShift);
		return *this;
	}

	bool u128::IsBitSet(s32 nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);
		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = nIndex >> 5;
		return bool((mUInt32[dwBitIndex] & dwBitMask) != 0);
	}

	void u128::SetBit(s32 nIndex, bool value)
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);

		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = nIndex >> 5;

		if (value)
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] | dwBitMask;
		else
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] & ~dwBitMask;
	}

	u128& u128::operator^=(const u128& value)
	{
		m_LSB ^= value.m_LSB;
		m_CSB ^= value.m_CSB;
		m_DSB ^= value.m_DSB;
		m_MSB ^= value.m_MSB;
		return *this;
	}

	u128& u128::operator|=(const u128& value)
	{
		m_LSB |= value.m_LSB;
		m_CSB |= value.m_CSB;
		m_DSB |= value.m_DSB;
		m_MSB |= value.m_MSB;
		return *this;
	}

	u128& u128::operator&=(const u128& value)
	{
		m_LSB &= value.m_LSB;
		m_CSB &= value.m_CSB;
		m_DSB &= value.m_DSB;
		m_MSB &= value.m_MSB;
		return *this;
	}

	u128& u128::operator%=(const u128& value)
	{
		u128 Remainder;
		u128 Quotient;
		Modulus(value, Quotient, Remainder);
		*this = Remainder;
		return *this;
	}

	void u128::Modulus(const u128& divisor, u128& Quotient, u128& Remainder) const
	{
		// Correctly handle negative values
		const u128& tempDividend = *this;
		u128 tempDivisor(divisor);

		// Handle the special case's
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
	}

	s32 u128::Compare(const u128& value) const
	{
		for (s32 i=0; i<NUM_INT32; i++)
			if (mUInt32[i] != value.mUInt32[i])
				return (mUInt32[i] < value.mUInt32[i]) ? -1 : 1;

		return 0;
	}

	u128::operator s32() const
	{
		ASSERT(m_CSB == 0 && m_DSB == 0 && m_MSB == 0 && ((m_LSB & 0x80000000) == 0));
		return (s32) m_LSB;
	}

	u128::operator u32() const
	{
		return m_LSB;
	}

	u128::operator s64() const
	{
		ASSERT(m_MSB == 0 && m_DSB == 0 && ((m_CSB & 0x80000000) == 0));
		return (((s64) m_CSB) << 32) + m_LSB;
	}

	u128::operator u64() const
	{
		return (((u64) m_CSB) << 32) + m_LSB;
	}

};

#endif
