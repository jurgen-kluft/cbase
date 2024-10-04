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
    // There is no handling for 'construction' or 'destruction', furthermore there is also
    // NO hashing done on the key.
    // The index of a node that that is managed by the tree is the same index at which you
    // can find the key/value, so here there is no indirection between the index at which
    // you can find the key/value and the node index.

    template <typename K, typename V>
    class map32_t
    {
        alloc_t* m_allocator;
        struct data_t
        {
            ntree32::tree_t           m_tree;
            u32                       m_empty;
            ntree32::tree_t::nnode_t* m_nodes;
            u8*                       m_colors;
            K*                        m_keys;
            V*                        m_values;
        };

        data_t m_data;

        static s8 s_compare(u32 _key, u32 _item, void const* user_data)
        {
            data_t* data = (data_t*)user_data;
            K&      key  = data->m_keys[_key];
            K&      item = data->m_keys[_item];
            if (key < item)
                return -1;
            else if (key > item)
                return 1;
            return 0;
        }

    public:
        inline map32_t(alloc_t* a, u32 max_items = 65535)
            : m_allocator(a)
            , m_data()
        {
            m_data.m_empty  = max_items;
            m_data.m_nodes  = g_allocate_array<ntree32::tree_t::nnode_t>(m_allocator, max_items + 1);
            m_data.m_colors = g_allocate_array<u8>(m_allocator, (max_items + 1 + 31) >> 5);
            m_data.m_keys   = g_allocate_array<K>(m_allocator, max_items + 1);
            m_data.m_values = g_allocate_array<V>(m_allocator, max_items + 1);
            ntree32::setup_tree(m_data.m_tree, max_items, m_data.m_nodes, m_data.m_colors);
        }

        inline ~map32_t()
        {
            ntree32::node_t n;
            while (!ntree32::clear(m_data.m_tree, n))
            {
                m_data.m_tree.v_del_node(n);
            }
            ntree32::teardown_tree(m_data.m_tree);
            g_deallocate_array(m_allocator, m_data.m_keys);
            g_deallocate_array(m_allocator, m_data.m_values);
            g_deallocate_array(m_allocator, m_data.m_colors);
            g_deallocate_array(m_allocator, m_data.m_nodes);
        }

        bool insert(K const& _key, V const& _value)
        {
            m_data.m_keys[m_data.m_empty] = _key;
            ntree32::node_t inserted;
            if (!ntree32::insert(m_data.m_tree, m_data.m_empty, s_compare, &m_data, inserted))
            {
                return false;
            }
            m_data.m_keys[inserted]   = _key;
            m_data.m_values[inserted] = _value;
            return true;
        }

        bool remove(K const& key)
        {
            m_data.m_keys[m_data.m_empty] = key;
            ntree32::node_t removed;
            if (ntree32::remove(m_data.m_tree, m_data.m_empty, s_compare, &m_data, removed))
            {
                m_data.m_tree.v_del_node(removed);
                return true;
            }
            return false;
        }

        bool find(K const& _key, V& _value) const
        {
            m_data.m_keys[m_data.m_empty] = _key;
            ntree32::node_t found;
            bool            result = ntree32::find(m_data.m_tree, m_data.m_empty, s_compare, &m_data, found);
            if (found != ntree32::c_invalid_node)
                _value = m_data.m_values[found];
            return result;
        }
    };

    template <typename K>
    class set32_t
    {
        alloc_t* m_allocator;
        struct data_t
        {
            ntree32::tree_t           m_tree;
            u32                       m_empty;
            ntree32::tree_t::nnode_t* m_nodes;
            u8*                       m_colors;
            K*                        m_keys;
        };
        data_t m_data;

        static s8 s_compare(u32 _key, u32 _item, void const* user_data)
        {
            data_t* data = (data_t*)user_data;
            K&      key  = data->m_keys[_key];
            K&      item = data->m_keys[_item];
            if (key < item)
                return -1;
            else if (key > item)
                return 1;
            return 0;
        }

    public:
        inline set32_t(alloc_t* a, u32 max_items = 65535)
            : m_allocator(a)
            , m_data()
        {
            m_data.m_empty  = max_items;
            m_data.m_nodes  = g_allocate_array<ntree32::tree_t::nnode_t>(m_allocator, max_items + 1);
            m_data.m_colors = g_allocate_array<u8>(m_allocator, (max_items + 1 + 31) >> 5);
            m_data.m_keys   = g_allocate_array<K>(m_allocator, max_items + 1);
            ntree32::setup_tree(m_data.m_tree, max_items, m_data.m_nodes, m_data.m_colors);
        }

        ~set32_t()
        {
            ntree32::node_t n;
            while (!ntree32::clear(m_data.m_tree, n))
            {
                m_data.m_tree.v_del_node(n);
            }
            ntree32::teardown_tree(m_data.m_tree);
            g_deallocate_array(m_allocator, m_data.m_keys);
            g_deallocate_array(m_allocator, m_data.m_colors);
            g_deallocate_array(m_allocator, m_data.m_nodes);
        }

        bool insert(K const& _key)
        {
            m_data.m_keys[m_data.m_empty] = _key;
            ntree32::node_t inserted;
            if (!ntree32::insert(m_data.m_tree, m_data.m_empty, s_compare, &m_data, inserted))
            {
                return false;
            }
            m_data.m_keys[inserted] = _key;
            return true;
        }

        bool contains(K const& key) const
        {
            m_data.m_keys[m_data.m_empty] = key;
            ntree32::node_t found         = ntree32::c_invalid_node;
            bool            result        = ntree32::find(m_data.m_tree, m_data.m_empty, s_compare, &m_data, found);
            return result;
        }

        bool remove(K const& key)
        {
            m_data.m_keys[m_data.m_empty] = key;
            ntree32::node_t removed;
            bool            result = ntree32::remove(m_data.m_tree, m_data.m_empty, s_compare, &m_data, removed);
            return result;
        }
    };

};  // namespace ncore

#endif  // __CBASE_MAP_H__
