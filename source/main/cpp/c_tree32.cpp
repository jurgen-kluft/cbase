#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_bit_field.h"
#include "cbase/c_tree32.h"

namespace ncore
{
    namespace ntree32
    {
        static inline node_t rotate_single(tree_t& tree, node_t node, s32 dir)
        {
            node_t save = tree.v_get_node(node, 1 - dir);
            tree.v_set_node(node, 1 - dir, tree.v_get_node(save, dir));
            tree.v_set_node(save, dir, node);
            tree.v_set_color(node, RED);
            tree.v_set_color(save, BLACK);
            return save;
        }

        static inline node_t rotate_single_track_parent(tree_t& tree, node_t node, s32 dir, node_t fn, node_t& fp)
        {
            node_t save = tree.v_get_node(node, 1 - dir);
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

        static inline node_t rotate_double(tree_t& tree, node_t node, s32 dir)
        {
            tree.v_set_node(node, 1 - dir, rotate_single(tree, tree.v_get_node(node, 1 - dir), 1 - dir));
            return rotate_single(tree, node, dir);
        }

        static inline node_t rotate_double_track_parent(tree_t& tree, node_t node, s32 dir, node_t fn, node_t& fp)
        {
            node_t child = rotate_single_track_parent(tree, tree.v_get_node(node, 1 - dir), 1 - dir, fn, fp);
            tree.v_set_node(node, 1 - dir, child);

            if (fn == child)  // never triggered
                fp = node;
            node_t save = rotate_single_track_parent(tree, node, dir, fn, fp);
            if (fn == node)
                fp = save;
            return save;
        }

        static inline s32 is_red(tree_t& tree, node_t n) { return n != c_invalid_node && tree.v_get_color(n) == RED; }

        bool insert(tree_t& tree, node_t& root, node_t temp, index_t key, compare_fn comparer, void const* user_data, node_t& inserted_or_found)
        {
            node_t inserted = c_invalid_node;
            node_t found    = c_invalid_node;
            if (root == c_invalid_node)
            {
                // We have an empty tree; attach the
                // new node directly to the root
                node_t new_node = tree.v_new_node();
                root            = new_node;
                inserted        = new_node;
            }
            else
            {
                node_t head = temp;  // False tree root
                node_t g, t;         // Grandparent & parent
                node_t p, n;         // Iterator & parent
                s32    dir = 0, last = 0;

                // Set up our helpers
                t = head;
                tree.v_set_color(t, BLACK);
                tree.v_set_node(t, RIGHT, root);
                tree.v_set_node(t, LEFT, c_invalid_node);

                g = p = c_invalid_node;
                n     = root;

                // Search down the tree for a place to insert
                for (;;)
                {
                    if (n == c_invalid_node)
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

                        inserted = n;
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

                    // Stop working if we inserted a node. This
                    // check also disallows duplicates in the tree
                    last = dir;
                    dir  = comparer(key, n, user_data);
                    if (dir == 0)
                    {
                        found = n;
                        break;
                    }
                    dir = ((dir + 1) >> 1);

                    // Move the helpers down
                    if (g != c_invalid_node)
                        t = g;

                    g = p;
                    p = n;
                    n = tree.v_get_node(n, dir);
                }

                // Update the root (it may be different)
                // root = head->get_right(tree);
                root = tree.v_get_node(head, RIGHT);
            }

            // Make the root black for simplified logic
            tree.v_set_color(root, BLACK);

            inserted_or_found = (inserted == c_invalid_node) ? found : inserted;
            return inserted != c_invalid_node;
        }

        bool rb_clear(tree_t& tree, node_t& root, node_t& removed_node)
        {
            removed_node = c_invalid_node;

            node_t node = root;
            if (node == c_invalid_node)
                return true;

            node_t todelete = node;

            if (tree.v_get_node(node, LEFT) == c_invalid_node)
            {
                root = tree.v_get_node(node, RIGHT);
            }
            else if (tree.v_get_node(node, RIGHT) == c_invalid_node)
            {
                root = tree.v_get_node(node, LEFT);
            }
            else
            {
                // We have left and right branches
                // Take right branch and place it
                // somewhere down the left branch
                node_t branch = tree.v_get_node(node, RIGHT);
                tree.v_set_node(node, RIGHT, c_invalid_node);

                // Find a node in the left branch that does not
                // have both a left and right branch and place
                // our branch there.
                node_t iter = tree.v_get_node(node, LEFT);
                while (tree.v_get_node(iter, LEFT) != c_invalid_node && tree.v_get_node(iter, RIGHT) != c_invalid_node)
                {
                    iter = tree.v_get_node(iter, LEFT);
                }
                if (tree.v_get_node(iter, LEFT) == c_invalid_node)
                {
                    tree.v_set_node(iter, LEFT, branch);
                }
                else if (tree.v_get_node(iter, RIGHT) == c_invalid_node)
                {
                    tree.v_set_node(iter, RIGHT, branch);
                }

                root = tree.v_get_node(node, LEFT);
            }

            removed_node = todelete;
            return false;
        }

        bool clear(tree_t& tree, node_t& root, node_t& n)
        {
            node_t node   = c_invalid_node;
            bool   result = rb_clear(tree, root, node);
            if (node != c_invalid_node)
            {
                n = node;
                tree.v_del_node(node);
            }
            return result;
        }

        bool find(tree_t const& tree, node_t root, index_t key, compare_fn comparer, void const* user_data, node_t& found)
        {
            node_t node = root;
            while (node != c_invalid_node)
            {
                const s32 c = comparer(key, node, user_data);
                if (c == 0)
                {
                    found = node;
                    return true;
                }
                node = tree.v_get_node(node, (c + 1) >> 1);
            }
            found = c_invalid_node;
            return false;
        }

        // validate the tree (return violation description in 'result'), also returns black height
        static s32 rb_validate(tree_t& tree, node_t root, const char*& result, compare_fn comparer, void const* user_data)
        {
            if (root == c_invalid_node)
            {
                return 1;
            }
            else
            {
                node_t ln = tree.v_get_node(root, LEFT);
                node_t rn = tree.v_get_node(root, RIGHT);

                // Consecutive red links
                if (is_red(tree, root))
                {
                    if (is_red(tree, ln) || is_red(tree, rn))
                    {
                        result = "Red violation";
                        return 0;
                    }
                }

                s8 lh = rb_validate(tree, ln, result, comparer, user_data);
                s8 rh = rb_validate(tree, rn, result, comparer, user_data);

                // Invalid binary search tree
                if ((ln != c_invalid_node && comparer(ln, root, user_data) >= 0) || (rn != c_invalid_node && comparer(rn, root, user_data) <= 0))
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

        bool remove(tree_t& tree, node_t& root, node_t temp, index_t key, compare_fn comparer, void const* user_data, node_t& out_removed)
        {
            if (root == c_invalid_node)
                return false;

            node_t head = temp;            // False tree root
            node_t fn   = c_invalid_node;  // Found node
            node_t fp   = c_invalid_node;  // Found node parent
            s32    dir  = 1;

            // Set up our helpers
            node_t g = c_invalid_node;
            node_t p = c_invalid_node;

            node_t n = head;
            tree.v_set_color(n, BLACK);  // Color it black
            tree.v_set_node(n, RIGHT, root);
            tree.v_set_node(n, LEFT, c_invalid_node);

            // Search and push a red node down
            // to fix red violations as we go
            while (tree.v_get_node(n, dir) != c_invalid_node)
            {
                const s32 last = dir;

                // Move the helpers down
                g   = p;
                p   = n;
                n   = tree.v_get_node(n, dir);
                dir = comparer(key, n, user_data);

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
                        node_t r = rotate_single_track_parent(tree, n, dir, fn, fp);
                        tree.v_set_node(p, last, r);
                        if (fn == r)  // never triggered
                            fp = p;
                        p = r;
                    }
                    else if (!is_red(tree, tree.v_get_node(n, 1 - dir)))
                    {
                        node_t s = tree.v_get_node(p, 1 - last);
                        if (s != c_invalid_node)
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
                                    node_t r = rotate_double_track_parent(tree, p, last, fn, fp);
                                    // g->set_child(tree, dir2, r);
                                    tree.v_set_node(g, dir2, r);
                                    if (fn == r)  // never triggered
                                        fp = g;
                                }
                                else if (is_red(tree, tree.v_get_node(s, 1 - last)))
                                {
                                    node_t r = rotate_single_track_parent(tree, p, last, fn, fp);
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
            root = tree.v_get_node(head, RIGHT);

            // Replace and remove the saved node
            if (fn != c_invalid_node)
            {
                ASSERT(tree.v_get_node(fp, RIGHT) == fn || tree.v_get_node(fp, LEFT) == fn);
                ASSERT(tree.v_get_node(p, RIGHT) == n || tree.v_get_node(p, LEFT) == n);

                node_t child1 = tree.v_get_node(n, tree.v_get_node(n, LEFT) == c_invalid_node);
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
                        root = c_invalid_node;
                }

                // tree.v_del_node(fn); // User must delete the node
                out_removed = fn;
            }

            // Make the root black for simplified logic
            if (root != c_invalid_node)
                tree.v_set_color(root, BLACK);

            return true;
        }

        bool validate(tree_t& tree, node_t root, const char*& error_str, compare_fn comparer, void const* user_data)
        {
            rb_validate(tree, root, error_str, comparer, user_data);
            return error_str == nullptr;
        }

        iterator_t iterate(tree_t& tree, node_t root)
        {
            iterator_t iter(tree, root);
            iter.m_it    = c_invalid_node;
            iter.m_stack = -1;
            return iter;
        }

        bool iterator_t::traverse(tree_t& tree, s32 d, node_t& out_node)
        {
            if (m_it == c_invalid_node)
            {
                m_it = m_root;
            }
            else
            {
                m_it = tree.v_get_node(m_it, d);
            }

            if (m_it != c_invalid_node)
            {
                out_node = m_it;
                return true;
            }
            return false;
        }

        bool iterator_t::preorder(tree_t& tree, s32 dir, node_t& out_node)
        {
            if (m_stack == -1)
            {
                m_stack = 0;
                if (m_root != c_invalid_node)
                {
                    m_stack_array[m_stack++] = m_root;
                }
            }

            if (m_stack == 0)
            {
                out_node = c_invalid_node;
                return false;
            }

            m_it = m_stack_array[--m_stack];

            node_t child1 = tree.v_get_node(m_it, 1 - dir);
            if (child1 != c_invalid_node)
                m_stack_array[m_stack++] = child1;

            node_t child2 = tree.v_get_node(m_it, dir);
            if (child2 != c_invalid_node)
                m_stack_array[m_stack++] = child2;

            out_node = m_it;
            return true;
        }

        bool iterator_t::sortorder(tree_t& tree, s32 dir, node_t& out_node)
        {
            if (m_stack == -1)
            {
                m_stack = 0;
                m_it    = m_root;
            }

            while (m_it != c_invalid_node)
            {
                m_stack_array[m_stack++] = m_it;
                m_it                     = tree.v_get_node(m_it, dir);
            }

            if (m_stack == 0)
                return false;

            m_it     = m_stack_array[--m_stack];
            out_node = m_it;
            m_it     = tree.v_get_node(m_it, 1 - dir);
            return true;
        }

        bool iterator_t::postorder(tree_t& tree, s32 dir, node_t& out_node)
        {
            if (m_stack == -1)
            {
                m_stack = 0;
                m_it    = m_root;
                if (m_it != c_invalid_node)
                {
                    m_stack_array[m_stack++] = m_it;
                }
            }

            if (m_stack == 0)
                return false;

            while (true)
            {
                node_t const node   = m_stack_array[m_stack - 1];
                node_t const child1 = tree.v_get_node(node, 1 - dir);
                node_t const child2 = tree.v_get_node(node, dir);
                if ((child1 == m_it || child2 == m_it) || (child1 == c_invalid_node && child2 == c_invalid_node))
                {
                    m_it     = node;
                    out_node = m_it;
                    m_stack--;
                    return true;
                }
                else
                {
                    if (child1 != c_invalid_node)
                        m_stack_array[m_stack++] = child1;
                    if (child2 != c_invalid_node)
                        m_stack_array[m_stack++] = child2;
                }
            }
            return true;
        }

        void setup_tree(tree_t& c, void* nodes, void* colors)
        {
            c.m_free_index = 0;
            c.m_free_head  = c_invalid_index;
            c.m_nodes      = (tree_t::nnode_t*)nodes;
            c.m_colors     = (u8*)colors;
        }

        void teardown_tree(tree_t& c)
        {
            c.m_free_index = 0;
            c.m_free_head  = c_invalid_index;
            c.m_nodes      = nullptr;
            c.m_colors     = nullptr;
        }

    }  // namespace ntree32
}  // namespace ncore
