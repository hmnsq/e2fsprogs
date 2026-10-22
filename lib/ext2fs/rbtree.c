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

#ضمن "rbtree.h"

ساكن فراغ __rb_rotate_left(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *right = node->rb_right;
	بنية rb_node *parent = ext2fs_rb_parent(node);

	ܐܢ ((node->rb_right = right->rb_left))
		ext2fs_rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	ext2fs_rb_set_parent(right, parent);

	ܐܢ (parent)
	{
		ܐܢ (node == parent->rb_left)
			parent->rb_left = right;
		ܐܚܪܢܐ
			parent->rb_right = right;
	}
	ܐܚܪܢܐ
		root->rb_node = right;
	ext2fs_rb_set_parent(node, right);
}

ساكن فراغ __rb_rotate_right(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *left = node->rb_left;
	بنية rb_node *parent = ext2fs_rb_parent(node);

	ܐܢ ((node->rb_left = left->rb_right))
		ext2fs_rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	ext2fs_rb_set_parent(left, parent);

	ܐܢ (parent)
	{
		ܐܢ (node == parent->rb_right)
			parent->rb_right = left;
		ܐܚܪܢܐ
			parent->rb_left = left;
	}
	ܐܚܪܢܐ
		root->rb_node = left;
	ext2fs_rb_set_parent(node, left);
}

فراغ ext2fs_rb_insert_color(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *parent, *gparent;

	ܟܡܐ ((parent = ext2fs_rb_parent(node)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		ܐܢ (parent == gparent->rb_left)
		{
			{
				سجل بنية rb_node *uncle = gparent->rb_right;
				ܐܢ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					استمر;
				}
			}

			ܐܢ (parent->rb_right == node)
			{
				سجل بنية rb_node *tmp;
				__rb_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, root);
		} ܐܚܪܢܐ {
			{
				سجل بنية rb_node *uncle = gparent->rb_left;
				ܐܢ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					استمر;
				}
			}

			ܐܢ (parent->rb_left == node)
			{
				سجل بنية rb_node *tmp;
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

ساكن فراغ __rb_erase_color(بنية rb_node *node, بنية rb_node *parent,
			     بنية rb_root *root)
{
	بنية rb_node *other;

	ܟܡܐ ((!node || ext2fs_rb_is_black(node)) && node != root->rb_node)
	{
		ܐܢ (parent->rb_left == node)
		{
			other = parent->rb_right;
			ܐܢ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			ܐܢ ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			ܐܚܪܢܐ
			{
				ܐܢ (!other->rb_right || ext2fs_rb_is_black(other->rb_right))
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
				اكسر;
			}
		}
		ܐܚܪܢܐ
		{
			other = parent->rb_left;
			ܐܢ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			ܐܢ ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			ܐܚܪܢܐ
			{
				ܐܢ (!other->rb_left || ext2fs_rb_is_black(other->rb_left))
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
				اكسر;
			}
		}
	}
	ܐܢ (node)
		ext2fs_rb_set_black(node);
}

فراغ ext2fs_rb_erase(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *child, *parent;
	صحيح color;

	ܐܢ (!node->rb_left)
		child = node->rb_right;
	ܐܚܪܢܐ ܐܢ (!node->rb_right)
		child = node->rb_left;
	ܐܚܪܢܐ
	{
		بنية rb_node *old = node, *left;

		node = node->rb_right;
		ܟܡܐ ((left = node->rb_left) != عدم)
			node = left;

		ܐܢ (ext2fs_rb_parent(old)) {
			ܐܢ (ext2fs_rb_parent(old)->rb_left == old)
				ext2fs_rb_parent(old)->rb_left = node;
			ܐܚܪܢܐ
				ext2fs_rb_parent(old)->rb_right = node;
		} ܐܚܪܢܐ
			root->rb_node = node;

		child = node->rb_right;
		parent = ext2fs_rb_parent(node);
		color = ext2fs_rb_color(node);

		ܐܢ (parent == old) {
			parent = node;
		} ܐܚܪܢܐ {
			ܐܢ (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_left = child;

			node->rb_right = old->rb_right;
			ext2fs_rb_set_parent(old->rb_right, node);
		}

		node->rb_parent_color = old->rb_parent_color;
		node->rb_left = old->rb_left;
		ext2fs_rb_set_parent(old->rb_left, node);

		اذهب color;
	}

	parent = ext2fs_rb_parent(node);
	color = ext2fs_rb_color(node);

	ܐܢ (child)
		ext2fs_rb_set_parent(child, parent);
	ܐܢ (parent)
	{
		ܐܢ (parent->rb_left == node)
			parent->rb_left = child;
		ܐܚܪܢܐ
			parent->rb_right = child;
	}
	ܐܚܪܢܐ
		root->rb_node = child;

 color:
	ܐܢ (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_node *ext2fs_rb_first(ثابت بنية rb_root *root)
{
	بنية rb_node	*n;

	n = root->rb_node;
	ܐܢ (!n)
		ܦܢܐ عدم;
	ܟܡܐ (n->rb_left)
		n = n->rb_left;
	ܦܢܐ n;
}

بنية rb_node *ext2fs_rb_last(ثابت بنية rb_root *root)
{
	بنية rb_node	*n;

	n = root->rb_node;
	ܐܢ (!n)
		ܦܢܐ عدم;
	ܟܡܐ (n->rb_right)
		n = n->rb_right;
	ܦܢܐ n;
}

بنية rb_node *ext2fs_rb_next(بنية rb_node *node)
{
	بنية rb_node *parent;

	ܐܢ (ext2fs_rb_parent(node) == node)
		ܦܢܐ عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	ܐܢ (node->rb_right) {
		node = node->rb_right;
		ܟܡܐ (node->rb_left)
			node=node->rb_left;
		ܦܢܐ (بنية rb_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	ܟܡܐ ((parent = ext2fs_rb_parent(node)) && node == parent->rb_right)
		node = parent;

	ܦܢܐ parent;
}

بنية rb_node *ext2fs_rb_prev(بنية rb_node *node)
{
	بنية rb_node *parent;

	ܐܢ (ext2fs_rb_parent(node) == node)
		ܦܢܐ عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	ܐܢ (node->rb_left) {
		node = node->rb_left;
		ܟܡܐ (node->rb_right)
			node=node->rb_right;
		ܦܢܐ (بنية rb_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	ܟܡܐ ((parent = ext2fs_rb_parent(node)) && node == parent->rb_left)
		node = parent;

	ܦܢܐ parent;
}

فراغ ext2fs_rb_replace_node(بنية rb_node *victim, بنية rb_node *new,
			  بنية rb_root *root)
{
	بنية rb_node *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	ܐܢ (parent) {
		ܐܢ (victim == parent->rb_left)
			parent->rb_left = new;
		ܐܚܪܢܐ
			parent->rb_right = new;
	} ܐܚܪܢܐ {
		root->rb_node = new;
	}
	ܐܢ (victim->rb_left)
		ext2fs_rb_set_parent(victim->rb_left, new);
	ܐܢ (victim->rb_right)
		ext2fs_rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
