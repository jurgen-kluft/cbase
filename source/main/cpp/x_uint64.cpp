/**
* @file x_uint64.cpp
* xCore custom 64 bit unsigned integer
*/

#include "xbase/x_target.h"
#ifndef X_NO_CUSTOM_UINT64

#include "xbase/x_limits.h"
#include "xbase/private/x_int64.h"
#include "xbase/private/x_uint64.h"

namespace xcore
{

	const __xuint64	__xuint64::Zero;
	const __xuint64	__xuint64::One((u32)1);
	const __xuint64	__xuint64::Max(limits_t<u32>::maximum(), limits_t<u32>::maximum());

	__xuint64::__xuint64()
	{
		m_MSB = 0;
		m_LSB = 0;
	}

	__xuint64::__xuint64(s8 _value)
	{
		m_MSB = 0;
		m_LSB = _value;
	}

	__xuint64::__xuint64(u8 _value)
	{
		m_MSB = 0;
		m_LSB = _value;
	}

	__xuint64::__xuint64(s16 _value)
	{
		m_MSB = 0;
		m_LSB = _value;
	}

	__xuint64::__xuint64(u16 _value)
	{
		m_MSB = 0;
		m_LSB = _value;
	}

	__xuint64::__xuint64(s32 _value)
	{
		m_MSB = 0;
		m_LSB = _value;
	}

	__xuint64::__xuint64(u32 _value)
	{
		m_MSB = 0;
		m_LSB = _value;
	}

	__xuint64::__xuint64(const s64& _value)
	{
		m_MSB = (u32) ((_value >> 32) & 0xFFFFFFFF);
		m_LSB = (u32) (_value & 0xFFFFFFFF);
	}

	__xuint64::__xuint64(const u64& _value)
	{
		m_MSB = (u32) ((_value >> 32) & 0xFFFFFFFF);
		m_LSB = (u32) (_value & 0xFFFFFFFF);
	}

	__xuint64::__xuint64(u32 msb, u32 lsb)
	{
		m_MSB = msb;
		m_LSB = lsb;
	}

	__xuint64& __xuint64::operator++()
	{
		*this += One;
		return *this;
	}

	__xuint64& __xuint64::operator--()
	{
		*this -= One;
		return *this;
	}

	__xuint64	__xuint64::operator++(s32)
	{
		__xuint64 r(*this);
		r += One;
		return r;
	}

	__xuint64	__xuint64::operator--(s32)
	{
		__xuint64 r(*this);
		r -= One;
		return r;
	}

	__xuint64& __xuint64::operator*=(const __xuint64& _value)
	{
		__xuint64 A(*this);
		__xuint64 B(_value);

		__xuint64 result;
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
					result += __xuint64(r1) << s;
				s += 16;
				if (r2!=0)
					result += __xuint64(r2) << s;
				if (r4!=0)
					result += __xuint64(r4) << s;
				s += 16;
				if (r3!=0)
					result += __xuint64(r3) << s;
			}
		}

		*this = result;
		return *this;
	}

	__xuint64& __xuint64::operator/=(const __xuint64& _value)
	{
		__xuint64 Remainder;
		__xuint64 Quotient;
		Modulus(_value, Quotient, Remainder);
		*this = Quotient;
		return *this;
	}

	__xuint64& __xuint64::operator+=(const __xuint64& _value)
	{
		u64 t = ((u64)m_LSB) + ((u64)_value.m_LSB);
		s32 nCarry = (t > 0xFFFFFFFF);
		m_LSB = (u32) (t);

		t = ((u64)m_MSB) + ((u64)_value.m_MSB) + nCarry;
		m_MSB = (u32) (t);
		return *this;
	}

	__xuint64& __xuint64::operator-=(const __xuint64& _value)
	{
		u64 t = ((u64)m_LSB) - ((u64)_value.m_LSB);
		s32 nCarry = ((t&0xFFFFFFFF00000000)!=0) ? 1 : 0;
		m_LSB = (u32) (t);

		t = ((u64)m_MSB) - ((u64)_value.m_MSB) - nCarry;
		m_MSB = (u32) (t);
		return *this;
	}

	__xuint64 __xuint64::operator~() const
	{
		__xuint64 rVal;

		rVal.m_MSB = ~m_MSB;
		rVal.m_LSB = ~m_LSB;

		return rVal;
	}

	__xuint64 __xuint64::operator>>(s32 nShift) const
	{
		__xuint64 rVal;
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

	__xuint64 __xuint64::operator<<(s32 nShift) const
	{
		__xuint64 rVal;
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

	__xuint64& __xuint64::operator>>=(s32 nShift)
	{
		*this = (*this >> nShift);
		return *this;
	}

	__xuint64& __xuint64::operator<<=(s32 nShift)
	{
		*this = (*this << nShift);
		return *this;
	}

	bool __xuint64::IsBitSet(s32 nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);
		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = (nIndex >> 5);
		return bool((mUInt32[dwBitIndex] & dwBitMask) != 0);
	}

	void __xuint64::SetBit(s32 nIndex, bool _value)
	{
		ASSERT(nIndex >= 0 && nIndex < NUM_BITS);

		u32 dwBitMask = 0x80000000 >> (nIndex % 32);
		u32 dwBitIndex = (nIndex >> 5);

		if (_value)
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] | dwBitMask;
		else
			mUInt32[dwBitIndex] = mUInt32[dwBitIndex] & ~dwBitMask;
	}

	__xuint64& __xuint64::operator^=(const __xuint64& _value)
	{
		m_LSB ^= _value.m_LSB;
		m_MSB ^= _value.m_MSB;
		return *this;
	}

	__xuint64& __xuint64::operator|=(const __xuint64& _value)
	{
		m_LSB |= _value.m_LSB;
		m_MSB |= _value.m_MSB;
		return *this;
	}

	__xuint64& __xuint64::operator&=(const __xuint64& _value)
	{
		m_LSB &= _value.m_LSB;
		m_MSB &= _value.m_MSB;
		return *this;
	}

	__xuint64& __xuint64::operator%=(const __xuint64& _value)
	{
		__xuint64 Remainder;
		__xuint64 Quotient;
		Modulus(_value, Quotient, Remainder);
		*this = Remainder;
		return *this;
	}

	void __xuint64::Modulus(const __xuint64& divisor, __xuint64& Quotient, __xuint64& Remainder) const
	{
		//Correctly handle negative values
		const __xuint64& tempDividend = *this;
		__xuint64 tempDivisor(divisor);

		//Handle the special case's
		if (tempDivisor == Zero)
		{
			//force a Divide by Zero exception
			ASSERTS(xFALSE, "Division by zero");
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

	s32 __xuint64::Compare(const __xuint64& _value) const
	{
		for (s32 i=0; i<NUM_INT32; i++)
			if (mUInt32[i] != _value.mUInt32[i])
				return (mUInt32[i] < _value.mUInt32[i]) ? -1 : 1;

		return 0;
	}

	__xuint64::operator bool() const
	{
		return m_MSB!=0 || m_LSB!=0;
	}

	__xuint64::operator s32() const
	{
		ASSERT(m_MSB == 0);
		return (s32) m_LSB;
	}

	__xuint64::operator u32() const
	{
		ASSERT(m_MSB == 0);
		return m_LSB;
	}

	__xuint64::operator s64() const
	{
		return ((s64)m_MSB << 32) | ((s64)m_LSB);
	}

	__xuint64::operator u64() const
	{
		return ((u64)m_MSB << 32) | ((u64)m_LSB);
	}

};

#endif
