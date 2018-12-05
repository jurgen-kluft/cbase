#ifndef __XCORE_CHARS_H__
#define __XCORE_CHARS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_va_list.h"

namespace xcore
{
	namespace ascii
	{
		struct runes;
		struct crunes;

		s32				cprintf(crunes const& format, X_VA_ARGS_16_DEF);
		s32				vcprintf(crunes const& format, const x_va_list& args);

		void			sprintf(runes& dst, crunes const& format, X_VA_ARGS_16_DEF);
		void			vsprintf(runes& dst, crunes const& format, const x_va_list& args);

		void			printf(crunes const& str);
		void			printf(crunes const& format, X_VA_ARGS_16_DEF);
		void			printf(crunes const& format, const x_va_list& args);

		s32				sscanf(crunes & str, crunes const& format, X_VA_R_ARGS_16_DEF);
		s32				vsscanf(crunes & str, crunes const& format, const x_va_r_list& vr_args);
	}; ///< end of ascii namespace


	namespace utf16
	{
		struct runes;
		struct crunes;

		s32				cprintf(crunes const& format, X_VA_ARGS_16_DEF);
		s32				vcprintf(crunes const& format, const x_va_list& args);

		void			sprintf(runes& dst, crunes const& format, X_VA_ARGS_16_DEF);
		void			vsprintf(runes& dst, crunes const& format, const x_va_list& args);

		void			printf(crunes const& str);
		void			printf(crunes const& format, X_VA_ARGS_16_DEF);
		void			printf(crunes const& format, const x_va_list& args);

		s32				sscanf(crunes & str, crunes const& format, X_VA_R_ARGS_16_DEF);
		s32				vsscanf(crunes & str, crunes const& format, const x_va_r_list& vr_args);
	}; ///< end of utf8 namespace

	namespace utf32
	{
		struct runes;
		struct crunes;

		s32				cprintf(crunes const& format, X_VA_ARGS_16_DEF);
		s32				vcprintf(crunes const& format, const x_va_list& args);

		void			sprintf(runes& dst, crunes const& format, X_VA_ARGS_16_DEF);
		void			vsprintf(runes& dst, crunes const& format, const x_va_list& args);

		void			printf(crunes const& str);
		void			printf(crunes const& format, X_VA_ARGS_16_DEF);
		void			printf(crunes const& format, const x_va_list& args);

		s32				sscanf(crunes & str, crunes const& format, X_VA_R_ARGS_16_DEF);
		s32				vsscanf(crunes & str, crunes const& format, const x_va_r_list& vr_args);
	}; ///< end of utf32 namespace

}

#endif	///< __XCORE_CHARS_H__
