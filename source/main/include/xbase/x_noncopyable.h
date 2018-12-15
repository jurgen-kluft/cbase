#ifndef __XBASE_NONCOPYABLE_H__
#define __XBASE_NONCOPYABLE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xcore
{
	/**
	* @class xnoncopyable
	* @date <2011-10-14T13:36:00-0800>
	* @version 1.0.0
	*
	* Some objects kind of objects should not be copied. This is particularly true
	* of objects involved in providing mutually exclusive access to something
	* (e.g. Singletons, Mutexes, Queues, Semaphores, etc.)
	*
	* Based on Dave Abrahams contribution to the Boost library.
	*/
	class xnoncopyable
	{
		//! Restrict the copy constructor
		xnoncopyable(const xnoncopyable&);

		//! Restrict the assignment operator
		const xnoncopyable& operator=(const xnoncopyable&);

	protected:

		//! Create a xnoncopyable object
		xnoncopyable() { }

		//! Destroy a xnoncopyable object
		~xnoncopyable() { }

	}; /* xnoncopyable */

} // namespace xcore

#endif // __XBASE_NONCOPYABLE_H__
