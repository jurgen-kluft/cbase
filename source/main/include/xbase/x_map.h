#ifndef __X_BASE_MAP_H__
#define __X_BASE_MAP_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_allocator.h"
#include "xbase/x_integer.h"
#include "xbase/x_hash.h"
#include "xbase/x_btree.h"

namespace xcore
{
    template <typename K, typename V, typename H = hasher_t<K>> class map_t
    {
    public:
        inline map_t(alloc_t* a = nullptr)
            : m_allocator(a)
            , m_fsa()
            , m_root(nullptr)
        {
            if (m_allocator == nullptr)
            {
                m_allocator = alloc_t::get_system();
            }
            m_fsa = fsa_to_alloc_t(btree_ptr_t::sizeof_node(), m_allocator);
            m_tree.init_from_mask(&m_fsa, &m_kv, xU64Max, false);
        }

		inline ~map_t()
		{
			m_tree.clear(m_root);
		}

        bool insert(K const& k, V const& v)
        {
            u64 const hash   = m_hasher.hash(k);
            void*     vvalue = nullptr;
            if (m_tree.find(m_root, hash, vvalue))
            {
                value_t* iter = (value_t*)vvalue;
                while (iter != nullptr)
                {
                    if (iter->m_key == k)
                    {
                        // Maybe the value needs to be replaced ?
                        if (iter->m_value == v)
                        {
                            return false;
                        }

                        // Replace value
                        iter->m_value = v;
                        return true;
                    }
                    iter = iter->m_next;
                }
                value_t* cur_value = (value_t*)vvalue;
                value_t* new_value = m_allocator->construct<value_t>(hash, k, v);
                new_value->m_next  = cur_value->m_next;
                cur_value->m_next  = new_value;
            }
            else
            {
                value_t* new_value = m_allocator->construct<value_t>(hash, k, v);
                m_tree.add(m_root, hash, new_value);
            }
            return true;
        }

        bool find(K const& k, V& v) const
        {
            u64 const hash = m_hasher.hash(k);

            void* vvalue = nullptr;
            if (m_tree.find(m_root, hash, vvalue))
            {
                value_t* iter = (value_t*)vvalue;
                while (iter != nullptr)
                {
                    if (iter->m_key == k)
                    {
                        v = iter->m_value;
                        return true;
                    }
                    iter = iter->m_next;
                }
            }
            return false;
        }

        bool remove(K const& k, V& v)
        {
            u64 const hash = m_hasher.hash(k);

            void* vvalue = nullptr;
            if (m_tree.rem(m_root, hash, vvalue))
            {
                value_t* prev = nullptr;
                value_t* iter = (value_t*)vvalue;
                while (iter != nullptr)
                {
                    if (iter->m_key == k)
                    {
                        value_t* value = iter;

                        // If there list is > 1 add one back to the tree
                        if (prev != nullptr)
                        {
                            prev->m_next = iter->m_next;
                        }
                        else
                        {
                            iter = iter->m_next;
                            if (iter != nullptr)
                            {
                                m_tree.add(m_root, iter->m_hash, iter);
                            }
                        }

                        v = value->m_value;
                        m_allocator->destruct(value);
                        return true;
                    }
                    prev = iter;
                    iter = iter->m_next;
                }
            }
            return false;
        }

    private:
        struct value_t
        {
            inline value_t(u64 hash, const K& key, const V& value)
                : m_hash(hash)
                , m_key(key)
                , m_value(value)
                , m_next(nullptr)
            {
            }
            u64      m_hash;
            value_t* m_next;
            K        m_key;
            V        m_value;
            XCORE_CLASS_PLACEMENT_NEW_DELETE
        };
        class kv_value_t : public btree_ptr_kv_t
        {
        public:
            virtual u64 get_key(void* value) const
            {
                value_t const* const pvalue = (value_t const*)value;
                return pvalue->m_hash;
            }
            virtual void set_key(void* value, u64 key)
            {
                value_t* const pvalue = (value_t*)value;
                pvalue->m_hash        = key;
            }
        };
        alloc_t*             m_allocator;
        fsa_to_alloc_t       m_fsa;
        H                   m_hasher;
        btree_ptr_t::node_t* m_root;
        kv_value_t          m_kv;
        btree_ptr_t          m_tree;
    };

    template <typename T, typename H = hasher_t<T>> class xset
    {
    public:
        inline xset(alloc_t* a = nullptr)
            : m_allocator(a)
            , m_fsa()
            , m_root(nullptr)
        {
            if (m_allocator == nullptr)
            {
                m_allocator = alloc_t::get_system();
            }
            m_fsa = fsa_to_alloc_t(btree_ptr_t::sizeof_node(), m_allocator);
            m_tree.init_from_mask(&m_fsa, &m_kv, xU64Max, false);
        }

		~xset()
		{
			m_tree.clear(m_root);
		}

        bool insert(T const& value) 
		{
			u64 const hash   = m_hasher.hash(value);
            void*     vvalue = nullptr;
            if (m_tree.find(m_root, hash, vvalue))
            {
                value_t* iter = (value_t*)vvalue;
                while (iter != nullptr)
                {
                    if (iter->m_value == value)
                    {
                        return false;
                    }
                    iter = iter->m_next;
                }
                value_t* cur_value = (value_t*)vvalue;
                value_t* new_value = m_allocator->construct<value_t>(hash, value);
                new_value->m_next  = cur_value->m_next;
                cur_value->m_next  = new_value;
            }
            else
            {
                value_t* new_value = m_allocator->construct<value_t>(hash, value);
                m_tree.add(m_root, hash, new_value);
            }
            return true;
		}

        bool contains(T const& value) const 
		{
            u64 const hash = m_hasher.hash(value);

            void* vvalue = nullptr;
            if (m_tree.find(m_root, hash, vvalue))
            {
                value_t* iter = (value_t*)vvalue;
                while (iter != nullptr)
                {
                    if (iter->m_value == value)
                    {
                        return true;
                    }
                    iter = iter->m_next;
                }
            }
            return false;
		}

        bool remove(T const& value) 
		{
			u64 const hash = m_hasher.hash(value);

            void* vvalue = nullptr;
            if (m_tree.rem(m_root, hash, vvalue))
            {
                value_t* prev = nullptr;
                value_t* iter = (value_t*)vvalue;
                while (iter != nullptr)
                {
                    if (iter->m_value == value)
                    {
                        value_t* value = iter;

                        // If there list is > 1 add one back to the tree
                        if (prev != nullptr)
                        {
                            prev->m_next = iter->m_next;
                        }
                        else
                        {
                            iter = iter->m_next;
                            if (iter != nullptr)
                            {
                                m_tree.add(m_root, iter->m_hash, iter);
                            }
                        }

                        m_allocator->destruct(value);
                        return true;
                    }
                    prev = iter;
                    iter = iter->m_next;
                }
            }
            return false;
		}

    private:
        struct value_t
        {
            inline value_t(u64 hash, const T& value)
                : m_hash(hash)
                , m_next(nullptr)
                , m_value(value)
            {
            }
            u64      m_hash;
            value_t* m_next;
            T        m_value;
            XCORE_CLASS_PLACEMENT_NEW_DELETE
        };
        class kv_value_t : public btree_ptr_kv_t
        {
        public:
            virtual u64 get_key(void* value) const
            {
                value_t const* const pvalue = (value_t const*)value;
                return pvalue->m_hash;
            }
            virtual void set_key(void* value, u64 key)
            {
                value_t* const pvalue = (value_t*)value;
                pvalue->m_hash        = key;
            }
        };
        alloc_t*             m_allocator;
        fsa_to_alloc_t       m_fsa;
        H                   m_hasher;
        btree_ptr_t::node_t* m_root;
        kv_value_t          m_kv;
        btree_ptr_t          m_tree;
    };

}; // namespace xcore

#endif // __X_BASE_MAP_H__