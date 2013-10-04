#ifndef __XBASE_SINGLETON_H__
#define __XBASE_SINGLETON_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif
#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"

#include "xbase\x_noncopyable.h"

namespace xcore
{
	//
	// This policy controls how an object is instantiated
	// as well as how and when its destroyed. Phoenix-style
	// singletons are not supported easily with type of policy, 
	// this is intentional since I do not believe that is in
	// the true spirit of a singleton.
	//
	// InstantiationPolicyContract
	// {
	//    create(pointer_type&)
	// }

	/**
	* @class xlocal_static_instantiation
	* @author Virtuos
	* @date <2011-02-16T17:57:45-0400>
	* @version 2.2.0 
	*
	* The xlocal_static_instantiation policy allows the creation 
	* and lifetime of an instance of a particular type
	* to be managed using static local data. This will
	* abide by the standard C++ rules for static objects
	* lifetimes.
	*/
	class xstatic_instantiation
	{
	protected:
		/**
		* Create an instance of an object, using a local static. The
		* object need to be destroyed by calling destroy.
		*
		* @param ptr reference to location to receive the address 
		* of the allocated object
		*/
		template <class T>
		static void singleton_create(T*& ptr)
		{
			static xcore::s32 _memblock[ (((sizeof(T)/T::SINGLETON_ALIGNMENT)+2) * T::SINGLETON_ALIGNMENT) / 4 ];
			u32 const a = T::SINGLETON_ALIGNMENT;
			void* mem = (void*)(((u32)_memblock + (a-1)) & ~(a-1));
			ptr = new (mem) T();
		}

		template <class T>
		static void singleton_destroy(T*& ptr)
		{
			ptr->~T();
			ptr = NULL;
		}
	};

	
	/**
	* @class xheap_instantiation
	* @author Virtuos
	* @date <2011-02-16T17:57:45-0400>
	* @version 2.2.0
	*
	* The xlazy_instantiation policy allows the creation 
	* and lifetime of an instance of a particular type
	* to be managed using dynamic allocation and a singleton
	* destroyer. This will abide by the standard C++ rules 
	* for static objects lifetimes.
	*/
	class xheap_instantiation
	{
	protected:
		/**
		* Create an instance of an object, using new, that will be
		* destroyed when an associated Destroyer object (allocated
		* statically) goes out of scope.
		*
		* @param ptr reference to location to receive the address 
		* of the allocated object
		*/
		template <class T>
		static void singleton_create(T*& ptr)
		{
			x_iallocator* allocator = T::singleton_allocator();
			u32 alignment = T::singleton_alignment();
			void* mem = allocator->allocate(sizeof(T), alignment);
			ptr = new (mem) T();
		}

		template <class T>
		static void singleton_destroy(T*& ptr)
		{
			ptr->~T();
			x_iallocator* allocator = T::singleton_allocator();
			allocator->deallocate(ptr);
			ptr = NULL;
		}
	};

	/**
	* @class xsingleton
	* @author Virtuos
	* @date <2011-02-16T17:57:45-0400>
	* @version 2.2.0 
	*
	* Based on the work of John Vlissidles in his book 'Pattern Hatching'
	* an article by Douglas Schmidtt on double-checked locking and policy
	* templates described by Andrei Alexandrescu.
	*
	* The create() and destroy() calls should execute once.
	*
	* For class T using xstatic_instantiation it is required that T has
	* an enum value:
	* - SINGLETON_ALIGNMENT = 4/8/16/32/64/...
	*
	* For class T using xheap_instantiation it is required that T has
	* 2 static functions:
	* - static x_iallocator* singleton_allocator();
	* - static u32           singleton_alignment();
	*
	* @code
	*
	* // Most common xsingleton (default is xheap_instantiation)
	* xsingleton<LonesomeType>
	*
	* // xsingleton that uses static storage 
	* xsingleton<LonesomeType, xstatic_instantiation>
	*
	* // xsingleton that uses heap storage 
	* xsingleton<LonesomeType, xheap_instantiation>
	*
	* @endcode
	*/
	template <class T, class InstantiationPolicy=xheap_instantiation>
	class xsingleton : private InstantiationPolicy, private xnoncopyable
	{
		enum EAction { CREATE, GET, DESTROY };
		static T*			handler(EAction action)
		{
			// Uses local static storage to avoid static construction
			// sequence issues. (regarding when the lock is created)
			static volatile T* sSingleton = 0;

			switch (action)
			{
			case CREATE:
				{
					if (sSingleton == NULL)
					{
						T* _ptr;
						InstantiationPolicy::singleton_create(_ptr);
						sSingleton = _ptr;
					}
				}
				break;
			case GET:
				{
					// Uses local static storage to avoid static construction
					// sequence issues. 
				}
				break;
			case DESTROY:
				{
					if (sSingleton != NULL)
					{
						T* ptr = (T*)sSingleton;
						sSingleton = NULL;
						InstantiationPolicy::singleton_destroy(ptr);
					}
				}
				break;
			}
			return (T*)sSingleton;
		}
	public:
							xsingleton()			{ }

		/**
		* Provide access to the single instance through double-checked locking 
		*
		* @return T* single instance 
		*/
		static inline void	s_create()			{ handler(CREATE); }
		static inline T*	s_instance()		{ return handler(GET); }
		static inline void	s_destroy()			{ handler(DESTROY); }
	};

};

#endif	/// __XBASE_SINGLETON_H__
