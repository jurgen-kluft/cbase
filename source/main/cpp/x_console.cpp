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

/**
 * xCore namespace
 */
namespace xcore
{
	#define XCONSOLE_LOCAL_STR_BUF(type, local_var_name, size)	type local_var_name##Buffer[size + 1]; local_var_name##Buffer[size] = '\0'; type* local_var_name = &local_var_name##Buffer[0]; type* local_var_name##_eos = &local_var_name##Buffer[size];

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

		virtual s32 			write(bool _value);
		virtual s32 			write(f64 _value);
		virtual s32 			write(s32 _value);
		virtual s32 			write(s64 _value);
		virtual s32 			write(f32 _value);

		virtual s32 			write(u32 _value);
		virtual s32 			write(u64 _value);
		virtual s32 			write(uchar32 _value);
		virtual s32 			write(const char* str, const char* str_end);
		virtual s32 			write(const char* fmt, const x_va_list& args);
		virtual s32 			write(const uchar8* str, const uchar8* str_end);
		virtual s32 			write(const uchar8* fmt, const x_va_list& args);
		virtual s32 			write(const uchar32* str, const uchar32* str_end);
		virtual s32 			write(const uchar32* fmt, const x_va_list& args);

		virtual s32 			writeLine();
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

	s32					xconsole_null_imp::write(bool _value)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(f64 _value)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(s32 _value)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(s64 _value)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(f32 _value)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(u32 _value)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(u64 _value)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(uchar32 _value)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(const char* str, const char* str_end)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(const char* fmt, const x_va_list& args)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(const uchar8* str, const uchar8* str_end)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(const uchar8* fmt, const x_va_list& args)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(const uchar32* str, const uchar32* str_end)
	{
		return 0;
	}

	s32 				xconsole_null_imp::write(const uchar32* fmt, const x_va_list& args)
	{
		return 0;
	}

	s32 				xconsole_null_imp::writeLine()
	{
		return 0;
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
									mOut = xconsole_out::write_uchar;
									mOut8 = xconsole_out::write_uchar8;
									mOut32 = xconsole_out::write_uchar32;
									mColor = xconsole_out::color;
								}

		virtual					~xiconsole_default() { }

		virtual void			initialize();
		virtual void			shutdown();

		virtual s32				setColor(xconsole::EColor color);
	  
		virtual s32 			write(bool _value);
		virtual s32 			write(f64 _value);
		virtual s32 			write(s32 _value);
		virtual s32 			write(s64 _value);
		virtual s32 			write(f32 _value);

		virtual s32 			write(u32 _value);
		virtual s32 			write(u64 _value);
		virtual s32 			write(uchar32 _value);

		virtual s32 			write(const char* str, const char* str_end);
		virtual s32 			write(const char* fmt, const x_va_list& args);

		virtual s32 			write(const uchar8* str, const uchar8* str_end);
		virtual s32 			write(const uchar8* fmt, const x_va_list& args);

		virtual s32 			write(const uchar32* str, const uchar32* str_end);
		virtual s32 			write(const uchar32* fmt, const x_va_list& args);

		virtual s32 			writeLine();
	};

	void				xiconsole_default::initialize()
	{
	}

	void				xiconsole_default::shutdown()
	{
	}

	s32					xiconsole_default::setColor(xconsole::EColor color)
	{
		return mColor(color);
	}

	s32					xiconsole_default::write(bool _value)
	{
		return write(_value ? "true" : "false", NULL);
	}

	s32 				xiconsole_default::write(f64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 256);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xiconsole_default::write(s32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 64);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xiconsole_default::write(s64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 64);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xiconsole_default::write(f32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 256);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xiconsole_default::write(u32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 256);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xiconsole_default::write(u64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 256);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}
	
	s32 				xiconsole_default::write(uchar32 _value)
	{
		uchar32 s[3];
		s[0] = _value;
		s[1] = '\0';
		s[2] = '\0';
		return mOut32(s, &s[1]);
	}

	s32 				xiconsole_default::write(const char* str, const char* str_end)
	{
		return mOut((const uchar*)str, (const uchar*)str_end);
	}

	s32 				xiconsole_default::write(const char* fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, str, 512);
		s32 len = ascii::vsprintf(str, str_eos, (const uchar*)fmt, ascii::len((const uchar*)fmt), args);
		return mOut(str, str + len);
	}

	s32 				xiconsole_default::write(const uchar8* str, const uchar8* str_end)
	{
		return mOut8((const uchar8*)str, (const uchar8*)str_end);
	}

	s32 				xiconsole_default::write(const uchar8* fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar8, str, 512);
		s32 len = utf8::vsprintf(str, str_eos, (const uchar8*)fmt, utf8::len((const uchar8*)fmt), args);
		return mOut8(str, str + len);
	}

	s32 				xiconsole_default::write(const uchar32* str, const uchar32* str_end)
	{
		return mOut32(str, str_end);
	}

	s32 				xiconsole_default::write(const uchar32* fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar32, str, 1024);
		s32 len = utf32::vsprintf(str, str_eos, (const uchar32*)fmt, utf32::len(fmt), args);
		return mOut32(str, str + len);
	}


	s32 				xiconsole_default::writeLine()
	{
		return write("\n", NULL);
	}


	static xiconsole_default		sDefaultConsole;
	static xiconsole_store_default	sDefaultConsoleStore;
	static xiconsole_store*			sConsoleStore = &sDefaultConsoleStore;

	void				xconsole::set_console_store(xiconsole_store* store)
	{
		if (store == NULL)
			sConsoleStore == &sDefaultConsoleStore;
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

	s32 				xconsole::write(bool _value)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(_value);
		return r;
	}

	s32 				xconsole::write(f64 _value)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(_value);
		return r;
	}

	s32 				xconsole::write(s32 _value)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(_value);
		return r;
	}

	s32 				xconsole::write(s64 _value)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(_value);
		return r;
	}

	s32 				xconsole::write(f32 _value)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(_value);
		return r;
	}

	s32 				xconsole::write(u32 _value)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(_value);
		return r;
	}

	s32 				xconsole::write(u64 _value)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(_value);
		return r;
	}


	s32 				xconsole::write(uchar32 _value)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(_value);
		return r;
	}

	s32 				xconsole::write(const char* str, const char* str_end)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(str, str_end);
		return r;
	}

	s32 				xconsole::write(const char* fmt, const x_va_list& args)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(fmt, args);
		return r;
	}

	s32 				xconsole::write(const char* fmt, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, 
		const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
	{
		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16);
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(fmt, args);
		return r;
	}

	s32 				xconsole::write(const uchar8* str, const uchar8* str_end)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(str, str_end);
		return r;
	}

	s32 				xconsole::write(const uchar8* fmt, const x_va_list& args)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(fmt, args);
		return r;
	}

	s32 				xconsole::write(const uchar8* fmt, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8,
		const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
	{
		x_va_list args(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16);
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(fmt, args);
		return r;
	}

	s32 				xconsole::write(const uchar32* str, const uchar32* str_end)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(str, str_end);
		return r;
	}

	s32 				xconsole::write(const uchar32* fmt, const x_va_list& args)
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(fmt, args);
		return r;
	}

	s32 				xconsole::write(const uchar32* fmt, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, 
		const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
	{
		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16);
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->write(fmt, args);
		return r;
	}


	s32 				xconsole::writeLine()
	{
		s32 r = 0;
		xiconsole_store::iterator iter = NULL;
		xiconsole* console;
		while (sConsoleStore->iterate(iter, console))
			r = console->writeLine();
		return r;
	}
};
/**
 *  END xCore namespace
 */

