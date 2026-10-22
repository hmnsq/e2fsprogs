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

ساكن فراغ __rb_rotate_left(بنية rb_عقدة *عقدة, بنية rb_جذر *جذر)
{
	بنية rb_عقدة *يمين = عقدة->rb_يمين;
	بنية rb_عقدة *parent = ext2fs_rb_parent(عقدة);

	إذا ((عقدة->rb_يمين = يمين->rb_يسار))
		ext2fs_rb_set_parent(يمين->rb_يسار, عقدة);
	يمين->rb_يسار = عقدة;

	ext2fs_rb_set_parent(يمين, parent);

	إذا (parent)
	{
		إذا (عقدة == parent->rb_يسار)
			parent->rb_يسار = يمين;
		وإلا
			parent->rb_يمين = يمين;
	}
	وإلا
		جذر->rb_عقدة = يمين;
	ext2fs_rb_set_parent(عقدة, يمين);
}

ساكن فراغ __rb_rotate_right(بنية rb_عقدة *عقدة, بنية rb_جذر *جذر)
{
	بنية rb_عقدة *يسار = عقدة->rb_يسار;
	بنية rb_عقدة *parent = ext2fs_rb_parent(عقدة);

	إذا ((عقدة->rb_يسار = يسار->rb_يمين))
		ext2fs_rb_set_parent(يسار->rb_يمين, عقدة);
	يسار->rb_يمين = عقدة;

	ext2fs_rb_set_parent(يسار, parent);

	إذا (parent)
	{
		إذا (عقدة == parent->rb_يمين)
			parent->rb_يمين = يسار;
		وإلا
			parent->rb_يسار = يسار;
	}
	وإلا
		جذر->rb_عقدة = يسار;
	ext2fs_rb_set_parent(عقدة, يسار);
}

فراغ ext2fs_rb_أدرج_color(بنية rb_عقدة *عقدة, بنية rb_جذر *جذر)
{
	بنية rb_عقدة *parent, *gparent;

	بينما ((parent = ext2fs_rb_parent(عقدة)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		إذا (parent == gparent->rb_يسار)
		{
			{
				سجل بنية rb_عقدة *uncle = gparent->rb_يمين;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					عقدة = gparent;
					استمر;
				}
			}

			إذا (parent->rb_يمين == عقدة)
			{
				سجل بنية rb_عقدة *tmp;
				__rb_rotate_left(parent, جذر);
				tmp = parent;
				parent = عقدة;
				عقدة = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, جذر);
		} وإلا {
			{
				سجل بنية rb_عقدة *uncle = gparent->rb_يسار;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					عقدة = gparent;
					استمر;
				}
			}

			إذا (parent->rb_يسار == عقدة)
			{
				سجل بنية rb_عقدة *tmp;
				__rb_rotate_right(parent, جذر);
				tmp = parent;
				parent = عقدة;
				عقدة = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, جذر);
		}
	}

	ext2fs_rb_set_black(جذر->rb_عقدة);
}

ساكن فراغ __rb_erase_color(بنية rb_عقدة *عقدة, بنية rb_عقدة *parent,
			     بنية rb_جذر *جذر)
{
	بنية rb_عقدة *other;

	بينما ((!عقدة || ext2fs_rb_is_black(عقدة)) && عقدة != جذر->rb_عقدة)
	{
		إذا (parent->rb_يسار == عقدة)
		{
			other = parent->rb_يمين;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, جذر);
				other = parent->rb_يمين;
			}
			إذا ((!other->rb_يسار || ext2fs_rb_is_black(other->rb_يسار)) &&
			    (!other->rb_يمين || ext2fs_rb_is_black(other->rb_يمين)))
			{
				ext2fs_rb_set_red(other);
				عقدة = parent;
				parent = ext2fs_rb_parent(عقدة);
			}
			وإلا
			{
				إذا (!other->rb_يمين || ext2fs_rb_is_black(other->rb_يمين))
				{
					ext2fs_rb_set_black(other->rb_يسار);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, جذر);
					other = parent->rb_يمين;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_يمين);
				__rb_rotate_left(parent, جذر);
				عقدة = جذر->rb_عقدة;
				اكسر;
			}
		}
		وإلا
		{
			other = parent->rb_يسار;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, جذر);
				other = parent->rb_يسار;
			}
			إذا ((!other->rb_يسار || ext2fs_rb_is_black(other->rb_يسار)) &&
			    (!other->rb_يمين || ext2fs_rb_is_black(other->rb_يمين)))
			{
				ext2fs_rb_set_red(other);
				عقدة = parent;
				parent = ext2fs_rb_parent(عقدة);
			}
			وإلا
			{
				إذا (!other->rb_يسار || ext2fs_rb_is_black(other->rb_يسار))
				{
					ext2fs_rb_set_black(other->rb_يمين);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, جذر);
					other = parent->rb_يسار;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_يسار);
				__rb_rotate_right(parent, جذر);
				عقدة = جذر->rb_عقدة;
				اكسر;
			}
		}
	}
	إذا (عقدة)
		ext2fs_rb_set_black(عقدة);
}

فراغ ext2fs_rb_erase(بنية rb_عقدة *عقدة, بنية rb_جذر *جذر)
{
	بنية rb_عقدة *child, *parent;
	صحيح color;

	إذا (!عقدة->rb_يسار)
		child = عقدة->rb_يمين;
	وإلا إذا (!عقدة->rb_يمين)
		child = عقدة->rb_يسار;
	وإلا
	{
		بنية rb_عقدة *old = عقدة, *يسار;

		عقدة = عقدة->rb_يمين;
		بينما ((يسار = عقدة->rb_يسار) != عدم)
			عقدة = يسار;

		إذا (ext2fs_rb_parent(old)) {
			إذا (ext2fs_rb_parent(old)->rb_يسار == old)
				ext2fs_rb_parent(old)->rb_يسار = عقدة;
			وإلا
				ext2fs_rb_parent(old)->rb_يمين = عقدة;
		} وإلا
			جذر->rb_عقدة = عقدة;

		child = عقدة->rb_يمين;
		parent = ext2fs_rb_parent(عقدة);
		color = ext2fs_rb_color(عقدة);

		إذا (parent == old) {
			parent = عقدة;
		} وإلا {
			إذا (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_يسار = child;

			عقدة->rb_يمين = old->rb_يمين;
			ext2fs_rb_set_parent(old->rb_يمين, عقدة);
		}

		عقدة->rb_parent_color = old->rb_parent_color;
		عقدة->rb_يسار = old->rb_يسار;
		ext2fs_rb_set_parent(old->rb_يسار, عقدة);

		اذهب color;
	}

	parent = ext2fs_rb_parent(عقدة);
	color = ext2fs_rb_color(عقدة);

	إذا (child)
		ext2fs_rb_set_parent(child, parent);
	إذا (parent)
	{
		إذا (parent->rb_يسار == عقدة)
			parent->rb_يسار = child;
		وإلا
			parent->rb_يمين = child;
	}
	وإلا
		جذر->rb_عقدة = child;

 color:
	إذا (color == RB_BLACK)
		__rb_erase_color(child, parent, جذر);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_عقدة *ext2fs_rb_first(ثابت بنية rb_جذر *جذر)
{
	بنية rb_عقدة	*n;

	n = جذر->rb_عقدة;
	إذا (!n)
		إرجاع عدم;
	بينما (n->rb_يسار)
		n = n->rb_يسار;
	إرجاع n;
}

بنية rb_عقدة *ext2fs_rb_last(ثابت بنية rb_جذر *جذر)
{
	بنية rb_عقدة	*n;

	n = جذر->rb_عقدة;
	إذا (!n)
		إرجاع عدم;
	بينما (n->rb_يمين)
		n = n->rb_يمين;
	إرجاع n;
}

بنية rb_عقدة *ext2fs_rb_next(بنية rb_عقدة *عقدة)
{
	بنية rb_عقدة *parent;

	إذا (ext2fs_rb_parent(عقدة) == عقدة)
		إرجاع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	إذا (عقدة->rb_يمين) {
		عقدة = عقدة->rb_يمين;
		بينما (عقدة->rb_يسار)
			عقدة=عقدة->rb_يسار;
		إرجاع (بنية rb_عقدة *)عقدة;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	بينما ((parent = ext2fs_rb_parent(عقدة)) && عقدة == parent->rb_يمين)
		عقدة = parent;

	إرجاع parent;
}

بنية rb_عقدة *ext2fs_rb_prev(بنية rb_عقدة *عقدة)
{
	بنية rb_عقدة *parent;

	إذا (ext2fs_rb_parent(عقدة) == عقدة)
		إرجاع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	إذا (عقدة->rb_يسار) {
		عقدة = عقدة->rb_يسار;
		بينما (عقدة->rb_يمين)
			عقدة=عقدة->rb_يمين;
		إرجاع (بنية rb_عقدة *)عقدة;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	بينما ((parent = ext2fs_rb_parent(عقدة)) && عقدة == parent->rb_يسار)
		عقدة = parent;

	إرجاع parent;
}

فراغ ext2fs_rb_replace_عقدة(بنية rb_عقدة *victim, بنية rb_عقدة *new,
			  بنية rb_جذر *جذر)
{
	بنية rb_عقدة *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	إذا (parent) {
		إذا (victim == parent->rb_يسار)
			parent->rb_يسار = new;
		وإلا
			parent->rb_يمين = new;
	} وإلا {
		جذر->rb_عقدة = new;
	}
	إذا (victim->rb_يسار)
		ext2fs_rb_set_parent(victim->rb_يسار, new);
	إذا (victim->rb_يمين)
		ext2fs_rb_set_parent(victim->rb_يمين, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
