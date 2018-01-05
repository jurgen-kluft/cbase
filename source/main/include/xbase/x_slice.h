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
						slice(x_iallocator* allocator, s32 tosize);
						slice(slice_data* data, s32 from, s32 to);
						slice(slice const& other);
						~slice();

		static void		alloc(slice& slice, x_iallocator* allocator, s32 tosize);

		void			resize(s32 len);
		s32				length() const;

		slice			view(u32 from, u32 to) const;

		void			release();

		slice_data*		mData;
		u32				mFrom;
		u32				mTo;
	};

	template<typename T>
	class slice_t
	{
	public:
						slice_t();

		s32				size() const;

		slice_t			operator () (s32 to) const;
		slice_t			operator () (s32 from, s32 to) const;

		T &				operator [] (s32 index);
		T const &		operator [] (s32 index) const;

		slice_data*		m_data;
	};

	template<typename T>
	void				make(memory mem, slice_t<T>& proto, s32 size);

	// ----------------------------------------------------------------------------------------
	//   ARRAY
	// ----------------------------------------------------------------------------------------
	template<typename T>
	class array_t;

	template<typename T>
	class array_iter_t
	{
	public:
						array_iter_t(array_t<T>& _array) : m_index(0), m_array(_array) {}

		s32				index() const { return m_index; }
		T const&		item() const { return m_array[m_index]; }

		T &				operator * (void)		{ return m_array[m_index]; }
		T const&		operator * (void) const	{ return m_array[m_index]; }

	protected:
		s32				m_index;
		array_t<T>&		m_array;
	};

	template<typename T>
	class array_t
	{
	public:
						array_t() : m_size(0) { }

		s32				len() const;
		s32				cap() const;

		slice_t<T>		operator () (s32 to) const;
		slice_t<T>		operator () (s32 from, s32 to) const;

		T &				operator [] (s32 index);
		T const &		operator [] (s32 index) const;

		array_iter_t<T>	begin() const;

		u32				m_size;
		slice_t<T>		m_data;
	};

	template<typename T>
	void				make(memory mem, array_t<T>& proto, s32 cap, s32 len);


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



	// ----------------------------------------------------------------------------------------
	//   MAP
	// ----------------------------------------------------------------------------------------

	template<typename K, typename V>
	class map_item_t
	{
	public:
		K				m_key;
		V				m_value;
	};

	template<typename K, typename V>
	class map_iter_t
	{
	public:
		map_iter_t(array_t<map_item_t<K, V>*>& table) : m_idx(0), m_max(0), m_table(table) {}

		s32				index() const	{ return m_idx; }
		K const&		key() const		{ return m_table[m_idx].m_key; }
		V const&		value() const	{ return m_table[m_idx].m_value; }

	protected:
		// Iteration variables
		s32				m_idx;
		s32				m_max;
		array_t<map_item_t<K,V>*>& m_table;
	};

	u32		map_key_hasher(xbyte const* data, u32 size);

	template<typename K>
	u32		map_key_hash(K const& key) { return map_key_hasher((xbyte const*)&key, sizeof(u32)); }

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
		void			grow();

		typedef			map_item_t<K, V>	item_t;

		K				m_empty_key;
		V				m_empty_value;

		freelist_t<item_t>	m_items;

		memory			m_mem;
		u32				m_size;
		array_t<item_t*> m_table;
	};

	template<typename K, typename V>
	void				make(memory mem, map_t<K, V>& proto, s32 cap);

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


	// ----------------------------------------------------------------------------------------
	//   SLICE DATA
	// ----------------------------------------------------------------------------------------

	struct slice_data
	{
		slice_data();
		slice_data(u32 size);
		slice_data(xbyte* data, u32 size);

		static slice_data	sNull;

		slice_data* 		incref();
		slice_data* 		incref() const;
		slice_data*			decref();

		slice_data*			resize(s32 from, s32 to);

		static slice_data*	alloc(x_iallocator* allocator, s32& tosize);

		mutable s32			mRefCount;
		s32					mSize;						/// Number of allocated bytes
		x_iallocator*		mAllocator;
		xbyte*				mData;
	};


	// ----------------------------------------------------------------------------------------
	//   FUNCTIONS
	// ----------------------------------------------------------------------------------------
	template<typename T>
	bool				range(array_iter_t<T>& iter, array_t<T>& container);
	template<typename T>
	bool				append(array_t<T>& array, T const& element);
	template<typename T>
	void				sort(array_t<T>& array);

	template<typename K, typename V>
	bool				range(map_iter_t<K, V>& iter, map_t<K, V>& container);

	template<typename T>
	bool				range(queue_iter_t<T>& iter, queue_t<T>& container);
}

#endif
