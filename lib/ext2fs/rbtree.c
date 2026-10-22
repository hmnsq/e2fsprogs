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

#समावेश "rbtree.h"

अटल व्योम __rb_rotate_left(काष्ठा rb_गाँठ *गाँठ, काष्ठा rb_मूल *मूल)
{
	काष्ठा rb_गाँठ *दायाँ = गाँठ->rb_दायाँ;
	काष्ठा rb_गाँठ *parent = ext2fs_rb_parent(गाँठ);

	अगर ((गाँठ->rb_दायाँ = दायाँ->rb_बायाँ))
		ext2fs_rb_set_parent(दायाँ->rb_बायाँ, गाँठ);
	दायाँ->rb_बायाँ = गाँठ;

	ext2fs_rb_set_parent(दायाँ, parent);

	अगर (parent)
	{
		अगर (गाँठ == parent->rb_बायाँ)
			parent->rb_बायाँ = दायाँ;
		अन्यथा
			parent->rb_दायाँ = दायाँ;
	}
	अन्यथा
		मूल->rb_गाँठ = दायाँ;
	ext2fs_rb_set_parent(गाँठ, दायाँ);
}

अटल व्योम __rb_rotate_right(काष्ठा rb_गाँठ *गाँठ, काष्ठा rb_मूल *मूल)
{
	काष्ठा rb_गाँठ *बायाँ = गाँठ->rb_बायाँ;
	काष्ठा rb_गाँठ *parent = ext2fs_rb_parent(गाँठ);

	अगर ((गाँठ->rb_बायाँ = बायाँ->rb_दायाँ))
		ext2fs_rb_set_parent(बायाँ->rb_दायाँ, गाँठ);
	बायाँ->rb_दायाँ = गाँठ;

	ext2fs_rb_set_parent(बायाँ, parent);

	अगर (parent)
	{
		अगर (गाँठ == parent->rb_दायाँ)
			parent->rb_दायाँ = बायाँ;
		अन्यथा
			parent->rb_बायाँ = बायाँ;
	}
	अन्यथा
		मूल->rb_गाँठ = बायाँ;
	ext2fs_rb_set_parent(गाँठ, बायाँ);
}

व्योम ext2fs_rb_जोड़ो_color(काष्ठा rb_गाँठ *गाँठ, काष्ठा rb_मूल *मूल)
{
	काष्ठा rb_गाँठ *parent, *gparent;

	जबतक ((parent = ext2fs_rb_parent(गाँठ)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		अगर (parent == gparent->rb_बायाँ)
		{
			{
				रेजिस्टर काष्ठा rb_गाँठ *uncle = gparent->rb_दायाँ;
				अगर (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					गाँठ = gparent;
					जारी;
				}
			}

			अगर (parent->rb_दायाँ == गाँठ)
			{
				रेजिस्टर काष्ठा rb_गाँठ *tmp;
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
				रेजिस्टर काष्ठा rb_गाँठ *uncle = gparent->rb_बायाँ;
				अगर (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					गाँठ = gparent;
					जारी;
				}
			}

			अगर (parent->rb_बायाँ == गाँठ)
			{
				रेजिस्टर काष्ठा rb_गाँठ *tmp;
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

अटल व्योम __rb_erase_color(काष्ठा rb_गाँठ *गाँठ, काष्ठा rb_गाँठ *parent,
			     काष्ठा rb_मूल *मूल)
{
	काष्ठा rb_गाँठ *other;

	जबतक ((!गाँठ || ext2fs_rb_is_black(गाँठ)) && गाँठ != मूल->rb_गाँठ)
	{
		अगर (parent->rb_बायाँ == गाँठ)
		{
			other = parent->rb_दायाँ;
			अगर (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, मूल);
				other = parent->rb_दायाँ;
			}
			अगर ((!other->rb_बायाँ || ext2fs_rb_is_black(other->rb_बायाँ)) &&
			    (!other->rb_दायाँ || ext2fs_rb_is_black(other->rb_दायाँ)))
			{
				ext2fs_rb_set_red(other);
				गाँठ = parent;
				parent = ext2fs_rb_parent(गाँठ);
			}
			अन्यथा
			{
				अगर (!other->rb_दायाँ || ext2fs_rb_is_black(other->rb_दायाँ))
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
				अवरोध;
			}
		}
		अन्यथा
		{
			other = parent->rb_बायाँ;
			अगर (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, मूल);
				other = parent->rb_बायाँ;
			}
			अगर ((!other->rb_बायाँ || ext2fs_rb_is_black(other->rb_बायाँ)) &&
			    (!other->rb_दायाँ || ext2fs_rb_is_black(other->rb_दायाँ)))
			{
				ext2fs_rb_set_red(other);
				गाँठ = parent;
				parent = ext2fs_rb_parent(गाँठ);
			}
			अन्यथा
			{
				अगर (!other->rb_बायाँ || ext2fs_rb_is_black(other->rb_बायाँ))
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
				अवरोध;
			}
		}
	}
	अगर (गाँठ)
		ext2fs_rb_set_black(गाँठ);
}

व्योम ext2fs_rb_erase(काष्ठा rb_गाँठ *गाँठ, काष्ठा rb_मूल *मूल)
{
	काष्ठा rb_गाँठ *child, *parent;
	पूर्णांक color;

	अगर (!गाँठ->rb_बायाँ)
		child = गाँठ->rb_दायाँ;
	अन्यथा अगर (!गाँठ->rb_दायाँ)
		child = गाँठ->rb_बायाँ;
	अन्यथा
	{
		काष्ठा rb_गाँठ *old = गाँठ, *बायाँ;

		गाँठ = गाँठ->rb_दायाँ;
		जबतक ((बायाँ = गाँठ->rb_बायाँ) != शून्य)
			गाँठ = बायाँ;

		अगर (ext2fs_rb_parent(old)) {
			अगर (ext2fs_rb_parent(old)->rb_बायाँ == old)
				ext2fs_rb_parent(old)->rb_बायाँ = गाँठ;
			अन्यथा
				ext2fs_rb_parent(old)->rb_दायाँ = गाँठ;
		} अन्यथा
			मूल->rb_गाँठ = गाँठ;

		child = गाँठ->rb_दायाँ;
		parent = ext2fs_rb_parent(गाँठ);
		color = ext2fs_rb_color(गाँठ);

		अगर (parent == old) {
			parent = गाँठ;
		} अन्यथा {
			अगर (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_बायाँ = child;

			गाँठ->rb_दायाँ = old->rb_दायाँ;
			ext2fs_rb_set_parent(old->rb_दायाँ, गाँठ);
		}

		गाँठ->rb_parent_color = old->rb_parent_color;
		गाँठ->rb_बायाँ = old->rb_बायाँ;
		ext2fs_rb_set_parent(old->rb_बायाँ, गाँठ);

		जाओ color;
	}

	parent = ext2fs_rb_parent(गाँठ);
	color = ext2fs_rb_color(गाँठ);

	अगर (child)
		ext2fs_rb_set_parent(child, parent);
	अगर (parent)
	{
		अगर (parent->rb_बायाँ == गाँठ)
			parent->rb_बायाँ = child;
		अन्यथा
			parent->rb_दायाँ = child;
	}
	अन्यथा
		मूल->rb_गाँठ = child;

 color:
	अगर (color == RB_BLACK)
		__rb_erase_color(child, parent, मूल);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
काष्ठा rb_गाँठ *ext2fs_rb_first(स्थिर काष्ठा rb_मूल *मूल)
{
	काष्ठा rb_गाँठ	*n;

	n = मूल->rb_गाँठ;
	अगर (!n)
		वापस शून्य;
	जबतक (n->rb_बायाँ)
		n = n->rb_बायाँ;
	वापस n;
}

काष्ठा rb_गाँठ *ext2fs_rb_last(स्थिर काष्ठा rb_मूल *मूल)
{
	काष्ठा rb_गाँठ	*n;

	n = मूल->rb_गाँठ;
	अगर (!n)
		वापस शून्य;
	जबतक (n->rb_दायाँ)
		n = n->rb_दायाँ;
	वापस n;
}

काष्ठा rb_गाँठ *ext2fs_rb_अग्र(काष्ठा rb_गाँठ *गाँठ)
{
	काष्ठा rb_गाँठ *parent;

	अगर (ext2fs_rb_parent(गाँठ) == गाँठ)
		वापस शून्य;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	अगर (गाँठ->rb_दायाँ) {
		गाँठ = गाँठ->rb_दायाँ;
		जबतक (गाँठ->rb_बायाँ)
			गाँठ=गाँठ->rb_बायाँ;
		वापस (काष्ठा rb_गाँठ *)गाँठ;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	जबतक ((parent = ext2fs_rb_parent(गाँठ)) && गाँठ == parent->rb_दायाँ)
		गाँठ = parent;

	वापस parent;
}

काष्ठा rb_गाँठ *ext2fs_rb_पूर्व(काष्ठा rb_गाँठ *गाँठ)
{
	काष्ठा rb_गाँठ *parent;

	अगर (ext2fs_rb_parent(गाँठ) == गाँठ)
		वापस शून्य;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	अगर (गाँठ->rb_बायाँ) {
		गाँठ = गाँठ->rb_बायाँ;
		जबतक (गाँठ->rb_दायाँ)
			गाँठ=गाँठ->rb_दायाँ;
		वापस (काष्ठा rb_गाँठ *)गाँठ;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	जबतक ((parent = ext2fs_rb_parent(गाँठ)) && गाँठ == parent->rb_बायाँ)
		गाँठ = parent;

	वापस parent;
}

व्योम ext2fs_rb_replace_गाँठ(काष्ठा rb_गाँठ *victim, काष्ठा rb_गाँठ *new,
			  काष्ठा rb_मूल *मूल)
{
	काष्ठा rb_गाँठ *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	अगर (parent) {
		अगर (victim == parent->rb_बायाँ)
			parent->rb_बायाँ = new;
		अन्यथा
			parent->rb_दायाँ = new;
	} अन्यथा {
		मूल->rb_गाँठ = new;
	}
	अगर (victim->rb_बायाँ)
		ext2fs_rb_set_parent(victim->rb_बायाँ, new);
	अगर (victim->rb_दायाँ)
		ext2fs_rb_set_parent(victim->rb_दायाँ, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
