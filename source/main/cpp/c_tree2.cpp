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

        static inline node_t* rotate_single(tree_t* tree, node_t* node, s32 dir)
        {
            node_t* save = node->get_child(tree, 1 - dir);
            node->set_child(tree, 1 - dir, save->get_child(tree, dir));
            save->set_child(tree, dir, node);
            node->set_red(tree);
            save->set_black(tree);
            return save;
        }

        static inline node_t* rotate_single_track_parent(tree_t* tree, node_t* node, s32 dir, node_t* fn, node_t*& fp)
        {
            node_t* save = node->get_child(tree, 1 - dir);
            node->set_child(tree, 1 - dir, save->get_child(tree, dir));
            save->set_child(tree, dir, node);
            node->set_red(tree);
            save->set_black(tree);

            if (fn == node)
                fp = save;
            else if (fn == node->get_child(tree, 1 - dir))  // never triggered
                fp = node;

            return save;
        }

        static inline node_t* rotate_double(tree_t* tree, node_t* node, s32 dir)
        {
            node->set_child(tree, 1 - dir, rotate_single(tree, node->get_child(tree, 1 - dir), 1 - dir));
            return rotate_single(tree, node, dir);
        }

        static inline node_t* rotate_double_track_parent(tree_t* tree, node_t* node, s32 dir, node_t* fn, node_t*& fp)
        {
            node_t* child = rotate_single_track_parent(tree, node->get_child(tree, 1 - dir), 1 - dir, fn, fp);
            node->set_child(tree, 1 - dir, child);

            if (fn == child)  // never triggered
                fp = node;
            node_t* save = rotate_single_track_parent(tree, node, dir, fn, fp);
            if (fn == node)
                fp = save;
            return save;
        }

        bool insert(tree_t* tree, void const* key, void const* value)
        {
            bool    inserted = false;
            node_t* root     = tree->v_get_root();
            if (root == nullptr)
            {
                // We have an empty tree; attach the
                // new node directly to the root
                node_t* new_node = tree->v_new_node(key, value);
                tree->v_set_root(new_node);
                inserted = true;
            }
            else
            {
                node_t* head = tree->v_get_temp();  // False tree root
                node_t *g, *t;                      // Grandparent & parent
                node_t *p, *n;                      // Iterator & parent
                s32     dir = 0, last = 0;

                // Set up our helpers
                t = head;
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
                        inserted = true;
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
            root = tree->v_get_root();
            root->set_black(tree);

            return inserted;
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

        bool clear(tree_t* tree)
        {
            node_t* node   = nullptr;
            bool    result = rb_clear(tree, node);
            if (node != nullptr)
            {
                tree->v_del_node(node);
            }
            return result;
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

        // validate the tree (return violation description in 'result'), also returns black height
        static s32 rb_validate(tree_t* tree, node_t* root, const char*& result)
        {
            if (root == nullptr)
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

                s32 lh = rb_validate(tree, ln, result);
                s32 rh = rb_validate(tree, rn, result);

                // Invalid binary search tree
                if ((ln != nullptr && tree->v_compare_nodes(ln, root) >= 0) || (rn != nullptr && tree->v_compare_nodes(rn, root) <= 0))
                {
                    result = "Binary tree violation";
                    return 0;
                }

                if (lh != 0 && rh != 0 && lh != rh)  // Black height mismatch
                {
                    result = "Black violation";
                    return 0;
                }

                if (lh != 0 && rh != 0)  // Only count black links
                {
                    return is_red(tree, root) ? lh : lh + 1;
                }
            }
            return 0;
        }

        bool remove(tree_t* tree, void const* key)
        {
            node_t* root = tree->v_get_root();
            if (root == nullptr)
                return false;

            node_t* head = tree->v_get_temp();  // False tree root
            node_t* fn   = nullptr;             // Found node
            node_t* fp   = nullptr;             // Found node parent
            s32     dir  = 1;

            // Set up our helpers
            node_t* g = nullptr;
            node_t* p = nullptr;

            node_t* n = head;
            n->set_black(tree);  // Color it black
            n->set_right(tree, root);
            n->set_left(tree, nullptr);

            // Search and push a red node down
            // to fix red violations as we go
            while (n->get_child(tree, dir) != nullptr)
            {
                const s32 last = dir;

                // Move the helpers down
                g = p, p = n;
                n   = n->get_child(tree, dir);
                dir = tree->v_compare_insert(key, n);

                // Save the node with matching data and keep
                // going; we'll do removal tasks at the end
                if (dir == 0)
                {
                    fn = n;
                    fp = p;
                }
                dir = ((dir + 1) >> 1);

                /* Push the red node down with rotations and color flips */
                if (!is_red(tree, n) && !is_red(tree, n->get_child(tree, dir)))
                {
                    if (is_red(tree, n->get_child(tree, 1 - dir)))
                    {
                        node_t* r = rotate_single_track_parent(tree, n, dir, fn, fp);
                        p->set_child(tree, last, r);
                        if (fn == r)  // never triggered
                            fp = p;
                        p = r;
                    }
                    else if (!is_red(tree, n->get_child(tree, 1 - dir)))
                    {
                        node_t* s = p->get_child(tree, 1 - last);
                        if (s != nullptr)
                        {
                            if (!is_red(tree, s->get_child(tree, 1 - last)) && !is_red(tree, s->get_child(tree, last)))
                            {
                                // Color flip
                                p->set_black(tree);
                                s->set_red(tree);
                                n->set_red(tree);
                            }
                            else
                            {
                                const s32 dir2 = g->get_right(tree) == p ? 1 : 0;
                                if (is_red(tree, s->get_child(tree, last)))
                                {
                                    node_t* r = rotate_double_track_parent(tree, p, last, fn, fp);
                                    g->set_child(tree, dir2, r);
                                    if (fn == r)  // never triggered
                                        fp = g;
                                }
                                else if (is_red(tree, s->get_child(tree, 1 - last)))
                                {
                                    node_t* r = rotate_single_track_parent(tree, p, last, fn, fp);
                                    g->set_child(tree, dir2, r);
                                    if (fn == r)  // never triggered
                                        fp = g;
                                }

                                // Ensure correct coloring
                                n->set_red(tree);
                                g->get_child(tree, dir2)->set_red(tree);

                                g->get_child(tree, dir2)->get_left(tree)->set_black(tree);
                                g->get_child(tree, dir2)->get_right(tree)->set_black(tree);
                            }
                        }
                    }
                }
            }

            // Update the root (it may be different)
            root = head->get_right(tree);

            // Replace and remove the saved node
            if (fn != nullptr)
            {
                ASSERT(fp->get_right(tree) == fn || fp->get_left(tree) == fn);

                ASSERT(p->get_right(tree) == n || p->get_left(tree) == n);
                node_t* child1 = n->get_child(tree, n->get_left(tree) == nullptr);
                p->set_child(tree, p->get_right(tree) == n, child1);

                if (fn != n)
                {
                    ASSERT(fp != p);

                    // swap 'n' and 'fn', we want to remove the node that was holding 'item'
                    fp->set_child(tree, fp->get_right(tree) == fn, n);
                    n->set_child(tree, LEFT, fn->get_left(tree));
                    n->set_child(tree, RIGHT, fn->get_right(tree));
                    n->set_color(tree, fn->get_color(tree));
                }

                if (root == fn)
                    root = n;

                tree->v_del_node(fn);
            }

            // Make the root black for simplified logic
            if (root != nullptr)
                root->set_black(tree);

            tree->v_set_root(root);
            return true;
        }

        bool validate(tree_t* tree, const char*& error_str)
        {
            node_t* root = tree->v_get_root();
            rb_validate(tree, root, error_str);
            return error_str == nullptr;
        }

        iterator_t iterate(tree_t* ctxt)
        {
            iterator_t iter;
            iter.m_tree  = ctxt;
            iter.m_it    = nullptr;
            iter.m_stack = 0;
            return iter;
        }

        bool iterator_t::traverse(s32 d, void const*& key)
        {
            if (m_it == nullptr)
            {
                m_it = m_tree->v_get_root()->get_child(m_tree, d);
            }
            else
            {
                m_it = m_it->get_child(m_tree, d);
            }

            if (m_it != nullptr)
            {
                key = m_it->get_key(m_tree);
                return true;
            }
            return false;
        }

        bool iterator_t::preorder(s32 dir, void const*& key)
        {
            key = nullptr;

            if (m_it == nullptr)
            {
                m_stack = 0;
                if (m_tree->v_get_root() != nullptr)
                    m_stack_array[m_stack++] = m_tree->v_get_root();
            }

            if (m_stack == 0)
                return false;

            node_t* node = m_stack_array[--m_stack];
            key          = node->get_key(m_tree);

            node = node->get_child(m_tree, 1 - dir);
            if (node != nullptr)
                m_stack_array[m_stack++] = node;

            node = node->get_child(m_tree, dir);
            if (node != nullptr)
                m_stack_array[m_stack++] = node;

            return true;
        }

        bool iterator_t::sortorder(s32 dir, void const*& key)
        {
            if (m_it == nullptr && m_stack == 0)
            {
                m_stack = 0;
                m_it    = m_tree->v_get_root();
                if (m_it == nullptr)
                    return false;
                m_stack_array[m_stack++] = m_it;
            }

            while (m_it != nullptr)
            {
                m_stack_array[m_stack++] = m_it;
                m_it                     = m_it->get_child(m_tree, dir);
            }

            m_it = m_stack_array[--m_stack];
            key  = m_it->get_key(m_tree);
            m_it = m_it->get_child(m_tree, 1 - dir);
            return true;
        }

        bool iterator_t::postorder(s32 dir, void const*& key)
        {
            if (m_it == nullptr)
            {
                m_stack = 0;
                m_it    = m_tree->v_get_root();
                if (m_it != nullptr)
                {
                    m_stack_array[m_stack++] = m_it;
                }
            }

            if (m_stack == 0)
                return false;

            while (true)
            {
                node_t* const node   = m_stack_array[m_stack - 1];
                node_t* const child1 = node->get_child(m_tree, 1 - dir);
                node_t* const child2 = node->get_child(m_tree, dir);
                if ((child1 == m_it || child2 == m_it) || (child1 == nullptr && child2 == nullptr))
                {
                    m_it = node;
                    key  = m_it->get_key(m_tree);
                    m_stack--;
                    return true;
                }
                else
                {
                    if (child1 != nullptr)
                        m_stack_array[m_stack++] = child1;
                    if (child2 != nullptr)
                        m_stack_array[m_stack++] = child2;
                }
            }
            return true;
        }
    }  // namespace ntree2
}  // namespace ncore
