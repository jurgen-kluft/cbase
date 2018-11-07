#ifndef __XBASE_MAP_T_H__
#define __XBASE_MAP_T_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_range.h"
#include "xbase/x_indexed_t.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	// ----------------------------------------------------------------------------------------
	//   MAP
	// ----------------------------------------------------------------------------------------
	struct map_index
	{
		static const u32 nill = 0xffffffff;

		static bool			is_null(u32 idx)
		{
			return idx == nill;
		}
		static bool			is_node(u32 idx)
		{
			return !is_null(idx) && (idx & 0x80000000) == 0x80000000;
		}
		static bool			is_item(u32 idx)
		{
			return !is_null(idx) && (idx & 0x80000000) == 0x00000000;
		}
		static u32			make_node_idx(u32 idx)
		{
			return idx | 0x80000000;
		}
		static u32			make_item_idx(u32 idx)
		{
			return idx & 0x7fffffff;
		}

		static inline u32	idx(u32 idx) { return idx & 0x7fffffff; }
	};

	template<typename K, typename V>
	class map_item_t
	{
	public:
		K				m_key;
		V				m_value;

		XCORE_CLASS_PLACEMENT_NEW_DELETE
	};

	template<typename K, typename V>
	class map_items_t
	{
	public:
		typedef			map_item_t<K, V>	item_t;
		indexed_t<item_t> m_items;

		inline u32			size() const { return m_items.size(); }
		inline u32			max() const { return m_items.max(); }

		u32					alloc()
		{
			item_t* ptr;
			u32 idx;
			m_items.alloc(ptr, idx);
			return map_index::make_item_idx(idx);
		}
		void				dealloc(u32 idx)
		{
			ASSERT(map_index::is_item(idx));
			ASSERT(map_index::idx(idx) < max());
			m_items.dealloc(map_index::idx(idx));
		}

		item_t*				get(u32 idx)
		{
			ASSERT(map_index::is_item(idx));
			ASSERT(map_index::idx(idx) < max());
			item_t* item = m_items.get(map_index::idx(idx));
			return item;
		}
		item_t const*		get(u32 idx) const
		{
			ASSERT(map_index::is_item(idx));
			ASSERT(map_index::idx(idx) < max());
			item_t const* item = m_items.get(map_index::idx(idx));
			return item;
		}

	};

	struct map_node_indexer;

	class map_node_t
	{
	public:
		enum { SIZE = 4 };
		u32				m_nodes[SIZE];

		void			init()
		{
			for (s32 i = 0; i < map_node_t::SIZE; ++i)
				m_nodes[i] = map_index::nill;
		}

		u32				get(u8 idx) const
		{
			ASSERT(idx < SIZE);
			return m_nodes[idx];
		}

		void			set(u8 idx, u32 item)
		{
			ASSERT(idx < SIZE);
			m_nodes[idx] = item;
		}

		inline bool		is_empty() const {
			for (s32 i = 0; i < map_node_t::SIZE; ++i) {
				if (m_nodes[i] != map_index::nill) {
					return false;
				}
			}
			return true;
		}

		bool			find(u8& idx) const
		{
			while (idx < SIZE)
			{
				ASSERT(idx < SIZE);
				if (!map_index::is_null(m_nodes[idx]))
					return true;
				++idx;
			}
			return false;
		}

		XCORE_CLASS_PLACEMENT_NEW_DELETE
	};

	struct map_nodes_t
	{
		typedef				map_node_t			node_t;
		indexed_t<node_t>	m_nodes;

		inline u32			size() const { return m_nodes.size(); }
		inline u32			max() const { return m_nodes.max(); }

		u32					alloc()
		{
			u32 idx;
			node_t* ptr;
			m_nodes.alloc(ptr, idx);
			ptr->init();
			return map_index::make_node_idx(idx);
		}
		void				dealloc(u32 idx)
		{
			ASSERT(map_index::is_node(idx));
			ASSERT(map_index::idx(idx) < max());
			m_nodes.dealloc(map_index::idx(idx));
		}

		node_t *			get(u32 idx)
		{
			ASSERT(map_index::is_node(idx));
			ASSERT(map_index::idx(idx) < max());
			node_t * node = m_nodes.get(map_index::idx(idx));
			return node;
		}

		node_t const*		get(u32 idx) const
		{
			ASSERT(map_index::is_node(idx));
			ASSERT(map_index::idx(idx) < max());
			node_t const* node = m_nodes.get(map_index::idx(idx));
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
			m_mask = node_size - 1;
			m_rshift = rshift;
			m_dshift = xcore::xilog2(node_size);
			return root();
		}

		u32				max_depth() const
		{
			// e.g.: (64 - 10) / 2 = 17
			return ((sizeof(m_key) * 8) - m_rshift) / m_dshift;
		}

		inline u32		root() const
		{
			return (u32)(m_key & ((u64(1) << m_rshift) - 1));
		}

		u32				get(u32 depth) const
		{
			if (depth == 0)
				return root();
			u32 const shift = m_rshift + depth * m_dshift;
			return (u32)((m_key >> shift) & m_mask);
		}
	};

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
			, m_table(NULL)
		{}

		s32				index() const	{ return m_idx; }
		K const&		key() const		{ return m_item->m_key; }
		V const&		value() const	{ return m_item->m_value; }

		bool			next();

	protected:
		s32				m_idx;
		s32				m_max;

		map_item_t<K,V>* m_item;

		map_nodes_t*	m_nodes;
		map_items_t<K,V>* m_items;

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
			if (m_table_index < m_table->m_table_size)
			{
				// Find next non null entry in the root table
				u32 ientry = 0;
				do
				{
					++m_table_index;
					ientry = m_table->get(m_table_index);
				} while (map_index::is_null(ientry) && m_table_index < m_table->m_table_size);

				// Push it onto the stack
				if (map_index::is_item(ientry))
				{
					m_stack_entry[m_stack_idx++] = ientry;
				}
				else if (map_index::is_node(ientry))
				{
					map_node_t* pnode = m_nodes->get(ientry);
					for (s32 i = 0; i<map_node_t::SIZE; ++i)
					{
						ientry = pnode->get(i);
						if (!map_index::is_null(i))
						{
							m_stack_entry[m_stack_idx++] = ientry;
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
			u32 ientry = m_stack_entry[--m_stack_idx];
			if (map_index::is_item(ientry))
			{
				m_item = m_items->get(ientry);
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
						m_stack_entry[m_stack_idx++] = ientry;
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
						map_value_t(map_t<K, V>& map, K const& key);

		operator		bool ()				{ return m_item != NULL; }
		operator		bool () const		{ return m_item != NULL; }

		operator		V const &()			{ return m_item->m_value; }
		operator		V const &() const	{ return m_item->m_value; }

		map_t<K, V>&	operator = (const V& value);

		map_t<K, V>&	m_map;
		K const&		m_key;
		map_item_t<K, V>* m_item;
	};

	template<typename K, typename V>
	struct map_cvalue_t
	{
		inline					map_cvalue_t(map_t<K, V> const& map, K const& key)
			: m_map(map)
			, m_key(key)
			, m_item(NULL) {}

		const K&				key() const		{ return m_item->m_key; }
		const V&				value() const	{ return m_item->m_value; }

		operator				bool() const	{ return m_item != NULL; }
		operator				V() const		{ return m_item->m_value; }

		map_t<K, V> const&		m_map;
		K const&				m_key;
		map_item_t<K, V> const*	m_item;
	};

	template<typename K, typename V>
	class map_t
	{
	public:
		K				m_empty_key;
		V				m_empty_value;

		xalloc*			m_mem;

		map_items_t<K, V> m_items;
		map_nodes_t		m_nodes;
		map_table_t		m_table;

						map_t();
						~map_t();

		s32				len() const;
		s32				cap() const;

		map_value_t<K, V> operator [] (const K& key);
		map_cvalue_t<K, V> operator [] (const K& key) const;

		bool			remove(K const& key);

		map_iter_t<K, V> begin();

	protected:
		friend struct map_value_t<K, V>;
		u32				get(K const& key) const;
		void			add(K const& key, V const& value);
	};

	template<typename K, typename V>
	map_t<K, V>::map_t()
	{

	}

	template<typename K, typename V>
	map_t<K, V>::~map_t()
	{
		m_mem->deallocate(m_table.m_table);
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
		map_value_t<K, V> value(*this, key);
		if (map_index::is_item(iitem))
		{	// Are the keys indentical, if so then we have found the item.
			map_item_t<K,V>* pitem = m_items.get(iitem);
			if (pitem->m_key == key)
				value.m_item = pitem;
		}
		return value;
	}

	template<typename K, typename V>
	map_cvalue_t<K, V>		map_t<K, V>::operator [] (const K& key) const
	{
		u32 const iitem = get(key);
		map_cvalue_t<K, V> cvalue(*this, key);
		if (map_index::is_item(iitem))
			cvalue.m_item = m_items.get(iitem);
		return cvalue;
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

		u32 const ientry = m_table.get(indxr.root());
		if (map_index::is_item(ientry))
		{
			// We need to put a node here
			// We also need the hash of the key of this item
			// map_item_t<K, V> const* pitem = m_items.get(iitem);
			return ientry;
		}
		else if (map_index::is_node(ientry)) {
			// The entry in the table is a node, ok
			pnode = m_nodes.get(ientry);
		}
		else if (map_index::is_null(ientry)) {
			return map_index::nill;
		}

		u32 depth = 1;
		while (depth < indxr.max_depth())
		{
			u32 const ientry = pnode->get(indxr.get(depth));
			if (map_index::is_item(ientry))
			{
				// We need to put a node here
				// We also need the hash of the key of this item
				//map_item_t<K, V>* pitem = m_items.get(iitem);
				return ientry;
			}
			else if (map_index::is_node(ientry)) {
				// The entry is a node, ok
				pnode = m_nodes.get(ientry);
			}
			else if (map_index::is_null(ientry)) {
				return map_index::nill;
			}
			++depth;
		}
		return map_index::nill;
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
				u32 const ientry = m_table.get(indxr.root());
				if (map_index::is_item(ientry))
				{
					// We need to put a node here
					// We also need the hash of the key of this item
					map_item_t<K, V>* pitem = m_items.get(ientry);
					u32 const inode_next = m_nodes.alloc();
					map_node_t* pnode_next = m_nodes.get(inode_next);
					map_node_indexer indxr2;
					indxr2.initialize(map_key_hash<K>(pitem->m_key));
					pnode_next->set(indxr2.get(depth + 1), ientry);
					m_table.set(indxr.root(), inode_next);
					pnode = pnode_next;
				}
				else if (map_index::is_node(ientry)) {
					// The entry in the table is a node, ok
					pnode = m_nodes.get(ientry);
				}
				else if (map_index::is_null(ientry)) {
					// We can put the item here and we are done
					u32 const iitem = m_items.alloc();
					map_item_t<K, V>* pitem = m_items.get(iitem);
					pitem->m_key = key;
					pitem->m_value = value;
					m_table.set(indxr.root(), iitem);
					break;
				}
			}
			else {
				// We are at depth > 0 and we have a node
				u32 const ientry = pnode->get(indxr.get(depth));
				if (map_index::is_item(ientry))
				{
					// We need to put a node here
					// We also need the hash of the key of this item
					map_item_t<K, V>* pitem = m_items.get(ientry);
					u32 const inode_next = m_nodes.alloc();
					map_node_t* pnode_next = m_nodes.get(inode_next);
					map_node_indexer indxr2;
					indxr2.initialize(map_key_hash<K>(pitem->m_key));
					pnode_next->set(indxr2.get(depth + 1), ientry);
					pnode->set(indxr.get(depth), inode_next);
					pnode = pnode_next;
				}
				else if (map_index::is_node(ientry)) {
					// The entry is a node, ok
					pnode = m_nodes.get(ientry);
				}
				else if (map_index::is_null(ientry)) {
					// We can put the item here and we are done
					u32 const iitem = m_items.alloc();
					map_item_t<K, V>* pitem = m_items.get(iitem);
					pitem->m_key = key;
					pitem->m_value = value;
					pnode->set(indxr.get(depth), iitem);
					break;
				}
			}

			++depth;
		}
	}

	template<typename K, typename V>
	bool			map_t<K, V>::remove(K const& key)
	{
		map_node_indexer indxr;
		indxr.initialize(map_key_hash<K>(key));

		u32	path_nodes[32];
		map_node_t* pnode = NULL;

		u32 depth = 0;
		while (depth < indxr.max_depth())
		{
			u32 ientry;
			if (depth == 0)
			{
				ientry = m_table.get(indxr.get(depth));
			}
			else
			{
				ientry = pnode->get(indxr.get(depth));
			}

			if (map_index::is_item(ientry))
			{
				map_item_t<K, V>* pitem = m_items.get(ientry);
				if (pitem->m_key == key) {
					m_items.dealloc(ientry);
					if (depth == 0) {
						m_table.set(indxr.get(depth), map_index::nill);
						return true;
					}
					break;
				}
				else {
					return false;
				}
			}
			else if (map_index::is_node(ientry)) {
				// The entry is a node, ok
				path_nodes[depth] = ientry;
				pnode = m_nodes.get(ientry);
			}
			else if (map_index::is_null(ientry)) {
				return map_index::nill;
			}
			++depth;
		}

		// Go back up the path and see if we can dealloc empty map nodes
		if (depth < indxr.max_depth()) {
			while (depth > 0) {
				depth -= 1;
				u32 const inode = path_nodes[depth];
				pnode = m_nodes.get(inode);
				pnode->set(indxr.get(depth+1), map_index::nill);
				if (!pnode->is_empty())
					break;
				m_nodes.dealloc(inode);
				if (depth == 0) {
					m_table.set(indxr.get(depth), map_index::nill);
					break;
				}
			}
			return true;
		}

		return false;
	}

	template<typename K, typename V>
	void				make(xalloc* mem, map_t<K, V>& proto, s32 cap);

	template<typename K, typename V>
	bool				iterate(map_iter_t<K, V>& iter);


	template<typename K, typename V>
	void				make(xalloc* mem, map_t<K, V>& proto, s32 cap)
	{
		proto.m_mem = mem;
		make(mem, proto.m_items.m_items, cap);
		make(mem, proto.m_nodes.m_nodes, cap);
		proto.m_table.m_table_size = 1 << 10;
		proto.m_table.m_table = (u32*)mem.allocate_and_clear(sizeof(u32) * proto.m_table.m_table_size, sizeof(void*), 0xFFFFFFFF);
	}

	template<typename K, typename V>
	bool				iterate(map_iter_t<K, V>& iter)
	{
		return iter.next();
	}


	// ----------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------
	template<typename K, typename V>
	map_value_t<K, V>::map_value_t(map_t<K, V>& map, K const& key)
		: m_map(map)
		, m_key(key)
		, m_item(NULL)
	{
	}

	template<typename K, typename V>
	map_t<K, V>&	map_value_t<K, V>::operator = (const V& value)
	{
		if (m_item == NULL) {
			// Key is not present in the map, so we have to add it
			m_map.add(m_key, value);
		}
		else {
			// Overwrite the value ?
		}
		return m_map;
	}
}

#endif
