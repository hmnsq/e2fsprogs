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

ساکن خالی __rb_rotate_left(ساخت rb_گرہ *گرہ, ساخت rb_جڑ *جڑ)
{
	ساخت rb_گرہ *دایاں = گرہ->rb_دایاں;
	ساخت rb_گرہ *والد = ext2fs_rb_والد(گرہ);

	اگر ((گرہ->rb_دایاں = دایاں->rb_بایاں))
		ext2fs_rb_سیٹ_والد(دایاں->rb_بایاں, گرہ);
	دایاں->rb_بایاں = گرہ;

	ext2fs_rb_سیٹ_والد(دایاں, والد);

	اگر (والد)
	{
		اگر (گرہ == والد->rb_بایاں)
			والد->rb_بایاں = دایاں;
		ورنہ
			والد->rb_دایاں = دایاں;
	}
	ورنہ
		جڑ->rb_گرہ = دایاں;
	ext2fs_rb_سیٹ_والد(گرہ, دایاں);
}

ساکن خالی __rb_rotate_right(ساخت rb_گرہ *گرہ, ساخت rb_جڑ *جڑ)
{
	ساخت rb_گرہ *بایاں = گرہ->rb_بایاں;
	ساخت rb_گرہ *والد = ext2fs_rb_والد(گرہ);

	اگر ((گرہ->rb_بایاں = بایاں->rb_دایاں))
		ext2fs_rb_سیٹ_والد(بایاں->rb_دایاں, گرہ);
	بایاں->rb_دایاں = گرہ;

	ext2fs_rb_سیٹ_والد(بایاں, والد);

	اگر (والد)
	{
		اگر (گرہ == والد->rb_دایاں)
			والد->rb_دایاں = بایاں;
		ورنہ
			والد->rb_بایاں = بایاں;
	}
	ورنہ
		جڑ->rb_گرہ = بایاں;
	ext2fs_rb_سیٹ_والد(گرہ, بایاں);
}

خالی ext2fs_rb_داخل_کرو_color(ساخت rb_گرہ *گرہ, ساخت rb_جڑ *جڑ)
{
	ساخت rb_گرہ *والد, *gparent;

	جب ((والد = ext2fs_rb_والد(گرہ)) && ext2fs_rb_is_red(والد))
	{
		gparent = ext2fs_rb_والد(والد);

		اگر (والد == gparent->rb_بایاں)
		{
			{
				اندراج ساخت rb_گرہ *uncle = gparent->rb_دایاں;
				اگر (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_سیٹ_black(uncle);
					ext2fs_rb_سیٹ_black(والد);
					ext2fs_rb_سیٹ_red(gparent);
					گرہ = gparent;
					جاری;
				}
			}

			اگر (والد->rb_دایاں == گرہ)
			{
				اندراج ساخت rb_گرہ *tmp;
				__rb_rotate_left(والد, جڑ);
				tmp = والد;
				والد = گرہ;
				گرہ = tmp;
			}

			ext2fs_rb_سیٹ_black(والد);
			ext2fs_rb_سیٹ_red(gparent);
			__rb_rotate_right(gparent, جڑ);
		} ورنہ {
			{
				اندراج ساخت rb_گرہ *uncle = gparent->rb_بایاں;
				اگر (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_سیٹ_black(uncle);
					ext2fs_rb_سیٹ_black(والد);
					ext2fs_rb_سیٹ_red(gparent);
					گرہ = gparent;
					جاری;
				}
			}

			اگر (والد->rb_بایاں == گرہ)
			{
				اندراج ساخت rb_گرہ *tmp;
				__rb_rotate_right(والد, جڑ);
				tmp = والد;
				والد = گرہ;
				گرہ = tmp;
			}

			ext2fs_rb_سیٹ_black(والد);
			ext2fs_rb_سیٹ_red(gparent);
			__rb_rotate_left(gparent, جڑ);
		}
	}

	ext2fs_rb_سیٹ_black(جڑ->rb_گرہ);
}

ساکن خالی __rb_erase_color(ساخت rb_گرہ *گرہ, ساخت rb_گرہ *والد,
			     ساخت rb_جڑ *جڑ)
{
	ساخت rb_گرہ *other;

	جب ((!گرہ || ext2fs_rb_is_black(گرہ)) && گرہ != جڑ->rb_گرہ)
	{
		اگر (والد->rb_بایاں == گرہ)
		{
			other = والد->rb_دایاں;
			اگر (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_سیٹ_black(other);
				ext2fs_rb_سیٹ_red(والد);
				__rb_rotate_left(والد, جڑ);
				other = والد->rb_دایاں;
			}
			اگر ((!other->rb_بایاں || ext2fs_rb_is_black(other->rb_بایاں)) &&
			    (!other->rb_دایاں || ext2fs_rb_is_black(other->rb_دایاں)))
			{
				ext2fs_rb_سیٹ_red(other);
				گرہ = والد;
				والد = ext2fs_rb_والد(گرہ);
			}
			ورنہ
			{
				اگر (!other->rb_دایاں || ext2fs_rb_is_black(other->rb_دایاں))
				{
					ext2fs_rb_سیٹ_black(other->rb_بایاں);
					ext2fs_rb_سیٹ_red(other);
					__rb_rotate_right(other, جڑ);
					other = والد->rb_دایاں;
				}
				ext2fs_rb_سیٹ_color(other, ext2fs_rb_color(والد));
				ext2fs_rb_سیٹ_black(والد);
				ext2fs_rb_سیٹ_black(other->rb_دایاں);
				__rb_rotate_left(والد, جڑ);
				گرہ = جڑ->rb_گرہ;
				توڑو;
			}
		}
		ورنہ
		{
			other = والد->rb_بایاں;
			اگر (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_سیٹ_black(other);
				ext2fs_rb_سیٹ_red(والد);
				__rb_rotate_right(والد, جڑ);
				other = والد->rb_بایاں;
			}
			اگر ((!other->rb_بایاں || ext2fs_rb_is_black(other->rb_بایاں)) &&
			    (!other->rb_دایاں || ext2fs_rb_is_black(other->rb_دایاں)))
			{
				ext2fs_rb_سیٹ_red(other);
				گرہ = والد;
				والد = ext2fs_rb_والد(گرہ);
			}
			ورنہ
			{
				اگر (!other->rb_بایاں || ext2fs_rb_is_black(other->rb_بایاں))
				{
					ext2fs_rb_سیٹ_black(other->rb_دایاں);
					ext2fs_rb_سیٹ_red(other);
					__rb_rotate_left(other, جڑ);
					other = والد->rb_بایاں;
				}
				ext2fs_rb_سیٹ_color(other, ext2fs_rb_color(والد));
				ext2fs_rb_سیٹ_black(والد);
				ext2fs_rb_سیٹ_black(other->rb_بایاں);
				__rb_rotate_right(والد, جڑ);
				گرہ = جڑ->rb_گرہ;
				توڑو;
			}
		}
	}
	اگر (گرہ)
		ext2fs_rb_سیٹ_black(گرہ);
}

خالی ext2fs_rb_erase(ساخت rb_گرہ *گرہ, ساخت rb_جڑ *جڑ)
{
	ساخت rb_گرہ *بچہ, *والد;
	صحیح color;

	اگر (!گرہ->rb_بایاں)
		بچہ = گرہ->rb_دایاں;
	ورنہ اگر (!گرہ->rb_دایاں)
		بچہ = گرہ->rb_بایاں;
	ورنہ
	{
		ساخت rb_گرہ *old = گرہ, *بایاں;

		گرہ = گرہ->rb_دایاں;
		جب ((بایاں = گرہ->rb_بایاں) != تہی)
			گرہ = بایاں;

		اگر (ext2fs_rb_والد(old)) {
			اگر (ext2fs_rb_والد(old)->rb_بایاں == old)
				ext2fs_rb_والد(old)->rb_بایاں = گرہ;
			ورنہ
				ext2fs_rb_والد(old)->rb_دایاں = گرہ;
		} ورنہ
			جڑ->rb_گرہ = گرہ;

		بچہ = گرہ->rb_دایاں;
		والد = ext2fs_rb_والد(گرہ);
		color = ext2fs_rb_color(گرہ);

		اگر (والد == old) {
			والد = گرہ;
		} ورنہ {
			اگر (بچہ)
				ext2fs_rb_سیٹ_والد(بچہ, والد);
			والد->rb_بایاں = بچہ;

			گرہ->rb_دایاں = old->rb_دایاں;
			ext2fs_rb_سیٹ_والد(old->rb_دایاں, گرہ);
		}

		گرہ->rb_والد_color = old->rb_والد_color;
		گرہ->rb_بایاں = old->rb_بایاں;
		ext2fs_rb_سیٹ_والد(old->rb_بایاں, گرہ);

		جاؤ color;
	}

	والد = ext2fs_rb_والد(گرہ);
	color = ext2fs_rb_color(گرہ);

	اگر (بچہ)
		ext2fs_rb_سیٹ_والد(بچہ, والد);
	اگر (والد)
	{
		اگر (والد->rb_بایاں == گرہ)
			والد->rb_بایاں = بچہ;
		ورنہ
			والد->rb_دایاں = بچہ;
	}
	ورنہ
		جڑ->rb_گرہ = بچہ;

 color:
	اگر (color == RB_BLACK)
		__rb_erase_color(بچہ, والد, جڑ);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
ساخت rb_گرہ *ext2fs_rb_پہلا(ثابت ساخت rb_جڑ *جڑ)
{
	ساخت rb_گرہ	*n;

	n = جڑ->rb_گرہ;
	اگر (!n)
		واپس تہی;
	جب (n->rb_بایاں)
		n = n->rb_بایاں;
	واپس n;
}

ساخت rb_گرہ *ext2fs_rb_آخری(ثابت ساخت rb_جڑ *جڑ)
{
	ساخت rb_گرہ	*n;

	n = جڑ->rb_گرہ;
	اگر (!n)
		واپس تہی;
	جب (n->rb_دایاں)
		n = n->rb_دایاں;
	واپس n;
}

ساخت rb_گرہ *ext2fs_rb_اگلا(ساخت rb_گرہ *گرہ)
{
	ساخت rb_گرہ *والد;

	اگر (ext2fs_rb_والد(گرہ) == گرہ)
		واپس تہی;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	اگر (گرہ->rb_دایاں) {
		گرہ = گرہ->rb_دایاں;
		جب (گرہ->rb_بایاں)
			گرہ=گرہ->rb_بایاں;
		واپس (ساخت rb_گرہ *)گرہ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	جب ((والد = ext2fs_rb_والد(گرہ)) && گرہ == والد->rb_دایاں)
		گرہ = والد;

	واپس والد;
}

ساخت rb_گرہ *ext2fs_rb_پچھلا(ساخت rb_گرہ *گرہ)
{
	ساخت rb_گرہ *والد;

	اگر (ext2fs_rb_والد(گرہ) == گرہ)
		واپس تہی;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	اگر (گرہ->rb_بایاں) {
		گرہ = گرہ->rb_بایاں;
		جب (گرہ->rb_دایاں)
			گرہ=گرہ->rb_دایاں;
		واپس (ساخت rb_گرہ *)گرہ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	جب ((والد = ext2fs_rb_والد(گرہ)) && گرہ == والد->rb_بایاں)
		گرہ = والد;

	واپس والد;
}

خالی ext2fs_rb_replace_گرہ(ساخت rb_گرہ *victim, ساخت rb_گرہ *new,
			  ساخت rb_جڑ *جڑ)
{
	ساخت rb_گرہ *والد = ext2fs_rb_والد(victim);

	/* Set the surrounding nodes to point to the replacement */
	اگر (والد) {
		اگر (victim == والد->rb_بایاں)
			والد->rb_بایاں = new;
		ورنہ
			والد->rb_دایاں = new;
	} ورنہ {
		جڑ->rb_گرہ = new;
	}
	اگر (victim->rb_بایاں)
		ext2fs_rb_سیٹ_والد(victim->rb_بایاں, new);
	اگر (victim->rb_دایاں)
		ext2fs_rb_سیٹ_والد(victim->rb_دایاں, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
