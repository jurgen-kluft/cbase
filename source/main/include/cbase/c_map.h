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
    // Note:
    // The map and set provided here assume that a key and value are very simple POD types.
    // There is no handling for 'construction' or 'destruction', furthermore there is also
    // NO hashing done on the key.

    template <typename K, typename V>
    class map_t
    {
        alloc_t*      m_allocator;
        ntree::tree_t m_tree;

        static inline s8 compare_key_with_item(const void* _key, const void* _item)
        {
            K const*      key  = (K*)_key;
            item_t const* item = (item_t const*)_item;
            if (*key < item->key)
                return -1;
            else if (*key > item->key)
                return 1;
            return 0;
        }

        static inline s8 compare_items(const void* _itemA, const void* _itemB)
        {
            item_t const* itemA = (item_t*)_itemA;
            item_t const* itemB = (item_t*)_itemB;
            if (itemA->key < itemB->key)
                return -1;
            else if (itemA->key > itemB->key)
                return 1;
            return 0;
        }

    public:
        inline map_t(alloc_t* a)
            : m_allocator(a)
            , m_tree()
        {
            ntree::setup_tree(m_allocator, m_tree);
        }

        struct item_t
        {
            item_t(K const& _key, V const& _value)
                : key(_key)
                , value(_value)
            {
            }
            K key;
            V value;

            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        inline ~map_t()
        {
            ntree::node_t* n;
            while (!ntree::clear(m_tree, n))
            {
                item_t* item = (item_t*)m_tree.v_get_item(n);
                m_allocator->destruct(item);
                m_tree.v_del_node(n);
            }
            ntree::teardown_tree(m_allocator, m_tree);
        }

        inline int_t size() const { return ntree::size(m_tree); }
        inline bool  empty() const { return ntree::size(m_tree) == 0; }

        inline bool insert(K const& _key, V const& _value)
        {
            ntree::node_t* inserted;
            if (!ntree::insert(m_tree, (void const*)&_key, compare_key_with_item, inserted))
            {
                return false;
            }
            item_t* item = m_allocator->construct<item_t>(_key, _value);
            m_tree.v_set_item(inserted, item);
            return true;
        }

        inline bool find(K const& _key, V& _value) const
        {
            ntree::node_t* found;
            if (ntree::find(m_tree, (void const*)&_key, compare_key_with_item, found))
            {
                item_t* item = (item_t*)m_tree.v_get_item(found);
                _value       = item->value;
                return true;
            }
            return false;
        }

        inline bool remove(K const& key)
        {
            ntree::node_t* removed;
            if (ntree::remove(m_tree, (void const*)&key, compare_key_with_item, removed))
            {
                item_t* item = (item_t*)m_tree.v_get_item(removed);
                m_allocator->destruct(item);
                m_tree.v_del_node(removed);
                return true;
            }
            return false;
        }

        struct iterator_t
        {
            inline bool          traverse(s32 d = ntree::LEFT) { return m_iter.traverse(*m_tree, d, m_item); }
            inline bool          preorder(s32 d = ntree::LEFT) { return m_iter.preorder(*m_tree, d, m_item); }
            inline bool          sortorder(s32 d = ntree::LEFT) { return m_iter.sortorder(*m_tree, d, m_item); }
            inline bool          postorder(s32 d = ntree::LEFT) { return m_iter.postorder(*m_tree, d, m_item); }
            inline bool          next() { return traverse(); }
            inline bool          order() { return sortorder(); }
            inline item_t const* item() const { return (item_t const*)m_item; }

        protected:
            friend class map_t;
            iterator_t(ntree::tree_t const* tree)
                : m_tree(tree)
                , m_iter()
            {
            }

        private:
            ntree::tree_t const* m_tree;
            ntree::iterator_t    m_iter;
            void*                m_item;
        };

        inline iterator_t iterate() const { return iterator_t(&m_tree); }
    };

    template <typename K>
    class set_t
    {
        alloc_t*      m_allocator;
        ntree::tree_t m_tree;

        static inline s8 compare_key_with_item(const void* _key, const void* _item)
        {
            K const*      key  = (K*)_key;
            item_t const* item = (item_t const*)_item;
            if (*key < item->key)
                return -1;
            else if (*key > item->key)
                return 1;
            return 0;
        }

        static inline s8 compare_items(const void* _itemA, const void* _itemB)
        {
            item_t const* itemA = (item_t*)_itemA;
            item_t const* itemB = (item_t*)_itemB;
            if (itemA->key < itemB->key)
                return -1;
            else if (itemA->key > itemB->key)
                return 1;
            return 0;
        }

    public:
        inline set_t(alloc_t* a)
            : m_allocator(a)
            , m_tree()
        {
            ntree::setup_tree(m_allocator, m_tree);
        }

        struct item_t
        {
            item_t(K const& _key)
                : key(_key)
            {
            }
            K key;

            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        ~set_t()
        {
            ntree::node_t* n;
            while (!ntree::clear(m_tree, n))
            {
                item_t* item = (item_t*)m_tree.v_get_item(n);
                m_allocator->destruct(item);
                m_tree.v_del_node(n);
            }
            ntree::teardown_tree(m_allocator, m_tree);
        }

        inline int_t size() const { return ntree::size(m_tree); }
        inline bool  empty() const { return ntree::size(m_tree) == 0; }

        inline bool insert(K const& _key)
        {
            ntree::node_t* inserted;
            if (!ntree::insert(m_tree, (void const*)&_key, compare_key_with_item, inserted))
            {
                return false;
            }
            item_t* item = m_allocator->construct<item_t>(_key);
            m_tree.v_set_item(inserted, item);
            return true;
        }

        inline bool contains(K const& _key) const
        {
            ntree::node_t* found;
            return ntree::find(m_tree, (void const*)&_key, compare_key_with_item, found);
        }

        inline bool remove(K const& key)
        {
            ntree::node_t* removed;
            if (ntree::remove(m_tree, (void const*)&key, compare_key_with_item, removed))
            {
                item_t* item = (item_t*)m_tree.v_get_item(removed);
                m_allocator->destruct(item);
                m_tree.v_del_node(removed);
                return true;
            }
            return false;
        }

        struct iterator_t
        {
            inline bool          traverse(s32 d = ntree::LEFT) { return m_iter.traverse(*m_tree, d, m_item); }
            inline bool          preorder(s32 d = ntree::LEFT) { return m_iter.preorder(*m_tree, d, m_item); }
            inline bool          sortorder(s32 d = ntree::LEFT) { return m_iter.sortorder(*m_tree, d, m_item); }
            inline bool          postorder(s32 d = ntree::LEFT) { return m_iter.postorder(*m_tree, d, m_item); }
            inline bool          next() { return traverse(); }
            inline bool          order() { return sortorder(); }
            inline item_t const* item() const { return (item_t const*)m_item; }

        protected:
            friend class set_t;
            inline iterator_t(ntree::tree_t const* tree)
                : m_tree(tree)
                , m_iter()
            {
            }

        private:
            ntree::tree_t const* m_tree;
            ntree::iterator_t    m_iter;
            void*                m_item;
        };

        inline iterator_t iterate() const { return iterator_t(&m_tree); }
    };

};  // namespace ncore

#endif  // __CBASE_MAP_H__
