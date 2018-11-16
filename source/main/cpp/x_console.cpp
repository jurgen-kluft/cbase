/**
* @file x_console.cpp
* Core console
*/

#include "xbase/x_debug.h"
#include "xbase/x_memory.h"
#include "xbase/x_va_list.h"
#include "xbase/x_console.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

namespace xcore
{
	//#define XCONSOLE_LOCAL_STR_BUF(type, local_var_name, size)	type local_var_name##Buffer[size + 1]; local_var_name##Buffer[size] = '\0'; type* local_var_name = &local_var_name##Buffer[0]; type* local_var_name##_eos = &local_var_name##Buffer[size];
	#define XCONSOLE_LOCAL_STR_BUF(type, local_var_name, size)	type local_var_name##Buffer[size]; type##s local_var_name(local_var_name##Buffer, local_var_name##Buffer, &local_var_name##Buffer[size-1]);

	class xconsole_null : public xconsole
	{
	public:
								xconsole_null() { }
		virtual					~xconsole_null() { }

		virtual void			initialize();
		virtual void			shutdown();

		virtual s32				setColor(xconsole::EColor color);

		virtual void 			write(bool _value);
		virtual void 			write(f64 _value);
		virtual void 			write(s32 _value);
		virtual void 			write(s64 _value);
		virtual void 			write(f32 _value);

		virtual void 			write(u32 _value);
		virtual void 			write(u64 _value);

		virtual void 			write(const ascii::crunes& str);
		virtual void 			write(const ascii::crunes& fmt, const x_va_list& args);

		virtual void 			write(const utf8::crunes& str);
		virtual void 			write(const utf8::crunes& fmt, const x_va_list& args);

		virtual void 			write(const utf32::crunes& str);
		virtual void 			write(const utf32::crunes& fmt, const x_va_list& args);

		virtual void 			writeLine();
	};

	void				xconsole_null::initialize()
	{
	}

	void				xconsole_null::shutdown()
	{
	}

	s32					xconsole_null::setColor(xconsole::EColor color)
	{
		return 0;
	}

	void				xconsole_null::write(bool _value)
	{
	}

	void				xconsole_null::write(f64 _value)
	{
	}

	void				xconsole_null::write(s32 _value)
	{
	}

	void				xconsole_null::write(s64 _value)
	{
	}

	void				xconsole_null::write(f32 _value)
	{
	}

	void				xconsole_null::write(u32 _value)
	{
	}

	void				xconsole_null::write(u64 _value)
	{
	}

	void				xconsole_null::write(const ascii::crunes& str)
	{
	}

	void				xconsole_null::write(const ascii::crunes& fmt, const x_va_list& args)
	{
	}

	void				xconsole_null::write(const utf8::crunes& str)
	{
	}

	void				xconsole_null::write(const utf8::crunes& fmt, const x_va_list& args)
	{
	}

	void				xconsole_null::write(const utf32::crunes& str)
	{
	}

	void				xconsole_null::write(const utf32::crunes& fmt, const x_va_list& args)
	{
	}

	void				xconsole_null::writeLine()
	{
	}

	static xconsole_null 		sNullConsole;

	class xconsole_default : public xconsole
	{
		xconsole::xout*			mOut;
	public:
								xconsole_default(xconsole::xout* _out)
									: mOut(_out)
								{
								}

		virtual					~xconsole_default() { }

		virtual void			initialize();
		virtual void			shutdown();

		virtual s32				setColor(xconsole::EColor color);
	  
		virtual void 			write(bool _value);
		virtual void 			write(f64 _value);
		virtual void 			write(s32 _value);
		virtual void 			write(s64 _value);
		virtual void 			write(f32 _value);

		virtual void			write(u32 _value);
		virtual void			write(u64 _value);

		virtual void			write(const ascii::crunes& str);
		virtual void			write(const ascii::crunes& fmt, const x_va_list& args);

		virtual void			write(const utf8::crunes& str);
		virtual void			write(const utf8::crunes& fmt, const x_va_list& args);

		virtual void			write(const utf32::crunes& str);
		virtual void			write(const utf32::crunes& fmt, const x_va_list& args);

		virtual void			writeLine();
	};

	void				xconsole_default::initialize()
	{
	}

	void				xconsole_default::shutdown()
	{
	}

	s32    xconsole_default::setColor(xconsole::EColor color)
	{
		return mOut->color(color);
	}

	void    xconsole_default::write(bool _value)
	{
		ascii::pcrune truestr = "true";
		ascii::pcrune falsestr = "false";
		write(_value ? ascii::crunes(truestr, truestr+4) : ascii::crunes(falsestr, falsestr+5));
	}

	void    xconsole_default::write(f64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(ascii::rune, tmp, 256);
		ascii::to_string(tmp, _value, 2);
		write(tmp);
	}

	void    xconsole_default::write(s32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(ascii::rune, tmp, 64);
		ascii::to_string(tmp, _value, 2);
		write(tmp);
	}

	void    xconsole_default::write(s64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(ascii::rune, tmp, 64);
		ascii::to_string(tmp, _value, 2);
		write(tmp);
	}

	void    xconsole_default::write(f32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(ascii::rune, tmp, 256);
		ascii::to_string(tmp, _value, 2);
		write(tmp);
	}

	void    xconsole_default::write(u32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(ascii::rune, tmp, 256);
		ascii::to_string(tmp, _value, 2);
		write(tmp);
	}

	void    xconsole_default::write(u64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(ascii::rune, tmp, 256);
		ascii::to_string(tmp, _value, 2);
		write(tmp);
	}
	
	void    xconsole_default::write(const ascii::crunes& str)
	{
		mOut->write(str);
	}

	void    xconsole_default::write(const ascii::crunes& fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(ascii::rune, str, 512);
		ascii::vsprintf(str, fmt, args);
		mOut->write(str);
	}

	void    xconsole_default::write(const utf8::crunes& str)
	{
		mOut->write(str);
	}

	void    xconsole_default::write(const utf8::crunes& fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(utf8::rune, str, 512);
		utf8::vsprintf(str, utf8::crunes(fmt), args);
		utf8::crunes outstr(str);
		mOut->write(outstr);
	}

	void    xconsole_default::write(const utf32::crunes& str)
	{
		mOut->write(str);
	}

	void    xconsole_default::write(const utf32::crunes& fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(utf32::rune, str, 1024);
		utf32::vsprintf(str, utf32::crunes(fmt), args);
		utf32::crunes outstr(str);
		mOut->write(outstr);
	}


	void    xconsole_default::writeLine()
	{
		ascii::pcrune line = "\n";
		write(ascii::crunes(line, line + 1));
	}

	extern xconsole::xout*	gGetDefaultConsoleOut();

	void					xconsole::init_default_console()
	{
		static xconsole_default sDefaultConsole(gGetDefaultConsoleOut());
		console = &sDefaultConsole;
	}

};
/**
 *  END xCore namespace
 */

