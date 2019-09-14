#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_guid.h"
#include "xbase/x_va_list.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

namespace xcore
{
	//-------------------------------------------------------------------------------

	xguid& xguid::operator = (const xguid& other)
	{
		for (s32 i=0; i<SIZE32; ++i)
			mGuid.ma32[i] = other.mGuid.ma32[i];
		return *this;
	}

	//-------------------------------------------------------------------------------

	bool xguid::operator == (const xguid& other) const
	{
		for (s32 i=0; i<SIZE32; ++i)
		{
			if (mGuid.ma32[i] != other.mGuid.ma32[i])
				return false;
		}
		return true;
	}

	//-------------------------------------------------------------------------------

	bool xguid::operator != (const xguid& other) const
	{
		for (s32 i=0; i<SIZE32; ++i)
		{
			if (mGuid.ma32[i] != other.mGuid.ma32[i])
				return true;
		}
		return false;
	}

	//-------------------------------------------------------------------------------

	void xguid::toString(ascii::runes& str)const
	{ 
		// high, word2, word1, low
		ascii::pcrune fmtstr = "%08X:%08X:%08X:%08X";
		ascii::crunes fmt(fmtstr, ascii::endof(fmtstr, NULL));
		ascii::sprintf(str, fmt, x_va(mGuid.ma32[0]), x_va(mGuid.ma32[1]), x_va(mGuid.ma32[2]), x_va(mGuid.ma32[3]) );
	}


	/**
     *------------------------------------------------------------------------------
	 * Arguments:
	 *     strGuid - use this string to generate new GUID
	 * Returns:
	 *     void
	 * Summary:
	 *     parse the string and generate new guid. Then assign it to mGuid
	 * Description:
	 *     mGuid will be changed by the input string
	 *------------------------------------------------------------------------------
	 */

	void xguid::fromString(ascii::crunes const& _str)
	{
		setNull();

		ascii::crunes iter = _str;
		for (s32 i=0, s=0; s<32; i++)
		{
			uchar32 c = utf::read(iter);
			if (c == '\0')
				break;

			s32 d = -1;
			if (c>='0' && c<='9')
				d = c - '0';
			else if (c>='a' && c<='f')
				d = 10 + (c - 'a');
			else if (c>='A' && c<='F')
				d = 10 + (c - 'A');

			if (d != -1)
			{
				u32& _word = mGuid.ma32[(s>>3)];
				_word = (_word << 4) | (d & 0xf);
				++s;
			}
		}
	}
};
