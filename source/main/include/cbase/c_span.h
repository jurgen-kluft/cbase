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
        span_t(std::initializer_list<T> init);
        span_t(T* data, uint_t size);
        template <uint_t N> span_t(T (&array)[N]);
        explicit span_t(T* data);
        explicit span_t(T& data);

        T const*     m_data;
        uint_t m_size;
    };

    template <typename T>
    span_t<T>::span_t(std::initializer_list<T> init)
        : m_data(init.begin())
        , m_size(init.size())
    {
    }

    template <typename T>
    span_t<T>::span_t(T* data, uint_t size)
        : m_data(data)
        , m_size(size)
    {
    }

    template <typename T> template<uint_t N>
    span_t<T>::span_t(T(&array)[N])
        : m_data(array)
        , m_size(sizeof(array) / sizeof(array[0]))
    {
    }

    template <typename T>
    span_t<T>::span_t(T* data)
        : m_data(data)
        , m_size(1)
    {
    }

    template <typename T>
    span_t<T>::span_t(T& data)
        : m_data(&data)
        , m_size(1)
    {
    }

} // namespace ncore

#endif