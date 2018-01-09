#ifndef __XBASE_SLICE_H__
#define __XBASE_SLICE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"
#include "xbase/x_integer.h"
#include "xbase/x_memory_std.h"
#include "xbase/x_range.h"
#include "xbase/x_qsort.h"


//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class x_iallocator;
	struct slice_data;

	struct memory
	{
		memory(x_iallocator* allocator) : m_allocator(allocator) {}

		void*			alloc(xsize_t size, u32 align);
		void*			realloc(void* p, xsize_t size, u32 align);
		void			dealloc(void* p);

		x_iallocator*	m_allocator;
	};

	//==============================================================================
	// Slice
	// A reference counted slice owning a memory block with a view/window (from,to).
	//==============================================================================
	struct slice
	{
						slice();
						slice(x_iallocator* allocator, u32 item_count, u32 item_size);
						slice(slice_data* data, s32 from, s32 to);
						slice(slice const& other);
						~slice();

		static void		alloc(slice& slice, x_iallocator* allocator, u32 item_count, u32 item_size);

		u32				size() const;
		u32				refcnt() const;

		void			resize(u32 count);

		slice			view(u32 from, u32 to) const;

		slice			obtain() const;
		void			release() const;

		void*			get_at(s32 index);
		void const*		get_at(s32 index) const;

		slice_data*		mData;
		u32				mFrom;
		u32				mTo;
	};

	template<typename T>
	class slice_t
	{
	public:
						slice_t();
						slice_t(memory mem, s32 size);
						slice_t(slice_t<T> const& other);

		s32				size() const;

		slice_t			operator () (s32 to) const;
		slice_t			operator () (s32 from, s32 to) const;

		T &				operator [] (s32 index);
		T const &		operator [] (s32 index) const;

		slice			m_slice;

	protected:
						slice_t(slice& slice) : m_slice(data->incref()) {}
	};

	template<typename T>
	inline slice_t<T>::slice_t()
		: m_slice()
	{
	}

	template<typename T>
	inline slice_t<T>::slice_t(memory mem, s32 count)
		: m_slice(mem.m_allocator, count, sizeof(T))
	{
	}

	template<typename T>
	inline slice_t<T>::slice_t(slice_t<T> const& other)
		: m_slice(other.m_slice.obtain())
	{
	}

	template<typename T>
	inline s32			slice_t<T>::size() const
	{
		return m_slice.size();
	}

	template<typename T>
	inline slice_t<T>	slice_t<T>::operator () (s32 to) const
	{
		slice_t<T> slice;
		slice.m_slice = m_slice.view(0, to);
		return slice;
	}
	
	template<typename T>
	inline slice_t<T>	slice_t<T>::operator () (s32 from, s32 to) const
	{
		slice_t<T> slice;
		slice.m_slice = m_slice.view(from, to);
		return slice;
	}

	template<typename T>
	inline T &			slice_t<T>::operator [] (s32 index)
	{
		T * item = (T *)m_slice.get_at(index);
		return *item;
	}

	template<typename T>
	inline T const &	slice_t<T>::operator [] (s32 index) const
	{
		T const* item = (T const*)m_slice.get_at(index);
		return *item;
	}


	template<typename T>
	void				make(memory mem, slice_t<T>& proto, s32 size);

	template<typename T>
	inline void			make(memory mem, slice_t<T>& proto, s32 size)
	{
		slice::alloc(proto.m_slice, mem.m_allocator, size, sizeof(T));
	}

	// ----------------------------------------------------------------------------------------
	//   ARRAY
	// ----------------------------------------------------------------------------------------
	template<typename T>
	class array_t;

	template<typename T>
	class array_iter_t
	{
	public:
						array_iter_t(array_t<T> const& _array) : m_index(0), m_array(_array) {}

		s32				index() const			{ return m_index; }
		T const&		item() const			{ return m_array[m_index]; }

		T const&		operator * (void) const	{ return m_array[m_index]; }

		bool			next()
		{
			if (m_index >= m_array.size())
				return false;
			++m_index;
			return true;
		}

	protected:
		u32				m_index;
		array_t<T> const&	m_array;
	};

	template<typename T>
	s32					compare_t(T const& a, T const& b)
	{
		if (a < b)
			return -1;
		else if (a > b)
			return 1;
		return 0;
	}

	template<typename T>
	class array_t
	{
	public:
						array_t() : m_size(0) { }

		inline u32		size() const	{ return m_size; }
		inline u32		max() const		{ return m_data.size(); }

		array_t<T>		operator () (s32 to) const;
		array_t<T>		operator () (s32 from, s32 to) const;

		T &				operator [] (s32 index);
		T const &		operator [] (s32 index) const;

		array_iter_t<T>	begin() const;

		static s32		compare(const void* const le, const void* const re, void* data)
		{
			T const& let = *(T const*)le;
			T const& ret = *(T const*)re;
			s32 const c = compare_t<T>(let, ret);
			return c;
		}

		u32				m_size;
		slice_t<T>		m_data;
	};

	template<typename T>
	inline array_t<T>		array_t<T>::operator () (s32 to) const
	{
		array_t<T> other;
		other.m_data = m_data(0, to);
		other.m_size = m_size;
		if (to < m_size)
			other.m_size = to;
		return other;
	}

	template<typename T>
	inline array_t<T>		array_t<T>::operator () (s32 from, s32 to) const
	{
		array_t<T> other;
		other.m_data = m_data(from, to);
		other.m_size = m_size;
		if ((u32)(to-from) < m_size)
			other.m_size = (to - from);
		return other;
	}


	template<typename T>
	inline T &				array_t<T>::operator [] (s32 index)
	{
		return m_data[index];
	}

	template<typename T>
	inline T const &		array_t<T>::operator [] (s32 index) const
	{
		return m_data[index];
	}

	template<typename T>
	inline array_iter_t<T>	array_t<T>::begin() const
	{
		return array_iter_t<T>(*this);
	}


	template<typename T>
	void				make(memory mem, array_t<T>& proto, s32 cap, s32 len);
	template<typename T>
	bool				iterate(array_iter_t<T>& iter);
	template<typename T>
	bool				append(array_t<T>& array, T const& element);
	template<typename T>
	void				sort(array_t<T>& array);


	template<typename T>
	inline void			make(memory mem, array_t<T>& proto, s32 cap, s32 len)
	{
		if (len > cap)
			cap = len;

		make(mem, proto.m_data, cap);
		proto.m_size = len;
	}

	template<typename T>
	inline bool			iterate(array_iter_t<T>& iter)
	{
		return iter.next();
	}

	template<typename T>
	inline bool			append(array_t<T>& array, T const& element)
	{
		// Logic:
		// Appending inline will only happen when the refcount on the
		// data is 1, otherwise a new copy will be made.
		u32 maxsize = array.m_data.size();
		if (array.m_data.m_slice.refcnt() == 1 && (array.size() < maxsize))
		{
			array.m_data[array.m_size] = element;
			array.m_size += 1;
		}
		else {
			// Make a copy, first see what kind of size we need
			if (array.size() == maxsize)
				maxsize += 2 * (maxsize / 3);
			array.m_data.m_slice.resize(maxsize);

			array.m_data[array.m_size] = element;
			array.m_size += 1;
		}
		return true;
	}
	
	template<typename T>
	inline void			sort(array_t<T>& array)
	{
		xbyte* data = array.m_data.m_slice.mData->mData;
		u32 n = array.m_size;
		u32 es = array.m_data.m_slice.mData->mItemSize;
		xqsort(data, n, es, &array.compare, NULL);
	}

	// ----------------------------------------------------------------------------------------
	//   FREELIST
	// ----------------------------------------------------------------------------------------

	template<typename T>
	class freelist_t
	{
	public:
						freelist_t() : m_max_cnt(0), m_free_cnt(0), m_free(NULL), m_items(NULL) {}

		u32				size() const;
		u32				max() const;

		T*				alloc();
		void			dealloc(T*);

	protected:
		u32				m_max_cnt;
		u32				m_free_cnt;
		u32*			m_free;
		T*				m_items;
	};

	template<typename T>
	void				make(memory mem, freelist_t<T>& proto, s32 cap);

	template<typename T>
	class indexed_t
	{
	public:
		inline			indexed_t() : m_free(), m_items() {}

		u32				size() const;
		u32				max() const;

		bool			is_empty() const;
		bool			is_full() const;

		T*				get_ptr(u32 idx);

		bool			alloc(T*& ptr, u32& index);
		void			dealloc(u32 index);

		u32				m_size;
		array_t<u32>	m_free;
		array_t<T>		m_items;
	};

	template<typename T>
	u32				indexed_t<T>::size() const
	{
		return m_size;
	}

	template<typename T>
	u32				indexed_t<T>::max() const
	{
		return m_free.size();
	}

	template<typename T>
	bool			indexed_t<T>::is_empty() const
	{
		return size() == 0;
	}

	template<typename T>
	bool			indexed_t<T>::is_full() const
	{
		return size() == max();
	}

	template<typename T>
	T*				indexed_t<T>::get_ptr(u32 idx)
	{
		return &m_items[idx];
	}

	template<typename T>
	bool			indexed_t<T>::alloc(T*& ptr, u32& index)
	{
		if (is_full())
			return false;
		s32 const i = max() - m_size - 1;
		index = m_free[i];
		m_items[i] = T();	// Construct
		ptr = get_ptr(index);
		m_size += 1;
		return true;
	}

	template<typename T>
	void			indexed_t<T>::dealloc(u32 index)
	{
		m_size -= 1;
		s32 const i = max() - m_size - 1;
		m_free[i] = index;
		(&m_items[i])->~T();	// Destruct
	}



	template<typename T>
	void				make(memory mem, indexed_t<T>& proto, s32 cap);

	template<typename T>
	void				make(memory mem, indexed_t<T>& proto, s32 cap)
	{
		make(mem, proto.m_free, cap, cap);
		make(mem, proto.m_items, cap, cap);
		proto.m_size = cap;

		range_t<s32> iter(from(0), count(cap));
		while (iterate(iter)) {
			proto.m_free[*iter] = *iter;
		}
	}

	// ----------------------------------------------------------------------------------------
	//   MAP
	// ----------------------------------------------------------------------------------------
	struct map_index
	{
		static bool			is_null(u32 idx)
		{
			return (idx & 0x7fffffff) == 0x0;
		}
		static bool			is_node(u32 idx)
		{
			return (idx & 0x80000000) == 0x80000000;
		}
		static bool			is_item(u32 idx)
		{
			return !is_null(idx) && !is_node(idx);
		}
		static u32			make_node_idx(u32 idx)
		{
			return idx | 0x80000000;
		}
		static u32			make_item_idx(u32 idx)
		{
			return idx & 0x7fffffff;
		}
		static u32			get_idx(u32 idx)
		{
			return idx & 0x7fffffff;
		}
	};
	template<typename K, typename V>
	class map_item_t
	{
	public:
		K				m_key;
		V				m_value;
	};

	template<typename K, typename V>
	class map_items_t
	{
	public:
		typedef			map_item_t<K, V>	item_t;
		indexed_t<item_t> m_items;

		u32					alloc()
		{
			teim_t* ptr;
			u32 idx;
			m_items.alloc(ptr, idx);
			return idx;
		}

		item_t*				get_item(u32 idx)
		{
			item_t* item = m_items.get_ptr(idx);
			return item;
		}
	};

	struct map_node_indexer;

	class map_node_t
	{
	public:
		enum { SIZE = 4 };
		u32				m_nodes[SIZE];

		u32				get(u8 idx) const { return m_nodes[idx]; }

	
		bool			find(u8& idx) const
		{
			while (idx < SIZE)
			{
				if (!map_index::is_null(m_nodes[idx]))
					return true;
				++idx;
			}
			return false;
		}
	};

	struct map_nodes_t
	{
		typedef				map_node_t			node_t;
		indexed_t<node_t>	m_nodes;

		u32					alloc()
		{
			node_t* ptr;
			u32 idx;
			m_nodes.alloc(ptr, idx);
			return idx;
		}

		node_t*				get(u32 idx)
		{
			node_t* node = m_nodes.get_ptr(idx);
			return node;
		}
	};

	struct map_table_t
	{
		u32					get(map_node_indexer& ni);

		u32					get(u32 idx) const;
		void				set(u32 root, u32 idx);
	
		// The initial size of the root table does have an impact
		// on the performance of this map. The larger the table
		// the better performance however this does use more
		// memory. Mostly it is sufficient to put the root table
		// size to around 25% of the maximum amount if items that
		// you are going to insert.
		// Initial size MUST be a power-of-2 like 1024, 2048, 8192.
		// Default is 1024
		u32					m_table_po2;	// root table size in power-of-2 (1<<x)
		u32					*m_table;		// root table 
	};


	// Node Indexer can give you the index at every level, from root down-to the last node of
	// the key that you initialized it with.
	// Default 
	struct map_node_indexer
	{
		u64				m_key;
		u32				m_mask;
		u32				m_rshift;	// root shift
		u32				m_dshift;	// delta shift

		u32				initialize(u64 key, u32 rshift=10, u32 node_size=map_node_t::SIZE)
		{
			m_key = key;
			m_mask = (1 << node_size) - 1;
			m_rshift = rshift;
			m_dshift = node_size;
			return root();
		}

		u32				max_depth() const
		{
			// e.g.: (64 - 10) / 2 = 17
			return ((sizeof(m_key) * 8) - m_rshift) / m_dshift;
		}

		u32				root() const
		{
			return (u32)(m_key & ((u64(1) << m_rshift) - 1));
		}

		u32				get(u32 depth) const
		{
			u32 shift = m_rshift + depth * m_dshift;
			return (u32)((m_key >> shift) & m_mask);
		}
	};


	inline u32			map_table_t::get(u32 idx) const {
		return m_table[idx];
	}

	inline u32			map_table_t::get(map_node_indexer& ni)
	{
		u32 idx = ni.root();
		return m_table[idx];
	}

	inline void			map_table_t::set(u32 root, u32 idx)
	{
		m_table[root] = idx;
	}


	template<typename K, typename V>
	class map_iter_t
	{
	public:
		map_iter_t(array_t<map_item_t<K, V>*>& table) : m_idx(0), m_max(0), m_table(table) {}

		s32				index() const	{ return m_idx; }
		K const&		key() const		{ return m_item->m_key; }
		V const&		value() const	{ return m_item->m_value; }

		bool			next()
		{
			while (m_stack_idx == 0) 
			{
				if (m_table_index < m_table_size) 
				{
					// Find next non null entry in the root table
					u32 ientry = 0;
					do
					{
						++m_table_index;
						ientry = m_table->get(m_table_index);
					} while (map_index::is_null(ientry) && m_table_index < m_table_size);

					// Push it onto the stack
					if (map_index::is_item(ientry))
					{
						m_stack_entry[m_stack_index++] = ientry;
					}
					else if (map_index::is_node(ientry)) 
					{
						map_node_t* pnode = m_nodes.get(ientry);
						for (s32 i=0; i<map_node_t::SIZE; ++i) 
						{
							ientry = pnode->get(i);
							if (!map_index::is_null(i)) 
							{
								m_stack_node[m_stack_index++] = ientry;
							}
						}
					}
				} 
				else 
				{
					return false;
				}
			}

			while (m_stack_idx > 0) 
			{
				// Get something from the stack.
				// When we get an entry from the stack 
				// check if it is a node or an item.
				u32 ientry = m_stack_node[--m_stack_idx];
				if (map_index::is_item(ientry)) 
				{
					m_item = m_items.get(ientry);
					break;
				} 
				else 
				{
					map_node_t* pnode = m_nodes->get(ientry);
					for (s32 i=0; i<map_node_t::SIZE; ++i) 
					{
						ientry = pnode->get(i);
						if (!map_index::is_null(i)) 
						{
							m_stack_node[m_stack_index++] = ientry;
						}
					}
				}
			}

			return true;
		}

	protected:
		s32				m_idx;
		s32				m_max;

		map_item_t*		m_item;

		map_nodes_t*	m_nodes;
		map_items_t*	m_items;

		u32				m_stack_idx;
		u32				m_stack_entry[32*map_node_t::SIZE];

		u32				m_table_index;
		u32				m_table_size;
		map_table_t*	m_table;
	};

	u64		map_key_hasher(xbyte const* data, u32 size);

	template<typename K>
	u64		map_key_hash(K const& key) { return map_key_hasher((xbyte const*)&key, sizeof(u32)); }


	template<typename K, typename V>
	class map_t
	{
	public:
						map_t();

		s32				len() const;
		s32				cap() const;

		V &				operator [] (const K& key);
		V const &		operator [] (const K& key) const;

		map_iter_t<K, V> begin();

	protected:
		map_item_t<K, V>* get(K const& key) const
		{
			map_node_indexer indxr;
			indxr.initialize(map_key_hash<K>(key));

			map_node_t* pnode = NULL;

			u32 const ientry = m_table.get(indxr);
			if (map_index::is_item(ientry))
			{
				u32 const iitem = map_index::get_idx(ientry);
				// We need to put a node here
				// We also need the hash of the key of this item
				map_item_t<K, V>* pitem = m_items.get_item(iitem);
				return pitem;
			}
			else if (map_index::is_node(ientry)) {
				// The entry in the table is a node, ok
				u32 const inode = map_index::get_idx(ientry);
				pnode = m_nodes.get(inode);
			}
			else if (map_index::is_null(ientry)) {
				return NULL;
			}

			u32 depth = 1;
			while (depth < indxr.max_depth())
			{
				u32 const ientry = pnode->get(indxr);
				if (map_index::is_item(ientry))
				{
					u32 const iitem = map_index::get_idx(ientry);
					// We need to put a node here
					// We also need the hash of the key of this item
					map_item_t<K, V>* pitem = m_items.get_item(iitem);
					return pitem;
				}
				else if (map_index::is_node(ientry)) {
					// The entry is a node, ok
					u32 const inode = map_index::get_idx(ientry);
					pnode = m_nodes.get(inode);
				}
				else if (map_index::is_null(ientry)) {
					return NULL;
				}
				++depth;
			}
		}

		void			add(K const& key, V const& value)
		{
			map_node_indexer indxr;
			indxr.initialize(map_key_hash<K>(key));
			
			u32 depth = 0;
			map_node_t* pnode = NULL;
			while (depth < 17)
			{
				if (depth == 0)
				{
					u32 const ientry = m_table.get(indxr);
					if (map_index::is_item(ientry))
					{
						u32 const iitem = map_index::get_idx(ientry);
						// We need to put a node here
						// We also need the hash of the key of this item
						map_item_t<K, V>* pitem = m_items.get_item(iitem);
						u32 const inode = m_nodes.alloc() | 0x80000000;
						pnode = m_nodes.get(map_index::get_idx(inode));
						map_node_indexer indxr2;
						indxr2.initialize(map_key_hash<K>(pitem->m_key));
						pnode->set(indxr2, depth + 1, iitem);
						m_table.set(indxr, inode);
					} else if (map_index::is_node(ientry)) {
						// The entry in the table is a node, ok
						u32 const inode = map_index::get_idx(ientry);
						pnode = m_nodes.get(inode);
					} else if (map_index::is_null(ientry)) {
						// We can put the item here and we are done
						u32 const iitem = m_items.alloc();
						map_item_t<K, V>* pitem = m_items.get(iitem);
						pitem->m_key = key;
						pitem->m_value = value;
						m_table.set(indxr, iitem);
						break;
					}
				} else {
					// We are at depth > 0 and we have a node
					u32 const ientry = pnode->get(indxr);
					if (map_index::is_item(ientry))
					{
						u32 const iitem = map_index::get_idx(ientry);
						// We need to put a node here
						// We also need the hash of the key of this item
						map_item_t<K, V>* pitem = m_items.get_item(iitem);
						u32 const inode = m_nodes.alloc() | 0x80000000;
						pnode2 = m_nodes.get(map_index::get_idx(inode));
						map_node_indexer indxr2;
						indxr2.initialize(map_key_hash<K>(pitem->m_key));
						pnode2->set(indxr2, depth + 1, iitem);
					} else if (map_index::is_node(ientry)) {
						// The entry is a node, ok
						u32 const inode = map_index::get_idx(ientry);
						pnode = m_nodes.get(inode);
					} else if (map_index::is_null(ientry)) {
						// We can put the item here and we are done
						u32 const iitem = m_items.alloc();
						map_item_t<K, V>* pitem = m_items.get(iitem);
						pitem->m_key = key;
						pitem->m_value = value;
						pnode->set(indxr, iitem);
						break;
					}
				}

				++depth;
			}
		}

		K				m_empty_key;
		V				m_empty_value;

		memory			m_mem;

		map_items_t<K,V> m_items;
		map_nodes_t		m_nodes;
		map_table_t		m_table;
	};

	template<typename K, typename V>
	void				make(memory mem, map_t<K, V>& proto, s32 cap);

	template<typename K, typename V>
	bool				iterate(map_iter_t<K, V>& iter, map_t<K, V>& container);

	// ----------------------------------------------------------------------------------------
	//   QUEUE
	// ----------------------------------------------------------------------------------------

	template<typename T>
	class queue_t;

	template<typename T>
	class queue_iter_t
	{
	public:
		queue_iter_t(slice_t<T>& _slice) : m_index(0), m_slice(_slice) {}

		s32				index() const { return m_index; }
		T const&		item() const { return m_slice[m_index]; }

		T const&		operator * (void) const { return m_slice[m_index]; }

	protected:
		s32				m_head;
		s32				m_tail;
		slice_t<T>&		m_slice;
	};

	template<typename T>
	class queue_t
	{
	public:
		inline			queue_t() : m_head(0), m_tail(0) {}

		s32				len() const;
		s32				cap() const;

		bool			push(T const& item);
		bool			pop(T& item);

		typedef			queue_iter_t<T>		iter_t;
		iter_t			begin() const;

		u32				m_head;
		u32				m_tail;
		slice_t<T>		m_data;
	};

	template<typename T>
	void				make(memory mem, queue_t<T>& proto, s32 cap);
	template<typename T>
	bool				append(queue_t<T>& array, T const& element);


	// ----------------------------------------------------------------------------------------
	//   SLICE DATA
	// ----------------------------------------------------------------------------------------

	struct slice_data
	{
		slice_data();
		slice_data(u32 item_count, u32 item_size);
		slice_data(xbyte* data, u32 item_count, u32 item_size);

		static slice_data	sNull;

		slice_data* 		incref();
		slice_data* 		incref() const;
		slice_data*			decref();

		slice_data*			resize(s32 from, s32 to);

		static slice_data*	alloc(x_iallocator* allocator, u32& to_itemcount, u32& to_itemsize);

		mutable s32			mRefCount;
		u32					mItemCount;					/// Count of total items
		u32					mItemSize;					/// Size of one item
		u32					mDummy;
		x_iallocator*		mAllocator;
		xbyte*				mData;
	};


	template<typename T>
	bool				range(queue_iter_t<T>& iter, queue_t<T>& container);
}

#endif
