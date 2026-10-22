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

	सचे ((ग्रन्थिः->rb_दक्षिणः = दक्षिणः->rb_वामः))
		ext2fs_rb_set_parent(दक्षिणः->rb_वामः, ग्रन्थिः);
	दक्षिणः->rb_वामः = ग्रन्थिः;

	ext2fs_rb_set_parent(दक्षिणः, parent);

	सचे (parent)
	{
		सचे (ग्रन्थिः == parent->rb_वामः)
			parent->rb_वामः = दक्षिणः;
		अञ्ञथा
			parent->rb_दक्षिणः = दक्षिणः;
	}
	अञ्ञथा
		मूलम्->rb_ग्रन्थिः = दक्षिणः;
	ext2fs_rb_set_parent(ग्रन्थिः, दक्षिणः);
}

स्थिर शून्य __rb_rotate_right(संरचना rb_ग्रन्थिः *ग्रन्थिः, संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *वामः = ग्रन्थिः->rb_वामः;
	संरचना rb_ग्रन्थिः *parent = ext2fs_rb_parent(ग्रन्थिः);

	सचे ((ग्रन्थिः->rb_वामः = वामः->rb_दक्षिणः))
		ext2fs_rb_set_parent(वामः->rb_दक्षिणः, ग्रन्थिः);
	वामः->rb_दक्षिणः = ग्रन्थिः;

	ext2fs_rb_set_parent(वामः, parent);

	सचे (parent)
	{
		सचे (ग्रन्थिः == parent->rb_दक्षिणः)
			parent->rb_दक्षिणः = वामः;
		अञ्ञथा
			parent->rb_वामः = वामः;
	}
	अञ्ञथा
		मूलम्->rb_ग्रन्थिः = वामः;
	ext2fs_rb_set_parent(ग्रन्थिः, वामः);
}

शून्य ext2fs_rb_निवेशय_color(संरचना rb_ग्रन्थिः *ग्रन्थिः, संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *parent, *gparent;

	याव ((parent = ext2fs_rb_parent(ग्रन्थिः)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		सचे (parent == gparent->rb_वामः)
		{
			{
				पञ्जिका संरचना rb_ग्रन्थिः *uncle = gparent->rb_दक्षिणः;
				सचे (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ग्रन्थिः = gparent;
					अनुवर्त;
				}
			}

			सचे (parent->rb_दक्षिणः == ग्रन्थिः)
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
		} अञ्ञथा {
			{
				पञ्जिका संरचना rb_ग्रन्थिः *uncle = gparent->rb_वामः;
				सचे (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					ग्रन्थिः = gparent;
					अनुवर्त;
				}
			}

			सचे (parent->rb_वामः == ग्रन्थिः)
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

	याव ((!ग्रन्थिः || ext2fs_rb_is_black(ग्रन्थिः)) && ग्रन्थिः != मूलम्->rb_ग्रन्थिः)
	{
		सचे (parent->rb_वामः == ग्रन्थिः)
		{
			other = parent->rb_दक्षिणः;
			सचे (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, मूलम्);
				other = parent->rb_दक्षिणः;
			}
			सचे ((!other->rb_वामः || ext2fs_rb_is_black(other->rb_वामः)) &&
			    (!other->rb_दक्षिणः || ext2fs_rb_is_black(other->rb_दक्षिणः)))
			{
				ext2fs_rb_set_red(other);
				ग्रन्थिः = parent;
				parent = ext2fs_rb_parent(ग्रन्थिः);
			}
			अञ्ञथा
			{
				सचे (!other->rb_दक्षिणः || ext2fs_rb_is_black(other->rb_दक्षिणः))
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
		अञ्ञथा
		{
			other = parent->rb_वामः;
			सचे (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, मूलम्);
				other = parent->rb_वामः;
			}
			सचे ((!other->rb_वामः || ext2fs_rb_is_black(other->rb_वामः)) &&
			    (!other->rb_दक्षिणः || ext2fs_rb_is_black(other->rb_दक्षिणः)))
			{
				ext2fs_rb_set_red(other);
				ग्रन्थिः = parent;
				parent = ext2fs_rb_parent(ग्रन्थिः);
			}
			अञ्ञथा
			{
				सचे (!other->rb_वामः || ext2fs_rb_is_black(other->rb_वामः))
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
	सचे (ग्रन्थिः)
		ext2fs_rb_set_black(ग्रन्थिः);
}

शून्य ext2fs_rb_erase(संरचना rb_ग्रन्थिः *ग्रन्थिः, संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *child, *parent;
	पूर्णाङ्क color;

	सचे (!ग्रन्थिः->rb_वामः)
		child = ग्रन्थिः->rb_दक्षिणः;
	अञ्ञथा सचे (!ग्रन्थिः->rb_दक्षिणः)
		child = ग्रन्थिः->rb_वामः;
	अञ्ञथा
	{
		संरचना rb_ग्रन्थिः *old = ग्रन्थिः, *वामः;

		ग्रन्थिः = ग्रन्थिः->rb_दक्षिणः;
		याव ((वामः = ग्रन्थिः->rb_वामः) != रिक्त)
			ग्रन्थिः = वामः;

		सचे (ext2fs_rb_parent(old)) {
			सचे (ext2fs_rb_parent(old)->rb_वामः == old)
				ext2fs_rb_parent(old)->rb_वामः = ग्रन्थिः;
			अञ्ञथा
				ext2fs_rb_parent(old)->rb_दक्षिणः = ग्रन्थिः;
		} अञ्ञथा
			मूलम्->rb_ग्रन्थिः = ग्रन्थिः;

		child = ग्रन्थिः->rb_दक्षिणः;
		parent = ext2fs_rb_parent(ग्रन्थिः);
		color = ext2fs_rb_color(ग्रन्थिः);

		सचे (parent == old) {
			parent = ग्रन्थिः;
		} अञ्ञथा {
			सचे (child)
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

	सचे (child)
		ext2fs_rb_set_parent(child, parent);
	सचे (parent)
	{
		सचे (parent->rb_वामः == ग्रन्थिः)
			parent->rb_वामः = child;
		अञ्ञथा
			parent->rb_दक्षिणः = child;
	}
	अञ्ञथा
		मूलम्->rb_ग्रन्थिः = child;

 color:
	सचे (color == RB_BLACK)
		__rb_erase_color(child, parent, मूलम्);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
संरचना rb_ग्रन्थिः *ext2fs_rb_first(ध्रुव संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः	*n;

	n = मूलम्->rb_ग्रन्थिः;
	सचे (!n)
		पटिनिवत्त रिक्त;
	याव (n->rb_वामः)
		n = n->rb_वामः;
	पटिनिवत्त n;
}

संरचना rb_ग्रन्थिः *ext2fs_rb_last(ध्रुव संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः	*n;

	n = मूलम्->rb_ग्रन्थिः;
	सचे (!n)
		पटिनिवत्त रिक्त;
	याव (n->rb_दक्षिणः)
		n = n->rb_दक्षिणः;
	पटिनिवत्त n;
}

संरचना rb_ग्रन्थिः *ext2fs_rb_next(संरचना rb_ग्रन्थिः *ग्रन्थिः)
{
	संरचना rb_ग्रन्थिः *parent;

	सचे (ext2fs_rb_parent(ग्रन्थिः) == ग्रन्थिः)
		पटिनिवत्त रिक्त;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	सचे (ग्रन्थिः->rb_दक्षिणः) {
		ग्रन्थिः = ग्रन्थिः->rb_दक्षिणः;
		याव (ग्रन्थिः->rb_वामः)
			ग्रन्थिः=ग्रन्थिः->rb_वामः;
		पटिनिवत्त (संरचना rb_ग्रन्थिः *)ग्रन्थिः;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	याव ((parent = ext2fs_rb_parent(ग्रन्थिः)) && ग्रन्थिः == parent->rb_दक्षिणः)
		ग्रन्थिः = parent;

	पटिनिवत्त parent;
}

संरचना rb_ग्रन्थिः *ext2fs_rb_prev(संरचना rb_ग्रन्थिः *ग्रन्थिः)
{
	संरचना rb_ग्रन्थिः *parent;

	सचे (ext2fs_rb_parent(ग्रन्थिः) == ग्रन्थिः)
		पटिनिवत्त रिक्त;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	सचे (ग्रन्थिः->rb_वामः) {
		ग्रन्थिः = ग्रन्थिः->rb_वामः;
		याव (ग्रन्थिः->rb_दक्षिणः)
			ग्रन्थिः=ग्रन्थिः->rb_दक्षिणः;
		पटिनिवत्त (संरचना rb_ग्रन्थिः *)ग्रन्थिः;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	याव ((parent = ext2fs_rb_parent(ग्रन्थिः)) && ग्रन्थिः == parent->rb_वामः)
		ग्रन्थिः = parent;

	पटिनिवत्त parent;
}

शून्य ext2fs_rb_replace_ग्रन्थिः(संरचना rb_ग्रन्थिः *victim, संरचना rb_ग्रन्थिः *new,
			  संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	सचे (parent) {
		सचे (victim == parent->rb_वामः)
			parent->rb_वामः = new;
		अञ्ञथा
			parent->rb_दक्षिणः = new;
	} अञ्ञथा {
		मूलम्->rb_ग्रन्थिः = new;
	}
	सचे (victim->rb_वामः)
		ext2fs_rb_set_parent(victim->rb_वामः, new);
	सचे (victim->rb_दक्षिणः)
		ext2fs_rb_set_parent(victim->rb_दक्षिणः, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
