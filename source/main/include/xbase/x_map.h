#ifndef __X_ALLOCATOR_MAP_H__
#define __X_ALLOCATOR_MAP_H__
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

    // Every node consumes 5 bits (maximum 32 branches)
    // Key is 64-bits (the hash of K or V)
    // So we have a tree with a maximum depth of 13

    struct xztree
    {
        // The array is compressed, only non-nullptr entries are there.
        struct node
        {
            u32    m_nodemap;   // In the array, a bit set indicates a node  (index=bit-pos)
            u32    m_valuemap;  // In the array, a bit set indicates a value (index=bit-pos)
            node* m_branches[1]; // Node*[->] - Value*[<-]
        };

        static s32 max_level() { return (64+4) / 5; }
        static s8  calc_index(s32 level, u64 key);

        static node* find(node*& root, node* n, s8 level, u64 k, void ** v, node ** path);
        static node* insert(node*& root, s32 level, node** nodes, u64* keys, void** values, s32 count, xalloc* allocator);
        static node* remove(node*& root, node** path, s8 level, u64 key, xalloc* allocator);
    };

    template <typename K, typename V, typename H = xhasher<K>> class xmap
    {
    public:
        inline xmap(xalloc* a = nullptr) : m_allocator(a), m_root(nullptr)
        {
            if (m_allocator == nullptr)
            {
                m_allocator = xalloc::get_system();
            }
        }

        bool insert(K const& k, V const& v)
        {            
            u64 const hash = m_hasher.hash(k);

            s32 level = 0;
            xztree::node* node = m_root;
            void** ppvalue = nullptr;
            s32 ivalue = -1;
            while ((xztree::find(m_root, node, level, hash, ppvalue, ivalue, nullptr)) != null)
            {
                level++;
            }
            if (ppvalue == nullptr)
            {
                // There is

            }
            else
            {
                // So we got a pointer to the place where the value pointer is stored
                if (*ppvalue == nullptr)
                {
                    // In this case there is no value pointer so we can create a value_t
                    // and store the pointer directly
                    value_t* pvalue = m_alloc->construct<value_t>(hash, k, v);
                    
                }
                else
                    value_t* pvalue = static_cast<value_t*>(*ppvalue);
                {
                    // In this case there is a value pointer so we first need to determine
                    // if this value is unique

                    value_t* piter = pvalue;
                    while (piter != nullptr)
                    {
                        if (hash == piter->m_hash)
                        {
                            if (k == piter->m_key && v == piter->m_value)
                            {
                                // Ok, so we found a duplicate, don't do anything more, just return false
                                // meaning to say that we did not add this key-value pair.
                                return false;
                            }
                        }
                        piter = piter->m_next;
                    }
                    if (piter == nullptr)
                    {
                        // We did not find any key-value pair so at this point our incoming
                        // k,v is unique and we should add it and insert it into the linked list.
                        value_t* pnewvalue = m_alloc->construct<value_t>(hash, k, v);
                        pnewvalue->m_next = pvalue;
                        *ppvalue = pnewvalue;
                        return true;
                    }
                }
            }
            return false;
        }

        bool find(K const& k, V& v) const
        {
            u64 const hash = m_hasher.hash(k);

            s32 level = 0;
            xztree::node* node = nullptr;
            void* pvalue = nullptr;
            s32 ivalue = 0;
            while ((xztree::find(m_root, node, level, hash, value, ivalue, nullptr)) != null)
            {
                level++;
            }
            if (pvalue != nullptr)
            {
                value_t* pvalue = static_cast<value_t*>(*ppvalue);
                value_t* piter = pvalue;
                while (piter != nullptr)
                {
                    if (hash == piter->m_hash)
                    {
                        if (k == piter->m_key && v == piter->m_value)
                        {
                            // Ok, so we found it, don't do anything more
                            v = piter->m_value;
                            return true;
                        }
                    }
                    piter = piter->m_next;
                }
            }
            return false;
        }

        bool remove(K const& k, V& v)
        {
            u64 const hash = m_hasher.hash(k);

            xztree::node* history[14];

            s32 level = 0;
            xztree::node* node = nullptr;
            void* pvalue = nullptr;
            s32 ivalue = 0;
            while ((xztree::find(m_root, node, level, hash, value, ivalue, history)) != null)
            {
                level++;
            }
            if (pvalue != nullptr)
            {
                value_t* pvalue = static_cast<value_t*>(*ppvalue);
                value_t* pprev = nullptr;
                value_t* piter = pvalue;
                while (piter != nullptr)
                {
                    if (hash == piter->m_hash)
                    {
                        if (k == piter->m_key && v == piter->m_value)
                        {
                            // Ok, so we found it, remove from linked list
                            if (pprev == nullptr)
                            {
                                **pvalue = piter->m_next;
                            }
                            else
                            {
                                pprev->m_next = piter->m_next;
                            }

                            // Linked list is empty then call node::remove
                            if (**pvalue == nullptr)
                            {
                                xztree::remove(m_root, history, level, hash, m_allocator);
                            }
                            return true;
                        }
                    }
                    pprev = piter;
                    piter = piter->m_next;
                }
            }
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
        };
        xalloc*       m_allocator;
        xztree::node* m_root;
        H             m_hasher;
    };

    template <typename T, typename H = xhasher<T>> class xset
    {
    public:
        inline xset(xalloc* a = nullptr) : m_allocator(a), m_root(nullptr) {}

        bool insert(T const& value);
        bool remove(T const& value);
        bool contains(T const& value) const;

    private:
        struct value_t
        {
            inline value_t(u32 key, const T& value) : m_key(key), m_value(value), m_next(nullptr) {}
            u64      m_key;
            value_t* m_next;
            T        m_value;
        };
        xalloc*       m_allocator;
        xztree::node* m_root;
        H             m_hasher;
    };



}; // namespace xcore

#endif // __X_ALLOCATOR_MAP_H__