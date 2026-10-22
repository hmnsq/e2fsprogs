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

#ਅਨ੍ਤਰ੍ਭਾਵ "rbtree.h"

ਸਥਿਰ ਖਾਲੀ __rb_rotate_left(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ, ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਸੱਜਾ = ਗੰਢ->rb_ਸੱਜਾ;
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(ਗੰਢ);

	ਜੇ ((ਗੰਢ->rb_ਸੱਜਾ = ਸੱਜਾ->rb_ਖੱਬਾ))
		ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਸੱਜਾ->rb_ਖੱਬਾ, ਗੰਢ);
	ਸੱਜਾ->rb_ਖੱਬਾ = ਗੰਢ;

	ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਸੱਜਾ, ਮਾਪਾ);

	ਜੇ (ਮਾਪਾ)
	{
		ਜੇ (ਗੰਢ == ਮਾਪਾ->rb_ਖੱਬਾ)
			ਮਾਪਾ->rb_ਖੱਬਾ = ਸੱਜਾ;
		ਨਹੀਂਤਾਂ
			ਮਾਪਾ->rb_ਸੱਜਾ = ਸੱਜਾ;
	}
	ਨਹੀਂਤਾਂ
		ਜੜ੍ਹ->rb_ਗੰਢ = ਸੱਜਾ;
	ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਗੰਢ, ਸੱਜਾ);
}

ਸਥਿਰ ਖਾਲੀ __rb_rotate_right(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ, ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਖੱਬਾ = ਗੰਢ->rb_ਖੱਬਾ;
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(ਗੰਢ);

	ਜੇ ((ਗੰਢ->rb_ਖੱਬਾ = ਖੱਬਾ->rb_ਸੱਜਾ))
		ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਖੱਬਾ->rb_ਸੱਜਾ, ਗੰਢ);
	ਖੱਬਾ->rb_ਸੱਜਾ = ਗੰਢ;

	ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਖੱਬਾ, ਮਾਪਾ);

	ਜੇ (ਮਾਪਾ)
	{
		ਜੇ (ਗੰਢ == ਮਾਪਾ->rb_ਸੱਜਾ)
			ਮਾਪਾ->rb_ਸੱਜਾ = ਖੱਬਾ;
		ਨਹੀਂਤਾਂ
			ਮਾਪਾ->rb_ਖੱਬਾ = ਖੱਬਾ;
	}
	ਨਹੀਂਤਾਂ
		ਜੜ੍ਹ->rb_ਗੰਢ = ਖੱਬਾ;
	ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਗੰਢ, ਖੱਬਾ);
}

ਖਾਲੀ ext2fs_rb_ਪਾਓ_color(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ, ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ, *gparent;

	ਜਦੋਂ ((ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(ਗੰਢ)) && ext2fs_rb_is_red(ਮਾਪਾ))
	{
		gparent = ext2fs_rb_ਮਾਪਾ(ਮਾਪਾ);

		ਜੇ (ਮਾਪਾ == gparent->rb_ਖੱਬਾ)
		{
			{
				ਰਜਿਸਟਰ ਬਣਤਰ rb_ਗੰਢ *uncle = gparent->rb_ਸੱਜਾ;
				ਜੇ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_ਸੈੱਟ_black(uncle);
					ext2fs_rb_ਸੈੱਟ_black(ਮਾਪਾ);
					ext2fs_rb_ਸੈੱਟ_red(gparent);
					ਗੰਢ = gparent;
					ਜਾਰੀ;
				}
			}

			ਜੇ (ਮਾਪਾ->rb_ਸੱਜਾ == ਗੰਢ)
			{
				ਰਜਿਸਟਰ ਬਣਤਰ rb_ਗੰਢ *tmp;
				__rb_rotate_left(ਮਾਪਾ, ਜੜ੍ਹ);
				tmp = ਮਾਪਾ;
				ਮਾਪਾ = ਗੰਢ;
				ਗੰਢ = tmp;
			}

			ext2fs_rb_ਸੈੱਟ_black(ਮਾਪਾ);
			ext2fs_rb_ਸੈੱਟ_red(gparent);
			__rb_rotate_right(gparent, ਜੜ੍ਹ);
		} ਨਹੀਂਤਾਂ {
			{
				ਰਜਿਸਟਰ ਬਣਤਰ rb_ਗੰਢ *uncle = gparent->rb_ਖੱਬਾ;
				ਜੇ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_ਸੈੱਟ_black(uncle);
					ext2fs_rb_ਸੈੱਟ_black(ਮਾਪਾ);
					ext2fs_rb_ਸੈੱਟ_red(gparent);
					ਗੰਢ = gparent;
					ਜਾਰੀ;
				}
			}

			ਜੇ (ਮਾਪਾ->rb_ਖੱਬਾ == ਗੰਢ)
			{
				ਰਜਿਸਟਰ ਬਣਤਰ rb_ਗੰਢ *tmp;
				__rb_rotate_right(ਮਾਪਾ, ਜੜ੍ਹ);
				tmp = ਮਾਪਾ;
				ਮਾਪਾ = ਗੰਢ;
				ਗੰਢ = tmp;
			}

			ext2fs_rb_ਸੈੱਟ_black(ਮਾਪਾ);
			ext2fs_rb_ਸੈੱਟ_red(gparent);
			__rb_rotate_left(gparent, ਜੜ੍ਹ);
		}
	}

	ext2fs_rb_ਸੈੱਟ_black(ਜੜ੍ਹ->rb_ਗੰਢ);
}

ਸਥਿਰ ਖਾਲੀ __rb_erase_color(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ, ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ,
			     ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ rb_ਗੰਢ *other;

	ਜਦੋਂ ((!ਗੰਢ || ext2fs_rb_is_black(ਗੰਢ)) && ਗੰਢ != ਜੜ੍ਹ->rb_ਗੰਢ)
	{
		ਜੇ (ਮਾਪਾ->rb_ਖੱਬਾ == ਗੰਢ)
		{
			other = ਮਾਪਾ->rb_ਸੱਜਾ;
			ਜੇ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_ਸੈੱਟ_black(other);
				ext2fs_rb_ਸੈੱਟ_red(ਮਾਪਾ);
				__rb_rotate_left(ਮਾਪਾ, ਜੜ੍ਹ);
				other = ਮਾਪਾ->rb_ਸੱਜਾ;
			}
			ਜੇ ((!other->rb_ਖੱਬਾ || ext2fs_rb_is_black(other->rb_ਖੱਬਾ)) &&
			    (!other->rb_ਸੱਜਾ || ext2fs_rb_is_black(other->rb_ਸੱਜਾ)))
			{
				ext2fs_rb_ਸੈੱਟ_red(other);
				ਗੰਢ = ਮਾਪਾ;
				ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(ਗੰਢ);
			}
			ਨਹੀਂਤਾਂ
			{
				ਜੇ (!other->rb_ਸੱਜਾ || ext2fs_rb_is_black(other->rb_ਸੱਜਾ))
				{
					ext2fs_rb_ਸੈੱਟ_black(other->rb_ਖੱਬਾ);
					ext2fs_rb_ਸੈੱਟ_red(other);
					__rb_rotate_right(other, ਜੜ੍ਹ);
					other = ਮਾਪਾ->rb_ਸੱਜਾ;
				}
				ext2fs_rb_ਸੈੱਟ_color(other, ext2fs_rb_color(ਮਾਪਾ));
				ext2fs_rb_ਸੈੱਟ_black(ਮਾਪਾ);
				ext2fs_rb_ਸੈੱਟ_black(other->rb_ਸੱਜਾ);
				__rb_rotate_left(ਮਾਪਾ, ਜੜ੍ਹ);
				ਗੰਢ = ਜੜ੍ਹ->rb_ਗੰਢ;
				ਰੋਕੋ;
			}
		}
		ਨਹੀਂਤਾਂ
		{
			other = ਮਾਪਾ->rb_ਖੱਬਾ;
			ਜੇ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_ਸੈੱਟ_black(other);
				ext2fs_rb_ਸੈੱਟ_red(ਮਾਪਾ);
				__rb_rotate_right(ਮਾਪਾ, ਜੜ੍ਹ);
				other = ਮਾਪਾ->rb_ਖੱਬਾ;
			}
			ਜੇ ((!other->rb_ਖੱਬਾ || ext2fs_rb_is_black(other->rb_ਖੱਬਾ)) &&
			    (!other->rb_ਸੱਜਾ || ext2fs_rb_is_black(other->rb_ਸੱਜਾ)))
			{
				ext2fs_rb_ਸੈੱਟ_red(other);
				ਗੰਢ = ਮਾਪਾ;
				ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(ਗੰਢ);
			}
			ਨਹੀਂਤਾਂ
			{
				ਜੇ (!other->rb_ਖੱਬਾ || ext2fs_rb_is_black(other->rb_ਖੱਬਾ))
				{
					ext2fs_rb_ਸੈੱਟ_black(other->rb_ਸੱਜਾ);
					ext2fs_rb_ਸੈੱਟ_red(other);
					__rb_rotate_left(other, ਜੜ੍ਹ);
					other = ਮਾਪਾ->rb_ਖੱਬਾ;
				}
				ext2fs_rb_ਸੈੱਟ_color(other, ext2fs_rb_color(ਮਾਪਾ));
				ext2fs_rb_ਸੈੱਟ_black(ਮਾਪਾ);
				ext2fs_rb_ਸੈੱਟ_black(other->rb_ਖੱਬਾ);
				__rb_rotate_right(ਮਾਪਾ, ਜੜ੍ਹ);
				ਗੰਢ = ਜੜ੍ਹ->rb_ਗੰਢ;
				ਰੋਕੋ;
			}
		}
	}
	ਜੇ (ਗੰਢ)
		ext2fs_rb_ਸੈੱਟ_black(ਗੰਢ);
}

ਖਾਲੀ ext2fs_rb_erase(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ, ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਬੱਚਾ, *ਮਾਪਾ;
	ਪੂਰਨ color;

	ਜੇ (!ਗੰਢ->rb_ਖੱਬਾ)
		ਬੱਚਾ = ਗੰਢ->rb_ਸੱਜਾ;
	ਨਹੀਂਤਾਂ ਜੇ (!ਗੰਢ->rb_ਸੱਜਾ)
		ਬੱਚਾ = ਗੰਢ->rb_ਖੱਬਾ;
	ਨਹੀਂਤਾਂ
	{
		ਬਣਤਰ rb_ਗੰਢ *old = ਗੰਢ, *ਖੱਬਾ;

		ਗੰਢ = ਗੰਢ->rb_ਸੱਜਾ;
		ਜਦੋਂ ((ਖੱਬਾ = ਗੰਢ->rb_ਖੱਬਾ) != ਨਿਰਾ)
			ਗੰਢ = ਖੱਬਾ;

		ਜੇ (ext2fs_rb_ਮਾਪਾ(old)) {
			ਜੇ (ext2fs_rb_ਮਾਪਾ(old)->rb_ਖੱਬਾ == old)
				ext2fs_rb_ਮਾਪਾ(old)->rb_ਖੱਬਾ = ਗੰਢ;
			ਨਹੀਂਤਾਂ
				ext2fs_rb_ਮਾਪਾ(old)->rb_ਸੱਜਾ = ਗੰਢ;
		} ਨਹੀਂਤਾਂ
			ਜੜ੍ਹ->rb_ਗੰਢ = ਗੰਢ;

		ਬੱਚਾ = ਗੰਢ->rb_ਸੱਜਾ;
		ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(ਗੰਢ);
		color = ext2fs_rb_color(ਗੰਢ);

		ਜੇ (ਮਾਪਾ == old) {
			ਮਾਪਾ = ਗੰਢ;
		} ਨਹੀਂਤਾਂ {
			ਜੇ (ਬੱਚਾ)
				ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਬੱਚਾ, ਮਾਪਾ);
			ਮਾਪਾ->rb_ਖੱਬਾ = ਬੱਚਾ;

			ਗੰਢ->rb_ਸੱਜਾ = old->rb_ਸੱਜਾ;
			ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(old->rb_ਸੱਜਾ, ਗੰਢ);
		}

		ਗੰਢ->rb_ਮਾਪਾ_color = old->rb_ਮਾਪਾ_color;
		ਗੰਢ->rb_ਖੱਬਾ = old->rb_ਖੱਬਾ;
		ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(old->rb_ਖੱਬਾ, ਗੰਢ);

		ਜਾਓ color;
	}

	ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(ਗੰਢ);
	color = ext2fs_rb_color(ਗੰਢ);

	ਜੇ (ਬੱਚਾ)
		ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਬੱਚਾ, ਮਾਪਾ);
	ਜੇ (ਮਾਪਾ)
	{
		ਜੇ (ਮਾਪਾ->rb_ਖੱਬਾ == ਗੰਢ)
			ਮਾਪਾ->rb_ਖੱਬਾ = ਬੱਚਾ;
		ਨਹੀਂਤਾਂ
			ਮਾਪਾ->rb_ਸੱਜਾ = ਬੱਚਾ;
	}
	ਨਹੀਂਤਾਂ
		ਜੜ੍ਹ->rb_ਗੰਢ = ਬੱਚਾ;

 color:
	ਜੇ (color == RB_BLACK)
		__rb_erase_color(ਬੱਚਾ, ਮਾਪਾ, ਜੜ੍ਹ);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
ਬਣਤਰ rb_ਗੰਢ *ext2fs_rb_ਪਹਿਲਾ(ਅਚਲ ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ rb_ਗੰਢ	*n;

	n = ਜੜ੍ਹ->rb_ਗੰਢ;
	ਜੇ (!n)
		ਵਾਪਸ ਨਿਰਾ;
	ਜਦੋਂ (n->rb_ਖੱਬਾ)
		n = n->rb_ਖੱਬਾ;
	ਵਾਪਸ n;
}

ਬਣਤਰ rb_ਗੰਢ *ext2fs_rb_ਆਖਰੀ(ਅਚਲ ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ rb_ਗੰਢ	*n;

	n = ਜੜ੍ਹ->rb_ਗੰਢ;
	ਜੇ (!n)
		ਵਾਪਸ ਨਿਰਾ;
	ਜਦੋਂ (n->rb_ਸੱਜਾ)
		n = n->rb_ਸੱਜਾ;
	ਵਾਪਸ n;
}

ਬਣਤਰ rb_ਗੰਢ *ext2fs_rb_ਅਗਲਾ(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ;

	ਜੇ (ext2fs_rb_ਮਾਪਾ(ਗੰਢ) == ਗੰਢ)
		ਵਾਪਸ ਨਿਰਾ;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	ਜੇ (ਗੰਢ->rb_ਸੱਜਾ) {
		ਗੰਢ = ਗੰਢ->rb_ਸੱਜਾ;
		ਜਦੋਂ (ਗੰਢ->rb_ਖੱਬਾ)
			ਗੰਢ=ਗੰਢ->rb_ਖੱਬਾ;
		ਵਾਪਸ (ਬਣਤਰ rb_ਗੰਢ *)ਗੰਢ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	ਜਦੋਂ ((ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(ਗੰਢ)) && ਗੰਢ == ਮਾਪਾ->rb_ਸੱਜਾ)
		ਗੰਢ = ਮਾਪਾ;

	ਵਾਪਸ ਮਾਪਾ;
}

ਬਣਤਰ rb_ਗੰਢ *ext2fs_rb_prev(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ;

	ਜੇ (ext2fs_rb_ਮਾਪਾ(ਗੰਢ) == ਗੰਢ)
		ਵਾਪਸ ਨਿਰਾ;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	ਜੇ (ਗੰਢ->rb_ਖੱਬਾ) {
		ਗੰਢ = ਗੰਢ->rb_ਖੱਬਾ;
		ਜਦੋਂ (ਗੰਢ->rb_ਸੱਜਾ)
			ਗੰਢ=ਗੰਢ->rb_ਸੱਜਾ;
		ਵਾਪਸ (ਬਣਤਰ rb_ਗੰਢ *)ਗੰਢ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	ਜਦੋਂ ((ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(ਗੰਢ)) && ਗੰਢ == ਮਾਪਾ->rb_ਖੱਬਾ)
		ਗੰਢ = ਮਾਪਾ;

	ਵਾਪਸ ਮਾਪਾ;
}

ਖਾਲੀ ext2fs_rb_replace_ਗੰਢ(ਬਣਤਰ rb_ਗੰਢ *victim, ਬਣਤਰ rb_ਗੰਢ *new,
			  ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ext2fs_rb_ਮਾਪਾ(victim);

	/* Set the surrounding nodes to point to the replacement */
	ਜੇ (ਮਾਪਾ) {
		ਜੇ (victim == ਮਾਪਾ->rb_ਖੱਬਾ)
			ਮਾਪਾ->rb_ਖੱਬਾ = new;
		ਨਹੀਂਤਾਂ
			ਮਾਪਾ->rb_ਸੱਜਾ = new;
	} ਨਹੀਂਤਾਂ {
		ਜੜ੍ਹ->rb_ਗੰਢ = new;
	}
	ਜੇ (victim->rb_ਖੱਬਾ)
		ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(victim->rb_ਖੱਬਾ, new);
	ਜੇ (victim->rb_ਸੱਜਾ)
		ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(victim->rb_ਸੱਜਾ, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
