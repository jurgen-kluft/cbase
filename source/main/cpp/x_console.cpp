/**
* @file x_console.cpp
* Core console
*/

#include "xbase/x_debug.h"
#include "xbase/x_memory_std.h"
#include "xbase/x_va_list.h"
#include "xbase/x_string_ascii.h"
#include "xbase/x_string_utf.h"
#include "xbase/x_console.h"
#include "xbase/x_chars.h"

/**
 * xCore namespace
 */
namespace xcore
{
//#define XCONSOLE_LOCAL_STR_BUF(type, local_var_name, size)	type local_var_name##Buffer[size + 1]; local_var_name##Buffer[size] = '\0'; type* local_var_name = &local_var_name##Buffer[0]; type* local_var_name##_eos = &local_var_name##Buffer[size];
	#define XCONSOLE_LOCAL_STR_BUF(type, local_var_name, size)	type##size local_var_name;

	class xiconsole_store_default : public xiconsole_store
	{
		enum EConfiguration
		{
			MAX_CONSOLES = 16
		};

		xiconsole*				mFixedNumberOfConsoles[MAX_CONSOLES];
		s32						mNumberOfConsoles;

	public:
		xiconsole_store_default()
			: mNumberOfConsoles(0)
		{
			for (s32 i = 0; i < MAX_CONSOLES; ++i)
				mFixedNumberOfConsoles[i] = NULL;
		}

		virtual					~xiconsole_store_default() { }

		virtual void			add(xiconsole* node)
		{
			if (mNumberOfConsoles < MAX_CONSOLES)
			{
				mFixedNumberOfConsoles[mNumberOfConsoles++] = node;
			}
		}

		virtual void			remove(xiconsole* console)
		{
			for (s32 i = 0; i < mNumberOfConsoles; ++i)
			{
				if (mFixedNumberOfConsoles[i] == console)
				{
					if ((i + 1) < mNumberOfConsoles)
					{
						xmem_utils::memmove(&mFixedNumberOfConsoles[i], &mFixedNumberOfConsoles[i + 1], (mNumberOfConsoles - 1 - i) * sizeof(void*));
					}
					mNumberOfConsoles -= 1;
					break;
				}
			}
		}

		virtual bool			iterate(iterator& iter, xiconsole*& console)
		{
			if (iter == 0)
			{
				iter = (void**)&mFixedNumberOfConsoles[0];
			}
			else
			{
				if ((xiconsole**)iter == &mFixedNumberOfConsoles[MAX_CONSOLES - 1])
				{
					console = NULL;
					return false;
				}
				++iter;
			}
			console = *(xiconsole**)iter;
			return console != NULL;
		}
	};


	class xconsole_null_imp : public xiconsole
	{
	public:
		xconsole_null_imp()
		{
		}

		virtual					~xconsole_null_imp() { }

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

		virtual void 			write(const xcuchars& str);
		virtual void 			write(const xcuchars& fmt, const x_va_list& args);

		virtual void 			write(const xcuchar8s& str);
		virtual void 			write(const xcuchar8s& fmt, const x_va_list& args);

		virtual void 			write(const xcuchar32s& str);
		virtual void 			write(const xcuchar32s& fmt, const x_va_list& args);

		virtual void 			writeLine();
	};

	void				xconsole_null_imp::initialize()
	{
	}

	void				xconsole_null_imp::shutdown()
	{
	}

	s32					xconsole_null_imp::setColor(xconsole::EColor color)
	{
		return 0;
	}

	void				xconsole_null_imp::write(bool _value)
	{
	}

	void				xconsole_null_imp::write(f64 _value)
	{
	}

	void				xconsole_null_imp::write(s32 _value)
	{
	}

	void				xconsole_null_imp::write(s64 _value)
	{
	}

	void				xconsole_null_imp::write(f32 _value)
	{
	}

	void				xconsole_null_imp::write(u32 _value)
	{
	}

	void				xconsole_null_imp::write(u64 _value)
	{
	}

	void				xconsole_null_imp::write(const xcuchars& str)
	{
	}

	void				xconsole_null_imp::write(const xcuchars& fmt, const x_va_list& args)
	{
	}

	void				xconsole_null_imp::write(const xcuchar8s& str)
	{
	}

	void				xconsole_null_imp::write(const xcuchar8s& fmt, const x_va_list& args)
	{
	}

	void				xconsole_null_imp::write(const xcuchar32s& str)
	{
	}

	void				xconsole_null_imp::write(const xcuchar32s& fmt, const x_va_list& args)
	{
	}

	void				xconsole_null_imp::writeLine()
	{
	}

	static xconsole_null_imp sNullConsole;


	class xiconsole_default : public xiconsole
	{
		xconsole::ConsoleOutDelegate mOut;
		xconsole::ConsoleOut8Delegate mOut8;
		xconsole::ConsoleOut32Delegate mOut32;
		xconsole::ConsoleColorDelegate mColor;

	public:
								xiconsole_default()
								{
									mOut = xconsole_out::write;
									mOut8 = xconsole_out::write;
									mOut32 = xconsole_out::write;
									mColor = xconsole_out::color;
								}

		virtual					~xiconsole_default() { }

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

		virtual void			write(const xcuchars& str);
		virtual void			write(const xcuchars& fmt, const x_va_list& args);

		virtual void			write(const xcuchar8s& str);
		virtual void			write(const xcuchar8s& fmt, const x_va_list& args);

		virtual void			write(const xcuchar32s& str);
		virtual void			write(const xcuchar32s& fmt, const x_va_list& args);

		virtual void			writeLine();
	};

	void				xiconsole_default::initialize()
	{
	}

	void				xiconsole_default::shutdown()
	{
	}

	s32    xiconsole_default::setColor(xconsole::EColor color)
	{
		return mColor(color);
	}

	void    xiconsole_default::write(bool _value)
	{
		write(_value ? xcuchars("true") : xcuchars("false"));
	}

	void    xiconsole_default::write(f64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(xuchars, tmp, 256);
		ascii::to_string(tmp.chars(), _value, 2);
		write(tmp.cchars());
	}

	void    xiconsole_default::write(s32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(xuchars, tmp, 64);
		ascii::to_string(tmp.chars(), _value, 2);
		write(tmp.cchars());
	}

	void    xiconsole_default::write(s64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(xuchars, tmp, 64);
		ascii::to_string(tmp.chars(), _value, 2);
		write(tmp.cchars());
	}

	void    xiconsole_default::write(f32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(xuchars, tmp, 256);
		ascii::to_string(tmp.chars(), _value, 2);
		write(tmp.cchars());
	}

	void    xiconsole_default::write(u32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(xuchars, tmp, 256);
		ascii::to_string(tmp.chars(), _value, 2);
		write(tmp.cchars());
	}

	void    xiconsole_default::write(u64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(xuchars, tmp, 256);
		ascii::to_string(tmp.chars(), _value, 2);
		write(tmp.cchars());
	}
	
	void    xiconsole_default::write(const xcuchars& str)
	{
		mOut(str);
	}

	void    xiconsole_default::write(const xcuchars& fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(xuchars, str, 512);
		ascii::vsprintf(str.chars(), fmt, args);
		mOut(str.cchars());
	}

	void    xiconsole_default::write(const xcuchar8s& str)
	{
		mOut8(str);
	}

	void    xiconsole_default::write(const xcuchar8s& fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(xuchar8s, str, 512);
		utf8::vsprintf(str.chars(), xcuchar8s(fmt), args);
		mOut8(str.cchars());
	}

	void    xiconsole_default::write(const xcuchar32s& str)
	{
		mOut32(str);
	}

	void    xiconsole_default::write(const xcuchar32s& fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(xuchar32s, str, 1024);
		utf32::vsprintf(str.chars(), xcuchar32s(fmt), args);
		mOut32(str.cchars());
	}


	void    xiconsole_default::writeLine()
	{
		write(xcuchars("\n"));
	}


	static xiconsole_default		sDefaultConsole;
	static xiconsole_store_default	sDefaultConsoleStore;
	static xiconsole_store*			sConsoleStore = &sDefaultConsoleStore;

	void				xconsole::set_console_store(xiconsole_store* store)
	{
		if (store == NULL)
			sConsoleStore = &sDefaultConsoleStore;
		else
			sConsoleStore = store;
	}

	void				xconsole::add_default_console()
	{
		sConsoleStore->add(&sDefaultConsole);
	}

	void				xconsole::setColor(EColor color)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->setColor(color);
	}

	void				xconsole::restoreColor()
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->setColor(xconsole::NORMAL);
	}

	void 				xconsole::write(bool _value)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(_value);
	}

	void    xconsole::write(f64 _value)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(_value);
	}

	void    xconsole::write(s32 _value)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(_value);
	}

	void    xconsole::write(s64 _value)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(_value);
	}

	void    xconsole::write(f32 _value)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(_value);
	}

	void    xconsole::write(u32 _value)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(_value);
	}

	void    xconsole::write(u64 _value)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(_value);
	}

	void    xconsole::write(const xcuchars& str)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(str);
	}

	void    xconsole::write(const xcuchars& fmt, const x_va_list& args)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(fmt, args);
	}

	void    xconsole::write(const xcuchars& fmt, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
		const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
	{
		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16);
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(fmt, args);
	}

	void    xconsole::write(const xcuchar8s& str)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(str);
	}

	void    xconsole::write(const xcuchar8s& fmt, const x_va_list& args)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(fmt, args);
	}

	void    xconsole::write(const xcuchar8s& fmt, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
		const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
	{
		x_va_list args(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16);
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(fmt, args);
	}

	void    xconsole::write(const xcuchar32s& str)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(str);
	}

	void    xconsole::write(const xcuchar32s& fmt, const x_va_list& args)
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(fmt, args);
	}

	void    xconsole::write(const xcuchar32s& fmt, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
		const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
	{
		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16);
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->write(fmt, args);
	}


	void    xconsole::writeLine()
	{
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			console->writeLine();
	}
};
/**
 *  END xCore namespace
 */

