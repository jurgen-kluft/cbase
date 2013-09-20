#ifndef __X_GUID_H__
#define __X_GUID_H__ 
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"

/**
 * xCore namespace
 */
namespace xcore
{
	class xcstring;
	class xccstring;

	/**
	 * Author:
	 *     Virtuos
	 * Note: For a generator, take a look here http://cvs.ossp.org/dir?d=ossp-pkg/uuid
	 * Description:
	 *     This class described a 128bit guid. The guid is not created here, this is
	 *     a data container holding the guid value. From the point of view of xcore 
	 *     this defines a system type.
	 */
	struct xguid
	{
	public:
					xguid();
					xguid(const xguid&);
					xguid(u32 wordh, u32 word2, u32 word1, u32 wordl);         

		void		setNull(void);                
		bool		isNull(void) const;          

		void		set(u32 wordh, u32 word2, u32 word1, u32 wordl);
		void		get(u32& wordh, u32& word2, u32& word1, u32& wordl) const;

		xguid&		operator = (const xguid&);

		bool		operator == (const xguid&) const;
		bool		operator != (const xguid&) const;

		void		toString(char* str, s32 str_max_len) const;     
		void		fromString(const char*);

	private:
		u32			mGuid[4];
	};

	/**
	 * INCLUDE THE INLINE FUNCTIONS
	 */
	#include "private\x_guid_inline.h"

//==============================================================================
// END xCore namespace
//==============================================================================
};
/**
 *  END xCore namespace
 */

//==============================================================================
// END
//==============================================================================
#endif
