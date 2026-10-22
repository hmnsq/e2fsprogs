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

ساكن فراغ __rb_rotate_left(بنية rb_تۈگۈن *تۈگۈن, بنية rb_يىلتىز *يىلتىز)
{
	بنية rb_تۈگۈن *ئوڭ = تۈگۈن->rb_ئوڭ;
	بنية rb_تۈگۈن *parent = ext2fs_rb_parent(تۈگۈن);

	إذا ((تۈگۈن->rb_ئوڭ = ئوڭ->rb_سول))
		ext2fs_rb_set_parent(ئوڭ->rb_سول, تۈگۈن);
	ئوڭ->rb_سول = تۈگۈن;

	ext2fs_rb_set_parent(ئوڭ, parent);

	إذا (parent)
	{
		إذا (تۈگۈن == parent->rb_سول)
			parent->rb_سول = ئوڭ;
		وإلا
			parent->rb_ئوڭ = ئوڭ;
	}
	وإلا
		يىلتىز->rb_تۈگۈن = ئوڭ;
	ext2fs_rb_set_parent(تۈگۈن, ئوڭ);
}

ساكن فراغ __rb_rotate_right(بنية rb_تۈگۈن *تۈگۈن, بنية rb_يىلتىز *يىلتىز)
{
	بنية rb_تۈگۈن *سول = تۈگۈن->rb_سول;
	بنية rb_تۈگۈن *parent = ext2fs_rb_parent(تۈگۈن);

	إذا ((تۈگۈن->rb_سول = سول->rb_ئوڭ))
		ext2fs_rb_set_parent(سول->rb_ئوڭ, تۈگۈن);
	سول->rb_ئوڭ = تۈگۈن;

	ext2fs_rb_set_parent(سول, parent);

	إذا (parent)
	{
		إذا (تۈگۈن == parent->rb_ئوڭ)
			parent->rb_ئوڭ = سول;
		وإلا
			parent->rb_سول = سول;
	}
	وإلا
		يىلتىز->rb_تۈگۈن = سول;
	ext2fs_rb_set_parent(تۈگۈن, سول);
}

فراغ ext2fs_rb_قىستۇرۇش_color(بنية rb_تۈگۈن *تۈگۈن, بنية rb_يىلتىز *يىلتىز)
{
	بنية rb_تۈگۈن *parent, *gparent;

	طالما ((parent = ext2fs_rb_parent(تۈگۈن)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		إذا (parent == gparent->rb_سول)
		{
			{
				سجل بنية rb_تۈگۈن *uncle = gparent->rb_ئوڭ;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					تۈگۈن = gparent;
					استمر;
				}
			}

			إذا (parent->rb_ئوڭ == تۈگۈن)
			{
				سجل بنية rb_تۈگۈن *tmp;
				__rb_rotate_left(parent, يىلتىز);
				tmp = parent;
				parent = تۈگۈن;
				تۈگۈن = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, يىلتىز);
		} وإلا {
			{
				سجل بنية rb_تۈگۈن *uncle = gparent->rb_سول;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					تۈگۈن = gparent;
					استمر;
				}
			}

			إذا (parent->rb_سول == تۈگۈن)
			{
				سجل بنية rb_تۈگۈن *tmp;
				__rb_rotate_right(parent, يىلتىز);
				tmp = parent;
				parent = تۈگۈن;
				تۈگۈن = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, يىلتىز);
		}
	}

	ext2fs_rb_set_black(يىلتىز->rb_تۈگۈن);
}

ساكن فراغ __rb_erase_color(بنية rb_تۈگۈن *تۈگۈن, بنية rb_تۈگۈن *parent,
			     بنية rb_يىلتىز *يىلتىز)
{
	بنية rb_تۈگۈن *other;

	طالما ((!تۈگۈن || ext2fs_rb_is_black(تۈگۈن)) && تۈگۈن != يىلتىز->rb_تۈگۈن)
	{
		إذا (parent->rb_سول == تۈگۈن)
		{
			other = parent->rb_ئوڭ;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, يىلتىز);
				other = parent->rb_ئوڭ;
			}
			إذا ((!other->rb_سول || ext2fs_rb_is_black(other->rb_سول)) &&
			    (!other->rb_ئوڭ || ext2fs_rb_is_black(other->rb_ئوڭ)))
			{
				ext2fs_rb_set_red(other);
				تۈگۈن = parent;
				parent = ext2fs_rb_parent(تۈگۈن);
			}
			وإلا
			{
				إذا (!other->rb_ئوڭ || ext2fs_rb_is_black(other->rb_ئوڭ))
				{
					ext2fs_rb_set_black(other->rb_سول);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, يىلتىز);
					other = parent->rb_ئوڭ;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_ئوڭ);
				__rb_rotate_left(parent, يىلتىز);
				تۈگۈن = يىلتىز->rb_تۈگۈن;
				اكسر;
			}
		}
		وإلا
		{
			other = parent->rb_سول;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, يىلتىز);
				other = parent->rb_سول;
			}
			إذا ((!other->rb_سول || ext2fs_rb_is_black(other->rb_سول)) &&
			    (!other->rb_ئوڭ || ext2fs_rb_is_black(other->rb_ئوڭ)))
			{
				ext2fs_rb_set_red(other);
				تۈگۈن = parent;
				parent = ext2fs_rb_parent(تۈگۈن);
			}
			وإلا
			{
				إذا (!other->rb_سول || ext2fs_rb_is_black(other->rb_سول))
				{
					ext2fs_rb_set_black(other->rb_ئوڭ);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, يىلتىز);
					other = parent->rb_سول;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_سول);
				__rb_rotate_right(parent, يىلتىز);
				تۈگۈن = يىلتىز->rb_تۈگۈن;
				اكسر;
			}
		}
	}
	إذا (تۈگۈن)
		ext2fs_rb_set_black(تۈگۈن);
}

فراغ ext2fs_rb_erase(بنية rb_تۈگۈن *تۈگۈن, بنية rb_يىلتىز *يىلتىز)
{
	بنية rb_تۈگۈن *child, *parent;
	صحيح color;

	إذا (!تۈگۈن->rb_سول)
		child = تۈگۈن->rb_ئوڭ;
	وإلا إذا (!تۈگۈن->rb_ئوڭ)
		child = تۈگۈن->rb_سول;
	وإلا
	{
		بنية rb_تۈگۈن *old = تۈگۈن, *سول;

		تۈگۈن = تۈگۈن->rb_ئوڭ;
		طالما ((سول = تۈگۈن->rb_سول) != عدم)
			تۈگۈن = سول;

		إذا (ext2fs_rb_parent(old)) {
			إذا (ext2fs_rb_parent(old)->rb_سول == old)
				ext2fs_rb_parent(old)->rb_سول = تۈگۈن;
			وإلا
				ext2fs_rb_parent(old)->rb_ئوڭ = تۈگۈن;
		} وإلا
			يىلتىز->rb_تۈگۈن = تۈگۈن;

		child = تۈگۈن->rb_ئوڭ;
		parent = ext2fs_rb_parent(تۈگۈن);
		color = ext2fs_rb_color(تۈگۈن);

		إذا (parent == old) {
			parent = تۈگۈن;
		} وإلا {
			إذا (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_سول = child;

			تۈگۈن->rb_ئوڭ = old->rb_ئوڭ;
			ext2fs_rb_set_parent(old->rb_ئوڭ, تۈگۈن);
		}

		تۈگۈن->rb_parent_color = old->rb_parent_color;
		تۈگۈن->rb_سول = old->rb_سول;
		ext2fs_rb_set_parent(old->rb_سول, تۈگۈن);

		اذهب color;
	}

	parent = ext2fs_rb_parent(تۈگۈن);
	color = ext2fs_rb_color(تۈگۈن);

	إذا (child)
		ext2fs_rb_set_parent(child, parent);
	إذا (parent)
	{
		إذا (parent->rb_سول == تۈگۈن)
			parent->rb_سول = child;
		وإلا
			parent->rb_ئوڭ = child;
	}
	وإلا
		يىلتىز->rb_تۈگۈن = child;

 color:
	إذا (color == RB_BLACK)
		__rb_erase_color(child, parent, يىلتىز);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_تۈگۈن *ext2fs_rb_first(ثابت بنية rb_يىلتىز *يىلتىز)
{
	بنية rb_تۈگۈن	*n;

	n = يىلتىز->rb_تۈگۈن;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_سول)
		n = n->rb_سول;
	أرجع n;
}

بنية rb_تۈگۈن *ext2fs_rb_last(ثابت بنية rb_يىلتىز *يىلتىز)
{
	بنية rb_تۈگۈن	*n;

	n = يىلتىز->rb_تۈگۈن;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_ئوڭ)
		n = n->rb_ئوڭ;
	أرجع n;
}

بنية rb_تۈگۈن *ext2fs_rb_next(بنية rb_تۈگۈن *تۈگۈن)
{
	بنية rb_تۈگۈن *parent;

	إذا (ext2fs_rb_parent(تۈگۈن) == تۈگۈن)
		أرجع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	إذا (تۈگۈن->rb_ئوڭ) {
		تۈگۈن = تۈگۈن->rb_ئوڭ;
		طالما (تۈگۈن->rb_سول)
			تۈگۈن=تۈگۈن->rb_سول;
		أرجع (بنية rb_تۈگۈن *)تۈگۈن;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	طالما ((parent = ext2fs_rb_parent(تۈگۈن)) && تۈگۈن == parent->rb_ئوڭ)
		تۈگۈن = parent;

	أرجع parent;
}

بنية rb_تۈگۈن *ext2fs_rb_prev(بنية rb_تۈگۈن *تۈگۈن)
{
	بنية rb_تۈگۈن *parent;

	إذا (ext2fs_rb_parent(تۈگۈن) == تۈگۈن)
		أرجع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	إذا (تۈگۈن->rb_سول) {
		تۈگۈن = تۈگۈن->rb_سول;
		طالما (تۈگۈن->rb_ئوڭ)
			تۈگۈن=تۈگۈن->rb_ئوڭ;
		أرجع (بنية rb_تۈگۈن *)تۈگۈن;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	طالما ((parent = ext2fs_rb_parent(تۈگۈن)) && تۈگۈن == parent->rb_سول)
		تۈگۈن = parent;

	أرجع parent;
}

فراغ ext2fs_rb_replace_تۈگۈن(بنية rb_تۈگۈن *victim, بنية rb_تۈگۈن *new,
			  بنية rb_يىلتىز *يىلتىز)
{
	بنية rb_تۈگۈن *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	إذا (parent) {
		إذا (victim == parent->rb_سول)
			parent->rb_سول = new;
		وإلا
			parent->rb_ئوڭ = new;
	} وإلا {
		يىلتىز->rb_تۈگۈن = new;
	}
	إذا (victim->rb_سول)
		ext2fs_rb_set_parent(victim->rb_سول, new);
	إذا (victim->rb_ئوڭ)
		ext2fs_rb_set_parent(victim->rb_ئوڭ, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
