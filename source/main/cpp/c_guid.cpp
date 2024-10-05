#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_guid.h"
#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"

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
		crunes_t fmt = make_crunes(fmtstr, fmtstr + 19);
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
		crunes_t fmt = make_crunes(fmtstr, fmtstr + 19);
		crunes_t str = make_crunes(_str);
		sscanf(str, fmt, &mGuid.ma32[0], &mGuid.ma32[1], &mGuid.ma32[2], &mGuid.ma32[3]);
	}
};
