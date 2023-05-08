#include "ccore/c_target.h"
#include "cbase/c_bit_field.h"
#include "cbase/c_tree.h"

namespace ncore
{
    //
    // The tree_t::ctxt_t interface allows you to construct a tree with any kind of
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

#define rbnil(t) ((t)->m_nill)
#define rbfirst(t) ((t)->m_root->get_left(t->m_ctxt))
#define rbroot(t) ((t)->m_root)
#define rbsetfirst(t, n) ((t)->m_root->set_left(t->m_ctxt, n))

#define rbchild(n, child) ((n)->get_child(tree->m_ctxt, child))
#define rbdata(n) ((n)->get_data(tree->m_ctxt))
#define rbleft(n) ((n)->get_left(tree->m_ctxt))
#define rbright(n) ((n)->get_right(tree->m_ctxt))
#define rbparent(n) ((n)->get_parent(tree->m_ctxt))
#define rbcolor(n) ((n)->get_color(tree->m_ctxt))
#define rbsetchild(n, child, v) ((n)->set_child(tree->m_ctxt, child, v))
#define rbsetleft(n, v) ((n)->set_left(tree->m_ctxt, v))
#define rbsetright(n, v) ((n)->set_right(tree->m_ctxt, v))
#define rbsetparent(n, v) ((n)->set_parent(tree->m_ctxt, v))
#define rbsetcolor(n, color) ((n)->set_color(tree->m_ctxt, color))

    class tree_internal_t
    {
    public:
        static inline s32 is_red(tree_t* tree, tree_t::node_t* n) { return n != rbnil(tree) && n->is_red(tree->m_ctxt); }

        static void rotate_left(tree_t* tree, tree_t::node_t* node);
        static void rotate_right(tree_t* tree, tree_t::node_t* node);

        static tree_t::node_t* tree_successor(tree_t* tree, tree_t::node_t* node);

        static bool tree_insert(tree_t* tree, void const* key, void const* value);
        static bool tree_find(tree_t const* tree, void const* key, tree_t::node_t*& found);
        static bool tree_clear(tree_t* tree, tree_t::node_t*& removed_node);
        static void tree_repair(tree_t* tree, tree_t::node_t* node);
        static void tree_remove(tree_t* tree, tree_t::node_t* node);

        static s32 tree_validate(tree_t* tree, tree_t::node_t* root, const char*& result);
    };

    void tree_internal_t::rotate_left(tree_t* tree, tree_t::node_t* node)
    {
        tree_t::node_t* child;

        child = node->get_right(tree->m_ctxt);
        node->set_right(tree->m_ctxt, child->get_left(tree->m_ctxt));

        if (child->get_left(tree->m_ctxt) != rbnil(tree))
            child->get_left(tree->m_ctxt)->set_parent(tree->m_ctxt, node);

        child->set_parent(tree->m_ctxt, node->get_parent(tree->m_ctxt));

        if (node == node->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt))
            node->get_parent(tree->m_ctxt)->set_left(tree->m_ctxt, child);
        else
            node->get_parent(tree->m_ctxt)->set_right(tree->m_ctxt, child);

        child->set_left(tree->m_ctxt, node);
        node->set_parent(tree->m_ctxt, child);
    }

    void tree_internal_t::rotate_right(tree_t* tree, tree_t::node_t* node)
    {
        tree_t::node_t* child = rbleft(node);
        rbsetleft(node, rbright(child));

        // if (child->get_right(tree->m_ctxt) != rbnil(tree))
        //     child->get_right(tree->m_ctxt)->set_parent(tree->m_ctxt, node);
        // child->set_parent(tree->m_ctxt, node->get_parent(tree->m_ctxt));
        if (rbright(child) != rbnil(tree))
            rbsetparent(rbright(child), node);
        rbsetparent(child, rbparent(node));

        if (node == node->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt))
            node->get_parent(tree->m_ctxt)->set_left(tree->m_ctxt, child);
        else
            node->get_parent(tree->m_ctxt)->set_right(tree->m_ctxt, child);

        child->set_right(tree->m_ctxt, node);
        node->set_parent(tree->m_ctxt, child);
    }

    // Returns the successor of node, or nil if there is none.
    tree_t::node_t* tree_internal_t::tree_successor(tree_t* tree, tree_t::node_t* node)
    {
        tree_t::node_t* succ = succ = node->get_right(tree->m_ctxt);
        if (succ != rbnil(tree))
        {
            while (succ->get_left(tree->m_ctxt) != rbnil(tree))
                succ = succ->get_left(tree->m_ctxt);
        }
        else
        {
            // No right child, move up until we find it or hit the root
            for (succ = node->get_parent(tree->m_ctxt); node == succ->get_right(tree->m_ctxt); succ = succ->get_parent(tree->m_ctxt))
                node = succ;

            if (succ == rbroot(tree))
                succ = rbnil(tree);
        }
        return (succ);
    }

    // Remove node 'z' from the tree
    void tree_internal_t::tree_remove(tree_t* tree, tree_t::node_t* z)
    {
        tree_t::node_t* y = nullptr;
        if (z->get_left(tree->m_ctxt) == rbnil(tree) || z->get_right(tree->m_ctxt) == rbnil(tree))
        {
            y = z;
        }
        else
        {
            y = tree_successor(tree, z);
        }

        tree_t::node_t* x = (y->get_left(tree->m_ctxt) == rbnil(tree)) ? y->get_right(tree->m_ctxt) : y->get_left(tree->m_ctxt);

        x->set_parent(tree->m_ctxt, y->get_parent(tree->m_ctxt));
        if (x->get_parent(tree->m_ctxt) == rbroot(tree))
        {
            rbsetfirst(tree, x);
        }
        else
        {
            if (y == y->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt))
                y->get_parent(tree->m_ctxt)->set_left(tree->m_ctxt, x);
            else
                y->get_parent(tree->m_ctxt)->set_right(tree->m_ctxt, x);
        }
        if (y->is_black(tree->m_ctxt))
            tree_repair(tree, x);

        if (y != z)
        {
            y->set_left(tree->m_ctxt, z->get_left(tree->m_ctxt));
            y->set_right(tree->m_ctxt, z->get_right(tree->m_ctxt));
            y->set_parent(tree->m_ctxt, z->get_parent(tree->m_ctxt));
            y->set_color(tree->m_ctxt, z->get_color(tree->m_ctxt));
            z->get_left(tree->m_ctxt)->set_parent(tree->m_ctxt, y);
            z->get_right(tree->m_ctxt)->set_parent(tree->m_ctxt, y);

            if (z == z->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt))
            {
                z->get_parent(tree->m_ctxt)->set_left(tree->m_ctxt, y);
            }
            else
            {
                z->get_parent(tree->m_ctxt)->set_right(tree->m_ctxt, y);
            }
        }
    }

    // Repair the tree after a node has been deleted by rotating and repainting
    // colors to restore the 4 properties inherent in red-black trees.
    void tree_internal_t::tree_repair(tree_t* tree, tree_t::node_t* node)
    {
        tree_t::node_t* sibling;

        while (node->is_black(tree->m_ctxt))
        {
            if (node == node->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt))
            {
                sibling = node->get_parent(tree->m_ctxt)->get_right(tree->m_ctxt);
                if (sibling->is_red(tree->m_ctxt))
                {
                    sibling->set_black(tree->m_ctxt);
                    node->get_parent(tree->m_ctxt)->set_red(tree->m_ctxt);
                    rotate_left(tree, node->get_parent(tree->m_ctxt));
                    sibling = node->get_parent(tree->m_ctxt)->get_right(tree->m_ctxt);
                }
                if (sibling->get_right(tree->m_ctxt)->is_black(tree->m_ctxt) && sibling->get_left(tree->m_ctxt)->is_black(tree->m_ctxt))
                {
                    sibling->set_red(tree->m_ctxt);
                    node = node->get_parent(tree->m_ctxt);
                }
                else
                {
                    if (sibling->get_right(tree->m_ctxt)->is_black(tree->m_ctxt))
                    {
                        sibling->get_left(tree->m_ctxt)->set_black(tree->m_ctxt);
                        sibling->set_red(tree->m_ctxt);
                        rotate_right(tree, sibling);
                        sibling = node->get_parent(tree->m_ctxt)->get_right(tree->m_ctxt);
                    }
                    sibling->set_color(tree->m_ctxt, node->get_parent(tree->m_ctxt)->get_color(tree->m_ctxt));
                    node->get_parent(tree->m_ctxt)->set_black(tree->m_ctxt);
                    sibling->get_right(tree->m_ctxt)->set_black(tree->m_ctxt);
                    rotate_left(tree, node->get_parent(tree->m_ctxt));
                    break;
                }
            }
            else // if (node == node->get_parent(tree->m_ctxt)->get_right(tree->m_ctxt))
            {
                sibling = node->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt);
                if (sibling->is_red(tree->m_ctxt))
                {
                    sibling->set_black(tree->m_ctxt);
                    node->get_parent(tree->m_ctxt)->set_red(tree->m_ctxt);
                    rotate_right(tree, node->get_parent(tree->m_ctxt));
                    sibling = node->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt);
                }
                if (sibling->get_right(tree->m_ctxt)->is_black(tree->m_ctxt) && sibling->get_left(tree->m_ctxt)->is_black(tree->m_ctxt))
                {
                    sibling->set_red(tree->m_ctxt);
                    node = node->get_parent(tree->m_ctxt);
                }
                else
                {
                    if (sibling->get_left(tree->m_ctxt)->is_black(tree->m_ctxt))
                    {
                        sibling->get_right(tree->m_ctxt)->set_black(tree->m_ctxt);
                        sibling->set_red(tree->m_ctxt);
                        rotate_left(tree, sibling);
                        sibling = node->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt);
                    }
                    sibling->set_color(tree->m_ctxt, node->get_parent(tree->m_ctxt)->get_color(tree->m_ctxt));
                    node->get_parent(tree->m_ctxt)->set_black(tree->m_ctxt);
                    sibling->get_left(tree->m_ctxt)->set_black(tree->m_ctxt);
                    rotate_right(tree, node->get_parent(tree->m_ctxt));
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
        tree_t::node_t* node   = rbfirst(tree);
        tree_t::node_t* parent = rbroot(tree);

        // Find correct insertion point.
        while (node != rbnil(tree))
        {
            parent = node;
            // s32 res = tree->m_compare(key, node->get_data());
            s32 res = tree->m_ctxt->v_compare_insert(key, node);
            if (res == 0)
                return false;
            node = res < 0 ? node->get_left(tree->m_ctxt) : node->get_right(tree->m_ctxt);
        }

        node = tree->m_ctxt->v_new_node(key, value);
        node->set_right(tree->m_ctxt, rbnil(tree));
        node->set_left(tree->m_ctxt, rbnil(tree));
        node->set_parent(tree->m_ctxt, parent);
        //        if (parent == rbroot(tree) || tree->m_compare(key, parent->get_data()) < 0)
        if (parent == rbroot(tree) || tree->m_ctxt->v_compare_insert(key, parent) < 0)
            parent->set_left(tree->m_ctxt, node);
        else
            parent->set_right(tree->m_ctxt, node);
        node->set_red(tree->m_ctxt);

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

        while (node->get_parent(tree->m_ctxt)->is_red(tree->m_ctxt))
        {
            tree_t::node_t* uncle;
            if (node->get_parent(tree->m_ctxt) == node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt))
            {
                uncle = node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt)->get_right(tree->m_ctxt);
                if (uncle->is_red(tree->m_ctxt))
                {
                    node->get_parent(tree->m_ctxt)->set_black(tree->m_ctxt);
                    uncle->set_black(tree->m_ctxt);
                    node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt)->set_red(tree->m_ctxt);
                    node = node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt);
                }
                else /* if (uncle->color == black) */
                {
                    if (node == node->get_parent(tree->m_ctxt)->get_right(tree->m_ctxt))
                    {
                        node = node->get_parent(tree->m_ctxt);
                        rotate_left(tree, node);
                    }
                    node->get_parent(tree->m_ctxt)->set_black(tree->m_ctxt);
                    node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt)->set_red(tree->m_ctxt);
                    rotate_right(tree, node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt));
                }
            }
            else
            {
                // if (node->get_parent(tree->m_ctxt) == node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt)->get_right(tree->m_ctxt))
                uncle = node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt);
                if (uncle->is_red(tree->m_ctxt))
                {
                    node->get_parent(tree->m_ctxt)->set_black(tree->m_ctxt);
                    uncle->set_black(tree->m_ctxt);
                    node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt)->set_red(tree->m_ctxt);
                    node = node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt);
                }
                else // if (uncle->is_black(tree->m_ctxt))
                {
                    if (node == node->get_parent(tree->m_ctxt)->get_left(tree->m_ctxt))
                    {
                        node = node->get_parent(tree->m_ctxt);
                        rotate_right(tree, node);
                    }
                    node->get_parent(tree->m_ctxt)->set_black(tree->m_ctxt);
                    node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt)->set_red(tree->m_ctxt);
                    rotate_left(tree, node->get_parent(tree->m_ctxt)->get_parent(tree->m_ctxt));
                }
            }
        }
        rbfirst(tree)->set_black(tree->m_ctxt); // first node is always black
        return true;
    }

    // Look for a node matching key in tree.
    // Returns a pointer to the node if found, else nullptr.
    bool tree_internal_t::tree_find(tree_t const* tree, void const* key, tree_t::node_t*& found)
    {
        tree_t::node_t* node = rbfirst(tree);
        while (node != rbnil(tree))
        {
            //            s32 c = tree->m_compare(key, node->get_data());
            s32 c = tree->m_ctxt->v_compare_insert(key, node);
            if (c == 0)
            {
                found = node;
                return true;
            }
            c    = (c + 1) >> 1;
            node = node->get_child(tree->m_ctxt, c);
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
    bool tree_internal_t::tree_clear(tree_t* tree, tree_t::node_t*& removed_node)
    {
        removed_node = nullptr;

        tree_t::node_t* node = rbfirst(tree);
        if (node == rbnil(tree))
            return true;

        tree_t::node_t* todelete = tree->m_root->get_left(tree->m_ctxt);
        if (node->get_left(tree->m_ctxt) == rbnil(tree))
        {
            tree->m_root->set_left(tree->m_ctxt, node->get_right(tree->m_ctxt));
        }
        else if (node->get_right(tree->m_ctxt) == rbnil(tree))
        {
            tree->m_root->set_left(tree->m_ctxt, node->get_left(tree->m_ctxt));
        }
        else
        {
            // We have left and right branches
            // Take right branch and place it
            // somewhere down the left branch
            tree_t::node_t* branch = node->get_right(tree->m_ctxt);
            node->set_right(tree->m_ctxt, rbnil(tree));

            // Find a node in the left branch that does not
            // have both a left and right branch and place
            // our branch there.
            tree_t::node_t* iter = node->get_left(tree->m_ctxt);
            while (iter->get_left(tree->m_ctxt) != rbnil(tree) && iter->get_right(tree->m_ctxt) != rbnil(tree))
            {
                iter = iter->get_left(tree->m_ctxt);
            }
            if (iter->get_left(tree->m_ctxt) == rbnil(tree))
            {
                iter->set_left(tree->m_ctxt, branch);
            }
            else if (iter->get_right(tree->m_ctxt) == rbnil(tree))
            {
                iter->set_right(tree->m_ctxt, branch);
            }

            tree->m_root->set_left(tree->m_ctxt, node->get_left(tree->m_ctxt));
        }

        removed_node = todelete;
        return false;
    }

    // Test the integrity of the red-black tree
    // @return: The depth of the tree
    // @result: If any error it returns a description of the error in 'result', when no error it will be nullptr
    s32 tree_internal_t::tree_validate(tree_t* tree, tree_t::node_t* root, const char*& result)
    {
        if (root == rbnil(tree))
        {
            return 1;
        }
        else
        {
            tree_t::node_t* ln = root->get_left(tree->m_ctxt);
            tree_t::node_t* rn = root->get_right(tree->m_ctxt);

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
            if ((ln != rbnil(tree) && tree->m_ctxt->v_compare_nodes(ln, root) >= 0) || (rn != rbnil(tree) && tree->m_ctxt->v_compare_nodes(rn, root) <= 0))
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

    tree_t::tree_t() 
        : m_ctxt(nullptr)
        , m_nill(nullptr)
        , m_root(nullptr)
        , m_size(0)
    {}

    void tree_t::init(tree_t::ctxt_t* ctxt)
    {
        m_ctxt = ctxt;
        m_nill = nullptr;
        m_root = nullptr;
        m_size = 0;
        
        // We use a self-referencing sentinel node called nil to simplify the
        // code by avoiding the need to check for nullptr pointers.
        m_nill = m_ctxt->v_new_node(nullptr, nullptr);
        m_nill->set_parent(m_ctxt, m_nill);
        m_nill->set_left(m_ctxt, m_nill);
        m_nill->set_right(m_ctxt, m_nill);
        m_nill->set_black(m_ctxt);

        // Similarly, the fake root node keeps us from having to worry
        // about splitting the root.
        m_root = m_ctxt->v_new_node(nullptr, nullptr);
        m_root->set_parent(m_ctxt, m_nill);
        m_root->set_left(m_ctxt, m_nill);
        m_root->set_right(m_ctxt, m_nill);
        m_root->set_black(m_ctxt);
    }

    void tree_t::exit()
    {
        m_ctxt->v_del_node(m_nill);
        m_ctxt->v_del_node(m_root);
        m_nill = nullptr;
        m_root = nullptr;
        m_ctxt = nullptr;
    }

    bool tree_t::clear()
    {
        node_t* node = nullptr;
        bool result = tree_internal_t::tree_clear(this, node);
        if (node != nullptr)
        {
            m_ctxt->v_del_node(node);
            m_size--;
        }
        return result;
    }

    bool tree_t::find(void const* key, node_t*& found) const
    {
        bool result = tree_internal_t::tree_find(this, key, found);
        return result;
    }

    bool tree_t::insert(void const* key, void const* value) 
    { 
        if (tree_internal_t::tree_insert(this, key, value))
        {
            m_size++;
            return true;
        }
        return false;
    }

    bool tree_t::remove(void const* key)
    {
        node_t* removed_node;
        tree_internal_t::tree_find(this, key, removed_node);
        if (removed_node == nullptr)
            return false;
        tree_internal_t::tree_remove(this, removed_node);
        m_ctxt->v_del_node(removed_node);
        m_size--;
        return true;
    }

    bool tree_t::validate(const char*& error_str)
    {
        s32 depth = tree_internal_t::tree_validate(this, rbfirst(this), error_str);
        return (error_str == nullptr);
    }

    tree_t::iterator_t tree_t::iterate()
    {
        tree_t::iterator_t iter;
        iter.m_tree = this;
        iter.m_it   = nullptr;
        return iter;
    }

    bool tree_t::iterator_t::traverse(s32 d, void const*& key)
    {
        if (m_it == nullptr)
        {
            m_it = rbfirst(m_tree);
        }
        else
        {
            if (d == LEFT)
            {
                m_it = m_it->get_left(m_tree->m_ctxt);
            }
            else
            {
                m_it = m_it->get_right(m_tree->m_ctxt);
            }
        }

        if (m_it != rbnil(m_tree))
        {
            key = m_it->get_key(m_tree->m_ctxt);
            return true;
        }
        return false;
    }

    bool tree_t::iterator_t::preorder(s32 dir, void const*& key)
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
            tree_t::node_t* next = m_it->get_child(m_tree->m_ctxt, 1 - (s32)dir);
            if (next == rbnil(m_tree))
            {
                next = m_it->get_child(m_tree->m_ctxt, (s32)dir);
                if (next == rbnil(m_tree))
                {
                    do
                    {
                        while (m_it != rbnil(m_tree) && m_it->get_parent_side(m_tree->m_ctxt) == dir)
                        {
                            m_it = m_it->get_parent(m_tree->m_ctxt);
                        }
                        m_it = m_it->get_parent(m_tree->m_ctxt);
                    } while (m_it != rbnil(m_tree) && m_it->get_child(m_tree->m_ctxt, (s32)dir) == rbnil(m_tree));

                    if (m_it != rbnil(m_tree))
                    {
                        m_it = m_it->get_child(m_tree->m_ctxt, (s32)dir);
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
            key = m_it->get_key(m_tree->m_ctxt);
            return true;
        }

        return false;
    }

    bool tree_t::iterator_t::sortorder(s32 dir, void const*& key)
    {
        key = nullptr;
        if (m_it == rbnil(m_tree))
            return false;

        if (m_it == nullptr)
        {
            m_it = rbfirst(m_tree);
            while (m_it->get_child(m_tree->m_ctxt, 1 - (s32)dir) != rbnil(m_tree))
            {
                m_it = m_it->get_child(m_tree->m_ctxt, 1 - (s32)dir);
            }
        }
        else
        {
            tree_t::node_t* next = m_it->get_child(m_tree->m_ctxt, (s32)dir);
            if (next == rbnil(m_tree))
            {
                if (m_it->get_parent_side(m_tree->m_ctxt) == dir)
                {
                    while (m_it != rbnil(m_tree) && m_it->get_parent_side(m_tree->m_ctxt) == dir)
                    {
                        m_it = m_it->get_parent(m_tree->m_ctxt);
                    }
                    if (m_it != rbnil(m_tree))
                    {
                        m_it = m_it->get_parent(m_tree->m_ctxt);
                    }
                }
                else
                {
                    m_it = m_it->get_parent(m_tree->m_ctxt);
                }
            }
            else
            {
                do
                {
                    m_it = next;
                    next = next->get_child(m_tree->m_ctxt, 1 - (s32)dir);
                } while (next != rbnil(m_tree));
            }
        }

        if (m_it != rbnil(m_tree) && m_it != rbroot(m_tree))
        {
            key = m_it->get_key(m_tree->m_ctxt);
            return true;
        }

        return false;
    }

    bool tree_t::iterator_t::postorder(s32 dir, void const*& key)
    {
        key = nullptr;
        if (m_it == rbnil(m_tree))
            return false;

        if (m_it == nullptr)
        {
            m_it = rbfirst(m_tree);

            // Travel on the outside (left - right - left - right ....)
            tree_t::node_t* branch = m_it->get_child(m_tree->m_ctxt, 1 - (s32)dir);
            while (branch != rbnil(m_tree))
            {
                while (branch != rbnil(m_tree))
                {
                    m_it   = branch;
                    branch = branch->get_child(m_tree->m_ctxt, 1 - (s32)dir);
                }
                branch = m_it->get_child(m_tree->m_ctxt, (s32)dir);
            }
        }
        else
        {
            if (m_it->get_parent_side(m_tree->m_ctxt) == dir)
            {
                m_it = m_it->get_parent(m_tree->m_ctxt);
            }
            else
            {
                m_it = m_it->get_parent(m_tree->m_ctxt);

                // Travel on the outside (left - right - left - right ....)
                tree_t::node_t* branch = m_it->get_child(m_tree->m_ctxt, (s32)dir);
                while (branch != rbnil(m_tree))
                {
                    while (branch != rbnil(m_tree))
                    {
                        m_it   = branch;
                        branch = branch->get_child(m_tree->m_ctxt, 1 - (s32)dir);
                    }
                    branch = m_it->get_child(m_tree->m_ctxt, (s32)dir);
                }
            }
        }

        if (m_it != rbnil(m_tree) && m_it != rbroot(m_tree))
        {
            key = m_it->get_key(m_tree->m_ctxt);
            return true;
        }

        return false;
    }

#undef rbnil
#undef rbfirst
#undef rbroot
#undef rbsetfirst
} // namespace ncore
