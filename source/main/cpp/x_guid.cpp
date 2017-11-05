//===============================================================================
// INCLUDES
//===============================================================================
#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_guid.h"
#include "xbase/x_va_list.h"
#include "xbase/x_string_ascii.h"

/**
 * xCore namespace
 */
namespace xcore
{
	//-------------------------------------------------------------------------------

	xguid& xguid::operator = (const xguid& other)
	{
		for (s32 i=0; i<4; ++i)
			mGuid[i] = other.mGuid[i];
		return *this;
	}

	//-------------------------------------------------------------------------------

	bool xguid::operator == (const xguid& other) const
	{
		return mGuid[0]==other.mGuid[0] && mGuid[1]==other.mGuid[1] && mGuid[2]==other.mGuid[2] && mGuid[3]==other.mGuid[3];
	}

	//-------------------------------------------------------------------------------

	bool xguid::operator != (const xguid& other) const
	{
		return mGuid[0]!=other.mGuid[0] || mGuid[1]!=other.mGuid[1] || mGuid[2]!=other.mGuid[2] || mGuid[3]!=other.mGuid[3];
	}

	//-------------------------------------------------------------------------------

	void xguid::toString( char* str, s32 str_max_len ) const
	{ 
		// high, word2, word1, low
		const uchar* fmt = (const uchar*)"%08X:%08X:%08X:%08X";
		const uchar* fmt_end = fmt + 19;
		ascii::sprintf((uchar*)str, (uchar const*)str + str_max_len, fmt, fmt_end, x_va(mGuid[0]), x_va(mGuid[1]), x_va(mGuid[2]), x_va(mGuid[3]) );
	}


	/**
     *------------------------------------------------------------------------------
	 * Author:
	 *     Virtuos
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

	void xguid::fromString( const char* strGuid )
	{
		setNull();
		if (strGuid == NULL)
			return;

		for (s32 i=0, s=0; s<32; i++)
		{
			const char c = strGuid[i];
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
				u32& _word = mGuid[(s>>3)];
				_word = (_word << 4) | (d & 0xf);
				++s;
			}
		}
	}
};
/**
 *  END xCore namespace
 */