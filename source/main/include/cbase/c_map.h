#ifndef __CBASE_MAP_H__
#define __CBASE_MAP_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"
#include "cbase/c_tree.h"

namespace ncore
{
    // -----------------------------------------------------------------------------------
    // -----------------------------------------------------------------------------------
    // Note of caution:
    // The map and set provided here assume that a key and value are very simple POD types.
    // There is no handling for 'construction' or 'destruction', furthermore there is also
    // NO hashing done on the key.
    // -----------------------------------------------------------------------------------

    template <typename K, typename V>
    class map_t
    {
        struct data_t
        {
            data_t(alloc_t* a)
                : m_allocator(a)
                , m_root(nullptr)
                , m_size(0)
            {
            }
            alloc_t*       m_allocator;
            ntree::node_t* m_root;
            s32            m_size;
        };
        data_t m_data;

        static inline ntree::node_t* s_new_node(void* user_data)
        {
            data_t* data                = (data_t*)user_data;
            item_t* item                = g_construct<item_t>(data->m_allocator);
            item->set_child(ntree::LEFT, nullptr);
            item->set_child(ntree::RIGHT,nullptr);
            return (ntree::node_t*)item;
        }

        static inline s8 compare_key_with_node(const void* _key, const ntree::node_t* _node)
        {
            K const*      key  = (K*)_key;
            item_t const* item = (item_t const*)_node;
            if (*key < item->key)
                return -1;
            else if (*key > item->key)
                return 1;
            return 0;
        }

        static inline s8 compare_node_with_node(const ntree::node_t* _nodeA, const ntree::node_t* _nodeB)
        {
            item_t const* itemA = (item_t*)_nodeA;
            item_t const* itemB = (item_t*)_nodeB;
            if (itemA->key < itemB->key)
                return -1;
            else if (itemA->key > itemB->key)
                return 1;
            return 0;
        }

    public:
        inline map_t(alloc_t* a)
            : m_data(a)
        {
        }

        struct item_t : public ntree::node_t
        {
            K key;
            V value;

            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        inline ~map_t()
        {
            ntree::node_t* n;
            while (!ntree::clear(m_data.m_root, n))
            {
                item_t* item = (item_t*)n;
                g_destruct(m_data.m_allocator, item);
            }
        }

        inline int_t size() const { return m_data.m_size; }
        inline bool  empty() const { return size() == 0; }

        inline bool insert(K const& _key, V const& _value)
        {
            item_t         temp;
            ntree::node_t* inserted;
            if (!ntree::insert(m_data.m_root, &temp, (void const*)&_key, compare_key_with_node, s_new_node, &m_data, inserted))
            {
                return false;
            }
            m_data.m_size++;
            item_t* item = (item_t*)inserted;
            item->key    = _key;
            item->value  = _value;
            return true;
        }

        inline bool find(K const& _key, V& _value) const
        {
            ntree::node_t* found;
            if (ntree::find(m_data.m_root, (void const*)&_key, compare_key_with_node, found))
            {
                item_t* item = (item_t*)found;
                _value       = item->value;
                return true;
            }
            return false;
        }

        inline bool remove(K const& key)
        {
            item_t         temp;
            ntree::node_t* removed;
            if (ntree::remove(m_data.m_root, &temp, (void const*)&key, compare_key_with_node, removed))
            {
                item_t* item = (item_t*)removed;
                m_data.m_allocator->destruct(item);
                m_data.m_size--;
                return true;
            }
            return false;
        }

        struct iterator_t
        {
            inline bool          traverse(s32 d = ntree::LEFT) { return m_iter.traverse(m_root, d, m_item); }
            inline bool          preorder(s32 d = ntree::LEFT) { return m_iter.preorder(m_root, d, m_item); }
            inline bool          sortorder(s32 d = ntree::LEFT) { return m_iter.sortorder(m_root, d, m_item); }
            inline bool          postorder(s32 d = ntree::LEFT) { return m_iter.postorder(m_root, d, m_item); }
            inline bool          next() { return traverse(); }
            inline bool          order() { return sortorder(); }
            inline item_t const* item() const { return (item_t const*)m_item; }

        protected:
            friend class map_t;
            iterator_t(ntree::node_t const* root)
                : m_root(root)
                , m_item(nullptr)
                , m_iter()
            {
            }

        private:
            ntree::node_t const* m_root;
            ntree::node_t const* m_item;
            ntree::iterator_t    m_iter;
        };

        inline iterator_t iterate() const { return iterator_t(m_data.m_root); }
    };

    template <typename K>
    class set_t
    {
        struct data_t
        {
            data_t(alloc_t* a)
                : m_allocator(a)
                , m_root(nullptr)
                , m_size(0)
            {
            }
            alloc_t*       m_allocator;
            ntree::node_t* m_root;
            s32            m_size;
        };

        data_t m_data;

        static inline ntree::node_t* s_new_node(void* user_data)
        {
            data_t* data                = (data_t*)user_data;
            item_t* item                = g_construct<item_t>(data->m_allocator);
            item->set_child(ntree::LEFT, nullptr);
            item->set_child(ntree::RIGHT,nullptr);
            return (ntree::node_t*)item;
        }

        static inline s8 compare_key_with_node(const void* _key, const ntree::node_t* _node)
        {
            K const*      key  = (K*)_key;
            item_t const* item = (item_t const*)_node;
            if (*key < item->key)
                return -1;
            else if (*key > item->key)
                return 1;
            return 0;
        }

        static inline s8 compare_node_with_node(const ntree::node_t* _nodeA, const ntree::node_t* _nodeB)
        {
            item_t const* itemA = (item_t*)_nodeA;
            item_t const* itemB = (item_t*)_nodeB;
            if (itemA->key < itemB->key)
                return -1;
            else if (itemA->key > itemB->key)
                return 1;
            return 0;
        }

    public:
        inline set_t(alloc_t* a)
            : m_data(a)
        {
        }

        struct item_t : public ntree::node_t
        {
            K key;

            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        ~set_t()
        {
            ntree::node_t* n;
            while (!ntree::clear(m_data.m_root, n))
            {
                item_t* item = (item_t*)n;
                g_destruct(m_data.m_allocator, item);
            }
        }

        inline int_t size() const { return m_data.m_size; }
        inline bool  empty() const { return size() == 0; }

        inline bool insert(K const& _key)
        {
            item_t         temp;
            ntree::node_t* inserted;
            if (!ntree::insert(m_data.m_root, &temp, (void const*)&_key, compare_key_with_node, s_new_node, &m_data, inserted))
            {
                return false;
            }
            item_t* item = (item_t*)inserted;
            item->key    = _key;
            m_data.m_size++;
            return true;
        }

        inline bool contains(K const& _key) const
        {
            ntree::node_t* found;
            return ntree::find(m_data.m_root, (void const*)&_key, compare_key_with_node, found);
        }

        inline bool remove(K const& key)
        {
            item_t         temp;
            ntree::node_t* removed;
            if (ntree::remove(m_data.m_root, &temp, (void const*)&key, compare_key_with_node, removed))
            {
                item_t* item = (item_t*)removed;
                m_data.m_allocator->destruct(item);
                return true;
            }
            return false;
        }

        struct iterator_t
        {
            inline bool          traverse(s32 d = ntree::LEFT) { return m_iter.traverse(m_root, d, m_item); }
            inline bool          preorder(s32 d = ntree::LEFT) { return m_iter.preorder(m_root, d, m_item); }
            inline bool          sortorder(s32 d = ntree::LEFT) { return m_iter.sortorder(m_root, d, m_item); }
            inline bool          postorder(s32 d = ntree::LEFT) { return m_iter.postorder(m_root, d, m_item); }
            inline bool          next() { return traverse(); }
            inline bool          order() { return sortorder(); }
            inline item_t const* item() const { return (item_t const*)m_item; }

        protected:
            friend class set_t;
            inline iterator_t(ntree::node_t const* root)
                : m_root(root)
                , m_item(nullptr)
                , m_iter()
            {
            }

        private:
            ntree::node_t const* m_root;
            ntree::node_t const* m_item;
            ntree::iterator_t    m_iter;
        };

        inline iterator_t iterate() const { return iterator_t(m_data.m_root); }
    };

};  // namespace ncore

#endif  // __CBASE_MAP_H__
