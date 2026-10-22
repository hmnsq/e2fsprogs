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

स्थिर शून्य __rb_rotate_left(संरचना rb_ग्रन्थिः *ग्रन्थिः, संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *दक्षिणः = ग्रन्थिः->rb_दक्षिणः;
	संरचना rb_ग्रन्थिः *parent = ext2fs_rb_parent(ग्रन्थिः);

	जइ ((ग्रन्थिः->rb_दक्षिणः = दक्षिणः->rb_वामः))
		ext2fs_rb_set_parent(दक्षिणः->rb_वामः, ग्रन्थिः);
	दक्षिणः->rb_वामः = ग्रन्थिः;

	ext2fs_rb_set_parent(दक्षिणः, parent);

	जइ (parent)
	{
		जइ (ग्रन्थिः == parent->rb_वामः)
			parent->rb_वामः = दक्षिणः;
		अण्णहा
			parent->rb_दक्षिणः = दक्षिणः;
	}
	अण्णहा
		मूलम्->rb_ग्रन्थिः = दक्षिणः;
	ext2fs_rb_set_parent(ग्रन्थिः, दक्षिणः);
}

स्थिर शून्य __rb_rotate_right(संरचना rb_ग्रन्थिः *ग्रन्थिः, संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *वामः = ग्रन्थिः->rb_वामः;
	संरचना rb_ग्रन्थिः *parent = ext2fs_rb_parent(ग्रन्थिः);

	जइ ((ग्रन्थिः->rb_वामः = वामः->rb_दक्षिणः))
		ext2fs_rb_set_parent(वामः->rb_दक्षिणः, ग्रन्थिः);
	वामः->rb_दक्षिणः = ग्रन्थिः;

	ext2fs_rb_set_parent(वामः, parent);

	जइ (parent)
	{
		जइ (ग्रन्थिः == parent->rb_दक्षिणः)
			parent->rb_दक्षिणः = वामः;
		अण्णहा
			parent->rb_वामः = वामः;
	}
	अण्णहा
		मूलम्->rb_ग्रन्थिः = वामः;
	ext2fs_rb_set_parent(ग्रन्थिः, वामः);
}

शून्य ext2fs_rb_निवेशय_color(संरचना rb_ग्रन्थिः *ग्रन्थिः, संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *parent, *gparent;

	जाव ((parent = ext2fs_rb_parent(ग्रन्थिः)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		जइ (parent == gparent->rb_वामः)
		{
			{
				पञ्जिका संरचना rb_ग्रन्थिः *uncle = gparent->rb_दक्षिणः;
				जइ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ग्रन्थिः = gparent;
					अनुवर्त;
				}
			}

			जइ (parent->rb_दक्षिणः == ग्रन्थिः)
			{
				पञ्जिका संरचना rb_ग्रन्थिः *tmp;
				__rb_rotate_left(parent, मूलम्);
				tmp = parent;
				parent = ग्रन्थिः;
				ग्रन्थिः = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, मूलम्);
		} अण्णहा {
			{
				पञ्जिका संरचना rb_ग्रन्थिः *uncle = gparent->rb_वामः;
				जइ (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ग्रन्थिः = gparent;
					अनुवर्त;
				}
			}

			जइ (parent->rb_वामः == ग्रन्थिः)
			{
				पञ्जिका संरचना rb_ग्रन्थिः *tmp;
				__rb_rotate_right(parent, मूलम्);
				tmp = parent;
				parent = ग्रन्थिः;
				ग्रन्थिः = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, मूलम्);
		}
	}

	ext2fs_rb_set_black(मूलम्->rb_ग्रन्थिः);
}

स्थिर शून्य __rb_erase_color(संरचना rb_ग्रन्थिः *ग्रन्थिः, संरचना rb_ग्रन्थिः *parent,
			     संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *other;

	जाव ((!ग्रन्थिः || ext2fs_rb_is_black(ग्रन्थिः)) && ग्रन्थिः != मूलम्->rb_ग्रन्थिः)
	{
		जइ (parent->rb_वामः == ग्रन्थिः)
		{
			other = parent->rb_दक्षिणः;
			जइ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, मूलम्);
				other = parent->rb_दक्षिणः;
			}
			जइ ((!other->rb_वामः || ext2fs_rb_is_black(other->rb_वामः)) &&
			    (!other->rb_दक्षिणः || ext2fs_rb_is_black(other->rb_दक्षिणः)))
			{
				ext2fs_rb_set_red(other);
				ग्रन्थिः = parent;
				parent = ext2fs_rb_parent(ग्रन्थिः);
			}
			अण्णहा
			{
				जइ (!other->rb_दक्षिणः || ext2fs_rb_is_black(other->rb_दक्षिणः))
				{
					ext2fs_rb_set_black(other->rb_वामः);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, मूलम्);
					other = parent->rb_दक्षिणः;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_दक्षिणः);
				__rb_rotate_left(parent, मूलम्);
				ग्रन्थिः = मूलम्->rb_ग्रन्थिः;
				भङ्ग;
			}
		}
		अण्णहा
		{
			other = parent->rb_वामः;
			जइ (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, मूलम्);
				other = parent->rb_वामः;
			}
			जइ ((!other->rb_वामः || ext2fs_rb_is_black(other->rb_वामः)) &&
			    (!other->rb_दक्षिणः || ext2fs_rb_is_black(other->rb_दक्षिणः)))
			{
				ext2fs_rb_set_red(other);
				ग्रन्थिः = parent;
				parent = ext2fs_rb_parent(ग्रन्थिः);
			}
			अण्णहा
			{
				जइ (!other->rb_वामः || ext2fs_rb_is_black(other->rb_वामः))
				{
					ext2fs_rb_set_black(other->rb_दक्षिणः);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, मूलम्);
					other = parent->rb_वामः;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_वामः);
				__rb_rotate_right(parent, मूलम्);
				ग्रन्थिः = मूलम्->rb_ग्रन्थिः;
				भङ्ग;
			}
		}
	}
	जइ (ग्रन्थिः)
		ext2fs_rb_set_black(ग्रन्थिः);
}

शून्य ext2fs_rb_erase(संरचना rb_ग्रन्थिः *ग्रन्थिः, संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *child, *parent;
	पूर्णाङ्क color;

	जइ (!ग्रन्थिः->rb_वामः)
		child = ग्रन्थिः->rb_दक्षिणः;
	अण्णहा जइ (!ग्रन्थिः->rb_दक्षिणः)
		child = ग्रन्थिः->rb_वामः;
	अण्णहा
	{
		संरचना rb_ग्रन्थिः *old = ग्रन्थिः, *वामः;

		ग्रन्थिः = ग्रन्थिः->rb_दक्षिणः;
		जाव ((वामः = ग्रन्थिः->rb_वामः) != रिक्त)
			ग्रन्थिः = वामः;

		जइ (ext2fs_rb_parent(old)) {
			जइ (ext2fs_rb_parent(old)->rb_वामः == old)
				ext2fs_rb_parent(old)->rb_वामः = ग्रन्थिः;
			अण्णहा
				ext2fs_rb_parent(old)->rb_दक्षिणः = ग्रन्थिः;
		} अण्णहा
			मूलम्->rb_ग्रन्थिः = ग्रन्थिः;

		child = ग्रन्थिः->rb_दक्षिणः;
		parent = ext2fs_rb_parent(ग्रन्थिः);
		color = ext2fs_rb_color(ग्रन्थिः);

		जइ (parent == old) {
			parent = ग्रन्थिः;
		} अण्णहा {
			जइ (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_वामः = child;

			ग्रन्थिः->rb_दक्षिणः = old->rb_दक्षिणः;
			ext2fs_rb_set_parent(old->rb_दक्षिणः, ग्रन्थिः);
		}

		ग्रन्थिः->rb_parent_color = old->rb_parent_color;
		ग्रन्थिः->rb_वामः = old->rb_वामः;
		ext2fs_rb_set_parent(old->rb_वामः, ग्रन्थिः);

		गच्छ color;
	}

	parent = ext2fs_rb_parent(ग्रन्थिः);
	color = ext2fs_rb_color(ग्रन्थिः);

	जइ (child)
		ext2fs_rb_set_parent(child, parent);
	जइ (parent)
	{
		जइ (parent->rb_वामः == ग्रन्थिः)
			parent->rb_वामः = child;
		अण्णहा
			parent->rb_दक्षिणः = child;
	}
	अण्णहा
		मूलम्->rb_ग्रन्थिः = child;

 color:
	जइ (color == RB_BLACK)
		__rb_erase_color(child, parent, मूलम्);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
संरचना rb_ग्रन्थिः *ext2fs_rb_first(ध्रुव संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः	*n;

	n = मूलम्->rb_ग्रन्थिः;
	जइ (!n)
		प्रत्यागम रिक्त;
	जाव (n->rb_वामः)
		n = n->rb_वामः;
	प्रत्यागम n;
}

संरचना rb_ग्रन्थिः *ext2fs_rb_last(ध्रुव संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः	*n;

	n = मूलम्->rb_ग्रन्थिः;
	जइ (!n)
		प्रत्यागम रिक्त;
	जाव (n->rb_दक्षिणः)
		n = n->rb_दक्षिणः;
	प्रत्यागम n;
}

संरचना rb_ग्रन्थिः *ext2fs_rb_next(संरचना rb_ग्रन्थिः *ग्रन्थिः)
{
	संरचना rb_ग्रन्थिः *parent;

	जइ (ext2fs_rb_parent(ग्रन्थिः) == ग्रन्थिः)
		प्रत्यागम रिक्त;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	जइ (ग्रन्थिः->rb_दक्षिणः) {
		ग्रन्थिः = ग्रन्थिः->rb_दक्षिणः;
		जाव (ग्रन्थिः->rb_वामः)
			ग्रन्थिः=ग्रन्थिः->rb_वामः;
		प्रत्यागम (संरचना rb_ग्रन्थिः *)ग्रन्थिः;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	जाव ((parent = ext2fs_rb_parent(ग्रन्थिः)) && ग्रन्थिः == parent->rb_दक्षिणः)
		ग्रन्थिः = parent;

	प्रत्यागम parent;
}

संरचना rb_ग्रन्थिः *ext2fs_rb_prev(संरचना rb_ग्रन्थिः *ग्रन्थिः)
{
	संरचना rb_ग्रन्थिः *parent;

	जइ (ext2fs_rb_parent(ग्रन्थिः) == ग्रन्थिः)
		प्रत्यागम रिक्त;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	जइ (ग्रन्थिः->rb_वामः) {
		ग्रन्थिः = ग्रन्थिः->rb_वामः;
		जाव (ग्रन्थिः->rb_दक्षिणः)
			ग्रन्थिः=ग्रन्थिः->rb_दक्षिणः;
		प्रत्यागम (संरचना rb_ग्रन्थिः *)ग्रन्थिः;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	जाव ((parent = ext2fs_rb_parent(ग्रन्थिः)) && ग्रन्थिः == parent->rb_वामः)
		ग्रन्थिः = parent;

	प्रत्यागम parent;
}

शून्य ext2fs_rb_replace_ग्रन्थिः(संरचना rb_ग्रन्थिः *victim, संरचना rb_ग्रन्थिः *new,
			  संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	जइ (parent) {
		जइ (victim == parent->rb_वामः)
			parent->rb_वामः = new;
		अण्णहा
			parent->rb_दक्षिणः = new;
	} अण्णहा {
		मूलम्->rb_ग्रन्थिः = new;
	}
	जइ (victim->rb_वामः)
		ext2fs_rb_set_parent(victim->rb_वामः, new);
	जइ (victim->rb_दक्षिणः)
		ext2fs_rb_set_parent(victim->rb_दक्षिणः, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
