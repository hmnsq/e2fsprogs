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

ಸ್ಥಿರ ಶೂನ್ಯ __rb_rotate_left(ರಚನೆ rb_ಗಂಟು *ಗಂಟು, ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ rb_ಗಂಟು *ಬಲ = ಗಂಟು->rb_ಬಲ;
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(ಗಂಟು);

	ಆದರೆ ((ಗಂಟು->rb_ಬಲ = ಬಲ->rb_ಎಡ))
		ext2fs_rb_ಗಣ_ಪೋಷಕ(ಬಲ->rb_ಎಡ, ಗಂಟು);
	ಬಲ->rb_ಎಡ = ಗಂಟು;

	ext2fs_rb_ಗಣ_ಪೋಷಕ(ಬಲ, ಪೋಷಕ);

	ಆದರೆ (ಪೋಷಕ)
	{
		ಆದರೆ (ಗಂಟು == ಪೋಷಕ->rb_ಎಡ)
			ಪೋಷಕ->rb_ಎಡ = ಬಲ;
		ಇಲ್ಲದಿದ್ದರೆ
			ಪೋಷಕ->rb_ಬಲ = ಬಲ;
	}
	ಇಲ್ಲದಿದ್ದರೆ
		ಬೇರು->rb_ಗಂಟು = ಬಲ;
	ext2fs_rb_ಗಣ_ಪೋಷಕ(ಗಂಟು, ಬಲ);
}

ಸ್ಥಿರ ಶೂನ್ಯ __rb_rotate_right(ರಚನೆ rb_ಗಂಟು *ಗಂಟು, ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ rb_ಗಂಟು *ಎಡ = ಗಂಟು->rb_ಎಡ;
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(ಗಂಟು);

	ಆದರೆ ((ಗಂಟು->rb_ಎಡ = ಎಡ->rb_ಬಲ))
		ext2fs_rb_ಗಣ_ಪೋಷಕ(ಎಡ->rb_ಬಲ, ಗಂಟು);
	ಎಡ->rb_ಬಲ = ಗಂಟು;

	ext2fs_rb_ಗಣ_ಪೋಷಕ(ಎಡ, ಪೋಷಕ);

	ಆದರೆ (ಪೋಷಕ)
	{
		ಆದರೆ (ಗಂಟು == ಪೋಷಕ->rb_ಬಲ)
			ಪೋಷಕ->rb_ಬಲ = ಎಡ;
		ಇಲ್ಲದಿದ್ದರೆ
			ಪೋಷಕ->rb_ಎಡ = ಎಡ;
	}
	ಇಲ್ಲದಿದ್ದರೆ
		ಬೇರು->rb_ಗಂಟು = ಎಡ;
	ext2fs_rb_ಗಣ_ಪೋಷಕ(ಗಂಟು, ಎಡ);
}

ಶೂನ್ಯ ext2fs_rb_ಸೇರಿಸು_color(ರಚನೆ rb_ಗಂಟು *ಗಂಟು, ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ, *gparent;

	ಆಗ ((ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(ಗಂಟು)) && ext2fs_rb_is_red(ಪೋಷಕ))
	{
		gparent = ext2fs_rb_ಪೋಷಕ(ಪೋಷಕ);

		ಆದರೆ (ಪೋಷಕ == gparent->rb_ಎಡ)
		{
			{
				ನೋಂದಣಿ ರಚನೆ rb_ಗಂಟು *uncle = gparent->rb_ಬಲ;
				ಆದರೆ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_ಗಣ_black(uncle);
					ext2fs_rb_ಗಣ_black(ಪೋಷಕ);
					ext2fs_rb_ಗಣ_red(gparent);
					ಗಂಟು = gparent;
					ಮುಂದುವರಿ;
				}
			}

			ಆದರೆ (ಪೋಷಕ->rb_ಬಲ == ಗಂಟು)
			{
				ನೋಂದಣಿ ರಚನೆ rb_ಗಂಟು *tmp;
				__rb_rotate_left(ಪೋಷಕ, ಬೇರು);
				tmp = ಪೋಷಕ;
				ಪೋಷಕ = ಗಂಟು;
				ಗಂಟು = tmp;
			}

			ext2fs_rb_ಗಣ_black(ಪೋಷಕ);
			ext2fs_rb_ಗಣ_red(gparent);
			__rb_rotate_right(gparent, ಬೇರು);
		} ಇಲ್ಲದಿದ್ದರೆ {
			{
				ನೋಂದಣಿ ರಚನೆ rb_ಗಂಟು *uncle = gparent->rb_ಎಡ;
				ಆದರೆ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_ಗಣ_black(uncle);
					ext2fs_rb_ಗಣ_black(ಪೋಷಕ);
					ext2fs_rb_ಗಣ_red(gparent);
					ಗಂಟು = gparent;
					ಮುಂದುವರಿ;
				}
			}

			ಆದರೆ (ಪೋಷಕ->rb_ಎಡ == ಗಂಟು)
			{
				ನೋಂದಣಿ ರಚನೆ rb_ಗಂಟು *tmp;
				__rb_rotate_right(ಪೋಷಕ, ಬೇರು);
				tmp = ಪೋಷಕ;
				ಪೋಷಕ = ಗಂಟು;
				ಗಂಟು = tmp;
			}

			ext2fs_rb_ಗಣ_black(ಪೋಷಕ);
			ext2fs_rb_ಗಣ_red(gparent);
			__rb_rotate_left(gparent, ಬೇರು);
		}
	}

	ext2fs_rb_ಗಣ_black(ಬೇರು->rb_ಗಂಟು);
}

ಸ್ಥಿರ ಶೂನ್ಯ __rb_erase_color(ರಚನೆ rb_ಗಂಟು *ಗಂಟು, ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ,
			     ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ rb_ಗಂಟು *other;

	ಆಗ ((!ಗಂಟು || ext2fs_rb_is_black(ಗಂಟು)) && ಗಂಟು != ಬೇರು->rb_ಗಂಟು)
	{
		ಆದರೆ (ಪೋಷಕ->rb_ಎಡ == ಗಂಟು)
		{
			other = ಪೋಷಕ->rb_ಬಲ;
			ಆದರೆ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_ಗಣ_black(other);
				ext2fs_rb_ಗಣ_red(ಪೋಷಕ);
				__rb_rotate_left(ಪೋಷಕ, ಬೇರು);
				other = ಪೋಷಕ->rb_ಬಲ;
			}
			ಆದರೆ ((!other->rb_ಎಡ || ext2fs_rb_is_black(other->rb_ಎಡ)) &&
			    (!other->rb_ಬಲ || ext2fs_rb_is_black(other->rb_ಬಲ)))
			{
				ext2fs_rb_ಗಣ_red(other);
				ಗಂಟು = ಪೋಷಕ;
				ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(ಗಂಟು);
			}
			ಇಲ್ಲದಿದ್ದರೆ
			{
				ಆದರೆ (!other->rb_ಬಲ || ext2fs_rb_is_black(other->rb_ಬಲ))
				{
					ext2fs_rb_ಗಣ_black(other->rb_ಎಡ);
					ext2fs_rb_ಗಣ_red(other);
					__rb_rotate_right(other, ಬೇರು);
					other = ಪೋಷಕ->rb_ಬಲ;
				}
				ext2fs_rb_ಗಣ_color(other, ext2fs_rb_color(ಪೋಷಕ));
				ext2fs_rb_ಗಣ_black(ಪೋಷಕ);
				ext2fs_rb_ಗಣ_black(other->rb_ಬಲ);
				__rb_rotate_left(ಪೋಷಕ, ಬೇರು);
				ಗಂಟು = ಬೇರು->rb_ಗಂಟು;
				ನಿಲ್ಲಿಸು;
			}
		}
		ಇಲ್ಲದಿದ್ದರೆ
		{
			other = ಪೋಷಕ->rb_ಎಡ;
			ಆದರೆ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_ಗಣ_black(other);
				ext2fs_rb_ಗಣ_red(ಪೋಷಕ);
				__rb_rotate_right(ಪೋಷಕ, ಬೇರು);
				other = ಪೋಷಕ->rb_ಎಡ;
			}
			ಆದರೆ ((!other->rb_ಎಡ || ext2fs_rb_is_black(other->rb_ಎಡ)) &&
			    (!other->rb_ಬಲ || ext2fs_rb_is_black(other->rb_ಬಲ)))
			{
				ext2fs_rb_ಗಣ_red(other);
				ಗಂಟು = ಪೋಷಕ;
				ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(ಗಂಟು);
			}
			ಇಲ್ಲದಿದ್ದರೆ
			{
				ಆದರೆ (!other->rb_ಎಡ || ext2fs_rb_is_black(other->rb_ಎಡ))
				{
					ext2fs_rb_ಗಣ_black(other->rb_ಬಲ);
					ext2fs_rb_ಗಣ_red(other);
					__rb_rotate_left(other, ಬೇರು);
					other = ಪೋಷಕ->rb_ಎಡ;
				}
				ext2fs_rb_ಗಣ_color(other, ext2fs_rb_color(ಪೋಷಕ));
				ext2fs_rb_ಗಣ_black(ಪೋಷಕ);
				ext2fs_rb_ಗಣ_black(other->rb_ಎಡ);
				__rb_rotate_right(ಪೋಷಕ, ಬೇರು);
				ಗಂಟು = ಬೇರು->rb_ಗಂಟು;
				ನಿಲ್ಲಿಸು;
			}
		}
	}
	ಆದರೆ (ಗಂಟು)
		ext2fs_rb_ಗಣ_black(ಗಂಟು);
}

ಶೂನ್ಯ ext2fs_rb_erase(ರಚನೆ rb_ಗಂಟು *ಗಂಟು, ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ rb_ಗಂಟು *ಮಗು, *ಪೋಷಕ;
	ಪೂರ್ಣಾಂಕ color;

	ಆದರೆ (!ಗಂಟು->rb_ಎಡ)
		ಮಗು = ಗಂಟು->rb_ಬಲ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಗಂಟು->rb_ಬಲ)
		ಮಗು = ಗಂಟು->rb_ಎಡ;
	ಇಲ್ಲದಿದ್ದರೆ
	{
		ರಚನೆ rb_ಗಂಟು *old = ಗಂಟು, *ಎಡ;

		ಗಂಟು = ಗಂಟು->rb_ಬಲ;
		ಆಗ ((ಎಡ = ಗಂಟು->rb_ಎಡ) != ಖಾಲಿ)
			ಗಂಟು = ಎಡ;

		ಆದರೆ (ext2fs_rb_ಪೋಷಕ(old)) {
			ಆದರೆ (ext2fs_rb_ಪೋಷಕ(old)->rb_ಎಡ == old)
				ext2fs_rb_ಪೋಷಕ(old)->rb_ಎಡ = ಗಂಟು;
			ಇಲ್ಲದಿದ್ದರೆ
				ext2fs_rb_ಪೋಷಕ(old)->rb_ಬಲ = ಗಂಟು;
		} ಇಲ್ಲದಿದ್ದರೆ
			ಬೇರು->rb_ಗಂಟು = ಗಂಟು;

		ಮಗು = ಗಂಟು->rb_ಬಲ;
		ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(ಗಂಟು);
		color = ext2fs_rb_color(ಗಂಟು);

		ಆದರೆ (ಪೋಷಕ == old) {
			ಪೋಷಕ = ಗಂಟು;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (ಮಗು)
				ext2fs_rb_ಗಣ_ಪೋಷಕ(ಮಗು, ಪೋಷಕ);
			ಪೋಷಕ->rb_ಎಡ = ಮಗು;

			ಗಂಟು->rb_ಬಲ = old->rb_ಬಲ;
			ext2fs_rb_ಗಣ_ಪೋಷಕ(old->rb_ಬಲ, ಗಂಟು);
		}

		ಗಂಟು->rb_ಪೋಷಕ_color = old->rb_ಪೋಷಕ_color;
		ಗಂಟು->rb_ಎಡ = old->rb_ಎಡ;
		ext2fs_rb_ಗಣ_ಪೋಷಕ(old->rb_ಎಡ, ಗಂಟು);

		ಹೋಗು color;
	}

	ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(ಗಂಟು);
	color = ext2fs_rb_color(ಗಂಟು);

	ಆದರೆ (ಮಗು)
		ext2fs_rb_ಗಣ_ಪೋಷಕ(ಮಗು, ಪೋಷಕ);
	ಆದರೆ (ಪೋಷಕ)
	{
		ಆದರೆ (ಪೋಷಕ->rb_ಎಡ == ಗಂಟು)
			ಪೋಷಕ->rb_ಎಡ = ಮಗು;
		ಇಲ್ಲದಿದ್ದರೆ
			ಪೋಷಕ->rb_ಬಲ = ಮಗು;
	}
	ಇಲ್ಲದಿದ್ದರೆ
		ಬೇರು->rb_ಗಂಟು = ಮಗು;

 color:
	ಆದರೆ (color == RB_BLACK)
		__rb_erase_color(ಮಗು, ಪೋಷಕ, ಬೇರು);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
ರಚನೆ rb_ಗಂಟು *ext2fs_rb_ಮೊದಲ(ಸ್ಥಿರಾಂಕ ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ rb_ಗಂಟು	*n;

	n = ಬೇರು->rb_ಗಂಟು;
	ಆದರೆ (!n)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಆಗ (n->rb_ಎಡ)
		n = n->rb_ಎಡ;
	ಹಿಂತಿರುಗಿ n;
}

ರಚನೆ rb_ಗಂಟು *ext2fs_rb_ಕೊನೆಯ(ಸ್ಥಿರಾಂಕ ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ rb_ಗಂಟು	*n;

	n = ಬೇರು->rb_ಗಂಟು;
	ಆದರೆ (!n)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಆಗ (n->rb_ಬಲ)
		n = n->rb_ಬಲ;
	ಹಿಂತಿರುಗಿ n;
}

ರಚನೆ rb_ಗಂಟು *ext2fs_rb_ಮುಂದಿನ(ರಚನೆ rb_ಗಂಟು *ಗಂಟು)
{
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ;

	ಆದರೆ (ext2fs_rb_ಪೋಷಕ(ಗಂಟು) == ಗಂಟು)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	ಆದರೆ (ಗಂಟು->rb_ಬಲ) {
		ಗಂಟು = ಗಂಟು->rb_ಬಲ;
		ಆಗ (ಗಂಟು->rb_ಎಡ)
			ಗಂಟು=ಗಂಟು->rb_ಎಡ;
		ಹಿಂತಿರುಗಿ (ರಚನೆ rb_ಗಂಟು *)ಗಂಟು;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	ಆಗ ((ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(ಗಂಟು)) && ಗಂಟು == ಪೋಷಕ->rb_ಬಲ)
		ಗಂಟು = ಪೋಷಕ;

	ಹಿಂತಿರುಗಿ ಪೋಷಕ;
}

ರಚನೆ rb_ಗಂಟು *ext2fs_rb_prev(ರಚನೆ rb_ಗಂಟು *ಗಂಟು)
{
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ;

	ಆದರೆ (ext2fs_rb_ಪೋಷಕ(ಗಂಟು) == ಗಂಟು)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	ಆದರೆ (ಗಂಟು->rb_ಎಡ) {
		ಗಂಟು = ಗಂಟು->rb_ಎಡ;
		ಆಗ (ಗಂಟು->rb_ಬಲ)
			ಗಂಟು=ಗಂಟು->rb_ಬಲ;
		ಹಿಂತಿರುಗಿ (ರಚನೆ rb_ಗಂಟು *)ಗಂಟು;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	ಆಗ ((ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(ಗಂಟು)) && ಗಂಟು == ಪೋಷಕ->rb_ಎಡ)
		ಗಂಟು = ಪೋಷಕ;

	ಹಿಂತಿರುಗಿ ಪೋಷಕ;
}

ಶೂನ್ಯ ext2fs_rb_replace_ಗಂಟು(ರಚನೆ rb_ಗಂಟು *victim, ರಚನೆ rb_ಗಂಟು *new,
			  ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ext2fs_rb_ಪೋಷಕ(victim);

	/* Set the surrounding nodes to point to the replacement */
	ಆದರೆ (ಪೋಷಕ) {
		ಆದರೆ (victim == ಪೋಷಕ->rb_ಎಡ)
			ಪೋಷಕ->rb_ಎಡ = new;
		ಇಲ್ಲದಿದ್ದರೆ
			ಪೋಷಕ->rb_ಬಲ = new;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬೇರು->rb_ಗಂಟು = new;
	}
	ಆದರೆ (victim->rb_ಎಡ)
		ext2fs_rb_ಗಣ_ಪೋಷಕ(victim->rb_ಎಡ, new);
	ಆದರೆ (victim->rb_ಬಲ)
		ext2fs_rb_ಗಣ_ಪೋಷಕ(victim->rb_ಬಲ, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
