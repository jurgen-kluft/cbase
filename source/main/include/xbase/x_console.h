/**
* @file Head file x_console.h
* Core Console for Input, Output and Error
*/

#ifndef __XBASE_CONSOLE_H__
#define __XBASE_CONSOLE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_va_list.h"

/**
 * xCore namespace
 */
namespace xcore
{
	// Forward declares
	class xiconsole;

	class xiconsole_store
	{
	public:
		virtual					~xiconsole_store() { }
		
		virtual void			add(xiconsole* node) = 0;
		virtual void			remove(xiconsole* node) = 0;

		typedef	void**			iterator;
		virtual bool			iterate(iterator& iter, xiconsole*& ) = 0;
	};

	class xconsole
	{
	public:
		enum EColor
		{
			NORMAL     = 0,
			BLACK      , 
			RED        , 
			GREEN      ,
			YELLOW     ,
			BLUE       ,
			MAGENTA    ,
			CYAN       ,
			LTGREY     ,

			BG_BLACK   ,
			BG_RED     ,
			BG_GREEN   ,
			BG_YELLOW  ,
			BG_BLUE    ,
			BG_MAGENTA ,
			BG_CYAN    ,
			BG_LTGREY  ,
		};

		typedef s32(*ConsoleColorDelegate)(xconsole::EColor);
		typedef s32 (*ConsoleOutDelegate)(xcuchars const& str);
		typedef s32 (*ConsoleOut8Delegate)(xcuchar8s const& str);
		typedef s32 (*ConsoleOut32Delegate)(xcuchar32s const& str);

		static void				set_console_store(xiconsole_store* store);
		static void				add_default_console();

		static void				setColor(EColor color);
		static void				restoreColor();

		static s32	 			write(bool _value);
		static s32	 			write(f64 _value);
		static s32	 			write(s32 _value);
		static s32	 			write(s64 _value);
		static s32	 			write(f32 _value);
		static s32	 			write(u32 _value);
		static s32	 			write(u64 _value);
		static s32	 			write(uchar32 _value);

		static s32	 			write(const char* str, const char* str_end);
		static s32	 			write(const uchar8* str, const uchar8* str_end);
		static s32	 			write(const uchar32* str, const uchar32* str_end);
	
		static s32	 			write(const char* fmt, const x_va_list& args);
		static s32	 			write(const char* fmt, X_VA_ARGS_16_DEF);

		static s32	 			write(const uchar8* fmt, const x_va_list& args);
		static s32	 			write(const uchar8* fmt, X_VA_ARGS_16_DEF);

		static s32	 			write(const uchar32* fmt, const x_va_list& args);
		static s32	 			write(const uchar32* fmt, X_VA_ARGS_16_DEF);

		static s32 				writeLine();

		inline static s32 		writeLine(bool _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(f64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(s32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(s64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(f32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(u32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(u64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(uchar32 _value)						{ s32 l = write(_value); l+=writeLine(); return l; }

		inline static s32 		writeLine(const char* str, const char* str_end)			{ s32 l = write(str, str_end); l += writeLine(); return l; }
		inline static s32 		writeLine(const uchar32* str, const uchar32* str_end)	{ s32 l = write(str, str_end); l += writeLine(); return l; }

		inline static s32 		writeLine(const char* fmt, const x_va_list& args){ s32 l = write(fmt, args); l += writeLine(); return l; }
		inline static s32 		writeLine(const char* fmt, X_VA_ARGS_16_DEF)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9);
			s32 l = write(fmt, args); 
			l += writeLine();
			return l; 
		}
		inline static s32 		writeLine(const uchar32* fmt, const x_va_list& args){ s32 l = write(fmt, args); l += writeLine(); return l; }
		inline static s32 		writeLine(const uchar32* fmt, X_VA_ARGS_16_DEF)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9);
			s32 l = write(fmt, args); 
			l += writeLine();
			return l; 
		}
	};

	/// Platform/Configuration specific console 'out'
	namespace xconsole_out
	{
		extern s32				color(xconsole::EColor color);

		extern s32				write_uchar(xcuchars const& str);
		extern s32				write_uchar8(xcuchar8s const& str);
		extern s32				write_uchar32(xcuchar32s const& str);
	};

	/// Interface class, has specific (or configurable) implementations for different environments/platforms
	class xiconsole
	{
	private:
		friend class xconsole;

	public:
		virtual					~xiconsole() { }

		virtual void			initialize() = 0;
		virtual void			shutdown() = 0;
		
		virtual s32				setColor(xconsole::EColor color) = 0;

		virtual s32 			write(bool _value) = 0;
		virtual s32 			write(f64 _value) = 0;
		virtual s32 			write(s32 _value) = 0;
		virtual s32 			write(s64 _value) = 0;
		virtual s32 			write(f32 _value) = 0;
		virtual s32 			write(u32 _value) = 0;
		virtual s32 			write(u64 _value) = 0;
		virtual s32 			write(uchar32 _value) = 0;

		virtual s32 			write(const char* buffer, const char* buffer_end) = 0;
		virtual s32 			write(const uchar8* buffer, const uchar8* buffer_end) = 0;
		virtual s32 			write(const uchar32* buffer, const uchar32* buffer_end) = 0;

		virtual s32 			write(const char* fmt, const x_va_list& args) = 0;
		virtual s32 			write(const uchar8* fmt, const x_va_list& args) = 0;
		virtual s32 			write(const uchar32* fmt, const x_va_list& args) = 0;

		virtual s32 			writeLine() = 0;

		inline s32 				writeLine(bool _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(f64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(s32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(s64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(f32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(u32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(u64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(char _value)							{ s32 l = write(_value); l+=writeLine(); return l; }

		inline s32 				writeLine(const char* str, const char* str_end) { s32 l = write(str, str_end); l += writeLine(); return l; }
		inline s32 				writeLine(const uchar8* str, const uchar8* str_end) { s32 l = write(str, str_end); l += writeLine(); return l; }
		inline s32 				writeLine(const uchar32* str, const uchar32* str_end)	{ s32 l = write(str, str_end); l+=writeLine(); return l; }

		inline s32 				writeLine(const char* format, const x_va_list& args) { s32 l = write(format, args); l += writeLine(); return l; }
		inline s32 				writeLine(const uchar8* format, const x_va_list& args) { s32 l = write(format, args); l += writeLine(); return l; }
		inline s32 				writeLine(const uchar32* format, const x_va_list& args)	{ s32 l = write(format, args); l+=writeLine(); return l; }
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif	///< __XBASE_CONSOLE_H__
