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

	ຖ້າ ((node->rb_right = right->rb_left))
		ext2fs_rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	ext2fs_rb_set_parent(right, parent);

	ຖ້າ (parent)
	{
		ຖ້າ (node == parent->rb_left)
			parent->rb_left = right;
		ບໍ່ດັ່ງນັ້ນ
			parent->rb_right = right;
	}
	ບໍ່ດັ່ງນັ້ນ
		root->rb_node = right;
	ext2fs_rb_set_parent(node, right);
}

static void __rb_rotate_right(sa_nracanaa rb_node *node, sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *left = node->rb_left;
	sa_nracanaa rb_node *parent = ext2fs_rb_parent(node);

	ຖ້າ ((node->rb_left = left->rb_right))
		ext2fs_rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	ext2fs_rb_set_parent(left, parent);

	ຖ້າ (parent)
	{
		ຖ້າ (node == parent->rb_right)
			parent->rb_right = left;
		ບໍ່ດັ່ງນັ້ນ
			parent->rb_left = left;
	}
	ບໍ່ດັ່ງນັ້ນ
		root->rb_node = left;
	ext2fs_rb_set_parent(node, left);
}

void ext2fs_rb_insert_color(sa_nracanaa rb_node *node, sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *parent, *gparent;

	ໃນຂະນະທີ່ ((parent = ext2fs_rb_parent(node)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		ຖ້າ (parent == gparent->rb_left)
		{
			{
				pa_njikaa sa_nracanaa rb_node *uncle = gparent->rb_right;
				ຖ້າ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			ຖ້າ (parent->rb_right == node)
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
		} ບໍ່ດັ່ງນັ້ນ {
			{
				pa_njikaa sa_nracanaa rb_node *uncle = gparent->rb_left;
				ຖ້າ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			ຖ້າ (parent->rb_left == node)
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

	ໃນຂະນະທີ່ ((!node || ext2fs_rb_is_black(node)) && node != root->rb_node)
	{
		ຖ້າ (parent->rb_left == node)
		{
			other = parent->rb_right;
			ຖ້າ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			ຖ້າ ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			ບໍ່ດັ່ງນັ້ນ
			{
				ຖ້າ (!other->rb_right || ext2fs_rb_is_black(other->rb_right))
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
		ບໍ່ດັ່ງນັ້ນ
		{
			other = parent->rb_left;
			ຖ້າ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			ຖ້າ ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			ບໍ່ດັ່ງນັ້ນ
			{
				ຖ້າ (!other->rb_left || ext2fs_rb_is_black(other->rb_left))
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
	ຖ້າ (node)
		ext2fs_rb_set_black(node);
}

void ext2fs_rb_erase(sa_nracanaa rb_node *node, sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *child, *parent;
	ຈຳນວນເຕັມ color;

	ຖ້າ (!node->rb_left)
		child = node->rb_right;
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (!node->rb_right)
		child = node->rb_left;
	ບໍ່ດັ່ງນັ້ນ
	{
		sa_nracanaa rb_node *old = node, *left;

		node = node->rb_right;
		ໃນຂະນະທີ່ ((left = node->rb_left) != NULL)
			node = left;

		ຖ້າ (ext2fs_rb_parent(old)) {
			ຖ້າ (ext2fs_rb_parent(old)->rb_left == old)
				ext2fs_rb_parent(old)->rb_left = node;
			ບໍ່ດັ່ງນັ້ນ
				ext2fs_rb_parent(old)->rb_right = node;
		} ບໍ່ດັ່ງນັ້ນ
			root->rb_node = node;

		child = node->rb_right;
		parent = ext2fs_rb_parent(node);
		color = ext2fs_rb_color(node);

		ຖ້າ (parent == old) {
			parent = node;
		} ບໍ່ດັ່ງນັ້ນ {
			ຖ້າ (child)
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

	ຖ້າ (child)
		ext2fs_rb_set_parent(child, parent);
	ຖ້າ (parent)
	{
		ຖ້າ (parent->rb_left == node)
			parent->rb_left = child;
		ບໍ່ດັ່ງນັ້ນ
			parent->rb_right = child;
	}
	ບໍ່ດັ່ງນັ້ນ
		root->rb_node = child;

 color:
	ຖ້າ (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
sa_nracanaa rb_node *ext2fs_rb_first(const sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node	*n;

	n = root->rb_node;
	ຖ້າ (!n)
		ສົ່ງຄືນ NULL;
	ໃນຂະນະທີ່ (n->rb_left)
		n = n->rb_left;
	ສົ່ງຄືນ n;
}

sa_nracanaa rb_node *ext2fs_rb_last(const sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node	*n;

	n = root->rb_node;
	ຖ້າ (!n)
		ສົ່ງຄືນ NULL;
	ໃນຂະນະທີ່ (n->rb_right)
		n = n->rb_right;
	ສົ່ງຄືນ n;
}

sa_nracanaa rb_node *ext2fs_rb_next(sa_nracanaa rb_node *node)
{
	sa_nracanaa rb_node *parent;

	ຖ້າ (ext2fs_rb_parent(node) == node)
		ສົ່ງຄືນ NULL;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	ຖ້າ (node->rb_right) {
		node = node->rb_right;
		ໃນຂະນະທີ່ (node->rb_left)
			node=node->rb_left;
		ສົ່ງຄືນ (sa_nracanaa rb_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	ໃນຂະນະທີ່ ((parent = ext2fs_rb_parent(node)) && node == parent->rb_right)
		node = parent;

	ສົ່ງຄືນ parent;
}

sa_nracanaa rb_node *ext2fs_rb_prev(sa_nracanaa rb_node *node)
{
	sa_nracanaa rb_node *parent;

	ຖ້າ (ext2fs_rb_parent(node) == node)
		ສົ່ງຄືນ NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	ຖ້າ (node->rb_left) {
		node = node->rb_left;
		ໃນຂະນະທີ່ (node->rb_right)
			node=node->rb_right;
		ສົ່ງຄືນ (sa_nracanaa rb_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	ໃນຂະນະທີ່ ((parent = ext2fs_rb_parent(node)) && node == parent->rb_left)
		node = parent;

	ສົ່ງຄືນ parent;
}

void ext2fs_rb_replace_node(sa_nracanaa rb_node *victim, sa_nracanaa rb_node *new,
			  sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	ຖ້າ (parent) {
		ຖ້າ (victim == parent->rb_left)
			parent->rb_left = new;
		ບໍ່ດັ່ງນັ້ນ
			parent->rb_right = new;
	} ບໍ່ດັ່ງນັ້ນ {
		root->rb_node = new;
	}
	ຖ້າ (victim->rb_left)
		ext2fs_rb_set_parent(victim->rb_left, new);
	ຖ້າ (victim->rb_right)
		ext2fs_rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
