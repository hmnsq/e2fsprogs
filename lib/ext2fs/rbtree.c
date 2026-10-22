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

#شامل "rbtree.h"

ایستا تهی __rb_rotate_left(ساختار rb_گره *گره, ساختار rb_ریشه *ریشه)
{
	ساختار rb_گره *راست = گره->rb_راست;
	ساختار rb_گره *parent = ext2fs_rb_parent(گره);

	اگر ((گره->rb_راست = راست->rb_چپ))
		ext2fs_rb_set_parent(راست->rb_چپ, گره);
	راست->rb_چپ = گره;

	ext2fs_rb_set_parent(راست, parent);

	اگر (parent)
	{
		اگر (گره == parent->rb_چپ)
			parent->rb_چپ = راست;
		وگرنه
			parent->rb_راست = راست;
	}
	وگرنه
		ریشه->rb_گره = راست;
	ext2fs_rb_set_parent(گره, راست);
}

ایستا تهی __rb_rotate_right(ساختار rb_گره *گره, ساختار rb_ریشه *ریشه)
{
	ساختار rb_گره *چپ = گره->rb_چپ;
	ساختار rb_گره *parent = ext2fs_rb_parent(گره);

	اگر ((گره->rb_چپ = چپ->rb_راست))
		ext2fs_rb_set_parent(چپ->rb_راست, گره);
	چپ->rb_راست = گره;

	ext2fs_rb_set_parent(چپ, parent);

	اگر (parent)
	{
		اگر (گره == parent->rb_راست)
			parent->rb_راست = چپ;
		وگرنه
			parent->rb_چپ = چپ;
	}
	وگرنه
		ریشه->rb_گره = چپ;
	ext2fs_rb_set_parent(گره, چپ);
}

تهی ext2fs_rb_درج_color(ساختار rb_گره *گره, ساختار rb_ریشه *ریشه)
{
	ساختار rb_گره *parent, *gparent;

	وقتی ((parent = ext2fs_rb_parent(گره)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		اگر (parent == gparent->rb_چپ)
		{
			{
				ثبات ساختار rb_گره *uncle = gparent->rb_راست;
				اگر (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					گره = gparent;
					ادامه;
				}
			}

			اگر (parent->rb_راست == گره)
			{
				ثبات ساختار rb_گره *tmp;
				__rb_rotate_left(parent, ریشه);
				tmp = parent;
				parent = گره;
				گره = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, ریشه);
		} وگرنه {
			{
				ثبات ساختار rb_گره *uncle = gparent->rb_چپ;
				اگر (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					گره = gparent;
					ادامه;
				}
			}

			اگر (parent->rb_چپ == گره)
			{
				ثبات ساختار rb_گره *tmp;
				__rb_rotate_right(parent, ریشه);
				tmp = parent;
				parent = گره;
				گره = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, ریشه);
		}
	}

	ext2fs_rb_set_black(ریشه->rb_گره);
}

ایستا تهی __rb_erase_color(ساختار rb_گره *گره, ساختار rb_گره *parent,
			     ساختار rb_ریشه *ریشه)
{
	ساختار rb_گره *other;

	وقتی ((!گره || ext2fs_rb_is_black(گره)) && گره != ریشه->rb_گره)
	{
		اگر (parent->rb_چپ == گره)
		{
			other = parent->rb_راست;
			اگر (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, ریشه);
				other = parent->rb_راست;
			}
			اگر ((!other->rb_چپ || ext2fs_rb_is_black(other->rb_چپ)) &&
			    (!other->rb_راست || ext2fs_rb_is_black(other->rb_راست)))
			{
				ext2fs_rb_set_red(other);
				گره = parent;
				parent = ext2fs_rb_parent(گره);
			}
			وگرنه
			{
				اگر (!other->rb_راست || ext2fs_rb_is_black(other->rb_راست))
				{
					ext2fs_rb_set_black(other->rb_چپ);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, ریشه);
					other = parent->rb_راست;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_راست);
				__rb_rotate_left(parent, ریشه);
				گره = ریشه->rb_گره;
				بشکن;
			}
		}
		وگرنه
		{
			other = parent->rb_چپ;
			اگر (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, ریشه);
				other = parent->rb_چپ;
			}
			اگر ((!other->rb_چپ || ext2fs_rb_is_black(other->rb_چپ)) &&
			    (!other->rb_راست || ext2fs_rb_is_black(other->rb_راست)))
			{
				ext2fs_rb_set_red(other);
				گره = parent;
				parent = ext2fs_rb_parent(گره);
			}
			وگرنه
			{
				اگر (!other->rb_چپ || ext2fs_rb_is_black(other->rb_چپ))
				{
					ext2fs_rb_set_black(other->rb_راست);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, ریشه);
					other = parent->rb_چپ;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_چپ);
				__rb_rotate_right(parent, ریشه);
				گره = ریشه->rb_گره;
				بشکن;
			}
		}
	}
	اگر (گره)
		ext2fs_rb_set_black(گره);
}

تهی ext2fs_rb_erase(ساختار rb_گره *گره, ساختار rb_ریشه *ریشه)
{
	ساختار rb_گره *child, *parent;
	صحیح color;

	اگر (!گره->rb_چپ)
		child = گره->rb_راست;
	وگرنه اگر (!گره->rb_راست)
		child = گره->rb_چپ;
	وگرنه
	{
		ساختار rb_گره *old = گره, *چپ;

		گره = گره->rb_راست;
		وقتی ((چپ = گره->rb_چپ) != پوچ)
			گره = چپ;

		اگر (ext2fs_rb_parent(old)) {
			اگر (ext2fs_rb_parent(old)->rb_چپ == old)
				ext2fs_rb_parent(old)->rb_چپ = گره;
			وگرنه
				ext2fs_rb_parent(old)->rb_راست = گره;
		} وگرنه
			ریشه->rb_گره = گره;

		child = گره->rb_راست;
		parent = ext2fs_rb_parent(گره);
		color = ext2fs_rb_color(گره);

		اگر (parent == old) {
			parent = گره;
		} وگرنه {
			اگر (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_چپ = child;

			گره->rb_راست = old->rb_راست;
			ext2fs_rb_set_parent(old->rb_راست, گره);
		}

		گره->rb_parent_color = old->rb_parent_color;
		گره->rb_چپ = old->rb_چپ;
		ext2fs_rb_set_parent(old->rb_چپ, گره);

		برو color;
	}

	parent = ext2fs_rb_parent(گره);
	color = ext2fs_rb_color(گره);

	اگر (child)
		ext2fs_rb_set_parent(child, parent);
	اگر (parent)
	{
		اگر (parent->rb_چپ == گره)
			parent->rb_چپ = child;
		وگرنه
			parent->rb_راست = child;
	}
	وگرنه
		ریشه->rb_گره = child;

 color:
	اگر (color == RB_BLACK)
		__rb_erase_color(child, parent, ریشه);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
ساختار rb_گره *ext2fs_rb_first(ثابت ساختار rb_ریشه *ریشه)
{
	ساختار rb_گره	*n;

	n = ریشه->rb_گره;
	اگر (!n)
		بازگشت پوچ;
	وقتی (n->rb_چپ)
		n = n->rb_چپ;
	بازگشت n;
}

ساختار rb_گره *ext2fs_rb_last(ثابت ساختار rb_ریشه *ریشه)
{
	ساختار rb_گره	*n;

	n = ریشه->rb_گره;
	اگر (!n)
		بازگشت پوچ;
	وقتی (n->rb_راست)
		n = n->rb_راست;
	بازگشت n;
}

ساختار rb_گره *ext2fs_rb_next(ساختار rb_گره *گره)
{
	ساختار rb_گره *parent;

	اگر (ext2fs_rb_parent(گره) == گره)
		بازگشت پوچ;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	اگر (گره->rb_راست) {
		گره = گره->rb_راست;
		وقتی (گره->rb_چپ)
			گره=گره->rb_چپ;
		بازگشت (ساختار rb_گره *)گره;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	وقتی ((parent = ext2fs_rb_parent(گره)) && گره == parent->rb_راست)
		گره = parent;

	بازگشت parent;
}

ساختار rb_گره *ext2fs_rb_prev(ساختار rb_گره *گره)
{
	ساختار rb_گره *parent;

	اگر (ext2fs_rb_parent(گره) == گره)
		بازگشت پوچ;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	اگر (گره->rb_چپ) {
		گره = گره->rb_چپ;
		وقتی (گره->rb_راست)
			گره=گره->rb_راست;
		بازگشت (ساختار rb_گره *)گره;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	وقتی ((parent = ext2fs_rb_parent(گره)) && گره == parent->rb_چپ)
		گره = parent;

	بازگشت parent;
}

تهی ext2fs_rb_replace_گره(ساختار rb_گره *victim, ساختار rb_گره *new,
			  ساختار rb_ریشه *ریشه)
{
	ساختار rb_گره *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	اگر (parent) {
		اگر (victim == parent->rb_چپ)
			parent->rb_چپ = new;
		وگرنه
			parent->rb_راست = new;
	} وگرنه {
		ریشه->rb_گره = new;
	}
	اگر (victim->rb_چپ)
		ext2fs_rb_set_parent(victim->rb_چپ, new);
	اگر (victim->rb_راست)
		ext2fs_rb_set_parent(victim->rb_راست, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
