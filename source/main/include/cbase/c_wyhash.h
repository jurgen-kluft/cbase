#ifndef __CBASE_DEFAULT_WYHASH_H__
#define __CBASE_DEFAULT_WYHASH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"

namespace ncore
{
    namespace nhash
    {
        class wyreg
        {
        public:
            wyreg();
            ~wyreg();
            
            void init(alloc_t* allocator, s32 size, u64 seed = 0);
            s32  size() const { return m_size; }
            s32  count() const { return m_count; }
            bool insert(void* key, s32 keysize, s32& pos);
            bool find(void* key, s32 keysize, s32& pos) const;
            bool remove(void* key, s32 keysize, s32& pos);

            alloc_t* m_allocator;
            s32      m_size;
            s32      m_count;
            u64*     m_index;
            u64      m_secret[4];
        };
        template <typename K, typename V> class wymap
        {
        public:
            void init(alloc_t* allocator, s32 size, u64 seed = 0);
            s32  size() const { return m_reg.m_size; }
            s32  count() const { return m_reg.m_count; }
            bool insert(K const& key, V const& value);
            bool find(K const& key, V& value) const;
            bool contains(K const& key) const;
            bool remove(K const& key);

            wyreg m_reg;
            V*    m_values;
        };

        template <typename K, typename V> void wymap<K, V>::init(alloc_t* allocator, s32 size, u64 seed)
        {
            m_reg.init(allocator, size, seed);
            m_values = (V*)allocator->allocate(sizeof(V) * m_reg.m_size);
        }

        template <typename K, typename V> bool wymap<K, V>::insert(K const& key, V const& value)
        {
            s32 pos;
            if (m_reg.insert((void*)&key, (s32)sizeof(key), pos))
            {
                m_values[pos] = value;
                return true;
            }
            return false;
        }

        template <typename K, typename V> bool wymap<K, V>::find(K const& key, V& value) const
        {
            s32 pos;
            if (m_reg.find((void*)&key, (s32)sizeof(key), pos))
            {
                value = m_values[pos];
                return true;
            }
            return false;
        }

        template <typename K, typename V> bool wymap<K, V>::contains(K const& key) const
        {
            s32 pos;
            return m_reg.find((void*)&key, (s32)sizeof(key), pos);
        }

        template <typename K, typename V> bool wymap<K, V>::remove(K const& key)
        {
            s32 pos;
            if (m_reg.remove((void*)&key, (s32)sizeof(key), pos))
            {
                m_values[pos] = V();
                return true;
            }
            return false;
        }

        template <typename T> class wyset
        {
        public:
            void init(alloc_t* allocator, s32 size, u64 seed = 0);
            s32  size() const { return m_reg.m_size; }
            s32  count() const { return m_reg.m_count; }
            bool insert(T const& item);
            bool contains(T const& item) const;
            bool remove(T const& item);

            wyreg m_reg;
        };

        template <typename T> void wyset<T>::init(alloc_t* allocator, s32 size, u64 seed) { m_reg.init(allocator, size, seed); }

        template <typename T> bool wyset<T>::insert(T const& item)
        {
            s32 pos;
            if (m_reg.insert((void*)&item, (s32)sizeof(item), pos))
            {
                return true;
            }
            return false;
        }

        template <typename T> bool wyset<T>::contains(T const& item) const
        {
            s32 pos;
            return m_reg.find((void*)&item, (s32)sizeof(item), pos);
        }

        template <typename T> bool wyset<T>::remove(T const& item)
        {
            s32 pos;
            if (m_reg.remove((void*)&item, (s32)sizeof(item), pos))
            {
                return true;
            }
            return false;
        }

    } // namespace nhash
} // namespace ncore

#endif // __CBASE_DEFAULT_WYHASH_H__