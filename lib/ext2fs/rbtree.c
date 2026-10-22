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

ساكن فراغ __rb_rotate_left(بنية rb_كلي *كلي, بنية rb_سيو *سيو)
{
	بنية rb_كلي *دام = كلي->rb_دام;
	بنية rb_كلي *parent = ext2fs_rb_parent(كلي);

	إذا ((كلي->rb_دام = دام->rb_هغو))
		ext2fs_rb_set_parent(دام->rb_هغو, كلي);
	دام->rb_هغو = كلي;

	ext2fs_rb_set_parent(دام, parent);

	إذا (parent)
	{
		إذا (كلي == parent->rb_هغو)
			parent->rb_هغو = دام;
		وإلا
			parent->rb_دام = دام;
	}
	وإلا
		سيو->rb_كلي = دام;
	ext2fs_rb_set_parent(كلي, دام);
}

ساكن فراغ __rb_rotate_right(بنية rb_كلي *كلي, بنية rb_سيو *سيو)
{
	بنية rb_كلي *هغو = كلي->rb_هغو;
	بنية rb_كلي *parent = ext2fs_rb_parent(كلي);

	إذا ((كلي->rb_هغو = هغو->rb_دام))
		ext2fs_rb_set_parent(هغو->rb_دام, كلي);
	هغو->rb_دام = كلي;

	ext2fs_rb_set_parent(هغو, parent);

	إذا (parent)
	{
		إذا (كلي == parent->rb_دام)
			parent->rb_دام = هغو;
		وإلا
			parent->rb_هغو = هغو;
	}
	وإلا
		سيو->rb_كلي = هغو;
	ext2fs_rb_set_parent(كلي, هغو);
}

فراغ ext2fs_rb_سك_color(بنية rb_كلي *كلي, بنية rb_سيو *سيو)
{
	بنية rb_كلي *parent, *gparent;

	طالما ((parent = ext2fs_rb_parent(كلي)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		إذا (parent == gparent->rb_هغو)
		{
			{
				سجل بنية rb_كلي *uncle = gparent->rb_دام;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					كلي = gparent;
					استمر;
				}
			}

			إذا (parent->rb_دام == كلي)
			{
				سجل بنية rb_كلي *tmp;
				__rb_rotate_left(parent, سيو);
				tmp = parent;
				parent = كلي;
				كلي = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, سيو);
		} وإلا {
			{
				سجل بنية rb_كلي *uncle = gparent->rb_هغو;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					كلي = gparent;
					استمر;
				}
			}

			إذا (parent->rb_هغو == كلي)
			{
				سجل بنية rb_كلي *tmp;
				__rb_rotate_right(parent, سيو);
				tmp = parent;
				parent = كلي;
				كلي = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, سيو);
		}
	}

	ext2fs_rb_set_black(سيو->rb_كلي);
}

ساكن فراغ __rb_erase_color(بنية rb_كلي *كلي, بنية rb_كلي *parent,
			     بنية rb_سيو *سيو)
{
	بنية rb_كلي *other;

	طالما ((!كلي || ext2fs_rb_is_black(كلي)) && كلي != سيو->rb_كلي)
	{
		إذا (parent->rb_هغو == كلي)
		{
			other = parent->rb_دام;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, سيو);
				other = parent->rb_دام;
			}
			إذا ((!other->rb_هغو || ext2fs_rb_is_black(other->rb_هغو)) &&
			    (!other->rb_دام || ext2fs_rb_is_black(other->rb_دام)))
			{
				ext2fs_rb_set_red(other);
				كلي = parent;
				parent = ext2fs_rb_parent(كلي);
			}
			وإلا
			{
				إذا (!other->rb_دام || ext2fs_rb_is_black(other->rb_دام))
				{
					ext2fs_rb_set_black(other->rb_هغو);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, سيو);
					other = parent->rb_دام;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_دام);
				__rb_rotate_left(parent, سيو);
				كلي = سيو->rb_كلي;
				اكسر;
			}
		}
		وإلا
		{
			other = parent->rb_هغو;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, سيو);
				other = parent->rb_هغو;
			}
			إذا ((!other->rb_هغو || ext2fs_rb_is_black(other->rb_هغو)) &&
			    (!other->rb_دام || ext2fs_rb_is_black(other->rb_دام)))
			{
				ext2fs_rb_set_red(other);
				كلي = parent;
				parent = ext2fs_rb_parent(كلي);
			}
			وإلا
			{
				إذا (!other->rb_هغو || ext2fs_rb_is_black(other->rb_هغو))
				{
					ext2fs_rb_set_black(other->rb_دام);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, سيو);
					other = parent->rb_هغو;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_هغو);
				__rb_rotate_right(parent, سيو);
				كلي = سيو->rb_كلي;
				اكسر;
			}
		}
	}
	إذا (كلي)
		ext2fs_rb_set_black(كلي);
}

فراغ ext2fs_rb_erase(بنية rb_كلي *كلي, بنية rb_سيو *سيو)
{
	بنية rb_كلي *child, *parent;
	صحيح color;

	إذا (!كلي->rb_هغو)
		child = كلي->rb_دام;
	وإلا إذا (!كلي->rb_دام)
		child = كلي->rb_هغو;
	وإلا
	{
		بنية rb_كلي *old = كلي, *هغو;

		كلي = كلي->rb_دام;
		طالما ((هغو = كلي->rb_هغو) != عدم)
			كلي = هغو;

		إذا (ext2fs_rb_parent(old)) {
			إذا (ext2fs_rb_parent(old)->rb_هغو == old)
				ext2fs_rb_parent(old)->rb_هغو = كلي;
			وإلا
				ext2fs_rb_parent(old)->rb_دام = كلي;
		} وإلا
			سيو->rb_كلي = كلي;

		child = كلي->rb_دام;
		parent = ext2fs_rb_parent(كلي);
		color = ext2fs_rb_color(كلي);

		إذا (parent == old) {
			parent = كلي;
		} وإلا {
			إذا (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_هغو = child;

			كلي->rb_دام = old->rb_دام;
			ext2fs_rb_set_parent(old->rb_دام, كلي);
		}

		كلي->rb_parent_color = old->rb_parent_color;
		كلي->rb_هغو = old->rb_هغو;
		ext2fs_rb_set_parent(old->rb_هغو, كلي);

		اذهب color;
	}

	parent = ext2fs_rb_parent(كلي);
	color = ext2fs_rb_color(كلي);

	إذا (child)
		ext2fs_rb_set_parent(child, parent);
	إذا (parent)
	{
		إذا (parent->rb_هغو == كلي)
			parent->rb_هغو = child;
		وإلا
			parent->rb_دام = child;
	}
	وإلا
		سيو->rb_كلي = child;

 color:
	إذا (color == RB_BLACK)
		__rb_erase_color(child, parent, سيو);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_كلي *ext2fs_rb_first(ثابت بنية rb_سيو *سيو)
{
	بنية rb_كلي	*n;

	n = سيو->rb_كلي;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_هغو)
		n = n->rb_هغو;
	أرجع n;
}

بنية rb_كلي *ext2fs_rb_last(ثابت بنية rb_سيو *سيو)
{
	بنية rb_كلي	*n;

	n = سيو->rb_كلي;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_دام)
		n = n->rb_دام;
	أرجع n;
}

بنية rb_كلي *ext2fs_rb_next(بنية rb_كلي *كلي)
{
	بنية rb_كلي *parent;

	إذا (ext2fs_rb_parent(كلي) == كلي)
		أرجع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	إذا (كلي->rb_دام) {
		كلي = كلي->rb_دام;
		طالما (كلي->rb_هغو)
			كلي=كلي->rb_هغو;
		أرجع (بنية rb_كلي *)كلي;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	طالما ((parent = ext2fs_rb_parent(كلي)) && كلي == parent->rb_دام)
		كلي = parent;

	أرجع parent;
}

بنية rb_كلي *ext2fs_rb_prev(بنية rb_كلي *كلي)
{
	بنية rb_كلي *parent;

	إذا (ext2fs_rb_parent(كلي) == كلي)
		أرجع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	إذا (كلي->rb_هغو) {
		كلي = كلي->rb_هغو;
		طالما (كلي->rb_دام)
			كلي=كلي->rb_دام;
		أرجع (بنية rb_كلي *)كلي;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	طالما ((parent = ext2fs_rb_parent(كلي)) && كلي == parent->rb_هغو)
		كلي = parent;

	أرجع parent;
}

فراغ ext2fs_rb_replace_كلي(بنية rb_كلي *victim, بنية rb_كلي *new,
			  بنية rb_سيو *سيو)
{
	بنية rb_كلي *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	إذا (parent) {
		إذا (victim == parent->rb_هغو)
			parent->rb_هغو = new;
		وإلا
			parent->rb_دام = new;
	} وإلا {
		سيو->rb_كلي = new;
	}
	إذا (victim->rb_هغو)
		ext2fs_rb_set_parent(victim->rb_هغو, new);
	إذا (victim->rb_دام)
		ext2fs_rb_set_parent(victim->rb_دام, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
