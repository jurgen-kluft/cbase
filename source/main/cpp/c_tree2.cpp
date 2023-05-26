#include "ccore/c_target.h"
#include "cbase/c_bit_field.h"
#include "cbase/c_tree2.h"

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

    namespace ntree2
    {
        struct node_t
        {
            inline void const* get_key(tree_t const* c) const { return c->v_get_key(this); }
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
        };

        static inline s32 is_red(tree_t* tree, node_t* n) { return n != nullptr && n->is_red(tree); }

        s32 rb_insert(tree_t* tree, void const* key, void const* value)
        {
            s32     result = 0;
            node_t* root   = tree->v_get_root();
            if (root == nullptr)
            {
                // We have an empty tree; attach the
                // new node directly to the root
                node_t* new_node = tree->v_new_node(key, value);
                tree->v_set_root(new_node);
                result = 1;
            }
            else
            {
                node_t* head = tree->v_get_temp_root();  // False tree root
                node_t *g, *t;                           // Grandparent & parent
                node_t *p, *n;                           // Iterator & parent
                s32     dir = 0, last = 0;

                // Set up our helpers
                t         = head;
                t->set_black(tree);  // Color it black
                t->set_right(tree, root);
                t->set_left(tree, nullptr);

                g = p = nullptr;
                n     = root;

                // Search down the tree for a place to insert
                for (;;)
                {
                    if (n == nullptr)
                    {
                        // Insert a new node at the first null link
                        n = tree->v_new_node(key, value);
                        p->set_child(tree, dir, n);
                        result = 1;
                    }
                    else if (is_red(tree, n->get_left(tree)) && is_red(tree, n->get_right(tree)))
                    {
                        // Simple red violation: color flip
                        n->set_red(tree);
                        n->get_left(tree)->set_black(tree);
                        n->get_right(tree)->set_black(tree);
                    }

                    if (is_red(tree, n) && is_red(tree, p))
                    {
                        // Hard red violation: rotations necessary
                        const s32 dir2 = (t->get_right(tree) == g) ? 1 : 0;

                        if (n == p->get_child(tree, last))
                            t->set_child(tree, dir2, rotate_single(tree, g, 1 - last));
                        else
                            t->set_child(tree, dir2, rotate_double(tree, g, 1 - last));
                    }

                    // Stop working if we inserted a node. This
                    // check also disallows duplicates in the tree
                    last = dir;
                    dir  = tree->v_compare_insert(key, n);
                    if (dir == 0)
                        break;
                    dir = ((dir + 1) >> 1);

                    // Move the helpers down
                    if (g != nullptr)
                        t = g;

                    g = p, p = n;
                    n = n->get_child(tree, dir);
                }

                // Update the root (it may be different)
                root = head->get_right(tree);
                tree->v_set_root(root);
            }

            // Make the root black for simplified logic
            root->set_black(tree);

            return result;
        }

        bool rb_clear(tree_t* tree, node_t*& removed_node)
        {
            removed_node = nullptr;

            node_t* node = tree->v_get_root();
            if (node == nullptr)
                return true;

            node_t* todelete = node;
            if (node->get_left(tree) == nullptr)
            {
                tree->v_set_root(node->get_right(tree));
            }
            else if (node->get_right(tree) == nullptr)
            {
                tree->v_set_root(node->get_left(tree));
            }
            else
            {
                // We have left and right branches
                // Take right branch and place it
                // somewhere down the left branch
                node_t* branch = node->get_right(tree);
                node->set_right(tree, nullptr);

                // Find a node in the left branch that does not
                // have both a left and right branch and place
                // our branch there.
                node_t* iter = node->get_left(tree);
                while (iter->get_left(tree) != nullptr && iter->get_right(tree) != nullptr)
                {
                    iter = iter->get_left(tree);
                }
                if (iter->get_left(tree) == nullptr)
                {
                    iter->set_left(tree, branch);
                }
                else if (iter->get_right(tree) == nullptr)
                {
                    iter->set_right(tree, branch);
                }

                tree->v_set_root(node->get_left(tree));
            }

            removed_node = todelete;
            return false;
        }

        bool find(tree_t* tree, void const* key, node_t*& found)
        {
            node_t* node = tree->v_get_root();
            while (node != nullptr)
            {
                const s32 c = tree->v_compare_insert(key, node);
                if (c == 0)
                {
                    found = node;
                    return true;
                }
                node = node->get_child(tree, (c + 1) >> 1);
            }
            found = nullptr;
            return false;
        }

        bool insert(tree_t* ctxt, void const* key, void const* value) { return false; }

        bool remove(tree_t* ctxt, void const* key) { return false; }

        bool validate(tree_t* ctxt, const char*& error_str) { return false; }

        iterator_t iterate(tree_t* ctxt)
        {
            iterator_t iter;
            iter.m_tree = ctxt;
            iter.m_it   = nullptr;
            return iter;
        }

    }  // namespace ntree2
}  // namespace ncore
