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

	אם ((node->rb_right = right->rb_left))
		ext2fs_rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	ext2fs_rb_set_parent(right, parent);

	אם (parent)
	{
		אם (node == parent->rb_left)
			parent->rb_left = right;
		אחרת
			parent->rb_right = right;
	}
	אחרת
		root->rb_node = right;
	ext2fs_rb_set_parent(node, right);
}

ساكن فراغ __rb_rotate_right(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *left = node->rb_left;
	بنية rb_node *parent = ext2fs_rb_parent(node);

	אם ((node->rb_left = left->rb_right))
		ext2fs_rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	ext2fs_rb_set_parent(left, parent);

	אם (parent)
	{
		אם (node == parent->rb_right)
			parent->rb_right = left;
		אחרת
			parent->rb_left = left;
	}
	אחרת
		root->rb_node = left;
	ext2fs_rb_set_parent(node, left);
}

فراغ ext2fs_rb_insert_color(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *parent, *gparent;

	כלעוד ((parent = ext2fs_rb_parent(node)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		אם (parent == gparent->rb_left)
		{
			{
				سجل بنية rb_node *uncle = gparent->rb_right;
				אם (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					استمر;
				}
			}

			אם (parent->rb_right == node)
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
		} אחרת {
			{
				سجل بنية rb_node *uncle = gparent->rb_left;
				אם (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					استمر;
				}
			}

			אם (parent->rb_left == node)
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

	כלעוד ((!node || ext2fs_rb_is_black(node)) && node != root->rb_node)
	{
		אם (parent->rb_left == node)
		{
			other = parent->rb_right;
			אם (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			אם ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			אחרת
			{
				אם (!other->rb_right || ext2fs_rb_is_black(other->rb_right))
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
		אחרת
		{
			other = parent->rb_left;
			אם (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			אם ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			אחרת
			{
				אם (!other->rb_left || ext2fs_rb_is_black(other->rb_left))
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
	אם (node)
		ext2fs_rb_set_black(node);
}

فراغ ext2fs_rb_erase(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *child, *parent;
	صحيح color;

	אם (!node->rb_left)
		child = node->rb_right;
	אחרת אם (!node->rb_right)
		child = node->rb_left;
	אחרת
	{
		بنية rb_node *old = node, *left;

		node = node->rb_right;
		כלעוד ((left = node->rb_left) != عدم)
			node = left;

		אם (ext2fs_rb_parent(old)) {
			אם (ext2fs_rb_parent(old)->rb_left == old)
				ext2fs_rb_parent(old)->rb_left = node;
			אחרת
				ext2fs_rb_parent(old)->rb_right = node;
		} אחרת
			root->rb_node = node;

		child = node->rb_right;
		parent = ext2fs_rb_parent(node);
		color = ext2fs_rb_color(node);

		אם (parent == old) {
			parent = node;
		} אחרת {
			אם (child)
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

	אם (child)
		ext2fs_rb_set_parent(child, parent);
	אם (parent)
	{
		אם (parent->rb_left == node)
			parent->rb_left = child;
		אחרת
			parent->rb_right = child;
	}
	אחרת
		root->rb_node = child;

 color:
	אם (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_node *ext2fs_rb_first(ثابت بنية rb_root *root)
{
	بنية rb_node	*n;

	n = root->rb_node;
	אם (!n)
		החזר عدم;
	כלעוד (n->rb_left)
		n = n->rb_left;
	החזר n;
}

بنية rb_node *ext2fs_rb_last(ثابت بنية rb_root *root)
{
	بنية rb_node	*n;

	n = root->rb_node;
	אם (!n)
		החזר عدم;
	כלעוד (n->rb_right)
		n = n->rb_right;
	החזר n;
}

بنية rb_node *ext2fs_rb_next(بنية rb_node *node)
{
	بنية rb_node *parent;

	אם (ext2fs_rb_parent(node) == node)
		החזר عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	אם (node->rb_right) {
		node = node->rb_right;
		כלעוד (node->rb_left)
			node=node->rb_left;
		החזר (بنية rb_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	כלעוד ((parent = ext2fs_rb_parent(node)) && node == parent->rb_right)
		node = parent;

	החזר parent;
}

بنية rb_node *ext2fs_rb_prev(بنية rb_node *node)
{
	بنية rb_node *parent;

	אם (ext2fs_rb_parent(node) == node)
		החזר عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	אם (node->rb_left) {
		node = node->rb_left;
		כלעוד (node->rb_right)
			node=node->rb_right;
		החזר (بنية rb_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	כלעוד ((parent = ext2fs_rb_parent(node)) && node == parent->rb_left)
		node = parent;

	החזר parent;
}

فراغ ext2fs_rb_replace_node(بنية rb_node *victim, بنية rb_node *new,
			  بنية rb_root *root)
{
	بنية rb_node *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	אם (parent) {
		אם (victim == parent->rb_left)
			parent->rb_left = new;
		אחרת
			parent->rb_right = new;
	} אחרת {
		root->rb_node = new;
	}
	אם (victim->rb_left)
		ext2fs_rb_set_parent(victim->rb_left, new);
	אם (victim->rb_right)
		ext2fs_rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
