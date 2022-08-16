#ifndef __CBASE_XSTD_H__
#define __CBASE_XSTD_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif


//==============================================================================
//  XCORE GLOBAL PLACEMENT NEW
//==============================================================================
struct xcore_t {};
inline void*	operator new(ncore::uint_t num_bytes, void* mem, xcore_t module)			{ return mem; }
inline void		operator delete(void* p, void* mem, xcore_t module)							{}

namespace ncore
{
	namespace xstd
	{
		//==============================================================================
		//  TYPE TRAITS
		//==============================================================================
		class xnull { };

		template<typename T> class xknown_type   { public: enum { isSystem = false, isIntegral = false, isFloat = false, isSigned = false }; };
		template<>  class xknown_type<bool>     { public: enum { isSystem =  true, isIntegral = false, isFloat = false, isSigned = false }; };
		template<>  class xknown_type<bool32>   { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned =  true }; };
		template<>  class xknown_type<u8>        { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned = false }; };
		template<>  class xknown_type<s8>        { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned =  true }; };
		template<>  class xknown_type<u16>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned = false }; };
		template<>  class xknown_type<s16>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned =  true }; };
		template<>  class xknown_type<u32>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned = false }; };
		template<>  class xknown_type<s32>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned =  true }; };
		template<>  class xknown_type<u64>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned = false }; };
		template<>  class xknown_type<s64>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned =  true }; };
		template<>  class xknown_type<f32>       { public: enum { isSystem =  true, isIntegral = false, isFloat =  true, isSigned = false }; };
		template<>  class xknown_type<f64>       { public: enum { isSystem =  true, isIntegral = false, isFloat =  true, isSigned = false }; };

		template<typename T>
		class xtypetrait
		{
#define TMTT1	template <typename U> struct
			TMTT1 TTReference				{ enum { result = false }; typedef U ReferredType; };
			TMTT1 TTReference<U&>			{ enum { result = true  }; typedef U ReferredType; };
			TMTT1 TTPointer					{ enum { result = false }; typedef xnull PointeeType; };
			TMTT1 TTPointer<U*>				{ enum { result = true  }; typedef U PointeeType; };
			TMTT1 TTPointer<U*&>			{ enum { result = true  }; typedef U PointeeType; };
			TMTT1 TTConst					{ typedef U Result;  enum { isConst = false }; };
			TMTT1 TTConst<const U>			{ typedef U Result;  enum { isConst = true  }; };
			TMTT1 TTConst<const U&>			{ typedef U& Result; enum { isConst = true  }; };
			TMTT1 TTVolatile				{ typedef U Result;  enum { isVolatile = false }; };
			TMTT1 TTVolatile<volatile U>	{ typedef U Result;  enum { isVolatile = true  }; };
			TMTT1 TTVolatile<volatile U&>	{ typedef U& Result;enum { isVolatile = true  }; };
#undef TMTT1

		public:
			typedef typename TTConst<T>::Result										NonConstType;
			typedef typename TTVolatile<T>::Result									NonVolatileType;
			typedef typename TTVolatile<typename TTConst<T>::Result>::Result		UnqualifiedType;
			typedef typename TTPointer<UnqualifiedType>::PointeeType				PointeeType;
			typedef typename TTReference<T>::ReferredType							ReferredType;

			enum { isConst          = TTConst<T>::isConst };
			enum { isVolatile       = TTVolatile<T>::isVolatile };
			enum { isReference      = TTReference<UnqualifiedType>::result };
			enum { isPointer        = TTPointer<typename TTReference<UnqualifiedType>::ReferredType >::result};
			enum { isSystem         = xknown_type<T>::isSystem || isPointer };
		};


		/// Assigns the contents of a to b and the contents of b to a.
		/// This is used as a primitive operation by many other algorithms. 
		///
		template <typename Assignable> 
		inline void xswap (Assignable& a, Assignable& b)
		{
			Assignable tmp = a;
			a = b;
			b = tmp;
		}

		/// Copy copies elements from the range [first, last) to the range
		/// [result, result + (last - first)). That is, it performs the assignments
		/// *result = *first, *(result + 1) = *(first + 1), and so on. [1] Generally,
		/// for every integer n from 0 to last - first, copy performs the assignment
		/// *(result + n) = *(first + n). Assignments are performed in forward order,
		/// i.e. in order of increasing n. 
		///
		template <typename InputIterator, typename OutputIterator>
		inline OutputIterator xcopy (InputIterator first, InputIterator last, OutputIterator result)
		{
			for (; first != last; ++result, ++first)
				*result = *first;
			return (result);
		}

		/// Copy_n copies elements from the range [first, first + n) to the range
		/// [result, result + n). That is, it performs the assignments
		/// *result = *first, *(result + 1) = *(first + 1), and so on. Generally,
		/// for every integer i from 0 up to (but not including) n, copy_n performs
		/// the assignment *(result + i) = *(first + i). Assignments are performed
		/// in forward order, i.e. in order of increasing n.
		///
		template <typename InputIterator, typename OutputIterator>
		inline OutputIterator xcopy_n (InputIterator first, u32 count, OutputIterator result)
		{
			for (; count; --count, ++result, ++first)
				*result = *first;
			return (result);
		}

		/// Fill assigns the value value to every element in the range [first, last).
		/// That is, for every iterator i in [first, last),
		/// it performs the assignment *i = value.
		///
		template <typename ForwardIterator, typename T>
		inline void xfill (ForwardIterator first, ForwardIterator last, const T& value)
		{
			for (; first != last; ++first)
				*first = value;
		}

		/// Fill_n assigns the value value to every element in the range
		/// [first, first+count). That is, for every iterator i in [first, first+count),
		/// it performs the assignment *i = value. The return value is first + count.
		///
		template <typename OutputIterator, typename T>
		inline OutputIterator xfill_n (OutputIterator first, u32 count, const T& value)
		{
			for (; count; --count, ++first)
				*first = value;
			return (first);
		}


		///< Calls the destructor of \p p without calling delete.
		template <typename U>
		inline void				xdestroy (U* p)										{ p->~U(); }

		// Helper templates to not instantiate anything for integral types.
		template <typename U>
		void					xdtors (U first, U last)
		{
			for (--last; first <= last; ++first)
				xdestroy (&*first); 
		}

		template <typename U, bool B>
		struct xdtorsr
		{
			inline void operator()(U first, U last) { xdtors (first, last); }
		};

		template<typename U>
		struct xdtorsr<U, true>
		{
			inline void operator()(U, U) {}
		};

		/// Calls the destructor on elements in range [first, last) without calling delete.
		///
		template <typename ForwardIterator>
		inline void				xdestroy (ForwardIterator first, ForwardIterator last)
		{
			typedef typename ForwardIterator::value_type	valuetype;
			xdtorsr<ForwardIterator, xstd::xtypetrait<valuetype>::isSystem>()(first, last);
		}

		template <typename U>
		inline void				xconstruct (U* p)
		{
			new (p, xcore_t()) U;
		}

		template <typename ForwardIterator>
		inline void				xconstruct (ForwardIterator first, ForwardIterator last)
		{
			typedef typename ForwardIterator::value_type	valuetype;
			if (xstd::xtypetrait<valuetype>::isSystem)
			{
				//
			}
			else for (--last; first<=last; ++first)
				xconstruct (&*first);
		}

		//==============================================================================
		//  Binary functions
		//==============================================================================
		template <typename Arg1, typename Arg2, typename Result>
		struct xbinary_function
		{
			typedef Arg1	first_argument_type;
			typedef Arg2	second_argument_type;
			typedef Result	result_type;
		};

		template <typename Arg, typename Result>
		struct xunary_function
		{
			typedef Arg		argument_type;
			typedef Result	result_type;
		};

		#define STD_BINARY_FUNCTOR(name, rv, func)	\
			template <class T> struct name : public xbinary_function<T,T,rv> \
			{ inline rv operator()(const T& a, const T& b) const { return func; } };
		#define STD_UNARY_FUNCTOR(name, rv, func)	\
			template <class T> struct name : public xunary_function<T,rv> \
			{ inline rv operator()(const T& a) const { return func; } };

		STD_BINARY_FUNCTOR (xplus,				T,		(a + b))
		STD_BINARY_FUNCTOR (xminus,				T,		(a - b))
		STD_BINARY_FUNCTOR (xdivides,			T,		(a / b))
		STD_BINARY_FUNCTOR (xmodulus,			T,		(a % b))
		STD_BINARY_FUNCTOR (xmultiplies,		T,		(a * b))
		STD_BINARY_FUNCTOR (xlogical_and,		T,		(a && b))
		STD_BINARY_FUNCTOR (xlogical_or,		T,		(a || b))
		STD_UNARY_FUNCTOR  (xlogical_not,		T,		(!a))
		STD_BINARY_FUNCTOR (xbitwise_or,		T,		(a | b))
		STD_BINARY_FUNCTOR (xbitwise_and,		T,		(a & b))
		STD_BINARY_FUNCTOR (xbitwise_xor,		T,		(a ^ b))
		STD_UNARY_FUNCTOR  (xbitwise_not,		T,		(~a))
		STD_UNARY_FUNCTOR  (xnegate,			T,		(-a))
		STD_BINARY_FUNCTOR (xequal_to,			bool,	(a == b))
		STD_BINARY_FUNCTOR (xnot_equal_to,		bool,	(!(a == b)))
		STD_BINARY_FUNCTOR (xgreater,			bool,	(b < a))
		STD_BINARY_FUNCTOR (xless,				bool,	(a < b))
		STD_BINARY_FUNCTOR (xgreater_equal,		bool,	(!(a < b)))
		STD_BINARY_FUNCTOR (xless_equal,		bool,	(!(b < a)))
		STD_BINARY_FUNCTOR (xcompare,			s32,	(a < b ? -1 : (b < a)))
		STD_UNARY_FUNCTOR  (xidentity,			T,		(a))
	}
}

#endif	///< __CBASE_XSTD_H__
