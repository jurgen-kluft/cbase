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

	void xguid::toString(runes_t& str)const
	{ 
		// high, word2, word1, low
		ascii::pcrune fmtstr = "%08X:%08X:%08X:%08X";
		crunes_t fmt(fmtstr);
		sprintf(str, fmt, x_va(mGuid.ma32[0]), x_va(mGuid.ma32[1]), x_va(mGuid.ma32[2]), x_va(mGuid.ma32[3]) );
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

	void xguid::fromString(crunes_t const& _str)
	{
		setNull();

		ascii::pcrune fmtstr = "%08X:%08X:%08X:%08X";
		crunes_t fmt(fmtstr);
		crunes_t str(_str);
		sscanf(str, fmt, x_va_r(&mGuid.ma32[0]), x_va_r(&mGuid.ma32[1]), x_va_r(&mGuid.ma32[2]), x_va_r(&mGuid.ma32[3]));
	}
};
