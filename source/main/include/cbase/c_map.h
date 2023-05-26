#ifndef __CBASE_MAP_H__
#define __CBASE_MAP_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_allocator.h"
#include "cbase/c_integer.h"
#include "cbase/c_limits.h"
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
        {
            if (m_allocator == nullptr)
            {
                m_allocator = get_system_alloc();
            }

            if (max_items <= type_t<u16>::max())
            {
                m_ctxt = m_allocator->construct<map_tree_ctxt_t<u16, K, V, H>>(a, max_items);
            }
            else if (max_items <= type_t<u32>::max())
            {
                m_ctxt = m_allocator->construct<map_tree_ctxt_t<u32, K, V, H>>(a, max_items);
            }
        }

        inline ~map_t()
        {
            while (!ntree::clear(m_ctxt)) {}
            m_allocator->destruct(m_ctxt);
        }

        bool insert(K const& key, V const& value) { return ntree::insert(m_ctxt, &key, &value); }
        bool remove(K const& key) { return ntree::remove(m_ctxt, &key); }
        bool find(K const& key, V const*& value) const
        {
            ntree::node_t* found;
            bool            result = ntree::find(m_ctxt, &key, found);
            if (found != nullptr)
                value= (V const*)m_ctxt->v_get_value(found);
            return result;
        }

    private:
        alloc_t*        m_allocator;
        ntree::tree_t* m_ctxt;
    };

    template <typename K, typename H = nhash::hasher_t<K>> class set_t
    {
    public:
        inline set_t(alloc_t* a = nullptr, u32 max_items = 65535)
            : m_allocator(a)
            , m_ctxt(nullptr)
        {
            if (m_allocator == nullptr)
            {
                m_allocator = get_system_alloc();
            }

            if (max_items <= type_t<u16>::max())
            {
                m_ctxt = m_allocator->construct<map_tree_ctxt_t<u16, K, u8, H>>(a, max_items, false);
            }
            else if (max_items <= type_t<u32>::max())
            {
                m_ctxt = m_allocator->construct<map_tree_ctxt_t<u32, K, u8, H>>(a, max_items, false);
            }
        }

        ~set_t() 
        {
            while (!ntree::clear(m_ctxt)) {}
            m_allocator->destruct(m_ctxt); 
        }

        bool insert(K const& key) { return ntree::insert(m_ctxt, &key, nullptr); }
        bool contains(K const& key) const
        {
            ntree::node_t* found  = nullptr;
            bool            result = ntree::find(m_ctxt, &key, found);
            return result;
        }
        bool remove(K const& key)
        {
            bool result = ntree::remove(m_ctxt, &key);
            return result;
        }

    private:
        alloc_t*        m_allocator;
        ntree::tree_t* m_ctxt;
    };

}; // namespace ncore

#endif // __CBASE_MAP_H__