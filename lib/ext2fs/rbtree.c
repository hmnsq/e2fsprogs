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

	якщо ((node->rb_right = right->rb_left))
		ext2fs_rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	ext2fs_rb_set_parent(right, parent);

	якщо (parent)
	{
		якщо (node == parent->rb_left)
			parent->rb_left = right;
		інакше
			parent->rb_right = right;
	}
	інакше
		root->rb_node = right;
	ext2fs_rb_set_parent(node, right);
}

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *left = node->rb_left;
	struct rb_node *parent = ext2fs_rb_parent(node);

	якщо ((node->rb_left = left->rb_right))
		ext2fs_rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	ext2fs_rb_set_parent(left, parent);

	якщо (parent)
	{
		якщо (node == parent->rb_right)
			parent->rb_right = left;
		інакше
			parent->rb_left = left;
	}
	інакше
		root->rb_node = left;
	ext2fs_rb_set_parent(node, left);
}

void ext2fs_rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;

	поки ((parent = ext2fs_rb_parent(node)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		якщо (parent == gparent->rb_left)
		{
			{
				register struct rb_node *uncle = gparent->rb_right;
				якщо (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			якщо (parent->rb_right == node)
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
		} інакше {
			{
				register struct rb_node *uncle = gparent->rb_left;
				якщо (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			якщо (parent->rb_left == node)
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

	поки ((!node || ext2fs_rb_is_black(node)) && node != root->rb_node)
	{
		якщо (parent->rb_left == node)
		{
			other = parent->rb_right;
			якщо (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			якщо ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			інакше
			{
				якщо (!other->rb_right || ext2fs_rb_is_black(other->rb_right))
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
		інакше
		{
			other = parent->rb_left;
			якщо (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			якщо ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			інакше
			{
				якщо (!other->rb_left || ext2fs_rb_is_black(other->rb_left))
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
	якщо (node)
		ext2fs_rb_set_black(node);
}

void ext2fs_rb_erase(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;
	ціле color;

	якщо (!node->rb_left)
		child = node->rb_right;
	інакше якщо (!node->rb_right)
		child = node->rb_left;
	інакше
	{
		struct rb_node *old = node, *left;

		node = node->rb_right;
		поки ((left = node->rb_left) != NULL)
			node = left;

		якщо (ext2fs_rb_parent(old)) {
			якщо (ext2fs_rb_parent(old)->rb_left == old)
				ext2fs_rb_parent(old)->rb_left = node;
			інакше
				ext2fs_rb_parent(old)->rb_right = node;
		} інакше
			root->rb_node = node;

		child = node->rb_right;
		parent = ext2fs_rb_parent(node);
		color = ext2fs_rb_color(node);

		якщо (parent == old) {
			parent = node;
		} інакше {
			якщо (child)
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

	якщо (child)
		ext2fs_rb_set_parent(child, parent);
	якщо (parent)
	{
		якщо (parent->rb_left == node)
			parent->rb_left = child;
		інакше
			parent->rb_right = child;
	}
	інакше
		root->rb_node = child;

 color:
	якщо (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
struct rb_node *ext2fs_rb_first(const struct rb_root *root)
{
	struct rb_node	*n;

	n = root->rb_node;
	якщо (!n)
		повернути NULL;
	поки (n->rb_left)
		n = n->rb_left;
	повернути n;
}

struct rb_node *ext2fs_rb_last(const struct rb_root *root)
{
	struct rb_node	*n;

	n = root->rb_node;
	якщо (!n)
		повернути NULL;
	поки (n->rb_right)
		n = n->rb_right;
	повернути n;
}

struct rb_node *ext2fs_rb_next(struct rb_node *node)
{
	struct rb_node *parent;

	якщо (ext2fs_rb_parent(node) == node)
		повернути NULL;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	якщо (node->rb_right) {
		node = node->rb_right;
		поки (node->rb_left)
			node=node->rb_left;
		повернути (struct rb_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	поки ((parent = ext2fs_rb_parent(node)) && node == parent->rb_right)
		node = parent;

	повернути parent;
}

struct rb_node *ext2fs_rb_prev(struct rb_node *node)
{
	struct rb_node *parent;

	якщо (ext2fs_rb_parent(node) == node)
		повернути NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	якщо (node->rb_left) {
		node = node->rb_left;
		поки (node->rb_right)
			node=node->rb_right;
		повернути (struct rb_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	поки ((parent = ext2fs_rb_parent(node)) && node == parent->rb_left)
		node = parent;

	повернути parent;
}

void ext2fs_rb_replace_node(struct rb_node *victim, struct rb_node *new,
			  struct rb_root *root)
{
	struct rb_node *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	якщо (parent) {
		якщо (victim == parent->rb_left)
			parent->rb_left = new;
		інакше
			parent->rb_right = new;
	} інакше {
		root->rb_node = new;
	}
	якщо (victim->rb_left)
		ext2fs_rb_set_parent(victim->rb_left, new);
	якщо (victim->rb_right)
		ext2fs_rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
