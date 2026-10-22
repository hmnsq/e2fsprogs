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

	जर ((गाठ->rb_उजवा = उजवा->rb_डावा))
		ext2fs_rb_set_parent(उजवा->rb_डावा, गाठ);
	उजवा->rb_डावा = गाठ;

	ext2fs_rb_set_parent(उजवा, parent);

	जर (parent)
	{
		जर (गाठ == parent->rb_डावा)
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

	जर ((गाठ->rb_डावा = डावा->rb_उजवा))
		ext2fs_rb_set_parent(डावा->rb_उजवा, गाठ);
	डावा->rb_उजवा = गाठ;

	ext2fs_rb_set_parent(डावा, parent);

	जर (parent)
	{
		जर (गाठ == parent->rb_उजवा)
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

	पर्यंत ((parent = ext2fs_rb_parent(गाठ)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		जर (parent == gparent->rb_डावा)
		{
			{
				नोंदणी संरचना rb_गाठ *uncle = gparent->rb_उजवा;
				जर (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					गाठ = gparent;
					सुरूठेवा;
				}
			}

			जर (parent->rb_उजवा == गाठ)
			{
				नोंदणी संरचना rb_गाठ *tmp;
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
				नोंदणी संरचना rb_गाठ *uncle = gparent->rb_डावा;
				जर (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					गाठ = gparent;
					सुरूठेवा;
				}
			}

			जर (parent->rb_डावा == गाठ)
			{
				नोंदणी संरचना rb_गाठ *tmp;
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

	पर्यंत ((!गाठ || ext2fs_rb_is_black(गाठ)) && गाठ != मूळ->rb_गाठ)
	{
		जर (parent->rb_डावा == गाठ)
		{
			other = parent->rb_उजवा;
			जर (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, मूळ);
				other = parent->rb_उजवा;
			}
			जर ((!other->rb_डावा || ext2fs_rb_is_black(other->rb_डावा)) &&
			    (!other->rb_उजवा || ext2fs_rb_is_black(other->rb_उजवा)))
			{
				ext2fs_rb_set_red(other);
				गाठ = parent;
				parent = ext2fs_rb_parent(गाठ);
			}
			अन्यथा
			{
				जर (!other->rb_उजवा || ext2fs_rb_is_black(other->rb_उजवा))
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
				थांबा;
			}
		}
		अन्यथा
		{
			other = parent->rb_डावा;
			जर (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, मूळ);
				other = parent->rb_डावा;
			}
			जर ((!other->rb_डावा || ext2fs_rb_is_black(other->rb_डावा)) &&
			    (!other->rb_उजवा || ext2fs_rb_is_black(other->rb_उजवा)))
			{
				ext2fs_rb_set_red(other);
				गाठ = parent;
				parent = ext2fs_rb_parent(गाठ);
			}
			अन्यथा
			{
				जर (!other->rb_डावा || ext2fs_rb_is_black(other->rb_डावा))
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
				थांबा;
			}
		}
	}
	जर (गाठ)
		ext2fs_rb_set_black(गाठ);
}

शून्य ext2fs_rb_erase(संरचना rb_गाठ *गाठ, संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ *child, *parent;
	पूर्णांक color;

	जर (!गाठ->rb_डावा)
		child = गाठ->rb_उजवा;
	अन्यथा जर (!गाठ->rb_उजवा)
		child = गाठ->rb_डावा;
	अन्यथा
	{
		संरचना rb_गाठ *old = गाठ, *डावा;

		गाठ = गाठ->rb_उजवा;
		पर्यंत ((डावा = गाठ->rb_डावा) != रिक्त)
			गाठ = डावा;

		जर (ext2fs_rb_parent(old)) {
			जर (ext2fs_rb_parent(old)->rb_डावा == old)
				ext2fs_rb_parent(old)->rb_डावा = गाठ;
			अन्यथा
				ext2fs_rb_parent(old)->rb_उजवा = गाठ;
		} अन्यथा
			मूळ->rb_गाठ = गाठ;

		child = गाठ->rb_उजवा;
		parent = ext2fs_rb_parent(गाठ);
		color = ext2fs_rb_color(गाठ);

		जर (parent == old) {
			parent = गाठ;
		} अन्यथा {
			जर (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_डावा = child;

			गाठ->rb_उजवा = old->rb_उजवा;
			ext2fs_rb_set_parent(old->rb_उजवा, गाठ);
		}

		गाठ->rb_parent_color = old->rb_parent_color;
		गाठ->rb_डावा = old->rb_डावा;
		ext2fs_rb_set_parent(old->rb_डावा, गाठ);

		जा color;
	}

	parent = ext2fs_rb_parent(गाठ);
	color = ext2fs_rb_color(गाठ);

	जर (child)
		ext2fs_rb_set_parent(child, parent);
	जर (parent)
	{
		जर (parent->rb_डावा == गाठ)
			parent->rb_डावा = child;
		अन्यथा
			parent->rb_उजवा = child;
	}
	अन्यथा
		मूळ->rb_गाठ = child;

 color:
	जर (color == RB_BLACK)
		__rb_erase_color(child, parent, मूळ);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
संरचना rb_गाठ *ext2fs_rb_first(स्थिरांक संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ	*n;

	n = मूळ->rb_गाठ;
	जर (!n)
		परत रिक्त;
	पर्यंत (n->rb_डावा)
		n = n->rb_डावा;
	परत n;
}

संरचना rb_गाठ *ext2fs_rb_last(स्थिरांक संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ	*n;

	n = मूळ->rb_गाठ;
	जर (!n)
		परत रिक्त;
	पर्यंत (n->rb_उजवा)
		n = n->rb_उजवा;
	परत n;
}

संरचना rb_गाठ *ext2fs_rb_next(संरचना rb_गाठ *गाठ)
{
	संरचना rb_गाठ *parent;

	जर (ext2fs_rb_parent(गाठ) == गाठ)
		परत रिक्त;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	जर (गाठ->rb_उजवा) {
		गाठ = गाठ->rb_उजवा;
		पर्यंत (गाठ->rb_डावा)
			गाठ=गाठ->rb_डावा;
		परत (संरचना rb_गाठ *)गाठ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	पर्यंत ((parent = ext2fs_rb_parent(गाठ)) && गाठ == parent->rb_उजवा)
		गाठ = parent;

	परत parent;
}

संरचना rb_गाठ *ext2fs_rb_prev(संरचना rb_गाठ *गाठ)
{
	संरचना rb_गाठ *parent;

	जर (ext2fs_rb_parent(गाठ) == गाठ)
		परत रिक्त;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	जर (गाठ->rb_डावा) {
		गाठ = गाठ->rb_डावा;
		पर्यंत (गाठ->rb_उजवा)
			गाठ=गाठ->rb_उजवा;
		परत (संरचना rb_गाठ *)गाठ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	पर्यंत ((parent = ext2fs_rb_parent(गाठ)) && गाठ == parent->rb_डावा)
		गाठ = parent;

	परत parent;
}

शून्य ext2fs_rb_replace_गाठ(संरचना rb_गाठ *victim, संरचना rb_गाठ *new,
			  संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	जर (parent) {
		जर (victim == parent->rb_डावा)
			parent->rb_डावा = new;
		अन्यथा
			parent->rb_उजवा = new;
	} अन्यथा {
		मूळ->rb_गाठ = new;
	}
	जर (victim->rb_डावा)
		ext2fs_rb_set_parent(victim->rb_डावा, new);
	जर (victim->rb_उजवा)
		ext2fs_rb_set_parent(victim->rb_उजवा, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
