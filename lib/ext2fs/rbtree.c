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

#അന്തര്ഭാവ "rbtree.h"

സ്ഥിതം ശൂന്യം __rb_rotate_left(ഘടന rb_കണ്ണി *കണ്ണി, ഘടന rb_വേര് *വേര്)
{
	ഘടന rb_കണ്ണി *വലത് = കണ്ണി->rb_വലത്;
	ഘടന rb_കണ്ണി *parent = ext2fs_rb_parent(കണ്ണി);

	എങ്കിൽ ((കണ്ണി->rb_വലത് = വലത്->rb_ഇടത്))
		ext2fs_rb_set_parent(വലത്->rb_ഇടത്, കണ്ണി);
	വലത്->rb_ഇടത് = കണ്ണി;

	ext2fs_rb_set_parent(വലത്, parent);

	എങ്കിൽ (parent)
	{
		എങ്കിൽ (കണ്ണി == parent->rb_ഇടത്)
			parent->rb_ഇടത് = വലത്;
		അല്ലെങ്കിൽ
			parent->rb_വലത് = വലത്;
	}
	അല്ലെങ്കിൽ
		വേര്->rb_കണ്ണി = വലത്;
	ext2fs_rb_set_parent(കണ്ണി, വലത്);
}

സ്ഥിതം ശൂന്യം __rb_rotate_right(ഘടന rb_കണ്ണി *കണ്ണി, ഘടന rb_വേര് *വേര്)
{
	ഘടന rb_കണ്ണി *ഇടത് = കണ്ണി->rb_ഇടത്;
	ഘടന rb_കണ്ണി *parent = ext2fs_rb_parent(കണ്ണി);

	എങ്കിൽ ((കണ്ണി->rb_ഇടത് = ഇടത്->rb_വലത്))
		ext2fs_rb_set_parent(ഇടത്->rb_വലത്, കണ്ണി);
	ഇടത്->rb_വലത് = കണ്ണി;

	ext2fs_rb_set_parent(ഇടത്, parent);

	എങ്കിൽ (parent)
	{
		എങ്കിൽ (കണ്ണി == parent->rb_വലത്)
			parent->rb_വലത് = ഇടത്;
		അല്ലെങ്കിൽ
			parent->rb_ഇടത് = ഇടത്;
	}
	അല്ലെങ്കിൽ
		വേര്->rb_കണ്ണി = ഇടത്;
	ext2fs_rb_set_parent(കണ്ണി, ഇടത്);
}

ശൂന്യം ext2fs_rb_തിരുകുക_color(ഘടന rb_കണ്ണി *കണ്ണി, ഘടന rb_വേര് *വേര്)
{
	ഘടന rb_കണ്ണി *parent, *gparent;

	ആകുമ്പോൾ ((parent = ext2fs_rb_parent(കണ്ണി)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		എങ്കിൽ (parent == gparent->rb_ഇടത്)
		{
			{
				രജിസ്റ്റർ ഘടന rb_കണ്ണി *uncle = gparent->rb_വലത്;
				എങ്കിൽ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					കണ്ണി = gparent;
					തുടരുക;
				}
			}

			എങ്കിൽ (parent->rb_വലത് == കണ്ണി)
			{
				രജിസ്റ്റർ ഘടന rb_കണ്ണി *tmp;
				__rb_rotate_left(parent, വേര്);
				tmp = parent;
				parent = കണ്ണി;
				കണ്ണി = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, വേര്);
		} അല്ലെങ്കിൽ {
			{
				രജിസ്റ്റർ ഘടന rb_കണ്ണി *uncle = gparent->rb_ഇടത്;
				എങ്കിൽ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					കണ്ണി = gparent;
					തുടരുക;
				}
			}

			എങ്കിൽ (parent->rb_ഇടത് == കണ്ണി)
			{
				രജിസ്റ്റർ ഘടന rb_കണ്ണി *tmp;
				__rb_rotate_right(parent, വേര്);
				tmp = parent;
				parent = കണ്ണി;
				കണ്ണി = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, വേര്);
		}
	}

	ext2fs_rb_set_black(വേര്->rb_കണ്ണി);
}

സ്ഥിതം ശൂന്യം __rb_erase_color(ഘടന rb_കണ്ണി *കണ്ണി, ഘടന rb_കണ്ണി *parent,
			     ഘടന rb_വേര് *വേര്)
{
	ഘടന rb_കണ്ണി *other;

	ആകുമ്പോൾ ((!കണ്ണി || ext2fs_rb_is_black(കണ്ണി)) && കണ്ണി != വേര്->rb_കണ്ണി)
	{
		എങ്കിൽ (parent->rb_ഇടത് == കണ്ണി)
		{
			other = parent->rb_വലത്;
			എങ്കിൽ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, വേര്);
				other = parent->rb_വലത്;
			}
			എങ്കിൽ ((!other->rb_ഇടത് || ext2fs_rb_is_black(other->rb_ഇടത്)) &&
			    (!other->rb_വലത് || ext2fs_rb_is_black(other->rb_വലത്)))
			{
				ext2fs_rb_set_red(other);
				കണ്ണി = parent;
				parent = ext2fs_rb_parent(കണ്ണി);
			}
			അല്ലെങ്കിൽ
			{
				എങ്കിൽ (!other->rb_വലത് || ext2fs_rb_is_black(other->rb_വലത്))
				{
					ext2fs_rb_set_black(other->rb_ഇടത്);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, വേര്);
					other = parent->rb_വലത്;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_വലത്);
				__rb_rotate_left(parent, വേര്);
				കണ്ണി = വേര്->rb_കണ്ണി;
				നിർത്തുക;
			}
		}
		അല്ലെങ്കിൽ
		{
			other = parent->rb_ഇടത്;
			എങ്കിൽ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, വേര്);
				other = parent->rb_ഇടത്;
			}
			എങ്കിൽ ((!other->rb_ഇടത് || ext2fs_rb_is_black(other->rb_ഇടത്)) &&
			    (!other->rb_വലത് || ext2fs_rb_is_black(other->rb_വലത്)))
			{
				ext2fs_rb_set_red(other);
				കണ്ണി = parent;
				parent = ext2fs_rb_parent(കണ്ണി);
			}
			അല്ലെങ്കിൽ
			{
				എങ്കിൽ (!other->rb_ഇടത് || ext2fs_rb_is_black(other->rb_ഇടത്))
				{
					ext2fs_rb_set_black(other->rb_വലത്);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, വേര്);
					other = parent->rb_ഇടത്;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_ഇടത്);
				__rb_rotate_right(parent, വേര്);
				കണ്ണി = വേര്->rb_കണ്ണി;
				നിർത്തുക;
			}
		}
	}
	എങ്കിൽ (കണ്ണി)
		ext2fs_rb_set_black(കണ്ണി);
}

ശൂന്യം ext2fs_rb_erase(ഘടന rb_കണ്ണി *കണ്ണി, ഘടന rb_വേര് *വേര്)
{
	ഘടന rb_കണ്ണി *child, *parent;
	പൂർണ്ണസംഖ്യ color;

	എങ്കിൽ (!കണ്ണി->rb_ഇടത്)
		child = കണ്ണി->rb_വലത്;
	അല്ലെങ്കിൽ എങ്കിൽ (!കണ്ണി->rb_വലത്)
		child = കണ്ണി->rb_ഇടത്;
	അല്ലെങ്കിൽ
	{
		ഘടന rb_കണ്ണി *old = കണ്ണി, *ഇടത്;

		കണ്ണി = കണ്ണി->rb_വലത്;
		ആകുമ്പോൾ ((ഇടത് = കണ്ണി->rb_ഇടത്) != ശൂന്യ)
			കണ്ണി = ഇടത്;

		എങ്കിൽ (ext2fs_rb_parent(old)) {
			എങ്കിൽ (ext2fs_rb_parent(old)->rb_ഇടത് == old)
				ext2fs_rb_parent(old)->rb_ഇടത് = കണ്ണി;
			അല്ലെങ്കിൽ
				ext2fs_rb_parent(old)->rb_വലത് = കണ്ണി;
		} അല്ലെങ്കിൽ
			വേര്->rb_കണ്ണി = കണ്ണി;

		child = കണ്ണി->rb_വലത്;
		parent = ext2fs_rb_parent(കണ്ണി);
		color = ext2fs_rb_color(കണ്ണി);

		എങ്കിൽ (parent == old) {
			parent = കണ്ണി;
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_ഇടത് = child;

			കണ്ണി->rb_വലത് = old->rb_വലത്;
			ext2fs_rb_set_parent(old->rb_വലത്, കണ്ണി);
		}

		കണ്ണി->rb_parent_color = old->rb_parent_color;
		കണ്ണി->rb_ഇടത് = old->rb_ഇടത്;
		ext2fs_rb_set_parent(old->rb_ഇടത്, കണ്ണി);

		പോകുക color;
	}

	parent = ext2fs_rb_parent(കണ്ണി);
	color = ext2fs_rb_color(കണ്ണി);

	എങ്കിൽ (child)
		ext2fs_rb_set_parent(child, parent);
	എങ്കിൽ (parent)
	{
		എങ്കിൽ (parent->rb_ഇടത് == കണ്ണി)
			parent->rb_ഇടത് = child;
		അല്ലെങ്കിൽ
			parent->rb_വലത് = child;
	}
	അല്ലെങ്കിൽ
		വേര്->rb_കണ്ണി = child;

 color:
	എങ്കിൽ (color == RB_BLACK)
		__rb_erase_color(child, parent, വേര്);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
ഘടന rb_കണ്ണി *ext2fs_rb_first(സ്ഥിരം ഘടന rb_വേര് *വേര്)
{
	ഘടന rb_കണ്ണി	*n;

	n = വേര്->rb_കണ്ണി;
	എങ്കിൽ (!n)
		തിരികെ ശൂന്യ;
	ആകുമ്പോൾ (n->rb_ഇടത്)
		n = n->rb_ഇടത്;
	തിരികെ n;
}

ഘടന rb_കണ്ണി *ext2fs_rb_last(സ്ഥിരം ഘടന rb_വേര് *വേര്)
{
	ഘടന rb_കണ്ണി	*n;

	n = വേര്->rb_കണ്ണി;
	എങ്കിൽ (!n)
		തിരികെ ശൂന്യ;
	ആകുമ്പോൾ (n->rb_വലത്)
		n = n->rb_വലത്;
	തിരികെ n;
}

ഘടന rb_കണ്ണി *ext2fs_rb_next(ഘടന rb_കണ്ണി *കണ്ണി)
{
	ഘടന rb_കണ്ണി *parent;

	എങ്കിൽ (ext2fs_rb_parent(കണ്ണി) == കണ്ണി)
		തിരികെ ശൂന്യ;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	എങ്കിൽ (കണ്ണി->rb_വലത്) {
		കണ്ണി = കണ്ണി->rb_വലത്;
		ആകുമ്പോൾ (കണ്ണി->rb_ഇടത്)
			കണ്ണി=കണ്ണി->rb_ഇടത്;
		തിരികെ (ഘടന rb_കണ്ണി *)കണ്ണി;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	ആകുമ്പോൾ ((parent = ext2fs_rb_parent(കണ്ണി)) && കണ്ണി == parent->rb_വലത്)
		കണ്ണി = parent;

	തിരികെ parent;
}

ഘടന rb_കണ്ണി *ext2fs_rb_prev(ഘടന rb_കണ്ണി *കണ്ണി)
{
	ഘടന rb_കണ്ണി *parent;

	എങ്കിൽ (ext2fs_rb_parent(കണ്ണി) == കണ്ണി)
		തിരികെ ശൂന്യ;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	എങ്കിൽ (കണ്ണി->rb_ഇടത്) {
		കണ്ണി = കണ്ണി->rb_ഇടത്;
		ആകുമ്പോൾ (കണ്ണി->rb_വലത്)
			കണ്ണി=കണ്ണി->rb_വലത്;
		തിരികെ (ഘടന rb_കണ്ണി *)കണ്ണി;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	ആകുമ്പോൾ ((parent = ext2fs_rb_parent(കണ്ണി)) && കണ്ണി == parent->rb_ഇടത്)
		കണ്ണി = parent;

	തിരികെ parent;
}

ശൂന്യം ext2fs_rb_replace_കണ്ണി(ഘടന rb_കണ്ണി *victim, ഘടന rb_കണ്ണി *new,
			  ഘടന rb_വേര് *വേര്)
{
	ഘടന rb_കണ്ണി *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	എങ്കിൽ (parent) {
		എങ്കിൽ (victim == parent->rb_ഇടത്)
			parent->rb_ഇടത് = new;
		അല്ലെങ്കിൽ
			parent->rb_വലത് = new;
	} അല്ലെങ്കിൽ {
		വേര്->rb_കണ്ണി = new;
	}
	എങ്കിൽ (victim->rb_ഇടത്)
		ext2fs_rb_set_parent(victim->rb_ഇടത്, new);
	എങ്കിൽ (victim->rb_വലത്)
		ext2fs_rb_set_parent(victim->rb_വലത്, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
