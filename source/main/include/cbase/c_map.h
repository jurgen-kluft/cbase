#ifndef __CBASE_MAP_H__
#define __CBASE_MAP_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_allocator.h"
#include "cbase/c_integer.h"
#include "cbase/c_hash.h"
#include "cbase/c_limits.h"
#include "cbase/c_tree.h"
#include "cbase/private/c_std.h"
#include "cbase/private/c_map_internal.h"

namespace ncore
{

    template <typename K, typename V, typename H = nhash::hasher_t<K>> class map_t
    {
    public:
        inline map_t(alloc_t* a = nullptr, u32 max_items = 65535)
            : m_allocator(a)
            , m_ctxt(nullptr)
            , m_tree()
        {
            if (m_allocator == nullptr)
            {
                m_allocator = alloc_t::get_system();
            }

            if (max_items <= type_t<u16>::max())
            {
                m_ctxt = m_allocator->construct<map_tree_ctxt_t<u16, K, u32, H>>(a, max_items);
            }
            else if (max_items <= type_t<u32>::max())
            {
                m_ctxt = m_allocator->construct<map_tree_ctxt_t<u32, K, u32, H>>(a, max_items);
            }

            m_tree.init(m_ctxt);
        }

        inline ~map_t()
        {
            while (!m_tree.clear()) {}
            m_tree.exit();
            m_allocator->destruct(m_ctxt);
        }

        bool insert(K const& key, V const& value) { return m_tree.insert(&key, &value); }
        bool remove(K const& key) { return m_tree.remove(&key); }
        bool find(K const& key, V const*& value) const
        {
            tree_t::node_t* found;
            bool    result = m_tree.find(&key, found);
            if (found != nullptr)
                value= (V const*)m_ctxt->v_get_value(found);
            return result;
        }

    private:
        alloc_t*        m_allocator;
        tree_t::ctxt_t* m_ctxt;
        tree_t          m_tree;
    };

    template <typename K, typename H = nhash::hasher_t<K>> class set_t
    {
    public:
        inline set_t(alloc_t* a = nullptr, u32 max_items = 65535)
            : m_allocator(a)
            , m_ctxt(nullptr)
            , m_tree()
        {
            if (m_allocator == nullptr)
            {
                m_allocator = alloc_t::get_system();
            }

            if (max_items <= type_t<u16>::max())
            {
                m_ctxt = m_allocator->construct<map_tree_ctxt_t<u16, K, u8, H>>(a, max_items, false);
            }
            else if (max_items <= type_t<u32>::max())
            {
                m_ctxt = m_allocator->construct<map_tree_ctxt_t<u32, K, u8, H>>(a, max_items, false);
            }

            m_tree.init(m_ctxt);
        }

        ~set_t() 
        {
            m_tree.exit();
            m_allocator->destruct(m_ctxt); 
        }

        bool insert(K const& key) { return m_tree.insert(&key, nullptr); }
        bool contains(K const& key) const
        {
            tree_t::node_t* found  = nullptr;
            bool            result = m_tree.find(&key, found);
            return result;
        }
        bool remove(K const& key)
        {
            bool result = m_tree.remove(&key);
            return result;
        }

    private:
        alloc_t*        m_allocator;
        tree_t::ctxt_t* m_ctxt;
        tree_t          m_tree;
    };

}; // namespace ncore

#endif // __CBASE_MAP_H__