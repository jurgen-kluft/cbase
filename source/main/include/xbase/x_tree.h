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

	struct xnode_t
	{
		inline			xnode_t() : data(nullptr) { link[0] = link[1] = 0; }

		void *			data;		// User-defined content 

		void			clear() 
		{
			data = nullptr; 
			link[0] = link[1] = 0;
		}

		xnode_t *		get_left() const { return (xnode_t*)link[0]; }
		xnode_t *		get_right() const { uptr p = link[1] & ~(uptr)0x1; return (xnode_t*)p; }
		void			set_left(xnode_t*n) { link[0] = (uptr)n; }
		void			set_right(xnode_t*n) { uptr p = link[1] & (uptr)0x1; link[1] = p | ((uptr)n); }

		xnode_t *		get_child(s32 child) const { uptr p = link[child] & (uptr)~0x1; return (xnode_t*)p; }
		void			set_child(s32 child, xnode_t*n) { uptr p = link[child] & (uptr)0x1; link[child] = p | ((uptr)n); }

		bool			is_red() const { uptr p = link[1] & (uptr)0x1; return p == 0; }
		bool			is_black() const { uptr p = link[1] & (uptr)0x1; return p == 1; }
		void			set_red() { uptr p = link[1] & (uptr)~0x1; link[1] = p; }
		void			set_black() { uptr p = link[1] | (uptr)0x1; link[1] = p; }
	private:
		uptr			link[2];	// Left (0) and right (1) links 
	};

	class xtree;
	
	struct xtree_iterator
	{
		enum EConfig { HEIGHT_LIMIT = 64 };		// Tallest allowable tree

		void		init_preorder();
		void		init_sortorder(s32 dir);
		void		init_postorder();

		bool		iterate(void *& data);
		bool		backwards(void *& data);

	protected:
		friend class xtree;
		friend class xtree_internal;

		void		clear();

		bool		preorder(void *& data);
		bool		sortorder(void *& data);
		bool		postorder(void *& data);

		bool		sortorder_backwards(void *& data);

		xnode_t*	pop(xbyte& state);
		void		push(xnode_t*, xbyte state);

		void		enqueue(xnode_t*);
		xnode_t*	dequeue();

		xtree *		m_tree;						// Paired tree
		s32			m_traversal;
		xnode_t *	m_path[HEIGHT_LIMIT];		// Traversal path, node pointer
		xbyte		m_state[HEIGHT_LIMIT];		// Traversal path, node state
		u32			m_top;						// 'Top of stack' or 'Head of Queue'
		u32			m_bottom;					// 'Tail of Queue'
	};

	class xtree
	{
	public:
						xtree(x_iallocator* node_allocator);

		u32				size() const			{ return m_size;}
		bool			clear(void *& data);	// Repeatedly call 'clear' until true is returned

		bool			find(void * data, void *& found);
		s32				insert(void * data);
		s32				remove(void * data);

		bool			validate(const char*& error_str);

		typedef s32(*compare_f) (void const* p1, void const* p2);
		void			set_cmp(compare_f func) { m_compare = func; }

		void			iterate(xtree_iterator& iter);

	protected:
		friend class xtree_internal;
		friend struct xtree_iterator;

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
		inline			~xtree_t()					{ void* data; while (!m_tree.clear(data)) {} }

		u32				size() const				{ return m_tree.size(); }
		bool			clear(T *& data)			{ void* p; bool result = m_tree.clear(p); data = (T*)p; return result; }

		bool			find(T * data, T *& found)	{ void * p = (void *)data; void * f;  bool result = m_tree.find(p, f); found = (T*)f; return result; }
		bool			insert(T * data)			{ return m_tree.insert(data) == 1; }
		bool			remove(T * data)			{ void * p = (void *)data; return m_tree.remove(p) == 1; }

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
