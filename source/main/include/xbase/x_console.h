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

		static void	 			write(bool _value);
		static void	 			write(f64 _value);
		static void	 			write(s32 _value);
		static void	 			write(s64 _value);
		static void	 			write(f32 _value);
		static void	 			write(u32 _value);
		static void	 			write(u64 _value);
		static void	 			write(uchar32 _value);

		static void	 			write(const xcuchars& str);
		static void	 			write(const xcuchar8s& str);
		static void	 			write(const xcuchar32s& str);
	
		static void	 			write(const xcuchars& fmt, const x_va_list& args);
		static void	 			write(const xcuchars& fmt, X_VA_ARGS_16_DEF);

		static void	 			write(const xcuchar8s& fmt, const x_va_list& args);
		static void	 			write(const xcuchar8s& fmt, X_VA_ARGS_16_DEF);

		static void	 			write(const xcuchar32s& fmt, const x_va_list& args);
		static void	 			write(const xcuchar32s& fmt, X_VA_ARGS_16_DEF);

		static void 			writeLine();

		inline static void 		writeLine(bool _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(f64 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(s32 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(s64 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(f32 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(u32 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(u64 _value)							{ write(_value); writeLine(); }
		inline static void 		writeLine(uchar32 _value)						{ write(_value); writeLine(); }

		inline static void 		writeLine(const xcuchars& str)					{ write(str); writeLine(); }
		inline static void 		writeLine(const xcuchar32s& str)				{ write(str); writeLine(); }

		inline static void		writeLine(const xcuchars& fmt, const x_va_list& args) { write(fmt, args); writeLine(); }
		inline static void		writeLine(const xcuchars& fmt, X_VA_ARGS_16_DEF)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9);
			write(fmt, args); 
			writeLine();
		}
		inline static void		writeLine(const xcuchar32s& fmt, const x_va_list& args)	{ write(fmt, args); writeLine(); }
		inline static void		writeLine(const xcuchar32s& fmt, X_VA_ARGS_16_DEF)
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

		extern s32				write(xcuchars const& str);
		extern s32				write(xcuchar8s const& str);
		extern s32				write(xcuchar32s const& str);
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
		virtual void			write(uchar32 _value) = 0;

		virtual void			write(const xcuchars& buffer) = 0;
		virtual void			write(const xcuchar8s& buffer) = 0;
		virtual void			write(const xcuchar32s& buffer) = 0;

		virtual void			write(const xcuchars& fmt, const x_va_list& args) = 0;
		virtual void			write(const xcuchar8s& fmt, const x_va_list& args) = 0;
		virtual void			write(const xcuchar32s& fmt, const x_va_list& args) = 0;

		virtual void			writeLine() = 0;

		inline void				writeLine(bool _value)							{ write(_value); writeLine(); }
		inline void				writeLine(f64 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(s32 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(s64 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(f32 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(u32 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(u64 _value)							{ write(_value); writeLine(); }
		inline void				writeLine(char _value)							{ write(_value); writeLine(); }

		inline void				writeLine(const xcuchars&   str) { write(str); writeLine(); }
		inline void				writeLine(const xcuchar8s&  str) { write(str); writeLine(); }
		inline void				writeLine(const xcuchar32s& str) { write(str); writeLine(); }

		inline void				writeLine(const xcuchars&   format, const x_va_list& args) { write(format, args); writeLine(); }
		inline void				writeLine(const xcuchar8s&  format, const x_va_list& args) { write(format, args); writeLine(); }
		inline void				writeLine(const xcuchar32s& format, const x_va_list& args) { write(format, args); writeLine(); }
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif	///< __XBASE_CONSOLE_H__
