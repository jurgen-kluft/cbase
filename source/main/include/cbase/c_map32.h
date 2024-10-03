#ifndef __CBASE_MAP32_H__
#define __CBASE_MAP32_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"
#include "cbase/c_tree32.h"

namespace ncore
{
    // Note:
    // The map and set provided here assume that a key and value are very simple POD types.
    // There is no handling for 'constrution' or 'destruction', furthermore there is also
    // NO hashing done on the key.

    template <typename K, typename V>
    class map32_t
    {
        alloc_t*        m_allocator;
        ntree32::tree_t m_tree;
        K*              m_keys;
        V*              m_values;

        static s8 compare_keys(const void* _keyA, const void* _keyB)
        {
            K const* keyA = (K*)_keyA;
            K const* keyB = (K*)_keyB;
            if (*keyA < *keyB)
                return -1;
            else if (*keyA > *keyB)
                return 1;
            return 0;
        }

    public:
        inline map32_t(alloc_t* a, u32 max_items = 65535)
            : m_allocator(a)
            , m_tree()
        {
            m_keys   = static_cast<K*>(m_allocator->allocate((max_items + 1) * sizeof(K)));
            m_values = static_cast<V*>(m_allocator->allocate((max_items + 1) * sizeof(V)));

            ntree32::create_tree(m_allocator, m_tree, max_items, compare_keys, m_keys, sizeof(K), m_values, sizeof(V));
        }

        inline ~map32_t()
        {
            ntree32::node_t n;
            while (!ntree32::clear(m_tree, n))
            {
                // We assume that K and V do not have destructors.
            }
            ntree32::destroy_tree(m_allocator, m_tree);
            m_allocator->deallocate(m_keys);
            m_allocator->deallocate(m_values);
        }

        bool insert(K const& _key, V const& _value)
        {
            ntree32::node_t n = m_tree.v_new_node();
            m_keys[n]         = _key;
            m_values[n]       = _value;
            if (!ntree32::insert(m_tree, n))
            {
                m_tree.v_del_node(n);
                return false;
            }
            return true;
        }

        bool remove(K const& key)
        {
            ntree32::node_t removed;
            if (ntree32::remove(m_tree, &key, removed))
            {
                return true;
            }
            return false;
        }

        bool find(K const& _key, V const*& _value) const
        {
            ntree32::node_t found;
            bool            result = ntree32::find(m_tree, &_key, found);
            if (found != ntree32::c_invalid_node)
                _value = &m_values[found];
            return result;
        }
    };

    template <typename K>
    class set32_t
    {
        alloc_t*        m_allocator;
        ntree32::tree_t m_tree;
        K*              m_keys;

        inline void* operator new(u64, void* where) { return where; }

        static s8 compare_keys(const void* _keyA, const void* _keyB)
        {
            K const* keyA = (K*)_keyA;
            K const* keyB = (K*)_keyB;
            if (*keyA < *keyB)
                return -1;
            else if (*keyA > *keyB)
                return 1;
            return 0;
        }

    public:
        inline set32_t(alloc_t* a, u32 max_items = 65535)
            : m_allocator(a)
            , m_tree()
        {
            m_keys = static_cast<K*>(m_allocator->allocate((max_items + 1) * sizeof(K)));
            ntree32::create_tree(m_allocator, m_tree, max_items, compare_keys, m_keys, sizeof(K), nullptr, 0);
        }

        ~set32_t()
        {
            ntree32::node_t n;
            while (!ntree32::clear(m_tree, n))
            {
                // We assume that K does not have a destructor
            }
            ntree32::destroy_tree(m_allocator, m_tree);
            m_allocator->deallocate(m_keys);
        }

        bool insert(K const& _key)
        {
            ntree32::node_t n = m_tree.v_new_node();
            m_keys[n]         = _key;
            if (!ntree32::insert(m_tree, n))
            {
                m_tree.v_del_node(n);
                return false;
            }
            return true;
        }

        bool contains(K const& key) const
        {
            ntree32::node_t found  = ntree32::c_invalid_node;
            bool            result = ntree32::find(m_tree, &key, found);
            return result;
        }

        bool remove(K const& key)
        {
            ntree32::node_t removed;
            bool            result = ntree32::remove(m_tree, &key, removed);
            return result;
        }
    };

};  // namespace ncore

#endif  // __CBASE_MAP_H__
