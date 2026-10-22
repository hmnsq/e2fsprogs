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

#ଅନ୍ତର୍ଭାଵ "rbtree.h"

ସ୍ଥିର ଶୂନ୍ୟ __rb_rotate_left(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି, ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *ଡାହାଣ = ଗଣ୍ଠି->rb_ଡାହାଣ;
	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ext2fs_rb_parent(ଗଣ୍ଠି);

	ଯଦି ((ଗଣ୍ଠି->rb_ଡାହାଣ = ଡାହାଣ->rb_ବାମ))
		ext2fs_rb_set_parent(ଡାହାଣ->rb_ବାମ, ଗଣ୍ଠି);
	ଡାହାଣ->rb_ବାମ = ଗଣ୍ଠି;

	ext2fs_rb_set_parent(ଡାହାଣ, parent);

	ଯଦି (parent)
	{
		ଯଦି (ଗଣ୍ଠି == parent->rb_ବାମ)
			parent->rb_ବାମ = ଡାହାଣ;
		ଅନ୍ୟଥା
			parent->rb_ଡାହାଣ = ଡାହାଣ;
	}
	ଅନ୍ୟଥା
		ମୂଳ->rb_ଗଣ୍ଠି = ଡାହାଣ;
	ext2fs_rb_set_parent(ଗଣ୍ଠି, ଡାହାଣ);
}

ସ୍ଥିର ଶୂନ୍ୟ __rb_rotate_right(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି, ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *ବାମ = ଗଣ୍ଠି->rb_ବାମ;
	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ext2fs_rb_parent(ଗଣ୍ଠି);

	ଯଦି ((ଗଣ୍ଠି->rb_ବାମ = ବାମ->rb_ଡାହାଣ))
		ext2fs_rb_set_parent(ବାମ->rb_ଡାହାଣ, ଗଣ୍ଠି);
	ବାମ->rb_ଡାହାଣ = ଗଣ୍ଠି;

	ext2fs_rb_set_parent(ବାମ, parent);

	ଯଦି (parent)
	{
		ଯଦି (ଗଣ୍ଠି == parent->rb_ଡାହାଣ)
			parent->rb_ଡାହାଣ = ବାମ;
		ଅନ୍ୟଥା
			parent->rb_ବାମ = ବାମ;
	}
	ଅନ୍ୟଥା
		ମୂଳ->rb_ଗଣ୍ଠି = ବାମ;
	ext2fs_rb_set_parent(ଗଣ୍ଠି, ବାମ);
}

ଶୂନ୍ୟ ext2fs_rb_ଭର୍ତ୍ତି_color(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି, ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *parent, *gparent;

	ଯେତେବେଳେ ((parent = ext2fs_rb_parent(ଗଣ୍ଠି)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		ଯଦି (parent == gparent->rb_ବାମ)
		{
			{
				ପଞ୍ଜିକରଣ ସଂରଚନା rb_ଗଣ୍ଠି *uncle = gparent->rb_ଡାହାଣ;
				ଯଦି (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ଗଣ୍ଠି = gparent;
					ଜାରି;
				}
			}

			ଯଦି (parent->rb_ଡାହାଣ == ଗଣ୍ଠି)
			{
				ପଞ୍ଜିକରଣ ସଂରଚନା rb_ଗଣ୍ଠି *tmp;
				__rb_rotate_left(parent, ମୂଳ);
				tmp = parent;
				parent = ଗଣ୍ଠି;
				ଗଣ୍ଠି = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, ମୂଳ);
		} ଅନ୍ୟଥା {
			{
				ପଞ୍ଜିକରଣ ସଂରଚନା rb_ଗଣ୍ଠି *uncle = gparent->rb_ବାମ;
				ଯଦି (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ଗଣ୍ଠି = gparent;
					ଜାରି;
				}
			}

			ଯଦି (parent->rb_ବାମ == ଗଣ୍ଠି)
			{
				ପଞ୍ଜିକରଣ ସଂରଚନା rb_ଗଣ୍ଠି *tmp;
				__rb_rotate_right(parent, ମୂଳ);
				tmp = parent;
				parent = ଗଣ୍ଠି;
				ଗଣ୍ଠି = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, ମୂଳ);
		}
	}

	ext2fs_rb_set_black(ମୂଳ->rb_ଗଣ୍ଠି);
}

ସ୍ଥିର ଶୂନ୍ୟ __rb_erase_color(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି, ସଂରଚନା rb_ଗଣ୍ଠି *parent,
			     ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *other;

	ଯେତେବେଳେ ((!ଗଣ୍ଠି || ext2fs_rb_is_black(ଗଣ୍ଠି)) && ଗଣ୍ଠି != ମୂଳ->rb_ଗଣ୍ଠି)
	{
		ଯଦି (parent->rb_ବାମ == ଗଣ୍ଠି)
		{
			other = parent->rb_ଡାହାଣ;
			ଯଦି (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, ମୂଳ);
				other = parent->rb_ଡାହାଣ;
			}
			ଯଦି ((!other->rb_ବାମ || ext2fs_rb_is_black(other->rb_ବାମ)) &&
			    (!other->rb_ଡାହାଣ || ext2fs_rb_is_black(other->rb_ଡାହାଣ)))
			{
				ext2fs_rb_set_red(other);
				ଗଣ୍ଠି = parent;
				parent = ext2fs_rb_parent(ଗଣ୍ଠି);
			}
			ଅନ୍ୟଥା
			{
				ଯଦି (!other->rb_ଡାହାଣ || ext2fs_rb_is_black(other->rb_ଡାହାଣ))
				{
					ext2fs_rb_set_black(other->rb_ବାମ);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, ମୂଳ);
					other = parent->rb_ଡାହାଣ;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_ଡାହାଣ);
				__rb_rotate_left(parent, ମୂଳ);
				ଗଣ୍ଠି = ମୂଳ->rb_ଗଣ୍ଠି;
				ଭାଙ୍ଗ;
			}
		}
		ଅନ୍ୟଥା
		{
			other = parent->rb_ବାମ;
			ଯଦି (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, ମୂଳ);
				other = parent->rb_ବାମ;
			}
			ଯଦି ((!other->rb_ବାମ || ext2fs_rb_is_black(other->rb_ବାମ)) &&
			    (!other->rb_ଡାହାଣ || ext2fs_rb_is_black(other->rb_ଡାହାଣ)))
			{
				ext2fs_rb_set_red(other);
				ଗଣ୍ଠି = parent;
				parent = ext2fs_rb_parent(ଗଣ୍ଠି);
			}
			ଅନ୍ୟଥା
			{
				ଯଦି (!other->rb_ବାମ || ext2fs_rb_is_black(other->rb_ବାମ))
				{
					ext2fs_rb_set_black(other->rb_ଡାହାଣ);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, ମୂଳ);
					other = parent->rb_ବାମ;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_ବାମ);
				__rb_rotate_right(parent, ମୂଳ);
				ଗଣ୍ଠି = ମୂଳ->rb_ଗଣ୍ଠି;
				ଭାଙ୍ଗ;
			}
		}
	}
	ଯଦି (ଗଣ୍ଠି)
		ext2fs_rb_set_black(ଗଣ୍ଠି);
}

ଶୂନ୍ୟ ext2fs_rb_erase(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି, ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *child, *parent;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା color;

	ଯଦି (!ଗଣ୍ଠି->rb_ବାମ)
		child = ଗଣ୍ଠି->rb_ଡାହାଣ;
	ଅନ୍ୟଥା ଯଦି (!ଗଣ୍ଠି->rb_ଡାହାଣ)
		child = ଗଣ୍ଠି->rb_ବାମ;
	ଅନ୍ୟଥା
	{
		ସଂରଚନା rb_ଗଣ୍ଠି *old = ଗଣ୍ଠି, *ବାମ;

		ଗଣ୍ଠି = ଗଣ୍ଠି->rb_ଡାହାଣ;
		ଯେତେବେଳେ ((ବାମ = ଗଣ୍ଠି->rb_ବାମ) != ଖାଲି)
			ଗଣ୍ଠି = ବାମ;

		ଯଦି (ext2fs_rb_parent(old)) {
			ଯଦି (ext2fs_rb_parent(old)->rb_ବାମ == old)
				ext2fs_rb_parent(old)->rb_ବାମ = ଗଣ୍ଠି;
			ଅନ୍ୟଥା
				ext2fs_rb_parent(old)->rb_ଡାହାଣ = ଗଣ୍ଠି;
		} ଅନ୍ୟଥା
			ମୂଳ->rb_ଗଣ୍ଠି = ଗଣ୍ଠି;

		child = ଗଣ୍ଠି->rb_ଡାହାଣ;
		parent = ext2fs_rb_parent(ଗଣ୍ଠି);
		color = ext2fs_rb_color(ଗଣ୍ଠି);

		ଯଦି (parent == old) {
			parent = ଗଣ୍ଠି;
		} ଅନ୍ୟଥା {
			ଯଦି (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_ବାମ = child;

			ଗଣ୍ଠି->rb_ଡାହାଣ = old->rb_ଡାହାଣ;
			ext2fs_rb_set_parent(old->rb_ଡାହାଣ, ଗଣ୍ଠି);
		}

		ଗଣ୍ଠି->rb_parent_color = old->rb_parent_color;
		ଗଣ୍ଠି->rb_ବାମ = old->rb_ବାମ;
		ext2fs_rb_set_parent(old->rb_ବାମ, ଗଣ୍ଠି);

		ଯାଅ color;
	}

	parent = ext2fs_rb_parent(ଗଣ୍ଠି);
	color = ext2fs_rb_color(ଗଣ୍ଠି);

	ଯଦି (child)
		ext2fs_rb_set_parent(child, parent);
	ଯଦି (parent)
	{
		ଯଦି (parent->rb_ବାମ == ଗଣ୍ଠି)
			parent->rb_ବାମ = child;
		ଅନ୍ୟଥା
			parent->rb_ଡାହାଣ = child;
	}
	ଅନ୍ୟଥା
		ମୂଳ->rb_ଗଣ୍ଠି = child;

 color:
	ଯଦି (color == RB_BLACK)
		__rb_erase_color(child, parent, ମୂଳ);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
ସଂରଚନା rb_ଗଣ୍ଠି *ext2fs_rb_first(ଅଚଳ ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି	*n;

	n = ମୂଳ->rb_ଗଣ୍ଠି;
	ଯଦି (!n)
		ଫେରନ୍ତୁ ଖାଲି;
	ଯେତେବେଳେ (n->rb_ବାମ)
		n = n->rb_ବାମ;
	ଫେରନ୍ତୁ n;
}

ସଂରଚନା rb_ଗଣ୍ଠି *ext2fs_rb_last(ଅଚଳ ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି	*n;

	n = ମୂଳ->rb_ଗଣ୍ଠି;
	ଯଦି (!n)
		ଫେରନ୍ତୁ ଖାଲି;
	ଯେତେବେଳେ (n->rb_ଡାହାଣ)
		n = n->rb_ଡାହାଣ;
	ଫେରନ୍ତୁ n;
}

ସଂରଚନା rb_ଗଣ୍ଠି *ext2fs_rb_next(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *parent;

	ଯଦି (ext2fs_rb_parent(ଗଣ୍ଠି) == ଗଣ୍ଠି)
		ଫେରନ୍ତୁ ଖାଲି;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	ଯଦି (ଗଣ୍ଠି->rb_ଡାହାଣ) {
		ଗଣ୍ଠି = ଗଣ୍ଠି->rb_ଡାହାଣ;
		ଯେତେବେଳେ (ଗଣ୍ଠି->rb_ବାମ)
			ଗଣ୍ଠି=ଗଣ୍ଠି->rb_ବାମ;
		ଫେରନ୍ତୁ (ସଂରଚନା rb_ଗଣ୍ଠି *)ଗଣ୍ଠି;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	ଯେତେବେଳେ ((parent = ext2fs_rb_parent(ଗଣ୍ଠି)) && ଗଣ୍ଠି == parent->rb_ଡାହାଣ)
		ଗଣ୍ଠି = parent;

	ଫେରନ୍ତୁ parent;
}

ସଂରଚନା rb_ଗଣ୍ଠି *ext2fs_rb_prev(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *parent;

	ଯଦି (ext2fs_rb_parent(ଗଣ୍ଠି) == ଗଣ୍ଠି)
		ଫେରନ୍ତୁ ଖାଲି;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	ଯଦି (ଗଣ୍ଠି->rb_ବାମ) {
		ଗଣ୍ଠି = ଗଣ୍ଠି->rb_ବାମ;
		ଯେତେବେଳେ (ଗଣ୍ଠି->rb_ଡାହାଣ)
			ଗଣ୍ଠି=ଗଣ୍ଠି->rb_ଡାହାଣ;
		ଫେରନ୍ତୁ (ସଂରଚନା rb_ଗଣ୍ଠି *)ଗଣ୍ଠି;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	ଯେତେବେଳେ ((parent = ext2fs_rb_parent(ଗଣ୍ଠି)) && ଗଣ୍ଠି == parent->rb_ବାମ)
		ଗଣ୍ଠି = parent;

	ଫେରନ୍ତୁ parent;
}

ଶୂନ୍ୟ ext2fs_rb_replace_ଗଣ୍ଠି(ସଂରଚନା rb_ଗଣ୍ଠି *victim, ସଂରଚନା rb_ଗଣ୍ଠି *new,
			  ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	ଯଦି (parent) {
		ଯଦି (victim == parent->rb_ବାମ)
			parent->rb_ବାମ = new;
		ଅନ୍ୟଥା
			parent->rb_ଡାହାଣ = new;
	} ଅନ୍ୟଥା {
		ମୂଳ->rb_ଗଣ୍ଠି = new;
	}
	ଯଦି (victim->rb_ବାମ)
		ext2fs_rb_set_parent(victim->rb_ବାମ, new);
	ଯଦି (victim->rb_ଡାହାଣ)
		ext2fs_rb_set_parent(victim->rb_ଡାହାଣ, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
