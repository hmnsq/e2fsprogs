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

ساكن فراغ __rb_rotate_left(بنية rb_گره *گره, بنية rb_ریشه *ریشه)
{
	بنية rb_گره *راست = گره->rb_راست;
	بنية rb_گره *والد = ext2fs_rb_والد(گره);

	إذا ((گره->rb_راست = راست->rb_چپ))
		ext2fs_rb_مجموعه_والد(راست->rb_چپ, گره);
	راست->rb_چپ = گره;

	ext2fs_rb_مجموعه_والد(راست, والد);

	إذا (والد)
	{
		إذا (گره == والد->rb_چپ)
			والد->rb_چپ = راست;
		وإلا
			والد->rb_راست = راست;
	}
	وإلا
		ریشه->rb_گره = راست;
	ext2fs_rb_مجموعه_والد(گره, راست);
}

ساكن فراغ __rb_rotate_right(بنية rb_گره *گره, بنية rb_ریشه *ریشه)
{
	بنية rb_گره *چپ = گره->rb_چپ;
	بنية rb_گره *والد = ext2fs_rb_والد(گره);

	إذا ((گره->rb_چپ = چپ->rb_راست))
		ext2fs_rb_مجموعه_والد(چپ->rb_راست, گره);
	چپ->rb_راست = گره;

	ext2fs_rb_مجموعه_والد(چپ, والد);

	إذا (والد)
	{
		إذا (گره == والد->rb_راست)
			والد->rb_راست = چپ;
		وإلا
			والد->rb_چپ = چپ;
	}
	وإلا
		ریشه->rb_گره = چپ;
	ext2fs_rb_مجموعه_والد(گره, چپ);
}

فراغ ext2fs_rb_درج_color(بنية rb_گره *گره, بنية rb_ریشه *ریشه)
{
	بنية rb_گره *والد, *gparent;

	طالما ((والد = ext2fs_rb_والد(گره)) && ext2fs_rb_is_red(والد))
	{
		gparent = ext2fs_rb_والد(والد);

		إذا (والد == gparent->rb_چپ)
		{
			{
				سجل بنية rb_گره *uncle = gparent->rb_راست;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_مجموعه_black(uncle);
					ext2fs_rb_مجموعه_black(والد);
					ext2fs_rb_مجموعه_red(gparent);
					گره = gparent;
					استمر;
				}
			}

			إذا (والد->rb_راست == گره)
			{
				سجل بنية rb_گره *tmp;
				__rb_rotate_left(والد, ریشه);
				tmp = والد;
				والد = گره;
				گره = tmp;
			}

			ext2fs_rb_مجموعه_black(والد);
			ext2fs_rb_مجموعه_red(gparent);
			__rb_rotate_right(gparent, ریشه);
		} وإلا {
			{
				سجل بنية rb_گره *uncle = gparent->rb_چپ;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_مجموعه_black(uncle);
					ext2fs_rb_مجموعه_black(والد);
					ext2fs_rb_مجموعه_red(gparent);
					گره = gparent;
					استمر;
				}
			}

			إذا (والد->rb_چپ == گره)
			{
				سجل بنية rb_گره *tmp;
				__rb_rotate_right(والد, ریشه);
				tmp = والد;
				والد = گره;
				گره = tmp;
			}

			ext2fs_rb_مجموعه_black(والد);
			ext2fs_rb_مجموعه_red(gparent);
			__rb_rotate_left(gparent, ریشه);
		}
	}

	ext2fs_rb_مجموعه_black(ریشه->rb_گره);
}

ساكن فراغ __rb_erase_color(بنية rb_گره *گره, بنية rb_گره *والد,
			     بنية rb_ریشه *ریشه)
{
	بنية rb_گره *other;

	طالما ((!گره || ext2fs_rb_is_black(گره)) && گره != ریشه->rb_گره)
	{
		إذا (والد->rb_چپ == گره)
		{
			other = والد->rb_راست;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_مجموعه_black(other);
				ext2fs_rb_مجموعه_red(والد);
				__rb_rotate_left(والد, ریشه);
				other = والد->rb_راست;
			}
			إذا ((!other->rb_چپ || ext2fs_rb_is_black(other->rb_چپ)) &&
			    (!other->rb_راست || ext2fs_rb_is_black(other->rb_راست)))
			{
				ext2fs_rb_مجموعه_red(other);
				گره = والد;
				والد = ext2fs_rb_والد(گره);
			}
			وإلا
			{
				إذا (!other->rb_راست || ext2fs_rb_is_black(other->rb_راست))
				{
					ext2fs_rb_مجموعه_black(other->rb_چپ);
					ext2fs_rb_مجموعه_red(other);
					__rb_rotate_right(other, ریشه);
					other = والد->rb_راست;
				}
				ext2fs_rb_مجموعه_color(other, ext2fs_rb_color(والد));
				ext2fs_rb_مجموعه_black(والد);
				ext2fs_rb_مجموعه_black(other->rb_راست);
				__rb_rotate_left(والد, ریشه);
				گره = ریشه->rb_گره;
				اكسر;
			}
		}
		وإلا
		{
			other = والد->rb_چپ;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_مجموعه_black(other);
				ext2fs_rb_مجموعه_red(والد);
				__rb_rotate_right(والد, ریشه);
				other = والد->rb_چپ;
			}
			إذا ((!other->rb_چپ || ext2fs_rb_is_black(other->rb_چپ)) &&
			    (!other->rb_راست || ext2fs_rb_is_black(other->rb_راست)))
			{
				ext2fs_rb_مجموعه_red(other);
				گره = والد;
				والد = ext2fs_rb_والد(گره);
			}
			وإلا
			{
				إذا (!other->rb_چپ || ext2fs_rb_is_black(other->rb_چپ))
				{
					ext2fs_rb_مجموعه_black(other->rb_راست);
					ext2fs_rb_مجموعه_red(other);
					__rb_rotate_left(other, ریشه);
					other = والد->rb_چپ;
				}
				ext2fs_rb_مجموعه_color(other, ext2fs_rb_color(والد));
				ext2fs_rb_مجموعه_black(والد);
				ext2fs_rb_مجموعه_black(other->rb_چپ);
				__rb_rotate_right(والد, ریشه);
				گره = ریشه->rb_گره;
				اكسر;
			}
		}
	}
	إذا (گره)
		ext2fs_rb_مجموعه_black(گره);
}

فراغ ext2fs_rb_erase(بنية rb_گره *گره, بنية rb_ریشه *ریشه)
{
	بنية rb_گره *فرزند, *والد;
	صحيح color;

	إذا (!گره->rb_چپ)
		فرزند = گره->rb_راست;
	وإلا إذا (!گره->rb_راست)
		فرزند = گره->rb_چپ;
	وإلا
	{
		بنية rb_گره *old = گره, *چپ;

		گره = گره->rb_راست;
		طالما ((چپ = گره->rb_چپ) != عدم)
			گره = چپ;

		إذا (ext2fs_rb_والد(old)) {
			إذا (ext2fs_rb_والد(old)->rb_چپ == old)
				ext2fs_rb_والد(old)->rb_چپ = گره;
			وإلا
				ext2fs_rb_والد(old)->rb_راست = گره;
		} وإلا
			ریشه->rb_گره = گره;

		فرزند = گره->rb_راست;
		والد = ext2fs_rb_والد(گره);
		color = ext2fs_rb_color(گره);

		إذا (والد == old) {
			والد = گره;
		} وإلا {
			إذا (فرزند)
				ext2fs_rb_مجموعه_والد(فرزند, والد);
			والد->rb_چپ = فرزند;

			گره->rb_راست = old->rb_راست;
			ext2fs_rb_مجموعه_والد(old->rb_راست, گره);
		}

		گره->rb_والد_color = old->rb_والد_color;
		گره->rb_چپ = old->rb_چپ;
		ext2fs_rb_مجموعه_والد(old->rb_چپ, گره);

		اذهب color;
	}

	والد = ext2fs_rb_والد(گره);
	color = ext2fs_rb_color(گره);

	إذا (فرزند)
		ext2fs_rb_مجموعه_والد(فرزند, والد);
	إذا (والد)
	{
		إذا (والد->rb_چپ == گره)
			والد->rb_چپ = فرزند;
		وإلا
			والد->rb_راست = فرزند;
	}
	وإلا
		ریشه->rb_گره = فرزند;

 color:
	إذا (color == RB_BLACK)
		__rb_erase_color(فرزند, والد, ریشه);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_گره *ext2fs_rb_اولی(ثابت بنية rb_ریشه *ریشه)
{
	بنية rb_گره	*n;

	n = ریشه->rb_گره;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_چپ)
		n = n->rb_چپ;
	أرجع n;
}

بنية rb_گره *ext2fs_rb_آخرین(ثابت بنية rb_ریشه *ریشه)
{
	بنية rb_گره	*n;

	n = ریشه->rb_گره;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_راست)
		n = n->rb_راست;
	أرجع n;
}

بنية rb_گره *ext2fs_rb_بعدی(بنية rb_گره *گره)
{
	بنية rb_گره *والد;

	إذا (ext2fs_rb_والد(گره) == گره)
		أرجع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	إذا (گره->rb_راست) {
		گره = گره->rb_راست;
		طالما (گره->rb_چپ)
			گره=گره->rb_چپ;
		أرجع (بنية rb_گره *)گره;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	طالما ((والد = ext2fs_rb_والد(گره)) && گره == والد->rb_راست)
		گره = والد;

	أرجع والد;
}

بنية rb_گره *ext2fs_rb_prev(بنية rb_گره *گره)
{
	بنية rb_گره *والد;

	إذا (ext2fs_rb_والد(گره) == گره)
		أرجع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	إذا (گره->rb_چپ) {
		گره = گره->rb_چپ;
		طالما (گره->rb_راست)
			گره=گره->rb_راست;
		أرجع (بنية rb_گره *)گره;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	طالما ((والد = ext2fs_rb_والد(گره)) && گره == والد->rb_چپ)
		گره = والد;

	أرجع والد;
}

فراغ ext2fs_rb_replace_گره(بنية rb_گره *victim, بنية rb_گره *new,
			  بنية rb_ریشه *ریشه)
{
	بنية rb_گره *والد = ext2fs_rb_والد(victim);

	/* Set the surrounding nodes to point to the replacement */
	إذا (والد) {
		إذا (victim == والد->rb_چپ)
			والد->rb_چپ = new;
		وإلا
			والد->rb_راست = new;
	} وإلا {
		ریشه->rb_گره = new;
	}
	إذا (victim->rb_چپ)
		ext2fs_rb_مجموعه_والد(victim->rb_چپ, new);
	إذا (victim->rb_راست)
		ext2fs_rb_مجموعه_والد(victim->rb_راست, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
