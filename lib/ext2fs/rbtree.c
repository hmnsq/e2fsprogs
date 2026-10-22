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

#અન્તર્ભાવ "rbtree.h"

સ્થિર શૂન્ય __rb_rotate_left(સંરચના rb_ગાંઠ *ગાંઠ, સંરચના rb_મૂળ *મૂળ)
{
	સંરચના rb_ગાંઠ *જમણું = ગાંઠ->rb_જમણું;
	સંરચના rb_ગાંઠ *parent = ext2fs_rb_parent(ગાંઠ);

	જો ((ગાંઠ->rb_જમણું = જમણું->rb_ડાબું))
		ext2fs_rb_set_parent(જમણું->rb_ડાબું, ગાંઠ);
	જમણું->rb_ડાબું = ગાંઠ;

	ext2fs_rb_set_parent(જમણું, parent);

	જો (parent)
	{
		જો (ગાંઠ == parent->rb_ડાબું)
			parent->rb_ડાબું = જમણું;
		નહીંતર
			parent->rb_જમણું = જમણું;
	}
	નહીંતર
		મૂળ->rb_ગાંઠ = જમણું;
	ext2fs_rb_set_parent(ગાંઠ, જમણું);
}

સ્થિર શૂન્ય __rb_rotate_right(સંરચના rb_ગાંઠ *ગાંઠ, સંરચના rb_મૂળ *મૂળ)
{
	સંરચના rb_ગાંઠ *ડાબું = ગાંઠ->rb_ડાબું;
	સંરચના rb_ગાંઠ *parent = ext2fs_rb_parent(ગાંઠ);

	જો ((ગાંઠ->rb_ડાબું = ડાબું->rb_જમણું))
		ext2fs_rb_set_parent(ડાબું->rb_જમણું, ગાંઠ);
	ડાબું->rb_જમણું = ગાંઠ;

	ext2fs_rb_set_parent(ડાબું, parent);

	જો (parent)
	{
		જો (ગાંઠ == parent->rb_જમણું)
			parent->rb_જમણું = ડાબું;
		નહીંતર
			parent->rb_ડાબું = ડાબું;
	}
	નહીંતર
		મૂળ->rb_ગાંઠ = ડાબું;
	ext2fs_rb_set_parent(ગાંઠ, ડાબું);
}

શૂન્ય ext2fs_rb_ઉમેરો_color(સંરચના rb_ગાંઠ *ગાંઠ, સંરચના rb_મૂળ *મૂળ)
{
	સંરચના rb_ગાંઠ *parent, *gparent;

	જ્યારે ((parent = ext2fs_rb_parent(ગાંઠ)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		જો (parent == gparent->rb_ડાબું)
		{
			{
				નોંધણી સંરચના rb_ગાંઠ *uncle = gparent->rb_જમણું;
				જો (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ગાંઠ = gparent;
					ચાલુ;
				}
			}

			જો (parent->rb_જમણું == ગાંઠ)
			{
				નોંધણી સંરચના rb_ગાંઠ *tmp;
				__rb_rotate_left(parent, મૂળ);
				tmp = parent;
				parent = ગાંઠ;
				ગાંઠ = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, મૂળ);
		} નહીંતર {
			{
				નોંધણી સંરચના rb_ગાંઠ *uncle = gparent->rb_ડાબું;
				જો (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ગાંઠ = gparent;
					ચાલુ;
				}
			}

			જો (parent->rb_ડાબું == ગાંઠ)
			{
				નોંધણી સંરચના rb_ગાંઠ *tmp;
				__rb_rotate_right(parent, મૂળ);
				tmp = parent;
				parent = ગાંઠ;
				ગાંઠ = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, મૂળ);
		}
	}

	ext2fs_rb_set_black(મૂળ->rb_ગાંઠ);
}

સ્થિર શૂન્ય __rb_erase_color(સંરચના rb_ગાંઠ *ગાંઠ, સંરચના rb_ગાંઠ *parent,
			     સંરચના rb_મૂળ *મૂળ)
{
	સંરચના rb_ગાંઠ *other;

	જ્યારે ((!ગાંઠ || ext2fs_rb_is_black(ગાંઠ)) && ગાંઠ != મૂળ->rb_ગાંઠ)
	{
		જો (parent->rb_ડાબું == ગાંઠ)
		{
			other = parent->rb_જમણું;
			જો (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, મૂળ);
				other = parent->rb_જમણું;
			}
			જો ((!other->rb_ડાબું || ext2fs_rb_is_black(other->rb_ડાબું)) &&
			    (!other->rb_જમણું || ext2fs_rb_is_black(other->rb_જમણું)))
			{
				ext2fs_rb_set_red(other);
				ગાંઠ = parent;
				parent = ext2fs_rb_parent(ગાંઠ);
			}
			નહીંતર
			{
				જો (!other->rb_જમણું || ext2fs_rb_is_black(other->rb_જમણું))
				{
					ext2fs_rb_set_black(other->rb_ડાબું);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, મૂળ);
					other = parent->rb_જમણું;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_જમણું);
				__rb_rotate_left(parent, મૂળ);
				ગાંઠ = મૂળ->rb_ગાંઠ;
				તોડો;
			}
		}
		નહીંતર
		{
			other = parent->rb_ડાબું;
			જો (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, મૂળ);
				other = parent->rb_ડાબું;
			}
			જો ((!other->rb_ડાબું || ext2fs_rb_is_black(other->rb_ડાબું)) &&
			    (!other->rb_જમણું || ext2fs_rb_is_black(other->rb_જમણું)))
			{
				ext2fs_rb_set_red(other);
				ગાંઠ = parent;
				parent = ext2fs_rb_parent(ગાંઠ);
			}
			નહીંતર
			{
				જો (!other->rb_ડાબું || ext2fs_rb_is_black(other->rb_ડાબું))
				{
					ext2fs_rb_set_black(other->rb_જમણું);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, મૂળ);
					other = parent->rb_ડાબું;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_ડાબું);
				__rb_rotate_right(parent, મૂળ);
				ગાંઠ = મૂળ->rb_ગાંઠ;
				તોડો;
			}
		}
	}
	જો (ગાંઠ)
		ext2fs_rb_set_black(ગાંઠ);
}

શૂન્ય ext2fs_rb_erase(સંરચના rb_ગાંઠ *ગાંઠ, સંરચના rb_મૂળ *મૂળ)
{
	સંરચના rb_ગાંઠ *child, *parent;
	પૂર્ણાંક color;

	જો (!ગાંઠ->rb_ડાબું)
		child = ગાંઠ->rb_જમણું;
	નહીંતર જો (!ગાંઠ->rb_જમણું)
		child = ગાંઠ->rb_ડાબું;
	નહીંતર
	{
		સંરચના rb_ગાંઠ *old = ગાંઠ, *ડાબું;

		ગાંઠ = ગાંઠ->rb_જમણું;
		જ્યારે ((ડાબું = ગાંઠ->rb_ડાબું) != ખાલી)
			ગાંઠ = ડાબું;

		જો (ext2fs_rb_parent(old)) {
			જો (ext2fs_rb_parent(old)->rb_ડાબું == old)
				ext2fs_rb_parent(old)->rb_ડાબું = ગાંઠ;
			નહીંતર
				ext2fs_rb_parent(old)->rb_જમણું = ગાંઠ;
		} નહીંતર
			મૂળ->rb_ગાંઠ = ગાંઠ;

		child = ગાંઠ->rb_જમણું;
		parent = ext2fs_rb_parent(ગાંઠ);
		color = ext2fs_rb_color(ગાંઠ);

		જો (parent == old) {
			parent = ગાંઠ;
		} નહીંતર {
			જો (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_ડાબું = child;

			ગાંઠ->rb_જમણું = old->rb_જમણું;
			ext2fs_rb_set_parent(old->rb_જમણું, ગાંઠ);
		}

		ગાંઠ->rb_parent_color = old->rb_parent_color;
		ગાંઠ->rb_ડાબું = old->rb_ડાબું;
		ext2fs_rb_set_parent(old->rb_ડાબું, ગાંઠ);

		જાઓ color;
	}

	parent = ext2fs_rb_parent(ગાંઠ);
	color = ext2fs_rb_color(ગાંઠ);

	જો (child)
		ext2fs_rb_set_parent(child, parent);
	જો (parent)
	{
		જો (parent->rb_ડાબું == ગાંઠ)
			parent->rb_ડાબું = child;
		નહીંતર
			parent->rb_જમણું = child;
	}
	નહીંતર
		મૂળ->rb_ગાંઠ = child;

 color:
	જો (color == RB_BLACK)
		__rb_erase_color(child, parent, મૂળ);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
સંરચના rb_ગાંઠ *ext2fs_rb_first(અચલ સંરચના rb_મૂળ *મૂળ)
{
	સંરચના rb_ગાંઠ	*n;

	n = મૂળ->rb_ગાંઠ;
	જો (!n)
		પાછા ખાલી;
	જ્યારે (n->rb_ડાબું)
		n = n->rb_ડાબું;
	પાછા n;
}

સંરચના rb_ગાંઠ *ext2fs_rb_last(અચલ સંરચના rb_મૂળ *મૂળ)
{
	સંરચના rb_ગાંઠ	*n;

	n = મૂળ->rb_ગાંઠ;
	જો (!n)
		પાછા ખાલી;
	જ્યારે (n->rb_જમણું)
		n = n->rb_જમણું;
	પાછા n;
}

સંરચના rb_ગાંઠ *ext2fs_rb_next(સંરચના rb_ગાંઠ *ગાંઠ)
{
	સંરચના rb_ગાંઠ *parent;

	જો (ext2fs_rb_parent(ગાંઠ) == ગાંઠ)
		પાછા ખાલી;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	જો (ગાંઠ->rb_જમણું) {
		ગાંઠ = ગાંઠ->rb_જમણું;
		જ્યારે (ગાંઠ->rb_ડાબું)
			ગાંઠ=ગાંઠ->rb_ડાબું;
		પાછા (સંરચના rb_ગાંઠ *)ગાંઠ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	જ્યારે ((parent = ext2fs_rb_parent(ગાંઠ)) && ગાંઠ == parent->rb_જમણું)
		ગાંઠ = parent;

	પાછા parent;
}

સંરચના rb_ગાંઠ *ext2fs_rb_prev(સંરચના rb_ગાંઠ *ગાંઠ)
{
	સંરચના rb_ગાંઠ *parent;

	જો (ext2fs_rb_parent(ગાંઠ) == ગાંઠ)
		પાછા ખાલી;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	જો (ગાંઠ->rb_ડાબું) {
		ગાંઠ = ગાંઠ->rb_ડાબું;
		જ્યારે (ગાંઠ->rb_જમણું)
			ગાંઠ=ગાંઠ->rb_જમણું;
		પાછા (સંરચના rb_ગાંઠ *)ગાંઠ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	જ્યારે ((parent = ext2fs_rb_parent(ગાંઠ)) && ગાંઠ == parent->rb_ડાબું)
		ગાંઠ = parent;

	પાછા parent;
}

શૂન્ય ext2fs_rb_replace_ગાંઠ(સંરચના rb_ગાંઠ *victim, સંરચના rb_ગાંઠ *new,
			  સંરચના rb_મૂળ *મૂળ)
{
	સંરચના rb_ગાંઠ *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	જો (parent) {
		જો (victim == parent->rb_ડાબું)
			parent->rb_ડાબું = new;
		નહીંતર
			parent->rb_જમણું = new;
	} નહીંતર {
		મૂળ->rb_ગાંઠ = new;
	}
	જો (victim->rb_ડાબું)
		ext2fs_rb_set_parent(victim->rb_ડાબું, new);
	જો (victim->rb_જમણું)
		ext2fs_rb_set_parent(victim->rb_જમણું, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
