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

ساكن فراغ __rb_rotate_left(بنية rb_نود *نود, بنية rb_اکر *اکر)
{
	بنية rb_نود *کانن = نود->rb_کانن;
	بنية rb_نود *parent = ext2fs_rb_parent(نود);

	إذا ((نود->rb_کانن = کانن->rb_کيري))
		ext2fs_rb_set_parent(کانن->rb_کيري, نود);
	کانن->rb_کيري = نود;

	ext2fs_rb_set_parent(کانن, parent);

	إذا (parent)
	{
		إذا (نود == parent->rb_کيري)
			parent->rb_کيري = کانن;
		وإلا
			parent->rb_کانن = کانن;
	}
	وإلا
		اکر->rb_نود = کانن;
	ext2fs_rb_set_parent(نود, کانن);
}

ساكن فراغ __rb_rotate_right(بنية rb_نود *نود, بنية rb_اکر *اکر)
{
	بنية rb_نود *کيري = نود->rb_کيري;
	بنية rb_نود *parent = ext2fs_rb_parent(نود);

	إذا ((نود->rb_کيري = کيري->rb_کانن))
		ext2fs_rb_set_parent(کيري->rb_کانن, نود);
	کيري->rb_کانن = نود;

	ext2fs_rb_set_parent(کيري, parent);

	إذا (parent)
	{
		إذا (نود == parent->rb_کانن)
			parent->rb_کانن = کيري;
		وإلا
			parent->rb_کيري = کيري;
	}
	وإلا
		اکر->rb_نود = کيري;
	ext2fs_rb_set_parent(نود, کيري);
}

فراغ ext2fs_rb_سيسيڤ_color(بنية rb_نود *نود, بنية rb_اکر *اکر)
{
	بنية rb_نود *parent, *gparent;

	طالما ((parent = ext2fs_rb_parent(نود)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		إذا (parent == gparent->rb_کيري)
		{
			{
				سجل بنية rb_نود *uncle = gparent->rb_کانن;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					نود = gparent;
					استمر;
				}
			}

			إذا (parent->rb_کانن == نود)
			{
				سجل بنية rb_نود *tmp;
				__rb_rotate_left(parent, اکر);
				tmp = parent;
				parent = نود;
				نود = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, اکر);
		} وإلا {
			{
				سجل بنية rb_نود *uncle = gparent->rb_کيري;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					نود = gparent;
					استمر;
				}
			}

			إذا (parent->rb_کيري == نود)
			{
				سجل بنية rb_نود *tmp;
				__rb_rotate_right(parent, اکر);
				tmp = parent;
				parent = نود;
				نود = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, اکر);
		}
	}

	ext2fs_rb_set_black(اکر->rb_نود);
}

ساكن فراغ __rb_erase_color(بنية rb_نود *نود, بنية rb_نود *parent,
			     بنية rb_اکر *اکر)
{
	بنية rb_نود *other;

	طالما ((!نود || ext2fs_rb_is_black(نود)) && نود != اکر->rb_نود)
	{
		إذا (parent->rb_کيري == نود)
		{
			other = parent->rb_کانن;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, اکر);
				other = parent->rb_کانن;
			}
			إذا ((!other->rb_کيري || ext2fs_rb_is_black(other->rb_کيري)) &&
			    (!other->rb_کانن || ext2fs_rb_is_black(other->rb_کانن)))
			{
				ext2fs_rb_set_red(other);
				نود = parent;
				parent = ext2fs_rb_parent(نود);
			}
			وإلا
			{
				إذا (!other->rb_کانن || ext2fs_rb_is_black(other->rb_کانن))
				{
					ext2fs_rb_set_black(other->rb_کيري);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, اکر);
					other = parent->rb_کانن;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_کانن);
				__rb_rotate_left(parent, اکر);
				نود = اکر->rb_نود;
				اكسر;
			}
		}
		وإلا
		{
			other = parent->rb_کيري;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, اکر);
				other = parent->rb_کيري;
			}
			إذا ((!other->rb_کيري || ext2fs_rb_is_black(other->rb_کيري)) &&
			    (!other->rb_کانن || ext2fs_rb_is_black(other->rb_کانن)))
			{
				ext2fs_rb_set_red(other);
				نود = parent;
				parent = ext2fs_rb_parent(نود);
			}
			وإلا
			{
				إذا (!other->rb_کيري || ext2fs_rb_is_black(other->rb_کيري))
				{
					ext2fs_rb_set_black(other->rb_کانن);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, اکر);
					other = parent->rb_کيري;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_کيري);
				__rb_rotate_right(parent, اکر);
				نود = اکر->rb_نود;
				اكسر;
			}
		}
	}
	إذا (نود)
		ext2fs_rb_set_black(نود);
}

فراغ ext2fs_rb_erase(بنية rb_نود *نود, بنية rb_اکر *اکر)
{
	بنية rb_نود *child, *parent;
	صحيح color;

	إذا (!نود->rb_کيري)
		child = نود->rb_کانن;
	وإلا إذا (!نود->rb_کانن)
		child = نود->rb_کيري;
	وإلا
	{
		بنية rb_نود *old = نود, *کيري;

		نود = نود->rb_کانن;
		طالما ((کيري = نود->rb_کيري) != عدم)
			نود = کيري;

		إذا (ext2fs_rb_parent(old)) {
			إذا (ext2fs_rb_parent(old)->rb_کيري == old)
				ext2fs_rb_parent(old)->rb_کيري = نود;
			وإلا
				ext2fs_rb_parent(old)->rb_کانن = نود;
		} وإلا
			اکر->rb_نود = نود;

		child = نود->rb_کانن;
		parent = ext2fs_rb_parent(نود);
		color = ext2fs_rb_color(نود);

		إذا (parent == old) {
			parent = نود;
		} وإلا {
			إذا (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_کيري = child;

			نود->rb_کانن = old->rb_کانن;
			ext2fs_rb_set_parent(old->rb_کانن, نود);
		}

		نود->rb_parent_color = old->rb_parent_color;
		نود->rb_کيري = old->rb_کيري;
		ext2fs_rb_set_parent(old->rb_کيري, نود);

		اذهب color;
	}

	parent = ext2fs_rb_parent(نود);
	color = ext2fs_rb_color(نود);

	إذا (child)
		ext2fs_rb_set_parent(child, parent);
	إذا (parent)
	{
		إذا (parent->rb_کيري == نود)
			parent->rb_کيري = child;
		وإلا
			parent->rb_کانن = child;
	}
	وإلا
		اکر->rb_نود = child;

 color:
	إذا (color == RB_BLACK)
		__rb_erase_color(child, parent, اکر);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_نود *ext2fs_rb_first(ثابت بنية rb_اکر *اکر)
{
	بنية rb_نود	*n;

	n = اکر->rb_نود;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_کيري)
		n = n->rb_کيري;
	أرجع n;
}

بنية rb_نود *ext2fs_rb_last(ثابت بنية rb_اکر *اکر)
{
	بنية rb_نود	*n;

	n = اکر->rb_نود;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_کانن)
		n = n->rb_کانن;
	أرجع n;
}

بنية rb_نود *ext2fs_rb_next(بنية rb_نود *نود)
{
	بنية rb_نود *parent;

	إذا (ext2fs_rb_parent(نود) == نود)
		أرجع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	إذا (نود->rb_کانن) {
		نود = نود->rb_کانن;
		طالما (نود->rb_کيري)
			نود=نود->rb_کيري;
		أرجع (بنية rb_نود *)نود;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	طالما ((parent = ext2fs_rb_parent(نود)) && نود == parent->rb_کانن)
		نود = parent;

	أرجع parent;
}

بنية rb_نود *ext2fs_rb_prev(بنية rb_نود *نود)
{
	بنية rb_نود *parent;

	إذا (ext2fs_rb_parent(نود) == نود)
		أرجع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	إذا (نود->rb_کيري) {
		نود = نود->rb_کيري;
		طالما (نود->rb_کانن)
			نود=نود->rb_کانن;
		أرجع (بنية rb_نود *)نود;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	طالما ((parent = ext2fs_rb_parent(نود)) && نود == parent->rb_کيري)
		نود = parent;

	أرجع parent;
}

فراغ ext2fs_rb_replace_نود(بنية rb_نود *victim, بنية rb_نود *new,
			  بنية rb_اکر *اکر)
{
	بنية rb_نود *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	إذا (parent) {
		إذا (victim == parent->rb_کيري)
			parent->rb_کيري = new;
		وإلا
			parent->rb_کانن = new;
	} وإلا {
		اکر->rb_نود = new;
	}
	إذا (victim->rb_کيري)
		ext2fs_rb_set_parent(victim->rb_کيري, new);
	إذا (victim->rb_کانن)
		ext2fs_rb_set_parent(victim->rb_کانن, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
