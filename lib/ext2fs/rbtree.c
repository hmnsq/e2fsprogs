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

#अन्तर्भाव "rbtree.h"

स्थिर शून्य __rb_rotate_left(संरचना rb_गाँठ *गाँठ, संरचना rb_मूल *मूल)
{
	संरचना rb_गाँठ *दायाँ = गाँठ->rb_दायाँ;
	संरचना rb_गाँठ *parent = ext2fs_rb_parent(गाँठ);

	यदि ((गाँठ->rb_दायाँ = दायाँ->rb_बायाँ))
		ext2fs_rb_set_parent(दायाँ->rb_बायाँ, गाँठ);
	दायाँ->rb_बायाँ = गाँठ;

	ext2fs_rb_set_parent(दायाँ, parent);

	यदि (parent)
	{
		यदि (गाँठ == parent->rb_बायाँ)
			parent->rb_बायाँ = दायाँ;
		अन्यथा
			parent->rb_दायाँ = दायाँ;
	}
	अन्यथा
		मूल->rb_गाँठ = दायाँ;
	ext2fs_rb_set_parent(गाँठ, दायाँ);
}

स्थिर शून्य __rb_rotate_right(संरचना rb_गाँठ *गाँठ, संरचना rb_मूल *मूल)
{
	संरचना rb_गाँठ *बायाँ = गाँठ->rb_बायाँ;
	संरचना rb_गाँठ *parent = ext2fs_rb_parent(गाँठ);

	यदि ((गाँठ->rb_बायाँ = बायाँ->rb_दायाँ))
		ext2fs_rb_set_parent(बायाँ->rb_दायाँ, गाँठ);
	बायाँ->rb_दायाँ = गाँठ;

	ext2fs_rb_set_parent(बायाँ, parent);

	यदि (parent)
	{
		यदि (गाँठ == parent->rb_दायाँ)
			parent->rb_दायाँ = बायाँ;
		अन्यथा
			parent->rb_बायाँ = बायाँ;
	}
	अन्यथा
		मूल->rb_गाँठ = बायाँ;
	ext2fs_rb_set_parent(गाँठ, बायाँ);
}

शून्य ext2fs_rb_जोड़ो_color(संरचना rb_गाँठ *गाँठ, संरचना rb_मूल *मूल)
{
	संरचना rb_गाँठ *parent, *gparent;

	यावत् ((parent = ext2fs_rb_parent(गाँठ)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		यदि (parent == gparent->rb_बायाँ)
		{
			{
				पञ्जिका संरचना rb_गाँठ *uncle = gparent->rb_दायाँ;
				यदि (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					गाँठ = gparent;
					अनुवर्त;
				}
			}

			यदि (parent->rb_दायाँ == गाँठ)
			{
				पञ्जिका संरचना rb_गाँठ *tmp;
				__rb_rotate_left(parent, मूल);
				tmp = parent;
				parent = गाँठ;
				गाँठ = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, मूल);
		} अन्यथा {
			{
				पञ्जिका संरचना rb_गाँठ *uncle = gparent->rb_बायाँ;
				यदि (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					गाँठ = gparent;
					अनुवर्त;
				}
			}

			यदि (parent->rb_बायाँ == गाँठ)
			{
				पञ्जिका संरचना rb_गाँठ *tmp;
				__rb_rotate_right(parent, मूल);
				tmp = parent;
				parent = गाँठ;
				गाँठ = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, मूल);
		}
	}

	ext2fs_rb_set_black(मूल->rb_गाँठ);
}

स्थिर शून्य __rb_erase_color(संरचना rb_गाँठ *गाँठ, संरचना rb_गाँठ *parent,
			     संरचना rb_मूल *मूल)
{
	संरचना rb_गाँठ *other;

	यावत् ((!गाँठ || ext2fs_rb_is_black(गाँठ)) && गाँठ != मूल->rb_गाँठ)
	{
		यदि (parent->rb_बायाँ == गाँठ)
		{
			other = parent->rb_दायाँ;
			यदि (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, मूल);
				other = parent->rb_दायाँ;
			}
			यदि ((!other->rb_बायाँ || ext2fs_rb_is_black(other->rb_बायाँ)) &&
			    (!other->rb_दायाँ || ext2fs_rb_is_black(other->rb_दायाँ)))
			{
				ext2fs_rb_set_red(other);
				गाँठ = parent;
				parent = ext2fs_rb_parent(गाँठ);
			}
			अन्यथा
			{
				यदि (!other->rb_दायाँ || ext2fs_rb_is_black(other->rb_दायाँ))
				{
					ext2fs_rb_set_black(other->rb_बायाँ);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, मूल);
					other = parent->rb_दायाँ;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_दायाँ);
				__rb_rotate_left(parent, मूल);
				गाँठ = मूल->rb_गाँठ;
				भङ्ग;
			}
		}
		अन्यथा
		{
			other = parent->rb_बायाँ;
			यदि (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, मूल);
				other = parent->rb_बायाँ;
			}
			यदि ((!other->rb_बायाँ || ext2fs_rb_is_black(other->rb_बायाँ)) &&
			    (!other->rb_दायाँ || ext2fs_rb_is_black(other->rb_दायाँ)))
			{
				ext2fs_rb_set_red(other);
				गाँठ = parent;
				parent = ext2fs_rb_parent(गाँठ);
			}
			अन्यथा
			{
				यदि (!other->rb_बायाँ || ext2fs_rb_is_black(other->rb_बायाँ))
				{
					ext2fs_rb_set_black(other->rb_दायाँ);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, मूल);
					other = parent->rb_बायाँ;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_बायाँ);
				__rb_rotate_right(parent, मूल);
				गाँठ = मूल->rb_गाँठ;
				भङ्ग;
			}
		}
	}
	यदि (गाँठ)
		ext2fs_rb_set_black(गाँठ);
}

शून्य ext2fs_rb_erase(संरचना rb_गाँठ *गाँठ, संरचना rb_मूल *मूल)
{
	संरचना rb_गाँठ *child, *parent;
	पूर्णाङ्क color;

	यदि (!गाँठ->rb_बायाँ)
		child = गाँठ->rb_दायाँ;
	अन्यथा यदि (!गाँठ->rb_दायाँ)
		child = गाँठ->rb_बायाँ;
	अन्यथा
	{
		संरचना rb_गाँठ *old = गाँठ, *बायाँ;

		गाँठ = गाँठ->rb_दायाँ;
		यावत् ((बायाँ = गाँठ->rb_बायाँ) != रिक्त)
			गाँठ = बायाँ;

		यदि (ext2fs_rb_parent(old)) {
			यदि (ext2fs_rb_parent(old)->rb_बायाँ == old)
				ext2fs_rb_parent(old)->rb_बायाँ = गाँठ;
			अन्यथा
				ext2fs_rb_parent(old)->rb_दायाँ = गाँठ;
		} अन्यथा
			मूल->rb_गाँठ = गाँठ;

		child = गाँठ->rb_दायाँ;
		parent = ext2fs_rb_parent(गाँठ);
		color = ext2fs_rb_color(गाँठ);

		यदि (parent == old) {
			parent = गाँठ;
		} अन्यथा {
			यदि (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_बायाँ = child;

			गाँठ->rb_दायाँ = old->rb_दायाँ;
			ext2fs_rb_set_parent(old->rb_दायाँ, गाँठ);
		}

		गाँठ->rb_parent_color = old->rb_parent_color;
		गाँठ->rb_बायाँ = old->rb_बायाँ;
		ext2fs_rb_set_parent(old->rb_बायाँ, गाँठ);

		गच्छ color;
	}

	parent = ext2fs_rb_parent(गाँठ);
	color = ext2fs_rb_color(गाँठ);

	यदि (child)
		ext2fs_rb_set_parent(child, parent);
	यदि (parent)
	{
		यदि (parent->rb_बायाँ == गाँठ)
			parent->rb_बायाँ = child;
		अन्यथा
			parent->rb_दायाँ = child;
	}
	अन्यथा
		मूल->rb_गाँठ = child;

 color:
	यदि (color == RB_BLACK)
		__rb_erase_color(child, parent, मूल);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
संरचना rb_गाँठ *ext2fs_rb_first(ध्रुव संरचना rb_मूल *मूल)
{
	संरचना rb_गाँठ	*n;

	n = मूल->rb_गाँठ;
	यदि (!n)
		प्रत्यागम रिक्त;
	यावत् (n->rb_बायाँ)
		n = n->rb_बायाँ;
	प्रत्यागम n;
}

संरचना rb_गाँठ *ext2fs_rb_last(ध्रुव संरचना rb_मूल *मूल)
{
	संरचना rb_गाँठ	*n;

	n = मूल->rb_गाँठ;
	यदि (!n)
		प्रत्यागम रिक्त;
	यावत् (n->rb_दायाँ)
		n = n->rb_दायाँ;
	प्रत्यागम n;
}

संरचना rb_गाँठ *ext2fs_rb_next(संरचना rb_गाँठ *गाँठ)
{
	संरचना rb_गाँठ *parent;

	यदि (ext2fs_rb_parent(गाँठ) == गाँठ)
		प्रत्यागम रिक्त;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	यदि (गाँठ->rb_दायाँ) {
		गाँठ = गाँठ->rb_दायाँ;
		यावत् (गाँठ->rb_बायाँ)
			गाँठ=गाँठ->rb_बायाँ;
		प्रत्यागम (संरचना rb_गाँठ *)गाँठ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	यावत् ((parent = ext2fs_rb_parent(गाँठ)) && गाँठ == parent->rb_दायाँ)
		गाँठ = parent;

	प्रत्यागम parent;
}

संरचना rb_गाँठ *ext2fs_rb_prev(संरचना rb_गाँठ *गाँठ)
{
	संरचना rb_गाँठ *parent;

	यदि (ext2fs_rb_parent(गाँठ) == गाँठ)
		प्रत्यागम रिक्त;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	यदि (गाँठ->rb_बायाँ) {
		गाँठ = गाँठ->rb_बायाँ;
		यावत् (गाँठ->rb_दायाँ)
			गाँठ=गाँठ->rb_दायाँ;
		प्रत्यागम (संरचना rb_गाँठ *)गाँठ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	यावत् ((parent = ext2fs_rb_parent(गाँठ)) && गाँठ == parent->rb_बायाँ)
		गाँठ = parent;

	प्रत्यागम parent;
}

शून्य ext2fs_rb_replace_गाँठ(संरचना rb_गाँठ *victim, संरचना rb_गाँठ *new,
			  संरचना rb_मूल *मूल)
{
	संरचना rb_गाँठ *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	यदि (parent) {
		यदि (victim == parent->rb_बायाँ)
			parent->rb_बायाँ = new;
		अन्यथा
			parent->rb_दायाँ = new;
	} अन्यथा {
		मूल->rb_गाँठ = new;
	}
	यदि (victim->rb_बायाँ)
		ext2fs_rb_set_parent(victim->rb_बायाँ, new);
	यदि (victim->rb_दायाँ)
		ext2fs_rb_set_parent(victim->rb_दायाँ, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
