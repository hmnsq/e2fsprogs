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

	𐤀𐤌 ((node->rb_right = right->rb_left))
		ext2fs_rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	ext2fs_rb_set_parent(right, parent);

	𐤀𐤌 (parent)
	{
		𐤀𐤌 (node == parent->rb_left)
			parent->rb_left = right;
		وإلا
			parent->rb_right = right;
	}
	وإلا
		root->rb_node = right;
	ext2fs_rb_set_parent(node, right);
}

ساكن فراغ __rb_rotate_right(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *left = node->rb_left;
	بنية rb_node *parent = ext2fs_rb_parent(node);

	𐤀𐤌 ((node->rb_left = left->rb_right))
		ext2fs_rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	ext2fs_rb_set_parent(left, parent);

	𐤀𐤌 (parent)
	{
		𐤀𐤌 (node == parent->rb_right)
			parent->rb_right = left;
		وإلا
			parent->rb_left = left;
	}
	وإلا
		root->rb_node = left;
	ext2fs_rb_set_parent(node, left);
}

فراغ ext2fs_rb_insert_color(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *parent, *gparent;

	طالما ((parent = ext2fs_rb_parent(node)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		𐤀𐤌 (parent == gparent->rb_left)
		{
			{
				سجل بنية rb_node *uncle = gparent->rb_right;
				𐤀𐤌 (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					استمر;
				}
			}

			𐤀𐤌 (parent->rb_right == node)
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
		} وإلا {
			{
				سجل بنية rb_node *uncle = gparent->rb_left;
				𐤀𐤌 (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					استمر;
				}
			}

			𐤀𐤌 (parent->rb_left == node)
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

	طالما ((!node || ext2fs_rb_is_black(node)) && node != root->rb_node)
	{
		𐤀𐤌 (parent->rb_left == node)
		{
			other = parent->rb_right;
			𐤀𐤌 (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			𐤀𐤌 ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			وإلا
			{
				𐤀𐤌 (!other->rb_right || ext2fs_rb_is_black(other->rb_right))
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
		وإلا
		{
			other = parent->rb_left;
			𐤀𐤌 (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			𐤀𐤌 ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			وإلا
			{
				𐤀𐤌 (!other->rb_left || ext2fs_rb_is_black(other->rb_left))
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
	𐤀𐤌 (node)
		ext2fs_rb_set_black(node);
}

فراغ ext2fs_rb_erase(بنية rb_node *node, بنية rb_root *root)
{
	بنية rb_node *child, *parent;
	صحيح color;

	𐤀𐤌 (!node->rb_left)
		child = node->rb_right;
	وإلا 𐤀𐤌 (!node->rb_right)
		child = node->rb_left;
	وإلا
	{
		بنية rb_node *old = node, *left;

		node = node->rb_right;
		طالما ((left = node->rb_left) != عدم)
			node = left;

		𐤀𐤌 (ext2fs_rb_parent(old)) {
			𐤀𐤌 (ext2fs_rb_parent(old)->rb_left == old)
				ext2fs_rb_parent(old)->rb_left = node;
			وإلا
				ext2fs_rb_parent(old)->rb_right = node;
		} وإلا
			root->rb_node = node;

		child = node->rb_right;
		parent = ext2fs_rb_parent(node);
		color = ext2fs_rb_color(node);

		𐤀𐤌 (parent == old) {
			parent = node;
		} وإلا {
			𐤀𐤌 (child)
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

	𐤀𐤌 (child)
		ext2fs_rb_set_parent(child, parent);
	𐤀𐤌 (parent)
	{
		𐤀𐤌 (parent->rb_left == node)
			parent->rb_left = child;
		وإلا
			parent->rb_right = child;
	}
	وإلا
		root->rb_node = child;

 color:
	𐤀𐤌 (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_node *ext2fs_rb_first(ثابت بنية rb_root *root)
{
	بنية rb_node	*n;

	n = root->rb_node;
	𐤀𐤌 (!n)
		أرجع عدم;
	طالما (n->rb_left)
		n = n->rb_left;
	أرجع n;
}

بنية rb_node *ext2fs_rb_last(ثابت بنية rb_root *root)
{
	بنية rb_node	*n;

	n = root->rb_node;
	𐤀𐤌 (!n)
		أرجع عدم;
	طالما (n->rb_right)
		n = n->rb_right;
	أرجع n;
}

بنية rb_node *ext2fs_rb_next(بنية rb_node *node)
{
	بنية rb_node *parent;

	𐤀𐤌 (ext2fs_rb_parent(node) == node)
		أرجع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	𐤀𐤌 (node->rb_right) {
		node = node->rb_right;
		طالما (node->rb_left)
			node=node->rb_left;
		أرجع (بنية rb_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	طالما ((parent = ext2fs_rb_parent(node)) && node == parent->rb_right)
		node = parent;

	أرجع parent;
}

بنية rb_node *ext2fs_rb_prev(بنية rb_node *node)
{
	بنية rb_node *parent;

	𐤀𐤌 (ext2fs_rb_parent(node) == node)
		أرجع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	𐤀𐤌 (node->rb_left) {
		node = node->rb_left;
		طالما (node->rb_right)
			node=node->rb_right;
		أرجع (بنية rb_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	طالما ((parent = ext2fs_rb_parent(node)) && node == parent->rb_left)
		node = parent;

	أرجع parent;
}

فراغ ext2fs_rb_replace_node(بنية rb_node *victim, بنية rb_node *new,
			  بنية rb_root *root)
{
	بنية rb_node *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	𐤀𐤌 (parent) {
		𐤀𐤌 (victim == parent->rb_left)
			parent->rb_left = new;
		وإلا
			parent->rb_right = new;
	} وإلا {
		root->rb_node = new;
	}
	𐤀𐤌 (victim->rb_left)
		ext2fs_rb_set_parent(victim->rb_left, new);
	𐤀𐤌 (victim->rb_right)
		ext2fs_rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
