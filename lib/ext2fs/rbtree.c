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

ساكن فراغ __rb_rotate_left(بنية rb_گرێ *گرێ, بنية rb_ڕەگ *ڕەگ)
{
	بنية rb_گرێ *ڕاست = گرێ->rb_ڕاست;
	بنية rb_گرێ *parent = ext2fs_rb_parent(گرێ);

	إذا ((گرێ->rb_ڕاست = ڕاست->rb_چەپ))
		ext2fs_rb_set_parent(ڕاست->rb_چەپ, گرێ);
	ڕاست->rb_چەپ = گرێ;

	ext2fs_rb_set_parent(ڕاست, parent);

	إذا (parent)
	{
		إذا (گرێ == parent->rb_چەپ)
			parent->rb_چەپ = ڕاست;
		وإلا
			parent->rb_ڕاست = ڕاست;
	}
	وإلا
		ڕەگ->rb_گرێ = ڕاست;
	ext2fs_rb_set_parent(گرێ, ڕاست);
}

ساكن فراغ __rb_rotate_right(بنية rb_گرێ *گرێ, بنية rb_ڕەگ *ڕەگ)
{
	بنية rb_گرێ *چەپ = گرێ->rb_چەپ;
	بنية rb_گرێ *parent = ext2fs_rb_parent(گرێ);

	إذا ((گرێ->rb_چەپ = چەپ->rb_ڕاست))
		ext2fs_rb_set_parent(چەپ->rb_ڕاست, گرێ);
	چەپ->rb_ڕاست = گرێ;

	ext2fs_rb_set_parent(چەپ, parent);

	إذا (parent)
	{
		إذا (گرێ == parent->rb_ڕاست)
			parent->rb_ڕاست = چەپ;
		وإلا
			parent->rb_چەپ = چەپ;
	}
	وإلا
		ڕەگ->rb_گرێ = چەپ;
	ext2fs_rb_set_parent(گرێ, چەپ);
}

فراغ ext2fs_rb_زیادکردن_color(بنية rb_گرێ *گرێ, بنية rb_ڕەگ *ڕەگ)
{
	بنية rb_گرێ *parent, *gparent;

	طالما ((parent = ext2fs_rb_parent(گرێ)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		إذا (parent == gparent->rb_چەپ)
		{
			{
				سجل بنية rb_گرێ *uncle = gparent->rb_ڕاست;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					گرێ = gparent;
					استمر;
				}
			}

			إذا (parent->rb_ڕاست == گرێ)
			{
				سجل بنية rb_گرێ *tmp;
				__rb_rotate_left(parent, ڕەگ);
				tmp = parent;
				parent = گرێ;
				گرێ = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, ڕەگ);
		} وإلا {
			{
				سجل بنية rb_گرێ *uncle = gparent->rb_چەپ;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					گرێ = gparent;
					استمر;
				}
			}

			إذا (parent->rb_چەپ == گرێ)
			{
				سجل بنية rb_گرێ *tmp;
				__rb_rotate_right(parent, ڕەگ);
				tmp = parent;
				parent = گرێ;
				گرێ = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, ڕەگ);
		}
	}

	ext2fs_rb_set_black(ڕەگ->rb_گرێ);
}

ساكن فراغ __rb_erase_color(بنية rb_گرێ *گرێ, بنية rb_گرێ *parent,
			     بنية rb_ڕەگ *ڕەگ)
{
	بنية rb_گرێ *other;

	طالما ((!گرێ || ext2fs_rb_is_black(گرێ)) && گرێ != ڕەگ->rb_گرێ)
	{
		إذا (parent->rb_چەپ == گرێ)
		{
			other = parent->rb_ڕاست;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, ڕەگ);
				other = parent->rb_ڕاست;
			}
			إذا ((!other->rb_چەپ || ext2fs_rb_is_black(other->rb_چەپ)) &&
			    (!other->rb_ڕاست || ext2fs_rb_is_black(other->rb_ڕاست)))
			{
				ext2fs_rb_set_red(other);
				گرێ = parent;
				parent = ext2fs_rb_parent(گرێ);
			}
			وإلا
			{
				إذا (!other->rb_ڕاست || ext2fs_rb_is_black(other->rb_ڕاست))
				{
					ext2fs_rb_set_black(other->rb_چەپ);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, ڕەگ);
					other = parent->rb_ڕاست;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_ڕاست);
				__rb_rotate_left(parent, ڕەگ);
				گرێ = ڕەگ->rb_گرێ;
				اكسر;
			}
		}
		وإلا
		{
			other = parent->rb_چەپ;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, ڕەگ);
				other = parent->rb_چەپ;
			}
			إذا ((!other->rb_چەپ || ext2fs_rb_is_black(other->rb_چەپ)) &&
			    (!other->rb_ڕاست || ext2fs_rb_is_black(other->rb_ڕاست)))
			{
				ext2fs_rb_set_red(other);
				گرێ = parent;
				parent = ext2fs_rb_parent(گرێ);
			}
			وإلا
			{
				إذا (!other->rb_چەپ || ext2fs_rb_is_black(other->rb_چەپ))
				{
					ext2fs_rb_set_black(other->rb_ڕاست);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, ڕەگ);
					other = parent->rb_چەپ;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_چەپ);
				__rb_rotate_right(parent, ڕەگ);
				گرێ = ڕەگ->rb_گرێ;
				اكسر;
			}
		}
	}
	إذا (گرێ)
		ext2fs_rb_set_black(گرێ);
}

فراغ ext2fs_rb_erase(بنية rb_گرێ *گرێ, بنية rb_ڕەگ *ڕەگ)
{
	بنية rb_گرێ *child, *parent;
	صحيح color;

	إذا (!گرێ->rb_چەپ)
		child = گرێ->rb_ڕاست;
	وإلا إذا (!گرێ->rb_ڕاست)
		child = گرێ->rb_چەپ;
	وإلا
	{
		بنية rb_گرێ *old = گرێ, *چەپ;

		گرێ = گرێ->rb_ڕاست;
		طالما ((چەپ = گرێ->rb_چەپ) != عدم)
			گرێ = چەپ;

		إذا (ext2fs_rb_parent(old)) {
			إذا (ext2fs_rb_parent(old)->rb_چەپ == old)
				ext2fs_rb_parent(old)->rb_چەپ = گرێ;
			وإلا
				ext2fs_rb_parent(old)->rb_ڕاست = گرێ;
		} وإلا
			ڕەگ->rb_گرێ = گرێ;

		child = گرێ->rb_ڕاست;
		parent = ext2fs_rb_parent(گرێ);
		color = ext2fs_rb_color(گرێ);

		إذا (parent == old) {
			parent = گرێ;
		} وإلا {
			إذا (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_چەپ = child;

			گرێ->rb_ڕاست = old->rb_ڕاست;
			ext2fs_rb_set_parent(old->rb_ڕاست, گرێ);
		}

		گرێ->rb_parent_color = old->rb_parent_color;
		گرێ->rb_چەپ = old->rb_چەپ;
		ext2fs_rb_set_parent(old->rb_چەپ, گرێ);

		اذهب color;
	}

	parent = ext2fs_rb_parent(گرێ);
	color = ext2fs_rb_color(گرێ);

	إذا (child)
		ext2fs_rb_set_parent(child, parent);
	إذا (parent)
	{
		إذا (parent->rb_چەپ == گرێ)
			parent->rb_چەپ = child;
		وإلا
			parent->rb_ڕاست = child;
	}
	وإلا
		ڕەگ->rb_گرێ = child;

 color:
	إذا (color == RB_BLACK)
		__rb_erase_color(child, parent, ڕەگ);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_گرێ *ext2fs_rb_first(ثابت بنية rb_ڕەگ *ڕەگ)
{
	بنية rb_گرێ	*n;

	n = ڕەگ->rb_گرێ;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_چەپ)
		n = n->rb_چەپ;
	أرجع n;
}

بنية rb_گرێ *ext2fs_rb_last(ثابت بنية rb_ڕەگ *ڕەگ)
{
	بنية rb_گرێ	*n;

	n = ڕەگ->rb_گرێ;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_ڕاست)
		n = n->rb_ڕاست;
	أرجع n;
}

بنية rb_گرێ *ext2fs_rb_next(بنية rb_گرێ *گرێ)
{
	بنية rb_گرێ *parent;

	إذا (ext2fs_rb_parent(گرێ) == گرێ)
		أرجع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	إذا (گرێ->rb_ڕاست) {
		گرێ = گرێ->rb_ڕاست;
		طالما (گرێ->rb_چەپ)
			گرێ=گرێ->rb_چەپ;
		أرجع (بنية rb_گرێ *)گرێ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	طالما ((parent = ext2fs_rb_parent(گرێ)) && گرێ == parent->rb_ڕاست)
		گرێ = parent;

	أرجع parent;
}

بنية rb_گرێ *ext2fs_rb_prev(بنية rb_گرێ *گرێ)
{
	بنية rb_گرێ *parent;

	إذا (ext2fs_rb_parent(گرێ) == گرێ)
		أرجع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	إذا (گرێ->rb_چەپ) {
		گرێ = گرێ->rb_چەپ;
		طالما (گرێ->rb_ڕاست)
			گرێ=گرێ->rb_ڕاست;
		أرجع (بنية rb_گرێ *)گرێ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	طالما ((parent = ext2fs_rb_parent(گرێ)) && گرێ == parent->rb_چەپ)
		گرێ = parent;

	أرجع parent;
}

فراغ ext2fs_rb_replace_گرێ(بنية rb_گرێ *victim, بنية rb_گرێ *new,
			  بنية rb_ڕەگ *ڕەگ)
{
	بنية rb_گرێ *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	إذا (parent) {
		إذا (victim == parent->rb_چەپ)
			parent->rb_چەپ = new;
		وإلا
			parent->rb_ڕاست = new;
	} وإلا {
		ڕەگ->rb_گرێ = new;
	}
	إذا (victim->rb_چەپ)
		ext2fs_rb_set_parent(victim->rb_چەپ, new);
	إذا (victim->rb_ڕاست)
		ext2fs_rb_set_parent(victim->rb_ڕاست, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
