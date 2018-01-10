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
		inline			memory() : m_allocator(NULL) {}
		inline			memory(x_iallocator* allocator) : m_allocator(allocator) {}

		void*			alloc(xsize_t size, u32 align)				{ return m_allocator->allocate(size, align); }
		void*			realloc(void* p, xsize_t size, u32 align)	{ return m_allocator->reallocate(p, size, align); }
		void			dealloc(void* p)							{ return m_allocator->deallocate(p); }

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

		array_t<T>		operator () (u32 to) const;
		array_t<T>		operator () (u32 from, u32 to) const;

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
	inline array_t<T>		array_t<T>::operator () (u32 to) const
	{
		array_t<T> other;
		other.m_data = m_data(0, to);
		other.m_size = m_size;
		if (to < m_size)
			other.m_size = to;
		return other;
	}

	template<typename T>
	inline array_t<T>		array_t<T>::operator () (u32 from, u32 to) const
	{
		array_t<T> other;
		other.m_data = m_data(from, to);
		other.m_size = m_size;
		if (from >= m_size) {
			other.m_size = 0;
		}
		else if (to > m_size) {
			other.m_size = m_size - from;
		}
		else {
			other.m_size = to - from;
		}
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

		T*				get(u32 idx);
		T const*		get(u32 idx) const;

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
	T*				indexed_t<T>::get(u32 idx)
	{
		return &m_items[idx];
	}

	template<typename T>
	T const*		indexed_t<T>::get(u32 idx) const
	{
		return &m_items[idx];
	}

	template<typename T>
	bool			indexed_t<T>::alloc(T*& ptr, u32& index)
	{
		if (is_full())
			return false;
		index = m_free[m_size];
		m_items[index] = T();	// Construct
		ptr = get(index);
		m_size += 1;
		return true;
	}

	template<typename T>
	void			indexed_t<T>::dealloc(u32 index)
	{
		m_size -= 1;
		m_free[m_size] = index;
		(&m_items[index])->~T();	// Destruct
	}



	template<typename T>
	void				make(memory mem, indexed_t<T>& proto, s32 cap);

	template<typename T>
	void				make(memory mem, indexed_t<T>& proto, s32 cap)
	{
		make(mem, proto.m_free, cap, cap);
		make(mem, proto.m_items, cap, cap);
		proto.m_size = 0;

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

		inline u32			size() const { return m_items.size(); }

		u32					alloc()
		{
			teim_t* ptr;
			u32 idx;
			m_items.alloc(ptr, idx);
			return idx;
		}
		void				dealloc(u32 idx)
		{
			m_items.dealloc(idx);
		}

		item_t*				get(u32 idx)
		{
			item_t* item = m_items.get(idx);
			return item;
		}
		item_t const*		get(u32 idx) const
		{
			item_t const* item = m_items.get(idx);
			return item;
		}

	};

	struct map_node_indexer;

	class map_node_t
	{
	public:
		enum { SIZE = 4 };
		u32				m_nodes[SIZE];

		u32				get(u8 idx) const
		{
			return m_nodes[idx]; 
		}
	
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

		inline u32			size() const { return m_nodes.size(); }

		u32					alloc()
		{
			node_t* ptr;
			u32 idx;
			m_nodes.alloc(ptr, idx);
			return idx;
		}

		node_t *			get(u32 idx)
		{
			node_t * node = m_nodes.get(idx);
			return node;
		}

		node_t const*		get(u32 idx) const
		{
			node_t const* node = m_nodes.get(idx);
			return node;
		}
	};

	struct map_table_t
	{
		u32					get(map_node_indexer const& ni) const;

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
		u32					m_table_size;	// root table size in power-of-2 (1<<x)
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


	inline u32			map_table_t::get(map_node_indexer const& ni) const
	{
		u32 idx = ni.root();
		return m_table[idx];
	}

	inline u32			map_table_t::get(u32 idx) const 
	{
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
		inline map_iter_t()
			: m_idx(0)
			, m_max(0)
			, m_item(NULL)
			, m_nodes(NULL)
			, m_items(NULL)
			, m_stack_idx(0)
			, m_table_index(0)
			, m_table_size(0)
			, m_table(NULL)
		{}

		s32				index() const	{ return m_idx; }
		K const&		key() const		{ return m_item->m_key; }
		V const&		value() const	{ return m_item->m_value; }

		bool			next();

	protected:
		s32				m_idx;
		s32				m_max;

		map_item_t*		m_item;

		map_nodes_t*	m_nodes;
		map_items_t*	m_items;

		u32				m_stack_idx;
		u32				m_stack_entry[32*map_node_t::SIZE];

		u32				m_table_index;
		map_table_t*	m_table;
	};

	template<typename K, typename V>
	bool			map_iter_t<K,V>::next()
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
					for (s32 i = 0; i<map_node_t::SIZE; ++i)
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
				for (s32 i = 0; i<map_node_t::SIZE; ++i)
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

	u64		map_key_hasher(xbyte const* data, u32 size);

	template<typename K>
	u64		map_key_hash(K const& key) { return map_key_hasher((xbyte const*)&key, sizeof(u32)); }

	template<typename K, typename V>
	class map_t;

	template<typename K, typename V>
	struct map_value_t
	{
						map_value_t(map_t<K, V>& map);

						operator bool () const;
						operator V const& () const;
						operator V & ();
						operator V ();

		map_t<K, V>&	operator = (const V& value);

		map_item_t<K, V>* m_item;
		map_t<K, V>&	m_map;
	};

	template<typename K, typename V>
	class map_t
	{
	public:
		K				m_empty_key;
		V				m_empty_value;

		memory			m_mem;

		map_items_t<K, V> m_items;
		map_nodes_t		m_nodes;
		map_table_t		m_table;

						map_t();

		s32				len() const;
		s32				cap() const;

		map_value_t<K, V> operator [] (const K& key);
		V const &		operator [] (const K& key) const;

		map_iter_t<K, V> begin();

	protected:
		u32				get(K const& key) const;
		void			add(K const& key, V const& value);
	};

	template<typename K, typename V>
	map_t<K, V>::map_t()
	{

	}

	template<typename K, typename V>
	s32				map_t<K, V>::len() const
	{
		return m_items.size();
	}

	template<typename K, typename V>
	s32				map_t<K, V>::cap() const
	{
		return 0;
	}

	template<typename K, typename V>
	map_value_t<K, V>	map_t<K, V>::operator [] (const K& key)
	{
		u32 const iitem = get(key);
		map_value_t<K, V> value(*this);
		value.m_item = m_items.get(iitem);
		return value;
	}

	template<typename K, typename V>
	V const &		map_t<K, V>::operator [] (const K& key) const
	{
		u32 const iitem = get(key);
		if (iitem == 0)
			return m_empty_value;
		map_item_t<K, V> const * item = m_items.get(iitem);
		return item->m_value;
	}

	template<typename K, typename V>
	map_iter_t<K, V> map_t<K, V>::begin()
	{
		map_iter_t<K, V> iter;
		iter.m_idx = 0;
		iter.m_max = 0;
		iter.m_item = NULL;
		iter.m_nodes = &m_nodes;
		iter.m_items = &m_items;
		iter.m_stack_idx = 0;
		iter.m_table_index = 0;
		iter.m_table = &m_table;
	}

	template<typename K, typename V>
	u32		map_t<K,V>::get(K const& key) const
	{
		map_node_indexer indxr;
		indxr.initialize(map_key_hash<K>(key));

		map_node_t const* pnode = NULL;

		u32 const ientry = m_table.get(indxr);
		if (map_index::is_item(ientry))
		{
			u32 const iitem = map_index::get_idx(ientry);
			// We need to put a node here
			// We also need the hash of the key of this item
			// map_item_t<K, V> const* pitem = m_items.get(iitem);
			return iitem;
		}
		else if (map_index::is_node(ientry)) {
			// The entry in the table is a node, ok
			u32 const inode = map_index::get_idx(ientry);
			pnode = m_nodes.get(inode);
		}
		else if (map_index::is_null(ientry)) {
			return 0;
		}

		u32 depth = 1;
		while (depth < indxr.max_depth())
		{
			u32 const ientry = pnode->get(indxr.get(depth));
			if (map_index::is_item(ientry))
			{
				u32 const iitem = map_index::get_idx(ientry);
				// We need to put a node here
				// We also need the hash of the key of this item
				//map_item_t<K, V>* pitem = m_items.get(iitem);
				return iitem;
			}
			else if (map_index::is_node(ientry)) {
				// The entry is a node, ok
				u32 const inode = map_index::get_idx(ientry);
				pnode = m_nodes.get(inode);
			}
			else if (map_index::is_null(ientry)) {
				return 0;
			}
			++depth;
		}
		return 0;
	}

	template<typename K, typename V>
	void			map_t<K,V>::add(K const& key, V const& value)
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
				}
				else if (map_index::is_node(ientry)) {
					// The entry in the table is a node, ok
					u32 const inode = map_index::get_idx(ientry);
					pnode = m_nodes.get(inode);
				}
				else if (map_index::is_null(ientry)) {
					// We can put the item here and we are done
					u32 const iitem = m_items.alloc();
					map_item_t<K, V>* pitem = m_items.get(iitem);
					pitem->m_key = key;
					pitem->m_value = value;
					m_table.set(indxr, iitem);
					break;
				}
			}
			else {
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
				}
				else if (map_index::is_node(ientry)) {
					// The entry is a node, ok
					u32 const inode = map_index::get_idx(ientry);
					pnode = m_nodes.get(inode);
				}
				else if (map_index::is_null(ientry)) {
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

	template<typename K, typename V>
	void				make(memory mem, map_t<K, V>& proto, s32 cap);

	template<typename K, typename V>
	bool				iterate(map_iter_t<K, V>& iter);


	template<typename K, typename V>
	void				make(memory mem, map_t<K, V>& proto, s32 cap)
	{
		proto.m_mem = mem;
		make(mem, proto.m_items.m_items, cap);
		make(mem, proto.m_nodes.m_nodes, cap);
		proto.m_table.m_table_size = 1 << 10;
		proto.m_table.m_table = (u32*)mem.alloc(sizeof(u32) * proto.m_table.m_table_size, sizeof(void*));
	}

	template<typename K, typename V>
	bool				iterate(map_iter_t<K, V>& iter)
	{
		return iter.next();
	}


	// ----------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------
	template<typename K, typename V>
	map_value_t<K, V>::map_value_t(map_t<K, V>& map)
		: m_map(map)
		, m_item(NULL)
	{
	}

	template<typename K, typename V>
	map_value_t<K, V>::operator bool() const
	{
		return m_item != NULL;
	}

	template<typename K, typename V>
	map_value_t<K, V>::operator V const& () const
	{
		return m_item->m_value;
	}

	template<typename K, typename V>
	map_value_t<K, V>::operator V & ()
	{
		return m_item->m_value;
	}

	template<typename K, typename V>
	map_value_t<K, V>::operator V ()
	{
		return m_item->m_value;
	}

	template<typename K, typename V>
	map_t<K, V>&	map_value_t<K, V>::operator = (const V& value)
	{
		return m_map;
	}

	// ----------------------------------------------------------------------------------------
	//   QUEUE
	// ----------------------------------------------------------------------------------------

	template<typename T>
	class queue_t;

	template<typename T>
	class queue_iter_t
	{
	public:
		queue_iter_t(slice_t<T>& _slice, s32 head, s32 tail) : m_head(head), m_tail(tail), m_index(head), m_slice(_slice) { m_size = m_slice.size(); }

		s32				index() const			{ return m_index; }
		T const&		item() const			{ return m_slice[m_index]; }

		T const&		operator * (void) const	{ return m_slice[m_index]; }

		bool			next();

		s32				m_head;
		s32				m_tail;
		s32				m_index;
		s32				m_size;
		slice_t<T>&		m_slice;
	};

	template<typename T>
	class queue_t
	{
	public:
		inline			queue_t() : m_head(0), m_tail(0) {}

		u32				size() const;
		u32				max() const;

		bool			is_empty() const;
		bool			is_full() const;

		bool			push(T const& item);
		bool			pop(T& item);

		typedef			queue_iter_t<T>		iter_t;
		iter_t			begin() const;

		u32				m_head;
		u32				m_tail;
		slice_t<T>		m_data;
	};

	template<typename T>
	u32				queue_t<T>::size() const
	{
		return m_head - m_tail;
	}

	template<typename T>
	u32				queue_t<T>::max() const
	{
		return m_data.size();
	}

	template<typename T>
	bool			queue_t<T>::is_empty() const
	{
		return m_head == m_tail;
	}

	template<typename T>
	bool			queue_t<T>::is_full() const
	{
		return (m_head > m_tail) && (m_head % size()) == m_tail;
	}

	template<typename T>
	bool			queue_t<T>::push(T const& item)
	{
		if (is_full())
			return false;
		s32 const index = m_head % size();
		m_data[index] = item;
		m_head++;
		return true;
	}

	template<typename T>
	bool			queue_t<T>::pop(T& item)
	{
		if (is_empty())
			return false;
		s32 const index = m_tail % size();
		item = m_data[index];
		m_tail += 1;
		if (m_tail >= size())
		{
			m_tail = m_tail % size();
			m_head = m_head % size();
		}
		return true;
	}

	template<typename T>
	queue_iter_t<T>	queue_t<T>::begin() const
	{
		queue_t<T>::iter_t iter(m_data, m_head, m_tail);
		return iter;
	}



	template<typename T>
	void				make(memory mem, queue_t<T>& proto, s32 cap);
	template<typename T>
	bool				append(queue_t<T>& array, T const& element);
	template<typename T>
	bool				iterate(queue_iter_t<T>& iter);



	template<typename T>
	inline void			make(memory mem, queue_t<T>& proto, s32 cap)
	{
		make(mem, proto.m_data, cap);
		proto.m_head = 0;
		proto.m_tail = 0;
	}

	template<typename T>
	inline bool			append(queue_t<T>& q, T const& element)
	{
		return q.push(element);
	}

	template<typename T>
	inline bool			iterate(queue_iter_t<T>& iter)
	{
		return iter.next();
	}

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
