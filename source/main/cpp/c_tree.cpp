#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_bit_field.h"
#include "cbase/c_tree.h"

namespace ncore
{
    namespace ntree
    {
        static inline void s_init_node(node_t* node)
        {
            node->set_child(LEFT, nullptr);
            node->set_child(RIGHT,nullptr);
            node->set_color(RED);
        }

        static inline node_t* rotate_single(node_t* node, s32 dir)
        {
            node_t* save = node->get_child(1 - dir);
            node->set_child(1 - dir, save->get_child(dir));
            save->set_child(dir, node);
            node->set_color(RED);
            save->set_color(BLACK);
            return save;
        }

        static inline node_t* rotate_single_track_parent(node_t* node, s32 dir, node_t* fn, node_t*& fp)
        {
            node_t* save = node->get_child(1 - dir);
            node->set_child(1 - dir, save->get_child(dir));
            save->set_child(dir, node);
            node->set_color(RED);
            save->set_color(BLACK);

            if (fn == node)
                fp = save;
            else if (fn == node->get_child(1 - dir))
                fp = node;

            return save;
        }

        static inline node_t* rotate_double(node_t* node, s32 dir)
        {
            node->set_child(1 - dir, rotate_single(node->get_child(1 - dir), 1 - dir));
            return rotate_single(node, dir);
        }

        static inline node_t* rotate_double_track_parent(node_t* node, s32 dir, node_t* fn, node_t*& fp)
        {
            node_t* child = rotate_single_track_parent(node->get_child(1 - dir), 1 - dir, fn, fp);
            node->set_child(1 - dir, child);

            if (fn == child)  // never triggered
                fp = node;
            node_t* save = rotate_single_track_parent(node, dir, fn, fp);
            if (fn == node)
                fp = save;
            return save;
        }

        static inline s32 is_red(node_t const* n) { return n != nullptr && n->get_color() == RED; }

        bool insert(node_t*& root, node_t* temp, void const* key, compare_key_and_node_fn comparer, new_node_fn new_node, void* user_data, node_t*& _inserted)
        {
            if (root == nullptr)
            {
                // We have an empty tree; attach the new node directly to the root
                root      = new_node(user_data);
                _inserted = root;
            }
            else
            {
                node_t* head = temp;  // False tree root
                node_t *g, *t;        // Grandparent & parent
                node_t *p, *n;        // Iterator & parent
                s8      dir = 0, last = 0;

                // Set up our helpers
                t = head;
                t->set_color(BLACK);
                t->set_child(RIGHT, root);
                t->set_child(LEFT, nullptr);

                g = p = nullptr;
                n     = root;

                // Search down the tree for a place to insert
                _inserted = nullptr;
                for (;;)
                {
                    if (n == nullptr)
                    {
                        // Insert a new node at the first null link
                        n = new_node(user_data);
                        p->set_child(dir, n);

                        if (is_red(n) && is_red(p))
                        {
                            // Hard red violation: rotations necessary
                            const s32 dir2 = (t->get_child(RIGHT) == g) ? 1 : 0;

                            if (n == p->get_child(last))
                                t->set_child(dir2, rotate_single(g, 1 - last));
                            else
                                t->set_child(dir2, rotate_double(g, 1 - last));
                        }

                        _inserted = n;
                        break;
                    }
                    else if (is_red(n->get_child(LEFT)) && is_red(n->get_child(RIGHT)))
                    {
                        // Simple red violation: color flip
                        n->set_color(RED);
                        n->get_child(LEFT)->set_color(BLACK);
                        n->get_child(RIGHT)->set_color(BLACK);
                    }

                    if (is_red(n) && is_red(p))
                    {
                        // Hard red violation: rotations necessary
                        const s32 dir2 = (t->get_child(RIGHT) == g) ? 1 : 0;

                        if (n == p->get_child(last))
                            t->set_child(dir2, rotate_single(g, 1 - last));
                        else
                            t->set_child(dir2, rotate_double(g, 1 - last));
                    }

                    // This check disallows duplicates in the tree
                    last = dir;
                    dir  = comparer(key, n);
                    if (dir == 0)
                        break;
                    dir = ((dir + 1) >> 1);

                    // Move the helpers down
                    if (g != nullptr)
                        t = g;

                    g = p;
                    p = n;
                    n = n->get_child(dir);
                }

                // Update the root (it may be different)
                root = head->get_child(RIGHT);
            }

            // Make the root black for simplified logic
            root->set_color(BLACK);

            return _inserted != nullptr;
        }

        bool rb_clear(node_t*& root, node_t*& removed_node)
        {
            removed_node = nullptr;

            node_t* node = root;
            if (node == nullptr)
                return true;

            node_t* todelete = node;

            if (node->get_child(LEFT) == nullptr)
            {
                root = node->get_child(RIGHT);
            }
            else if (node->get_child(RIGHT) == nullptr)
            {
                root = node->get_child(LEFT);
            }
            else
            {
                // We have left and right branches
                // Take right branch and place it
                // somewhere down the left branch
                node_t* branch = node->get_child(RIGHT);
                node->set_child(RIGHT, nullptr);

                // Find a node in the left branch that does not
                // have both a left and right branch and place
                // our branch there.
                node_t* iter = node->get_child(LEFT);
                while (iter->get_child(LEFT) != nullptr && iter->get_child(RIGHT) != nullptr)
                {
                    iter = iter->get_child(LEFT);
                }
                if (iter->get_child(LEFT) == nullptr)
                {
                    iter->set_child(LEFT, branch);
                }
                else if (iter->get_child(RIGHT) == nullptr)
                {
                    iter->set_child(RIGHT, branch);
                }

                root = node->get_child(LEFT);
            }

            removed_node = todelete;
            return false;
        }

        bool clear(node_t*& root, node_t*& n)
        {
            node_t* node   = nullptr;
            bool    result = rb_clear(root, node);
            n              = node;
            return result;
        }

        bool find(node_t* root, void const* key, compare_key_and_node_fn comparer, node_t*& found)
        {
            node_t* node = root;
            while (node != nullptr)
            {
                const s8 c = comparer(key, node);
                if (c == 0)
                {
                    found = node;
                    return true;
                }
                node = node->get_child((c + 1) >> 1);
            }
            found = nullptr;
            return false;
        }

        // validate the tree (return violation description in 'result'), also returns black height
        static s32 rb_validate(node_t const* root, const char*& result, compare_node_and_node_fn comparer)
        {
            if (root == nullptr)
            {
                return 1;
            }
            else
            {
                node_t* ln = root->get_child(LEFT);
                node_t* rn = root->get_child(RIGHT);

                // Consecutive red links
                if (is_red(root))
                {
                    if (is_red(ln) || is_red(rn))
                    {
                        result = "Red violation";
                        return 0;
                    }
                }

                s8 lh = rb_validate(ln, result, comparer);
                s8 rh = rb_validate(rn, result, comparer);

                // Invalid binary search tree
                if ((ln != nullptr && comparer(ln, root) >= 0) || (rn != nullptr && comparer(rn, root) <= 0))
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
                    return is_red(root) ? lh : lh + 1;
                }
            }
            return 0;
        }

        bool remove(node_t*& root, node_t* temp, void const* key, compare_key_and_node_fn comparer, node_t*& out_removed)
        {
            if (root == nullptr)
                return false;

            node_t* head = temp;     // False tree root
            node_t* fn   = nullptr;  // Found node
            node_t* fp   = nullptr;  // Found node parent
            s8      dir  = 1;

            // Set up our helpers
            node_t* g = nullptr;
            node_t* p = nullptr;

            node_t* n = head;
            n->set_color(BLACK);  // Color it black
            n->set_child(RIGHT, root);
            n->set_child(LEFT, nullptr);

            // Search and push a red node down
            // to fix red violations as we go
            while (n->get_child(dir) != nullptr)
            {
                const s8 last = dir;

                // Move the helpers down
                g   = p;
                p   = n;
                n   = n->get_child(dir);
                dir = comparer(key, n);

                // Save the node with matching data and keep
                // going; we'll do removal tasks at the end
                if (dir == 0)
                {
                    fn = n;
                    fp = p;
                }
                dir = ((dir + 1) >> 1);

                /* Push the red node down with rotations and color flips */
                if (!is_red(n) && !is_red(n->get_child(dir)))
                {
                    if (is_red(n->get_child(1 - dir)))
                    {
                        node_t* r = rotate_single_track_parent(n, dir, fn, fp);
                        p->set_child(last, r);
                        if (fn == r)  // never triggered
                            fp = p;
                        p = r;
                    }
                    else if (!is_red(n->get_child(1 - dir)))
                    {
                        node_t* s = p->get_child(1 - last);
                        if (s != nullptr)
                        {
                            if (!is_red(s->get_child(1 - last)) && !is_red(s->get_child(last)))
                            {
                                // Color flip
                                p->set_color(BLACK);
                                s->set_color(RED);
                                n->set_color(RED);
                            }
                            else
                            {
                                // const s32 dir2 = g->get_right(tree) == p ? 1 : 0;
                                const s32 dir2 = g->get_child(RIGHT) == p ? 1 : 0;
                                if (is_red(s->get_child(last)))
                                {
                                    node_t* r = rotate_double_track_parent(p, last, fn, fp);
                                    // g-> dir2->set_child( r);
                                    g->set_child(dir2, r);
                                    if (fn == r)  // never triggered
                                        fp = g;
                                }
                                else if (is_red(s->get_child(1 - last)))
                                {
                                    node_t* r = rotate_single_track_parent(p, last, fn, fp);
                                    // g-> dir2->set_child( r);
                                    g->set_child(dir2, r);
                                    if (fn == r)  // never triggered
                                        fp = g;
                                }

                                // Ensure correct coloring
                                n->set_color(RED);
                                g->get_child(dir2)->set_color(RED);

                                // get_child->set_color(g->get_child( dir2), LEFT), BLACK);
                                // get_child->set_color(g->get_child( dir2), RIGHT), BLACK);
                                g->get_child(dir2)->get_child(LEFT)->set_color(BLACK);
                                g->get_child(dir2)->get_child(RIGHT)->set_color(BLACK);
                            }
                        }
                    }
                }
            }

            // Update the root (it may be different)
            root = head->get_child(RIGHT);

            // Replace and remove the saved node
            if (fn != nullptr)
            {
                ASSERT(fp->get_child( RIGHT) == fn || fp->get_child( LEFT) == fn);
                ASSERT(p->get_child(RIGHT) == n || p->get_child(LEFT) == n);

                node_t* child1 = n->get_child(n->get_child(LEFT) == nullptr);
                p->set_child(p->get_child(RIGHT) == n, child1);

                if (fn != n)
                {
                    ASSERT(fp != p);

                    // swap 'n' and 'fn', we want to remove the node that was holding 'item'
                    fp->set_child(fp->get_child( RIGHT) == fn, n);
                    n->set_child(LEFT, fn->get_child( LEFT));
                    n->set_child(RIGHT, fn->get_child( RIGHT));
                    n->set_color(fn->get_color());
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
            else
            {
                out_removed = nullptr;
            }

            // Make the root black for simplified logic
            if (root != nullptr)
                root->set_color(BLACK);

            return true;
        }

        bool validate(node_t const* root, const char*& error_str, compare_node_and_node_fn comparer)
        {
            rb_validate(root, error_str, comparer);
            return error_str == nullptr;
        }

        bool iterator_t::traverse(node_t const* root, s32 d, node_t const*& out_node)
        {
            if (m_it == nullptr)
            {
                m_it = root;
            }
            else
            {
                m_it = m_it->get_child(d);
            }

            if (m_it != nullptr)
            {
                out_node = m_it;
                return true;
            }
            return false;
        }

        bool iterator_t::preorder(node_t const* root, s32 dir, node_t const*& out_node)
        {
            if (m_stack == -1)
            {
                m_stack = 0;
                if (root != nullptr)
                {
                    m_stack_array[m_stack++] = root;
                }
            }

            out_node = nullptr;
            if (m_stack == 0)
                return false;

            node_t const* node = m_stack_array[--m_stack];
            out_node           = node;

            node_t* child1 = node->get_child(1 - dir);
            if (child1 != nullptr)
                m_stack_array[m_stack++] = child1;

            node_t* child2 = node->get_child(dir);
            if (child2 != nullptr)
                m_stack_array[m_stack++] = child2;

            return true;
        }

        bool iterator_t::sortorder(node_t const* root, s32 dir, node_t const*& out_node)
        {
            if (m_stack == -1)
            {
                m_stack = 0;
                m_it    = root;
            }

            while (m_it != nullptr)
            {
                m_stack_array[m_stack++] = m_it;
                m_it                     = m_it->get_child(dir);
            }

            if (m_stack == 0)
                return false;

            m_it     = m_stack_array[--m_stack];
            out_node = m_it;
            m_it     = m_it->get_child(1 - dir);
            return true;
        }

        bool iterator_t::postorder(node_t const* root, s32 dir, node_t const*& out_node)
        {
            if (m_stack == -1)
            {
                m_stack = 0;
                m_it    = root;
                if (m_it != nullptr)
                {
                    m_stack_array[m_stack++] = m_it;
                }
            }

            if (m_stack == 0)
                return false;

            while (true)
            {
                node_t const* const node   = m_stack_array[m_stack - 1];
                node_t const* const child1 = node->get_child(1 - dir);
                node_t const* const child2 = node->get_child(dir);
                if ((child1 == m_it || child2 == m_it) || (child1 == nullptr && child2 == nullptr))
                {
                    m_it     = node;
                    out_node = m_it;
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

    }  // namespace ntree
}  // namespace ncore
