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

स्थिर शून्य __rb_rotate_left(संरचना rb_गाठ *गाठ, संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ *उजवा = गाठ->rb_उजवा;
	संरचना rb_गाठ *parent = ext2fs_rb_parent(गाठ);

	यदि ((गाठ->rb_उजवा = उजवा->rb_डावा))
		ext2fs_rb_set_parent(उजवा->rb_डावा, गाठ);
	उजवा->rb_डावा = गाठ;

	ext2fs_rb_set_parent(उजवा, parent);

	यदि (parent)
	{
		यदि (गाठ == parent->rb_डावा)
			parent->rb_डावा = उजवा;
		अन्यथा
			parent->rb_उजवा = उजवा;
	}
	अन्यथा
		मूळ->rb_गाठ = उजवा;
	ext2fs_rb_set_parent(गाठ, उजवा);
}

स्थिर शून्य __rb_rotate_right(संरचना rb_गाठ *गाठ, संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ *डावा = गाठ->rb_डावा;
	संरचना rb_गाठ *parent = ext2fs_rb_parent(गाठ);

	यदि ((गाठ->rb_डावा = डावा->rb_उजवा))
		ext2fs_rb_set_parent(डावा->rb_उजवा, गाठ);
	डावा->rb_उजवा = गाठ;

	ext2fs_rb_set_parent(डावा, parent);

	यदि (parent)
	{
		यदि (गाठ == parent->rb_उजवा)
			parent->rb_उजवा = डावा;
		अन्यथा
			parent->rb_डावा = डावा;
	}
	अन्यथा
		मूळ->rb_गाठ = डावा;
	ext2fs_rb_set_parent(गाठ, डावा);
}

शून्य ext2fs_rb_घाला_color(संरचना rb_गाठ *गाठ, संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ *parent, *gparent;

	यावत् ((parent = ext2fs_rb_parent(गाठ)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		यदि (parent == gparent->rb_डावा)
		{
			{
				पञ्जिका संरचना rb_गाठ *uncle = gparent->rb_उजवा;
				यदि (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					गाठ = gparent;
					अनुवर्त;
				}
			}

			यदि (parent->rb_उजवा == गाठ)
			{
				पञ्जिका संरचना rb_गाठ *tmp;
				__rb_rotate_left(parent, मूळ);
				tmp = parent;
				parent = गाठ;
				गाठ = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, मूळ);
		} अन्यथा {
			{
				पञ्जिका संरचना rb_गाठ *uncle = gparent->rb_डावा;
				यदि (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					गाठ = gparent;
					अनुवर्त;
				}
			}

			यदि (parent->rb_डावा == गाठ)
			{
				पञ्जिका संरचना rb_गाठ *tmp;
				__rb_rotate_right(parent, मूळ);
				tmp = parent;
				parent = गाठ;
				गाठ = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, मूळ);
		}
	}

	ext2fs_rb_set_black(मूळ->rb_गाठ);
}

स्थिर शून्य __rb_erase_color(संरचना rb_गाठ *गाठ, संरचना rb_गाठ *parent,
			     संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ *other;

	यावत् ((!गाठ || ext2fs_rb_is_black(गाठ)) && गाठ != मूळ->rb_गाठ)
	{
		यदि (parent->rb_डावा == गाठ)
		{
			other = parent->rb_उजवा;
			यदि (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, मूळ);
				other = parent->rb_उजवा;
			}
			यदि ((!other->rb_डावा || ext2fs_rb_is_black(other->rb_डावा)) &&
			    (!other->rb_उजवा || ext2fs_rb_is_black(other->rb_उजवा)))
			{
				ext2fs_rb_set_red(other);
				गाठ = parent;
				parent = ext2fs_rb_parent(गाठ);
			}
			अन्यथा
			{
				यदि (!other->rb_उजवा || ext2fs_rb_is_black(other->rb_उजवा))
				{
					ext2fs_rb_set_black(other->rb_डावा);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, मूळ);
					other = parent->rb_उजवा;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_उजवा);
				__rb_rotate_left(parent, मूळ);
				गाठ = मूळ->rb_गाठ;
				भङ्ग;
			}
		}
		अन्यथा
		{
			other = parent->rb_डावा;
			यदि (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, मूळ);
				other = parent->rb_डावा;
			}
			यदि ((!other->rb_डावा || ext2fs_rb_is_black(other->rb_डावा)) &&
			    (!other->rb_उजवा || ext2fs_rb_is_black(other->rb_उजवा)))
			{
				ext2fs_rb_set_red(other);
				गाठ = parent;
				parent = ext2fs_rb_parent(गाठ);
			}
			अन्यथा
			{
				यदि (!other->rb_डावा || ext2fs_rb_is_black(other->rb_डावा))
				{
					ext2fs_rb_set_black(other->rb_उजवा);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, मूळ);
					other = parent->rb_डावा;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_डावा);
				__rb_rotate_right(parent, मूळ);
				गाठ = मूळ->rb_गाठ;
				भङ्ग;
			}
		}
	}
	यदि (गाठ)
		ext2fs_rb_set_black(गाठ);
}

शून्य ext2fs_rb_erase(संरचना rb_गाठ *गाठ, संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ *child, *parent;
	पूर्णाङ्क color;

	यदि (!गाठ->rb_डावा)
		child = गाठ->rb_उजवा;
	अन्यथा यदि (!गाठ->rb_उजवा)
		child = गाठ->rb_डावा;
	अन्यथा
	{
		संरचना rb_गाठ *old = गाठ, *डावा;

		गाठ = गाठ->rb_उजवा;
		यावत् ((डावा = गाठ->rb_डावा) != रिक्त)
			गाठ = डावा;

		यदि (ext2fs_rb_parent(old)) {
			यदि (ext2fs_rb_parent(old)->rb_डावा == old)
				ext2fs_rb_parent(old)->rb_डावा = गाठ;
			अन्यथा
				ext2fs_rb_parent(old)->rb_उजवा = गाठ;
		} अन्यथा
			मूळ->rb_गाठ = गाठ;

		child = गाठ->rb_उजवा;
		parent = ext2fs_rb_parent(गाठ);
		color = ext2fs_rb_color(गाठ);

		यदि (parent == old) {
			parent = गाठ;
		} अन्यथा {
			यदि (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_डावा = child;

			गाठ->rb_उजवा = old->rb_उजवा;
			ext2fs_rb_set_parent(old->rb_उजवा, गाठ);
		}

		गाठ->rb_parent_color = old->rb_parent_color;
		गाठ->rb_डावा = old->rb_डावा;
		ext2fs_rb_set_parent(old->rb_डावा, गाठ);

		गच्छ color;
	}

	parent = ext2fs_rb_parent(गाठ);
	color = ext2fs_rb_color(गाठ);

	यदि (child)
		ext2fs_rb_set_parent(child, parent);
	यदि (parent)
	{
		यदि (parent->rb_डावा == गाठ)
			parent->rb_डावा = child;
		अन्यथा
			parent->rb_उजवा = child;
	}
	अन्यथा
		मूळ->rb_गाठ = child;

 color:
	यदि (color == RB_BLACK)
		__rb_erase_color(child, parent, मूळ);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
संरचना rb_गाठ *ext2fs_rb_first(ध्रुव संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ	*n;

	n = मूळ->rb_गाठ;
	यदि (!n)
		प्रत्यागम रिक्त;
	यावत् (n->rb_डावा)
		n = n->rb_डावा;
	प्रत्यागम n;
}

संरचना rb_गाठ *ext2fs_rb_last(ध्रुव संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ	*n;

	n = मूळ->rb_गाठ;
	यदि (!n)
		प्रत्यागम रिक्त;
	यावत् (n->rb_उजवा)
		n = n->rb_उजवा;
	प्रत्यागम n;
}

संरचना rb_गाठ *ext2fs_rb_next(संरचना rb_गाठ *गाठ)
{
	संरचना rb_गाठ *parent;

	यदि (ext2fs_rb_parent(गाठ) == गाठ)
		प्रत्यागम रिक्त;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	यदि (गाठ->rb_उजवा) {
		गाठ = गाठ->rb_उजवा;
		यावत् (गाठ->rb_डावा)
			गाठ=गाठ->rb_डावा;
		प्रत्यागम (संरचना rb_गाठ *)गाठ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	यावत् ((parent = ext2fs_rb_parent(गाठ)) && गाठ == parent->rb_उजवा)
		गाठ = parent;

	प्रत्यागम parent;
}

संरचना rb_गाठ *ext2fs_rb_prev(संरचना rb_गाठ *गाठ)
{
	संरचना rb_गाठ *parent;

	यदि (ext2fs_rb_parent(गाठ) == गाठ)
		प्रत्यागम रिक्त;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	यदि (गाठ->rb_डावा) {
		गाठ = गाठ->rb_डावा;
		यावत् (गाठ->rb_उजवा)
			गाठ=गाठ->rb_उजवा;
		प्रत्यागम (संरचना rb_गाठ *)गाठ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	यावत् ((parent = ext2fs_rb_parent(गाठ)) && गाठ == parent->rb_डावा)
		गाठ = parent;

	प्रत्यागम parent;
}

शून्य ext2fs_rb_replace_गाठ(संरचना rb_गाठ *victim, संरचना rb_गाठ *new,
			  संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	यदि (parent) {
		यदि (victim == parent->rb_डावा)
			parent->rb_डावा = new;
		अन्यथा
			parent->rb_उजवा = new;
	} अन्यथा {
		मूळ->rb_गाठ = new;
	}
	यदि (victim->rb_डावा)
		ext2fs_rb_set_parent(victim->rb_डावा, new);
	यदि (victim->rb_उजवा)
		ext2fs_rb_set_parent(victim->rb_उजवा, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
