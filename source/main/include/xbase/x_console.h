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
	class xconsole_imp;

	struct xconsole_node
	{
								xconsole_node();
		
		bool					used() const;
		void					reset();

		void					add(xconsole_node* node, xconsole_imp* imp);
		void					remove(xconsole_node* node);

		class iterator
		{
		public:
									iterator(xconsole_node const*);

			bool					next();
			xconsole_imp*			imp();

		private:
			xconsole_node const*	mNode;
		};

		iterator				begin() const;

	private:
		xconsole_node*			mNext;
		xconsole_node*			mPrev;
		xconsole_imp*			mImp;
	};

	class xconsole
	{
	private:

		static xconsole_node	sConsoleList;

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
		typedef s32 (*ConsoleOutDelegate)(const uchar*, const uchar*);
		typedef s32(*ConsoleOut8Delegate)(const uchar8*, const uchar8*);
		typedef s32 (*ConsoleOut32Delegate)(const uchar32*, const uchar32*);

		static void				add(xconsole_imp* imp);							///< Returns old implementation
		static void				addDefault();									///< Attach default (debug, printf) implementation
		static void				remove(xconsole_imp* imp);						///< Returns old implementation, sets current implementation to NULL

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

		extern s32				write_uchar(const uchar* str, const uchar* str_end);
		extern s32				write_uchar8(const uchar8* str, const uchar8* str_end);
		extern s32				write_uchar32(const uchar32* str, const uchar32* str_end);
	};

	/// Interface class, has specific (or configurable) implementations for different environments/platforms
	class xconsole_imp
	{
	private:
		friend class xconsole;
		xconsole_node			mNode;

	public:
		virtual					~xconsole_imp() { }

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
