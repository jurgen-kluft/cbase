/**
* @file x_console.cpp
* Core console
*/

#ifdef SPU

#include "xbase\x_console.h"

// empty function so other test programs will compile..
namespace xcore
{
	void xconsole::addDefault()
	{

	}
}

#else

#include "xbase\x_debug.h"
#include "xbase\x_va_list.h"
#include "xbase\x_string_ascii.h"
#include "xbase\x_string_utf.h"
#include "xbase\x_console.h"

/**
 * xCore namespace
 */
namespace xcore
{
	#define XCONSOLE_LOCAL_STR_BUF(type, local_var_name, size)	type local_var_name##Buffer[size + 1]; local_var_name##Buffer[size] = '\0'; type* local_var_name = &local_var_name##Buffer[0]; type* local_var_name##_eos = &local_var_name##Buffer[size];

	xconsole_node::xconsole_node()
		: mNext(NULL)
		, mPrev(NULL)
		, mImp(NULL)
	{
	}

	bool					xconsole_node::used() const
	{
		return mImp != NULL;
	}

	void					xconsole_node::reset()
	{
		mNext = mPrev = NULL;
		mImp = NULL;
	}

	void					xconsole_node::add(xconsole_node* node, xconsole_imp* imp)
	{
		ASSERT(node->mNext==NULL && node->mPrev==NULL && node->mImp==NULL); 
		
		node->mImp = imp;

		if (mNext!=NULL)
		{
			mNext->mPrev = node;
			node->mNext = mNext;
		}

		mNext = node;
		node->mPrev = this;
	}

	void					xconsole_node::remove(xconsole_node* node)
	{
		if (node->mPrev!=NULL)
			node->mPrev->mNext = node->mNext;
		if (node->mNext!=NULL)
			node->mNext->mPrev = mPrev;

		node->reset();
	}

	xconsole_node::iterator::iterator(xconsole_node const* node)
		: mNode(node)
	{
	}

	bool					xconsole_node::iterator::next()
	{
		mNode = mNode->mNext;
		return mNode!=NULL;
	}

	xconsole_imp*			xconsole_node::iterator::imp()
	{
		return mNode->mImp;
	}

	xconsole_node::iterator	xconsole_node::begin() const
	{
		return iterator(this);
	}

	class xconsole_null_imp : public xconsole_imp
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
		virtual s32 			write(const char* str);
		virtual s32 			write(const char* str, const char* str_end);
		virtual s32 			write(const char* fmt, const x_va_list& args);
		virtual s32 			write(const uchar32* str);
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

	s32 				xconsole_null_imp::write(const char* str)
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

	s32 				xconsole_null_imp::write(const uchar32* str)
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


	class xconsole_default_imp : public xconsole_imp
	{
		xconsole::ConsoleOutDelegate mOut;
		xconsole::ConsoleOut8Delegate mOut8;
		xconsole::ConsoleOut32Delegate mOut32;
		xconsole::ConsoleColorDelegate mColor;

	public:
								xconsole_default_imp()
								{
									mOut = xconsole_out::write_uchar;
									mOut8 = xconsole_out::write_uchar8;
									mOut32 = xconsole_out::write_uchar32;
									mColor = xconsole_out::color;
								}

		virtual					~xconsole_default_imp() { }

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

		virtual s32 			write(const char* str);
		virtual s32 			write(const char* str, const char* str_end);
		virtual s32 			write(const char* fmt, const x_va_list& args);

		virtual s32 			write(const uchar32* str);
		virtual s32 			write(const uchar32* str, const uchar32* str_end);
		virtual s32 			write(const uchar32* fmt, const x_va_list& args);

		virtual s32 			writeLine();
	};

	void				xconsole_default_imp::initialize()
	{
	}

	void				xconsole_default_imp::shutdown()
	{
	}

	s32					xconsole_default_imp::setColor(xconsole::EColor color)
	{
		return mColor(color);
	}

	s32					xconsole_default_imp::write(bool _value)
	{
		return write(_value ? "true" : "false");
	}

	s32 				xconsole_default_imp::write(f64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 256);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xconsole_default_imp::write(s32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 64);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xconsole_default_imp::write(s64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 64);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xconsole_default_imp::write(f32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 256);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xconsole_default_imp::write(u32 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 256);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}

	s32 				xconsole_default_imp::write(u64 _value)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, tmp, 256);
		ascii::to_string(tmp, tmp_eos, tmp_eos, _value, 2);
		return write(tmpBuffer);
	}
	
	s32 				xconsole_default_imp::write(uchar32 _value)
	{
		uchar32 s[3];
		s[0] = _value;
		s[1] = '\0';
		s[2] = '\0';
		return mOut32(s, &s[1]);
	}

	s32 				xconsole_default_imp::write(const char* str)
	{
		return mOut((const uchar*)str, ascii::len((const uchar*)str));
	}

	s32 				xconsole_default_imp::write(const char* str, const char* str_end)
	{
		return mOut((const uchar*)str, (const uchar*)str_end);
	}

	s32 				xconsole_default_imp::write(const char* fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar, str, 512);
		s32 len = ascii::vsprintf(str, str_eos, (const uchar*)fmt, ascii::len((const uchar*)fmt), args);
		return mOut(str, str + len);
	}

	s32 				xconsole_default_imp::write(const uchar32* str)
	{
		return mOut32(str, utf32::len(str));
	}

	s32 				xconsole_default_imp::write(const uchar32* str, const uchar32* str_end)
	{
		return mOut32(str, str_end);
	}

	s32 				xconsole_default_imp::write(const uchar32* fmt, const x_va_list& args)
	{
		XCONSOLE_LOCAL_STR_BUF(uchar32, str, 1024);
		s32 len = utf32::vsprintf(str, str_eos, (const uchar32*)fmt, utf32::len(fmt), args);
		return mOut32(str, str + len);
	}


	s32 				xconsole_default_imp::writeLine()
	{
		return write("\n");
	}


	static xconsole_default_imp sDefaultConsole;
	xconsole_node		xconsole::sConsoleList;

	void				xconsole::add(xconsole_imp* imp)
	{
		sConsoleList.add(&imp->mNode, imp);
	}

	void				xconsole::addDefault()
	{
		if (!sDefaultConsole.mNode.used())
		{
			return add(&sDefaultConsole);
		}
	}

	void				xconsole::remove(xconsole_imp* imp)
	{
		sConsoleList.remove(&imp->mNode);
	}

	void				xconsole::setColor(EColor color)
	{
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			i.imp()->setColor(color);
	}

	void				xconsole::restoreColor()
	{
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			i.imp()->setColor(xconsole::NORMAL);
	}

	s32 				xconsole::write(bool _value)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(_value);
		return r;
	}

	s32 				xconsole::write(f64 _value)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(_value);
		return r;
	}

	s32 				xconsole::write(s32 _value)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(_value);
		return r;
	}

	s32 				xconsole::write(s64 _value)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(_value);
		return r;
	}

	s32 				xconsole::write(f32 _value)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(_value);
		return r;
	}

	s32 				xconsole::write(u32 _value)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(_value);
		return r;
	}

	s32 				xconsole::write(u64 _value)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(_value);
		return r;
	}


	s32 				xconsole::write(uchar32 _value)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(_value);
		return r;
	}

	s32 				xconsole::write(const char* str)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(str);
		return r;
	}

	s32 				xconsole::write(const char* str, const char* str_end)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(str, str_end);
		return r;
	}

	s32 				xconsole::write(const char* fmt, const x_va_list& args)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(fmt, args);
		return r;
	}

	s32 				xconsole::write(const char* fmt, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, 
		const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
	{
		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16);
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(fmt, args);
		return r;
	}

	s32 				xconsole::write(const uchar32* str)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(str);
		return r;
	}

	s32 				xconsole::write(const uchar32* str, const uchar32* str_end)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(str, str_end);
		return r;
	}

	s32 				xconsole::write(const uchar32* fmt, const x_va_list& args)
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(fmt, args);
		return r;
	}

	s32 				xconsole::write(const uchar32* fmt, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, 
		const x_va& v9, const x_va& v10, const x_va& v11, const x_va& v12, const x_va& v13, const x_va& v14, const x_va& v15, const x_va& v16)
	{
		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16);
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->write(fmt, args);
		return r;
	}


	s32 				xconsole::writeLine()
	{
		s32 r = 0;
		for (xconsole_node::iterator i=sConsoleList.begin(); i.next(); )
			r = i.imp()->writeLine();
		return r;
	}
};
/**
 *  END xCore namespace
 */


#endif // ifndef SPU