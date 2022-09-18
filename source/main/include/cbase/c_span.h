#ifndef __CCORE_SPAN_H__
#define __CCORE_SPAN_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include <initializer_list>

namespace ncore
{
    template <typename T> struct span_t
    {
        span_t(T* data, uint_t size)
            : m_data(data)
            , m_size(size)
        {
        }
        span_t(T (&array)[], uint_t size)
            : m_data(array)
            , m_size(size)
        {
        }
        explicit span_t(T* data)
            : m_data(data)
            , m_size(1)
        {
        }
        explicit span_t(T& data)
            : m_data(&data)
            , m_size(1)
        {
        }

        T*     m_data;
        uint_t m_size;
    };

    template <typename P> constexpr auto as_span_impl(P* p, uint_t s) { return span_t<P>(p, s); }

    template <class C> auto as_span(C& c) { return as_span_impl(c.data(), c.size()); }
    template <class C> auto as_span(const C& c) { return as_span_impl(c.data(), c.size()); }

    template <typename T, uint_t N> constexpr auto as_span(T (&arr)[N]) noexcept { return as_span_impl(arr, N); }
    template <typename T, uint_t N> constexpr auto as_span(const T (&arr)[N]) noexcept { return as_span_impl(arr, N); }

} // namespace ncore

#endif