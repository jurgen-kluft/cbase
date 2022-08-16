#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_guid.h"
#include "xbase/x_va_list.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

namespace ncore
{
	//-------------------------------------------------------------------------------

	guid_t& guid_t::operator = (const guid_t& other)
	{
		for (s32 i=0; i<SIZE32; ++i)
			mGuid.ma32[i] = other.mGuid.ma32[i];
		return *this;
	}

	//-------------------------------------------------------------------------------

	bool guid_t::operator == (const guid_t& other) const
	{
		for (s32 i=0; i<SIZE32; ++i)
		{
			if (mGuid.ma32[i] != other.mGuid.ma32[i])
				return false;
		}
		return true;
	}

	//-------------------------------------------------------------------------------

	bool guid_t::operator != (const guid_t& other) const
	{
		for (s32 i=0; i<SIZE32; ++i)
		{
			if (mGuid.ma32[i] != other.mGuid.ma32[i])
				return true;
		}
		return false;
	}

	//-------------------------------------------------------------------------------

	void guid_t::toString(runes_t& str)const
	{ 
		// high, word2, word1, low
		ascii::pcrune fmtstr = "%08X:%08X:%08X:%08X";
		crunes_t fmt(fmtstr);
		sprintf(str, fmt, va_t(mGuid.ma32[0]), va_t(mGuid.ma32[1]), va_t(mGuid.ma32[2]), va_t(mGuid.ma32[3]) );
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

	void guid_t::fromString(crunes_t const& _str)
	{
		setNull();

		ascii::pcrune fmtstr = "%08X:%08X:%08X:%08X";
		crunes_t fmt(fmtstr);
		crunes_t str(_str);
		sscanf(str, fmt, va_r_t(&mGuid.ma32[0]), va_r_t(&mGuid.ma32[1]), va_r_t(&mGuid.ma32[2]), va_r_t(&mGuid.ma32[3]));
	}
};
