#ifndef __CBASE_NONCOPYABLE_H__
#define __CBASE_NONCOPYABLE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    /**
     * @class noncopyable_t
     * @date <2011-10-14T13:36:00-0800>
     * @version 1.0.0
     *
     * Some objects kind of objects should not be copied. This is particularly true
     * of objects involved in providing mutually exclusive access to something
     * (e.g. Singletons, Mutexes, Queues, Semaphores, etc.)
     *
     * Based on Dave Abrahams contribution to the Boost library.
     */
    class noncopyable_t
    {
        //! Restrict the copy constructor
        noncopyable_t(const noncopyable_t&);

        //! Restrict the assignment operator
        const noncopyable_t& operator=(const noncopyable_t&);

    protected:
        noncopyable_t() {}
        ~noncopyable_t() {}
    };
} // namespace ncore

#endif // __CBASE_NONCOPYABLE_H__
