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
						xtree(x_iallocator* node_allocator);

		u32				size() const			{ return m_size;}
		bool			clear(void *& data);	// Repeatedly call 'clear' until true is returned

		bool			find(void * data);
		s32				insert(void * data);
		s32				remove(void * data);

		bool			validate(const char*& error_str);

		typedef s32(*compare_f) (void const* p1, void const* p2);
		void			set_cmp(compare_f func) { m_compare = func; }

	protected:
		friend class xtree_internal;

		x_iallocator*	m_node_allocator;
		compare_f		m_compare;
		xnode_t *		m_root;				// Top of the tree
		u32				m_size;				// Number of items (user-defined)
	};

	template<typename T>
	class xtree_t
	{
	public:
		inline			xtree_t(x_iallocator* node_allocator) : m_tree(node_allocator) { m_tree.set_cmp(&compare); }
		inline			~xtree_t()				{ void* data; while (!m_tree.clear(data)) {} }

		u32				size() const			{ return m_tree.size(); }
		bool			clear(T *& data)		{ void* p; bool result = m_tree.clear(p); data = (T*)p; return result; }

		bool			find(T * data)			{ void * p = (void *)data; return m_tree.find(p); }
		bool			insert(T * data)		{ return m_tree.insert(data) == 1; }
		bool			remove(T * data)		{ void * p = (void *)data; return m_tree.remove(p) == 1; }

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

		xtree			m_tree;
	};

}

#endif
