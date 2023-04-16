#ifndef __CBASE_SINGLETON_H__
#define __CBASE_SINGLETON_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif
#include "cbase/c_debug.h"
#include "cbase/c_allocator.h"

namespace ncore
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
	* @date <2011-02-16T17:57:45-0400>
	* @version 2.2.0 
	*
	* The xlocal_static_instantiation policy allows the creation 
	* and lifetime of an instance of a particular type
	* to be managed using static local data. This will
	* abide by the standard C++ rules for static objects
	* lifetimes.
	*/
	class static_instantiation_t
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
			static ncore::s64 _memblock[ ((((sizeof(T)+2*T::SINGLETON_ALIGNMENT-1)/T::SINGLETON_ALIGNMENT) * T::SINGLETON_ALIGNMENT) + 7) / 8 ];
			ptr_t const a = T::SINGLETON_ALIGNMENT;
			void* mem = (void*)(((ptr_t)_memblock + (a-1)) & ~(a-1));
			ptr = new (mem) T();
		}

		template <class T>
		static void singleton_destroy(T*& ptr)
		{
			ptr->~T();
			ptr = nullptr;
		}
	};

	
	/**
	* @class heap_instantiation_t
	* @date <2011-02-16T17:57:45-0400>
	* @version 2.2.0
	*
	* The heap_instantiation_t policy allows the creation 
	* and lifetime of an instance of a particular type
	* to be managed using dynamic allocation and a singleton
	* destroyer. This will abide by the standard C++ rules 
	* for static objects lifetimes.
	*/
	class heap_instantiation_t
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
			alloc_t* allocator = T::singleton_allocator();
			u32 alignment = T::singleton_alignment();
			void* mem = allocator->allocate(sizeof(T), alignment);
			ptr = new (mem) T();
		}

		template <class T>
		static void singleton_destroy(T*& ptr)
		{
			ptr->~T();
			alloc_t* allocator = T::singleton_allocator();
			allocator->deallocate(ptr);
			ptr = nullptr;
		}
	};

	/**
	* @class singleton_t
	* @date <2011-02-16T17:57:45-0400>
	* @version 2.2.0 
	*
	* Based on the work of John Vlissidles in his book 'Pattern Hatching'
	* an article by Douglas Schmidtt on double-checked locking and policy
	* templates described by Andrei Alexandrescu.
	*
	* The create() and destroy() calls should execute once.
	*
	* For class T using static_instantiation_t it is required that T has
	* an enum value:
	* - SINGLETON_ALIGNMENT = 4/8/16/32/64/...
	*
	* For class T using heap_instantiation_t it is required that T has
	* 2 static functions:
	* - static alloc_t* singleton_allocator();
	* - static u32           singleton_alignment();
	*
	* @code
	*
	* // Most common singleton_t (default is heap_instantiation_t)
	* singleton_t<LonesomeType>
	*
	* // singleton_t that uses static storage 
	* singleton_t<LonesomeType, static_instantiation_t>
	*
	* // singleton_t that uses heap storage 
	* singleton_t<LonesomeType, heap_instantiation_t>
	*
	* @endcode
	*/
	template <class T, class InstantiationPolicy=heap_instantiation_t>
	class singleton_t : private InstantiationPolicy
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
					if (sSingleton == nullptr)
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
					if (sSingleton != nullptr)
					{
						T* ptr = (T*)sSingleton;
						sSingleton = nullptr;
						InstantiationPolicy::singleton_destroy(ptr);
					}
				}
				break;
			}
			return (T*)sSingleton;
		}
	public:
		inline				singleton_t()			{ }

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

#endif	/// __CBASE_SINGLETON_H__
