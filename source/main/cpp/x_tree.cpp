#include "xbase\x_debug.h"
#include "xbase\x_tree.h"

namespace xcore
{
	void			rb_insert_fixup(xrbnode& head, xrbnode* node) 
	{
		xrbnode* cur = node;
		xrbnode* p = cur->get_parent();
		while (p->is_red()) 
		{
			xrbnode* pp = p->get_parent();
			ASSERT(pp != &head);
			s32 s = p->get_parent_side();
			s32 o = (1 - s);
			xrbnode* pp_right = pp->get_child(o);
			if (pp_right->is_red())
			{
				p->set_color(xrbnode::BLACK);
				pp_right->set_color(xrbnode::BLACK);
				pp->set_color(xrbnode::RED);
				cur = pp;
				p = cur->get_parent();
			} 
			else 
			{
				if (cur == p->get_child(o))
				{
					cur = p;
					rb_rotate(cur, s);
					p = cur->get_parent();
				}
				p->set_color(xrbnode::BLACK);
				pp->set_color(xrbnode::RED);
				rb_rotate(pp, o);
			} 
		}
		head.get_child(xrbnode::LEFT)->set_color(xrbnode::BLACK);
	}

	void		rb_erase_fixup(xrbnode* root, xrbnode* node)
	{
		xrbnode* head = root;

		xrbnode* cur = node;
		while (!cur->is_red() && cur != head->get_child(xrbnode::LEFT)) 
		{
			xrbnode* p = cur->get_parent();
			ASSERT(p != head);
			s32 const s = cur->get_parent_side();
			s32 const o = (1 - s);
			xrbnode* w = p->get_child(o);
			ASSERT(w != head);
			if (w->is_red()) 
			{
				ASSERT(w->get_child(xrbnode::LEFT)->is_black() && w->get_child(xrbnode::RIGHT)->is_black());
				w->set_black();
				p->set_red();
				w = w->get_child(s);
				rb_rotate(p, s);
			}
			ASSERT(w != head);
			if (w->get_child(xrbnode::LEFT)->is_black() && w->get_child(xrbnode::RIGHT)->is_black())
			{ 
				w->set_red();
				cur = p;
			} 
			else 
			{
				if (w->get_child(o)->is_black())
				{
					xrbnode* c = w->get_child(s);
					c->set_black();
					w->set_red();
					rb_rotate(w, o);
					w = c;
					ASSERT(w != head);
				}
				ASSERT(w->get_child(o)->is_red());
				w->set_color(p->get_color());
				p->set_black();
				w->get_child(o)->set_black();
				rb_rotate(p, s);
				cur = head->get_child(xrbnode::LEFT);
			}
		}
		cur->set_black();
	}

	void			rb_switch_with(xrbnode* _this, xrbnode* _node)
	{
		ASSERT(_this != _node);

		// Copy pointers (includes color and parent side)
		xrbnode* parent = _node->get_parent();
		_this->set_parent(parent);
		xrbnode* left = _node->get_left();
		_this->set_left(left);
		xrbnode* right = _node->get_right();
		_this->set_right(right);

		_this->set_color(_node->get_color());
		s32 const ps = _node->get_parent_side();
		_this->set_parent_side(ps);

		left->set_parent(_this);
		left->set_parent_side(xrbnode::LEFT);
		right->set_parent(_this);
		right->set_parent_side(xrbnode::RIGHT);

		// Set ourselves as the new child of our parent
		parent->set_child(_this, ps);
	}

	bool			xtree_insert(xrbnode* root, xrbnode * node_to_insert, void* key, xtree_compare_delegate comparer)
	{
		ASSERT(node_to_insert!=0);

		xrbnode* endNode  = root;
		xrbnode* lastNode = root;
		xrbnode* curNode  = (xrbnode*)root->get_child(xrbnode::LEFT);
		s32 s = xrbnode::LEFT;
		while (curNode != endNode)
		{
			lastNode = curNode;

			s32 c = comparer(curNode, key);
			if (c == 0)
				return xFALSE;	// Duplicate
			s = (c + 1) >> 1;
			curNode = (xrbnode*)curNode->get_child(s);
		}

		xrbnode* node = node_to_insert;

		rb_attach_to(node, lastNode, s);
		rb_insert_fixup(*root, node);

#ifdef DEBUG_RBTREE
		rb_check(root);
#endif
		return xTRUE;
	}

	bool		xtree_remove(xrbnode* root, xrbnode*& found_node, void* key, xtree_compare_delegate comparer)
	{
		xrbnode* nill = root;
		xrbnode* it   = (xrbnode*)root->get_child(xrbnode::LEFT);
		while ( it != nill )
		{
			s32 cmp = comparer(it, key);
			if ( cmp == 0 )
				break;
			s32 const s = (cmp + 1) >> 1;
			it = (xrbnode*)it->get_child(s);
		}

		// Did we find it? If not we have don't have to continue.
		if (it == nill)
		{
			found_node = NULL;
			return false;
		}

		// The node to remove
		xrbnode* node = it;
		ASSERT(node != root);

		xrbnode* endNode = root;
		xrbnode* repl = node;
		s32 s = xrbnode::LEFT;
		if (node->get_child(xrbnode::RIGHT) != endNode)
		{
			if (node->get_child(xrbnode::LEFT) != endNode)
			{
				repl = (xrbnode*)node->get_child(xrbnode::RIGHT);
				while (repl->get_child(xrbnode::LEFT) != endNode)
					repl = (xrbnode*)repl->get_child(xrbnode::LEFT);
			}
			s = xrbnode::RIGHT;
		}
		ASSERT(repl->get_child(1-s) == endNode);
		bool red = repl->is_red();
		xrbnode* replChild = (xrbnode*)repl->get_child(s);

		rb_substitute_with(repl, replChild);
		ASSERT(endNode->is_black());

		if (repl != node)
			rb_switch_with(repl, node);

		ASSERT(endNode->is_black());

		if (!red) 
			rb_erase_fixup(root, replChild);

		node->clear();

#ifdef DEBUG_RBTREE
		rb_check(root);
#endif
		found_node = node;
		return true;
	}

	bool		xtree_find  (xrbnode* root, xrbnode*& found_node, void* key, xtree_compare_delegate comparer)
	{
		xrbnode* nill = root;
		xrbnode* it = (xrbnode*)root->get_child(xrbnode::LEFT);
		while ( it != nill )
		{
			s32 const cmp = comparer(it, key);
			if ( cmp == 0 )
				break;
			s32 const s = (cmp + 1) >> 1;
			it = (xrbnode*)it->get_child(s);
		}
		found_node = ( it == nill ) ? NULL : it;
		return found_node != NULL;
	}

	xrbnode*	xtree_clear (xrbnode* root, xrbnode*& save)
	{
		//	Rotate away the left links so that
		//	we can treat this like the destruction
		//	of a linked list
		if (save == NULL)
			save = (xrbnode*)root->get_child(xrbnode::LEFT);

		xrbnode* it = save;
		save = NULL;

		while ( it != root ) 
		{
			if ( it->get_child(xrbnode::LEFT) == root ) 
			{
				/* No left links, just kill the node and move on */
				save = (xrbnode*)it->get_child(xrbnode::RIGHT);
				if (it != root)
				{
					it->clear();
					return it;
				}
			}
			else
			{
				/* Rotate away the left link and check again */
				save = (xrbnode*)it->get_child(xrbnode::LEFT);
				it->set_child(save->get_child(xrbnode::RIGHT), xrbnode::LEFT);
				save->set_child(it, xrbnode::RIGHT);
			}

			it = save;
			save = NULL;
		}
		root->clear();
		return NULL;
	}
}
