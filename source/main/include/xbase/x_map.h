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

    // Every node consumes 5 bits (maximum 32 branches)
    // Key is 64-bits (the hash of K or V)
    // So we have a tree with a maximum depth of 13

    struct xztree
    {
        // The array is compressed, only non-nullptr entries are there.
        struct node_t
        {
            u32     m_nodemap;   // In the array, a bit set indicates a node  (index=bit-pos)
            u32     m_valuemap;  // In the array, a bit set indicates a value (index=bit-pos)
            void*	m_branches[1]; // Node*[->] - Value*[<-]
			XCORE_CLASS_PLACEMENT_NEW_DELETE
        };

		struct path_t
		{
			node_t*	m_level[32];
			s32		m_levels;
		};

		struct found_t
		{
			u64 m_key;

		};

        static s32 max_level() { return (64+4) / 5; }

        static node_t* find(node_t* n, s8 level, u64 k, node_t*& node, void*& value, path_t* path);
        static node_t* insert(node_t*& root, s32 level, node_t* parent, node_t* node, u64 hash1, void* value1, u64 hash2, void* value2, xalloc* allocator);
        static node_t* remove(node_t*& root, path_t& path, s8 level, node_t* node, u64 key, xalloc* allocator);
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

            void* ovalue = nullptr;
            xztree::node_t* onode = nullptr;

            s32 level = 0;
			xztree::node_t* parent = nullptr;
            xztree::node_t* node = m_root;
			while (node != nullptr)
            {
				xztree::node_t* child = xztree::find(node, level, hash, onode, ovalue, nullptr);
				if (child == nullptr)
					break;
				parent = node;
				node = child;
                level++;
            }

            if (ovalue == nullptr)
            {
                // There is no such key in the map yet, so we can insert it
                value_t* pvalue = m_allocator->construct<value_t>(hash, k, v);
				xztree::insert(m_root, level, onode, hash, pvalue, 0, nullptr, m_allocator);
            }
            else
            {
                // So we encountered a branch where there is a value already
                // In this case there is a value pointer so we first need to determine
                // if this value is unique.
				// We can condition this here but for now we keep it simple, we will only
				// push members on the linked-list in the case of a hash-collision.

				value_t* pvalue = static_cast<value_t*>(ovalue);
                value_t* piter = pvalue;
                while (piter != nullptr)
                {
                    if (hash == piter->m_hash)
                    {
                        if (k == piter->m_key && v == piter->m_value)
                        {
                            // Ok, so we found a duplicate, don't do anything more, just return false
                            // indicating that we did not add this key-value pair.
                            return false;
                        }
                    }
					else
					{
						// The value here has a different hash, so we need to insert our value here
						xztree::insert(m_root, level, onode, piter->m_hash, piter, hash, ovalue, m_allocator);
						return true;
					}
                    piter = piter->m_next;
                }
                if (piter == nullptr)
                {
					// We have a hash collision, so add insert it into the linked-list
                    value_t* pnewvalue = m_allocator->construct<value_t>(hash, k, v);
					pnewvalue->m_next = pvalue->m_next;
					pvalue->m_next = pnewvalue;
                    return true;
                }
            }
            return false;
        }

        bool find(K const& k, V& v) const
        {
            u64 const hash = m_hasher.hash(k);

            void* value = nullptr;
            xztree::node_t* vnode = nullptr;

            s32 level = 0;
            xztree::node_t* node = m_root;
			while (xztree::find(node, level, hash, vnode, value, nullptr) != nullptr)
            {
                level++;
            }
            if (value != nullptr)
            {
                value_t* pvalue = static_cast<value_t*>(value);
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

            xztree::path_t path;

            void* value = nullptr;
            node* vnode = nullptr;

            s32 level = 0;
			xztree::node_t* node = m_root;
            while ((xztree::find(node, level, hash, vnode, value, ivalue, &path)) != null)
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

                            // Linked list is empty? then call node::remove
                            if (**pvalue == nullptr)
                            {
                                xztree::remove(m_root, path, level, vnode, hash, m_allocator);
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
			XCORE_CLASS_PLACEMENT_NEW_DELETE
        };
        xalloc*         m_allocator;
        xztree::node_t* m_root;
        H               m_hasher;
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
			XCORE_CLASS_PLACEMENT_NEW_DELETE
        };
        xalloc*         m_allocator;
        xztree::node_t* m_root;
        H               m_hasher;
    };

};

#endif // __X_BASE_MAP_H__