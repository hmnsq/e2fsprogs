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

ساكن فراغ __rb_rotate_left(بنية rb_ڳنڍ *ڳنڍ, بنية rb_پاڙ *پاڙ)
{
	بنية rb_ڳنڍ *ساڄو = ڳنڍ->rb_ساڄو;
	بنية rb_ڳنڍ *والد = ext2fs_rb_والد(ڳنڍ);

	إذا ((ڳنڍ->rb_ساڄو = ساڄو->rb_کاٻو))
		ext2fs_rb_سيٽ_والد(ساڄو->rb_کاٻو, ڳنڍ);
	ساڄو->rb_کاٻو = ڳنڍ;

	ext2fs_rb_سيٽ_والد(ساڄو, والد);

	إذا (والد)
	{
		إذا (ڳنڍ == والد->rb_کاٻو)
			والد->rb_کاٻو = ساڄو;
		وإلا
			والد->rb_ساڄو = ساڄو;
	}
	وإلا
		پاڙ->rb_ڳنڍ = ساڄو;
	ext2fs_rb_سيٽ_والد(ڳنڍ, ساڄو);
}

ساكن فراغ __rb_rotate_right(بنية rb_ڳنڍ *ڳنڍ, بنية rb_پاڙ *پاڙ)
{
	بنية rb_ڳنڍ *کاٻو = ڳنڍ->rb_کاٻو;
	بنية rb_ڳنڍ *والد = ext2fs_rb_والد(ڳنڍ);

	إذا ((ڳنڍ->rb_کاٻو = کاٻو->rb_ساڄو))
		ext2fs_rb_سيٽ_والد(کاٻو->rb_ساڄو, ڳنڍ);
	کاٻو->rb_ساڄو = ڳنڍ;

	ext2fs_rb_سيٽ_والد(کاٻو, والد);

	إذا (والد)
	{
		إذا (ڳنڍ == والد->rb_ساڄو)
			والد->rb_ساڄو = کاٻو;
		وإلا
			والد->rb_کاٻو = کاٻو;
	}
	وإلا
		پاڙ->rb_ڳنڍ = کاٻو;
	ext2fs_rb_سيٽ_والد(ڳنڍ, کاٻو);
}

فراغ ext2fs_rb_وجهو_color(بنية rb_ڳنڍ *ڳنڍ, بنية rb_پاڙ *پاڙ)
{
	بنية rb_ڳنڍ *والد, *gparent;

	طالما ((والد = ext2fs_rb_والد(ڳنڍ)) && ext2fs_rb_is_red(والد))
	{
		gparent = ext2fs_rb_والد(والد);

		إذا (والد == gparent->rb_کاٻو)
		{
			{
				سجل بنية rb_ڳنڍ *uncle = gparent->rb_ساڄو;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_سيٽ_black(uncle);
					ext2fs_rb_سيٽ_black(والد);
					ext2fs_rb_سيٽ_red(gparent);
					ڳنڍ = gparent;
					استمر;
				}
			}

			إذا (والد->rb_ساڄو == ڳنڍ)
			{
				سجل بنية rb_ڳنڍ *tmp;
				__rb_rotate_left(والد, پاڙ);
				tmp = والد;
				والد = ڳنڍ;
				ڳنڍ = tmp;
			}

			ext2fs_rb_سيٽ_black(والد);
			ext2fs_rb_سيٽ_red(gparent);
			__rb_rotate_right(gparent, پاڙ);
		} وإلا {
			{
				سجل بنية rb_ڳنڍ *uncle = gparent->rb_کاٻو;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_سيٽ_black(uncle);
					ext2fs_rb_سيٽ_black(والد);
					ext2fs_rb_سيٽ_red(gparent);
					ڳنڍ = gparent;
					استمر;
				}
			}

			إذا (والد->rb_کاٻو == ڳنڍ)
			{
				سجل بنية rb_ڳنڍ *tmp;
				__rb_rotate_right(والد, پاڙ);
				tmp = والد;
				والد = ڳنڍ;
				ڳنڍ = tmp;
			}

			ext2fs_rb_سيٽ_black(والد);
			ext2fs_rb_سيٽ_red(gparent);
			__rb_rotate_left(gparent, پاڙ);
		}
	}

	ext2fs_rb_سيٽ_black(پاڙ->rb_ڳنڍ);
}

ساكن فراغ __rb_erase_color(بنية rb_ڳنڍ *ڳنڍ, بنية rb_ڳنڍ *والد,
			     بنية rb_پاڙ *پاڙ)
{
	بنية rb_ڳنڍ *other;

	طالما ((!ڳنڍ || ext2fs_rb_is_black(ڳنڍ)) && ڳنڍ != پاڙ->rb_ڳنڍ)
	{
		إذا (والد->rb_کاٻو == ڳنڍ)
		{
			other = والد->rb_ساڄو;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_سيٽ_black(other);
				ext2fs_rb_سيٽ_red(والد);
				__rb_rotate_left(والد, پاڙ);
				other = والد->rb_ساڄو;
			}
			إذا ((!other->rb_کاٻو || ext2fs_rb_is_black(other->rb_کاٻو)) &&
			    (!other->rb_ساڄو || ext2fs_rb_is_black(other->rb_ساڄو)))
			{
				ext2fs_rb_سيٽ_red(other);
				ڳنڍ = والد;
				والد = ext2fs_rb_والد(ڳنڍ);
			}
			وإلا
			{
				إذا (!other->rb_ساڄو || ext2fs_rb_is_black(other->rb_ساڄو))
				{
					ext2fs_rb_سيٽ_black(other->rb_کاٻو);
					ext2fs_rb_سيٽ_red(other);
					__rb_rotate_right(other, پاڙ);
					other = والد->rb_ساڄو;
				}
				ext2fs_rb_سيٽ_color(other, ext2fs_rb_color(والد));
				ext2fs_rb_سيٽ_black(والد);
				ext2fs_rb_سيٽ_black(other->rb_ساڄو);
				__rb_rotate_left(والد, پاڙ);
				ڳنڍ = پاڙ->rb_ڳنڍ;
				اكسر;
			}
		}
		وإلا
		{
			other = والد->rb_کاٻو;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_سيٽ_black(other);
				ext2fs_rb_سيٽ_red(والد);
				__rb_rotate_right(والد, پاڙ);
				other = والد->rb_کاٻو;
			}
			إذا ((!other->rb_کاٻو || ext2fs_rb_is_black(other->rb_کاٻو)) &&
			    (!other->rb_ساڄو || ext2fs_rb_is_black(other->rb_ساڄو)))
			{
				ext2fs_rb_سيٽ_red(other);
				ڳنڍ = والد;
				والد = ext2fs_rb_والد(ڳنڍ);
			}
			وإلا
			{
				إذا (!other->rb_کاٻو || ext2fs_rb_is_black(other->rb_کاٻو))
				{
					ext2fs_rb_سيٽ_black(other->rb_ساڄو);
					ext2fs_rb_سيٽ_red(other);
					__rb_rotate_left(other, پاڙ);
					other = والد->rb_کاٻو;
				}
				ext2fs_rb_سيٽ_color(other, ext2fs_rb_color(والد));
				ext2fs_rb_سيٽ_black(والد);
				ext2fs_rb_سيٽ_black(other->rb_کاٻو);
				__rb_rotate_right(والد, پاڙ);
				ڳنڍ = پاڙ->rb_ڳنڍ;
				اكسر;
			}
		}
	}
	إذا (ڳنڍ)
		ext2fs_rb_سيٽ_black(ڳنڍ);
}

فراغ ext2fs_rb_erase(بنية rb_ڳنڍ *ڳنڍ, بنية rb_پاڙ *پاڙ)
{
	بنية rb_ڳنڍ *ٻار, *والد;
	صحيح color;

	إذا (!ڳنڍ->rb_کاٻو)
		ٻار = ڳنڍ->rb_ساڄو;
	وإلا إذا (!ڳنڍ->rb_ساڄو)
		ٻار = ڳنڍ->rb_کاٻو;
	وإلا
	{
		بنية rb_ڳنڍ *old = ڳنڍ, *کاٻو;

		ڳنڍ = ڳنڍ->rb_ساڄو;
		طالما ((کاٻو = ڳنڍ->rb_کاٻو) != عدم)
			ڳنڍ = کاٻو;

		إذا (ext2fs_rb_والد(old)) {
			إذا (ext2fs_rb_والد(old)->rb_کاٻو == old)
				ext2fs_rb_والد(old)->rb_کاٻو = ڳنڍ;
			وإلا
				ext2fs_rb_والد(old)->rb_ساڄو = ڳنڍ;
		} وإلا
			پاڙ->rb_ڳنڍ = ڳنڍ;

		ٻار = ڳنڍ->rb_ساڄو;
		والد = ext2fs_rb_والد(ڳنڍ);
		color = ext2fs_rb_color(ڳنڍ);

		إذا (والد == old) {
			والد = ڳنڍ;
		} وإلا {
			إذا (ٻار)
				ext2fs_rb_سيٽ_والد(ٻار, والد);
			والد->rb_کاٻو = ٻار;

			ڳنڍ->rb_ساڄو = old->rb_ساڄو;
			ext2fs_rb_سيٽ_والد(old->rb_ساڄو, ڳنڍ);
		}

		ڳنڍ->rb_والد_color = old->rb_والد_color;
		ڳنڍ->rb_کاٻو = old->rb_کاٻو;
		ext2fs_rb_سيٽ_والد(old->rb_کاٻو, ڳنڍ);

		اذهب color;
	}

	والد = ext2fs_rb_والد(ڳنڍ);
	color = ext2fs_rb_color(ڳنڍ);

	إذا (ٻار)
		ext2fs_rb_سيٽ_والد(ٻار, والد);
	إذا (والد)
	{
		إذا (والد->rb_کاٻو == ڳنڍ)
			والد->rb_کاٻو = ٻار;
		وإلا
			والد->rb_ساڄو = ٻار;
	}
	وإلا
		پاڙ->rb_ڳنڍ = ٻار;

 color:
	إذا (color == RB_BLACK)
		__rb_erase_color(ٻار, والد, پاڙ);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_ڳنڍ *ext2fs_rb_پهريون(ثابت بنية rb_پاڙ *پاڙ)
{
	بنية rb_ڳنڍ	*n;

	n = پاڙ->rb_ڳنڍ;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_کاٻو)
		n = n->rb_کاٻو;
	أرجع n;
}

بنية rb_ڳنڍ *ext2fs_rb_آخري(ثابت بنية rb_پاڙ *پاڙ)
{
	بنية rb_ڳنڍ	*n;

	n = پاڙ->rb_ڳنڍ;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_ساڄو)
		n = n->rb_ساڄو;
	أرجع n;
}

بنية rb_ڳنڍ *ext2fs_rb_ايندڙ(بنية rb_ڳنڍ *ڳنڍ)
{
	بنية rb_ڳنڍ *والد;

	إذا (ext2fs_rb_والد(ڳنڍ) == ڳنڍ)
		أرجع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	إذا (ڳنڍ->rb_ساڄو) {
		ڳنڍ = ڳنڍ->rb_ساڄو;
		طالما (ڳنڍ->rb_کاٻو)
			ڳنڍ=ڳنڍ->rb_کاٻو;
		أرجع (بنية rb_ڳنڍ *)ڳنڍ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	طالما ((والد = ext2fs_rb_والد(ڳنڍ)) && ڳنڍ == والد->rb_ساڄو)
		ڳنڍ = والد;

	أرجع والد;
}

بنية rb_ڳنڍ *ext2fs_rb_prev(بنية rb_ڳنڍ *ڳنڍ)
{
	بنية rb_ڳنڍ *والد;

	إذا (ext2fs_rb_والد(ڳنڍ) == ڳنڍ)
		أرجع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	إذا (ڳنڍ->rb_کاٻو) {
		ڳنڍ = ڳنڍ->rb_کاٻو;
		طالما (ڳنڍ->rb_ساڄو)
			ڳنڍ=ڳنڍ->rb_ساڄو;
		أرجع (بنية rb_ڳنڍ *)ڳنڍ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	طالما ((والد = ext2fs_rb_والد(ڳنڍ)) && ڳنڍ == والد->rb_کاٻو)
		ڳنڍ = والد;

	أرجع والد;
}

فراغ ext2fs_rb_replace_ڳنڍ(بنية rb_ڳنڍ *victim, بنية rb_ڳنڍ *new,
			  بنية rb_پاڙ *پاڙ)
{
	بنية rb_ڳنڍ *والد = ext2fs_rb_والد(victim);

	/* Set the surrounding nodes to point to the replacement */
	إذا (والد) {
		إذا (victim == والد->rb_کاٻو)
			والد->rb_کاٻو = new;
		وإلا
			والد->rb_ساڄو = new;
	} وإلا {
		پاڙ->rb_ڳنڍ = new;
	}
	إذا (victim->rb_کاٻو)
		ext2fs_rb_سيٽ_والد(victim->rb_کاٻو, new);
	إذا (victim->rb_ساڄو)
		ext2fs_rb_سيٽ_والد(victim->rb_ساڄو, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
