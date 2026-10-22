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

#அந்தர்பாவ "rbtree.h"

நிலையான வெறுமை __rb_rotate_left(கட்டமைப்பு rb_முனை *முனை, கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு rb_முனை *வலது = முனை->rb_வலது;
	கட்டமைப்பு rb_முனை *parent = ext2fs_rb_parent(முனை);

	எனில் ((முனை->rb_வலது = வலது->rb_இடது))
		ext2fs_rb_set_parent(வலது->rb_இடது, முனை);
	வலது->rb_இடது = முனை;

	ext2fs_rb_set_parent(வலது, parent);

	எனில் (parent)
	{
		எனில் (முனை == parent->rb_இடது)
			parent->rb_இடது = வலது;
		இல்லையெனில்
			parent->rb_வலது = வலது;
	}
	இல்லையெனில்
		வேர்->rb_முனை = வலது;
	ext2fs_rb_set_parent(முனை, வலது);
}

நிலையான வெறுமை __rb_rotate_right(கட்டமைப்பு rb_முனை *முனை, கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு rb_முனை *இடது = முனை->rb_இடது;
	கட்டமைப்பு rb_முனை *parent = ext2fs_rb_parent(முனை);

	எனில் ((முனை->rb_இடது = இடது->rb_வலது))
		ext2fs_rb_set_parent(இடது->rb_வலது, முனை);
	இடது->rb_வலது = முனை;

	ext2fs_rb_set_parent(இடது, parent);

	எனில் (parent)
	{
		எனில் (முனை == parent->rb_வலது)
			parent->rb_வலது = இடது;
		இல்லையெனில்
			parent->rb_இடது = இடது;
	}
	இல்லையெனில்
		வேர்->rb_முனை = இடது;
	ext2fs_rb_set_parent(முனை, இடது);
}

வெறுமை ext2fs_rb_செருகு_color(கட்டமைப்பு rb_முனை *முனை, கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு rb_முனை *parent, *gparent;

	போது ((parent = ext2fs_rb_parent(முனை)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		எனில் (parent == gparent->rb_இடது)
		{
			{
				பதிவு கட்டமைப்பு rb_முனை *uncle = gparent->rb_வலது;
				எனில் (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					முனை = gparent;
					தொடர்;
				}
			}

			எனில் (parent->rb_வலது == முனை)
			{
				பதிவு கட்டமைப்பு rb_முனை *tmp;
				__rb_rotate_left(parent, வேர்);
				tmp = parent;
				parent = முனை;
				முனை = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, வேர்);
		} இல்லையெனில் {
			{
				பதிவு கட்டமைப்பு rb_முனை *uncle = gparent->rb_இடது;
				எனில் (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					முனை = gparent;
					தொடர்;
				}
			}

			எனில் (parent->rb_இடது == முனை)
			{
				பதிவு கட்டமைப்பு rb_முனை *tmp;
				__rb_rotate_right(parent, வேர்);
				tmp = parent;
				parent = முனை;
				முனை = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, வேர்);
		}
	}

	ext2fs_rb_set_black(வேர்->rb_முனை);
}

நிலையான வெறுமை __rb_erase_color(கட்டமைப்பு rb_முனை *முனை, கட்டமைப்பு rb_முனை *parent,
			     கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு rb_முனை *other;

	போது ((!முனை || ext2fs_rb_is_black(முனை)) && முனை != வேர்->rb_முனை)
	{
		எனில் (parent->rb_இடது == முனை)
		{
			other = parent->rb_வலது;
			எனில் (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, வேர்);
				other = parent->rb_வலது;
			}
			எனில் ((!other->rb_இடது || ext2fs_rb_is_black(other->rb_இடது)) &&
			    (!other->rb_வலது || ext2fs_rb_is_black(other->rb_வலது)))
			{
				ext2fs_rb_set_red(other);
				முனை = parent;
				parent = ext2fs_rb_parent(முனை);
			}
			இல்லையெனில்
			{
				எனில் (!other->rb_வலது || ext2fs_rb_is_black(other->rb_வலது))
				{
					ext2fs_rb_set_black(other->rb_இடது);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, வேர்);
					other = parent->rb_வலது;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_வலது);
				__rb_rotate_left(parent, வேர்);
				முனை = வேர்->rb_முனை;
				நிறுத்து;
			}
		}
		இல்லையெனில்
		{
			other = parent->rb_இடது;
			எனில் (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, வேர்);
				other = parent->rb_இடது;
			}
			எனில் ((!other->rb_இடது || ext2fs_rb_is_black(other->rb_இடது)) &&
			    (!other->rb_வலது || ext2fs_rb_is_black(other->rb_வலது)))
			{
				ext2fs_rb_set_red(other);
				முனை = parent;
				parent = ext2fs_rb_parent(முனை);
			}
			இல்லையெனில்
			{
				எனில் (!other->rb_இடது || ext2fs_rb_is_black(other->rb_இடது))
				{
					ext2fs_rb_set_black(other->rb_வலது);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, வேர்);
					other = parent->rb_இடது;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_இடது);
				__rb_rotate_right(parent, வேர்);
				முனை = வேர்->rb_முனை;
				நிறுத்து;
			}
		}
	}
	எனில் (முனை)
		ext2fs_rb_set_black(முனை);
}

வெறுமை ext2fs_rb_erase(கட்டமைப்பு rb_முனை *முனை, கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு rb_முனை *child, *parent;
	முழு color;

	எனில் (!முனை->rb_இடது)
		child = முனை->rb_வலது;
	இல்லையெனில் எனில் (!முனை->rb_வலது)
		child = முனை->rb_இடது;
	இல்லையெனில்
	{
		கட்டமைப்பு rb_முனை *old = முனை, *இடது;

		முனை = முனை->rb_வலது;
		போது ((இடது = முனை->rb_இடது) != வெற்று)
			முனை = இடது;

		எனில் (ext2fs_rb_parent(old)) {
			எனில் (ext2fs_rb_parent(old)->rb_இடது == old)
				ext2fs_rb_parent(old)->rb_இடது = முனை;
			இல்லையெனில்
				ext2fs_rb_parent(old)->rb_வலது = முனை;
		} இல்லையெனில்
			வேர்->rb_முனை = முனை;

		child = முனை->rb_வலது;
		parent = ext2fs_rb_parent(முனை);
		color = ext2fs_rb_color(முனை);

		எனில் (parent == old) {
			parent = முனை;
		} இல்லையெனில் {
			எனில் (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_இடது = child;

			முனை->rb_வலது = old->rb_வலது;
			ext2fs_rb_set_parent(old->rb_வலது, முனை);
		}

		முனை->rb_parent_color = old->rb_parent_color;
		முனை->rb_இடது = old->rb_இடது;
		ext2fs_rb_set_parent(old->rb_இடது, முனை);

		செல் color;
	}

	parent = ext2fs_rb_parent(முனை);
	color = ext2fs_rb_color(முனை);

	எனில் (child)
		ext2fs_rb_set_parent(child, parent);
	எனில் (parent)
	{
		எனில் (parent->rb_இடது == முனை)
			parent->rb_இடது = child;
		இல்லையெனில்
			parent->rb_வலது = child;
	}
	இல்லையெனில்
		வேர்->rb_முனை = child;

 color:
	எனில் (color == RB_BLACK)
		__rb_erase_color(child, parent, வேர்);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
கட்டமைப்பு rb_முனை *ext2fs_rb_first(மாறிலி கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு rb_முனை	*n;

	n = வேர்->rb_முனை;
	எனில் (!n)
		திரும்பு வெற்று;
	போது (n->rb_இடது)
		n = n->rb_இடது;
	திரும்பு n;
}

கட்டமைப்பு rb_முனை *ext2fs_rb_last(மாறிலி கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு rb_முனை	*n;

	n = வேர்->rb_முனை;
	எனில் (!n)
		திரும்பு வெற்று;
	போது (n->rb_வலது)
		n = n->rb_வலது;
	திரும்பு n;
}

கட்டமைப்பு rb_முனை *ext2fs_rb_next(கட்டமைப்பு rb_முனை *முனை)
{
	கட்டமைப்பு rb_முனை *parent;

	எனில் (ext2fs_rb_parent(முனை) == முனை)
		திரும்பு வெற்று;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	எனில் (முனை->rb_வலது) {
		முனை = முனை->rb_வலது;
		போது (முனை->rb_இடது)
			முனை=முனை->rb_இடது;
		திரும்பு (கட்டமைப்பு rb_முனை *)முனை;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	போது ((parent = ext2fs_rb_parent(முனை)) && முனை == parent->rb_வலது)
		முனை = parent;

	திரும்பு parent;
}

கட்டமைப்பு rb_முனை *ext2fs_rb_prev(கட்டமைப்பு rb_முனை *முனை)
{
	கட்டமைப்பு rb_முனை *parent;

	எனில் (ext2fs_rb_parent(முனை) == முனை)
		திரும்பு வெற்று;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	எனில் (முனை->rb_இடது) {
		முனை = முனை->rb_இடது;
		போது (முனை->rb_வலது)
			முனை=முனை->rb_வலது;
		திரும்பு (கட்டமைப்பு rb_முனை *)முனை;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	போது ((parent = ext2fs_rb_parent(முனை)) && முனை == parent->rb_இடது)
		முனை = parent;

	திரும்பு parent;
}

வெறுமை ext2fs_rb_replace_முனை(கட்டமைப்பு rb_முனை *victim, கட்டமைப்பு rb_முனை *new,
			  கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு rb_முனை *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	எனில் (parent) {
		எனில் (victim == parent->rb_இடது)
			parent->rb_இடது = new;
		இல்லையெனில்
			parent->rb_வலது = new;
	} இல்லையெனில் {
		வேர்->rb_முனை = new;
	}
	எனில் (victim->rb_இடது)
		ext2fs_rb_set_parent(victim->rb_இடது, new);
	எனில் (victim->rb_வலது)
		ext2fs_rb_set_parent(victim->rb_வலது, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
