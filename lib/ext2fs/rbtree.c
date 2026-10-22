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

#ಅನ್ತರ್ಭಾವ "rbtree.h"

ಸ್ಥಿರ ಶೂನ್ಯ __rb_rotate_left(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು, ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ rb_ಗಂಟು *ಬಲ = ಗಂಟು->rb_ಬಲ;
	ಸಂರಚನಾ rb_ಗಂಟು *parent = ext2fs_rb_parent(ಗಂಟು);

	ಯದಿ ((ಗಂಟು->rb_ಬಲ = ಬಲ->rb_ಎಡ))
		ext2fs_rb_set_parent(ಬಲ->rb_ಎಡ, ಗಂಟು);
	ಬಲ->rb_ಎಡ = ಗಂಟು;

	ext2fs_rb_set_parent(ಬಲ, parent);

	ಯದಿ (parent)
	{
		ಯದಿ (ಗಂಟು == parent->rb_ಎಡ)
			parent->rb_ಎಡ = ಬಲ;
		ಅನ್ಯಥಾ
			parent->rb_ಬಲ = ಬಲ;
	}
	ಅನ್ಯಥಾ
		ಬೇರು->rb_ಗಂಟು = ಬಲ;
	ext2fs_rb_set_parent(ಗಂಟು, ಬಲ);
}

ಸ್ಥಿರ ಶೂನ್ಯ __rb_rotate_right(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು, ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ rb_ಗಂಟು *ಎಡ = ಗಂಟು->rb_ಎಡ;
	ಸಂರಚನಾ rb_ಗಂಟು *parent = ext2fs_rb_parent(ಗಂಟು);

	ಯದಿ ((ಗಂಟು->rb_ಎಡ = ಎಡ->rb_ಬಲ))
		ext2fs_rb_set_parent(ಎಡ->rb_ಬಲ, ಗಂಟು);
	ಎಡ->rb_ಬಲ = ಗಂಟು;

	ext2fs_rb_set_parent(ಎಡ, parent);

	ಯದಿ (parent)
	{
		ಯದಿ (ಗಂಟು == parent->rb_ಬಲ)
			parent->rb_ಬಲ = ಎಡ;
		ಅನ್ಯಥಾ
			parent->rb_ಎಡ = ಎಡ;
	}
	ಅನ್ಯಥಾ
		ಬೇರು->rb_ಗಂಟು = ಎಡ;
	ext2fs_rb_set_parent(ಗಂಟು, ಎಡ);
}

ಶೂನ್ಯ ext2fs_rb_ಸೇರಿಸು_color(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು, ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ rb_ಗಂಟು *parent, *gparent;

	ಯಾವತ್ ((parent = ext2fs_rb_parent(ಗಂಟು)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		ಯದಿ (parent == gparent->rb_ಎಡ)
		{
			{
				ಪಞ್ಜಿಕಾ ಸಂರಚನಾ rb_ಗಂಟು *uncle = gparent->rb_ಬಲ;
				ಯದಿ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ಗಂಟು = gparent;
					ಅನುವರ್ತ;
				}
			}

			ಯದಿ (parent->rb_ಬಲ == ಗಂಟು)
			{
				ಪಞ್ಜಿಕಾ ಸಂರಚನಾ rb_ಗಂಟು *tmp;
				__rb_rotate_left(parent, ಬೇರು);
				tmp = parent;
				parent = ಗಂಟು;
				ಗಂಟು = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, ಬೇರು);
		} ಅನ್ಯಥಾ {
			{
				ಪಞ್ಜಿಕಾ ಸಂರಚನಾ rb_ಗಂಟು *uncle = gparent->rb_ಎಡ;
				ಯದಿ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ಗಂಟು = gparent;
					ಅನುವರ್ತ;
				}
			}

			ಯದಿ (parent->rb_ಎಡ == ಗಂಟು)
			{
				ಪಞ್ಜಿಕಾ ಸಂರಚನಾ rb_ಗಂಟು *tmp;
				__rb_rotate_right(parent, ಬೇರು);
				tmp = parent;
				parent = ಗಂಟು;
				ಗಂಟು = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, ಬೇರು);
		}
	}

	ext2fs_rb_set_black(ಬೇರು->rb_ಗಂಟು);
}

ಸ್ಥಿರ ಶೂನ್ಯ __rb_erase_color(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು, ಸಂರಚನಾ rb_ಗಂಟು *parent,
			     ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ rb_ಗಂಟು *other;

	ಯಾವತ್ ((!ಗಂಟು || ext2fs_rb_is_black(ಗಂಟು)) && ಗಂಟು != ಬೇರು->rb_ಗಂಟು)
	{
		ಯದಿ (parent->rb_ಎಡ == ಗಂಟು)
		{
			other = parent->rb_ಬಲ;
			ಯದಿ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, ಬೇರು);
				other = parent->rb_ಬಲ;
			}
			ಯದಿ ((!other->rb_ಎಡ || ext2fs_rb_is_black(other->rb_ಎಡ)) &&
			    (!other->rb_ಬಲ || ext2fs_rb_is_black(other->rb_ಬಲ)))
			{
				ext2fs_rb_set_red(other);
				ಗಂಟು = parent;
				parent = ext2fs_rb_parent(ಗಂಟು);
			}
			ಅನ್ಯಥಾ
			{
				ಯದಿ (!other->rb_ಬಲ || ext2fs_rb_is_black(other->rb_ಬಲ))
				{
					ext2fs_rb_set_black(other->rb_ಎಡ);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, ಬೇರು);
					other = parent->rb_ಬಲ;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_ಬಲ);
				__rb_rotate_left(parent, ಬೇರು);
				ಗಂಟು = ಬೇರು->rb_ಗಂಟು;
				ಭಙ್ಗ;
			}
		}
		ಅನ್ಯಥಾ
		{
			other = parent->rb_ಎಡ;
			ಯದಿ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, ಬೇರು);
				other = parent->rb_ಎಡ;
			}
			ಯದಿ ((!other->rb_ಎಡ || ext2fs_rb_is_black(other->rb_ಎಡ)) &&
			    (!other->rb_ಬಲ || ext2fs_rb_is_black(other->rb_ಬಲ)))
			{
				ext2fs_rb_set_red(other);
				ಗಂಟು = parent;
				parent = ext2fs_rb_parent(ಗಂಟು);
			}
			ಅನ್ಯಥಾ
			{
				ಯದಿ (!other->rb_ಎಡ || ext2fs_rb_is_black(other->rb_ಎಡ))
				{
					ext2fs_rb_set_black(other->rb_ಬಲ);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, ಬೇರು);
					other = parent->rb_ಎಡ;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_ಎಡ);
				__rb_rotate_right(parent, ಬೇರು);
				ಗಂಟು = ಬೇರು->rb_ಗಂಟು;
				ಭಙ್ಗ;
			}
		}
	}
	ಯದಿ (ಗಂಟು)
		ext2fs_rb_set_black(ಗಂಟು);
}

ಶೂನ್ಯ ext2fs_rb_erase(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು, ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ rb_ಗಂಟು *child, *parent;
	ಪೂರ್ಣಾಙ್ಕ color;

	ಯದಿ (!ಗಂಟು->rb_ಎಡ)
		child = ಗಂಟು->rb_ಬಲ;
	ಅನ್ಯಥಾ ಯದಿ (!ಗಂಟು->rb_ಬಲ)
		child = ಗಂಟು->rb_ಎಡ;
	ಅನ್ಯಥಾ
	{
		ಸಂರಚನಾ rb_ಗಂಟು *old = ಗಂಟು, *ಎಡ;

		ಗಂಟು = ಗಂಟು->rb_ಬಲ;
		ಯಾವತ್ ((ಎಡ = ಗಂಟು->rb_ಎಡ) != ರಿಕ್ತ)
			ಗಂಟು = ಎಡ;

		ಯದಿ (ext2fs_rb_parent(old)) {
			ಯದಿ (ext2fs_rb_parent(old)->rb_ಎಡ == old)
				ext2fs_rb_parent(old)->rb_ಎಡ = ಗಂಟು;
			ಅನ್ಯಥಾ
				ext2fs_rb_parent(old)->rb_ಬಲ = ಗಂಟು;
		} ಅನ್ಯಥಾ
			ಬೇರು->rb_ಗಂಟು = ಗಂಟು;

		child = ಗಂಟು->rb_ಬಲ;
		parent = ext2fs_rb_parent(ಗಂಟು);
		color = ext2fs_rb_color(ಗಂಟು);

		ಯದಿ (parent == old) {
			parent = ಗಂಟು;
		} ಅನ್ಯಥಾ {
			ಯದಿ (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_ಎಡ = child;

			ಗಂಟು->rb_ಬಲ = old->rb_ಬಲ;
			ext2fs_rb_set_parent(old->rb_ಬಲ, ಗಂಟು);
		}

		ಗಂಟು->rb_parent_color = old->rb_parent_color;
		ಗಂಟು->rb_ಎಡ = old->rb_ಎಡ;
		ext2fs_rb_set_parent(old->rb_ಎಡ, ಗಂಟು);

		ಗಚ್ಛ color;
	}

	parent = ext2fs_rb_parent(ಗಂಟು);
	color = ext2fs_rb_color(ಗಂಟು);

	ಯದಿ (child)
		ext2fs_rb_set_parent(child, parent);
	ಯದಿ (parent)
	{
		ಯದಿ (parent->rb_ಎಡ == ಗಂಟು)
			parent->rb_ಎಡ = child;
		ಅನ್ಯಥಾ
			parent->rb_ಬಲ = child;
	}
	ಅನ್ಯಥಾ
		ಬೇರು->rb_ಗಂಟು = child;

 color:
	ಯದಿ (color == RB_BLACK)
		__rb_erase_color(child, parent, ಬೇರು);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
ಸಂರಚನಾ rb_ಗಂಟು *ext2fs_rb_first(ಧ್ರುವ ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ rb_ಗಂಟು	*n;

	n = ಬೇರು->rb_ಗಂಟು;
	ಯದಿ (!n)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	ಯಾವತ್ (n->rb_ಎಡ)
		n = n->rb_ಎಡ;
	ಪ್ರತ್ಯಾಗಮ n;
}

ಸಂರಚನಾ rb_ಗಂಟು *ext2fs_rb_last(ಧ್ರುವ ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ rb_ಗಂಟು	*n;

	n = ಬೇರು->rb_ಗಂಟು;
	ಯದಿ (!n)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	ಯಾವತ್ (n->rb_ಬಲ)
		n = n->rb_ಬಲ;
	ಪ್ರತ್ಯಾಗಮ n;
}

ಸಂರಚನಾ rb_ಗಂಟು *ext2fs_rb_next(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು)
{
	ಸಂರಚನಾ rb_ಗಂಟು *parent;

	ಯದಿ (ext2fs_rb_parent(ಗಂಟು) == ಗಂಟು)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	ಯದಿ (ಗಂಟು->rb_ಬಲ) {
		ಗಂಟು = ಗಂಟು->rb_ಬಲ;
		ಯಾವತ್ (ಗಂಟು->rb_ಎಡ)
			ಗಂಟು=ಗಂಟು->rb_ಎಡ;
		ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ rb_ಗಂಟು *)ಗಂಟು;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	ಯಾವತ್ ((parent = ext2fs_rb_parent(ಗಂಟು)) && ಗಂಟು == parent->rb_ಬಲ)
		ಗಂಟು = parent;

	ಪ್ರತ್ಯಾಗಮ parent;
}

ಸಂರಚನಾ rb_ಗಂಟು *ext2fs_rb_prev(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು)
{
	ಸಂರಚನಾ rb_ಗಂಟು *parent;

	ಯದಿ (ext2fs_rb_parent(ಗಂಟು) == ಗಂಟು)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	ಯದಿ (ಗಂಟು->rb_ಎಡ) {
		ಗಂಟು = ಗಂಟು->rb_ಎಡ;
		ಯಾವತ್ (ಗಂಟು->rb_ಬಲ)
			ಗಂಟು=ಗಂಟು->rb_ಬಲ;
		ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ rb_ಗಂಟು *)ಗಂಟು;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	ಯಾವತ್ ((parent = ext2fs_rb_parent(ಗಂಟು)) && ಗಂಟು == parent->rb_ಎಡ)
		ಗಂಟು = parent;

	ಪ್ರತ್ಯಾಗಮ parent;
}

ಶೂನ್ಯ ext2fs_rb_replace_ಗಂಟು(ಸಂರಚನಾ rb_ಗಂಟು *victim, ಸಂರಚನಾ rb_ಗಂಟು *new,
			  ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ rb_ಗಂಟು *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	ಯದಿ (parent) {
		ಯದಿ (victim == parent->rb_ಎಡ)
			parent->rb_ಎಡ = new;
		ಅನ್ಯಥಾ
			parent->rb_ಬಲ = new;
	} ಅನ್ಯಥಾ {
		ಬೇರು->rb_ಗಂಟು = new;
	}
	ಯದಿ (victim->rb_ಎಡ)
		ext2fs_rb_set_parent(victim->rb_ಎಡ, new);
	ಯದಿ (victim->rb_ಬಲ)
		ext2fs_rb_set_parent(victim->rb_ಬಲ, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
