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

		typedef s32 (*ConsoleOutDelegate)(const char*, s32);
		typedef s32 (*ConsoleOut8Delegate)(const ustr8*, s32);
		typedef s32 (*ConsoleColorDelegate)(xconsole::EColor);

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
		static s32	 			write(char _value);

		static s32	 			write(const char* str);
		static s32	 			write(s32 index, s32 count, const char* str);
		static s32	 			write(const ustr8* str);
		static s32	 			write(s32 index, s32 count, const ustr8* str);
	
		static s32	 			write(const char* formatString, const x_va_list& args);
		static s32	 			write(const char* formatString, const x_va& v1             , const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty,
																const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty, const x_va& v11=x_va::sEmpty, const x_va& v12=x_va::sEmpty, const x_va& v13=x_va::sEmpty, const x_va& v14=x_va::sEmpty, const x_va& v15=x_va::sEmpty, const x_va& v16=x_va::sEmpty);

		static s32	 			write(const ustr8* formatString, const x_va_list& args);
		static s32	 			write(const ustr8* formatString, const x_va& v1             , const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty,
																const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty, const x_va& v11=x_va::sEmpty, const x_va& v12=x_va::sEmpty, const x_va& v13=x_va::sEmpty, const x_va& v14=x_va::sEmpty, const x_va& v15=x_va::sEmpty, const x_va& v16=x_va::sEmpty);

		static s32 				writeLine();

		inline static s32 		writeLine(bool _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(f64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(s32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(s64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(f32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(u32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(u64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline static s32 		writeLine(char _value)							{ s32 l = write(_value); l+=writeLine(); return l; }

		inline static s32 		writeLine(const char* str)						{ s32 l = write(str); l += writeLine(); return l; }
		inline static s32 		writeLine(s32 index, s32 count, const char* str){ s32 l = write(index, count, str); l += writeLine(); return l; }
		inline static s32 		writeLine(const ustr8* str)						{ s32 l = write(str); l += writeLine(); return l; }
		inline static s32 		writeLine(s32 index, s32 count, const ustr8* str){ s32 l = write(index, count, str); l += writeLine(); return l; }

		inline static s32 		writeLine(const char* formatString, const x_va_list& args){ s32 l = write(formatString, args); l += writeLine(); return l; }
		inline static s32 		writeLine(const char* formatString, const x_va& v1, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9);
			s32 l = write(formatString, args); 
			l += writeLine();
			return l; 
		}
		inline static s32 		writeLine(const ustr8* formatString, const x_va_list& args){ s32 l = write(formatString, args); l += writeLine(); return l; }
		inline static s32 		writeLine(const ustr8* formatString, const x_va& v1, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty)
		{
			x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9);
			s32 l = write(formatString, args); 
			l += writeLine();
			return l; 
		}
	};

	/// Platform/Configuration specific console 'out'
	namespace xconsole_out
	{
		extern s32				color(xconsole::EColor color);
		extern s32				write(const char* str, s32 len);
		extern s32				write8(const ustr8* str, s32 len);
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
		virtual s32 			write(char _value) = 0;

		virtual s32 			write(const char* buffer) = 0;
		virtual s32 			write(s32 index, s32 count, const char* buffer) = 0;
		virtual s32 			write(const ustr8* buffer) = 0;
		virtual s32 			write(s32 index, s32 count, const ustr8* buffer) = 0;

		virtual s32 			write(const char* formatString, const x_va_list& args) = 0;
		virtual s32 			write(const ustr8* formatString, const x_va_list& args) = 0;

		virtual s32 			writeLine() = 0;

		inline s32 				writeLine(bool _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(f64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(s32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(s64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(f32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(u32 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(u64 _value)							{ s32 l = write(_value); l+=writeLine(); return l; }
		inline s32 				writeLine(char _value)							{ s32 l = write(_value); l+=writeLine(); return l; }

		inline s32 				writeLine(const char* str)						{ s32 l = write(str); l+=writeLine(); return l; }
		inline s32 				writeLine(s32 index, s32 count, const char* str){ s32 l = write(str); l+=writeLine(); return l; }
		inline s32 				writeLine(const ustr8* str)						{ s32 l = write(str); l+=writeLine(); return l; }
		inline s32 				writeLine(s32 index, s32 count, const ustr8* str){ s32 l = write(str); l+=writeLine(); return l; }

		inline s32 				writeLine(const char* format, const x_va_list& args) { s32 l = write(format, args); l+=writeLine(); return l; }
		inline s32 				writeLine(const ustr8* format, const x_va_list& args) { s32 l = write(format, args); l+=writeLine(); return l; }
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif	///< __XBASE_CONSOLE_H__
