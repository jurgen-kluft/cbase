#ifndef __XBASE_TREE_H__
#define __XBASE_TREE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_allocator.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class x_iallocator;

	struct xnode_t;

	class xtree
	{
	public:
						xtree(x_iallocator* node_allocator, x_iallocator* item_allocator);

		u32				size() const			{ return m_size;}
		void			clear();

		bool			find(void * data);
		s32				insert(void * data);
		s32				erase(void * data);

		bool			validate(const char*& error_str);

		typedef s32(*compare_f) (void const* p1, void const* p2);
		void			set_cmp(compare_f func) { m_compare = func; }

		x_iallocator*	m_node_allocator;
		x_iallocator*	m_item_allocator;

	protected:
		friend class xtree_internal;

		compare_f		m_compare;
		xnode_t *		m_root;				// Top of the tree
		u32				m_size;				// Number of items (user-defined)
	};

	template<typename T>
	class xtree_t
	{
	public:
		inline			xtree_t(x_iallocator* node_allocator, x_iallocator* item_allocator) : m_tree(node_allocator, item_allocator) { m_tree.set_cmp(&compare); }
		inline			~xtree_t()				{ m_tree.clear(); }

		u32				size() const			{ return m_tree.size(); }
		void			clear()					{ m_tree.clear(); }

		bool			find(T const& data)		{ void* p = (void*)&data; return m_tree.find(p); }
		s32				insert(T const& data);
		s32				erase(T const& data)	{ void* p = (void*)&data; return m_tree.erase(p); }

		bool			validate(const char*& error_str)	{ return m_tree.validate(error_str); }

	protected:
		static s32		compare(void const* p1, void const* p2)
		{
			T const& a = *(T const*)p1;
			T const& b = *(T const*)p2;
			if (a < b)
				return -1;
			else if (a > b)
				return 1;
			return 0;
		}

		struct item_t
		{
		public:
			inline	item_t(T const& data) : m_item(data) {}
			T		m_item;

			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

		xtree			m_tree;
	};

	template<typename T>
	s32					xtree_t<T>::insert(T const& data)
	{
		void* item_mem = m_tree.m_item_allocator->allocate(sizeof(T), sizeof(void*));
		item_t* item = new (item_mem) item_t(data);
		s32 result = m_tree.insert((void*)item);
		if (result == 0) 
			m_tree.m_item_allocator->deallocate(item_mem);
		return result;
	}

}

#endif
