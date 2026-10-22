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

ساكن فراغ __rb_rotate_left(بنية rb_گرہ *گرہ, بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ *دایاں = گرہ->rb_دایاں;
	بنية rb_گرہ *ماپا = ext2fs_rb_ماپا(گرہ);

	إذا ((گرہ->rb_دایاں = دایاں->rb_بایاں))
		ext2fs_rb_سیٹ_ماپا(دایاں->rb_بایاں, گرہ);
	دایاں->rb_بایاں = گرہ;

	ext2fs_rb_سیٹ_ماپا(دایاں, ماپا);

	إذا (ماپا)
	{
		إذا (گرہ == ماپا->rb_بایاں)
			ماپا->rb_بایاں = دایاں;
		وإلا
			ماپا->rb_دایاں = دایاں;
	}
	وإلا
		جڑ->rb_گرہ = دایاں;
	ext2fs_rb_سیٹ_ماپا(گرہ, دایاں);
}

ساكن فراغ __rb_rotate_right(بنية rb_گرہ *گرہ, بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ *بایاں = گرہ->rb_بایاں;
	بنية rb_گرہ *ماپا = ext2fs_rb_ماپا(گرہ);

	إذا ((گرہ->rb_بایاں = بایاں->rb_دایاں))
		ext2fs_rb_سیٹ_ماپا(بایاں->rb_دایاں, گرہ);
	بایاں->rb_دایاں = گرہ;

	ext2fs_rb_سیٹ_ماپا(بایاں, ماپا);

	إذا (ماپا)
	{
		إذا (گرہ == ماپا->rb_دایاں)
			ماپا->rb_دایاں = بایاں;
		وإلا
			ماپا->rb_بایاں = بایاں;
	}
	وإلا
		جڑ->rb_گرہ = بایاں;
	ext2fs_rb_سیٹ_ماپا(گرہ, بایاں);
}

فراغ ext2fs_rb_داخل_کرو_color(بنية rb_گرہ *گرہ, بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ *ماپا, *gparent;

	طالما ((ماپا = ext2fs_rb_ماپا(گرہ)) && ext2fs_rb_is_red(ماپا))
	{
		gparent = ext2fs_rb_ماپا(ماپا);

		إذا (ماپا == gparent->rb_بایاں)
		{
			{
				سجل بنية rb_گرہ *uncle = gparent->rb_دایاں;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_سیٹ_black(uncle);
					ext2fs_rb_سیٹ_black(ماپا);
					ext2fs_rb_سیٹ_red(gparent);
					گرہ = gparent;
					استمر;
				}
			}

			إذا (ماپا->rb_دایاں == گرہ)
			{
				سجل بنية rb_گرہ *tmp;
				__rb_rotate_left(ماپا, جڑ);
				tmp = ماپا;
				ماپا = گرہ;
				گرہ = tmp;
			}

			ext2fs_rb_سیٹ_black(ماپا);
			ext2fs_rb_سیٹ_red(gparent);
			__rb_rotate_right(gparent, جڑ);
		} وإلا {
			{
				سجل بنية rb_گرہ *uncle = gparent->rb_بایاں;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_سیٹ_black(uncle);
					ext2fs_rb_سیٹ_black(ماپا);
					ext2fs_rb_سیٹ_red(gparent);
					گرہ = gparent;
					استمر;
				}
			}

			إذا (ماپا->rb_بایاں == گرہ)
			{
				سجل بنية rb_گرہ *tmp;
				__rb_rotate_right(ماپا, جڑ);
				tmp = ماپا;
				ماپا = گرہ;
				گرہ = tmp;
			}

			ext2fs_rb_سیٹ_black(ماپا);
			ext2fs_rb_سیٹ_red(gparent);
			__rb_rotate_left(gparent, جڑ);
		}
	}

	ext2fs_rb_سیٹ_black(جڑ->rb_گرہ);
}

ساكن فراغ __rb_erase_color(بنية rb_گرہ *گرہ, بنية rb_گرہ *ماپا,
			     بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ *other;

	طالما ((!گرہ || ext2fs_rb_is_black(گرہ)) && گرہ != جڑ->rb_گرہ)
	{
		إذا (ماپا->rb_بایاں == گرہ)
		{
			other = ماپا->rb_دایاں;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_سیٹ_black(other);
				ext2fs_rb_سیٹ_red(ماپا);
				__rb_rotate_left(ماپا, جڑ);
				other = ماپا->rb_دایاں;
			}
			إذا ((!other->rb_بایاں || ext2fs_rb_is_black(other->rb_بایاں)) &&
			    (!other->rb_دایاں || ext2fs_rb_is_black(other->rb_دایاں)))
			{
				ext2fs_rb_سیٹ_red(other);
				گرہ = ماپا;
				ماپا = ext2fs_rb_ماپا(گرہ);
			}
			وإلا
			{
				إذا (!other->rb_دایاں || ext2fs_rb_is_black(other->rb_دایاں))
				{
					ext2fs_rb_سیٹ_black(other->rb_بایاں);
					ext2fs_rb_سیٹ_red(other);
					__rb_rotate_right(other, جڑ);
					other = ماپا->rb_دایاں;
				}
				ext2fs_rb_سیٹ_color(other, ext2fs_rb_color(ماپا));
				ext2fs_rb_سیٹ_black(ماپا);
				ext2fs_rb_سیٹ_black(other->rb_دایاں);
				__rb_rotate_left(ماپا, جڑ);
				گرہ = جڑ->rb_گرہ;
				اكسر;
			}
		}
		وإلا
		{
			other = ماپا->rb_بایاں;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_سیٹ_black(other);
				ext2fs_rb_سیٹ_red(ماپا);
				__rb_rotate_right(ماپا, جڑ);
				other = ماپا->rb_بایاں;
			}
			إذا ((!other->rb_بایاں || ext2fs_rb_is_black(other->rb_بایاں)) &&
			    (!other->rb_دایاں || ext2fs_rb_is_black(other->rb_دایاں)))
			{
				ext2fs_rb_سیٹ_red(other);
				گرہ = ماپا;
				ماپا = ext2fs_rb_ماپا(گرہ);
			}
			وإلا
			{
				إذا (!other->rb_بایاں || ext2fs_rb_is_black(other->rb_بایاں))
				{
					ext2fs_rb_سیٹ_black(other->rb_دایاں);
					ext2fs_rb_سیٹ_red(other);
					__rb_rotate_left(other, جڑ);
					other = ماپا->rb_بایاں;
				}
				ext2fs_rb_سیٹ_color(other, ext2fs_rb_color(ماپا));
				ext2fs_rb_سیٹ_black(ماپا);
				ext2fs_rb_سیٹ_black(other->rb_بایاں);
				__rb_rotate_right(ماپا, جڑ);
				گرہ = جڑ->rb_گرہ;
				اكسر;
			}
		}
	}
	إذا (گرہ)
		ext2fs_rb_سیٹ_black(گرہ);
}

فراغ ext2fs_rb_erase(بنية rb_گرہ *گرہ, بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ *بچہ, *ماپا;
	صحيح color;

	إذا (!گرہ->rb_بایاں)
		بچہ = گرہ->rb_دایاں;
	وإلا إذا (!گرہ->rb_دایاں)
		بچہ = گرہ->rb_بایاں;
	وإلا
	{
		بنية rb_گرہ *old = گرہ, *بایاں;

		گرہ = گرہ->rb_دایاں;
		طالما ((بایاں = گرہ->rb_بایاں) != عدم)
			گرہ = بایاں;

		إذا (ext2fs_rb_ماپا(old)) {
			إذا (ext2fs_rb_ماپا(old)->rb_بایاں == old)
				ext2fs_rb_ماپا(old)->rb_بایاں = گرہ;
			وإلا
				ext2fs_rb_ماپا(old)->rb_دایاں = گرہ;
		} وإلا
			جڑ->rb_گرہ = گرہ;

		بچہ = گرہ->rb_دایاں;
		ماپا = ext2fs_rb_ماپا(گرہ);
		color = ext2fs_rb_color(گرہ);

		إذا (ماپا == old) {
			ماپا = گرہ;
		} وإلا {
			إذا (بچہ)
				ext2fs_rb_سیٹ_ماپا(بچہ, ماپا);
			ماپا->rb_بایاں = بچہ;

			گرہ->rb_دایاں = old->rb_دایاں;
			ext2fs_rb_سیٹ_ماپا(old->rb_دایاں, گرہ);
		}

		گرہ->rb_ماپا_color = old->rb_ماپا_color;
		گرہ->rb_بایاں = old->rb_بایاں;
		ext2fs_rb_سیٹ_ماپا(old->rb_بایاں, گرہ);

		اذهب color;
	}

	ماپا = ext2fs_rb_ماپا(گرہ);
	color = ext2fs_rb_color(گرہ);

	إذا (بچہ)
		ext2fs_rb_سیٹ_ماپا(بچہ, ماپا);
	إذا (ماپا)
	{
		إذا (ماپا->rb_بایاں == گرہ)
			ماپا->rb_بایاں = بچہ;
		وإلا
			ماپا->rb_دایاں = بچہ;
	}
	وإلا
		جڑ->rb_گرہ = بچہ;

 color:
	إذا (color == RB_BLACK)
		__rb_erase_color(بچہ, ماپا, جڑ);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_گرہ *ext2fs_rb_پہلا(ثابت بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ	*n;

	n = جڑ->rb_گرہ;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_بایاں)
		n = n->rb_بایاں;
	أرجع n;
}

بنية rb_گرہ *ext2fs_rb_آخری(ثابت بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ	*n;

	n = جڑ->rb_گرہ;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_دایاں)
		n = n->rb_دایاں;
	أرجع n;
}

بنية rb_گرہ *ext2fs_rb_اگلا(بنية rb_گرہ *گرہ)
{
	بنية rb_گرہ *ماپا;

	إذا (ext2fs_rb_ماپا(گرہ) == گرہ)
		أرجع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	إذا (گرہ->rb_دایاں) {
		گرہ = گرہ->rb_دایاں;
		طالما (گرہ->rb_بایاں)
			گرہ=گرہ->rb_بایاں;
		أرجع (بنية rb_گرہ *)گرہ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	طالما ((ماپا = ext2fs_rb_ماپا(گرہ)) && گرہ == ماپا->rb_دایاں)
		گرہ = ماپا;

	أرجع ماپا;
}

بنية rb_گرہ *ext2fs_rb_prev(بنية rb_گرہ *گرہ)
{
	بنية rb_گرہ *ماپا;

	إذا (ext2fs_rb_ماپا(گرہ) == گرہ)
		أرجع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	إذا (گرہ->rb_بایاں) {
		گرہ = گرہ->rb_بایاں;
		طالما (گرہ->rb_دایاں)
			گرہ=گرہ->rb_دایاں;
		أرجع (بنية rb_گرہ *)گرہ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	طالما ((ماپا = ext2fs_rb_ماپا(گرہ)) && گرہ == ماپا->rb_بایاں)
		گرہ = ماپا;

	أرجع ماپا;
}

فراغ ext2fs_rb_replace_گرہ(بنية rb_گرہ *victim, بنية rb_گرہ *new,
			  بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ *ماپا = ext2fs_rb_ماپا(victim);

	/* Set the surrounding nodes to point to the replacement */
	إذا (ماپا) {
		إذا (victim == ماپا->rb_بایاں)
			ماپا->rb_بایاں = new;
		وإلا
			ماپا->rb_دایاں = new;
	} وإلا {
		جڑ->rb_گرہ = new;
	}
	إذا (victim->rb_بایاں)
		ext2fs_rb_سیٹ_ماپا(victim->rb_بایاں, new);
	إذا (victim->rb_دایاں)
		ext2fs_rb_سیٹ_ماپا(victim->rb_دایاں, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
