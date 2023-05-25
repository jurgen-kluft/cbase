#include "ccore/c_target.h"
#include "cbase/c_bit_field.h"
#include "cbase/c_tree.h"

namespace ncore
{
    //
    // The tree_t::tree_t interface allows you to construct a tree with any kind of
    // back-end to store the nodes, colors and values.
    //
    // Example 1:
    //    If you know that you will have less than 65536 nodes, you can use a fixed size
    //    array to store the nodes, and use the array index as the 16 bit node id. Then
    //    for the values you can have a seperate array.
    //    You will also need a bit-array for storing the red/black color.
    //
    // Example 2:
    //    The most common case is to use a fixed size array to store the nodes, and use
    //    the array index as the 32 bit node id. Then for the values you can use a seperate
    //    array as well as for the red/black color.
    //
    // Example 3:
    //    You could also for each member of a node create a seperate array (SOA).
    //
    // Example 4:
    //    You can also provide a normal implementation where a node just holds a pointer to the
    //    value, parent and children.
    //
    // TODO implement a couple of different backends

    namespace ntree
    {

#define rbnil(t)         ((t)->v_get_nill())
#define rbfirst(t)       ((t)->v_get_root()->get_left(t))
#define rbroot(t)        ((t)->v_get_root())
#define rbsetfirst(t, n) ((t)->v_get_root()->set_left(t, n))

#define rbchild(n, child)       ((n)->get_child(tree, child))
#define rbdata(n)               ((n)->get_data(tree))
#define rbleft(n)               ((n)->get_left(tree))
#define rbright(n)              ((n)->get_right(tree))
#define rbparent(n)             ((n)->get_parent(tree))
#define rbcolor(n)              ((n)->get_color(tree))
#define rbsetchild(n, child, v) ((n)->set_child(tree, child, v))
#define rbsetleft(n, v)         ((n)->set_left(tree, v))
#define rbsetright(n, v)        ((n)->set_right(tree, v))
#define rbsetparent(n, v)       ((n)->set_parent(tree, v))
#define rbsetcolor(n, color)    ((n)->set_color(tree, color))

        struct node_t
        {
            inline void const* get_key(tree_t const* c) const { return c->v_get_key(this); }
            inline node_t*     get_parent(tree_t const* c) const { return c->v_get_node(this, PARENT); }
            inline void        set_parent(tree_t* c, node_t* p) { c->v_set_node(this, PARENT, p); }
            inline node_t*     get_left(tree_t const* c) const { return c->v_get_node(this, LEFT); }
            inline node_t*     get_right(tree_t const* c) const { return c->v_get_node(this, RIGHT); }
            inline void        set_left(tree_t* c, node_t* child) { c->v_set_node(this, LEFT, child); }
            inline void        set_right(tree_t* c, node_t* child) { c->v_set_node(this, RIGHT, child); }
            inline node_t*     get_child(tree_t const* c, s32 child) const { return c->v_get_node(this, (node_e)child); }
            inline void        set_child(tree_t* c, s32 child, node_t* n) { c->v_set_node(this, (node_e)child, n); }

            inline bool is_red(tree_t const* c) const { return c->v_get_color(this) == RED; }
            inline bool is_black(tree_t const* c) const { return c->v_get_color(this) == BLACK; }
            inline void set_red(tree_t* c) { c->v_set_color(this, RED); }
            inline void set_black(tree_t* c) { c->v_set_color(this, BLACK); }
            inline s32  get_color(tree_t const* c) const { return c->v_get_color(this); }
            inline void set_color(tree_t* c, s32 color) { c->v_set_color(this, (color_e)color); }

            inline s32 get_parent_side(tree_t const* c) const
            {
                node_t* parent = get_parent(c);
                if (parent->get_left(c) == this)
                    return LEFT;
                return RIGHT;
            }
        };

        class tree_internal_t
        {
        public:
            static inline s32 is_red(tree_t* tree, node_t* n) { return n != rbnil(tree) && n->is_red(tree); }

            static void rotate_left(tree_t* tree, node_t* node);
            static void rotate_right(tree_t* tree, node_t* node);

            static node_t* tree_successor(tree_t* tree, node_t* node);

            static bool tree_insert(tree_t* tree, void const* key, void const* value);
            static bool tree_find(tree_t const* tree, void const* key, node_t*& found);
            static bool tree_clear(tree_t* tree, node_t*& removed_node);
            static void tree_repair(tree_t* tree, node_t* node);
            static void tree_remove(tree_t* tree, node_t* node);

            static s32 tree_validate(tree_t* tree, node_t* root, const char*& result);
        };

        void tree_internal_t::rotate_left(tree_t* tree, node_t* node)
        {
            node_t* child;

            child = node->get_right(tree);
            node->set_right(tree, child->get_left(tree));

            if (child->get_left(tree) != rbnil(tree))
                child->get_left(tree)->set_parent(tree, node);

            child->set_parent(tree, node->get_parent(tree));

            if (node == node->get_parent(tree)->get_left(tree))
                node->get_parent(tree)->set_left(tree, child);
            else
                node->get_parent(tree)->set_right(tree, child);

            child->set_left(tree, node);
            node->set_parent(tree, child);
        }

        void tree_internal_t::rotate_right(tree_t* tree, node_t* node)
        {
            node_t* child = rbleft(node);
            rbsetleft(node, rbright(child));

            // if (child->get_right(tree) != rbnil(tree))
            //     child->get_right(tree)->set_parent(tree, node);
            // child->set_parent(tree, node->get_parent(tree));
            if (rbright(child) != rbnil(tree))
                rbsetparent(rbright(child), node);
            rbsetparent(child, rbparent(node));

            if (node == node->get_parent(tree)->get_left(tree))
                node->get_parent(tree)->set_left(tree, child);
            else
                node->get_parent(tree)->set_right(tree, child);

            child->set_right(tree, node);
            node->set_parent(tree, child);
        }

        // Returns the successor of node, or nil if there is none.
        node_t* tree_internal_t::tree_successor(tree_t* tree, node_t* node)
        {
            node_t* succ = succ = node->get_right(tree);
            if (succ != rbnil(tree))
            {
                while (succ->get_left(tree) != rbnil(tree))
                    succ = succ->get_left(tree);
            }
            else
            {
                // No right child, move up until we find it or hit the root
                for (succ = node->get_parent(tree); node == succ->get_right(tree); succ = succ->get_parent(tree))
                    node = succ;

                if (succ == rbroot(tree))
                    succ = rbnil(tree);
            }
            return (succ);
        }

        // Remove node 'z' from the tree
        void tree_internal_t::tree_remove(tree_t* tree, node_t* z)
        {
            node_t* y = nullptr;
            if (z->get_left(tree) == rbnil(tree) || z->get_right(tree) == rbnil(tree))
            {
                y = z;
            }
            else
            {
                y = tree_successor(tree, z);
            }

            node_t* x = (y->get_left(tree) == rbnil(tree)) ? y->get_right(tree) : y->get_left(tree);

            x->set_parent(tree, y->get_parent(tree));
            if (x->get_parent(tree) == rbroot(tree))
            {
                rbsetfirst(tree, x);
            }
            else
            {
                if (y == y->get_parent(tree)->get_left(tree))
                    y->get_parent(tree)->set_left(tree, x);
                else
                    y->get_parent(tree)->set_right(tree, x);
            }
            if (y->is_black(tree))
                tree_repair(tree, x);

            if (y != z)
            {
                y->set_left(tree, z->get_left(tree));
                y->set_right(tree, z->get_right(tree));
                y->set_parent(tree, z->get_parent(tree));
                y->set_color(tree, z->get_color(tree));
                z->get_left(tree)->set_parent(tree, y);
                z->get_right(tree)->set_parent(tree, y);

                if (z == z->get_parent(tree)->get_left(tree))
                {
                    z->get_parent(tree)->set_left(tree, y);
                }
                else
                {
                    z->get_parent(tree)->set_right(tree, y);
                }
            }
        }

        // Repair the tree after a node has been deleted by rotating and repainting
        // colors to restore the 4 properties inherent in red-black trees.
        void tree_internal_t::tree_repair(tree_t* tree, node_t* node)
        {
            node_t* sibling;

            while (node->is_black(tree))
            {
                if (node == node->get_parent(tree)->get_left(tree))
                {
                    sibling = node->get_parent(tree)->get_right(tree);
                    if (sibling->is_red(tree))
                    {
                        sibling->set_black(tree);
                        node->get_parent(tree)->set_red(tree);
                        rotate_left(tree, node->get_parent(tree));
                        sibling = node->get_parent(tree)->get_right(tree);
                    }
                    if (sibling->get_right(tree)->is_black(tree) && sibling->get_left(tree)->is_black(tree))
                    {
                        sibling->set_red(tree);
                        node = node->get_parent(tree);
                    }
                    else
                    {
                        if (sibling->get_right(tree)->is_black(tree))
                        {
                            sibling->get_left(tree)->set_black(tree);
                            sibling->set_red(tree);
                            rotate_right(tree, sibling);
                            sibling = node->get_parent(tree)->get_right(tree);
                        }
                        sibling->set_color(tree, node->get_parent(tree)->get_color(tree));
                        node->get_parent(tree)->set_black(tree);
                        sibling->get_right(tree)->set_black(tree);
                        rotate_left(tree, node->get_parent(tree));
                        break;
                    }
                }
                else  // if (node == node->get_parent(tree)->get_right(tree))
                {
                    sibling = node->get_parent(tree)->get_left(tree);
                    if (sibling->is_red(tree))
                    {
                        sibling->set_black(tree);
                        node->get_parent(tree)->set_red(tree);
                        rotate_right(tree, node->get_parent(tree));
                        sibling = node->get_parent(tree)->get_left(tree);
                    }
                    if (sibling->get_right(tree)->is_black(tree) && sibling->get_left(tree)->is_black(tree))
                    {
                        sibling->set_red(tree);
                        node = node->get_parent(tree);
                    }
                    else
                    {
                        if (sibling->get_left(tree)->is_black(tree))
                        {
                            sibling->get_right(tree)->set_black(tree);
                            sibling->set_red(tree);
                            rotate_left(tree, sibling);
                            sibling = node->get_parent(tree)->get_left(tree);
                        }
                        sibling->set_color(tree, node->get_parent(tree)->get_color(tree));
                        node->get_parent(tree)->set_black(tree);
                        sibling->get_left(tree)->set_black(tree);
                        rotate_right(tree, node->get_parent(tree));
                        break;
                    }
                }
            }
        }

        // Insert key pointer into a redblack tree.
        // Returns a nullptr pointer on success.  If a node matching "key"
        // already exists, a pointer to the existant node is returned.
        bool tree_internal_t::tree_insert(tree_t* tree, void const* key, void const* value)
        {
            node_t* node   = rbfirst(tree);
            node_t* parent = rbroot(tree);

            // Find correct insertion point.
            while (node != rbnil(tree))
            {
                parent = node;
                // s32 res = tree->m_compare(key, node->get_data());
                s32 res = tree->v_compare_insert(key, node);
                if (res == 0)
                    return false;
                node = res < 0 ? node->get_left(tree) : node->get_right(tree);
            }

            node = tree->v_new_node(key, value);
            node->set_right(tree, rbnil(tree));
            node->set_left(tree, rbnil(tree));
            node->set_parent(tree, parent);
            //        if (parent == rbroot(tree) || tree->m_compare(key, parent->get_data()) < 0)
            if (parent == rbroot(tree) || tree->v_compare_insert(key, parent) < 0)
                parent->set_left(tree, node);
            else
                parent->set_right(tree, node);
            node->set_red(tree);

            // If the parent node is black we are all set, if it is red we have
            // the following possible cases to deal with.  We iterate through
            // the rest of the tree to make sure none of the required properties
            // is violated.
            //
            //  1) The uncle is red.  We repaint both the parent and uncle black
            //     and repaint the grandparent node red.
            //
            //  2) The uncle is black and the new node is the right child of its
            //     parent, and the parent in turn is the left child of its parent.
            //     We do a left rotation to switch the roles of the parent and
            //     child, relying on further iterations to fixup the old parent.
            //
            //  3) The uncle is black and the new node is the left child of its
            //     parent, and the parent in turn is the left child of its parent.
            //     We switch the colors of the parent and grandparent and perform
            //     a right rotation around the grandparent.  This makes the former
            //     parent the parent of the new node and the former grandparent.
            //
            // Note that because we use a sentinel for the root node we never
            // need to worry about replacing the root.

            while (node->get_parent(tree)->is_red(tree))
            {
                node_t* uncle;
                if (node->get_parent(tree) == node->get_parent(tree)->get_parent(tree)->get_left(tree))
                {
                    uncle = node->get_parent(tree)->get_parent(tree)->get_right(tree);
                    if (uncle->is_red(tree))
                    {
                        node->get_parent(tree)->set_black(tree);
                        uncle->set_black(tree);
                        node->get_parent(tree)->get_parent(tree)->set_red(tree);
                        node = node->get_parent(tree)->get_parent(tree);
                    }
                    else /* if (uncle->color == black) */
                    {
                        if (node == node->get_parent(tree)->get_right(tree))
                        {
                            node = node->get_parent(tree);
                            rotate_left(tree, node);
                        }
                        node->get_parent(tree)->set_black(tree);
                        node->get_parent(tree)->get_parent(tree)->set_red(tree);
                        rotate_right(tree, node->get_parent(tree)->get_parent(tree));
                    }
                }
                else
                {
                    // if (node->get_parent(tree) == node->get_parent(tree)->get_parent(tree)->get_right(tree))
                    uncle = node->get_parent(tree)->get_parent(tree)->get_left(tree);
                    if (uncle->is_red(tree))
                    {
                        node->get_parent(tree)->set_black(tree);
                        uncle->set_black(tree);
                        node->get_parent(tree)->get_parent(tree)->set_red(tree);
                        node = node->get_parent(tree)->get_parent(tree);
                    }
                    else  // if (uncle->is_black(tree))
                    {
                        if (node == node->get_parent(tree)->get_left(tree))
                        {
                            node = node->get_parent(tree);
                            rotate_right(tree, node);
                        }
                        node->get_parent(tree)->set_black(tree);
                        node->get_parent(tree)->get_parent(tree)->set_red(tree);
                        rotate_left(tree, node->get_parent(tree)->get_parent(tree));
                    }
                }
            }
            rbfirst(tree)->set_black(tree);  // first node is always black
            return true;
        }

        // Look for a node matching key in tree.
        // Returns a pointer to the node if found, else nullptr.
        bool tree_internal_t::tree_find(tree_t const* tree, void const* key, node_t*& found)
        {
            node_t* node = rbfirst(tree);
            while (node != rbnil(tree))
            {
                const s32 c = tree->v_compare_insert(key, node);
                if (c == 0)
                {
                    found = node;
                    return true;
                }
                node = node->get_child(tree, (c + 1) >> 1);
            }
            found = (nullptr);
            return false;
        }

        // <summary>
        //   Releases a valid red black tree iteratively
        //   You will have to call 'clear' repeatedly until
        //   you get 'true', this is when the tree is empty.
        // <summary>
        // <param name="tree">The tree to release</param>
        bool tree_internal_t::tree_clear(tree_t* tree, node_t*& removed_node)
        {
            removed_node = nullptr;

            node_t* node = rbfirst(tree);
            if (node == rbnil(tree))
                return true;

            node_t* todelete = tree->v_get_root()->get_left(tree);
            if (node->get_left(tree) == rbnil(tree))
            {
                tree->v_get_root()->set_left(tree, node->get_right(tree));
            }
            else if (node->get_right(tree) == rbnil(tree))
            {
                tree->v_get_root()->set_left(tree, node->get_left(tree));
            }
            else
            {
                // We have left and right branches
                // Take right branch and place it
                // somewhere down the left branch
                node_t* branch = node->get_right(tree);
                node->set_right(tree, rbnil(tree));

                // Find a node in the left branch that does not
                // have both a left and right branch and place
                // our branch there.
                node_t* iter = node->get_left(tree);
                while (iter->get_left(tree) != rbnil(tree) && iter->get_right(tree) != rbnil(tree))
                {
                    iter = iter->get_left(tree);
                }
                if (iter->get_left(tree) == rbnil(tree))
                {
                    iter->set_left(tree, branch);
                }
                else if (iter->get_right(tree) == rbnil(tree))
                {
                    iter->set_right(tree, branch);
                }

                tree->v_get_root()->set_left(tree, node->get_left(tree));
            }

            removed_node = todelete;
            return false;
        }

        // Test the integrity of the red-black tree
        // @return: The depth of the tree
        // @result: If any error it returns a description of the error in 'result', when no error it will be nullptr
        s32 tree_internal_t::tree_validate(tree_t* tree, node_t* root, const char*& result)
        {
            if (root == rbnil(tree))
            {
                return 1;
            }
            else
            {
                node_t* ln = root->get_left(tree);
                node_t* rn = root->get_right(tree);

                // Consecutive red links
                if (is_red(tree, root))
                {
                    if (is_red(tree, ln) || is_red(tree, rn))
                    {
                        result = "Red violation";
                        return 0;
                    }
                }

                s32 const lh = tree_validate(tree, ln, result);
                s32 const rh = tree_validate(tree, rn, result);

                // Invalid binary search tree
                // if ((ln != rbnil(tree) && tree->m_compare(ln->get_data(), root->get_data()) >= 0) || (rn != rbnil(tree) && tree->m_compare(rn->get_data(), root->get_data()) <= 0))
                if ((ln != rbnil(tree) && tree->v_compare_nodes(ln, root) >= 0) || (rn != rbnil(tree) && tree->v_compare_nodes(rn, root) <= 0))
                {
                    result = "Binary tree violation";
                    return 0;
                }

                // Black height mismatch
                if (lh != 0 && rh != 0 && lh != rh)
                {
                    result = "Black violation";
                    return 0;
                }

                // Only count black links
                if (lh != 0 && rh != 0)
                {
                    return is_red(tree, root) ? lh : lh + 1;
                }
                return 0;
            }
        }

        void initialize(tree_t* ctxt, node_t*& nill, node_t*& root)
        {
            // We use a self-referencing sentinel node called nil to simplify the
            // code by avoiding the need to check for nullptr pointers.
            nill = ctxt->v_get_nill();
            nill->set_parent(ctxt, nill);
            nill->set_left(ctxt, nill);
            nill->set_right(ctxt, nill);
            nill->set_black(ctxt);

            // Similarly, the fake root node keeps us from having to worry
            // about splitting the root.
            root = ctxt->v_get_root();
            root->set_parent(ctxt, nill);
            root->set_left(ctxt, nill);
            root->set_right(ctxt, nill);
            root->set_black(ctxt);
        }

        bool clear(tree_t* ctxt)
        {
            node_t* node   = nullptr;
            bool    result = tree_internal_t::tree_clear(ctxt, node);
            if (node != nullptr)
            {
                ctxt->v_del_node(node);
            }
            return result;
        }

        bool find(tree_t* ctxt, void const* key, node_t*& found) 
        {
            bool result = tree_internal_t::tree_find(ctxt, key, found);
            return result;
        }

        bool insert(tree_t* ctxt, void const* key, void const* value) { return (tree_internal_t::tree_insert(ctxt, key, value)); }

        bool remove(tree_t* ctxt, void const* key)
        {
            node_t* removed_node;
            tree_internal_t::tree_find(ctxt, key, removed_node);
            if (removed_node == nullptr)
                return false;
            tree_internal_t::tree_remove(ctxt, removed_node);
            ctxt->v_del_node(removed_node);
            return true;
        }

        bool validate(tree_t* ctxt, const char*& error_str)
        {
            s32 depth = tree_internal_t::tree_validate(ctxt, rbfirst(ctxt), error_str);
            return (error_str == nullptr);
        }

        iterator_t iterate(tree_t* ctxt)
        {
            iterator_t iter;
            iter.m_tree = ctxt;
            iter.m_it   = nullptr;
            return iter;
        }

        bool iterator_t::traverse(s32 d, void const*& key)
        {
            if (m_it == nullptr)
            {
                m_it = rbfirst(m_tree);
            }
            else
            {
                if (d == LEFT)
                {
                    m_it = m_it->get_left(m_tree);
                }
                else
                {
                    m_it = m_it->get_right(m_tree);
                }
            }

            if (m_it != rbnil(m_tree))
            {
                key = m_it->get_key(m_tree);
                return true;
            }
            return false;
        }

        bool iterator_t::preorder(s32 dir, void const*& key)
        {
            key = nullptr;
            if (m_it == rbnil(m_tree))
                return false;

            if (m_it == nullptr)
            {
                m_it = rbfirst(m_tree);
            }
            else
            {
                node_t* next = m_it->get_child(m_tree, 1 - (s32)dir);
                if (next == rbnil(m_tree))
                {
                    next = m_it->get_child(m_tree, (s32)dir);
                    if (next == rbnil(m_tree))
                    {
                        do
                        {
                            while (m_it != rbnil(m_tree) && m_it->get_parent_side(m_tree) == dir)
                            {
                                m_it = m_it->get_parent(m_tree);
                            }
                            m_it = m_it->get_parent(m_tree);
                        } while (m_it != rbnil(m_tree) && m_it->get_child(m_tree, (s32)dir) == rbnil(m_tree));

                        if (m_it != rbnil(m_tree))
                        {
                            m_it = m_it->get_child(m_tree, (s32)dir);
                        }
                    }
                    else
                    {
                        m_it = next;
                    }
                }
                else
                {
                    m_it = next;
                }
            }

            if (m_it != rbnil(m_tree))
            {
                key = m_it->get_key(m_tree);
                return true;
            }

            return false;
        }

        bool iterator_t::sortorder(s32 dir, void const*& key)
        {
            key = nullptr;
            if (m_it == rbnil(m_tree))
                return false;

            if (m_it == nullptr)
            {
                m_it = rbfirst(m_tree);
                while (m_it->get_child(m_tree, 1 - (s32)dir) != rbnil(m_tree))
                {
                    m_it = m_it->get_child(m_tree, 1 - (s32)dir);
                }
            }
            else
            {
                node_t* next = m_it->get_child(m_tree, (s32)dir);
                if (next == rbnil(m_tree))
                {
                    if (m_it->get_parent_side(m_tree) == dir)
                    {
                        while (m_it != rbnil(m_tree) && m_it->get_parent_side(m_tree) == dir)
                        {
                            m_it = m_it->get_parent(m_tree);
                        }
                        if (m_it != rbnil(m_tree))
                        {
                            m_it = m_it->get_parent(m_tree);
                        }
                    }
                    else
                    {
                        m_it = m_it->get_parent(m_tree);
                    }
                }
                else
                {
                    do
                    {
                        m_it = next;
                        next = next->get_child(m_tree, 1 - (s32)dir);
                    } while (next != rbnil(m_tree));
                }
            }

            if (m_it != rbnil(m_tree) && m_it != rbroot(m_tree))
            {
                key = m_it->get_key(m_tree);
                return true;
            }

            return false;
        }

        bool iterator_t::postorder(s32 dir, void const*& key)
        {
            key = nullptr;
            if (m_it == rbnil(m_tree))
                return false;

            if (m_it == nullptr)
            {
                m_it = rbfirst(m_tree);

                // Travel on the outside (left - right - left - right ....)
                node_t* branch = m_it->get_child(m_tree, 1 - (s32)dir);
                while (branch != rbnil(m_tree))
                {
                    while (branch != rbnil(m_tree))
                    {
                        m_it   = branch;
                        branch = branch->get_child(m_tree, 1 - (s32)dir);
                    }
                    branch = m_it->get_child(m_tree, (s32)dir);
                }
            }
            else
            {
                if (m_it->get_parent_side(m_tree) == dir)
                {
                    m_it = m_it->get_parent(m_tree);
                }
                else
                {
                    m_it = m_it->get_parent(m_tree);

                    // Travel on the outside (left - right - left - right ....)
                    node_t* branch = m_it->get_child(m_tree, (s32)dir);
                    while (branch != rbnil(m_tree))
                    {
                        while (branch != rbnil(m_tree))
                        {
                            m_it   = branch;
                            branch = branch->get_child(m_tree, 1 - (s32)dir);
                        }
                        branch = m_it->get_child(m_tree, (s32)dir);
                    }
                }
            }

            if (m_it != rbnil(m_tree) && m_it != rbroot(m_tree))
            {
                key = m_it->get_key(m_tree);
                return true;
            }

            return false;
        }

#undef rbnil
#undef rbfirst
#undef rbroot
#undef rbsetfirst

    }  // namespace ntree
}  // namespace ncore
