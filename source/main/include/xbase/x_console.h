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
		typedef s32 (*ConsoleOutDelegate)(ascii::crunes const& str);
		typedef s32 (*ConsoleOut8Delegate)(utf8::crunes const& str);
		typedef s32 (*ConsoleOut32Delegate)(utf32::crunes const& str);

		static void				set_console_store(xiconsole_store* store);
		static void				add_default_console();

		static void				setColor(EColor color);
		static void				restoreColor();

		static void	 			write(bool _value);
		static void	 			write(f64 _value);
		static void	 			write(s32 _value);
		static void	 			write(s64 _value);
		static void	 			write(f32 _value);
		static void	 			write(u32 _value);
		static void	 			write(u64 _value);

		static void	 			write(const ascii::crunes& str);
		static void	 			write(const utf8::crunes& str);
		static void	 			write(const utf32::crunes& str);
	
		static void	 			write(const ascii::crunes& fmt, const x_va_list& args);
		static void	 			write(const ascii::crunes& fmt, X_VA_ARGS_16_DEF);

		static void	 			write(const utf8::crunes& fmt, const x_va_list& args);
		static void	 			write(const utf8::crunes& fmt, X_VA_ARGS_16_DEF);

		static void	 			write(const utf32::crunes& fmt, const x_va_list& args);
		static void	 			write(const utf32::crunes& fmt, X_VA_ARGS_16_DEF);

		static void 			writeLine();

		inline static void 		writeLine(bool _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(f64 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(s32 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(s64 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(f32 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(u32 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(u64 _value)							{ write(_value); writeLine(); }

		inline static void 		writeLine(const ascii::crunes& str)					{ write(str); writeLine(); }
		inline static void 		writeLine(const utf32::crunes& str)				{ write(str); writeLine(); }

		inline static void		writeLine(const ascii::crunes& fmt, const x_va_list& args) { write(fmt, args); writeLine(); }
		inline static void		writeLine(const ascii::crunes& fmt, X_VA_ARGS_16_DEF)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9);
			write(fmt, args); 
			writeLine();
		}
		inline static void		writeLine(const utf32::crunes& fmt, const x_va_list& args)	{ write(fmt, args); writeLine(); }
		inline static void		writeLine(const utf32::crunes& fmt, X_VA_ARGS_16_DEF)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9);
			write(fmt, args); 
			writeLine();
		}
	};

	/// Platform/Configuration specific console 'out'
	namespace xconsole_out
	{
		extern s32				color(xconsole::EColor color);

		extern s32				write(ascii::crunes const& str);
		extern s32				write(utf8::crunes const& str);
		extern s32				write(utf32::crunes const& str);
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

		virtual void			write(bool _value) = 0;
		virtual void			write(f64 _value) = 0;
		virtual void			write(s32 _value) = 0;
		virtual void			write(s64 _value) = 0;
		virtual void			write(f32 _value) = 0;
		virtual void			write(u32 _value) = 0;
		virtual void			write(u64 _value) = 0;

		virtual void			write(const ascii::crunes& buffer) = 0;
		virtual void			write(const utf8::crunes& buffer) = 0;
		virtual void			write(const utf32::crunes& buffer) = 0;

		virtual void			write(const ascii::crunes& fmt, const x_va_list& args) = 0;
		virtual void			write(const utf8::crunes& fmt, const x_va_list& args) = 0;
		virtual void			write(const utf32::crunes& fmt, const x_va_list& args) = 0;

		virtual void			writeLine() = 0;

		inline void				writeLine(bool _value)							{ write(_value); writeLine(); }
		inline void				writeLine(f64 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(s32 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(s64 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(f32 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(u32 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(u64 _value)							{ write(_value); writeLine(); }

		inline void				writeLine(const ascii::crunes&   str) { write(str); writeLine(); }
		inline void				writeLine(const utf8::crunes&  str) { write(str); writeLine(); }
		inline void				writeLine(const utf32::crunes& str) { write(str); writeLine(); }

		inline void				writeLine(const ascii::crunes&   format, const x_va_list& args) { write(format, args); writeLine(); }
		inline void				writeLine(const utf8::crunes&  format, const x_va_list& args) { write(format, args); writeLine(); }
		inline void				writeLine(const utf32::crunes& format, const x_va_list& args) { write(format, args); writeLine(); }
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif	///< __XBASE_CONSOLE_H__
