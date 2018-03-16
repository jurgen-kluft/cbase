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

	class xtree
	{
	public:
						xtree(x_iallocator* node_allocator);

		const static s32 cLeft = 0;
		const static s32 cRight = 1;

		struct node
		{
			inline			node() : data(0) { parent = link[0] = link[1] = nullptr; }

			void *			get_data() const { uptr p = data & ~(uptr)0x1; return (void*)p; }
			void			set_data(void * d) { data = (data & (uptr)0x1) | ((uptr)d & ~(uptr)0x1); }

			node *			get_parent() const { return parent; }
			void			set_parent(node * p) { parent = p; }
			node *			get_left() const { return link[0]; }
			node *			get_right() const { return link[1]; }
			void			set_left(node * c) { link[0] = c; }
			void			set_right(node * c) { link[1] = c; }
			node *			get_child(s32 child) const { return link[child&1]; }
			void			set_child(s32 child, node*n) { link[child&1] = n; }

			bool			is_red() const { uptr p = data & (uptr)0x1; return p == 0; }
			bool			is_black() const { uptr p = data & (uptr)0x1; return p == 1; }
			void			set_red() { uptr p = data & (uptr)~0x1; data = p; }
			void			set_black() { uptr p = data | (uptr)0x1; data = p; }
			s32				get_color() const { s32 p = (s32)data & (s32)0x1; return p; }
			void			set_color(s32 c) { uptr p = data & (uptr)~0x1; data = p | ((uptr)c & 0x1); }

			s32				get_parent_side() const
			{
				node* parent = get_parent();
				if (parent->get_left() == this)
					return cLeft;
				return cRight;
			}

		private:
			uptr			data;		// User-defined content 
			node *			parent;		// Parent
			node *			link[2];	// Left (0) and right (1) links 
		};

		struct iterator
		{
			bool		traverse(s32 d, void *& data);
			bool		preorder(s32 d, void *& data);
			bool		sortorder(s32 d, void *& data);
			bool		postorder(s32 d, void *& data);

			s32			getdir(s32 compare) const { return (compare + 1) >> 1; }

		protected:
			friend class xtree;

			xtree *		m_tree;
			node *		m_it;
		};

		u32				size() const			{ return m_size;}
		bool			clear(void *& data);	// Repeatedly call 'clear' until true is returned

		bool			find(void * data, void *& found);
		bool			insert(void * data);
		bool			remove(void * data);

		bool			validate(const char*& error_str);

		typedef s32(*compare_f) (void const* p1, void const* p2);
		void			set_cmp(compare_f func) { m_compare = func; }

		iterator		iterate();

	protected:
		friend class xtree_internal;
		friend struct iterator;
		friend struct search;

		x_iallocator*	m_node_allocator;
		compare_f		m_compare;
		node			m_nill;
		node 			m_root;				// Top of the tree
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
