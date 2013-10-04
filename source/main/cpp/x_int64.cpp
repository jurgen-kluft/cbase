/**
* @file HwInt64.cpp
* Core custom 64 bit signed integer
*/

#include "xbase\x_target.h"
#ifndef X_NO_CUSTOM_INT64
#include "xbase\x_limits.h"
#include "xbase\private\x_uint64.h"
#include "xbase\private\x_int64.h"

namespace xcore {

	const __xint64	__xint64::Zero;
	const __xint64	__xint64::One((u32)1);


	__xint64::__xint64()
	{
		m_MSB = 0;
		m_LSB = 0;
	}

	__xint64::__xint64(s8 _value)
	{
		if (_value < 0)
		{
			*this = __xint64((u16)-_value);
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_LSB = _value;
		}
	}

	__xint64::__xint64(u8 _value)
	{
		m_MSB = 0;
		m_LSB = _value;
	}

	__xint64::__xint64(s16 _value)
	{
		if (_value < 0)
		{
			*this = __xint64((u16)-_value);
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_LSB = _value;
		}
	}

	__xint64::__xint64(u16 _value)
	{
		m_MSB = 0;
		m_LSB = _value;
	}

	__xint64::__xint64(s32 _value)
	{
		if (_value < 0)
		{
			*this = __xint64((u32)-_value);
			TwosComplement();
		}
		else
		{
			m_MSB = 0;
			m_LSB = _value;
		}
	}

	__xint64::__xint64(u32 _value)
	{
		m_MSB = 0;
		m_LSB = _value;
	}

	__xint64::__xint64(const s64& _value)
	{
		if (_value < 0)
		{
			*this = __xint64((u64)-_value);
			TwosComplement();
		}
		else
		{
			m_MSB = (u32) ((_value >> 32) & 0xFFFFFFFF);
			m_LSB = (u32) (_value & 0xFFFFFFFF);
		}
	}

	__xint64::__xint64(const u64& _value)
	{
		m_MSB = (u32) ((_value >> 32) & 0xFFFFFFFF);
		m_LSB = (u32) (_value & 0xFFFFFFFF);
	}

	__xint64::__xint64(u32 h, u32 l)
	{
		m_MSB = h;
		m_LSB = l;
	}

	__xint64& __xint64::operator=(const s64& _value)
	{
		if (_value < 0)
		{
			*this = __xint64((u64)-_value);
			TwosComplement();
		}
		else
		{
			m_MSB = (u32) ((_value >> 32) & 0xFFFFFFFF);
			m_LSB = (u32) (_value & 0xFFFFFFFF);
		}

		return *this;
	}

	__xint64& __xint64::operator=(const u64& _value)
	{
		m_MSB = (u32) ((_value >> 32) & 0xFFFFFFFF);
		m_LSB = (u32) (_value & 0xFFFFFFFF);

		return *this;
	}

	__xint64 operator-(const __xint64& _value)
	{
		__xint64 rVal(_value);
		rVal.Negate();
		return rVal;
	}

	__xint64& __xint64::operator++()
	{
		*this = *this + __xint64((s32)1);
		return *this;
	}

	__xint64& __xint64::operator--()
	{
		*this = *this - __xint64((s32)1);
		return *this;
	}

	__xint64& __xint64::operator*=(const __xint64& _value)
	{
		__xint64 B(_value);

		// Correctly handle negative values
		xbool bANegative = xFALSE;
		xbool bBNegative = xFALSE;
		if (IsNegative())
		{
			bANegative = xTRUE;
			Negate();
		}
		if (B.IsNegative())
		{
			bBNegative = xTRUE;
			B.Negate();
		}

		__xint64 result;
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
					result += __xint64(r1) << s;
				s += 16;
				if (r2!=0)
					result += __xint64(r2) << s;
				if (r4!=0)
					result += __xint64(r4) << s;
				s += 16;
				if (r3!=0)
					result += __xint64(r3) << s;
			}
		}

		if ((bANegative && !bBNegative) || (!bANegative && bBNegative))
			result.Negate();

		m_MSB = result.m_MSB;
		m_LSB = result.m_LSB;

		return *this;
	}

	__xint64& __xint64::operator/=(const __xint64& _value)
	{
		__xint64 Remainder;
		__xint64 Quotient;
		Modulus(_value, Quotient, Remainder);
		*this = Quotient;
		return *this;
	}

	__xint64& __xint64::operator+=(const __xint64& _value)
	{
		u64 t = ((u64)m_LSB) + ((u64)_value.m_LSB);
		s32 nCarry = (t > 0xFFFFFFFF);
		m_LSB = (u32) (t);

		t = ((u64)m_MSB) + ((u64)_value.m_MSB) + nCarry;
		m_MSB = (u32) (t);
		return *this;
	}

	__xint64& __xint64::operator-=(const __xint64& _value)
	{
		u64 t = ((u64)m_LSB) - ((u64)_value.m_LSB);
		s32 nCarry = ((t&X_CONSTANT_64(0xFFFFFFFF00000000))!=0) ? 1 : 0;
		m_LSB = (u32) (t);

		t = ((u64)m_MSB) - ((u64)_value.m_MSB) - nCarry;
		m_MSB = (u32) (t);

		return *this;
	}

	__xint64 operator~(const __xint64& _value)
	{
		__xint64 rVal;
		rVal.m_MSB = ~_value.m_MSB;
		rVal.m_LSB = ~_value.m_LSB;
		return rVal;
	}

	void __xint64::Negate()
	{
		if (IsPositive())
			TwosComplement();
		else
			InverseTwosComplement();
	}

	xbool __xint64::IsNegative() const
	{
		return xbool((m_MSB & 0x80000000) != 0);
	}

	xbool __xint64::IsPositive() const
	{
		return xbool((m_MSB & 0x80000000) == 0);
	}

	void __xint64::TwosComplement()
	{
		m_MSB = ~m_MSB;
		m_LSB = ~m_LSB;
		operator++();
	}

	void __xint64::InverseTwosComplement()
	{
		operator--();
		m_MSB = ~m_MSB;
		m_LSB = ~m_LSB;
	}

	__xint64 __xint64::operator>>(s32 nShift) const
	{
		__xint64 rVal;
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

	__xint64 __xint64::operator<<(s32 nShift) const
	{
		__xint64 rVal;
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

	__xint64& __xint64::operator>>=(s32 nShift)
	{
		*this = (*this >> nShift);
		return *this;
	}

	__xint64& __xint64::operator<<=(s32 nShift)
	{
		*this = (*this << nShift);
		return *this;
	}

	xbool __xint64::IsBitSet(s32 nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < 96);

		bool rVal;
		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		if (nIndex < 32)
			rVal = ((m_MSB & dwBitMask) != 0);
		else
			rVal = ((m_LSB & dwBitMask) != 0);

		return xbool(rVal);
	}

	void __xint64::SetBit(s32 nIndex, xbool _value)
	{
		ASSERT(nIndex >= 0 && nIndex < 96);

		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		if (_value == xFALSE)
			dwBitMask = ~dwBitMask;
		if (nIndex < 32)
		{
			if (_value)
				m_MSB = m_MSB | dwBitMask;
			else
				m_MSB = m_MSB & dwBitMask;
		}
		else
		{
			if (_value)
				m_LSB = m_LSB | dwBitMask;
			else
				m_LSB = m_LSB & dwBitMask;
		}
	}

	__xint64& __xint64::operator^=(const __xint64& _value)
	{
		m_LSB ^= _value.m_LSB;
		m_MSB ^= _value.m_MSB;
		return *this;
	}

	__xint64& __xint64::operator|=(const __xint64& _value)
	{
		m_LSB |= _value.m_LSB;
		m_MSB |= _value.m_MSB;
		return *this;
	}

	__xint64& __xint64::operator&=(const __xint64& _value)
	{
		m_LSB &= _value.m_LSB;
		m_MSB &= _value.m_MSB;
		return *this;
	}

	__xint64& __xint64::operator%=(const __xint64& _value)
	{
		__xint64 Remainder;
		__xint64 Quotient;
		Modulus(_value, Quotient, Remainder);
		*this = Remainder;
		return *this;
	}

	void __xint64::Modulus(const __xint64& divisor, __xint64& Quotient, __xint64& Remainder) const
	{
		//Correctly handle negative values
		__xint64 tempDividend(*this);
		__xint64 tempDivisor(divisor);
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

	s32	__xint64::Compare(const __xint64& other) const
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
					return (mUInt32[i] < other.mUInt32[i]) ? (thisPositive ? -1 : 1) : (thisPositive ? 1 : -1);
			}
		}

		// No differences could be found!
		return 0;
	}

	__xint64::operator s32() const
	{
		if (IsNegative())
		{
			__xint64 t(*this);
			t.Negate();
			return -(s32)t.m_LSB;
		}
		else
		{
			ASSERT(m_MSB == 0 && ((m_LSB & 0x80000000) == 0));
			return (s32)m_LSB;
		}
	}

	__xint64::operator u32() const
	{
		ASSERT(m_MSB == 0);
		ASSERT(!IsNegative());
		return m_LSB;
	}

	__xint64::operator s64() const
	{
		return ((s64)m_MSB << 32) | ((s64)m_LSB);
	}

	__xint64::operator u64() const
	{
		return (((u64)m_MSB) << 32) | ((u64)m_LSB);
	}

};
#endif
