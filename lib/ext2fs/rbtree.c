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

#অন্তর্ভাব "rbtree.h"

স্থির শূন্য __rb_rotate_left(কাঠামো rb_গ্রন্থি *গ্রন্থি, কাঠামো rb_মূল *মূল)
{
	কাঠামো rb_গ্রন্থি *ডান = গ্রন্থি->rb_ডান;
	কাঠামো rb_গ্রন্থি *parent = ext2fs_rb_parent(গ্রন্থি);

	যদি ((গ্রন্থি->rb_ডান = ডান->rb_বাম))
		ext2fs_rb_set_parent(ডান->rb_বাম, গ্রন্থি);
	ডান->rb_বাম = গ্রন্থি;

	ext2fs_rb_set_parent(ডান, parent);

	যদি (parent)
	{
		যদি (গ্রন্থি == parent->rb_বাম)
			parent->rb_বাম = ডান;
		অন্যথা
			parent->rb_ডান = ডান;
	}
	অন্যথা
		মূল->rb_গ্রন্থি = ডান;
	ext2fs_rb_set_parent(গ্রন্থি, ডান);
}

স্থির শূন্য __rb_rotate_right(কাঠামো rb_গ্রন্থি *গ্রন্থি, কাঠামো rb_মূল *মূল)
{
	কাঠামো rb_গ্রন্থি *বাম = গ্রন্থি->rb_বাম;
	কাঠামো rb_গ্রন্থি *parent = ext2fs_rb_parent(গ্রন্থি);

	যদি ((গ্রন্থি->rb_বাম = বাম->rb_ডান))
		ext2fs_rb_set_parent(বাম->rb_ডান, গ্রন্থি);
	বাম->rb_ডান = গ্রন্থি;

	ext2fs_rb_set_parent(বাম, parent);

	যদি (parent)
	{
		যদি (গ্রন্থি == parent->rb_ডান)
			parent->rb_ডান = বাম;
		অন্যথা
			parent->rb_বাম = বাম;
	}
	অন্যথা
		মূল->rb_গ্রন্থি = বাম;
	ext2fs_rb_set_parent(গ্রন্থি, বাম);
}

শূন্য ext2fs_rb_ঢোকাও_color(কাঠামো rb_গ্রন্থি *গ্রন্থি, কাঠামো rb_মূল *মূল)
{
	কাঠামো rb_গ্রন্থি *parent, *gparent;

	যখন ((parent = ext2fs_rb_parent(গ্রন্থি)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		যদি (parent == gparent->rb_বাম)
		{
			{
				নিবন্ধ কাঠামো rb_গ্রন্থি *uncle = gparent->rb_ডান;
				যদি (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					গ্রন্থি = gparent;
					চালু;
				}
			}

			যদি (parent->rb_ডান == গ্রন্থি)
			{
				নিবন্ধ কাঠামো rb_গ্রন্থি *tmp;
				__rb_rotate_left(parent, মূল);
				tmp = parent;
				parent = গ্রন্থি;
				গ্রন্থি = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, মূল);
		} অন্যথা {
			{
				নিবন্ধ কাঠামো rb_গ্রন্থি *uncle = gparent->rb_বাম;
				যদি (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					গ্রন্থি = gparent;
					চালু;
				}
			}

			যদি (parent->rb_বাম == গ্রন্থি)
			{
				নিবন্ধ কাঠামো rb_গ্রন্থি *tmp;
				__rb_rotate_right(parent, মূল);
				tmp = parent;
				parent = গ্রন্থি;
				গ্রন্থি = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, মূল);
		}
	}

	ext2fs_rb_set_black(মূল->rb_গ্রন্থি);
}

স্থির শূন্য __rb_erase_color(কাঠামো rb_গ্রন্থি *গ্রন্থি, কাঠামো rb_গ্রন্থি *parent,
			     কাঠামো rb_মূল *মূল)
{
	কাঠামো rb_গ্রন্থি *other;

	যখন ((!গ্রন্থি || ext2fs_rb_is_black(গ্রন্থি)) && গ্রন্থি != মূল->rb_গ্রন্থি)
	{
		যদি (parent->rb_বাম == গ্রন্থি)
		{
			other = parent->rb_ডান;
			যদি (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, মূল);
				other = parent->rb_ডান;
			}
			যদি ((!other->rb_বাম || ext2fs_rb_is_black(other->rb_বাম)) &&
			    (!other->rb_ডান || ext2fs_rb_is_black(other->rb_ডান)))
			{
				ext2fs_rb_set_red(other);
				গ্রন্থি = parent;
				parent = ext2fs_rb_parent(গ্রন্থি);
			}
			অন্যথা
			{
				যদি (!other->rb_ডান || ext2fs_rb_is_black(other->rb_ডান))
				{
					ext2fs_rb_set_black(other->rb_বাম);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, মূল);
					other = parent->rb_ডান;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_ডান);
				__rb_rotate_left(parent, মূল);
				গ্রন্থি = মূল->rb_গ্রন্থি;
				বিরতি;
			}
		}
		অন্যথা
		{
			other = parent->rb_বাম;
			যদি (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, মূল);
				other = parent->rb_বাম;
			}
			যদি ((!other->rb_বাম || ext2fs_rb_is_black(other->rb_বাম)) &&
			    (!other->rb_ডান || ext2fs_rb_is_black(other->rb_ডান)))
			{
				ext2fs_rb_set_red(other);
				গ্রন্থি = parent;
				parent = ext2fs_rb_parent(গ্রন্থি);
			}
			অন্যথা
			{
				যদি (!other->rb_বাম || ext2fs_rb_is_black(other->rb_বাম))
				{
					ext2fs_rb_set_black(other->rb_ডান);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, মূল);
					other = parent->rb_বাম;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_বাম);
				__rb_rotate_right(parent, মূল);
				গ্রন্থি = মূল->rb_গ্রন্থি;
				বিরতি;
			}
		}
	}
	যদি (গ্রন্থি)
		ext2fs_rb_set_black(গ্রন্থি);
}

শূন্য ext2fs_rb_erase(কাঠামো rb_গ্রন্থি *গ্রন্থি, কাঠামো rb_মূল *মূল)
{
	কাঠামো rb_গ্রন্থি *child, *parent;
	পূর্ণসংখ্যা color;

	যদি (!গ্রন্থি->rb_বাম)
		child = গ্রন্থি->rb_ডান;
	অন্যথা যদি (!গ্রন্থি->rb_ডান)
		child = গ্রন্থি->rb_বাম;
	অন্যথা
	{
		কাঠামো rb_গ্রন্থি *old = গ্রন্থি, *বাম;

		গ্রন্থি = গ্রন্থি->rb_ডান;
		যখন ((বাম = গ্রন্থি->rb_বাম) != ফাঁকা)
			গ্রন্থি = বাম;

		যদি (ext2fs_rb_parent(old)) {
			যদি (ext2fs_rb_parent(old)->rb_বাম == old)
				ext2fs_rb_parent(old)->rb_বাম = গ্রন্থি;
			অন্যথা
				ext2fs_rb_parent(old)->rb_ডান = গ্রন্থি;
		} অন্যথা
			মূল->rb_গ্রন্থি = গ্রন্থি;

		child = গ্রন্থি->rb_ডান;
		parent = ext2fs_rb_parent(গ্রন্থি);
		color = ext2fs_rb_color(গ্রন্থি);

		যদি (parent == old) {
			parent = গ্রন্থি;
		} অন্যথা {
			যদি (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_বাম = child;

			গ্রন্থি->rb_ডান = old->rb_ডান;
			ext2fs_rb_set_parent(old->rb_ডান, গ্রন্থি);
		}

		গ্রন্থি->rb_parent_color = old->rb_parent_color;
		গ্রন্থি->rb_বাম = old->rb_বাম;
		ext2fs_rb_set_parent(old->rb_বাম, গ্রন্থি);

		যাও color;
	}

	parent = ext2fs_rb_parent(গ্রন্থি);
	color = ext2fs_rb_color(গ্রন্থি);

	যদি (child)
		ext2fs_rb_set_parent(child, parent);
	যদি (parent)
	{
		যদি (parent->rb_বাম == গ্রন্থি)
			parent->rb_বাম = child;
		অন্যথা
			parent->rb_ডান = child;
	}
	অন্যথা
		মূল->rb_গ্রন্থি = child;

 color:
	যদি (color == RB_BLACK)
		__rb_erase_color(child, parent, মূল);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
কাঠামো rb_গ্রন্থি *ext2fs_rb_first(ধ্রুবক কাঠামো rb_মূল *মূল)
{
	কাঠামো rb_গ্রন্থি	*n;

	n = মূল->rb_গ্রন্থি;
	যদি (!n)
		ফেরত ফাঁকা;
	যখন (n->rb_বাম)
		n = n->rb_বাম;
	ফেরত n;
}

কাঠামো rb_গ্রন্থি *ext2fs_rb_last(ধ্রুবক কাঠামো rb_মূল *মূল)
{
	কাঠামো rb_গ্রন্থি	*n;

	n = মূল->rb_গ্রন্থি;
	যদি (!n)
		ফেরত ফাঁকা;
	যখন (n->rb_ডান)
		n = n->rb_ডান;
	ফেরত n;
}

কাঠামো rb_গ্রন্থি *ext2fs_rb_পরবর্তী(কাঠামো rb_গ্রন্থি *গ্রন্থি)
{
	কাঠামো rb_গ্রন্থি *parent;

	যদি (ext2fs_rb_parent(গ্রন্থি) == গ্রন্থি)
		ফেরত ফাঁকা;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	যদি (গ্রন্থি->rb_ডান) {
		গ্রন্থি = গ্রন্থি->rb_ডান;
		যখন (গ্রন্থি->rb_বাম)
			গ্রন্থি=গ্রন্থি->rb_বাম;
		ফেরত (কাঠামো rb_গ্রন্থি *)গ্রন্থি;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	যখন ((parent = ext2fs_rb_parent(গ্রন্থি)) && গ্রন্থি == parent->rb_ডান)
		গ্রন্থি = parent;

	ফেরত parent;
}

কাঠামো rb_গ্রন্থি *ext2fs_rb_পূর্ব(কাঠামো rb_গ্রন্থি *গ্রন্থি)
{
	কাঠামো rb_গ্রন্থি *parent;

	যদি (ext2fs_rb_parent(গ্রন্থি) == গ্রন্থি)
		ফেরত ফাঁকা;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	যদি (গ্রন্থি->rb_বাম) {
		গ্রন্থি = গ্রন্থি->rb_বাম;
		যখন (গ্রন্থি->rb_ডান)
			গ্রন্থি=গ্রন্থি->rb_ডান;
		ফেরত (কাঠামো rb_গ্রন্থি *)গ্রন্থি;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	যখন ((parent = ext2fs_rb_parent(গ্রন্থি)) && গ্রন্থি == parent->rb_বাম)
		গ্রন্থি = parent;

	ফেরত parent;
}

শূন্য ext2fs_rb_replace_গ্রন্থি(কাঠামো rb_গ্রন্থি *victim, কাঠামো rb_গ্রন্থি *new,
			  কাঠামো rb_মূল *মূল)
{
	কাঠামো rb_গ্রন্থি *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	যদি (parent) {
		যদি (victim == parent->rb_বাম)
			parent->rb_বাম = new;
		অন্যথা
			parent->rb_ডান = new;
	} অন্যথা {
		মূল->rb_গ্রন্থি = new;
	}
	যদি (victim->rb_বাম)
		ext2fs_rb_set_parent(victim->rb_বাম, new);
	যদি (victim->rb_ডান)
		ext2fs_rb_set_parent(victim->rb_ডান, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
