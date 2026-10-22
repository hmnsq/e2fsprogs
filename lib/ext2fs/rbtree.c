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

#అన్తర్భావ "rbtree.h"

స్థిర శూన్య __rb_rotate_left(నిర్మాణం rb_కణుపు *కణుపు, నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం rb_కణుపు *కుడి = కణుపు->rb_కుడి;
	నిర్మాణం rb_కణుపు *మాతృ = ext2fs_rb_మాతృ(కణుపు);

	ఒకవేళ ((కణుపు->rb_కుడి = కుడి->rb_ఎడమ))
		ext2fs_rb_సమితి_మాతృ(కుడి->rb_ఎడమ, కణుపు);
	కుడి->rb_ఎడమ = కణుపు;

	ext2fs_rb_సమితి_మాతృ(కుడి, మాతృ);

	ఒకవేళ (మాతృ)
	{
		ఒకవేళ (కణుపు == మాతృ->rb_ఎడమ)
			మాతృ->rb_ఎడమ = కుడి;
		లేకపోతే
			మాతృ->rb_కుడి = కుడి;
	}
	లేకపోతే
		మూలం->rb_కణుపు = కుడి;
	ext2fs_rb_సమితి_మాతృ(కణుపు, కుడి);
}

స్థిర శూన్య __rb_rotate_right(నిర్మాణం rb_కణుపు *కణుపు, నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం rb_కణుపు *ఎడమ = కణుపు->rb_ఎడమ;
	నిర్మాణం rb_కణుపు *మాతృ = ext2fs_rb_మాతృ(కణుపు);

	ఒకవేళ ((కణుపు->rb_ఎడమ = ఎడమ->rb_కుడి))
		ext2fs_rb_సమితి_మాతృ(ఎడమ->rb_కుడి, కణుపు);
	ఎడమ->rb_కుడి = కణుపు;

	ext2fs_rb_సమితి_మాతృ(ఎడమ, మాతృ);

	ఒకవేళ (మాతృ)
	{
		ఒకవేళ (కణుపు == మాతృ->rb_కుడి)
			మాతృ->rb_కుడి = ఎడమ;
		లేకపోతే
			మాతృ->rb_ఎడమ = ఎడమ;
	}
	లేకపోతే
		మూలం->rb_కణుపు = ఎడమ;
	ext2fs_rb_సమితి_మాతృ(కణుపు, ఎడమ);
}

శూన్య ext2fs_rb_చొప్పించు_color(నిర్మాణం rb_కణుపు *కణుపు, నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం rb_కణుపు *మాతృ, *gparent;

	అయితే ((మాతృ = ext2fs_rb_మాతృ(కణుపు)) && ext2fs_rb_is_red(మాతృ))
	{
		gparent = ext2fs_rb_మాతృ(మాతృ);

		ఒకవేళ (మాతృ == gparent->rb_ఎడమ)
		{
			{
				నమోదు నిర్మాణం rb_కణుపు *uncle = gparent->rb_కుడి;
				ఒకవేళ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_సమితి_black(uncle);
					ext2fs_rb_సమితి_black(మాతృ);
					ext2fs_rb_సమితి_red(gparent);
					కణుపు = gparent;
					కొనసాగు;
				}
			}

			ఒకవేళ (మాతృ->rb_కుడి == కణుపు)
			{
				నమోదు నిర్మాణం rb_కణుపు *tmp;
				__rb_rotate_left(మాతృ, మూలం);
				tmp = మాతృ;
				మాతృ = కణుపు;
				కణుపు = tmp;
			}

			ext2fs_rb_సమితి_black(మాతృ);
			ext2fs_rb_సమితి_red(gparent);
			__rb_rotate_right(gparent, మూలం);
		} లేకపోతే {
			{
				నమోదు నిర్మాణం rb_కణుపు *uncle = gparent->rb_ఎడమ;
				ఒకవేళ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_సమితి_black(uncle);
					ext2fs_rb_సమితి_black(మాతృ);
					ext2fs_rb_సమితి_red(gparent);
					కణుపు = gparent;
					కొనసాగు;
				}
			}

			ఒకవేళ (మాతృ->rb_ఎడమ == కణుపు)
			{
				నమోదు నిర్మాణం rb_కణుపు *tmp;
				__rb_rotate_right(మాతృ, మూలం);
				tmp = మాతృ;
				మాతృ = కణుపు;
				కణుపు = tmp;
			}

			ext2fs_rb_సమితి_black(మాతృ);
			ext2fs_rb_సమితి_red(gparent);
			__rb_rotate_left(gparent, మూలం);
		}
	}

	ext2fs_rb_సమితి_black(మూలం->rb_కణుపు);
}

స్థిర శూన్య __rb_erase_color(నిర్మాణం rb_కణుపు *కణుపు, నిర్మాణం rb_కణుపు *మాతృ,
			     నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం rb_కణుపు *other;

	అయితే ((!కణుపు || ext2fs_rb_is_black(కణుపు)) && కణుపు != మూలం->rb_కణుపు)
	{
		ఒకవేళ (మాతృ->rb_ఎడమ == కణుపు)
		{
			other = మాతృ->rb_కుడి;
			ఒకవేళ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_సమితి_black(other);
				ext2fs_rb_సమితి_red(మాతృ);
				__rb_rotate_left(మాతృ, మూలం);
				other = మాతృ->rb_కుడి;
			}
			ఒకవేళ ((!other->rb_ఎడమ || ext2fs_rb_is_black(other->rb_ఎడమ)) &&
			    (!other->rb_కుడి || ext2fs_rb_is_black(other->rb_కుడి)))
			{
				ext2fs_rb_సమితి_red(other);
				కణుపు = మాతృ;
				మాతృ = ext2fs_rb_మాతృ(కణుపు);
			}
			లేకపోతే
			{
				ఒకవేళ (!other->rb_కుడి || ext2fs_rb_is_black(other->rb_కుడి))
				{
					ext2fs_rb_సమితి_black(other->rb_ఎడమ);
					ext2fs_rb_సమితి_red(other);
					__rb_rotate_right(other, మూలం);
					other = మాతృ->rb_కుడి;
				}
				ext2fs_rb_సమితి_color(other, ext2fs_rb_color(మాతృ));
				ext2fs_rb_సమితి_black(మాతృ);
				ext2fs_rb_సమితి_black(other->rb_కుడి);
				__rb_rotate_left(మాతృ, మూలం);
				కణుపు = మూలం->rb_కణుపు;
				ఆపు;
			}
		}
		లేకపోతే
		{
			other = మాతృ->rb_ఎడమ;
			ఒకవేళ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_సమితి_black(other);
				ext2fs_rb_సమితి_red(మాతృ);
				__rb_rotate_right(మాతృ, మూలం);
				other = మాతృ->rb_ఎడమ;
			}
			ఒకవేళ ((!other->rb_ఎడమ || ext2fs_rb_is_black(other->rb_ఎడమ)) &&
			    (!other->rb_కుడి || ext2fs_rb_is_black(other->rb_కుడి)))
			{
				ext2fs_rb_సమితి_red(other);
				కణుపు = మాతృ;
				మాతృ = ext2fs_rb_మాతృ(కణుపు);
			}
			లేకపోతే
			{
				ఒకవేళ (!other->rb_ఎడమ || ext2fs_rb_is_black(other->rb_ఎడమ))
				{
					ext2fs_rb_సమితి_black(other->rb_కుడి);
					ext2fs_rb_సమితి_red(other);
					__rb_rotate_left(other, మూలం);
					other = మాతృ->rb_ఎడమ;
				}
				ext2fs_rb_సమితి_color(other, ext2fs_rb_color(మాతృ));
				ext2fs_rb_సమితి_black(మాతృ);
				ext2fs_rb_సమితి_black(other->rb_ఎడమ);
				__rb_rotate_right(మాతృ, మూలం);
				కణుపు = మూలం->rb_కణుపు;
				ఆపు;
			}
		}
	}
	ఒకవేళ (కణుపు)
		ext2fs_rb_సమితి_black(కణుపు);
}

శూన్య ext2fs_rb_erase(నిర్మాణం rb_కణుపు *కణుపు, నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం rb_కణుపు *బిడ్డ, *మాతృ;
	పూర్ణాంకం color;

	ఒకవేళ (!కణుపు->rb_ఎడమ)
		బిడ్డ = కణుపు->rb_కుడి;
	లేకపోతే ఒకవేళ (!కణుపు->rb_కుడి)
		బిడ్డ = కణుపు->rb_ఎడమ;
	లేకపోతే
	{
		నిర్మాణం rb_కణుపు *old = కణుపు, *ఎడమ;

		కణుపు = కణుపు->rb_కుడి;
		అయితే ((ఎడమ = కణుపు->rb_ఎడమ) != ఖాళీ)
			కణుపు = ఎడమ;

		ఒకవేళ (ext2fs_rb_మాతృ(old)) {
			ఒకవేళ (ext2fs_rb_మాతృ(old)->rb_ఎడమ == old)
				ext2fs_rb_మాతృ(old)->rb_ఎడమ = కణుపు;
			లేకపోతే
				ext2fs_rb_మాతృ(old)->rb_కుడి = కణుపు;
		} లేకపోతే
			మూలం->rb_కణుపు = కణుపు;

		బిడ్డ = కణుపు->rb_కుడి;
		మాతృ = ext2fs_rb_మాతృ(కణుపు);
		color = ext2fs_rb_color(కణుపు);

		ఒకవేళ (మాతృ == old) {
			మాతృ = కణుపు;
		} లేకపోతే {
			ఒకవేళ (బిడ్డ)
				ext2fs_rb_సమితి_మాతృ(బిడ్డ, మాతృ);
			మాతృ->rb_ఎడమ = బిడ్డ;

			కణుపు->rb_కుడి = old->rb_కుడి;
			ext2fs_rb_సమితి_మాతృ(old->rb_కుడి, కణుపు);
		}

		కణుపు->rb_మాతృ_color = old->rb_మాతృ_color;
		కణుపు->rb_ఎడమ = old->rb_ఎడమ;
		ext2fs_rb_సమితి_మాతృ(old->rb_ఎడమ, కణుపు);

		వెళ్ళు color;
	}

	మాతృ = ext2fs_rb_మాతృ(కణుపు);
	color = ext2fs_rb_color(కణుపు);

	ఒకవేళ (బిడ్డ)
		ext2fs_rb_సమితి_మాతృ(బిడ్డ, మాతృ);
	ఒకవేళ (మాతృ)
	{
		ఒకవేళ (మాతృ->rb_ఎడమ == కణుపు)
			మాతృ->rb_ఎడమ = బిడ్డ;
		లేకపోతే
			మాతృ->rb_కుడి = బిడ్డ;
	}
	లేకపోతే
		మూలం->rb_కణుపు = బిడ్డ;

 color:
	ఒకవేళ (color == RB_BLACK)
		__rb_erase_color(బిడ్డ, మాతృ, మూలం);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
నిర్మాణం rb_కణుపు *ext2fs_rb_మొదటి(స్థిరరాశి నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం rb_కణుపు	*n;

	n = మూలం->rb_కణుపు;
	ఒకవేళ (!n)
		తిరిగి ఖాళీ;
	అయితే (n->rb_ఎడమ)
		n = n->rb_ఎడమ;
	తిరిగి n;
}

నిర్మాణం rb_కణుపు *ext2fs_rb_చివరి(స్థిరరాశి నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం rb_కణుపు	*n;

	n = మూలం->rb_కణుపు;
	ఒకవేళ (!n)
		తిరిగి ఖాళీ;
	అయితే (n->rb_కుడి)
		n = n->rb_కుడి;
	తిరిగి n;
}

నిర్మాణం rb_కణుపు *ext2fs_rb_తదుపరి(నిర్మాణం rb_కణుపు *కణుపు)
{
	నిర్మాణం rb_కణుపు *మాతృ;

	ఒకవేళ (ext2fs_rb_మాతృ(కణుపు) == కణుపు)
		తిరిగి ఖాళీ;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	ఒకవేళ (కణుపు->rb_కుడి) {
		కణుపు = కణుపు->rb_కుడి;
		అయితే (కణుపు->rb_ఎడమ)
			కణుపు=కణుపు->rb_ఎడమ;
		తిరిగి (నిర్మాణం rb_కణుపు *)కణుపు;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	అయితే ((మాతృ = ext2fs_rb_మాతృ(కణుపు)) && కణుపు == మాతృ->rb_కుడి)
		కణుపు = మాతృ;

	తిరిగి మాతృ;
}

నిర్మాణం rb_కణుపు *ext2fs_rb_prev(నిర్మాణం rb_కణుపు *కణుపు)
{
	నిర్మాణం rb_కణుపు *మాతృ;

	ఒకవేళ (ext2fs_rb_మాతృ(కణుపు) == కణుపు)
		తిరిగి ఖాళీ;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	ఒకవేళ (కణుపు->rb_ఎడమ) {
		కణుపు = కణుపు->rb_ఎడమ;
		అయితే (కణుపు->rb_కుడి)
			కణుపు=కణుపు->rb_కుడి;
		తిరిగి (నిర్మాణం rb_కణుపు *)కణుపు;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	అయితే ((మాతృ = ext2fs_rb_మాతృ(కణుపు)) && కణుపు == మాతృ->rb_ఎడమ)
		కణుపు = మాతృ;

	తిరిగి మాతృ;
}

శూన్య ext2fs_rb_replace_కణుపు(నిర్మాణం rb_కణుపు *victim, నిర్మాణం rb_కణుపు *new,
			  నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం rb_కణుపు *మాతృ = ext2fs_rb_మాతృ(victim);

	/* Set the surrounding nodes to point to the replacement */
	ఒకవేళ (మాతృ) {
		ఒకవేళ (victim == మాతృ->rb_ఎడమ)
			మాతృ->rb_ఎడమ = new;
		లేకపోతే
			మాతృ->rb_కుడి = new;
	} లేకపోతే {
		మూలం->rb_కణుపు = new;
	}
	ఒకవేళ (victim->rb_ఎడమ)
		ext2fs_rb_సమితి_మాతృ(victim->rb_ఎడమ, new);
	ఒకవేళ (victim->rb_కుడి)
		ext2fs_rb_సమితి_మాతృ(victim->rb_కుడి, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
