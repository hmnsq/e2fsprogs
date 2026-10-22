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

static void __rb_rotate_left(struct rb_बुहोल *बुहोल, struct rb_उगात *उगात)
{
	struct rb_बुहोल *कानान = बुहोल->rb_कानान;
	struct rb_बुहोल *parent = ext2fs_rb_parent(बुहोल);

	if ((बुहोल->rb_कानान = कानान->rb_कालिवा))
		ext2fs_rb_set_parent(कानान->rb_कालिवा, बुहोल);
	कानान->rb_कालिवा = बुहोल;

	ext2fs_rb_set_parent(कानान, parent);

	if (parent)
	{
		if (बुहोल == parent->rb_कालिवा)
			parent->rb_कालिवा = कानान;
		else
			parent->rb_कानान = कानान;
	}
	else
		उगात->rb_बुहोल = कानान;
	ext2fs_rb_set_parent(बुहोल, कानान);
}

static void __rb_rotate_right(struct rb_बुहोल *बुहोल, struct rb_उगात *उगात)
{
	struct rb_बुहोल *कालिवा = बुहोल->rb_कालिवा;
	struct rb_बुहोल *parent = ext2fs_rb_parent(बुहोल);

	if ((बुहोल->rb_कालिवा = कालिवा->rb_कानान))
		ext2fs_rb_set_parent(कालिवा->rb_कानान, बुहोल);
	कालिवा->rb_कानान = बुहोल;

	ext2fs_rb_set_parent(कालिवा, parent);

	if (parent)
	{
		if (बुहोल == parent->rb_कानान)
			parent->rb_कानान = कालिवा;
		else
			parent->rb_कालिवा = कालिवा;
	}
	else
		उगात->rb_बुहोल = कालिवा;
	ext2fs_rb_set_parent(बुहोल, कालिवा);
}

void ext2fs_rb_इसिंगित_color(struct rb_बुहोल *बुहोल, struct rb_उगात *उगात)
{
	struct rb_बुहोल *parent, *gparent;

	while ((parent = ext2fs_rb_parent(बुहोल)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		if (parent == gparent->rb_कालिवा)
		{
			{
				register struct rb_बुहोल *uncle = gparent->rb_कानान;
				if (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					बुहोल = gparent;
					continue;
				}
			}

			if (parent->rb_कानान == बुहोल)
			{
				register struct rb_बुहोल *tmp;
				__rb_rotate_left(parent, उगात);
				tmp = parent;
				parent = बुहोल;
				बुहोल = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, उगात);
		} else {
			{
				register struct rb_बुहोल *uncle = gparent->rb_कालिवा;
				if (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					बुहोल = gparent;
					continue;
				}
			}

			if (parent->rb_कालिवा == बुहोल)
			{
				register struct rb_बुहोल *tmp;
				__rb_rotate_right(parent, उगात);
				tmp = parent;
				parent = बुहोल;
				बुहोल = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, उगात);
		}
	}

	ext2fs_rb_set_black(उगात->rb_बुहोल);
}

static void __rb_erase_color(struct rb_बुहोल *बुहोल, struct rb_बुहोल *parent,
			     struct rb_उगात *उगात)
{
	struct rb_बुहोल *other;

	while ((!बुहोल || ext2fs_rb_is_black(बुहोल)) && बुहोल != उगात->rb_बुहोल)
	{
		if (parent->rb_कालिवा == बुहोल)
		{
			other = parent->rb_कानान;
			if (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, उगात);
				other = parent->rb_कानान;
			}
			if ((!other->rb_कालिवा || ext2fs_rb_is_black(other->rb_कालिवा)) &&
			    (!other->rb_कानान || ext2fs_rb_is_black(other->rb_कानान)))
			{
				ext2fs_rb_set_red(other);
				बुहोल = parent;
				parent = ext2fs_rb_parent(बुहोल);
			}
			else
			{
				if (!other->rb_कानान || ext2fs_rb_is_black(other->rb_कानान))
				{
					ext2fs_rb_set_black(other->rb_कालिवा);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, उगात);
					other = parent->rb_कानान;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_कानान);
				__rb_rotate_left(parent, उगात);
				बुहोल = उगात->rb_बुहोल;
				break;
			}
		}
		else
		{
			other = parent->rb_कालिवा;
			if (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, उगात);
				other = parent->rb_कालिवा;
			}
			if ((!other->rb_कालिवा || ext2fs_rb_is_black(other->rb_कालिवा)) &&
			    (!other->rb_कानान || ext2fs_rb_is_black(other->rb_कानान)))
			{
				ext2fs_rb_set_red(other);
				बुहोल = parent;
				parent = ext2fs_rb_parent(बुहोल);
			}
			else
			{
				if (!other->rb_कालिवा || ext2fs_rb_is_black(other->rb_कालिवा))
				{
					ext2fs_rb_set_black(other->rb_कानान);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, उगात);
					other = parent->rb_कालिवा;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_कालिवा);
				__rb_rotate_right(parent, उगात);
				बुहोल = उगात->rb_बुहोल;
				break;
			}
		}
	}
	if (बुहोल)
		ext2fs_rb_set_black(बुहोल);
}

void ext2fs_rb_erase(struct rb_बुहोल *बुहोल, struct rb_उगात *उगात)
{
	struct rb_बुहोल *child, *parent;
	int color;

	if (!बुहोल->rb_कालिवा)
		child = बुहोल->rb_कानान;
	else if (!बुहोल->rb_कानान)
		child = बुहोल->rb_कालिवा;
	else
	{
		struct rb_बुहोल *old = बुहोल, *कालिवा;

		बुहोल = बुहोल->rb_कानान;
		while ((कालिवा = बुहोल->rb_कालिवा) != NULL)
			बुहोल = कालिवा;

		if (ext2fs_rb_parent(old)) {
			if (ext2fs_rb_parent(old)->rb_कालिवा == old)
				ext2fs_rb_parent(old)->rb_कालिवा = बुहोल;
			else
				ext2fs_rb_parent(old)->rb_कानान = बुहोल;
		} else
			उगात->rb_बुहोल = बुहोल;

		child = बुहोल->rb_कानान;
		parent = ext2fs_rb_parent(बुहोल);
		color = ext2fs_rb_color(बुहोल);

		if (parent == old) {
			parent = बुहोल;
		} else {
			if (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_कालिवा = child;

			बुहोल->rb_कानान = old->rb_कानान;
			ext2fs_rb_set_parent(old->rb_कानान, बुहोल);
		}

		बुहोल->rb_parent_color = old->rb_parent_color;
		बुहोल->rb_कालिवा = old->rb_कालिवा;
		ext2fs_rb_set_parent(old->rb_कालिवा, बुहोल);

		goto color;
	}

	parent = ext2fs_rb_parent(बुहोल);
	color = ext2fs_rb_color(बुहोल);

	if (child)
		ext2fs_rb_set_parent(child, parent);
	if (parent)
	{
		if (parent->rb_कालिवा == बुहोल)
			parent->rb_कालिवा = child;
		else
			parent->rb_कानान = child;
	}
	else
		उगात->rb_बुहोल = child;

 color:
	if (color == RB_BLACK)
		__rb_erase_color(child, parent, उगात);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
struct rb_बुहोल *ext2fs_rb_first(const struct rb_उगात *उगात)
{
	struct rb_बुहोल	*n;

	n = उगात->rb_बुहोल;
	if (!n)
		return NULL;
	while (n->rb_कालिवा)
		n = n->rb_कालिवा;
	return n;
}

struct rb_बुहोल *ext2fs_rb_last(const struct rb_उगात *उगात)
{
	struct rb_बुहोल	*n;

	n = उगात->rb_बुहोल;
	if (!n)
		return NULL;
	while (n->rb_कानान)
		n = n->rb_कानान;
	return n;
}

struct rb_बुहोल *ext2fs_rb_next(struct rb_बुहोल *बुहोल)
{
	struct rb_बुहोल *parent;

	if (ext2fs_rb_parent(बुहोल) == बुहोल)
		return NULL;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	if (बुहोल->rb_कानान) {
		बुहोल = बुहोल->rb_कानान;
		while (बुहोल->rb_कालिवा)
			बुहोल=बुहोल->rb_कालिवा;
		return (struct rb_बुहोल *)बुहोल;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	while ((parent = ext2fs_rb_parent(बुहोल)) && बुहोल == parent->rb_कानान)
		बुहोल = parent;

	return parent;
}

struct rb_बुहोल *ext2fs_rb_prev(struct rb_बुहोल *बुहोल)
{
	struct rb_बुहोल *parent;

	if (ext2fs_rb_parent(बुहोल) == बुहोल)
		return NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	if (बुहोल->rb_कालिवा) {
		बुहोल = बुहोल->rb_कालिवा;
		while (बुहोल->rb_कानान)
			बुहोल=बुहोल->rb_कानान;
		return (struct rb_बुहोल *)बुहोल;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	while ((parent = ext2fs_rb_parent(बुहोल)) && बुहोल == parent->rb_कालिवा)
		बुहोल = parent;

	return parent;
}

void ext2fs_rb_replace_बुहोल(struct rb_बुहोल *victim, struct rb_बुहोल *new,
			  struct rb_उगात *उगात)
{
	struct rb_बुहोल *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	if (parent) {
		if (victim == parent->rb_कालिवा)
			parent->rb_कालिवा = new;
		else
			parent->rb_कानान = new;
	} else {
		उगात->rb_बुहोल = new;
	}
	if (victim->rb_कालिवा)
		ext2fs_rb_set_parent(victim->rb_कालिवा, new);
	if (victim->rb_कानान)
		ext2fs_rb_set_parent(victim->rb_कानान, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
