#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_bit_field.h"
#include "cbase/c_tree.h"

namespace ncore
{
    namespace ntree
    {
        static inline void s_init_node(tree_t& tree, node_t* node)
        {
            node->m_child[LEFT]  = nullptr;
            node->m_child[RIGHT] = nullptr;
            node->m_item         = nullptr;
            tree.v_set_color(node, RED);
        }

        static inline node_t* rotate_single(tree_t& tree, node_t* node, s32 dir)
        {
            node_t* save = tree.v_get_node(node, 1 - dir);
            tree.v_set_node(node, 1 - dir, tree.v_get_node(save, dir));
            tree.v_set_node(save, dir, node);
            tree.v_set_color(node, RED);
            tree.v_set_color(save, BLACK);
            return save;
        }

        static inline node_t* rotate_single_track_parent(tree_t& tree, node_t* node, s32 dir, node_t* fn, node_t*& fp)
        {
            node_t* save = tree.v_get_node(node, 1 - dir);
            tree.v_set_node(node, 1 - dir, tree.v_get_node(save, dir));
            tree.v_set_node(save, dir, node);
            tree.v_set_color(node, RED);
            tree.v_set_color(save, BLACK);

            if (fn == node)
                fp = save;
            else if (fn == tree.v_get_node(node, 1 - dir))
                fp = node;

            return save;
        }

        static inline node_t* rotate_double(tree_t& tree, node_t* node, s32 dir)
        {
            tree.v_set_node(node, 1 - dir, rotate_single(tree, tree.v_get_node(node, 1 - dir), 1 - dir));
            return rotate_single(tree, node, dir);
        }

        static inline node_t* rotate_double_track_parent(tree_t& tree, node_t* node, s32 dir, node_t* fn, node_t*& fp)
        {
            node_t* child = rotate_single_track_parent(tree, tree.v_get_node(node, 1 - dir), 1 - dir, fn, fp);
            tree.v_set_node(node, 1 - dir, child);

            if (fn == child)  // never triggered
                fp = node;
            node_t* save = rotate_single_track_parent(tree, node, dir, fn, fp);
            if (fn == node)
                fp = save;
            return save;
        }

        static inline s32 is_red(tree_t& tree, node_t* n) { return n != nullptr && tree.v_get_color(n) == RED; }

        bool insert(tree_t& tree, void const* key, compare_fn comparer, node_t*& _inserted)
        {
            _inserted    = nullptr;
            node_t* root = tree.v_get_root();
            if (root == nullptr)
            {
                // We have an empty tree; attach the new node directly to the root
                root      = tree.v_new_node();
                _inserted = root;
            }
            else
            {
                node_t  head;   // False tree root
                node_t *g, *t;  // Grandparent & parent
                node_t *p, *n;  // Iterator & parent
                s8      dir = 0, last = 0;

                // Set up our helpers
                t         = &head;
                t->m_item = nullptr;
                tree.v_set_color(t, BLACK);
                tree.v_set_node(t, RIGHT, root);
                tree.v_set_node(t, LEFT, nullptr);

                g = p = nullptr;
                n     = root;

                // Search down the tree for a place to insert
                for (;;)
                {
                    if (n == nullptr)
                    {
                        // Insert a new node at the first null link
                        n = tree.v_new_node();
                        tree.v_set_node(p, dir, n);

                        if (is_red(tree, n) && is_red(tree, p))
                        {
                            // Hard red violation: rotations necessary
                            const s32 dir2 = (tree.v_get_node(t, RIGHT) == g) ? 1 : 0;

                            if (n == tree.v_get_node(p, last))
                                tree.v_set_node(t, dir2, rotate_single(tree, g, 1 - last));
                            else
                                tree.v_set_node(t, dir2, rotate_double(tree, g, 1 - last));
                        }

                        _inserted = n;
                        break;
                    }
                    else if (is_red(tree, tree.v_get_node(n, LEFT)) && is_red(tree, tree.v_get_node(n, RIGHT)))
                    {
                        // Simple red violation: color flip
                        tree.v_set_color(n, RED);
                        tree.v_set_color(tree.v_get_node(n, LEFT), BLACK);
                        tree.v_set_color(tree.v_get_node(n, RIGHT), BLACK);
                    }

                    if (is_red(tree, n) && is_red(tree, p))
                    {
                        // Hard red violation: rotations necessary
                        const s32 dir2 = (tree.v_get_node(t, RIGHT) == g) ? 1 : 0;

                        if (n == tree.v_get_node(p, last))
                            tree.v_set_node(t, dir2, rotate_single(tree, g, 1 - last));
                        else
                            tree.v_set_node(t, dir2, rotate_double(tree, g, 1 - last));
                    }

                    // This check disallows duplicates in the tree
                    last = dir;
                    dir  = comparer(key, tree.v_get_item(n));
                    if (dir == 0)
                        break;
                    dir = ((dir + 1) >> 1);

                    // Move the helpers down
                    if (g != nullptr)
                        t = g;

                    g = p;
                    p = n;
                    n = tree.v_get_node(n, dir);
                }

                // Update the root (it may be different)
                root = tree.v_get_node(&head, RIGHT);
            }

            // Make the root black for simplified logic
            tree.v_set_root(root);
            tree.v_set_color(root, BLACK);

            return _inserted != nullptr;
        }

        bool rb_clear(tree_t& tree, node_t*& removed_node)
        {
            removed_node = nullptr;

            node_t* node = tree.v_get_root();
            if (node == nullptr)
                return true;

            node_t* todelete = node;

            if (tree.v_get_node(node, LEFT) == nullptr)
            {
                tree.v_set_root(tree.v_get_node(node, RIGHT));
            }
            else if (tree.v_get_node(node, RIGHT) == nullptr)
            {
                tree.v_set_root(tree.v_get_node(node, LEFT));
            }
            else
            {
                // We have left and right branches
                // Take right branch and place it
                // somewhere down the left branch
                node_t* branch = tree.v_get_node(node, RIGHT);
                tree.v_set_node(node, RIGHT, nullptr);

                // Find a node in the left branch that does not
                // have both a left and right branch and place
                // our branch there.
                node_t* iter = tree.v_get_node(node, LEFT);
                while (tree.v_get_node(iter, LEFT) != nullptr && tree.v_get_node(iter, RIGHT) != nullptr)
                {
                    iter = tree.v_get_node(iter, LEFT);
                }
                if (tree.v_get_node(iter, LEFT) == nullptr)
                {
                    tree.v_set_node(iter, LEFT, branch);
                }
                else if (tree.v_get_node(iter, RIGHT) == nullptr)
                {
                    tree.v_set_node(iter, RIGHT, branch);
                }

                tree.v_set_root(tree.v_get_node(node, LEFT));
            }

            removed_node = todelete;
            return false;
        }

        bool clear(tree_t& tree, node_t*& n)
        {
            node_t* node   = nullptr;
            bool    result = rb_clear(tree, node);
            n              = node;
            return result;
        }

        bool find(tree_t const& tree, void const* key, compare_fn comparer, node_t*& found)
        {
            node_t* node = tree.v_get_root();
            while (node != nullptr)
            {
                const s8 c = comparer(key, tree.v_get_item(node));
                if (c == 0)
                {
                    found = node;
                    return true;
                }
                node = tree.v_get_node(node, (c + 1) >> 1);
            }
            found = nullptr;
            return false;
        }

        // validate the tree (return violation description in 'result'), also returns black height
        static s32 rb_validate(tree_t& tree, node_t* root, const char*& result, compare_fn comparer)
        {
            if (root == nullptr)
            {
                return 1;
            }
            else
            {
                // node_t* ln = root->get_left(tree);
                // node_t* rn = root->get_right(tree);
                node_t* ln = tree.v_get_node(root, LEFT);
                node_t* rn = tree.v_get_node(root, RIGHT);

                // Consecutive red links
                if (is_red(tree, root))
                {
                    if (is_red(tree, ln) || is_red(tree, rn))
                    {
                        result = "Red violation";
                        return 0;
                    }
                }

                s8 lh = rb_validate(tree, ln, result, comparer);
                s8 rh = rb_validate(tree, rn, result, comparer);

                // Invalid binary search tree
                if ((ln != nullptr && comparer(tree.v_get_item(ln), tree.v_get_item(root)) >= 0) || (rn != nullptr && comparer(tree.v_get_item(rn), tree.v_get_item(root)) <= 0))
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

        bool remove(tree_t& tree, void const* key, compare_fn comparer, node_t*& out_removed)
        {
            node_t* root = tree.v_get_root();
            if (root == nullptr)
                return false;

            node_t  head;           // False tree root
            node_t* fn  = nullptr;  // Found node
            node_t* fp  = nullptr;  // Found node parent
            s8      dir = 1;

            // Set up our helpers
            node_t* g = nullptr;
            node_t* p = nullptr;

            node_t* n = &head;
            tree.v_set_color(n, BLACK);  // Color it black
            tree.v_set_node(n, RIGHT, root);
            tree.v_set_node(n, LEFT, nullptr);

            // Search and push a red node down
            // to fix red violations as we go
            while (tree.v_get_node(n, dir) != nullptr)
            {
                const s8 last = dir;

                // Move the helpers down
                g   = p;
                p   = n;
                n   = tree.v_get_node(n, dir);
                dir = comparer(key, tree.v_get_item(n));

                // Save the node with matching data and keep
                // going; we'll do removal tasks at the end
                if (dir == 0)
                {
                    fn = n;
                    fp = p;
                }
                dir = ((dir + 1) >> 1);

                /* Push the red node down with rotations and color flips */
                if (!is_red(tree, n) && !is_red(tree, tree.v_get_node(n, dir)))
                {
                    if (is_red(tree, tree.v_get_node(n, 1 - dir)))
                    {
                        node_t* r = rotate_single_track_parent(tree, n, dir, fn, fp);
                        tree.v_set_node(p, last, r);
                        if (fn == r)  // never triggered
                            fp = p;
                        p = r;
                    }
                    else if (!is_red(tree, tree.v_get_node(n, 1 - dir)))
                    {
                        node_t* s = tree.v_get_node(p, 1 - last);
                        if (s != nullptr)
                        {
                            if (!is_red(tree, tree.v_get_node(s, 1 - last)) && !is_red(tree, tree.v_get_node(s, last)))
                            {
                                // Color flip
                                tree.v_set_color(p, BLACK);
                                tree.v_set_color(s, RED);
                                tree.v_set_color(n, RED);
                            }
                            else
                            {
                                // const s32 dir2 = g->get_right(tree) == p ? 1 : 0;
                                const s32 dir2 = tree.v_get_node(g, RIGHT) == p ? 1 : 0;
                                if (is_red(tree, tree.v_get_node(s, last)))
                                {
                                    node_t* r = rotate_double_track_parent(tree, p, last, fn, fp);
                                    // g->set_child(tree, dir2, r);
                                    tree.v_set_node(g, dir2, r);
                                    if (fn == r)  // never triggered
                                        fp = g;
                                }
                                else if (is_red(tree, tree.v_get_node(s, 1 - last)))
                                {
                                    node_t* r = rotate_single_track_parent(tree, p, last, fn, fp);
                                    // g->set_child(tree, dir2, r);
                                    tree.v_set_node(g, dir2, r);
                                    if (fn == r)  // never triggered
                                        fp = g;
                                }

                                // Ensure correct coloring
                                tree.v_set_color(n, RED);
                                tree.v_set_color(tree.v_get_node(g, dir2), RED);

                                tree.v_set_color(tree.v_get_node(tree.v_get_node(g, dir2), LEFT), BLACK);
                                tree.v_set_color(tree.v_get_node(tree.v_get_node(g, dir2), RIGHT), BLACK);
                            }
                        }
                    }
                }
            }

            // Update the root (it may be different)
            root = tree.v_get_node(&head, RIGHT);

            // Replace and remove the saved node
            if (fn != nullptr)
            {
                ASSERT(tree.v_get_node(fp, RIGHT) == fn || tree.v_get_node(fp, LEFT) == fn);
                ASSERT(tree.v_get_node(p, RIGHT) == n || tree.v_get_node(p, LEFT) == n);

                node_t* child1 = tree.v_get_node(n, tree.v_get_node(n, LEFT) == nullptr);
                tree.v_set_node(p, tree.v_get_node(p, RIGHT) == n, child1);

                if (fn != n)
                {
                    ASSERT(fp != p);

                    // swap 'n' and 'fn', we want to remove the node that was holding 'item'
                    tree.v_set_node(fp, tree.v_get_node(fp, RIGHT) == fn, n);
                    tree.v_set_node(n, LEFT, tree.v_get_node(fn, LEFT));
                    tree.v_set_node(n, RIGHT, tree.v_get_node(fn, RIGHT));
                    tree.v_set_color(n, tree.v_get_color(fn));
                    if (root == fn)
                        root = n;
                }
                else
                {
                    if (root == fn)
                        root = nullptr;
                }

                // tree.v_del_node(fn);
                out_removed = fn;  // User is responsible for deleting the node
            }

            // Make the root black for simplified logic
            if (root != nullptr)
                tree.v_set_color(root, BLACK);

            tree.v_set_root(root);
            return true;
        }

        bool validate(tree_t& tree, const char*& error_str, compare_fn comparer)
        {
            node_t* root = tree.v_get_root();
            rb_validate(tree, root, error_str, comparer);
            return error_str == nullptr;
        }

        iterator_t iterate()
        {
            iterator_t iter;
            iter.m_it    = nullptr;
            iter.m_stack = -1;
            return iter;
        }

        bool iterator_t::traverse(tree_t const& tree, s32 d, void*& item)
        {
            if (m_it == nullptr)
            {
                m_it = tree.v_get_root();
            }
            else
            {
                m_it = tree.v_get_node(m_it, d);
            }

            if (m_it != nullptr)
            {
                item = tree.v_get_item(m_it);
                return true;
            }
            return false;
        }

        bool iterator_t::preorder(tree_t const& tree, s32 dir, void*& item)
        {
            if (m_stack == -1)
            {
                m_stack = 0;
                if (tree.v_get_root() != nullptr)
                {
                    m_stack_array[m_stack++] = tree.v_get_root();
                }
            }

            item = nullptr;
            if (m_stack == 0)
                return false;

            node_t* node = m_stack_array[--m_stack];
            item         = tree.v_get_item(node);

            node_t* child1 = tree.v_get_node(node, 1 - dir);
            if (child1 != nullptr)
                m_stack_array[m_stack++] = child1;

            node_t* child2 = tree.v_get_node(node, dir);
            if (child2 != nullptr)
                m_stack_array[m_stack++] = child2;

            return true;
        }

        bool iterator_t::sortorder(tree_t const& tree, s32 dir, void*& item)
        {
            if (m_stack == -1)
            {
                m_stack = 0;
                m_it    = tree.v_get_root();
            }

            while (m_it != nullptr)
            {
                m_stack_array[m_stack++] = m_it;
                m_it                     = tree.v_get_node(m_it, dir);
            }

            if (m_stack == 0)
                return false;

            m_it = m_stack_array[--m_stack];
            item = tree.v_get_item(m_it);
            m_it = tree.v_get_node(m_it, 1 - dir);
            return true;
        }

        bool iterator_t::postorder(tree_t const& tree, s32 dir, void*& item)
        {
            if (m_stack == -1)
            {
                m_stack = 0;
                m_it    = tree.v_get_root();
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
                node_t* const child1 = tree.v_get_node(node, 1 - dir);
                node_t* const child2 = tree.v_get_node(node, dir);
                if ((child1 == m_it || child2 == m_it) || (child1 == nullptr && child2 == nullptr))
                {
                    m_it = node;
                    item = tree.v_get_item(m_it);
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

        void setup_tree(alloc_t* allocator, tree_t& c)
        {
            c.m_alloc = allocator;
            c.m_root  = nullptr;
        }

        void teardown_tree(alloc_t* allocator, tree_t& c) {}

    }  // namespace ntree
}  // namespace ncore
