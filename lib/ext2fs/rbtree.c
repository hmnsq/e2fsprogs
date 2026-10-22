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

static void __rb_rotate_left(sa_nracanaa rb_node *node, sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *right = node->rb_right;
	sa_nracanaa rb_node *parent = ext2fs_rb_parent(node);

	ᬬᬾᬦ᭄ ((node->rb_right = right->rb_left))
		ext2fs_rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	ext2fs_rb_set_parent(right, parent);

	ᬬᬾᬦ᭄ (parent)
	{
		ᬬᬾᬦ᭄ (node == parent->rb_left)
			parent->rb_left = right;
		ᬮᬶᬬ
			parent->rb_right = right;
	}
	ᬮᬶᬬ
		root->rb_node = right;
	ext2fs_rb_set_parent(node, right);
}

static void __rb_rotate_right(sa_nracanaa rb_node *node, sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *left = node->rb_left;
	sa_nracanaa rb_node *parent = ext2fs_rb_parent(node);

	ᬬᬾᬦ᭄ ((node->rb_left = left->rb_right))
		ext2fs_rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	ext2fs_rb_set_parent(left, parent);

	ᬬᬾᬦ᭄ (parent)
	{
		ᬬᬾᬦ᭄ (node == parent->rb_right)
			parent->rb_right = left;
		ᬮᬶᬬ
			parent->rb_left = left;
	}
	ᬮᬶᬬ
		root->rb_node = left;
	ext2fs_rb_set_parent(node, left);
}

void ext2fs_rb_insert_color(sa_nracanaa rb_node *node, sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *parent, *gparent;

	ᬲᭂᬤᭂᬮ ((parent = ext2fs_rb_parent(node)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		ᬬᬾᬦ᭄ (parent == gparent->rb_left)
		{
			{
				pa_njikaa sa_nracanaa rb_node *uncle = gparent->rb_right;
				ᬬᬾᬦ᭄ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			ᬬᬾᬦ᭄ (parent->rb_right == node)
			{
				pa_njikaa sa_nracanaa rb_node *tmp;
				__rb_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, root);
		} ᬮᬶᬬ {
			{
				pa_njikaa sa_nracanaa rb_node *uncle = gparent->rb_left;
				ᬬᬾᬦ᭄ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			ᬬᬾᬦ᭄ (parent->rb_left == node)
			{
				pa_njikaa sa_nracanaa rb_node *tmp;
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

static void __rb_erase_color(sa_nracanaa rb_node *node, sa_nracanaa rb_node *parent,
			     sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *other;

	ᬲᭂᬤᭂᬮ ((!node || ext2fs_rb_is_black(node)) && node != root->rb_node)
	{
		ᬬᬾᬦ᭄ (parent->rb_left == node)
		{
			other = parent->rb_right;
			ᬬᬾᬦ᭄ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			ᬬᬾᬦ᭄ ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			ᬮᬶᬬ
			{
				ᬬᬾᬦ᭄ (!other->rb_right || ext2fs_rb_is_black(other->rb_right))
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
				bha_nga;
			}
		}
		ᬮᬶᬬ
		{
			other = parent->rb_left;
			ᬬᬾᬦ᭄ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			ᬬᬾᬦ᭄ ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			ᬮᬶᬬ
			{
				ᬬᬾᬦ᭄ (!other->rb_left || ext2fs_rb_is_black(other->rb_left))
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
				bha_nga;
			}
		}
	}
	ᬬᬾᬦ᭄ (node)
		ext2fs_rb_set_black(node);
}

void ext2fs_rb_erase(sa_nracanaa rb_node *node, sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *child, *parent;
	puur_naa_nka color;

	ᬬᬾᬦ᭄ (!node->rb_left)
		child = node->rb_right;
	ᬮᬶᬬ ᬬᬾᬦ᭄ (!node->rb_right)
		child = node->rb_left;
	ᬮᬶᬬ
	{
		sa_nracanaa rb_node *old = node, *left;

		node = node->rb_right;
		ᬲᭂᬤᭂᬮ ((left = node->rb_left) != NULL)
			node = left;

		ᬬᬾᬦ᭄ (ext2fs_rb_parent(old)) {
			ᬬᬾᬦ᭄ (ext2fs_rb_parent(old)->rb_left == old)
				ext2fs_rb_parent(old)->rb_left = node;
			ᬮᬶᬬ
				ext2fs_rb_parent(old)->rb_right = node;
		} ᬮᬶᬬ
			root->rb_node = node;

		child = node->rb_right;
		parent = ext2fs_rb_parent(node);
		color = ext2fs_rb_color(node);

		ᬬᬾᬦ᭄ (parent == old) {
			parent = node;
		} ᬮᬶᬬ {
			ᬬᬾᬦ᭄ (child)
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

	ᬬᬾᬦ᭄ (child)
		ext2fs_rb_set_parent(child, parent);
	ᬬᬾᬦ᭄ (parent)
	{
		ᬬᬾᬦ᭄ (parent->rb_left == node)
			parent->rb_left = child;
		ᬮᬶᬬ
			parent->rb_right = child;
	}
	ᬮᬶᬬ
		root->rb_node = child;

 color:
	ᬬᬾᬦ᭄ (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
sa_nracanaa rb_node *ext2fs_rb_first(const sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node	*n;

	n = root->rb_node;
	ᬬᬾᬦ᭄ (!n)
		ᬯᬮᬶ NULL;
	ᬲᭂᬤᭂᬮ (n->rb_left)
		n = n->rb_left;
	ᬯᬮᬶ n;
}

sa_nracanaa rb_node *ext2fs_rb_last(const sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node	*n;

	n = root->rb_node;
	ᬬᬾᬦ᭄ (!n)
		ᬯᬮᬶ NULL;
	ᬲᭂᬤᭂᬮ (n->rb_right)
		n = n->rb_right;
	ᬯᬮᬶ n;
}

sa_nracanaa rb_node *ext2fs_rb_next(sa_nracanaa rb_node *node)
{
	sa_nracanaa rb_node *parent;

	ᬬᬾᬦ᭄ (ext2fs_rb_parent(node) == node)
		ᬯᬮᬶ NULL;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	ᬬᬾᬦ᭄ (node->rb_right) {
		node = node->rb_right;
		ᬲᭂᬤᭂᬮ (node->rb_left)
			node=node->rb_left;
		ᬯᬮᬶ (sa_nracanaa rb_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	ᬲᭂᬤᭂᬮ ((parent = ext2fs_rb_parent(node)) && node == parent->rb_right)
		node = parent;

	ᬯᬮᬶ parent;
}

sa_nracanaa rb_node *ext2fs_rb_prev(sa_nracanaa rb_node *node)
{
	sa_nracanaa rb_node *parent;

	ᬬᬾᬦ᭄ (ext2fs_rb_parent(node) == node)
		ᬯᬮᬶ NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	ᬬᬾᬦ᭄ (node->rb_left) {
		node = node->rb_left;
		ᬲᭂᬤᭂᬮ (node->rb_right)
			node=node->rb_right;
		ᬯᬮᬶ (sa_nracanaa rb_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	ᬲᭂᬤᭂᬮ ((parent = ext2fs_rb_parent(node)) && node == parent->rb_left)
		node = parent;

	ᬯᬮᬶ parent;
}

void ext2fs_rb_replace_node(sa_nracanaa rb_node *victim, sa_nracanaa rb_node *new,
			  sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	ᬬᬾᬦ᭄ (parent) {
		ᬬᬾᬦ᭄ (victim == parent->rb_left)
			parent->rb_left = new;
		ᬮᬶᬬ
			parent->rb_right = new;
	} ᬮᬶᬬ {
		root->rb_node = new;
	}
	ᬬᬾᬦ᭄ (victim->rb_left)
		ext2fs_rb_set_parent(victim->rb_left, new);
	ᬬᬾᬦ᭄ (victim->rb_right)
		ext2fs_rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
