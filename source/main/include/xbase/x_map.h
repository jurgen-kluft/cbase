#ifndef __X_BASE_MAP_H__
#define __X_BASE_MAP_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_allocator.h"
#include "xbase/x_integer.h"
#include "xbase/x_hash.h"

namespace xcore
{
    // BTree with dynamic size nodes, using bitset to identify the slots that are set.
    //
    // On a 32-bit system:
    //   Size of node is from 8B to 136B so this works great with an allocator
    //   that has a good Fixed Size Allocator setup.
    //
    // On a 64-bit system:
    //   Size of node is from 16B to 264B so this works great with an allocator
    //   that has a good Fixed Size Allocator setup.

    // Every node consumes 5 bits out of the key (maximum 32 branches)
    // Key is 64-bits (the hash of K or V)
    // So we have a tree with a maximum depth of 13


    template <typename K, typename V, typename H = xhasher<K>> class xmap
    {
    public:
        inline xmap(xalloc* a = nullptr) : m_allocator(a)
        {
            if (m_allocator == nullptr)
            {
                m_allocator = xalloc::get_system();
            }
        }

        bool insert(K const& k, V const& v)
        {            
            return false;
        }

        bool find(K const& k, V& v) const
        {
            return false;
        }

        bool remove(K const& k, V& v)
        {
            return false;
        }

    private:
        struct value_t
        {
            inline value_t(u64 hash, const K& key, const V& value) : m_hash(hash), m_key(key), m_value(value), m_next(nullptr) {}
            u64      m_hash;
            value_t* m_next;
            K        m_key;
            V        m_value;
			XCORE_CLASS_PLACEMENT_NEW_DELETE
        };
        xalloc*         m_allocator;
        H               m_hasher;
    };

    template <typename T, typename H = xhasher<T>> class xset
    {
    public:
        inline xset(xalloc* a = nullptr) : m_allocator(a) {}

        bool insert(T const& value)
		{
			return false;
		}

		bool remove(T const& value)
		{
			return false;
		}

        bool contains(T const& value) const
		{
			return false;
		}

    private:
        struct value_t
        {
            inline value_t(u32 key, const T& value) : m_key(key), m_value(value), m_next(nullptr) {}
            u64      m_key;
            value_t* m_next;
            T        m_value;
			XCORE_CLASS_PLACEMENT_NEW_DELETE
        };
        xalloc*         m_allocator;
        H               m_hasher;
    };

};

#endif // __X_BASE_MAP_H__