/*
  Red Black Trees
  (C) 1999  Andrea Arcangeli <andrea@suse.de>
  (C) 2002  David Woodhouse <dwmw2@infradead.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

  linux/lib/rbtree.c
*/

#include "rbtree.h"

static void __rb_rotate_left(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *right = node->rb_right;
	struct rb_node *parent = ext2fs_rb_parent(node);

	如果 ((node->rb_right = right->rb_left))
		ext2fs_rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	ext2fs_rb_set_parent(right, parent);

	如果 (parent)
	{
		如果 (node == parent->rb_left)
			parent->rb_left = right;
		否则
			parent->rb_right = right;
	}
	否则
		root->rb_node = right;
	ext2fs_rb_set_parent(node, right);
}

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *left = node->rb_left;
	struct rb_node *parent = ext2fs_rb_parent(node);

	如果 ((node->rb_left = left->rb_right))
		ext2fs_rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	ext2fs_rb_set_parent(left, parent);

	如果 (parent)
	{
		如果 (node == parent->rb_right)
			parent->rb_right = left;
		否则
			parent->rb_left = left;
	}
	否则
		root->rb_node = left;
	ext2fs_rb_set_parent(node, left);
}

void ext2fs_rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;

	当 ((parent = ext2fs_rb_parent(node)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		如果 (parent == gparent->rb_left)
		{
			{
				register struct rb_node *uncle = gparent->rb_right;
				如果 (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			如果 (parent->rb_right == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, root);
		} 否则 {
			{
				register struct rb_node *uncle = gparent->rb_left;
				如果 (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			如果 (parent->rb_left == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, root);
		}
	}

	ext2fs_rb_set_black(root->rb_node);
}

static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
			     struct rb_root *root)
{
	struct rb_node *other;

	当 ((!node || ext2fs_rb_is_black(node)) && node != root->rb_node)
	{
		如果 (parent->rb_left == node)
		{
			other = parent->rb_right;
			如果 (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			如果 ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			否则
			{
				如果 (!other->rb_right || ext2fs_rb_is_black(other->rb_right))
				{
					ext2fs_rb_set_black(other->rb_left);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, root);
					other = parent->rb_right;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_right);
				__rb_rotate_left(parent, root);
				node = root->rb_node;
				break;
			}
		}
		否则
		{
			other = parent->rb_left;
			如果 (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			如果 ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			否则
			{
				如果 (!other->rb_left || ext2fs_rb_is_black(other->rb_left))
				{
					ext2fs_rb_set_black(other->rb_right);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, root);
					other = parent->rb_left;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_left);
				__rb_rotate_right(parent, root);
				node = root->rb_node;
				break;
			}
		}
	}
	如果 (node)
		ext2fs_rb_set_black(node);
}

void ext2fs_rb_erase(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;
	整数 color;

	如果 (!node->rb_left)
		child = node->rb_right;
	否则 如果 (!node->rb_right)
		child = node->rb_left;
	否则
	{
		struct rb_node *old = node, *left;

		node = node->rb_right;
		当 ((left = node->rb_left) != NULL)
			node = left;

		如果 (ext2fs_rb_parent(old)) {
			如果 (ext2fs_rb_parent(old)->rb_left == old)
				ext2fs_rb_parent(old)->rb_left = node;
			否则
				ext2fs_rb_parent(old)->rb_right = node;
		} 否则
			root->rb_node = node;

		child = node->rb_right;
		parent = ext2fs_rb_parent(node);
		color = ext2fs_rb_color(node);

		如果 (parent == old) {
			parent = node;
		} 否则 {
			如果 (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_left = child;

			node->rb_right = old->rb_right;
			ext2fs_rb_set_parent(old->rb_right, node);
		}

		node->rb_parent_color = old->rb_parent_color;
		node->rb_left = old->rb_left;
		ext2fs_rb_set_parent(old->rb_left, node);

		goto color;
	}

	parent = ext2fs_rb_parent(node);
	color = ext2fs_rb_color(node);

	如果 (child)
		ext2fs_rb_set_parent(child, parent);
	如果 (parent)
	{
		如果 (parent->rb_left == node)
			parent->rb_left = child;
		否则
			parent->rb_right = child;
	}
	否则
		root->rb_node = child;

 color:
	如果 (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
struct rb_node *ext2fs_rb_first(const struct rb_root *root)
{
	struct rb_node	*n;

	n = root->rb_node;
	如果 (!n)
		返回 NULL;
	当 (n->rb_left)
		n = n->rb_left;
	返回 n;
}

struct rb_node *ext2fs_rb_last(const struct rb_root *root)
{
	struct rb_node	*n;

	n = root->rb_node;
	如果 (!n)
		返回 NULL;
	当 (n->rb_right)
		n = n->rb_right;
	返回 n;
}

struct rb_node *ext2fs_rb_next(struct rb_node *node)
{
	struct rb_node *parent;

	如果 (ext2fs_rb_parent(node) == node)
		返回 NULL;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	如果 (node->rb_right) {
		node = node->rb_right;
		当 (node->rb_left)
			node=node->rb_left;
		返回 (struct rb_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	当 ((parent = ext2fs_rb_parent(node)) && node == parent->rb_right)
		node = parent;

	返回 parent;
}

struct rb_node *ext2fs_rb_prev(struct rb_node *node)
{
	struct rb_node *parent;

	如果 (ext2fs_rb_parent(node) == node)
		返回 NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	如果 (node->rb_left) {
		node = node->rb_left;
		当 (node->rb_right)
			node=node->rb_right;
		返回 (struct rb_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	当 ((parent = ext2fs_rb_parent(node)) && node == parent->rb_left)
		node = parent;

	返回 parent;
}

void ext2fs_rb_replace_node(struct rb_node *victim, struct rb_node *new,
			  struct rb_root *root)
{
	struct rb_node *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	如果 (parent) {
		如果 (victim == parent->rb_left)
			parent->rb_left = new;
		否则
			parent->rb_right = new;
	} 否则 {
		root->rb_node = new;
	}
	如果 (victim->rb_left)
		ext2fs_rb_set_parent(victim->rb_left, new);
	如果 (victim->rb_right)
		ext2fs_rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
