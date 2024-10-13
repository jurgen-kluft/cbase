#ifndef __CBASE_STD_H__
#define __CBASE_STD_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

//==============================================================================
//  CORE GLOBAL PLACEMENT NEW
//==============================================================================

// clang-format off
struct core_t{}; // core_t is a tag type used to distinguish core placement new
inline void* operator new(ncore::uint_t num_bytes, void* mem, core_t module) { return mem; }
inline void  operator delete(void* p, void* mem, core_t module) {}
// clang-format on

namespace ncore
{
    namespace cstd
    {
        //==============================================================================
        //  TYPE TRAITS
        //==============================================================================
        class nill
        {
        };

        // clang-format off
		template<typename T> class known_type_t   { public: enum { isSystem = false, isIntegral = false, isFloat = false, isSigned = false }; };
		template<>  class known_type_t<bool>     { public: enum { isSystem =  true, isIntegral = false, isFloat = false, isSigned = false }; };
		template<>  class known_type_t<u8>        { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned = false }; };
		template<>  class known_type_t<s8>        { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned =  true }; };
		template<>  class known_type_t<u16>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned = false }; };
		template<>  class known_type_t<s16>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned =  true }; };
		template<>  class known_type_t<u32>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned = false }; };
		template<>  class known_type_t<s32>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned =  true }; };
		template<>  class known_type_t<u64>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned = false }; };
		template<>  class known_type_t<s64>       { public: enum { isSystem =  true, isIntegral =  true, isFloat = false, isSigned =  true }; };
		template<>  class known_type_t<f32>       { public: enum { isSystem =  true, isIntegral = false, isFloat =  true, isSigned = false }; };
		template<>  class known_type_t<f64>       { public: enum { isSystem =  true, isIntegral = false, isFloat =  true, isSigned = false }; };
        // clang-format on

        // clang-format off
		template<typename T>
		class typetrait_t
		{
#define TMTT1	template <typename U> struct
			TMTT1 TTReference				{ enum { result = false }; typedef U ReferredType; };
			TMTT1 TTReference<U&>			{ enum { result = true  }; typedef U ReferredType; };
			TMTT1 TTPointer					{ enum { result = false }; typedef nill PointeeType; };
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
			enum { isSystem         = known_type_t<T>::isSystem || isPointer };
		};
        // clang-format on

        /// Assigns the contents of a to b and the contents of b to a.
        /// This is used as a primitive operation by many other algorithms.
        ///
        template <typename Assignable>
        inline void swap(Assignable& a, Assignable& b)
        {
            Assignable tmp = a;
            a              = b;
            b              = tmp;
        }

        /// Copy copies elements from the range [first, last) to the range
        /// [result, result + (last - first)). That is, it performs the assignments
        /// *result = *first, *(result + 1) = *(first + 1), and so on. [1] Generally,
        /// for every integer n from 0 to last - first, copy performs the assignment
        /// *(result + n) = *(first + n). Assignments are performed in forward order,
        /// i.e. in order of increasing n.
        ///

        // clang-format off
        template <typename InputIterator, typename OutputIterator> 
		inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
        {
            for (; first != last; ++result, ++first)
                *result = *first;
            return (result);
        }
        // clang-format on

        /// Copy_n copies elements from the range [first, first + n) to the range
        /// [result, result + n). That is, it performs the assignments
        /// *result = *first, *(result + 1) = *(first + 1), and so on. Generally,
        /// for every integer i from 0 up to (but not including) n, copy_n performs
        /// the assignment *(result + i) = *(first + i). Assignments are performed
        /// in forward order, i.e. in order of increasing n.
        ///
        // clang-format off
        template <typename InputIterator, typename OutputIterator>
		inline OutputIterator copy_n(InputIterator first, u32 count, OutputIterator result)
        {
            for (; count; --count, ++result, ++first)
                *result = *first;
            return (result);
        }
        // clang-format on

        /// Fill assigns the value value to every element in the range [first, last).
        /// That is, for every iterator i in [first, last),
        /// it performs the assignment *i = value.
        ///
        // clang-format off
        template <typename ForwardIterator, typename T>
		inline void fill(ForwardIterator first, ForwardIterator last, const T& value)
        {
            for (; first != last; ++first)
                *first = value;
        }
        // clang-format on

        /// Fill_n assigns the value value to every element in the range
        /// [first, first+count). That is, for every iterator i in [first, first+count),
        /// it performs the assignment *i = value. The return value is first + count.
        ///
        // clang-format off
        template <typename OutputIterator, typename T>
		inline OutputIterator fill_n(OutputIterator first, u32 count, const T& value)
        {
            for (; count; --count, ++first)
                *first = value;
            return (first);
        }
        // clang-format on

        ///< Calls the destructor of \p p without calling delete.
        template <typename U>
        inline void destruct(U* p)
        {
            p->~U();
        }

        // Helper templates to not instantiate anything for integral types.
        template <typename U>
        void destructors(U first, U last)
        {
            for (--last; first <= last; ++first)
                destruct(&*first);
        }

        // clang-format off
        template <typename U, bool B>
		struct destructors_t
        {
            inline void operator()(U first, U last) { destructors(first, last); }
        };

        template <typename U>
		struct destructors_t<U, true>
        {
            inline void operator()(U, U) {}
        };
        // clang-format on

        /// Calls the destructor on elements in range [first, last) without calling delete.
        ///
        template <typename ForwardIterator>
        inline void destroy(ForwardIterator first, ForwardIterator last)
        {
            typedef typename ForwardIterator::value_type valuetype;
            destructors<ForwardIterator, cstd::typetrait_t<valuetype>::isSystem>()(first, last);
        }

        template <typename U>
        inline void construct(U* p)
        {
            new (p, core_t()) U;
        }
        template <typename U>
        inline void copy_construct(U* p, U const& x)
        {
            new (p, core_t()) U(x);
        }

        template <typename ForwardIterator>
        inline void construct(ForwardIterator first, ForwardIterator last)
        {
            typedef typename ForwardIterator::value_type valuetype;
            if (cstd::typetrait_t<valuetype>::isSystem == false)
            {
                for (--last; first <= last; ++first)
                    construct(&*first);
            }
        }

        //==============================================================================
        //  Binary functions
        //==============================================================================
        template <typename Arg1, typename Arg2, typename Result>
        struct binary_function_t
        {
            typedef Arg1   first_argument_type;
            typedef Arg2   second_argument_type;
            typedef Result result_type;
        };

        template <typename Arg, typename Result>
        struct unary_function_t
        {
            typedef Arg    argument_type;
            typedef Result result_type;
        };

#define STD_BINARY_FUNCTOR(name, rv, func)                                  \
    template <class T>                                                      \
    struct name : public binary_function_t<T, T, rv>                        \
    {                                                                       \
        inline rv operator()(const T& a, const T& b) const { return func; } \
    };
#define STD_UNARY_FUNCTOR(name, rv, func)                       \
    template <class T>                                          \
    struct name : public unary_function_t<T, rv>                \
    {                                                           \
        inline rv operator()(const T& a) const { return func; } \
    };

        STD_BINARY_FUNCTOR(plus, T, (a + b))
        STD_BINARY_FUNCTOR(minus, T, (a - b))
        STD_BINARY_FUNCTOR(divides, T, (a / b))
        STD_BINARY_FUNCTOR(modulus, T, (a % b))
        STD_BINARY_FUNCTOR(multiplies, T, (a * b))
        STD_BINARY_FUNCTOR(logical_and, T, (a && b))
        STD_BINARY_FUNCTOR(logical_or, T, (a || b))
        STD_UNARY_FUNCTOR(logical_not, T, (!a))
        STD_BINARY_FUNCTOR(bitwise_or, T, (a | b))
        STD_BINARY_FUNCTOR(bitwise_and, T, (a & b))
        STD_BINARY_FUNCTOR(bitwise_xor, T, (a ^ b))
        STD_UNARY_FUNCTOR(bitwise_not, T, (~a))
        STD_UNARY_FUNCTOR(negate, T, (-a))
        STD_BINARY_FUNCTOR(equal_to, bool, (a == b))
        STD_BINARY_FUNCTOR(not_equal_to, bool, (!(a == b)))
        STD_BINARY_FUNCTOR(greater, bool, (b < a))
        STD_BINARY_FUNCTOR(less, bool, (a < b))
        STD_BINARY_FUNCTOR(greater_equal, bool, (!(a < b)))
        STD_BINARY_FUNCTOR(less_equal, bool, (!(b < a)))
        STD_BINARY_FUNCTOR(compare, s32, (a < b ? -1 : (b < a)))
        STD_UNARY_FUNCTOR(identity, T, (a))
    }  // namespace cstd
}  // namespace ncore

#endif  ///< __CBASE_STD_H__
